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

#include "omSoundAIFFDecoder.h"


#define CHUNK_ID_SIZE 4


#define SAMPLE_RATE_OFFSET 8
#define COMPRESSION_TYPE_OFFSET 18
#define SOUND_CHUNK_INFO_SIZE 8

#define COMPRESSION_TYPE_NONE 0
#define COMPRESSION_TYPE_MU_LAW 1
#define COMPRESSION_TYPE_A_LAW 2


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




OM_INLINE static Bool chunkIDEquals( const UByte* chunkID, const Char* id )
{
	return chunkID[0] == id[0] && chunkID[1] == id[1] && chunkID[2] == id[2] && chunkID[3] == id[3];
}




OM_INLINE static Int16 getInt16( const UByte* bytes )
{
	return ((Int16)bytes[0] << 8) | (Int16)bytes[1];
}




OM_INLINE static Int16 getInt16LittleEndian( const UByte* bytes )
{
	return ((Int16)bytes[1] << 8) | (Int16)bytes[0];
}




OM_INLINE static Int24 getInt24( const UByte* bytes )
{
	Int32 high = static_cast<Int8>(bytes[0]);
	
	return Int24( (high << 16) | ((Int32)bytes[1] << 8) | (Int32)bytes[2] );
}




OM_INLINE static Int24 getInt24LittleEndian( const UByte* bytes )
{
	Int32 high = static_cast<Int8>(bytes[2]);
	
	return Int24( (high << 16) | ((Int32)bytes[1] << 8) | (Int32)bytes[0] );
}




OM_INLINE static Int32 getInt32( const UByte* bytes )
{
	return ((Int32)bytes[0] << 24) | ((Int32)bytes[1] << 16) | ((Int32)bytes[2] << 8) | (Int32)bytes[3];
}




OM_INLINE static Int32 getInt32LittleEndian( const UByte* bytes )
{
	return ((Int32)bytes[3] << 24) | ((Int32)bytes[2] << 16) | ((Int32)bytes[1] << 8) | (Int32)bytes[0];
}




OM_INLINE static UInt32 getUInt32( const UByte* bytes )
{
	return ((UInt32)bytes[0] << 24) | ((UInt32)bytes[1] << 16) | ((UInt32)bytes[2] << 8) | (UInt32)bytes[3];
}




OM_INLINE static Int64 getInt64( const UByte* bytes )
{
	return ((Int64)bytes[0] << 56) | ((Int64)bytes[1] << 48) |
			((Int64)bytes[2] << 40) | ((Int64)bytes[3] << 32) |
			((Int64)bytes[4] << 24) | ((Int64)bytes[5] << 16) |
			((Int64)bytes[6] << 8) | (Int64)bytes[7];
}




OM_INLINE static Int64 getInt64LittleEndian( const UByte* bytes )
{
	return ((Int64)bytes[7] << 56) | ((Int64)bytes[6] << 48) |
			((Int64)bytes[5] << 40) | ((Int64)bytes[4] << 32) |
			((Int64)bytes[3] << 24) | ((Int64)bytes[2] << 16) |
			((Int64)bytes[1] << 8) | (Int64)bytes[0];
}




OM_INLINE static Float32 getFloat32( const UByte* bytes )
{
	// Convert the floating point number from big to native endianness.
	return data::endian::fromBigEndian( *(Float32*)bytes );
}




