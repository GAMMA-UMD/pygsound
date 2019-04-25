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

#include "omThreadBase.h"

#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)

#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>

//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Thread Wrapper Class Definition
//############		
//##########################################################################################
//##########################################################################################




class ThreadBase:: ThreadWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE ThreadWrapper()
				:	handle( pthread_t() ),
					threadID( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Thread Entry Point
			
			
			static void* entryPoint( void* argument )
			{
				ThreadBase* thread = static_cast<ThreadBase*>(argument);
				
				if ( thread )
				{
					thread->wrapper->threadID = ThreadBase::getCurrentID();
					thread->runThread();
				}
				
				return NULL;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A handle to a pThread.
			pthread_t handle;
			
			/// A unique integer ID for the thread.
			ThreadID threadID;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ThreadBase:: ThreadBase()
	:	wrapper( util::construct<ThreadWrapper>() ),
		threadIsRunning( false )
{
}




ThreadBase:: ThreadBase( const ThreadBase& other )
	:	wrapper( util::construct<ThreadWrapper>() ),
		threadIsRunning( false )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ThreadBase:: ~ThreadBase()
{
	pthread_detach( wrapper->handle );
	util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ThreadBase& ThreadBase:: operator = ( const ThreadBase& other )
{
	if ( this != &other )
	{
		util::destruct( wrapper );
		wrapper = util::construct<ThreadWrapper>();
		threadIsRunning = false;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Start Method
//############		
//##########################################################################################
//##########################################################################################




Bool ThreadBase:: startThread()
{
	if ( !threadIsRunning )
	{
		int success = pthread_create( &wrapper->handle, NULL, ThreadWrapper::entryPoint, this );
		
		if ( success != 0 )
			return false;
		
		threadIsRunning = true;
	}
	
	return true;
}




#if !defined(OM_PLATFORM_APPLE)
void ThreadBase:: runThread()
{
	threadIsRunning = true;
	this->run();
	threadIsRunning = false;
}
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Stop Method
//############		
//##########################################################################################
//##########################################################################################




Bool ThreadBase:: stop()
{
	if ( threadIsRunning )
	{
		int success = pthread_cancel( wrapper->handle );
		
		if ( success != 0 )
			return false;
		
		pthread_join( wrapper->handle, NULL );
		threadIsRunning = false;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Join Method
//############		
//##########################################################################################
//##########################################################################################




Bool ThreadBase:: joinThread()
{
	if ( threadIsRunning )
	{
		int result = pthread_join( wrapper->handle, NULL );
		
		return result == 0;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread ID Accessor Method
//############		
//##########################################################################################
//##########################################################################################




ThreadID ThreadBase:: getID() const
{
	return wrapper->threadID;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Priority Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




ThreadPriority ThreadBase:: getPriority( const ThreadPriority& newPriority )
{
	int policy = SCHED_OTHER;
	struct sched_param param;
	
	if ( pthread_getschedparam( wrapper->handle, &policy, &param ) == 0 )
		return ThreadPriority::DEFAULT;
	
	switch ( policy )
	{
		case SCHED_FIFO:
			return ThreadPriority::TIME_CRITICAL;
		
		case SCHED_RR:
		{
			if ( param.sched_priority == sched_get_priority_min(policy) )
				return ThreadPriority::LOW;
			else if ( param.sched_priority == sched_get_priority_max(policy) )
				return ThreadPriority::HIGH;
			else
				return ThreadPriority::MEDIUM;
		}
	}
	
	return ThreadPriority::DEFAULT;
}




Bool ThreadBase:: setPriority( const ThreadPriority& newPriority )
{
	int policy = SCHED_OTHER;
	struct sched_param param;
	
	switch ( newPriority )
	{
		case ThreadPriority::LOW:
			policy = SCHED_RR;
			param.sched_priority = sched_get_priority_min(policy);
			break;
		
		case ThreadPriority::MEDIUM:
			policy = SCHED_RR;
			param.sched_priority = (sched_get_priority_min(policy) + sched_get_priority_max(policy)) / 2;
			break;
		
		case ThreadPriority::HIGH:
			policy = SCHED_RR;
			param.sched_priority = sched_get_priority_max(policy);
			break;
		
		case ThreadPriority::TIME_CRITICAL:
			policy = SCHED_FIFO;
			param.sched_priority = sched_get_priority_max(policy);
			break;
	}
	
    return pthread_setschedparam( wrapper->handle, policy, &param );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Current Thread Control Methods
//############		
//##########################################################################################
//##########################################################################################




void ThreadBase:: sleepMs( UInt milliseconds )
{
	// Convert to microseconds.
	usleep( milliseconds*1000 );
}




void ThreadBase:: sleep( double seconds )
{
	// Make sure that we aren't sleeping for a negative amount of time.
	if ( seconds < 0 )
		return;
	
	Int64 microseconds = Int64(seconds*1.0e6);
	
	if ( microseconds < Int64(1000000) )
		usleep( (UInt32)microseconds );
	else
	{
		UInt intSeconds = UInt(seconds);
		
		for ( UInt i = 0; i < intSeconds; i++ )
			usleep( 999999 );
		
		usleep( UInt((seconds - (double)intSeconds)*1.0e6) );
	}
}




void ThreadBase:: yield()
{
	sched_yield();
}




void ThreadBase:: exit()
{
	pthread_exit(NULL);
}




ThreadID ThreadBase:: getCurrentID()
{
#if defined(OM_PLATFORM_LINUX)
	return syscall(SYS_gettid);
#elif defined(OM_PLATFORM_APPLE)
	UInt64 threadID;
	
	if ( pthread_threadid_np( pthread_self(), &threadID ) == 0 )
		return (ThreadID)threadID;
	else
		return 0;
#endif
}




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_APPLE || OM_PLATFORM_LINUX
