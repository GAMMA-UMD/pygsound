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

#ifndef INCLUDE_OM_VECTOR_3D_H
#define INCLUDE_OM_VECTOR_3D_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "omVectorND.h"
#include "omVector2D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


template < typename T, Size dimension, Index i0, Index i1, Index i2 >
class Swizzle3D;




//********************************************************************************
/// A class that represents a 3-dimensional point or vector.
template < typename T >
class VectorND<T,3>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new 3D vector with all elements equal to zero.
			OM_FORCE_INLINE VectorND()
				:	x( T(0) ),
					y( T(0) ),
					z( T(0) )
			{
			}
			
			
			/// Create a new 3D vector with all elements equal to a single value.
			OM_FORCE_INLINE explicit VectorND( T value )
				:	x( value ),
					y( value ),
					z( value )
			{
			}
			
			
			/// Create a new 3D vector from a pointer to a 3 element array.
			OM_FORCE_INLINE explicit VectorND( const T array[3] )
				:	x( array[0] ),
					y( array[1] ),
					z( array[2] )
			{
			}
			
			
			/// Create a new 3D vector by specifying its x, y, and z values.
			OM_FORCE_INLINE VectorND( T newX, T newY, T newZ )
				:	x( newX ),
					y( newY ),
					z( newZ )
			{
			}
			
			
			/// Create a new 3D vector from a 2D vector and a value for the Z coordinate.
			OM_FORCE_INLINE VectorND( const VectorND<T,2>& vector, T newZ )
				:	x( vector.x ),
					y( vector.y ),
					z( newZ )
			{
			}
			
			
			/// Create a new 3D vector from a value for the X coordinate and a 2D vector.
			OM_FORCE_INLINE VectorND( T newX, const VectorND<T,2>& vector )
				:	x( newX ),
					y( vector.x ),
					z( vector.y )
			{
			}
			
			
			/// Create a new 3D vector from an existing vector with a different template type.
			template < typename U >
			OM_FORCE_INLINE VectorND( const VectorND<U,3>& vector )
				:	x( (T)vector.x ),
					y( (T)vector.y ),
					z( (T)vector.z )
			{
			}
			
			
			/// Create a new 3D vector from an existing vector with the same template type.
			/**
			  * This is declared because the auto-generated one was not working with the crazy uniform mess.
			  */
			OM_FORCE_INLINE VectorND( const VectorND& vector )
				:	x( vector.x ),
					y( vector.y ),
					z( vector.z )
			{
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Create a new 3D vector from an existing vector with a different template type.
			/**
			  * This is declared because the auto-generated one was not working with the crazy uniform mess.
			  */
			OM_FORCE_INLINE VectorND& operator = ( const VectorND& vector )
			{
				x = vector.x;
				y = vector.y;
				z = vector.z;
				return *this;
			}
			
			
		//********************************************************************************
		//******	Static Construction Methods
			
			
			/// Return a 3D cartesian unit-length vector for the specified spherical coordinates.
			OM_FORCE_INLINE static VectorND spherical( T theta, T phi )
			{
				const T sinTheta = math::sin(theta);
				return VectorND( sinTheta*math::cos(phi),
								sinTheta*math::sin(phi),
								math::cos(theta) );
			}
			
			
			/// Return a 3D cartesian vector for the specified spherical coordinates.
			OM_FORCE_INLINE static VectorND spherical( T theta, T phi, T r )
			{
				const T sinTheta = math::sin(theta);
				return r*VectorND( sinTheta*math::cos(phi),
								sinTheta*math::sin(phi),
								math::cos(theta) );
			}
			
			
			/// Return a 3D cartesian vector for the specified cylindrical coordinates.
			OM_FORCE_INLINE static VectorND cylindrical( T theta, T r, T z )
			{
				return VectorND( r*math::cos(theta), r*math::sin(theta), z );
			}
			
			
			/// Convert the specified polar angle in radians to a unit-length 3D vector in the XY plane.
			static VectorND polarXY( T angle )
			{
				return VectorND( math::cos( angle ), math::sin( angle ), T(0) );
			}
			
			
			/// Convert the specified polar coordinates in radians to an r-length 3D vector in the XY plane.
			static VectorND polarXY( T angle, T r )
			{
				return VectorND( r*math::cos( angle ), r*math::sin( angle ), T(0) );
			}
			
			
			/// Convert the specified polar angle in radians to a unit-length 3D vector in the XZ plane.
			static VectorND polarXZ( T angle )
			{
				return VectorND( math::sin( angle ), T(0), math::cos( angle ) );
			}
			
			
			/// Convert the specified polar coordinates in radians to an r-length 3D vector in the XZ plane.
			static VectorND polarXZ( T angle, T r )
			{
				return VectorND( r*math::sin( angle ), T(0), r*math::cos( angle ) );
			}
			
			
			/// Convert the specified polar angle in radians to a unit-length 3D vector in the YZ plane.
			static VectorND polarYZ( T angle )
			{
				return VectorND( T(0), math::sin( angle ), math::cos( angle ) );
			}
			
			
			/// Convert the specified polar coordinates in radians to an r-length 3D vector in the YZ plane.
			static VectorND polarYZ( T angle, T r )
			{
				return VectorND( T(0), r*math::sin( angle ), r*math::cos( angle ) );
			}
			
			
		//********************************************************************************
		//******	Magnitude Methods
			
			
			/// Return the magnitude of this vector (the length).
			OM_FORCE_INLINE T getMagnitude() const
			{
				return math::sqrt( x*x + y*y + z*z );
			}
			
			
			/// Return the square of the magnitude of this vector.
			OM_FORCE_INLINE T getMagnitudeSquared() const
			{
				return x*x + y*y + z*z;
			}
			
			
			/// Return a normalized version of this vector.
			OM_FORCE_INLINE VectorND normalize() const
			{
				T inverseMagnitude = T(1)/math::sqrt( x*x + y*y + z*z );
				
				return VectorND( x*inverseMagnitude,
								y*inverseMagnitude,
								z*inverseMagnitude );
			}
			
			
			/// Return a normalized version of this vector, placing the vector's magnitude in the output parameter.
			OM_FORCE_INLINE VectorND normalize( T& magnitude ) const
			{
				magnitude = math::sqrt( x*x + y*y + z*z );
				T inverseMagnitude = T(1)/magnitude;
				
				return VectorND( x*inverseMagnitude,
								y*inverseMagnitude,
								z*inverseMagnitude );
			}
			
			
			/// Project this vector on another vector and return the projected vector.
			OM_FORCE_INLINE VectorND projectOn( const VectorND& vector ) const
			{
				VectorND norm = vector.normalize();
				
				return (x*norm.x + y*norm.y + z*norm.z)*norm;
			}
			
			
			/// Project this vector on a normalized vector and return the projected vector.
			OM_FORCE_INLINE VectorND projectOnNormalized( const VectorND& vector ) const
			{
				return (x*vector.x + y*vector.y + z*vector.z)*vector;
			}
			
			
		//********************************************************************************
		//******	Distance Methods
			
			
			/// Return the distance from this vector to another in 3D Euclidean space.
			OM_FORCE_INLINE T getDistanceTo( const VectorND& vector ) const
			{
				T minusX = vector.x - x;
				T minusY = vector.y - y;
				T minusZ = vector.z - z;
				
				return math::sqrt( minusX*minusX + minusY*minusY + minusZ*minusZ );
			}
			
			
			/// Return the square of the distance from this vector to another in 3D Euclidean space.
			OM_FORCE_INLINE T getDistanceToSquared( const VectorND& vector ) const
			{
				T minusX = vector.x - x;
				T minusY = vector.y - y;
				T minusZ = vector.z - z;
				
				return minusX*minusX + minusY*minusY + minusZ*minusZ;
			}
			
			
		//********************************************************************************
		//******	Element Accessor Methods
			
			
			/// Return a pointer to the internal storage of this vector.
			OM_FORCE_INLINE T* toArray()
			{
				return &x;
			}
			
			
			/// Return a const pointer to the internal storage of this vector.
			OM_FORCE_INLINE const T* toArray() const
			{
				return v;
			}
			
			
			/// Return the minimum component of this vector.
			OM_FORCE_INLINE T getMin() const
			{
				return math::min( math::min( x, y ), z );
			}
			
			
			/// Return the maximum component of this vector.
			OM_FORCE_INLINE T getMax() const
			{
				return math::max( math::max( x, y ), z );
			}
			
			
			/// Return a reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE T& get( Index index )
			{
				OM_DEBUG_ASSERT( index < 3 );
				return v[index];
			}
			
			
			/// Return a const reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE const T& get( Index index ) const
			{
				OM_DEBUG_ASSERT( index < 3 );
				return v[index];
			}
			
			
			/// Return a reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE T& operator [] ( Index index )
			{
				OM_DEBUG_ASSERT( index < 3 );
				return v[index];
			}
			
			
			/// Return a const reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE const T& operator [] ( Index index ) const
			{
				OM_DEBUG_ASSERT( index < 3 );
				return v[index];
			}
			
			
			/// Set the X, Y, and Z coordinates of the vector to the specified values.
			OM_FORCE_INLINE void setAll( T newX, T newY, T newZ )
			{
				x = newX;
				y = newY;
				z = newZ;
			}
			
			
			/// Set the coordinate of this vector at the specified index to a new value.
			OM_FORCE_INLINE void set( Index index, T newValue )
			{
				OM_DEBUG_ASSERT( index < 3 );
				v[index] = newValue;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this vector is equal to another for all components.
			OM_FORCE_INLINE Bool operator == ( const VectorND& v ) const
			{
				return x == v.x && y == v.y && z == v.z;
			}
			
			
			/// Return whether or not this vector is not equal to another for any component.
			OM_FORCE_INLINE Bool operator != ( const VectorND& v ) const
			{
				return x != v.x || y != v.y || z != v.z;
			}
			
			
		//********************************************************************************
		//******	Unary Operators
			
			
			/// Negate each component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator - () const
			{
				return VectorND( -x, -y, -z );
			}
			
			
			/// Return copy of the original vector.
			OM_FORCE_INLINE VectorND operator + () const
			{
				return VectorND( x, y, z );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add a scalar to every component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator + ( T value ) const
			{
				return VectorND( x + value, y + value, z + value );
			}
			
			
			/// Subtract a scalar from every component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator - ( T value ) const
			{
				return VectorND( x - value, y - value, z - value );
			}
			
			
			/// Multiply every component of this vector by a scalar and return the result.
			OM_FORCE_INLINE VectorND operator * ( T value ) const
			{
				return VectorND( x*value, y*value, z*value );
			}
			
			
			/// Divide every component of this vector by a scalar and return the result.
			OM_FORCE_INLINE VectorND operator / ( T value ) const
			{
				T inverseValue = T(1) / value;
				
				return VectorND( x*inverseValue, y*inverseValue, z*inverseValue );
			}
			
			
			/// Add this vector to another and return the result and return the result.
			OM_FORCE_INLINE VectorND operator + ( const VectorND& vector ) const
			{
				return VectorND( x + vector.x, y + vector.y, z + vector.z );
			}
			
			
			/// Subtract a vector from this vector and return the result.
			OM_FORCE_INLINE VectorND operator - ( const VectorND& vector ) const
			{
				return VectorND( x - vector.x, y - vector.y, z - vector.z );
			}
			
			
			/// Multiply this vector by another vector and return the result.
			OM_FORCE_INLINE VectorND operator * ( const VectorND& vector ) const
			{
				return VectorND( x*vector.x, y*vector.y, z*vector.z );
			}
			
			
			/// Divide this vector by another vector and return the result.
			OM_FORCE_INLINE VectorND operator / ( const VectorND& vector ) const
			{
				return VectorND( x/vector.x, y/vector.y, z/vector.z );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to each component of this vector and modify the original vector.
			OM_FORCE_INLINE VectorND& operator += ( const T& value )
			{
				x += value;
				y += value;
				z += value;
				
				return *this;
			}
			
			
			/// Subtract a scalar from each component of this vector and modify the original vector.
			OM_FORCE_INLINE VectorND& operator -= ( const T& value )
			{
				x -= value;
				y -= value;
				z -= value;
				
				return *this;
			}
			
			
			/// Multiply each component of this vector by a scalar and modify the original vector.
			OM_FORCE_INLINE VectorND& operator *= ( const T& value )
			{
				x *= value;
				y *= value;
				z *= value;
				
				return *this;
			}
			
			
			/// Divide each component of this vector by a scalar and modify the original vector.
			OM_FORCE_INLINE VectorND& operator /= ( const T& value )
			{
				const T inverseValue = T(1) / value;
				
				x *= inverseValue;
				y *= inverseValue;
				z *= inverseValue;
				
				return *this;
			}
			
			
			/// Add a vector to this vector, modifying this original vector.
			OM_FORCE_INLINE VectorND& operator += ( const VectorND& vector )
			{
				x += vector.x;
				y += vector.y;
				z += vector.z;
				
				return *this;
			}
			
			
			/// Subtract a vector from this vector, modifying this original vector.
			OM_FORCE_INLINE VectorND& operator -= ( const VectorND& vector )
			{
				x -= vector.x;
				y -= vector.y;
				z -= vector.z;
				
				return *this;
			}
			
			
			/// Multiply this vector by another vector and modify this vector.
			OM_FORCE_INLINE VectorND& operator *= ( const VectorND& vector )
			{
				x *= vector.x;
				y *= vector.y;
				z *= vector.z;
				
				return *this;
			}
			
			
			/// Divide this vector by another vector and modify this vector.
			OM_FORCE_INLINE VectorND& operator /= ( const VectorND& vector )
			{
				x /= vector.x;
				y /= vector.y;
				z /= vector.z;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this 3D vector into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "< " << x << ", " << y << ", " << z << " >";
				
				return buffer.toString();
			}
			
			
			/// Convert this 3D vector into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			union
			{
				/// An array that contains the vector components.
				T v[3];
				
				
				/// A struct for when the vector should be interpreted as a 3-channel color.
				struct
				{
					/// The red component of a 3-component color.
					T r;
					
					/// The green component of a 3-component color.
					T g;
					
					/// The blue component of a 3-component color.
					T b;
				};
				
				
				/// A struct for when the vector should be interpreted as a 3D cartesian vector.
				struct
				{
					/// The X coordinate of a 3D vector.
					T x;
					
					/// The Y coordinate of a 3D vector.
					T y;
					
					/// The Z coordinate of a 3D vector.
					T z;
				};
				
				
				Swizzle2D<T,3,0,0> xx;
				Swizzle2D<T,3,0,0> rr;
				Swizzle2D<T,3,1,1> yy;
				Swizzle2D<T,3,1,1> gg;
				Swizzle2D<T,3,2,2> zz;
				Swizzle2D<T,3,2,2> bb;
				
				Swizzle2D<T,3,0,1> xy;
				Swizzle2D<T,3,0,1> rg;
				Swizzle2D<T,3,0,2> xz;
				Swizzle2D<T,3,0,2> rb;
				
				Swizzle2D<T,3,1,0> yx;
				Swizzle2D<T,3,1,0> gr;
				Swizzle2D<T,3,1,2> yz;
				Swizzle2D<T,3,1,2> gb;
				
				Swizzle2D<T,3,2,0> zx;
				Swizzle2D<T,3,2,0> br;
				Swizzle2D<T,3,2,1> zy;
				Swizzle2D<T,3,2,1> bg;
				
				
				Swizzle3D<T,3,0,0,0> xxx;
				Swizzle3D<T,3,0,0,0> rrr;
				Swizzle3D<T,3,1,1,1> yyy;
				Swizzle3D<T,3,1,1,1> ggg;
				Swizzle3D<T,3,2,2,2> zzz;
				Swizzle3D<T,3,2,2,2> bbb;
				
				Swizzle3D<T,3,0,1,1> xyy;
				Swizzle3D<T,3,0,1,1> rgg;
				Swizzle3D<T,3,1,0,1> yxy;
				Swizzle3D<T,3,1,0,1> grg;
				Swizzle3D<T,3,1,1,0> yyx;
				Swizzle3D<T,3,1,1,0> ggr;
				
				Swizzle3D<T,3,0,2,2> xzz;
				Swizzle3D<T,3,0,2,2> rbb;
				Swizzle3D<T,3,2,0,2> zxz;
				Swizzle3D<T,3,2,0,2> brb;
				Swizzle3D<T,3,2,2,0> zzx;
				Swizzle3D<T,3,2,2,0> bbr;
				
				Swizzle3D<T,3,1,0,0> yxx;
				Swizzle3D<T,3,1,0,0> grr;
				Swizzle3D<T,3,0,1,0> xyx;
				Swizzle3D<T,3,0,1,0> rgr;
				Swizzle3D<T,3,0,0,1> xxy;
				Swizzle3D<T,3,0,0,1> rrg;
				
				Swizzle3D<T,3,1,2,2> yzz;
				Swizzle3D<T,3,1,2,2> gbb;
				Swizzle3D<T,3,2,1,2> zyz;
				Swizzle3D<T,3,2,1,2> bgb;
				Swizzle3D<T,3,2,2,1> zzy;
				Swizzle3D<T,3,2,2,1> bbg;
				
				Swizzle3D<T,3,2,0,0> zxx;
				Swizzle3D<T,3,2,0,0> brr;
				Swizzle3D<T,3,0,2,0> xzx;
				Swizzle3D<T,3,0,2,0> rbr;
				Swizzle3D<T,3,0,0,2> xxz;
				Swizzle3D<T,3,0,0,2> rrb;
				
				Swizzle3D<T,3,2,1,1> zyy;
				Swizzle3D<T,3,2,1,1> bgg;
				Swizzle3D<T,3,1,2,1> yzy;
				Swizzle3D<T,3,1,2,1> gbg;
				Swizzle3D<T,3,1,1,2> yyz;
				Swizzle3D<T,3,1,1,2> ggb;
				
				Swizzle3D<T,3,0,1,2> xyz;
				Swizzle3D<T,3,0,1,2> rgb;
				Swizzle3D<T,3,0,2,1> xzy;
				Swizzle3D<T,3,0,2,1> rbg;
				Swizzle3D<T,3,1,0,2> yxz;
				Swizzle3D<T,3,1,0,2> grb;
				Swizzle3D<T,3,1,2,0> yzx;
				Swizzle3D<T,3,1,2,0> gbr;
				Swizzle3D<T,3,2,1,0> zyx;
				Swizzle3D<T,3,2,1,0> bgr;
				Swizzle3D<T,3,2,0,1> zxy;
				Swizzle3D<T,3,2,0,1> brg;
			};
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// A constant vector with all elements equal to zero
			static const VectorND ZERO;
			
			
};




template <typename T>
const VectorND<T,3> VectorND<T,3>:: ZERO;




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a value to every component of the vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> operator + ( T value, const VectorND<T,3>& vector )
{
    return VectorND<T,3>( vector.x + value, vector.y + value, vector.z + value );
}




/// Subtract every component of the vector from the value, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> operator - ( T value, const VectorND<T,3>& vector )
{
	return VectorND<T,3>( value - vector.x, value - vector.y, value - vector.z );
}




/// Multiply every component of the vector with the value, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> operator * ( T value, const VectorND<T,3>& vector ) 
{
	return VectorND<T,3>( vector.x*value, vector.y*value,  vector.z*value );
}




/// Divide a value by every component of the vector, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> operator / ( T value, const VectorND<T,3>& vector ) 
{
	return VectorND<T,3>( value/vector.x, value/vector.y,  value/vector.z );
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
OM_FORCE_INLINE T dot( const VectorND<T,3>& vector1, const VectorND<T,3>& vector2 )
{
	return vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z;
}




/// Compute and return the cross product of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> cross( const VectorND<T,3>& vector1, const VectorND<T,3>& vector2 )
{
	return VectorND<T,3>( vector1.y*vector2.z - vector1.z*vector2.y,
						vector1.z*vector2.x - vector1.x*vector2.z,
						vector1.x*vector2.y - vector1.y*vector2.x );
}




/// Compute the midpoint (e.g. average) of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> midpoint( const VectorND<T,3>& vector1, const VectorND<T,3>& vector2 )
{
	return VectorND<T,3>(	(vector1.x + vector2.x)*T(0.5), 
						(vector1.y + vector2.y)*T(0.5),
						(vector1.z + vector2.z)*T(0.5) );
}




/// Return the absolute value of the specified vector, such that the every component is positive.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> abs( const VectorND<T,3>& vector )
{
	return VectorND<T,3>( math::abs(vector.x), math::abs(vector.y), math::abs(vector.z) );
}




/// Compute the component-wise minimum of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> min( const VectorND<T,3>& vector1, const VectorND<T,3>& vector2 )
{
	return VectorND<T,3>(	math::min( vector1.x, vector2.x ),
						math::min( vector1.y, vector2.y ),
						math::min( vector1.z, vector2.z ) );
}




/// Compute the component-wise minimum of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> max( const VectorND<T,3>& vector1, const VectorND<T,3>& vector2 )
{
	return VectorND<T,3>(	math::max( vector1.x, vector2.x ),
						math::max( vector1.y, vector2.y ),
						math::max( vector1.z, vector2.z ) );
}




/// Return the floor of the specified vector, rounding each component down to the nearest integer.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> floor( const VectorND<T,3>& vector )
{
	return VectorND<T,3>( math::floor(vector.x), math::floor(vector.y), math::floor(vector.z) );
}




/// Return the floor of the specified vector, rounding each component up to the nearest integer.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> ceiling( const VectorND<T,3>& vector )
{
	return VectorND<T,3>( math::ceiling(vector.x), math::ceiling(vector.y), math::ceiling(vector.z) );
}




/// Return the component-wise modulus of the specified vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> mod( const VectorND<T,3>& vector, const T& modulus )
{
	return VectorND<T,3>( math::mod(vector.x, modulus), math::mod(vector.y, modulus), math::mod(vector.z, modulus) );
}




/// Return the component-wise modulus of the specified vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,3> mod( const VectorND<T,3>& vector, const VectorND<T,3>& modulus )
{
	return VectorND<T,3>( math::mod(vector.x, modulus.x), math::mod(vector.y, modulus.y), math::mod(vector.z, modulus.z) );
}




/// Return whether or not any component of this vector is Not-A-Number.
template < typename T >
OM_FORCE_INLINE Bool isNAN( const VectorND<T,3>& vector)
{
	return math::isNAN(vector.x) || math::isNAN(vector.y) || math::isNAN(vector.z);
}




/// Compute and return the barycentric coordinates of a point relative to the given triangle vertices.
/**
  * The projection of the point into the triangle's plane should lie within the triangle.
  */
template < typename T >
OM_FORCE_INLINE VectorND<T,3> barycentric( const VectorND<T,3>& v1, const VectorND<T,3>& v2, 
										const VectorND<T,3>& v3, const VectorND<T,3>& point )
{
	VectorND<T,3> e0 = v2 - v1;
	VectorND<T,3> e1 = v3 - v1;
	VectorND<T,3> e2 = point - v1;
	
	T d00 = math::dot( e0, e0 );
	T d01 = math::dot( e0, e1 );
	T d11 = math::dot( e1, e1 );
	T d20 = math::dot( e2, e0 );
	T d21 = math::dot( e2, e1 );
	T inverseDenom = T(1) / (d00*d11 - d01*d01);
	
	T v = (d11*d20 - d01*d21) * inverseDenom;
	T w = (d00*d21 - d01*d20) * inverseDenom;
	T u = T(1) - v - w;
	
	return VectorND<T,3>( u, v, w );
}




/// Compute the point of closest approach on the first of two lines.
/**
  * The lines are defined as L(t) = p + t*v, where v is a unit vector.
  * The lines must not be parallel.
  * The return value is the t value of the first line of the closest point
  * to the other line.
  */
template < typename T >
OM_FORCE_INLINE T closestApproach( const VectorND<T,3>& p1, const VectorND<T,3>& v1, 
									const VectorND<T,3>& p2, const VectorND<T,3>& v2 )
{
	T v1DotV2 = math::dot( v1, v2 );
	VectorND<T,3> p1ToP2 = p2 - p1;
	
	// calculate the parameters of the lines of the points of closest approach.
	return (math::dot( p1ToP2, v1 ) - math::dot( p1ToP2, v2 )*v1DotV2) / ( T(1) - v1DotV2*v1DotV2 );
}




/// Compute the points of closest approach on two lines.
/**
  * The lines are defined as L(t) = p + t*v, where v is a unit vector.
  * The lines must not be parallel.
  * The t values of the closest points are placed in the output parameters.
  */
template < typename T >
OM_FORCE_INLINE void closestApproach( const VectorND<T,3>& p1, const VectorND<T,3>& v1, 
									const VectorND<T,3>& p2, const VectorND<T,3>& v2,
									T& t1, T& t2 )
{
	T v1DotV2 = math::dot( v1, v2 );
	VectorND<T,3> p1ToP2 = p2 - p1;
	
	// calculate the parameters of the lines of the points of closest approach.
	t1 = (math::dot( p1ToP2, v1 ) - math::dot( p1ToP2, v2 )*v1DotV2) / ( T(1) - v1DotV2*v1DotV2 );
	t2 = (t1*v1DotV2 - math::dot( p1ToP2, v2 ));
}




//##########################################################################################
//##########################################################################################
//############		
//############		Swizzle Class Declaration
//############		
//##########################################################################################
//##########################################################################################




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Index dimension, Index i0, Index i1, Index i2 >
class Swizzle3D
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,3> () const
		{
			return VectorND<T,3>( v[i0], v[i1], v[i2] );
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] + scalar, v[i1] + scalar, v[i2] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] - scalar, v[i1] - scalar, v[i2] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] * scalar, v[i1] * scalar, v[i2] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] / scalar, v[i1] / scalar, v[i2] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] + other.x, v[i1] + other.y, v[i2] + other.z );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] - other.x, v[i1] - other.y, v[i2] - other.z );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] * other.x, v[i1] * other.y, v[i2] * other.z );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] / other.x, v[i1] / other.y, v[i2] / other.z );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator = ( const VectorND<T,3>& other )
		{
			v[i0] = other.x;
			v[i1] = other.y;
			v[i2] = other.z;
			return *(VectorND<T,3>*)this;
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator += ( const VectorND<T,3>& other )
		{
			v[i0] += other.x;
			v[i1] += other.y;
			v[i2] += other.z;
			return *(VectorND<T,3>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator -= ( const VectorND<T,3>& other )
		{
			v[i0] -= other.x;
			v[i1] -= other.y;
			v[i2] -= other.z;
			return *(VectorND<T,3>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator *= ( const VectorND<T,3>& other )
		{
			v[i0] *= other.x;
			v[i1] *= other.y;
			v[i2] *= other.z;
			return *(VectorND<T,3>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator /= ( const VectorND<T,3>& other )
		{
			v[i0] /= other.x;
			v[i1] /= other.y;
			v[i2] /= other.z;
			return *(VectorND<T,3>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,3>( v[i0], v[i1], v[i2] ).toString();
		}
		
	private:
		
		/// Storage for the vector components.
		T v[dimension];
		
};




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Index dimension, Index i0, Index i1 >
class Swizzle3D<T,dimension,i0,i1,i1>
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,3> () const
		{
			return VectorND<T,3>( v[i0], v[i1], v[i1] );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator = ( T value )
		{
			v[i0] = value;
			v[i1] = value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] + scalar, v[i1] + scalar, v[i1] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] - scalar, v[i1] - scalar, v[i1] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] * scalar, v[i1] * scalar, v[i1] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] / scalar, v[i1] / scalar, v[i1] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] + other.x, v[i1] + other.y, v[i1] + other.z );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] - other.x, v[i1] - other.y, v[i1] - other.z );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] * other.x, v[i1] * other.y, v[i1] * other.z );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] / other.x, v[i1] / other.y, v[i1] / other.z );
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator += ( T value )
		{
			v[i0] += value;
			v[i1] += value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator -= ( T value )
		{
			v[i0] -= value;
			v[i1] -= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator *= ( T value )
		{
			v[i0] *= value;
			v[i1] *= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator /= ( T value )
		{
			v[i0] /= value;
			v[i1] /= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,3>( v[i0], v[i1], v[i1] ).toString();
		}
		
	private:
		
		/// Storage for the vector components.
		T v[dimension];
		
};




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Index dimension, Index i0, Index i1 >
class Swizzle3D<T,dimension,i1,i0,i1>
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,3> () const
		{
			return VectorND<T,3>( v[i1], v[i0], v[i1] );
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] + scalar, v[i0] + scalar, v[i1] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] - scalar, v[i0] - scalar, v[i1] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] * scalar, v[i0] * scalar, v[i1] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] / scalar, v[i0] / scalar, v[i1] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] + other.x, v[i0] + other.y, v[i1] + other.z );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] - other.x, v[i0] - other.y, v[i1] - other.z );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] * other.x, v[i0] * other.y, v[i1] * other.z );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] / other.x, v[i0] / other.y, v[i1] / other.z );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator = ( T value )
		{
			v[i0] = value;
			v[i1] = value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator += ( T value )
		{
			v[i0] += value;
			v[i1] += value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator -= ( T value )
		{
			v[i0] -= value;
			v[i1] -= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator *= ( T value )
		{
			v[i0] *= value;
			v[i1] *= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator /= ( T value )
		{
			v[i0] /= value;
			v[i1] /= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,3>( v[i1], v[i0], v[i1] ).toString();
		}
		
	private:
		
		/// Storage for the vector components.
		T v[dimension];
		
};




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Index dimension, Index i0, Index i1 >
class Swizzle3D<T,dimension,i1,i1,i0>
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,3> () const
		{
			return VectorND<T,3>( v[i1], v[i1], v[i0] );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator = ( T value )
		{
			v[i0] = value;
			v[i1] = value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] + scalar, v[i1] + scalar, v[i0] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] - scalar, v[i1] - scalar, v[i0] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] * scalar, v[i1] * scalar, v[i0] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( T scalar ) const
		{
			return VectorND<T,3>( v[i1] / scalar, v[i1] / scalar, v[i0] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] + other.x, v[i1] + other.y, v[i0] + other.z );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] - other.x, v[i1] - other.y, v[i0] - other.z );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] * other.x, v[i1] * other.y, v[i0] * other.z );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i1] / other.x, v[i1] / other.y, v[i0] / other.z );
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator += ( T value )
		{
			v[i0] += value;
			v[i1] += value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator -= ( T value )
		{
			v[i0] -= value;
			v[i1] -= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator *= ( T value )
		{
			v[i0] *= value;
			v[i1] *= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator /= ( T value )
		{
			v[i0] /= value;
			v[i1] /= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,3>( v[i1], v[i1], v[i0] ).toString();
		}
		
	private:
		
		/// Storage for the vector components.
		T v[dimension];
		
};




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Index dimension, Index i0 >
class Swizzle3D<T,dimension,i0,i0,i0>
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,3> () const
		{
			return VectorND<T,3>( v[i0], v[i0], v[i0] );
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] + scalar, v[i0] + scalar, v[i0] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] - scalar, v[i0] - scalar, v[i0] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] * scalar, v[i0] * scalar, v[i0] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( T scalar ) const
		{
			return VectorND<T,3>( v[i0] / scalar, v[i0] / scalar, v[i0] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator + ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] + other.x, v[i0] + other.y, v[i0] + other.z );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator - ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] - other.x, v[i0] - other.y, v[i0] - other.z );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator * ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] * other.x, v[i0] * other.y, v[i0] * other.z );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,3> operator / ( const VectorND<T,3>& other ) const
		{
			return VectorND<T,3>( v[i0] / other.x, v[i0] / other.y, v[i0] / other.z );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator = ( T value )
		{
			v[i0] = value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator += ( T value )
		{
			v[i0] += value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,3>& operator -= ( T value )
		{
			v[i0] -= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator *= ( T value )
		{
			v[i0] *= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,3>& operator /= ( T value )
		{
			v[i0] /= value;
			return *(VectorND<T,3>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,3>( v[i0], v[i0], v[i0] ).toString();
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
template < typename T, Index dimension, Index i0, Index i1, Index i2 >
OM_FORCE_INLINE VectorND<T,3> operator + ( T value, const Swizzle3D<T,dimension,i0,i1,i2>& vector )
{
	return value + vector.operator VectorND<T,3>();
}




/// Subtract a scalar to every component of the vector.
template < typename T, Index dimension, Index i0, Index i1, Index i2 >
OM_FORCE_INLINE VectorND<T,3> operator - ( T value, const Swizzle3D<T,dimension,i0,i1,i2>& vector )
{
	return value - vector.operator VectorND<T,3>();
}




/// Multiply every component of a vector with scalar, returning a vector result.
template < typename T, Index dimension, Index i0, Index i1, Index i2 >
OM_FORCE_INLINE VectorND<T,3> operator * ( T value, const Swizzle3D<T,dimension,i0,i1,i2>& vector )
{
	return value * vector.operator VectorND<T,3>();
}




/// Divide a value by every component of the vector, returning a vector result.
template < typename T, Index dimension, Index i0, Index i1, Index i2 >
OM_FORCE_INLINE VectorND<T,3> operator / ( T value, const Swizzle3D<T,dimension,i0,i1,i2>& vector )
{
	return value / vector.operator VectorND<T,3>();
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_VECTOR_3D_H
