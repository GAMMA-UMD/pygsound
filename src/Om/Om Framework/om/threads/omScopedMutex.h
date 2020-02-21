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

#ifndef INCLUDE_OM_SCOPED_MUTEX_H
#define INCLUDE_OM_SCOPED_MUTEX_H


#include "omThreadsConfig.h"


#include "omMutex.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that locks a mutex upon construction and unlocks it upon destruction.
/**
  * This class can be used to ensure that a mutex that is locked at the beginning of a
  * function will be automatically unlocked for each return path.
  */
class ScopedMutex
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new scoped mutex that locks the specified mutex.
			OM_INLINE ScopedMutex( Mutex& newMutex )
				:	mutex( newMutex )
			{
				mutex.lock();
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Unlock the scoped mutex's mutex.
			OM_INLINE ~ScopedMutex()
			{
				mutex.unlock();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Copy Operations
			
			
			/// Declared private to prevent copying.
			OM_INLINE ScopedMutex( const ScopedMutex& other )
				:	mutex( other.mutex )
			{
			}
			
			
			/// Declared private to prevent copying.
			OM_INLINE ScopedMutex& operator = ( const ScopedMutex& other )
			{
				return *this;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A temporary reference to the mutex that this scoped mutex is locking.
			Mutex& mutex;
			
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SCOPED_MUTEX_H
