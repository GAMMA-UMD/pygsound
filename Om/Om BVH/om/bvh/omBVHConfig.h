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

#ifndef INCLUDE_OM_BVH_CONFIG_H
#define INCLUDE_OM_BVH_CONFIG_H


#include "om/omFramework.h"


//##########################################################################################
//##########################################################################################
//############		
//############		Library Namespace Configuration
//############		
//##########################################################################################
//##########################################################################################




/// The enclosing namespace for the entire Om library.
/**
  * @namespace om
  */
namespace om
{
	#define OM_BVH_NAMESPACE_START		namespace om { namespace bvh {
	#define OM_BVH_NAMESPACE_END		}; };
	
	/// A namespace containing bounding volume hierarchy and ray-tracing classes.
	/**
	  * @namespace om::bvh
	  */
	namespace bvh
	{
	};
};




//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


using om::math::Vector2f;
using om::math::Vector2d;

using om::math::Vector3f;
using om::math::Vector3d;

using om::math::Matrix3f;
using om::math::Matrix4f;

using om::math::Transform3f;

using om::math::AABB1D;
using om::math::AABB1f;
using om::math::AABB1d;


using om::math::AABB2D;
using om::math::AABB2f;
using om::math::AABB2d;

using om::math::AABB3D;
using om::math::AABB3f;
using om::math::AABB3d;

using om::math::Ray3D;
using om::math::Ray3f;

using om::math::Sphere3D;
using om::math::Sphere3f;
using om::math::Sphere3d;


using om::math::SIMDFloat4;
using om::math::SIMDInt4;


typedef om::math::SIMDVector3D<Float32,4> SIMDVector3f;
typedef om::math::SIMDRay3D<Float32,4> SIMDRay3f;
typedef om::math::SIMDAABB3D<Float32,4> SIMDAABB3f;
typedef om::math::SIMDTriangle3D<Float32,4> SIMDTriangle3f;
typedef om::math::SIMDPlane3D<Float32,4> SIMDPlane3f;


typedef UInt32 PrimitiveIndex;
typedef UInt32 PrimitiveCount;
typedef UInt32 BVHIndex;
typedef UInt32 BVHCount;


//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_CONFIG_H
