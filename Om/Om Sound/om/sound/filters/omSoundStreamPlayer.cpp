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

#include "omSoundStreamPlayer.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String StreamPlayer:: NAME( "Stream Player" );
const UTF8String StreamPlayer:: MANUFACTURER( "Om Sound" );
const FilterVersion StreamPlayer:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




StreamPlayer:: StreamPlayer()
	:	SoundFilter( 0, 1 ),
		stream(),
		currentStreamPosition( 0 ),
		currentStreamLength( 0 ),
		playingEnabled( false ),
		loopingEnabled( false ),
		seekingAllowed( false )
{
}




StreamPlayer:: StreamPlayer( const Shared<SoundInputStream>& newStream )
	:	SoundFilter( 0, 1 ),
		stream( newStream ),
		currentStreamPosition( 0 ),
		currentStreamLength( 0 ),
		playingEnabled( false ),
		loopingEnabled( false ),
		seekingAllowed( false )
{
	if ( !stream.isNull() )
		seekingAllowed = stream->canSeek();
}




StreamPlayer:: StreamPlayer( const StreamPlayer& other )
	:	SoundFilter( 0, 1 ),
		stream( other.stream ),
		currentStreamPosition( other.currentStreamPosition ),
		currentStreamLength( other.currentStreamLength ),
		playingEnabled( other.playingEnabled ),
		loopingEnabled( other.loopingEnabled ),
		seekingAllowed( other.seekingAllowed )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




StreamPlayer:: ~StreamPlayer()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




