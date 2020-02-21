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

#ifndef INCLUDE_OM_RESOURCE_GROUP_TRANSCODER_H
#define INCLUDE_OM_RESOURCE_GROUP_TRANSCODER_H


#include "omResourcesConfig.h"


#include "omResourceSetTypeTranscoder.h"
#include "omResourceGroup.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encodes and decodes resource groups to/from the resource set format.
class ResourceGroupTranscoder : public ResourceSetTypeTranscoder<ResourceGroup>
{
	public:
		
		//********************************************************************************
		//******	Encoding/Decoding Methods
			
			
			/// Encode an object of this resource type into a binary format.
			virtual Bool encode( const ResourceID& resourceID, const ResourceGroup& object, const ResourceSet& resources,
									ResourceEncoder& encoder, ResourceManager* resourceManager = NULL );
			
			
			/// Decode an object of the resource type, constructing and returning a pointer to the final object.
			virtual ResourceGroup* decode( ResourceID& resourceID, ResourceDecoder& decoder,
										ResourceReferencePatcher& references,
										ResourceManager* resourceManager = NULL );
			
			
			/// Patch the specified array of references for a previously-decoded object from the given set of resources.
			virtual void patchReferences( const ResourceReference* references, Size numReferences,
											const ResourceSet& resources, ResourceGroup& object );
			
			
		//********************************************************************************
		//******	Instancing Methods
			
			
			/// Replace child object instances on a new prototype resource.
			virtual void replaceInstances( ResourceGroup& data, const Prototype& prototype, const Prototype& instance,
											const ResourceSetTranscoder& transcoder, ResourceSet& resources ) const;
			
			
			/// Return whether or not any child objects were instanced by a prototype.
			virtual Bool findInstances( const ResourceGroup& data, const Prototype& prototype, const Prototype& instance,
										const ResourceSetTranscoder& transcoder ) const;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_GROUP_TRANSCODER_H
