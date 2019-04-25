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

#include "omSignal.h"


// Include platform-specific header files
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	#include <pthread.h>
	#include <time.h>
#elif defined(OM_PLATFORM_WINDOWS)
	#define NOMINMAX
	#include <Windows.h>
#else
	#error unsupported platform
#endif


#define ERROR_CREATING_SIGNAL "Attempt to create a signal failed."


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Signal Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class Signal:: SignalWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE SignalWrapper()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_init( &mutex, NULL );
				result |= pthread_cond_init( &condition, NULL );
				
				OM_ASSERT_MESSAGE( result == 0, ERROR_CREATING_SIGNAL );
				
#elif defined(OM_PLATFORM_WINDOWS)
				InitializeConditionVariable( &condition );
				InitializeCriticalSection( &criticalSection );
#endif
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			OM_INLINE ~SignalWrapper()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				pthread_mutex_destroy( &mutex );
				pthread_cond_destroy( &condition );
				
#elif defined(OM_PLATFORM_WINDOWS)
				WakeAllConditionVariable( &condition );
				DeleteCriticalSection( &criticalSection );
				// No need to delete the condition.
#endif
			}
			
			
		//********************************************************************************
		//******	Wait/Signal Methods
			
			
			OM_INLINE Bool wait()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_cond_wait( &condition, &mutex );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
				BOOL success = SleepConditionVariableCS( &condition, &criticalSection, INFINITE );
				return success != FALSE;
#endif
			}
			
			
			OM_INLINE Bool wait( const time::Time& timeout )
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				// Convert relative to absolute time.
				Int64 nano = (time::Time::getCurrent() + timeout).getNanoseconds();
				timespec t;
				t.tv_sec = nano / Int64(1000000000);
				t.tv_nsec = nano - t.tv_sec*Int64(1000000000);
				
				int result = pthread_cond_timedwait( &condition, &mutex, &t );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
				Int64 milliseconds = math::max( timeout.getNanoseconds() / Int64(1000000), Int64(1) );
				BOOL success = SleepConditionVariableCS( &condition, &criticalSection, (DWORD)milliseconds );
				return success != FALSE;
#endif
			}
			
			
			OM_INLINE Bool signal()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_cond_broadcast( &condition );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
				WakeAllConditionVariable( &condition );
				return true;
#endif
			}
			
			
			OM_INLINE Bool signalOne()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_cond_signal( &condition );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
				WakeConditionVariable( &condition );
				return true;
#endif
			}
			
			
			OM_INLINE Bool lock()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_lock( &mutex );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
				EnterCriticalSection( &criticalSection );
				return true;
#endif
			}
			
			
			OM_INLINE Bool unlock()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_unlock( &mutex );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
				LeaveCriticalSection( &criticalSection );
				return true;
#endif
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
			mutable pthread_mutex_t mutex;
			pthread_cond_t condition;
			
#elif defined(OM_PLATFORM_WINDOWS)
			mutable CONDITION_VARIABLE condition;
			mutable CRITICAL_SECTION criticalSection;
				
#endif
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Signal:: Signal()
	:	wrapper( util::construct<SignalWrapper>() )
{
}




Signal:: Signal( const Signal& signal )
	:	wrapper( util::construct<SignalWrapper>() )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Signal:: ~Signal()
{
	util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Signal& Signal:: operator = ( const Signal& signal )
{
	if ( this != &signal )
	{
		util::destruct( wrapper );
		
		wrapper = util::construct<SignalWrapper>();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Wait/Signal Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Signal:: signal()
{
	return wrapper->signal();
}




Bool Signal:: signalOne()
{
	return wrapper->signalOne();
}




Bool Signal:: wait() const
{
	return wrapper->wait();
}




Bool Signal:: wait( const time::Time& timeout ) const
{
	return wrapper->wait( timeout );
}




Bool Signal:: lock()
{
	return wrapper->lock();
}




Bool Signal:: unlock()
{
	return wrapper->unlock();
}




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
