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

#include "omSemaphore.h"


#include "omAtomics.h"


#if defined(OM_PLATFORM_APPLE)
	#include <mach/mach_traps.h>
	#include <mach/semaphore.h>
	#include <mach/task.h>
	#include <mach/mach.h>
#elif defined(OM_PLATFORM_LINUX)
	#include <pthread.h>
#elif defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
#else
	#error unsupported platform
#endif


#define ERROR_CREATING_SEMAPHORE "Error while creating semaphore object."


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Semaphore Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class Semaphore:: SemaphoreWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			OM_INLINE SemaphoreWrapper( Int initialValue )
				:	value( initialValue )
			{
#if defined(OM_PLATFORM_APPLE)
				kern_return_t result = semaphore_create( mach_task_self(), &semaphore, SYNC_POLICY_FIFO, initialValue );
				OM_ASSERT_MESSAGE( result == KERN_SUCCESS, ERROR_CREATING_SEMAPHORE );
				
#elif defined(OM_PLATFORM_LINUX)
				int result = pthread_cond_init( &condition, NULL );
				result |= pthread_mutex_init( &mutex, NULL );
				OM_ASSERT_MESSAGE( result == 0, ERROR_CREATING_SEMAPHORE );
				
#elif defined(OM_PLATFORM_WINDOWS)
				semaphore = CreateSemaphore( NULL, initialValue, math::max<LONG>(), NULL );
				OM_ASSERT_MESSAGE( semaphore != NULL, ERROR_CREATING_SEMAPHORE );
#endif
			}
			
			
			OM_INLINE SemaphoreWrapper( const SemaphoreWrapper& wrapper )
				:	value( wrapper.getValue() )
			{
#if defined(OM_PLATFORM_APPLE)
				kern_return_t result = semaphore_create( mach_task_self(), &semaphore, SYNC_POLICY_FIFO, value );
				OM_ASSERT_MESSAGE( result == KERN_SUCCESS, ERROR_CREATING_SEMAPHORE );
				
#elif defined(OM_PLATFORM_LINUX)
				int result = pthread_cond_init( &condition, NULL );
				result |= pthread_mutex_init( &mutex, NULL );
				OM_ASSERT_MESSAGE( result != 0, ERROR_CREATING_SEMAPHORE );
				
#elif defined(OM_PLATFORM_WINDOWS)
				semaphore = CreateSemaphore( NULL, value, math::max<LONG>(), NULL );
				OM_ASSERT_MESSAGE( semaphore != NULL, ERROR_CREATING_SEMAPHORE );
#endif
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			OM_INLINE ~SemaphoreWrapper()
			{
#if defined(OM_PLATFORM_APPLE)
				// Awaken other threads that are waiting on the semaphore then destroy it.
				semaphore_signal_all( semaphore );
				semaphore_destroy( mach_task_self(),  semaphore );
				
#elif defined(OM_PLATFORM_LINUX)
				pthread_mutex_destroy( &mutex );
				pthread_cond_destroy( &condition );
				
#elif defined(OM_PLATFORM_WINDOWS)
				// Awaken other threads that are waiting on the semaphore then destroy it.
				ReleaseSemaphore( semaphore, math::max<LONG>(), NULL );
				CloseHandle( semaphore );
#endif
			}
			
			
		//********************************************************************************
		//******	Semaphore Up/Down Methods
			
			
			OM_INLINE Bool up()
			{
#if defined(OM_PLATFORM_APPLE)
				value++;
				kern_return_t result = semaphore_signal( semaphore );
				
				return result == KERN_SUCCESS;
				
#elif defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_lock( &mutex );
				value++;
				if ( value <= 0 )
					result |= pthread_cond_signal( &condition );
				
				result |= pthread_mutex_unlock( &mutex );
				return result != 0;
#elif defined(OM_PLATFORM_WINDOWS)
				value++;
				BOOL error = ReleaseSemaphore( semaphore, 1, NULL );
				
				return error != FALSE;
#endif
			}
			
			
			OM_INLINE Bool down()
			{
#if defined(OM_PLATFORM_APPLE)
				kern_return_t result = semaphore_wait( semaphore );
				value--;
				
				return result == KERN_SUCCESS;
				
#elif defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_lock( &mutex );
				value--;
				if ( value < 0 )
					result |= pthread_cond_wait( &condition, &mutex );
				
				result |= pthread_mutex_unlock( &mutex );
				return result != 0;
#elif defined(OM_PLATFORM_WINDOWS)
				DWORD result = WaitForSingleObject( semaphore, INFINITE );
				value--;
				
				return result != WAIT_FAILED;
#endif
			}
			
			
			OM_INLINE Bool reset()
			{
#if defined(OM_PLATFORM_APPLE)
				value = 0;
				kern_return_t result = semaphore_signal_all( semaphore );
				
				return result == KERN_SUCCESS;
				
#elif defined(OM_PLATFORM_LINUX)
				int result = pthread_mutex_lock( &mutex );
				value = 0;
				result |= pthread_cond_broadcast( &condition );
				result |= pthread_mutex_unlock( &mutex );
				return result != 0;
#elif defined(OM_PLATFORM_WINDOWS)
				value = 0;
				BOOL error = ReleaseSemaphore( semaphore, math::max<LONG>(), NULL );
				
				return error != FALSE;
#endif
			}
			
			
		//********************************************************************************
		//******	Semaphore Value Accessor Method
			
			
			OM_INLINE Int getValue() const
			{
				return value;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			
#if defined(OM_PLATFORM_APPLE)
			semaphore_t semaphore;
			
#elif defined(OM_PLATFORM_LINUX)
			pthread_mutex_t mutex;
			pthread_cond_t condition;
			
#elif defined(OM_PLATFORM_WINDOWS)
			HANDLE semaphore;
#endif
			
			Int value;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Semaphore:: Semaphore()
	:	wrapper( util::construct<SemaphoreWrapper>( 0 ) )
{
}




Semaphore:: Semaphore( Int initialValue )
	:	wrapper( util::construct<SemaphoreWrapper>(initialValue) )
{
}




Semaphore:: Semaphore( const Semaphore& semaphore )
	:	wrapper( util::construct<SemaphoreWrapper>(*semaphore.wrapper) )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Semaphore:: ~Semaphore()
{
	// Destroy the wrapper object.
	util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Semaphore& Semaphore:: operator = ( const Semaphore& semaphore )
{
	if ( this != &semaphore )
	{
		// Release and destroy the previous semaphore.
		util::destruct( wrapper );
		
		// Create a new semaphore wrapper object.
		wrapper = util::construct<SemaphoreWrapper>(*semaphore.wrapper);
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Semaphore Up/Down Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Semaphore:: up()
{
	return wrapper->up();
}




Bool Semaphore:: down()
{
	return wrapper->down();
}




Bool Semaphore:: reset()
{
	return wrapper->reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Semaphore Value Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Int Semaphore:: getValue() const
{
	return wrapper->getValue();
}




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


