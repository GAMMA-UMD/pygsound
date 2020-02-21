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

#include "omSoundWaveDecoder.h"


#define WAVE_HEADER_SIZE 44

#define RIFF_CHUNK_HEADER_SIZE 12
#define CHUNK_HEADER_SIZE 8
#define MAX_FORMAT_CHUNK_SIZE 40


// Define the different WAVE formats that can be decoded
#define WAVE_FORMAT_PCM 1
#define WAVE_FORMAT_IEEE_FLOAT 3
#define WAVE_FORMAT_A_LAW 6
#define WAVE_FORMAT_MU_LAW 7


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Decoding Helper Methods
//############		
//##########################################################################################
//##########################################################################################




OM_INLINE static Int16 getInt16( const UByte* bytes )
{
	return ((Int16)bytes[1] << 8) | (Int16)bytes[0];
}




OM_INLINE static Int24 getInt24( const UByte* bytes )
{
	Int32 high = static_cast<Int8>(bytes[2]);
	
	return Int24((high << 16) | ((Int32)bytes[1] << 8) | (Int32)bytes[0]);
}




OM_INLINE static Int32 getInt32( const UByte* bytes )
{
	return ((Int32)bytes[3] << 24) | ((Int32)bytes[2] << 16) | ((Int32)bytes[1] << 8) | (Int32)bytes[0];
}




OM_INLINE static Int64 getInt64( const UByte* bytes )
{
	return ((Int64)bytes[7] << 56) | ((Int64)bytes[6] << 48) |
			((Int64)bytes[5] << 40) | ((Int64)bytes[4] << 32) |
			((Int64)bytes[3] << 24) | ((Int64)bytes[2] << 16) |
			((Int64)bytes[1] << 8) | (Int64)bytes[0];
}




OM_INLINE static Float32 getFloat32( const UByte* bytes )
{
	// Convert the floating point number from little to native endianness.
	return data::endian::fromLittleEndian( *(Float32*)bytes );
}




