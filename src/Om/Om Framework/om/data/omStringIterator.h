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

#ifndef INCLUDE_OM_BASIC_STRING_ITERATOR_H
#define INCLUDE_OM_BASIC_STRING_ITERATOR_H


#include "omDataConfig.h"


#include "omString.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// Declaration for a string iterator which iterates over strings with the specified character type.
template < typename CharType >
class GenericStringIterator;




//##########################################################################################
//##########################################################################################
//############		
//############		ASCII Basic String Iterator Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that iterates over ASCII character strings.
template <>
class GenericStringIterator<Char>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a string iterator that iterates over the specified NULL-terminated string.
			OM_INLINE GenericStringIterator( const Char* string )
				:	current( string ),
					start( string ),
					end( NULL )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified string.
			OM_INLINE GenericStringIterator( const Char* string, Size length )
				:	current( string ),
					start( string ),
					end( string + length )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified string object.
			OM_INLINE GenericStringIterator( const GenericString<Char>& string )
				:	current( string.getCString() ),
					start( string.getCString() )
			{
				end = start + string.getLength();
			}
			
			
		//********************************************************************************
		//******	Increment Methods
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ()
			{
				current++;
			}
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ( int )
			{
				current++;
			}
			
			
		//********************************************************************************
		//******	Iterator State Accessor Method
			
			
			/// Return whether or not the end of the string has been reached.
			OM_INLINE operator Bool () const
			{
				if ( end )
					return current != end;
				else
					return *current != '\0';
			}
			
			
			/// Return the current code point index within the string being iterated over.
			OM_INLINE Index getIndex() const
			{
				return current - start;
			}
			
			
			/// Return the index of the next character in the string.
			OM_INLINE Index getCharacterIndex() const
			{
				return current - start;
			}
			
			
		//********************************************************************************
		//******	Current Character Accessor Method
			
			
			/// Return the current character of the iterator.
			OM_INLINE Char operator * () const
			{
				return *current;
			}
			
			
			/// Return a pointer to the current character of the iterator.
			OM_INLINE operator const Char* () const
			{
				return current;
			}
			
			
		//********************************************************************************
		//******	Iterator Reset Method
			
			
			/// Reset the iterator to the first character in the string it is iterating over.
			OM_INLINE void reset()
			{
				current = start;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the current character of the string iterator.
			const Char* current;
			
			
			/// A pointer to the first character of the string being iterated over.
			const Char* start;
			
			
			/// A pointer to just past the last character of the string being iterated over.
			const Char* end;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-8 Basic String Iterator Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that iterates over UTF-8 encoded character strings.
template <>
class GenericStringIterator<UTF8Char>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a string iterator that iterates over the specified NULL-terminated string.
			OM_INLINE GenericStringIterator( const UTF8Char* string )
				:	current( string ),
					start( string ),
					end( NULL ),
					characterIndex( 0 )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified string.
			OM_INLINE GenericStringIterator( const UTF8Char* string, Size length )
				:	current( string ),
					start( string ),
					end( string + length ),
					characterIndex( 0 )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified string object.
			OM_INLINE GenericStringIterator( const GenericString<UTF8Char>& string )
				:	current( string.getCString() ),
					start( string.getCString() ),
					characterIndex( 0 )
			{
				end = start + string.getLength();
			}
			
			
		//********************************************************************************
		//******	Increment Methods
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ()
			{
				(*this)++;
			}
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ( int )
			{
				if ( *current < 0x80 )
				{
					// This is a single-byte UTF-8 character, skip one byte.
					current++;
				}
				else
					this->advanceMultiByteCharacter();
				
				characterIndex++;
			}
			
			
		//********************************************************************************
		//******	Iterator State Accessor Method
			
			
			/// Return whether or not the end of the string has been reached.
			OM_INLINE operator Bool () const
			{
				if ( end )
					return current != end;
				else
					return *current != '\0';
			}
			
			
			/// Return the current code point index within the string being iterated over.
			OM_INLINE Index getIndex() const
			{
				return current - start;
			}
			
			
			/// Return the index of the next character in the string.
			/**
			  * This is not the same as the code point index because there may be
			  * characters which use more than one code point in their representation. 
			  */
			OM_INLINE Index getCharacterIndex() const
			{
				return characterIndex;
			}
			
			
		//********************************************************************************
		//******	Current Character Accessor Method
			
			
			/// Return the current character of the iterator.
			OM_INLINE UTF32Char operator * () const
			{
				if ( *current < 0x80 )
				{
					// This is a single-byte UTF-8 character.
					return UTF32Char(*current);
				}
				else
					return this->getMultiByteCharacter();
			}
			
			
			/// Return a pointer to the current character of the iterator.
			OM_INLINE operator const UTF8Char* () const
			{
				return current;
			}
			
			
		//********************************************************************************
		//******	Iterator Reset Method
			
			
			/// Reset the iterator to the first character in the string it is iterating over.
			OM_INLINE void reset()
			{
				current = start;
				characterIndex = 0;
			}
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Return a UTF-32 character when the current character describes the start of multibyte character.
			UTF32Char getMultiByteCharacter() const;
			
			
			/// Advance to the next character if it the current character is a multibyte character.
			void advanceMultiByteCharacter();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the current character of the string iterator.
			const UTF8Char* current;
			
			
			/// A pointer to the first character of the string being iterated over.
			const UTF8Char* start;
			
			
			/// A pointer to just past the last character of the string being iterated over.
			const UTF8Char* end;
			
			
			/// The index of the next character in the string, independent of the code point index.
			Index characterIndex;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-16 Basic String Iterator Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that iterates over UTF-16 encoded character strings.
template <>
class GenericStringIterator<UTF16Char>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a string iterator that iterates over the specified NULL-terminated string.
			OM_INLINE GenericStringIterator( const UTF16Char* string )
				:	current( string ),
					start( string ),
					end( NULL ),
					characterIndex( 0 )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified NULL-terminated string.
			OM_INLINE GenericStringIterator( const UTF16Char* string, Size length )
				:	current( string ),
					start( string ),
					end( string + length ),
					characterIndex( 0 )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified string object.
			OM_INLINE GenericStringIterator( const GenericString<UTF16Char>& string )
				:	current( string.getCString() ),
					start( string.getCString() ),
					characterIndex( 0 )
			{
				end = start + string.getLength();
			}
			
			
		//********************************************************************************
		//******	Increment Methods
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ()
			{
				(*this)++;
			}
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ( int )
			{
				if ( *current >= 0xD800 && *current <= 0xDBFF )
				{
					// This is two-point UTF-16 character. Skip two points.
					current += 2;
				}
				else
				{
					// This is either a malformed character or a single point character. Skip one.
					current++;
				}
				
				characterIndex++;
			}
			
			
		//********************************************************************************
		//******	Iterator State Accessor Method
			
			
			/// Return whether or not the end of the string has been reached.
			OM_INLINE operator Bool () const
			{
				if ( end )
					return current != end;
				else
					return *current != '\0';
			}
			
			
			/// Return the current code point index within the string being iterated over.
			OM_INLINE Index getIndex() const
			{
				return current - start;
			}
			
			
			/// Return the index of the next character in the string.
			/**
			  * This is not the same as the code point index because there may be
			  * characters which use more than one code point in their representation. 
			  */
			OM_INLINE Index getCharacterIndex() const
			{
				return characterIndex;
			}
			
			
		//********************************************************************************
		//******	Current Character Accessor Method
			
			
			/// Return the current character of the iterator.
			UTF32Char operator * () const;
			
			
			/// Return a pointer to the current character of the iterator.
			OM_INLINE operator const UTF16Char* () const
			{
				return current;
			}
			
			
		//********************************************************************************
		//******	Iterator Reset Method
			
			
			/// Reset the iterator to the first character in the string it is iterating over.
			OM_INLINE void reset()
			{
				current = start;
				characterIndex = 0;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the current character of the string iterator.
			const UTF16Char* current;
			
			
			/// A pointer to the first character of the string being iterated over.
			const UTF16Char* start;
			
			
			/// A pointer to just past the last character of the string being iterated over.
			const UTF16Char* end;
			
			
			/// The index of the next character in the string, independent of the code point index.
			Index characterIndex;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-32 Basic String Iterator Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that iterates over UTF-32 encoded character strings.
template <>
class GenericStringIterator<UTF32Char>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a string iterator that iterates over the specified NULL-terminated string.
			OM_INLINE GenericStringIterator( const UTF32Char* string )
				:	current( string ),
					start( string ),
					end( NULL )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified string.
			OM_INLINE GenericStringIterator( const UTF32Char* string, Size length )
				:	current( string ),
					start( string ),
					end( string + length )
			{
				OM_DEBUG_ASSERT_MESSAGE( string != NULL, "Cannot iterate over NULL string" );
			}
			
			
			/// Create a string iterator that iterates over the specified string object.
			OM_INLINE GenericStringIterator( const GenericString<UTF32Char>& string )
				:	current( string.getCString() ),
					start( string.getCString() )
			{
				end = start + string.getLength();
			}
			
			
		//********************************************************************************
		//******	Increment Methods
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ()
			{
				current++;
			}
			
			
			/// Increment the iterator to the next character in the string.
			OM_INLINE void operator ++ ( int )
			{
				current++;
			}
			
			
		//********************************************************************************
		//******	Iterator State Accessor Method
			
			
			/// Return whether or not the end of the string has been reached.
			OM_INLINE operator Bool () const
			{
				if ( end )
					return current != end;
				else
					return *current != '\0';
			}
			
			
			/// Return the current code point index within the string being iterated over.
			OM_INLINE Index getIndex() const
			{
				return current - start;
			}
			
			
			/// Return the character index within the string being iterated over.
			OM_INLINE Index getCharacterIndex() const
			{
				return current - start;
			}
			
			
		//********************************************************************************
		//******	Current Character Accessor Method
			
			
			/// Return the current character of the iterator.
			OM_INLINE UTF32Char operator * () const
			{
				return *current;
			}
			
			
			/// Return a pointer to the current character of the iterator.
			OM_INLINE operator const UTF32Char* () const
			{
				return current;
			}
			
			
		//********************************************************************************
		//******	Iterator Reset Method
			
			
			/// Reset the iterator to the first character in the string it is iterating over.
			OM_INLINE void reset()
			{
				current = start;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the current character of the string iterator.
			const UTF32Char* current;
			
			
			/// A pointer to the first character of the string being iterated over.
			const UTF32Char* start;
			
			
			/// A pointer to just past the last character of the string being iterated over.
			const UTF32Char* end;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		String Iterator Type Definitions
//############		
//##########################################################################################
//##########################################################################################




/// A class that iterates over ASCII encoded character strings.
typedef GenericStringIterator<Char> ASCIIStringIterator;




/// A class that iterates over ASCII encoded character strings.
typedef ASCIIStringIterator StringIterator;




/// A class that iterates over UTF-8 encoded character strings.
typedef GenericStringIterator<UTF8Char> UTF8StringIterator;




/// A class that iterates over UTF-16 encoded character strings.
typedef GenericStringIterator<UTF16Char> UTF16StringIterator;




/// A class that iterates over UTF-32 encoded character strings.
typedef GenericStringIterator<UTF32Char> UTF32StringIterator;




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BASIC_STRING_ITERATOR_H
