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

#ifndef INCLUDE_OM_BVH_RAY_H
#define INCLUDE_OM_BVH_RAY_H


#include "omBVHConfig.h"


#include "omBVHGeometry.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a ray intersection query for BVH geometry.
class OM_ALIGN(16) BVHRay
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Construct a BVH ray with uninitialized contents.
			OM_FORCE_INLINE BVHRay()
			{
			}
			
			
			/// Construct a BVH ray and initialize it for the specified ray.
			/**
			  * The ray intersection query considers the distance range [0,infinity].
			  */
			OM_FORCE_INLINE BVHRay( const Ray3f& ray )
				:	origin( ray.origin ),
					direction( ray.direction ),
					tMin( 0 ),
					tMax( math::infinity<Float>() ),
					primitive( BVHGeometry::INVALID_PRIMITIVE )
			{
			}
			
			
			/// Construct a BVH ray and initialize it for the specified ray information.
			OM_FORCE_INLINE BVHRay( const Ray3f& ray, Float newTMin, Float newTMax )
				:	origin( ray.origin ),
					direction( ray.direction ),
					tMin( newTMin ),
					tMax( newTMax ),
					primitive( BVHGeometry::INVALID_PRIMITIVE )
			{
			}
			
			
			/// Construct a BVH ray and initialize it for the specified ray information.
			OM_FORCE_INLINE BVHRay( const SIMDFloat4& rayOrigin, const SIMDFloat4& rayDirection, Float newTMin, Float newTMax )
				:	origin( rayOrigin ),
					direction( rayDirection ),
					tMin( newTMin ),
					tMax( newTMax ),
					primitive( BVHGeometry::INVALID_PRIMITIVE )
			{
			}
			
			
		//********************************************************************************
		//******	Public Methods
			
			
			/// Compute and return the hit point for the ray.
			OM_FORCE_INLINE SIMDFloat4 getHitPoint() const
			{
				return origin + direction*tMax;
			}
			
			
			/// A convenience method that returns whether or not the ray hit something after a ray query.
			OM_FORCE_INLINE Bool hitValid() const
			{
				return primitive != BVHGeometry::INVALID_PRIMITIVE;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The origin of the ray.
			SIMDFloat4 origin;
			
			
			/// The direction of the ray, can be unnormalized.
			SIMDFloat4 direction;
			
			
			/// The distance along the ray where the ray intersection query starts.
			Float32 tMin;
			
			
			/// The distance along the ray's direction vector where the ray intersection query stops.
			/**
			  * If the ray intersects something, after a ray intersection query this
			  * contains the distance along the ray to the intersection.
			  */
			Float32 tMax;
			
			
			/// The barycentric coordinate of the first vertex of the triangle intersected by the ray.
			Float32 bary0;
			
			
			/// The barycentric coordinate of the second vertex of the triangle intersected by the ray.
			Float32 bary1;
			
			
			/// The un-normalized normal of the ray's hit point on the nearest intersected surface.
			SIMDFloat4 normal;
			
			
			/// The index of the primitive that was hit by the ray.
			/**
			  * If this value is not equal to BVHGeometry::INVALID_PRIMITIVE(-1), then the ray hit something.
			  * E.g. a value of 0 indicates a valid intersection with the 0th primitive.
			  */
			PrimitiveIndex primitive;
			
			
			/// The ID within the BVHScene of the BVH instance that was hit by the ray.
			/**
			  * This value is only used if the ray query is given to a BVHScene that
			  * contains BVH instances.
			  */
			BVHIndex instance;
			
			
			/// A pointer to the BVH geometry that was intersected by the ray, or NULL if the geometry is opaque.
			BVHGeometry* geometry;
			
			
};





//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_RAY_H
