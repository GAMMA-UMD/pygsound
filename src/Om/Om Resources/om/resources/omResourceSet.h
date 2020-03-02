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

#ifndef INCLUDE_OM_RESOURCE_SET_H
#define INCLUDE_OM_RESOURCE_SET_H


#include "omResourcesConfig.h"


#include "omResourceType.h"
#include "omResourceTypeSet.h"
#include "omResourceTypeSetBase.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents a generic collection of arbitrarily-typed resource objects.
class ResourceSet
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty resource set that has no stored resources.
			ResourceSet();
			
			
			/// Create a copy of another resource set, copying all of its stored resources.
			ResourceSet( const ResourceSet& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an resource set, deallocating all stored resources.
			virtual ~ResourceSet();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of one resource set to another, copying all stored resources.
			ResourceSet& operator = ( const ResourceSet& other );
			
			
		//********************************************************************************
		//******	Resource Type Accessor Methods
			
			
			/// Return the number of resource types that this resource set contains.
			OM_INLINE Size getTypeCount() const
			{
				return types.getSize();
			}
			
			
			/// Return a pointer to the object that manages the resource type at the specified index.
			OM_INLINE ResourceTypeSetBase* getType( Index typeIndex )
			{
				return types[typeIndex];
			}
			
			
			/// Return a pointer to the object that manages the resource type at the specified index.
			OM_INLINE const ResourceTypeSetBase* getType( Index typeIndex ) const
			{
				return types[typeIndex];
			}
			
			
			/// Get the index of the specified resource type within this resource set.
			Bool getTypeIndex( const ResourceType& type, ResourceLocalID& typeIndex ) const;
			
			
		//********************************************************************************
		//******	Resource Accessor Methods
			
			
			/// Return a pointer to the first resource that the resource set has for the specified type.
			/**
			  * If the resource set does not have any resources for the specified template type,
			  * NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE Resource<DataType>* getResource()
			{
				ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL || resourceSet->getResourceCount() == 0 )
					return NULL;
				
				return &resourceSet->getResource(0);
			}
			
			
			/// Return a const pointer to the first resource that the resource set has for the specified type.
			/**
			  * If the resource set does not have any resources for the specified template type,
			  * NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE const Resource<DataType>* getResource() const
			{
				const ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL || resourceSet->getResourceCount() == 0 )
					return NULL;
				
				return &resourceSet->getResource(0);
			}
			
			
			/// Return a pointer to the resource that the resource set has for the specified type and with the given name.
			/**
			  * If the resource set does not have any resources for the specified template type and
			  * a matching name string, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE Resource<DataType>* getResource( const UTF8String& name )
			{
				ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL || resourceSet->getResourceCount() == 0 )
					return NULL;
				
				return resourceSet->getResource( name );
			}
			
			
			/// Return a const pointer to the resource that the resource set has for the specified type and with the given name.
			/**
			  * If the resource set does not have any resources for the specified template type and
			  * a matching name string, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE const Resource<DataType>* getResource( const UTF8String& name ) const
			{
				const ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL || resourceSet->getResourceCount() == 0 )
					return NULL;
				
				return resourceSet->getResource( name );
			}
			
			
			/// Return a const pointer to the resource that the resource set has for the specified type and with the given ID.
			/**
			  * If the resource set does not have any resources for the specified template type and
			  * a matching ID, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE Resource<DataType>* getResource( const ResourceID& identifier )
			{
				ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL || resourceSet->getResourceCount() == 0 )
					return NULL;
				
				return resourceSet->getResource( identifier );
			}
			
			
			/// Return a const pointer to the resource that the resource set has for the specified type and with the given ID.
			/**
			  * If the resource set does not have any resources for the specified template type and
			  * a matching ID, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE const Resource<DataType>* getResource( const ResourceID& identifier ) const
			{
				const ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL || resourceSet->getResourceCount() == 0 )
					return NULL;
				
				return resourceSet->getResource( identifier );
			}
			
			
			/// Return a const pointer to the resource of the templated type at the given index within its type set.
			/**
			  * If there is no resource of the templated type or index, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE Resource<DataType>* getResource( ResourceLocalID localID )
			{
				ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL )
					return NULL;
				
				return resourceSet->getResource( localID );
			}
			
			
			/// Return a const pointer to the resource of the templated type at the given index within its type set.
			/**
			  * If there is no resource of the templated type or index, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE const Resource<DataType>* getResource( ResourceLocalID localID ) const
			{
				const ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL )
					return NULL;
				
				return resourceSet->getResource( localID );
			}
			
			
			/// Return a an opaque pointer to the resource data at the given index within its type set.
			/**
			  * If there is no resource of the specified type or index, NULL is returned.
			  */
			void* getResourceData( const ResourceType& type, ResourceLocalID localID ) const;
			
			
			/// Return an opaque resource at the given local ID within its type set.
			/**
			  * If there is no resource of the specified type or index, a NULL resource is returned.
			  */
			OpaqueResource getResource( const ResourceType& type, ResourceLocalID localID ) const;
			
			
			/// Return an opaque resource at the given local IDs within its type set.
			/**
			  * If there is no resource of the specified type or index, a NULL resource is returned.
			  */
			OpaqueResource getResource( ResourceLocalID localTypeID, ResourceLocalID localID ) const;
			
			
			/// Return a pointer to the ID for the resource with the given type and opaque data pointer.
			/**
			  * If the resource set does not contain the given resource type or resource, the method fails
			  * and NULL is returned.
			  */
			const ResourceID* getResourceID( const ResourceType& type, const void* resource ) const;
			
			
			/// Get the index within its data type for the resource with the given raw data pointer in the output parameter.
			/**
			  * If the resource set does not contain the given resource, the method fails
			  * and FALSE is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool getResourceLocalID( const DataType* resource, ResourceLocalID& localID ) const
			{
				const ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL )
					return false;
				
				return resourceSet->getResourceLocalID( resource, localID );
			}
			
			
			/// Return the index for the resource with the given raw data pointer.
			/**
			  * If the resource set does not contain the given resource, the method returns
			  * ResourceID::INVALID_LOCAL_ID.
			  */
			template < typename DataType >
			OM_NO_INLINE ResourceLocalID getResourceLocalID( const DataType* resource ) const
			{
				const ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				ResourceLocalID localID = ResourceID::INVALID_LOCAL_ID;
				
				if ( resourceSet == NULL || resourceSet->getResourceCount() == 0 )
					return localID;
				
				resourceSet->getResourceLocalID( resource, localID );
				return localID;
			}
			
			
			/// Get the index for the resource with the given type and opaque data pointer in the output parameter.
			/**
			  * If the resource set does not contain the given resource type or resource, the method fails
			  * and FALSE is returned.
			  */
			Bool getResourceLocalID( const ResourceType& type, const void* resource, ResourceLocalID& localID ) const;
			
			
			/// Return the index for the resource with the given resource type and raw data pointer.
			/**
			  * If the resource set does not contain the given resource, the method returns
			  * ResourceID::INVALID_LOCAL_ID.
			  */
			ResourceLocalID getResourceLocalID( const ResourceType& type, const void* resource ) const;
			
			
			/// Get the type index and local index for the resource with the given type and opaque data pointer in the output parameter.
			/**
			  * If the resource set does not contain the given resource type or resource, the method fails
			  * and FALSE is returned.
			  */
			Bool getResourceLocalIDs( const ResourceType& type, const void* resource,
										ResourceLocalID& typeID, ResourceLocalID& localID ) const;
			
			
			/// Return a pointer to a set of resources of the specified template type.
			/**
			  * If the resource set does not have any resources for the specified template type,
			  * NULL is returned.
			  */
			template < typename DataType >
			OM_INLINE ResourceTypeSet<DataType>* getResources()
			{
				return getResourceType<DataType>();
			}
			
			
			/// Return a const pointer to a set of resources of the specified template type.
			/**
			  * If the resource set does not have any resources for the specified template type,
			  * NULL is returned.
			  */
			template < typename DataType >
			OM_INLINE const ResourceTypeSet<DataType>* getResources() const
			{
				return getResourceType<DataType>();
			}
			
			
			/// Return a pointer to a set of resources of the specified resource type.
			/**
			  * If the resource set does not have any resources for the specified resource type,
			  * NULL is returned.
			  */
			ResourceTypeSetBase* getResources( const ResourceType& type );
			
			
			/// Return a const pointer to a set of resources of the specified resource type.
			/**
			  * If the resource set does not have any resources for the specified resource type,
			  * NULL is returned.
			  */
			const ResourceTypeSetBase* getResources( const ResourceType& type ) const;
			
			
			/// Return a pointer to a global resource type object that matches the specified resource type.
			/**
			  * If there is no valid resource type, NULL is returned.
			  */
			const ResourceType* getResourceType( const ResourceType& type ) const;
			
			
		//********************************************************************************
		//******	Resource Adding Methods
			
			
			/// Add the specified resource to this resource set.
			/**
			  * The method returns whether or not adding the resource was successful.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool addResource( const Resource<DataType>& resource )
			{
				ResourceTypeSet<DataType>* resourceSet = initializeResourceType<DataType>();
				
				if ( resourceSet == NULL )
					return false;
				
				return resourceSet->addResource( resource );
			}
			
			
			/// Add all resources from another resource set to this one.
			void addResources( const ResourceSet& resources );
			
			
		//********************************************************************************
		//******	Resource Removing Methods
			
			
			/// Remove the specified resource from this resource set if it exists.
			/**
			  * If the resource set contains a resource that is equal in type and
			  * value to the specified resource, the resource is removed and
			  * TRUE is returned. Otherwise, FALSE is returned and the resource set
			  * is unchanged.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool removeResource( const DataType* resource )
			{
				ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL )
					return false;
				
				return resourceSet->removeResource( resource );
			}
			
			
			/// Remove the resource with the specified name from this resource set if it exists.
			/**
			  * If the resource set contains a resource that is equal in type and
			  * has the specified name, the resource is removed and
			  * TRUE is returned. Otherwise, FALSE is returned and the resource set
			  * is unchanged.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool removeResource( const UTF8String& name )
			{
				ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL )
					return false;
				
				return resourceSet->remove( name );
			}
			
			
			/// Remove the specified opaque resource from this resource set.
			/**
			  * If the resource set contains a resource that is equal in type and
			  * has the specified name, the resource is removed and
			  * TRUE is returned. Otherwise, FALSE is returned and the resource set
			  * is unchanged.
			  */
			Bool removeResource( const OpaqueResource& resource );
			
			
			/// Remove all resources with the specified resource type from this resource set.
			/**
			  * If the resource set contains any resources that have the specified type,
			  * the resources are removed and TRUE is returned. Otherwise, FALSE is
			  * returned and the resource set is unchanged.
			  */
			template < typename DataType >
			OM_NO_INLINE Bool removeResources()
			{
				ResourceTypeSet<DataType>* resourceSet = getResourceType<DataType>();
				
				if ( resourceSet == NULL )
					return false;
				
				resourceSet->clearResources();
				
				return true;
			}
			
			
			/// Remove all resources from this resource set.
			void clearResources();
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return a reference to a string representing the name of this resource set.
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Return a reference to a string representing the name of this resource set.
			OM_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Description Accessor Methods
			
			
			/// Return a reference to a string representing the name of this resource set.
			OM_INLINE const UTF8String& getDescription() const
			{
				return description;
			}
			
			
			/// Return a reference to a string representing the name of this resource set.
			OM_INLINE void setDescription( const UTF8String& newDescription )
			{
				description = newDescription;
			}
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a resource set.
			static const ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Initialize the resource set for the templated data type and return a pointer to it.
			template < typename DataType >
			OM_NO_INLINE ResourceTypeSet<DataType>* initializeResourceType()
			{
				const ResourceType& type = ResourceType::of<DataType>();
				Index* typeIndex;
				
				if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
					return static_cast<ResourceTypeSet<DataType>*>( types[*typeIndex] );
				else
				{
					ResourceTypeSet<DataType>* typeSet = util::construct< ResourceTypeSet<DataType> >();
					
					typeMap.add( type.getHashCode(), type, types.getSize() );
					types.add( typeSet );
					
					return typeSet;
				}
			}
			
			
			/// Access the resource set for the templated data type and return a pointer to it.
			/**
			  * If no resource set with the templated type is found, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE ResourceTypeSet<DataType>* getResourceType()
			{
				const ResourceType& type = ResourceType::of<DataType>();
				Index* typeIndex;
				
				if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
					return static_cast<ResourceTypeSet<DataType>*>( types[*typeIndex] );
				
				return NULL;
			}
			
			
			/// Access the resource set for the templated data type and return a pointer to it.
			/**
			  * If no resource set with the templated type is found, NULL is returned.
			  */
			template < typename DataType >
			OM_NO_INLINE const ResourceTypeSet<DataType>* getResourceType() const
			{
				const ResourceType& type = ResourceType::of<DataType>();
				const Index* typeIndex;
				
				if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
					return static_cast<const ResourceTypeSet<DataType>*>( types[*typeIndex] );
				
				return NULL;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A map from Type object to resource type arrays of all of the resources in the resource set.
			util::HashMap<ResourceType,Index> typeMap;
			
			
			/// A list of the resource types in the resource set.
			util::ArrayList<ResourceTypeSetBase*> types;
			
			
			/// The name for this resource set.
			data::UTF8String name;
			
			
			/// A text description of this resource set.
			data::UTF8String description;
			
			
			
};




template < typename DataType >
void ResourceTypeSet<DataType>:: copyResources( ResourceSet& resourceSet ) const
{
	Index i = 0;
	const Size numResources = resources.getSize();
	ResourceTypeSet<DataType>* typeSet = resourceSet.getResources<DataType>();
	
	// If the resource set doesn't have any resources of this type, add the first one to initialize it.
	if ( !typeSet )
	{
		resourceSet.addResource( resources[i] );
		typeSet = resourceSet.getResources<DataType>();
		i++;
	}
	
	// Add the rest of the resources to the type set more quickly.
	for ( Index i = 0; i < numResources; i++ )
		typeSet->addResource( resources[i] );
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::resources::ResourceSet, om::resources::ResourceSet::RESOURCE_TYPE );


#endif // INCLUDE_OM_RESOURCE_SET_H
