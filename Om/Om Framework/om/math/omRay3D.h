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

#ifndef INCLUDE_OM_RAY_3D_H
#define INCLUDE_OM_RAY_3D_H


#include "omMathConfig.h"


#include "omVector3D.h"
#include "omMatrix3D.h"
#include "omSphere3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a ray in 3D space.
/**
  * This class contains two data members: origin and direction. Origin represents
  * the starting position of the ray and direction represents the positively
  * parameterized direction along the ray.
  */
template < typename T >
class Ray3D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a ray starting at the origin pointing in the positive Z direction.
			OM_FORCE_INLINE Ray3D()
				:	origin(),
					direction( 0, 0, 1 )
			{
			}
			
			
			/// Create a ray with the specified origin and direction.
			OM_FORCE_INLINE Ray3D( const VectorND<T,3>& newOrigin, const VectorND<T,3>& newDirection )
				:	origin( newOrigin ),
					direction( newDirection )
			{
			}
			
			
		//********************************************************************************
		//******	Ray Methods
			
			
			/// Get the position along the ray at the specified parameter value.
			/**
			  * This position is calculated using the equation:
			  * position = origin + parameter*direction.
			  */
			OM_FORCE_INLINE VectorND<T,3> getPositionAt( T parameter ) const
			{
				return origin + parameter*direction;
			}
			
			
			/// Return a new ray with a unit-length direction vector.
			OM_FORCE_INLINE Ray3D<T> normalize() const
			{
				return Ray3D<T>( origin, direction.normalize() );
			}
			
			
		//********************************************************************************
		//******	Sphere Intersection Methods
			
			
			/// Return whether or not this ray intersects the specified sphere.
			OM_INLINE Bool intersectsSphere( const VectorND<T,3>& position, T radius ) const;
			
			
			/// Return whether or not this ray intersects the specified sphere.
			OM_INLINE Bool intersectsSphere( const Sphere3D<T>& sphere ) const
			{
				return this->intersectsSphere( sphere.position, sphere.radius );
			}
			
			
			/// Return whether or not this ray intersects the specified sphere.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter.
			  */
			OM_INLINE Bool intersectsSphere( const VectorND<T,3>& position, T radius, T& distance ) const;
			
			
			/// Return whether or not this ray intersects the specified sphere.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter.
			  */
			OM_INLINE Bool intersectsSphere( const Sphere3D<T>& sphere, T& distance ) const
			{
				return this->intersectsSphere( sphere.position, sphere.radius, distance );
			}
			
			
		//********************************************************************************
		//******	Triangle Intersection Methods
			
			
			/// Return whether or not this ray intersects the specified triangle.
			OM_INLINE Bool intersectsTriangle( const VectorND<T,3>& v1, const VectorND<T,3>& v2,
												const VectorND<T,3>& v3 ) const;
			
			
			/// Return whether or not this ray intersects the specified triangle.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter.
			  */
			OM_INLINE Bool intersectsTriangle( const VectorND<T,3>& v1, const VectorND<T,3>& v2,
												const VectorND<T,3>& v3, T& distance ) const;
			
			
			/// Return whether or not this ray intersects the specified triangle.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter, along with the first two barycentric coordinates.
			  */
			OM_INLINE Bool intersectsTriangle( const VectorND<T,3>& v1, const VectorND<T,3>& v2,
												const VectorND<T,3>& v3, T& distance, T& u, T& v ) const;
			
			
		//********************************************************************************
		//******	Cylinder Intersection Methods
			
			
			/// Return whether or not this ray intersects a cylinder with the specified attributes.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter.
			  * 
			  * The cylinder is specified by an endpoint, unit axis vector, height, and radius.
			  */
			OM_INLINE Bool intersectsCylinder( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
												T height, T radius, T& distance ) const;
			
			
			/// Return whether or not this ray intersects a cylinder with the specified attributes.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter.
			  *
			  * The cylinder is specified by an endpoint, unit axis vector, height, and radii.
			  * This method allows the cylinder endpoints to have different radii.
			  */
			OM_INLINE Bool intersectsCylinder2( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
												T height, T radius1, T radius2, T& distance ) const;
			
			
		//********************************************************************************
		//******	Capsule Intersection Methods
			
			
			/// Return whether or not this ray intersects a capsule with the specified attributes.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter.
			  * 
			  * The capsule is specified by an endpoint, unit axis vector, height, and radius.
			  */
			OM_INLINE Bool intersectsCapsule( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
												T height, T radius, T& distance ) const;
			
			
			/// Return whether or not this ray intersects a capsule with the specified attributes.
			/**
			  * The distance along the ray where the intersection occured is placed in the
			  * output parameter.
			  *
			  * The capsule is specified by an endpoint, unit axis vector, height, and radii.
			  * This method allows the capsule endpoints to have different radii.
			  */
			OM_INLINE Bool intersectsCapsule2( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
												T height, T radius1, T radius2, T& distance ) const;
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The origin of the ray in 3D space;
			VectorND<T,3> origin;
			
			
			/// The direction of the ray in 3D space.
			VectorND<T,3> direction;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Sphere Intersection Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
