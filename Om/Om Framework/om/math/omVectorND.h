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

#ifndef INCLUDE_OM_VECTOR_ND_H
#define INCLUDE_OM_VECTOR_ND_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a vector of a fixed arbitrary number of components.
template < typename T, Size dimension >
class VectorND
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a vector with all of its elements equal to zero
			OM_FORCE_INLINE VectorND()
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] = T(0);
			}
			
			
			/// Create a new vector with all elements equal to a single value.
			OM_FORCE_INLINE VectorND( T value )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] = value;
			}
			
			
			/// Create a vector with elements from the specified array.
			/**
			  * The array must be of length greater than or equal to the
			  * dimension of the vector.
			  */
			OM_FORCE_INLINE VectorND( const T array[dimension] )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] = array[i];
			}
			
			
		//********************************************************************************
		//******	VectorND Operations
			
			
			/// Get the magnitude of the vector.
			OM_FORCE_INLINE T getMagnitude() const
			{
				return math::sqrt( getMagnitudeSquared() );
			}
			
			
			/// Get the squared magnitude of the vector.
			OM_FORCE_INLINE T getMagnitudeSquared() const
			{
				T sum = T(0);
				
				for ( Index i = 0; i < dimension; i++ )
					sum += x[i]*x[i];
				
				return sum;
			}
			
			
			/// Return a a normalized version of this vector.
			OM_FORCE_INLINE VectorND normalize() const
			{
				T mag = getMagnitude();
				
				if ( mag == T(0) )
					return VectorND::ZERO;
				
				T invMag = T(1)/mag;
				
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i]*invMag;
				
				return result;
			}
			
			
			/// Project this vector onto another vector
			OM_FORCE_INLINE VectorND projectOn( const VectorND& v ) const
			{
				// compute this dot v:
				T result = T(0);
				
				for ( Index i = 0; i < dimension; i++ )
					result += x[i]*v.x[i];
				
				return result * v.normalize();
			}
			
			
			/// Get the distance between the this vector and another.
			OM_FORCE_INLINE T getDistanceTo( const VectorND& v ) const
			{
				return math::sqrt( distanceToSquared(v) );
			}
			
			
			/// Get the squared distance between the this vector and another.
			OM_FORCE_INLINE T getDistanceToSquared( const VectorND& v ) const
			{
				return ((*this) - v).getMagnitudeSquared();
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Return an array representation of this vector.
			OM_FORCE_INLINE const T* toArray() const
			{
				return x;
			}
			
			
			/// Return a reference to the element at the specified index in the vector.
			OM_FORCE_INLINE T& get( Index i )
			{
				OM_DEBUG_ASSERT( i < dimension );
				
				return x[i];
			}
			
			
			/// Return a const reference to the element at the specified index in the vector.
			OM_FORCE_INLINE const T& get( Index i ) const
			{
				OM_DEBUG_ASSERT( i < dimension );
				
				return x[i];
			}
			
			
			/// Return a reference to the element at the specified index in the vector.
			OM_FORCE_INLINE T& operator () ( Index i )
			{
				OM_DEBUG_ASSERT( i < dimension );
				
				return x[i];
			}
			
			
			/// Return const a reference to the element at the specified index in the vector.
			OM_FORCE_INLINE const T& operator () ( Index i ) const
			{
				OM_DEBUG_ASSERT( i < dimension );
				
				return x[i];
			}
			
			
			/// Return a reference to the element at the specified index in the vector.
			OM_FORCE_INLINE T& operator [] ( Index i )
			{
				OM_DEBUG_ASSERT( i < dimension );
				
				return x[i];
			}
			
			
			/// Return a const reference to the element at the specified index in the vector.
			OM_FORCE_INLINE const T& operator [] ( Index i ) const
			{
				OM_DEBUG_ASSERT( i < dimension );
				
				return x[i];
			}
			
			
			/// Return the dimension of this vector (number of components it has).
			OM_FORCE_INLINE Size getSize() const
			{
				return dimension;
			}
			
			
			/// Return the dimension of this vector (number of components it has).
			OM_FORCE_INLINE Size getDimension() const
			{
				return dimension;
			}
			
			
			/// Set the element at the specified index in the vector.
			OM_FORCE_INLINE void set( Index i, T newX )
			{
				OM_DEBUG_ASSERT( i < dimension );
				
				x[i] = newX;
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate every component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator - () const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = -x[i];
				
				return result;
			}
			
			
			/// 'Positivate' every component of this vector, effectively returning a copy.
			OM_FORCE_INLINE VectorND operator + () const
			{
				return *this;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add a vector to this vector and return the resulting vector.
			OM_FORCE_INLINE VectorND operator + ( const VectorND& vector ) const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i] + vector.x[i];
				
				return result;
			}
			
			
			/// Add a scalar value to each component of this vector and return the resulting vector.
			OM_FORCE_INLINE VectorND operator + ( T value ) const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i] + value;
				
				return result;
			}
			
			
			/// Subtract a vector from this vector and return the resulting vector.
			OM_FORCE_INLINE VectorND operator - ( const VectorND& vector ) const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i] - vector.x[i];
				
				return result;
			}
			
			
			/// Subtract a scalar value from each component of this vector and return the resulting vector.
			OM_FORCE_INLINE VectorND operator - ( T value ) const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i] - value;
				
				return result;
			}
			
			
			/// Return the result of a component-wise vector multiplication with this vector.
			OM_FORCE_INLINE VectorND operator * ( const VectorND& vector ) const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i]*vector.x[i];
				
				return result;
			}
			
			
			/// Multiply a scalar value by each component of this vector and return the resulting vector.
			OM_FORCE_INLINE VectorND operator * ( T value ) const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i]*value;
				
				return result;
			}
			
			
			/// Divide each component of this vector by a scalar value and return the resulting vector.
			OM_FORCE_INLINE VectorND operator / ( T value ) const
			{
				VectorND result;
				
				for ( Index i = 0; i < dimension; i++ )
					result.x[i] = x[i]/value;
				
				return result;
			}			
			
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			OM_FORCE_INLINE VectorND& operator += ( const VectorND& v2 )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] += v2.x[i];
				
				return *this;
			}
			
			
			OM_FORCE_INLINE VectorND& operator -= ( const VectorND& v2 )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] -= v2.x[i];
				
				return *this;
			}
			
			
			OM_FORCE_INLINE VectorND& operator += ( const T value )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] += value;
				
				return *this;
			}
			
			
			OM_FORCE_INLINE VectorND& operator -= ( const T value )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] -= value;
				
				return *this;
			}
			
			
			OM_FORCE_INLINE VectorND& operator *= ( const T value )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] *= value;
				
				return *this;
			}
			
			
			OM_FORCE_INLINE VectorND& operator /= ( const T value )
			{
				for ( Index i = 0; i < dimension; i++ )
					x[i] /= value;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this vector into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "< ";
				
				for ( Index i = 0; i < dimension; i++ )
				{
					if ( i != dimension - 1 )
						buffer << x[i] << ", ";
					else
						buffer << x[i];
				}
				
				buffer << " >";
				
				return buffer.toString();
			}
			
			
			/// Convert this vector into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The components of this vector.
			T x[dimension];
			
			
	public:
		
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// A constant vector with every component equal to zero.
			static const VectorND ZERO;
			
			
};




