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

#ifndef INCLUDE_OM_SOUND_FILTER_PARAMETER_TYPE_H
#define INCLUDE_OM_SOUND_FILTER_PARAMETER_TYPE_H


#include "omSoundFiltersConfig.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the actual type of a SoundFilter parameter.
class FilterParameterType
{
	public:
		
		//********************************************************************************
		//******	Parameter Type Enum Declaration
			
			
			/// An enum that specifies the allowed SoundFilter parameter types.
			enum Enum
			{
				/// A undefined filter parameter type.
				UNDEFINED = 0,
				
				/// A filter parameter type which indicates a boolean parameter value.
				BOOLEAN = 1,
				
				/// A filter parameter type which indicates an integral parameter value.
				INTEGER = 2,
				
				/// A filter parameter type that is an enumeration of possible integer values.
				ENUMERATION = 3,
				
				/// A filter parameter type which indicates a single-precision parameter value.
				FLOAT = 4,
				
				/// A filter parameter type which indicates a double-precision parameter value.
				DOUBLE = 5,
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new filter parameter type object with an UNDEFINED parameter type.
			OM_INLINE FilterParameterType()
				:	type( (UByte)UNDEFINED )
			{
			}
			
			
			/// Create a new filter parameter type object with the specified type enum value.
			OM_INLINE FilterParameterType( Enum newType )
				:	type( (UByte)newType )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this filter parameter type to an enum value.
			/**
			  * This operator is provided so that the FilterParameterType object can be used
			  * directly in a switch statement without the need to explicitly access
			  * the underlying enum value.
			  * 
			  * @return the enum representation of this parameter type.
			  */
			OM_INLINE operator Enum () const
			{
				return (Enum)type;
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the parameter type.
			UTF8String toString() const;
			
			
			/// Convert this parameter type into a string representation.
			OM_INLINE operator UTF8String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value indicating the concrete type of a SoundFilter parameter.
			UByte type;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_PARAMETER_TYPE_H
