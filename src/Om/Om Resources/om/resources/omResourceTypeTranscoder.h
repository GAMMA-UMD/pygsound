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

#ifndef INCLUDE_OM_RESOURCE_TYPE_TRANSCODER_H
#define INCLUDE_OM_RESOURCE_TYPE_TRANSCODER_H


#include "omResourcesConfig.h"


#include "omResourceTranscoder.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class ResourceManager;
class ResourceSet;



//********************************************************************************
/// A class that defines the interface for other classes that load and save resource data of the templated type.
template < typename DataType >
class ResourceTypeTranscoder : public ResourceTranscoder
{
	public:
		
		//********************************************************************************
		//******	Resource Type Accessor Method
			
			
			/// Return an object that represents the resource type that this transcoder can read and write.
			virtual const ResourceType& getResourceType() const
			{
				return ResourceType::of<DataType>();
			}
			
			
		//********************************************************************************
		//******	Encoding Methods
			
			
			/// Return whether or not this transcoder is able to encode the specified resource.
			virtual Bool canEncode( const DataType& resource ) const = 0;
			
			
			/// Save the specified resource object at the specified ID location.
			/**
			  * The method returns whether or not the resource was successfully written.
			  */
			virtual Bool encode( const ResourceID& identifier, const DataType& resource,
								const ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL ) = 0;
			
			
		//********************************************************************************
		//******	Decoding Methods
			
			
			/// Load the resource pointed to by the specified identifier.
			/**
			  * The caller can supply a pointer to a resource manager which can be used
			  * to load child resources. A resource set is provided as a place to store child
			  * resources. The caller is responsible for destructing the returned object.
			  *
			  * If the method fails, the return value will be NULL.
			  */
			virtual DataType* decode( ResourceID& identifier, ResourceSet* resourceSet = NULL,
										ResourceManager* manager = NULL ) = 0;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_TYPE_TRANSCODER_H
