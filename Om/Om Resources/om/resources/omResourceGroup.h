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

#ifndef INCLUDE_OM_RESOURCE_GROUP_H
#define INCLUDE_OM_RESOURCE_GROUP_H


#include "omResourcesConfig.h"


#include "omOpaqueResource.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that stores an ordered list of opaque resources that is used as a semantic grouping.
class ResourceGroup
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new empty resource group.
			ResourceGroup();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a resource group.
			~ResourceGroup();
			
			
		//********************************************************************************
		//******	Parent Accessor Methods
			
			
			/// Return a const reference to the resource that is the parent of the group.
			OM_INLINE const OpaqueResource& getParent() const
			{
				return parent;
			}
			
			
			/// Set the resource that is the parent of the group.
			OM_INLINE void setParent( const OpaqueResource& newParent )
			{
				parent = newParent;
			}
			
			
		//********************************************************************************
		//******	Resource Accessor Methods
			
			
			/// Return the number of resources there are in this resource group.
			OM_INLINE Size getResourceCount() const
			{
				return resources.getSize();
			}
			
			
			/// Return a reference to the resource at the specified index in this resource group.
			OM_INLINE OpaqueResource& getResource( Index resourceIndex )
			{
				return resources[resourceIndex];
			}
			
			
			/// Return a const reference to the resource at the specified index in this resource group.
			OM_INLINE const OpaqueResource& getResource( Index resourceIndex ) const
			{
				return resources[resourceIndex];
			}
			
			
			/// Set the resource at the specified index in this group.
			OM_INLINE void setResource( Index resourceIndex, const OpaqueResource& resource )
			{
				resources[resourceIndex] = resource;
			}
			
			
			/// Add a new resource to the end of this resource group.
			/**
			  * The method returns whether or not the resource was able to be added.
			  */
			Bool addResource( const OpaqueResource& newResource );
			
			
			/// Insert a new resource at the specified index within this resource group.
			/**
			  * The method returns whether or not the resource was able to be inserted.
			  */
			Bool insertResource( Index resourceIndex, const OpaqueResource& newResource );
			
			
			/// Remove the resource in this group at the specified index.
			void removeResource( Index resourceIndex );
			
			
			/// Remove all resources from this resource group.
			void clearResources();
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a resource group.
			static const ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The resource that is the parent of the group.
			OpaqueResource parent;
			
			
			/// A list of the resources that are in this resource group.
			ArrayList<OpaqueResource> resources;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::resources::ResourceGroup, om::resources::ResourceGroup::RESOURCE_TYPE );


#endif // INCLUDE_OM_RESOURCE_GROUP_H
