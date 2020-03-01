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

#ifndef INCLUDE_OM_RESOURCE_SET_TYPE_H
#define INCLUDE_OM_RESOURCE_SET_TYPE_H


#include "omResourcesConfig.h"


#include "omResourceTypeSetBase.h"
#include "omResource.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// The base class for a set of resources of a templated type.
template < typename DataType >
class ResourceTypeSet : public ResourceTypeSetBase
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new resource set that is empty and has no resources.
			OM_INLINE ResourceTypeSet()
			{
			}
			
			
		//********************************************************************************
		//******	Virtual Copy Method
			
			
			/// Create and return a copy of the concrete type of this resource set.
			/**
			  * This enables copies of an resource set to be made without knowing its concrete type.
			  * The caller is responsible for destructing the new resource set.
			  */
			virtual ResourceTypeSet<DataType>* copy() const
			{
				return util::construct<ResourceTypeSet<DataType> >( *this );
			}
			
			
		//********************************************************************************
		//******	Resource Type Accessor Methods
			
			
			/// Return a reference to the resource type for this resource set.
			virtual const ResourceType& getResourceType() const
			{
				return ResourceType::of<DataType>();
			}
			
			
		//********************************************************************************
		//******	Resource Accessor Methods
			
			
			/// Return the number of resources that this resource set stores.
			virtual Size getResourceCount() const
			{
				return resources.getSize();
			}
			
			
			/// Return a pointer to the resource in this resource set at the given local ID (index).
			OM_INLINE Resource<DataType>* getResource( ResourceLocalID localID )
			{
				return localID < resources.getSize() ? &resources[localID] : NULL;
			}
			
			
			/// Return a pointer to the resource in this resource set at the given local ID (index).
			OM_INLINE const Resource<DataType>* getResource( ResourceLocalID localID ) const
			{
				return localID < resources.getSize() ? &resources[localID] : NULL;
			}
			
			
			/// Return an opaque resource with the specified local ID.
			virtual OpaqueResource getOpaqueResource( ResourceLocalID localID ) const
			{
				if ( localID >= resources.getSize() )
					return OpaqueResource();
				
				return OpaqueResource( resources[localID] );
			}
			
			
			/// Return an opaque pointer to the resource object data for the given local ID (index).
			virtual void* getResourceData( ResourceLocalID localID ) const
			{
				return localID < resources.getSize() ? resources[localID].getPointer() : NULL;
			}
			
			
			/// Return a pointer to the resource in this resource set with the given name.
			/**
			  * The method returns a NULL pointer if there is no resource with that name.
			  */
			OM_NO_INLINE Resource<DataType>* getResource( const data::UTF8String& name )
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					Resource<DataType>& resource = resources[i];
					ResourceID* resourceID = resource.getID();
					
					if ( resourceID && resourceID->getName() == name )
						return &resource;
				}
				
				return NULL;
			}
			
			
			/// Return a pointer to the resource in this resource set with the given name.
			/**
			  * The method returns a NULL pointer if there is no resource with that name.
			  */
			OM_NO_INLINE const Resource<DataType>* getResource( const data::UTF8String& name ) const
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					const Resource<DataType>& resource = resources[i];
					const ResourceID* resourceID = resource.getID();
					
					if ( resourceID && resourceID->getName() == name )
						return &resource;
				}
				
				return NULL;
			}
			
			
			/// Return a pointer to the resource in this resource set with the given identifier.
			/**
			  * The method returns a NULL pointer if there is no resource with that ID.
			  */
			OM_NO_INLINE Resource<DataType>* getResource( const ResourceID& identifier )
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					Resource<DataType>& resource = resources[i];
					const ResourceID* resourceID = resource.getID();
					
					if ( resourceID && *resourceID == identifier )
						return &resource;
				}
				
				return NULL;
			}
			
			
			/// Find the resource in this resource set with the given identifier.
			/**
			  * The method returns a NULL pointer if there is no resource with that ID.
			  */
			OM_NO_INLINE const Resource<DataType>* getResource( const ResourceID& identifier ) const
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					const Resource<DataType>& resource = resources[i];
					const ResourceID* resourceID = resource.getID();
					
					if ( resourceID && *resourceID == identifier )
						return &resource;
				}
				
				return NULL;
			}
			
			
			/// Return a pointer to the ID for the resource with the given raw data pointer.
			/**
			  * If the resource set does not contain the given resource, the method fails
			  * and NULL is returned.
			  */
			virtual const ResourceID* getResourceID( const void* resourceData ) const
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					const Resource<DataType>& resource = resources[i];
					
					if ( resource.getPointer() == resourceData )
					{
						return resource.getID();
					}
				}
				
				return NULL;
			}
			
			
			/// Get the local ID (e.g. index) for the resource with the given raw data pointer in the output parameter.
			/**
			  * If the resource set does not contain the given resource, the method fails
			  * and FALSE is returned.
			  */
			virtual Bool getResourceLocalID( const void* resourceData, ResourceLocalID& localID ) const
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					const Resource<DataType>& resource = resources[i];
					
					if ( resource.getPointer() == resourceData )
					{
						localID = i;
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Add all of the resources of this type to the specified resource set.
			virtual void copyResources( ResourceSet& resourceSet ) const;
			
			
			/// Return a pointer to the internal array of resources that this resource set stores.
			OM_INLINE Resource<DataType>* getResources()
			{
				return resources.getPointer();
			}
			
			
			/// Return a const pointer to the internal array of resources that this resource set stores.
			OM_INLINE const Resource<DataType>* getResources() const
			{
				return resources.getPointer();
			}
			
			
		//********************************************************************************
		//******	Resource Add Methods
			
			
			/// Add a new resource to this resource set.
			/**
			  * The method returns whether or not the resource was able to be added.
			  */
			OM_INLINE Bool addResource( const Resource<DataType>& resource )
			{
				resources.add( resource );
				return true;
			}
			
			
		//********************************************************************************
		//******	Resource Remove Methods
			
			
			/// Remove a resource with a specific pointer from this resource set.
			/**
			  * The method returns whether or not the resource was successfully removed.
			  */
			OM_INLINE Bool removeResource( const DataType* resource )
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					if ( resources[i].getPointer() == resource )
					{
						resources.removeAtIndexUnordered(i);
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Remove a specific resource from this resource set.
			/**
			  * The method returns whether or not the resource was successfully removed.
			  */
			OM_INLINE Bool removeResource( const Resource<DataType>& resource )
			{
				return resources.remove( resource );
			}
			
			
			/// Remove an resource with the given name from this resource set.
			/**
			  * The method returns whether or not the resource was successfully removed.
			  */
			OM_INLINE Bool removeResource( const data::UTF8String& name )
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					const Resource<DataType>& resource = resources[i];
					const ResourceID* resourceID = resource.getID();
					
					if ( resourceID && resourceID->getName() == name )
					{
						resources.removeAtIndexUnordered(i);
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Remove the specified resource from this resource type set.
			/**
			  * The method returns whether or not the resource was successfully removed.
			  */
			virtual Bool removeResource( const void* resourceData )
			{
				const Size numResources = resources.getSize();
				
				for ( Index i = 0; i < numResources; i++ )
				{
					if ( resources[i].getPointer() == (const DataType*)resourceData )
					{
						resources.removeAtIndexUnordered(i);
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Remove all resources from this resource set.
			virtual void clearResources()
			{
				resources.clear();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A set of the resources stored by this concrete resource array.
			util::ArrayList< Resource<DataType> > resources;
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_SET_TYPE_H
