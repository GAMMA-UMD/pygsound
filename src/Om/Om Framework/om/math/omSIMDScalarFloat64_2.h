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

#ifndef INCLUDE_OM_SIMD_SCALAR_FLOAT_64_2_H
#define INCLUDE_OM_SIMD_SCALAR_FLOAT_64_2_H


#include "omMathConfig.h"


#include "omSIMDScalar.h"
#include "omSIMDScalarInt64_2.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a 2-component 64-bit floating-point SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses a 128-bit value to encode
  * 2 64-bit floating-point values. All basic arithmetic operations are supported,
  * plus a subset of standard scalar operations: abs(), min(), max(), sqrt().
  */
template <>
class OM_ALIGN(16) SIMDScalar<Float64,2>
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
			
			
			/// Create a new 2D scalar with the specified raw float value.
			OM_FORCE_INLINE SIMDScalar( Float64x2 rawFloat64x2 )
				:	vf( rawFloat64x2 )
			{
			}
			
			
			/// Create a new 2D scalar with the specified raw integer value, reinterpreting as floats.
			OM_FORCE_INLINE SIMDScalar( Int64x2 rawInt64x2 )
				:	vi( rawInt64x2 )
			{
			}
			
			
			/// Create a new 2D SIMD scalar with all elements equal to the specified value.
			OM_FORCE_INLINE SIMDScalar( Float64 value )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vf = _mm_set1_pd( value );
#else
				a = b = value;
#endif
			}
			
			
			/// Create a new 2D SIMD scalar with the elements equal to the specified 2 values.
			OM_FORCE_INLINE SIMDScalar( Float64 newA, Float64 newB )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				// The parameters are reversed to keep things consistent with loading from an address.
				vf = _mm_set_pd( newB, newA );
#else
				a = newA;
				b = newB;
#endif
			}
			
			
			/// Create a new 2D SIMD scalar from the first 4 values stored at specified pointer's location.
			OM_FORCE_INLINE explicit SIMDScalar( const Float64* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vf = _mm_load_pd( array );
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
				vf = other.vf;
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
				vf = other.vf;
#else
				a = other.a;
				b = other.b;
#endif
				return *this;
			}
			
			
		//********************************************************************************
		//******	Load Methods
			
			
			OM_FORCE_INLINE static SIMDScalar load( const Float64* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_load_pd( array ) );
#else
				return SIMDScalar( array[0], array[1] );
#endif
			}
			
			
			OM_FORCE_INLINE static SIMDScalar loadUnaligned( const Float64* array )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_loadu_pd( array ) );
#else
				return SIMDScalar( array[0], array[1] );
#endif
			}
			
			
		//********************************************************************************
		//******	Store Method
			
			
			OM_FORCE_INLINE void store( Float64* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				_mm_store_pd( destination, vf );
#else
				destination[0] = a;
				destination[1] = b;
#endif
			}
			
			
			OM_FORCE_INLINE void storeUnaligned( Float64* destination ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				_mm_storeu_pd( destination, vf );
#else
				destination[0] = a;
				destination[1] = b;
#endif
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Get a reference to the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Float64& operator [] ( Index i )
			{
				return x[i];
			}
			
			
			/// Get the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Float64 operator [] ( Index i ) const
			{
				return x[i];
			}
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			OM_FORCE_INLINE const Float64* toArray() const
			{
				return x;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two 2D SIMD scalars component-wise for equality.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components is equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator == ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpeq_pd( vf, scalar.vf ) );
#else
				return SIMDScalar<Int64,2>( a == scalar.a, b == scalar.b );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for equality.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator == ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpeq_pd( vf, _mm_set1_pd( value ) ) );
#else
				return SIMDScalar<Int64,2>( a == value, b == value );
#endif
			}
			
			
			/// Compare two 2D SIMD scalars component-wise for inequality
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator != ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpneq_pd( vf, scalar.vf ) );
#else
				return SIMDScalar<Int64,2>( a != scalar.a, b != scalar.b );
#endif
			}
			
			
			/// Compare this scalar to a single floating point value for inequality.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components is not equal, the corresponding result
			  * component is non-zero. Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator != ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpneq_pd( vf, _mm_set1_pd( value ) ) );
