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

#ifndef INCLUDE_OM_SOUND_HRTF_TRANSCODER_H
#define INCLUDE_OM_SOUND_HRTF_TRANSCODER_H


#include "omSoundIOConfig.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encodes and decodes the HRTF format.
/**
  * Version 1 of the HRTF binary format.
  *
  * Primitive types:
  * - float64 - double-precision IEEE 754 floating point number.
  * - float32 - single-precision IEEE 754 floating point number.
  * - uint8 - 8-bit unsigned integer.
  * - uint64 - 64-bit unsigned integer.
  * - string - UTF-8 encoded string, given by a uint64 length followed by that number of UTF8 uint8 code points,
  *   padded to the nearest 4-byte boundary.
  *
  * The version-independent 16-byte header for the file is as follows:
  * - Bytes 0 through 8: The ASCII character string 'SOUNDHRTF'.
  * - Byte 9: uint8 indicating the version number of the HRTF format.
  * - Byte 10: uint8 that specifies the endianness of the file, 0 if little endian, 1 if big endian.
  * - Byte 11: reserved/padding.
  * - Bytes 12 through 15: uint32 indicating a checksum of the rest of the file
  *   after the header. The checksum is computed by summing the value of every byte in the file as
  *   interpreted as 8-bit integers and allowing overflow. This value is provided to detect corruption
  *   of the file. A value of 0 indicates that the checksum is not used.
  *
  * The version-specific HRTF header contains basic information about the HRTF:
  * - sizeInBytes: uint64 specifying the total encoded size in bytes of the header.
  * - numChannels: uint64 specifying the number of channels in the HRTF.
  * - sampleRate: float64 specifying the sample rate of the HRTF in hertz.
  * - orientation: 9*float32 specifying a 3x3 orthonormal rotation matrix that is applied to the HRTF.
  * - name: string specifying the name of the person the HRTF was measured for.
  * - description: string containing a description of the HRTF and/or how it was measured.
  * - organization: string containing the name of the organization that created the HRTF.
  * - license: string containing the license of the HRTF.
  *
  * The channels for the HRTF: (per channel)
  * - sizeInBytes: uint64 specifying the total encoded size in bytes of this channel.
  * - numSamples: uint64 specifying the number of impulse response samples in this channel.
  * - position: 3*float32 cartesian coordinate of the channel relative to the listener, in meters.
  * - name: string specifying the name of the channel.
  * - samples: numSamples samples for the channel.
  *
  * Each IR sample for a channel:
  * - sizeInBytes: uint64 specifying the total encoded size in bytes of this sample.
  * - direction: 3*float32 unit-length cartesian direction of the IR sample relative to the channel's position.
  * - sampleDelay: uint64 indicating the broadband delay in samples for this IR measurement.
  * - irLength: uint64 indicating the length of the IR in samples.
  * - ir: irLength*float32 samples for the IR.
  */
class HRTFTranscoder : public ResourceTypeTranscoder<HRTF>
{
	public:
		
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return an object that represents the resource format that this transcoder can read and write.
			virtual const ResourceFormat& getResourceFormat() const;
			
			
		//********************************************************************************
		//******	Encoding Methods
			
			
			/// Return whether or not this transcoder is able to encode the specified resource.
			virtual Bool canEncode( const HRTF& hrtf ) const;
			
			
			/// Save the specified HRTF at the specified ID location.
			/**
			  * The method returns whether or not the resource was successfully written.
			  */
			virtual Bool encode( const ResourceID& identifier, const HRTF& hrtf,
								const ResourceSet* resourceSet = NULL, ResourceManager* manager = NULL );
			
			
		//********************************************************************************
		//******	Decoding Methods
			
			
			/// Return whether or not the specified identifier refers to a valid resource for this transcoder.
			/**
			  * If the identifier represents a valid resource, TRUE is returned. Otherwise,
			  * if the resource is not valid, FALSE is returned.
			  */
			virtual Bool canDecode( const ResourceID& identifier ) const;
			
			
			/// Load the HRTF pointed to by the specified identifier.
			/**
			  * The caller can supply a pointer to a resource manager which can be used
			  * to manage the creation of child resources.
			  *
			  * If the method fails, the return value will be NULL.
			  */
			virtual HRTF* decode( ResourceID& identifier, ResourceSet* resourceSet = NULL,
									ResourceManager* manager = NULL );
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The resource format for HRTFs.
			static const ResourceFormat FORMAT;
			
			
	private:
		
