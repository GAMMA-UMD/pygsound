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

#ifndef INCLUDE_OM_BVH_INSTANCE_H
#define INCLUDE_OM_BVH_INSTANCE_H


#include "omBVHConfig.h"


#include "omBVHBVH.h"
#include "omBVHTransform.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that allows a BVH to be instanced with a local transformation.
class BVHInstance : public BVH
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new BVH instance with no BVH and with the identity transform.
			BVHInstance();
			
			
			/// Create a new BVH instance for the specified BVH with the identity transform.
			BVHInstance( BVH* newBVH );
			
			
			/// Create a new BVH instance for the specified BVH with the specified transform.
			BVHInstance( BVH* newBVH, const Transform3f& newTransform );
			
			
		//********************************************************************************
		//******	BVH Accessor Methods
			
			
			/// Return a pointer to the BVH that this BVH instance is instancing.
			OM_INLINE BVH* getBVH() const
			{
				return bvh;
			}
			
			
			/// Set a pointer to the BVH that this BVH instance is instancing.
			OM_INLINE void setBVH( BVH* newBVH )
			{
				bvh = newBVH;
			}
			
			
		//********************************************************************************
		//******	Transform Accessor Methods
			
			
			/// Return the transformation for this BVH instance from local to world space.
			OM_INLINE const BVHTransform& getLocalToWorldTransform() const
			{
				return localToWorld;
			}
			
			
			/// Return the transformation for this BVH instance from world to local space.
			OM_INLINE const BVHTransform& getWorldToLocalTransform() const
			{
				return worldToLocal;
			}
			
			
			/// Set the transformation for this BVH instance with respect to its parent coordinate space.
			OM_INLINE void setTransform( const Transform3f& newTransform )
			{
				localToWorld = BVHTransform( newTransform );
				worldToLocal = localToWorld.invert();
			}
			
			
			/// Set the transformation for this BVH instance with respect to its parent coordinate space.
			OM_INLINE void setTransform( const Matrix4f& newTransform )
			{
				localToWorld = BVHTransform( newTransform );
				worldToLocal = localToWorld.invert();
			}
			
			
		//********************************************************************************
		//******	BVH Geometry Accessor Methods
			
			
			/// Return a pointer to the user geometry used by this BVH.
			/**
			  * The default implementation returns FALSE,
			  * indicating that the BVH does not support user-defined geometry.
			  */
			virtual BVHGeometry* getGeometry() const;
			
			
			/// Set a pointer to the user geometry that this BVH should use.
			/**
			  * This method forwards the geometery to the instance's internal BVH.
			  */
			virtual Bool setGeometry( BVHGeometry* newGeometry );
			
			
		//********************************************************************************
		//******	BVH Building Methods
			
			
			/// Rebuild the BVH using the current set of primitives.
			virtual void rebuild();
			
			
			/// Do a quick update of the BVH by refitting the bounding volumes without changing the hierarchy.
			virtual void refit();
			
			
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			/// Trace the specified ray through this BVH and get the closest intersection.
			/**
			  * The ray is populated with information about the intersection.
			  */
			virtual void intersectRay( BVHRay& ray ) const;
			
			
			/// Test whether or not the specified ray hits anything in this BVH.
			/**
			  * The ray is populated with information about whether or not the ray was hit,
			  * but no intersection results are provided.
			  *
			  * This method can be faster than the intersectRay() method if only a
			  * boolean occlusion result is needed.
			  */
			virtual void testRay( BVHRay& ray ) const;
			
			
		//********************************************************************************
		//******	BVH Attribute Accessor Methods
			
			
			/// Return whether or not this BVH is built, valid, and ready for use.
			virtual Bool isValid() const;
			
			
			/// Return the approximate total amount of memory in bytes allocated for this BVH.
			virtual Size getSizeInBytes() const;
			
			
		//********************************************************************************
		//******	Bounding Volume Accessor Methods
			
			
			/// Return an axis-aligned bounding box for this BVH's contents.
			virtual AABB3f getAABB() const;
			
			
			/// Return a bounding sphere for this BVH's contents.
			/**
			  * The default implementation computes the bounding sphere from the BVH's
			  * bounding box.
			  */
			virtual Sphere3f getBoundingSphere() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A transformation from the parent coordinate frame to the local space of the instance.
			BVHTransform worldToLocal;
			
			
			/// A transformation from the local space of the instance to its parent coordinate frame.
			BVHTransform localToWorld;
			
			
			/// A pointer to the BVH that is instanced.
			BVH* bvh;
			
			
};





//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_INSTANCE_H
