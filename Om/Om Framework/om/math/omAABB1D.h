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

#ifndef INCLUDE_OM_AABB_1D_H
#define INCLUDE_OM_AABB_1D_H


#include "omMathConfig.h"


#include "../data/omString.h"
#include "../data/omStringBuffer.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a range of values in 1D space.
/**
  * This class contains two data members: min and max. These indicate the minimum
  * and maximum values that this axis-aligned bounding box represents. The class
  * invariant is that min is less than max, though this is not enforced. The class
  * supports union, containment, and intersection operations.
  */
template < typename T >
class AABB1D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a 1D axis-aligned bounding box with no extent centered about the origin.
			OM_FORCE_INLINE AABB1D()
				:	min(),
					max()
			{
			}
			
			
			/// Create a 1D axis-aligned bounding box with the minimum and maximum coordinates equal to the specified value.
			OM_FORCE_INLINE AABB1D( T center )
				:	min( center ),
					max( center )
			{
			}
			
			
			/// Create a 1D axis-aligned bounding box with the specified minimum and maximum coodinates.
			OM_FORCE_INLINE AABB1D( T newMin, T newMax )
				:	min( newMin ),
					max( newMax )
			{
			}
			
			
			/// Create a 1D axis-aligned bounding box for the specified array of values.
			OM_FORCE_INLINE AABB1D( const T* points, Size numPoints )
				:	min( math::max<Float>() ),
					max( math::min<Float>() )
			{
				const T* const pointsEnd = points + numPoints;
				
				while ( points != pointsEnd )
				{
					min = math::min( min, *points );
					max = math::max( max, *points );
					points++;
				}
			}
			
			
		//********************************************************************************
		//******	AABB Cast Operator
			
			
			/// Cast this bounding box to a bounding box with a different underlying primitive type.
			template < typename U >
			OM_FORCE_INLINE operator AABB1D<U> () const
			{
				return AABB1D<U>( (U)min, (U)max );
			}
			
			
		//********************************************************************************
		//******	AABB Comparison Methods
			
			
			/// Return whether or not this bounding box completely contains another.
			OM_FORCE_INLINE Bool contains( const AABB1D& bounds ) const
			{
				return min <= bounds.min && max >= bounds.max;
			}
			
			
			/// Return whether or not this bounding box contains the specified value.
			OM_FORCE_INLINE Bool contains( T value ) const
			{
				return value >= min && value <= max;
			}
			
			
			/// Return whether or not this bounding box intersects another.
			OM_FORCE_INLINE Bool intersects( const AABB1D& bounds ) const
			{
				return (min < bounds.max) && (max > bounds.min);
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Set the minimum and maximum values of the axis-aligned bounding box.
			OM_FORCE_INLINE void set( T newMin, T newMax )
			{
				min = newMin;
				max = newMax;
			}
			
			
			/// Get the difference between the maximum and minimum coordinates.
			OM_FORCE_INLINE T getWidth() const
			{
				return max - min;
			}
			
			
			/// Get the difference between the maximum and minimum coordinates.
			OM_FORCE_INLINE T getSize() const
			{
				return max - min;
			}
			
			
			/// Return the distance from the center to the farthest corner of the box.
			OM_FORCE_INLINE T getRadius() const
			{
				return T(0.5)*(max - min);
			}
			
			
			/// Get the center of the bounding box.
			OM_FORCE_INLINE T getCenter() const
			{
				return math::average( min, max );
			}
			
			
			/// Return either the minimal or maximal vertex of this AABB.
			/**
			  * If the index parameter is 0, the minimal vertex is returned, if the
			  * index parameter is 1, the maximal vertex is returned. Otherwise the
			  * result is undefined.
			  */
			OM_FORCE_INLINE T getMinMax( Index i ) const
			{
				return (&min)[i];
			}
			
			
		//********************************************************************************
		//******	Enlargement Methods
			
			
			/// Modify the current bounding box such that it encloses the specified value.
			OM_FORCE_INLINE void enlargeFor( T value )
			{
				min = math::min( min, value );
				max = math::max( max, value );
			}
			
			
			/// Modify the current bounding box such that it encloses the specified box.
			OM_FORCE_INLINE void enlargeFor( const AABB1D& box )
			{
				min = math::min( min, box.min );
				max = math::max( max, box.max );
			}
			
			
			/// Modify the current bounding box such that it encloses the specified value.
			OM_FORCE_INLINE AABB1D<T>& operator |= ( T value )
			{
				min = math::min( min, value );
				max = math::max( max, value );
				
				return *this;
			}
			
			
			/// Return the bounding box necessary to enclose a value and the current bounding box.
			OM_FORCE_INLINE AABB1D<T> operator | ( T value ) const
			{
				return AABB1D<T>( math::min( min, value ), math::max( max, value ) );
			}
			
			
		//********************************************************************************
		//******	Union Methods
			
			
			/// Return the union of this bounding box and another.
			OM_FORCE_INLINE AABB1D<T> getUnion( const AABB1D<T>& bounds ) const
			{
				return AABB1D<T>( math::min( min, bounds.min ), math::max( max, bounds.max ) );
			}
			
			
			/// Modify this bounding box such that it contains the specified bounding box.
			OM_FORCE_INLINE AABB1D<T>& operator |= ( const AABB1D<T>& bounds )
			{
				min = math::min( min, bounds.min );
				max = math::max( max, bounds.max );
				
				return *this;
			}
			
			
			/// Return the union of this bounding box and another.
			OM_FORCE_INLINE AABB1D<T> operator | ( const AABB1D<T>& bounds ) const
			{
				return this->getUnion( bounds );
			}
			
			
		//********************************************************************************
		//******	Intersection Methods
			
			
			/// Return the intersection of this bounding box and another.
			OM_FORCE_INLINE AABB1D<T> getIntersection( const AABB1D<T>& bounds ) const
			{
				return AABB1D<T>( math::max( min, bounds.min ), math::min( max, bounds.max ) );
			}
			
			
			/// Return the intersection of this bounding box and another.
			OM_FORCE_INLINE AABB1D<T>& operator &= ( const AABB1D<T>& bounds )
			{
				min = math::max( min, bounds.min );
				max = math::min( max, bounds.max );
				
				return *this;
			}
			
			
			/// Return the intersection of this bounding box and another.
			OM_FORCE_INLINE AABB1D<T> operator & ( const AABB1D<T>& bounds ) const
			{
				return this->getIntersection( bounds );
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this bounding box is exactly the same as another.
			OM_INLINE Bool operator == ( const AABB1D<T>& other ) const
			{
				return min == other.min && max == other.max;
			}
			
			
			/// Return whether or not this bounding box is different than another.
			OM_INLINE Bool operator != ( const AABB1D<T>& other ) const
			{
				return min != other.min || max != other.max;
			}
			
			
		//********************************************************************************
		//******	Arithmetic Operators
			
			
			/// Multiply the minimum and maximum coordinates of this bounding box by the specified scale factor.
			OM_INLINE AABB1D<T> operator * ( T scale ) const
			{
				return AABB1D<T>( min*scale, max*scale );
			}

			
			
			/// Divide the minimum and maximum coordinates of this bounding box by the specified scale factor.
			OM_INLINE AABB1D<T> operator / ( T scale ) const
			{
				return AABB1D<T>( min/scale, max/scale );
			}
			
			
		//********************************************************************************
		//******	String Conversion Methods
			
			
			/// Convert this 1D range into a human-readable string representation.
			OM_NO_INLINE data::String toString() const
			{
				data::StringBuffer buffer;
				
				buffer << "[ " << min << " < " << max << " ]";
				
				return buffer.toString();
			}
			
			
			/// Convert this 1D range into a human-readable string representation.
			OM_FORCE_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The minumum coordinate of the bounding box.
			T min;
			
			
			/// The maximum coordinate of the bounding box.
			T max;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_AABB_1D_H
