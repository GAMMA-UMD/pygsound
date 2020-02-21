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

#ifndef INCLUDE_OM_THREAD_PRIORITY_H
#define INCLUDE_OM_THREAD_PRIORITY_H


#include "omThreadsConfig.h"


#include "../omData.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum class that specifies the different execution priorities that a thread can have.
class ThreadPriority
{
	public:
		
		//********************************************************************************
		//******	Thread Priority Enum Definition
			
			
			/// An enum type which represents the types of thread scheduling priority.
			enum Enum
			{
				/// The default thread priority.
				DEFAULT,
				
				/// A thread priority for processes that don't need to be run often.
				LOW,
				
				/// A thread priority for processes with priority between low and high.
				MEDIUM,
				
				/// A thread priority for processes that need to be run often.
				HIGH,
				
				/// A thread priority used for time-critical processing with strict real-time requirements.
				TIME_CRITICAL
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new thread priority with the DEFAULT thread priority.
			OM_INLINE ThreadPriority()
				:	type( DEFAULT )
			{
			}
			
			
			/// Create a new thread priority with the specified thread priority enum value.
			OM_INLINE ThreadPriority( Enum newType )
				:	type( newType )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this thread priority type to an enum value.
			OM_INLINE operator Enum () const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the thread priority.
			data::String toString() const;
			
			
			/// Convert this thread priority into a string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value that indicates the type of thread priority.
			Enum type;
			
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_THREAD_PRIORITY_H
