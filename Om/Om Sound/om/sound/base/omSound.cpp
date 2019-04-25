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

#include "omSound.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const om::resources::ResourceType Sound:: RESOURCE_TYPE("Sound");


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Sound:: Sound()
	:	stream(),
		streamPosition( 0 ),
		streamStartPosition( 0 ),
		buffer(),
		bufferPosition( 0 ),
		encodedData( NULL ),
		encodedDataSize( 0 )
{
}




Sound:: Sound( const SoundBuffer& newBuffer )
	:	stream(),
		streamPosition( 0 ),
		streamStartPosition( 0 ),
		buffer( om::util::construct<SoundBuffer>( newBuffer ) ),
		bufferPosition( 0 ),
		encodedData( NULL ),
		encodedDataSize( 0 )
{
}




Sound:: Sound( const SoundBuffer& newBuffer, Size numSamples )
	:	stream(),
		streamPosition( 0 ),
		streamStartPosition( 0 ),
		buffer( om::util::construct<SoundBuffer>( newBuffer, numSamples ) ),
		bufferPosition( 0 ),
		encodedData( NULL ),
		encodedDataSize( 0 )
{
}




Sound:: Sound( const Shared<SoundBuffer>& newBuffer )
	:	stream(),
		streamPosition( 0 ),
		streamStartPosition( 0 ),
		buffer( newBuffer ),
		bufferPosition( 0 ),
		encodedData( NULL ),
		encodedDataSize( 0 )
{
}




