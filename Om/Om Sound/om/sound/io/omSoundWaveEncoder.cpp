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

#include "omSoundWaveEncoder.h"


#define WAVE_HEADER_SIZE 44


// Define the different WAVE formats that can be encoded
#define WAVE_FORMAT_PCM 1
#define WAVE_FORMAT_IEEE_FLOAT 3
#define WAVE_FORMAT_A_LAW 6
#define WAVE_FORMAT_MU_LAW 7


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



OM_INLINE static void setInt16( UByte* bytes, Int16 i )
{
	bytes[0] = i & 0xFF;
	bytes[1] = (i >> 8) & 0xFF;
}




OM_INLINE static void setUInt16( UByte* bytes, UInt16 i )
{
	bytes[0] = i & 0xFF;
	bytes[1] = (i >> 8) & 0xFF;
}




OM_INLINE static void setInt24( UByte* bytes, Int32 i )
{
	bytes[0] = i & 0xFF;
	bytes[1] = (i >> 8) & 0xFF;
	bytes[2] = (i >> 16) & 0xFF;
}




OM_INLINE static void setInt32( UByte* bytes, Int32 i )
{
	bytes[0] = i & 0xFF;
	bytes[1] = (i >> 8) & 0xFF;
	bytes[2] = (i >> 16) & 0xFF;
	bytes[3] = (i >> 24) & 0xFF;
}




OM_INLINE static void setUInt32( UByte* bytes, UInt32 i )
{
	bytes[0] = i & 0xFF;
	bytes[1] = (i >> 8) & 0xFF;
	bytes[2] = (i >> 16) & 0xFF;
	bytes[3] = (i >> 24) & 0xFF;
}




OM_INLINE static void setInt64( UByte* bytes, Int64 i )
{
	bytes[0] = UByte(i & 0xFF);
	bytes[1] = UByte((i >> 8) & 0xFF);
	bytes[2] = UByte((i >> 16) & 0xFF);
	bytes[3] = UByte((i >> 24) & 0xFF);
	bytes[4] = UByte((i >> 32) & 0xFF);
	bytes[5] = UByte((i >> 40) & 0xFF);
	bytes[6] = UByte((i >> 48) & 0xFF);
	bytes[7] = UByte((i >> 56) & 0xFF);
}




OM_INLINE static void setUInt64( UByte* bytes, UInt64 i )
{
	bytes[0] = UByte(i & 0xFF);
	bytes[1] = UByte((i >> 8) & 0xFF);
	bytes[2] = UByte((i >> 16) & 0xFF);
	bytes[3] = UByte((i >> 24) & 0xFF);
	bytes[4] = UByte((i >> 32) & 0xFF);
	bytes[5] = UByte((i >> 40) & 0xFF);
	bytes[6] = UByte((i >> 48) & 0xFF);
	bytes[7] = UByte((i >> 56) & 0xFF);
}




OM_INLINE static void setFloat32( UByte* bytes, Float32 f )
{
	*(Float32*)bytes = data::endian::toLittleEndian( f );
}




