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

#ifndef INCLUDE_OM_SIMD_ARRAY_INT_32_H
#define INCLUDE_OM_SIMD_ARRAY_INT_32_H


#include "omMathConfig.h"


#include "omSIMDConfig.h"
#include "omSIMDScalar.h"
#include "omSIMDScalarInt32_4.h"
#include "omSIMDArray.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing an N-component 32-bit signed-integer SIMD array.
/**
  * This specialization of the SIMDScalar class uses one or more 4-component
  * SIMD values to simulate an N-wide SIMD register.
  */
template < Size width >
class OM_ALIGN(16) SIMDArray<Int32,width>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new SIMD array with all elements left uninitialized.
			OM_FORCE_INLINE SIMDArray()
			{
			}
			
			
			/// Create a new SIMD array with all elements equal to the specified value.
			OM_FORCE_INLINE SIMDArray( Int32 value )
			{
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					v[i] = simdValue;
			}
			
			
			/// Create a new SIMD array from the first N values stored at specified pointer's location.
			OM_FORCE_INLINE SIMDArray( const Int32* array )
			{
				for ( Index i = 0; i < numIterations; i++ )
				{
					v[i] = SIMDBaseType( array );
					array += SIMD_WIDTH;
				}
			}
			
			
		//********************************************************************************
		//******	Load Methods
			
			
			/// Load a SIMD array from the specified aligned pointer to values.
			OM_FORCE_INLINE static SIMDArray load( const Int32* array )
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
				{
					result.v[i] = SIMDBaseType::load( array );
					array += SIMD_WIDTH;
				}
				
				return result;
			}
			
			
			/// Load a SIMD array from the specified unaligned pointer to values.
			OM_FORCE_INLINE static SIMDArray loadUnaligned( const Int32* array )
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
				{
					result.v[i] = SIMDBaseType::loadUnaligned( array );
					array += SIMD_WIDTH;
				}
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Store Method
			
			
			/// Store this SIMD array starting at the specified destination pointer.
			OM_FORCE_INLINE void store( Int32* destination ) const
			{
				for ( Index i = 0; i < numIterations; i++ )
				{
					v[i].store( destination );
					destination += SIMD_WIDTH;
				}
			}
			
			
			/// Store this SIMD array starting at the specified unaligned destination pointer.
			OM_FORCE_INLINE void storeUnaligned( Int32* destination ) const
			{
				for ( Index i = 0; i < numIterations; i++ )
				{
					v[i].storeUnaligned( destination );
					destination += SIMD_WIDTH;
				}
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Get a reference to the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int32& operator [] ( Index i )
			{
				return ((Int32*)v)[i];
			}
			
			
			/// Get the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int32 operator [] ( Index i ) const
			{
				return ((const Int32*)v)[i];
			}
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			OM_FORCE_INLINE const Int32* toArray() const
			{
				return (const Int32*)v;
			}
			
			
		//********************************************************************************
		//******	Mask Operator
			
			
			/// Return whether or not any component of this array has the high-order bit set.
			OM_FORCE_INLINE operator Bool () const
			{
				SIMDBaseType result = v[0];
				
				for ( Index i = 1; i < numIterations; i++ )
					result |= v[i];
				
				return result.getMask() != 0;
			}
			
			
			/// Return whether or not any component of this array has the high-order bit set.
			OM_FORCE_INLINE Bool testMaskAny() const
			{
				SIMDBaseType result = v[0];
				
				for ( Index i = 1; i < numIterations; i++ )
					result |= v[i];
				
				return result.getMask() != 0;
			}
			
			
			/// Return whether or not all components of this array have the high-order bit set.
			OM_FORCE_INLINE Bool testMaskAll() const
			{
				SIMDBaseType result = v[0];
				
				for ( Index i = 1; i < numIterations; i++ )
					result &= v[i];
				
				return result.getMask() == 0xF;
			}
			
			
		//********************************************************************************
		//******	Logical Operators
			
			
			/// Return the bitwise NOT of this 4D SIMD vector.
			OM_FORCE_INLINE SIMDArray operator ~ () const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = ~v[i];
				
				return result;
			}
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDArray operator & ( const SIMDArray& scalar ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] & scalar.v[i];
				
				return result;
			}
			
			
			/// Compute the bitwise OR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDArray operator | ( const SIMDArray& scalar ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] | scalar.v[i];
				
				return result;
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDArray operator ^ ( const SIMDArray& scalar ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] ^ scalar.v[i];
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDArray& operator &= ( const SIMDArray& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] &= scalar.v[i];
				
				return *this;
			}
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDArray& operator |= ( const SIMDArray& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] |= scalar.v[i];
				
				return *this;
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDArray& operator ^= ( const SIMDArray& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] ^= scalar.v[i];
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two SIMD arrays component-wise for equality.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator == ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == scalar.v[i];
				
				return result;
			}
			
			
			/// Compare this scalar to a single value for equality.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator == ( const Int32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == simdValue;
				
				return result;
			}
			
			
			/// Compare two SIMD arrays component-wise for inequality.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator != ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != scalar.v[i];
				
				return result;
			}
			
			
			/// Compare this scalar to a single floating point value for inequality.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator != ( const Int32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise less-than comparison between this an another SIMD array.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator < ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] < scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise less-than comparison between this SIMD array and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator < ( const Int32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] < simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than comparison between this an another SIMD array.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator > ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] > scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than comparison between this SIMD array and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator > ( const Int32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] > simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another SIMD array.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator <= ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] <= scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this SIMD array and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator <= ( const Int32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] <= simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another SIMD array.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator >= ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >= scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this SIMD array and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator >= ( const Int32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >= simdValue;
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Shifting Operators
			
			
			/// Shift each component of the SIMD array to the left by the specified amount of bits.
			OM_FORCE_INLINE SIMDArray operator << ( Int bitShift ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] << bitShift;
				
				return result;
			}
			
			
			/// Shift each component of the SIMD array to the right by the specified amount of bits.
			OM_FORCE_INLINE SIMDArray operator >> ( Int bitShift ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >> bitShift;
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate a scalar.
			OM_FORCE_INLINE SIMDArray operator - () const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = -v[i];
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this scalar to another and return the result.
			OM_FORCE_INLINE SIMDArray operator + ( const SIMDArray& scalar ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] + scalar.v[i];
				
				return result;
			}
			
			
			/// Add a value to every component of this scalar.
			OM_FORCE_INLINE SIMDArray operator + ( const Int32 value ) const
			{
				SIMDArray result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] + simdValue;
				
				return result;
			}
			
			
			/// Subtract a scalar from this scalar component-wise and return the result.
			OM_FORCE_INLINE SIMDArray operator - ( const SIMDArray& scalar ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] - scalar.v[i];
				
				return result;
			}
			
			
			/// Subtract a value from every component of this scalar.
			OM_FORCE_INLINE SIMDArray operator - ( const Int32 value ) const
			{
				SIMDArray result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] - simdValue;
				
				return result;
			}
			
			
			/// Multiply component-wise this scalar and another scalar.
			OM_FORCE_INLINE SIMDArray operator * ( const SIMDArray& scalar ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] * scalar.v[i];
				
				return result;
			}
			
			
			/// Multiply every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDArray operator * ( const Int32 value ) const
			{
				SIMDArray result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] * simdValue;
				
				return result;
			}
			
			
			/// Divide this scalar by another scalar component-wise.
			OM_FORCE_INLINE SIMDArray operator / ( const SIMDArray& scalar ) const
			{
				SIMDArray result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] / scalar.v[i];
				
				return result;
			}
			
			
			/// Divide every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDArray operator / ( const Int32 value ) const
			{
				SIMDArray result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] / simdValue;
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDArray& operator += ( const SIMDArray& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] += scalar.v[i];
				
				return *this;
			}
			
			/// Subtract a scalar from this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDArray& operator -= ( const SIMDArray& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] -= scalar.v[i];
				
				return *this;
			}
			
			
			/// Multiply component-wise this scalar and another scalar and modify this scalar.
			OM_FORCE_INLINE SIMDArray& operator *= ( const SIMDArray& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] *= scalar.v[i];
				
				return *this;
			}
			
			
			/// Divide this scalar by another scalar component-wise and modify this scalar.
			OM_FORCE_INLINE SIMDArray& operator /= ( const SIMDArray& scalar )
			{
				for ( Index i = 0; i < numIterations; i++ )
					v[i] /= scalar.v[i];
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Required Alignment Accessor Methods
			
			
			/// Return the alignment required for objects of this type.
			OM_FORCE_INLINE static Size getAlignment()
			{
				return SIMDBaseType::getAlignment();
			}
			
			
			/// Get the width of this scalar (number of components it has).
			OM_FORCE_INLINE static Size getWidth()
			{
				return width;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this SIMD array into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				const Int32* array = this->toArray();
				data::StringBuffer buffer;
				
				buffer << "(";
				
				for ( Index i = 0; i < width - 1; i++ )
					buffer << array[i] << ", ";
				
				buffer << array[width-1] << ")";
				
				return buffer.toString();
			}
			
			
			/// Convert this SIMD array into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The width of the underlying SIMD type used.
			static const Size SIMD_WIDTH = SIMDType<Int32>::WIDTH;
			
			
			/// The underlying SIMD type used to implement this class.
			typedef SIMDScalar<Int32,SIMD_WIDTH> SIMDBaseType;
			
			
			/// The number of SIMD processing iterations that must occur.
			static const Size numIterations = SIMD_WIDTH*(width / SIMD_WIDTH) == width ? 
												width / SIMD_WIDTH : width / SIMD_WIDTH + 1;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of SIMD values that simulate an N-wide SIMD register.
			SIMDBaseType v[numIterations];
			
			
		//********************************************************************************
		//******	Private Friend Declarations
			
			
			template < typename T, Size dimension >
			friend class SIMDArray;
			
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Int32,width2> abs( const SIMDArray<Int32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Int32,width2> sqrt( const SIMDArray<Int32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Int32,width2> min( const SIMDArray<Int32,width2>& scalar1, const SIMDArray<Int32,width2>& scalar2 );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Int32,width2> max( const SIMDArray<Int32,width2>& scalar1, const SIMDArray<Int32,width2>& scalar2 );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Int32,width2> select( const SIMDArray<Int32,width2>& selector,
															const SIMDArray<Int32,width2>& scalar1, const SIMDArray<Int32,width2>& scalar2 );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> select( const SIMDArray<Int32,width2>& selector,
															const SIMDArray<Float32,width2>& scalar1, const SIMDArray<Float32,width2>& scalar2 );
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Associative SIMD Scalar Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a scalar value to each component of this scalar and return the resulting scalar.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> operator + ( const Int32 value, const SIMDArray<Int32,width>& scalar )
{
	return SIMDArray<Int32,width>(value) + scalar;
}




/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> operator - ( const Int32 value, const SIMDArray<Int32,width>& scalar )
{
	return SIMDArray<Int32,width>(value) - scalar;
}




/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> operator * ( const Int32 value, const SIMDArray<Int32,width>& scalar )
{
	return SIMDArray<Int32,width>(value) * scalar;
}




/// Divide each component of this scalar by a scalar value and return the resulting scalar.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> operator / ( const Int32 value, const SIMDArray<Int32,width>& scalar )
{
	return SIMDArray<Int32,width>(value) / scalar;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute the absolute value of each component of the specified SIMD array and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> abs( const SIMDArray<Int32,width>& scalar )
{
	SIMDArray<Int32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Int32,width>::numIterations; i++ )
		result.v[i] = math::abs( scalar.v[i] );
	
	return result;
}




/// Compute the square root of each component of the specified SIMD array and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> sqrt( const SIMDArray<Int32,width>& scalar )
{
	SIMDArray<Int32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Int32,width>::numIterations; i++ )
		result.v[i] = math::sqrt( scalar.v[i] );
	
	return result;
}




/// Compute the minimum of each component of the specified SIMD arrays and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> min( const SIMDArray<Int32,width>& scalar1, const SIMDArray<Int32,width>& scalar2 )
{
	SIMDArray<Int32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Int32,width>::numIterations; i++ )
		result.v[i] = math::min( scalar1.v[i], scalar2.v[i] );
	
	return result;
}




/// Compute the maximum of each component of the specified SIMD arrays and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> max( const SIMDArray<Int32,width>& scalar1, const SIMDArray<Int32,width>& scalar2 )
{
	SIMDArray<Int32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Int32,width>::numIterations; i++ )
		result.v[i] = math::max( scalar1.v[i], scalar2.v[i] );
	
	return result;
}




/// Select elements from the first SIMD array if the selector is TRUE, otherwise from the second.
template < Size width >
OM_FORCE_INLINE SIMDArray<Int32,width> select( const SIMDArray<Int32,width>& selector,
												const SIMDArray<Int32,width>& scalar1, const SIMDArray<Int32,width>& scalar2 )
{
	SIMDArray<Int32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Int32,width>::numIterations; i++ )
		result.v[i] = math::select( selector.v[i], scalar1.v[i], scalar2.v[i] );
	
	return result;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_ARRAY_INT_32_H
