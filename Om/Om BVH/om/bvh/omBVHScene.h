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

#ifndef INCLUDE_OM_BVH_SCENE_H
#define INCLUDE_OM_BVH_SCENE_H


#include "omBVHConfig.h"


#include "omBVHBVH.h"
#include "omAABBTree4.h"
#include "omBVHTransform.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a collection of transformed bounding volume hierarchies.
class BVHScene : public BVH
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty BVH scene.
			BVHScene();
			
			
		//********************************************************************************
		//******	BVH Accessor Methods
			
			
			/// Return the number of child BVHs that are in this scene.
			OM_INLINE BVHCount getBVHCount() const
			{
				return geometry.bvhs.getSize();
			}
			
			
			/// Return a pointer to the BVH in this scene at the specified index.
			OM_INLINE BVH* getBVH( BVHIndex bvhIndex ) const
			{
				return geometry.bvhs[bvhIndex];
			}
			
			
			/// Return the local-to-world transformation for the BVH in this scene at the specified index.
			OM_INLINE const BVHTransform& getBVHTransform( BVHIndex bvhIndex ) const
			{
				return geometry.transforms[bvhIndex].localToWorld;
			}
			
			
			/// Set the local-to-world transformation for the BVH in this scene at the specified index.
			OM_INLINE void setBVHTransform( BVHIndex bvhIndex, const BVHTransform& newTransform )
			{
				geometry.transforms[bvhIndex] = Transform( newTransform );
			}
			
			
			/// Add a new BVH to this scene with the specified transformation.
			Bool addBVH( BVH* newBVH, const BVHTransform& transform = BVHTransform() );
			
			
			/// Remove the BVH in this scene at the specified index
			void removeBVH( BVHIndex bvhIndex );
			
			
			/// Remove all BVH's from this scene, making it empty.
			void clearBVHs();
			
			
		//********************************************************************************
		//******	BVH Building Methods
			
			
			/// Rebuild the scene BVH using the current set of child BVH's.
			virtual void rebuild();
			
			
			/// Do a quick update of the scene BVH by refitting the bounding volumes without changing the hierarchy.
			virtual void refit();
			
			
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			/// Trace the specified ray through this BVH and get the closest intersection.
			/**
			  * The ray is populated with information about the intersection.
			  */
			OM_INLINE virtual void intersectRay( BVHRay& ray ) const
			{
				bvh.intersectRay( ray );
			}
			
			
			/// Test whether or not the specified ray hits anything in this BVH.
			/**
			  * The ray is populated with information about whether or not the ray was hit,
			  * but no intersection results are provided.
			  *
			  * This method can be faster than the intersectRay() method if only a
			  * boolean occlusion result is needed.
			  */
			OM_INLINE virtual void testRay( BVHRay& ray ) const
			{
				bvh.testRay( ray );
			}
			
			
		//********************************************************************************
		//******	BVH Attribute Accessor Methods
			
			
			/// Return whether or not this scene BVH is built, valid, and ready for use.
			virtual Bool isValid() const;
			
			
			/// Return the approximate total amount of memory in bytes allocated for this scene BVH.
			virtual Size getSizeInBytes() const;
			
			
		//********************************************************************************
		//******	Bounding Volume Accessor Methods
			
			
			/// Return an axis-aligned bounding box for this scene BVH's contents.
			virtual AABB3f getAABB() const;
			
			
			/// Return a bounding sphere for this scene BVH's contents.
			/**
			  * The default implementation computes the bounding sphere from the scene's
			  * bounding box.
			  */
			virtual Sphere3f getBoundingSphere() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that represents a transformed instance of a BVH within the scene.
			class Transform
			{
				public:
					
					/// Create a new transform for the specified local-to-world transform.
					OM_FORCE_INLINE Transform( const BVHTransform& newLocalToWorld )
						:	worldToLocal( newLocalToWorld.invert() ),
							localToWorld( newLocalToWorld )
					{
					}
					
					
					/// A transformation from the parent coordinate frame to the local space of the instance.
					BVHTransform worldToLocal;
					
					
					/// A transformation from the local space of the instance to its parent coordinate frame.
					BVHTransform localToWorld;
					
			};
			
			
			/// A class that contains the geometry for the scene.
			class SceneGeometry : public BVHGeometry
			{
				public:
					
					/// Create a new scene geometry with no child BVH's.
					OM_INLINE SceneGeometry()
					{
					}
					
					
					/// Return the number of BVH's contained in this scene geometry.
					virtual PrimitiveIndex getPrimitiveCount() const
					{
						return bvhs.getSize();
					}
					
					
					/// Return an axis-aligned bounding box for the BVH with the specified index.
					virtual AABB3f getPrimitiveAABB( PrimitiveIndex primitiveIndex ) const
					{
						const BVHTransform localToWorld = transforms[primitiveIndex].localToWorld;
						return localToWorld.transformAABB( bvhs[primitiveIndex]->getAABB() );
					}
					
					
					/// Return a bounding sphere for the BVH with the specified index.
					virtual Sphere3f getPrimitiveBoundingSphere( PrimitiveIndex primitiveIndex ) const
					{
						const BVHTransform localToWorld = transforms[primitiveIndex].localToWorld;
						Sphere3f localSphere = bvhs[primitiveIndex]->getBoundingSphere();
						
						return Sphere3f( localToWorld.transformPoint( localSphere.position ),
										math::max( localToWorld.transformVector( SIMDFloat4( localSphere.radius ) ) )[0] );
					}
					
					
					/// Return whether or not the primitive with the specified index is intersected by the specified ray.
					virtual void intersectRay( PrimitiveIndex primitiveIndex, BVHRay& ray ) const
					{
						intersectSingleBVH( primitiveIndex, ray );
					}
					
					
					/// Return whether or not the primitives with the specified indices are intersected by the specified ray.
					virtual void intersectRay( const PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives, BVHRay& ray ) const
					{
						for ( PrimitiveIndex i = 0; i < numPrimitives; i++ )
							intersectSingleBVH( primitiveIndices[i], ray );
					}
					
					
					/// Return whether or not the primitive with the specified index is intersected by the specified ray.
					OM_FORCE_INLINE void intersectSingleBVH( PrimitiveIndex bvhIndex, BVHRay& ray ) const
					{
						// Transform the ray to local space.
						const BVHTransform worldToLocal = transforms[bvhIndex].worldToLocal;
						const SIMDFloat4 worldOrigin = ray.origin;
						const SIMDFloat4 worldDirection = ray.direction;
						const PrimitiveIndex worldPrimitive = ray.primitive;
						ray.origin = worldToLocal.transformPoint( worldOrigin );
						ray.direction = worldToLocal.transformVector( worldDirection );
						ray.primitive = BVHGeometry::INVALID_PRIMITIVE;
						
						// Intersect the ray with the child BVH.
						bvhs[bvhIndex]->intersectRay( ray );
						
						// Restore the ray state.
						ray.origin = worldOrigin;
						ray.direction = worldDirection;
						
						if ( ray.hitValid() )
						{
							// Transform the normal to world space.
							ray.normal = transforms[bvhIndex].localToWorld.transformVector( ray.normal );
							ray.instance = bvhIndex;
						}
						else
							ray.primitive = worldPrimitive;
					}
					
					
					/// A list of the child BVHs that are in this scene.
					ArrayList<BVH*,PrimitiveIndex> bvhs;
					
					
					/// A list of the transformations for the BVHs that are in the scene.
					ArrayList<Transform,PrimitiveIndex,AlignedAllocator<16> > transforms;
					
			};
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The BVH object that manages the hierarchy of children.
			AABBTree4 bvh;
			
			
			/// An object containing the geometry in the scene.
			SceneGeometry geometry;
			
			
			
};





//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_SCENE_H
