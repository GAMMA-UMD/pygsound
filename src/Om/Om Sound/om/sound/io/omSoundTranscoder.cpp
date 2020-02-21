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

#include "omSoundTranscoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundTranscoder:: encode( const ResourceID& identifier, const Sound& sound,
								const ResourceSet* resourceSet, ResourceManager* manager )
{
	if ( !canEncode( sound ) )
		return false;
	
	FileWriter writer( identifier.getURL() );
	
	if ( !writer.open() )
		return false;
	
	return this->encode( writer, sound );
}




Bool SoundTranscoder:: encodeToStream( const Sound& sound, SoundOutputStream& encoder )
{
	if ( sound.hasBuffer() )
	{
		const SoundBuffer& buffer = *sound.getBuffer();
		encoder.write( buffer, buffer.getSize() );
	}
	else
	{
		// Get the sound's stream.
		SoundInputStream* soundStream = sound.getStream();
		if ( soundStream == NULL )
			return false;
		
		// Rewind to the beginning of the stream if possible.
		Int64 seekAmount = -(Int64)soundStream->getPosition();
		soundStream->seek( seekAmount );
		
		// Create a temporary buffer.
		const Size bufferSize = 1024;
		const Size numChannels = sound.getChannelCount();
		SoundBuffer tempBuffer( numChannels, bufferSize );
		
		// Transcode from one stream to another.
		while ( soundStream->hasSamplesRemaining() )
		{
			// Read from the stream into the buffer.
			Size samplesRead = (Size)math::min( (LargeSize)bufferSize, soundStream->getSamplesRemaining() );
			samplesRead = soundStream->read( tempBuffer, samplesRead );
			
			// Write to the output encoder.
			encoder.write( tempBuffer, samplesRead );
		}
		
		// Seek back to the previous position in the stream.
		soundStream->seek( (-seekAmount) - (Int64)soundStream->getPosition() );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




Sound* SoundTranscoder:: decode( ResourceID& identifier, ResourceSet* resourceSet, ResourceManager* manager )
{
	if ( !canDecode( identifier ) )
		return NULL;
	
	Shared<FileReader> reader = Shared<FileReader>::construct( identifier.getURL() );
	
	if ( !reader->open() )
		return NULL;
	
	Sound* sound = util::construct<Sound>();
	Bool result = this->decode( reader, *sound );
	
	if ( result )
	{
		identifier.setName( Path( identifier.getURL() ).getBaseName() );
		return sound;
	}
	else
	{
		util::destruct( sound );
		return NULL;
	}
}




Bool SoundTranscoder:: decodeFromStream( Sound& sound, SoundInputStream& decoder )
{
	if ( sound.hasBuffer() )
	{
		SoundBuffer& buffer = *sound.getBuffer();
		buffer.setSize( decoder.read( buffer, decoder.getSamplesRemaining() ) );
	}
	else
	{
		// Create a buffer to hold the sound.
		const Size numChannels = decoder.getChannelCount();
		const Size numSamples = decoder.getSamplesRemaining();
		const SampleRate sampleRate = decoder.getSampleRate();
		
		Shared<SoundBuffer> buffer = Shared<SoundBuffer>::construct( numChannels, numSamples, sampleRate );
		const Size bufferSize = 1024;
		SoundBuffer tempBuffer( numChannels, bufferSize );
		
		// Read the entire contents of the stream into the new buffer.
		Size numSamplesRead = decoder.read( *buffer, numSamples );
		
		// Inform the buffer of its new size.
		buffer->setSize( numSamplesRead );
		
		// Set the sound's buffer.
		sound.setBuffer( buffer );
	}
	
	return true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
