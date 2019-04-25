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

#ifndef INCLUDE_OM_SIMD_SCALAR_INT_32_4_H
#define INCLUDE_OM_SIMD_SCALAR_INT_32_4_H


#include "omMathConfig.h"


#include "omSIMDScalar.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a 4-component 32-bit signed-integer SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 4 32-bit signed-integer values.
  */
template <>
class OM_ALIGN(16) SIMDScalar<Int32,4>
{
	public:
		
		//********************************************************************************
		//******	Public Type Declarations
			
			
			/// The platform-specific vector type to use for 4 32-bit floats.
			typedef SIMDTypeN<Float32,4>::Vector Float32x4;
			
			
			/// The platform-specific vector type to use for 4 32-bit integers.
			typedef SIMDTypeN<Int32,4>::Vector Int32x4;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new 4D SIMD scalar with all elements left uninitialized.
			OM_FORCE_INLINE SIMDScalar()
			{
			}
			
			
			/// Create a new 4D vector with the specified raw integer value.
			OM_FORCE_INLINE SIMDScalar( Int32x4 simdScalar )
				:	vi( simdScalar )
			{
			}
			
			
			/// Create a new 4D vector with the specified raw float value, reinterpreting as integers.
			OM_FORCE_INLINE SIMDScalar( Float32x4 simdScalar )
				:	vf( simdScalar )
			{
			}
			
			
			/// Create a new 4D SIMD scalar with all elements equal to the specified value.
			OM_FORCE_INLINE SIMDScalar( Int32 value )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = (Int32x4)(value);
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_set1_epi32( value );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vdupq_n_s32( value );
#else
				a = b = c = d = value;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar with the elements equal to the specified 4 values.
			OM_FORCE_INLINE SIMDScalar( Int32 newA, Int32 newB, Int32 newC, Int32 newD )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = (Int32x4)( newA, newB, newC, newD );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				vi = _mm_set_epi32( newD, newC, newB, newA );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				const Int32 abcd[4] = {newA, newB, newC, newD};
				vi = vld1q_s32( abcd );
#else
				a = newA; b = newB; c = newC; d = newD;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar from the first 4 values stored at specified aligned pointer's location.
			OM_FORCE_INLINE SIMDScalar( const Int32* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				a = array[0]; b = array[1]; c = array[2]; d = array[3];
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_load_si128( (const Int32x4*)array );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vld1q_s32( array );
#else
				a = array[0]; b = array[1]; c = array[2]; d = array[3];
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
				vf = other.vf;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = other.vi;
#else
				a = other.a; b = other.b; c = other.c; d = other.d;
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
				vf = other.vf;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = other.vi;
#else
				a = other.a; b = other.b; c = other.c; d = other.d;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Load Methods
			
			
			OM_FORCE_INLINE static SIMDScalar load( const Int32* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_load_si128( (const Int32x4*)array ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vld1q_s32( array ) );
#else
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#endif
			}
			
			
			OM_FORCE_INLINE static SIMDScalar loadUnaligned( const Int32* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_loadu_si128( (const Int32x4*)array ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vld1q_s32( array ) );
#else
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#endif
			}
			
			
		//********************************************************************************
		//******	Store Method
			
			
			OM_FORCE_INLINE void store( Int32* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				_mm_store_si128( (Int32x4*)destination, vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vst1q_s32( destination, vi );
#else
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#endif
			}
			
			
			OM_FORCE_INLINE void storeUnaligned( Int32* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				_mm_storeu_si128( (Int32x4*)destination, vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vst1q_s32( destination, vi );
#else
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#endif
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Return a reference to the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int32& operator [] ( Index i )
			{
				return x[i];
			}
			
			
			/// Return the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Int32 operator [] ( Index i ) const
			{
				return x[i];
			}
			
			
			/// Return a pointer to the first element in this scalar.
			OM_FORCE_INLINE const Int32* toArray() const
			{
				return x;
			}
			
			
		//********************************************************************************
		//******	Mask Methods
			
			
			/// Return a mask which indicates if high-order bits of each of the components are true.
			OM_FORCE_INLINE Int getMask() const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return ((a & 0x80000000) << 3) | ((b & 0x80000000) << 2) | ((c & 0x80000000) << 1) | ((d & 0x80000000));
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return _mm_movemask_ps( vf );
#else
				return ((a & 0x80000000) << 3) | ((b & 0x80000000) << 2) | ((c & 0x80000000) << 1) | ((d & 0x80000000));
#endif
			}
			
			
			/// Return whether or not any component of this scalar have the high-order bit set.
			OM_FORCE_INLINE operator Int () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return (a & 0x80000000) | (b & 0x80000000) | (c & 0x80000000) | (d & 0x80000000);
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return _mm_movemask_ps( vf );
#else
				return (a & 0x80000000) | (b & 0x80000000) | (c & 0x80000000) | (d & 0x80000000);
#endif
			}
			
			
			/// Return whether or not any component of this array has the high-order bit set.
			OM_FORCE_INLINE operator Bool () const
			{
				return this->getMask() != 0;
			}
			
			
			/// Return whether or not any component of this scalar has the high-order bit set.
			OM_FORCE_INLINE Bool testMaskAny() const
			{
				return this->getMask() != 0;
			}
			
			
			/// Return whether or not all components of this scalar have the high-order bit set.
			OM_FORCE_INLINE Bool testMaskAll() const
			{
				return this->getMask() == 0xF;
			}
			
			
		//********************************************************************************
		//******	Logical Operators
			
			
			/// Return the bitwise NOT of this 4D SIMD vector.
			OM_FORCE_INLINE SIMDScalar operator ~ () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_nor( vi, vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( vi, _mm_set1_epi32( 0xFFFFFFFF ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vmvnq_s32( vi ) );
#else
				return SIMDScalar( ~a, ~b, ~c, ~d );
#endif
			}
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_and( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_and_si128( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vandq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a & vector.a, b & vector.b, c & vector.c, d & vector.d );
#endif
			}
			
			
			/// Compute the bitwise OR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_or( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vorrq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a | vector.a, b | vector.b, c | vector.c, d | vector.d );
#endif
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_xor( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( veorq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a ^ vector.a, b ^ vector.b, c ^ vector.c, d ^ vector.d );
#endif
			}
			
			
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator &= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_and( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_and_si128( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vandq_s32( vi, scalar.vi );
#else
				a &= scalar.a; b &= scalar.b; c &= scalar.c; d &= scalar.d;
#endif
				return *this;
			}
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator |= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_or( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_or_si128( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vorrq_s32( vi, scalar.vi );
#else
				a |= scalar.a; b |= scalar.b; c |= scalar.c; d |= scalar.d;
#endif
				return *this;
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator ^= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_xor( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_xor_si128( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = veorq_s32( vi, scalar.vi );
#else
				a ^= scalar.a; b ^= scalar.b; c ^= scalar.c; d ^= scalar.d;
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
				return SIMDScalar( _mm_cmpeq_epi32( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vceqq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a == scalar.a, b == scalar.b, c == scalar.c, d == scalar.d );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for equality.
			OM_FORCE_INLINE SIMDScalar operator == ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpeq( vi, (Int32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpeq_epi32( vi, _mm_set1_epi32(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vceqq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a == value, b == value, c == value, d == value );
#endif
			}
			
			
			/// Compare two 4D SIMD scalars component-wise for inequality
			OM_FORCE_INLINE SIMDScalar operator != ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				const Int32x4 temp = vec_cmpeq( vi, scalar.vi );
				return SIMDScalar( vec_nor( temp, temp ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( _mm_cmpeq_epi32( vi, scalar.vi ), _mm_set1_epi32(0xFFFFFFFF) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vmvnq_s32( vceqq_s32( vi, scalar.vi ) ) );
#else
				return SIMDScalar( a != scalar.a, b != scalar.b, c != scalar.c, d != scalar.d );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for inequality.
			OM_FORCE_INLINE SIMDScalar operator != ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				const Int32x4 temp = vec_cmpeq( vi, (Int32x4)(value) );
				return SIMDScalar( vec_nor( temp, temp ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( _mm_cmpeq_epi32( vi, _mm_set1_epi32(value) ), _mm_set1_epi32(0xFFFFFFFF) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vmvnq_s32( vceqq_s32( vi, vdupq_n_s32(value) ) ) );
#else
				return SIMDScalar( a != value, b != value, c != value, d != value );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator < ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmplt( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmplt_epi32( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcltq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a < scalar.a, b < scalar.b, c < scalar.c, d < scalar.d );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator < ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmplt( vi, (Int32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmplt_epi32( vi, _mm_set1_epi32(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcltq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a < value, b < value, c < value, d < value );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator > ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpgt( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpgt_epi32( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcgtq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a > scalar.a, b > scalar.b, c > scalar.c, d > scalar.d );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator > ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpgt( vi, (Int32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_cmpgt_epi32( vi, _mm_set1_epi32(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcgtq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a > value, b > value, c > value, d > value );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator <= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmple( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( _mm_cmplt_epi32( vi, scalar.vi ),
														_mm_cmpeq_epi32( vi, scalar.vi ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcleq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a <= scalar.a, b <= scalar.b, c <= scalar.c, d <= scalar.d );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator <= ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmple( vi, (Int32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				Int32x4 scalar = _mm_set1_epi32(value);
				return SIMDScalar( _mm_or_si128( _mm_cmplt_epi32( vi, scalar ),
														_mm_cmpeq_epi32( vi, scalar ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcleq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a <= value, b <= value, c <= value, d <= value );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar operator >= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpge( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( _mm_cmpgt_epi32( vi, scalar.vi ),
														_mm_cmpeq_epi32( vi, scalar.vi ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcgeq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a >= scalar.a, b >= scalar.b, c >= scalar.c, d >= scalar.d );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar operator >= ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_cmpge( vi, (Int32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				Int32x4 scalar = _mm_set1_epi32(value);
				return SIMDScalar( _mm_or_si128( _mm_cmpgt_epi32( vi, scalar ),
														_mm_cmpeq_epi32( vi, scalar ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vcgeq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a >= value, b >= value, c >= value, d >= value );
#endif
			}
			
			
		//********************************************************************************
		//******	Shifting Operators
			
			
			/// Shift each component of the SIMD scalar to the left by the specified amount of bits.
			OM_FORCE_INLINE SIMDScalar operator << ( Int32 bitShift ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sl( vi, (Int32x4)(bitShift) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_slli_epi32( vi, bitShift ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vshlq_n_s32( vi, bitShift ) );
#else
				return SIMDScalar( a << bitShift, b << bitShift, c << bitShift, d << bitShift );
#endif
			}
			
			
			/// Shift each component of the SIMD scalar to the right by the specified amount of bits.
			OM_FORCE_INLINE SIMDScalar operator >> ( Int32 bitShift ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sra( vi, (Int32x4)(bitShift) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_srai_epi32( vi, bitShift ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vshr_n_s32( vi, bitShift ) );
#else
				return SIMDScalar( a >> bitShift, b >> bitShift, c >> bitShift, d >> bitShift );
#endif
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate a scalar.
			OM_FORCE_INLINE SIMDScalar operator - () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( (Int32x4)(0), vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi32( _mm_set1_epi32(0), vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vnegq_s32( vi ) );
#else
				return SIMDScalar( -a, -b, -c, -d );
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
				return SIMDScalar( _mm_add_epi32( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vaddq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a + scalar.a, b + scalar.b, c + scalar.c, d + scalar.d );
#endif
			}
			
			
			/// Add a value to every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator + ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_add( vi, (Int32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_epi32( vi, _mm_set1_epi32(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vaddq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a + value, b + value, c + value, d + value );
#endif
			}
			
			
			/// Subtract a scalar from this scalar component-wise and return the result.
			OM_FORCE_INLINE SIMDScalar operator - ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi32( vi, scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vsubq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a - scalar.a, b - scalar.b, c - scalar.c, d - scalar.d );
#endif
			}
			
			
			/// Subtract a value from every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator - ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( vi, (Int32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_epi32( vi, _mm_set1_epi32(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vsubq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a - value, b - value, c - value, d - value );
#endif
			}
			
			
			/// Multiply component-wise this scalar and another scalar.
			OM_FORCE_INLINE SIMDScalar operator * ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vmulq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( a*scalar.a, b*scalar.b, c*scalar.c, d*scalar.d );
#endif
			}
			
			
			/// Multiply every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator * ( const Int32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vmulq_s32( vi, vdupq_n_s32(value) ) );
#else
				return SIMDScalar( a*value, b*value, c*value, d*value );
#endif
			}
			
			
			/// Divide this scalar by another scalar component-wise.
			OM_FORCE_INLINE SIMDScalar operator / ( const SIMDScalar& scalar ) const
			{
				return SIMDScalar( a/scalar.a, b/scalar.b, c/scalar.c, d/scalar.d );
			}
			
			
			/// Divide every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator / ( const Int32 value ) const
			{
				return SIMDScalar( a/value, b/value, c/value, d/value );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator += ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_add( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_add_epi32( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vaddq_s32( vi, scalar.vi );
#else
				a += scalar.a; b += scalar.b; c += scalar.c; d += scalar.d;
#endif
				return *this;
			}
			
			
			/// Subtract a scalar from this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator -= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_sub( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_sub_epi32( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vsubq_s32( vi, scalar.vi );
#else
				a -= scalar.a; b -= scalar.b; c -= scalar.c; d -= scalar.d;
#endif
				return *this;
			}
			
			
			/// Multiply component-wise this scalar and another scalar and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator *= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vmulq_s32( vi, scalar.vi );
#else
				a *= scalar.a; b *= scalar.b; c *= scalar.c; d *= scalar.d;
#endif
				return *this;
			}
			
			
			/// Divide this scalar by another scalar component-wise and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator /= ( const SIMDScalar& scalar )
			{
				a /= scalar.a; b /= scalar.b; c /= scalar.c; d /= scalar.d;
				return *this;
			}
			
			
		//********************************************************************************
		//******	Required Alignment Accessor Methods
			
			
			/// Return the alignment required for objects of this type.
			/**
			  * For most SIMD types this value will be 16 bytes. If there is
			  * no alignment required, 1 is returned.
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
				
				buffer << "(" << a << ", " << b << ", " << c << ", " << d << ")";
				
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
			static const Size WIDTH = SIMDTypeN<Int32,4>::WIDTH;
			
			
			/// The required alignment of this scalar type.
			static const Size ALIGNMENT = SIMDTypeN<Int32,4>::ALIGNMENT;


			union OM_ALIGN(16)
			{
				/// The platform-specific vector to use for 4 32-bit floats.
				Float32x4 vf;
				
				/// The platform-specific vector to use for 4 32-bit integers.
				Int32x4 vi;
				
				/// The components of a 4D SIMD scalar in array format.
				Int32 x[4];
				
				struct
				{
					/// The A component of a 4D SIMD scalar.
					Int32 a;
					
					/// The B component of a 4D SIMD scalar.
					Int32 b;
					
					/// The C component of a 4D SIMD scalar.
					Int32 c;
					
					/// The D component of a 4D SIMD scalar.
					Int32 d;
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
OM_FORCE_INLINE SIMDScalar<Int32,4> abs( const SIMDScalar<Int32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_abs( scalar.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE)
	return SIMDScalar<Int32,4>( math::abs(scalar.a), math::abs(scalar.b), math::abs(scalar.c), math::abs(scalar.d) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Int32,4>( vabsq_s32( scalar.vi ) );
#else
	return SIMDScalar<Int32,4>( math::abs(scalar.a), math::abs(scalar.b), math::abs(scalar.c), math::abs(scalar.d) );
#endif
}



/// Compute the minimum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Int32,4> min( const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_min( scalar1.v, scalar2.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE)
	return SIMDScalar<Int32,4>( math::min(scalar1.a, scalar2.a), math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c), math::min(scalar1.d, scalar2.d) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Int32,4>( vminq_s32( scalar1.v, scalar2.vi ) );
#else
	return SIMDScalar<Int32,4>( math::min(scalar1.a, scalar2.a), math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c), math::min(scalar1.d, scalar2.d) );
#endif
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Int32,4> max( const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_max( scalar1.v, scalar2.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE)
	return SIMDScalar<Int32,4>( math::max(scalar1.a, scalar2.a), math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c), math::max(scalar1.d, scalar2.d) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Int32,4>( vmaxq_s32( scalar1.v, scalar2.vi ) );
#else
	return SIMDScalar<Int32,4>( math::max(scalar1.a, scalar2.a), math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c), math::max(scalar1.d, scalar2.d) );
#endif
}




/// Pick 4 elements from the specified SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
OM_FORCE_INLINE SIMDScalar<Int32,4> shuffle( const SIMDScalar<Int32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Int32,4>( _mm_shuffle_ps( scalar.vf, scalar.vf, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Int32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#endif
}




/// Pick two elements from each SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
OM_FORCE_INLINE SIMDScalar<Int32,4> shuffle( const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Int32,4>( _mm_shuffle_ps( scalar1.vf, scalar2.vf, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#else
	return SIMDScalar<Int32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
OM_FORCE_INLINE SIMDScalar<Int32,4> select( const SIMDScalar<Int32,4>& selector,
												const SIMDScalar<Int32,4>& scalar1, const SIMDScalar<Int32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( vec_sel( scalar2.v, scalar1.v, selector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Int32,4>( _mm_xor_ps( scalar2.vf,
										_mm_and_ps( selector.vf,
													_mm_xor_ps( scalar1.vf, scalar2.vf ) ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Int32,4>( vbslq_s32( selector.v, scalar1.v, scalar2.vi ) );
#else
	return SIMDScalar<Int32,4>( selector.a ? scalar1.a : scalar2.a, selector.b ? scalar1.b : scalar2.b,
								selector.c ? scalar1.c : scalar2.c, selector.d ? scalar1.d : scalar2.d );
#endif
}




/// Copy the first and third element of the specified SIMD scalar into the second and fourth places.
OM_FORCE_INLINE SIMDScalar<Int32,4> lows( const SIMDScalar<Int32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( scalar.a, scalar.a, scalar.c, scalar.c );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	return SIMDScalar<Int32,4>( _mm_moveldup_ps( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vcombine_s32( vdup_lane_s32( vget_low_s32( scalar.vi ), 0 ),
												vdup_lane_s32( vget_high_s32( scalar.vi ), 0 )) );
#else
	return SIMDScalar<Int32,4>( scalar.a, scalar.a, scalar.c, scalar.c );
#endif
}




/// Copy the second and fourth element of the specified SIMD scalar into the first and third places.
OM_FORCE_INLINE SIMDScalar<Int32,4> highs( const SIMDScalar<Int32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Int32,4>( scalar.b, scalar.b, scalar.d, scalar.d );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	return SIMDScalar<Int32,4>( _mm_movehdup_ps( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vcombine_s32( vdup_lane_s32( vget_low_s32( scalar.vi ), 1 ),
												vdup_lane_s32( vget_high_s32( scalar.vi ), 1 )) );
#else
	return SIMDScalar<Int32,4>( scalar.b, scalar.b, scalar.d, scalar.d );
#endif
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_SCALAR_INT_32_4_H