OM_INLINE static void setFloat64( UByte* bytes, Float64 f )
{
	*(Float64*)bytes = data::endian::toLittleEndian( f );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




WaveEncoder:: WaveEncoder( DataOutputStream* outputStream, Size newNumChannels,
							SampleType newSampleType )
	:	numChannels( math::max( newNumChannels, Size(1) ) ),
		sampleType( newSampleType ),
		sampleRate( 0 ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		valid( sampleTypeIsValid( newSampleType ) ),
		writtenHeader( false ),
		stream( outputStream )
{
	if ( stream == NULL )
		valid = false;
}




WaveEncoder:: WaveEncoder( DataOutputStream* outputStream, Size newNumChannels,
							SampleType newSampleType, SampleRate newSampleRate )
	:	numChannels( math::max( newNumChannels, Size(1) ) ),
		sampleType( newSampleType ),
		sampleRate( math::max( newSampleRate, SampleRate(0) ) ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		valid( sampleTypeIsValid( newSampleType ) ),
		writtenHeader( false ),
		stream( outputStream )
{
	if ( stream == NULL )
		valid = false;
}




WaveEncoder:: WaveEncoder( const WaveEncoder& other )
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




WaveEncoder:: ~WaveEncoder()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




WaveEncoder& WaveEncoder:: operator = ( const WaveEncoder& other )
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




void WaveEncoder:: flush()
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




Bool WaveEncoder:: canSeek() const
{
	encodingMutex.lock();
	
	Bool seekingAllowed = valid && stream != NULL && stream->canSeek();
	
	encodingMutex.unlock();
	
	return seekingAllowed;
}




Bool WaveEncoder:: canSeek( Int64 relativeSampleOffset ) const
{
	encodingMutex.lock();
	
	// If there is no stream or if it can't seek, return that the seek is not allowed.
	if ( !valid || stream == NULL || !stream->canSeek() )
	{
		encodingMutex.unlock();
		return 0;
	}
	
	Int64 destination = (Int64)currentSampleIndex + relativeSampleOffset;
	Bool seekAllowed = destination > 0 && SoundSize(destination) < lengthInSamples;
	
	encodingMutex.unlock();
	
	return seekAllowed;
}




Int64 WaveEncoder:: seek( Int64 relativeSampleOffset )
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




Size WaveEncoder:: getChannelCount() const
{
	if ( valid )
		return 0;
	else
		return numChannels;
}




SampleRate WaveEncoder:: getSampleRate() const
{
	if ( valid )
		return SampleRate(0);
	else
		return sampleRate;
}




SampleType WaveEncoder:: getNativeSampleType() const
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




SoundResult WaveEncoder:: writeSamples( const SoundBuffer& inputBuffer, Size numSamples )
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
	
	const SoundBuffer* finalInputBuffer;
	
	// Get a temporary buffer for use in potential sample rate conversion.
	SharedSoundBuffer tempBuffer = SharedBufferPool::getGlobalBuffer();
	
	// Test to see if the input buffer should be sample rate converted.
	if ( inputBuffer.getSampleRate() != sampleRate )
	{
		resampler.setOutputSampleRate( sampleRate );
		numSamples = resampler.process( inputBuffer, tempBuffer.getBuffer(), numSamples );
		finalInputBuffer = &tempBuffer.getBuffer();
	}
	else
		finalInputBuffer = &inputBuffer;
	
	//***************************************************************************
	// Write the wave file's header if we haven't already.
	
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
				
				// Figure out which bit depth to use for the wave sample data.
				switch ( sampleType )
				{
					case SampleType::SAMPLE_8:
						{
							while ( input != inputEnd )
							{
								Int8 sample = Sample8(*input);
								*output = UByte((Int32(sample) + 128) & 0xFF);
								
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
				// Write zeros to the wave file for this channel.
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
								setInt32( output, 0 );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_64:
						{
							while ( output != outputEnd )
							{
								setInt64( output, 0 );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_32F:
						{
							while ( output != outputEnd )
							{
								setFloat32( output, 0 );
								output += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_64F:
						{
							while ( output != outputEnd )
							{
								setFloat64( output, 0 );
								output += bytesPerSampleFrame;
							}
						}
						break;
				}
			}
		}
		
		// Write the sample data in the buffer to the data output stream.
		Size numBytesWritten = stream->writeData( dataBuffer, bytesInBuffer );
		
		// Compute the number of samples actually written.
		numOutputSamples = numBytesWritten / bytesPerSampleFrame;
		
		// Update the encoding loop counters.
		numSamplesRemaining -= numOutputSamples;
		numSamplesWritten += numOutputSamples;
	}
	
	//**********************************************************************************
	// Update the total length of the audio file (if seeking in the stream is enabled).
	
	currentSampleIndex += numSamplesWritten;
	lengthInSamples = math::max( currentSampleIndex, lengthInSamples );
	
	if ( stream->canSeek() )
	{
		UInt32 subChunk2Size = UInt32(currentSampleIndex*bytesPerSampleFrame);
		UInt32 extraByte = 0;
		
		// Do we write an extra byte so that the last sample is on an even byte boundary?
		if ( subChunk2Size & 0x1 )
		{
			stream->writeData( UByte(0) );
			extraByte = 1;
		}
		
		// Seek to the position of the data stream to the 40th byte in the file.
		Int64 relativeSeek = -Int64(subChunk2Size) - 4 - extraByte;
		Int64 totalSeek = stream->seek(relativeSeek);
		
		// Write a sub chunk size that is consistent with the even-byte-boundary padding.
		subChunk2Size += extraByte;
		
		if ( totalSeek == relativeSeek )
		{
			// Write the data chunk size, the length of the sample data in bytes.
			setUInt32( dataBuffer, subChunk2Size );
			
			stream->writeData( dataBuffer, sizeof(UInt32) );
			totalSeek += sizeof(UInt32);
			
			// Seek to the byte at index 4 in the file.
			totalSeek += stream->seek( -40 );
			
			// Write the chunk size, the size of the file minus 8 bytes.
			UInt32 chunkSize = 36 + subChunk2Size;
			setUInt32( dataBuffer, chunkSize );
			
			stream->writeData( dataBuffer, sizeof(UInt32) );
			totalSeek += sizeof(UInt32);
		}
		
		// Seek back to the original location, before any even-byte-boundary padding.
		stream->seek( -totalSeek - extraByte );
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




void WaveEncoder:: writeHeader()
{
	if ( !valid )
		return;
	
	// Create an array of bytes on the stack to hold the output data.
	const Size headerSize = WAVE_HEADER_SIZE;
	UByte dataBuffer[headerSize];
	
	//*************************************************************
	// Write the RIFF Header
	
	// Write the Chunk ID
	dataBuffer[0] = 'R';
	dataBuffer[1] = 'I';
	dataBuffer[2] = 'F';
	dataBuffer[3] = 'F';
	
	// Write a placeholder 4 bytes for the chunk size
	dataBuffer[7] = dataBuffer[6] = dataBuffer[5] = dataBuffer[4] = UByte(0);
	
	// Write the format of the file
	dataBuffer[8] = 'W';
	dataBuffer[9] = 'A';
	dataBuffer[10] = 'V';
	dataBuffer[11] = 'E';
	
	//*************************************************************
	// Write the format chunk
	
	// Write the format chunk ID
	dataBuffer[12] = 'f';
	dataBuffer[13] = 'm';
	dataBuffer[14] = 't';
	dataBuffer[15] = ' ';
	
	// Write the size of the format chunk in bytes, a 32-bit integer.
	setUInt32( dataBuffer + 16, 16 );
	
	// Write the audio format (1 for PCM), a 16-bit integer.
	if ( sampleType.isFloatingPoint() )
		setUInt16( dataBuffer + 20, WAVE_FORMAT_IEEE_FLOAT );
	else
		setUInt16( dataBuffer + 20, WAVE_FORMAT_PCM );
	
	// Write the number of channels, a 16-bit integer.
	setUInt16( dataBuffer + 22, (UInt16)numChannels );
	
	// Write the sample rate, a 32-bit integer.
	UInt32 intSampleRate = (UInt32)sampleRate;
	sampleRate = (SampleRate)intSampleRate;
	
	setUInt32( dataBuffer + 24, intSampleRate );
	
	// Write the byte rate (bytes per second of audio), a 32-bit integer.
	UInt32 byteRate = UInt32(intSampleRate*numChannels*sampleType.getSizeInBytes());
	
	setUInt32( dataBuffer + 28, byteRate );
	
	// Write the block align (the number of bytes for one sample of all channels), a 16-bit integer.
	UInt16 blockAlign = UInt16(numChannels*sampleType.getSizeInBytes());
	
	setUInt16( dataBuffer + 32, blockAlign );
	
	// Write the number of bits per sample, a 16-bit integer.
	UInt16 bitsPerSample = UInt16(sampleType.getSizeInBytes()*8);
	
	setUInt16( dataBuffer + 34, bitsPerSample );
	
	//*************************************************************
	// Write the data chunk
	
	// Write the data chunk ID
	dataBuffer[36] = 'd';
	dataBuffer[37] = 'a';
	dataBuffer[38] = 't';
	dataBuffer[39] = 'a';
	
	// Write a placeholder for the data chunk size
	setUInt16( dataBuffer + 40, UInt32(0) );
	
	// Write the header to the stream.
	stream->writeData( dataBuffer, headerSize );
	
	writtenHeader = true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
