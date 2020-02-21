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

#include "omSoundStreamRecorder.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String StreamRecorder:: NAME( "Stream Recorder" );
const UTF8String StreamRecorder:: MANUFACTURER( "Om Sound" );
const FilterVersion StreamRecorder:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




StreamRecorder:: StreamRecorder()
	:	SoundFilter( 1, 0 ),
		stream( NULL ),
		currentStreamPosition( 0 ),
		currentStreamLength( 0 ),
		recordingEnabled( false ),
		seekingAllowed( false )
{
}




StreamRecorder:: StreamRecorder( SoundOutputStream* newStream )
	:	SoundFilter( 1, 0 ),
		stream( newStream ),
		currentStreamPosition( 0 ),
		currentStreamLength( 0 ),
		recordingEnabled( false ),
		seekingAllowed( false )
{
	if ( stream )
		seekingAllowed = stream->canSeek();
}




StreamRecorder:: StreamRecorder( const StreamRecorder& other )
	:	SoundFilter( 1, 0 ),
		stream( other.stream ),
		currentStreamPosition( other.currentStreamPosition ),
		currentStreamLength( other.currentStreamLength ),
		recordingEnabled( other.recordingEnabled ),
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




StreamRecorder:: ~StreamRecorder()
{
	if ( stream )
		stream->flush();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




StreamRecorder& StreamRecorder:: operator = ( const StreamRecorder& other )
{
	if ( this != &other )
	{
		// Copy the stream, now both classes reference it (hopefully one won't use it).
		stream = other.stream;
		currentStreamPosition = other.currentStreamPosition;
		currentStreamLength = other.currentStreamLength;
		recordingEnabled = other.recordingEnabled;
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




UTF8String StreamRecorder:: getName() const
{
	return NAME;
}




UTF8String StreamRecorder:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion StreamRecorder:: getVersion() const
{
	return VERSION;
}




FilterCategory StreamRecorder:: getCategory() const
{
	return FilterCategory::RECORDING;
}




Bool StreamRecorder:: allowsInPlaceProcessing() const
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





const SoundOutputStream* StreamRecorder:: getStream() const
{
	lockMutex();
	
	const SoundOutputStream* pointer = stream;
	
	unlockMutex();
	
	return pointer;
}




void StreamRecorder:: setStream( SoundOutputStream* newStream )
{
	lockMutex();
	
	stream = newStream;
	currentStreamPosition = 0;
	currentStreamLength = 0;
	seekingAllowed = false;
	
	if ( stream )
		seekingAllowed = stream->canSeek();
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Recording Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool StreamRecorder:: isRecording() const
{
	lockMutex();
	
	Bool recording = recordingEnabled;
	
	unlockMutex();
	
	return recording;
}




Bool StreamRecorder:: setIsRecording( Bool newIsRecording )
{
	lockMutex();
	
	recordingEnabled = newIsRecording;
	Bool recordingIsValid = newIsRecording && stream != NULL;
	
	unlockMutex();
	
	return recordingIsValid;
}




Bool StreamRecorder:: record()
{
	return this->setIsRecording( true );
}




void StreamRecorder:: stop()
{
	this->setIsRecording( false );
}




Bool StreamRecorder:: rewind()
{
	lockMutex();
	
	Bool didRewind;
	
	if ( stream )
	{
		// Seek to the beginning of the stream.
		Int64 seekOffset = -Int64(currentStreamPosition);
		Int64 offset = stream->seek( seekOffset );
		
		didRewind = seekingAllowed && offset == seekOffset;
		
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
//############		Main Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




SoundResult StreamRecorder:: processFrame( const SoundFrame& inputFrame,
												SoundFrame& outputFrame, Size numSamples )
{
	// If the input frame has no input buffers, return that there was an error.
	if ( inputFrame.getBufferCount() == 0 )
		return SoundResult::ERROR;
	
	// If the input buffer is NULL, return that there was an error.
	const SoundBuffer* inputBuffer = inputFrame.getBuffer(0);
	
	if ( inputBuffer == NULL )
		return SoundResult::ERROR;
	
	// If recording is not enabled, return that 0 samples were processed.
	if ( !recordingEnabled )
		return 0;
	
	// Write the data to the sound output stream.
	stream->write( *inputBuffer, numSamples );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*******************************************************************************
	
	// If the output frame has any buffers, pass through the input to output.
	if ( outputFrame.getBufferCount() > 0 )
	{
		SoundBuffer* outputBuffer = outputFrame.getBuffer(0);
		
		if ( outputBuffer != NULL )
		{
			inputBuffer->copyFormatTo( *outputBuffer, numSamples );
			inputBuffer->copyTo( *outputBuffer, numSamples );
		}
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
