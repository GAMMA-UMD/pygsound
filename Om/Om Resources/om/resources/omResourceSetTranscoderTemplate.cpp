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

#include "omResourceSetTranscoderTemplate.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


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
															ResourceManager* resourceManager )
{
	encoder.writeASCIIString( string );
	return true;
}




template <>
data::String* ResourceSetTranscoderTemplate<data::String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager )
{
	return util::construct<data::String>( decoder.readASCIIString() );
}




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
															ResourceManager* resourceManager )
{
	encoder.writeUTF8String( string );
	return true;
}




template <>
data::UTF8String* ResourceSetTranscoderTemplate<data::UTF8String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager )
{
	return util::construct<data::UTF8String>( decoder.readUTF8String() );
}




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
															ResourceManager* resourceManager )
{
	encoder.writeUTF16String( string );
	return true;
}




template <>
data::UTF16String* ResourceSetTranscoderTemplate<data::UTF16String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager )
{
	return util::construct<data::UTF16String>( decoder.readUTF16String() );
}




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
															ResourceManager* resourceManager )
{
	encoder.writeUTF32String( string );
	return true;
}




template <>
data::UTF32String* ResourceSetTranscoderTemplate<data::UTF32String>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager )
{
	return util::construct<data::UTF32String>( decoder.readUTF32String() );
}




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
															ResourceManager* resourceManager )
{
	// Write the size of the data.
	UInt64 dataSize = data.getSizeInBytes();
	encoder.write( dataSize );
	
	// Write the data.
	encoder.write( data.getPointer(), data.getSizeInBytes() );
	
	return true;
}




template <>
data::Data* ResourceSetTranscoderTemplate<data::Data>:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager )
{
	// Read the size of the data.
	UInt64 dataSize = 0;
	decoder.read( dataSize );
	
	// Read the data.
	UByte* data = util::allocate<UByte>( (Size)dataSize );
	decoder.read( data, (Size)dataSize );
	
	// Construct the data object.
	return util::construct<data::Data>( data::Data::shallow( data, (Size)dataSize ) );
}




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
