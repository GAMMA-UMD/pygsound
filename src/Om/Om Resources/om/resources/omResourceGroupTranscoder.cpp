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

#include "omResourceGroupTranscoder.h"


#include "omResourceReferencePatcher.h"
#include "omResourceSetTranscoder.h"
#include "omResourceEncoder.h"
#include "omResourceDecoder.h"


#define PARENT_REFERENCE 1
#define RESOURCE_REFERENCE 2


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




Bool ResourceGroupTranscoder:: encode( const ResourceID& resourceID, const ResourceGroup& group, const ResourceSet& resources,
										ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	// Write the parent of the group.
	encoder.writeOpaqueReference( group.getParent(), resources );
	
	//*****************************************************************************
	
	// Write the number of resources in the group.
	const Size numResources = group.getResourceCount();
	encoder.write( (UInt64)numResources );
	
	// Write all of the resources that are in the group.
	for ( Index i = 0; i < numResources; i++ )
		encoder.writeOpaqueReference( group.getResource(i), resources );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Decoding Method
//############		
//##########################################################################################
//##########################################################################################




ResourceGroup* ResourceGroupTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
												ResourceReferencePatcher& references,
												ResourceManager* resourceManager )
{
	// Create a new group object.
	ResourceGroup* group = util::construct<ResourceGroup>();
	
	// Read the parent of the group.
	decoder.readOpaqueReference( references, PARENT_REFERENCE );
	
	//*****************************************************************************
	
	// Read the number of resources there are for this group.
	UInt64 numResources = 0;
	decoder.read( numResources );
	
	// Read each Resource.
	for ( Index i = 0; i < numResources; i++ )
		decoder.readOpaqueReference( references, RESOURCE_REFERENCE, i );
	
	return group;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reference Patching Method
//############		
//##########################################################################################
//##########################################################################################




void ResourceGroupTranscoder:: patchReferences( const ResourceReference* references, Size numReferences,
											const ResourceSet& resources, ResourceGroup& group )
{
	for ( Index i = 0; i < numReferences; i++ )
	{
		const ResourceReference& reference = references[i];
		
		// Get the resource with the local ID and add it to the group.
		OpaqueResource resource = resources.getResource( reference.localTypeID, reference.localID );
		
		if ( resource.isSet() )
		{
			if ( reference.userType == RESOURCE_REFERENCE )
				group.addResource( resource );
			else if ( reference.userType == PARENT_REFERENCE )
				group.setParent( resource );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instancing Methods
//############		
//##########################################################################################
//##########################################################################################




void ResourceGroupTranscoder:: replaceInstances( ResourceGroup& group, const Prototype& prototype, const Prototype& instance,
												const ResourceSetTranscoder& transcoder, ResourceSet& resources ) const
{
	const OpaqueResource& parent = group.getParent();
	OpaqueResource parent2 = replaceInstance( parent, prototype, instance, transcoder, resources );
	
	// Replace the original if the parent object was instanced.
	if ( parent2 != parent )
		group.setParent( parent2 );
	
	//*****************************************************************************
	
	const Size numResources = group.getResourceCount();
	
	for ( Index i = 0; i < numResources; i++ )
	{
		const OpaqueResource& resource = group.getResource(i);
		OpaqueResource resource2 = replaceInstance( resource, prototype, instance, transcoder, resources );
		
		// Replace the original if the child object was instanced.
		if ( resource2 != resource )
			group.setResource( i, resource2 );
	}
}




Bool ResourceGroupTranscoder:: findInstances( const ResourceGroup& group, const Prototype& prototype, const Prototype& instance,
												const ResourceSetTranscoder& transcoder ) const
{
	const Size numResources = group.getResourceCount();
	
	for ( Index i = 0; i < numResources; i++ )
	{
		const OpaqueResource& resource = group.getResource(i);
		
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
