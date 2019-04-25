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

#include "omSoundOggTranscoder.h"


#include "omSoundOggDecoder.h"
#include "omSoundOggEncoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




const ResourceFormat& OggTranscoder:: getResourceFormat() const
{
	return SoundFormat::OGG_FORMAT;
}




SoundFormat OggTranscoder:: getSoundFormat() const
{
	return SoundFormat::OGG;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool OggTranscoder:: canEncode( const Sound& sound ) const
{
	return sound.isValid() && sound.getChannelCount() > Size(0);
}




Bool OggTranscoder:: encode( DataOutputStream& stream, const Sound& sound )
{
	if ( !OggTranscoder::canEncode( sound ) )
		return false;
	
	OggEncoder encoder( &stream, sound.getChannelCount(), sound.getSampleRate(),
						sound.getFormatSettings() );
	
	return SoundTranscoder::encodeToStream( sound, encoder );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool OggTranscoder:: canDecode( const ResourceID& resourceID ) const
{
	return resourceID.getType() == ResourceType::of<Sound>() && 
			(resourceID.getFormat() == ResourceFormat::UNDEFINED || resourceID.getFormat() == SoundFormat::OGG_FORMAT);
}




Bool OggTranscoder:: decode( DataInputStream& stream, Sound& sound )
{
	OggDecoder decoder( &stream );
	
	if ( !decoder.isValid() )
		return false;
	
	// Clean up the previous sound.
	sound.unloadBuffer();
	sound.clearEncodedData();
	
	// Decode the entire sound from the stream.
	decodeFromStream( sound, decoder );
	
	// Set the sound's format.
	sound.setFormat( SoundFormat::OGG );
	
	// Set the settings for the sound.
	SoundFormatSettings formatSettings;
	formatSettings.setBitRate( decoder.getBitRate() );
	formatSettings.setMinBitRate( decoder.getMinBitRate() );
	formatSettings.setMaxBitRate( decoder.getMaxBitRate() );
	sound.setFormatSettings( formatSettings );
	
	return true;
}




Bool OggTranscoder:: decode( const Shared<DataInputStream>& stream, Sound& sound )
{
	if ( stream.isNull() )
		return false;
	
	Shared<OggDecoder> decoder = Shared<OggDecoder>::construct( stream );
	
	if ( !decoder->isValid() )
		return false;
	
	// Clean up the previous sound.
	sound.unloadBuffer();
	sound.clearEncodedData();
	
	// Initialize the stream for the sound.
	sound.setStream( Shared<SoundDataInputStream>::construct( decoder, stream ) );
	
	// Set the sound's format.
	sound.setFormat( SoundFormat::OGG );
	
	// Set the settings for the sound.
	SoundFormatSettings formatSettings;
	formatSettings.setBitRate( decoder->getBitRate() );
	formatSettings.setMinBitRate( decoder->getMinBitRate() );
	formatSettings.setMaxBitRate( decoder->getMaxBitRate() );
	sound.setFormatSettings( formatSettings );
	
	return true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
