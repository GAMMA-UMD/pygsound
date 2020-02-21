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

#ifndef INCLUDE_OM_SIMD_SCALAR_INT_16_8_H
#define INCLUDE_OM_SIMD_SCALAR_INT_16_8_H


#include "omMathConfig.h"


#include "omSIMDScalar.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a 8-component 16-bit signed-integer SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 8 16-bit signed-integer values.
  */
template <>
class OM_ALIGN(16) SIMDScalar<Int16,8>
{
	public:
		
		//********************************************************************************
		//******	Public Type Declarations
			
			
			/// The platform-specific vector type to use for 8 16-bit integers.
			typedef SIMDTypeN<Int16,8>::Vector Int16x8;
			
			
			/// The platform-specific vector type to use for 4 32-bit floats.
			typedef SIMDTypeN<Float32,4>::Vector Float32x4;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new 4D SIMD scalar with all elements left uninitialized.
			OM_FORCE_INLINE SIMDScalar()
			{
			}
			
			
			/// Create a new 4D vector with the specified raw integer value.
			OM_FORCE_INLINE SIMDScalar( Int16x8 rawInt64x2 )
				:	vi( rawInt64x2 )
			{
			}
			
			
			/// Create a new 4D vector with the specified raw float value, reinterpreting as integers.
			OM_FORCE_INLINE SIMDScalar( Float32x4 simdScalar )
				:	vf( simdScalar )
			{
			}
			
			
			/// Create a new 4D SIMD scalar with all elements equal to the specified value.
			OM_FORCE_INLINE SIMDScalar( Int16 value )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = (Int16x8)(value);
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_set1_epi16( value );
#else
				a = b = c = d = e = f = g = h = value;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar with the elements equal to the specified 4 values.
			OM_FORCE_INLINE SIMDScalar( Int16 newA, Int16 newB, Int16 newC, Int16 newD, Int16 newE, Int16 newF, Int16 newG, Int16 newH )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = (Int16x8)( newA, newB, newC, newD, newE, newF, newG, newH );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				vi = _mm_set_epi16( newH, newG, newF, newE, newD, newC, newB, newA );
#else
				a = newA;	b = newB;	c = newC;	d = newD;
				e = newE;	f = newF;	g = newG;	h = newH;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar from the first 4 values stored at specified pointer's location.
			OM_FORCE_INLINE SIMDScalar( const Int16* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				a = array[0];	b = array[1];	c = array[2];	d = array[3];
				e = array[4];	f = array[5];	g = array[6];	h = array[7];
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_load_si128( (const Int16x8*)array );
#else
				a = array[0];	b = array[1];	c = array[2];	d = array[3];
				e = array[4];	f = array[5];	g = array[6];	h = array[7];
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
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = other.vi;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = other.vi;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vFloat = other.vf;
#else
				a = other.a;	b = other.b;	c = other.c;	d = other.d;
				e = other.e;	f = other.f;	g = other.g;	h = other.h;
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
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = other.vi;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = other.vi;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vFloat = other.vf;
#else
				a = other.a;	b = other.b;	c = other.c;	d = other.d;
				e = other.e;	f = other.f;	g = other.g;	h = other.h;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Load Methods
			
			
			OM_FORCE_INLINE static SIMDScalar load( const Int16* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( array[0], array[1], array[2], array[3],
									array[4], array[5], array[6], array[7] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_load_si128( (const Int16x8*)array ) );
#else
				return SIMDScalar( array[0], array[1], array[2], array[3],
									array[4], array[5], array[6], array[7] );
#endif
			}
			
			
			OM_FORCE_INLINE static SIMDScalar loadUnaligned( const Int16* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( array[0], array[1], array[2], array[3],
									array[4], array[5], array[6], array[7] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_loadu_si128( (const Int16x8*)array ) );
#else
				return SIMDScalar( array[0], array[1], array[2], array[3],
									array[4], array[5], array[6], array[7] );
#endif
			}
			
			
		//********************************************************************************
		//******	Store Method
			
			
			/// Store the contents of this SIMD scalar into the specified aligned destination pointer.
			OM_FORCE_INLINE void store( Int16* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				destination[0] = a;	destination[1] = b;	destination[2] = c;	destination[3] = d;
				destination[4] = e;	destination[5] = f;	destination[6] = g;	destination[7] = h;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				_mm_store_si128( (Int16x8*)destination, vi );
#else
				destination[0] = a;	destination[1] = b;	destination[2] = c;	destination[3] = d;
				destination[4] = e;	destination[5] = f;	destination[6] = g;	destination[7] = h;
#endif
			}
			
			
			/// Store the contents of this SIMD scalar into the specified unaligned destination pointer.
			OM_FORCE_INLINE void storeUnaligned( Int16* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				destination[0] = a;	destination[1] = b;	destination[2] = c;	destination[3] = d;
				destination[4] = e;	destination[5] = f;	destination[6] = g;	destination[7] = h;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				_mm_storeu_si128( (Int16x8*)destination, vi );
#else
				destination[0] = a;	destination[1] = b;	destination[2] = c;	destination[3] = d;
				destination[4] = e;	destination[5] = f;	destination[6] = g;	destination[7] = h;
#endif
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Return a reference to the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int16& operator [] ( Index i )
			{
				return x[i];
			}
			
			
			/// Return the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int16 operator [] ( Index i ) const
			{
				return x[i];
			}
			
			
			/// Return a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 7 locations after the
			  * first element.
			  */
			OM_FORCE_INLINE const Int16* toArray() const
			{
				return x;
			}
			
			
		//********************************************************************************
		//******	Logical Operators
			
			
			/// Return the bitwise NOT of this 4D SIMD vector.
			OM_FORCE_INLINE SIMDScalar operator ~ () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_nor( vi, v ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( vi, _mm_set1_epi16( Int16(0xFFFF) ) ) );
#else
				return SIMDScalar( ~a, ~b, ~c, ~d, ~e, ~f, ~g, ~h );
#endif
			}
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_and( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_and_si128( vi, vector.vi ) );
#else
				return SIMDScalar( a & vector.a, b & vector.b, c & vector.c, d & vector.d,
									e & vector.e, f & vector.f, g & vector.g, h & vector.h );
#endif
			}
			
			
			/// Compute the bitwise OR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_or( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( vi, vector.vi ) );
#else
				return SIMDScalar( a | vector.a, b | vector.b, c | vector.c, d | vector.d,
									e | vector.e, f | vector.f, g | vector.g, h | vector.h );
#endif
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_xor( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( vi, vector.vi ) );
#else
				return SIMDScalar( a ^ vector.a, b ^ vector.b, c ^ vector.c, d ^ vector.d,
									e ^ vector.e, f ^ vector.f, g ^ vector.g, h ^ vector.h );
#endif
			}
			
			
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator &= ( const SIMDScalar& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_and( vi, vector.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_and_si128( vi, vector.vi );
#else
				a &= vector.a;	b &= vector.b;	c &= vector.c;	d &= vector.d;
				e &= vector.e;	f &= vector.f;	g &= vector.g;	h &= vector.h;
#endif
				return *this;
			}
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator |= ( const SIMDScalar& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_or( vi, vector.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_or_si128( vi, vector.vi );
#else
				a |= vector.a;	b |= vector.b;	c |= vector.c;	d |= vector.d;
				e |= vector.e;	f |= vector.f;	g |= vector.g;	h |= vector.h;
#endif
				return *this;
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator ^= ( const SIMDScalar& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_xor( vi, vector.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_xor_si128( vi, vector.vi );
#else
				a ^= vector.a;	b ^= vector.b;	c ^= vector.c;	d ^= vector.d;
				e ^= vector.e;	f ^= vector.f;	g ^= vector.g;	h ^= vector.h;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two 4D SIMD scalars component-wise for equality.
			OM_FORCE_INLINE SIMDScalar operator == ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpeq( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpeq_epi16( vi, scalar.vi ) );
#else
				return SIMDScalar( a == scalar.a, b == scalar.b, c == scalar.c, d == scalar.d,
									e == scalar.e, f == scalar.f, g == scalar.g, h == scalar.h );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for equality.
			OM_FORCE_INLINE SIMDScalar operator == ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpeq( vi, (Int16x8)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpeq_epi16( vi, _mm_set1_epi16(value) ) );
#else
				return SIMDScalar( a == value, b == value, c == value, d == value,
									e == value, f == value, g == value, h == value );
#endif
			}
			
			
			/// Compare two 4D SIMD scalars component-wise for inequality.
			OM_FORCE_INLINE SIMDScalar operator != ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				const Int16x8 temp = vec_cmpeq( vi, scalar.vi );
				return SIMDScalar( vec_nor( temp, temp ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( _mm_cmpeq_epi16( vi, scalar.vi ), _mm_set1_epi16( Int16(0xFFFF) ) ) );
#else
				return SIMDScalar( a != scalar.a, b != scalar.b, c != scalar.c, d != scalar.d,
									e != scalar.e, f != scalar.f, g != scalar.g, h != scalar.h );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for inequality.
			OM_FORCE_INLINE SIMDScalar operator != ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				const Int16x8 temp = vec_cmpeq( vi, (Int16x8)(value) );
				return SIMDScalar( vec_nor( temp, temp ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( _mm_cmpeq_epi16( vi, _mm_set1_epi16(value) ), _mm_set1_epi16( Int16(0xFFFF) ) ) );
#else
				return SIMDScalar( a != value, b != value, c != value, d != value,
									e != value, f != value, g != value, h != value );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator < ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmplt( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmplt_epi16( vi, scalar.vi ) );
#else
				return SIMDScalar( a < scalar.a, b < scalar.b, c < scalar.c, d < scalar.d,
									e < scalar.e, f < scalar.f, g < scalar.g, h < scalar.h );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator < ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmplt( vi, (Int16x8)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmplt_epi16( vi, _mm_set1_epi16(value) ) );
#else
				return SIMDScalar( a < value, b < value, c < value, d < value,
									e < value, f < value, g < value, h < value );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator > ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpgt( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpgt_epi16( vi, scalar.vi ) );
#else
				return SIMDScalar( a > scalar.a, b > scalar.b, c > scalar.c, d > scalar.d,
									e > scalar.e, f > scalar.f, g > scalar.g, h > scalar.h );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator > ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpgt( vi, (Int16x8)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpgt_epi16( vi, _mm_set1_epi16(value) ) );
#else
				return SIMDScalar( a > value, b > value, c > value, d > value,
									e > value, f > value, g > value, h > value );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator <= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmple( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( _mm_cmplt_epi16( vi, scalar.vi ),
														_mm_cmpeq_epi16( vi, scalar.vi ) ) );
#else
				return SIMDScalar( a <= scalar.a, b <= scalar.b, c <= scalar.c, d <= scalar.d,
									e <= scalar.e, f <= scalar.f, g <= scalar.g, h <= scalar.h );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator <= ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmple( vi, (Int16x8)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				Int16x8 scalar = _mm_set1_epi16(value);
				return SIMDScalar( _mm_or_si128( _mm_cmplt_epi16( vi, scalar ),
														_mm_cmpeq_epi16( vi, scalar ) ) );
#else
				return SIMDScalar( a <= value, b <= value, c <= value, d <= value,
									e <= value, f <= value, g <= value, h <= value );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator >= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpge( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( _mm_cmpgt_epi16( vi, scalar.vi ),
														_mm_cmpeq_epi16( vi, scalar.vi ) ) );
#else
				return SIMDScalar( a >= scalar.a, b >= scalar.b, c >= scalar.c, d >= scalar.d,
									e >= scalar.e, f >= scalar.f, g >= scalar.g, h >= scalar.h );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator >= ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpge( vi, (Int16x8)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				Int16x8 scalar = _mm_set1_epi16(value);
				return SIMDScalar( _mm_or_si128( _mm_cmpgt_epi16( vi, scalar ),
														_mm_cmpeq_epi16( vi, scalar ) ) );
#else
				return SIMDScalar( a >= value, b >= value, c >= value, d >= value,
									e >= value, f >= value, g >= value, h >= value );
#endif
			}
			
			
		//********************************************************************************
		//******	Shifting Operators
			
			
			/// Shift each component of the SIMD scalar to the left by the specified amount of bits.
			OM_FORCE_INLINE SIMDScalar operator << ( Int16 bitShift ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sl( vi, (Int16x8)(bitShift) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_slli_epi16( vi, bitShift ) );
#else
				return SIMDScalar( a << bitShift, b << bitShift, c << bitShift, d << bitShift,
									e << bitShift, f << bitShift, g << bitShift, h << bitShift );
#endif
			}
			
			
			/// Shift each component of the SIMD scalar to the right by the specified amount of bits.
			OM_FORCE_INLINE SIMDScalar operator >> ( Int16 bitShift ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sra( vi, (Int16x8)(bitShift) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_srai_epi16( vi, bitShift ) );
#else
				return SIMDScalar( a >> bitShift, b >> bitShift, c >> bitShift, d >> bitShift,
									e >> bitShift, f >> bitShift, g >> bitShift, h >> bitShift );
#endif
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate a scalar.
			OM_FORCE_INLINE SIMDScalar operator - () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( (Int16x8)(0), v ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi16( _mm_set1_epi16(0), vi ) );
#else
				return SIMDScalar( -a, -b, -c, -d, -e, -f, -g, -h );
#endif
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this scalar to another and return the result.
			OM_FORCE_INLINE SIMDScalar operator + ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_add( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_epi16( vi, scalar.vi ) );
#else
				return SIMDScalar( a + scalar.a, b + scalar.b, c + scalar.c, d + scalar.d,
									e + scalar.e, f + scalar.f, g + scalar.g, h + scalar.h );
#endif
			}
			
			
			/// Add a value to every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator + ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_add( vi, (Int16x8)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_epi16( vi, _mm_set1_epi16(value) ) );
#else
				return SIMDScalar( a + value, b + value, c + value, d + value,
									e + value, f + value, g + value, h + value );
#endif
			}
			
			
			/// Subtract a scalar from this scalar component-wise and return the result.
			OM_FORCE_INLINE SIMDScalar operator - ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi16( vi, scalar.vi ) );
#else
				return SIMDScalar( a - scalar.a, b - scalar.b, c - scalar.c, d - scalar.d,
									e - scalar.e, f - scalar.f, g - scalar.g, h - scalar.h );
#endif
			}
			
			
			/// Subtract a value from every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator - ( const Int16 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( vi, (Int16x8)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi16( vi, _mm_set1_epi16(value) ) );
#else
				return SIMDScalar( a - value, b - value, c - value, d - value,
									e - value, f - value, g - value, h - value );
#endif
			}
			
			
			/// Multiply component-wise this scalar and another scalar.
			OM_FORCE_INLINE SIMDScalar operator * ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a*scalar.a, b*scalar.b, c*scalar.c, d*scalar.d,
									e*scalar.e, f*scalar.f, g*scalar.g, h*scalar.h );
			}
			
			
			/// Multiply every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator * ( const Int16 value ) const
			{
				return SIMDScalar( a*value, b*value, c*value, d*value, e*value, f*value, g*value, h*value );
			}
			
			
			/// Divide this scalar by another scalar component-wise.
			OM_FORCE_INLINE SIMDScalar operator / ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a/scalar.a, b/scalar.b, c/scalar.c, d/scalar.d,
									e/scalar.e, f/scalar.f, g/scalar.g, h/scalar.h );
			}
			
			
			/// Divide every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator / ( const Int16 value ) const
			{
				return SIMDScalar( a/value, b/value, c/value, d/value, e/value, f/value, g/value, h/value );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator += ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_add( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_add_epi16( vi, scalar.vi );
#else
				a += scalar.a;	b += scalar.b;	c += scalar.c;	d += scalar.d;
				e += scalar.e;	f += scalar.f;	g += scalar.g;	h += scalar.h;
#endif
				return *this;
			}
			
			
			/// Subtract a scalar from this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator -= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_sub( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_sub_epi16( vi, scalar.vi );
#else
				a -= scalar.a;	b -= scalar.b;	c -= scalar.c;	d -= scalar.d;
				e -= scalar.e;	f -= scalar.f;	g -= scalar.g;	h -= scalar.h;
#endif
				return *this;
			}
			
			
			/// Multiply component-wise this scalar and another scalar and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator *= ( const SIMDScalar& scalar )
			{
				a *= scalar.a;	b *= scalar.b;	c *= scalar.c;	d *= scalar.d;
				e *= scalar.e;	f *= scalar.f;	g *= scalar.g;	h *= scalar.h;
				
				return *this;
			}
			
			
			/// Divide this scalar by another scalar component-wise and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator /= ( const SIMDScalar& scalar )
			{
				a /= scalar.a;	b /= scalar.b;	c /= scalar.c;	d /= scalar.d;
				e /= scalar.e;	f /= scalar.f;	g /= scalar.g;	h /= scalar.h;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Required Alignment Accessor Methods
			
			
			/// Return the alignment required for objects of this type.
			/**
			  * For most SIMD types this value will be 16 bytes. If there is
			  * no alignment required, 0 is returned.
			  */
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
				const SIMDFlags flags = SIMDFlags::get();
				
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return flags & SIMDFlags::ALTIVEC != 0;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE)
				return (flags & SIMDFlags::SSE_2) != 0;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return flags & SIMDFlags::ARM_NEON != 0;
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
				
				buffer << "(" << a << ", " << b << ", " << c << ", " << d << ", " << e << ", " << f << ", " << g << ", " << h << ")";
				
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
			static const Size WIDTH = SIMDTypeN<Int16,8>::WIDTH;
			
			
			/// The required alignment of this scalar type.
			static const Size ALIGNMENT = SIMDTypeN<Int16,8>::ALIGNMENT;


			union OM_ALIGN(16)
			{
				/// The platform-specific vector to use for 4 32-bit floats.
				Float32x4 vf;
				
				/// The platform-specific vector to use for 8 16-bit integers.
				Int16x8 vi;
				
				/// The components of a 8D SIMD scalar in array format.
				Int16 x[8];
				
				struct
				{
					/// The first component of a 8x16-bit SIMD integer.
					Int16 a;
					
					/// The second component of a 4D SIMD scalar.
					Int16 b;
					
					/// The third component of a 4D SIMD scalar.
					Int16 c;
					
					/// The fourth component of a 4D SIMD scalar.
					Int16 d;
					
					/// The fifth component of a 4D SIMD scalar.
					Int16 e;
					
					/// The sixth component of a 4D SIMD scalar.
					Int16 f;
					
					/// The seventh component of a 4D SIMD scalar.
					Int16 g;
					
					/// The eighth component of a 4D SIMD scalar.
					Int16 h;
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
OM_FORCE_INLINE SIMDScalar<Int16,8> abs( const SIMDScalar<Int16,8>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int16,8>( vec_abs( scalar.vi ) );
#else
	return SIMDScalar<Int16,8>( math::abs(scalar.a), math::abs(scalar.b), math::abs(scalar.c), math::abs(scalar.d),
								math::abs(scalar.e), math::abs(scalar.f), math::abs(scalar.g), math::abs(scalar.h) );
#endif
}



/// Compute the minimum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Int16,8> min( const SIMDScalar<Int16,8>& scalar1, const SIMDScalar<Int16,8>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int16,8>( vec_min( scalar1.vi, scalar2.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE)
	return SIMDScalar<Int16,8>( math::min(scalar1.a, scalar2.a), math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c), math::min(scalar1.d, scalar2.d),
								math::min(scalar1.e, scalar2.e), math::min(scalar1.f, scalar2.f),
								math::min(scalar1.g, scalar2.g), math::min(scalar1.h, scalar2.h) );
#else
	return SIMDScalar<Int16,8>( math::min(scalar1.a, scalar2.a), math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c), math::min(scalar1.d, scalar2.d),
								math::min(scalar1.e, scalar2.e), math::min(scalar1.f, scalar2.f),
								math::min(scalar1.g, scalar2.g), math::min(scalar1.h, scalar2.h) );
