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

#ifndef INCLUDE_OM_SIGNAL_H
#define INCLUDE_OM_SIGNAL_H


#include "omThreadsConfig.h"


#include "../omUtilities.h"
#include "../omTime.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a thread-to-thread condition signaling event.
/**
  * This class allows a thread to wait for a condition to be met. This will
  * halt the execution of that thread until another thread signals that the condition
  * is met. At that point the waiting thread wakes up and begins execution
  * again. A typical application of this class would be in produce-consumer systems
  * where one thread is waiting for data from another thread.
  */
class Signal
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new signal object.
			Signal();
			
			
			/// Create a copy of a signal object.
			/**
			  * This has the effect of linking the two signal objects - 
			  * whenever one is signaled, all listeners for the other
			  * receive the signal.
			  */
			Signal( const Signal& signal );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a signal object.
			/**
			  * This has the effect of signaling all listening
			  * threads that the signal has been sent. This is
			  * done in order to prevent deadlocks.
			  */
			~Signal();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign one signal to another.
			/**
			  * This has the effect of linking the two signal objects - 
			  * whenever one is signaled, all listeners for the other
			  * receive the signal.
			  */
			Signal& operator = ( const Signal& signal );
			
			
		//********************************************************************************
		//******	Signal Methods
			
			
			/// Send a signal to all listening threads.
			Bool signal();
			
			
			/// Send a signal to just one of the listening threads.
			Bool signalOne();
			
			
			/// Wait until the signal is given by another thread.
			/**
			  * This method blocks the calling thread until another
			  * thread calls the signal() method on this object.
			  */
			Bool wait() const;
			
			
			/// Wait until the signal is given by another thread or a time period elapses.
			/**
			  * This method blocks the calling thread until another
			  * thread calls the signal() method on this object, or the specified timeout
			  * period elapses.
			  *
			  * The return value is TRUE if the thread was signaled, or FALSE if the timeout expired.
			  */
			Bool wait( const time::Time& maxWaitTime ) const;
			
			
			/// Lock the mutex associated with this signal.
			Bool lock();
			
			
			/// Unlock this mutex associated with this signal.
			Bool unlock();
			
			
	private:
		
		//********************************************************************************
		//******	Private Signal Wrapper Class Declaration
			
			
			/// A class that encapsulates internal platform-specific Signal code.
			class SignalWrapper;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A wrapper of the internal platform-specific state of the signal.
			SignalWrapper* wrapper;
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIGNAL_H
