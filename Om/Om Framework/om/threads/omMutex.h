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

#ifndef INCLUDE_OM_MUTEX_H
#define INCLUDE_OM_MUTEX_H


#include "omThreadsConfig.h"


#include "../omUtilities.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class whose job is to provide a means of thread synchronization by exclusion.
/** 
  * The class is essentially a wrapper around the host platform's mutex facilities.
  * It allows threads to be synchronized so that access to data or other sensitive
  * items can be restricted to one thread at a time.
  *
  *	In order to use the class properly, call the lock() method to lock the
  * mutex and call the unlock() method to unlock it. The lock() method blocks
  * execution of the calling thread until the mutex has been released by another
  * thread. One can also query the state of the mutex (locked or unlocked) using
  * the method isUnlocked().
  */
class Mutex
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new mutex in the default state of not locked.
			Mutex();
			
			
			/// Create a copy of a Mutex object.
			/**
			  * The new mutex is created in the unlocked state and is independent
			  * of the other mutex. The effect is the same as the default constructor.
			  */
			Mutex( const Mutex& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a Mutex object, releasing all internal state.
			~Mutex();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign one Mutex to another.
			/**
			  * This operator releases any previous lock on this mutex and effectively results
			  * in a new mutex in the unlocked state. The new mutex is independent
			  * of the other mutex.
			  */
			Mutex& operator = ( const Mutex& other );
			
			
		//********************************************************************************
		//******	Mutex Locking Methods
			
			
			/// Try to lock a mutex, but return immediately if it is already locked.
			/**
			  * The mutex is only locked if it is available.
			  *
			  * The method returns whether or not the mutex was locked.
			  */
			Bool tryLock();
			
			
			/// Wait until the Mutex is available for the current thread of execution.
			/**
			  * This method blocks the current thread until the signal is recieved
			  * that the Mutex has been released, at which time the Mutex is acquired
			  * by the current thread and the method returns. If the Mutex is available,
			  * the method returns immediately and the Mutex is acquired.
			  *
			  * The method returns whether or not the operation was successful.
			  */
			Bool lock();
			
			
			/// Release the mutex so that another thread can acquire it.
			/**
			  * If the mutex is not already acquired, this method
			  * has no effect.
			  *
			  * The method returns whether or not the operation was successful.
			  */
			Bool unlock();
			
			
		//********************************************************************************
		//******	Mutex Status Accessor
			
			
			/// Get whether or not the mutex is available.
			/**
			  * If the mutex is free for acquisition, TRUE is returned. Otherwise
			  * FALSE is returned.
			  * 
			  * @return whether or not the mutex is available for acquisition.
			  */
			Bool isLocked() const;
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Mutex Wrapper Class Declaration
			
			
			/// A class that encapsulates internal platform-specific Mutex code.
			class MutexWrapper;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to a wrapper object containing the internal state of the Mutex.
			MutexWrapper* wrapper;
			
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_MUTEX_H