OM_INLINE static Float64 getFloat64( const UByte* bytes )
{
	// Convert the floating point number from little to native endianness.
	return data::endian::fromLittleEndian( *(Float64*)bytes );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




WaveDecoder:: WaveDecoder( DataInputStream* waveStream )
	:	stream( waveStream ),
		channelLayout(),
		sampleRate( 0 ),
		sampleType( SampleType::UNDEFINED ),
		format( 0 ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		validFile( false )
{
	openFile();
}




WaveDecoder:: WaveDecoder( const WaveDecoder& other )
	:	stream( other.stream ),
		channelLayout( other.channelLayout ),
		sampleRate( other.sampleRate ),
		sampleType( other.sampleType ),
		format( other.format ),
		lengthInSamples( other.lengthInSamples ),
		currentSampleIndex( other.currentSampleIndex ),
		validFile( other.validFile )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




WaveDecoder:: ~WaveDecoder()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




WaveDecoder& WaveDecoder:: operator = ( const WaveDecoder& other )
{
	if ( this != &other )
	{
		stream = other.stream;
		channelLayout = other.channelLayout;
		sampleRate = other.sampleRate;
		sampleType = other.sampleType;
		format = other.format;
		lengthInSamples = other.lengthInSamples;
		currentSampleIndex = other.currentSampleIndex;
		validFile = other.validFile;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek Methods
//############		
//##########################################################################################
//##########################################################################################




Bool WaveDecoder:: canSeek() const
{
	decodingMutex.lock();
	
	Bool seekingAllowed = stream != NULL && stream->canSeek();
	
	decodingMutex.unlock();
	
	return seekingAllowed;
}




Bool WaveDecoder:: canSeek( Int64 relativeSampleOffset ) const
{
	decodingMutex.lock();
	
	// If there is no stream or if it can't seek, return that the seek is not allowed.
	if ( stream == NULL || !stream->canSeek() )
	{
		decodingMutex.unlock();
		return 0;
	}
	
	Int64 destination = (Int64)currentSampleIndex + relativeSampleOffset;
	Bool seekAllowed = destination > 0 && SoundSize(destination) < lengthInSamples;
	
	decodingMutex.unlock();
	
	return seekAllowed;
}




Int64 WaveDecoder:: seek( Int64 relativeSampleOffset )
{
	decodingMutex.lock();
	
	// If there is no stream or if it can't seek, return that we didn't seek any.
	if ( stream == NULL || !stream->canSeek() )
	{
		decodingMutex.unlock();
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
	Int64 bytesPerSampleFrame = channelLayout.getChannelCount()*getBytesPerSample();
	
	// Seek the maximum allowed amount.
	Int64 actualSeek = stream->seek( seekAmount*bytesPerSampleFrame );
	Int64 actualSampleSeek = actualSeek / bytesPerSampleFrame;
	currentSampleIndex = SampleIndex(Int64(currentSampleIndex) + actualSampleSeek);
	
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




SoundSize WaveDecoder:: getSamplesRemaining() const
{
	if ( !validFile )
		return SoundSize(0);
	else
		return SoundSize(lengthInSamples - currentSampleIndex);
}




SampleIndex WaveDecoder:: getPosition() const
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




Size WaveDecoder:: getChannelCount() const
{
	return channelLayout.getChannelCount();
}




SampleRate WaveDecoder:: getSampleRate() const
{
	return sampleRate;
}




SampleType WaveDecoder:: getNativeSampleType() const
{
	return sampleType;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoder Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool WaveDecoder:: isValid() const
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




SoundResult WaveDecoder:: readSamples( SoundBuffer& inputBuffer, Size numSamples )
{
	// Acquire the mutex which synchronizes decoding with other class access.
	decodingMutex.lock();
	
	// If the file was not able to be opened or the end of the file has been reached,
	// return indicating that no samples were read.
	if ( !validFile || stream == NULL || currentSampleIndex >= lengthInSamples )
	{
		decodingMutex.unlock();
		return SoundResult::ERROR;
	}
	
	// Make sure that the sample output buffer has the correct sample rate.
	inputBuffer.setSampleRate( sampleRate );
	inputBuffer.setLayout( channelLayout );
	const Size numChannels = channelLayout.getChannelCount();
	
	// Compute the total number of bytes to read from the WAVE file.
	Size numSamplesToRead = (Size)math::min( lengthInSamples - currentSampleIndex, SoundSize(numSamples) );
	Size bytesPerSample = getBytesPerSample();
	Size bytesPerSampleFrame = numChannels*bytesPerSample;
	
	// Declare a stack-based temporary buffer for the input data.
	const Size dataBufferSize = 1024;
	UByte dataBuffer[dataBufferSize];
	
	// Keep track of the number of samples that we have to read in order to fill the quota.
	Size numSamplesRemaining = numSamplesToRead;
	Size numSamplesRead = 0;
	
	// Fill the output buffer with samples from the wave file.
	while ( numSamplesRemaining > 0 && stream->hasBytesRemaining() )
	{
		// Compute the total number of samples frames that we can fit in the input data buffer.
		Size numInputSamples = math::min( dataBufferSize / bytesPerSampleFrame, numSamplesRemaining );
		Size numInputBytes = numInputSamples*bytesPerSampleFrame;
		
		// Read that much data from the input data stream.
		Size numBytesRead = stream->readData( dataBuffer, numInputBytes );
		numInputSamples = numBytesRead / bytesPerSampleFrame;
		
		// Copy and convert the format for the input sample data.
		for ( Index c = 0; c < numChannels; c++ )
		{
			Sample32f* output = inputBuffer.getChannel(c) + numSamplesRead;
			const Sample32f* const outputEnd = output + numInputSamples;
			const UByte* source = dataBuffer + c*bytesPerSample;
			
			if ( format == WAVE_FORMAT_PCM )
			{
				switch ( bytesPerSample )
				{
					case 1:
						while ( output != outputEnd )
						{
							*output = Sample8( Int8((Int16)source[0] - 127) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					case 2:
						while ( output != outputEnd )
						{
							*output = Sample16( getInt16(source) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					case 3:
						while ( output != outputEnd )
						{
							*output = Sample<Float32>( Sample24( getInt24(source) ) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					case 4:
						while ( output != outputEnd )
						{
							*output = Sample32( getInt32(source) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					case 8:
						while ( output != outputEnd )
						{
							*output = Sample64( getInt64(source) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					default:
					{
						// Return 0 indicating no samples were read if the format is invalid.
						decodingMutex.unlock();
						return 0;
					}
				}
			}
			else if ( format == WAVE_FORMAT_IEEE_FLOAT )
			{
				switch ( bytesPerSample )
				{
					case 4:
						while ( output != outputEnd )
						{
							*output = Sample32f( getFloat32(source) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					case 8:
						while ( output != outputEnd )
						{
							*output = Sample32f( Sample64f( getFloat64(source) ) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					default:
					{
						// Return 0 indicating no samples were read if the format is invalid.
						decodingMutex.unlock();
						return 0;
					}
				}
			}
			else if ( format == WAVE_FORMAT_A_LAW )
			{
				while ( output != outputEnd )
				{
					*output = Sample16( decodeALaw( *((const UInt8*)source) ) );
					
					output++;
					source += bytesPerSampleFrame;
				}
			}
			else if ( format == WAVE_FORMAT_MU_LAW )
			{
				while ( output != outputEnd )
				{
					*output = Sample16( decodeMuLaw( *((const UInt8*)source) ) );
					
					output++;
					source += bytesPerSampleFrame;
				}
			}
			else
			{
				// Return 0 indicating no samples were read if the format is invalid.
				decodingMutex.unlock();
				return 0;
			}
		}
		
		numSamplesRemaining -= numInputSamples;
		numSamplesRead += numInputSamples;
	}
	
	
	// Update the current sample index.
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




void WaveDecoder:: openFile()
{
	// If the stream is NULL, the file doesn't exist and we can return here.
	if ( stream == NULL )
	{
		validFile = false;
		return;
	}
	
	//*******************************************************************************
	// Read the header of the wave file.
	
	UByte riffHeader[RIFF_CHUNK_HEADER_SIZE];
	
	Size numberRead = stream->readData( riffHeader, RIFF_CHUNK_HEADER_SIZE );
	
	// If the file is shorter than the header size for a WAVE file, close the file.
	if ( numberRead != RIFF_CHUNK_HEADER_SIZE )
	{
		validFile = false;
		return;
	}
	
	//*******************************************************************************
	// Verify that this is an RIFF and WAVE file.
	
	Bool isRIFF = riffHeader[0] == 'R' && riffHeader[1] == 'I' && riffHeader[2] == 'F' && riffHeader[3] == 'F';
	Bool isWAVE = riffHeader[8] == 'W' && riffHeader[9] == 'A' && riffHeader[10] == 'V' && riffHeader[11] == 'E';
	
	// If the file is not a RIFF or WAVE file, close it.
	if ( !isRIFF || !isWAVE )
	{
		validFile = false;
		return;
	}
	
	//*******************************************************************************
	// Find the 'fmt ' chunk.
	
	UByte chunkHeader[CHUNK_HEADER_SIZE];
	Int32 bytesPerSample = 0;
	Size numChannels = 0;
	
	while ( 1 )
	{
		// Read in the next chunk's header.
		if ( stream->readData( chunkHeader, CHUNK_HEADER_SIZE ) != CHUNK_HEADER_SIZE )
		{
			// Failed to read chunk header, file is not valid.
			validFile = false;
			return;
		}
		
		// Compute the size of the chunk.
		Int32 chunkSize = getInt32( chunkHeader + 4 );
		
		// What kind of chunk is this?
		if ( chunkHeader[0] == 'f' && chunkHeader[1] == 'm' && chunkHeader[2] == 't' && chunkHeader[3] == ' ' )
		{
			// Read the format chunk.
			UByte formatChunk[MAX_FORMAT_CHUNK_SIZE];
			
			if ( stream->readData( formatChunk, chunkSize ) != chunkSize )
			{
				validFile = false;
				return;
			}
			
			//*******************************************************************************
			// Determine the format of the WAVE file.
			
			// Get the format (1 == PCM).
			format = getInt16( formatChunk + 0 );
			
			// If the file's format is not PCM or one of the supported compressed formats,
			// close the wave file.
			if ( format != WAVE_FORMAT_PCM && format != WAVE_FORMAT_IEEE_FLOAT && format != WAVE_FORMAT_A_LAW &&
				format != WAVE_FORMAT_MU_LAW )
			{
				validFile = false;
				return;
			}
			
			//*******************************************************************************
			// Get information about the audio format of the file.
			
			numChannels = getInt16( formatChunk + 2 );
			sampleRate = Float(getInt32( formatChunk + 4 ));
			bytesPerSample = getInt16( formatChunk + 14 ) >> 3;
			
			if ( format == WAVE_FORMAT_PCM || format == WAVE_FORMAT_A_LAW || format == WAVE_FORMAT_MU_LAW )
			{
				switch ( bytesPerSample )
				{
					case 1:		sampleType = SampleType::SAMPLE_8;	break;
					case 2:		sampleType = SampleType::SAMPLE_16;	break;
					case 3:		sampleType = SampleType::SAMPLE_24;	break;
					case 4:		sampleType = SampleType::SAMPLE_32;	break;
					case 8:		sampleType = SampleType::SAMPLE_64;	break;
					default:
						validFile = false;
						return;
				}
			}
			else if ( format == WAVE_FORMAT_IEEE_FLOAT )
			{
				switch ( bytesPerSample )
				{
					case 4:		sampleType = SampleType::SAMPLE_32F;	break;
					case 8:		sampleType = SampleType::SAMPLE_64F;	break;
					default:
						validFile = false;
						return;
				}
			}
			
			// Break from loop.
			break;
		}
		else
		{
			// Skip this chunk.
			if ( stream->canSeek( chunkSize ) )
				stream->seek( chunkSize );
			else
			{
				// Can't seek, file is invalid.
				validFile = false;
				return;
			}
		}
	}
	
	// Determine the channel layout for the file's channel count.
	channelLayout = ChannelLayout( numChannels );
	channelLayout.setType( ChannelLayoutType( numChannels ) );
	
	//*******************************************************************************
	// Find the 'data' chunk.
	
	while ( 1 )
	{
		// Read in the next chunk's header.
		if ( stream->readData( chunkHeader, CHUNK_HEADER_SIZE ) != CHUNK_HEADER_SIZE )
		{
			// Failed to read chunk header, file is not valid.
			validFile = false;
			return;
		}
		
		// Compute the size of the chunk.
		Int32 chunkSize = getInt32( chunkHeader + 4 );
		
		// What kind of chunk is this?
		if ( chunkHeader[0] == 'd' && chunkHeader[1] == 'a' && chunkHeader[2] == 't' && chunkHeader[3] == 'a' )
		{
			// Compute the length of the file in samples from the length of the data chunk.
			lengthInSamples = chunkSize / (numChannels * bytesPerSample);
			
			// Break from loop.
			break;
		}
		else
		{
			// Skip this chunk.
			if ( stream->canSeek( chunkSize ) )
				stream->seek( chunkSize );
			else
			{
				// Can't seek, file is invalid.
				validFile = false;
				return;
			}
		}
	}
	
	//*******************************************************************************
	// Set the current read position to be the beginning of the file.
	
	currentSampleIndex = 0;
	validFile = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Bytes Per Sample Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Size WaveDecoder:: getBytesPerSample() const
{
	switch ( format )
	{
		case WAVE_FORMAT_PCM:
		case WAVE_FORMAT_IEEE_FLOAT:
			return sampleType.getSizeInBytes();
		case WAVE_FORMAT_A_LAW:
		case WAVE_FORMAT_MU_LAW:
			return 1;
		default:
			return 1;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		A-Law Decoding Method
//############		
//##########################################################################################
//##########################################################################################




Int16 WaveDecoder:: decodeALaw( UInt8 aLaw )
{
	const Int16 ALAW_SIGN_BIT = 0x80;
	const Int16 ALAW_SEG_SHIFT = 4;
	const Int16 ALAW_SEG_MASK = 0x70;
	const UInt8 ALAW_QUANT_MASK = 0xf;
	
	aLaw ^= 0x55;

	Int16 t = (aLaw & ALAW_QUANT_MASK) << 4;
	UInt16 seg = ((UInt16)aLaw & (UInt16)ALAW_SEG_MASK) >> ALAW_SEG_SHIFT;
	
	switch ( seg )
	{
		case 0:
			t += 8;
			break;
		case 1:
			t += 0x108;
			break;
		default:
			t += 0x108;
			t <<= seg - 1;
	}
	
	return ((aLaw & ALAW_SIGN_BIT) ? t : -t);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mu-Law Decoding Method
//############		
//##########################################################################################
//##########################################################################################




Int16 WaveDecoder:: decodeMuLaw( UInt8 muLaw )
{
	const Int16 MULAW_SIGN_BIT = 0x80;
	const Int16 MULAW_SEG_SHIFT = 4;
	const Int16 MULAW_SEG_MASK = 0x70;
	const UInt8 MULAW_QUANT_MASK = 0xf;
	const Int16 MULAW_BIAS = 0x84;
	
	/* Complement to obtain normal u-law value. */
	muLaw = ~muLaw;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	Int16 t = ((Int16)(muLaw & MULAW_QUANT_MASK) << 3) + MULAW_BIAS;
	t <<= ((UInt16)muLaw & MULAW_SEG_MASK) >> MULAW_SEG_SHIFT;

	return ((muLaw & MULAW_SIGN_BIT) ? (MULAW_BIAS - t) : (t - MULAW_BIAS));
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
