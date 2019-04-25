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

#ifndef INCLUDE_OM_SOUND_SAMPLE_TYPE_H
#define INCLUDE_OM_SOUND_SAMPLE_TYPE_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// An enum wrapper class that specifies the type of a sample of audio data.
/**
  * In addition to providing conversion operator to and from the underlying enum type,
  * the class also provides a way to query the size in bytes of a given sample type,
  * avoiding the need for an external switch statement.
  */
class SampleType
{
	public:
		
		//********************************************************************************
		//******	Sample Type Enum Definition
			
			
			/// The underlying enum type that specifies the type of a sample of audio data.
			enum Enum
			{
				/// An 8-bit signed integer sample, stored in native endian format.
				SAMPLE_8,
				
				/// A 16-bit signed integer sample, stored in native endian format.
				SAMPLE_16,
				
				/// A 24-bit signed integer sample, stored in native endian format.
				SAMPLE_24,
				
				/// A 32-bit signed integer sample, stored in native endian format.
				SAMPLE_32,
				
				/// A 64-bit signed integer sample, stored in native endian format.
				SAMPLE_64,
				
				/// A 32-bit floating point sample, stored in native endian format.
				SAMPLE_32F,
				
				/// A 64-bit floating point sample, stored in native endian format.
				SAMPLE_64F,
				
				/// An undefined/unsupported sample type.
				UNDEFINED
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sample type with the undefined sample type enum value.
			OM_INLINE SampleType()
				:	type( UNDEFINED )
			{
			}
			
			
			/// Create a new sample type with the specified sample type enum value.
			OM_INLINE SampleType( Enum newType )
				:	type( newType )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this sample type to an enum value.
			/**
			  * This operator is provided so that the SampleType object can be used
			  * directly in a switch statement without the need to explicitly access
			  * the underlying enum value.
			  * 
			  * @return the enum representation of this sample type.
			  */
			OM_INLINE operator Enum () const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	Sample Size Accessor Methods
			
			
			/// Get the size in bytes that this sample type occupies.
			OM_INLINE Size getSizeInBytes() const
			{
				switch ( type )
				{
					case SAMPLE_8:		return 1;
					case SAMPLE_16:		return 2;
					case SAMPLE_24:		return 3;
					case SAMPLE_32:
					case SAMPLE_32F:	return 4;
					case SAMPLE_64:
					case SAMPLE_64F:	return 8;
					default:			return 0;
				}
			}
			
			
		//********************************************************************************
		//******	Sample Type Accessor Methods
			
			
			/// Return whether or not this sample type is an integer-based sample type.
			OM_INLINE Bool isIntegral() const
			{
				switch ( type )
				{
					case SAMPLE_8:
					case SAMPLE_16:
					case SAMPLE_24:
					case SAMPLE_32:
					case SAMPLE_64:
						return true;
					default:
						return false;
				}
			}
			
			
			/// Return whether or not this sample type is a floating-point-based sample type.
			OM_INLINE Bool isFloatingPoint() const
			{
				switch ( type )
				{
					case SAMPLE_32F:
					case SAMPLE_64F:
						return true;
					default:
						return false;
				}
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the sample type.
			data::String toString() const;
			
			
			/// Convert this sample type into a string representation.
			OM_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The underlying enum representing the type of sample for this SampleType object.
			Enum type;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SAMPLE_TYPE_H
