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




Bool SoundBufferResourceTranscoder:: encode( const ResourceID& resourceID, const SoundBuffer& buffer, const ResourceSet& resources,
										ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	encode( buffer, encoder );
	
	return true;
}




void SoundBufferResourceTranscoder:: encode( const SoundBuffer& buffer, ResourceEncoder& encoder )
{
	// Write the channel layout for the buffer.
	encodeChannelLayout( buffer.getLayout(), encoder );
	
	// Write the size of the buffer.
	const Size numSamples = buffer.getSize();
	encoder.write( (UInt64)numSamples );
	
	// Write the sample rate of the buffer.
	encoder.write( (Float64)buffer.getSampleRate() );
	
	// Write the sample data for each channel.
	const Size numChannels = buffer.getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* channel = buffer.getChannel(c);
		
		if ( encoder.getFormat().endianness.isNative() )
			encoder.write( (const UByte*)channel, numSamples*sizeof(Sample32f) );
		else
			encoder.writeArray( (const Float32*)channel, numSamples );
	}
}




void SoundBufferResourceTranscoder:: encodeChannelLayout( const ChannelLayout& layout, ResourceEncoder& encoder )
{
	// Write the semantic type of the channel layout.
	encoder.write( (UInt64)layout.getType() );
	
	// Write the number of channels in the channel layout.
	const Size numChannels = layout.getChannelCount();
	encoder.write( (UInt64)numChannels );
	
	// Write the layout data for each channel.
	for ( Index c = 0; c < numChannels; c++ )
	{
		const ChannelInfo& channelInfo = layout.getChannelInfo(c);
		
		// Write the semantic type of the channel.
		encoder.write( (UInt64)channelInfo.getType() );
		
		// Write the position of the channel.
		encoder.write( channelInfo.getPosition() );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Decoding Method
//############		
//##########################################################################################
//##########################################################################################




SoundBuffer* SoundBufferResourceTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
													ResourceReferencePatcher& references,
													ResourceManager* resourceManager )
{
	// Create a new buffer to hold the sound data.
	SoundBuffer* buffer = util::construct<SoundBuffer>();
	
	// Decode the buffer.
	decode( decoder, *buffer );
	
	return buffer;
}




void SoundBufferResourceTranscoder:: decode( ResourceDecoder& decoder, SoundBuffer& buffer )
{
	// Create a channel layout for the buffer.
	ChannelLayout layout;
	
	// Read the channel layout.
	decodeChannelLayout( decoder, layout );
	
	// Read the size of the buffer.
	UInt64 numSamples = 0;
	decoder.read( numSamples );
	
	// Read the sample rate of the buffer.
	Float64 sampleRate = 0;
	decoder.read( sampleRate );
	
	// Make sure the buffer has the correct format.
	buffer.setLayout( layout );
	buffer.setSampleRate( sampleRate );
	buffer.setSize( numSamples );
	
	// Read the sample data for each channel.
	const Size numChannels = layout.getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		Sample32f* channel = buffer.getChannel(c);
		
		if ( decoder.getFormat().endianness.isNative() )
			decoder.read( (UByte*)channel, numSamples*sizeof(Sample32f) );
		else
			decoder.readArray( (Float32*)channel, numSamples );
	}
}




void SoundBufferResourceTranscoder:: decodeChannelLayout( ResourceDecoder& decoder, ChannelLayout& layout )
{
	// Read the semantic type of the channel layout.
	UInt64 layoutType = ChannelLayout::UNDEFINED;
	decoder.read( layoutType );
	layout.setType( (ChannelLayout::Type)layoutType );
	
	// Read the number of channels in the channel layout.
	UInt64 numChannels = 0;
	decoder.read( numChannels );
	layout.setChannelCount( numChannels );
	
	// Read the layout data for each channel.
	for ( Index c = 0; c < numChannels; c++ )
	{
		// Read the semantic type of the channel.
		UInt64 channelType = ChannelType::UNDEFINED;
		decoder.read( channelType );
		layout.setChannelType( c, (ChannelType::Enum)channelType );
		
		// Read the position of the channel.
		Vector3f position;
		decoder.read( position );
		layout.setChannelPosition( c, position );
	}
}




//##########################################################################################
//*************************  End Om Sound Resources Namespace  *****************************
OM_SOUND_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