OM_INLINE static Float64 getFloat64( const UByte* bytes )
{
	// Convert the floating point number from big to native endianness.
	return data::endian::fromBigEndian( *(Float64*)bytes );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




AIFFDecoder:: AIFFDecoder( DataInputStream* aiffStream )
	:	stream( aiffStream ),
		channelLayout(),
		sampleRate( 0 ),
		sampleType( SampleType::UNDEFINED ),
		compressionType( COMPRESSION_TYPE_NONE ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		isLittleEndian( false ),
		validFile( false )
{
	openFile();
}




AIFFDecoder:: AIFFDecoder( const AIFFDecoder& other )
	:	stream( other.stream ),
		channelLayout( other.channelLayout ),
		sampleRate( other.sampleRate ),
		sampleType( other.sampleType ),
		compressionType( other.compressionType ),
		lengthInSamples( other.lengthInSamples ),
		currentSampleIndex( other.currentSampleIndex ),
		isLittleEndian( other.isLittleEndian ),
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




AIFFDecoder:: ~AIFFDecoder()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




AIFFDecoder& AIFFDecoder:: operator = ( const AIFFDecoder& other )
{
	if ( this != &other )
	{
		stream = other.stream;
		channelLayout = other.channelLayout;
		sampleRate = other.sampleRate;
		sampleType = other.sampleType;
		compressionType = other.compressionType;
		lengthInSamples = other.lengthInSamples;
		currentSampleIndex = other.currentSampleIndex;
		isLittleEndian = other.isLittleEndian;
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




Bool AIFFDecoder:: canSeek() const
{
	decodingMutex.lock();
	
	Bool seekingAllowed = stream != NULL && stream->canSeek();
	
	decodingMutex.unlock();
	
	return seekingAllowed;
}




Bool AIFFDecoder:: canSeek( Int64 relativeSampleOffset ) const
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




Int64 AIFFDecoder:: seek( Int64 relativeSampleOffset )
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




SoundSize AIFFDecoder:: getSamplesRemaining() const
{
	if ( !validFile )
		return SoundSize(0);
	else
		return SoundSize(lengthInSamples - currentSampleIndex);
}




SampleIndex AIFFDecoder:: getPosition() const
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




Size AIFFDecoder:: getChannelCount() const
{
	return channelLayout.getChannelCount();
}




SampleRate AIFFDecoder:: getSampleRate() const
{
	return sampleRate;
}




SampleType AIFFDecoder:: getNativeSampleType() const
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




Bool AIFFDecoder:: isValid() const
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




SoundResult AIFFDecoder:: readSamples( SoundBuffer& inputBuffer, Size numSamples )
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
	
	// Make sure that the sample output buffer has the correct sample rate and channel layout.
	inputBuffer.setSampleRate( sampleRate );
	inputBuffer.setLayout( channelLayout );
	const Size numChannels = channelLayout.getChannelCount();
	
	// Compute the total number of bytes to read from the WAVE file.
	const Size numSamplesToRead = (Size)math::min( lengthInSamples - currentSampleIndex, SoundSize(numSamples) );
	const Size bytesPerSample = getBytesPerSample();
	const Size bytesPerSampleFrame = numChannels*bytesPerSample;
	
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
			
			if ( compressionType == COMPRESSION_TYPE_NONE )
			{
				switch ( sampleType )
				{
					case SampleType::SAMPLE_8:
						while ( output != outputEnd )
						{
							*output = Sample8( *((const Int8*)source) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					case SampleType::SAMPLE_16:
						if ( isLittleEndian )
						{
							while ( output != outputEnd )
							{
								*output = Sample16( getInt16LittleEndian(source) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						else
						{
							while ( output != outputEnd )
							{
								*output = Sample16( getInt16(source) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_24:
						if ( isLittleEndian )
						{
							while ( output != outputEnd )
							{
								*output = Sample<Float32>( Sample24( getInt24LittleEndian(source) ) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						else
						{
							while ( output != outputEnd )
							{
								*output = Sample<Float32>( Sample24( getInt24(source) ) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_32:
						if ( isLittleEndian )
						{
							while ( output != outputEnd )
							{
								*output = Sample32( getInt32LittleEndian(source) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						else
						{
							while ( output != outputEnd )
							{
								*output = Sample32( getInt32(source) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_64:
						if ( isLittleEndian )
						{
							while ( output != outputEnd )
							{
								*output = Sample64( getInt64LittleEndian(source) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						else
						{
							while ( output != outputEnd )
							{
								*output = Sample64( getInt64(source) );
								
								output++;
								source += bytesPerSampleFrame;
							}
						}
						break;
						
					case SampleType::SAMPLE_32F:
						while ( output != outputEnd )
						{
							*output = Sample32f( getFloat32(source) );
							
							output++;
							source += bytesPerSampleFrame;
						}
						break;
						
					case SampleType::SAMPLE_64F:
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
			else if ( compressionType == COMPRESSION_TYPE_A_LAW )
			{
				while ( output != outputEnd )
				{
					*output = Sample16( decodeALaw( *((const UInt8*)source) ) );
					
					output++;
					source += bytesPerSampleFrame;
				}
			}
			else if ( compressionType == COMPRESSION_TYPE_MU_LAW )
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
//############		Bytes Per Sample Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Size AIFFDecoder:: getBytesPerSample() const
{
	switch ( compressionType )
	{
		case COMPRESSION_TYPE_NONE:
			return sampleType.getSizeInBytes();
		case COMPRESSION_TYPE_A_LAW:
		case COMPRESSION_TYPE_MU_LAW:
			return 1;
		default:
			return 1;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		AIFF Header Read Method
//############		
//##########################################################################################
//##########################################################################################




void AIFFDecoder:: openFile()
{
	// Assume the file is invalid until it is parsed successfully.
	validFile = false;
	
	// If the stream is NULL, the file doesn't exist and we can return here.
	if ( stream == NULL )
		return;
	
	/// The current byte index within the file.
	LargeIndex currentReadPosition = 0;
	
	//*******************************************************************************
	// Find and read the 'FORM' chunk.
	
	const Size chunkHeaderSize = CHUNK_ID_SIZE + sizeof(Int32);
	UByte formHeader[chunkHeaderSize];
	
	// Break from the loop only when the form chunk has been found. (should be first in most cases).
	while ( 1 )
	{
		// Read the form header, returning that the file is not valid if unsuccessful.
		if ( !stream->hasBytesRemaining() || stream->readData( formHeader, chunkHeaderSize ) != chunkHeaderSize )
			return;
		
		currentReadPosition += chunkHeaderSize;
		
		// Compute the size of this chunk.
		Int32 formSize = (formHeader[4] << 24) | (formHeader[5] << 16) | (formHeader[6] << 8) | formHeader[7];
		
		// Is this the correct chunk type? If so, break from the search loop.
		if ( chunkIDEquals( formHeader, "FORM" ) )
			break;
		else if ( stream->canSeek( formSize ) )
		{
			// Seek past this chunk since it is not the form chunk.
			stream->seek( formSize );
			currentReadPosition += formSize;
		}
		else
			return;
	}
	
	// Determine the form type (AIFF or AIFC).
	UByte formType[CHUNK_ID_SIZE];
	
	if ( stream->readData( formType, CHUNK_ID_SIZE ) != CHUNK_ID_SIZE )
		return;
	
	currentReadPosition += CHUNK_ID_SIZE;
	
	if ( formType[0] == 'A' && formType[1] == 'I' && formType[2] == 'F' )
	{
		if ( formType[3] == 'F' )
			isAIFC = false;
		else if ( formType[3] == 'C' )
			isAIFC = true;
		else
			return;
	}
	else
		return;
	
	//*******************************************************************************
	// Find the common 'COMM' chunk to get information about the file.
	
	LargeIndex soundChunkPosition = 0;
	Size numChannels = 0;
	
	while ( 1 )
	{
		UByte chunkHeader[chunkHeaderSize];
		
		// Attempt to read this chunk's header info. If it fails, the file must be invalid.
		if ( stream->readData( chunkHeader, chunkHeaderSize ) != chunkHeaderSize )
			return;
		
		currentReadPosition += chunkHeaderSize;
		
		Int32 chunkDataSize = getInt32( chunkHeader + CHUNK_ID_SIZE );
		
		// Determine the kind of chunk that this is.
		if ( chunkIDEquals( chunkHeader, "COMM" ) )
		{
			// This is the common chunk, read and extract the information about the AIFF file.
			UByte* chunkInfo = om::util::allocate<UByte>( chunkDataSize );
			
			// Attempt to read the chunk. If it fails, clean up and return.
			if ( stream->readData( chunkInfo, chunkDataSize ) != chunkDataSize )
			{
				om::util::deallocate( chunkInfo );
				return;
			}
			
			currentReadPosition += chunkDataSize;
			
			// Get the attributes for the file.
			numChannels = (chunkInfo[0] << 8) | chunkInfo[1];
			lengthInSamples = getInt32( chunkInfo + 2 );
			
			Bool isFloatingPoint = false;
			
			// Determine the compression type.
			if ( isAIFC )
			{
				const UByte* compressionTypeID = chunkInfo + COMPRESSION_TYPE_OFFSET;
				
				if ( chunkIDEquals( compressionTypeID, "NONE" ) ||
					chunkIDEquals( compressionTypeID, "in24" ) || chunkIDEquals( compressionTypeID, "in32" ) )
					compressionType = COMPRESSION_TYPE_NONE;
				else if ( chunkIDEquals( compressionTypeID, "sowt" ) )
				{
					compressionType = COMPRESSION_TYPE_NONE;
					isLittleEndian = true;
				}
				else if ( chunkIDEquals( compressionTypeID, "fl32" ) || chunkIDEquals( compressionTypeID, "FL32" ) )
				{
					compressionType = COMPRESSION_TYPE_NONE;
					isFloatingPoint = true;
				}
				else if ( chunkIDEquals( compressionTypeID, "fl64" ) || chunkIDEquals( compressionTypeID, "FL64" ) )
				{
					compressionType = COMPRESSION_TYPE_NONE;
					isFloatingPoint = true;
				}
				else if ( chunkIDEquals( compressionTypeID, "alaw" ) || chunkIDEquals( compressionTypeID, "ALAW" ) )
					compressionType = COMPRESSION_TYPE_A_LAW;
				else if ( chunkIDEquals( compressionTypeID, "ulaw" ) || chunkIDEquals( compressionTypeID, "ULAW" ) )
					compressionType = COMPRESSION_TYPE_MU_LAW;
				else
				{
					om::util::deallocate( chunkInfo );
					return;
				}
			}
			else
				compressionType = COMPRESSION_TYPE_NONE;
			
			Size sampleSize = (chunkInfo[6] << 8) | chunkInfo[7];
			
			// Convert the sample size to a sample type.
			switch ( sampleSize )
			{
				case 8:		sampleType = SampleType::SAMPLE_8;	break;
				case 16:	sampleType = SampleType::SAMPLE_16;	break;
				case 24:	sampleType = SampleType::SAMPLE_24;	break;
				case 32:
					if ( isFloatingPoint )
						sampleType = SampleType::SAMPLE_32F;
					else
						sampleType = SampleType::SAMPLE_32;
					break;
				case 64:
					if ( isFloatingPoint )
						sampleType = SampleType::SAMPLE_64F;
					else
						sampleType = SampleType::SAMPLE_64;
					break;
				default:
					om::util::deallocate( chunkInfo );
					return;
			}
			
			// Get the sample rate of the file.
			sampleRate = convertFP80ToFP64( chunkInfo + SAMPLE_RATE_OFFSET );
			
			// Clean up the chunk info buffer.
			om::util::deallocate( chunkInfo );
			
			break;
		}
		else if ( chunkIDEquals( chunkHeader, "SSND" ) )
		{
			// This is the sound data chunk. Save its location so that we can return here.
			soundChunkPosition = currentReadPosition - chunkHeaderSize;
		}
		
		if ( stream->canSeek( chunkDataSize ) )
		{
			// Unknown chunk type, skip it.
			stream->seek( chunkDataSize );
			currentReadPosition += chunkDataSize;
		}
		else
			return;
	}
	
	// Determine the channel layout for the file's channel count.
	channelLayout = ChannelLayout( numChannels );
	channelLayout.setType( ChannelLayoutType( numChannels ) );
	
	//*******************************************************************************
	// Find the 'SSND' chunk and setup the read position to the beginning of the sound data.
	
	UByte soundChunkHeader[chunkHeaderSize];
	Int32 soundChunkDataSize = 0;
	
	if ( soundChunkPosition != 0 )
	{
		// We already passed the 'SSND' chunk while searching for the 'COMM' chunk.
		// Rewind to that position.
		Int64 offset = (Int64)soundChunkPosition - (Int64)currentReadPosition;
		
		if ( !stream->canSeek( offset ) || stream->seek( offset ) != offset )
		{
			// Couldn't seek to the 'SSND' chunk's position using this stream.
			// Format is invalid.
			return;
		}
		
		// Attempt to read this chunk's header info. If it fails, the file must be invalid.
		if ( stream->readData( soundChunkHeader, chunkHeaderSize ) != chunkHeaderSize )
			return;
		
		currentReadPosition += chunkHeaderSize;
		soundChunkDataSize = getInt32( soundChunkHeader + CHUNK_ID_SIZE );
	}
	else
	{
		// Find the 'SSND' chunk, which should be ahead of the current read position.
		while ( 1 )
		{
			// Attempt to read this chunk's header info. If it fails, the file must be invalid.
			if ( stream->readData( soundChunkHeader, chunkHeaderSize ) != chunkHeaderSize )
				return;
			
			currentReadPosition += chunkHeaderSize;
			soundChunkDataSize = getInt32( soundChunkHeader + CHUNK_ID_SIZE );
			
			Size paddingSize = soundChunkDataSize % 2;
			
			if ( chunkIDEquals( soundChunkHeader, "SSND" ) )
			{
				// Found the sound data chunk, break from the search loop.
				break;
			}
			else if ( stream->canSeek( soundChunkDataSize + paddingSize ) )
			{
				// Unknown chunk type, skip it.
				stream->seek( soundChunkDataSize + paddingSize );
				currentReadPosition += soundChunkDataSize + paddingSize;
			}
			else
			{
				// Bad file format.
				return;
			}
		}
	}
	
	// Make sure that the size of the chunk corresponds to the size of the sound reported in the 'COMM' chunk.
	LargeSize expectedSoundDataSize;
	
	switch ( compressionType )
	{
		case COMPRESSION_TYPE_NONE:
			expectedSoundDataSize = expectedSoundDataSize = lengthInSamples*numChannels*sampleType.getSizeInBytes() + SOUND_CHUNK_INFO_SIZE;
			break;
		case COMPRESSION_TYPE_MU_LAW:
		case COMPRESSION_TYPE_A_LAW:
			expectedSoundDataSize = lengthInSamples*numChannels + SOUND_CHUNK_INFO_SIZE;
			break;
		default:
			return;
	}
	
	// If the sizes don't match, the file is invalid.
	if ( soundChunkDataSize != expectedSoundDataSize )
		return;
	
	// Read the offset and block-align values of the sound data.
	UByte soundChunkInfo[SOUND_CHUNK_INFO_SIZE];
	
	if ( stream->readData( soundChunkInfo, SOUND_CHUNK_INFO_SIZE ) != SOUND_CHUNK_INFO_SIZE )
		return;
	
	UInt32 offset = getUInt32( soundChunkInfo );
	UInt32 blockSize = getUInt32( soundChunkInfo + sizeof(UInt32) );
	
	// We don't handle non-zero block alignment, so mark the file as invalid.
	if ( offset != 0 || blockSize != 0 )
		return;
	
	//*******************************************************************************
	// Set the current read position to be the beginning of the sound data.
	
	currentSampleIndex = 0;
	validFile = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		80 Bit Float Conversion Method
//############		
//##########################################################################################
//##########################################################################################


# define UnsignedToFloat(u)         (((double)((long)(u - 2147483647L - 1))) + 2147483648.0)


Float64 AIFFDecoder:: convertFP80ToFP64( const UByte* fp80 )
{
	// Get the sign bit of the number.
	Float64 sign = (fp80[0] & 0x80) == 0 ? Float64(1) : Float64(-1);
	
	// Get the 15-bit exponent of the number.
	Int32 exponent = ((fp80[0] & 0x7F) << 8) | (fp80[1] & 0xFF);
	
	// Get the high and low-order 32-bit words of the mantissa.
	UInt32 highMantissa = ((UInt32)(fp80[2] & 0xFF) << 24) | ((UInt32)(fp80[3] & 0xFF) << 16)
						| ((UInt32)(fp80[4] & 0xFF) << 8) | ((UInt32)(fp80[5] & 0xFF));
	
	UInt32 lowMantissa = ((UInt32)(fp80[6] & 0xFF) << 24) | ((UInt32)(fp80[7] & 0xFF) << 16)
						| ((UInt32)(fp80[8] & 0xFF) << 8) | ((UInt32)(fp80[9] & 0xFF));
	
	// Check for zero value.
	if ( exponent == 0 && highMantissa == 0 && lowMantissa == 0 )
		return sign*Float64(0);
	else
	{
		// Check for infinity or NAN.
		if ( exponent == 0x7FFF )
		{
			UInt32 bits63to62 = (highMantissa & 0xC0000000) >> 30;
			UInt64 bits61to0 = (UInt64)(highMantissa & 0x30000000) << 32 | lowMantissa;
			
			// Check to see if the number is infinite.
			if ( (bits63to62 == 0 && bits61to0 == 0) || (bits63to62 == 2 && bits61to0 == 0) )
				return sign*math::infinity<Float64>();
			else
			{
				// Must be not-a-number.
				return math::nan<Float64>();
			}
		}
		else
		{
			// Bias the exponent.
			exponent -= 16383;
			
			// Accumulate the value of the number.
			Float64 result = ldexp( UnsignedToFloat(highMantissa), exponent -= 31 );
			result += ldexp( UnsignedToFloat(lowMantissa), exponent -= 32 );
			
			// Apply the sign and return.
			return sign*result;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		A-Law Decoding Method
//############		
//##########################################################################################
//##########################################################################################




Int16 AIFFDecoder:: decodeALaw( UInt8 aLaw )
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




Int16 AIFFDecoder:: decodeMuLaw( UInt8 muLaw )
{
	const Int16 MULAW_SIGN_BIT = 0x80;
	const Int16 MULAW_SEG_SHIFT = 4;
	const Int16 MULAW_SEG_MASK = 0x70;
	const UInt8 MULAW_QUANT_MASK = 0xf;
	const Int16 MULAW_BIAS = 0x84;
	
	// Complement to obtain normal u-law value.
	muLaw = ~muLaw;

	// Extract and bias the quantization bits.
	// Then shift up by the segment number and subtract out the bias.
	Int16 t = ((Int16)(muLaw & MULAW_QUANT_MASK) << 3) + MULAW_BIAS;
	t <<= ((UInt16)muLaw & MULAW_SEG_MASK) >> MULAW_SEG_SHIFT;

	return ((muLaw & MULAW_SIGN_BIT) ? (MULAW_BIAS - t) : (t - MULAW_BIAS));
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
