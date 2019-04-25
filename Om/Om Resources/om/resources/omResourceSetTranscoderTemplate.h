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

#ifndef INCLUDE_OM_RESOURCE_SET_TRANSCODER_TEMPLATE_H
#define INCLUDE_OM_RESOURCE_SET_TRANSCODER_TEMPLATE_H


#include "omResourcesConfig.h"


#include "omResourceSetTypeTranscoder.h"
#include "omResourceEncoder.h"
#include "omResourceDecoder.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encodes and decodes a simple type to/from the resource set format.
/**
  * The class's encode and decode methods can be specialized for specific types.
  *
  * The standard template can be used for primitive and vector/matrix types.
  */
template < typename DataType >
class ResourceSetTranscoderTemplate : public ResourceSetTypeTranscoder<DataType>
{
	public:
		
		//********************************************************************************
		//******	Text Encoding/Decoding Methods
			
			
			/// Encode an object of this resource type into a binary format.
			virtual Bool encode( const ResourceID& resourceID, const DataType& data, const ResourceSet& resources,
									ResourceEncoder& encoder,
									ResourceManager* resourceManager = NULL )
			{
				encoder.write( data );
				return true;
			}
			
			
			/// Decode an object of the resource type, constructing and returning a pointer to the final object.
			virtual DataType* decode( ResourceID& resourceID, ResourceDecoder& decoder,
										ResourceReferencePatcher& references,
										ResourceManager* resourceManager = NULL )
			{
				DataType* data = util::construct<DataType>();
				decoder.read( *data );
				return data;
			}
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		ASCII String Specializations
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool ResourceSetTranscoderTemplate<data::String>:: encode( const ResourceID& resourceID, const data::String& string, const ResourceSet& resources,
															ResourceEncoder& encoder,
															ResourceManager* resourceManager );




template <>
data::String* ResourceSetTranscoderTemplate<data::String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager );




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-8 String Specializations
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool ResourceSetTranscoderTemplate<data::UTF8String>:: encode( const ResourceID& resourceID, const data::UTF8String& string, const ResourceSet& resources,
															ResourceEncoder& encoder,
															ResourceManager* resourceManager );




template <>
data::UTF8String* ResourceSetTranscoderTemplate<data::UTF8String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager );




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-16 String Specializations
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool ResourceSetTranscoderTemplate<data::UTF16String>:: encode( const ResourceID& resourceID, const data::UTF16String& string, const ResourceSet& resources,
															ResourceEncoder& encoder,
															ResourceManager* resourceManager );




template <>
data::UTF16String* ResourceSetTranscoderTemplate<data::UTF16String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager );




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-32 String Specializations
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool ResourceSetTranscoderTemplate<data::UTF32String>:: encode( const ResourceID& resourceID, const data::UTF32String& string, const ResourceSet& resources,
															ResourceEncoder& encoder,
															ResourceManager* resourceManager );




template <>
data::UTF32String* ResourceSetTranscoderTemplate<data::UTF32String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager );




//##########################################################################################
//##########################################################################################
//############		
//############		Data Specializations
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool ResourceSetTranscoderTemplate<data::Data>:: encode( const ResourceID& resourceID, const data::Data& data, const ResourceSet& resources,
															ResourceEncoder& encoder,
															ResourceManager* resourceManager );




template <>
data::Data* ResourceSetTranscoderTemplate<data::Data>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager );




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_SET_TRANSCODER_TEMPLATE_H
