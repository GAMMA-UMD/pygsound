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

#include "omSoundAIFFEncoder.h"

#define AIFC_VERSION_1 0xA2805140

#define CHUNK_ID_SIZE 4
#define COMMON_CHUNK_DATA_SIZE 24
#define FP80_SIZE 10

#define NUM_CHANNELS_OFFSET (CHUNK_ID_SIZE + sizeof(Int32))
#define NUM_SAMPLES_OFFSET (NUM_CHANNELS_OFFSET + sizeof(Int16))
#define SAMPLE_SIZE_OFFSET (NUM_SAMPLES_OFFSET + sizeof(UInt32))
#define SAMPLE_RATE_OFFSET (SAMPLE_SIZE_OFFSET + sizeof(Int16))
#define COMPRESSION_TYPE_OFFSET (SAMPLE_RATE_OFFSET + FP80_SIZE)

#define OFFSET_OFFSET (CHUNK_ID_SIZE + sizeof(Int32))
#define BLOCK_SIZE_OFFSET (OFFSET_OFFSET + sizeof(UInt32))


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




OM_INLINE static void setID( UByte* bytes, const Char* id )
{
	bytes[0] = id[0];
	bytes[1] = id[1];
	bytes[2] = id[2];
	bytes[3] = id[3];
}




OM_INLINE static void setInt16( UByte* bytes, Int16 i )
{
	bytes[0] = (i >> 8) & 0xFF;
	bytes[1] = i & 0xFF;
}




OM_INLINE static void setInt24( UByte* bytes, Int32 i )
{
	bytes[0] = (i >> 16) & 0xFF;
	bytes[1] = (i >> 8) & 0xFF;
	bytes[2] = i & 0xFF;
}




OM_INLINE static void setInt32( UByte* bytes, Int32 i )
{
	bytes[0] = (i >> 24) & 0xFF;
	bytes[1] = (i >> 16) & 0xFF;
	bytes[2] = (i >> 8) & 0xFF;
	bytes[3] = i & 0xFF;
}




OM_INLINE static void setUInt32( UByte* bytes, UInt32 i )
{
	bytes[0] = (i >> 24) & 0xFF;
	bytes[1] = (i >> 16) & 0xFF;
	bytes[2] = (i >> 8) & 0xFF;
	bytes[3] = i & 0xFF;
}




OM_INLINE static void setInt64( UByte* bytes, Int64 i )
{
	bytes[0] = UByte((i >> 56) & 0xFF);
	bytes[1] = UByte((i >> 48) & 0xFF);
	bytes[2] = UByte((i >> 40) & 0xFF);
	bytes[3] = UByte((i >> 32) & 0xFF);
	bytes[4] = UByte((i >> 24) & 0xFF);
	bytes[5] = UByte((i >> 16) & 0xFF);
	bytes[6] = UByte((i >> 8) & 0xFF);
	bytes[7] = UByte(i & 0xFF);
}




OM_INLINE static void setUInt64( UByte* bytes, UInt64 i )
{
	bytes[0] = UByte((i >> 56) & 0xFF);
	bytes[1] = UByte((i >> 48) & 0xFF);
	bytes[2] = UByte((i >> 40) & 0xFF);
	bytes[3] = UByte((i >> 32) & 0xFF);
	bytes[4] = UByte((i >> 24) & 0xFF);
	bytes[5] = UByte((i >> 16) & 0xFF);
	bytes[6] = UByte((i >> 8) & 0xFF);
	bytes[7] = UByte(i & 0xFF);
}




OM_INLINE static void setFloat32( UByte* bytes, Float32 f )
{
	// Not sure why this is the case, should be big endian.
	Float32 bigEndian = f; //data::endian::toBigEndian(f);
	setUInt32( bytes, *(UInt32*)&bigEndian );
}