		//********************************************************************************
		//******	Reading Helper Methods
			
			
			/// Decode an HRTF from the specified data stream.
			static HRTF* decodeFromStream( DataInputStream& stream );
			
			
			/// Decode a version 1 HRTF from the given data stream and endianness, after the initial header has been parsed.
			static HRTF* decodeFromStreamV1( DataInputStream& stream, const Endianness& endianness );
			
			
			/// Read a version 1 header from the given data stream and endianness.
			static HRTF* readHeaderV1( DataInputStream& stream, const Endianness& endianness,
										UByte*& dataBuffer, Size& dataBufferSize );
			
			
			/// Read a version 1 channel from the given data stream and endianness.
			OM_FORCE_INLINE static Bool readChannelV1( DataInputStream& stream, const Endianness& endianness,
														UByte*& dataBuffer, Size& dataBufferSize, HRTF& hrtf, Index channelIndex );
			
			
			/// Read a 64-bit unsigned integer from the given data pointer, converting endianness.
			OM_FORCE_INLINE static void readUInt64( const UByte*& data, const Endianness& endianness, UInt64& value );
			
			
			/// Read a 32-bit float from the given data pointer, converting endianness.
			OM_FORCE_INLINE static void readFloat32( const UByte*& data, const Endianness& endianness, Float32& value );
			
			
			/// Read a 64-bit float from the given data pointer, converting endianness.
			OM_FORCE_INLINE static void readFloat64( const UByte*& data, const Endianness& endianness, Float64& value );
			
			
			/// Read a UTF-8 string from the given data pointer, converting endianness.
			OM_FORCE_INLINE static UTF8String readString( const UByte*& data, const Endianness& endianness );
			
			
		//********************************************************************************
		//******	Writing Helper Methods
			
			
			/// Encode an HRTF to the given data output stream.
			static Bool encodeToStream( const HRTF& hrtf, DataOutputStream& stream );
			
			/// Encode an HRTF header to the given data output stream.
			static Bool writeHeader( const HRTF& hrtf, UByte*& dataBuffer, Size& dataBufferSize,
									DataOutputStream& stream );
			
			/// Encode an HRTF channel to the given data output stream.
			static Bool writeChannel( const HRTF& hrtf, Index channelIndex,
									UByte*& dataBuffer, Size& dataBufferSize,
									DataOutputStream& stream );
			
			/// Encode an HRTF sample to the given data output stream.
			static Bool writeSample( const HRTF& hrtf, Index channelIndex, Index sampleIndex,
									UByte*& dataBuffer, Size& dataBufferSize,
									DataOutputStream& stream );
			
			
			/// Write a 64-bit unsigned integer to the given data pointer.
			OM_FORCE_INLINE static void writeUInt64( UByte*& data, UInt64 value );
			
			
			/// Write a 32-bit float to the given data pointer.
			OM_FORCE_INLINE static void writeFloat32( UByte*& data, Float32 value );
			
			
			/// Write a 64-bit float to the given data pointer.
			OM_FORCE_INLINE static void writeFloat64( UByte*& data, Float64 value );
			
			
			/// Write a UTF-8 string to the given data pointer.
			OM_FORCE_INLINE static void writeString( UByte*& data, const UTF8String& string );
			
			
		//********************************************************************************
		//******	Temporary Buffer Helper Methods
			
			
			/// Enlarge a temporary buffer so that it can hold the specified number of bytes.
			OM_FORCE_INLINE static void enlargeBuffer( UByte*& data, Size& dataSize, Size desiredSize );
			
			/// Release the memory used by the specified temporary buffer.
			OM_FORCE_INLINE static void releaseBuffer( const UByte* data );
			
			
			/// Return the size in bytes of the specified HRTF channel when it is encoded.
			OM_FORCE_INLINE static Size getChannelHeaderSize( const HRTF& hrtf, Index channelIndex );
			
			/// Return the size in bytes of the specified HRTF channel when it is encoded.
			OM_FORCE_INLINE static Size getChannelSize( const HRTF& hrtf, Index channelIndex );
			
			/// Return the size in bytes of the specified HRTF channel sample when it is encoded.
			OM_FORCE_INLINE static Size getSampleSize( const HRTF& hrtf, Index channelIndex, Index sampleIndex );
			
			
			/// Return the size in bytes of the specified string when it is encoded.
			OM_FORCE_INLINE static Size getStringSize( const UTF8String& string );
			
			
			
};




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_HRTF_TRANSCODER_H