#endif
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Int16,8> max( const SIMDScalar<Int16,8>& scalar1, const SIMDScalar<Int16,8>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int16,8>( vec_max( scalar1.vi, scalar2.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE)
	return SIMDScalar<Int16,8>( math::max(scalar1.a, scalar2.a), math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c), math::max(scalar1.d, scalar2.d),
								math::max(scalar1.e, scalar2.e), math::max(scalar1.f, scalar2.f),
								math::max(scalar1.g, scalar2.g), math::max(scalar1.h, scalar2.h) );
#else
	return SIMDScalar<Int16,8>( math::max(scalar1.a, scalar2.a), math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c), math::max(scalar1.d, scalar2.d),
								math::max(scalar1.e, scalar2.e), math::max(scalar1.f, scalar2.f),
								math::max(scalar1.g, scalar2.g), math::max(scalar1.h, scalar2.h) );
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
OM_FORCE_INLINE SIMDScalar<Int16,8> select( const SIMDScalar<Int16,8>& selector,
												const SIMDScalar<Int16,8>& scalar1, const SIMDScalar<Int16,8>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int16,8>( vec_sel( scalar2.vi, scalar1.vi, selector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Int16,8>( _mm_xor_ps( scalar2.vf,
										_mm_and_ps( selector.vf,
													_mm_xor_ps( scalar1.vf, scalar2.vf ) ) ) );
#else
	return SIMDScalar<Int16,8>( selector.a ? scalar1.a : scalar2.a, selector.b ? scalar1.b : scalar2.b,
								selector.c ? scalar1.c : scalar2.c, selector.d ? scalar1.d : scalar2.d,
								selector.e ? scalar1.e : scalar2.e, selector.f ? scalar1.f : scalar2.f,
								selector.g ? scalar1.g : scalar2.g, selector.h ? scalar1.h : scalar2.h );
#endif
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_SCALAR_INT_16_8_H
