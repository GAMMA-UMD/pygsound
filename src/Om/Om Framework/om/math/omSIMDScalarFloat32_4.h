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

#ifndef INCLUDE_OM_SIMD_SCALAR_FLOAT_32_4_H
#define INCLUDE_OM_SIMD_SCALAR_FLOAT_32_4_H


#include "omMathConfig.h"


#include "omVector3D.h"
#include "omVector4D.h"
#include "omSIMDScalar.h"
#include "omSIMDScalarInt32_4.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a 4-component 32-bit floating-point SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 4 32-bit floating-point values.
  */
template <>
class OM_ALIGN(16) SIMDScalar<Float32,4>
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
			
			
			/// Create a new 4D scalar with the specified raw float value.
			OM_FORCE_INLINE SIMDScalar( Float32x4 rawFloat32x4 )
				:	vf( rawFloat32x4 )
			{
			}
			
			
			/// Create a new 4D vector with the specified raw integer value, reinterpreting as floats.
			OM_FORCE_INLINE SIMDScalar( Int32x4 rawInt32x4 )
				:	vi( rawInt32x4 )
			{
			}
			
			
			/// Create a new 4D scalar with the specified 4D SIMD scalar value.
			OM_FORCE_INLINE SIMDScalar( const SIMDScalar<Int32,4>& simdScalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = (Float32x4)(simdScalar.vf);
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vf = _mm_cvtepi32_ps( simdScalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = vcvtq_f32_s32( simdScalar.vf );
#else
				a = simdScalar.a; b = simdScalar.b; c = simdScalar.c; d = simdScalar.d;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar with all elements equal to the specified value.
			OM_FORCE_INLINE SIMDScalar( Float32 value )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = (Float32x4)(value);
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_set1_ps( value );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = vdupq_n_f32( value );
#else
				a = b = c = d = value;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar with the elements equal to the specified 4 values.
			OM_FORCE_INLINE SIMDScalar( Float32 newA, Float32 newB, Float32 newC, Float32 newD )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = (Float32x4)( newA, newB, newC, newD );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				vf = _mm_set_ps( newD, newC, newB, newA );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				const Float32 abcd[4] = {newA, newB, newC, newD};
				vf = vld1q_f32( abcd );
#else
				a = newA; b = newB; c = newC; d = newD;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar with the first 3 elements equal to the specified vector's components.
			/**
			  * The last element of the SIMD scalar is initialized to 0.
			  */
			OM_FORCE_INLINE SIMDScalar( const VectorND<Float32,3>& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = (Float32x4)( vector.x, vector.y, vector.z, Float32(0) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				vf = _mm_set_ps( Float32(0), vector.z, vector.y, vector.x );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				const Float32 abcd[4] = {vector.x, vector.y, vector.z, Float32(0)};
				vf = vld1q_f32( abcd );
#else
				a = vector.x; b = vector.y; c = vector.z; d = 0;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar with the elements equal to the specified vector's components.
			OM_FORCE_INLINE SIMDScalar( const VectorND<Float32,4>& vector )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = (Float32x4)( vector.x, vector.y, vector.z, vector.w );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				vf = _mm_set_ps( vector.w, vector.z, vector.y, vector.x );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				const Float32 abcd[4] = {vector.x, vector.y, vector.z, vector.w};
				vf = vld1q_f32( abcd );
#else
				a = vector.x; b = vector.y; c = vector.z; d = vector.w;
#endif
			}
			
			
			/// Create a new 4D SIMD scalar from the first 4 values stored at specified aligned pointer's location.
			OM_FORCE_INLINE explicit SIMDScalar( const Float32* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				a = array[0]; b = array[1]; c = array[2]; d = array[3];
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_load_ps( array );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = vld1q_f32( array );
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
				vf = other.vf;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = other.vf;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = other.vf;
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
				vf = other.vf;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = other.vf;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = other.vf;
#else
				a = other.a; b = other.b; c = other.c; d = other.d;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Load Methods
			
			
			OM_FORCE_INLINE static SIMDScalar load( const Float32* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_load_ps( array ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vld1q_f32( array ) );
#else
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#endif
			}
			
			
			OM_FORCE_INLINE static SIMDScalar loadUnaligned( const Float32* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_loadu_ps( array ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vld1q_f32( array ) );
#else
				return SIMDScalar( array[0], array[1], array[2], array[3] );
#endif
			}
			
			
		//********************************************************************************
		//******	Store Methods
			
			
			OM_FORCE_INLINE void store( Float32* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				_mm_store_ps( destination, vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vst1q_f32( destination, vf );
#else
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#endif
			}
			
			
			OM_FORCE_INLINE void storeUnaligned( Float32* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				_mm_storeu_ps( destination, vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vst1q_f32( destination, vf );
#else
				destination[0] = a; destination[1] = b; destination[2] = c; destination[3] = d;
#endif
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Get a reference to the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Float32& operator [] ( Index i )
			{
				return xf[i];
			}
			
			
			/// Get the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Float32 operator [] ( Index i ) const
			{
				return xf[i];
			}
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			OM_FORCE_INLINE const Float32* toArray() const
			{
				return xf;
			}
			
			
			/// Convert this scalar to a 3D vector using the first 3 components.
			OM_FORCE_INLINE operator VectorND<Float32,3> () const
			{
				return VectorND<Float32,3>( xf[0], xf[1], xf[2] );
			}
			
			
			/// Convert this scalar to a 4D vector.
			OM_FORCE_INLINE operator VectorND<Float32,4> () const
			{
				return VectorND<Float32,4>( xf[0], xf[1], xf[2], xf[3] );
			}
			
			
			/// Convert this scalar to a 4D integer SIMD scalar.
			OM_FORCE_INLINE operator SIMDScalar<Int32,4> () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( (SIMDScalar<Int32,4>::Int32x4)v );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int32,4>( _mm_cvttps_epi32( vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcvtq_s32_f32( vf ) );
#else
				return SIMDScalar<Int32,4>( (Int32)a, (Int32)b, (Int32)c, (Int32)d );
#endif
			}
			
			
		//********************************************************************************
		//******	Logical Operators
			
			
			/// Compute the bitwise AND of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator & ( const SIMDScalar<Int32,4>& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_and( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_and_si128( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_and_ps( vf, vector.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vandq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( xi[0] & vector.a, xi[1] & vector.b, xi[2] & vector.c, xi[3] & vector.d );
#endif
			}
			
			
			/// Compute the bitwise OR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator | ( const SIMDScalar<Int32,4>& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_or( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_or_si128( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_or_ps( vf, vector.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vorrq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( xi[0] | vector.a, xi[1] | vector.b, xi[2] | vector.c, xi[3] | vector.d );
#endif
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and return the result.
			OM_FORCE_INLINE SIMDScalar operator ^ ( const SIMDScalar<Int32,4>& vector ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_xor( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_xor_si128( vi, vector.vi ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_xor_ps( vf, vector.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( veorq_s32( vi, scalar.vi ) );
#else
				return SIMDScalar( xi[0] ^ vector.a, xi[1] ^ vector.b, xi[2] ^ vector.c, xi[3] ^ vector.d );
#endif
			}
			
			
		//********************************************************************************
		//******	Logical Assignment Operators
			
			
			/// Compute the logical AND of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator &= ( const SIMDScalar<Int32,4>& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_and( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_and_si128( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_and_ps( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vandq_s32( vi, scalar.vi );
#else
				xi[0] &= scalar.a; xi[1] &= scalar.b; xi[2] &= scalar.c; xi[3] &= scalar.d;
#endif
				return *this;
			}
			
			
			/// Compute the logical OR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator |= ( const SIMDScalar<Int32,4>& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_or( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_or_si128( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_or_ps( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = vorrq_s32( vi, scalar.vi );
#else
				xi[0] |= scalar.a; xi[1] |= scalar.b; xi[2] |= scalar.c; xi[3] |= scalar.d;
#endif
				return *this;
			}
			
			
			/// Compute the bitwise XOR of this 4D SIMD vector with another and assign it to this vector.
			OM_FORCE_INLINE SIMDScalar& operator ^= ( const SIMDScalar<Int32,4>& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vi = vec_xor( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vi = _mm_xor_si128( vi, scalar.vi );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_xor_ps( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vi = veorq_s32( vi, scalar.vi );
#else
				xi[0] ^= scalar.a; xi[1] ^= scalar.b; xi[2] ^= scalar.c; xi[3] ^= scalar.d;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two 4D SIMD scalars component-wise for equality.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator == ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmpeq( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpeq_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vceqq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar<Int32,4>( a == scalar.a, b == scalar.b, c == scalar.c, d == scalar.d );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for equality.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator == ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmpeq( vf, (Float32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpeq_ps( vf, _mm_set1_ps( value ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vceqq_f32( vf, vdupq_n_f32(value) ) );
#else
				return SIMDScalar<Int32,4>( a == value, b == value, c == value, d == value );
#endif
			}
			
			
			/// Compare two 4D SIMD scalars component-wise for inequality
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator != ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				const OM_ALTIVEC_VECTOR bool temp = vec_cmpeq( vf, scalar.vf );
				return SIMDScalar<Int32,4>( vec_nor( temp, temp ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpneq_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vmvnq_s32( vceqq_f32( vf, scalar.vf ) ) );
#else
				return SIMDScalar<Int32,4>( a != scalar.a, b != scalar.b, c != scalar.c, d != scalar.d );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for inequality.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator != ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				const OM_ALTIVEC_VECTOR bool temp = vec_cmpeq( vf, (Float32x4)(value) );
				return SIMDScalar<Int32,4>( vec_nor( temp, temp ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpneq_ps( vf, _mm_set1_ps( value ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vmvnq_s32( vceqq_f32( vf, vdupq_n_f32(value) ) ) );
#else
				return SIMDScalar<Int32,4>( a != value, b != value, c != value, d != value );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator < ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmplt( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmplt_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcltq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar<Int32,4>( a < scalar.a, b < scalar.b, c < scalar.c, d < scalar.d );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator < ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmplt( vf, (Float32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmplt_ps( vf, _mm_set1_ps( value ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcltq_f32( vf, vdupq_n_f32( value ) ) );
#else
				return SIMDScalar<Int32,4>( a < value, b < value, c < value, d < value );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator > ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmpgt( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpgt_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcgtq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar<Int32,4>( a > scalar.a, b > scalar.b, c > scalar.c, d > scalar.d );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator > ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmpgt( vf, (Float32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpgt_ps( vf, _mm_set1_ps( value ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcgtq_f32( vf, vdupq_n_f32( value ) ) );
#else
				return SIMDScalar<Int32,4>( a > value, b > value, c > value, d > value );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator <= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmple( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmple_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcleq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar<Int32,4>( a <= scalar.a, b <= scalar.b, c <= scalar.c, d <= scalar.d );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator <= ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmple( vf, (Float32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmple_ps( vf, _mm_set1_ps( value ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcleq_f32( vf, vdupq_n_f32( value ) ) );
#else
				return SIMDScalar<Int32,4>( a <= value, b <= value, c <= value, d <= value );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator >= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmpge( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpge_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcgeq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar<Int32,4>( a >= scalar.a, b >= scalar.b, c >= scalar.c, d >= scalar.d );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDScalar<Int32,4> operator >= ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar<Int32,4>( vec_cmpge( vf, (Float32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar<Int32,4>( _mm_cmpge_ps( vf, _mm_set1_ps( value ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar<Int32,4>( vcgeq_f32( vf, vdupq_n_f32( value ) ) );
#else
				return SIMDScalar<Int32,4>( a >= value, b >= value, c >= value, d >= value );
#endif
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate a scalar.
			OM_FORCE_INLINE SIMDScalar operator - () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( (Float32x4)(Float32(0)), vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_sub_ps( _mm_set1_ps(Float32(0)), vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vnegq_f32( vf ) );
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
				return SIMDScalar( vec_add( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_add_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vaddq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar( a + scalar.a, b + scalar.b, c + scalar.c, d + scalar.d );
#endif
			}
			
			
			/// Add a value to every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator + ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_add( vf, (Float32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_add_ps( vf, _mm_set1_ps(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vaddq_f32( vf, vdupq_n_f32(value) ) );
#else
				return SIMDScalar( a + value, b + value, c + value, d + value );
#endif
			}
			
			
			/// Subtract a scalar from this scalar component-wise and return the result.
			OM_FORCE_INLINE SIMDScalar operator - ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_sub_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vsubq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar( a - scalar.a, b - scalar.b, c - scalar.c, d - scalar.d );
#endif
			}
			
			
			/// Subtract a value from every component of this scalar.
			OM_FORCE_INLINE SIMDScalar operator - ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_sub( vf, (Float32x4)(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_sub_ps( vf, _mm_set1_ps(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vsubq_f32( vf, vdupq_n_f32(value) ) );
#else
				return SIMDScalar( a - value, b - value, c - value, d - value );
#endif
			}
			
			
			/// Multiply component-wise this scalar and another scalar.
			OM_FORCE_INLINE SIMDScalar operator * ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_madd( vf, scalar.vf, (Float32x4)(float(0)) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_mul_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vmulq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar( a*scalar.a, b*scalar.b, c*scalar.c, d*scalar.d );
#endif
			}
			
			
			/// Multiply every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator * ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_madd( vf, (Float32x4)(value),
												(Float32x4)(Float32(0)) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_mul_ps( vf, _mm_set1_ps(value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vmulq_n_f32( vf, value ) );
#else
				return SIMDScalar( a*value, b*value, c*value, d*value );
#endif
			}
			
			
			/// Divide this scalar by another scalar component-wise.
			OM_FORCE_INLINE SIMDScalar operator / ( const SIMDScalar& scalar ) const
			{
#if GOM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				//Get the reciprocal estimate
				Float32x4 reciprocalEstimate = vec_re( scalar.vf );
				
				//One round of Newton-Raphson refinement
				Float32x4 reciprocal = vec_madd( vec_nmsub( reciprocalEstimate, vf, 
														(Float32x4)(1) ), reciprocalEstimate, reciprocalEstimate );
				
				return SIMDScalar( vec_madd( vf, reciprocal, ((Float32x4)(0)) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_div_ps( vf, scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vdivq_f32( vf, scalar.vf ) );
#else
				return SIMDScalar( a/scalar.a, b/scalar.b, c/scalar.c, d/scalar.d );
#endif
			}
			
			
			/// Divide every component of this scalar by a value and return the result.
			OM_FORCE_INLINE SIMDScalar operator / ( const Float32 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return SIMDScalar( vec_madd( vf, ((Float32x4)(Float32(1) / value)), ((Float32x4)(0)) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				return SIMDScalar( _mm_mul_ps( vf, _mm_set1_ps(Float32(1) / value) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return SIMDScalar( vdivq_f32( vf, vdupq_n_f32(Float32(1) / value) ) );
#else
				Float32 inverse = Float32(1) / value;
				return SIMDScalar( a*inverse, b*inverse, c*inverse, d*inverse );
#endif
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator += ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = vec_add( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_add_ps( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = vaddq_f32( vf, scalar.vf );
#else
				a += scalar.a; b += scalar.b; c += scalar.c; d += scalar.d;
#endif
				return *this;
			}
			
			/// Subtract a scalar from this scalar, modifying this original scalar.
			OM_FORCE_INLINE SIMDScalar& operator -= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = vec_sub( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_sub_ps( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = vsubq_f32( vf, scalar.vf );
#else
				a -= scalar.a; b -= scalar.b; c -= scalar.c; d -= scalar.d;
#endif
				return *this;
			}
			
			
			/// Multiply component-wise this scalar and another scalar and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator *= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				vf = vec_madd( vf, scalar.vf, (Float32x4)(Float32(0)) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_mul_ps( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = vmulq_f32( vf, scalar.vf );
#else
				a *= scalar.a; b *= scalar.b; c *= scalar.c; d *= scalar.d;
#endif
				return *this;
			}
			
			
			/// Divide this scalar by another scalar component-wise and modify this scalar.
			OM_FORCE_INLINE SIMDScalar& operator /= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				a /= scalar.a; b /= scalar.b; c /= scalar.c; d /= scalar.d;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
				vf = _mm_div_ps( vf, scalar.vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				vf = vdivq_f32( vf, scalar.vf );
#else
				a /= scalar.a; b /= scalar.b; c /= scalar.c; d /= scalar.d;
#endif
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
				const SIMDFlags flags = SIMDFlags::get();
				
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
				return (flags & SIMDFlags::ALTIVEC) != 0;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE)
				return (flags & SIMDFlags::SSE) != 0;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
				return (flags & SIMDFlags::ARM_NEON) != 0;
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
			static const Size WIDTH = SIMDTypeN<Float32,4>::WIDTH;
			
			
			/// The required alignment of this scalar type.
			static const Size ALIGNMENT = SIMDTypeN<Float32,4>::ALIGNMENT;
			
			
			union OM_ALIGN(16)
			{
				/// The platform-specific vector to use for 4 32-bit floats.
				Float32x4 vf;
				
				/// The platform-specific vector to use for 4 32-bit integers.
				Int32x4 vi;
				
				/// The components of a 4D SIMD scalar in array format.
				Float32 xf[4];
				
				/// The components of a 4D SIMD scalar in array format.
				Int32 xi[4];
				
				struct
				{
					/// The first component of a 4D SIMD scalar.
					Float32 a;
					
					/// The second component of a 4D SIMD scalar.
					Float32 b;
					
					/// The third component of a 4D SIMD scalar.
					Float32 c;
					
					/// The fourth component of a 4D SIMD scalar.
					Float32 d;
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
OM_FORCE_INLINE SIMDScalar<Float32,4> abs( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_abs( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	OM_ALIGN(16) const UInt32 absMask[4] = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
	return SIMDScalar<Float32,4>( _mm_and_ps( scalar.vf, _mm_load_ps( reinterpret_cast<const Float32*>(absMask) ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vabsq_f32( scalar.vf ) );
#else
	return SIMDScalar<Float32,4>( math::abs(scalar.a), math::abs(scalar.b), math::abs(scalar.c), math::abs(scalar.d) );
#endif
}




/// Compute the floor of each component of the specified SIMD scalar and return the result.
OM_FORCE_INLINE SIMDScalar<Float32,4> floor( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_floor( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(4,1)
	return SIMDScalar<Float32,4>( _mm_round_ps( scalar.vf, _MM_FROUND_TO_NEG_INF ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	// create vector 1.0f
	__m128i v0 = _mm_setzero_si128();
    __m128i v1 = _mm_cmpeq_epi32( v0, v0 );
    __m128i ji = _mm_srli_epi32( v1, 25 );
	ji = _mm_slli_epi32( ji, 23 );
    __m128 j = *(__m128*)&ji;
	
    __m128i i = _mm_cvttps_epi32( scalar.vf );
    __m128 fi = _mm_cvtepi32_ps( i );
    __m128 igx = _mm_cmpgt_ps( fi, scalar.vf );
    j = _mm_and_ps( igx, j );
    return SIMDScalar<Float32,4>( _mm_sub_ps( fi, j ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( math::floor(scalar.a), math::floor(scalar.b), math::floor(scalar.c), math::floor(scalar.d) );
#else
	return SIMDScalar<Float32,4>( math::floor(scalar.a), math::floor(scalar.b), math::floor(scalar.c), math::floor(scalar.d) );
#endif
}




/// Compute the ceiling of each component of the specified SIMD scalar and return the result.
OM_FORCE_INLINE SIMDScalar<Float32,4> ceiling( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_ceil( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(4,1)
	return SIMDScalar<Float32,4>( _mm_round_ps( scalar.vf, _MM_FROUND_TO_POS_INF ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	// create vector 1.0f
	__m128i v0 = _mm_setzero_si128();
    __m128i v1 = _mm_cmpeq_epi32(v0,v0);
    __m128i ji = _mm_srli_epi32( v1, 25 );
	ji = _mm_slli_epi32( ji, 23 );
    __m128 j = *(__m128*)&ji;
	
    __m128i i = _mm_cvttps_epi32( scalar.vf );
    __m128 fi = _mm_cvtepi32_ps( i );
    __m128 igx = _mm_cmplt_ps( fi, scalar.vf );
    j = _mm_and_ps( igx, j );
    return SIMDScalar<Float32,4>( _mm_add_ps( fi, j ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( math::ceiling(scalar.a), math::ceiling(scalar.b), math::ceiling(scalar.c), math::ceiling(scalar.d) );
#else
	return SIMDScalar<Float32,4>( math::ceiling(scalar.a), math::ceiling(scalar.b), math::ceiling(scalar.c), math::ceiling(scalar.d) );
#endif
}




/// Return an approximate reciprocal of the specified value with 23 bits of precision.
OM_FORCE_INLINE SIMDScalar<Float32,4> reciprocal( const SIMDScalar<Float32,4>& v )
{
#if OM_USE_SIMD && (defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0))
	// Compute reciprocal approximation, 12 bits of precision.
	SIMDScalar<Float32,4> rcp( _mm_rcp_ps(v.vf) );
	
	// One iteration of newton-raphson increases to 23 bits of precision.
	return (rcp + rcp) - v*(rcp*rcp);
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	// Compute reciprocal approximation.
	SIMDScalar<Float32,4> rcp( vrecpeq_f32(v.vf) );
	
	// One iteration of newton-raphson increases the precision.
	SIMDScalar<Float32,4> rcp2( vrecpsq_f32(v.vf, rcp) );
	return rcp2*rcp;
#else
	return 1.0f / v;
#endif
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
OM_FORCE_INLINE SIMDScalar<Float32,4> sqrt( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	// Get the square root reciprocal estimate
	OM_ALTIVEC_VECTOR float zero = (OM_ALTIVEC_VECTOR float)(0);
	OM_ALTIVEC_VECTOR float oneHalf = (OM_ALTIVEC_VECTOR float)(0.5);
	OM_ALTIVEC_VECTOR float one = (OM_ALTIVEC_VECTOR float)(1.0);
	OM_ALTIVEC_VECTOR float estimate = vec_rsqrte( scalar.vf );
	
	// One round of Newton-Raphson refinement
	OM_ALTIVEC_VECTOR float estimateSquared = vec_madd( estimate, estimate, zero );
	OM_ALTIVEC_VECTOR float halfEstimate = vec_madd( estimate, oneHalf, zero );
	
	OM_ALTIVEC_VECTOR float reciprocalSquareRoot = vec_madd( vec_nmsub( scalar.vf, estimateSquared, one ), halfEstimate, estimate );
	
	return SIMDScalar<Float32,4>( vec_madd( scalar.vf, reciprocalSquareRoot, (OM_ALTIVEC_VECTOR float)(0) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_sqrt_ps( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vrsqrtsq_f32( scalar.vf ) );
#else
	return SIMDScalar<Float32,4>( math::sqrt(scalar.a), math::sqrt(scalar.b), math::sqrt(scalar.c), math::sqrt(scalar.d) );
#endif
}




/// Return an approximate reciprocal square root of the specified value with 23 bits of precision.
OM_FORCE_INLINE SIMDScalar<Float32,4> reciprocalSqrt( const SIMDScalar<Float32,4>& v )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	// Get the square root reciprocal estimate
	OM_ALTIVEC_VECTOR float zero = (OM_ALTIVEC_VECTOR float)(0);
	OM_ALTIVEC_VECTOR float oneHalf = (OM_ALTIVEC_VECTOR float)(0.5);
	OM_ALTIVEC_VECTOR float one = (OM_ALTIVEC_VECTOR float)(1.0);
	OM_ALTIVEC_VECTOR float estimate = vec_rsqrte( scalar.vf );
	
	// One round of Newton-Raphson refinement
	OM_ALTIVEC_VECTOR float estimateSquared = vec_madd( estimate, estimate, zero );
	OM_ALTIVEC_VECTOR float halfEstimate = vec_madd( estimate, oneHalf, zero );
	
	return SIMDScalar<Float32,4>( vec_madd( vec_nmsub( scalar.vf, estimateSquared, one ), halfEstimate, estimate ) );
#elif OM_USE_SIMD && (defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0))
	// Compute reciprocal sqrt approximation, 12 bits of precision.
	SIMDScalar<Float32,4> rcpSqrt( _mm_rsqrt_ps(v.vf) );
	
	// One iteration of newton-raphson increases to 23 bits of precision.
	return 0.5f*rcpSqrt * (3.0f - v*(rcpSqrt*rcpSqrt));
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	// Compute reciprocal approximation.
	SIMDScalar<Float32,4> rcpSqrt( vrsqrteq_f32(v.vf) );
	
	// One iteration of newton-raphson increases the precision.
	SIMDScalar<Float32,4> rcpSqrt2 = rcpSqrt*v;
	return rcpSqrt*SIMDScalar<Float32,4>( vrsqrtsq_f32(rcpSqrt2, rcpSqrt) );
#else
	return 1.0f / math::sqrt(v);
#endif
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
OM_FORCE_INLINE SIMDScalar<Float32,4> fastSqrt( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return scalar*math::reciprocalSqrt( scalar );
#else
	return math::sqrt( scalar );
#endif
}




/// Pick 4 elements from the specified SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
OM_FORCE_INLINE SIMDScalar<Float32,4> shuffle( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_shuffle_ps( scalar.vf, scalar.vf, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#else
	return SIMDScalar<Float32,4>( scalar[i1], scalar[i2], scalar[i3], scalar[i4] );
#endif
}




/// Pick two elements from each SIMD scalar and return the result.
template < UInt i1, UInt i2, UInt i3, UInt i4 >
OM_FORCE_INLINE SIMDScalar<Float32,4> shuffle( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_shuffle_ps( scalar1.vf, scalar2.vf, _MM_SHUFFLE(i4, i3, i2, i1) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#else
	return SIMDScalar<Float32,4>( scalar1[i1], scalar1[i2], scalar2[i3], scalar2[i4] );
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
OM_FORCE_INLINE SIMDScalar<Float32,4> select( const SIMDScalar<Int32,4>& selector,
												const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_sel( scalar2.vf, scalar1.vf, selector.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Float32,4>( _mm_xor_ps( scalar2.vf, _mm_and_ps( selector.vf, _mm_xor_ps( scalar1.vf, scalar2.vf ) ) ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vbslq_f32( selector.vf, scalar1.vf, scalar2.vf ) );
#else
	return SIMDScalar<Float32,4>( selector.a ? scalar1.a : scalar2.a, selector.b ? scalar1.b : scalar2.b,
								selector.c ? scalar1.c : scalar2.c, selector.d ? scalar1.d : scalar2.d );
#endif
}




/// Copy the first and third element of the specified SIMD scalar into the second and fourth places.
OM_FORCE_INLINE SIMDScalar<Float32,4> lows( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( scalar.a, scalar.a, scalar.c, scalar.c );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	return SIMDScalar<Float32,4>( _mm_moveldup_ps( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vcombine_f32( vdup_lane_f32( vget_low_f32( scalar.vf ), 0 ),
												vdup_lane_f32( vget_high_f32( scalar.vf ), 0 )) );
#else
	return SIMDScalar<Float32,4>( scalar.a, scalar.a, scalar.c, scalar.c );
#endif
}




/// Copy the second and fourth element of the specified SIMD scalar into the first and third places.
OM_FORCE_INLINE SIMDScalar<Float32,4> highs( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( scalar.b, scalar.b, scalar.d, scalar.d );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	return SIMDScalar<Float32,4>( _mm_movehdup_ps( scalar.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vcombine_f32( vdup_lane_f32( vget_low_f32( scalar.vf ), 1 ),
												vdup_lane_f32( vget_high_f32( scalar.vf ), 1 )) );
#else
	return SIMDScalar<Float32,4>( scalar.b, scalar.b, scalar.d, scalar.d );
#endif
}




/// Interleave the first two elements of each scalar.
OM_FORCE_INLINE SIMDScalar<Float32,4> interleaveLows( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_mergeh( scalar1.vf, scalar2.vf ) ); // opposite due to endianness.
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_unpacklo_ps( scalar1.vf, scalar2.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	const float32x2x2_t result = vzip_f32( vget_low_f32( scalar1.vf ), vget_low_f32( scalar2.vf ) )) );
	return SIMDScalar<Float32,4>( vcombine_f32( result.vfal[0], result.vfal[1] ) );
#else
	return SIMDScalar<Float32,4>( scalar1.a, scalar2.a, scalar1.b, scalar2.b );
#endif
}




/// Interleave the last two elements of each scalar.
OM_FORCE_INLINE SIMDScalar<Float32,4> interleaveHighs( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_mergel( scalar1.vf, scalar2.vf ) ); // opposite due to endianness.
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_unpackhi_ps( scalar1.vf, scalar2.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	const float32x2x2_t result = vzip_f32( vget_high_f32( scalar1.vf ), vget_high_f32( scalar2.vf ) )) );
	return SIMDScalar<Float32,4>( vcombine_f32( result.vfal[0], result.vfal[1] ) );
#else
	return SIMDScalar<Float32,4>( scalar1.c, scalar2.c, scalar1.d, scalar2.d );
#endif
}




/// Subtract the first and third elements and add the second and fourth.
OM_FORCE_INLINE SIMDScalar<Float32,4> subAdd( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( scalar1.a - scalar2.a, scalar1.b + scalar2.b, scalar1.c - scalar2.c, scalar1.d + scalar2.d );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	return SIMDScalar<Float32,4>( _mm_addsub_ps( scalar1.vf, scalar2.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( scalar1.a - scalar2.a, scalar1.b + scalar2.b, scalar1.c - scalar2.c, scalar1.d + scalar2.d );
#else
	return SIMDScalar<Float32,4>( scalar1.a - scalar2.a, scalar1.b + scalar2.b, scalar1.c - scalar2.c, scalar1.d + scalar2.d );
#endif
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Float32,4> min( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_min( scalar1.vf, scalar2.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_min_ps( scalar1.vf, scalar2.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vminq_f32( scalar1.vf, scalar2.vf ) );
#else
	return SIMDScalar<Float32,4>( math::min(scalar1.a, scalar2.a), math::min(scalar1.b, scalar2.b),
								math::min(scalar1.c, scalar2.c), math::min(scalar1.d, scalar2.d) );
#endif
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Float32,4> max( const SIMDScalar<Float32,4>& scalar1, const SIMDScalar<Float32,4>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar<Float32,4>( vec_max( scalar1.vf, scalar2.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return SIMDScalar<Float32,4>( _mm_max_ps( scalar1.vf, scalar2.vf ) );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	return SIMDScalar<Float32,4>( vmaxq_f32( scalar1.vf, scalar2.vf ) );
#else
	return SIMDScalar<Float32,4>( math::max(scalar1.a, scalar2.a), math::max(scalar1.b, scalar2.b),
								math::max(scalar1.c, scalar2.c), math::max(scalar1.d, scalar2.d) );
#endif
}




/// Compute the minimum component of the specified SIMD scalar and return the wide result.
OM_FORCE_INLINE SIMDScalar<Float32,4> min( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && (defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0))
	// Shuffle the value once to find the minimum of 0 & 2, 1 & 3.
	SIMDScalar<Float32,4> scalar2 = math::shuffle<2,3,0,1>( scalar );
	scalar2 = math::min( scalar, scalar2 );
	
	// Shuffle the values again to determine the minimum value.
	SIMDScalar<Float32,4> scalar3 = math::shuffle<1,0,3,2>( scalar2 );
	return math::min( scalar2, scalar3 );
#else
	return SIMDScalar<Float32,4>( math::min( scalar.a, math::min( scalar.b, math::min(scalar.c, scalar.d) ) ) );
#endif
}




/// Compute the maximum component of the specified SIMD scalar and return the wide result.
OM_FORCE_INLINE SIMDScalar<Float32,4> max( const SIMDScalar<Float32,4>& scalar )
{
#if OM_USE_SIMD && (defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0))
	// Shuffle the value once to find the minimum of 0 & 2, 1 & 3.
	SIMDScalar<Float32,4> scalar2 = math::shuffle<2,3,0,1>( scalar );
	scalar2 = math::max( scalar, scalar2 );
	
	// Shuffle the values again to determine the minimum value.
	SIMDScalar<Float32,4> scalar3 = math::shuffle<1,0,3,2>( scalar2 );
	return math::max( scalar2, scalar3 );
#else
	return SIMDScalar<Float32,4>( math::max( scalar.a, math::max( scalar.b, math::max(scalar.c, scalar.d) ) ) );
#endif
}




/// Return the horizontal sum of a vector as a wide vector.
OM_FORCE_INLINE SIMDScalar<Float32,4> sum( const SIMDScalar<Float32,4>& v )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return SIMDScalar( v.a + v.b + v.c + v.d );
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	SIMDScalar<Float32,4> temp = v + math::shuffle<3,2,1,0>( v );
	return temp + math::shuffle<2,3,0,1>( temp );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	const float32x2_t temp = vadd_f32( vget_high_f32(v), vget_low_f32(v) );
	return SIMDScalar<Float32,4>( vdupq_lane_f32( vpadd_f32( r, r ), 0 ) );
#else
	return SIMDScalar<Float32,4>( v.a + v.b + v.c + v.d );
#endif
}




/// Return the horizontal sum of a vector as a wide vector.
OM_FORCE_INLINE Float32 sumScalar( const SIMDScalar<Float32,4>& v )
{
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
	return v.a + v.b + v.c + v.d;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
	return _mm_cvtss_f32( math::sum( v ).vf );
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
	const float32x2_t temp = vadd_f32( vget_high_f32(v), vget_low_f32(v) );
	return vget_lane_f32( vpadd_f32( r, r ), 0 );
#else
	return v.a + v.b + v.c + v.d;
#endif
}




//##########################################################################################
//##########################################################################################
//############		
//############		Fancy Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute and return the natural logarithm of the specified vector.
OM_FORCE_INLINE SIMDScalar<Float32,4> ln( const SIMDScalar<Float32,4>& v )
{
	typedef SIMDScalar<Float32,4> V;
	typedef SIMDScalar<Int32,4> Vi;
	V one( 1.0f );
	Vi invalid_mask = (v <= V(0.0f));
	
	// cut off denormalized stuff
	V x = math::max( v, V(Vi(0x00800000).vf) ); // The smallest non denormalized float number
	Vi emm0 = Vi(x.vf) >> 23;
	
	// keep only the fractional part
	x = x & Vi(~0x7f800000);
	x = x | Vi(V(0.5f).vi);
	
	emm0 = emm0 - Vi(0x7f);
	V e( emm0 );
	e = e + one;
	
	Vi mask = x < V(0.707106781186547524f);
	V tmp = x & mask;
	x = x - one;
	e = e - (one & mask);
	x = x + tmp;
	
	V z = x * x;
	V y( 7.0376836292e-2f );
	y = y * x;
	y = y + V(-1.1514610310e-1f);
	y = y * x;
	y = y + V(1.1676998740e-1f);
	y = y * x;
	y = y + V(-1.2420140846e-1f);
	y = y * x;
	y = y + V(1.4249322787e-1f);
	y = y * x;
	y = y + V(-1.6668057665e-1f);
	y = y * x;
	y = y + V(2.0000714765e-1f);
	y = y * x;
	y = y + V(-2.4999993993e-1f);
	y = y * x;
	y = y + V(3.3333331174e-1f);
	y = y * x;
	y = y * z;
	
	tmp = e * V(-2.12194440e-4f);
	y = y + tmp;
	
	tmp = z * V(0.5f);
	y = y - tmp;
	
	tmp = e * V(0.693359375f);
	x = x + y;
	x = x + tmp;
	x = x | invalid_mask; // negative arg will be NAN
	
	return x;
}




/// Compute and return the base-10 logarithm of the specified vector.
OM_FORCE_INLINE SIMDScalar<Float32,4> log10( const SIMDScalar<Float32,4>& v )
{
	// ln(v) / ln(10)
	return math::ln(v) * SIMDScalar<Float32,4>(0.43429448190325182765f);
}




/// Compute and return e raised to the specified vector power.
/**
  * Original SSE implementation by Julien Pommier, Copyright (C) 2007, z-lib license,
  * adapted to this codebase.
  */
OM_FORCE_INLINE SIMDScalar<Float32,4> exp( const SIMDScalar<Float32,4>& v )
{
	typedef SIMDScalar<Float32,4> V;
	typedef SIMDScalar<Int32,4> Vi;
	V one( 1.0f );
	V x = math::min( v.vf, V(88.3762626647949f) );
	x = math::max( x, V(-88.3762626647949f) );
	
	// Express exp(x) as exp(g + n*log(2))
	V fx = x * V(1.44269504088896341f);
	fx = fx + V(0.5f);
	fx  = math::floor(fx);
	V tmp = fx * V(0.693359375f);
	V z = fx * V(-2.12194440e-4f);
	x = x - tmp;
	x = x - z;
	z = x * x;
	
	// Evaluate polynomial.
	V y( 1.9875691500E-4f );
	y = y * x;
	y = y + V( 1.3981999507E-3f );
	y = y * x;
	y = y + V( 8.3334519073E-3f );
	y = y * x;
	y = y + V( 4.1665795894E-2f );
	y = y * x;
	y = y + V( 1.6666665459E-1f );
	y = y * x;
	y = y + V( 5.0000001201E-1f );
	y = y * z;
	y = y + x;
	y = y + one;
	
	// Build 2^n
	Vi emm0 = (Vi)fx;
	emm0 = emm0 + Vi(0x7f);
	emm0 = emm0 << 23;
	V pow2n( emm0.vf );
	y = y * pow2n;
	
	return y;
}




/// Compute and return the power of the specified vectors.
OM_FORCE_INLINE SIMDScalar<Float32,4> pow( const SIMDScalar<Float32,4>& base, const SIMDScalar<Float32,4>& power )
{/*
	// Doesn't work for all cases (e.g. base < 0, power = integer)
	// Invalid if base < 0 and power is not an integer.
	SIMDScalar<Int32,4> invalidMask = ((base < SIMDScalar<Float32,4>(0.0f)) & (math::floor( power ) != power));
	
	return math::exp( math::ln(base) * power ) | invalidMask;*/
	return SIMDScalar<Float32,4>( math::pow(base.a,power.a), math::pow(base.b,power.b),
								math::pow(base.c,power.c), math::pow(base.d,power.d) );
}




/// Compute and return the sine of the specified vector.
/**
  * Original SSE implementation by Julien Pommier, Copyright (C) 2007, z-lib license,
  * adapted to this codebase.
  */
OM_FORCE_INLINE SIMDScalar<Float32,4> sin( const SIMDScalar<Float32,4>& v )
{
	typedef SIMDScalar<Float32,4> V;
	typedef SIMDScalar<Int32,4> Vi;
	
	// take the absolute value.
	V x = math::abs(v);
	
	// extract the sign bit (upper one).
	Vi sign_bit = Vi(v.vi) & Vi((int)0x80000000);
	
	// scale by 4/Pi.
	V y = x * V(1.27323954473516f);
	
	// store the integer part of y in mm0.
	Vi emm2( y );
	
	// j=(j+1) & (~1) (see the cephes sources).
	emm2 = emm2 + Vi(1);
	emm2 = emm2 & Vi(~1);
	y = V( emm2 );
	
	// get the swap sign flag.
	Vi emm0 = emm2 & Vi(4);
	emm0 = emm0 << 29;
	sign_bit = sign_bit ^ emm0;
	
	// get the polynom selection mask. there is one polynom for 0 <= x <= Pi/4
    // and another one for Pi/4<x<=Pi/2. Both branches will be computed.
	emm2 = emm2 & Vi(2);
	emm2 = emm2 == Vi(0);
	
	// The magic pass: "Extended precision modular arithmetic"
	// x = ((x - y * DP1) - y * DP2) - y * DP3;
	V xmm1 = y * V(-0.78515625f);
	V xmm2 = y * V(-2.4187564849853515625e-4f);
	V xmm3 = y * V(-3.77489497744594108e-8f);
	x = x + xmm1;
	x = x + xmm2;
	x = x + xmm3;
	
	// Evaluate the first polynom  (0 <= x <= Pi/4)
	y = V(2.443315711809948e-5f);
	V z = x * x;
	y = y * z;
	y = y + V(-1.388731625493765e-3f);
	y = y * z;
	y = y + V(4.166664568298827e-2f);
	y = y * z;
	y = y * z;
	V tmp = z * V(0.5f);
	y = y - tmp;
	y = y + V(1.0f);
	
	// Evaluate the second polynom  (Pi/4 <= x <= 0)
	V y2( -1.9515295891e-4f );
	y2 = y2 * z;
	y2 = y2 + V(8.3321608736e-3f);
	y2 = y2 * z;
	y2 = y2 + V(-1.6666654611e-1f);
	y2 = y2 * z;
	y2 = y2 * x;
	y2 = y2 + x;
	
	// select the correct result from the two polynoms. 
	y2 = y2 & emm2;
	y = y & ~emm2;
	y = y + y2;
	
	// update the sign.
	return y ^ sign_bit;
}




/// Compute and return the cosine of the specified vector.
/**
  * Original SSE implementation by Julien Pommier, Copyright (C) 2007, z-lib license,
  * adapted to this codebase.
  */
OM_FORCE_INLINE SIMDScalar<Float32,4> cos( const SIMDScalar<Float32,4>& v )
{
	typedef SIMDScalar<Float32,4> V;
	typedef SIMDScalar<Int32,4> Vi;
	
	// take the absolute value.
	V x = math::abs(v);
	
	// scale by 4/Pi.
	V y = x * V(1.27323954473516f);
	
	// store the integer part of y in mm0.
	Vi emm2( y );
	
	// j=(j+1) & (~1) (see the cephes sources).
	emm2 = emm2 + Vi(1);
	emm2 = emm2 & Vi(~1);
	y = V( emm2 );
	emm2 = emm2 - Vi(2);
	
	// get the swap sign flag.
	Vi emm0 = ~emm2 & Vi(4);
	emm0 = emm0 << 29;
	
	// get the polynom selection mask.
	emm2 = emm2 & Vi(2);
	emm2 = emm2 == Vi(0);
	
	// The magic pass: "Extended precision modular arithmetic"
	// x = ((x - y * DP1) - y * DP2) - y * DP3;
	V xmm1 = y * V(-0.78515625f);
	V xmm2 = y * V(-2.4187564849853515625e-4f);
	V xmm3 = y * V(-3.77489497744594108e-8f);
	x = x + xmm1;
	x = x + xmm2;
	x = x + xmm3;
	
	// Evaluate the first polynom  (0 <= x <= Pi/4)
	y = V(2.443315711809948e-5f);
	V z = x * x;
	y = y * z;
	y = y + V(-1.388731625493765e-3f);
	y = y * z;
	y = y + V(4.166664568298827e-2f);
	y = y * z;
	y = y * z;
	V tmp = z * V(0.5f);
	y = y - tmp;
	y = y + V(1.0f);
	
	// Evaluate the second polynom  (Pi/4 <= x <= 0)
	V y2( -1.9515295891e-4f );
	y2 = y2 * z;
	y2 = y2 + V(8.3321608736e-3f);
	y2 = y2 * z;
	y2 = y2 + V(-1.6666654611e-1f);
	y2 = y2 * z;
	y2 = y2 * x;
	y2 = y2 + x;
	
	// select the correct result from the two polynoms. 
	y2 = y2 & emm2;
	y = y & ~emm2;
	y = y + y2;
	
	// update the sign.
	return y ^ emm0;
}




/// Compute the sine and cosine of the specified vector.
/**
  * Original SSE implementation by Julien Pommier, Copyright (C) 2007, z-lib license,
  * adapted to this codebase.
  */
OM_FORCE_INLINE void sinCos( const SIMDScalar<Float32,4>& v, SIMDScalar<Float32,4>& s, SIMDScalar<Float32,4>& c )
{
	typedef SIMDScalar<Float32,4> V;
	typedef SIMDScalar<Int32,4> Vi;
	
	// take the absolute value.
	V x = math::abs(v);
	
	// extract the sign bit (upper one).
	Vi sign_bit_sin = Vi(v.vi) & Vi((int)0x80000000);
	
	// scale by 4/Pi.
	V y = x * V(1.27323954473516f);
	
	// store the integer part of y in mm0.
	Vi emm2( y );
	
	// j=(j+1) & (~1) (see the cephes sources).
	emm2 = emm2 + Vi(1);
	emm2 = emm2 & Vi(~1);
	y = V( emm2 );
	Vi emm4 = emm2;
	
	// get the swap sign flag for the sine.
	Vi emm0 = emm2 & Vi(4);
	emm0 = emm0 << 29;
	sign_bit_sin = sign_bit_sin ^ emm0;
	
	// get the polynom selection mask for the sine.
	emm2 = emm2 & Vi(2);
	emm2 = emm2 == Vi(0);
	
	// get the swap sign flag for the cosine.
	emm4 = emm4 - Vi(2);
	emm4 = ~emm4 & Vi(4);
	Vi sign_bit_cos = emm4 << 29;
	
	// The magic pass: "Extended precision modular arithmetic"
	// x = ((x - y * DP1) - y * DP2) - y * DP3;
	V xmm1 = y * V(-0.78515625f);
	V xmm2 = y * V(-2.4187564849853515625e-4f);
	V xmm3 = y * V(-3.77489497744594108e-8f);
	x = x + xmm1;
	x = x + xmm2;
	x = x + xmm3;
	
	// Evaluate the first polynom  (0 <= x <= Pi/4)
	y = V(2.443315711809948e-5f);
	V z = x * x;
	y = y * z;
	y = y + V(-1.388731625493765e-3f);
	y = y * z;
	y = y + V(4.166664568298827e-2f);
	y = y * z;
	y = y * z;
	V tmp = z * V(0.5f);
	y = y - tmp;
	y = y + V(1.0f);
	
	// Evaluate the second polynom  (Pi/4 <= x <= 0)
	V y2( -1.9515295891e-4f );
	y2 = y2 * z;
	y2 = y2 + V(8.3321608736e-3f);
	y2 = y2 * z;
	y2 = y2 + V(-1.6666654611e-1f);
	y2 = y2 * z;
	y2 = y2 * x;
	y2 = y2 + x;
	
	// select the correct result from the two polynoms. 
	V ysin2 = y2 & emm2;
	V ysin1 = y & ~emm2;
	y2 = y2 - ysin2;
	y = y - ysin1;
	
	xmm1 = ysin1 + ysin2;
	xmm2 = y + y2;
	
	// update the sign.
	s = xmm1 ^ sign_bit_sin;
	c = xmm2 ^ sign_bit_cos;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Return the 3D cross product of two vectors, where each vector has the form (x,y,z,0).
OM_FORCE_INLINE SIMDScalar<Float32,4> cross( const SIMDScalar<Float32,4>& v1, const SIMDScalar<Float32,4>& v2 )
{
	const SIMDScalar<Float32,4> a0 = v1;
	const SIMDScalar<Float32,4> b0 = math::shuffle<1,2,0,3>( v2 );
	const SIMDScalar<Float32,4> a1 = math::shuffle<1,2,0,3>( v1 );
	const SIMDScalar<Float32,4> b1 = v2;
	return math::shuffle<1,2,0,3>( a0*b0 - a1*b1 );
}




/// Return the dot product of two vectors as a wide vector.
OM_FORCE_INLINE SIMDScalar<Float32,4> dot( const SIMDScalar<Float32,4>& v1, const SIMDScalar<Float32,4>& v2 )
{
	return math::sum( v1*v2 );
}




/// Return the dot product of two vectors as a scalar.
OM_FORCE_INLINE Float32 dotScalar( const SIMDScalar<Float32,4>& v1, const SIMDScalar<Float32,4>& v2 )
{
	return math::sumScalar( v1*v2 );
}




/// Normalize the specified vector using a fast approximate algorithm, accurate to ~23 bits.
OM_FORCE_INLINE SIMDScalar<Float32,4> normalize( const SIMDScalar<Float32,4>& v )
{
	return v * math::reciprocalSqrt( math::dot(v,v) );
}




/// Reconstruct the X and Y columns of a 3x3 matrix that has the given unit-length Z-column.
/**
  * The X and Y vectors form a basis for the plane with the Z vector as its normal.
  * The Z vector must be unit length.
  */
OM_FORCE_INLINE void planeBasis( const SIMDScalar<Float32,4>& z, SIMDScalar<Float32,4>& x, SIMDScalar<Float32,4>& y )
{
	SIMDScalar<Float32,4> n = math::abs( z );
	
	if ( n[0] < n[1] )
	{
		if ( n[0] < n[2] )
		{
			// X smallest.
			y = math::shuffle<3,2,1,3>( z ) * SIMDScalar<Float32,4>(0.0f,-1.0f,1.0f,0.0f);
		}
		else
		{
			// Z smallest.
			y = math::shuffle<1,3,0,3>( z ) * SIMDScalar<Float32,4>(-1.0f,0.0f,1.0f,0.0f);
		}
	}
	else if ( n[1] < n[2] )
	{
		// Y smallest.
		y = math::shuffle<2,3,0,3>( z ) * SIMDScalar<Float32,4>(-1.0f,0.0f,1.0f,0.0f);
	}
	else
	{
		// Z smallest.
		y = math::shuffle<1,3,0,3>( z ) * SIMDScalar<Float32,4>(-1.0f,0.0f,1.0f,0.0f);
	}
	
	y = math::normalize( y );
	x = math::cross( y, z );
}




/// Transpose a 4x4 matrix and place the result in the output parameters.
OM_FORCE_INLINE void transpose4x4( const SIMDScalar<Float32,4>& mx, const SIMDScalar<Float32,4>& my,
									const SIMDScalar<Float32,4>& mz, const SIMDScalar<Float32,4>& mw,
									SIMDScalar<Float32,4>& tx, SIMDScalar<Float32,4>& ty,
									SIMDScalar<Float32,4>& tz, SIMDScalar<Float32,4>& tw )
{
	const SIMDScalar<Float32,4> lowsXZ = math::interleaveLows( mx, mz );
	const SIMDScalar<Float32,4> highsXZ = math::interleaveHighs( mx, mz );
	const SIMDScalar<Float32,4> lowsYW = math::interleaveLows( my, mw );
	const SIMDScalar<Float32,4> highsYW = math::interleaveHighs( my, mw );
	tx = math::interleaveLows( lowsXZ, lowsYW );
	ty = math::interleaveHighs( lowsXZ, lowsYW );
	tz = math::interleaveLows( highsXZ, highsYW );
	tw = math::interleaveHighs( highsXZ, highsYW );
}




/// Transpose a 3x3 matrix and place the result in the output parameters.
OM_FORCE_INLINE void transpose3x3( const SIMDScalar<Float32,4>& mx, const SIMDScalar<Float32,4>& my, const SIMDScalar<Float32,4>& mz,
									SIMDScalar<Float32,4>& tx, SIMDScalar<Float32,4>& ty, SIMDScalar<Float32,4>& tz )
{
	const SIMDScalar<Float32,4> mw( 0.0f );
	const SIMDScalar<Float32,4> lowsXZ = math::interleaveLows( mx, mz );
	const SIMDScalar<Float32,4> highsXZ = math::interleaveHighs( mx, mz );
	const SIMDScalar<Float32,4> lowsYW = math::interleaveLows( my, mw );
	const SIMDScalar<Float32,4> highsYW = math::interleaveHighs( my, mw );
	tx = math::interleaveLows( lowsXZ, lowsYW );
	ty = math::interleaveHighs( lowsXZ, lowsYW );
	tz = math::interleaveLows( highsXZ, highsYW );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_SCALAR_FLOAT_32_4_H
