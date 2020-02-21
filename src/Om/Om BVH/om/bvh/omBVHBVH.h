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

#ifndef INCLUDE_OM_BVH_BVH_H
#define INCLUDE_OM_BVH_BVH_H


#include "omBVHConfig.h"


#include "omBVHRay.h"
#include "omBVHGeometry.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A generic interface for a Bounding Volume Hierarchy.
class BVH
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this BVH.
			virtual ~BVH()
			{
			}
			
			
		//********************************************************************************
		//******	BVH Geometry Accessor Methods
			
			
			/// Return a pointer to the user geometry used by this BVH.
			/**
			  * If NULL is returned, there is no user geometry for the BVH.
			  */
			virtual BVHGeometry* getGeometry() const;
			
			
			/// Set a pointer to the user geometry that this BVH should use.
			/**
			  * Calling this method invalidates the current BVH, requiring it
			  * to be rebuilt before it can be used.
			  * The method returns whether or not it succeeds.
			  * The default implementation returns FALSE and has no effect,
			  * indicating that the BVH does not support user-defined geometry.
			  */
			virtual Bool setGeometry( BVHGeometry* newGeometry );
			
			
		//********************************************************************************
		//******	BVH Building Methods
			
			
			/// Rebuild the BVH using the current set of primitives.
			virtual void rebuild() = 0;
			
			
			/// Do a quick update of the BVH by refitting the bounding volumes without changing the hierarchy.
			/**
			  * The default implementation just calls the rebuild() method.
			  */
			virtual void refit();
			
			
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			/// Trace the specified ray through this BVH and get the closest intersection.
			/**
			  * The ray is populated with information about the intersection.
			  */
			virtual void intersectRay( BVHRay& ray ) const = 0;
			
			
			/// Test whether or not the specified ray hits anything in this BVH.
			/**
			  * The ray is populated with information about whether or not the ray was hit,
			  * but no intersection results are provided.
			  *
			  * This method can be faster than the intersectRay() method if only a
			  * boolean occlusion result is needed.
			  */
			virtual void testRay( BVHRay& ray ) const = 0;
			
			
		//********************************************************************************
		//******	BVH Attribute Accessor Methods
			
			
			/// Return whether or not this BVH is built, valid, and ready for use.
			virtual Bool isValid() const = 0;
			
			
			/// Return the approximate total amount of memory in bytes allocated for this BVH.
			virtual Size getSizeInBytes() const = 0;
			
			
		//********************************************************************************
		//******	Bounding Volume Accessor Methods
			
			
			/// Return an axis-aligned bounding box for this BVH's contents.
			virtual AABB3f getAABB() const = 0;
			
			
			/// Return a bounding sphere for this BVH's contents.
			/**
			  * The default implementation computes the bounding sphere from the BVH's
			  * bounding box.
			  */
			virtual Sphere3f getBoundingSphere() const;
			
			
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			/// Return an object containing a pointer to user data for this BVH.
			OM_FORCE_INLINE const UserData& getUserData() const
			{
				return userData;
			}
			
			
			/// Set an object containing a pointer to user data for this BVH.
			OM_FORCE_INLINE void setUserData( const UserData& newData )
			{
				userData = newData;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that contains user data for the BVH.
			UserData userData;
			
			
			
};





//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_BVH_H
