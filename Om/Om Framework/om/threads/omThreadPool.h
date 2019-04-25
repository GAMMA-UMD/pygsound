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

#ifndef INCLUDE_OM_THREAD_POOL_H
#define INCLUDE_OM_THREAD_POOL_H


#include "omThreadsConfig.h"


#include "omThreadBase.h"
#include "omSemaphore.h"
#include "omMutex.h"
#include "omSignal.h"
#include "omAtomics.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that executes jobs on a set of worker threads.
class ThreadPool
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a thread pool which has no threads and no jobs queued.
			ThreadPool();
			
			
			/// Create a new thread pool which has the specified number of execution threads.
			ThreadPool( Size newNumberOfThreads );
			
			
			/// Create a thread pool with the same queued jobs and number of threads as another thread pool.
			private: ThreadPool( const ThreadPool& other );
			public:
			
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Finish the current job being processed by each worker thread then destroy the thread pool.
			~ThreadPool();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the queued jobs of another thread pool and copy the number of threads to this thread pool.
			private: ThreadPool& operator = ( const ThreadPool& other );
			public:
			
			
			
		//********************************************************************************
		//******	Thread Management Methods
			
			
			/// Return the number of threads that are currently executing as part of this thread pool.
			Size getThreadCount() const;
			
			
			/// Set the desired number of threads that should be in this thread pool.
			void setThreadCount( Size numThreads );
			
			
			/// Add a new worker thread to this thread pool.
			void addThread();
			
			
			/// Wait for a thread to finish processing and remove it from the queue.
			void removeThread();
			
			
			/// Return the index of the thread in this thread pool corresponding to the calling thread, or -1 if no match found.
			Index getCurrentThreadIndex() const;
			
			
		//********************************************************************************
		//******	Job Management Methods
			
			
			/// Return the total number of jobs that are queued to be executed by this thread pool.
			Size getJobCount() const;
			
			
			/// Add the specified job function call to this thread pool, executing with the given priority.
			/**
			  * The method allows the user to specify an integer ID for the job, that is
			  * used to differentiate between different job sets that have different deadlines
			  * on separate threads. The user can then wait for all jobs or just jobs with that ID to finish.
			  */
			template < typename Signature >
			OM_INLINE void addJob( const lang::FunctionCall<Signature>& job, Index jobID = 0,
									Float priority = Float(0), Index threadIndex = Index(-1) )
			{
				// Construct a new job.
				JobBase* newJob = util::construct< Job<Signature> >( job, priority, threadIndex );
				
				// Add the job to this thread pool.
				this->addNewJob( newJob, jobID );
			}
			
			
			/// Wait for all of the jobs queued in this thread pool for the specified job ID to finish.
			void finishJob( Index jobID );
			
			
			/// Wait for all of the jobs queued in this thread pool to finish before returning.
			void finishJobs();
			
			
			/// Remove all currently queued jobs from this thread pool.
			void clearJobs();
			
			
		//********************************************************************************
		//******	Thread Priority Accessor Methods
			
			
			/// Return the thread priority that is used for all of the threads in this pool.
			OM_INLINE ThreadPriority getPriority() const
			{
				return priority;
			}
			
			
			/// Set the thread priority that is used for all of the threads in this pool.
			/**
			  * The method returns whether or not the priority was successfully changed.
			  */
			Bool setPriority( const ThreadPriority& newPriority );
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that represents a single worker thread that is a part of this pool.
			class Worker;
			
			
			/// A class that keeps track of information needed for a particular job ID.
			class JobID
			{
				public:
					
					OM_INLINE JobID()
						:	jobID( 0 ),
							numUnfinishedJobs( 0 ),
							numWaiting( 0 )
					{
					}
					
					OM_INLINE JobID( Index newJobID )
						:	jobID( newJobID ),
							numUnfinishedJobs( 0 ),
							numWaiting( 0 )
					{
					}
					
					/// Wait for this job ID to finish processing.
					OM_INLINE void finish()
					{
						// Indicate that there is another thread waiting on the job.
						numWaiting++;
						
						// Wait for the job to finish if it has not already.
						if ( numUnfinishedJobs > 0 )
						{
							// Wait for the job ID to finish.
							finishSignal.lock();
							
							while ( numUnfinishedJobs > 0 )
								finishSignal.wait();
							
							finishSignal.unlock();
						}
						
						// Indicate that this thread is no longer waiting for the job.
						numWaiting--;
					}
					
					/// Signal to any waiting threads that this job ID is finished.
					OM_INLINE void signal()
					{
						// Signal waiting threads if necessary.
						if ( --numUnfinishedJobs == 0 )
						{
							finishSignal.lock();
							finishSignal.signal();
							finishSignal.unlock();
						}
					}
					
					
					/// The ID of the job.
					Index jobID;
					
					/// The number of unfinished jobs there are for this job ID.
					Atomic<Size> numUnfinishedJobs;
					
				private:
					
					/// The number of threads that are waiting for this job ID to finish.
					Atomic<Size> numWaiting;
					
					/// An object that is used to signal waiting threads upon job completion.
					Signal finishSignal;
					
			};
			
			
			/// A class that represents the base class a single generic job for this thread pool.
			class JobBase
			{
				public:
					
					/// Create a new thread job object with the given priority.
					OM_INLINE JobBase( Float newPriority, Index newThreadIndex )
						:	jobID( NULL ),
							jobIndex( 0 ),
							threadIndex( newThreadIndex ),
							priority( newPriority )
					{
					}
					
					
					/// Destroy this job.
					virtual ~JobBase()
					{
					}
					
					
					/// Execute this job on the calling thread.
					virtual void execute() = 0;
					
					
					/// Construct and return a copy of this job.
					virtual JobBase* copy() const = 0;
					
					
					/// Return whether or not this job has a higher priority than another job.
					OM_FORCE_INLINE Bool operator < ( const JobBase& other ) const
					{
						if ( priority < other.priority )
							return true;
						else if ( priority == other.priority )
							return jobIndex > other.jobIndex;
						else
							return false;
					}
					
					
					/// The index of this job's ID, used to group it with other jobs.
					JobID* jobID;
					
					
					/// The job index of this job (based on when it was added to the pool).
					/**
					  * This value is used to sort earlier jobs higher in the queue than later jobs
					  * when the priority is the same.
					  */
					Index jobIndex;
					
					
					/// The thread index that the user wants this job to execute on, or -1 if unspecified.
					Index threadIndex;
					
					
					/// The priority of this job.
					/**
					  * A higher priority means that the job will preempt other jobs with lower
					  * priorities.
					  */
					Float priority;
					
			};
			
			
			/// A class that encapsulates a templated generic job for this thread pool.
			template < typename Signature >
			class Job : public JobBase
			{
				public:
					
					/// Create a new  thread job object that runs the given function call with its priority.
					OM_INLINE Job( const lang::FunctionCall<Signature>& newJob, Float newPriority, Index newThreadIndex )
						:	JobBase( newPriority, newThreadIndex ),
							job( newJob )
					{
					}
					
					
					/// Execute this job on the calling thread.
					virtual void execute()
					{
						job();
					}
					
					
					/// Construct and return a copy of this job.
					virtual Job<Signature>* copy() const
					{
						return util::construct< Job<Signature> >( *this );
					}
					
					
					/// A function call object specifying what job to perform on the thread.
					lang::FunctionCall<Signature> job;
					
					
			};
			
			
			/// A proxy object that stores a pointer to a generic job and allows sorting based on job priority.
			class SortableJob
			{
				public:
					
					/// Create a new sortable job object corresponding to the specified job.
					OM_INLINE SortableJob( JobBase* newJob )
						:	job( newJob )
					{
					}
					
					/// Return whether or not this job has a higher priority than another job.
					OM_FORCE_INLINE Bool operator < ( const SortableJob& other ) const
					{
						return (*job) < (*other.job);
					}
					
					
					/// A pointer to the job which this sortable job is a part of.
					JobBase* job;
					
			};
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Add a new job to the internal queue of jobs.
			void addNewJob( JobBase* newJob, Index jobID );
			
			
			/// Find and return a pointer to the JobID object with the given job ID, creating a new one if necessary.
			OM_FORCE_INLINE JobID* createJobID( Index jobID );
			
			
			/// Find and return the index of the JobID object with the given job ID.
			/**
			  * If no such job ID is found, the method returns -1.
			  */
			JobID* findJobID( Index jobID );
			
			
			/// Clear and deallocate all jobs and job IDs from the thread pool.
			void deallocateJobs();
			
			
			/// Wait until the job queue is not accessed by another thread.
			OM_FORCE_INLINE void lockJobs() const;
			
			
			/// Unlock access to the job queue.
			OM_FORCE_INLINE void unlockJobs() const;
			
			
			/// Wait until the threads are not accessed by another thread.
			OM_FORCE_INLINE void lockThreads() const;
			
			
			/// Unlock access to the threads.
			OM_FORCE_INLINE void unlockThreads() const;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the threads that are currently part of this thread pool and are executing jobs.
			util::ArrayList<Worker*> threads;
			
			
			/// A queue of jobs to be performed by the thread pool.
			util::PriorityQueue<SortableJob> jobs;
			
			
			/// A list of the currently pending job IDs.
			util::ArrayList<JobID*> jobIDs;
			
			
			/// The index of the next job to be queued.
			Index jobIndex;
			
			
			/// The thread priority to use for all of the threads in this pool.
			ThreadPriority priority;
			
			
			/// A signal that worker threads wait on where there are no jobs available.
			Semaphore workerSemaphore;
			
			
			/// An atomic value that counts the number of threads that are idle and waiting for jobs.
			mutable Atomic<Size> numWaitingWorkers;
			
			
			/// An object that is used to signal waiting threads upon completion of all jobs.
			JobID allJobs;
			
			
#if defined(OM_PLATFORM_WINDOWS)
			/// A mutex that protects the threads.
			mutable Mutex threadMutex;
#else
			/// An atomic value that protects the threads.
			mutable Atomic<Size> threadsAccessed;
#endif
			
			
#if defined(OM_PLATFORM_WINDOWS)
			/// A mutex that protects the job queue.
			mutable Mutex jobMutex;
#else
			/// An atomic value that protects the job queue.
			mutable Atomic<Size> jobsAccessed;
#endif
			
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_THREAD_POOL_H