template < typename T, Size dimension >
const VectorND<T,dimension> VectorND<T,dimension>:: ZERO;




//##########################################################################################
//##########################################################################################
//############		
//############		Reverse Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Multiply every component of a vector by a scalar value and return the resulting vector.
template < typename T, Size dimension >
OM_FORCE_INLINE VectorND<T,dimension> operator * ( const T& c, const VectorND<T,dimension>& vector ) 
{
	VectorND<T,dimension> result;
	
	for ( Index i = 0; i < dimension; i++ )
		result.set( i, vector.get(i) * c );
	
	return result;
}




/// Multiply every component of a vector by a scalar value and return the resulting vector.
template < typename T, Size dimension >
OM_FORCE_INLINE VectorND<T,dimension> operator + ( const T& c, const VectorND<T,dimension>& vector ) 
{
	VectorND<T,dimension> result;
	
	for ( Index i = 0; i < dimension; i++ )
		result.set( i, vector.get(i) + c );
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Standalone Functions
//############		
//##########################################################################################
//##########################################################################################




/// Return the dot product of two vectors.
template < typename T, Size dimension >
OM_FORCE_INLINE T dot( const VectorND<T,dimension>& v1, const VectorND<T,dimension>& v2 )
{
	T result(0);
	
	for ( Index i = 0; i < dimension; i++ )
		result += v1[i]*v2[i];
	
	return result;
}




/// Return the midpoint of two vectors.
template < typename T, Size dimension >
OM_FORCE_INLINE VectorND<T,dimension> midpoint( const VectorND<T,dimension>& v1, const VectorND<T,dimension>& v2 )
{
	VectorND<T,dimension> result;
	
	for ( Index i = 0; i < dimension; i++ )
		result[i] = (v1[i] + v2[i])*T(0.5);
	
	return result;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_VECTOR_ND_H
