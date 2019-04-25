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

#include "omSoundOggDecoder.h"


#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Ogg Decoder Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class OggDecoder:: OggDecoderWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE OggDecoderWrapper( DataInputStream* newStream )
				:	stream( newStream ),
					currentStreamPosition( 0 )
			{
				// Reference the ogg callbacks here so that we can avoid some compiler warnings
				// that they are unreferenced.
				OV_CALLBACKS_DEFAULT;
				OV_CALLBACKS_NOCLOSE;
				OV_CALLBACKS_STREAMONLY;
				OV_CALLBACKS_STREAMONLY_NOCLOSE;
			}
			
			
		//********************************************************************************
		//******	Ogg File I/O Callback Methods
			
			
			/// The function used by the ogg decoder whenever reading data from a custom source.
			static size_t readFunction( void* ptr, size_t size, size_t nmemb, void* dataSource );
			
			
			/// The function used by the ogg decoder whenever it wants to seek in the custom data source.
			static int seekFunction( void* dataSource, ogg_int64_t offset, int whence );
			
			
			/// The function to close a stream used by the ogg decoder whenever it is done reading from a file.
			static int closeFunction( void* dataSource );
			
			
			/// A function that tells the ogg decoder where the decoder is in the input file.
			static long tellFunction( void* dataSource );
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to the data input stream that is being used to read ogg data.
			DataInputStream* stream;
			
			
			/// The current reading position in bytes within the data stream, relative to the starting position.
			LargeIndex currentStreamPosition;
			
			
			/// The structure used by the libogg library to decode the ogg file.
			OggVorbis_File vf;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		VorbisFile Read Callback Functions
//############		
//##########################################################################################
//##########################################################################################




size_t OggDecoder::OggDecoderWrapper:: readFunction( void* ptr, size_t size, size_t nmemb, void* dataSource )
{
	OggDecoderWrapper* wrapper = (OggDecoderWrapper*)dataSource;
	
	// Check to see if the stream is valid and hasn't reached the end of file.
	// If so, return that 0 bytes were read.
	if ( wrapper == NULL || wrapper->stream == NULL || !wrapper->stream->hasBytesRemaining() )
		return 0;
	
	// Compute the total number of bytes that are requested.
	Size numBytesToRead = nmemb*size;
	
	// Read that number of bytes from the data stream.
	Size numBytesRead = wrapper->stream->readData( (UByte*)ptr, numBytesToRead );
	
	// Update the current position within the stream.
	wrapper->currentStreamPosition += numBytesRead;
	
	return numBytesRead / size;
}




int OggDecoder::OggDecoderWrapper:: seekFunction( void* dataSource, ogg_int64_t offset, int whence )
{
	OggDecoderWrapper* wrapper = (OggDecoderWrapper*)dataSource;
	
	// Return failure if the stream is invalid or if it can't seek.
	if ( wrapper == NULL || wrapper->stream == NULL || !wrapper->stream->canSeek() )
		return -1;
	
	if ( whence == SEEK_SET )
	{
		Int64 relativeSeek = (Int64)offset - (Int64)wrapper->currentStreamPosition;
		
		wrapper->currentStreamPosition += wrapper->stream->seek( relativeSeek );
	}
	else if ( whence == SEEK_CUR )
	{
		Int64 relativeSeek = (Int64)offset;
		
		wrapper->currentStreamPosition += wrapper->stream->seek( relativeSeek );
	}
	else if ( whence == SEEK_END )
	{
		Int64 numRemaining = (Int64)wrapper->stream->getBytesRemaining();
		Int64 relativeSeek = numRemaining + offset;
		
		wrapper->currentStreamPosition += wrapper->stream->seek( relativeSeek );
	}
	
	// Return success;
	return 0;
}



int OggDecoder::OggDecoderWrapper:: closeFunction( void* dataSource )
{
	return 0;
}




long OggDecoder::OggDecoderWrapper:: tellFunction( void* dataSource )
{
	OggDecoderWrapper* wrapper = (OggDecoderWrapper*)dataSource;
	
	// Return failure if the stream is invalid or if it can't seek.
	if ( wrapper == NULL || wrapper->stream == NULL )
		return 0;
	
	return (long)wrapper->currentStreamPosition;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




OggDecoder:: OggDecoder( DataInputStream* oggStream )
	:	wrapper( om::util::construct<OggDecoderWrapper>( oggStream ) ),
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




OggDecoder:: OggDecoder( const OggDecoder& other )
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




OggDecoder:: ~OggDecoder()
{
	// Clean up the ogg stuff.
	if ( validFile )
		ov_clear( &wrapper->vf );
	
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




OggDecoder& OggDecoder:: operator = ( const OggDecoder& other )
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




Bool OggDecoder:: canSeek() const
{
	decodingMutex.lock();
	
	Bool seekingAllowed = wrapper->stream != NULL && wrapper->stream->canSeek();
	
	decodingMutex.unlock();
	
	return seekingAllowed;
}




Bool OggDecoder:: canSeek( Int64 relativeSampleOffset ) const
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




Int64 OggDecoder:: seek( Int64 relativeSampleOffset )
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
	
	int result = ov_pcm_seek( &wrapper->vf, destination );
	
	// Check to see if there was an error. If so, return that the position didn't move.
	if ( result != 0 )
		return 0;
	
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




SoundSize OggDecoder:: getSamplesRemaining() const
{
	if ( !validFile )
		return SoundSize(0);
	else
		return SoundSize(lengthInSamples - currentSampleIndex);
}




SampleIndex OggDecoder:: getPosition() const
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




Size OggDecoder:: getChannelCount() const
{
	return channelLayout.getChannelCount();
}




SampleRate OggDecoder:: getSampleRate() const
{
	return sampleRate;
}




SampleType OggDecoder:: getNativeSampleType() const
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




Bool OggDecoder:: isValid() const
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




SoundResult OggDecoder:: readSamples( SoundBuffer& inputBuffer, Size numSamples )
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
	
	int bitstreamNumber;
	float** readBuffer;
	Size numSamplesRemaining = numSamples;
	Size numSamplesRead = 0;
	
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




void OggDecoder:: openFile()
{
	//*******************************************************************************
	// Define the callbacks for reading from memory
	
	ov_callbacks callbacks;
	
	callbacks.read_func = OggDecoderWrapper::readFunction;
	callbacks.seek_func = OggDecoderWrapper::seekFunction;
	callbacks.close_func = OggDecoderWrapper::closeFunction;
	callbacks.tell_func = OggDecoderWrapper::tellFunction;
	
	// open the vorbis-file
	int result = ov_open_callbacks( wrapper, &wrapper->vf, NULL, 0, callbacks );
	
	if ( result < 0 )
	{
		validFile = false;
		return;
	}
	
	// Get some information about the Ogg Vorbis file and cache it.
	lengthInSamples = ov_pcm_total( &wrapper->vf, -1 );
	lengthInSeconds = ov_time_total( &wrapper->vf, -1 );
	
	vorbis_info* info = ov_info( &wrapper->vf, -1 );
	
	channelLayout = ChannelLayout( (Size)info->channels );
	channelLayout.setType( ChannelLayoutType( (Size)info->channels ) );
	sampleRate = info->rate;
	bitRate = (Float)info->bitrate_nominal / Float(1000);
	minBitRate = (Float)info->bitrate_lower / Float(1000);
	maxBitRate = (Float)info->bitrate_upper / Float(1000);
	
	validFile = true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
