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

#include "omMutex.h"


// Include platform-specific header files
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
	#include <pthread.h>
#elif defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
	
	/*
		Change this to 1 to indicate that heavyweight Win32 mutex objects
		should be used instead of lightweight critical sections.
		
		Heavyweight mutexes are up to 25x slower because they synchronize processes
		and so must enter the kernel to lock a mutex, even an uncontested one.
	*/
	#define USE_HEAVYWEIGHT_MUTEX 0
#else
	#error unsupported platform
#endif


#define ERROR_CREATING_MUTEX "Error while creating mutex object."


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Mutex Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class Mutex:: MutexWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE MutexWrapper()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_init( &mutex, NULL );
				OM_ASSERT_MESSAGE( result == 0, ERROR_CREATING_MUTEX );
				
#elif defined(OM_PLATFORM_WINDOWS)
	#if USE_HEAVYWEIGHT_MUTEX
				// Create a new Mutex object.
				mutex = CreateMutex( NULL, FALSE, NULL );
				OM_ASSERT_MESSAGE( mutex != NULL, ERROR_CREATING_MUTEX );
	#else
				// Initialize the critical section.
				InitializeCriticalSection( &criticalSection );
	#endif
#endif
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			OM_INLINE ~MutexWrapper()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				pthread_mutex_destroy( &mutex );
#elif defined(OM_PLATFORM_WINDOWS)
	#if USE_HEAVYWEIGHT_MUTEX
				CloseHandle( mutex );
	#else
				// Destroy the critical section.
				DeleteCriticalSection( &criticalSection );
	#endif
#endif
			}
			
			
		//********************************************************************************
		//******	Mutex Availability Test Method
			
			
			OM_INLINE Bool isLocked() const
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				
				// Test the mutex to see if it is available at this moment.
				int result = pthread_mutex_trylock( &mutex );
				
				if ( result == -1 )
					return true;
				else
				{
					// We acquired the mutex, so release it again to make it available.
					pthread_mutex_unlock( &mutex );
					
					return false;
				}
				
#elif defined(OM_PLATFORM_WINDOWS)
	#if USE_HEAVYWEIGHT_MUTEX
				// Try to acquire the mutex with an instant time-out if it is already acquired.
				DWORD result = WaitForSingleObject( mutex, 0 );
				
				// If the wait operation timed out, the mutex is not available.
				if ( result == WAIT_TIMEOUT )
					return true;
				else
				{
					// We acquired the mutex, so release it again to make it available.
					ReleaseMutex( mutex );
					
					return false;
				}
	#else
				// Try locking the critical section.
				BOOL success = TryEnterCriticalSection( &criticalSection );
				
				if ( !success )
					return true;
				else
				{
					// Release the acquired critical section.
					LeaveCriticalSection( &criticalSection );
					
					return false;
				}
	#endif
#endif
			}
			
			
		//********************************************************************************
		//******	Mutex Acquisition Methods
			
			
			OM_INLINE Bool tryLock()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_trylock( &mutex );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
	#if USE_HEAVYWEIGHT_MUTEX
				// Try to lock the mutex with a zero timeout.
				DWORD result = WaitForSingleObject( mutex, 0 );
				
				return result == WAIT_OBJECT_0;
	#else
				// Lock the critical section.
				return TryEnterCriticalSection( &criticalSection ) != 0;
	#endif
#endif
			}
			
			
			OM_INLINE Bool lock()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_lock( &mutex );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
	#if USE_HEAVYWEIGHT_MUTEX
				// Wait for the mutex.
				DWORD result = WaitForSingleObject( mutex, INFINITE );
				
				return result == WAIT_OBJECT_0 || result == WAIT_ABANDONED;
	#else
				// Lock the critical section.
				EnterCriticalSection( &criticalSection );
				return true;
	#endif
#endif
			}
			
			
		//********************************************************************************
		//******	Mutex Release Method
			
			
			OM_INLINE Bool unlock()
			{
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
				// Unlock the mutex.
				int result = pthread_mutex_unlock( &mutex );
				return result == 0;
				
#elif defined(OM_PLATFORM_WINDOWS)
	#if USE_HEAVYWEIGHT_MUTEX
				// Release the mutex.
				BOOL success = ReleaseMutex( mutex );
				return success != 0;
	#else
				// Unlock the critical section.
				LeaveCriticalSection( &criticalSection );
				return true;
	#endif
#endif
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
#if defined(OM_PLATFORM_APPLE) || defined(OM_PLATFORM_LINUX)
			
			/// A handle to a pthread mutex object.
			mutable pthread_mutex_t mutex;
			
#elif defined(OM_PLATFORM_WINDOWS)
	#if USE_HEAVYWEIGHT_MUTEX
			/// A handle to a windows mutex object.
			mutable HANDLE mutex;
	#else
			/// A structure containing information for the critical section of this mutex.
			mutable CRITICAL_SECTION criticalSection;
	#endif
#endif
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Platform-Independent Code
//############		
//##########################################################################################
//##########################################################################################




Mutex:: Mutex()
	:	wrapper( util::construct<MutexWrapper>() )
{
}




Mutex:: Mutex( const Mutex& other )
	:	wrapper( util::construct<MutexWrapper>() )
{
}




Mutex:: ~Mutex()
{
	// Destoy the wrapper object.
	util::destruct( wrapper );
}




Mutex& Mutex:: operator = ( const Mutex& other )
{
	if ( this != &other )
	{
		// Release the previous mutex.
		wrapper->unlock();
	}
	
	return *this;
}




Bool Mutex:: tryLock()
{
	return wrapper->tryLock();
}




Bool Mutex:: lock()
{
	return wrapper->lock();
}




Bool Mutex:: unlock()
{
	return wrapper->unlock();
}




Bool Mutex:: isLocked() const
{
	return wrapper->isLocked();
}




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
