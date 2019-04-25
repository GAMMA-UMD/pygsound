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

#ifndef INCLUDE_OM_SEMAPHORE_H
#define INCLUDE_OM_SEMAPHORE_H


#include "omThreadsConfig.h"


#include "../omUtilities.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements a count-based synchronization object.
class Semaphore
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new semaphore object with an initial value of 0.
			Semaphore();
			
			
			/// Create a new semaphore object with the specified initial value.
			Semaphore( Int initialValue );
			
			
			/// Create a new semaphore with the same value as the specified semaphore.
			Semaphore( const Semaphore& semaphore );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a semaphore object.
			/**
			  * This has the effect of awakening all waiting
			  * threads. This is done in order to prevent deadlocks
			  * in the case of accidental destruction.
			  */
			~Semaphore();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign one semaphore to another.
			/**
			  * This awakens all threads currently waiting on the semaphore
			  * and gives the semaphore an initial value equal to the other
			  * semaphore's current value.
			  */
			Semaphore& operator = ( const Semaphore& semaphore );
			
			
		//********************************************************************************
		//******	Up/Down Methods
			
			
			/// Increase the value of the semaphore by 1.
			/**
			  * This awakens at most 1 thread which was suspended after a call
			  * to down() when the value of the semaphore was 0. The awoken thread
			  * then decreases the value of the semaphore by 1 and returns from
			  * down(). Threads are awoken in a first-in-first-out order: the first
			  * thread to start waiting on a call to down is the first awoken after
			  * a call to up().
			  *
			  * The method returns whether or not the operation was successful.
			  */
			Bool up();
			
			
			/// Decrease the value of the semaphore by 1.
			/**
			  * If the value of the semaphore before the call to down() was 0,
			  * then the calling thread is blocked until another thread makes
			  * a call to up().
			  *
			  * The method returns whether or not the operation was successful.
			  */
			Bool down();
			
			
			/// Awaken all threads that are waiting on the semaphore and set its value to 0.
			/**
			  * The method returns whether or not the operation was successful.
			  */
			Bool reset();
			
			
		//********************************************************************************
		//******	Value Accessor Method
			
			
			/// Return the current value of the semaphore.
			/**
			  * The returned value is not sychronized with calls to up()/down() on
			  * other threads, so it may not represent the instantaneous state of the semaphore.
			  */
			Int getValue() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Semaphore Wrapper Class Declaration
			
			
			/// A class that encapsulates internal platform-specific Semaphore code.
			class SemaphoreWrapper;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A wrapper of the internal platform-specific state of the semaphore.
			SemaphoreWrapper* wrapper;
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SEMAPHORE_H
