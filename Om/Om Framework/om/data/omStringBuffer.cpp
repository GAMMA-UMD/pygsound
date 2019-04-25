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

#include "omStringBuffer.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericStringBuffer<Char>:: GenericStringBuffer()
	:	buffer( util::allocate<Char>( DEFAULT_CAPACITY ) ),
		capacity( DEFAULT_CAPACITY ),
		resizeFactor( DEFAULT_RESIZE_FACTOR )
{
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF8Char>:: GenericStringBuffer()
	:	buffer( util::allocate<UTF8Char>( DEFAULT_CAPACITY ) ),
		capacity( DEFAULT_CAPACITY ),
		resizeFactor( DEFAULT_RESIZE_FACTOR )
{
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF16Char>:: GenericStringBuffer()
	:	buffer( util::allocate<UTF16Char>( DEFAULT_CAPACITY ) ),
		capacity( DEFAULT_CAPACITY ),
		resizeFactor( DEFAULT_RESIZE_FACTOR )
{
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF32Char>:: GenericStringBuffer()
	:	buffer( util::allocate<UTF32Char>( DEFAULT_CAPACITY ) ),
		capacity( DEFAULT_CAPACITY ),
		resizeFactor( DEFAULT_RESIZE_FACTOR )
{
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<Char>:: GenericStringBuffer( Size initialCapacity, Float newResizeFactor )
	:	capacity( initialCapacity + 1 ),
		resizeFactor( math::clamp( resizeFactor, 1.1f, 10.0f ) )
{
	buffer = util::allocate<Char>( capacity );
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF8Char>:: GenericStringBuffer( Size initialCapacity, Float newResizeFactor )
	:	capacity( initialCapacity + 1 ),
		resizeFactor( math::clamp( resizeFactor, 1.1f, 10.0f ) )
{
	buffer = util::allocate<UTF8Char>( capacity );
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF16Char>:: GenericStringBuffer( Size initialCapacity, Float newResizeFactor )
	:	capacity( initialCapacity + 1 ),
		resizeFactor( math::clamp( resizeFactor, 1.1f, 10.0f ) )
{
	buffer = util::allocate<UTF16Char>( capacity );
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF32Char>:: GenericStringBuffer( Size initialCapacity, Float newResizeFactor )
	:	capacity( initialCapacity + 1 ),
		resizeFactor( math::clamp( resizeFactor, 1.1f, 10.0f ) )
{
	buffer = util::allocate<UTF32Char>( capacity );
	*buffer = '\0';
	nextElement = buffer;
	bufferEnd = buffer + capacity;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Copy Constructors
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericStringBuffer<Char>:: GenericStringBuffer( const GenericStringBuffer& other )
	:	buffer( util::allocate<Char>( other.capacity ) ),
		capacity( other.capacity ),
		resizeFactor( other.resizeFactor )
{
	Size otherSize = other.getSize();
	
	util::copy( buffer, other.buffer, otherSize );
	
	nextElement = buffer + otherSize;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF8Char>:: GenericStringBuffer( const GenericStringBuffer& other )
	:	buffer( util::allocate<UTF8Char>( other.capacity ) ),
		capacity( other.capacity ),
		resizeFactor( other.resizeFactor )
{
	Size otherSize = other.getSize();
	
	util::copy( buffer, other.buffer, otherSize );
	
	nextElement = buffer + otherSize;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF16Char>:: GenericStringBuffer( const GenericStringBuffer& other )
	:	buffer( util::allocate<UTF16Char>( other.capacity ) ),
		capacity( other.capacity ),
		resizeFactor( other.resizeFactor )
{
	Size otherSize = other.getSize();
	
	util::copy( buffer, other.buffer, otherSize );
	
	nextElement = buffer + otherSize;
	bufferEnd = buffer + capacity;
}




template <>
GenericStringBuffer<UTF32Char>:: GenericStringBuffer( const GenericStringBuffer& other )
	:	buffer( util::allocate<UTF32Char>( other.capacity ) ),
		capacity( other.capacity ),
		resizeFactor( other.resizeFactor )
{
	Size otherSize = other.getSize();
	
	util::copy( buffer, other.buffer, otherSize );
	
	nextElement = buffer + otherSize;
	bufferEnd = buffer + capacity;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operators
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericStringBuffer<Char>& GenericStringBuffer<Char>:: operator = ( const GenericStringBuffer& other )
{
	if ( this != &other )
	{
		// Call the destructors of all objects that are already in the buffer.
		util::deallocate( buffer );
		
		capacity = other.capacity;
		resizeFactor = other.resizeFactor;
		Size otherSize = other.getSize();
		
		// Reallocate the buffer array.
		buffer = util::allocate<Char>( other.capacity );
		
		// Copy objects from the other buffer to this one.
		util::copy( buffer, other.buffer, otherSize );
		
		nextElement = buffer + otherSize;
		bufferEnd = buffer + capacity;
	}
	
	return *this;
}




template <>
GenericStringBuffer<UTF8Char>& GenericStringBuffer<UTF8Char>:: operator = ( const GenericStringBuffer& other )
{
	if ( this != &other )
	{
		// Call the destructors of all objects that are already in the buffer.
		util::deallocate( buffer );
		
		capacity = other.capacity;
		resizeFactor = other.resizeFactor;
		Size otherSize = other.getSize();
		
		// Reallocate the buffer array.
		buffer = util::allocate<UTF8Char>( other.capacity );
		
		// Copy objects from the other buffer to this one.
		util::copy( buffer, other.buffer, otherSize );
		
		nextElement = buffer + otherSize;
		bufferEnd = buffer + capacity;
	}
	
	return *this;
}




template <>
GenericStringBuffer<UTF16Char>& GenericStringBuffer<UTF16Char>:: operator = ( const GenericStringBuffer& other )
{
	if ( this != &other )
	{
		// Call the destructors of all objects that are already in the buffer.
		util::deallocate( buffer );
		
		capacity = other.capacity;
		resizeFactor = other.resizeFactor;
		Size otherSize = other.getSize();
		
		// Reallocate the buffer array.
		buffer = util::allocate<UTF16Char>( other.capacity );
		
		// Copy objects from the other buffer to this one.
		util::copy( buffer, other.buffer, otherSize );
		
		nextElement = buffer + otherSize;
		bufferEnd = buffer + capacity;
	}
	
	return *this;
}




template <>
GenericStringBuffer<UTF32Char>& GenericStringBuffer<UTF32Char>:: operator = ( const GenericStringBuffer& other )
{
	if ( this != &other )
	{
		// Call the destructors of all objects that are already in the buffer.
		util::deallocate( buffer );
		
		capacity = other.capacity;
		resizeFactor = other.resizeFactor;
		Size otherSize = other.getSize();
		
		// Reallocate the buffer array.
		buffer = util::allocate<UTF32Char>( other.capacity );
		
		// Copy objects from the other buffer to this one.
		util::copy( buffer, other.buffer, otherSize );
		
		nextElement = buffer + otherSize;
		bufferEnd = buffer + capacity;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Append Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericStringBuffer<Char>& GenericStringBuffer<Char>:: append( const Char& character )
{
	Char* nextLocation = nextElement + 1;
	
	if ( nextLocation == bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + 1;
	}
	
	*nextElement = character;
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




template <>
GenericStringBuffer<UTF8Char>& GenericStringBuffer<UTF8Char>:: append( const UTF8Char& character )
{
	UTF8Char* nextLocation = nextElement + 1;
	
	if ( nextLocation == bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + 1;
	}
	
	*nextElement = character;
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




template <>
GenericStringBuffer<UTF16Char>& GenericStringBuffer<UTF16Char>:: append( const UTF16Char& character )
{
	UTF16Char* nextLocation = nextElement + 1;
	
	if ( nextLocation == bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + 1;
	}
	
	*nextElement = character;
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




template <>
GenericStringBuffer<UTF32Char>& GenericStringBuffer<UTF32Char>:: append( const UTF32Char& character )
{
	UTF32Char* nextLocation = nextElement + 1;
	
	if ( nextLocation == bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + 1;
	}
	
	*nextElement = character;
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




template <>
GenericStringBuffer<Char>& GenericStringBuffer<Char>:: append( const Char* source )
{
	const Char* lastChar = bufferEnd - 1;
	
	while ( *source )
	{
		if ( nextElement == lastChar )
		{
			increaseCapacity();
			lastChar = bufferEnd - 1;
		}
		
		*nextElement = *source;
		nextElement++;
		source++;
	}
	
	*nextElement = '\0';
	
	return *this;
}




template <>
GenericStringBuffer<UTF8Char>& GenericStringBuffer<UTF8Char>:: append( const UTF8Char* source )
{
	const UTF8Char* lastChar = bufferEnd - 1;
	
	while ( *source )
	{
		if ( nextElement == lastChar )
		{
			increaseCapacity();
			lastChar = bufferEnd - 1;
		}
		
		*nextElement = *source;
		nextElement++;
		source++;
	}
	
	*nextElement = '\0';
	
	return *this;
}




template <>
GenericStringBuffer<UTF16Char>& GenericStringBuffer<UTF16Char>:: append( const UTF16Char* source )
{
	const UTF16Char* lastChar = bufferEnd - 1;
	
	while ( *source )
	{
		if ( nextElement == lastChar )
		{
			increaseCapacity();
			lastChar = bufferEnd - 1;
		}
		
		*nextElement = *source;
		nextElement++;
		source++;
	}
	
	*nextElement = '\0';
	
	return *this;
}




template <>
GenericStringBuffer<UTF32Char>& GenericStringBuffer<UTF32Char>:: append( const UTF32Char* source )
{
	const UTF32Char* lastChar = bufferEnd - 1;
	
	while ( *source )
	{
		if ( nextElement == lastChar )
		{
			increaseCapacity();
			lastChar = bufferEnd - 1;
		}
		
		*nextElement = *source;
		nextElement++;
		source++;
	}
	
	*nextElement = '\0';
	
	return *this;
}




template <>
GenericStringBuffer<Char>& GenericStringBuffer<Char>:: append( const Char* source, Size numElements )
{
	Char* nextLocation = nextElement + numElements;
	
	if ( nextLocation >= bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + numElements;
	}
	
	util::copy( nextElement, source, numElements );
	
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




template <>
GenericStringBuffer<UTF8Char>& GenericStringBuffer<UTF8Char>:: append( const UTF8Char* source, Size numElements )
{
	UTF8Char* nextLocation = nextElement + numElements;
	
	if ( nextLocation >= bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + numElements;
	}
	
	util::copy( nextElement, source, numElements );
	
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




template <>
GenericStringBuffer<UTF16Char>& GenericStringBuffer<UTF16Char>:: append( const UTF16Char* source, Size numElements )
{
	UTF16Char* nextLocation = nextElement + numElements;
	
	if ( nextLocation >= bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + numElements;
	}
	
	util::copy( nextElement, source, numElements );
	
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




template <>
GenericStringBuffer<UTF32Char>& GenericStringBuffer<UTF32Char>:: append( const UTF32Char* source, Size numElements )
{
	UTF32Char* nextLocation = nextElement + numElements;
	
	if ( nextLocation >= bufferEnd )
	{
		increaseCapacity( nextLocation - buffer + 1 );
		nextLocation = nextElement + numElements;
	}
	
	util::copy( nextElement, source, numElements );
	
	*nextLocation = '\0';
	nextElement = nextLocation;
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Resize Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_NO_INLINE void GenericStringBuffer<Char>:: resize( Size newCapacity )
{
	Size numElements = nextElement - buffer;
	
	// Update the capacity and allocate a new array.
	capacity = newCapacity;
	Char* oldBuffer = buffer;
	buffer = util::allocate<Char>(capacity);
	
	// copy the elements from the old array to the new array.
	util::copy( buffer, oldBuffer, numElements );
	
	// Update pointers needed for quick adding to the buffer.
	nextElement = buffer + numElements;
	bufferEnd = buffer + capacity;
	
	// deallocate the old array.
	util::deallocate( oldBuffer );
}




template <>
OM_NO_INLINE void GenericStringBuffer<UTF8Char>:: resize( Size newCapacity )
{
	Size numElements = nextElement - buffer;
	
	// Update the capacity and allocate a new array.
	capacity = newCapacity;
	UTF8Char* oldBuffer = buffer;
	buffer = util::allocate<UTF8Char>(capacity);
	
	// copy the elements from the old array to the new array.
	util::copy( buffer, oldBuffer, numElements );
	
	// Update pointers needed for quick adding to the buffer.
	nextElement = buffer + numElements;
	bufferEnd = buffer + capacity;
	
	// deallocate the old array.
	util::deallocate( oldBuffer );
}




template <>
OM_NO_INLINE void GenericStringBuffer<UTF16Char>:: resize( Size newCapacity )
{
	Size numElements = nextElement - buffer;
	
	// Update the capacity and allocate a new array.
	capacity = newCapacity;
	UTF16Char* oldBuffer = buffer;
	buffer = util::allocate<UTF16Char>(capacity);
	
	// copy the elements from the old array to the new array.
	util::copy( buffer, oldBuffer, numElements );
	
	// Update pointers needed for quick adding to the buffer.
	nextElement = buffer + numElements;
	bufferEnd = buffer + capacity;
	
	// deallocate the old array.
	util::deallocate( oldBuffer );
}




template <>
OM_NO_INLINE void GenericStringBuffer<UTF32Char>:: resize( Size newCapacity )
{
	Size numElements = nextElement - buffer;
	
	// Update the capacity and allocate a new array.
	capacity = newCapacity;
	UTF32Char* oldBuffer = buffer;
	buffer = util::allocate<UTF32Char>(capacity);
	
	// copy the elements from the old array to the new array.
	util::copy( buffer, oldBuffer, numElements );
	
	// Update pointers needed for quick adding to the buffer.
	nextElement = buffer + numElements;
	bufferEnd = buffer + capacity;
	
	// deallocate the old array.
	util::deallocate( oldBuffer );
}




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
