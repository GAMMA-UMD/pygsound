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

#ifndef INCLUDE_OM_COMPLEX_H
#define INCLUDE_OM_COMPLEX_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a complex number.
/**
  * Overloaded arithmetic operators are provided for all basic arithmetic operations
  * (+, -, *, /, +=, -=, *=, /=). The class allows any underlying type to be used
  * as the coefficients of the r and i components of the complex number
  * via a template parameter.
  */
template < typename T >
class Complex
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a complex number with both r and i parts equal to zero.
			OM_FORCE_INLINE Complex()
				:	r( 0 ),
					i( 0 )
			{
			}
			
			/// Create a complex number with the specified r component and zero i component.
			OM_FORCE_INLINE Complex( T newReal )
				:	r( newReal ),
					i( 0 )
			{
			}
			
			/// Create a complex number with the specified r and i components.
			OM_FORCE_INLINE Complex( T newReal, T newImaginary )
				:	r( newReal ),
					i( newImaginary )
			{
			}
			
			
			/// Create a complex number from another with different template parameter type.
			template < typename U >
			OM_FORCE_INLINE Complex( const Complex<U>& other )
				:	r( other.r ),
					i( other.i )
			{
			}
			
			
		//********************************************************************************
		//******	Complex Number Operations
			
			
			/// Return a complex number for the specified magnitude and phase angle in radians.
			OM_FORCE_INLINE static Complex<T> polar( T magnitude, T phase )
			{
				return Complex<T>( magnitude*math::cos( phase ), magnitude*math::sin( phase ) );
			}
			
			
		//********************************************************************************
		//******	Complex Number Operations
			
			
			/// Return the conjugate of this complex number.
			OM_FORCE_INLINE Complex conjugate() const
			{
				return Complex( r, -i );
			}
			
			
			/// Return the magnitude of this complex number.
			OM_FORCE_INLINE T getMagnitude() const
			{
				return math::sqrt( r*r + i*i );
			}
			
			
			/// Return the phase of this complex number.
			OM_FORCE_INLINE T getPhase() const
			{
				return math::atan2( i, r );
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this complex number is equal to another.
			OM_FORCE_INLINE Bool operator == ( const Complex& other ) const
			{
				return r == other.r && i == other.i;
			}
			
			
			/// Return whether or not this complex number is equal to a r number.
			OM_FORCE_INLINE Bool operator == ( T value ) const
			{
				return r == value && i == T(0);
			}
			
			
			/// Return whether or not this complex number is not equal to another.
			OM_FORCE_INLINE Bool operator != ( const Complex& other ) const
			{
				return r != other.r || i != other.i;
			}
			
			
			/// Return whether or not this complex number is not equal to a r number.
			OM_FORCE_INLINE Bool operator != ( T value ) const
			{
				return r != value || i != T(0);
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Return the negation of this complex number.
			OM_FORCE_INLINE Complex operator - () const
			{
				return Complex( -r, -i );
			}
			
			
			/// Return the positivation of this complex number.
			OM_FORCE_INLINE Complex operator + () const
			{
				return Complex( r, i );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Return the sum of this complex number with a r number.
			OM_FORCE_INLINE Complex operator + ( T value ) const
			{
				return Complex( r + value, i );
			}
			
			
			/// Return the difference between this complex number a r number.
			OM_FORCE_INLINE Complex operator - ( T value ) const
			{
				return Complex( r - value, i );
			}
			
			
			/// Multiply this complex number by r number and return the resulting complex number.
			OM_FORCE_INLINE Complex operator * ( T value ) const
			{
				return Complex( r*value, i*value );
			}
			
			
			/// Divide this complex number by another and return the resulting complex number.
			OM_FORCE_INLINE Complex operator / ( T value ) const
			{
				T inverseValue = T(1)/value;
				
				return Complex( r*inverseValue, i*inverseValue );
			}
			
			
			/// Return the sum of this complex number with another.
			OM_FORCE_INLINE Complex operator + ( const Complex& other ) const
			{
				return Complex( r + other.r, i + other.i );
			}
			
			
			/// Return the difference between this complex number and other.
			OM_FORCE_INLINE Complex operator - ( const Complex& other ) const
			{
				return Complex( r - other.r, i - other.i );
			}
			
			
			/// Multiply this complex number with another and return the resulting complex number.
			OM_FORCE_INLINE Complex operator * ( const Complex& other ) const
			{
				return Complex( r*other.r - i*other.i,
								i*other.r + r*other.i );
			}
			
			
			/// Divide this complex number by another and return the resulting complex number.
			OM_FORCE_INLINE Complex operator / ( const Complex& other ) const
			{
				T inverseDenominator = T(1)/(other.r*other.r + other.i*other.i);
				
				return Complex( (r*other.r - i*other.i)*inverseDenominator,
								(i*other.r + r*other.i)*inverseDenominator );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a r number to this complex number, modifying it.
			OM_FORCE_INLINE Complex& operator += ( T value )
			{
				r += value;
				
				return *this;
			}
			
			
			/// Subtract a r number from this complex number, modifying it.
			OM_FORCE_INLINE Complex& operator -= ( T value )
			{
				r -= value;
				
				return *this;
			}
			
			
			/// Multiply a r number with this complex number, modifying it.
			OM_FORCE_INLINE Complex& operator *= ( T value )
			{
				r *= value;
				i *= value;
				
				return *this;
			}
			
			
			/// Divide this complex number by a r number, modifying it.
			OM_FORCE_INLINE Complex& operator /= ( T value )
			{
				T inverseValue = T(1)/value;
				
				r *= inverseValue;
				i *= inverseValue;
				
				return *this;
			}
			
			
			/// Add a complex number to this complex number, modifying it.
			OM_FORCE_INLINE Complex& operator += ( const Complex& other )
			{
				r += other.r;
				i += other.i;
				
				return *this;
			}
			
			
			/// Subtract a complex number from this complex number, modifying it.
			OM_FORCE_INLINE Complex& operator -= ( const Complex& other )
			{
				r -= other.r;
				i -= other.i;
				
				return *this;
			}
			
			
			/// Multiply a complex number with this complex number, modifying it.
			OM_FORCE_INLINE Complex& operator *= ( const Complex& other )
			{
				T rTemp = r*other.r - i*other.i;
				i = i*other.r + r*other.i;
				r = rTemp;
				
				return *this;
			}
			
			
			/// Divide this complex number by another complex number, modifying it.
			OM_FORCE_INLINE Complex& operator /= ( const Complex& other )
			{
				T inverseDenominator = T(1)/(other.r*other.r + other.i*other.i);
				
				T rTemp = (r*other.r - i*other.i)*inverseDenominator;
				i = (i*other.r + r*other.i)*inverseDenominator;
				r = rTemp;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this complex number into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << r;
				
				if ( i < T(0) )
					buffer << " - " << math::abs(i) << 'i';
				else if ( i > T(0) )
					buffer << " + " << math::abs(i) << 'i';
				
				return buffer.toString();
			}
			
			
			/// Convert this complex number into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			/*
			/// The real portion of the complex number.
			union
			{
				T real;
				T r;
			};
			
			/// The imaginary portion of the complex number.
			union
			{
				T imaginary;
				T imag;
				T i;
			};
			*/
			
			/// The real portion of the complex number.
			T r;
			
			
			/// The imaginary portion of the complex number.
			T i;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Comparison Operators
//############		
//##########################################################################################
//##########################################################################################




/// Return whether or not a r number is equal to a complex number.
template < typename T >
OM_FORCE_INLINE Bool operator == ( T value, const Complex<T>& complex )
{
	return complex == value;
}




/// Return whether or not a r number is not equal to a complex number.
template < typename T >
OM_FORCE_INLINE Bool operator != ( T value, const Complex<T>& complex )
{
	return complex != value;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a complex number to a r number and return the resulting complex number.
template < typename T >
OM_FORCE_INLINE Complex<T> operator + ( T value, const Complex<T>& complex )
{
	return Complex<T>( value + complex.r, complex.i );
}




/// Subtract a complex number from a r number and return the resulting complex number.
template < typename T >
OM_FORCE_INLINE Complex<T> operator - ( T value, const Complex<T>& complex )
{
	return Complex<T>( value - complex.r, -complex.i );
}




/// Multiply a r number by a complex number and return the resulting complex number.
template < typename T >
OM_FORCE_INLINE Complex<T> operator * ( T value, const Complex<T>& complex )
{
	return Complex<T>( value*complex.r, value*complex.i );
}




/// Divide a r number by a complex number and return the resulting complex number.
template < typename T >
OM_FORCE_INLINE Complex<T> operator / ( T value, const Complex<T>& complex )
{
	return Complex<T>( value ) / complex;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Other Complex Number Functions
//############		
//##########################################################################################
//##########################################################################################




/// Return the absolute value (magnitude) of the specified complex number.
template < typename T >
OM_FORCE_INLINE T abs( const Complex<T>& complex )
{
	return math::sqrt( complex.r*complex.r + complex.i*complex.i );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_COMPLEX_H
