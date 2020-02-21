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

#ifndef INCLUDE_OM_RESOURCE_ENCODER_H
#define INCLUDE_OM_RESOURCE_ENCODER_H


#include "omResourcesConfig.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles encoding of resources to binary data.
class ResourceEncoder : public io::DataOutputStream
{
	public:
		
		//********************************************************************************
		//******	File ID Accessor Methods
			
			
			/// Return the resource ID of the resource set file that is being encoded.
			OM_INLINE const ResourceID& getFileID() const
			{
				return fileID;
			}
			
			
			/// Set the resource ID of the resource set file that is being encoded.
			OM_INLINE void setFileID( const ResourceID& newFileID )
			{
				fileID = newFileID;
			}
			
			
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return a reference to the binary format this encoder is using.
			OM_INLINE const io::BinaryFormat& getFormat() const
			{
				return encoder.getFormat();
			}
			
			
			/// Set the binary format this encoder is using.
			OM_INLINE void setFormat( const io::BinaryFormat& newFormat )
			{
				encoder.setFormat( newFormat );
			}
			
			
		//********************************************************************************
		//******	Stream Accessor Methods
			
			
			/// Return a pointer to a data stream that is the destination of this encoder.
			/**
			  * If the stream is NULL, the encoder keeps enlarging its internal buffer as necessary.
			  */
			OM_INLINE io::DataOutputStream* getStream() const
			{
				return encoder.getStream();
			}
			
			
			/// Set a pointer to a data stream that is the destination of this encoder.
			/**
			  * If the stream is NULL, the encoder keeps enlarging its internal buffer as necessary.
			  */
			OM_INLINE void setStream( io::DataOutputStream* newStream )
			{
				encoder.setStream( newStream );
			}
			
			
		//********************************************************************************
		//******	Buffer Accessor Methods
			
			
			/// Return a pointer to the start of the data encoding buffer.
			OM_INLINE const UByte* getBufferData() const
			{
				return encoder.getBufferData();
			}
			
			
			/// Return the number of bytes that have been written to the buffer.
			OM_INLINE Size getBufferSize() const
			{
				return encoder.getBufferSize();
			}
			
			
			/// Return the total capacity of the internal buffer.
			OM_INLINE Size getBufferCapacity() const
			{
				return encoder.getBufferCapacity();
			}
			
			
			/// Set the total capacity of the internal buffer.
			/**
			  * The method returns whether or not the capacity was successfully changed.
			  */
			OM_INLINE Bool setBufferCapacity( Size newCapacity )
			{
				return encoder.setBufferCapacity( newCapacity );
			}
			
			
		//********************************************************************************
		//******	Padding Methods
			
			
			/// Pad the alignment of the write position to the next aligned object boundary.
			OM_INLINE void pad()
			{
				encoder.pad();
			}
			
			
		//********************************************************************************
		//******	Template Type Writing Methods
			
			
			/// Write a value of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const T& value )
			{
				encoder.write( value );
			}
			
			
			/// Write an array of values of the templated type and advance the data pointer.
			template < typename T >
			OM_INLINE void writeArray( const T* array, Size number )
			{
				encoder.writeArray( array, number );
			}
			
			
		//********************************************************************************
		//******	Reference Writing Methods
			
			
			/// Write a resource reference for the specified pointer to a concrete template resource type.
			template < typename T >
			OM_INLINE void writeReference( const T* resource, const ResourceSet& resourceSet )
			{
				encoder.write( resourceSet.getResourceLocalID( resource ) );
			}
			
			
			/// Write a resource reference for the specified opaque resource.
			OM_INLINE void writeOpaqueReference( const OpaqueResource& resource, const ResourceSet& resourceSet )
			{
				// Determine the local type ID for the resource.
				const ResourceID* resourceID = resource.getID();
				ResourceLocalID localTypeID;
				ResourceLocalID localID;
				
				if ( resourceID && 
					resourceSet.getResourceLocalIDs( resourceID->getType(), resource.getData(), localTypeID, localID ) )
				{
					// Write type type ID and resource ID.
					encoder.write( localTypeID );
					encoder.write( localID );
				}
				else
				{
					// Write an invalid type ID and resource ID.
					encoder.write( ResourceID::INVALID_LOCAL_ID );
					encoder.write( ResourceID::INVALID_LOCAL_ID );
				}
			}
			
			
			/// Write a resource reference for the specified opaque resource.
			OM_INLINE void writeOpaqueReference( const ResourceType& type, const void* resource, const ResourceSet& resourceSet )
			{
				// Determine the local type ID for the resource.
				ResourceLocalID localTypeID;
				ResourceLocalID localID;
				
				if ( resourceSet.getResourceLocalIDs( type, resource, localTypeID, localID ) )
				{
					// Write type type ID and resource ID.
					encoder.write( localTypeID );
					encoder.write( localID );
				}
				else
				{
					// Write an invalid type ID and resource ID.
					encoder.write( ResourceID::INVALID_LOCAL_ID );
					encoder.write( ResourceID::INVALID_LOCAL_ID );
				}
			}
			
			
		//********************************************************************************
		//******	String Writing Methods
			
			
			/// Write an ASCII encoded string starting at the specified data pointer and return it and advance the position.
			OM_INLINE void writeASCIIString( const data::String& string )
			{
				encoder.writeASCIIString( string );
			}
			
			
			/// Write a UTF-8 encoded string starting at the specified data pointer and return it and advance the position.
			OM_INLINE void writeUTF8String( const data::UTF8String& string )
			{
				encoder.writeUTF8String( string );
			}
			
			
			/// Write a UTF-16 encoded string starting at the specified data pointer and return it and advance the position.
			OM_INLINE void writeUTF16String( const data::UTF16String& string )
			{
				encoder.writeUTF16String( string );
			}
			
			
			/// Write a UTF-32 encoded string starting at the specified data pointer and return it and advance the position.
			OM_INLINE void writeUTF32String( const data::UTF32String& string )
			{
				encoder.writeUTF32String( string );
			}
			
			
		//********************************************************************************
		//******	Data Write Methods
			
			
			/// Write the specified number of bytes of data from the buffer to the stream.
			OM_INLINE Bool write( const UByte* newData, Size number )
			{
				return encoder.write( newData, number );
			}
			
			
			/// Write the specified number of bytes of data from the buffer to the stream.
			/**
			  * The method returns the number of bytes that were actually written.
			  */
			OM_INLINE virtual Size writeData( const UByte* newData, Size number )
			{
				return encoder.writeData( newData, number );
			}
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not this type of stream allows seeking.
			OM_INLINE virtual Bool canSeek() const
			{
				return encoder.canSeek();
			}
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			OM_INLINE virtual Bool canSeek( Int64 relativeOffset ) const
			{
				return encoder.canSeek( relativeOffset );
			}
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			OM_INLINE virtual Int64 seek( Int64 relativeOffset )
			{
				return encoder.seek( relativeOffset );
			}
			
			
		//********************************************************************************
		//******	Flush Method
			
			
			/// Flush buffered output to the stream if the encoder has a valid stream.
			OM_INLINE virtual void flush()
			{
				encoder.flush();
			}
			
			
		//********************************************************************************
		//******	Write Position Accessor Method
			
			
			/// Return the current write position of the encoder relative to the start of the output.
			OM_INLINE virtual LargeIndex getPosition() const
			{
				return encoder.getPosition();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The binary encoder that is encoding resources for this resource encoder.
			io::BinaryEncoder encoder;
			
			
			/// The resource ID of the file that is being decoded.
			ResourceID fileID;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RESOURCE_ENCODER_H
