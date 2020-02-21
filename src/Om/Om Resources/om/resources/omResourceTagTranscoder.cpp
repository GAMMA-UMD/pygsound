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

#include "omResourceTagTranscoder.h"


#include "omResourceReferencePatcher.h"
#include "omResourceSetTranscoder.h"
#include "omResourceEncoder.h"
#include "omResourceDecoder.h"


#define RESOURCE_REFERENCE 1


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Main Encoding Method
//############		
//##########################################################################################
//##########################################################################################




Bool ResourceTagTranscoder:: encode( const ResourceID& resourceID, const ResourceTag& tag, const ResourceSet& resources,
									ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	// Write the tags's name.
	encoder.writeUTF8String( tag.getName() );
	
	// Write the number of resources.
	const Size numResources = tag.getResourceCount();
	encoder.write( (UInt64)numResources );
	
	// Write all of the resources of each type.
	for ( Index i = 0; i < numResources; i++ )
		encoder.writeOpaqueReference( tag.getResource(i), resources );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Decoding Method
//############		
//##########################################################################################
//##########################################################################################




ResourceTag* ResourceTagTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
											ResourceReferencePatcher& references,
											ResourceManager* resourceManager )
{
	// Decode the tag's name and create a new tag object.
	ResourceTag* tag = util::construct<ResourceTag>( decoder.readUTF8String() );
	
	// Read the number of resources there are for this tag.
	UInt64 numResources = 0;
	decoder.read( numResources );
	
	// Read each Resource.
	for ( Index i = 0; i < numResources; i++ )
		decoder.readOpaqueReference( references, RESOURCE_REFERENCE, i );
	
	return tag;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reference Patching Method
//############		
//##########################################################################################
//##########################################################################################




void ResourceTagTranscoder:: patchReferences( const ResourceReference* references, Size numReferences,
											const ResourceSet& resources, ResourceTag& tag )
{
	for ( Index i = 0; i < numReferences; i++ )
	{
		const ResourceReference& reference = references[i];
		
		// Get the resource with the local ID and add it to the tag.
		OpaqueResource resource = resources.getResource( reference.localTypeID, reference.localID );
		
		if ( resource.isSet() )
			tag.addResource( resource );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instancing Methods
//############		
//##########################################################################################
//##########################################################################################




void ResourceTagTranscoder:: replaceInstances( ResourceTag& tag, const Prototype& prototype, const Prototype& instance,
												const ResourceSetTranscoder& transcoder, ResourceSet& resources ) const
{
	const Size numResources = tag.getResourceCount();
	
	for ( Index i = 0; i < numResources; i++ )
	{
		const OpaqueResource& resource = tag.getResource(i);
		OpaqueResource resource2 = replaceInstance( resource, prototype, instance, transcoder, resources );
		
		// Replace the original if the child object was instanced.
		if ( resource2 != resource )
			tag.setResource( i, resource2 );
	}
}




Bool ResourceTagTranscoder:: findInstances( const ResourceTag& tag, const Prototype& prototype, const Prototype& instance,
												const ResourceSetTranscoder& transcoder ) const
{
	const Size numResources = tag.getResourceCount();
	
	for ( Index i = 0; i < numResources; i++ )
	{
		const OpaqueResource& resource = tag.getResource(i);
		
		if ( findInstance( resource, prototype, instance, transcoder ) )
			return true;
	}
	
	return false;
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
