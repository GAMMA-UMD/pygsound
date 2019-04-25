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

#ifndef INCLUDE_OM_BINARY_ENCODER_H
#define INCLUDE_OM_BINARY_ENCODER_H


#include "omIOConfig.h"


#include "omBinaryFormat.h"
#include "omDataOutputStream.h"
#include "../omMath.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles encoding of standard types to binary data.
class BinaryEncoder : public DataOutputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new binary encoder that uses the native endianness for primitive types.
			BinaryEncoder();
			
			
			/// Create a copy of another binary encoder and its current state.
			BinaryEncoder( const BinaryEncoder& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a binary encoder and release its internal data.
			~BinaryEncoder();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the current state of one binary encoder to this one.
			BinaryEncoder& operator = ( const BinaryEncoder& other );
			
			
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return a reference to the binary format this encoder is using.
			OM_INLINE const BinaryFormat& getFormat() const
			{
				return format;
			}
			
			
			/// Set the binary format this encoder is using.
			OM_INLINE void setFormat( const BinaryFormat& newFormat )
			{
				format = newFormat;
			}
			
			
		//********************************************************************************
		//******	Stream Accessor Methods
			
			
			/// Return a pointer to a data stream that is the destination of this encoder.
			/**
			  * If the stream is NULL, the encoder keeps enlarging its internal buffer as necessary.
			  */
			OM_INLINE DataOutputStream* getStream() const
			{
				return stream;
			}
			
			
			/// Set a pointer to a data stream that is the destination of this encoder.
			/**
			  * If the stream is NULL, the encoder keeps enlarging its internal buffer as necessary.
			  */
			OM_INLINE void setStream( DataOutputStream* newStream )
			{
				stream = newStream;
				streamPosition = 0;
			}
			
			
		//********************************************************************************
		//******	Buffer Accessor Methods
			
			
			/// Return a pointer to the start of the data encoding buffer.
			OM_INLINE const UByte* getBufferData() const
			{
				return dataStart;
			}
			
			
			/// Return the number of bytes that have been written to the buffer.
			OM_INLINE Size getBufferSize() const
			{
				return data - dataStart;
			}
			
			
			/// Return the total capacity of the internal buffer.
			OM_INLINE Size getBufferCapacity() const
			{
				return dataEnd - dataStart;
			}
			
			
			/// Set the total capacity of the internal buffer.
			/**
			  * The method returns whether or not the capacity was successfully changed.
			  */
			Bool setBufferCapacity( Size newCapacity );
			
			
		//********************************************************************************
		//******	Padding Methods
			
			
			/// Pad the alignment of the write position to the next aligned object boundary.
			OM_INLINE void pad()
			{
				PointerInt dataInt = (PointerInt)data;
				PointerInt dataIntOffset = dataInt % format.alignment;
				
				if ( dataIntOffset != 0 )
					data += (format.alignment - dataIntOffset);
			}
			
			
		//********************************************************************************
		//******	Primitive Type Writing Methods
			
			
			/// Write a boolean and advance the position.
			OM_INLINE void write( Bool value )
			{
				if ( data + sizeof(UInt8) > dataEnd  )
					checkBufferCapacity( sizeof(UInt8) );
				
				writePrimitive( (UInt8)value );
				data += sizeof(UInt8);
			}
			
			
			/// Write a signed 8-bit and advance the position.
			OM_INLINE void write( Int8 value )
			{
				if ( data + sizeof(Int8) > dataEnd  )
					checkBufferCapacity( sizeof(Int8) );
				
				writePrimitive( value );
				data += sizeof(Int8);
			}
			
			
			/// Write an unsigned 8-bit integer and advance the position.
			OM_INLINE void write( UInt8 value )
			{
				if ( data + sizeof(UInt8) > dataEnd  )
					checkBufferCapacity( sizeof(UInt8) );
				
				writePrimitive( value );
				data += sizeof(UInt8);
			}
			
			
			/// Write a signed 16-bit integer and advance the position.
			OM_INLINE void write( Int16 value )
			{
				if ( data + sizeof(Int16) > dataEnd  )
					checkBufferCapacity( sizeof(Int16) );
				
				writePrimitive( value );
				data += sizeof(Int16);
			}
			
			
			/// Write an unsigned 16-bit integer and advance the position.
			OM_INLINE void write( UInt16 value )
			{
				if ( data + sizeof(UInt16) > dataEnd  )
					checkBufferCapacity( sizeof(UInt16) );
				
				writePrimitive( value );
				data += sizeof(UInt16);
			}
			
			
			/// Write a signed 32-bit integer and advance the position.
			OM_INLINE void write( Int32 value )
			{
				if ( data + sizeof(Int32) > dataEnd  )
					checkBufferCapacity( sizeof(Int32) );
				
				writePrimitive( value );
				data += sizeof(Int32);
			}
			
			
			/// Write an unsigned 32-bit integer and advance the position.
			OM_INLINE void write( UInt32 value )
			{
				if ( data + sizeof(UInt32) > dataEnd  )
					checkBufferCapacity( sizeof(UInt32) );
				
				writePrimitive( value );
				data += sizeof(UInt32);
			}
			
			
			/// Write a signed 64-bit integer and advance the position.
			OM_INLINE void write( Int64 value )
			{
				if ( data + sizeof(Int64) > dataEnd  )
					checkBufferCapacity( sizeof(Int64) );
				
				writePrimitive( value );
				data += sizeof(Int64);
			}
			
			
			/// Write an unsigned 64-bit integer and advance the position.
			OM_INLINE void write( UInt64 value )
			{
				if ( data + sizeof(UInt64) > dataEnd  )
					checkBufferCapacity( sizeof(UInt64) );
				
				writePrimitive( value );
				data += sizeof(UInt64);
			}
			
			
			/// Write a 32-bit float and advance the position.
			OM_INLINE void write( Float32 value )
			{
				if ( data + sizeof(Float32) > dataEnd  )
					checkBufferCapacity( sizeof(Float32) );
				
				writePrimitive( value );
				data += sizeof(Float32);
			}
			
			
			/// Write a 64-bit float and advance the position.
			OM_INLINE void write( Float64 value )
			{
				if ( data + sizeof(Float64) > dataEnd  )
					checkBufferCapacity( sizeof(Float64) );
				
				writePrimitive( value );
				data += sizeof(Float64);
			}
			
			
			/// Write a complex number of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::Complex<T>& value )
			{
				if ( data + sizeof(math::Complex<T>) > dataEnd  )
					checkBufferCapacity( sizeof(math::Complex<T>) );
				
				writePrimitive( value.r, 0 );
				writePrimitive( value.i, 1 );
				data += sizeof(math::Complex<T>);
			}
			
			
		//********************************************************************************
		//******	Vector Type Writing Methods
			
			
			/// Write a 2D vector of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::VectorND<T,2>& value )
			{
				if ( data + sizeof(math::VectorND<T,2>) > dataEnd  )
					checkBufferCapacity( sizeof(math::VectorND<T,2>) );
				
				writePrimitive( value.x, 0 );
				writePrimitive( value.y, 1 );
				data += sizeof(math::VectorND<T,2>);
			}
			
			
			/// Write a 3D vector of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::VectorND<T,3>& value )
			{
				if ( data + sizeof(math::VectorND<T,3>) > dataEnd  )
					checkBufferCapacity( sizeof(math::VectorND<T,3>) );
				
				writePrimitive( value.x, 0 );
				writePrimitive( value.y, 1 );
				writePrimitive( value.z, 2 );
				data += sizeof(math::VectorND<T,3>);
			}
			
			
			/// Write a 4D vector of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::VectorND<T,4>& value )
			{
				if ( data + sizeof(math::VectorND<T,4>) > dataEnd  )
					checkBufferCapacity( sizeof(math::VectorND<T,4>) );
				
				writePrimitive( value.x, 0 );
				writePrimitive( value.y, 1 );
				writePrimitive( value.z, 2 );
				writePrimitive( value.w, 3 );
				data += sizeof(math::VectorND<T,4>);
			}
			
			
		//********************************************************************************
		//******	Matrix Type Writing Methods
			
			
			/// Write a 2x2 matrix of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::MatrixND<T,2,2>& value )
			{
				if ( data + sizeof(math::MatrixND<T,2,2>) > dataEnd  )
					checkBufferCapacity( sizeof(math::MatrixND<T,2,2>) );
				
				writePrimitive( value.x.x, 0 );
				writePrimitive( value.x.y, 1 );
				writePrimitive( value.y.x, 2 );
				writePrimitive( value.y.y, 3 );
				data += sizeof(math::MatrixND<T,2,2>);
			}
			
			
			/// Write a 3x3 matrix of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::MatrixND<T,3,3>& value )
			{
				if ( data + sizeof(math::MatrixND<T,3,3>) > dataEnd  )
					checkBufferCapacity( sizeof(math::MatrixND<T,3,3>) );
				
				writePrimitive( value.x.x, 0 );
				writePrimitive( value.x.y, 1 );
				writePrimitive( value.x.z, 2 );
				writePrimitive( value.y.x, 3 );
				writePrimitive( value.y.y, 4 );
				writePrimitive( value.y.z, 5 );
				writePrimitive( value.z.x, 6 );
				writePrimitive( value.z.y, 7 );
				writePrimitive( value.z.z, 8 );
				data += sizeof(math::MatrixND<T,3,3>);
			}
			
			
			/// Write a 4x4 matrix of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::MatrixND<T,4,4>& value )
			{
				if ( data + sizeof(math::MatrixND<T,4,4>) > dataEnd  )
					checkBufferCapacity( sizeof(math::MatrixND<T,4,4>) );
				
				writePrimitive( value.x.x, 0 );
				writePrimitive( value.x.y, 1 );
				writePrimitive( value.x.z, 2 );
				writePrimitive( value.x.w, 3 );
				writePrimitive( value.y.x, 4 );
				writePrimitive( value.y.y, 5 );
				writePrimitive( value.y.z, 6 );
				writePrimitive( value.y.w, 7 );
				writePrimitive( value.z.x, 8 );
				writePrimitive( value.z.y, 9 );
				writePrimitive( value.z.z, 10 );
				writePrimitive( value.z.w, 11 );
				writePrimitive( value.w.x, 12 );
				writePrimitive( value.w.y, 13 );
				writePrimitive( value.w.z, 14 );
				writePrimitive( value.w.w, 15 );
				data += sizeof(math::MatrixND<T,4,4>);
			}
			
			
		//********************************************************************************
		//******	Transform Type Writing Methods
			
			
			/// Write a 3D transform of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::Transform3D<T>& value )
			{
				if ( data + sizeof(math::Transform3D<T>) > dataEnd  )
					checkBufferCapacity( sizeof(math::Transform3D<T>) );
				
				writePrimitive( value.position.x, 0 );
				writePrimitive( value.position.y, 1 );
				writePrimitive( value.position.z, 2 );
				writePrimitive( value.orientation.x.x, 3 );
				writePrimitive( value.orientation.x.y, 4 );
				writePrimitive( value.orientation.x.z, 5 );
				writePrimitive( value.orientation.y.x, 6 );
				writePrimitive( value.orientation.y.y, 7 );
				writePrimitive( value.orientation.y.z, 8 );
				writePrimitive( value.orientation.z.x, 9 );
				writePrimitive( value.orientation.z.y, 10 );
				writePrimitive( value.orientation.z.z, 11 );
				writePrimitive( value.scale.x, 12 );
				writePrimitive( value.scale.y, 13 );
				writePrimitive( value.scale.z, 14 );
				data += sizeof(math::Transform3D<T>);
			}
			
			
		//********************************************************************************
		//******	Range Type Writing Methods
			
			
			/// Write a 1D range of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::AABB1D<T>& value )
			{
				if ( data + sizeof(math::AABB1D<T>) > dataEnd  )
					checkBufferCapacity( sizeof(math::AABB1D<T>) );
				
				writePrimitive( value.min, 0 );
				writePrimitive( value.max, 1 );
				data += sizeof(math::AABB1D<T>);
			}
			
			
			/// Write a 2D range of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::AABB2D<T>& value )
			{
				if ( data + sizeof(math::AABB2D<T>) > dataEnd  )
					checkBufferCapacity( sizeof(math::AABB2D<T>) );
				
				writePrimitive( value.min.x, 0 );
				writePrimitive( value.min.y, 1 );
				writePrimitive( value.max.x, 2 );
				writePrimitive( value.max.y, 3 );
				data += sizeof(math::AABB2D<T>);
			}
			
			
			/// Write a 3D range of the templated type and advance the position.
			template < typename T >
			OM_INLINE void write( const math::AABB3D<T>& value )
			{
				if ( data + sizeof(math::AABB3D<T>) > dataEnd  )
					checkBufferCapacity( sizeof(math::AABB3D<T>) );
				
				writePrimitive( value.min.x, 0 );
				writePrimitive( value.min.y, 1 );
				writePrimitive( value.min.z, 2 );
				writePrimitive( value.max.x, 3 );
				writePrimitive( value.max.y, 4 );
				writePrimitive( value.max.z, 5 );
				data += sizeof(math::AABB3D<T>);
			}
			
			
		//********************************************************************************
		//******	Array Writing Methods
			
			
			/// Write an array of values of the templated type and advance the data pointer.
			template < typename T >
			OM_INLINE void writeArray( const T* array, Size number )
			{
				const T* const arrayEnd = array + number;
				
				while ( array != arrayEnd )
				{
					this->write( *array );
					array++;
				}
			}
			
			
		//********************************************************************************
		//******	String Writing Methods
			
			
			/// Write an ASCII encoded string starting at the specified data pointer and return it and advance the position.
			void writeASCIIString( const data::String& string );
			
			
			/// Write a UTF-8 encoded string starting at the specified data pointer and return it and advance the position.
			void writeUTF8String( const data::UTF8String& string );
			
			
			/// Write a UTF-16 encoded string starting at the specified data pointer and return it and advance the position.
			void writeUTF16String( const data::UTF16String& string );
			
			
			/// Write a UTF-32 encoded string starting at the specified data pointer and return it and advance the position.
			void writeUTF32String( const data::UTF32String& string );
			
			
		//********************************************************************************
		//******	Data Write Methods
			
			
			/// Write the specified number of bytes of data from the buffer to the stream.
			Bool write( const UByte* newData, Size number );
			
			
			/// Write the specified number of bytes of data from the buffer to the stream.
			/**
			  * The method returns the number of bytes that were actually written.
			  */
			virtual Size writeData( const UByte* newData, Size number );
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not this type of stream allows seeking.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			virtual Bool canSeek( Int64 relativeOffset ) const;
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			virtual Int64 seek( Int64 relativeOffset );
			
			
		//********************************************************************************
		//******	Flush Method
			
			
			/// Flush buffered output to the stream if the encoder has a valid stream.
			virtual void flush();
			
			
		//********************************************************************************
		//******	Write Position Accessor Method
			
			
			/// Return the current write position of the encoder relative to the start of the output.
			virtual LargeIndex getPosition() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Functions
			
			
			/// Pad the specified size in bytes to a multiple of the alignment.
			OM_INLINE Size padSize( Size sizeInBytes )
			{
				return sizeInBytes % format.alignment == 0 ? sizeInBytes :
											math::nextMultiple( sizeInBytes, format.alignment );
			}
			
			
			/// Increase the capacity of the encoding buffer by at least the specified amount.
			void addBufferCapacity( Size numBytes );
			
			
			/// Make sure there is space in the internal buffer for the specified number of bytes.
			void checkBufferCapacity( Size numBytes );
			
			
			/// Flush any buffered data to the internal stream if it is valid and return whether or not the flush was successful.
			Bool flushToStream();
			
			
			/// Write a primitive type of the templated type and convert its endianness if applicable.
			template < typename T >
			OM_FORCE_INLINE void writePrimitive( T primitive, Index index = 0 )
			{
				((T*)data)[index] = format.endianness.convertFromNative( primitive );
			}
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default capacity of the encoding buffer in bytes.
			static const Size DEFAULT_BUFFER_CAPACITY = 4096;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The start of the internal data buffer of the binary encoder.
			UByte* dataStart;
			
			
			/// The end of the internal data buffer of the binary encoder.
			UByte* dataEnd;
			
			
			/// The current write position of the binary encoder.
			UByte* data;
			
			
			/// A pointer to a data stream that is the destination of this encoder.
			/**
			  * If the stream is NULL, the encoder keeps enlarging its internal buffer as necessary.
			  */
			DataOutputStream* stream;
			
			
			/// The start of the internal data buffer of the binary encoder.
			UByte* internalData;
			
			
			/// The format that the binary encoder is currently writing in.
			BinaryFormat format;
			
			
			/// The offset from the beginning of the stream of the start of the internal buffer.
			LargeIndex streamPosition;
			
			
			
};





//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BINARY_ENCODER_H
