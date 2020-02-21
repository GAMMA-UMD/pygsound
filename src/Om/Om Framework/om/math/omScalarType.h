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

#ifndef INCLUDE_OM_SCALAR_TYPE_H
#define INCLUDE_OM_SCALAR_TYPE_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../lang/omHalfFloat.h"
#include "omComplex.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a type of scalar math value.
class ScalarType
{
	public:
		
		//********************************************************************************
		//******	Scalar Type Enum Definition
			
			
			/// An enum type used to indicate a primitive built-in type.
			enum Enum
			{
				/// An undefined scalar type.
				UNDEFINED = 0,
				
				/// A scalar type representing the 'bool' type.
				BOOLEAN = 1,
				
				/// A scalar type representing an 8-bit signed integer type.
				INT8 = 2,
				
				/// A scalar type representing an 8-bit unsigned integer type.
				UINT8 = 3,
				
				/// A scalar type representing an 16-bit signed signed integer type.
				INT16 = 4,
				
				/// A scalar type representing an 16-bit unsigned integer type.
				UINT16 = 5,
				
				/// A scalar type representing a 32-bit signed integer type.
				INT32 = 6,
				
				/// A scalar type representing a 32-bit unsigned integer type.
				UINT32 = 7,
				
				/// A scalar type representing a 64-bit signed integer type.
				INT64 = 8,
				
				/// A scalar type representing a 64-bit unsigned integer type.
				UINT64 = 9,
				
				/// A scalar type representing an 16-bit floating-point type.
				FLOAT16 = 10,
				
				/// A scalar type representing a 32-bit floating-point type.
				FLOAT32 = 11,
				
				/// A scalar type representing a 64-bit floating-point type.
				FLOAT64 = 12,
				
				/// A scalar type representing a 16-bit complex floating-point type.
				COMPLEX_FLOAT16 = 13,
				
				/// A scalar type representing a 32-bit complex floating-point type.
				COMPLEX_FLOAT32 = 14,
				
				/// A scalar type representing a 64-bit complex floating-point type.
				COMPLEX_FLOAT64 = 15,
				
				/// The number of scalar type enum values.
				COUNT = 16
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new scalar type with the specified scalar type enum value.
			OM_INLINE ScalarType( Enum newType )
				:	type( (UByte)newType )
			{
			}
			
			
			
		//********************************************************************************
		//******	Scalar Type Static Templated Accessor Method
			
			
			/// Get the scalar type of a templated type.
			/**
			  * If the templated type is not a scalar type, an UNDEFINED
			  * scalar type is returned.
			  */
			template < typename T >
			OM_INLINE static ScalarType of()
			{
				return UNDEFINED;
			}
			
			
		//********************************************************************************
		//******	Scalar Type Checking Method
			
			
			/// Check to see if the templated type is a supported scalar type.
			/**
			  * Calling this empty method will produce a compiler error if the
			  * templated type is not a supported scalar type.
			  */
			template < typename T >
			OM_FORCE_INLINE static void check();
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this scalar type to an enum value.
			OM_INLINE operator Enum () const
			{
				return (Enum)type;
			}
			
			
		//********************************************************************************
		//******	Byte Size Accessor Method
			
			
			/// Return the size of this scalar type in bits.
			OM_INLINE Size getSizeInBits() const
			{
				return this->getSizeInBytes() << 3;
			}
			
			
			/// Return the size of this scalar type in bytes.
			Size getSizeInBytes() const;
			
			
		//********************************************************************************
		//******	Attribute Accessor Methods
			
			
			/// Return whether or not this scalar type can represent negative numbers.
			Bool isSigned() const;
			
			
			/// Return whether or not this scalar type is an integer type.
			Bool isInteger() const;
			
			
			/// Return whether or not this scalar type is a floating point type.
			Bool isFloatingPoint() const;
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the scalar type.
			data::String toString() const;
			
			
			/// Convert this scalar type into a string representation.
			OM_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum for the scalar type.
			UByte type;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Scalar Type Accessor Method Specializations
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_INLINE ScalarType ScalarType:: of<Bool>()
{
	return BOOLEAN;
}




template <>
OM_INLINE ScalarType ScalarType:: of<Int8>()
{
	return INT8;
}




template <>
OM_INLINE ScalarType ScalarType:: of<UInt8>()
{
	return UINT8;
}




template <>
OM_INLINE ScalarType ScalarType:: of<Int16>()
{
	return INT16;
}




template <>
OM_INLINE ScalarType ScalarType:: of<UInt16>()
{
	return UINT16;
}




template <>
OM_INLINE ScalarType ScalarType:: of<Int32>()
{
	return INT32;
}




template <>
OM_INLINE ScalarType ScalarType:: of<UInt32>()
{
	return UINT32;
}




template <>
OM_INLINE ScalarType ScalarType:: of<Int64>()
{
	return INT64;
}




template <>
OM_INLINE ScalarType ScalarType:: of<UInt64>()
{
	return UINT64;
}




template <>
OM_INLINE ScalarType ScalarType:: of<lang::Float16>()
{
	return FLOAT16;
}




template <>
OM_INLINE ScalarType ScalarType:: of<Float32>()
{
	return FLOAT32;
}




template <>
OM_INLINE ScalarType ScalarType:: of<Float64>()
{
	return FLOAT64;
}




template <>
OM_INLINE ScalarType ScalarType:: of< Complex<Float32> >()
{
	return COMPLEX_FLOAT32;
}




template <>
OM_INLINE ScalarType ScalarType:: of< Complex<Float64> >()
{
	return COMPLEX_FLOAT64;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Scalar Type Checking Method Specializations
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_INLINE void ScalarType:: check<Bool>()
{
}




template <>
OM_INLINE void ScalarType:: check<Int8>()
{
}




template <>
OM_INLINE void ScalarType:: check<UInt8>()
{
}




template <>
OM_INLINE void ScalarType:: check<Int16>()
{
}




template <>
OM_INLINE void ScalarType:: check<UInt16>()
{
}




template <>
OM_INLINE void ScalarType:: check<Int32>()
{
}




template <>
OM_INLINE void ScalarType:: check<UInt32>()
{
}




template <>
OM_INLINE void ScalarType:: check<Int64>()
{
}




template <>
OM_INLINE void ScalarType:: check<UInt64>()
{
}




template <>
OM_INLINE void ScalarType:: check<lang::Float16>()
{
}




template <>
OM_INLINE void ScalarType:: check<Float32>()
{
}




template <>
OM_INLINE void ScalarType:: check<Float64>()
{
}




template <>
OM_INLINE void ScalarType:: check< Complex<Float32> >()
{
}




template <>
OM_INLINE void ScalarType:: check< Complex<Float64> >()
{
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SCALAR_TYPE_H
