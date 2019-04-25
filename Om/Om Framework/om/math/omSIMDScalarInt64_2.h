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

#ifndef INCLUDE_OM_SIMD_SCALAR_INT_64_2_H
#define INCLUDE_OM_SIMD_SCALAR_INT_64_2_H


#include "omMathConfig.h"


#include "omSIMDScalar.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a 2-component 64-bit signed integer SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 2 64-bit signed integer values. All basic arithmetic operations are supported,
  * plus a subset of standard scalar operations: abs(), min(), max(), sqrt().
  */
template <>
class OM_ALIGN(16) SIMDScalar<Int64,2>
{
	public:
		
		//********************************************************************************
		//******	Public Type Declarations
			
			
			/// The platform-specific vector type to use for 2 64-bit floats.
			typedef SIMDTypeN<Float64,2>::Vector Float64x2;
			
			
			/// The platform-specific vector type to use for 2 64-bit integers.
			typedef SIMDTypeN<Int64,2>::Vector Int64x2;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new 2D SIMD scalar with all elements left uninitialized.
			OM_FORCE_INLINE SIMDScalar()
			{
			}
			
			
			/// Create a new 2D scalar with the specified raw integer value.
			OM_FORCE_INLINE SIMDScalar( Int64x2 rawInt64x2 )
				:	vi( rawInt64x2 )
			{
			}
			
			
			/// Create a new 2D scalar with the specified raw float value, reinterpreting as integers.
			OM_FORCE_INLINE SIMDScalar( Float64x2 rawFloat64x2 )
				:	vf( rawFloat64x2 )
			{
			}
			
			
			/// Create a new 2D SIMD scalar with all elements equal to the specified value.
			OM_FORCE_INLINE SIMDScalar( Int64 value )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
#if defined(OM_PLATFORM_64_BIT)
				vi = _mm_set1_epi64x( value );
#else
				a = b = value;
#endif
#else
				a = b = value;
#endif
			}
			
			
			/// Create a new 2D SIMD scalar with the elements equal to the specified 2 values.
			OM_FORCE_INLINE SIMDScalar( Int64 newA, Int64 newB )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				// The parameters are reversed to keep things consistent with loading from an address.
#if defined(OM_PLATFORM_64_BIT)
				vi = _mm_set_epi64x( newB, newA );
#else
				a = newA;
				b = newB;
#endif
#else
				a = newA;
				b = newB;
#endif
			}
			
			
			/// Create a new 2D SIMD scalar from the first 4 values stored at specified pointer's location.
			OM_FORCE_INLINE SIMDScalar( const Int64* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_load_si128( (Int64x2*)array );
#else
				a = array[0];
				b = array[1];
#endif
			}
			
			
		//********************************************************************************
		//******	Copy Constructor
			
			
			/// Create a new SIMD scalar with the same contents as another.
			/**
			  * This shouldn't have to be overloaded, but for some reason the compiler (GCC)
			  * optimizes SIMD code better with it overloaded. Before, the compiler would
			  * store the result of a SIMD operation on the stack before transfering it to
			  * the destination, resulting in an extra 8+ load/stores per computation.
			  */
			OM_FORCE_INLINE SIMDScalar( const SIMDScalar& other )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = other.vi;
#else
				a = other.a;
				b = other.b;
#endif
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of one SIMDScalar object to another.
			/**
			  * This shouldn't have to be overloaded, but for some reason the compiler (GCC)
			  * optimizes SIMD code better with it overloaded. Before, the compiler would
			  * store the result of a SIMD operation on the stack before transfering it to
			  * the destination, resulting in an extra 8+ load/stores per computation.
			  */
			OM_FORCE_INLINE SIMDScalar& operator = ( const SIMDScalar& other )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = other.vi;
#else
				a = other.a;
				b = other.b;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Load Methods
			
			
			OM_FORCE_INLINE static SIMDScalar load( const Int64* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_load_si128( (Int64x2*)array ) );
#else
				return SIMDScalar( array[0], array[1] );
