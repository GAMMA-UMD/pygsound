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

#ifndef INCLUDE_OM_TRANSFORM_2D_H
#define INCLUDE_OM_TRANSFORM_2D_H


#include "omMathConfig.h"


#include "omVector2D.h"
#include "omMatrix2D.h"
#include "omMatrix3D.h"
#include "omMatrix4D.h"
#include "omRay2D.h"
#include "omPlane2D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a 2-dimensional transformation.
/**
  * The transformation is composed of translation, rotation, and scaling.
  * The components are assumed to be in the following order: translation, rotation,
  * and scaling. Thus, when transforming a point from world to object space by the
  * transformation, translation is first applied, followed by scaling, and finally
  * rotation. The reverse holds true for transformations from object to world space.
  */
template < typename T >
class Transform2D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an identity transformation that doesn't modify transformed points.
			OM_FORCE_INLINE Transform2D()
				:	position( VectorND<T,2>::ZERO ),
					orientation( MatrixND<T,2,2>::IDENTITY ),
					scale( 1 )
			{
			}
			
			
			/// Create a transformation with the specified translation and no rotation or scaling.
			OM_FORCE_INLINE Transform2D( const VectorND<T,2>& newPosition )
				:	position( newPosition ),
					orientation( MatrixND<T,2,2>::IDENTITY ),
					scale( 1 )
			{
			}
			
			
			/// Create a transformation with the specified translation, rotation, and no scaling.
			OM_FORCE_INLINE Transform2D( const VectorND<T,2>& newPosition, 
												const MatrixND<T,2,2>& newOrientation )
				:	position( newPosition ),
					orientation( newOrientation ),
					scale( 1 )
			{
			}
			
			
			/// Create a transformation with the specified translation, rotation, and uniform scaling.
			OM_FORCE_INLINE Transform2D( const VectorND<T,2>& newPosition, 
												const MatrixND<T,2,2>& newOrientation, 
												T newScale )
				:	position( newPosition ),
					orientation( newOrientation ),
					scale( newScale )
			{
			}
			
			
			/// Create a transformation with the specified translation, rotation, and uniform scaling.
			OM_FORCE_INLINE Transform2D( const VectorND<T,2>& newPosition, 
												const MatrixND<T,2,2>& newOrientation, 
												const VectorND<T,2>& newScale )
				:	position( newPosition ),
					orientation( newOrientation ),
					scale( newScale )
			{
			}
			
			
		//********************************************************************************
		//******	Object Space Transforms
			
			
			/// Transform the specified scalar value to object space.
			/**
			  * This will perform any scaling necessary to satisfy the transformation.
			  */
			OM_FORCE_INLINE VectorND<T,2> transformToLocal( T original ) const
			{
				return original/scale;
			}
			
			
			/// Transform the specified position vector to object space.
			OM_FORCE_INLINE VectorND<T,2> transformToLocal( const VectorND<T,2>& original ) const
			{
				return ((original - position)*orientation)/scale;
			}
			
			
			/// Transform the specified matrix to object space.
			/**
			  * This returns what the specified matrix would be in this transformation's
			  * coordinate frame. This method does not perform any scaling on the input
			  * matrix. The input matrix is assumed to be an orthonormal rotation matrix.
			  */
			OM_FORCE_INLINE MatrixND<T,2,2> transformToLocal( const MatrixND<T,2,2>& original ) const
			{
				return original*orientation;
			}
			
			
			/// Transform the specified axis-aligned box into object space, producing another axis-aligned box that encloses the original.
			OM_FORCE_INLINE AABB2D<T> transformToLocal( const AABB2D<T>& box ) const
			{
				AABB2D<T> result( this->transformToLocal( VectorND<T,2>( box.min.x, box.min.y ) ) );
				result.enlargeFor( this->transformToLocal( VectorND<T,2>( box.min.x, box.max.y ) ) );
				result.enlargeFor( this->transformToLocal( VectorND<T,2>( box.max.x, box.max.y ) ) );
				result.enlargeFor( this->transformToLocal( VectorND<T,2>( box.max.x, box.min.y ) ) );
				
				return result;
			}
			
			
			/// Rotate the specified vector to object space.
			/**
			  * This method does not perform any translation or scaling on the
			  * input point. This function is ideal for transforming directional
			  * quantities like surface normal vectors.
			  */
			OM_FORCE_INLINE VectorND<T,2> rotateToLocal( const VectorND<T,2>& original ) const
			{
				return original*orientation;
			}
			
			
			/// Scale a vector to object space.
			OM_FORCE_INLINE VectorND<T,2> scaleToLocal( const VectorND<T,2>& original ) const
			{
				return original*orientation;
			}
			
			
		//********************************************************************************
		//******	World Space Transforms
			
			
			/// Transform the specified scalar value to world space.
			/**
			  * This will perform any scaling necessary to satisfy the transformation.
			  */
			OM_FORCE_INLINE VectorND<T,2> transformToWorld( T original ) const
			{
				return original*scale;
			}
			
			
			/// Transform the specified position vector to world space.
			OM_FORCE_INLINE VectorND<T,2> transformToWorld( const VectorND<T,2>& original ) const
			{
				return position + (orientation*original)*scale;
			}
			
			
			/// Transform the specified matrix to world space.
			/**
			  * This returns what the specified matrix would be in this transformation's
			  * coordinate frame. This method does not perform any scaling on the input
			  * matrix. The input matrix is assumed to be an orthonormal rotation matrix.
			  */
			OM_FORCE_INLINE MatrixND<T,2,2> transformToWorld( const MatrixND<T,2,2>& original ) const
			{
				return orientation*original;
			}
			
			
			/// Transform the specified axis-aligned box into world space, producing another axis-aligned box that encloses the original.
			OM_FORCE_INLINE AABB2D<T> transformToWorld( const AABB2D<T>& box ) const
			{
				// Move the box to the origin and scale it.
				VectorND<T,2> center = box.getCenter();
				AABB2D<T> scaledBox( (box.min - center)*scale, (box.max - center)*scale );
				
				// Start at the world-space center of the box.
				AABB2D<T> result( this->transformToWorld(center) );
				
				for ( Index i = 0; i < 2; i++ )
				{
					// Compute the extents along each world axis.
					VectorND<T,2> a = orientation[i]*scaledBox.min[i];
					VectorND<T,2> b = orientation[i]*scaledBox.max[i];
					
					// Update the final AABB.
					result.min += math::min( a, b );
					result.max += math::max( a, b );
				}
				
				return result;
			}
			
			
			/// Rotate the specified vector to world space.
			/**
			  * This method does not perform any translation or scaling on the
			  * input point. This function is ideal for transforming directional
			  * quantities like surface normal vectors.
			  */
			OM_FORCE_INLINE VectorND<T,2> rotateToWorld( const VectorND<T,2>& original ) const
			{
				return orientation*original;
			}
			
			
			/// Scale a vector to world space.
			OM_FORCE_INLINE VectorND<T,2> scaleToWorld( const VectorND<T,2>& original ) const
			{
				return original*scale;
			}
			
			
		//********************************************************************************
		//******	Transform Multiplication Operators
			
			
			/// Scale the specified value to world space with this transformation.
			OM_FORCE_INLINE T operator * ( T value ) const
			{
				return this->transformToWorld( value );
			}
			
			
			/// Transform the specified vector to world space with this transformation.
			OM_FORCE_INLINE VectorND<T,2> operator * ( const VectorND<T,2>& vector ) const
			{
				return this->transformToWorld( vector );
			}
			
			
			/// Transform the specified matrix to world space with this transformation.
			OM_FORCE_INLINE MatrixND<T,2,2> operator * ( const MatrixND<T,2,2>& matrix ) const
			{
				return this->transformToWorld( matrix );
			}
			
			
			/// Transform the specified ray to world space with this transformation.
			OM_FORCE_INLINE Ray2D<T> operator * ( const Ray2D<T>& ray ) const
			{
				return this->transformToWorld( ray );
			}
			
			
			/// Transform the specified plane to world space with this transformation.
			OM_FORCE_INLINE Plane2D<T> operator * ( const Plane2D<T>& plane ) const
			{
				return this->transformToWorld( plane );
			}
			
			
			/// Concatenate this transformation with another and return the combined transformation.
			/**
			  * This transformation represents the total transformation from object space of the
			  * other, into this transformation's object space, and then to world space.
			  */
			OM_FORCE_INLINE Transform2D<T> operator * ( const Transform2D<T>& other ) const
			{
				return Transform2D<T>( this->transformToWorld( other.position ),
											this->transformToWorld( other.orientation ),
											scale*this->rotateToWorld( other.scale ) );
			}
			
			
		//********************************************************************************
		//******	Transform Inversion Method
			
			
			/// Return the inverse of this transformations that applys the opposite transformation.
			OM_FORCE_INLINE Transform2D<T> invert() const
			{
				VectorND<T,2> inverseScale = T(1)/scale;
				
				return Transform2D<T>( (position*(-inverseScale))*orientation, orientation.transpose(), inverseScale );
			}
			
			
		//********************************************************************************
		//******	Matrix Conversion Methods
			
			
			/// Convert this transformation into a 3x3 homogeneous-coordinate matrix.
			OM_FORCE_INLINE MatrixND<T,3,3> toMatrix() const
			{
				return MatrixND<T,3,3>(	scale.x*orientation.x.x, scale.y*orientation.y.x, position.x,
									scale.x*orientation.x.y, scale.y*orientation.y.y, position.y,
									T(0), T(0), T(1) );
			}
			
			
			/// Convert the inverse of this transformation into a 3x3 homogeneous-coordinate matrix.
			OM_FORCE_INLINE MatrixND<T,3,3> toMatrixInverse() const
			{
				VectorND<T,2> inverseScale = T(1)/scale;
				T zx = -(position.x*orientation.x.x + position.y*orientation.x.y);
				T zy = -(position.x*orientation.y.x + position.y*orientation.y.y);
				
				return MatrixND<T,3,3>( inverseScale.x*orientation.x.x, inverseScale.x*orientation.x.y, zx,
									inverseScale.y*orientation.y.x, inverseScale.y*orientation.y.y, zy,
									T(0), T(0), T(1) );
			}
			
			
			/// Convert this transformation into a 4x4 homogeneous-coordinate matrix.
			OM_FORCE_INLINE MatrixND<T,4,4> toMatrix4() const
			{
				return MatrixND<T,4,4>(	scale.x*orientation.x.x, scale.y*orientation.y.x, T(0), position.x,
									scale.x*orientation.x.y, scale.y*orientation.y.y, T(0), position.y,
									T(0), T(0), T(1), T(0),
									T(0), T(0), T(0), T(1) );
			}
			
			
			/// Convert the inverse of this transformation into a 4x4 homogeneous-coordinate matrix.
			OM_FORCE_INLINE MatrixND<T,4,4> toMatrix4Inverse() const
			{
				VectorND<T,2> inverseScale = T(1)/scale;
				T wx = -( position.x*orientation.x.x + position.y*orientation.x.y );
				T wy = -( position.x*orientation.y.x + position.y*orientation.y.y );
				
				return MatrixND<T,4,4>( inverseScale.x*orientation.x.x, inverseScale.x*orientation.x.y, T(0), wx,
									inverseScale.y*orientation.y.x, inverseScale.y*orientation.y.y, T(0), wy,
									T(0), T(0), T(1), T(0),
									T(0), T(0), T(0), T(1) );
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The translation component of the rigid transformation.
			VectorND<T,2> position;
			
			
			/// The rotation component of the rigid transformation.
			MatrixND<T,2,2> orientation;
			
			
			/// The scaling component of the rigid transformation.
			VectorND<T,2> scale;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Inverse Transform Multiplication Operators
//############		
//##########################################################################################
//##########################################################################################




/// Scale the specified scalar value to object space with the inverse of the specified transformation.
template < typename T >
OM_FORCE_INLINE T operator * ( T value, const Transform2D<T>& transform )
{
	return transform.transformToWorld( value );
}




/// Transform the specified vector to object space with the inverse of the specified transformation.
template < typename T >
OM_FORCE_INLINE VectorND<T,2> operator * ( const VectorND<T,2>& vector, const Transform2D<T>& transform )
{
	return transform.transformToLocal( vector );
}




/// Transform the specified matrix to object space with the inverse of the specified transformation.
template < typename T >
OM_FORCE_INLINE MatrixND<T,2,2> operator * ( const MatrixND<T,2,2>& matrix, const Transform2D<T>& transform )
{
	return transform.transformToLocal( matrix );
}




/// Transform the specified ray to object space with the inverse of the specified transformation.
template < typename T >
OM_FORCE_INLINE Ray2D<T> operator * ( const Ray2D<T>& ray, const Transform2D<T>& transform )
{
	return transform.transformToLocal( ray );
}




/// Transform the specified plane to object space with the inverse of the specified transformation.
template < typename T >
OM_FORCE_INLINE Plane2D<T> operator * ( const Plane2D<T>& plane, const Transform2D<T>& transform )
{
	return transform.transformToLocal( plane );
}




//##########################################################################################
//##########################################################################################
//############		
//############		2D Transform Type Definitions
//############		
//##########################################################################################
//##########################################################################################




typedef Transform2D<int>		Transform2i;
typedef Transform2D<float>		Transform2f;
typedef Transform2D<double>	Transform2d;




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_TRANSFORM_2D_H
