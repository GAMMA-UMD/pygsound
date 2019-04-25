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

#include "omSoundThreadedStreamRecorder.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String ThreadedStreamRecorder:: NAME( "Threaded Stream Recorder" );
const UTF8String ThreadedStreamRecorder:: MANUFACTURER( "Om Sound" );
const FilterVersion ThreadedStreamRecorder:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ThreadedStreamRecorder:: ThreadedStreamRecorder()
	:	SoundFilter( 1, 0 ),
		stream(),
		currentStreamPosition( 0 ),
		currentStreamLength( 0 ),
		recordingEnabled( false ),
		seekingAllowed( false )
{
}




ThreadedStreamRecorder:: ThreadedStreamRecorder( const Shared<SoundOutputStream>& newStream )
	:	SoundFilter( 1, 0 ),
		stream( newStream ),
		currentStreamPosition( 0 ),
		currentStreamLength( 0 ),
		recordingEnabled( false ),
		seekingAllowed( false )
{
	if ( !stream.isNull() )
		seekingAllowed = stream->canSeek();
}




ThreadedStreamRecorder:: ThreadedStreamRecorder( const ThreadedStreamRecorder& other )
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




ThreadedStreamRecorder:: ~ThreadedStreamRecorder()
{
	if ( this->isRecording() )
		this->stop();
	
	if ( !stream.isNull() )
		stream->flush();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ThreadedStreamRecorder& ThreadedStreamRecorder:: operator = ( const ThreadedStreamRecorder& other )
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




UTF8String ThreadedStreamRecorder:: getName() const
{
	return NAME;
}




UTF8String ThreadedStreamRecorder:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion ThreadedStreamRecorder:: getVersion() const
{
	return VERSION;
}




FilterCategory ThreadedStreamRecorder:: getCategory() const
{
	return FilterCategory::RECORDING;
}




Bool ThreadedStreamRecorder:: allowsInPlaceProcessing() const
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





const SoundOutputStream* ThreadedStreamRecorder:: getStream() const
{
	lockMutex();
	
	const SoundOutputStream* pointer = stream;
	
	unlockMutex();
	
	return pointer;
}




void ThreadedStreamRecorder:: setStream( const Shared<SoundOutputStream>& newStream, Index inputIndex )
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
//############		Recording Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ThreadedStreamRecorder:: isRecording() const
{
	lockMutex();
	
	Bool recording = recordingEnabled;
	
	unlockMutex();
	
	return recording;
}




Bool ThreadedStreamRecorder:: setIsRecording( Bool newIsRecording )
{
	lockMutex();
	
	Bool recordingIsValid;
	
	if ( newIsRecording )
	{
		if ( !recordingEnabled )
		{
			recordingEnabled = recordingIsValid = newIsRecording && !stream.isNull();
			
			// Start the recording thread if everything else is ok.
			if ( recordingIsValid )
				recordingThread.start( Function<void()>( &ThreadedStreamRecorder::recordingThreadEntry, this ) );
		}
		
		unlockMutex();
	}
	else
	{
		recordingEnabled = recordingIsValid = false;
		unlockMutex();
		
		// Join the recording thread.
		recordingThread.join();
	}
	
	return recordingIsValid;
}




Bool ThreadedStreamRecorder:: record()
{
	return this->setIsRecording( true );
}




void ThreadedStreamRecorder:: stop()
{
	this->setIsRecording( false );
}




Bool ThreadedStreamRecorder:: rewind()
{
	lockMutex();
	
	Bool didRewind;
	
	if ( !stream.isNull() )
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




SoundResult ThreadedStreamRecorder:: processFrame( const SoundFrame& inputFrame,
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
	
	//queue.addBuffer( *inputBuffer, numSamples );
	
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




void ThreadedStreamRecorder:: recordingThreadEntry()
{
	while ( 1 )
	{
		lockMutex();
		
		Bool hasNext = false;//queue.hasNextBuffer();
		
		// Should we stop recording?
		if ( !recordingEnabled && !hasNext )
		{
			unlockMutex();
			return;
		}
		/*
		// Write the next buffer if there is anything available.
		if ( queue.getBufferCount() > 0 )
		{
			const SoundBuffer& buffer = queue.getNextBuffer();
			stream->write( buffer, buffer.getSize() );
			queue.removeBuffer();
			
			unlockMutex();
		}
		else*/
		{
			unlockMutex();
			
			// Sleep for ten milliseconds to wait for another buffer.
			Thread::sleepMs(10);
		}
	}
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
