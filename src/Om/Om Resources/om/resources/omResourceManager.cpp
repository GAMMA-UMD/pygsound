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

#include "omResourceManager.h"


#include "omResourceModule.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ResourceManager:: ResourceManager()
{
}




ResourceManager:: ResourceManager( const ResourceManager& other )
	:	formats( other.formats )
{
	// Clone the type managers from the other manager.
	util::HashMap<ResourceType,TypeManager*>::ConstIterator i = other.types.getIterator();

	while ( i )
	{
		TypeManager* typeManager = (*i)->copy();
		
		if ( typeManager != NULL )
			types.add( i.getKeyHash(), i.getKey(), typeManager );
		
		i++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ResourceManager:: ~ResourceManager()
{
	// Clear all formats to clean up.
	this->clearFormats();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ResourceManager& ResourceManager:: operator = ( const ResourceManager& other )
{
	if ( this != &other )
	{
		// Clear the previous formats from this manager.
		this->clearFormats();
		formats.addAll( other.formats );
		
		// Clone the format managers from the other manager.
		util::HashMap<ResourceType,TypeManager*>::ConstIterator i = other.types.getIterator();
	
		while ( i )
		{
			TypeManager* typeManager = (*i)->copy();
			
			if ( typeManager != NULL )
				types.add( i.getKeyHash(), i.getKey(), typeManager );
			
			i++;
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Load Methods
//############		
//##########################################################################################
//##########################################################################################




void* ResourceManager:: load( const ResourceID& identifier, ResourceSet& resourceSet )
{
	const ResourceType& nominalType = identifier.getType();
	
	if ( nominalType != ResourceType::UNDEFINED )
	{
		// Try to load the data using the nominal resource type transcoder.
		TypeManager** typeManagerBase;
		
		if ( types.find( nominalType.getHashCode(), nominalType, typeManagerBase ) )
		{
			void* result = (*typeManagerBase)->load( identifier, resourceSet, this );
			
			if ( result )
				return result;
		}
	}
	
	// Try every type manager to see if it can load the resource.
	for ( HashMap<ResourceType,TypeManager*>::Iterator i = types.getIterator(); i; i++ )
	{
		void* result = (*i)->load( identifier, resourceSet, this );
		
		if ( result )
			return result;
	}
	
	return NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Save Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ResourceManager:: save( const ResourceID& identifier, const void* data, const ResourceSet& resourceSet )
{
	const ResourceType& nominalType = identifier.getType();
	
	if ( nominalType != ResourceType::UNDEFINED )
	{
		// Try to load the data using the nominal resource type transcoder.
		TypeManager** typeManagerBase;
		
		if ( types.find( nominalType.getHashCode(), nominalType, typeManagerBase ) )
		{
			if ( (*typeManagerBase)->save( identifier, data, resourceSet, this ) )
				return true;
		}
	}
	
	// Try every type manager to see if it can load the resource.
	for ( HashMap<ResourceType,TypeManager*>::Iterator i = types.getIterator(); i; i++ )
	{
		if ( (*i)->save( identifier, data, resourceSet, this ) )
			return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################



void ResourceManager:: addModule( ResourceModule& module )
{
	module.getFormats( *this );
}




void ResourceManager:: clearFormats()
{
	// Destroy the manager for each format.
	util::HashMap<ResourceType,TypeManager*>::Iterator i = types.getIterator();
	
	while ( i )
	{
		util::destruct( *i );
		i++;
	}
	
	// Clear the format map.
	types.clear();
	formats.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Internal Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void ResourceManager:: addFormatInternal( ResourceTranscoder* transcoder )
{
	formats.add( transcoder );
}




void ResourceManager:: removeFormatInternal( const ResourceFormat& format )
{
	for ( Index i = 0; i < formats.getSize(); )
	{
		if ( formats[i]->getResourceFormat() == format )
		{
			formats.removeAtIndexUnordered(i);
			continue;
		}
		
		 i++;
	}
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
