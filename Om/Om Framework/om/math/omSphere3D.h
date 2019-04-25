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

#ifndef INCLUDE_OM_MATH_SPHERE_3D_H
#define INCLUDE_OM_MATH_SPHERE_3D_H


#include "omMathConfig.h"


#include "omVector3D.h"
#include "omAABB3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a sphere in 3D space.
template < typename T >
class Sphere3D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a sphere object that has a radius of 0 and a position centered at the origin.
			OM_INLINE Sphere3D()
				:	position(),
					radius( 0 )
			{
			}
			
			
			/// Create a sphere object with the specified center position and radius.
			/**
			  * @param newPosition - the position of the center of the Sphere3D in 3D space.
			  * @param newRadius - the radius of the Sphere3D.
			  */
			OM_INLINE Sphere3D( const VectorND<T,3>& newPosition, T newRadius )
				:	position( newPosition ),
					radius( newRadius )
			{
			}
			
			
			/// Create a sphere object that tightly bounds the specified 3 points in space.
			/**
			  * This constructor can be used to easily create a bounding sphere for a triangle.
			  * 
			  * @param a - a point that should be enclosed by the Sphere3D.
			  * @param b - a point that should be enclosed by the Sphere3D.
			  * @param c - a point that should be enclosed by the Sphere3D.
			  */
			OM_INLINE Sphere3D( const VectorND<T,3>& a, const VectorND<T,3>& b, const VectorND<T,3>& c )
			{
				computeTriangleBoundingSphere( a, b, c, position, radius );
			}
			
			
			/// Create a sphere object that encloses all of the points in the specified array.
			/**
			  * This constructor uses an implementation-defined method of generating a bounding sphere
			  * for an arbitrary set of points in 3D space. The resulting sphere is not guaranteed to 
			  * be a minimal bounding sphere but should offer a reasonably-tight fit of the input
			  * point set. The constructor uses numPoints sequential points from the array of input points
			  * to construct the bounding sphere. If the number of input points is 0 or the point array
			  * is NULL, the resulting sphere will be a sphere of 0 radius centered at the origin.
			  * 
			  * @param points - a pointer to an array of input point data.
			  * @param numPoints - the number of sequential points to use from the point array.
			  */
			Sphere3D( const VectorND<T,3>* points, Size numPoints )
			{
				computeBoundingSphere( points, numPoints, position, radius );
			}
			
			
			/// Create a sphere object that encloses the two given bounding spheres.
			OM_INLINE Sphere3D( const Sphere3D& sphere1, const Sphere3D& sphere2 )
			{
				Sphere3D both = sphere1 | sphere2;
				position = both.position;
				radius = both.radius;
			}
			
			
			/// Create a copy of a sphere object with another templated type.
			/**
			  * @param other - the bounding sphere object that should be copied.
			  */
			template < typename U >
			OM_INLINE Sphere3D( const Sphere3D<U>& other )
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
			OM_INLINE Sphere3D& operator = ( const Sphere3D<U>& other )
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
			  * @param sphere - the sphere to test for intersection with this Sphere3D.
			  * @return whether or not this sphere intersects another.
			  */
			OM_FORCE_INLINE Bool intersects( const Sphere3D& sphere ) const
			{
				T distanceSquared = position.getDistanceToSquared( sphere.position );
				T radii = radius + sphere.radius;
				
				return distanceSquared < radii*radii;
			}
			
			
		//********************************************************************************
		//******	Bounding Sphere Enlargement Methods
			
			
			/// Enlarge this bounding sphere so that it encloses both its original volume and another sphere.
			OM_INLINE void enlargeFor( const Sphere3D& other )
			{
				// Compute the vector from this sphere to the other sphere.
				VectorND<T,3> v = other.position - position;
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
			OM_INLINE void enlargeFor( const VectorND<T,3>& point )
			{
				// Compute the vector from the child's bounding sphere to current bounding sphere.
				VectorND<T,3> v = point - position;
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
			  * The resulting Sphere3D object is guaranteed to tightly
			  * bound both the L-value Sphere3D object as well as the R-value
			  * Sphere3D.
			  * 
			  * @param sphere - the sphere to union with this Sphere3D.
			  * @return a Sphere3D which encloses both spheres.
			  */
			Sphere3D operator | ( const Sphere3D& sphere ) const
			{
				// Compute the squared distance between the sphere centers.
				VectorND<T,3> d = sphere.position - position;
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
					
					return Sphere3D( distance > T(0) ? 
										position + ((newRadius - radius) / distance)*d
										: position,
									newRadius );
				}
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The position of the center of this Sphere3D.
			VectorND<T,3> position;
			
			
			/// The radius of this Sphere3D.
			T radius;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Compute the approximate bounding sphere for the specified array of points.
			static void computeBoundingSphere( const VectorND<T,3>* points, Size numPoints,
												VectorND<T,3>& position, T& radius );
			
			
			/// Compute the exact bounding sphere for three points.
			static void computeTriangleBoundingSphere( const VectorND<T,3>& a, const VectorND<T,3>& b, const VectorND<T,3>& c,
														VectorND<T,3>& position, T& radius );
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Point Bounding Sphere Computation Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
void Sphere3D<T>:: computeBoundingSphere( const VectorND<T,3>* points, Size numPoints,
										VectorND<T,3>& position, T& radius )
{
	if ( points == NULL || numPoints == 0 )
	{
		position = VectorND<T,3>();
		radius = T(0);
		return;
	}
	
	//****************************************************************************
	// First, find the most extreme points along the 3 principle axes.
	
	AABB3D<Index> extremePoints;
	
	for ( Index i = 1; i < numPoints; i++ )
	{
		if ( points[i].x < points[extremePoints.min.x].x )
			extremePoints.min.x = i;
		
		if ( points[i].x > points[extremePoints.max.x].x )
			extremePoints.max.x = i;
		
		if ( points[i].y < points[extremePoints.min.y].y )
			extremePoints.min.y = i;
		
		if ( points[i].y > points[extremePoints.max.y].y )
			extremePoints.max.y = i;
		
		if ( points[i].z < points[extremePoints.min.z].z )
			extremePoints.min.z = i;
		
		if ( points[i].z > points[extremePoints.max.z].z )
			extremePoints.max.z = i;
	}
	
	
	//****************************************************************************
	// Find the pair of min-max points most distant from each other
	
	T xDistanceSquared = points[extremePoints.min.x].getDistanceToSquared( points[extremePoints.max.x] );
	T yDistanceSquared = points[extremePoints.min.y].getDistanceToSquared( points[extremePoints.max.y] );
	T zDistanceSquared = points[extremePoints.min.z].getDistanceToSquared( points[extremePoints.max.z] );
	
	Index minIndex = extremePoints.min.x;
	Index maxIndex = extremePoints.max.x;
	T extremePointsDistanceSquared = xDistanceSquared;
	
	if ( yDistanceSquared > xDistanceSquared && yDistanceSquared > zDistanceSquared )
	{
		minIndex = extremePoints.min.y;
		maxIndex = extremePoints.max.y;
		extremePointsDistanceSquared = yDistanceSquared;
	}
	else if ( zDistanceSquared > xDistanceSquared && zDistanceSquared > yDistanceSquared )
	{
		minIndex = extremePoints.min.z;
		maxIndex = extremePoints.max.z;
		extremePointsDistanceSquared = zDistanceSquared;
	}
	
	
	//****************************************************************************
	// Make an early approximation of the sphere which just encompases
	// these two extreme points.
	
	position = midpoint( points[minIndex], points[maxIndex] );
	radius = math::sqrt( extremePointsDistanceSquared ) / T(2);
	
	//****************************************************************************
	// Refine this bounding sphere so that it encompases all of the points
	
	for ( Index i = 0; i < numPoints; i++ )
	{
		// Compute squared distance between point and sphere's center
		VectorND<T,3> centerToPoint = points[i] - position;
		T pointDistanceSquared = centerToPoint.getMagnitudeSquared();
		
		// If the point is outside of the sphere, then we need to update the 
		// radius and position of the sphere, otherwise go on to next point.
		if ( pointDistanceSquared > radius*radius )
		{
			T pointDistance = math::sqrt( pointDistanceSquared );
			T newRadius = math::average( pointDistance, radius );
			
			T positionChange = (newRadius - radius) / pointDistance;
			
			// update the sphere to contain the new point.
			radius = newRadius;
			position += positionChange * centerToPoint;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		3-point Bounding Sphere Computation Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
void Sphere3D<T>:: computeTriangleBoundingSphere( const VectorND<T,3>& a, const VectorND<T,3>& b, const VectorND<T,3>& c,
													VectorND<T,3>& position, T& radius )
{
	T dotABAB = math::dot(b - a, b - a);
	T dotABAC = math::dot(b - a, c - a);
	T dotACAC = math::dot(c - a, c - a);
	T d = T(2)*(dotABAB*dotACAC - dotABAC*dotABAC);
	VectorND<T,3> referencePt = a;
	
	if ( math::abs(d) <= math::epsilon<T>() )
	{
		// a, b, and c lie on a line. Sphere center is center of AABB of the
		// points, and radius is distance from circle center to AABB corner
		AABB3D<T> boundingBox( a );
		boundingBox |= b;
		boundingBox |= c;
		
		position = boundingBox.getCenter();
		referencePt = boundingBox.min;
	}
	else
	{
		T inverseD = T(1) / d;
		T s = (dotABAB*dotACAC - dotACAC*dotABAC) * inverseD;
		T t = (dotACAC*dotABAB - dotABAB*dotABAC) * inverseD;
		
		// s controls height over AC, t over AB, (1-s-t) over BC
		if ( s <= T(0) )
			position = math::midpoint(a,c);
		else if ( t <= T(0) )
			position = math::midpoint(a,b);
		else if ( s + t >= T(1) )
		{
			position = math::midpoint(b,c);
			referencePt = b;
		}
		else
			position = a + s*(b - a) + t*(c - a);
	}
	
	radius = math::sqrt( math::dot(position - referencePt, position - referencePt) );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################



#endif // INCLUDE_OM_MATH_SPHERE_3D_H
