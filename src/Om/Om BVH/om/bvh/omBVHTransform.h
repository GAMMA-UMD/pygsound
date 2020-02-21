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

#ifndef INCLUDE_OM_BVH_TRANSFORM_H
#define INCLUDE_OM_BVH_TRANSFORM_H


#include "omBVHConfig.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing an affine transformation for rays and BVH data.
class OM_ALIGN(16) BVHTransform
{
	public:
		
		//********************************************************************************
		//******	Basis Class Declaration
			
			
			/// A class that represents the scaling/rotation part of a 3D affine transformation.
			class OM_ALIGN(16) Basis
			{
				public:
					
					/// Create a new basis with the identity matrix.
					OM_FORCE_INLINE Basis()
						:	x( 1.0f, 0.0f, 0.0f, 0.0f ),
							y( 0.0f, 1.0f, 0.0f, 0.0f ),
							z( 0.0f, 0.0f, 1.0f, 0.0f )
					{
					}
					
					/// Create a new basis with the specified matrix columns.
					OM_FORCE_INLINE Basis( const SIMDFloat4& newX, const SIMDFloat4& newY, const SIMDFloat4& newZ )
						:	x( newX ),
							y( newY ),
							z( newZ )
					{
					}
					
					/// Transform a vector by a basis matrix and return the transformed vector.
					OM_FORCE_INLINE SIMDFloat4 operator * ( const SIMDFloat4& vector ) const
					{
						return x*vector[0] + y*vector[1] + z*vector[2];
					}
					
					/// Transform a basis by another basis and return the resulting combined basis.
					OM_FORCE_INLINE Basis operator * ( const Basis& b ) const
					{
						return Basis( (*this)*b.x, (*this)*b.y, (*this)*b.z );
					}
					
					/// Return the transpose of this basis matrix.
					OM_FORCE_INLINE Basis transpose() const
					{
						SIMDFloat4 tx, ty, tz;
						math::transpose3x3( x, y, z, tx, ty, tz );
						return Basis( tx, ty, tz );
					}
					
					/// Return the determinant of this basis matrix.
					OM_FORCE_INLINE Float32 determinant() const
					{
						return math::dot( x, math::cross( y, z ) )[0];
					}
					
					/// Return the inverse of this basis matrix.
					OM_FORCE_INLINE Basis invert() const
					{
						const Float32 inverseDet = Float32(1) / this->determinant();
						Basis adjoint = Basis( math::cross( y, z ), math::cross( z, x ), math::cross( x, y ) ).transpose();
						return Basis( adjoint.x*inverseDet, adjoint.y*inverseDet, adjoint.z*inverseDet );
					}
					
					/// The X column of the 3x3 rotation/scaling matrix.
					SIMDFloat4 x;
					
					/// The Y column of the 3x3 rotation/scaling matrix.
					SIMDFloat4 y;
					