#endif
			}
			
			
			OM_FORCE_INLINE static SIMDScalar loadUnaligned( const Int64* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_loadu_si128( (Int64x2*)array ) );
#else
				return SIMDScalar( array[0], array[1] );
#endif
			}
			
			
		//********************************************************************************
		//******	Store Methods
			
			
			OM_FORCE_INLINE void store( Int64* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				_mm_store_si128( (Int64x2*)destination, vi );
#else
				destination[0] = a;
				destination[1] = b;
#endif
			}
			
			
			OM_FORCE_INLINE void storeUnaligned( Int64* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				_mm_storeu_si128( (Int64x2*)destination, vi );
#else
				destination[0] = a;
				destination[1] = b;
#endif
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Get a reference to the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int64& operator [] ( Index i )
			{
				return x[i];
			}
			
			
			/// Get the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int64 operator [] ( Index i ) const
			{
				return x[i];
			}
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			OM_FORCE_INLINE const Int64* toArray() const
			{
				return x;
			}
			
			
		//********************************************************************************
		//******	Logical Operators
			
			
			/// Return the bitwise NOT of this 4D SIMD vector.
			OM_FORCE_INLINE SIMDScalar operator ~ () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( vi, _mm_set1_epi32( 0xFFFFFFFF ) ) );
#else
				return SIMDScalar( ~a, ~b );
#endif
			}
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_and_si128( vi, vector.vi ) );
#else
				return SIMDScalar( a & vector.a, b & vector.b );
#endif
			}
			
			
			/// Compute the bitwise OR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( vi, vector.vi ) );
#else
				return SIMDScalar( a | vector.a, b | vector.b );
#endif
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( vi, vector.vi ) );
#else
				return SIMDScalar( a ^ vector.a, b ^ vector.b );
#endif
			}
			
			
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator &= ( const SIMDScalar& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_and_si128( vi, vector.vi );
#else
				a &= vector.a;
				b &= vector.b;
#endif
				return *this;
			}
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator |= ( const SIMDScalar& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_or_si128( vi, vector.vi );
#else
				a |= vector.a;
				b |= vector.b;
#endif
				return *this;
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator ^= ( const SIMDScalar& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_xor_si128( vi, vector.vi );
#else
				a ^= vector.a;
				b ^= vector.b;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two 2D SIMD scalars component-wise for equality.
			OM_FORCE_INLINE SIMDScalar operator == ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpeq_pd( vf, scalar.vf ) );
#else
				return SIMDScalar( a == scalar.a, b == scalar.b );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for equality.
			OM_FORCE_INLINE SIMDScalar operator == ( const Int64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpeq_pd( vf, _mm_set1_pd( *(const Float64*)&value ) ) );
#else
				return SIMDScalar( a == value, b == value );
#endif
			}
			
			
			/// Compare two 2D SIMD scalars component-wise for inequality.
			OM_FORCE_INLINE SIMDScalar operator != ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpneq_pd( vf, scalar.vf ) );
#else
				return SIMDScalar( a != scalar.a, b != scalar.b );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for inequality.
			OM_FORCE_INLINE SIMDScalar operator != ( const Int64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpneq_pd( vf, _mm_set1_pd( *(const Float64*)&value ) ) );
#else
				return SIMDScalar( a != value, b != value );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this and another 2D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator < ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a < scalar.a, b < scalar.b );
			}
			
			
			/// Perform a component-wise less-than comparison between this 2D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator < ( const Int64 value ) const
			{
				return SIMDScalar( a < value, b < value );
			}
			
			
			/// Perform a component-wise greater-than comparison between this an another 2D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator > ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a > scalar.a, b > scalar.b );
			}
			
			
			/// Perform a component-wise greater-than comparison between this 2D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator > ( const Int64 value ) const
			{
				return SIMDScalar( a > value, b > value );
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 2D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator <= ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a <= scalar.a, b <= scalar.b );
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 2D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator <= ( const Int64 value ) const
			{
				return SIMDScalar( a <= value, b <= value );
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 2D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator >= ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a <= scalar.a, b <= scalar.b );
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 2D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator >= ( const Int64 value ) const
			{
				return SIMDScalar( a >= value, b >= value );
			}
			
			
		//********************************************************************************
		//******	Shifting Operators
			
			
			/// Shift each component of the SIMD scalar to the left by the specified amount of bits.
			OM_FORCE_INLINE SIMDScalar operator << ( Int32 bitShift ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_slli_epi64( vi, bitShift ) );
