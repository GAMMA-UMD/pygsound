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

#include "omBVHGeometry.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Geometry Update Method
//############		
//##########################################################################################
//##########################################################################################




void BVHGeometry:: update()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Primitive Acccessor Methods
//############		
//##########################################################################################
//##########################################################################################




BVHGeometry::Type BVHGeometry:: getPrimitiveType() const
{
	return UNDEFINED;
}




Sphere3f BVHGeometry:: getPrimitiveBoundingSphere( PrimitiveIndex primitiveIndex ) const
{
	AABB3f bbox = this->getPrimitiveAABB( primitiveIndex );
	return Sphere3f( bbox.getCenter(), Float(0.5)*bbox.getDiagonal().getMagnitude() );
}




Bool BVHGeometry:: getTriangle( PrimitiveIndex index, Vector3f& v0, Vector3f& v1, Vector3f& v2 ) const
{
	return false;
}




Bool BVHGeometry:: getSphere( PrimitiveIndex index, Vector3f& center, Float& radius ) const
{
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray Tracing Methods
//############		
//##########################################################################################
//##########################################################################################




void BVHGeometry:: intersectRay( PrimitiveIndex primitiveIndex, BVHRay& ray ) const
{
}




void BVHGeometry:: intersectRay( const PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives, BVHRay& ray ) const
{
	for ( PrimitiveIndex i = 0; i < numPrimitives; i++ )
		this->intersectRay( primitiveIndices[i], ray );
}




//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
