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

#include "omSoundPluginResourceTranscoder.h"


//##########################################################################################
//*************************  Start Om Sound Resources Namespace  ***************************
OM_SOUND_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Sound Encoding Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPluginResourceTranscoder:: encode( const ResourceID& resourceID, const SoundPlugin& plugin, const ResourceSet& resources,
											ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	// Write the ID of the plugin.
	const PluginID& pluginID = plugin.getID();
	
	// Write the plugin type.
	encoder.writeUTF8String( pluginID.getType().getName() );
	
	// Write the plugin manufacturer.
	encoder.writeUTF8String( pluginID.getManufacturer() );
	
	// Write the plugin name.
	encoder.writeUTF8String( pluginID.getName() );
	
	//**********************************************************************
	// Write the filter's data.
	
	const SoundFilter* filter = plugin.getFilter();
	
	// Save the start position of the filter data.
	LargeIndex startPosition = encoder.getPosition();
	
	// Write a placeholder for the size of the filter's data.
	UInt64 dataSize = 0;
	encoder.write( dataSize );
	
	// Write the filter data.
	if ( filter != NULL )
	{
		filter->writeState( encoder );
		
		// Seek back to the start of the data.
		Int64 seekAmount = (Int64)startPosition - (Int64)encoder.getPosition();
		encoder.seek( seekAmount );
		
		// Write the actual data size.
		dataSize = UInt64(-seekAmount) - sizeof(UInt64);
		encoder.write( dataSize );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Decoding Method
//############		
//##########################################################################################
//##########################################################################################




SoundPlugin* SoundPluginResourceTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
													ResourceReferencePatcher& references,
													ResourceManager* resourceManager )
{
	// Read the plugin type.
	PluginType type( decoder.readUTF8String() );
	
	// Read the plugin manufacturer.
	UTF8String manufacturer = decoder.readUTF8String();
	
	// Read the plugin name.
	UTF8String name = decoder.readUTF8String();
	PluginID pluginID( type, manufacturer, name );
	
	//**********************************************************************
	
	SoundFilter* filter;
	
	// Create a filter for the plugin ID.
	if ( pluginManager != NULL )
		filter = pluginManager->createPlugin( pluginID );
		
	// Read the size of the filter's data.
	UInt64 dataSize = 0;
	decoder.read( dataSize );
	
	// Read the filter's data.
	if ( filter != NULL )
		filter->readState( decoder );
	
	// Create the plugin object.
	SoundPlugin* plugin = util::construct<SoundPlugin>( pluginID, Shared<SoundFilter>(filter) );
	
	return plugin;
}




//##########################################################################################
//*************************  End Om Sound Resources Namespace  *****************************
OM_SOUND_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