Sound:: Sound( const Shared<SoundInputStream>& newStream )
	:	stream( newStream ),
		streamPosition( 0 ),
		streamStartPosition( 0 ),
		buffer(),
		bufferPosition( 0 ),
		encodedData( NULL ),
		encodedDataSize( 0 )
{
	if ( stream.isSet() )
		streamPosition = streamStartPosition = stream->getPosition();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Sound:: ~Sound()
{
	clearEncodedData();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Sound:: setStream( const Shared<SoundInputStream>& newStream )
{
	stream = newStream;
	
	if ( stream.isSet() )
	{
		streamPosition = streamStartPosition = stream->getPosition();
		return true;
	}
	else
	{
		streamPosition = 0;
		streamStartPosition = 0;
		return false;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void Sound:: setBuffer( const Shared<SoundBuffer>& newBuffer )
{
	buffer = newBuffer;
	bufferPosition = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Load Method
//############		
//##########################################################################################
//##########################################################################################




Bool Sound:: loadBuffer( Bool releaseStream )
{
	if ( stream.isNull() )
		return false;
	
	SampleIndex oldStreamPosition = stream->getPosition();
	
	// Determine the offset relative to the actual stream position.
	Int64 delta = (Int64)streamStartPosition - (Int64)oldStreamPosition;
	
	// Adjust the stream's samples remaining by the offset.
	SoundSize streamSize = SoundSize((Int64)stream->getSamplesRemaining() + delta);
	
	// Make sure that the stream has samples remaining and will fit in memory.
	if ( streamSize > SoundSize(0) && streamSize < SoundSize(math::max<Size>()) )
	{
		// Try to seek in the stream to the beginning if we can.
		if ( delta != 0 && stream->canSeek(delta) )
			stream->seek( delta );
		
		Size numChannels = stream->getChannelCount();
		Size numSamples = (Size)stream->getSamplesRemaining();
		SampleRate sampleRate = stream->getSampleRate();
		
		// Create a new buffer to hold the contents of the stream.
		if ( buffer.isNull() )
			buffer = Shared<SoundBuffer>::construct( numChannels, numSamples, sampleRate );
		else
			buffer->setFormat( numChannels, numSamples );
		
		// Read the entire contents of the stream into the new buffer.
		Size numSamplesRead = stream->read( *buffer, numSamples );
		
		// Inform the buffer of its new size.
		buffer->setSize( numSamplesRead );
		
		// Reset the current position within the buffer.
		bufferPosition = 0;
	}
	
	if ( buffer.isNull() )
		return false;
	else
	{
		if ( releaseStream )
			stream.release();
		
		return true;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Unload Method
//############		
//##########################################################################################
//##########################################################################################




Bool Sound:: unloadBuffer()
{
	if ( stream.isNull() )
		return false;
	
	buffer.release();
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Sound:: canSeek() const
{
	if ( buffer.isSet() )
		return true;
	else if ( stream.isSet() )
		return stream->canSeek();
	else
		return false;
}




Bool Sound:: canSeek( Int64 relativeSampleOffset ) const
{
	if ( buffer.isSet() )
	{
		if ( relativeSampleOffset > 0 )
			return bufferPosition + Size(relativeSampleOffset) <= buffer->getSize();
		else
			return bufferPosition >= Size(-relativeSampleOffset);
	}
	else if ( stream.isSet() )
		return stream->canSeek( relativeSampleOffset );
	else
		return false;
}




Int64 Sound:: seek( Int64 relativeSampleOffset )
{
	if ( buffer.isSet() )
	{
		Index oldPosition = bufferPosition;
		
		if ( relativeSampleOffset > 0 )
		{
			bufferPosition = math::min( bufferPosition + Size(relativeSampleOffset), buffer->getSize() );
			return Int64(bufferPosition) - Int64(oldPosition);
		}
		else
		{
			Size offset = Size(-relativeSampleOffset);
			
			if ( bufferPosition >= offset )
				bufferPosition -= offset;
			else
				bufferPosition = 0;
			
			return Int64(bufferPosition) - Int64(oldPosition);
		}
	}
	else if ( stream.isSet() && stream->canSeek() )
	{
		// Determine the actual position to seek to relative to the stream's current position.
		Int64 seekAmount = ((Int64)streamPosition + relativeSampleOffset) - (Int64)stream->getPosition();
		
		// Seek in the stream.
		seekAmount = stream->seek( seekAmount );
		
		// Update the stream position.
		if ( seekAmount > 0 )
			streamPosition += SampleIndex(seekAmount);
		else
			streamPosition -= SampleIndex(-seekAmount);
		
		return seekAmount;
	}
	else
		return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Size Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize Sound:: getSamplesRemaining() const
{
	if ( buffer.isSet() )
		return SoundSize(buffer->getSize() - bufferPosition);
	else if ( stream.isSet() )
	{
		if ( stream->canSeek() )
		{
			// Determine the offset relative to the actual stream position.
			Int64 delta = (Int64)stream->getPosition() - (Int64)streamPosition;
			
			// Adjust the stream's samples remaining by the offset.
			return SoundSize((Int64)stream->getSamplesRemaining() + delta);
		}
		else
			return stream->getSamplesRemaining();
	}
	else
		return SoundSize(0);
}




SampleIndex Sound:: getPosition() const
{
	if ( buffer.isSet() )
		return bufferPosition;
	else if ( stream.isSet() )
		return stream->getPosition();
	else
		return SampleIndex(0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size Sound:: getChannelCount() const
{
	if ( buffer.isSet() )
		return buffer->getChannelCount();
	else if ( stream.isSet() )
		return stream->getChannelCount();
	else
		return Size(0);
}




SampleRate Sound:: getSampleRate() const
{
	if ( buffer.isSet() )
		return buffer->getSampleRate();
	else if ( stream.isSet() )
		return stream->getSampleRate();
	else
		return Size(0);
}




SampleType Sound:: getNativeSampleType() const
{
	if ( buffer.isSet() )
		return SampleType::SAMPLE_32F;
	else if ( stream.isSet() )
		return stream->getNativeSampleType();
	else
		return SampleType::UNDEFINED;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Status Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool Sound:: isValid() const
{
	if ( buffer.isSet() || (stream.isSet() && stream->isValid()) )
		return true;
	else
		return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void Sound:: setFormat( const SoundFormat& newFormat )
{
	// Clear the encoded sound if the format type changes.
	if ( format != newFormat && encodedData != NULL )
		this->clearEncodedData();
	
	format = newFormat;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoded Data Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Sound:: setEncodedDataSize( Size newEncodedDataSize )
{
	if ( newEncodedDataSize == 0 )
		return false;
	
	// Deallocate any previous data.
	if ( encodedData )
		util::deallocate( encodedData );
	
	// Allocate new data.
	encodedData = util::allocate<UByte>( newEncodedDataSize );
	encodedDataSize = newEncodedDataSize;
	
	return true;
}




Bool Sound:: setEncodedData( const UByte* newEncodedData, Size newEncodedDataSize )
{
	if ( newEncodedData == NULL || newEncodedDataSize == 0 )
		return false;
	
	// Deallocate any previous data.
	if ( encodedData )
		util::deallocate( encodedData );
	
	// Allocate new data.
	encodedData = util::allocate<UByte>( newEncodedDataSize );
	encodedDataSize = newEncodedDataSize;
	
	// Copy the data.
	util::copyPOD( encodedData, newEncodedData, newEncodedDataSize );
	
	return true;
}




Bool Sound:: setEncodedData( const UByte* newEncodedData, Size newEncodedDataSize, const SoundFormat& newFormat )
{
	if ( newEncodedData == NULL || newEncodedDataSize == 0 || newFormat == SoundFormat::UNDEFINED )
		return false;
	
	// Deallocate any previous data.
	if ( encodedData )
		util::deallocate( encodedData );
	
	// Allocate new data.
	encodedData = util::allocate<UByte>( newEncodedDataSize );
	encodedDataSize = newEncodedDataSize;
	format = newFormat;
	
	// Copy the data.
	util::copyPOD( encodedData, newEncodedData, newEncodedDataSize );
	
	return true;
}




void Sound:: clearEncodedData()
{
	if ( encodedData )
	{
		util::deallocate( encodedData );
		encodedDataSize = 0;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Read Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Sound:: readSamples( SoundBuffer& outputBuffer, Size numSamples )
{
	if ( buffer.isSet() )
	{
		// Make sure the output buffer has the correct sample rate and channel layout.
		outputBuffer.setSampleRate( buffer->getSampleRate() );
		outputBuffer.setLayout( buffer->getLayout() );
		
		Size numChannels = buffer->getChannelCount();
		Size numSamplesToRead = math::min( numSamples, buffer->getSize() - bufferPosition );
		
		// Copy the samples from the buffer into the output buffer.
		for ( Index c = 0; c < numChannels; c++ )
			util::copyPOD( outputBuffer.getChannel(c), buffer->getChannel(c) + bufferPosition, numSamplesToRead );
		
		// Update the current buffer position.
		bufferPosition += numSamplesToRead;
		return numSamplesToRead;
	}
	else if ( stream.isSet() )
	{
		// Seek in the stream if necessary to reach the correct position.
		SampleIndex oldStreamPosition = stream->getPosition();
		
		if ( oldStreamPosition != streamPosition && stream->canSeek() )
			stream->seek( (Int64)streamPosition - (Int64)oldStreamPosition );
		
		// Read from the stream.
		numSamples = stream->read( outputBuffer, numSamples );
		
		// Update the current stream position.
		streamPosition += numSamples;
		return numSamples;
	}
	else
		return 0;
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
