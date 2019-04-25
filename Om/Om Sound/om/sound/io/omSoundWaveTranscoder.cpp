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

#include "omSoundWaveTranscoder.h"


#include "omSoundWaveDecoder.h"
#include "omSoundWaveEncoder.h"


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




const ResourceFormat& WaveTranscoder:: getResourceFormat() const
{
	return SoundFormat::WAVE_FORMAT;
}




SoundFormat WaveTranscoder:: getSoundFormat() const
{
	return SoundFormat::WAVE;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool WaveTranscoder:: canEncode( const Sound& sound ) const
{
	return sound.isValid() && sound.getChannelCount() > Size(0);
}




Bool WaveTranscoder:: encode( DataOutputStream& stream, const Sound& sound )
{
	if ( !WaveTranscoder::canEncode( sound ) )
		return false;
	
	// Create an encoder.
	const Size numChannels = sound.getChannelCount();
	WaveEncoder encoder( &stream, numChannels,
						sound.getFormatSettings().getSampleType(), sound.getSampleRate() );
	
	return SoundTranscoder::encodeToStream( sound, encoder );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool WaveTranscoder:: canDecode( const ResourceID& resourceID ) const
{
	return resourceID.getType() == ResourceType::of<Sound>() && 
			(resourceID.getFormat() == ResourceFormat::UNDEFINED || resourceID.getFormat() == SoundFormat::WAVE_FORMAT);
}




Bool WaveTranscoder:: decode( DataInputStream& stream, Sound& sound )
{
	WaveDecoder decoder( &stream );
	
	if ( !decoder.isValid() )
		return false;
	
	// Clean up the previous sound.
	sound.unloadBuffer();
	sound.clearEncodedData();
	
	// Decode the entire sound from the stream.
	decodeFromStream( sound, decoder );
	
	// Set the sound's format.
	sound.setFormat( SoundFormat::WAVE );
	
	// Set the settings for the sound.
	SoundFormatSettings formatSettings;
	formatSettings.setSampleType( decoder.getNativeSampleType() );
	sound.setFormatSettings( formatSettings );
	
	return true;
}




Bool WaveTranscoder:: decode( const Shared<DataInputStream>& stream, Sound& sound )
{
	if ( stream.isNull() )
		return false;
	
	Shared<WaveDecoder> decoder = Shared<WaveDecoder>::construct( stream );
	
	if ( !decoder->isValid() )
		return false;
	
	// Clean up the previous sound.
	sound.unloadBuffer();
	sound.clearEncodedData();
	
	// Initialize the stream for the sound.
	sound.setStream( Shared<SoundDataInputStream>::construct( decoder, stream ) );
	
	// Set the sound's format.
	sound.setFormat( SoundFormat::WAVE );
	
	// Set the settings for the sound.
	SoundFormatSettings formatSettings;
	formatSettings.setSampleType( decoder->getNativeSampleType() );
	sound.setFormatSettings( formatSettings );
	
	return true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
