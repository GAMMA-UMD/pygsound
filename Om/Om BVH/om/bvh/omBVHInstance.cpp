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

#include "omBVHInstance.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




BVHInstance:: BVHInstance()
	:	bvh( NULL )
{
}




BVHInstance:: BVHInstance( BVH* newBVH )
	:	bvh( newBVH )
{
}




BVHInstance:: BVHInstance( BVH* newBVH, const Transform3f& newTransform )
	:	bvh( newBVH )
{
	this->setTransform( newTransform );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Geometry Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BVHInstance:: setGeometry( BVHGeometry* newGeometry )
{
	return bvh->setGeometry( newGeometry );
}




BVHGeometry* BVHInstance:: getGeometry() const
{
	return bvh->getGeometry();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Rebuilding Methods
//############		
//##########################################################################################
//##########################################################################################




void BVHInstance:: rebuild()
{
	bvh->rebuild();
}




void BVHInstance:: refit()
{
	bvh->refit();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray Tracing Methods
//############		
//##########################################################################################
//##########################################################################################




void BVHInstance:: intersectRay( BVHRay& ray ) const
{
	// Transform the ray data into the local space.
	BVHRay localRay = ray;
	
	
	// Delegate the ray intersection to the child BVH.
	bvh->testRay( localRay );
	
	// Transform the ray data into the world space.
	
}




void BVHInstance:: testRay( BVHRay& ray ) const
{
	// Transform the ray data into the local space.
	BVHRay localRay = ray;
	
	// Delegate the ray intersection to the child BVH.
	bvh->testRay( localRay );
	
	// Put the result in the output ray.
	ray.geometry = localRay.geometry;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BVHInstance:: isValid() const
{
	return bvh != NULL && bvh->isValid();
}




Size BVHInstance:: getSizeInBytes() const
{
	return sizeof(this) + (bvh == NULL ? 0 : bvh->getSizeInBytes());
}




//##########################################################################################
//##########################################################################################
//############		
//############		Bounding Volume Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




AABB3f BVHInstance:: getAABB() const
{
	return bvh->getAABB();
}




Sphere3f BVHInstance:: getBoundingSphere() const
{
	return bvh->getBoundingSphere();
}




//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