OM_INLINE static void setFloat64( UByte* bytes, Float64 f )
{
	// Not sure why this is the case, should be big endian.
	Float64 bigEndian = f; //data::endian::toBigEndian(f);
	setUInt64( bytes, *(UInt64*)&bigEndian );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




AIFFEncoder:: AIFFEncoder( DataOutputStream* outputStream, Size newNumChannels,
							SampleType newSampleType )
	:	numChannels( math::max( newNumChannels, Size(1) ) ),
		sampleType( newSampleType ),
		sampleRate( 0 ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		currentWritePosition( 0 ),
		formChunkStart( 0 ),
		commonChunkStart( 0 ),
		soundChunkStart( 0 ),
		isAIFC( newSampleType.isFloatingPoint() ),
		valid( sampleTypeIsValid( newSampleType ) ),
		writtenHeader( false ),
		stream( outputStream )
{
	if ( stream == NULL )
		valid = false;
}




AIFFEncoder:: AIFFEncoder( DataOutputStream* outputStream, Size newNumChannels,
							SampleType newSampleType, SampleRate newSampleRate )
	:	numChannels( math::max( newNumChannels, Size(1) ) ),
		sampleType( newSampleType ),
		sampleRate( math::max( newSampleRate, SampleRate(0) ) ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		currentWritePosition( 0 ),
		formChunkStart( 0 ),
		commonChunkStart( 0 ),
		soundChunkStart( 0 ),
		isAIFC( newSampleType.isFloatingPoint() ),
		valid( sampleTypeIsValid( newSampleType ) ),
		writtenHeader( false ),
		stream( outputStream )
{
	if ( stream == NULL )
		valid = false;
}




AIFFEncoder:: AIFFEncoder( const AIFFEncoder& other )
{
	// Empty because it is private and shouldn't be called.
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




AIFFEncoder:: ~AIFFEncoder()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




AIFFEncoder& AIFFEncoder:: operator = ( const AIFFEncoder& other )
{
	// Empty because it is private and shouldn't be called.
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Flush Method
//############		
//##########################################################################################
//##########################################################################################




void AIFFEncoder:: flush()
{
	if ( valid && stream != NULL )
		stream->flush();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seeking Methods
//############		
//##########################################################################################
//##########################################################################################




Bool AIFFEncoder:: canSeek() const
{
	encodingMutex.lock();
	
	Bool seekingAllowed = valid && stream != NULL && stream->canSeek();
	
	encodingMutex.unlock();
	
	return seekingAllowed;
}




Bool AIFFEncoder:: canSeek( Int64 relativeSampleOffset ) const
{
	encodingMutex.lock();
	
	// If there is no stream or if it can't seek, return that the seek is not allowed.
	if ( !valid || stream == NULL || !stream->canSeek() )
	{
		encodingMutex.unlock();
		return 0;
	}
	
	Int64 destination = (Int64)currentSampleIndex + relativeSampleOffset;
	Bool seekAllowed = destination > 0 && (SoundSize)destination < lengthInSamples;
	
	encodingMutex.unlock();
	
	return seekAllowed;
}




Int64 AIFFEncoder:: seek( Int64 relativeSampleOffset )
{
	encodingMutex.lock();
	
	// If there is no stream or if it can't seek, return that we didn't seek any.
	if ( !valid || stream == NULL || !stream->canSeek() )
	{
		encodingMutex.unlock();
		return 0;
	}
	
	// Compute the destination sample index.
	Int64 destination = Int64(currentSampleIndex) + relativeSampleOffset;
	Int64 seekAmount;
	
	// Bounds check the seek amount.
	if ( destination <= 0 )
		seekAmount = -Int64(currentSampleIndex);
	else if ( SoundSize(destination) >= lengthInSamples )
		seekAmount = Int64(lengthInSamples) - Int64(currentSampleIndex);
	else
		seekAmount = relativeSampleOffset;
	
	// Compute the size of a sample frame in bytes.
	Int64 bytesPerSampleFrame = numChannels*sampleType.getSizeInBytes();
	
	// Seek the maximum allowed amount.
	Int64 actualSeek = stream->seek( seekAmount*bytesPerSampleFrame );
	
	// Update the current write position.
	currentWritePosition = (Int64)currentWritePosition + actualSeek;
	
	Int64 actualSampleSeek = actualSeek / bytesPerSampleFrame;
	currentSampleIndex = SampleIndex(Int64(currentSampleIndex) + actualSampleSeek);
	
	encodingMutex.unlock();
	
	return actualSampleSeek;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size AIFFEncoder:: getChannelCount() const
{
	if ( valid )
		return 0;
	else
		return numChannels;
}




SampleRate AIFFEncoder:: getSampleRate() const
{
	if ( valid )
		return SampleRate(0);
	else
		return sampleRate;
}




SampleType AIFFEncoder:: getNativeSampleType() const
{
	if ( valid )
		return SampleType::UNDEFINED;
	else
		return sampleType;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Sample Writing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult AIFFEncoder:: writeSamples( const SoundBuffer& inputBuffer, Size numSamples )
{
	encodingMutex.lock();
	
	// If the file was not able to be opened or the end of the file has been reached,
	// return indicating that no samples were read.
	if ( !valid || stream == NULL )
	{
		encodingMutex.unlock();
		return SoundResult::ERROR;
	}
	
	if ( numSamples == 0 )
	{
		encodingMutex.unlock();
		return 0;
	}
	
	//***************************************************************************
	// Detect the input sample rate if the output sample rate hasn't been initialized.
	
	if ( sampleRate == SampleRate(0) )
		sampleRate = inputBuffer.getSampleRate();
	
	const SoundBuffer* finalInputBuffer = &inputBuffer;
	
	// Get a temporary buffer for use in potential sample rate conversion.
	SharedSoundBuffer tempBuffer = SharedBufferPool::getGlobalBuffer();
	
	// Test to see if the input buffer should be sample rate converted.
	if ( inputBuffer.getSampleRate() != sampleRate )
	{
		resampler.setOutputSampleRate( sampleRate );
		numSamples = resampler.process( inputBuffer, tempBuffer.getBuffer(), numSamples );
		finalInputBuffer = &tempBuffer.getBuffer();
	}
	
	//***************************************************************************
	// Write the aiff file's header if we haven't already.
	
	if ( !writtenHeader )
		this->writeHeader();
	
	//***************************************************************************
	
	// Compute the size of an encoded sample and an encoded sample frame.
	Size bytesPerSample = sampleType.getSizeInBytes();
	Size bytesPerSampleFrame = numChannels*bytesPerSample;
	
	// Declare a stack-based temporary buffer for the output data.
	const Size dataBufferSize = 1024;
	UByte dataBuffer[dataBufferSize];
	
	// Keep track of the number of samples that we have to write in order to write all samples.
	Size numSamplesRemaining = numSamples;
	Size numSamplesWritten = 0;
	
	while ( numSamplesRemaining > 0 )
	{
		// Compute the number of output samples that the static-sized data buffer can hold.
		Size numOutputSamples = math::min( dataBufferSize / bytesPerSampleFrame, numSamplesRemaining );
		Size bytesInBuffer = numOutputSamples*bytesPerSampleFrame;
		
		const Size numInputChannels = finalInputBuffer->getChannelCount();
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			UByte* output = dataBuffer + c*bytesPerSample;
			
			// Check to see if the provided buffer has enough channels.
			if ( c < numInputChannels )
			{
				// Write the output buffer channel at that index.
				const Sample32f* input = finalInputBuffer->getChannel(c) + numSamplesWritten;
				const Sample32f* inputEnd = input + numOutputSamples;
				
				// Figure out which bit depth to use for the aiff sample data.
				switch ( sampleType )
				{
					case SampleType::SAMPLE_8:
						{
							while ( input != inputEnd )
							{
								Int8 sample = Sample8(*input);
								*output = *((UByte*)&sample);
								
								input++;
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_16:
						{
							while ( input != inputEnd )
							{
								Sample16 sample(*input);
								setInt16( output, sample );
								
								input++;
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_24:
						{
							while ( input != inputEnd )
							{
								Int24 sample = Sample24(*input);
								setInt24( output, sample );
								
								input++;
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_32:
						{
							while ( input != inputEnd )
							{
								Sample32 sample(*input);
								setInt32( output, sample );
								
								input++;
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_64:
						{
							while ( input != inputEnd )
							{
								Sample64 sample(*input);
								setInt64( output, sample );
								
								input++;
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_32F:
						{
							while ( input != inputEnd )
							{
								Sample32f sample(*input);
								setFloat32( output, sample );
								
								input++;
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_64F:
						{
							while ( input != inputEnd )
							{
								Sample64f sample(*input);
								setFloat64( output, sample );
								
								input++;
								output += bytesPerSampleFrame;
							}
						}
						break;
				}
			}
			else
			{
				// Write zeros to the aiff file for this channel.
				const UByte* const outputEnd = output + numOutputSamples*bytesPerSampleFrame;
				
				switch ( sampleType )
				{
					case SampleType::SAMPLE_8:
						{
							while ( output != outputEnd )
							{
								*output = 0;
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_16:
						{
							while ( output != outputEnd )
							{
								setInt16( output, 0 );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_24:
						{
							while ( output != outputEnd )
							{
								setInt24( output, 0 );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_32:
						{
							while ( output != outputEnd )
							{
								setInt32( output, Sample32() );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_64:
						{
							while ( output != outputEnd )
							{
								setInt64( output, Sample64() );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_32F:
						{
							while ( output != outputEnd )
							{
								setFloat32( output, 0.0f );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_64F:
						{
							while ( output != outputEnd )
							{
								setFloat64( output, 0.0 );
								output += bytesPerSampleFrame;
							}
						}
						break;
				}
			}
		}
		
		// Write the sample data in the buffer to the data output stream.
		Size numBytesWritten = stream->writeData( dataBuffer, bytesInBuffer );
		
		// Update the current write position in the stream.
		currentWritePosition += numBytesWritten;
		
		// Compute the number of samples actually written.
		numOutputSamples = numBytesWritten / bytesPerSampleFrame;
		
		// Update the encoding loop counters.
		numSamplesRemaining -= numOutputSamples;
		numSamplesWritten += numOutputSamples;
	}
	
	//**********************************************************************************
	// Update the total length of the audio file chunks (if seeking in the stream is enabled).
	
	currentSampleIndex += numSamplesWritten;
	lengthInSamples = math::max( currentSampleIndex, lengthInSamples );
	
	if ( stream->canSeek() )
	{
		// Save the current position for later.
		LargeIndex lastWritePosition = currentWritePosition;
		
		//*************************************************************
		
		// Seek backwards to the location of the sound chunk's data size member.
		Int64 soundChunkOffset = (Int64)soundChunkStart - (Int64)currentWritePosition + CHUNK_ID_SIZE;
		Int64 actualOffset = stream->seek( soundChunkOffset );
		
		// Update the current write position after the seek operation.
		currentWritePosition = (Int64)currentWritePosition + actualOffset;
		
		// Compute the total size of the sound chunk.
		Int32 soundChunkDataSize = Int32(8 + numChannels*lengthInSamples*sampleType.getSizeInBytes());
		
		// Write the size of the sound chunk.
		UByte chunkDataSize[sizeof(Int32)];
		setInt32( chunkDataSize, soundChunkDataSize );
		
		// Write the sound chunk size to the file.
		stream->writeData( chunkDataSize, sizeof(Int32) );
		
		// Update the current write position.
		currentWritePosition += sizeof(Int32);
		
		//*************************************************************
		
		// Seek backwards to the location of the common chunk's num samples data member.
		Int64 commonChunkOffset = (Int64)commonChunkStart - (Int64)currentWritePosition + NUM_SAMPLES_OFFSET;
		actualOffset = stream->seek( commonChunkOffset );
		
		// Update the current write position after the seek operation.
		currentWritePosition = (Int64)currentWritePosition + actualOffset;
		
		// Write the number of samples.
		UByte numSampleFrames[sizeof(UInt32)];
		setUInt32( numSampleFrames, UInt32(lengthInSamples) );
		
		// Write the number of sample frames to the file.
		stream->writeData( numSampleFrames, sizeof(UInt32) );
		
		// Update the current write position.
		currentWritePosition += sizeof(UInt32);
		
		//*************************************************************
		
		// Seek backwards to the location of the form chunk's data size member.
		Int64 formChunkOffset = (Int64)formChunkStart - (Int64)currentWritePosition + CHUNK_ID_SIZE;
		actualOffset = stream->seek( formChunkOffset );
		
		// Update the current write position after the seek operation.
		currentWritePosition = (Int64)currentWritePosition + actualOffset;
		
		// Write the size of the FORM chunk data.
		setUInt32( chunkDataSize, UInt32(lastWritePosition - currentWritePosition + CHUNK_ID_SIZE) );
		
		// Write the number of sample frames to the file.
		stream->writeData( numSampleFrames, sizeof(UInt32) );
		
		// Update the current write position.
		currentWritePosition += sizeof(UInt32);
		
		//*************************************************************
		
		// Seek back to the last location of the encoder.
		Int64 restoreOffset = lastWritePosition - currentWritePosition;
		
		currentWritePosition += stream->seek( restoreOffset );
	}
	
	encodingMutex.unlock();
	
	return numSamplesWritten;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Header Writing Method
//############		
//##########################################################################################
//##########################################################################################




void AIFFEncoder:: writeHeader()
{
	if ( !valid )
		return;
	
	// Initialize the current write position to be 0.
	currentWritePosition = 0;
	
	//*************************************************************************
	// Write the form header for the file.
	
	const Size formHeaderSize = 2*CHUNK_ID_SIZE + sizeof(Int32);
	UByte formHeader[formHeaderSize];
	
	// Set the form header chunk ID.
	setID( formHeader, "FORM" );
	
	// Set the size of the form chunk to 0 initially.
	setInt32( formHeader + CHUNK_ID_SIZE, 0 );
	
	// Set the form type.
	if ( isAIFC )
		setID( formHeader + 8, "AIFC" );
	else
		setID( formHeader + 8, "AIFF" );
	
	// Note the location of the start of the form chunk.
	formChunkStart = currentWritePosition;
	
	// Write the form chunk header to the stream.
	if ( stream->writeData( formHeader, formHeaderSize ) != formHeaderSize )
	{
		valid = false;
		return;
	}
	
	// Increment the current write position.
	currentWritePosition += formHeaderSize;
	
	//*************************************************************************
	// Write the format version chunk to the file for AIFC files.
	
	if ( isAIFC )
	{
		const Size formatVersionChunkSize = CHUNK_ID_SIZE + sizeof(Int32) + sizeof(UInt32);
		UByte formatVersionChunk[formatVersionChunkSize];
		
		// Set the format version chunk ID.
		setID( formatVersionChunk, "FVER" );
		
		// Set the data size for this chunk.
		setInt32( formatVersionChunk + CHUNK_ID_SIZE, sizeof(UInt32) );
		
		// Set the version timestamp for the format.
		setUInt32( formatVersionChunk + CHUNK_ID_SIZE + sizeof(Int32), AIFC_VERSION_1 );
		
		// Write the format version chunk to the stream.
		if ( stream->writeData( formatVersionChunk, formatVersionChunkSize ) != formatVersionChunkSize )
		{
			valid = false;
			return;
		}
		
		// Increment the current write position.
		currentWritePosition += formatVersionChunkSize;
	}
	
	//*************************************************************************
	// Write the common chunk for the file.
	
	// Create an array with the necessary space for either an AIFC or AIFF 'COMM' chunk.
	UByte commonChunk[CHUNK_ID_SIZE + sizeof(Int32) + COMMON_CHUNK_DATA_SIZE];
	
	// Determine the actual size of the common chunk's data.
	const Int32 commonChunkDataSize = isAIFC ? 24 : 18;
	const Size commonChunkSize = CHUNK_ID_SIZE + sizeof(Int32) + commonChunkDataSize;
	
	// Set the common chunk ID.
	setID( commonChunk, "COMM" );
	
	// Set the chunk data size.
	setInt32( commonChunk + CHUNK_ID_SIZE, commonChunkDataSize );
	
	// Set the number of channels.
	setInt16( commonChunk + NUM_CHANNELS_OFFSET, (UInt16)numChannels );
	
	// Set an initial value of 0 for the sound file's length in sample frames.
	setUInt32( commonChunk + NUM_SAMPLES_OFFSET, 0 );
	
	// Set the size of a sample.
	setInt16( commonChunk + SAMPLE_SIZE_OFFSET, (UInt16)(sampleType.getSizeInBytes()*8) );
	
	// Convert the sample rate to an 80-bit floating point number.
	convertFP64ToFP80( sampleRate, commonChunk + SAMPLE_RATE_OFFSET );
	
	// Write the compression type and name if the file should be an AIFC file.
	if ( isAIFC )
	{
		// If the sample type is floating point, we have to use the AIFC format.
		if ( sampleType == SampleType::SAMPLE_32F )
			setID( commonChunk + COMPRESSION_TYPE_OFFSET, "fl32" );
		else if ( sampleType == SampleType::SAMPLE_64F )
			setID( commonChunk + COMPRESSION_TYPE_OFFSET, "fl64" );
		else
			setID( commonChunk + COMPRESSION_TYPE_OFFSET, "NONE" );
		
		// Write an empty string for the compression type name. (numChars byte + 1 pad byte).
		setInt16( commonChunk + COMPRESSION_TYPE_OFFSET + CHUNK_ID_SIZE, 0 );
	}
	
	// Note the location of the start of the COMM chunk.
	commonChunkStart = currentWritePosition;
	
	// Write the common chunk to the stream.
	if ( stream->writeData( commonChunk, commonChunkSize ) != commonChunkSize )
	{
		valid = false;
		return;
	}
	
	// Increment the current write position.
	currentWritePosition += commonChunkSize;
	
	//*************************************************************************
	// Write the header for the sound data chunk for the file.
	
	const Size soundChunkHeaderSize = CHUNK_ID_SIZE + sizeof(Int32) + 2*sizeof(UInt32);
	UByte soundChunkHeader[soundChunkHeaderSize];
	
	// Set the ID for the sound chunk.
	setID( soundChunkHeader, "SSND" );
	
	// Set the initial size of the sound chunk to be 8 (offset + block size + 0 sample data).
	setInt32( soundChunkHeader + CHUNK_ID_SIZE, 8 );
	
	// Set the offset and block size to be 0.
	setUInt32( soundChunkHeader + OFFSET_OFFSET, 0 );
	setUInt32( soundChunkHeader + BLOCK_SIZE_OFFSET, 0 );
	
	// Note the location of the start of the SSND chunk.
	soundChunkStart = currentWritePosition;
	
	// Write the sound chunk to the stream.
	if ( stream->writeData( soundChunkHeader, soundChunkHeaderSize ) != soundChunkHeaderSize )
	{
		valid = false;
		return;
	}
	
	// Increment the current write position.
	currentWritePosition += soundChunkHeaderSize;
	
	
	//*************************************************************************
	
	
	writtenHeader = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		80-bit Floating Point Conversion Method
//############		
//##########################################################################################
//##########################################################################################



# define FloatToUnsigned(f) ((unsigned long)(((long)(f - 2147483648.0)) + 2147483647L) + 1)



void AIFFEncoder:: convertFP64ToFP80( Float64 number, UByte fp80[10] )
{
	int sign;
	int expon;
	double fMant, fsMant;
	unsigned long hiMant, loMant;

	if ( number < 0 )
	{
		sign = 0x8000;
		number *= -1;
	}
	else
		sign = 0;

	if ( number == 0 )
	{
		expon = 0;
		hiMant = 0;
		loMant = 0;
	}
	else
	{
		fMant = frexp( number, &expon );
		
		if ( (expon > 16384) || !(fMant < 1) )
		{
			// Infinity or NaN
			expon = sign | 0x7FFF;
			hiMant = 0;
			loMant = 0; /* infinity */
		}
		else
		{
			// Finite.
			expon += 16382;
			
			if ( expon < 0 )
			{
				// Denormalized.
				fMant = ldexp(fMant, expon);
				expon = 0;
			}
			
			expon |= sign;
			fMant = ldexp(fMant, 32);          
			fsMant = floor(fMant); 
			hiMant = FloatToUnsigned(fsMant);
			fMant = ldexp(fMant - fsMant, 32); 
			fsMant = floor(fMant); 
			loMant = FloatToUnsigned(fsMant);
		}
	}

	fp80[0] = UByte(expon >> 8);
	fp80[1] = UByte(expon);
	fp80[2] = UByte(hiMant >> 24);
	fp80[3] = UByte(hiMant >> 16);
	fp80[4] = UByte(hiMant >> 8);
	fp80[5] = UByte(hiMant);
	fp80[6] = UByte(loMant >> 24);
	fp80[7] = UByte(loMant >> 16);
	fp80[8] = UByte(loMant >> 8);
	fp80[9] = UByte(loMant);
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