#else
				return SIMDScalar<Int64,2>( a != value, b != value );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this and another 2D SIMD scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component less than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator < ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmplt_pd( vf, scalar.vf ) );
#else
				return SIMDScalar<Int64,2>( a < scalar.a, b < scalar.b );
#endif
			}
			
			
			/// Perform a component-wise less-than comparison between this 2D SIMD scalar and an expanded scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component less than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator < ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmplt_pd( vf, _mm_set1_pd( value ) ) );
#else
				return SIMDScalar<Int64,2>( a < value, b < value );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this an another 2D SIMD scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator > ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpgt_pd( vf, scalar.vf ) );
#else
				return SIMDScalar<Int64,2>( a > scalar.a, b > scalar.b );
#endif
			}
			
			
			/// Perform a component-wise greater-than comparison between this 2D SIMD scalar and an expanded scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator > ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpgt_pd( vf, _mm_set1_pd( value ) ) );
#else
				return SIMDScalar<Int64,2>( a > value, b > value );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 2D SIMD scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component less than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator <= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmple_pd( vf, scalar.vf ) );
#else
				return SIMDScalar<Int64,2>( a <= scalar.a, b <= scalar.b );
#endif
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 2D SIMD scalar and an expanded scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component less than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator <= ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmple_pd( vf, _mm_set1_pd( value ) ) );
#else
				return SIMDScalar<Int64,2>( a <= value, b <= value );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 2D SIMD scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator >= ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpge_pd( vf, scalar.vf ) );
#else
				return SIMDScalar<Int64,2>( a >= scalar.a, b >= scalar.b );
#endif
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 2D SIMD scalar and an expanded scalar.
			/**
			  * Return a 2D scalar of integers indicating the result of the comparison.
			  * The float value is expanded to a 4-wide SIMD scalar and compared with this scalar.
			  * If each corresponding pair of components has this scalar's component greater than 
			  * or equal to the other scalar's component, the corresponding result component is non-zero.
			  * Otherwise, that result component is equal to zero.
			  */
			OM_FORCE_INLINE SIMDScalar<Int64,2> operator >= ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar<Int64,2>( _mm_cmpge_pd( vf, _mm_set1_pd( value ) ) );
#else
				return SIMDScalar<Int64,2>( a >= value, b >= value );
#endif
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate a scalar.
			/** 
			  * This method negates every component of this 2D SIMD scalar
			  * and returns the result, leaving this scalar unmodified.
			  * 
			  * @return the negation of the original scalar.
			  */
			OM_FORCE_INLINE SIMDScalar operator - () const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_pd( _mm_set1_pd(Float64(0)), vf ) );
#else
				return SIMDScalar( -a, -b );
#endif
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add this scalar to another and return the result.
			/** 
			  * This method adds another scalar to this one, component-wise,
			  * and returns this addition. It does not modify either of the original
			  * scalars.
			  * 
			  * @param scalar - The scalar to add to this one.
			  * @return The addition of this scalar and the parameter.
			  */
			OM_FORCE_INLINE SIMDScalar operator + ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_pd( vf, scalar.vf ) );
#else
				return SIMDScalar( a + scalar.a, b + scalar.b );
#endif
			}
			
			
			/// Add a value to every component of this scalar.
			/** 
			  * This method adds the value parameter to every component
			  * of the scalar, and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to add to all components of this scalar.
			  * @return The resulting scalar of this addition.
			  */
			OM_FORCE_INLINE SIMDScalar operator + ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_add_pd( vf, _mm_set1_pd(value) ) );
#else
				return SIMDScalar( a + value, b + value );
#endif
			}
			
			
			/// Subtract a scalar from this scalar component-wise and return the result.
			/** 
			  * This method subtracts another scalar from this one, component-wise,
			  * and returns this subtraction. It does not modify either of the original
			  * scalars.
			  * 
			  * @param scalar - The scalar to subtract from this one.
			  * @return The subtraction of the the parameter from this scalar.
			  */
			OM_FORCE_INLINE SIMDScalar operator - ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_pd( vf, scalar.vf ) );
