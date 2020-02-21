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

#include "omBVHScene.h"


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




BVHScene:: BVHScene()
{
	bvh.setPrimitivesPerLeaf( 1 );
	bvh.setGeometry( &geometry );
}




//##########################################################################################
//##########################################################################################
//############		
//############		BVH Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BVHScene:: addBVH( BVH* newBVH, const BVHTransform& transform )
{
	if ( newBVH == NULL )
		return false;
	
	geometry.bvhs.add( newBVH );
	geometry.transforms.add( transform );
	
	return true;
}




void BVHScene:: removeBVH( BVHIndex bvhIndex )
{
	geometry.bvhs.removeAtIndexUnordered( bvhIndex );
	geometry.transforms.removeAtIndexUnordered( bvhIndex );
}




void BVHScene:: clearBVHs()
{
	geometry.bvhs.clear();
	geometry.transforms.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Rebuilding Methods
//############		
//##########################################################################################
//##########################################################################################




void BVHScene:: rebuild()
{
	bvh.rebuild();
}




void BVHScene:: refit()
{
	bvh.refit();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BVHScene:: isValid() const
{
	return bvh.isValid();
}




Size BVHScene:: getSizeInBytes() const
{
	return bvh.getSizeInBytes();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Bounding Volume Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




AABB3f BVHScene:: getAABB() const
{
	return bvh.getAABB();
}




Sphere3f BVHScene:: getBoundingSphere() const
{
	return bvh.getBoundingSphere();
}




//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
