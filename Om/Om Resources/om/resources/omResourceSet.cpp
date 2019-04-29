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

#include "omResourceSet.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const ResourceType ResourceSet:: RESOURCE_TYPE("ResourceSet");


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ResourceSet:: ResourceSet()
{
}




ResourceSet:: ResourceSet( const ResourceSet& other )
	:	typeMap( Hash(Float(1.4)*other.typeMap.getSize()), 0.75f )
{
	// Copy all of the other resource set's resources.
	for ( util::HashMap<ResourceType,Index>::ConstIterator i = other.typeMap.getIterator(); i; i++ )
	{
		ResourceTypeSetBase* copy = other.types[i.getValue()]->copy();
		typeMap.add( i.getKeyHash(), i.getKey(), types.getSize() );
		types.add( copy );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ResourceSet:: ~ResourceSet()
{
	this->clearResources();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ResourceSet& ResourceSet:: operator = ( const ResourceSet& other )
{
	if ( this != &other )
	{
		// Remove and destroy all of this resource set's old resources.
		this->clearResources();
		
		// Copy all of the other resource set's resources.
		for ( util::HashMap<ResourceType,Index>::ConstIterator i = other.typeMap.getIterator(); i; i++ )
		{
			ResourceTypeSetBase* copy = other.types[i.getValue()]->copy();
			typeMap.add( i.getKeyHash(), i.getKey(), types.getSize() );
			types.add( copy );
		}
		
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Type Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ResourceSet:: getTypeIndex( const ResourceType& type, ResourceLocalID& typeIndex ) const
{
	const Index* typeIndexPointer;
	
	if ( !typeMap.find( type.getHashCode(), type, typeIndexPointer ) )
		return false;
	
	typeIndex = *typeIndexPointer;
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void* ResourceSet:: getResourceData( const ResourceType& type, ResourceLocalID localID ) const
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
		return types[*typeIndex]->getResourceData( localID );
	
	return NULL;
}




OpaqueResource ResourceSet:: getResource( const ResourceType& type, ResourceLocalID localID ) const
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
		return types[*typeIndex]->getOpaqueResource( localID );
	
	return OpaqueResource();
}




OpaqueResource ResourceSet:: getResource( ResourceLocalID typeIndex, ResourceLocalID localID ) const
{
	if ( typeIndex < types.getSize() )
		return types[typeIndex]->getOpaqueResource( localID );
	
	return OpaqueResource();
}




const ResourceID* ResourceSet:: getResourceID( const ResourceType& type, const void* resource ) const
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
		return types[*typeIndex]->getResourceID( resource );
	
	return NULL;
}




Bool ResourceSet:: getResourceLocalID( const ResourceType& type, const void* resource, ResourceLocalID& localID ) const
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
		return types[*typeIndex]->getResourceLocalID( resource, localID );
	
	return false;
}




ResourceLocalID ResourceSet:: getResourceLocalID( const ResourceType& type, const void* resource ) const
{
	const Index* typeIndex;
	ResourceLocalID localID;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) && types[*typeIndex]->getResourceLocalID( resource, localID ) )
		return localID;
	
	return ResourceID::INVALID_LOCAL_ID;
}




Bool ResourceSet:: getResourceLocalIDs( const ResourceType& type, const void* resource, ResourceLocalID& typeID,
																						ResourceLocalID& localID ) const
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
	{
		typeID = *typeIndex;
		return types[*typeIndex]->getResourceLocalID( resource, localID );
	}
	
	return false;
}




ResourceTypeSetBase* ResourceSet:: getResources( const ResourceType& type )
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
		return types[*typeIndex];
	
	return NULL;
}




const ResourceTypeSetBase* ResourceSet:: getResources( const ResourceType& type ) const
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
		return types[*typeIndex];
	
	return NULL;
}




const ResourceType* ResourceSet:: getResourceType( const ResourceType& type ) const
{
	const Index* typeIndex;
	
	if ( typeMap.find( type.getHashCode(), type, typeIndex ) )
		return &types[*typeIndex]->getResourceType();
	
	return NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Add Methods
//############		
//##########################################################################################
//##########################################################################################




void ResourceSet:: addResources( const ResourceSet& newResources )
{
	const Size numOtherTypes = newResources.types.getSize();
	
	for ( Index i = 0; i < numOtherTypes; i++ )
		newResources.types[i]->copyResources( *this );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Resource Remove Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ResourceSet:: removeResource( const OpaqueResource& resource )
{
	void* resourceData = resource.getData();
	ResourceID* resourceID = resource.getID();
	
	if ( resourceData == NULL || resourceID == NULL )
		return false;
	
	const ResourceType& type = resourceID->getType();
	Index* typeIndex;
	
	if ( !typeMap.find( type.getHashCode(), type, typeIndex ) )
		return false;
	
	return types[(*typeIndex)]->removeResource( resource.getData() );
}




void ResourceSet:: clearResources()
{
	// Iterate over the stored resource arrays and destroy them.
	const Size numTypes = types.getSize();
	for ( Index i = 0; i < numTypes; i++ )
		util::destruct( types[i] );
	
	// Clear the resource types.
	types.clear();
	typeMap.clear();
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
