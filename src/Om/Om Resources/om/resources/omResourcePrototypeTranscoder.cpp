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

#include "omResourcePrototypeTranscoder.h"


#include "omResourceReferencePatcher.h"
#include "omResourceSetTranscoder.h"


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




Bool PrototypeResourceTranscoder:: encode( const ResourceID& resourceID, const Prototype& prototype, const ResourceSet& resources,
											ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	// Write the number of components.
	const Size numComponents = prototype.getComponentCount();
	encoder.write( (UInt64)numComponents );
	
	// Write all of the components of each type.
	for ( Index c = 0; c < numComponents; c++ )
	{
		const Prototype::Component& component = prototype.getComponent(c);
		
		// Write the component flags.
		encoder.write( (UInt64)component.flags );
		
		// Write the component reference.
		encoder.writeOpaqueReference( component.resource, resources );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Decoding Method
//############		
//##########################################################################################
//##########################################################################################




Prototype* PrototypeResourceTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
												ResourceReferencePatcher& references,
												ResourceManager* resourceManager )
{
	// Decode the prototype's name and create a new prototype.
	Prototype* prototype = util::construct<Prototype>();
	
	//***********************************************************************
	// Read the prototype's components of each type.
	
	// Read the number of components there are of this type.
	UInt64 numComponents = 0;
	decoder.read( numComponents );
	
	// Read each component.
	for ( Index c = 0; c < numComponents; c++ )
	{
		// Read the component flags.
		UInt64 flags = PrototypeFlags::DEFAULT;
		decoder.read( flags );
		
		// Read the component reference.
		decoder.readOpaqueReference( references, 0, c );
		
		// Add a placeholder for the component.
		prototype->addComponent( Prototype::Component( OpaqueResource(), PrototypeFlags(flags) ) );
	}
	
	return prototype;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reference Patching Method
//############		
//##########################################################################################
//##########################################################################################




void PrototypeResourceTranscoder:: patchReferences( const ResourceReference* references, Size numReferences,
													const ResourceSet& resources, Prototype& prototype )
{
	for ( Index i = 0; i < numReferences; i++ )
	{
		const ResourceReference& reference = references[i];
		
		// Get the resource with the local ID and replace the placeholder in the prototype.
		prototype.getComponent( reference.userID ).resource = resources.getResource( reference.localTypeID, reference.localID );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instancing Methods
//############		
//##########################################################################################
//##########################################################################################




Prototype* PrototypeResourceTranscoder:: instance( const Prototype& prototype,
													const ResourceSetTranscoder& transcoder, ResourceSet& storage ) const
{
	// Construct a new empty prototype.
	Prototype* result = util::construct<Prototype>();
	
	//*********************************************************************************
	// Instance each component.
	
	const Size numComponents = prototype.getComponentCount();
	
	for ( Index i = 0; i < numComponents; i++ )
	{
		Prototype::Component component = prototype.getComponent(i);
		const OpaqueResource& resource = component.resource;
		
		// If the component needs to be instanced, create a copy.
		if ( !component.flags.isSet( PrototypeFlags::SHARED ) )
		{
			// Create a copy of the instance data.
			OpaqueResource instanceData = transcoder.instance( resource, storage );
			
			// Update the component with the new data if the copy is valid.
			if ( instanceData.isSet() )
				component.resource = instanceData;
		}
		
		// Add the child component instance.
		result->addComponent( component );
	}
	
	//*********************************************************************************
	// Replace the instanced component references.
	
	for ( Index i = 0; i < numComponents; i++ )
	{
		Prototype::Component component = prototype.getComponent(i);
		const OpaqueResource& resource = component.resource;
		
		transcoder.replaceInstances( resource.getData(), resource.getID()->getType(),
									prototype, *result, storage );
	}
	
	return result;
}




void PrototypeResourceTranscoder:: replaceInstances( Prototype& data, const Prototype& prototype, const Prototype& instance,
													const ResourceSetTranscoder& transcoder, ResourceSet& resources ) const
{/*
	const Size numTechniques = material.getTechniqueCount();
	
	for ( Index i = 0; i < numTechniques; i++ )
	{
		Technique* technique = material.getTechnique(i);
		Technique* technique2 = replaceInstance( technique, prototype, instance, transcoder, resources );
		
		// Replace the original if the child object was instanced.
		if ( technique2 != technique )
			material.setTechnique( i, technique2 );
	}*/
}




Bool PrototypeResourceTranscoder:: findInstances( const Prototype& data, const Prototype& prototype, const Prototype& instance,
												const ResourceSetTranscoder& transcoder ) const
{/*
	const Size numTechniques = material.getTechniqueCount();
	
	for ( Index i = 0; i < numTechniques; i++ )
	{
		Technique* technique = material.getTechnique(i);
		
		if ( findInstance( technique, prototype, instance, transcoder ) )
			return true;
	}
	*/
	return false;
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
