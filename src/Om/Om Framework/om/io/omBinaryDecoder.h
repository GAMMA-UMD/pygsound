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

#ifndef INCLUDE_OM_BINARY_DECODER_H
#define INCLUDE_OM_BINARY_DECODER_H


#include "omIOConfig.h"


#include "omBinaryFormat.h"
#include "omDataInputStream.h"
#include "../omMath.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles decoding of standard types from binary data.
class BinaryDecoder : public DataInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new binary decoder that uses the native endianness for primitive types.
			BinaryDecoder();
			
			
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return a reference to the binary format this decoder is using.
			OM_INLINE const BinaryFormat& getFormat() const
			{
				return format;
			}
			
			
			/// Set the binary format this decoder is using.
			OM_INLINE void setFormat( const BinaryFormat& newFormat )
			{
				format = newFormat;
			}
			
			
		//********************************************************************************
		//******	Data Accessor Methods
			
			
			/// Return the total size of the decoder's data source in bytes.
			OM_INLINE Size getDataSize() const
			{
				return dataEnd - dataStart;
			}
			
			
			/// Set a pointer to an external data source that should be read with this decoder.
			/**
			  * This function prepares the decoder for reading from an external data source.
			  * When the decoder reaches the end of the data, it cannot read any further.
			  */
			OM_INLINE void setData( const UByte* newData, Size dataSizeInBytes )
			{
				dataStart = newData;
				dataEnd = newData + dataSizeInBytes;
				data = newData;
			}
			
			
		//********************************************************************************
		//******	Padding Methods
			
			
			/// Pad the alignment of the specified pointer to the next aligned object boundary.
			OM_INLINE void pad( const UByte*& data )
			{
				PointerInt dataInt = (PointerInt)data;
				PointerInt dataIntOffset = dataInt % format.alignment;
				
				if ( dataIntOffset != 0 )
					data += (format.alignment - dataIntOffset);
			}
			
			
		//********************************************************************************
		//******	Primitive Type Reading Methods
			
			
			/// Read a boolean from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( Bool& value )
			{
				const UByte* newData = data + sizeof(UInt8);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<UInt8>() != UInt8(0);
				data = newData;
				return true;
			}
			
			
			/// Read a signed 8-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( Int8& value )
			{
				const UByte* newData = data + sizeof(Int8);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<Int8>();
				data = newData;
				return true;
			}
			
			
			/// Read an unsigned 8-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( UInt8& value )
			{
				const UByte* newData = data + sizeof(UInt8);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<UInt8>();
				data = newData;
				return true;
			}
			
			
			/// Read a signed 16-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( Int16& value )
			{
				const UByte* newData = data + sizeof(Int16);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<Int16>();
				data = newData;
				return true;
			}
			
			
			/// Read an unsigned 16-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( UInt16& value )
			{
				const UByte* newData = data + sizeof(UInt16);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<UInt16>();
				data = newData;
				return true;
			}
			
			
			/// Read a signed 32-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( Int32& value )
			{
				const UByte* newData = data + sizeof(Int32);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<Int32>();
				data = newData;
				return true;
			}
			
			
			/// Read an unsigned 32-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( UInt32& value )
			{
				const UByte* newData = data + sizeof(UInt32);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<UInt32>();
				data = newData;
				return true;
			}
			
			
			/// Read a signed 64-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( Int64& value )
			{
				const UByte* newData = data + sizeof(Int64);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<Int64>();
				data = newData;
				return true;
			}
			
			
			/// Read an unsigned 64-bit integer from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( UInt64& value )
			{
				const UByte* newData = data + sizeof(UInt64);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<UInt64>();
				data = newData;
				return true;
			}
			
			
			/// Read a 16-bit float from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( lang::Float16& value )
			{
				const UByte* newData = data + sizeof(lang::Float16);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<lang::Float16>();
				data = newData;
				return true;
			}
			
			
			/// Read a 32-bit float from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( Float32& value )
			{
				const UByte* newData = data + sizeof(Float32);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<Float32>();
				data = newData;
				return true;
			}
			
			
			/// Read a 64-bit float from the data pointer into the output value parameter and advance the pointer.
			OM_INLINE Bool read( Float64& value )
			{
				const UByte* newData = data + sizeof(Float64);
				
				if ( newData > dataEnd )
					return false;
				
				value = readPrimitive<Float64>();
				data = newData;
				return true;
			}
			
			
			/// Read a 2D vector of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::Complex<T>& value )
			{
				const UByte* newData = data + sizeof(math::Complex<T>);
				
				if ( newData > dataEnd )
					return false;
				
				value.r = readPrimitive<T>(0);
				value.i = readPrimitive<T>(1);
				data = newData;
				return true;
			}
			
			
		//********************************************************************************
		//******	Vector Type Reading Methods
			
			
			/// Read a 2D vector of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::VectorND<T,2>& value )
			{
				const UByte* newData = data + sizeof(math::VectorND<T,2>);
				
				if ( newData > dataEnd )
					return false;
				
				value.x = readPrimitive<T>(0);
				value.y = readPrimitive<T>(1);
				data = newData;
				return true;
			}
			
			
			/// Read a 3D vector of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::VectorND<T,3>& value )
			{
				const UByte* newData = data + sizeof(math::VectorND<T,3>);
				
				if ( newData > dataEnd )
					return false;
				
				value.x = readPrimitive<T>(0);
				value.y = readPrimitive<T>(1);
				value.z = readPrimitive<T>(2);
				data = newData;
				return true;
			}
			
			
			/// Read a 4D vector of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::VectorND<T,4>& value )
			{
				const UByte* newData = data + sizeof(math::VectorND<T,4>);
				
				if ( newData > dataEnd )
					return false;
				
				value.x = readPrimitive<T>(0);
				value.y = readPrimitive<T>(1);
				value.z = readPrimitive<T>(2);
				value.w = readPrimitive<T>(2);
				data = newData;
				return true;
			}
			
			
		//********************************************************************************
		//******	Matrix Type Reading Methods
			
			
			/// Read a 2x2 matrix of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::MatrixND<T,2,2>& value )
			{
				const UByte* newData = data + sizeof(math::MatrixND<T,2,2>);
				
				if ( newData > dataEnd )
					return false;
				
				value.x.x = readPrimitive<T>(0);
				value.x.y = readPrimitive<T>(1);
				value.y.x = readPrimitive<T>(2);
				value.y.y = readPrimitive<T>(3);
				data = newData;
				return true;
			}
			
			
			/// Read a 3x3 matrix of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::MatrixND<T,3,3>& value )
			{
				const UByte* newData = data + sizeof(math::MatrixND<T,3,3>);
				
				if ( newData > dataEnd )
					return false;
				
				value.x.x = readPrimitive<T>(0);
				value.x.y = readPrimitive<T>(1);
				value.x.z = readPrimitive<T>(2);
				value.y.x = readPrimitive<T>(3);
				value.y.y = readPrimitive<T>(4);
				value.y.z = readPrimitive<T>(5);
				value.z.x = readPrimitive<T>(6);
				value.z.y = readPrimitive<T>(7);
				value.z.z = readPrimitive<T>(8);
				data = newData;
				return true;
			}
			
			
			/// Read a 4x4 matrix of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::MatrixND<T,4,4>& value )
			{
				const UByte* newData = data + sizeof(math::MatrixND<T,4,4>);
				
				if ( newData > dataEnd )
					return false;
				
				value.x.x = readPrimitive<T>(0);
				value.x.y = readPrimitive<T>(1);
				value.x.z = readPrimitive<T>(2);
				value.x.w = readPrimitive<T>(3);
				value.y.x = readPrimitive<T>(4);
				value.y.y = readPrimitive<T>(5);
				value.y.z = readPrimitive<T>(6);
				value.y.w = readPrimitive<T>(7);
				value.z.x = readPrimitive<T>(8);
				value.z.y = readPrimitive<T>(9);
				value.z.z = readPrimitive<T>(10);
				value.z.w = readPrimitive<T>(11);
				value.w.x = readPrimitive<T>(12);
				value.w.y = readPrimitive<T>(13);
				value.w.z = readPrimitive<T>(14);
				value.w.w = readPrimitive<T>(15);
				data = newData;
				return true;
			}
			
			
		//********************************************************************************
		//******	Transform Type Reading Methods
			
			
			/// Read a 3x3 matrix of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::Transform3D<T>& value )
			{
				const UByte* newData = data + sizeof(math::Transform3D<T>);
				
				if ( newData > dataEnd )
					return false;
				
				value.position.x = readPrimitive<T>(0);
				value.position.y = readPrimitive<T>(1);
				value.position.z = readPrimitive<T>(2);
				value.orientation.x.x = readPrimitive<T>(3);
				value.orientation.x.y = readPrimitive<T>(4);
				value.orientation.x.z = readPrimitive<T>(5);
				value.orientation.y.x = readPrimitive<T>(6);
				value.orientation.y.y = readPrimitive<T>(7);
				value.orientation.y.z = readPrimitive<T>(8);
				value.orientation.z.x = readPrimitive<T>(9);
				value.orientation.z.y = readPrimitive<T>(10);
				value.orientation.z.z = readPrimitive<T>(11);
				value.scale.x = readPrimitive<T>(12);
				value.scale.y = readPrimitive<T>(13);
				value.scale.z = readPrimitive<T>(14);
				data = newData;
				return true;
			}
			
			
		//********************************************************************************
		//******	Range Type Reading Methods
			
			
			/// Read a 1D range of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::AABB1D<T>& value )
			{
				const UByte* newData = data + sizeof(math::AABB1D<T>);
				
				if ( newData > dataEnd )
					return false;
				
				value.min = readPrimitive<T>(0);
				value.max = readPrimitive<T>(1);
				data = newData;
				return true;
			}
			
			
			/// Read a 2D range of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::AABB2D<T>& value )
			{
				const UByte* newData = data + sizeof(math::AABB2D<T>);
				
				if ( newData > dataEnd )
					return false;
				
				value.min.x = readPrimitive<T>(0);
				value.min.y = readPrimitive<T>(1);
				value.max.x = readPrimitive<T>(2);
				value.max.y = readPrimitive<T>(3);
				data = newData;
				return true;
			}
			
			
			/// Read a 3D range of the templated type from the data pointer into the output value parameter and advance the pointer.
			template < typename T >
			OM_INLINE Bool read( math::AABB3D<T>& value )
			{
				const UByte* newData = data + sizeof(math::AABB3D<T>);
				
				if ( newData > dataEnd )
					return false;
				
				value.min.x = readPrimitive<T>(0);
				value.min.y = readPrimitive<T>(1);
				value.min.z = readPrimitive<T>(2);
				value.max.x = readPrimitive<T>(3);
				value.max.y = readPrimitive<T>(4);
				value.max.z = readPrimitive<T>(5);
				data = newData;
				return true;
			}
			
			
		//********************************************************************************
		//******	Array Reading Methods
			
			
			/// Read an array of values of the templated type and advance the data pointer.
			template < typename T >
			OM_INLINE void readArray( T* array, Size number )
			{
				const UByte* newDataEnd = data + sizeof(T)*number;
				
				if ( newDataEnd > dataEnd )
					number = dataEnd - data;
				
				const T* const arrayEnd = array + number;
				
				while ( array != arrayEnd )
				{
					this->read( *array );
					array++;
				}
			}
			
			
		//********************************************************************************
		//******	String Reading Methods
			
			
			/// Read an ASCII encoded string starting at the specified data pointer and return it and advance the pointer.
			data::String readASCIIString();
			
			
			/// Read a UTF-8 encoded string starting at the specified data pointer and return it and advance the pointer.
			data::UTF8String readUTF8String();
			
			
			/// Read a UTF-16 encoded string starting at the specified data pointer and return it and advance the pointer.
			data::UTF16String readUTF16String();
			
			
			/// Read a UTF-32 encoded string starting at the specified data pointer and return it and advance the pointer.
			data::UTF32String readUTF32String();
			
			
		//********************************************************************************
		//******	Data Read Methods
			
			
			/// Read the specified number of bytes of data into the buffer.
			Bool read( UByte* newData, Size number );
			
			
			/// Read the specified number of bytes from the stream and place them in the buffer given by a pointer.
			virtual Size readData( UByte* buffer, Size number );
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not this type of stream allows seeking.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			virtual Bool canSeek( Int64 relativeOffset ) const;
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			virtual Int64 seek( Int64 relativeOffset );
			
			
		//********************************************************************************
		//******	Read Position Accessor Methods
			
			
			/// Return the number of bytes remaining in the stream.
			virtual LargeSize getBytesRemaining() const;
			
			
			/// Return the current byte index within the stream relative to the beginning.
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
			
			
			/// Read a primitive type of the templated type and convert its endianness if applicable.
			template < typename T >
			OM_FORCE_INLINE T readPrimitive( Index index = 0 ) const
			{
				return format.endianness.convertToNative( ((const T*)data)[index] );
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The start of the internal data buffer of the binary decoder.
			const UByte* dataStart;
			
			
			/// The end of the internal data buffer of the binary decoder.
			const UByte* dataEnd;
			
			
			/// The current read position of the binary decoder.
			const UByte* data;
			
			
			/// The format that the binary decoder is currently reading in.
			BinaryFormat format;
			
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BINARY_DECODER_H