StreamPlayer& StreamPlayer:: operator = ( const StreamPlayer& other )
{
	if ( this != &other )
	{
		// Copy the stream, now both classes reference it (hopefully one won't use it).
		stream = other.stream;
		currentStreamPosition = other.currentStreamPosition;
		currentStreamLength = other.currentStreamLength;
		playingEnabled = other.playingEnabled;
		loopingEnabled = other.loopingEnabled;
		seekingAllowed = other.seekingAllowed;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String StreamPlayer:: getName() const
{
	return NAME;
}




UTF8String StreamPlayer:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion StreamPlayer:: getVersion() const
{
	return VERSION;
}




FilterCategory StreamPlayer:: getCategory() const
{
	return FilterCategory::PLAYBACK;
}




Bool StreamPlayer:: allowsInPlaceProcessing() const
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




const SoundInputStream* StreamPlayer:: getStream() const
{
	lockMutex();
	
	const SoundInputStream* pointer = stream;
	
	unlockMutex();
	
	return pointer;
}




void StreamPlayer:: setStream( const Shared<SoundInputStream>& newStream )
{
	lockMutex();
	
	stream = newStream;
	currentStreamPosition = 0;
	currentStreamLength = 0;
	seekingAllowed = false;
	
	if ( !stream.isNull() )
		seekingAllowed = stream->canSeek();
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Playback Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool StreamPlayer:: isPlaying() const
{
	lockMutex();
	
	Bool playing = playingEnabled;
	
	unlockMutex();
	
	return playing;
}




Bool StreamPlayer:: setIsPlaying( Bool newIsPlaying )
{
	lockMutex();
	
	playingEnabled = newIsPlaying;
	Bool playbackIsValid = newIsPlaying && !stream.isNull() && stream->hasSamplesRemaining();
	
	unlockMutex();
	
	return playbackIsValid;
}




Bool StreamPlayer:: play()
{
	return this->setIsPlaying( true );
}




void StreamPlayer:: stop()
{
	this->setIsPlaying( false );
}




Bool StreamPlayer:: rewind()
{
	lockMutex();
	
	Bool didRewind;
	
	if ( !stream.isNull() && seekingAllowed )
	{
		// Seek to the beginning of the stream.
		Int64 seekOffset = -Int64(currentStreamPosition);
		Int64 offset = stream->seek( seekOffset );
		
		didRewind = offset == seekOffset;
		
		currentStreamPosition += offset;
	}
	else
		didRewind = false;
	
	unlockMutex();
	
	return didRewind;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Looping Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool StreamPlayer:: isLooping() const
{
	lockMutex();
	
	Bool looping = loopingEnabled;
	
	unlockMutex();
	
	return looping;
}




Bool StreamPlayer:: setIsLooping( Bool newIsLooping )
{
	lockMutex();
	
	loopingEnabled = newIsLooping;
	
	Bool willLoop = loopingEnabled && seekingAllowed;
	
	unlockMutex();
	
	return willLoop;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool StreamPlayer:: canSeek() const
{
	return false;
}




Bool StreamPlayer:: canSeek( Int64 relativeSampleOffset ) const
{
	return false;
}




Int64 StreamPlayer:: seek( Int64 relativeSampleOffset )
{
	return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Size Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize StreamPlayer:: getSamplesRemaining() const
{
	if ( stream->hasSamplesRemaining() )
		return stream->getSamplesRemaining();
	else if ( loopingEnabled && seekingAllowed )
		return currentStreamPosition;
	else
		return 0;
}




SampleIndex StreamPlayer:: getPosition() const
{
	return currentStreamPosition;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size StreamPlayer:: getChannelCount() const
{
	return stream->getChannelCount();
}




SampleRate StreamPlayer:: getSampleRate() const
{
	return stream->getSampleRate();
}




SampleType StreamPlayer:: getNativeSampleType() const
{
	return SampleType::SAMPLE_32F;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool StreamPlayer:: isValid() const
{
	return stream.isSet();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Stream Playback Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult StreamPlayer:: processFrame( const SoundFrame& inputFrame,
												SoundFrame& outputFrame, Size numSamples )
{
	if ( outputFrame.getBufferCount() == 0 )
		return SoundResult::ERROR;
	
	SoundBuffer* outputBuffer = outputFrame.getBuffer(0);
	
	// Check to make sure that the output buffer has the correct size.
	if ( outputBuffer->getSize() < numSamples )
		outputBuffer->setSize( numSamples );
	
	// Don't produce any sound and return there was an error if the output buffer or stream is NULL.
	if ( stream.isNull() || outputBuffer == NULL )
		return SoundResult::ERROR;
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*****************************************************************************
	
	// Indicate that the end of playback has been reached if we are no longer playing.
	if ( !playingEnabled )
		return SoundResult::END;
	
	// Make sure that the stream has the correct position (in case anyone else is reading from it).
	if ( stream->getPosition() != currentStreamPosition && stream->canSeek() )
	{
		// Seek to the current stream position.
		stream->seek( (Int64)currentStreamPosition - (Int64)stream->getPosition() );
	}
	
	
	if ( stream->hasSamplesRemaining() )
	{
		// We are not yet at the end of the stream.
		// Read the requested number of samples from the stream.
		// This method automatically sets the format of the output buffer.
		Size numSamplesRead = stream->read( *outputBuffer, numSamples );
		
		// Increment the current read position.
		currentStreamPosition += numSamplesRead;
		
		// Update the incrementally computed stream length.
		currentStreamLength = math::max( currentStreamPosition, currentStreamLength );
		
		return numSamplesRead;
	}
	else if ( loopingEnabled && seekingAllowed )
	{
		// We are at the end of the sound and looping is enabled.
		
		// Seek to the begining of the known stream.
		Int64 seekOffset = -Int64(currentStreamPosition);
		Int64 offset = stream->seek( seekOffset );
		currentStreamPosition += offset;
		
		// Read the requested number of samples from the stream.
		// This method automatically sets the format of the output buffer.
		Size numSamplesRead = stream->read( *outputBuffer, numSamples );
		
		// Increment the current read position.
		currentStreamPosition += numSamplesRead;
		
		// Update the incrementally computed stream length.
		currentStreamLength = math::max( currentStreamPosition, currentStreamLength );
		
		return numSamplesRead;
	}
	else
	{
		// No sound is remaining and the sound is not looping.
		// Stop playback and return that 0 samples were produced and the end was reached.
		playingEnabled = false;
		
		return SoundResult::END;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		SoundInputStream::readSamples() Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult StreamPlayer:: readSamples( SoundBuffer& outputBuffer, Size numSamples )
{
	return SoundFilter::read( outputBuffer, numSamples );
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
