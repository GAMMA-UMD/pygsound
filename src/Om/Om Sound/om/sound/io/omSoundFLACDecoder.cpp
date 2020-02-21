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

#include "omSoundFLACDecoder.h"


//#include "FLAC/stream_decoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		FLAC Decoder Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class FLACDecoder:: FLACDecoderWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE FLACDecoderWrapper( DataInputStream* newStream )
				:	stream( newStream ),
					currentStreamPosition( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to the data input stream that is being used to read ogg data.
			DataInputStream* stream;
			
			
			/// The current reading position in bytes within the data stream, relative to the starting position.
			LargeIndex currentStreamPosition;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




FLACDecoder:: FLACDecoder( DataInputStream* oggStream )
	:	wrapper( om::util::construct<FLACDecoderWrapper>( oggStream ) ),
		channelLayout(),
		sampleRate( 0 ),
		bitRate( 0 ),
		minBitRate( 0 ),
		maxBitRate( 0 ),
		lengthInSamples( 0 ),
		lengthInSeconds( 0 ),
		currentSampleIndex( 0 ),
		validFile( false )
{
	openFile();
}




FLACDecoder:: FLACDecoder( const FLACDecoder& other )
{
	// Empty because it is declared private and shouldn't be called.
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




FLACDecoder:: ~FLACDecoder()
{
	// Clean up the FLAC decoder.
	
	
	// Destroy the wrapper object.
	om::util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




FLACDecoder& FLACDecoder:: operator = ( const FLACDecoder& other )
{
	// Empty because it is declared private and shouldn't be called.
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek Methods
//############		
//##########################################################################################
//##########################################################################################




Bool FLACDecoder:: canSeek() const
{
	decodingMutex.lock();
	
	Bool seekingAllowed = wrapper->stream != NULL && wrapper->stream->canSeek();
	
	decodingMutex.unlock();
	
	return seekingAllowed;
}




Bool FLACDecoder:: canSeek( Int64 relativeSampleOffset ) const
{
	decodingMutex.lock();
	
	// If there is no stream or if it can't seek, return that we didn't seek any.
	if ( wrapper->stream == NULL || !wrapper->stream->canSeek() )
	{
		decodingMutex.unlock();
		return 0;
	}
	
	Int64 destination = currentSampleIndex + relativeSampleOffset;
	
	if ( destination > (Int64)lengthInSamples )
		return false;
	else if ( destination < 0 )
		return false;
	
	decodingMutex.unlock();
	
	return true;
}




Int64 FLACDecoder:: seek( Int64 relativeSampleOffset )
{
	decodingMutex.lock();
	
	// If there is no stream or if it can't seek, return that we didn't seek any.
	if ( wrapper->stream == NULL || !wrapper->stream->canSeek() )
	{
		decodingMutex.unlock();
		return 0;
	}
	
	Int64 destination = currentSampleIndex + relativeSampleOffset;
	
	if ( destination > (Int64)lengthInSamples )
		destination = lengthInSamples;
	else if ( destination < 0 )
		destination = 0;
	
	// Seek in the FLAC file.
	
	
	
	
	Int64 actualSampleSeek = destination - currentSampleIndex;
	currentSampleIndex = destination;
	
	decodingMutex.unlock();
	
	return actualSampleSeek;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Size Accessor Method
//############		
//##########################################################################################
//##########################################################################################




SoundSize FLACDecoder:: getSamplesRemaining() const
{
	if ( !validFile )
		return SoundSize(0);
	else
		return SoundSize(lengthInSamples - currentSampleIndex);
}




SampleIndex FLACDecoder:: getPosition() const
{
	if ( !validFile )
		return SampleIndex(0);
	else
		return currentSampleIndex;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoder Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size FLACDecoder:: getChannelCount() const
{
	return channelLayout.getChannelCount();
}




SampleRate FLACDecoder:: getSampleRate() const
{
	return sampleRate;
}




SampleType FLACDecoder:: getNativeSampleType() const
{
	return SampleType::SAMPLE_32F;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoder Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool FLACDecoder:: isValid() const
{
	return validFile;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Reading Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult FLACDecoder:: readSamples( SoundBuffer& inputBuffer, Size numSamples )
{
	// Acquire the mutex which synchronizes decoding with other class access.
	decodingMutex.lock();
	
	// If the file was not able to be opened or the end of the file has been reached,
	// return indicating that no samples were read.
	if ( !validFile || wrapper->stream == NULL )
	{
		decodingMutex.unlock();
		return SoundResult::ERROR;
	}
	
	// Make sure that the sample output buffer has the correct sample rate and channel layout.
	inputBuffer.setSampleRate( sampleRate );
	inputBuffer.setLayout( channelLayout );
	const Size numChannels = channelLayout.getChannelCount();
	
	Size numSamplesRemaining = numSamples;
	Size numSamplesRead = 0;
	/*
	while ( numSamplesRemaining > 0 )
	{
		long result = ov_read_float( &wrapper->vf, &readBuffer, (int)numSamplesRemaining, &bitstreamNumber );
		
		// Have we reached the end of the file? If so, stop requesting samples.
		if ( result == 0 )
			break;
		
		// Copy the decoded samples to the output buffer.
		for ( Index c = 0; c < numChannels; c++ )
		{
			const float* input = readBuffer[c];
			Sample32f* output = inputBuffer.getChannel(c) + numSamplesRead;
			const Sample32f* const outputEnd = output + result;
			
			while ( output != outputEnd )
			{
				*output = *input;
				input++;
				output++;
			}
		}
		
		numSamplesRead += result;
		numSamplesRemaining -= result;
	}
	
	currentSampleIndex += numSamplesRead;
	*/
	decodingMutex.unlock();
	
	return numSamplesRead;
}




//##########################################################################################
//##########################################################################################
//############		
//############		WAVE Header Read Method
//############		
//##########################################################################################
//##########################################################################################




void FLACDecoder:: openFile()
{
	//*******************************************************************************
	// Define the callbacks for reading from memory
	
	
	
	
	
	
	
	validFile = true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
