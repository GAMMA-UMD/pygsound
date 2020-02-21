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

#ifndef INCLUDE_OM_RESOURCE_SET_TYPE_TRANSCODER_H
#define INCLUDE_OM_RESOURCE_SET_TYPE_TRANSCODER_H


#include "omResourcesConfig.h"


#include "omResourceSet.h"
#include "omResourceReference.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class ResourceReferencePatcher;
class ResourceManager;
class Prototype;
class ResourceSetTranscoder;
class ResourceEncoder;
class ResourceDecoder;



//********************************************************************************
/// A class that specifies functions that determine how an resource type should be encoded/decoded.
template < typename DataType >
class ResourceSetTypeTranscoder
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy the resource type transcoder, releasing internal resources.
			virtual ~ResourceSetTypeTranscoder()
			{
			}
			
			
		//********************************************************************************
		//******	Resource Type Accessor Method
			
			
			/// Return the resource type for the data type that this resource type handles.
			OM_INLINE const ResourceType& getResourceType() const
			{
				return ResourceType::of<DataType>();
			}
			
			
		//********************************************************************************
		//******	Resource Encoding/Decoding Methods
			
			
			/// Encode an object of this resource type into a binary format.
			virtual Bool encode( const ResourceID& resourceID, const DataType& data, const ResourceSet& resources,
								ResourceEncoder& encoder, ResourceManager* resourceManager = NULL ) = 0;
			
			
			/// Decode an object of the resource type, constructing and returning a pointer to the final object.
			virtual DataType* decode( ResourceID& resourceID, ResourceDecoder& decoder,
										ResourceReferencePatcher& references,
										ResourceManager* resourceManager = NULL ) = 0;
			
			/// Patch the specified array of references for a previously-decoded object from the given set of resources.
			/**
			  * The default implementation has no effect.
			  */
			virtual void patchReferences( const ResourceReference* references, Size numReferences,
											const ResourceSet& resources, DataType& data )
			{
			}
			
			
			/// Finish loading an object and perform final initialization after all references have been patched.
			/**
			  * The default implementation has no effect.
			  */
			virtual void finalize( DataType& data )
			{
			}
			
			
		//********************************************************************************
		//******	Instancing Methods
			
			
			/// Duplicate an object from a prototype and return a pointer to the new object.
			/**
			  * The default implementation calls the object's copy constructor.
			  */
			virtual DataType* instance( const DataType& data,
										const ResourceSetTranscoder& transcoder, ResourceSet& resources ) const
			{
				return ResourceType::copy( data );
			}
			
			
			/// Replace child object instances on a new prototype component.
			/**
			  * The default implementation has no effect.
			  */
			virtual void replaceInstances( DataType& data, const Prototype& prototype, const Prototype& instance,
											const ResourceSetTranscoder& transcoder, ResourceSet& resources ) const
			{
			}
			
			
			/// Return whether or not any child objects were instanced by a prototype.
			/**
			  * The default implementation has no effect and returns FALSE.
			  */
			virtual Bool findInstances( const DataType& data, const Prototype& prototype, const Prototype& instance,
										const ResourceSetTranscoder& transcoder ) const
			{
				return false;
			}
			
			
	protected:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			template < typename InstanceType >
			static InstanceType* replaceInstance( InstanceType* data, const Prototype& prototype,
												const Prototype& instance, const ResourceSetTranscoder& transcoder, ResourceSet& resources );
			
			
			static OpaqueResource replaceInstance( const OpaqueResource& data, const Prototype& prototype,
											const Prototype& instance, const ResourceSetTranscoder& transcoder, ResourceSet& resources );
			
			
			static void* replaceInstance( void* data, const ResourceType& dataType, const Prototype& prototype,
											const Prototype& instance, const ResourceSetTranscoder& transcoder, ResourceSet& resources );
			
			
			template < typename InstanceType >
			static Bool findInstance( InstanceType* data, const Prototype& prototype,
								const Prototype& instance, const ResourceSetTranscoder& transcoder );
			
			
			static Bool findInstance( const OpaqueResource& data, const Prototype& prototype,
								const Prototype& instance, const ResourceSetTranscoder& transcoder );
			
			
			static Bool findInstance( void* data, const ResourceType& dataType, const Prototype& prototype,
									const Prototype& instance, const ResourceSetTranscoder& transcoder );
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_SET_TYPE_TRANSCODER_H
