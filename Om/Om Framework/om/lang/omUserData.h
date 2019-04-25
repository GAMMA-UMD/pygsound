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

#ifndef INCLUDE_OM_USER_DATA_H
#define INCLUDE_OM_USER_DATA_H


#include "omLanguageConfig.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that wraps opaque client data that represents a pointer or an integer.
/**
  * Client-specific data is often needed by the user of a complex system that
  * requires callbacks in order to identify an object or provide an opaque data source.
  * This class provides a uniform mechanism for storing either an opaque pointer or
  * integer to client data. The class stores at least 64 bits of information.
  */
class UserData
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a user data object that is NULL.
			OM_INLINE UserData()
				:	uint64( 0 )
			{
			}
			
			
			/// Create a user data object that refers to the specified index.
			OM_INLINE UserData( Index newIndex )
				:	uint64( 0 )
			{
				index = newIndex;
			}
			
			
			/// Create a user data object that refers to the specified pointer.
			OM_INLINE UserData( void* newPointer )
				:	uint64( 0 )
			{
				pointer = newPointer;
			}
			
			
			/// A union that contains the various possible representations of the user data.
			union
			{
				Int8 int8;
				UInt8 uint8;
				Int16 int16;
				UInt16 uint16;
				Int32 int32;
				UInt32 uint32;
				Int64 int64;
				UInt64 uint64;
				Float32 float32;
				Float64 float64;
				Index index;
				Size size;
				void* pointer;
			};
			
			
			
};




//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_USER_DATA_H
