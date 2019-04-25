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


#if defined(OM_PLATFORM_WINDOWS)


#include <Windows.h>


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
				:	handle( NULL )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			OM_INLINE ~ThreadWrapper()
			{
				CloseHandle( handle );
			}
			
			
		//********************************************************************************
		//******	Thread Entry Points
			
			
			static DWORD __stdcall entryPoint( LPVOID data )
			{
				ThreadBase* thread = static_cast<ThreadBase*>(data);
				
				if ( thread )
					thread->runThread();
				
				return NULL;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A handle to a windows thread
			HANDLE handle;
			
			
			/// The ID of a windows thread
			DWORD threadID;
			
			
			
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
	if ( wrapper->handle != NULL )
		CloseHandle( wrapper->handle );

	wrapper->handle = CreateThread(	NULL, 0, (LPTHREAD_START_ROUTINE)ThreadWrapper::entryPoint, 
									(LPVOID)this, 0, (LPDWORD)&wrapper->threadID );
	
	return (wrapper->handle != NULL);
}




void ThreadBase:: runThread()
{
	threadIsRunning = true;
	this->run();
	threadIsRunning = false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Stop Method
//############		
//##########################################################################################
//##########################################################################################




Bool ThreadBase:: stop()
{
	BOOL success = TerminateThread( wrapper->handle, 1 );
	
	if ( !success )
		return false;
		
	CloseHandle( wrapper->handle );
	wrapper->handle = NULL;
	threadIsRunning = false;

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
	if ( wrapper->handle != NULL )
		return WaitForSingleObject( wrapper->handle, INFINITE ) == WAIT_OBJECT_0;
	else
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
	return (ThreadID)wrapper->threadID;
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
	int priority = GetThreadPriority( wrapper->handle );
	
	if ( priority == THREAD_PRIORITY_ERROR_RETURN )
		return ThreadPriority::DEFAULT;
	
	switch ( priority )
	{
		case THREAD_PRIORITY_LOWEST:
			return ThreadPriority::LOW;
		
		case THREAD_PRIORITY_NORMAL:
			return ThreadPriority::MEDIUM;
		
		case THREAD_PRIORITY_HIGHEST:
			return ThreadPriority::HIGH;
		
		case THREAD_PRIORITY_TIME_CRITICAL:
			return ThreadPriority::TIME_CRITICAL;
	}
	
	return ThreadPriority::DEFAULT;
}




Bool ThreadBase:: setPriority( const ThreadPriority& newPriority )
{
	int priority = THREAD_PRIORITY_NORMAL;
	
	switch ( newPriority )
	{
		case ThreadPriority::LOW:
			priority = THREAD_PRIORITY_LOWEST;
			break;
			
		case ThreadPriority::HIGH:
			priority = THREAD_PRIORITY_HIGHEST;
			break;
			
		case ThreadPriority::TIME_CRITICAL:
			priority = THREAD_PRIORITY_TIME_CRITICAL;
			break;
	}
	
    return SetThreadPriority( wrapper->handle, priority ) != 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Sleeping Method
//############		
//##########################################################################################
//##########################################################################################




void ThreadBase:: sleepMs( UInt milliseconds )
{
	Sleep( (DWORD)milliseconds );
}




void ThreadBase:: sleep( double seconds )
{
	// Make sure that we aren't sleeping for a negative amount of time.
	if ( seconds < 0 )
		return;
	
	int milliseconds = int(seconds*1e3);
	
	if ( milliseconds < 1000000 )
		Sleep( milliseconds );
	else
	{
		int intSeconds = int(seconds);
		
		for ( int i = 0; i < intSeconds; i++ )
			Sleep( 1000 );
		
		Sleep( int((seconds - (double)intSeconds)*1.0e3) );
	}
}




void ThreadBase:: yield()
{
	Sleep( 0 );
}




void ThreadBase:: exit()
{
	ExitThread( NULL );
}




ThreadID ThreadBase:: getCurrentID()
{
	return (ThreadID)GetCurrentThreadId();
}




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_WINDOWS
