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

#ifndef INCLUDE_OM_VECTOR_4D_H
#define INCLUDE_OM_VECTOR_4D_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "omVectorND.h"
#include "omVector2D.h"
#include "omVector3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


template < typename T, Size dimension, Index i0, Index i1, Index i2, Index i3 >
class Swizzle4D;




//********************************************************************************
/// A templatized math class representing a 4-dimensional vector.
template < typename T >
class VectorND<T,4>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new 4D vector with all elements equal to zero.
			OM_FORCE_INLINE VectorND()
				:	x( T(0) ),
					y( T(0) ),
					z( T(0) ),
					w( T(0) )
			{
			}
			
			
			/// Create a new 4D vector with all elements equal to a single value.
			/** 
			  * This constructor creates a uniform 4D vector with all elements
			  * equal to each other and equal to the single constructor parameter
			  * value.
			  * 
			  * @param value - The value to set all elements of the vector to.
			  */
			OM_FORCE_INLINE explicit VectorND( T value )
				:	x( value ),
					y( value ),
					z( value ),
					w( value )
			{
			}
			
			
			/// Create a new 4D vector from a pointer to a 4 element array.
			/** 
			  * This constructor takes a pointer to an array of 4 values 
			  * and sets it's x, y, z, and w coordinates to be the 0th, 1th,
			  * 2th, and 3th indexed values in the array. No error checking is
			  * performed, so make sure to pass in a correct array of values
			  * or expect the worst.
			  * 
			  * @param array - An indexed array of 4 values for the vector's coordinates.
			  */
			OM_FORCE_INLINE explicit VectorND( const T array[4] )
				:	x( array[0] ),
					y( array[1] ),
					z( array[2] ),
					w( array[3] )
			{
			}
			
			
			/// Create a new 4D vector by specifying it's x, y, z, and w values.
			OM_FORCE_INLINE VectorND( T newX, T newY, T newZ, T newW )
				:	x( newX ),
					y( newY ),
					z( newZ ),
					w( newW )
			{
			}
			
			
			/// Create a new 4D vector from a 2D vector and 2 values for the Z and W coordinates.
			OM_FORCE_INLINE VectorND( const VectorND<T,2>& vector, T newZ, T newW )
				:	x( vector.x ),
					y( vector.y ),
					z( newZ ),
					w( newW )
			{
			}
			
			
			/// Create a new 4D vector from a scalar, a 2D vector and another scalar.
			OM_FORCE_INLINE VectorND( T newX, const VectorND<T,2>& vector, T newW )
				:	x( newX ),
					y( vector.x ),
					z( vector.y ),
					w( newW )
			{
			}
			
			
			/// Create a new 4D vector from two scalars for the X, Y coordinates and a 2D vector.
			OM_FORCE_INLINE VectorND( T newX, T newY, const VectorND<T,2>& vector )
				:	x( newX ),
					y( newY ),
					z( vector.x ),
					w( vector.y )
			{
			}
			
			
			/// Create a new 4D vector from a 3D vector and a value for the W coordinate.
			OM_FORCE_INLINE VectorND( const VectorND<T,3>& vector, T newW )
				:	x( vector.x ),
					y( vector.y ),
					z( vector.z ),
					w( newW )
			{
			}
			
			
			/// Create a new 4D vector from a value for the X coordinate and a 3D vector.
			OM_FORCE_INLINE VectorND( T newX, const VectorND<T,3>& vector )
				:	x( newX ),
					y( vector.x ),
					z( vector.y ),
					w( vector.w )
			{
			}
			
			
			/// Create a new 4D vector from an existing vector with a different template type.
			template < typename U >
			OM_FORCE_INLINE VectorND( const VectorND<U,4>& vector )
				:	x( (T)vector.x ),
					y( (T)vector.y ),
					z( (T)vector.z ),
					w( (T)vector.w )
			{
			}
			
			
			/// Create a new 4D vector from an existing vector with the same template type.
			/**
			  * This is declared because the auto-generated one was not working with the crazy uniform mess.
			  */
			OM_FORCE_INLINE VectorND( const VectorND<T,4>& vector )
				:	x( vector.x ),
					y( vector.y ),
					z( vector.z ),
					w( vector.w )
			{
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Create a new 4D vector from an existing vector with a different template type.
			/**
			  * This is declared because the auto-generated one was not working with the crazy uniform mess.
			  */
			OM_FORCE_INLINE VectorND& operator = ( const VectorND& vector )
			{
				x = vector.x;
				y = vector.y;
				z = vector.z;
				w = vector.w;
				return *this;
			}
			
			
		//********************************************************************************
		//******	Magnitude Methods
			
			
			/// Return the magnitude of this vector (the length).
			OM_FORCE_INLINE T getMagnitude() const
			{
				return math::sqrt( x*x + y*y + z*z + w*w );
			}
			
			
			/// Return the square of the magnitude of this vector.
			OM_FORCE_INLINE T getMagnitudeSquared() const
			{
				return x*x + y*y + z*z + w*w;
			}
			
			
			/// Return a normalized version of this vector.
			OM_FORCE_INLINE VectorND normalize() const
			{
				T inverseMagnitude = T(1)/math::sqrt( x*x + y*y + z*z + w*w );
				
				return VectorND( x*inverseMagnitude,
									y*inverseMagnitude,
									z*inverseMagnitude,
									w*inverseMagnitude );
			}
			
			
			/// Return a normalized version of this vector, placing the vector's magnitude in the output parameter.
			OM_FORCE_INLINE VectorND normalize( T& magnitude ) const
			{
				magnitude = math::sqrt( x*x + y*y + z*z + w*w );
				T inverseMagnitude = T(1)/magnitude;
				
				return VectorND( x*inverseMagnitude,
									y*inverseMagnitude,
									z*inverseMagnitude,
									w*inverseMagnitude );
			}
			
			
			/// Project this vector on another vector and return the projected vector.
			OM_FORCE_INLINE VectorND projectOn( const VectorND& vector ) const
			{
				VectorND norm = vector.normalize();
				
				return ( x*norm.x + y*norm.y + z*norm.z + w*norm.w )*norm;
			}
			
			
			/// Project this vector on a normalized vector and return the projected vector.
			OM_FORCE_INLINE VectorND projectOnNormalized( const VectorND& vector ) const
			{
				return ( x*vector.x + y*vector.y + z*vector.z + w*vector.w )*vector;
			}
			
			
		//********************************************************************************
		//******	Distance Methods
			
			
			/// Return the distance from this vector to another in 4D Euclidean space.
			OM_FORCE_INLINE T getDistanceTo( const VectorND& vector ) const
			{
				const T minusX = vector.x - x;
				const T minusY = vector.y - y;
				const T minusZ = vector.z - z;
				const T minusW = vector.w - w;
				
				return math::sqrt( minusX*minusX + minusY*minusY + minusZ*minusZ + minusW*minusW );
			}
			
			
			/// Return the square of the distance from this vector to another in 4D Euclidean space.
			OM_FORCE_INLINE T getDistanceToSquared( const VectorND& vector ) const
			{
				const T minusX = vector.x - x;
				const T minusY = vector.y - y;
				const T minusZ = vector.z - z;
				const T minusW = vector.w - w;
				
				return minusX*minusX + minusY*minusY + minusZ*minusZ + minusW*minusW;
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
			
			
			/// Return the minimum component of this vector.
			OM_FORCE_INLINE T getMin() const
			{
				return math::min( math::min( x, y ), math::min( z, w ) );
			}
			
			
			/// Return the maximum component of this vector.
			OM_FORCE_INLINE T getMax() const
			{
				return math::max( math::max( x, y ), math::max( z, w ) );
			}
			
			
			/// Return a reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE T& get( Index index )
			{
				OM_DEBUG_ASSERT( index < 4 );
				return v[index];
			}
			
			
			/// Return a const reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE const T& get( Index index ) const
			{
				OM_DEBUG_ASSERT( index < 4 );
				return v[index];
			}
			
			
			/// Return a reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE T& operator [] ( Index index )
			{
				OM_DEBUG_ASSERT( index < 4 );
				return v[index];
			}
			
			
			/// Return a const reference to the vector coordinate at the specified index.
			OM_FORCE_INLINE const T& operator [] ( Index index ) const
			{
				OM_DEBUG_ASSERT( index < 4 );
				return v[index];
			}
			
			
			/// Set the X, Y, Z, and W coordinates of the vector to the specified values.
			OM_FORCE_INLINE void setAll( T newX, T newY, T newZ, T newW )
			{
				x = newX;
				y = newY;
				z = newZ;
				w = newW;
			}
			
			
			/// Set the coordinate of this vector at the specified index to a new value.
			OM_FORCE_INLINE void set( Index index, T newValue )
			{
				OM_DEBUG_ASSERT( index < 4 );
				v[index] = newValue;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this vector is equal to another for all components.
			OM_FORCE_INLINE Bool operator == ( const VectorND& v ) const
			{
				return x == v.x && y == v.y && z == v.z && w == v.w;
			}
			
			
			/// Return whether or not this vector is not equal to another for any component.
			OM_FORCE_INLINE Bool operator != ( const VectorND& v ) const
			{
				return x != v.x || y != v.y || z != v.z || w != v.w;
			}
			
			
		//********************************************************************************
		//******	Negation/Positivation Operators
			
			
			/// Negate each component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator - () const
			{
				return VectorND( -x, -y, -z, -w );
			}
			
			
			/// Return copy of the original vector.
			OM_FORCE_INLINE VectorND operator + () const
			{
				return VectorND( x, y, z, w );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Add a scalar to every component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator + ( T value ) const
			{
				return VectorND( x + value, y + value, z + value, w + value );
			}
			
			
			/// Subtract a scalar from every component of this vector and return the result.
			OM_FORCE_INLINE VectorND operator - ( T value ) const
			{
				return VectorND( x - value, y - value, z - value, w - value );
			}
			
			
			/// Multiply every component of this vector by a scalar and return the result.
			OM_FORCE_INLINE VectorND operator * ( T value ) const
			{
				return VectorND( x*value, y*value, z*value, w*value );
			}
			
			
			/// Divide every component of this vector by a scalar and return the result.
			OM_FORCE_INLINE VectorND operator / ( T value ) const
			{
				T inverseValue = T(1) / value;
				
				return VectorND( x*inverseValue, y*inverseValue, z*inverseValue, w*inverseValue );
			}
			
			
			/// Add this vector to another and return the result and return the result.
			OM_FORCE_INLINE VectorND operator + ( const VectorND& vector ) const
			{
				return VectorND( x + vector.x, y + vector.y, z + vector.z, w + vector.w );
			}
			
			
			/// Subtract a vector from this vector and return the result.
			OM_FORCE_INLINE VectorND operator - ( const VectorND& vector ) const
			{
				return VectorND( x - vector.x, y - vector.y, z - vector.z, w - vector.w );
			}
			
			
			/// Multiply this vector by another vector and return the result.
			OM_FORCE_INLINE VectorND operator * ( const VectorND& vector ) const
			{
				return VectorND( x*vector.x, y*vector.y, z*vector.z, w*vector.w );
			}
			
			
			/// Divide this vector by another vector and return the result.
			OM_FORCE_INLINE VectorND operator / ( const VectorND& vector ) const
			{
				return VectorND( x/vector.x, y/vector.y, z/vector.z, w/vector.w );
			}
			
			
		//********************************************************************************
		//******	Arithmetic Assignment Operators
			
			
			/// Add a scalar to each component of this vector and modify the original vector.
			OM_FORCE_INLINE VectorND& operator += ( T value )
			{
				x += value;
				y += value;
				z += value;
				w += value;
				
				return *this;
			}
			
			
			/// Subtract a scalar from each component of this vector and modify the original vector.
			OM_FORCE_INLINE VectorND& operator -= ( T value )
			{
				x -= value;
				y -= value;
				z -= value;
				w -= value;
				
				return *this;
			}
			
			
			/// Multiply each component of this vector by a scalar and modify the original vector.
			OM_FORCE_INLINE VectorND& operator *= ( T value )
			{
				x *= value;
				y *= value;
				z *= value;
				w *= value;
				
				return *this;
			}
			
			
			/// Divide each component of this vector by a scalar and modify the original vector.
			OM_FORCE_INLINE VectorND& operator /= ( T value )
			{
				const T inverseValue = T(1) / value;
				
				x *= inverseValue;
				y *= inverseValue;
				z *= inverseValue;
				w *= inverseValue;
				
				return *this;
			}
			
			
			/// Add a vector to this vector, modifying this vector.
			OM_FORCE_INLINE VectorND& operator += ( const VectorND& vector )
			{
				x += vector.x;
				y += vector.y;
				z += vector.z;
				w += vector.w;
				
				return *this;
			}
			
			/// Subtract a vector from this vector, modifying this vector.
			OM_FORCE_INLINE VectorND& operator -= ( const VectorND& vector )
			{
				x -= vector.x;
				y -= vector.y;
				z -= vector.z;
				w -= vector.w;
				
				return *this;
			}
			
			
			/// Multiply this vector by another vector and modify this vector.
			OM_FORCE_INLINE VectorND& operator *= ( const VectorND& vector )
			{
				x *= vector.x;
				y *= vector.y;
				z *= vector.z;
				w *= vector.w;
				
				return *this;
			}
			
			
			/// Divide this vector by another vector and modify this vector.
			OM_FORCE_INLINE VectorND& operator /= ( const VectorND& vector )
			{
				x /= vector.x;
				y /= vector.y;
				z /= vector.z;
				w /= vector.w;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this 4D vector into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "< " << x << ", " << y << ", " << z << ", " << w << " >";
				
				return buffer.toString();
			}
			
			
			/// Convert this 4D vector into a human-readable string representation.
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
				
				
				/// A struct for when the vector should be interpreted as a 4-channel color.
				struct
				{
					/// The red component of a 4-component color.
					T r;
					
					/// The green component of a 4-component color.
					T g;
					
					/// The blue component of a 4-component color.
					T b;
					
					/// The alpha component of a 4-component color.
					T a;
				};
				
				
				/// A struct for when the vector should be interpreted as a 4D cartesian vector.
				struct
				{
					/// The X coordinate of a 4D vector.
					T x;
					
					/// The Y coordinate of a 4D vector.
					T y;
					
					/// The Z coordinate of a 4D vector.
					T z;
					
					/// The W coordinate of a 4D vector.
					T w;
				};
				
				
				Swizzle2D<T,4,0,0> xx;
				Swizzle2D<T,4,0,0> rr;
				Swizzle2D<T,4,1,1> yy;
				Swizzle2D<T,4,1,1> gg;
				Swizzle2D<T,4,2,2> zz;
				Swizzle2D<T,4,2,2> bb;
				Swizzle2D<T,4,3,3> ww;
				Swizzle2D<T,4,3,3> aa;
				
				Swizzle2D<T,4,0,1> xy;
				Swizzle2D<T,4,0,1> rg;
				Swizzle2D<T,4,0,2> xz;
				Swizzle2D<T,4,0,2> rb;
				Swizzle2D<T,4,0,3> xw;
				Swizzle2D<T,4,0,3> ra;
				
				Swizzle2D<T,4,1,0> yx;
				Swizzle2D<T,4,1,0> gr;
				Swizzle2D<T,4,1,2> yz;
				Swizzle2D<T,4,1,2> gb;
				Swizzle2D<T,4,1,3> yw;
				Swizzle2D<T,4,1,3> ga;
				
				Swizzle2D<T,4,2,0> zx;
				Swizzle2D<T,4,2,0> br;
				Swizzle2D<T,4,2,1> zy;
				Swizzle2D<T,4,2,1> bg;
				Swizzle2D<T,4,2,3> zw;
				Swizzle2D<T,4,2,3> ba;
				
				Swizzle2D<T,4,3,0> wx;
				Swizzle2D<T,4,3,0> ar;
				Swizzle2D<T,4,3,1> wy;
				Swizzle2D<T,4,3,1> ag;
				Swizzle2D<T,4,3,2> wz;
				Swizzle2D<T,4,3,2> ab;
				
				
				Swizzle3D<T,4,0,0,0> xxx;
				Swizzle3D<T,4,0,0,0> rrr;
				Swizzle3D<T,4,1,1,1> yyy;
				Swizzle3D<T,4,1,1,1> ggg;
				Swizzle3D<T,4,2,2,2> zzz;
				Swizzle3D<T,4,2,2,2> bbb;
				Swizzle3D<T,4,3,3,3> www;
				Swizzle3D<T,4,3,3,3> aaa;
				
				Swizzle3D<T,4,0,1,1> xyy;
				Swizzle3D<T,4,0,1,1> rgg;
				Swizzle3D<T,4,1,0,1> yxy;
				Swizzle3D<T,4,1,0,1> grg;
				Swizzle3D<T,4,1,1,0> yyx;
				Swizzle3D<T,4,1,1,0> ggr;
				
				Swizzle3D<T,4,0,2,2> xzz;
				Swizzle3D<T,4,0,2,2> rbb;
				Swizzle3D<T,4,2,0,2> zxz;
				Swizzle3D<T,4,2,0,2> brb;
				Swizzle3D<T,4,2,2,0> zzx;
				Swizzle3D<T,4,2,2,0> bbr;
				
				Swizzle3D<T,4,0,3,3> xww;
				Swizzle3D<T,4,0,3,3> raa;
				Swizzle3D<T,4,3,0,3> wxw;
				Swizzle3D<T,4,3,0,3> ara;
				Swizzle3D<T,4,3,3,0> wwx;
				Swizzle3D<T,4,3,3,0> aar;
				
				Swizzle3D<T,4,1,0,0> yxx;
				Swizzle3D<T,4,1,0,0> grr;
				Swizzle3D<T,4,0,1,0> xyx;
				Swizzle3D<T,4,0,1,0> rgr;
				Swizzle3D<T,4,0,0,1> xxy;
				Swizzle3D<T,4,0,0,1> rrg;
				
				Swizzle3D<T,4,1,2,2> yzz;
				Swizzle3D<T,4,1,2,2> gbb;
				Swizzle3D<T,4,2,1,2> zyz;
				Swizzle3D<T,4,2,1,2> bgb;
				Swizzle3D<T,4,2,2,1> zzy;
				Swizzle3D<T,4,2,2,1> bbg;
				
				Swizzle3D<T,4,1,3,3> yww;
				Swizzle3D<T,4,1,3,3> gaa;
				Swizzle3D<T,4,3,1,3> wyw;
				Swizzle3D<T,4,3,1,3> aga;
				Swizzle3D<T,4,3,3,1> wwy;
				Swizzle3D<T,4,3,3,1> aag;
				
				Swizzle3D<T,4,2,0,0> zxx;
				Swizzle3D<T,4,2,0,0> brr;
				Swizzle3D<T,4,0,2,0> xzx;
				Swizzle3D<T,4,0,2,0> rbr;
				Swizzle3D<T,4,0,0,2> xxz;
				Swizzle3D<T,4,0,0,2> rrb;
				
				Swizzle3D<T,4,2,1,1> zyy;
				Swizzle3D<T,4,2,1,1> bgg;
				Swizzle3D<T,4,1,2,1> yzy;
				Swizzle3D<T,4,1,2,1> gbg;
				Swizzle3D<T,4,1,1,2> yyz;
				Swizzle3D<T,4,1,1,2> ggb;
				
				Swizzle3D<T,4,2,3,3> zww;
				Swizzle3D<T,4,2,3,3> baa;
				Swizzle3D<T,4,3,2,3> wzw;
				Swizzle3D<T,4,3,2,3> aba;
				Swizzle3D<T,4,3,3,2> wwz;
				Swizzle3D<T,4,3,3,2> aab;
				
				Swizzle3D<T,4,3,0,0> wxx;
				Swizzle3D<T,4,3,0,0> arr;
				Swizzle3D<T,4,0,3,0> xwx;
				Swizzle3D<T,4,0,3,0> rar;
				Swizzle3D<T,4,0,0,3> xxw;
				Swizzle3D<T,4,0,0,3> rra;
				
				Swizzle3D<T,4,3,1,1> wyy;
				Swizzle3D<T,4,3,1,1> agg;
				Swizzle3D<T,4,1,3,1> ywy;
				Swizzle3D<T,4,1,3,1> gag;
				Swizzle3D<T,4,1,1,3> yyw;
				Swizzle3D<T,4,1,1,3> gga;
				
				Swizzle3D<T,4,3,2,2> wzz;
				Swizzle3D<T,4,3,2,2> abb;
				Swizzle3D<T,4,2,3,2> zwz;
				Swizzle3D<T,4,2,3,2> bab;
				Swizzle3D<T,4,2,2,3> zzw;
				Swizzle3D<T,4,2,2,3> bba;
				
				Swizzle3D<T,4,0,1,2> xyz;
				Swizzle3D<T,4,0,1,2> rgb;
				Swizzle3D<T,4,0,1,3> xyw;
				Swizzle3D<T,4,0,1,3> rga;
				Swizzle3D<T,4,0,2,1> xzy;
				Swizzle3D<T,4,0,2,1> rbg;
				Swizzle3D<T,4,0,2,3> xzw;
				Swizzle3D<T,4,0,2,3> rba;
				
				Swizzle3D<T,4,1,0,2> yxz;
				Swizzle3D<T,4,1,0,2> grb;
				Swizzle3D<T,4,1,0,3> yxw;
				Swizzle3D<T,4,1,0,3> gra;
				Swizzle3D<T,4,1,2,0> yzx;
				Swizzle3D<T,4,1,2,0> gbr;
				Swizzle3D<T,4,1,2,3> yzw;
				Swizzle3D<T,4,1,2,3> gba;
				
				Swizzle3D<T,4,2,1,0> zyx;
				Swizzle3D<T,4,2,1,0> bgr;
				Swizzle3D<T,4,2,1,3> zyw;
				Swizzle3D<T,4,2,1,3> bga;
				Swizzle3D<T,4,2,0,1> zxy;
				Swizzle3D<T,4,2,0,1> brg;
				Swizzle3D<T,4,2,0,3> zxw;
				Swizzle3D<T,4,2,0,3> bra;
				
				
				Swizzle4D<T,4,0,1,2,3> xyzw;
				Swizzle4D<T,4,0,1,3,2> xywz;
				Swizzle4D<T,4,0,2,1,3> xzyw;
				Swizzle4D<T,4,0,2,3,1> xzwy;
				Swizzle4D<T,4,0,3,1,2> xwyz;
				Swizzle4D<T,4,0,3,2,1> xwzy;
				
				Swizzle4D<T,4,1,0,2,3> yxzw;
				Swizzle4D<T,4,1,0,3,2> yxwz;
				Swizzle4D<T,4,1,2,0,3> yzxw;
				Swizzle4D<T,4,1,2,3,0> yzwx;
				Swizzle4D<T,4,1,3,0,2> ywxz;
				Swizzle4D<T,4,1,3,2,0> ywzx;
				
				Swizzle4D<T,4,2,0,1,3> zxyw;
				Swizzle4D<T,4,2,0,3,1> zxwy;
				Swizzle4D<T,4,2,1,0,3> zyxw;
				Swizzle4D<T,4,2,1,3,0> zywx;
				Swizzle4D<T,4,2,3,0,1> zwxy;
				Swizzle4D<T,4,2,3,1,0> zwyx;
				
				Swizzle4D<T,4,3,0,1,2> wxyz;
				Swizzle4D<T,4,3,0,2,1> wxzy;
				Swizzle4D<T,4,3,1,0,2> wyxz;
				Swizzle4D<T,4,3,1,2,0> wyzx;
				Swizzle4D<T,4,3,2,0,1> wzxy;
				Swizzle4D<T,4,3,2,1,0> wzyx;
			};
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// A constant vector with all elements equal to zero
			static const VectorND ZERO;
			
			
};




template <typename T>
const VectorND<T,4> VectorND<T,4>:: ZERO;




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Arithmetic Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a value to every component of the vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> operator + ( T value, const VectorND<T,4>& vector )
{
    return VectorND<T,4>( vector.x + value, vector.y + value, vector.z + value, vector.w + value );
}




/// Subtract every component of the vector from the value, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> operator - ( T value, const VectorND<T,4>& vector )
{
	return VectorND<T,4>( value - vector.x, value - vector.y, value - vector.z, value - vector.w );
}




/// Multiply every component of the vector with the value, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> operator * ( T value, const VectorND<T,4>& vector ) 
{
	return VectorND<T,4>( vector.x*value, vector.y*value, vector.z*value, vector.w*value );
}




/// Divide a value by every component of the vector, returning a vector result.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> operator / ( T value, const VectorND<T,4>& vector ) 
{
	return VectorND<T,4>( value/vector.x, value/vector.y, value/vector.z, value/vector.w );
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
OM_FORCE_INLINE T dot( const VectorND<T,4>& vector1, const VectorND<T,4>& vector2 )
{
	return vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z + vector1.w*vector2.w;
}




/// Compute the midpoint (e.g. average) of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> midpoint( const VectorND<T,4>& vector1, const VectorND<T,4>& vector2 )
{
	return VectorND<T,4>(	(vector1.x + vector2.x)*T(0.5), 
							(vector1.y + vector2.y)*T(0.5),
							(vector1.z + vector2.z)*T(0.5),
							(vector1.w + vector2.w)*T(0.5) );
}




/// Return the absolute value of the specified vector, such that the every component is positive.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> abs( const VectorND<T,4>& vector )
{
	return VectorND<T,4>( math::abs(vector.x), math::abs(vector.y), math::abs(vector.z), math::abs(vector.w) );
}




/// Compute the component-wise minimum of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> min( const VectorND<T,4>& vector1, const VectorND<T,4>& vector2 )
{
	return VectorND<T,4>(	math::min( vector1.x, vector2.x ),
						math::min( vector1.y, vector2.y ),
						math::min( vector1.z, vector2.z ),
						math::min( vector1.w, vector2.w ) );
}




/// Compute the component-wise maximum of two vectors.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> max( const VectorND<T,4>& vector1, const VectorND<T,4>& vector2 )
{
	return VectorND<T,4>(	math::max( vector1.x, vector2.x ),
						math::max( vector1.y, vector2.y ),
						math::max( vector1.z, vector2.z ),
						math::max( vector1.w, vector2.w ) );
}




/// Return the floor of the specified vector, rounding each component down to the nearest integer.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> floor( const VectorND<T,4>& vector )
{
	return VectorND<T,4>( math::floor(vector.x), math::floor(vector.y), math::floor(vector.z), math::floor(vector.w) );
}




/// Return the floor of the specified vector, rounding each component up to the nearest integer.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> ceiling( const VectorND<T,4>& vector )
{
	return VectorND<T,4>( math::ceiling(vector.x), math::ceiling(vector.y), math::ceiling(vector.z), math::ceiling(vector.w) );
}




/// Return the component-wise modulus of the specified vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> mod( const VectorND<T,4>& vector, const T& modulus )
{
	return VectorND<T,4>( math::mod(vector.x, modulus), math::mod(vector.y, modulus),
						math::mod(vector.z, modulus), math::mod(vector.w, modulus) );
}




/// Return the component-wise modulus of the specified vector.
template < typename T >
OM_FORCE_INLINE VectorND<T,4> mod( const VectorND<T,4>& vector, const VectorND<T,4>& modulus )
{
	return VectorND<T,4>( math::mod(vector.x, modulus.x), math::mod(vector.y, modulus.y),
						math::mod(vector.z, modulus.z), math::mod(vector.w, modulus.w) );
}




/// Return whether or not any component of this vector is Not-A-Number.
template < typename T >
OM_FORCE_INLINE Bool isNAN( const VectorND<T,4>& vector)
{
	return math::isNAN(vector.x) || math::isNAN(vector.y) || math::isNAN(vector.z) || math::isNAN(vector.w);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Swizzle Class Declaration
//############		
//##########################################################################################
//##########################################################################################




/// A class that is used to implement arbitrary vector permutations.
template < typename T, Index dimension, Index i0, Index i1, Index i2, Index i3 >
class Swizzle4D
{
	public:
		
		/// Convert this swizzle permutation to a vector.
		OM_FORCE_INLINE operator VectorND<T,4> () const
		{
			return VectorND<T,4>( v[i0], v[i1], v[i2], v[i3] );
		}
		
		/// Add a scalar to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator + ( T scalar ) const
		{
			return VectorND<T,4>( v[i0] + scalar, v[i1] + scalar, v[i2] + scalar, v[i3] + scalar );
		}
		
		/// Subtract a scalar from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator - ( T scalar ) const
		{
			return VectorND<T,4>( v[i0] - scalar, v[i1] - scalar, v[i2] - scalar, v[i3] - scalar );
		}
		
		/// Multiply this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator * ( T scalar ) const
		{
			return VectorND<T,4>( v[i0] * scalar, v[i1] * scalar, v[i2] * scalar, v[i3] * scalar );
		}
		
		/// Divide this swizzle permutation by a scalar and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator / ( T scalar ) const
		{
			return VectorND<T,4>( v[i0] / scalar, v[i1] / scalar, v[i2] / scalar, v[i3] / scalar );
		}
		
		/// Add a vector to this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator + ( const VectorND<T,4>& other ) const
		{
			return VectorND<T,4>( v[i0] + other.x, v[i1] + other.y, v[i2] + other.z, v[i3] + other.w );
		}
		
		/// Subtract a vector from this swizzle permutation and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator - ( const VectorND<T,4>& other ) const
		{
			return VectorND<T,4>( v[i0] - other.x, v[i1] - other.y, v[i2] - other.z, v[i3] - other.w );
		}
		
		/// Multiply this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator * ( const VectorND<T,4>& other ) const
		{
			return VectorND<T,4>( v[i0] * other.x, v[i1] * other.y, v[i2] * other.z, v[i3] * other.w );
		}
		
		/// Divide this swizzle permutation by another vector and return the result.
		OM_FORCE_INLINE VectorND<T,4> operator / ( const VectorND<T,4>& other ) const
		{
			return VectorND<T,4>( v[i0] / other.x, v[i1] / other.y, v[i2] / other.z, v[i3] / other.w );
		}
		
		/// Assign a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,4>& operator = ( const VectorND<T,4>& other )
		{
			v[i0] = other.x;
			v[i1] = other.y;
			v[i2] = other.z;
			v[i3] = other.w;
			return *(VectorND<T,4>*)this;
		}
		
		/// Add a vector to this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,4>& operator += ( const VectorND<T,4>& other )
		{
			v[i0] += other.x;
			v[i1] += other.y;
			v[i2] += other.z;
			v[i3] += other.w;
			return *(VectorND<T,4>*)this;
		}
		
		/// Subtract a vector from this swizzle permutation.
		OM_FORCE_INLINE VectorND<T,4>& operator -= ( const VectorND<T,4>& other )
		{
			v[i0] -= other.x;
			v[i1] -= other.y;
			v[i2] -= other.z;
			v[i3] -= other.w;
			return *(VectorND<T,4>*)this;
		}
		
		/// Multiply this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,4>& operator *= ( const VectorND<T,4>& other )
		{
			v[i0] *= other.x;
			v[i1] *= other.y;
			v[i2] *= other.z;
			v[i3] *= other.w;
			return *(VectorND<T,4>*)this;
		}
		
		/// Divide this swizzle permutation by another vector.
		OM_FORCE_INLINE VectorND<T,4>& operator /= ( const VectorND<T,4>& other )
		{
			v[i0] /= other.x;
			v[i1] /= other.y;
			v[i2] /= other.z;
			v[i3] /= other.w;
			return *(VectorND<T,4>*)this;
		}
		
		/// Convert this vector permutation to a string representation.
		OM_FORCE_INLINE operator data::String () const
		{
			return VectorND<T,4>( v[i0], v[i1], v[i2], v[i3] ).toString();
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
template < typename T, Index dimension, Index i0, Index i1, Index i2, Index i3 >
OM_FORCE_INLINE VectorND<T,4> operator + ( T value, const Swizzle4D<T,dimension,i0,i1,i2,i3>& vector )
{
	return value + vector.operator VectorND<T,4>();
}




/// Subtract a scalar to every component of the vector.
template < typename T, Index dimension, Index i0, Index i1, Index i2, Index i3 >
OM_FORCE_INLINE VectorND<T,4> operator - ( T value, const Swizzle4D<T,dimension,i0,i1,i2,i3>& vector )
{
	return value - vector.operator VectorND<T,4>();
}




/// Multiply every component of a vector with scalar, returning a vector result.
template < typename T, Index dimension, Index i0, Index i1, Index i2, Index i3 >
OM_FORCE_INLINE VectorND<T,4> operator * ( T value, const Swizzle4D<T,dimension,i0,i1,i2,i3>& vector )
{
	return value * vector.operator VectorND<T,4>();
}




/// Divide a value by every component of the vector, returning a vector result.
template < typename T, Index dimension, Index i0, Index i1, Index i2, Index i3 >
OM_FORCE_INLINE VectorND<T,4> operator / ( T value, const Swizzle4D<T,dimension,i0,i1,i2,i3>& vector )
{
	return value / vector.operator VectorND<T,4>();
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_VECTOR_4D_H
