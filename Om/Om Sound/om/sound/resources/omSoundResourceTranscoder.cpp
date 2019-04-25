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

#include "omSoundResourceTranscoder.h"


#include "omSoundBufferResourceTranscoder.h"


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




Bool SoundResourceTranscoder:: encode( const ResourceID& resourceID, const Sound& sound, const ResourceSet& resources,
										ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	// Write the sound encoding format.
	const SoundFormat& format = sound.getFormat();
	encoder.write( (UInt64)format );
	
	// Write the format settings
	const SoundFormatSettings& formatSettings = sound.getFormatSettings();
	encoder.write( (UInt32)formatSettings.getFlags() );
	encoder.write( (UInt64)formatSettings.getSampleType() );
	encoder.write( (Float32)formatSettings.getBitRate() );
	encoder.write( (Float32)formatSettings.getMinBitRate() );
	encoder.write( (Float32)formatSettings.getMaxBitRate() );
	
	//**********************************************************************
	// Write the sound data.
	
	if ( format != SoundFormat::UNDEFINED && sound.hasEncodedData() )
	{
		// Directly write the previously encoded sound data of any format.
		// Write the number of encoded bytes.
		encoder.write( (UInt64)sound.getEncodedDataSize() );
		
		// Write the encoded bytes.
		encoder.write( sound.getEncodedData(), sound.getEncodedDataSize() );
	}
	else
	{
		// See if the sound's preferred format can be encoded.
		if ( format != SoundFormat::UNDEFINED && converter.canEncode( format, sound ) )
		{
			// Write a placeholder for the encoded data size.
			encoder.write( UInt64(0) );
			
			// Save the starting point of the encoded sound so that we can compute its size.
			LargeIndex encodingStart = encoder.getPosition();
			
			// If encoding using an external format fails, just fail completely.
			if ( !converter.encode( format, encoder, sound ) )
				return false;
			
			// Compute the size of the encoded data.
			const UInt64 encodedDataSize = encoder.getPosition() - encodingStart;
			Int64 seekAmount = -Int64(encodedDataSize + sizeof(UInt64));
			
			// Seek so that we can update the encoded data size.
			if ( encoder.seek( seekAmount ) != seekAmount )
				return false;
			
			// Write the encoded data size.
			encoder.write( encodedDataSize );
			
			// Seek to reset the write position to the end of the object.
			encoder.seek( encodedDataSize );
		}
		else
		{
			// Unable to encode the sound using its preferred format, so just write the raw sound buffer.
			if ( sound.hasBuffer() )
			{
				const SoundBuffer& buffer = *sound.getBuffer();
				
				// Encode the buffer.
				SoundBufferResourceTranscoder::encode( buffer, encoder );
			}
			else if ( sound.hasStream() )
			{
				SoundInputStream& stream = *sound.getStream();
				
				// Create a channel layout for the stream.
				const Size numChannels = stream.getChannelCount();
				ChannelLayoutType layoutType(numChannels);
				ChannelLayout layout( layoutType );
				layout.setChannelCount( numChannels );
				
				// Rewind to the beginning of the stream if possible.
				Int64 seekAmount = -(Int64)stream.getPosition();
				stream.seek( seekAmount );
				
				// Get the size of the stream.
				const Size numSamples = stream.getSamplesRemaining();
				const SampleRate sampleRate = stream.getSampleRate();
				
				// Create a temporary buffer for the sound and read the stream into the buffer.
				SoundBuffer buffer( layout, numSamples, sampleRate );
				const Size numSamplesRead = stream.read( buffer, numSamples );
				
				// Shrink the buffer to the valid samples.
				buffer.setSize( numSamplesRead );
				
				// Encode the buffer.
				SoundBufferResourceTranscoder::encode( buffer, encoder );
			}
			else
			{
				// Encode a default sound buffer.
				SoundBufferResourceTranscoder::encode( SoundBuffer(), encoder );
			}
		}
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




Sound* SoundResourceTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
										ResourceReferencePatcher& references,
										ResourceManager* resourceManager )
{
	// Construct a new sound object.
	Sound* sound = util::construct<Sound>();
	
	// Read the sound encoding format.
	UInt64 soundFormatType = (UInt64)SoundFormat::UNDEFINED;
	decoder.read( soundFormatType );
	SoundFormat soundFormat( (SoundFormat::Enum)soundFormatType );
	SoundFormatSettings formatSettings;
	
	// Read the sound format flags.
	UInt32 formatFlags = (UInt32)SoundFormatFlags::UNDEFINED;
	decoder.read( formatFlags );
	formatSettings.setFlags( SoundFormatFlags(formatFlags) );
	
	// Read the sound format sample type.
	UInt64 sampleType = (UInt64)SampleType::UNDEFINED;
	decoder.read( sampleType );
	formatSettings.setSampleType( (SampleType::Enum)sampleType );
	
	// Read the sound format bit rate.
	Float32 bitRate = 0;
	decoder.read( bitRate );
	formatSettings.setBitRate( bitRate );
	
	// Read the sound format minimum bit rate.
	Float32 minBitRate = 0;
	decoder.read( minBitRate );
	formatSettings.setMinBitRate( minBitRate );
	
	// Read the sound format maximum bit rate.
	Float32 maxBitRate = 0;
	decoder.read( maxBitRate );
	formatSettings.setMaxBitRate( maxBitRate );
	
	// Set the format settings for the sound.
	sound->setFormat( soundFormat );
	sound->setFormatSettings( formatSettings );
	
	//**********************************************************************
	// Read the sound data.
	
	if ( converter.canDecode( soundFormat ) )
	{
		// Read the encoded data size.
		UInt64 encodedDataSize = 0;
		decoder.read( encodedDataSize );
		
		// If the format is lossy, load the encoded data so that it can be saved later.
		if ( soundFormat.isLossy() && (resourceManager && resourceManager->getMode() == ResourceMode::READ_WRITE) )
		{
			// Tell the sound to allocate memory for the encoded data.
			if ( !sound->setEncodedDataSize( encodedDataSize ) )
				return sound;
			
			// Read the encoded sound data into the sound's buffer.
			decoder.read( sound->getEncodedData(), encodedDataSize );
			
			// Seek backwards so that we can decode the sound.
			const Int64 seekAmount = -(Int64)encodedDataSize;
			if ( decoder.seek( seekAmount ) != seekAmount )
				return sound;
			
			Shared<BinaryDecoder> dataStream = Shared<BinaryDecoder>::construct();
			dataStream->setData( sound->getEncodedData(), encodedDataSize );
			
			// Decode the sound in the external format.
			if ( !converter.decode( soundFormat, dataStream, *sound ) )
				return sound;
		}
		else
		{
			// Create a copy of the decoder stream.
			Shared<DataInputStream> dataStream = decoder.duplicate();
			
			// Decode the sound in the external format.
			if ( !converter.decode( soundFormat, dataStream, *sound ) )
				return sound;
		}
	}
	else if ( soundFormat == SoundFormat::UNDEFINED )
	{
		// The sound was saved as a raw sound buffer.
		// Create a new buffer to hold the sound data.
		Shared<SoundBuffer> buffer = Shared<SoundBuffer>::construct();
		
		// Read the buffer data.
		SoundBufferResourceTranscoder::decode( decoder, *buffer );
		
		// Set the buffer for the sound.
		sound->setBuffer( buffer );
	}
	else
	{
		// Read the encoded sound data and save it, even though we can't decode the data.
		// Read the encoded data size.
		UInt64 encodedDataSize = 0;
		decoder.read( encodedDataSize );
		
		// Tell the sound to allocate memory for the encoded data.
		if ( !sound->setEncodedDataSize( encodedDataSize ) )
			return sound;
		
		// Read the encoded sound data into the sound's buffer.
		decoder.read( sound->getEncodedData(), encodedDataSize );
	}
	
	return sound;
}




//##########################################################################################
//*************************  End Om Sound Resources Namespace  *****************************
OM_SOUND_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
