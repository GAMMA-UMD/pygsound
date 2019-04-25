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

#ifndef INCLUDE_OM_SIMD_ARRAY_FLOAT_32_H
#define INCLUDE_OM_SIMD_ARRAY_FLOAT_32_H


#include "omMathConfig.h"


#include "omSIMDConfig.h"
#include "omSIMDTypes.h"
#include "omSIMDScalar.h"
#include "omSIMDScalarFloat32_4.h"
#include "omSIMDArray.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing an N-component 32-bit floating-point SIMD scalar.
/**
  * This specialization of the SIMDScalar class uses one or more 4-component
  * SIMD values to simulate an N-wide SIMD register.
  */
template < Size width >
class OM_ALIGN(16) SIMDArray<Float32,width>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new SIMD scalar with all elements left uninitialized.
			OM_FORCE_INLINE SIMDArray()
			{
			}
			
			
			/// Create a new SIMD scalar with all elements equal to the specified value.
			OM_FORCE_INLINE SIMDArray( Float32 value )
			{
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					v[i] = simdValue;
			}
			
			
			/// Create a new SIMD scalar from the first N values stored at specified pointer's location.
			OM_FORCE_INLINE explicit SIMDArray( const Float32* array )
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
			OM_FORCE_INLINE static SIMDArray load( const Float32* array )
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
			OM_FORCE_INLINE static SIMDArray loadUnaligned( const Float32* array )
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
			
			
			/// Store this SIMD scalar starting at the specified aligned destination pointer.
			OM_FORCE_INLINE void store( Float32* destination ) const
			{
				for ( Index i = 0; i < numIterations; i++ )
				{
					v[i].store( destination );
					destination += SIMD_WIDTH;
				}
			}
			
			
			/// Store this SIMD scalar starting at the specified unaligned destination pointer.
			OM_FORCE_INLINE void storeUnaligned( Float32* destination ) const
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
			OM_FORCE_INLINE Float32& operator [] ( Index i )
			{
				return ((Float32*)v)[i];
			}
			
			
			/// Get the value stored at the specified component index in this scalar.
			OM_FORCE_INLINE Float32 operator [] ( Index i ) const
			{
				return ((const Float32*)v)[i];
			}
			
			
			/// Get a pointer to the first element in this scalar.
			/**
			  * The remaining values are in the next 3 locations after the
			  * first element.
			  */
			OM_FORCE_INLINE const Float32* toArray() const
			{
				return (const Float32*)v;
			}
			
			
			/// Get the width of this scalar (number of components it has).
			OM_FORCE_INLINE static Size getWidth()
			{
				return width;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Compare two 4D SIMD scalars component-wise for equality.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator == ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == scalar.v[i];
				
				return result;
			}
			
			
			/// Compare this scalar to a single floating point value for equality.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator == ( const Float32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] == simdValue;
				
				return result;
			}
			
			
			/// Compare two 4D SIMD scalars component-wise for inequality
			OM_FORCE_INLINE SIMDArray<Int32,width> operator != ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != scalar.v[i];
				
				return result;
			}
			
			
			/// Compare this scalar to a single floating point value for inequality.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator != ( const Float32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] != simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise less-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator < ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] < scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise less-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator < ( const Float32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] < simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator > ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] > scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator > ( const Float32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] > simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator <= ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] <= scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise less-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator <= ( const Float32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] <= simdValue;
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this an another 4D SIMD scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator >= ( const SIMDArray& scalar ) const
			{
				SIMDArray<Int32,width> result;
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >= scalar.v[i];
				
				return result;
			}
			
			
			/// Perform a component-wise greater-than-or-equal-to comparison between this 4D SIMD scalar and an expanded scalar.
			OM_FORCE_INLINE SIMDArray<Int32,width> operator >= ( const Float32 value ) const
			{
				SIMDArray<Int32,width> result;
				SIMDBaseType simdValue( value );
				
				for ( Index i = 0; i < numIterations; i++ )
					result.v[i] = v[i] >= simdValue;
				
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
			OM_FORCE_INLINE SIMDArray operator + ( const Float32 value ) const
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
			OM_FORCE_INLINE SIMDArray operator - ( const Float32 value ) const
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
			OM_FORCE_INLINE SIMDArray operator * ( const Float32 value ) const
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
			OM_FORCE_INLINE SIMDArray operator / ( const Float32 value ) const
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
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this SIMD array into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				const Float32* array = this->toArray();
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
			static const Size SIMD_WIDTH = SIMDType<Float32>::WIDTH;
			
			
			/// The underlying SIMD type used to implement this class.
			typedef SIMDScalar<Float32,SIMD_WIDTH> SIMDBaseType;
			
			
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
			friend class SIMDArray3D;
			
			template < Size width2 >
			friend OM_FORCE_INLINE Float32 sumScalar( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> abs( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> ceiling( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> floor( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> sqrt( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> min( const SIMDArray<Float32,width2>& scalar1, const SIMDArray<Float32,width2>& scalar2 );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> max( const SIMDArray<Float32,width2>& scalar1, const SIMDArray<Float32,width2>& scalar2 );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> select( const SIMDArray<Int32,width2>& selector,
															const SIMDArray<Float32,width2>& scalar1, const SIMDArray<Float32,width2>& scalar2 );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> ln( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> exp( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> log10( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> pow( const SIMDArray<Float32,width2>& base, const SIMDArray<Float32,width2>& power );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> sin( const SIMDArray<Float32,width2>& scalar );
			
			template < Size width2 >
			friend OM_FORCE_INLINE SIMDArray<Float32,width2> cos( const SIMDArray<Float32,width2>& scalar );
			
			
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
OM_FORCE_INLINE SIMDArray<Float32,width> operator + ( const Float32 value, const SIMDArray<Float32,width>& scalar )
{
	return SIMDArray<Float32,4>(value) + scalar;
}




/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> operator - ( const Float32 value, const SIMDArray<Float32,width>& scalar )
{
	return SIMDArray<Float32,width>(value) - scalar;
}




/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> operator * ( const Float32 value, const SIMDArray<Float32,width>& scalar )
{
	return SIMDArray<Float32,width>(value) * scalar;
}




/// Divide each component of this scalar by a scalar value and return the resulting scalar.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> operator / ( const Float32 value, const SIMDArray<Float32,width>& scalar )
{
	return SIMDArray<Float32,width>(value) / scalar;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Free Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Return the horizontal sum of all components of a SIMD vector as a scalar value.
template < Size width >
OM_FORCE_INLINE Float32 sumScalar( const SIMDArray<Float32,width>& scalar )
{
	typename SIMDArray<Float32,width>::SIMDBaseType total = scalar.v[0];
	
	for ( Index i = 1; i < SIMDArray<Float32,width>::numIterations; i++ )
		total += scalar.v[i];
	
	return math::sumScalar( total );
}




/// Compute the absolute value of each component of the specified SIMD scalar and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> abs( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::abs( scalar.v[i] );
	
	return result;
}




/// Compute the ceiling of each component of the specified SIMD scalar and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> ceiling( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::ceiling( scalar.v[i] );
	
	return result;
}




/// Compute the floor of each component of the specified SIMD scalar and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> floor( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::floor( scalar.v[i] );
	
	return result;
}




/// Compute the square root of each component of the specified SIMD scalar and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> sqrt( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::sqrt( scalar.v[i] );
	
	return result;
}




/// Compute the minimum of each component of the specified SIMD scalars and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> min( const SIMDArray<Float32,width>& scalar1, const SIMDArray<Float32,width>& scalar2 )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::min( scalar1.v[i], scalar2.v[i] );
	
	return result;
}




/// Compute the maximum of each component of the specified SIMD scalars and return the result.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> max( const SIMDArray<Float32,width>& scalar1, const SIMDArray<Float32,width>& scalar2 )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::max( scalar1.v[i], scalar2.v[i] );
	
	return result;
}




/// Select elements from the first SIMD scalar if the selector is TRUE, otherwise from the second.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> select( const SIMDArray<Int32,width>& selector,
												const SIMDArray<Float32,width>& scalar1, const SIMDArray<Float32,width>& scalar2 )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::select( selector.v[i], scalar1.v[i], scalar2.v[i] );
	
	return result;
}




/// Compute and return the natural logarithm of the specified vector.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> ln( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::ln( scalar.v[i] );
	
	return result;
}




/// Compute and return the base-10 logarithm of the specified vector.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> log10( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::log10( scalar.v[i] );
	
	return result;
}




/// Compute and return e raised to the specified vector power.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> exp( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::exp( scalar.v[i] );
	
	return result;
}




/// Compute and return the power of the specified vector.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> pow( const SIMDArray<Float32,width>& base, const SIMDArray<Float32,width>& power )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::pow( base.v[i], power.v[i] );
	
	return result;
}




/// Compute and return the sine of the specified vector.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> sin( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::sin( scalar.v[i] );
	
	return result;
}




/// Compute and return the cosine of the specified vector.
template < Size width >
OM_FORCE_INLINE SIMDArray<Float32,width> cos( const SIMDArray<Float32,width>& scalar )
{
	SIMDArray<Float32,width> result;
	
	for ( Index i = 0; i < SIMDArray<Float32,width>::numIterations; i++ )
		result.v[i] = math::cos( scalar.v[i] );
	
	return result;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_ARRAY_FLOAT_32_H
