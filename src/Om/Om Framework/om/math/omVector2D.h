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

#ifndef INCLUDE_OM_VECTOR_2D_H
#define INCLUDE_OM_VECTOR_2D_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "omVectorND.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


template < typename T, Size dimension, Index i0, Index i1 >
class Swizzle2D;




//********************************************************************************
/// A class that represents a 2-dimensional point or vector.
template < typename T >
class VectorND<T,2>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new 2D vector with all elements equal to zero.
			OM_FORCE_INLINE VectorND()
				:	x( T(0) ),
					y( T(0) )
			{
			}
			
			
			/// Create a new 2D vector with all elements equal to a single value.
			OM_FORCE_INLINE explicit VectorND( T value )
				:	x( value ),
					y( value )
			{
			}
			
			
			/// Create a new 2D vector from a pointer to a 2 element array.
			OM_FORCE_INLINE explicit VectorND( const T array[2] )
				:	x( array[0] ),
					y( array[1] )
			{
			}
			
			
			/// Create a new 2D vector by specifying it's x and y values.
			OM_FORCE_INLINE VectorND( T newX, T newY )
				:	x( newX ),
					y( newY )
			{
			}
			
			/// Create a new 2D vector from an existing vector of a different template type.
			template < typename U >
			OM_FORCE_INLINE VectorND( const VectorND<U,2>& vector )
				:	x( (T)vector.x ),
					y( (T)vector.y )
			{
			}
			
			
			/// Create a new 2D vector from an existing vector with the same template type.
			/**
			  * This is declared because the auto-generated one was not working with the crazy uniform mess.
			  */
			OM_FORCE_INLINE VectorND( const VectorND& vector )
				:	x( vector.x ),
					y( vector.y )
			{
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Create a new 2D vector from an existing vector with a different template type.
			/**
			  * This is declared because the auto-generated one was not working with the crazy uniform mess.
			  */
			OM_FORCE_INLINE VectorND& operator = ( const VectorND& vector )
			{
				x = vector.x;
				y = vector.y;
				return *this;
			}
			
			
		//********************************************************************************
		//******	Static Construction Methods
			
			
			/// Convert the specified polar angle in radians to a unit-length 2D vector.
			static VectorND polar( T angle )
			{
				return VectorND( math::cos( angle ), math::sin( angle ) );
			}
			
			
			/// Convert the specified polar coordinates in radians to an r-length 2D vector.
			static VectorND polar( T angle, T r )
			{
				return r*VectorND( math::cos( angle ), math::sin( angle ) );
			}
			
			
		//********************************************************************************
		//******	Magnitude Methods
			
			
			/// Return the magnitude of this vector (the length).
			OM_FORCE_INLINE T getMagnitude() const
			{
				return math::sqrt( x*x + y*y );
			}
			
			
			/// Return the square of the magnitude of this vector.
			OM_FORCE_INLINE T getMagnitudeSquared() const
			{
				return x*x + y*y;
			}
			
			
			/// Return a normalized version of this vector.
			OM_FORCE_INLINE VectorND normalize() const
			{
				const T inverseMagnitude = T(1)/math::sqrt( x*x + y*y );
				
				return VectorND( x*inverseMagnitude, y*inverseMagnitude );
			}
			
			
			/// Return a normalized version of this vector, placing the vector's magnitude in the output parameter.
			OM_FORCE_INLINE VectorND normalize( T& magnitude ) const
			{
				magnitude = math::sqrt( x*x + y*y );
				T inverseMagnitude = T(1)/magnitude;
				
				return VectorND( x*inverseMagnitude, y*inverseMagnitude );
			}
			
			
			/// Project this vector on another vector and return the projected vector.
			OM_FORCE_INLINE VectorND projectOn( const VectorND& vector ) const
			{
				VectorND norm = vector.normalize();
				
				return (x*norm.x + y*norm.y)*norm;
			}
			
			
			/// Project this vector on a normalized vector and return the projected vector.
			OM_FORCE_INLINE VectorND projectOnNormalized( const VectorND& vector ) const
			{
				return (x*vector.x + y*vector.y)*vector;
			}
			
			
		//********************************************************************************
		//******	Distance Methods
			
			
			/// Return the distance from this vector to another in 2D Euclidean space.
			OM_FORCE_INLINE T getDistanceTo( const VectorND& vector ) const
			{
				const T minusX = vector.x - x;
				const T minusY = vector.y - y;
				return math::sqrt( minusX*minusX + minusY*minusY );
			}
			
			
			/// Return the square of the distance from this vector to another in 2D Euclidean space.
			OM_FORCE_INLINE T getDistanceToSquared( const VectorND& vector ) const
			{
				const T minusX = vector.x - x;
				const T minusY = vector.y - y;
				return minusX*minusX + minusY*minusY;
			}
			
			
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			/// Return a pointer to the internal storage of this vector.
			OM_FORCE_INLINE T* toArray()
			{
				return v;
			}
			
			
			/// Return a const pointer to the internal storage of this vector.
			OM_FORCE_INLINE const T* toArray() const
			{
				return v;
			}
			
			
			/// Return the minimum coordinate of this vector.
			OM_FORCE_INLINE T getMin() const
			{
				return math::min( x, y );
			}
			
			
			/// Return the maximum coordinate of this vector.
			OM_FORCE_INLINE T getMax() const
			{
				return math::max( x, y );
			}
			
			
			/// Return a reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE T& get( Index index )
			{
				OM_DEBUG_ASSERT( index < 2 );
				return v[index];
			}
			
			
			/// Return a const reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE const T& get( Index index ) const
			{
				OM_DEBUG_ASSERT( index < 2 );
				return v[index];
			}
			
			
			/// Return a reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE T& operator [] ( Index index )
			{
				OM_DEBUG_ASSERT( index < 2 );
				
				return v[index];
			}
			
			
			/// Return a const reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE const T& operator [] ( Index index ) const
			{
				OM_DEBUG_ASSERT( index < 2 );
				
				return v[index];
			}
			
			
			/// Set the X and Y coordinates of the vector to the specified values.
			OM_FORCE_INLINE void setAll( T newX, T newY )
			{
				x = newX;
				y = newY;
			}
			
			
			/// Set the coordinate of this vector at the specified index to a new value.
			OM_FORCE_INLINE void set( Index index, T newValue )
			{
				OM_DEBUG_ASSERT( index < 2 );
				v[index] = newValue;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this vector is equal to another for all components.
			OM_FORCE_INLINE Bool operator == ( const VectorND& v ) const
			{
				return x == v.x && y == v.y;
			}
			
			
			/// Return whether or not this vector is not equal to another for any component.
			OM_FORCE_INLINE Bool operator != ( const VectorND& v ) const
			{
				return x != v.x || y != v.y;
			}
			
			
		//********************************************************************************
		//******	Unary Operators
			
			
			/// Negate each component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator - () const
			{
				return VectorND( -x, -y );
			}
			
			
			/// Return copy of the original vector.
			OM_FORCE_INLINE VectorND operator + () const
			{
				return VectorND( x, y );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add a scalar to every component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator + ( T value ) const
			{
				return VectorND( x + value, y + value );
			}
			
			
			/// Subtract a scalar from every component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator - ( T value ) const
			{
				return VectorND( x - value, y - value );
			}
			
			
			/// Multiply every component of this vector by a scalar and return the result.
			OM_FORCE_INLINE VectorND operator * ( T value ) const
			{
				return VectorND( x*value, y*value );
			}
			
			
			/// Divide every component of this vector by a scalar and return the result.
			OM_FORCE_INLINE VectorND operator / ( T value ) const
			{
				const T inverseValue = T(1) / value;
				return VectorND( x*inverseValue, y*inverseValue );
			}
			
			
			/// Add this vector to another and return the result and return the result.
			OM_FORCE_INLINE VectorND operator + ( const VectorND& vector ) const
			{
				return VectorND( x + vector.x, y + vector.y );
			}
			
			
			/// Subtract a vector from this vector and return the result.
			OM_FORCE_INLINE VectorND operator - ( const VectorND& vector ) const
			{
				return VectorND( x - vector.x, y - vector.y );
			}
			
			
			/// Multiply this vector by another vector and return the result.
			OM_FORCE_INLINE VectorND operator * ( const VectorND& vector ) const
			{
				return VectorND( x*vector.x, y*vector.y );
			}
			
			
			/// Divide this vector by another vector and return the result.
			OM_FORCE_INLINE VectorND operator / ( const VectorND& vector ) const
			{
				return VectorND( x/vector.x, y/vector.y );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to each component of this vector and modify the original vector.
			OM_FORCE_INLINE VectorND& operator += ( T value )
			{
				x += value;
				y += value;
				return *this;
			}
			
			
			/// Subtract a scalar from each component of this vector and modify the original vector.
			OM_FORCE_INLINE VectorND& operator -= ( T value )
			{
				x -= value;
				y -= value;
				return *this;
			}
			
			
			/// Multiply each component of this vector by a scalar and modify the original vector.
			OM_FORCE_INLINE VectorND& operator *= ( T value )
			{
				x *= value;
				y *= value;
				return *this;
			}
			
			
			/// Divide each component of this vector by a scalar and modify the original vector.
			OM_FORCE_INLINE VectorND& operator /= ( T value )
			{
				const T inverseValue = T(1) / value;
				x *= inverseValue;
				y *= inverseValue;
				return *this;
			}
			
			
			/// Add a vector to this vector, modifying this vector.
			OM_FORCE_INLINE VectorND& operator += ( const VectorND& vector )
			{
				x += vector.x;
				y += vector.y;
				return *this;
			}
			
			
			/// Subtract a vector from this vector, modifying this vector.
			OM_FORCE_INLINE VectorND& operator -= ( const VectorND& vector )
			{
				x -= vector.x;
				y -= vector.y;
				return *this;
			}
			
			
			/// Multiply this vector by another vector and modify this vector.
			OM_FORCE_INLINE VectorND& operator *= ( const VectorND& vector )
			{
				x *= vector.x;
				y *= vector.y;
				return *this;
			}
			
			
			/// Divide this vector by another vector and modify this vector.
			OM_FORCE_INLINE VectorND& operator /= ( const VectorND& vector )
			{
				x /= vector.x;
				y /= vector.y;
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this 2D vector into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "< " << x << ", " << y << " >";
				
				return buffer.toString();
			}
			
			
			/// Convert this 2D vector into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			union
			{
				/// An array that contains the vector components.
				T v[2];
				
				
				struct
				{
					/// The red component of a 2-component color.
					T r;
					
					/// The green component of a 2-component color.
					T g;
				};
				
				
				struct
				{
					/// The X coordinate of a 3D vector.
					T x;
					
					/// The Y coordinate of a 3D vector.
					T y;
				};
				
				
				/// A permutation of the vector elements that consists of the X element repeated twice.
				Swizzle2D<T,2,0,0> xx;
				
				/// A permutation of the vector elements that consists of the Y element repeated twice.
				Swizzle2D<T,2,1,1> yy;
				
				/// A permutation of the vector elements that is the same as the original vector.
				Swizzle2D<T,2,0,1> xy;
				
				/// A permutation of the vector elements that consists of the X and Y element swapped.
				Swizzle2D<T,2,1,0> yx;
				
			};
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// A constant vector with all elements equal to zero
			static const VectorND ZERO;
			
			
};




template <typename T>
const VectorND<T,2> VectorND<T,2>:: ZERO;




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a value to every component of the vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> operator + ( T value, const VectorND<T,2>& vector )
{
    return VectorND<T,2>( vector.x + value, vector.y + value );
}




/// Subtract every component of the vector from the value, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> operator - ( T value, const VectorND<T,2>& vector )
{
	return VectorND<T,2>( value - vector.x, value - vector.y );
}




/// Multiply every component of the vector with the value, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> operator * ( T value, const VectorND<T,2>& vector ) 
{
	return VectorND<T,2>( vector.x*value, vector.y*value );
}




/// Divide a value by every component of the vector, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> operator / ( T value, const VectorND<T,2>& vector ) 
{
	return VectorND<T,2>( value/vector.x, value/vector.y );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Other Vector Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute and return the dot product of two vectors.
template < typename T >
OM_FORCE_INLINE T dot( const VectorND<T,2>& vector1, const VectorND<T,2>& vector2 )
{
	return vector1.x*vector2.x + vector1.y*vector2.y;
}




/// Return the "perp" product of a vector.
/** 
  * This method computes and returns a vector representing
  * the perp product of the input vector. The result is perpendicular
  * to the original vector and with the same length.
  */
template < typename T >
OM_FORCE_INLINE VectorND<T,2> perp( const VectorND<T,2>& vector )
{
	return VectorND<T,2>(	-vector.y, vector.x );
}




/// Compute the midpoint (e.g. average) of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> midpoint( const VectorND<T,2>& vector1, const VectorND<T,2>& vector2 )
{
	return VectorND<T,2>(	(vector1.x + vector2.x)*T(0.5), 
						(vector1.y + vector2.y)*T(0.5) );
}




/// Return the absolute value of the specified vector, such that the every component is positive.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> abs( const VectorND<T,2>& vector )
{
	return VectorND<T,2>( math::abs(vector.x), math::abs(vector.y) );
}




/// Compute the component-wise minimum of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> min( const VectorND<T,2>& vector1, const VectorND<T,2>& vector2 )
{
	return VectorND<T,2>(	math::min( vector1.x, vector2.x ),
						math::min( vector1.y, vector2.y ) );
}




/// Compute the component-wise minimum of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> max( const VectorND<T,2>& vector1, const VectorND<T,2>& vector2 )
{
	return VectorND<T,2>(	math::max( vector1.x, vector2.x ),
						math::max( vector1.y, vector2.y ) );
}




/// Return the floor of the specified vector, rounding each component down to the nearest integer.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> floor( const VectorND<T,2>& vector )
{
	return VectorND<T,2>( math::floor(vector.x), math::floor(vector.y) );
}




/// Return the floor of the specified vector, rounding each component up to the nearest integer.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> ceiling( const VectorND<T,2>& vector )
{
	return VectorND<T,2>( math::ceiling(vector.x), math::ceiling(vector.y) );
}




/// Return the component-wise modulus of the specified vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> mod( const VectorND<T,2>& vector, const T& modulus )
{
	return VectorND<T,2>( math::mod(vector.x, modulus), math::mod(vector.y, modulus) );
}




/// Return the component-wise modulus of the specified vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> mod( const VectorND<T,2>& vector, const VectorND<T,2>& modulus )
{
	return VectorND<T,2>( math::mod(vector.x, modulus.x), math::mod(vector.y, modulus.y) );
}




/// Return whether or not any component of this vector is Not-A-Number.
template < typename T >
OM_FORCE_INLINE Bool isNAN( const VectorND<T,2>& vector)
{
	return math::isNAN(vector.x) || math::isNAN(vector.y);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Swizzle Class Declaration
//############		
//##########################################################################################
//##########################################################################################




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Size dimension, Index i0, Index i1 >
class Swizzle2D
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,2> () const
		{
			return VectorND<T,2>( v[i0], v[i1] );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,2>& operator = ( const VectorND<T,2>& other )
		{
			v[i0] = other.x;
			v[i1] = other.y;
			return *(VectorND<T,2>*)this;
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator + ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] + scalar, v[i1] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator - ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] - scalar, v[i1] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator * ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] * scalar, v[i1] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator / ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] / scalar, v[i1] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator + ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] + other.x, v[i1] + other.y );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator - ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] - other.x, v[i1] - other.y );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator * ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] * other.x, v[i1] * other.y );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator / ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] / other.x, v[i1] / other.y );
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,2>& operator += ( const VectorND<T,2>& other )
		{
			v[i0] += other.x;
			v[i1] += other.y;
			return *(VectorND<T,2>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,2>& operator -= ( const VectorND<T,2>& other )
		{
			v[i0] -= other.x;
			v[i1] -= other.y;
			return *(VectorND<T,2>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,2>& operator *= ( const VectorND<T,2>& other )
		{
			v[i0] *= other.x;
			v[i1] *= other.y;
			return *(VectorND<T,2>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,2>& operator /= ( const VectorND<T,2>& other )
		{
			v[i0] /= other.x;
			v[i1] /= other.y;
			return *(VectorND<T,2>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,2>( v[i0], v[i1] ).toString();
		}
		
	private:
		
		/// Storage for the vector components.
		T v[dimension];
		
};




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Size dimension, Index i0 >
class Swizzle2D<T,dimension,i0,i0>
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,2> () const
		{
			return VectorND<T,2>( v[i0], v[i0] );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,2>& operator = ( T value )
		{
			v[i0] = value;
			return *(VectorND<T,2>*)this;
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator + ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] + scalar, v[i0] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator - ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] - scalar, v[i0] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator * ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] * scalar, v[i0] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator / ( T scalar ) const
		{
			return VectorND<T,2>( v[i0] / scalar, v[i0] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator + ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] + other.x, v[i0] + other.y );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator - ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] - other.x, v[i0] - other.y );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator * ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] * other.x, v[i0] * other.y );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,2> operator / ( const VectorND<T,2>& other ) const
		{
			return VectorND<T,2>( v[i0] / other.x, v[i0] / other.y );
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,2>& operator += ( T value )
		{
			v[i0] += value;
			return *(VectorND<T,2>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,2>& operator -= ( T value )
		{
			v[i0] -= value;
			return *(VectorND<T,2>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,2>& operator *= ( T value )
		{
			v[i0] *= value;
			return *(VectorND<T,2>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,2>& operator /= ( T value )
		{
			v[i0] /= value;
			return *(VectorND<T,2>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,2>( v[i0], v[i0] ).toString();
		}
		
	private:
		
		/// Storage for the vector components.
		T v[dimension];
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Swizzle Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a scalar to every component of the vector.
template < typename T, Index dimension, Index i0, Index i1 >
OM_FORCE_INLINE VectorND<T,2> operator + ( T value, const Swizzle2D<T,dimension,i0,i1>& vector )
{
	return value + vector.operator VectorND<T,2>();
}




/// Subtract a scalar to every component of the vector.
template < typename T, Index dimension, Index i0, Index i1 >
OM_FORCE_INLINE VectorND<T,2> operator - ( T value, const Swizzle2D<T,dimension,i0,i1>& vector )
{
	return value - vector.operator VectorND<T,2>();
}




/// Multiply every component of a vector with scalar, returning a vector result.
template < typename T, Index dimension, Index i0, Index i1 >
OM_FORCE_INLINE VectorND<T,2> operator * ( T value, const Swizzle2D<T,dimension,i0,i1>& vector )
{
	return value * vector.operator VectorND<T,2>();
}




/// Divide a value by every component of the vector, returning a vector result.
template < typename T, Index dimension, Index i0, Index i1 >
OM_FORCE_INLINE VectorND<T,2> operator / ( T value, const Swizzle2D<T,dimension,i0,i1>& vector )
{
	return value / vector.operator VectorND<T,2>();
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_VECTOR_2D_H
