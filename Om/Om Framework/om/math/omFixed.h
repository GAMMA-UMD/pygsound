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

#ifndef INCLUDE_OM_FIXED_H
#define INCLUDE_OM_FIXED_H


#include "omMathConfig.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a fixed-point number.
template < typename BaseType, Size fractionalBits >
class Fixed
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			OM_FORCE_INLINE Fixed()
				:	value( BaseType(0) )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( float a )
				:	value( BaseType(a*FLOAT_SHIFT) )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( double a )
				:	value( BaseType(a*DOUBLE_SHIFT) )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( int a )
				:	value( BaseType(a) << fractionalBits )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( unsigned int a )
				:	value( BaseType(a) << fractionalBits )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( long a )
				:	value( BaseType(a) << fractionalBits )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( unsigned long a )
				:	value( BaseType(a) << fractionalBits )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( long long a )
				:	value( BaseType(a) << fractionalBits )
			{
			}
			
			
			OM_FORCE_INLINE Fixed( unsigned long long a )
				:	value( BaseType(a) << fractionalBits )
			{
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed& operator = ( T a )
			{
				value = Fixed(a).value;
				return *this;
			}
			
			
		//********************************************************************************
		//******	Type Conversion Operators
			
			
			OM_FORCE_INLINE operator float () const
			{
				return float(value) * INVERSE_FLOAT_SHIFT;
			}
			
			
			OM_FORCE_INLINE operator double () const
			{
				return double(value) * INVERSE_DOUBLE_SHIFT;
			}
			
			
			OM_FORCE_INLINE operator int () const
			{
				return value >> fractionalBits;
			}
			
			
			OM_FORCE_INLINE operator unsigned int () const
			{
				return value >> fractionalBits;
			}
			
			
			OM_FORCE_INLINE operator long () const
			{
				return value >> fractionalBits;
			}
			
			
			OM_FORCE_INLINE operator unsigned long () const
			{
				return value >> fractionalBits;
			}
			
			
			OM_FORCE_INLINE operator long long () const
			{
				return value >> fractionalBits;
			}
			
			
			OM_FORCE_INLINE operator unsigned long long () const
			{
				return value >> fractionalBits;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			OM_FORCE_INLINE Bool operator == ( const Fixed& other ) const
			{
				return value == other.value;
			}
			
			
			OM_FORCE_INLINE Bool operator != ( const Fixed& other ) const
			{
				return value != other.value;
			}
			
			
			OM_FORCE_INLINE Bool operator <= ( const Fixed& other ) const
			{
				return value <= other.value;
			}
			
			
			OM_FORCE_INLINE Bool operator >= ( const Fixed& other ) const
			{
				return value >= other.value;
			}
			
			
			OM_FORCE_INLINE Bool operator < ( const Fixed& other ) const
			{
				return value < other.value;
			}
			
			
			OM_FORCE_INLINE Bool operator > ( const Fixed& other ) const
			{
				return value > other.value;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Bool operator == ( T other ) const
			{
				return value == Fixed(other).value;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Bool operator != ( T other ) const
			{
				return value != Fixed(other).value;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Bool operator <= ( T other ) const
			{
				return value <= Fixed(other).value;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Bool operator >= ( T other ) const
			{
				return value >= Fixed(other).value;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Bool operator < ( T other ) const
			{
				return value < Fixed(other).value;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Bool operator > ( T other ) const
			{
				return value > Fixed(other).value;
			}
			
			
		//********************************************************************************
		//******	Sign Change Operators
			
			
			OM_FORCE_INLINE Fixed operator + () const
			{
				return Fixed( value, 0 );
			}
			
			
			OM_FORCE_INLINE Fixed operator - () const
			{ 
				return Fixed( -value, 0 );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			OM_FORCE_INLINE Fixed operator + ( const Fixed& other ) const
			{
				return Fixed( value + other.value, 0 );
			}
			
			
			OM_FORCE_INLINE Fixed operator - ( const Fixed& other ) const
			{
				return Fixed( value - other.value, 0 );
			}
			
			
			OM_FORCE_INLINE Fixed operator * ( const Fixed& other ) const
			{
				return Fixed( (value * other.value) >> fractionalBits, 0 );
			}
			
			
			OM_FORCE_INLINE Fixed operator / ( const Fixed& other ) const
			{
				return Fixed( BaseType((double(value) / double(other.value))*DOUBLE_SHIFT), 0 );
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed operator + ( T value ) const
			{
				return (*this) + Fixed(value);
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed operator - ( T value ) const
			{
				return (*this) - Fixed(value);
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed operator * ( T value ) const
			{
				return (*this) * Fixed(value);
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed operator / ( T value ) const
			{
				return (*this) / Fixed(value);
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			OM_FORCE_INLINE Fixed& operator += ( const Fixed& other ) const
			{
				value += other.value;
				
				return *this;
			}
			
			
			OM_FORCE_INLINE Fixed& operator -= ( const Fixed& other ) const
			{
				value -= other.value;
				
				return *this;
			}
			
			
			OM_FORCE_INLINE Fixed& operator *= ( const Fixed& other ) const
			{
				value = (value * other.value) >> fractionalBits;
				
				return *this;
			}
			
			
			OM_FORCE_INLINE Fixed& operator /= ( const Fixed& other ) const
			{
				value = BaseType((double(value) / double(other.value))*DOUBLE_SHIFT);
				
				return *this;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed& operator += ( T value ) const
			{
				value = ((*this) + Fixed(value)).value;
				
				return *this;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed& operator -= ( T value ) const
			{
				value = ((*this) - Fixed(value)).value;
				
				return *this;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed& operator *= ( T value ) const
			{
				value = ((*this) * Fixed(value)).value;
				
				return *this;
			}
			
			
			template < typename T >
			OM_FORCE_INLINE Fixed& operator /= ( T value ) const
			{
				value = ((*this) / Fixed(value)).value;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this vector into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				return data::String( (Double)(*this) );
			}
			
			
			/// Convert this vector into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructor
			
			
			OM_FORCE_INLINE Fixed( BaseType newValue, int )
				:	value( newValue )
			{
			}
			
			
		//********************************************************************************
		//******	Private Static Constants
			
			
			static const Size fractionalBits2 = fractionalBits*2;
			
			static const BaseType SHIFT = BaseType(1) << fractionalBits;
			
			static const float FLOAT_SHIFT;
			
			static const double DOUBLE_SHIFT;
			
			static const float INVERSE_FLOAT_SHIFT;
			
			static const double INVERSE_DOUBLE_SHIFT;
			
			
		//********************************************************************************
		//******	Private Data Member
			
			
			/// The underlying value that stores the fixed point number.
			BaseType value;
			
			
};




template < typename BaseType, Size fractionalBits >
const float Fixed<BaseType,fractionalBits>:: FLOAT_SHIFT = float(SHIFT);




template < typename BaseType, Size fractionalBits >
const double Fixed<BaseType,fractionalBits>:: DOUBLE_SHIFT = double(SHIFT);




template < typename BaseType, Size fractionalBits >
const float Fixed<BaseType,fractionalBits>:: INVERSE_FLOAT_SHIFT = 1.0f/FLOAT_SHIFT;




template < typename BaseType, Size fractionalBits >
const double Fixed<BaseType,fractionalBits>:: INVERSE_DOUBLE_SHIFT = 1.0/DOUBLE_SHIFT;




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Comparison Operators
//############		
//##########################################################################################
//##########################################################################################




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Bool operator == ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) == b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Bool operator != ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) != b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Bool operator <= ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) <= b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Bool operator >= ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) >= b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Bool operator < ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) < b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Bool operator > ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) > b;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Fixed<BaseType,fractionalBits> operator + ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) + b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Fixed<BaseType,fractionalBits> operator - ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) - b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Fixed<BaseType,fractionalBits> operator * ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) * b;
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE Fixed<BaseType,fractionalBits> operator / ( T a, const Fixed<BaseType,fractionalBits>& b )
{
	return Fixed<BaseType,fractionalBits>(a) / b;
}



//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Assignment Operators
//############		
//##########################################################################################
//##########################################################################################




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE T& operator += ( T& a, const Fixed<BaseType,fractionalBits>& b )
{
	return a = (T)(Fixed<BaseType,fractionalBits>(a) + b);
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE T& operator -= ( T& a, const Fixed<BaseType,fractionalBits>& b )
{
	return a = (T)(Fixed<BaseType,fractionalBits>(a) - b);
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE T& operator *= ( T& a, const Fixed<BaseType,fractionalBits>& b )
{
	return a = (T)(Fixed<BaseType,fractionalBits>(a) * b);
}




template < typename T, typename BaseType, Size fractionalBits >
OM_FORCE_INLINE T& operator /= ( T& a, const Fixed<BaseType,fractionalBits>& b )
{
	return a = (T)(Fixed<BaseType,fractionalBits>(a) / b);
}



//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_FIXED_H