#else
				return SIMDScalar( a << bitShift, b << bitShift );
#endif
			}
			
			
			/// Shift each component of the SIMD scalar to the right by the specified amount of bits.
			OM_FORCE_INLINE SIMDScalar operator >> ( Int32 bitShift ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi64( _mm_setzero_si128(), _mm_srli_epi64( vi, bitShift ) ) );
#else
				return SIMDScalar( a >> bitShift, b >> bitShift );
#endif
			}
			
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate a scalar.
			OM_FORCE_INLINE SIMDScalar operator - () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi64( _mm_setzero_si128(), vi ) );
#else
				return SIMDScalar( -a, -b );
#endif
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this scalar to another and return the result.
			OM_FORCE_INLINE SIMDScalar operator + ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_epi64( vi, scalar.vi ) );
#else
				return SIMDScalar( a + scalar.a, b + scalar.b );
#endif
			}
			
			
			/// Add a value to every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator + ( const Int64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
#if defined(OM_PLATFORM_64_BIT)
				return SIMDScalar( _mm_add_epi64( vi, _mm_set1_epi64x( value ) ) );
#else
				return SIMDScalar( a + value, b + value );
#endif
#else
				return SIMDScalar( a + value, b + value );
#endif
			}
			
			
			/// Subtract a scalar from this scalar component-wise and return the result.
			OM_FORCE_INLINE SIMDScalar operator - ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi64( vi, scalar.vi ) );
#else
				return SIMDScalar( a - scalar.a, b - scalar.b );
#endif
			}
			
			
			/// Subtract a value from every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator - ( const Int64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
#if defined(OM_PLATFORM_64_BIT)
				return SIMDScalar( _mm_sub_epi64( vi, _mm_set1_epi64x( value ) ) );
#else
				return SIMDScalar( a - value, b - value );
#endif
#else
				return SIMDScalar( a - value, b - value );
#endif
			}
			
			
			/// Multiply component-wise this scalar and another scalar.
			OM_FORCE_INLINE SIMDScalar operator * ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a*scalar.a, b*scalar.b );
			}
			
			
			/// Multiply every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator * ( const Int64 value ) const
			{
				return SIMDScalar( a*value, b*value );
			}
			
			
			/// Divide this scalar by another scalar component-wise.
			OM_FORCE_INLINE SIMDScalar operator / ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a/scalar.a, b/scalar.b );
			}
			
			
			/// Divide every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator / ( const Int64 value ) const
			{
				return SIMDScalar( a/value, b/value );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator += ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_add_epi64( vi, scalar.vi );
#else
				a += scalar.a;
				b += scalar.b;
#endif
				return *this;
			}
			
			/// Subtract a scalar from this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator -= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_sub_epi64( vi, scalar.vi );
#else
				a -= scalar.a;
				b -= scalar.b;
