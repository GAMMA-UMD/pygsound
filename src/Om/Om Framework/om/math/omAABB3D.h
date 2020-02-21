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

#ifndef INCLUDE_OM_AABB_3D_H
#define INCLUDE_OM_AABB_3D_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"
#include "omAABB1D.h"
#include "omAABB2D.h"
#include "omVector3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a range of values in 3D space.
/**
  * This class contains two data members: min and max. These indicate the minimum
  * and maximum coordinates that this axis-aligned bounding box represents. The class
  * invariant is that min is less than max (on at least one dimension), though this
  * is not enforced. The class supports union, containment, and intersection operations.
  */
template < typename T >
class AABB3D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a 3D axis-aligned bounding box with no extent centered about the origin.
			OM_FORCE_INLINE AABB3D()
				:	min(),
					max()
			{
			}
			
			
			/// Create a 3D axis-aligned bounding box with the specified minimum and maximum coodinates for all axes.
			OM_FORCE_INLINE AABB3D( T newMin, T newMax )
				:	min( newMin ),
					max( newMax )
			{
			}
			
			
			/// Create a 3D axis-aligned bounding box with the specified minimum and maximum coodinates.
			OM_FORCE_INLINE AABB3D( T newXMin, T newXMax, 
									T newYMin, T newYMax, 
									T newZMin, T newZMax )
				:	min( newXMin, newYMin, newZMin ),
					max( newXMax, newYMax, newZMax )
			{
			}
			
			
			/// Create a 3D axis-aligned bounding box with the minimum and maximum coordinates equal to the specified vector.
			OM_FORCE_INLINE AABB3D( const VectorND<T,3>& center )
				:	min( center ),
					max( center )
			{
			}
			
			
			/// Create a 3D axis-aligned bounding box with the specified minimum and maximum coodinates.
			OM_FORCE_INLINE AABB3D( const VectorND<T,3>& newMin, const VectorND<T,3>& newMax )
				:	min( newMin ),
					max( newMax )
			{
			}
			
			
			/// Create a 3D axis-aligned bounding box for the specified array of points.
			OM_FORCE_INLINE AABB3D( const VectorND<T,3>* points, Size numPoints )
				:	min( math::max<Float>() ),
					max( math::min<Float>() )
			{
				const VectorND<T,3>* const pointsEnd = points + numPoints;
				
				while ( points != pointsEnd )
				{
					min = math::min( min, *points );
					max = math::max( max, *points );
					points++;
				}
			}
			
			
			/// Create a 3D axis-aligned bounding box from the specified 2D bounding box, setting the Z bounds to [0,0].
			OM_FORCE_INLINE explicit AABB3D( const AABB2D<T>& bounds2D )
				:	min( bounds2D.min, T() ),
					max( bounds2D.max, T() )
			{
			}
			
			
			/// Create a 3D axis-aligned bounding box from the specified 2D bounding box and Z coordinate bounding box.
			OM_FORCE_INLINE explicit AABB3D( const AABB2D<T>& bounds2D, const AABB1D<T>& boundsZ )
				:	min( bounds2D.min, boundsZ.min ),
					max( bounds2D.max, boundsZ.max )
			{
			}
			
			
		//********************************************************************************
		//******	AABB Cast Operator
			
			
			/// Cast this bounding box to a bounding box with a different underlying primitive type.
			template < typename U >
			OM_FORCE_INLINE operator AABB3D<U> () const
			{
				return AABB3D<U>( (U)min.x, (U)max.x, (U)min.y, (U)max.y, (U)min.z, (U)max.z );
			}
			
			
		//********************************************************************************
		//******	AABB Comparison Methods
			
			
			/// Return whether or not this bounding box completely contains another.
			OM_FORCE_INLINE Bool contains( const AABB3D& bounds ) const
			{
				return	min.x <= bounds.min.x && max.x >= bounds.max.x && 
						min.y <= bounds.min.y && max.y >= bounds.max.y && 
						min.z <= bounds.min.z && max.z >= bounds.max.z;
			}
			
			
			/// Return whether or not this bounding box contains the specified coordinate.
			OM_FORCE_INLINE Bool contains( const VectorND<T,3>& coordinate ) const
			{
				return coordinate.x >= min.x && coordinate.x <= max.x &&
						coordinate.y >= min.y && coordinate.y <= max.y &&
						coordinate.z >= min.z && coordinate.z <= max.z;
			}
			
			
			/// Return whether or not this bounding box intersects another.
			OM_FORCE_INLINE Bool intersects( const AABB3D& bounds ) const
			{
				return	(min.x < bounds.max.x) && (max.x > bounds.min.x) && 
						(min.y < bounds.max.y) && (max.y > bounds.min.y) && 
						(min.z < bounds.max.z) && (max.z > bounds.min.z);
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Set the minimum and maximum coordinates of the axis-aligned bounding box.
			OM_FORCE_INLINE void set( T newXMin, T newXMax, T newYMin, T newYMax, T newZMin, T newZMax )
			{
				min.set( newXMin, newYMin, newZMin );
				max.set( newXMax, newYMax, newZMax );
			}
			
			
			/// Get the difference between the maximum and minimum X coordinates.
			OM_FORCE_INLINE T getWidth() const
			{
				return max.x - min.x;
			}
			
			
			/// Get the difference between the maximum and minimum Y coordinates.
			OM_FORCE_INLINE T getHeight() const
			{
				return max.y - min.y;
			}
			
			
			/// Get the difference between the maximum and minimum Z coordinates.
			OM_FORCE_INLINE T getDepth() const
			{
				return max.z - min.z;
			}
			
			
			/// Return a vector indicating the axial distances between the minimum and maximum coordinate.
			OM_FORCE_INLINE VectorND<T,3> getSize() const
			{
				return max - min;
			}
			
			
			/// Return the vector from the minimum coordinate to the maximum.
			OM_FORCE_INLINE VectorND<T,3> getDiagonal() const
			{
				return max - min;
			}
			
			
			/// Return the distance from the center to the farthest corner of the box.
			OM_FORCE_INLINE T getRadius() const
			{
				return T(0.5)*(max - min).getMagnitude();
			}
			
			
			/// Return the center of the bounding box.
			OM_FORCE_INLINE VectorND<T,3> getCenter() const
			{
				return math::midpoint( min, max );
			}
			
			
			/// Return the volume in cubic units enclosed by this 3D range.
			OM_FORCE_INLINE T getVolume() const
			{
				return this->getWidth()*this->getHeight()*this->getDepth();
			}
			
			
			/// Return either the minimal or maximal vertex of this AABB.
			/**
			  * If the index parameter is 0, the minimal vertex is returned, if the
			  * index parameter is 1, the maximal vertex is returned. Otherwise the
			  * result is undefined.
			  */
			OM_FORCE_INLINE const VectorND<T,3>& getMinMax( Index i ) const
			{
				return (&min)[i];
			}
			
			
			/// Return a 1D AABB for the X coordinate range of this AABB.
			OM_FORCE_INLINE AABB1D<T> getX() const
			{
				return AABB1D<T>( min.x, max.x );
			}
			
			
			/// Return a 1D AABB for the Y coordinate range of this AABB.
			OM_FORCE_INLINE AABB1D<T> getY() const
			{
				return AABB1D<T>( min.y, max.y );
			}
			
			
			/// Return a 1D AABB for the Z coordinate range of this AABB.
			OM_FORCE_INLINE AABB1D<T> getZ() const
			{
				return AABB1D<T>( min.z, max.z );
			}
			
			
			/// Return a 2D AABB for the X and Y coordinate ranges of this AABB.
			OM_FORCE_INLINE AABB2D<T> getXY() const
			{
				return AABB2D<T>( min.xy, max.xy );
			}
			
			
			/// Return a 2D AABB for the X and Y coordinate ranges of this AABB.
			OM_FORCE_INLINE AABB2D<T> getYZ() const
			{
				return AABB2D<T>( min.yz, max.yz );
			}
			
			
			/// Return a 2D AABB for the X and Z coordinate ranges of this AABB.
			OM_FORCE_INLINE AABB2D<T> getXZ() const
			{
				return AABB2D<T>( min.xz, max.xz );
			}
			
			
		//********************************************************************************
		//******	Enlargement Methods
			
			
			/// Modify the current bounding box such that it encloses the specified point.
			OM_FORCE_INLINE void enlargeFor( const VectorND<T,3>& point )
			{
				min = math::min( min, point );
				max = math::max( max, point );
			}
			
			
			/// Modify the current bounding box such that it encloses the specified box.
			OM_FORCE_INLINE void enlargeFor( const AABB3D& bounds )
			{
				min = math::min( min, bounds.min );
				max = math::max( max, bounds.max );
			}
			
			
			/// Modify the current bounding box such that it encloses the specified point.
			OM_FORCE_INLINE AABB3D<T>& operator |= ( const VectorND<T,3>& point )
			{
				min = math::min( min, point );
				max = math::max( max, point );
				
				return *this;
			}
			
			
			/// Return the bounding box necessary to enclose a point and the current bounding box.
			OM_FORCE_INLINE AABB3D<T> operator | ( const VectorND<T,3>& point ) const
			{
				return AABB3D<T>( math::min( min, point ), math::max( max, point ) );
			}
			
			
		//********************************************************************************
		//******	Union Methods
			
			
			/// Return the union of this bounding box and another.
			OM_FORCE_INLINE AABB3D<T> getUnion( const AABB3D<T>& bounds ) const
			{
				return AABB3D<T>( math::min( min, bounds.min ), math::max( max, bounds.max ) );
			}
			
			
			/// Modify this bounding box such that it contains the specified bounding box.
			OM_FORCE_INLINE AABB3D<T>& operator |= ( const AABB3D<T>& bounds )
			{
				min = math::min( min, bounds.min );
				max = math::max( max, bounds.max );
				
				return *this;
			}
			
			
			/// Return the union of this bounding box and another.
			OM_FORCE_INLINE AABB3D<T> operator | ( const AABB3D<T>& bounds ) const
			{
				return this->getUnion( bounds );
			}
			
			
		//********************************************************************************
		//******	Intersection Methods
			
			
			/// Return the intersection of this bounding box and another.
			OM_FORCE_INLINE AABB3D<T> getIntersection( const AABB3D<T>& bounds ) const
			{
				return AABB3D<T>( math::min( math::max( min, bounds.min ), bounds.max ),
									math::max( math::min( max, bounds.max ), bounds.min ) );
			}
			
			
			/// Return the intersection of this bounding box and another.
			OM_FORCE_INLINE AABB3D<T>& operator &= ( const AABB3D<T>& bounds )
			{
				min = math::min( math::max( min, bounds.min ), bounds.max );
				max = math::max( math::min( max, bounds.max ), bounds.min );
				
				return *this;
			}
			
			
			/// Return the intersection of this bounding box and another.
			OM_FORCE_INLINE AABB3D<T> operator & ( const AABB3D<T>& bounds ) const
			{
				return this->getIntersection( bounds );
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this bounding box is exactly the same as another.
			OM_INLINE Bool operator == ( const AABB3D<T>& other ) const
			{
				return min == other.min && max == other.max;
			}
			
			
			/// Return whether or not this bounding box is different than another.
			OM_INLINE Bool operator != ( const AABB3D<T>& other ) const
			{
				return min != other.min || max != other.max;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Multiply the minimum and maximum coordinates of this bounding box by the specified scale factor.
			OM_INLINE AABB3D<T> operator * ( T scale ) const
			{
				return AABB3D<T>( min*scale, max*scale );
			}
			
			
			/// Multiply the minimum and maximum coordinates of this bounding box by the specified 3D scale factor.
			OM_INLINE AABB3D<T> operator * ( const VectorND<T,3>& scale ) const
			{
				return AABB3D<T>( min*scale, max*scale );
			}
			
			
			/// Divide the minimum and maximum coordinates of this bounding box by the specified scale factor.
			OM_INLINE AABB3D<T> operator / ( T scale ) const
			{
				return AABB3D<T>( min/scale, max/scale );
			}
			
			
			/// Divide the minimum and maximum coordinates of this bounding box by the specified 3D scale factor.
			OM_INLINE AABB3D<T> operator / ( const VectorND<T,3>& scale ) const
			{
				return AABB3D<T>( min/scale, max/scale );
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this 2D range into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "[ " << min.x << " < " << max.x;
				buffer << ", " << min.y << " < " << max.y;
				buffer << ", " << min.z << " < " << max.z << " ]";
				
				return buffer.toString();
			}
			
			
			/// Convert this 2D range into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Data Members
			
			
			/// The minumum coordinate of the bounding box.
			VectorND<T,3> min;
			
			
			/// The maximum coordinate of the bounding box.
			VectorND<T,3> max;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_AABB_3D_H