#else
				return SIMDScalar( a - scalar.a, b - scalar.b );
#endif
			}
			
			
			/// Subtract a value from every component of this scalar.
			/** 
			  * This method subtracts the value parameter from every component
			  * of the scalar, and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to subtract from all components of this scalar.
			  * @return The resulting scalar of this subtraction.
			  */
			OM_FORCE_INLINE SIMDScalar operator - ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_sub_pd( vf, _mm_set1_pd(value) ) );
#else
				return SIMDScalar( a - value, b - value );
#endif
			}
			
			
			/// Multiply component-wise this scalar and another scalar.
			/** 
			  * This operator multiplies each component of this scalar
			  * by the corresponding component of the other scalar and
			  * returns a scalar representing this result. It does not modify
			  * either original scalar.
			  * 
			  * @param scalar - The scalar to multiply this scalar by.
			  * @return The result of the multiplication.
			  */
			OM_FORCE_INLINE SIMDScalar operator * ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_mul_pd( vf, scalar.vf ) );
#else
				return SIMDScalar( a*scalar.a, b*scalar.b );
#endif
			}
			
			
			/// Multiply every component of this scalar by a value and return the result.
			/** 
			  * This method multiplies the value parameter with every component
			  * of the scalar, and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to multiplly with all components of this scalar.
			  * @return The resulting scalar of this multiplication.
			  */
			OM_FORCE_INLINE SIMDScalar operator * ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_mul_pd( vf, _mm_set1_pd(value) ) );
#else
				return SIMDScalar( a*value, b*value );
#endif
			}
			
			
			/// Divide this scalar by another scalar component-wise.
			/** 
			  * This operator divides each component of this scalar
			  * by the corresponding component of the other scalar and
			  * returns a scalar representing this result. It does not modify
			  * either original scalar.
			  * 
			  * @param scalar - The scalar to multiply this scalar by.
			  * @return The result of the division.
			  */
			OM_FORCE_INLINE SIMDScalar operator / ( const SIMDScalar& scalar ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_div_pd( vf, scalar.vf ) );
#else
				return SIMDScalar( a/scalar.a, b/scalar.b );
#endif
			}
			
			
			/// Divide every component of this scalar by a value and return the result.
			/** 
			  * This method Divides every component of the scalar by the value parameter, 
			  * and returns a scalar representing this result.
			  * It does not modifiy the original scalar.
			  * 
			  * @param value - The value to divide all components of this scalar by.
			  * @return The resulting scalar of this division.
			  */
			OM_FORCE_INLINE SIMDScalar operator / ( const Float64 value ) const
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				return SIMDScalar( _mm_mul_pd( vf, _mm_set1_pd(Float64(1) / value) ) );
#else
				Float64 inverse = Float64(1) / value;
				return SIMDScalar( a*inverse, b*inverse );
#endif
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to this scalar, modifying this original scalar.
			/**
			  * This method adds another scalar to this scalar, component-wise,
			  * and sets this scalar to have the result of this addition.
			  * 
			  * @param scalar - The scalar to add to this scalar.
			  * @return A reference to this modified scalar.
			  */
			OM_FORCE_INLINE SIMDScalar& operator += ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vf = _mm_add_pd( vf, scalar.vf );
#else
				a += scalar.a;
				b += scalar.b;
#endif
				return *this;
			}
			
			/// Subtract a scalar from this scalar, modifying this original scalar.
			/**
			  * This method subtracts another scalar from this scalar, component-wise,
			  * and sets this scalar to have the result of this subtraction.
			  * 
			  * @param scalar - The scalar to subtract from this scalar.
			  * @return A reference to this modified scalar.
			  */
			OM_FORCE_INLINE SIMDScalar& operator -= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vf = _mm_sub_pd( vf, scalar.vf );
#else
				a -= scalar.a;
				b -= scalar.b;
#endif
				return *this;
			}
			
			
			/// Multiply component-wise this scalar and another scalar and modify this scalar.
			/** 
			  * This operator multiplies each component of this scalar
			  * by the corresponding component of the other scalar and
			  * modifies this scalar to contain the result.
			  * 
			  * @param scalar - The scalar to multiply this scalar by.
			  * @return A reference to this modified scalar.
			  */
			OM_FORCE_INLINE SIMDScalar& operator *= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vf = _mm_mul_pd( vf, scalar.vf );
