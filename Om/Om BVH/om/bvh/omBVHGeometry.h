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

#ifndef INCLUDE_OM_BVH_GEOMETRY_H
#define INCLUDE_OM_BVH_GEOMETRY_H


#include "omBVHConfig.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class BVHRay;




//********************************************************************************
/// An interface to an opaque collection of generic geometric primitives.
/**
  * This class allows a BVH to not have to know the concrete type of the
  * geometric primitives that it contains, only generic attributes and operations.
  */
class BVHGeometry
{
	public:
		
		//********************************************************************************
		//******	Public Enum Declaration
			
			
			/// An enum that specifies the standard BVH geometry types.
			enum Type
			{
				/// An undefined geometry type.
				UNDEFINED = 0,
				
				/// A generic user geometry type.
				GENERIC = 0,
				
				/// An enum corresponding to a point geometry.
				POINTS = 1,
				
				/// An enum corresponding to a line segment geometry.
				LINES = 2,
				
				/// An enum corresponding to a triangle geometry.
				TRIANGLES = 3,
				
				/// An enum corresponding to a quad geometry.
				QUADS = 4,
				
				/// An enum corresponding to a AABB geometry.
				AABBS = 5,
				
				/// An enum corresponding to a sphere geometry.
				SPHERES = 6,
				
				/// An enum corresponding to a cylinder geometry.
				CYLINDERS = 7,
				
				/// An enum corresponding to a capsule geometry.
				CAPSULES = 8,
				
				/// An enum corresponding to oriented box geometry.
				BOXES = 9
			};
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this geometry.
			virtual ~BVHGeometry()
			{
			}
			
			
		//********************************************************************************
		//******	Update Method
			
			
			/// Update the internal data structures of this geometry to reflect new state.
			/**
			  * This method is called whenever the BVH is rebuilt.
			  * The default implementation has no effect.
			  */
			virtual void update();
			
			
		//********************************************************************************
		//******	Primitive Accessor Methods
			
			
			/// Return an enum indicating the type of primitives that are in this geometry.
			/**
			  * The default implementation returns UNDEFINED, indicating a generic user-defined
			  * primitive type.
			  */
			virtual Type getPrimitiveType() const;
			
			
			/// Return the number of primitives contained in this geometry.
			virtual PrimitiveIndex getPrimitiveCount() const = 0;
			
			
			/// Return an axis-aligned bounding box for the primitive with the specified index.
			virtual AABB3f getPrimitiveAABB( PrimitiveIndex primitiveIndex ) const = 0;
			
			
			/// Return a bounding sphere for the primitive with the specified index.
			/**
			  * The default implementation computes the bounding sphere from the primitive's
			  * bounding box.
			  */
			virtual Sphere3f getPrimitiveBoundingSphere( PrimitiveIndex primitiveIndex ) const;
			
			
		//********************************************************************************
		//******	Concrete-Type Primitive Accessor Methods
			
			
			/// Get the vertices of the triangle at the specified index in this primitive set.
			/**
			  * The method returns whether or not the triangle was able to be accessed
			  * and if so, places the vertices of the triangle in the output parameters.
			  * The method should only succeed when the type of this geometry
			  * is BVHGeometryType::TRIANGLES.
			  */
			virtual Bool getTriangle( PrimitiveIndex index, Vector3f& v0, Vector3f& v1, Vector3f& v2 ) const;
			
			
			/// Get the center and radius of the sphere at the specified index in this primitive set.
			/**
			  * The method returns whether or not the sphere was able to be accessed
			  * and if so, places the vertices of the sphere in the output parameters.
			  * The method should only succeed when the type of this geometry
			  * is BVHGeometryType::SPHERES.
			  */
			virtual Bool getSphere( PrimitiveIndex index, Vector3f& center, Float& radius ) const;
			
			
		//********************************************************************************
		//******	Intersections Methods
			
			
			/// Return whether or not the primitive with the specified index is intersected by the specified ray.
			/**
			  * The default implementation has no effect.
			  */
			virtual void intersectRay( PrimitiveIndex primitiveIndex, BVHRay& ray ) const;
			
			
			/// Return whether or not the primitives with the specified indices are intersected by the specified ray.
			/**
			  * The default implementation calls the single-primitive version of intersectRay().
			  * Override this method to implement a faster internal loop.
			  */
			virtual void intersectRay( const PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives, BVHRay& ray ) const;
			
			
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			/// Return an object containing a pointer to user data for this geometry.
			OM_FORCE_INLINE const UserData& getUserData() const
			{
				return userData;
			}
			
			
			/// Set an object containing a pointer to user data for this geometry.
			OM_FORCE_INLINE void setUserData( const UserData& newData )
			{
				userData = newData;
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// A constant that represents the index of an invalid primitive.
			static const PrimitiveIndex INVALID_PRIMITIVE = PrimitiveIndex(-1);
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that contains user data for the geometry.
			UserData userData;
			
			
};





//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_GEOMETRY_H
