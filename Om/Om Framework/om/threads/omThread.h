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

#ifndef INCLUDE_OM_THREAD_H
#define INCLUDE_OM_THREAD_H


#include "omThreadsConfig.h"


#include "omThreadBase.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that provides a function-based interface for creating threads.
template < typename Signature >
class FunctionThread : public ThreadBase
{
	public:
		
		//********************************************************************************
		//******	Constructors / Destructor
			
			
			/// Create a default thread with no runnable to execute.
			OM_INLINE FunctionThread()
				:	ThreadBase(),
					function()
			{
			}
			
			
			/// Destroy and clean up the thread, stopping it if necessary.
			OM_INLINE ~FunctionThread()
			{
				this->join();
			}
			
			
		//********************************************************************************
		//******	Thread Instance Control
			
			
			/// Start the execution of a thread using the specified function call.
			/** 
			  * This method creates a new thread if the thread is not already running
			  * and calls the specified function call object. When the function call returns,
			  * the thread exits.
			  * 
			  * @param functionCall - a function call that should be executed on the other thread.
			  */
			void start( const lang::FunctionCall<Signature>& functionCall )
			{
				if ( !ThreadBase::isRunning() )
				{
					function = lang::Shared< lang::FunctionCall<Signature> >::construct( functionCall );
					
					ThreadBase::startThread();
				}
			}
			
			
			OM_INLINE lang::Optional<typename lang::FunctionCall<Signature>::ReturnType> join()
			{
				ThreadBase::joinThread();
				
				return returnValue;
			}
			
			
		//********************************************************************************
		//******	Current Thread Control Methods
			
			
			/// Sleep the calling thread for the specified number of milliseconds.
			OM_INLINE static void sleep( int milliseconds )
			{
				ThreadBase::sleep( milliseconds );
			}
			
			
			/// Sleep the calling thread for the specified number of seconds.
			OM_INLINE static void sleep( double seconds )
			{
				ThreadBase::sleep( seconds );
			}
			
			
			/// Relinquish the calling thread's CPU time until it's rescheduled.
			OM_INLINE static void yield()
			{
				ThreadBase::yield();
			}
			
			
			/// Terminate the current calling thread
			OM_INLINE static void exit()
			{
				ThreadBase::exit();
			}
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Type Declarations
			
			
			/// A class that is used to facilitate accessing the return values of function threads.
			template < typename Sig, typename ReturnType >
			class ReturnValueWrapper
			{
				public:
					
					OM_INLINE lang::Optional<ReturnType> operator () ( const lang::Shared< lang::FunctionCall<Sig> >& function )
					{
						return (*function)();
					}
					
			};
			
			
			/// A class specialization that is used to handle void return values of function threads.
			template < typename Sig >
			class ReturnValueWrapper<Sig,void>
			{
				public:
					
					OM_INLINE lang::Optional<void> operator () ( const lang::Shared< lang::FunctionCall<Sig> >& function )
					{
						(*function)();
						
						return lang::Optional<void>();
					}
					
			};
			
			
		//********************************************************************************
		//******	Protected Virtual Run Function
			
			
			/// Start the execution of subclass client code on a new thread.
			virtual void run()
			{
				if ( !function.isNull() )
				{
					ReturnValueWrapper<Signature, typename lang::FunctionCall<Signature>::ReturnType> wrapper;
					
					returnValue = wrapper( function );
				}
			}
			
			
	private:
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The function that is being run by the thread.
			lang::Shared< lang::FunctionCall<Signature> > function;
			
			
			/// An object that stores the optionally-NULL return value for this thread.
			lang::Optional< typename lang::FunctionCall<Signature>::ReturnType > returnValue;
			
			
			
};




typedef FunctionThread<void()> Thread;




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_THREAD_H