#else
				a *= scalar.a;
				b *= scalar.b;
#endif
				return *this;
			}
			
			
			/// Divide this scalar by another scalar component-wise and modify this scalar.
			/** 
			  * This operator divides each component of this scalar
			  * by the corresponding component of the other scalar and
			  * modifies this scalar to contain the result.
			  * 
			  * @param scalar - The scalar to divide this scalar by.
			  * @return A reference to this modified scalar.
			  */
			OM_FORCE_INLINE SIMDScalar& operator /= ( const SIMDScalar& scalar )
			{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
				vf = _mm_div_pd( vf, scalar.vf );
#else
				a /= scalar.a;
				b /= scalar.b;
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
			static const Size WIDTH = SIMDTypeN<Float64,2>::WIDTH;
			
			
			/// The required alignment of this scalar type.
			static const Size ALIGNMENT = SIMDTypeN<Float64,2>::ALIGNMENT;


			union OM_ALIGN(16)
			{
				/// The platform-specific vector to use for 2 64-bit floats.
				Float64x2 vf;
				
				/// The platform-specific vector to use for 2 64-bit integers.
				Int64x2 vi;
				
				/// The components of a 2D SIMD scalar in array format.
				Float64 x[2];
				
				struct
				{
					/// The first component of a 2D SIMD scalar.
					Float64 a;
					
					/// The second component of a 2D SIMD scalar.
					Float64 b;
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
OM_FORCE_INLINE SIMDScalar<Float64,2> abs( const SIMDScalar<Float64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	OM_ALIGN(16) const UInt64 absMask[2] = { UInt64(0x7FFFFFFFFFFFFFFFull), UInt64(0x7FFFFFFFFFFFFFFFull) };
	return SIMDScalar<Float64,2>( _mm_and_pd( scalar.vf, _mm_load_pd( reinterpret_cast<const Float64*>(absMask) ) ) );
#else
	return SIMDScalar<Float64,2>( math::abs(scalar.a), math::abs(scalar.b) );
#endif
}




/// Compute the ceiling of each component of the specified SIMD scalar and return the result.
/**
  * This method is emulated in software on x86 platforms where SSE 4.1 is not available.
  */
OM_FORCE_INLINE SIMDScalar<Float64,2> ceiling( const SIMDScalar<Float64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(4,1)
	return SIMDScalar<Float64,2>( _mm_ceil_pd( scalar.vf ) );
#else
	return SIMDScalar<Float64,2>( math::ceiling(scalar.a), math::ceiling(scalar.b) );
#endif
}




/// Compute the floor of each component of the specified SIMD scalar and return the result.
/**
  * This method is emulated in software on x86 platforms where SSE 4.1 is not available.
  */
OM_FORCE_INLINE SIMDScalar<Float64,2> floor( const SIMDScalar<Float64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(4,1)
	return SIMDScalar<Float64,2>( _mm_floor_pd( scalar.vf ) );
#else
	return SIMDScalar<Float64,2>( math::floor(scalar.a), math::floor(scalar.b) );
#endif
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
OM_FORCE_INLINE SIMDScalar<Float64,2> sqrt( const SIMDScalar<Float64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return SIMDScalar<Float64,2>( _mm_sqrt_pd( scalar.vf ) );
#else
	return SIMDScalar<Float64,2>( math::sqrt(scalar.a), math::sqrt(scalar.b) );
#endif
}




/// Pick 2 elements from the specified SIMD scalar and return the result.
template < UInt i1, UInt i2 >
OM_FORCE_INLINE SIMDScalar<Float64,2> shuffle( const SIMDScalar<Float64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return SIMDScalar<Float64,2>( _mm_shuffle_pd( scalar.vf, scalar.vf, _MM_SHUFFLE2(i2, i1) ) );
#else
	return SIMDScalar<Float64,2>( scalar[i1], scalar[i2] );
#endif
}




/// Pick one element from each SIMD scalar and return the result.
template < UInt i1, UInt i2 >
OM_FORCE_INLINE SIMDScalar<Float64,2> shuffle( const SIMDScalar<Float64,2>& scalar1, const SIMDScalar<Float64,2>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return SIMDScalar<Float64,2>( _mm_shuffle_pd( scalar1.vf, scalar2.vf, _MM_SHUFFLE2(i2, i1) ) );
#else
	return SIMDScalar<Float64,2>( scalar1[i1], scalar1[i2] );
#endif
}




/// Return the horizontal sum of a vector as a vector.
OM_FORCE_INLINE SIMDScalar<Float64,2> sum( const SIMDScalar<Float64,2>& v )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return v + math::shuffle<1,0>( v );
#else
	return SIMDScalar<Float64,2>( v.a + v.b );
#endif
}




/// Return the horizontal sum of a vector as a scalar.
OM_FORCE_INLINE Float64 sumScalar( const SIMDScalar<Float64,2>& v )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return _mm_cvtsd_f64( math::sum( v ).vf );
#else
	return v.a + v.b;
#endif
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
OM_FORCE_INLINE SIMDScalar<Float64,2> select( const SIMDScalar<Int64,2>& selector,
												const SIMDScalar<Float64,2>& scalar1, const SIMDScalar<Float64,2>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	// (((b^a) & selector) ^ a)
	return SIMDScalar<Float64,2>( _mm_xor_pd( scalar2.vf, _mm_and_pd( selector.vf, _mm_xor_pd( scalar1.vf, scalar2.vf ) ) ) );
#else
	return SIMDScalar<Float64,2>( selector.a ? scalar1.a : scalar2.a, selector.b ? scalar1.b : scalar2.b );
#endif
}




/// Subtract the first and third elements and add the second and fourth.
OM_FORCE_INLINE SIMDScalar<Float64,2> subAdd( const SIMDScalar<Float64,2>& scalar1, const SIMDScalar<Float64,2>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(3,0)
	return SIMDScalar<Float64,2>( _mm_addsub_pd( scalar1.vf, scalar2.vf ) );
#else
	return SIMDScalar<Float64,2>( scalar1.a - scalar2.a, scalar1.b + scalar2.b );
#endif
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Float64,2> min( const SIMDScalar<Float64,2>& scalar1, const SIMDScalar<Float64,2>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return SIMDScalar<Float64,2>( _mm_min_pd( scalar1.vf, scalar2.vf ) );
#else
	return SIMDScalar<Float64,2>( math::min(scalar1.a, scalar2.a), math::min(scalar1.b, scalar2.b) );
#endif
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
OM_FORCE_INLINE SIMDScalar<Float64,2> max( const SIMDScalar<Float64,2>& scalar1, const SIMDScalar<Float64,2>& scalar2 )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	return SIMDScalar<Float64,2>( _mm_max_pd( scalar1.vf, scalar2.vf ) );
#else
	return SIMDScalar<Float64,2>( math::max(scalar1.a, scalar2.a), math::max(scalar1.b, scalar2.b) );
#endif
}




/// Compute the minimum component of the specified SIMD scalar and return the wide result.
OM_FORCE_INLINE SIMDScalar<Float64,2> min( const SIMDScalar<Float64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	// Shuffle the value once to find the minimum of 0 & 1.
	SIMDScalar<Float64,2> scalar2 = math::shuffle<1,0>( scalar );
	
	// Find the minimum of 0 & 1.
	return math::min( scalar, scalar2 );
#else
	return SIMDScalar<Float64,2>( math::min( scalar.a, scalar.b ) );
#endif
}




/// Compute the maximum component of the specified SIMD scalar and return the wide result.
OM_FORCE_INLINE SIMDScalar<Float64,2> max( const SIMDScalar<Float64,2>& scalar )
{
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
	// Shuffle the value once to find the minimum of 0 & 1.
	SIMDScalar<Float64,2> scalar2 = math::shuffle<1,0>( scalar );
	
	// Find the maximum of 0 & 1.
	return math::max( scalar, scalar2 );
#else
	return SIMDScalar<Float64,2>( math::max( scalar.a, scalar.b ) );
#endif
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_SCALAR_FLOAT_64_2_H