					/// The Z column of the 3x3 rotation/scaling matrix.
					SIMDFloat4 z;
					
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Construct a BVH transform with the identity transformation.
			OM_FORCE_INLINE BVHTransform()
				:	basis(),
					position( 0.0f )
			{
			}
			
			
			/// Construct a BVH transform for the specified transformation.
			OM_FORCE_INLINE BVHTransform( const math::Transform3f& t )
				:	basis( t.orientation.x*t.scale.x, t.orientation.y*t.scale.y, t.orientation.z*t.scale.z ),
					position( t.position )
			{
			}
			
			
			/// Construct a BVH transform for the specified 4x4 homogeneous coordinate transform matrix.
			OM_FORCE_INLINE BVHTransform( const math::Matrix4f& matrix )
				:	basis( (Vector3f)matrix.x.xyz, (Vector3f)matrix.y.xyz, (Vector3f)matrix.z.xyz ),
					position( matrix.w.xyz )
			{
			}
			
			
			/// Construct a BVH transform from a 3x3 rotation/scaling matrix and position.
			OM_FORCE_INLINE BVHTransform( const SIMDFloat4& newX, const SIMDFloat4& newY,
											const SIMDFloat4& newZ, const SIMDFloat4& newP )
				:	basis( newX, newY, newZ ),
					position( newP )
			{
			}
			
			
			/// Construct a BVH transform from a 3x3 rotation/scaling matrix and position.
			OM_FORCE_INLINE BVHTransform( const Basis& newBasis, const SIMDFloat4& newP )
				:	basis( newBasis ),
					position( newP )
			{
			}
			
			
		//********************************************************************************
		//******	Operators
			
			
			/// Transform a transform by another and return the combined transformation.
			OM_FORCE_INLINE BVHTransform operator * ( const BVHTransform& t ) const
			{
				return BVHTransform( basis*t.basis, basis*t.position + position );
			}
			
			
		//********************************************************************************
		//******	Public Transformation Methods
			
			
			/// Transform a 3D point by this transformation.
			OM_FORCE_INLINE SIMDFloat4 transformPoint( const SIMDFloat4& point ) const
			{
				return position + basis.x*point[0] + basis.y*point[1] + basis.z*point[2];
			}
			
			
			/// Transform a 3D vector by this transformation, neglecting the translation.
			OM_FORCE_INLINE SIMDFloat4 transformVector( const SIMDFloat4& vector ) const
			{
				return basis.x*vector[0] + basis.y*vector[1] + basis.z*vector[2];
			}
			
			
			/// Transform a 3D bounding box by this transformation.
			OM_FORCE_INLINE void transformAABB( const SIMDFloat4& aabbMin, const SIMDFloat4& aabbMax,
												SIMDFloat4& transformedMin, SIMDFloat4& transformedMax ) const
			{
				// Move the box to the origin and scale it.
				const SIMDFloat4 center = (aabbMin + aabbMax)*SIMDFloat4(0.5f);
				const SIMDFloat4 localMin = aabbMin - center;
				const SIMDFloat4 localMax = aabbMax - center;
				
				// Compute the extents along each world axis.
				SIMDFloat4 aX = basis.x*localMin[0];
				SIMDFloat4 bX = basis.x*localMax[0];
				SIMDFloat4 aY = basis.y*localMin[1];
				SIMDFloat4 bY = basis.y*localMax[1];
				SIMDFloat4 aZ = basis.z*localMin[2];
				SIMDFloat4 bZ = basis.z*localMax[2];
				
				// Start at the world-space center of the box.
				transformedMax = transformedMin = this->transformPoint(center);
				
				// Update the final AABB.
				transformedMin += (math::min( aX, bX ) + math::min( aY, bY ) + math::min( aZ, bZ ));
				transformedMax += (math::max( aX, bX ) + math::max( aY, bY ) + math::max( aZ, bZ ));
			}
			
			
			/// Transform a 3D bounding box by this transformation.
			OM_FORCE_INLINE void transformAABB( const AABB3f& aabb, SIMDFloat4& transformedMin, SIMDFloat4& transformedMax ) const
			{
				this->transformAABB( SIMDFloat4(aabb.min), SIMDFloat4(aabb.max), transformedMin, transformedMax );
			}
			
			
			/// Transform a 3D bounding box by this transformation.
			OM_FORCE_INLINE AABB3f transformAABB( const AABB3f& aabb ) const
			{
				SIMDFloat4 transformedMin;
				SIMDFloat4 transformedMax;
				this->transformAABB( SIMDFloat4(aabb.min), SIMDFloat4(aabb.max), transformedMin, transformedMax );
				return AABB3f( (Vector3f)transformedMin, (Vector3f)transformedMax );
			}
			
			
		//********************************************************************************
		//******	Public Inversion Methods
			
			
			/// Return the inverse of this transformation.
			OM_FORCE_INLINE BVHTransform invert() const
			{
				const Basis basisInverse = basis.invert();
				return BVHTransform( basisInverse, -(basisInverse*position) );
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The scaled basis matrix of this transform.
			Basis basis;
			
			
			/// The position offset of the transform's origin relative to the parent origin.
			SIMDFloat4 position;
			
			
};




//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_TRANSFORM_H
