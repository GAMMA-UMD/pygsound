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

#ifndef INCLUDE_OM_DATA_BUFFER_H
#define INCLUDE_OM_DATA_BUFFER_H


#include "omDataConfig.h"


#include "omData.h"
#include "omEndian.h"



// Forward-declare the DataInputStream class so that we can mark it as a friend.
namespace om { namespace io { class DataInputStream; }; };


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A buffer class used to accumulate an opaque array of unsigned bytes.
class DataBuffer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			/// Create a data buffer with the default capacity, resize factor, and endian-ness.
			OM_INLINE DataBuffer()
				:	buffer( NULL ),
					nextElement( NULL ),
					bufferEnd( NULL ),
					capacity( DEFAULT_CAPACITY ),
					resizeFactor( DEFAULT_RESIZE_FACTOR )
			{
			}
			
			
			/// Create a data buffer the specified capacity, default resize factor and native endian-ness.
			OM_INLINE DataBuffer( Size newCapacity )
				:	buffer( util::allocate<UByte>( math::max( newCapacity, Size(1) ) ) ),
					capacity( newCapacity ),
					resizeFactor( DEFAULT_RESIZE_FACTOR )
			{
				nextElement = buffer;
				bufferEnd = buffer + newCapacity;
			}
			
			
			/// Create a data buffer the specified capacity and resize factor and native endian-ness.
			OM_INLINE DataBuffer( Size newCapacity, Float newResizeFactor )
				:	buffer( util::allocate<UByte>( math::max( newCapacity, Size(1) ) ) ),
					capacity( math::max( newCapacity, Size(1) ) ),
					resizeFactor( math::clamp( newResizeFactor, 1.1f, 10.0f ) )
			{
				nextElement = buffer;
				bufferEnd = buffer + capacity;
			}
			
			
			/// Create a data buffer the specified capacity, resize factor and endian-ness.
			OM_INLINE DataBuffer( Size newCapacity, Float newResizeFactor, Endianness newEndianness )
				:	buffer( util::allocate<UByte>( math::max( newCapacity, Size(1) ) ) ),
					capacity( math::max( newCapacity, Size(1) ) ),
					resizeFactor( math::clamp( newResizeFactor, 1.1f, 10.0f ) ),
					endianness( newEndianness )
			{
				nextElement = buffer;
				bufferEnd = buffer + capacity;
			}
			
			
			/// Create a copy of the specified data buffer.
			DataBuffer( const DataBuffer& other );
			
			
			
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy the data buffer and all data contained within.
			~DataBuffer();
			
			
			
		
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another DataBuffer to this buffer, replacing the original contents.
			DataBuffer& operator = ( const DataBuffer& other );
			
			
			
		
		//********************************************************************************
		//******	Unsigned Byte Append Methods
			
			
			/// Append an unsigned byte to this data buffer.
			DataBuffer& append( UByte byte );
			
			
			/// Append the specified number of elements from an array of unsigned bytes.
			DataBuffer& append( const UByte* bytes, Size number );
			
			
			/// Append an array of unsigned bytes to this data buffer.
			OM_INLINE DataBuffer& append( const util::Array<UByte>& array )
			{
				return this->append( array.getPointer(), array.getSize() );
			}
			
			
			/// Append all data in the specified data buffer to this data buffer.
			OM_INLINE DataBuffer& append( const DataBuffer& dataBuffer )
			{
				return this->append( dataBuffer.getPointer(), dataBuffer.getSize() );
			}
			
			
			
		
		//********************************************************************************
		//******	Primitive Type Append Methods
			
			
			/// Append a signed 8-bit integer to this data buffer.
			OM_INLINE DataBuffer& append( Int8 value )
			{
				return this->append( *((UByte*)&value) );
			}
			
			
			/// Append a signed 16-bit integer to this data buffer.
			OM_INLINE DataBuffer& append( Int16 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(Int16) );
			}
			
			
			/// Append an unsigned 16-bit integer to this data buffer.
			OM_INLINE DataBuffer& append( UInt16 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(UInt16) );
			}
			
			
			/// Append a signed 32-bit integer to this data buffer.
			OM_INLINE DataBuffer& append( Int32 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(Int32) );
			}
			
			
			/// Append an unsigned 32-bit integer to this data buffer.
			OM_INLINE DataBuffer& append( UInt32 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(UInt32) );
			}
			
			
			/// Append a signed 64-bit integer to this data buffer.
			OM_INLINE DataBuffer& append( Int64 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(Int64) );
			}
			
			
			/// Append an unsigned 64-bit integer to this data buffer.
			OM_INLINE DataBuffer& append( UInt64 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(UInt64) );
			}
			
			
			/// Append a 32-bit floating point number to this data buffer.
			OM_INLINE DataBuffer& append( Float32 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(Float32) );
			}
			
			
			/// Append a 64-bit floating point number to this data buffer.
			OM_INLINE DataBuffer& append( Float64 value )
			{
				value = endianness.convertToNative( value );
				return this->append( (const UByte*)&value, sizeof(Float64) );
			}
			
			
		//********************************************************************************
		//******	Other Type Append Methods
			
			
			/// Append a certain number of elements from an array of a primitive type.
			template < typename T >
			OM_INLINE DataBuffer& append( const T* source, Size number )
			{
				const T* sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					this->append( *source );
					source++;
				}
				
				return *this;
			}
			
			
			/// Append all elements from an array of a primitive type to this data buffer.
			template < typename T >
			OM_INLINE DataBuffer& append( const util::Array<T>& array )
			{
				return this->append( array.getPointer(), array.getSize() );
			}
			
			
			/// Append a certain number of elements from an array of a primitive type.
			template < typename T >
			OM_INLINE DataBuffer& append( const util::Array<T>& array, Size number )
			{
				return this->append( array.getPointer(), math::min( array.getSize(), number ) );
			}
			
			
		//********************************************************************************
		//******	Append Operator
			
			
			/// Append to the data buffer.
			template < typename T >
			OM_INLINE DataBuffer& operator << ( const T& value )
			{
				return this->append( value );
			}
			
			
		//********************************************************************************
		//******	Reserve Method
			
			
			/// Reserve the specified number of bytes at the end of the buffer where data should be written.
			/**
			  * The number of valid bytes in the buffer is increased by this number. A pointer
			  * to the location where the bytes should be written is returned. If the specified number
			  * of bytes is not written to the pointer returned, the result is undefined.
			  * 
			  * @param numBytes - the number of bytes to reserve at the end of the buffer.
			  * @return a pointer to where the bytes should be written.
			  */
			OM_INLINE UByte* reserve( Size numBytes )
			{
				if ( nextElement + numBytes > bufferEnd )
					increaseCapacity( nextElement - buffer + numBytes );
				
				UByte* pointer = nextElement;
				nextElement += numBytes;
				
				return pointer;
			}
			
			
		//********************************************************************************
		//******	Clear Method
			
			
			/// Clear all previously added elements from the data buffer.
			OM_INLINE void clear()
			{
				nextElement = buffer;
			}
			
			
		//********************************************************************************
		//******	Content Accessor Methods
			
			
			/// Return a Data object containing the contents of this DataBuffer.
			/**
			  * This version of the toData() method leaves the DataBuffer unmodified
			  * and copies the data into the Data object.
			  */
			OM_INLINE Data toData() const
			{
				return Data( buffer, this->getSize() );
			}
			
			
			/// Get a pointer to the beginning of the DataBuffer's internal array.
			OM_INLINE operator const UByte* () const
			{
				return buffer;
			}
			
			
			/// Get a pointer pointing to the buffer's internal array.
			OM_INLINE const UByte* getPointer() const
			{
				return buffer;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Get the number of bytes of data contained in the buffer.
			OM_INLINE Size getSize() const
			{
				return nextElement - buffer;
			}
			
			
		//********************************************************************************
		//******	Capacity Accessor Methods
			
			
			/// Return the number of elements the buffer can hold without resizing.
			OM_INLINE Size getCapacity() const
			{
				return capacity;
			}
			
			
			/// Set the number of elements the buffer can hold.
			OM_INLINE Bool setCapacity( Size newCapacity )
			{
				if ( newCapacity < getSize() )
					return false;
				
				resize( newCapacity );
				return true;
			}
			
			
		//********************************************************************************
		//******	Resize Factor Accessor Methods
			
			
			/// Get the resize factor for this string buffer.
			OM_INLINE Float getResizeFactor() const
			{
				return resizeFactor;
			}
			
			
			/// Set the resize factor for this string buffer, clamped to [1.1, 10.0]
			OM_INLINE void setResizeFactor( Float newResizeFactor )
			{
				resizeFactor = math::clamp( newResizeFactor, 1.1f, 10.0f );
			}
			
			
		//********************************************************************************
		//******	Endian-ness Accessor Methods
			
			
			/// Get the current endianness of the data being written to the buffer.
			OM_INLINE Endianness getEndianness() const
			{
				return endianness;
			}
			
			
			/// Set the buffer to serialize data in big endian format.
			OM_INLINE void setEndianness( Endianness newEndianness )
			{
				endianness = newEndianness;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Methods
			
			
			/// Increase the capacity to the specified amount multiplied by the resize factor.
			OM_INLINE void increaseCapacity( Size minimumCapacity )
			{
				resize( math::max( minimumCapacity, Size(Float(capacity)*resizeFactor) ) );
			}
			
			
			/// Resize the internal buffer to be the specified length.
			void resize( Size newCapacity );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of elements that is the buffer.
			UByte* buffer;
			
			
			/// A pointer to the location in the buffer where the next element should be inserted.
			UByte* nextElement;
			
			
			/// A pointer to the first element past the end of the buffer.
			const UByte* bufferEnd;
			
			
			/// The number of elements that the buffer can hold.
			Size capacity;
			
			
			/// How much the buffer's capacity increases when it needs to.
			Float resizeFactor;
			
			
			/// Whether or not the buffer is currently encoding data in big or little endian format.
			Endianness endianness;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default capacity for a buffer if it is not specified.
			static const Size DEFAULT_CAPACITY = 32;
			
			
			/// The default factor by which the buffer resizes.
			static const Float DEFAULT_RESIZE_FACTOR;
			
			
		//********************************************************************************
		//******	Friend Class Declarations
			
			
			/// Mark the DataInputStream class as a friend so that it can modify a buffer's internals.
			friend class om::io::DataInputStream;
			
			
			
};




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DATA_BUFFER_H
