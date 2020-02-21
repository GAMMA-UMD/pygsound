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

#ifndef INCLUDE_OM_RESOURCE_DECODER_H
#define INCLUDE_OM_RESOURCE_DECODER_H


#include "omResourcesConfig.h"


#include "omResourceID.h"
#include "omResourceReferencePatcher.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles decoding of standard types from binary data.
class ResourceDecoder : public io::DataInputStream
{
	public:
		
		//********************************************************************************
		//******	File ID Accessor Methods
			
			
			/// Return the resource ID of the resource set file that is being decoded.
			OM_INLINE const ResourceID& getFileID() const
			{
				return fileID;
			}
			
			
			/// Set the resource ID of the resource set file that is being decoded.
			OM_INLINE void setFileID( const ResourceID& newFileID )
			{
				fileID = newFileID;
			}
			
			
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return a reference to the binary format this decoder is using.
			OM_INLINE const io::BinaryFormat& getFormat() const
			{
				return decoder.getFormat();
			}
			
			
			/// Set the binary format this decoder is using.
			OM_INLINE void setFormat( const io::BinaryFormat& newFormat )
			{
				decoder.setFormat( newFormat );
			}
			
			
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			/// Return the total size of the decoder's data source in bytes.
			OM_INLINE Size getDataSize() const
			{
				return decoder.getDataSize();
			}
			
			
			/// Set a pointer to an external data source that should be read with this decoder.
			/**
			  * This function prepares the decoder for reading from an external data source.
			  * When the decoder reaches the end of the data, it cannot read any further.
			  */
			OM_INLINE void setData( const UByte* newData, Size dataSizeInBytes )
			{
				decoder.setData( newData, dataSizeInBytes );
			}
			
			
		//********************************************************************************
		//******	Padding Methods
			
			
			/// Pad the alignment of the specified pointer to the next aligned object boundary.
			OM_INLINE void pad( const UByte*& data )
			{
				decoder.pad( data );
			}
			
			
		//********************************************************************************
		//******	Templated Type Reading Methods
			
			
			/// Read a value of the templated type into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( T& value )
			{
				return decoder.read( value );
			}
			
			
			/// Read an array of values of the templated type and advance the data pointer.
			template < typename T >
			OM_INLINE void readArray( T* array, Size number )
			{
				return decoder.readArray( array, number );
			}
			
			
		//********************************************************************************
		//******	Reference Reading Methods
			
			
			/// Read a resource reference for the specified pointer to a concrete template resource type.
			OM_INLINE void readReference( ResourceReferencePatcher& references,
											Index userType = 0, Index userID = 0, void* userData = NULL )
			{
				ResourceLocalID localID = ResourceID::INVALID_LOCAL_ID;
				decoder.read( localID );
				references.addReference( ResourceReference( ResourceID::INVALID_LOCAL_ID, localID, userType, userID, userData ) );
			}
			
			
			/// Read an opaque resource reference for the specified pointer to a concrete template resource type.
			OM_INLINE void readOpaqueReference( ResourceReferencePatcher& references,
												Index userType = 0, Index userID = 0, void* userData = NULL )
			{
				ResourceLocalID localTypeID = ResourceID::INVALID_LOCAL_ID;
				decoder.read( localTypeID );
				ResourceLocalID localID = ResourceID::INVALID_LOCAL_ID;
				decoder.read( localID );
				references.addReference( ResourceReference( localTypeID, localID, userType, userID, userData ) );
			}
			
			
		//********************************************************************************
		//******	String Reading Methods
			
			
			/// Read an ASCII encoded string starting at the specified data pointer and return it and advance the pointer.
			OM_INLINE data::String readASCIIString()
			{
				return decoder.readASCIIString();
			}
			
			
			/// Read a UTF-8 encoded string starting at the specified data pointer and return it and advance the pointer.
			OM_INLINE data::UTF8String readUTF8String()
			{
				return decoder.readUTF8String();
			}
			
			
			/// Read a UTF-16 encoded string starting at the specified data pointer and return it and advance the pointer.
			OM_INLINE data::UTF16String readUTF16String()
			{
				return decoder.readUTF16String();
			}
			
			
			/// Read a UTF-32 encoded string starting at the specified data pointer and return it and advance the pointer.
			OM_INLINE data::UTF32String readUTF32String()
			{
				return decoder.readUTF32String();
			}
			
			
		//********************************************************************************
		//******	Data Read Methods
			
			
			/// Read the specified number of bytes of data into the buffer.
			OM_INLINE Bool read( UByte* newData, Size number )
			{
				return decoder.read( newData, number );
			}
			
			
			/// Read the specified number of bytes from the stream and place them in the buffer given by a pointer.
			OM_INLINE virtual Size readData( UByte* buffer, Size number )
			{
				return decoder.readData( buffer, number );
			}
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not this type of stream allows seeking.
			OM_INLINE virtual Bool canSeek() const
			{
				return decoder.canSeek();
			}
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			OM_INLINE virtual Bool canSeek( Int64 relativeOffset ) const
			{
				return decoder.canSeek( relativeOffset );
			}
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			OM_INLINE virtual Int64 seek( Int64 relativeOffset )
			{
				return decoder.seek( relativeOffset );
			}
			
			
		//********************************************************************************
		//******	Read Position Accessor Methods
			
			
			/// Return the number of bytes remaining in the stream.
			OM_INLINE virtual LargeSize getBytesRemaining() const
			{
				return decoder.getBytesRemaining();
			}
			
			
			/// Return the current byte index within the stream relative to the beginning.
			OM_INLINE virtual LargeIndex getPosition() const
			{
				return decoder.getPosition();
			}
			
			
		//********************************************************************************
		//******	Stream Duplication Method
			
			
			/// Construct and return a copy of this resource decoder in same state as this one.
			/**
			  * This is used to create persistent decoders that can be used to stream
			  * resources into memory on demand.
			  */
			Shared<DataInputStream> duplicate() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The binary decoder that is decoding resources for this resource encoder.
			io::BinaryDecoder decoder;
			
			
			/// The resource ID of the file that is being decoded.
			ResourceID fileID;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_DECODER_H
