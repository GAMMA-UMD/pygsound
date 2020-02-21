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

#include "omBinaryEncoder.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




BinaryEncoder:: BinaryEncoder()
	:	format(),
		stream( NULL ),
		streamPosition( 0 )
{
	dataStart = util::allocate<UByte>( DEFAULT_BUFFER_CAPACITY );
	dataEnd = dataStart + DEFAULT_BUFFER_CAPACITY;
	data = dataStart;
}




BinaryEncoder:: BinaryEncoder( const BinaryEncoder& other )
	:	format( other.format ),
		stream( other.stream ),
		streamPosition( other.streamPosition )
{
	const Size bufferCapacity = other.getBufferCapacity();
	const Size dataSize = other.getBufferSize();
	
	// Allocate a buffer.
	dataStart = util::allocate<UByte>( bufferCapacity );
	dataEnd = dataStart + bufferCapacity;
	data = dataStart;
	
	// Copy the data.
	util::copyPOD( dataStart, other.dataStart, dataSize );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




BinaryEncoder:: ~BinaryEncoder()
{
	util::deallocate( dataStart );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




BinaryEncoder& BinaryEncoder:: operator = ( const BinaryEncoder& other )
{
	if ( this != &other )
	{
		format = other.format;
		stream = other.stream;
		streamPosition = other.streamPosition;
		
		const Size bufferCapacity = other.getBufferCapacity();
		const Size dataSize = other.getBufferSize();
		
		// Increase the capacity if necessary.
		if ( bufferCapacity >= this->getBufferCapacity() )
		{
			util::deallocate( dataStart );
			
			// Allocate a new buffer.
			dataStart = util::allocate<UByte>( bufferCapacity );
			dataEnd = dataStart + bufferCapacity;
			data = dataStart;
		}
		
		// Copy the data.
		util::copyPOD( dataStart, other.dataStart, dataSize );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Capacity Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BinaryEncoder:: setBufferCapacity( Size newCapacity )
{
	const Size dataSize = this->getBufferSize();
	
	if ( newCapacity < dataSize )
		return false;
	
	// Allocate a new buffer for the data.
	UByte* newData = util::allocate<UByte>( newCapacity );
	
	// Copy the previous contents.
	util::copyPOD( newData, dataStart, dataSize );
	util::deallocate( dataStart );
	
	dataStart = newData;
	dataEnd = newData + newCapacity;
	data = newData + dataSize;
	
	return true;
}




void BinaryEncoder:: addBufferCapacity( Size numBytes )
{
	const Size dataSize = this->getBufferSize();
	const Size oldCapacity = this->getBufferCapacity();
	const Size requiredCapacity = oldCapacity + numBytes;
	Size newCapacity = oldCapacity;
	
	while ( newCapacity < requiredCapacity )
		newCapacity *= 2;
	
	// Allocate a new buffer for the data.
	UByte* newData = util::allocate<UByte>( newCapacity );
	
	// Copy the previous contents.
	util::copyPOD( newData, dataStart, dataSize );
	util::deallocate( dataStart );
	
	dataStart = newData;
	dataEnd = newData + newCapacity;
	data = newData + dataSize;
}




void BinaryEncoder:: checkBufferCapacity( Size numBytes )
{
	// Increase the buffer size if the stream is not able to be flushed.
	if ( stream == NULL || !this->flushToStream() )
		addBufferCapacity( numBytes );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Write Methods
//############		
//##########################################################################################
//##########################################################################################




void BinaryEncoder:: writeASCIIString( const data::String& string )
{
	// Write the string's length.
	const UInt64 stringLength = (UInt64)string.getLength();
	const UInt64 stringBytes = stringLength*sizeof(Char);
	this->write( stringLength );
	
	// Compute the padded length according to the alignment.
	const Size paddedLength = padSize( stringBytes );
	
	// Write the string's characters.
	this->write( (const UByte*)string.getCString(), stringBytes );
	data += (paddedLength - stringBytes);
}




void BinaryEncoder:: writeUTF8String( const data::UTF8String& string )
{
	// Write the string's length.
	const UInt64 stringLength = (UInt64)string.getLength();
	const UInt64 stringBytes = stringLength*sizeof(UTF8Char);
	this->write( stringLength );
	
	// Compute the padded length according to the alignment.
	const Size paddedLength = padSize( stringBytes );
	
	// Write the string's characters.
	this->write( (const UByte*)string.getCString(), stringBytes );
	data += (paddedLength - stringBytes);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Data Write Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BinaryEncoder:: write( const UByte* newData, Size number )
{
	if ( data + number > dataEnd )
	{
		// If the stream is set and the new data is large, flush the stream and write directly.
		if ( stream && this->flushToStream() )
		{
			// If the new data is large, write directly to the stream.
			if ( number > this->getBufferCapacity() )
			{
				const Size numWritten = stream->writeData( newData, number );
				streamPosition += numWritten;
				return numWritten == number;
			}
		}
		else
			addBufferCapacity( number );
	}
	
	util::copyPOD( data, newData, number );
	data += number;
	return true;
}




Size BinaryEncoder:: writeData( const UByte* newData, Size number )
{
	if ( this->write( newData, number ) )
		return number;
	else
		return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seeking Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BinaryEncoder:: canSeek() const
{
	return true;
}




Bool BinaryEncoder:: canSeek( Int64 relativeOffset ) const
{
	if ( stream )
	{
		// Flush buffered data to the stream.
		const_cast<BinaryEncoder*>(this)->flushToStream();
		
		// Ask the stream directly.
		return stream->canSeek( relativeOffset );
	}
	else
	{
		// Seek as far as we can in the encoding buffer.
		UByte* newData = data + relativeOffset;
		
		if ( newData > dataEnd || newData < dataStart )
			return false;
		else
			return true;
	}
}




Int64 BinaryEncoder:: seek( Int64 relativeOffset )
{
	if ( stream )
	{
		// Flush buffered data to the stream.
		this->flushToStream();
		
		// Seek in the stream directly.
		Int64 delta = stream->seek( relativeOffset );
		streamPosition = (Int64)streamPosition + delta;
		
		return delta;
	}
	else
	{
		// Seek as far as we can in the encoding buffer.
		UByte* oldData = data;
		UByte* newData = data + relativeOffset;
		
		if ( newData > dataEnd )
			data = dataEnd;
		else if ( newData < dataStart )
			data = dataStart;
		else
			data = newData;
		
		return data - oldData;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Flush Methods
//############		
//##########################################################################################
//##########################################################################################




void BinaryEncoder:: flush()
{
	this->flushToStream();
	
	if ( stream )
		stream->flush();
}




Bool BinaryEncoder:: flushToStream()
{
	if ( stream == NULL )
		return false;
	
	// Write the data in the buffer to the stream.
	const Size bufferSize = this->getBufferSize();
	
	if ( bufferSize > 0 )
	{
		if ( stream->writeData( dataStart, bufferSize ) != bufferSize )
			return false;
		
		streamPosition += bufferSize;
		data = dataStart;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeIndex BinaryEncoder:: getPosition() const
{
	if ( stream == NULL )
		return data - dataStart;
	else
		return streamPosition + (data - dataStart);
}




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
