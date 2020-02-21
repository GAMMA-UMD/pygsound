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

#include "omSoundHRTFTranscoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const ResourceFormat HRTFTranscoder:: FORMAT("HRTF","hrtf");


//##########################################################################################
//##########################################################################################
//############		
//############		Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




const ResourceFormat& HRTFTranscoder:: getResourceFormat() const
{
	return FORMAT;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFTranscoder:: canEncode( const HRTF& hrtf ) const
{
	return true;
}




Bool HRTFTranscoder:: encode( const ResourceID& identifier, const HRTF& hrtf,
								const ResourceSet* resourceSet, ResourceManager* manager )
{
	if ( !canEncode( hrtf ) )
		return false;
	
	FileWriter writer( identifier.getURL() );
	
	if ( !writer.open() || !writer.erase() )
		return false;
	
	// Save this mesh to the file.
	Bool result = encodeToStream( hrtf, writer );
	
	// Close the file.
	writer.close();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFTranscoder:: canDecode( const ResourceID& resourceID ) const
{
	return resourceID.getType() == ResourceType::of<HRTF>() && 
			(resourceID.getFormat() == ResourceFormat::UNDEFINED || resourceID.getFormat() == FORMAT);
}




HRTF* HRTFTranscoder:: decode( ResourceID& identifier, ResourceSet* resourceSet, ResourceManager* manager )
{
	if ( !canDecode( identifier ) )
		return NULL;
	
	FileReader reader( identifier.getURL() );
	
	if ( !reader.open() )
		return NULL;
	
	HRTF* result = decodeFromStream( reader );
	identifier.setName( result->getName() );
	
	reader.close();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decode from Stream Method
//############		
//##########################################################################################
//##########################################################################################




HRTF* HRTFTranscoder:: decodeFromStream( DataInputStream& stream )
{
	//***************************************************************************
	// Read the header.
	
	const Size headerDataSize = 16;
	UByte headerData[headerDataSize];
	
	if ( stream.readData( headerData, headerDataSize ) < headerDataSize )
		return NULL;
	
	// Check the format code
	if ( headerData[0] != 'S' || headerData[1] != 'O' || headerData[2] != 'U' || headerData[3] != 'N' ||
		headerData[4] != 'D' || headerData[5] != 'H' || headerData[6] != 'R' || headerData[7] != 'T' ||
		headerData[8] != 'F' )
		return NULL;
	
	// Get the version and endianness of the file.
	Size version = headerData[9];
	Endianness endianness = headerData[10] ? Endianness::BIG : Endianness::LITTLE;
	
	//***************************************************************************
	// Read the rest of the file.
	
	switch ( version )
	{
		case 1:
			return decodeFromStreamV1( stream, endianness );
	}
	
	return NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version 1 Load Method
//############		
//##########################################################################################
//##########################################################################################




HRTF* HRTFTranscoder:: decodeFromStreamV1( DataInputStream& stream, const Endianness& endianness )
{
	// Create a temporary data buffer for decoding.
	Size dataBufferSize = 0;
	UByte* dataBuffer = NULL;
	
	// Read the header and create a new HRTF object.
	HRTF* hrtf = readHeaderV1( stream, endianness, dataBuffer, dataBufferSize );
	
	if ( !hrtf )
		return NULL;
	
	//***************************************************************************
	// Decode the channel data.
	
	const Size numChannels = hrtf->getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
		readChannelV1( stream, endianness, dataBuffer, dataBufferSize, *hrtf, c );
	
	//***************************************************************************
	
	// Clean up the temporary data buffer.
	releaseBuffer( dataBuffer );
	
	return hrtf;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Header Reading Method
//############		
//##########################################################################################
//##########################################################################################




HRTF* HRTFTranscoder:: readHeaderV1( DataInputStream& stream, const Endianness& endianness,
									UByte*& dataBuffer, Size& dataBufferSize )
{
	// Read the size of the header data from the stream.
	UInt64 headerDataSize = 0;
	stream.readData( (UByte*)&headerDataSize, sizeof(UInt64) );
	headerDataSize = endianness.convertToNative( headerDataSize ) - sizeof(UInt64);
	
	// Make sure the temporary data buffer is big enough.
	enlargeBuffer( dataBuffer, dataBufferSize, headerDataSize );
	
	// Read the header data from the stream.
	if ( stream.readData( dataBuffer, headerDataSize ) < headerDataSize )
		return NULL;
	
	//***************************************************************************
	// Decode the header data.
	
	const UByte* readPosition = dataBuffer;
	
	// Read the number of channels.
	UInt64 numChannels = 0;
	readUInt64( readPosition, endianness, numChannels );
	
	// Read the sample rate.
	Float64 sampleRate = 0;
	readFloat64( readPosition, endianness, sampleRate );
	
	// Create a new HRTF result.
	HRTF* hrtf = om::util::construct<HRTF>( (Size)numChannels, 0, sampleRate, HRTF::TIME_DOMAIN );
	
	// Read the orientation matrix in column-major order.
	Float32 o[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	
	for ( Index i = 0; i < 9; i++ )
		readFloat32( readPosition, endianness, o[i] );
	
	hrtf->setOrientation( Matrix3f( o ) );
	
	// Read the name, description, organization, and license strings of the HRTF.
	UTF8String name = readString( readPosition, endianness );
	UTF8String description = readString( readPosition, endianness );
	UTF8String organization = readString( readPosition, endianness );
	UTF8String license = readString( readPosition, endianness );
	hrtf->setName( name );
	hrtf->setDescription( description );
	//hrtf->setName( readString( readPosition, endianness ) );
	//hrtf->setDescription( readString( readPosition, endianness ) );
	//hrtf->setOrganization( readString( readPosition, endianness ) );
	//hrtf->setLicense( readString( readPosition, endianness ) );
	
	return hrtf;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Reading Method
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFTranscoder:: readChannelV1( DataInputStream& stream, const Endianness& endianness,
									UByte*& dataBuffer, Size& dataBufferSize, HRTF& hrtf, Index channelIndex )
{
	// Read the size of the channel data from the stream.
	UInt64 channelDataSize = 0;
	stream.readData( (UByte*)&channelDataSize, sizeof(UInt64) );
	channelDataSize = endianness.convertToNative( channelDataSize ) - sizeof(UInt64);
	
	// Make sure the temporary data buffer is big enough.
	enlargeBuffer( dataBuffer, dataBufferSize, channelDataSize );
	
	// Read the channel data from the stream.
	if ( stream.readData( dataBuffer, channelDataSize ) < channelDataSize )
		return NULL;
	
	//***************************************************************************
	// Decode the channel data.
	
	const UByte* readPosition = dataBuffer;
	
	// Read the number of samples.
	UInt64 numSamples = 0;
	readUInt64( readPosition, endianness, numSamples );
	
	// Read the position.
	Vector3f position;
	readFloat32( readPosition, endianness, position.x );
	readFloat32( readPosition, endianness, position.y );
	readFloat32( readPosition, endianness, position.z );
	//hrtf.setChannelPosition( channelIndex, position );
	
	// Read the name of the channel.
	readString( readPosition, endianness );
	//hrtf.setChannelName( channelIndex, readString( readPosition, endianness ) );
	
	// Create another temporary data buffer to hold the sample data before endian conversion.
	UByte* sampleDataBuffer = NULL;
	Size sampleDataBufferSize = 0;
	
	// Read the samples for the channel.
	for ( UInt64 i = 0; i < numSamples; i++ )
	{
		// Read the size in bytes of the sample.
		UInt64 sampleDataSize;
		readUInt64( readPosition, endianness, sampleDataSize );
		
		// Read the direction of the sample.
		Vector3f direction;
		readFloat32( readPosition, endianness, direction.x );
		readFloat32( readPosition, endianness, direction.y );
		readFloat32( readPosition, endianness, direction.z );
		
		// Read the sample delay of the sample.
		UInt64 sampleDelay;
		readUInt64( readPosition, endianness, sampleDelay );
		
		// Read the length of the IR for this sample.
		UInt64 irLength;
		readUInt64( readPosition, endianness, irLength );
		
		if ( hrtf.getLength() != irLength )
			hrtf.setLength( irLength );
		
		// Make sure the temporary data buffer is big enough.
		enlargeBuffer( sampleDataBuffer, sampleDataBufferSize, irLength*sizeof(Float32) );
		Float32* irSamples = (Float32*)sampleDataBuffer;
		
		// Read each of the samples in the IR.
		for ( UInt64 j = 0; j < irLength; j++ )
			readFloat32( readPosition, endianness, irSamples[j] );
		
		// Add the sample to the HRTF.
		hrtf.addSample( channelIndex, direction, Float(sampleDelay / hrtf.getSampleRate()), irSamples );
	}
	
	// Clean up the sample data buffer.
	releaseBuffer( sampleDataBuffer );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Encoding Method
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFTranscoder:: encodeToStream( const HRTF& hrtf, DataOutputStream& stream )
{
	//***************************************************************************
	// Write the base header.
	
	// Make the header.
	UByte header[16] =
	{
		'S','O','U','N','D','H','R','T','F', // format specifier.
		1, // format version
#if defined(OM_BIG_ENDIAN)
		1, // endianness
#else
		0, // endianness
#endif
		0, // padding.
		0, 0, 0, 0 // 32-bit checksum.
	};
	
	// Write the header to the stream.
	stream.writeData( header, 16 );
	
	// Initialize a temporary data buffer that is used for encoding.
	Size dataBufferSize = 0;
	UByte* dataBuffer = NULL;
	
	//***************************************************************************
	// Write the HRTF header.
	
	if ( !writeHeader( hrtf, dataBuffer, dataBufferSize, stream ) )
		return false;
	
	//***************************************************************************
	// Write the channels in the HRTF.
	
	const Size numChannels = hrtf.getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
		writeChannel( hrtf, c, dataBuffer, dataBufferSize, stream );
	
	//***************************************************************************
	// Clean up the temporary buffer.
	
	releaseBuffer( dataBuffer );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Header Writing Method
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFTranscoder:: writeHeader( const HRTF& hrtf, UByte*& dataBuffer, Size& dataBufferSize,
									DataOutputStream& stream )
{
	// Determine the size of the header data.
	const Size headerDataSize = 2*sizeof(UInt64) + sizeof(Float64) + 9*sizeof(Float32) +
								getStringSize( UTF8String()/*hrtf.getName()*/ ) +
								getStringSize( UTF8String()/*hrtf.getDescription()*/ ) + 
								getStringSize( UTF8String()/*hrtf.getOrganization()*/ ) + 
								getStringSize( UTF8String()/*hrtf.getLicense()*/ );
	
	// Make sure the temporary data buffer is big enough.
	enlargeBuffer( dataBuffer, dataBufferSize, headerDataSize );
	UByte* writePosition = dataBuffer;
	
	//***************************************************************************
	
	// Write the size of the header data.
	writeUInt64( writePosition, (UInt64)headerDataSize );
	
	// Write the number of channels in the HRTF.
	writeUInt64( writePosition, (UInt64)hrtf.getChannelCount() );
	
	// Write the sample rate.
	writeFloat64( writePosition, (Float64)hrtf.getSampleRate() );
	
	// Write the orientation matrix.
	const Float32* o = hrtf.getOrientation().toArrayColumnMajor();
	
	for ( Index i = 0; i < 9; i++ )
		writeFloat32( writePosition, o[i] );
	
	// Write the name, description, organization, and license strings.
	writeString( writePosition, UTF8String()/*hrtf.getName()*/ );
	writeString( writePosition, UTF8String()/*hrtf.getDescription()*/ );
	writeString( writePosition, UTF8String()/*hrtf.getOrganization()*/ );
	writeString( writePosition, UTF8String()/*hrtf.getLicense()*/ );
	
	//***************************************************************************
	
	// Write the header to the file.
	stream.writeData( dataBuffer, headerDataSize );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Writing Method
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFTranscoder:: writeChannel( const HRTF& hrtf, Index channelIndex, UByte*& dataBuffer, Size& dataBufferSize,
									DataOutputStream& stream )
{
	// Determine the size of the channel header data.
	Size channelHeaderSize = getChannelHeaderSize( hrtf, channelIndex );
	
	// Make sure the temporary data buffer is big enough.
	enlargeBuffer( dataBuffer, dataBufferSize, channelHeaderSize );
	UByte* writePosition = dataBuffer;
	
	//***************************************************************************
	// Write the channel header
	
	// Write the channel data size.
	writeUInt64( writePosition, (UInt64)getChannelSize( hrtf, channelIndex ) );
	
	// Write the number of samples.
	const Size numSamples = hrtf.getSampleCount(channelIndex);
	writeUInt64( writePosition, (UInt64)numSamples );
	
	// Write the position of the channel.
	//const Vector3f& position = hrtf.getChannelPosition(channelIndex);
	Vector3f position;
	writeFloat32( writePosition, position.x );
	writeFloat32( writePosition, position.y );
	writeFloat32( writePosition, position.z );
	
	// Write the name.
	writeString( writePosition, UTF8String(hrtf.getChannelInfo(channelIndex).getType().toString()) );
	
	// Write the channel header to the file.
	stream.writeData( dataBuffer, channelHeaderSize );
	
	//***************************************************************************
	// Write the samples that are part of this channel.
	
	for ( Index i = 0; i < numSamples; i++ )
	{
		if ( !writeSample( hrtf, channelIndex, i, dataBuffer, dataBufferSize, stream ) )
			return false;
	}
	
	//***************************************************************************
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Writing Method
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFTranscoder:: writeSample( const HRTF& hrtf, Index channelIndex, Index sampleIndex,
									UByte*& dataBuffer, Size& dataBufferSize,
									DataOutputStream& stream )
{
	// Determine the size of the sample data.
	Size sampleDataSize = getSampleSize( hrtf, channelIndex, sampleIndex );
	
	// Make sure the temporary data buffer is big enough.
	enlargeBuffer( dataBuffer, dataBufferSize, sampleDataSize );
	UByte* writePosition = dataBuffer;
	
	//***************************************************************************
	// Write the sample header
	
	const Vector3f& direction = hrtf.getSampleDirection( channelIndex, sampleIndex );
	const Size sampleDelay = Size(hrtf.getSampleDelay( channelIndex, sampleIndex )*hrtf.getSampleRate());
	const Size irLength = hrtf.getFilterLength();
	const Float32* ir = hrtf.getSampleData( channelIndex, sampleIndex );
	
	// Write the sample data size.
	writeUInt64( writePosition, (UInt64)sampleDataSize );
	
	// Write the sample direction.
	writeFloat32( writePosition, direction.x );
	writeFloat32( writePosition, direction.y );
	writeFloat32( writePosition, direction.z );
	
	// Write the sample delay.
	writeUInt64( writePosition, (UInt64)sampleDelay );
	
	// Write the IR length
	writeUInt64( writePosition, (UInt64)irLength );
	
	// Write the IR samples.
	for ( Index j = 0; j < irLength; j++ )
		writeFloat32( writePosition, ir[j] );
	
	//***************************************************************************
	
	// Write the channel header to the file.
	stream.writeData( dataBuffer, sampleDataSize );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reading Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void HRTFTranscoder:: readUInt64( const UByte*& data, const Endianness& endianness, UInt64& value )
{
	value = endianness.convertToNative( *(UInt64*)data );
	data += sizeof(UInt64);
}




void HRTFTranscoder:: readFloat32( const UByte*& data, const Endianness& endianness, Float32& value )
{
	value = endianness.convertToNative( *(Float32*)data );
	data += sizeof(Float32);
}




void HRTFTranscoder:: readFloat64( const UByte*& data, const Endianness& endianness, Float64& value )
{
	value = endianness.convertToNative( *(Float64*)data );
	data += sizeof(Float64);
}




UTF8String HRTFTranscoder:: readString( const UByte*& data, const Endianness& endianness )
{
	// Read the string's length.
	UInt64 stringLength = 0;
	readUInt64( data, endianness, stringLength );
	
	const Size paddedLength = stringLength % sizeof(UInt64) == 0 ? stringLength :
								math::nextMultiple( (Size)stringLength, (Size)sizeof(UInt32) );
	
	// Read the string's characters.
	UTF8String result( data, stringLength );
	data += paddedLength*sizeof(UTF8Char);
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Writing Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void HRTFTranscoder:: writeUInt64( UByte*& data, UInt64 value )
{
	// Native endianness.
	*(UInt64*)data = value;
	data += sizeof(UInt64);
}




void HRTFTranscoder:: writeFloat32( UByte*& data, Float32 value )
{
	// Native endianness.
	*(Float32*)data = value;
	data += sizeof(Float32);
}




void HRTFTranscoder:: writeFloat64( UByte*& data, Float64 value )
{
	// Native endianness.
	*(Float64*)data = value;
	data += sizeof(Float64);
}




void HRTFTranscoder:: writeString( UByte*& data, const UTF8String& string )
{
	const Size stringLength = string.getLength();
	const Size paddedLength = stringLength % sizeof(UInt64) == 0 ? stringLength :
								math::nextMultiple( stringLength, (Size)sizeof(UInt32) );
	
	writeUInt64( data, stringLength );
	om::util::copyPOD( data, string.getCString(), stringLength );
	
	if ( stringLength != paddedLength )
		om::util::zero( data + stringLength, paddedLength - stringLength );
	
	data += paddedLength*sizeof(UTF8Char);
	
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void HRTFTranscoder:: enlargeBuffer( UByte*& data, Size& dataSize, Size desiredSize )
{
	if ( dataSize < desiredSize )
	{
		if ( data )
			om::util::deallocate( data );
		
		data = om::util::allocate<UByte>( desiredSize );
		dataSize = desiredSize;
	}
}




void HRTFTranscoder:: releaseBuffer( const UByte* data )
{
	if ( data )
		om::util::deallocate( data );
}




Size HRTFTranscoder:: getChannelHeaderSize( const HRTF& hrtf, Index channelIndex )
{
	return 3*sizeof(Float32) + 2*sizeof(UInt64) + getStringSize( UTF8String(hrtf.getChannelInfo(channelIndex).getType().toString()) );
}




Size HRTFTranscoder:: getChannelSize( const HRTF& hrtf, Index channelIndex )
{
	Size channelDataSize = getChannelHeaderSize( hrtf, channelIndex );
	const Size numSamples = hrtf.getSampleCount(channelIndex);
	
	for ( Index i = 0; i < numSamples; i++ )
		channelDataSize += getSampleSize( hrtf, channelIndex, i );
	
	return channelDataSize;
}




Size HRTFTranscoder:: getSampleSize( const HRTF& hrtf, Index channelIndex, Index sampleIndex )
{
	return 3*sizeof(Float32) + 3*sizeof(UInt64) + hrtf.getFilterLength();
}




Size HRTFTranscoder:: getStringSize( const UTF8String& string )
{
	const Size stringLength = string.getLength();
	const Size paddedLength = stringLength % sizeof(UInt64) == 0 ? stringLength :
								math::nextMultiple( stringLength, (Size)sizeof(UInt32) );
	
	return sizeof(UInt64) + paddedLength*sizeof(UTF8Char);
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
