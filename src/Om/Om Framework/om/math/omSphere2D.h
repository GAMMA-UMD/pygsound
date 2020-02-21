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

#ifndef INCLUDE_OM_MATH_SPHERE_2D_H
#define INCLUDE_OM_MATH_SPHERE_2D_H


#include "omMathConfig.h"


#include "omVector2D.h"
#include "omRay2D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a sphere in 2D space (a circle).
template < typename T >
class Sphere2D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a sphere object that has a radius of 0 and a position centered at the origin.
			OM_INLINE Sphere2D()
				:	position(),
					radius( 0 )
			{
			}
			
			
			/// Create a sphere object with the specified center position and radius.
			/**
			  * @param newPosition - the position of the center of the Sphere2D in 2D space.
			  * @param newRadius - the radius of the Sphere2D.
			  */
			OM_INLINE Sphere2D( const VectorND<T,2>& newPosition, T newRadius )
				:	position( newPosition ),
					radius( newRadius )
			{
			}
			
			
			/// Create a sphere object that encloses the two given bounding spheres.
			OM_INLINE Sphere2D( const Sphere2D& sphere1, const Sphere2D& sphere2 )
			{
				Sphere2D both = sphere1 | sphere2;
				position = both.position;
				radius = both.radius;
			}
			
			
			/// Create a copy of a sphere object with another templated type.
			/**
			  * @param other - the bounding sphere object that should be copied.
			  */
			template < typename U >
			OM_INLINE Sphere2D( const Sphere2D<U>& other )
				:	radius( other.radius ),
					position( other.position )
			{
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign a sphere object with another templated type to this bounding sphere.
			/**
			  * @param other - the bounding sphere object that should be copied.
			  */
			template < typename U >
			OM_INLINE Sphere2D& operator = ( const Sphere2D<U>& other )
			{
				position = other.position;
				radius = (T)other.radius;
				return *this;
			}
			
			
		//********************************************************************************
		//******	Intersection Test Method
			
			
			/// Return whether or not this sphere intersects another.
			/**
			  * If the spheres intersect, TRUE is returned. Otherwise, FALSE is 
			  * returned.
			  * 
			  * @param sphere - the sphere to test for intersection with this Sphere2D.
			  * @return whether or not this sphere intersects another.
			  */
			OM_FORCE_INLINE Bool intersects( const Sphere2D& sphere ) const
			{
				T distanceSquared = position.getDistanceToSquared( sphere.position );
				T radii = radius + sphere.radius;
				
				return distanceSquared < radii*radii;
			}
			
			
		//********************************************************************************
		//******	Bounding Sphere Enlargement Methods
			
			
			/// Enlarge this bounding sphere so that it encloses both its original volume and the new sphere.
			OM_INLINE void enlargeFor( const Sphere2D& other )
			{
				// Compute the vector from the child's bounding sphere to current bounding sphere.
				VectorND<T,2> v = other.position - position;
				T distanceSquared = v.getMagnitudeSquared();
				T radiusDiff = other.radius - radius;
				
				if ( distanceSquared < radiusDiff*radiusDiff )
				{
					// The other bounding sphere completely contains the previous bounding sphere.
					if ( other.radius > radius )
					{
						position = other.position;
						radius = other.radius;
					}
				}
				else
				{
					// the other sphere is outside the previous bounding sphere. Resize the bounding
					// sphere to fit the other sphere.
					T distance = math::sqrt( distanceSquared );
					T newRadius = (distance + other.radius + radius)*T(0.5);
					
					if ( distance > T(0) )
						position += v*((newRadius - radius) / distance);
					
					radius = newRadius;
				}
			}
			
			
			/// Enlarge this bounding sphere so that it encloses both its original volume and the given point.
			OM_INLINE void enlargeFor( const VectorND<T,2>& point )
			{
				// Compute the vector from the child's bounding sphere to current bounding sphere.
				VectorND<T,2> v = point - position;
				T distanceSquared = v.getMagnitudeSquared();
				
				if ( distanceSquared > radius*radius )
				{
					// the point is outside the previous bounding sphere. Resize the bounding
					// sphere to enclose the point.
					T distance = math::sqrt( distanceSquared );
					T newRadius = (distance + radius)*T(0.5);
					
					position += v*((newRadius - radius) / distance);
					radius = newRadius;
				}
			}
			
			
		//********************************************************************************
		//******	Bounding Sphere Union Operator
			
			
			/// Compute the union of this bounding sphere with another.
			/**
			  * The resulting Sphere2D object is guaranteed to tightly
			  * bound both the L-value Sphere2D object as well as the R-value
			  * Sphere2D.
			  * 
			  * @param sphere - the sphere to union with this Sphere2D.
			  * @return a Sphere2D which encloses both spheres.
			  */
			Sphere2D operator | ( const Sphere2D& sphere ) const
			{
				// Compute the squared distance between the sphere centers.
				VectorND<T,2> d = sphere.position - position;
				T distanceSquared = d.getMagnitudeSquared();
				T radiusDiff = sphere.radius - radius;
				
				if ( radiusDiff*radiusDiff > distanceSquared )
				{
					// The sphere with the larger radius encloses the other.
					// Return the larger of these two spheres.
					if ( sphere.radius > radius )
						return sphere;
					else
						return *this;
				}
				else
				{
					// The spheres are partially overlapping or disjoint.
					T distance = math::sqrt(distanceSquared);
					T newRadius = (distance + radius + sphere.radius) * T(0.5);
					
					return Sphere2D( distance > T(0) ? 
										position + ((newRadius - radius) / distance)*d
										: position,
									newRadius );
				}
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The position of the center of this 2D sphere.
			VectorND<T,2> position;
			
			
			/// The radius of this 2D sphere.
			T radius;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Compute the approximate bounding sphere for the specified array of points.
			static void computeBoundingSphere( const VectorND<T,2>* points, Size numPoints,
												VectorND<T,2>& position, T& radius );
			
			
			/// Compute the exact bounding sphere for three points.
			static void computeTriangleBoundingSphere( const VectorND<T,2>& a, const VectorND<T,2>& b, const VectorND<T,2>& c,
														VectorND<T,2>& position, T& radius );
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################



#endif // INCLUDE_OM_MATH_SPHERE_2D_H