Bool Ray3D<T>:: intersectsSphere( const VectorND<T,3>& position, T radius ) const
{
	const VectorND<T,3> d = position - origin;
	const T dSquared = d.getMagnitudeSquared();
	const T rSquared = radius*radius;
	
	// If the ray starts inside the sphere there is an intersection.
	if ( dSquared < rSquared )
		return true;
	else
	{
		// Find the closest point on the ray to the sphere's center.
		const T t1 = math::dot( d, direction );
		
		// Check to see if the ray is outside and points away from the sphere so there is no intersection.
		if ( t1 < T(0) )
			return false;
		
		// Find the distance from the closest point to the sphere's surface.
		const T t2Squared = rSquared - dSquared + t1*t1;
		
		// If the descriminant is negative, there is no intersection.
		return t2Squared >= T(0);
	}
}




template < typename T >
Bool Ray3D<T>:: intersectsSphere( const VectorND<T,3>& position, T radius, T& distance ) const
{
	const VectorND<T,3> d = position - origin;
	const T dSquared = d.getMagnitudeSquared();
	const T rSquared = radius*radius;
	
	// Find the closest point on the ray to the sphere's center.
	const T t1 = math::dot( d, direction );
	
	// If the ray starts inside the sphere there is an intersection.
	if ( dSquared < rSquared )
	{
		// Find the distance from the closest point to the sphere's surface.
		const T t2Squared = rSquared - dSquared + t1*t1;
		
		// Compute the distance along the ray of the intersection.
		distance = t1 + math::sqrt(t2Squared);
		
		return true;
	}
	else
	{
		// Check to see if the ray is outside and points away from the sphere so there is no intersection.
		if ( t1 < T(0) )
			return false;
		
		// Find the distance from the closest point to the sphere's surface.
		const T t2Squared = rSquared - dSquared + t1*t1;
		
		// If the descriminant is negative, there is no intersection.
		if ( t2Squared < T(0) )
			return false;
		
		// Compute the distance along the ray of the intersection.
		distance = t1 - math::sqrt(t2Squared);
		
		return true;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle Intersection Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
Bool Ray3D<T>:: intersectsTriangle( const VectorND<T,3>& v1, const VectorND<T,3>& v2, const VectorND<T,3>& v3 ) const
{
	// find the edge vectors of the triangle
	const VectorND<T,3> v1ToV2 = v2 - v1;
	const VectorND<T,3> v1ToV3 = v3 - v1;
	
	// the vector perpendicular to edge 2 and the ray's direction
	const VectorND<T,3> pvec = math::cross( direction, v1ToV3 );
	
	const T det = math::dot( v1ToV2, pvec );
	
	if ( math::abs(det) < math::epsilon<T>() )
		return false;
	
	const T inverseDet = T(1) / det;
	
	const VectorND<T,3> v1ToSource = origin - v1;
	
	const T u = math::dot( v1ToSource, pvec ) * inverseDet;
	
	if ( u < T(0) || u > T(1) )
		return false;
	
	const VectorND<T,3> qvec = math::cross( v1ToSource, v1ToV2 );
	
	const T v = math::dot( direction, qvec ) * inverseDet;
	
	if ( v < T(0) || u + v > T(1) )
		return false;
	
	const T t = math::dot( v1ToV3, qvec ) * inverseDet;
	
	return t > T(0);
}




template < typename T >
Bool Ray3D<T>:: intersectsTriangle( const VectorND<T,3>& v1, const VectorND<T,3>& v2, const VectorND<T,3>& v3, T& distance ) const
{
	// find the edge vectors of the triangle
	const VectorND<T,3> v1ToV2 = v2 - v1;
	const VectorND<T,3> v1ToV3 = v3 - v1;
	
	// the vector perpendicular to edge 2 and the ray's direction
	const VectorND<T,3> pvec = math::cross( direction, v1ToV3 );
	
	const T det = math::dot( v1ToV2, pvec );
	
	if ( math::abs(det) < math::epsilon<T>() )
		return false;
	
	const T inverseDet = T(1) / det;
	
	VectorND<T,3> v1ToSource = origin - v1;
	
	const T u = math::dot( v1ToSource, pvec ) * inverseDet;
	
	if ( u < T(0) || u > T(1) )
		return false;
	
	const VectorND<T,3> qvec = math::cross( v1ToSource, v1ToV2 );
	
	const T v = math::dot( direction, qvec ) * inverseDet;
	
	if ( v < T(0) || u + v > T(1) )
		return false;
	
	const T t = math::dot( v1ToV3, qvec ) * inverseDet;
	
	if ( t > T(0) )
	{
		distance = t;
		return true;
	}
	
	return false;
}




template < typename T >
Bool Ray3D<T>:: intersectsTriangle( const VectorND<T,3>& v1, const VectorND<T,3>& v2, const VectorND<T,3>& v3, T& distance, T& u, T& v ) const
{
	// find the edge vectors of the triangle
	const VectorND<T,3> v1ToV2 = v2 - v1;
	const VectorND<T,3> v1ToV3 = v3 - v1;
	
	// the vector perpendicular to edge 2 and the ray's direction
	const VectorND<T,3> pvec = math::cross( direction, v1ToV3 );
	
	const T det = math::dot( v1ToV2, pvec );
	
	if ( math::abs(det) < math::epsilon<T>() )
		return false;
	
	const T inverseDet = T(1) / det;
	
	VectorND<T,3> v1ToSource = origin - v1;
	
	u = math::dot( v1ToSource, pvec ) * inverseDet;
	
	if ( u < T(0) || u > T(1) )
		return false;
	
	const VectorND<T,3> qvec = math::cross( v1ToSource, v1ToV2 );
	
	v = math::dot( direction, qvec ) * inverseDet;
	
	if ( v < T(0) || u + v > T(1) )
		return false;
	
	const T t = math::dot( v1ToV3, qvec ) * inverseDet;
	
	if ( t > T(0) )
	{
		distance = t;
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cylinder Intersection Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
Bool Ray3D<T>:: intersectsCylinder( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
									T height, T radius, T& distance ) const
{
	const VectorND<T,3> m = origin - p1;
	const T dd = height*height;
	const T mn = math::dot( m, direction );
	const T md = math::dot( m, axis )*height;
	const T nd = math::dot( direction, axis )*height;
	const T k = math::dot( m, m ) - radius*radius;
	const T a = dd - nd*nd;
	const T c = dd*k - md*md;
	
	if ( math::abs(a) < math::epsilon<T>() )
	{
		// The ray is parallel to the cylinder's axis.
		if ( c > T(0) )
			return false;
		
		// Ray intersects, compute the intersection.
		if ( md < T(0) ) // Intersects first endcap.
			distance = -mn;
		else if ( md > dd ) // Intersects second endcap.
			distance = nd - mn;
		else // Inside cylinder.
			distance = T(0);
		
		return true;
	}
	else
	{
		const T b = dd*mn - nd*md;
		const T discr = b*b - a*c;
		
		// No real roots to quadratic, no intersection.
		if ( discr < T(0) )
			return false;
		
		const T u = (-b - math::sqrt(discr)) / a;
		const T f = md + u*nd;
		
		if ( f < T(0) )
		{
			// Intersection near the first endcap.
			if ( nd <= T(0) )
				return false; // Ray points away from endcap.
			
			// Compute candidate intersection distance.
			const T s = -md / nd;
			
			// Make sure the point is within the circle of the endcap.
			if ( k + s*(T(2)*mn + s) > T(0) )
				return false;
			
			distance = s;
		}
		else if ( f > dd )
		{
			// Intersection near the second endcap.
			if ( nd >= T(0) )
				return false; // Ray points away from endcap.
			
			// Compute candidate intersection distance.
			const T s = (dd - md) / nd;
			
			// Make sure the point is within the circle of the endcap.
			if ( k + dd - T(2)*md + s*(T(2)*(mn - nd) + s) > T(0) )
				return false;
			
			distance = s;
		}
		else // Segment intersects cylinder between endcaps.
			distance = u;
		
		return true;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cylinder Intersection Method for Different Radii
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
Bool Ray3D<T>:: intersectsCylinder2( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
									T height, T radius1, T radius2, T& distance ) const
{
	if ( radius1 == radius2 )
		return this->intersectsCylinder( p1, axis, height, radius1, distance );
	
	const MatrixND<T,3,3> M = MatrixND<T,3,3>::planeBasis( axis );
	const VectorND<T,3> rayD = direction*M;
	const VectorND<T,3> rayO = (origin - p1)*M;
	const T L = height*radius1 / (radius1 - radius2);
	const T k = math::square( radius1 / L );
	const T A = rayD.x*rayD.x + rayD.y*rayD.y - k*rayD.z*rayD.z;
	const T B = T(2)*(rayD.x*rayO.x + rayD.y*rayO.y - k*rayD.z*(rayO.z - L));
	const T C = rayO.x*rayO.x + rayO.y*rayO.y - k*math::square(rayO.z - L);
	
	const T discriminant = B*B - T(4)*A*C;
	
	// No real roots to quadratic, no intersection.
	if ( discriminant < T(0) )
		return false;
	
	const T u = (-B - math::sqrt(discriminant)) / (2*A);
	const T hitZ = rayO.z + u*rayD.z;
	
	if ( hitZ < T(0) )
	{
		// Intersection near the first endcap.
		if ( rayD.z <= T(0) )
			return false; // Ray points away from endcap.
		
		// Compute candidate intersection distance.
		const T s = -rayO.z / rayD.z;
		const T hitX = rayO.x + s*rayD.x;
		const T hitY = rayO.y + s*rayD.y;
		
		if ( hitX*hitX + hitY*hitY - radius1*radius1 > T(0) )
			return false;
		
		distance = s;
	}
	else if ( hitZ > height )
	{
		// Intersection near the second endcap.
		if ( rayD.z >= T(0) )
			return false; // Ray points away from endcap.
		
		// Compute candidate intersection distance.
		const T s = (height - rayO.z) / rayD.z;
		const T hitX = rayO.x + s*rayD.x;
		const T hitY = rayO.y + s*rayD.y;
		
		if ( hitX*hitX + hitY*hitY - radius2*radius2 > T(0) )
			return false;
		
		distance = s;
	}
	else // Segment intersects cylinder between endcaps.
		distance = u;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Capsule Intersection Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
Bool Ray3D<T>:: intersectsCapsule( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
									T height, T radius, T& distance ) const
{
	const VectorND<T,3> m = origin - p1;
	const T dd = height*height;
	const T mn = math::dot( m, direction );
	const T md = math::dot( m, axis )*height;
	const T nd = math::dot( direction, axis )*height;
	const T k = math::dot( m, m ) - radius*radius;
	const T a = dd - nd*nd;
	const T c = dd*k - md*md;
	
	if ( math::abs(a) < math::epsilon<T>() )
	{
		// The ray is parallel to the cylinder's axis.
		if ( c > T(0) )
			return false;
		
		// Ray intersects, compute the intersection.
		if ( md < T(0) ) // Intersects first endcap.
			distance = -mn;
		else if ( md > dd ) // Intersects second endcap.
			distance = nd - mn;
		else // Inside cylinder.
			distance = T(0);
		
		return true;
	}
	else
	{
		const T b = dd*mn - nd*md;
		const T discr = b*b - a*c;
		
		// No real roots to quadratic, no intersection.
		if ( discr < T(0) )
			return false;
		
		const T shaftT = (-b - math::sqrt(discr)) / a;
		const T shaftZ = md + shaftT*nd;
		T minT = math::max<T>();
		
		// Pick the closest valid intersection with the shaft or endcaps.
		if ( shaftZ >= T(0) && shaftZ <= dd )
			minT = shaftT;
		
		T t;
		
		if ( this->intersectsSphere( p1, radius, t ) && t < minT )
			minT = t;
		
		if ( this->intersectsSphere( p1 + height*axis, radius, t ) && t < minT )
			minT = t;
		
		if ( minT < math::max<T>() )
		{
			distance = minT;
			return true;
		}
		
		return false;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Capsule Intersection Method for Different Radii
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
Bool Ray3D<T>:: intersectsCapsule2( const VectorND<T,3>& p1, const VectorND<T,3>& axis,
									T height, T radius1, T radius2, T& distance ) const
{
	if ( radius1 == radius2 )
		return this->intersectsCapsule( p1, axis, height, radius1, distance );
	
	T adjustedHeight = height;
	
	// Check to see if this is a degenerate capsule (i.e. a sphere)
	if ( adjustedHeight < math::abs(radius1 - radius2) )
		adjustedHeight = math::abs(radius1 - radius2);
	
	T minShaftHeight;
	T maxShaftHeight;
	T sinTheta;
	
	if ( radius1 > radius2 )
	{
		T tangentHeight = adjustedHeight + radius2*adjustedHeight/(radius1 - radius2);
		sinTheta = radius1 / tangentHeight;
		minShaftHeight = radius1*sinTheta;
		maxShaftHeight = adjustedHeight + radius2*sinTheta;
	}
	else
	{
		T tangentHeight = adjustedHeight + radius1*adjustedHeight/(radius2 - radius1);
		sinTheta = radius2 / tangentHeight;
		minShaftHeight = radius1*sinTheta;
		maxShaftHeight = adjustedHeight + radius2*sinTheta;
	}
	
	const T cosTheta = math::sqrt( T(1) - sinTheta*sinTheta );
	const T shaftHeight = maxShaftHeight - minShaftHeight;
	const T shaftR1 = radius1 * cosTheta;
	const T shaftR2 = radius2 * cosTheta;
	const VectorND<T,3> shaftP1 = p1 + minShaftHeight*axis;
	
	//************************************************************************
	
	const MatrixND<T,3,3> M = MatrixND<T,3,3>::planeBasis( axis );
	const VectorND<T,3> rayD = direction*M;
	const VectorND<T,3> rayO = (origin - shaftP1)*M;
	const T L = shaftHeight*shaftR1 / (shaftR1 - shaftR2);
	const T k = math::square( shaftR1 / L );
	const T A = rayD.x*rayD.x + rayD.y*rayD.y - k*rayD.z*rayD.z;
	const T B = T(2)*(rayD.x*rayO.x + rayD.y*rayO.y - k*rayD.z*(rayO.z - L));
	const T C = rayO.x*rayO.x + rayO.y*rayO.y - k*math::square(rayO.z - L);
	
	const T discriminant = B*B - T(4)*A*C;
	
	// No real roots to quadratic, no intersection.
	if ( discriminant < T(0) )
		return false;
	
	const T shaftT = (-B - math::sqrt(discriminant)) / (2*A);
	T shaftZ = rayO.z + shaftT*rayD.z;
	T minT = math::max<T>();
	
	// Pick the closest valid intersection with the shaft or endcaps.
	if ( shaftZ >= T(0) && shaftZ <= shaftHeight )
		minT = shaftT;
	
	T t;
	
	if ( this->intersectsSphere( p1, radius1, t ) && t < minT )
		minT = t;
	
	if ( this->intersectsSphere( p1 + height*axis, radius2, t ) && t < minT )
		minT = t;
	
	if ( minT < math::max<T>() )
	{
		distance = minT;
		return true;
	}
	
	return false;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RAY_3D_H
