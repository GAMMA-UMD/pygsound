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

#ifndef INCLUDE_OM_THREAD_BASE_H
#define INCLUDE_OM_THREAD_BASE_H


#include "omThreadsConfig.h"


#include "../omLanguage.h"
#include "../omUtilities.h"

#include "omThreadPriority.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




typedef PointerInt ThreadID;




//********************************************************************************
/// A class that provides a system-independent abstraction for a thread of execution.
/**
  * This is the base class for a platform-independent thread. Subclasses implement the
  * run() method, providing a better user interface to thread initialization. They can
  * provide more complex functionality (such as a worker thread).
  */
class ThreadBase
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a default thread that is not yet running.
			ThreadBase();
			
			
			/// Create a new thread that is a copy of another thread.
			/**
			  * This merely copies any attributes of the other thread, the
			  * new thread object still refers to a different OS thread than
			  * the copied object.
			  */
			ThreadBase( const ThreadBase& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this thread object.
			/**
			  * This doesn't stop the thread if it is currently running, only destroys any
			  * way of controlling or joining the thread - it is orphaned and will continue
			  * until it returns on its own.
			  */
			virtual ~ThreadBase();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the attributes of one thread to this thread.
			/**
			  * This doesn't stop the thread if it is currently running, only destroys any
			  * way of controlling or joining the thread - it is orphaned and will continue
			  * until it returns on its own. This operator initializes the thread object with
			  * any attributes of the other thread. The newly assigned thread is not running.
			  */
			ThreadBase& operator = ( const ThreadBase& other );
			
			
		//********************************************************************************
		//******	Thread Instance Control
			
			
			/// Forcibly end the execution of a thread.
			/** 
			  * This method forcibly stops the execution of a thread object if it is
			  * currently running. The use of this method is not recommended in general.
			  * Calling this method can result in memory not being correctly freed and
			  * other undefinend behavior.
			  */
			Bool stop();
			
			
			/// Get whether or not a thread is currently running.
			OM_INLINE Bool isRunning() const
			{
				return threadIsRunning;
			}
			
			
			/// Return the ID of this thread.
			ThreadID getID() const;
			
			
		//********************************************************************************
		//******	Thread Priority Accessor Methods
			
			
			/// Return an object describing the current scheduling priority for this thread.
			ThreadPriority getPriority( const ThreadPriority& newPriority );
			
			
			/// Set the scheduling priority for this thread.
			/**
			  * The method returns whether or not the thread's priority was
			  * successfully changed.
			  */
			Bool setPriority( const ThreadPriority& newPriority );
			
			
		//********************************************************************************
		//******	Current Thread Control Methods
			
			
			/// Sleep the calling thread for the specified number of milliseconds.
			/** 
			  * The calling thread yeilds it's position in the operating system's
			  * process queue to another thread and halts execution until the
			  * specified number of milliseconds has elapsed.
			  * 
			  * @param milliseconds - the number of milliseconds until execution will resume.
			  */
			static void sleepMs( UInt milliseconds );
			
			
			/// Sleep this thread for the specified number of seconds
			/** 
			  * The calling thread yeilds it's position in the operating system's
			  * process queue to another thread and halts execution until the
			  * specified number of seconds has elapsed.
			  * 
			  * @param seconds - the number of seconds until execution will resume.
			  */
			static void sleep( double seconds );
			
			/// Relinquish the calling thread's CPU time until is it rescheduled.
			/** 
			  * The calling thread yeilds it's position in the operating system's
			  * process queue to another thread and halts execution until it is
			  * again rescheduled for execution.
			  */
			static void yield();
			
			
			/// Terminate the current calling thread.
			/** 
			  * This method prematurely terminates the calling thread.
			  */
			static void exit();
			
			
			/// Return the ID of the calling thread.
			static ThreadID getCurrentID();
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Virtual Run Function
			
			
			/// Start this thread object's execution.
			/**
			  * The method returns whether or not starting the thread was successful.
			  */
			Bool startThread();
			
			
			/// Wait indefinitely to join a thread when it dies.
			/** 
			  * This method stops the execution of the calling thread indefinitely
			  * until the thread object has finished running. Use this with caution,
			  * as it can cause a program to lock up while it waits for the thread to 
			  * finish.
			  *
			  * The method returns whether or not joining the thread was successful.
			  */
			Bool joinThread();
			
			
			/// Start the execution of subclass client code on a new thread.
			/**
			  * This virtual method is called by the ThreadBase class when a new thread
			  * is started using the startThread() method. A subclass should override this
			  * method and provide its own custom hook into the new thread.
			  */
			virtual void run() = 0;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Functions
			
			
			/// A function called internally by the thread entry point that performs thread setup before calling the run() method.
			void runThread();
			
			
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that holds platform-specific data for this thread object.
			class ThreadWrapper;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A class that wraps platform-specific thread functionality.
			ThreadWrapper* wrapper;
			



			/// A boolean value indicating whether or not the thread is currently running.
			Bool threadIsRunning;
			
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_THREAD_BASE_H
