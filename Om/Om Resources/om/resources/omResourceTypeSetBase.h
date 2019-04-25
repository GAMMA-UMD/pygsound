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

#ifndef INCLUDE_OM_RESOURCE_TYPE_SET_BASE_H
#define INCLUDE_OM_RESOURCE_TYPE_SET_BASE_H


#include "omResourcesConfig.h"


#include "omResourceID.h"
#include "omOpaqueResource.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class ResourceSet;




//********************************************************************************
/// The base class for a set of resources of a generic type.
class ResourceTypeSetBase
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this base resource set.
			virtual ~ResourceTypeSetBase()
			{
			}
			
			
		//********************************************************************************
		//******	Virtual Copy Method
			
			
			/// Create and return a copy of the concrete type of this resource set.
			/**
			  * This enables copies of an resource set to be made without knowing its concrete type.
			  * The caller is responsible for destructing the new resource set.
			  */
			virtual ResourceTypeSetBase* copy() const = 0;
			
			
		//********************************************************************************
		//******	Resource Type Accessor Methods
			
			
			/// Return a reference to the resource type for this resource set.
			virtual const ResourceType& getResourceType() const = 0;
			
			
		//********************************************************************************
		//******	Resource Accessor Methods
			
			
			/// Return the number of resources that are stored in this type set.
			virtual Size getResourceCount() const = 0;
			
			
			/// Return an opaque resource with the specified local ID.
			/**
			  * If the resource set does not contain the given resource, the method fails
			  * and a NULL resource is returned.
			  */
			virtual OpaqueResource getOpaqueResource( ResourceLocalID localID ) const = 0;
			
			
			/// Return an opaque pointer to the resource object data for the given local ID (index).
			virtual void* getResourceData( ResourceLocalID localID ) const = 0;
			
			
			/// Return a pointer to the ID for the resource with the given raw data pointer.
			/**
			  * If the resource set does not contain the given resource, the method fails
			  * and NULL is returned.
			  */
			virtual const ResourceID* getResourceID( const void* resourceData ) const = 0;
			
			
			/// Get the index of the resource with the given raw data pointer in the output parameter.
			/**
			  * If the resource set does not contain the given resource, the method fails
			  * and FALSE is returned.
			  */
			virtual Bool getResourceLocalID( const void* resourceData, ResourceLocalID& localID ) const = 0;
			
			
			/// Add all of the resources of this type to the specified resource set.
			virtual void copyResources( ResourceSet& resourceSet ) const = 0;
			
			
		//********************************************************************************
		//******	Clear Method
			
			
			/// Remove the specified resource from this resource type set.
			/**
			  * The method returns whether or not the resource was successfully removed.
			  */
			virtual Bool removeResource( const void* resourceData ) = 0;
			
			
			/// Remove all resources from this resource set.
			virtual void clearResources() = 0;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_TYPE_SET_BASE_H
