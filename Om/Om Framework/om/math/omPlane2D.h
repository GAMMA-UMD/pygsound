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

#ifndef INCLUDE_OM_PLANE_2D_H
#define INCLUDE_OM_PLANE_2D_H


#include "omMathConfig.h"


#include "omVector2D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a plane in 2D space.
/**
  * It uses the normal and offset plane representation as it is the most universally
  * useful in computational mathematics, especially relating to graphics and geometry.
  */
template < typename T >
class Plane2D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a plane in 2D space with the normal pointing along the positive Y axis with offset = 0.
			OM_FORCE_INLINE Plane2D()
				:	normal( 0, 1 ),
					offset( 0 )
			{
			}
			
			
			/// Create a plane in 2D space with the specified normal and offset from the origin.
			OM_FORCE_INLINE Plane2D( const VectorND<T,2>& planeNormal, T planeOffset )
				:	normal( planeNormal ),
					offset( planeOffset )
			{
			}
			
			
			/// Create a plane in 2D space from two points in that plane.
			OM_FORCE_INLINE Plane2D( const VectorND<T,2>& p1, const VectorND<T,2>& p2 )
				:	normal( math::perp( p2 - p1 ).normalize() )
			{
				offset = -math::dot( p1, normal );
			}
			
			
		//********************************************************************************
		//******	Point Distance Methods
			
			
			/// Get the perpendicular distance from the specified point to the plane.
			OM_FORCE_INLINE T getDistanceTo( const VectorND<T,2>& point ) const
			{
				return math::abs( math::dot( normal, point ) + offset );
			}
			
			
			/// Get the perpendicular distance from the specified point to the plane.
			OM_FORCE_INLINE T getSignedDistanceTo( const VectorND<T,2>& point ) const
			{
				return math::dot( normal, point ) + offset;
			}
			
			
		//********************************************************************************
		//******	Point Projection Methods
			
			
			/// Return the projection of the given point onto the plane.
			OM_FORCE_INLINE VectorND<T,2> getProjection( const VectorND<T,2>& point ) const
			{
				T t = getSignedDistanceTo(point) / math::dot( normal, normal );
				return point - t*normal;
			}
			
			
			/// Return the projection of the given point onto the plane.
			/**
			  * The plane is assumed to have a normal vector of unit length. This
			  * results in a significantly faster function, however the results are
			  * meaningless if the precondition is not met.
			  */
			OM_FORCE_INLINE VectorND<T,2> getProjectionNormalized( const VectorND<T,2>& point ) const
			{
				return point - getSignedDistanceTo(point)*normal;
			}
			
			
		//********************************************************************************
		//******	Point Reflection Methods
			
			
			/// Get the reflection of a point over the plane.
			OM_FORCE_INLINE VectorND<T,2> getReflection( const VectorND<T,2>& point ) const
			{
				T t = getSignedDistanceTo(point) / math::dot( normal, normal );
				return point - T(2)*t*normal;
			}
			
			
			/// Get the reflection of a point over the plane.
			OM_FORCE_INLINE VectorND<T,2> getReflectionNormalized( const VectorND<T,2>& point ) const
			{
				return point - T(2)*getSignedDistanceTo(point)*normal;
			}
			
			
		//********************************************************************************
		//******	Plane Normalization Method
			
			
			/// Normalize the plane's normal vector and correct the offset to match.
			OM_FORCE_INLINE Plane2D normalize() const
			{
				T inverseMagnitude = T(1)/normal.getMagnitude();
				
				return Plane2D( normal*inverseMagnitude, offset*inverseMagnitude );
			}
			
			
		//********************************************************************************
		//******	Plane Inversion Operator
			
			
			/// Return the plane with the opposite normal vector and offset.
			/** 
			  * This plane is mathematically the same as the original plane.
			  */
			OM_FORCE_INLINE Plane2D operator - () const
			{
				return Plane2D( -normal, -offset );
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A vector perpendicular to the plane.
			VectorND<T,2> normal;
			
			
			/// The distance that the plane is offset from the origin.
			T offset;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		2D Plane Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef Plane2D<int> Plane2i;
typedef Plane2D<float> Plane2f;
typedef Plane2D<double> Plane2d;




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_PLANE_2D_H