#endif
				return *this;
			}
			
			
			/// Multiply component-wise this scalar and another scalar and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator *= ( const SIMDScalar& scalar )
			{
				a *= scalar.a;
				b *= scalar.b;
				
				return *this;
			}
			
			
			/// Divide this scalar by another scalar component-wise and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator /= ( const SIMDScalar& scalar )
			{
				a /= scalar.a;
				b /= scalar.b;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Required Alignment Accessor Methods
			
			
			/// Return the alignment required for objects of this type.
			OM_FORCE_INLINE static Size getAlignment()
			{
				return ALIGNMENT;
			}
			
			
			/// Get the width of this scalar (number of components it has).
			OM_FORCE_INLINE static Size getWidth()
			{
				return WIDTH;
			}
			
			
			/// Return whether or not this SIMD type is supported by the current CPU.
			OM_FORCE_INLINE static Bool isSupported()
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				SIMDFlags flags = SIMDFlags::get();
				return (flags & SIMDFlags::SSE_2) != 0;
#else
				return false;
#endif
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this SIMD scalar into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "(" << a << ", " << b << ")";
				
				return buffer.toString();
			}
			
			
			/// Convert this SIMD scalar into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The number of components there are in this scalar.
			static const Size WIDTH = SIMDTypeN<Int64,2>::WIDTH;
			
			
			/// The required alignment of this scalar type.
			static const Size ALIGNMENT = SIMDTypeN<Int64,2>::ALIGNMENT;


			union OM_ALIGN(16)
			{
				/// The platform-specific vector to use for 2 64-bit floats.
				Float64x2 vf;
				
				/// The platform-specific vector to use for 2 64-bit integers.
				Int64x2 vi;
				
				/// The components of a 2D SIMD scalar in array format.
				Int64 x[2];
				
				struct
				{
					/// The first component of a 2D SIMD scalar.
					Int64 a;
					
					
					/// The second component of a 2D SIMD scalar.
					Int64 b;
				};
			};
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute the absolute value of each component of the specified SIMD scalar and return the result.
OM_FORCE_INLINE SIMDScalar<Int64,2> abs( const SIMDScalar<Int64,2>& scalar )
{
	return SIMDScalar<Int64,2>( math::abs(scalar.a), math::abs(scalar.b) );
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Int64,2> min( const SIMDScalar<Int64,2>& scalar1, const SIMDScalar<Int64,2>& scalar2 )
{
	return SIMDScalar<Int64,2>( math::min(scalar1.a, scalar2.a),
								math::min(scalar1.b, scalar2.b) );
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Int64,2> max( const SIMDScalar<Int64,2>& scalar1, const SIMDScalar<Int64,2>& scalar2 )
{
	return SIMDScalar<Int64,2>( math::max(scalar1.a, scalar2.a),
								math::max(scalar1.b, scalar2.b) );
}




/// Pick 2 elements from the specified SIMD scalar and return the result.
template < UInt i1, UInt i2 >
OM_FORCE_INLINE SIMDScalar<Int64,2> shuffle( const SIMDScalar<Int64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return SIMDScalar<Int64,2>( scalar[i1], scalar[i2] );
	// This is a temporary fix made by Zhenyu Tang on 01/02/2017 for building on Linux.
	//return SIMDScalar<Int64,2>( _mm_shuffle_pd( scalar.vi, scalar.vi, _MM_SHUFFLE2(i2, i1) ) );
#else
	return SIMDScalar<Int64,2>( scalar[i1], scalar[i2] );
#endif
}




/// Pick one element from each SIMD scalar and return the result.
template < UInt i1, UInt i2 >
OM_FORCE_INLINE SIMDScalar<Int64,2> shuffle( const SIMDScalar<Int64,2>& scalar1, const SIMDScalar<Int64,2>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Int64,2>( scalar1[i1], scalar1[i2] );
	// This is a temporary fix made by Zhenyu Tang on 01/02/2017 for building on Linux.
	//return SIMDScalar<Int64,2>( _mm_shuffle_pd( scalar.vi, scalar.vi, _MM_SHUFFLE2(i2, i1) ) );
#else
	return SIMDScalar<Int64,2>( scalar1[i1], scalar1[i2] );
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
OM_FORCE_INLINE SIMDScalar<Int64,2> select( const SIMDScalar<Int64,2>& selector,
												const SIMDScalar<Int64,2>& scalar1, const SIMDScalar<Int64,2>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Int64,2>( _mm_xor_pd( scalar2.vf, _mm_and_pd( selector.vf, _mm_xor_pd( scalar1.vf, scalar2.vf ) ) ) );
#else
	return SIMDScalar<Int64,2>( selector.a ? scalar1.a : scalar2.a,
								selector.b ? scalar1.b : scalar2.b );
#endif
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_SCALAR_INT_64_2_H
