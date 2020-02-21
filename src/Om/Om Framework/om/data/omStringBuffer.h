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

#ifndef INCLUDE_OM_BASIC_STRING_BUFFER_H
#define INCLUDE_OM_BASIC_STRING_BUFFER_H


#include "omDataConfig.h"


#include "../util/omArray.h"


#include "omString.h"



// Forward-declare the StringInputStream class so that we can mark it as a friend.
namespace om { namespace io { class StringInputStream; }; };


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that contains a buffer of characters of templated type.
/** 
  * This class allows the user to accumulate characters in a resizing buffer,
  * then convert the buffer's internal array to a string for other uses.
  */
template < typename CharType >
class GenericStringBuffer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty buffer with the default initial capacity.
			GenericStringBuffer();
			
			
			/// Create an empty buffer with the specified initial capacity and resize factor.
			GenericStringBuffer( Size initialCapacity, Float newResizeFactor = DEFAULT_RESIZE_FACTOR );
			
			
			/// Create an identical copy of the specified buffer.
			GenericStringBuffer( const GenericStringBuffer& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a buffer, deallocating all resources used.
			OM_INLINE ~GenericStringBuffer()
			{
				util::deallocate( buffer );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of one string buffer to another, performing a deep copy.
			GenericStringBuffer& operator = ( const GenericStringBuffer& other );
			
			
		//********************************************************************************
		//******	Append Methods
			
			
			/// Append an element to the end of this buffer.
			GenericStringBuffer& append( const CharType& character );
			
			
			/// Append all characters from the given NULL-terminated string.
			GenericStringBuffer& append( const CharType* source );
			
			
			/// Append the specified number of elements from the given string.
			GenericStringBuffer& append( const CharType* source, Size numElements );
			
			
			/// Append a string to this string buffer.
			OM_INLINE GenericStringBuffer& append( const GenericString<CharType>& string )
			{
				return append( string.getCString(), string.getLength() );
			}
			
			
			template < typename OtherCharType >
			OM_INLINE GenericStringBuffer& append( const GenericString<OtherCharType>& string )
			{
				return append( GenericString<CharType>(string) );
			}
			
			
			/// Append all elements from the specified character array to the end of the buffer.
			OM_INLINE GenericStringBuffer& append( const util::Array<CharType>& array )
			{
				return append( array.getPointer(), array.getSize() );
			}
			
			
			/// Append a certain number of elements from the specified array to the end of the buffer.
			OM_INLINE GenericStringBuffer& append( const util::Array<CharType>& array, Size number )
			{
				return append( array.getPointer(), math::min( number, array.getSize() ) );
			}
			
			
			/// Append all data from the specified buffer.
			OM_INLINE GenericStringBuffer& append( const GenericStringBuffer& aBuffer )
			{
				return append( aBuffer.buffer, aBuffer.getSize() );
			}
			
			
			/// Convert the object of templated type to a string and append it to the buffer.
			template < typename T >
			OM_INLINE GenericStringBuffer& append( const T& object )
			{
				return append( (GenericString<CharType>)object );
			}
			
			
		//********************************************************************************
		//******	Append Operators
			
			
			/// Append a character to the end of this string buffer.
			OM_INLINE GenericStringBuffer& operator << ( const CharType& character )
			{
				return append( character );
			}
			
			
			/// Append a NULL-terminated character string to the end of this buffer.
			OM_INLINE GenericStringBuffer& operator << ( const CharType* character )
			{
				return append( character );
			}
			
			
			/// Append a string to the end of this buffer.
			OM_INLINE GenericStringBuffer& operator << ( const GenericString<CharType>& string )
			{
				return append( string );
			}
			
			
			/// Append a string to the end of this buffer.
			template < typename OtherCharType >
			OM_INLINE GenericStringBuffer& operator << ( const GenericString<OtherCharType>& string )
			{
				return append( GenericString<CharType>(string) );
			}
			
			
			/// Append all elements from the specified character array to the end of the string buffer.
			OM_INLINE GenericStringBuffer& operator << ( const util::Array<CharType>& array )
			{
				return append( array );
			}
			
			
			/// Append all characters from the specified buffer to this buffer.
			OM_INLINE GenericStringBuffer& operator << ( const GenericStringBuffer& aBuffer )
			{
				return append( aBuffer );
			}
			
			
			/// Convert the object of templated type to a string and append it to the buffer.
			template < typename T >
			OM_INLINE GenericStringBuffer& operator << ( const T& object )
			{
				return append( (GenericString<CharType>)object );
			}
			
			
		//********************************************************************************
		//******	Remove Methods
			
			
			/// Remove the specified number of code points from the end of this string buffer.
			/**
			  * The method returns the number of character code points that were removed.
			  */
			Size remove( Size numCharacters )
			{
				// Don't remove more characters than are in the buffer.
				numCharacters = math::min( numCharacters, Size(nextElement - buffer) );
				
				// Rewind the next element pointer.
				nextElement -= numCharacters;
				
				// Make the string NULL-terminated.
				*nextElement = '\0';
				
				return numCharacters;
			}
			
			
		//********************************************************************************
		//******	Clear Method
			
			
			/// Clear the contents of the buffer, keeping its capacity intact.
			OM_INLINE void clear()
			{
				nextElement = buffer;
				*nextElement = '\0';
			}
			
			
		//********************************************************************************
		//******	Content Accessor Methods
			
			
			/// Convert the contents of this buffer to a string object.
			OM_INLINE GenericString<CharType> toString() const
			{
				return GenericString<CharType>( buffer, nextElement - buffer );
			}
			
			
			/// Convert the contents of this buffer to a string object.
			OM_INLINE operator GenericString<CharType> ()
			{
				return GenericString<CharType>( buffer, nextElement - buffer );
			}
			
			
			/// Get a pointer pointing to the buffer's internal array.
			OM_INLINE const CharType* getPointer() const
			{
				return buffer;
			}
			
			
			/// Get a pointer pointing to the buffer's internal array.
			OM_INLINE const CharType* getCString() const
			{
				return buffer;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Get the number of characters in the buffer, excluding the NULL terminator.
			OM_INLINE Size getSize() const
			{
				return nextElement - buffer;
			}
			
			
			/// Get the number of characters in the buffer, excluding the NULL terminator.
			OM_INLINE Size getLength() const
			{
				return nextElement - buffer;
			}
			
			
		//********************************************************************************
		//******	Capacity Accessor Methods
			
			
			/// Get the number of elements the buffer can hold without resizing.
			OM_INLINE Size getCapacity() const
			{
				return capacity;
			}
			
			
			/// Set the number of elements the buffer can hold.
			OM_INLINE Bool setCapacity( Size newCapacity )
			{
				if ( newCapacity < getSize() )
					return false;
				else
				{
					resize( newCapacity );
					return true;
				}
			}
			
			
		//********************************************************************************
		//******	Resize Factor Accessor Methods
			
			
			/// Get the resize factor for this buffer.
			OM_INLINE Float getResizeFactor() const
			{
				return resizeFactor;
			}
			
			
			/// Set the resize factor for this buffer, clamped to [1.1, 10.0]
			OM_INLINE void setResizeFactor( Float newResizeFactor )
			{
				resizeFactor = math::clamp( newResizeFactor, 1.1f, 10.0f );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Methods
			
			
			/// Increase the capacity to at least the specified amount using the resize factor.
			OM_INLINE void increaseCapacity( Size minimumCapacity )
			{
				resize( math::max( minimumCapacity, Size(Float(capacity)*resizeFactor) ) );
			}
			
			
			/// Increase the capacity to be the current factor multiplied by the resize factor.
			OM_INLINE void increaseCapacity()
			{
				resize( Size(Float(capacity)*resizeFactor) );
			}
			
			
			/// Resize the internal buffer to be the specified length.
			void resize( Size newCapacity );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of elements that is the buffer.
			CharType* buffer;
			
			
			/// A pointer to the location in the buffer where the next element should be inserted.
			CharType* nextElement;
			
			
			/// A pointer to the first element past the end of the buffer.
			const CharType* bufferEnd;
			
			
			/// The number of elements that the buffer can hold.
			Size capacity;
			
			
			/// How much the buffer's capacity increases when it needs to.
			Float resizeFactor;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default capacity for a buffer if it is not specified.
			static const Size DEFAULT_CAPACITY = 32;
			
			
			/// The default factor by which the buffer resizes.
			static const Float DEFAULT_RESIZE_FACTOR;
			
			
		//********************************************************************************
		//******	Friend Class Declarations
			
			
			/// Mark the StringInputStream class as a friend so that it can modify a buffer's internals.
			friend class om::io::StringInputStream;
			
			
			
};




template < typename CharType >
const Float		GenericStringBuffer<CharType>::	DEFAULT_RESIZE_FACTOR = 2.0f;




//##########################################################################################
//##########################################################################################
//############		
//############		String Buffer Type Definitions
//############		
//##########################################################################################
//##########################################################################################




/// A class that represents a buffer of ASCII encoded characters.
typedef GenericStringBuffer<Char> ASCIIStringBuffer;




/// A class that represents a buffer of ASCII encoded characters.
typedef ASCIIStringBuffer StringBuffer;




/// A class that represents a buffer of UTF-8 encoded characters.
typedef GenericStringBuffer<UTF8Char> UTF8StringBuffer;




/// A class that represents a buffer of UTF-16 encoded characters.
typedef GenericStringBuffer<UTF16Char> UTF16StringBuffer;




/// A class that represents a buffer of UTF-32 encoded characters.
typedef GenericStringBuffer<UTF32Char> UTF32StringBuffer;




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BASIC_STRING_BUFFER_H
