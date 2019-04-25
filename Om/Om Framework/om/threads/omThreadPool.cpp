/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com/om
 * Author(s):   Carl Schissler
 * 
 * Copyright (c) 2016, Carl Schissler
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright
 * 	   notice, this list of conditions and the following disclaimer.
 * 	2. Redistributions in binary form must reproduce the above copyright
 * 	   notice, this list of conditions and the following disclaimer in the
 * 	   documentation and/or other materials provided with the distribution.
 * 	3. Neither the name of the copyright holder nor the
 * 	   names of its contributors may be used to endorse or promote products
 * 	   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "omThreadPool.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################

//##########################################################################################
//##########################################################################################
//############		
//############		Worker Thread Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class ThreadPool:: Worker : public ThreadBase
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new worker thread that is a member of the specified thread pool.
			OM_INLINE Worker( ThreadPool* newPool, Index newThreadIndex )
				:	pool( newPool ),
					threadIndex( newThreadIndex ),
					shouldStop( 0 )
			{
			}
			
			
			
		//********************************************************************************
		//******	Thread Control Methods
			
			
			/// Indicate to the worker thread that it should stop processing jobs after it finishes the current job.
			OM_INLINE void requestStop()
			{
				shouldStop++;
			}
			
			
			/// Wait for this thread to stop.
			OM_INLINE void join()
			{
				ThreadBase::joinThread();
			}
			
			
			/// Start this thread if it is not already running.
			OM_INLINE Bool start()
			{
				return ThreadBase::startThread();
			}
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Virtual Run Function
			
			
			/// The method called by ThreadBase as the entry point for a new worker thread.
			virtual void run()
			{
				if ( pool == NULL )
					return;
				
				while ( !shouldStop )
				{
					// Wait for a new job.
					if ( pool->allJobs.numUnfinishedJobs == 0 )
					{
						pool->numWaitingWorkers++;
						pool->workerSemaphore.down();
						pool->numWaitingWorkers--;
					}
					
					if ( shouldStop )
						break;
					
					// Wait until the jobs are not accessed by another thread.
					pool->lockJobs();
					
					// Are there any jobs left?
					if ( pool->jobs.getSize() > 0 )
					{
						// Get the next job from the queue.
						JobBase* const job = getNextJob();
						
						// Unlock access to the job queue.
						pool->unlockJobs();
						
						if ( job )
						{
							JobID* jobID = job->jobID;
							
							// Execute the job.
							job->execute();
							
							// Destroy the job.
							util::destruct( job );
							
							// Atomically update the number of unfinished jobs and signal waiting threads if necessary.
							jobID->signal();
							pool->allJobs.signal();
						}
					}
					else
						pool->unlockJobs();
				}
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Return a pointer to the next job that should be executed by this worker.
			OM_FORCE_INLINE JobBase* getNextJob() const
			{
				util::PriorityQueue<SortableJob>& jobs = pool->jobs;
				const Size numJobs = jobs.getSize();
				
				// Get the first job from the queue.
				JobBase* job = jobs.getFirst().job;
				
				// If the thread index is unspecified, just use the first job.
				if ( job->threadIndex == Index(-1) || job->threadIndex == threadIndex || numJobs == Size(1) )
				{
					// Remove the job from the queue.
					jobs.remove();
					return job;
				}
				else
				{
					// Find the next job that meets the thread index requirement.
					for ( Index i = 1; i < numJobs; i++ )
					{
						job = jobs[i].job;
						
						if ( job->threadIndex == Index(-1) || job->threadIndex == threadIndex )
						{
							jobs.removeAtIndex( i );
							return job;
						}
					}
					
					return NULL;
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the thread pool that has this worker thread.
			ThreadPool* pool;
			
			
			/// The index of this worker thread within the thread pool.
			Index threadIndex;
			
			
			/// A boolean value indicating whether or not this worker thread should stop processing.
			Atomic<Size> shouldStop;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ThreadPool:: ThreadPool()
	:	workerSemaphore( 0 ),
		jobIndex( 0 )
{
}




ThreadPool:: ThreadPool( Size newNumberOfThreads )
	:	workerSemaphore( 0 ),
		jobIndex( 0 )
{
	this->setThreadCount( newNumberOfThreads );
}




ThreadPool:: ThreadPool( const ThreadPool& other )
	:	workerSemaphore( 0 ),
		allJobs( other.allJobs ),
		jobIndex( other.jobIndex )
{
	Size newNumberOfThreads = other.getThreadCount();
	this->setThreadCount( newNumberOfThreads );
	
	// Lock access to the other pool's job queue.
	other.lockJobs();
	
	// Copy the jobIDs.
	const Size numJobIDs = other.jobIDs.getSize();
	for ( Index i = 0; i < numJobIDs; i++ )
		jobIDs.add( util::construct<JobID>( *other.jobIDs[i] ) );
	
	// Copy the jobs in the other job queue.
	const Size numJobs = other.jobs.getSize();
	
	for ( Index i = 0; i < numJobs; i++ )
		jobs.add( other.jobs[i].job->copy() );
	
	other.unlockJobs();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ThreadPool:: ~ThreadPool()
{
	lockThreads();
	
	// Signal all of the threads to stop processing.
	for ( Index i = 0; i < threads.getSize(); i++ )
		threads[i]->requestStop();
	
	// Awaken all worker threads.
	for ( Index i = 0; i < threads.getSize(); i++ )
		workerSemaphore.up();
	
	// Wait for each thread to finish.
	for ( Index i = 0; i < threads.getSize(); i++ )
	{
		threads[i]->join();
		util::destruct( threads[i] );
	}
	
	// Clear the threads.
	threads.clear();
	
	lockJobs();
	deallocateJobs();
	unlockJobs();
	unlockThreads();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ThreadPool& ThreadPool:: operator = ( const ThreadPool& other )
{
	if ( this != &other )
	{
		lockThreads();
		
		Size newNumberOfThreads = other.getThreadCount();
		
		// Make sure this pool has the same number of threads as the other thread pool.
		for ( Index i = threads.getSize(); i < newNumberOfThreads; i++ )
		{
			Worker* newThread = util::construct<Worker>( this, i );
			threads.add( newThread );
			
			// Start the new worker thread.
			newThread->start();
		}
		
		unlockThreads();
		
		//*************************************************
		
		// Lock access to the other pool's job queue.
		other.lockJobs();
		lockJobs();
		
		// Deallocate the previous jobs.
		deallocateJobs();
		
		// Copy the jobIDs.
		const Size numJobIDs = other.jobIDs.getSize();
		for ( Index i = 0; i < numJobIDs; i++ )
			jobIDs.add( util::construct<JobID>( *other.jobIDs[i] ) );
		
		// Copy the jobs in the other job queue.
		const Size numJobs = other.jobs.getSize();
		
		for ( Index i = 0; i < numJobs; i++ )
			jobs.add( other.jobs[i].job->copy() );
		
		allJobs = other.allJobs;
		jobIndex = other.jobIndex;
		
		unlockJobs();
		other.unlockJobs();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size ThreadPool:: getThreadCount() const
{
	lockThreads();
	
	Size numThreads = threads.getSize();
	
	unlockThreads();
	
	return numThreads;
}




void ThreadPool:: setThreadCount( Size numThreads )
{
	lockThreads();
	
	const Size oldNumThreads = threads.getSize();

	if ( oldNumThreads == numThreads )
		return;
	else if ( oldNumThreads > numThreads )
	{
		// Remove threads from the pool.

		// Signal the threads to stop.
		for ( Index i = numThreads; i < oldNumThreads; i++ )
			threads[i]->requestStop();
		
		// Awaken all worker threads.
		for ( Index i = 0; i < oldNumThreads; i++ )
			workerSemaphore.up();
		
		// Wait for the threads to finish.
		for ( Index i = numThreads; i < oldNumThreads; i++ )
		{
			threads[i]->join();
			util::destruct( threads[i] );
		}

		// Remove the finished threads.
		threads.removeLast( oldNumThreads - numThreads );
	}
	else
	{
		// Add new threads to the pool.
		for ( Index i = oldNumThreads; i < numThreads; i++ )
		{
			// Create the new thread and add it to the list of threads.
			Worker* newThread = util::construct<Worker>( this, i );
			threads.add( newThread );
			
			// Start the new worker thread.
			newThread->start();
			
			// Set the thread's priority.
			newThread->setPriority( priority );
		}
	}
	
	unlockThreads();
}




void ThreadPool:: addThread()
{
	lockThreads();
	
	// Create the new thread and add it to the list of threads.
	Worker* newThread = util::construct<Worker>( this, threads.getSize() );
	threads.add( newThread );
	
	// Start the new worker thread.
	newThread->start();
	
	// Set the thread's priority.
	newThread->setPriority( priority );
	
	unlockThreads();
}




void ThreadPool:: removeThread()
{
	lockThreads();
	
	if ( threads.getSize() > 0 )
	{
		// Choose the last thread from the queue.
		Index removeIndex = threads.getSize() - 1;
		
		// Tell the thread to stop processing.
		threads[removeIndex]->requestStop();
		
		// Awaken all worker threads.
		for ( Index i = 0; i < threads.getSize(); i++ )
			workerSemaphore.up();
		
		// Wait for the thread to finish.
		threads[removeIndex]->join();
		
		// Remove the thread.
		util::destruct( threads[removeIndex] );
		threads.removeAtIndexUnordered( removeIndex );
	}
	
	unlockThreads();
}




Index ThreadPool:: getCurrentThreadIndex() const
{
	ThreadID currentThreadID = ThreadBase::getCurrentID();
	
	lockThreads();
	
	const Size numThreads = threads.getSize();
	
	for ( Index i = 0; i < numThreads; i++ )
	{
		if ( threads[i]->getID() == currentThreadID )
		{
			unlockThreads();
			return i;
		}
	}
	
	unlockThreads();
	
	return Index(-1);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Job Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size ThreadPool:: getJobCount() const
{
	return allJobs.numUnfinishedJobs;
}




void ThreadPool:: finishJob( Index newJobID )
{
	// Determine the index of this job ID.
	lockJobs();
	JobID* jobID = findJobID( newJobID );
	unlockJobs();
	
	// If there was no job with that ID, return immediately.
	if ( jobID == NULL )
		return;
	
	// Wait for the job to finish if it has not already.
	jobID->finish();
}




void ThreadPool:: finishJobs()
{
	// Wait for all jobs to finish.
	allJobs.finish();
}




void ThreadPool:: clearJobs()
{
	lockJobs();
	deallocateJobs();
	unlockJobs();
}




//##########################################################################################
//##########################################################################################
//############		
//############		New Job Add Method
//############		
//##########################################################################################
//##########################################################################################




void ThreadPool:: addNewJob( JobBase* newJob, Index newJobID )
{
	lockJobs();
	
	// Find the index of this job ID in the list of of job IDs.
	JobID* jobID = createJobID( newJobID );
	newJob->jobID = jobID;
	newJob->jobIndex = jobIndex;
	
	// Add the new job to the job queue.
	jobs.add( SortableJob( newJob ) );
	
	// Atomically update the number of unfinished jobs.
	jobID->numUnfinishedJobs++;
	allJobs.numUnfinishedJobs++;
	jobIndex++;
	
	unlockJobs();
	
	// Notify a worker that there is a job ready.
	if ( numWaitingWorkers > 0 )
		workerSemaphore.up();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Job ID Find Method
//############		
//##########################################################################################
//##########################################################################################




ThreadPool::JobID* ThreadPool:: createJobID( Index jobID )
{
	const Size numJobIDs = jobIDs.getSize();
	Index firstUnusedIndex = math::max<Index>();
	
	// Does this job ID already exist?
	for ( Index i = 0; i < numJobIDs; i++ )
	{
		JobID* job = jobIDs[i];
		
		if ( job->jobID == jobID )
			return job;
		
		if ( job->numUnfinishedJobs == 0 && firstUnusedIndex == math::max<Index>() )
			firstUnusedIndex = i;
	}
	
	// If there is a free spot in the list, use it.
	if ( firstUnusedIndex != math::max<Index>() )
	{
		jobIDs[firstUnusedIndex]->jobID = jobID;
		
		return jobIDs[firstUnusedIndex];
	}
	
	// Otherwise, add a new job ID to the list.
	JobID* newJobID = util::construct<JobID>( jobID );
	jobIDs.add( newJobID );
	
	return newJobID;
}




ThreadPool::JobID* ThreadPool:: findJobID( Index jobID )
{
	const Size numJobIDs = jobIDs.getSize();
	
	// Does this job ID already exist?
	for ( Index i = 0; i < numJobIDs; i++ )
	{
		JobID* job = jobIDs[i];
		
		if ( job->jobID == jobID )
			return job;
	}
	
	return NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Priority Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool ThreadPool:: setPriority( const ThreadPriority& newPriority )
{
	priority = newPriority;
	
	lockThreads();
	
	const Size numThreads = threads.getSize();
	
	// Set the priority for all of the threads.
	for ( Index i = 0; i < numThreads; i++ )
		threads[i]->setPriority( newPriority );
	
	unlockThreads();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Job Deallocation Method
//############		
//##########################################################################################
//##########################################################################################




void ThreadPool:: deallocateJobs()
{
	// Deallocate the jobs.
	const Size numJobs = jobs.getSize();
	
	for ( Index i = 0; i < numJobs; i++ )
		util::destruct( jobs[i].job );
	
	jobs.clear();
	
	//**************************************************************************
	
	// Deallocate the job IDs, signaling any waiting threads.
	const Size numJobIDs = jobIDs.getSize();
	
	for ( Index i = 0; i < numJobIDs; i++ )
	{
		JobID* jobID = jobIDs[i];
		jobID->signal();
		util::destruct( jobID );
	}
	
	jobIDs.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Job Synchronization Methods
//############		
//##########################################################################################
//##########################################################################################




void ThreadPool:: lockJobs() const
{
#if defined(OM_PLATFORM_WINDOWS)
	jobMutex.lock();
#else
	while ( jobsAccessed++ )
	{
		jobsAccessed--;
		ThreadBase::yield();
	}
#endif
}




void ThreadPool:: unlockJobs() const
{
#if defined(OM_PLATFORM_WINDOWS)
	jobMutex.unlock();
#else
	jobsAccessed--;
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Synchronization Methods
//############		
//##########################################################################################
//##########################################################################################




void ThreadPool:: lockThreads() const
{
#if defined(OM_PLATFORM_WINDOWS)
	threadMutex.lock();
#else
	while ( threadsAccessed++ )
	{
		threadsAccessed--;
		ThreadBase::yield();
	}
#endif
}




void ThreadPool:: unlockThreads() const
{
#if defined(OM_PLATFORM_WINDOWS)
	threadMutex.unlock();
#else
	threadsAccessed--;
#endif
}




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
