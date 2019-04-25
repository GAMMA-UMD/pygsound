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

#include "omString.h"


#include "omStringIterator.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		String Literal Instantiations
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericString<Char>::SharedStaticString<1> GenericString<Char>:: nullString( "" );
template <>
GenericString<UTF8Char>::SharedStaticString<1> GenericString<UTF8Char>:: nullString( "" );
template <>
GenericString<UTF16Char>::SharedStaticString<1> GenericString<UTF16Char>:: nullString( "" );
template <>
GenericString<UTF32Char>::SharedStaticString<1> GenericString<UTF32Char>:: nullString( "" );




template <>
GenericString<Char>::SharedStaticString<5> GenericString<Char>:: trueString( "true" );
template <>
GenericString<UTF8Char>::SharedStaticString<5> GenericString<UTF8Char>:: trueString( "true" );
template <>
GenericString<UTF16Char>::SharedStaticString<5> GenericString<UTF16Char>:: trueString( "true" );
template <>
GenericString<UTF32Char>::SharedStaticString<5> GenericString<UTF32Char>:: trueString( "true" );




template <>
GenericString<Char>::SharedStaticString<6> GenericString<Char>:: falseString( "false" );
template <>
GenericString<UTF8Char>::SharedStaticString<6> GenericString<UTF8Char>:: falseString( "false" );
template <>
GenericString<UTF16Char>::SharedStaticString<6> GenericString<UTF16Char>:: falseString( "false" );
template <>
GenericString<UTF32Char>::SharedStaticString<6> GenericString<UTF32Char>:: falseString( "false" );




template <>
GenericString<Char>::SharedStaticString<9> GenericString<Char>:: positiveInfinityString( "Infinity" );
template <>
GenericString<UTF8Char>::SharedStaticString<9> GenericString<UTF8Char>:: positiveInfinityString( "Infinity" );
template <>
GenericString<UTF16Char>::SharedStaticString<9> GenericString<UTF16Char>:: positiveInfinityString( "Infinity" );
template <>
GenericString<UTF32Char>::SharedStaticString<9> GenericString<UTF32Char>:: positiveInfinityString( "Infinity" );




template <>
GenericString<Char>::SharedStaticString<10> GenericString<Char>:: negativeInfinityString( "-Infinity" );
template <>
GenericString<UTF8Char>::SharedStaticString<10> GenericString<UTF8Char>:: negativeInfinityString( "-Infinity" );
template <>
GenericString<UTF16Char>::SharedStaticString<10> GenericString<UTF16Char>:: negativeInfinityString( "-Infinity" );
template <>
GenericString<UTF32Char>::SharedStaticString<10> GenericString<UTF32Char>:: negativeInfinityString( "-Infinity" );




template <>
GenericString<Char>::SharedStaticString<4> GenericString<Char>:: nanString( "NaN" );
template <>
GenericString<UTF8Char>::SharedStaticString<4> GenericString<UTF8Char>:: nanString( "NaN" );
template <>
GenericString<UTF16Char>::SharedStaticString<4> GenericString<UTF16Char>:: nanString( "NaN" );
template <>
GenericString<UTF32Char>::SharedStaticString<4> GenericString<UTF32Char>:: nanString( "NaN" );




//##########################################################################################
//##########################################################################################
//############		
//############		Local Helper Functions
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
static Size getStringLength( const CharType* string )
{
	const CharType* const stringStart = string;
	
	while ( *string )
		string++;
	
	return string - stringStart;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unicode Length Functions
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
static Size getUnicodeStringLength( const CharType* string )
{
	Size numCharacters = 0;
	
	GenericStringIterator<CharType> iterator(string);
	
	while ( iterator )
	{
		numCharacters++;
		iterator++;
	}
	
	return numCharacters;
}




template < typename CharType >
static Size getUnicodeStringLength( const CharType* string, const CharType* stringEnd )
{
	Size numCharacters = 0;
	
	GenericStringIterator<CharType> iterator( string, stringEnd - string );
	
	while ( iterator )
	{
		numCharacters++;
		iterator++;
	}
	
	return numCharacters;
}




template < typename CharType >
static Size getUnicodeStringLengthInUTF8( const CharType* string )
{
	Size numCodePoints = 0;
	
	GenericStringIterator<CharType> iterator(string);
	
	while ( iterator )
	{
		UTF32Char character = *iterator;
		
		if ( character <= 0x0000007F )
			numCodePoints++;
		else if ( character <= 0x000007FF )
			numCodePoints += 2;
		else if ( character <= 0x0000D7FF ||
				(character >= 0x0000E000 && character <= 0x0000FFFF) ||
				character < 0x0000E000 )
		{
			numCodePoints += 3;
		}
		else if ( character <= 0x0010FFFF )
			numCodePoints += 4;
		else
			numCodePoints += 3;
		
		iterator++;
	}
	
	return numCodePoints;
}




template < typename CharType >
static Size getUnicodeStringLengthInUTF8( const CharType* string, const CharType* stringEnd )
{
	Size numCodePoints = 0;
	
	GenericStringIterator<CharType> iterator( string, stringEnd - string );
	
	while ( iterator )
	{
		UTF32Char character = *iterator;
		
		if ( character <= 0x0000007F )
			numCodePoints++;
		else if ( character <= 0x000007FF )
			numCodePoints += 2;
		else if ( character <= 0x0000D7FF ||
				(character >= 0x0000E000 && character <= 0x0000FFFF) ||
				character < 0x0000E000 )
		{
			numCodePoints += 3;
		}
		else if ( character <= 0x0010FFFF )
			numCodePoints += 4;
		else
			numCodePoints += 3;
		
		iterator++;
	}
	
	return numCodePoints;
}




template < typename CharType >
static Size getUnicodeStringLengthInUTF16( const CharType* string )
{
	Size numCodePoints = 0;
	
	GenericStringIterator<CharType> iterator(string);
	
	while ( iterator )
	{
		UTF32Char character = *iterator;
		
		if ( character <= 0x0000D7FF || (character >= 0x0000E000 && character <= 0x0000FFFF) )
			numCodePoints++;
		else if ( character < 0x0000E000 )
			numCodePoints++;
		else if ( character <= 0x0010FFFF )
			numCodePoints += 2;
		else
			numCodePoints++;
		
		iterator++;
	}
	
	return numCodePoints;
}




template < typename CharType >
static Size getUnicodeStringLengthInUTF16( const CharType* string, const CharType* stringEnd )
{
	Size numCodePoints = 0;
	
	GenericStringIterator<CharType> iterator( string, stringEnd - string );
	
	while ( iterator )
	{
		UTF32Char character = *iterator;
		
		if ( character <= 0x0000D7FF || (character >= 0x0000E000 && character <= 0x0000FFFF) )
			numCodePoints++;
		else if ( character < 0x0000E000 )
			numCodePoints++;
		else if ( character <= 0x0010FFFF )
			numCodePoints += 2;
		else
			numCodePoints++;
		
		iterator++;
	}
	
	return numCodePoints;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unicode to ASCII Conversion Functions
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType2 >
static void convertUnicodeToASCII( GenericStringIterator<CharType2>& iterator, Char* characters )
{
	Char* currentCharacter = characters;
	
	while ( iterator )
	{
		UTF32Char character = *iterator;
		
		if ( character < 0x80 )
			*currentCharacter = (Char)character;
		else
			*currentCharacter = '?';
		
		currentCharacter++;
		iterator++;
	}
	
	*currentCharacter = '\0';
}




template <> template < typename CharType2 >
typename GenericString<Char>::SharedString* GenericString<Char>:: convertUnicode( const CharType2* unicodeString )
{
	Size outputLength = getUnicodeStringLength( unicodeString ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString );
	
	convertUnicodeToASCII( iterator, string->getCharacters() );
	
	return string;
}




template <> template < typename CharType2 >
typename GenericString<Char>::SharedString* GenericString<Char>:: convertUnicode( const CharType2* unicodeString,
																			const CharType2* unicodeStringEnd )
{
	Size outputLength = getUnicodeStringLength( unicodeString, unicodeStringEnd ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString, unicodeStringEnd - unicodeString );
	
	convertUnicodeToASCII( iterator, string->getCharacters() );
	
	return string;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unicode to UTF8 Conversion Functions
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType2 >
static void convertUnicodeToUTF8( GenericStringIterator<CharType2>& iterator, UTF8Char* characters )
{
	UTF8Char* currentCharacter = characters;
	
	while ( iterator )
	{
		UTF32Char character = *iterator;
		
		if ( character <= 0x0000007F )
		{
			currentCharacter[0] = (UTF8Char)character;
			currentCharacter++;
		}
		else if ( character <= 0x000007FF )
		{
			currentCharacter[0] = (UTF8Char)(0xC0 | ((character >> 6) & 0x1F));
			currentCharacter[1] = (UTF8Char)(0x80 | (character & 0x3F));
			currentCharacter += 2;
		}
		else if ( character <= 0x0000D7FF ||
					(character >= 0x0000E000 && 
					character <= 0x0000FFFF) )
		{
			currentCharacter[0] = (UTF8Char)(0xE0 | ((character >> 12) & 0x0F));
			currentCharacter[1] = (UTF8Char)(0x80 | ((character >> 6)  & 0x3F));
			currentCharacter[2] = (UTF8Char)(0x80 | (character & 0x3F));
			currentCharacter += 3;
		}
		else if ( character < 0x0000E000)
		{
			currentCharacter[0] = (UTF8Char)0xEF;
			currentCharacter[1] = (UTF8Char)0xBF;
			currentCharacter[2] = (UTF8Char)0xBD;
			currentCharacter += 3;
		}
		else if ( character <= 0x0010FFFF )
		{
			currentCharacter[0] = (UTF8Char)(0xF0 | ((character >> 18) & 0x07));
			currentCharacter[1] = (UTF8Char)(0x80 | ((character >> 12) & 0x3F));
			currentCharacter[2] = (UTF8Char)(0x80 | ((character >> 6)  & 0x3F));
			currentCharacter[3] = (UTF8Char)(0x80 | (character & 0x3F));
			currentCharacter += 4;
		}
		else
		{
			currentCharacter[0] = (UTF8Char)0xEF;
			currentCharacter[1] = (UTF8Char)0xBF;
			currentCharacter[2] = (UTF8Char)0xBD;
			currentCharacter += 3;
		}
		
		iterator++;
	}
	
	*currentCharacter = '\0';
}




template <> template < typename CharType2 >
typename GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: convertUnicode( const CharType2* unicodeString )
{
	Size outputLength = getUnicodeStringLength( unicodeString ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString );
	
	convertUnicodeToUTF8( iterator, string->getCharacters() );
	
	return string;
}




template <> template < typename CharType2 >
typename GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: convertUnicode( const CharType2* unicodeString,
																						const CharType2* unicodeStringEnd )
{
	Size outputLength = getUnicodeStringLength( unicodeString, unicodeStringEnd ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString, unicodeStringEnd - unicodeString );
	
	convertUnicodeToUTF8( iterator, string->getCharacters() );
	
	return string;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unicode to UTF16 Conversion Functions
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType2 >
static void convertUnicodeToUTF16( GenericStringIterator<CharType2>& iterator, UTF16Char* characters )
{
	UTF16Char* currentCharacter = characters;
	
	while ( iterator )
	{
		UTF32Char character = *iterator;
		
		if ( character <= 0x0000D7FF || (character >= 0x0000E000 && character <= 0x0000FFFF) )
		{
			currentCharacter[0] = (UTF16Char)character;
			currentCharacter++;
		}
		else if ( character < 0x0000E000 )
		{
			currentCharacter[0] = (UTF16Char)0xFFFD;
			currentCharacter++;
		}
		else if ( character <= 0x0010FFFF )
		{
			currentCharacter[0] = (UTF16Char)(0xD800 | ((character - 0x00010000) >> 10));
			currentCharacter[1] = (UTF16Char)(0xDC00 | (character & 0x03FF));
			currentCharacter += 2;
		}
		else
		{
			currentCharacter[0] = (UTF16Char)0xFFFD;
			currentCharacter++;
		}
		
		iterator++;
	}
	
	*currentCharacter = '\0';
}




template <> template < typename CharType2 >
typename GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: convertUnicode( const CharType2* unicodeString )
{
	Size outputLength = getUnicodeStringLength( unicodeString ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString );
	
	convertUnicodeToUTF16( iterator, string->getCharacters() );
	
	return string;
}




template <> template < typename CharType2 >
typename GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: convertUnicode( const CharType2* unicodeString,
																						const CharType2* unicodeStringEnd )
{
	Size outputLength = getUnicodeStringLength( unicodeString, unicodeStringEnd ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString, unicodeStringEnd - unicodeString );
	
	convertUnicodeToUTF16( iterator, string->getCharacters() );
	
	return string;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unicode to UTF32 Conversion Functions
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType2 >
static void convertUnicodeToUTF32( GenericStringIterator<CharType2>& iterator, UTF32Char* characters )
{
	UTF32Char* currentCharacter = characters;
	
	while ( iterator )
	{
		*currentCharacter = *iterator;
		
		currentCharacter++;
		iterator++;
	}
	
	*currentCharacter = '\0';
}




template <> template < typename CharType2 >
typename GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: convertUnicode( const CharType2* unicodeString )
{
	Size outputLength = getUnicodeStringLength( unicodeString ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString );
	
	convertUnicodeToUTF32( iterator, string->getCharacters() );
	
	return string;
}




template <> template < typename CharType2 >
typename GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: convertUnicode( const CharType2* unicodeString,
																						const CharType2* unicodeStringEnd )
{
	Size outputLength = getUnicodeStringLength( unicodeString, unicodeStringEnd ) + 1;
	
	// Allocate space for the new string, plus a NULL terminator.
	SharedString* string = allocateString( outputLength );
	
	// Convert the string.
	GenericStringIterator<CharType2> iterator( unicodeString, unicodeStringEnd - unicodeString );
	
	convertUnicodeToUTF32( iterator, string->getCharacters() );
	
	return string;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericString<Char>:: GenericString( const Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = getStringLength(array) + 1;
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
}




template <>
GenericString<Char>:: GenericString( const UTF8Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<Char>:: GenericString( const UTF16Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<Char>:: GenericString( const UTF32Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<UTF8Char>:: GenericString( const Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = getStringLength(array) + 1;
	shared = allocateString(length);
	string = shared->getCharacters();
	util::convert( string, array, length );
}




template <>
GenericString<UTF8Char>:: GenericString( const UTF8Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = getStringLength(array) + 1;
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
}




template <>
GenericString<UTF8Char>:: GenericString( const UTF16Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<UTF8Char>:: GenericString( const UTF32Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<UTF16Char>:: GenericString( const Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = getStringLength(array) + 1;
	shared = allocateString(length);
	string = shared->getCharacters();
	util::convert( string, array, length );
}




template <>
GenericString<UTF16Char>:: GenericString( const UTF8Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<UTF16Char>:: GenericString( const UTF16Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = getStringLength(array) + 1;
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
}




template <>
GenericString<UTF16Char>:: GenericString( const UTF32Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<UTF32Char>:: GenericString( const Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = getStringLength(array) + 1;
	shared = allocateString(length);
	string = shared->getCharacters();
	util::convert( string, array, length );
}




template <>
GenericString<UTF32Char>:: GenericString( const UTF8Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<UTF32Char>:: GenericString( const UTF16Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array );
	string = shared->getCharacters();
}




template <>
GenericString<UTF32Char>:: GenericString( const UTF32Char* array )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = getStringLength(array) + 1;
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
}




template <>
GenericString<Char>:: GenericString( const Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = newLength;
	
	if ( newLength == 0 || array[newLength - 1] != '\0' )
		length += 1;
	
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
	string[shared->length - 1] = '\0';
}




template <>
GenericString<Char>:: GenericString( const UTF8Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<Char>:: GenericString( const UTF16Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<Char>:: GenericString( const UTF32Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<UTF8Char>:: GenericString( const Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = newLength;
	
	if ( newLength == 0 || array[newLength - 1] != '\0' )
		length += 1;
	
	shared = allocateString(length);
	string = shared->getCharacters();
	util::convert( string, array, length );
	string[shared->length - 1] = '\0';
}




template <>
GenericString<UTF8Char>:: GenericString( const UTF8Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = newLength;
	
	if ( newLength == 0 || array[newLength - 1] != '\0' )
		length += 1;
	
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
	string[shared->length - 1] = '\0';
}




template <>
GenericString<UTF8Char>:: GenericString( const UTF16Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<UTF8Char>:: GenericString( const UTF32Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<UTF16Char>:: GenericString( const Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = newLength;
	
	if ( newLength == 0 || array[newLength - 1] != '\0' )
		length += 1;
	
	shared = allocateString(length);
	string = shared->getCharacters();
	util::convert( string, array, length );
	string[shared->length - 1] = '\0';
}




template <>
GenericString<UTF16Char>:: GenericString( const UTF8Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<UTF16Char>:: GenericString( const UTF16Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = newLength;
	
	if ( newLength == 0 || array[newLength - 1] != '\0' )
		length += 1;
	
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
	string[shared->length - 1] = '\0';
}




template <>
GenericString<UTF16Char>:: GenericString( const UTF32Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<UTF32Char>:: GenericString( const Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = newLength;
	
	if ( newLength == 0 || array[newLength - 1] != '\0' )
		length += 1;
	
	shared = allocateString(length);
	string = shared->getCharacters();
	util::convert( string, array, length );
	string[shared->length - 1] = '\0';
}




template <>
GenericString<UTF32Char>:: GenericString( const UTF8Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<UTF32Char>:: GenericString( const UTF16Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	shared = convertUnicode( array, array + newLength );
	string = shared->getCharacters();
}




template <>
GenericString<UTF32Char>:: GenericString( const UTF32Char* array, Size newLength )
{
	OM_DEBUG_ASSERT_MESSAGE( array != NULL, "Cannot create a string from a NULL character array" );
	
	Size length = newLength;
	
	if ( newLength == 0 || array[newLength - 1] != '\0' )
		length += 1;
	
	shared = allocateString(length);
	string = shared->getCharacters();
	util::copy( string, array, length );
	string[shared->length - 1] = '\0';
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unicode Conversion Copy Constructors
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
GenericString<Char>:: GenericString( const GenericString<UTF8Char>& other )
{
	shared = convertUnicode( other.string );
	string = shared->getCharacters();
}




template <> template <>
GenericString<Char>:: GenericString( const GenericString<UTF16Char>& other )
{
	shared = convertUnicode( other.string );
	string = shared->getCharacters();
}




template <> template <>
GenericString<Char>:: GenericString( const GenericString<UTF32Char>& other )
{
	shared = convertUnicode( other.string, other.string + other.getLength() );
	string = shared->getCharacters();
}




template <> template <>
GenericString<UTF8Char>:: GenericString( const GenericString<Char>& other )
{
	shared = allocateString( other.shared->length );
	string = shared->getCharacters();
	util::convert( string, other.string, other.shared->length );
}




template <> template <>
GenericString<UTF8Char>:: GenericString( const GenericString<UTF16Char>& other )
{
	shared = convertUnicode( other.string, other.string + other.getLength() );
	string = shared->getCharacters();
}




template <> template <>
GenericString<UTF8Char>:: GenericString( const GenericString<UTF32Char>& other )
{
	shared = convertUnicode( other.string, other.string + other.getLength() );
	string = shared->getCharacters();
}




template <> template <>
GenericString<UTF16Char>:: GenericString( const GenericString<Char>& other )
{
	shared = allocateString( other.shared->length );
	string = shared->getCharacters();
	util::convert( string, other.string, other.shared->length );
}




template <> template <>
GenericString<UTF16Char>:: GenericString( const GenericString<UTF8Char>& other )
{
	shared = convertUnicode( other.string, other.string + other.getLength() );
	string = shared->getCharacters();
}




template <> template <>
GenericString<UTF16Char>:: GenericString( const GenericString<UTF32Char>& other )
{
	shared = convertUnicode( other.string, other.string + other.getLength() );
	string = shared->getCharacters();
}




template <> template <>
GenericString<UTF32Char>:: GenericString( const GenericString<Char>& other )
{
	shared = allocateString( other.shared->length );
	string = shared->getCharacters();
	util::convert( string, other.string, other.shared->length );
}




template <> template <>
GenericString<UTF32Char>:: GenericString( const GenericString<UTF8Char>& other )
{
	shared = convertUnicode( other.string, other.string + other.getLength() );
	string = shared->getCharacters();
}




template <> template <>
GenericString<UTF32Char>:: GenericString( const GenericString<UTF16Char>& other )
{
	shared = convertUnicode( other.string, other.string + other.getLength() );
	string = shared->getCharacters();
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Equality Comparison Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
OM_INLINE static Bool stringEquals( const CharType* string1, const CharType* string2 )
{
	while ( *string1 && *string2 )
	{
		if ( *string1 != *string2 )
			return false;
		
		string1++;
		string2++;
	}
	
	return *string1 == '\0' && *string2 == '\0';
}




template <>
Bool GenericString<Char>:: equals( const Char* string1, const Char* string2 )
{
	return stringEquals( string1, string2 );
}




template <>
Bool GenericString<UTF8Char>:: equals( const UTF8Char* string1, const UTF8Char* string2 )
{
	return stringEquals( string1, string2 );
}




template <>
Bool GenericString<UTF16Char>:: equals( const UTF16Char* string1, const UTF16Char* string2 )
{
	return stringEquals( string1, string2 );
}




template <>
Bool GenericString<UTF32Char>:: equals( const UTF32Char* string1, const UTF32Char* string2 )
{
	return stringEquals( string1, string2 );
}




template < typename CharType >
OM_INLINE static Bool stringEquals( const CharType* string1, const CharType* string2, Size length )
{
	const CharType* const string1End = string1 + length;
	
	while ( string1 != string1End )
	{
		if ( *string1 != *string2 )
			return false;
		
		string1++;
		string2++;
	}
	
	return true;
}




template <>
Bool GenericString<Char>:: equals( const Char* string1, const Char* string2, Size length )
{
	return stringEquals( string1, string2, length );
}




template <>
Bool GenericString<UTF8Char>:: equals( const UTF8Char* string1, const UTF8Char* string2, Size length )
{
	return stringEquals( string1, string2, length );
}




template <>
Bool GenericString<UTF16Char>:: equals( const UTF16Char* string1, const UTF16Char* string2, Size length )
{
	return stringEquals( string1, string2, length );
}




template <>
Bool GenericString<UTF32Char>:: equals( const UTF32Char* string1, const UTF32Char* string2, Size length )
{
	return stringEquals( string1, string2, length );
}




template < typename CharType >
OM_INLINE static Bool stringEqualsIgnoreCase( const CharType* string1, const CharType* string2 )
{
	while ( *string1 && *string2 )
	{
		if ( GenericString<CharType>::toLowerCase(*string1) != 
			GenericString<CharType>::toLowerCase(*string2) )
			return false;
		
		string1++;
		string2++;
	}
	
	return *string1 == '\0' && *string2 == '\0';
}




template <>
Bool GenericString<Char>:: equalsIgnoreCase( const Char* string1, const Char* string2 )
{
	return stringEqualsIgnoreCase( string1, string2 );
}




template <>
Bool GenericString<UTF8Char>:: equalsIgnoreCase( const UTF8Char* string1, const UTF8Char* string2 )
{
	return stringEqualsIgnoreCase( string1, string2 );
}




template <>
Bool GenericString<UTF16Char>:: equalsIgnoreCase( const UTF16Char* string1, const UTF16Char* string2 )
{
	return stringEqualsIgnoreCase( string1, string2 );
}




template <>
Bool GenericString<UTF32Char>:: equalsIgnoreCase( const UTF32Char* string1, const UTF32Char* string2 )
{
	return stringEqualsIgnoreCase( string1, string2 );
}




template < typename CharType >
OM_INLINE static Bool stringEqualsIgnoreCase( const CharType* string1, const CharType* string2, Size length )
{
	const CharType* const string1End = string1 + length;
	
	while ( string1 != string1End )
	{
		if ( GenericString<CharType>::toLowerCase(*string1) != 
			GenericString<CharType>::toLowerCase(*string2) )
			return false;
		
		string1++;
		string2++;
	}
	
	return true;
}




template <>
Bool GenericString<Char>:: equalsIgnoreCase( const Char* string1, const Char* string2, Size length )
{
	return stringEqualsIgnoreCase( string1, string2, length );
}




template <>
Bool GenericString<UTF8Char>:: equalsIgnoreCase( const UTF8Char* string1, const UTF8Char* string2, Size length )
{
	return stringEqualsIgnoreCase( string1, string2, length );
}




template <>
Bool GenericString<UTF16Char>:: equalsIgnoreCase( const UTF16Char* string1, const UTF16Char* string2, Size length )
{
	return stringEqualsIgnoreCase( string1, string2, length );
}




template <>
Bool GenericString<UTF32Char>:: equalsIgnoreCase( const UTF32Char* string1, const UTF32Char* string2, Size length )
{
	return stringEqualsIgnoreCase( string1, string2, length );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Sorting Comparison Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
OM_INLINE static Int stringCompare( const CharType* string1, const CharType* string2 )
{
	while ( *string1 && *string2 )
	{
		if ( *string1 > *string2 )
			return 1;
		else if ( *string1 < *string2 )
			return -1;
		
		string1++;
		string2++;
	}
	
	if ( *string1 == '\0' )
	{
		if ( *string2 == '\0' )
			return 0;
		else
			return -1;
	}
	else
		return 1;
}




template <>
Int GenericString<Char>:: compare( const Char* string1, const Char* string2 )
{
	return stringCompare( string1, string2 );
}




template <>
Int GenericString<UTF8Char>:: compare( const UTF8Char* string1, const UTF8Char* string2 )
{
	return stringCompare( string1, string2 );
}




template <>
Int GenericString<UTF16Char>:: compare( const UTF16Char* string1, const UTF16Char* string2 )
{
	return stringCompare( string1, string2 );
}




template <>
Int GenericString<UTF32Char>:: compare( const UTF32Char* string1, const UTF32Char* string2 )
{
	return stringCompare( string1, string2 );
}




template < typename CharType >
OM_INLINE static Int stringCompareIgnoreCase( const CharType* string1, const CharType* string2 )
{
	while ( *string1 && *string2 )
	{
		if ( GenericString<CharType>::toLowerCase(*string1) > GenericString<CharType>::toLowerCase(*string2) )
			return 1;
		else if ( GenericString<CharType>::toLowerCase(*string1) < GenericString<CharType>::toLowerCase(*string2 ) )
			return -1;
		
		string1++;
		string2++;
	}
	
	if ( *string1 == '\0' )
	{
		if ( *string2 == '\0' )
			return 0;
		else
			return -1;
	}
	else
		return 1;
}




template <>
Int GenericString<Char>:: compareIgnoreCase( const Char* string1, const Char* string2 )
{
	return stringCompareIgnoreCase( string1, string2 );
}




template <>
Int GenericString<UTF8Char>:: compareIgnoreCase( const UTF8Char* string1, const UTF8Char* string2 )
{
	return stringCompareIgnoreCase( string1, string2 );
}




template <>
Int GenericString<UTF16Char>:: compareIgnoreCase( const UTF16Char* string1, const UTF16Char* string2 )
{
	return stringCompareIgnoreCase( string1, string2 );
}




template <>
Int GenericString<UTF32Char>:: compareIgnoreCase( const UTF32Char* string1, const UTF32Char* string2 )
{
	return stringCompareIgnoreCase( string1, string2 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Contains Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
OM_INLINE static Bool stringContains( const CharType* string1, const CharType* string2Start )
{
	while ( *string1 )
	{
		const CharType* s1 = string1;
		const CharType* s2 = string2Start;
		
		// Loop until the end of one string is reached.
		while ( *s1 && *s2 )
		{
			// Stop if the characters are different.
			if ( *s1 != *s2 )
				break;
			
			s1++;
			s2++;
		}
		
		// If the end of the second string was reached, the second string is a substring.
		if ( *s2 == '\0' )
			return true;
		
		string1++;
	}
	
	return false;
}




template <>
Bool GenericString<Char>:: contains( const Char* string1, const Char* string2 )
{
	return stringContains( string1, string2 );
}




template <>
Bool GenericString<UTF8Char>:: contains( const UTF8Char* string1, const UTF8Char* string2 )
{
	return stringContains( string1, string2 );
}




template <>
Bool GenericString<UTF16Char>:: contains( const UTF16Char* string1, const UTF16Char* string2 )
{
	return stringContains( string1, string2 );
}




template <>
Bool GenericString<UTF32Char>:: contains( const UTF32Char* string1, const UTF32Char* string2 )
{
	return stringContains( string1, string2 );
}




template < typename CharType >
OM_INLINE static Bool stringContains( const CharType* string1, const CharType* string2Start, Size length )
{
	const CharType* const string2End = string2Start + length;
	
	while ( *string1 )
	{
		const CharType* s1 = string1;
		const CharType* s2 = string2Start;
		
		// Loop until the end of one string is reached.
		while ( *s1 && s2 != string2End )
		{
			// Stop if the characters are different.
			if ( *s1 != *s2 )
				break;
			
			s1++;
			s2++;
		}
		
		// If the end of the second string was reached, the second string is a substring.
		if ( s2 == string2End )
			return true;
		
		string1++;
	}
	
	return false;
}




template <>
Bool GenericString<Char>:: contains( const Char* string1, const Char* string2, Size length )
{
	return stringContains( string1, string2, length );
}




template <>
Bool GenericString<UTF8Char>:: contains( const UTF8Char* string1, const UTF8Char* string2, Size length )
{
	return stringContains( string1, string2, length );
}




template <>
Bool GenericString<UTF16Char>:: contains( const UTF16Char* string1, const UTF16Char* string2, Size length )
{
	return stringContains( string1, string2, length );
}




template <>
Bool GenericString<UTF32Char>:: contains( const UTF32Char* string1, const UTF32Char* string2, Size length )
{
	return stringContains( string1, string2, length );
}




template < typename CharType >
OM_INLINE static Bool stringContainsIgnoreCase( const CharType* string1, const CharType* string2Start )
{
	while ( *string1 )
	{
		const CharType* s1 = string1;
		const CharType* s2 = string2Start;
		
		// Loop until the end of one string is reached.
		while ( *s1 && *s2 )
		{
			// Stop if the characters are different.
			if ( GenericString<CharType>::toLowerCase(*s1) != GenericString<CharType>::toLowerCase(*s2) )
				break;
			
			s1++;
			s2++;
		}
		
		// If the end of the second string was reached, the second string is a substring.
		if ( *s2 == '\0' )
			return true;
		
		string1++;
	}
	
	return false;
}




template <>
Bool GenericString<Char>:: containsIgnoreCase( const Char* string1, const Char* string2 )
{
	return stringContainsIgnoreCase( string1, string2 );
}




template <>
Bool GenericString<UTF8Char>:: containsIgnoreCase( const UTF8Char* string1, const UTF8Char* string2 )
{
	return stringContainsIgnoreCase( string1, string2 );
}




template <>
Bool GenericString<UTF16Char>:: containsIgnoreCase( const UTF16Char* string1, const UTF16Char* string2 )
{
	return stringContainsIgnoreCase( string1, string2 );
}




template <>
Bool GenericString<UTF32Char>:: containsIgnoreCase( const UTF32Char* string1, const UTF32Char* string2 )
{
	return stringContainsIgnoreCase( string1, string2 );
}




template < typename CharType >
OM_INLINE static Bool stringContainsIgnoreCase( const CharType* string1, const CharType* string2Start, Size length )
{
	const CharType* const string2End = string2Start + length;
	
	while ( *string1 )
	{
		const CharType* s1 = string1;
		const CharType* s2 = string2Start;
		
		// Loop until the end of one string is reached.
		while ( *s1 && s2 != string2End )
		{
			// Stop if the characters are different.
			if ( GenericString<CharType>::toLowerCase(*s1) != GenericString<CharType>::toLowerCase(*s2) )
				break;
			
			s1++;
			s2++;
		}
		
		// If the end of the second string was reached, the second string is a substring.
		if ( s2 == string2End )
			return true;
		
		string1++;
	}
	
	return false;
}




template <>
Bool GenericString<Char>:: containsIgnoreCase( const Char* string1, const Char* string2, Size length )
{
	return stringContainsIgnoreCase( string1, string2, length );
}




template <>
Bool GenericString<UTF8Char>:: containsIgnoreCase( const UTF8Char* string1, const UTF8Char* string2, Size length )
{
	return stringContainsIgnoreCase( string1, string2, length );
}




template <>
Bool GenericString<UTF16Char>:: containsIgnoreCase( const UTF16Char* string1, const UTF16Char* string2, Size length )
{
	return stringContainsIgnoreCase( string1, string2, length );
}




template <>
Bool GenericString<UTF32Char>:: containsIgnoreCase( const UTF32Char* string1, const UTF32Char* string2, Size length )
{
	return stringContainsIgnoreCase( string1, string2, length );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Case Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
OM_INLINE static GenericString<CharType> stringToLowerCase( const GenericString<CharType>& string )
{
	GenericStringIterator<CharType> i( string.getPointer() );
	CharType* characters = util::allocate<CharType>( string.getLength() + 1 );
	CharType* currentCharacter = characters;
	
	while ( i )
	{
		*currentCharacter = GenericString<CharType>::toLowerCase(*i);
		currentCharacter++;
		i++;
	}
	
	*currentCharacter = '\0';
	
	GenericString<CharType> result( characters );
	util::deallocate( characters );
	
	return result;
}




template <>
GenericString<Char> GenericString<Char>:: toLowerCase() const
{
	return stringToLowerCase( *this );
}




template <>
GenericString<UTF8Char> GenericString<UTF8Char>:: toLowerCase() const
{
	return stringToLowerCase( *this );
}




template <>
GenericString<UTF16Char> GenericString<UTF16Char>:: toLowerCase() const
{
	return stringToLowerCase( *this );
}




template <>
GenericString<UTF32Char> GenericString<UTF32Char>:: toLowerCase() const
{
	return stringToLowerCase( *this );
}




template < typename CharType >
OM_INLINE static GenericString<CharType> stringToUpperCase( const GenericString<CharType>& string )
{
	GenericStringIterator<CharType> i( string.getPointer() );
	CharType* characters = util::allocate<CharType>( string.getLength() + 1 );
	CharType* currentCharacter = characters;
	
	while ( i )
	{
		*currentCharacter = GenericString<CharType>::toUpperCase(*i);
		currentCharacter++;
		i++;
	}
	
	*currentCharacter = '\0';
	
	GenericString<CharType> result( characters );
	util::deallocate( characters );
	
	return result;
}




template <>
GenericString<Char> GenericString<Char>:: toUpperCase() const
{
	return stringToUpperCase( *this );
}




template <>
GenericString<UTF8Char> GenericString<UTF8Char>:: toUpperCase() const
{
	return stringToUpperCase( *this );
}




template <>
GenericString<UTF16Char> GenericString<UTF16Char>:: toUpperCase() const
{
	return stringToUpperCase( *this );
}




template <>
GenericString<UTF32Char> GenericString<UTF32Char>:: toUpperCase() const
{
	return stringToUpperCase( *this );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Concatenation Functions
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
typename GenericString<CharType>::SharedString* GenericString<CharType>:: concatenateStrings( const CharType* string1, Size length1,
																							const CharType* string2, Size length2 )
{
	// Assume that length1 and length2 include the NULL terminator for the strings.
	Size outputLength = length1 + length2 - 1;
	
	// Allocate an array of characters for the output string.
	typename GenericString<CharType>::SharedString* output = GenericString<CharType>::allocateString( outputLength );
	CharType* destination = output->getCharacters();
	
	const CharType* const string1End = string1 + length1 - 1;
	const CharType* const string2End = string2 + length2 - 1;
	
	while ( string1 != string1End )
	{
		*destination = *string1;
		destination++;
		string1++;
	}
	
	while ( string2 != string2End )
	{
		*destination = *string2;
		destination++;
		string2++;
	}
	
	*destination = '\0';
	
	return output;
}




template <>
GenericString<Char> GenericString<Char>:: operator + ( const GenericString<Char>& other ) const
{
	return GenericString( concatenateStrings( string, shared->length, other.string, other.shared->length ) );
}




template <>
GenericString<UTF8Char> GenericString<UTF8Char>:: operator + ( const GenericString<UTF8Char>& other ) const
{
	return GenericString( concatenateStrings( string, shared->length, other.string, other.shared->length ) );
}




template <>
GenericString<UTF16Char> GenericString<UTF16Char>:: operator + ( const GenericString<UTF16Char>& other ) const
{
	return GenericString( concatenateStrings( string, shared->length, other.string, other.shared->length ) );
}




template <>
GenericString<UTF32Char> GenericString<UTF32Char>:: operator + ( const GenericString<UTF32Char>& other ) const
{
	return GenericString( concatenateStrings( string, shared->length, other.string, other.shared->length ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Static String Length Calculation Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Size GenericString<Char>:: getLengthInCharacters() const
{
	return shared->length - 1;
}




template <>
Size GenericString<UTF8Char>:: getLengthInCharacters() const
{
	return getUnicodeStringLength( string );
}




template <>
Size GenericString<UTF16Char>:: getLengthInCharacters() const
{
	return getUnicodeStringLength( string );
}




template <>
Size GenericString<UTF32Char>:: getLengthInCharacters() const
{
	return shared->length - 1;
}




template <>
Size GenericString<Char>:: getLength( const Char* characters )
{
	return getStringLength( characters );
}




template <>
Size GenericString<UTF8Char>:: getLength( const UTF8Char* characters )
{
	return getStringLength( characters );
}




template <>
Size GenericString<UTF16Char>:: getLength( const UTF16Char* characters )
{
	return getStringLength( characters );
}




template <>
Size GenericString<UTF32Char>:: getLength( const UTF32Char* characters )
{
	return getStringLength( characters );
}




template <>
Size GenericString<Char>:: getLengthInCharacters( const Char* characters )
{
	return getStringLength( characters );
}




template <>
Size GenericString<UTF8Char>:: getLengthInCharacters( const UTF8Char* characters )
{
	return getUnicodeStringLength( characters );
}




template <>
Size GenericString<UTF16Char>:: getLengthInCharacters( const UTF16Char* characters )
{
	return getUnicodeStringLength( characters );
}




template <>
Size GenericString<UTF32Char>:: getLengthInCharacters( const UTF32Char* characters )
{
	return getStringLength( characters );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Local Constant String Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




OM_INLINE static Size getNumberPrefixLengthForBase( Size base )
{
	switch ( base )
	{
		case 2:
		case 16:
			return 2;
		case 8:
			return 1;
		default:
			return 0;
	}
}




template < typename CharType >
OM_INLINE static void getNumberPrefixForBase( Size base, CharType* destination )
{
	switch ( base )
	{
		case 2:
			destination[0] = '0';
			destination[1] = 'b';
			return;
		case 8:
			destination[0] = '0';
			return;
		case 16:
			destination[0] = '0';
			destination[1] = 'x';
	}
}




template < typename CharType, typename T >
OM_INLINE static CharType getCharForInt( T value, Size base )
{
	if ( value < 10 )
		return CharType(value + 48);
	else if ( base == 16 )
		return CharType(value + 55);
	else
		return '?';
}




//##########################################################################################
//##########################################################################################
//############		
//############		Signed Integer to String Conversion Function
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType > template < typename ValueType >
typename GenericString<CharType>::SharedString* GenericString<CharType>:: fromSignedIntegerType( ValueType value, Size base )
{
	Index isNegative = value < ValueType(0);
	
	ValueType remainder = math::abs( value );
	Size logBase = 0;
	ValueType power = 1;
	const ValueType tBase = ValueType(base);
	
	while ( remainder / power >= tBase )
	{
		logBase++;
		power *= tBase;
	}
	
	const Size prefixLength = getNumberPrefixLengthForBase( base );
	Size stringLength = Size(logBase) + 2 + isNegative + prefixLength;
	
	typename GenericString<CharType>::SharedString* string = GenericString<CharType>::allocateString( stringLength );
	CharType* currentCharacter = string->getCharacters();
	
	*currentCharacter = '-';
	currentCharacter += isNegative;
	
	getNumberPrefixForBase( base, currentCharacter );
	currentCharacter += prefixLength;
	
	while ( power != ValueType(0) )
	{
		ValueType quotient = ValueType(remainder / power);
		remainder = remainder - quotient*power;
		power /= tBase;
		
		*currentCharacter = getCharForInt<CharType>( quotient, base );
		currentCharacter++;
	}
	
	*currentCharacter = '\0';
	
	return string;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unsigned Integer to String Conversion Function
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType > template < typename ValueType >
typename GenericString<CharType>::SharedString* GenericString<CharType>:: fromUnsignedIntegerType( ValueType value, Size base )
{
	Size logBase = 0;
	ValueType power = 1;
	const ValueType tBase = ValueType(base);
	
	while ( value / power >= tBase )
	{
		logBase++;
		power *= tBase;
	}
	
	const Size prefixLength = getNumberPrefixLengthForBase( base );
	
	Size stringLength = Size(logBase) + 2 + prefixLength;
	SharedString* string = allocateString( stringLength );
	
	CharType* currentCharacter = string->getCharacters();
	
	getNumberPrefixForBase( base, currentCharacter );
	currentCharacter += prefixLength;
	
	while ( power != ValueType(0) )
	{
		ValueType quotient = ValueType(value / power);
		value = value - quotient*power;
		power /= tBase;
		
		*currentCharacter = getCharForInt<CharType>( quotient, base );
		currentCharacter++;
	}
	
	*currentCharacter = '\0';
	
	return string;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer to String Conversion Forwarding Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericString<Char>::SharedString* GenericString<Char>:: fromIntegerType( Int32 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<Char>::SharedString* GenericString<Char>:: fromIntegerType( Int64 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<Char>::SharedString* GenericString<Char>:: fromIntegerType( UInt32 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




template <>
GenericString<Char>::SharedString* GenericString<Char>:: fromIntegerType( UInt64 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer to String Conversion Forwarding Methods (UTF-8)
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: fromIntegerType( Int32 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: fromIntegerType( Int64 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: fromIntegerType( UInt32 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




template <>
GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: fromIntegerType( UInt64 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer to String Conversion Forwarding Methods (UTF-16)
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: fromIntegerType( Int32 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: fromIntegerType( Int64 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: fromIntegerType( UInt32 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




template <>
GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: fromIntegerType( UInt64 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer to String Conversion Forwarding Methods (UTF-32)
//############		
//##########################################################################################
//##########################################################################################




template <>
GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: fromIntegerType( Int32 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: fromIntegerType( Int64 value, Size base )
{
	return fromSignedIntegerType( value, base );
}




template <>
GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: fromIntegerType( UInt32 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




template <>
GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: fromIntegerType( UInt64 value, Size base )
{
	return fromUnsignedIntegerType( value, base );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Floating Point Number to String Conversion Function
//############		
//##########################################################################################
//##########################################################################################





template < typename T >
static Size getPrecisionForBase( Size base );




template <>
Size getPrecisionForBase<Float>( Size base )
{
	switch ( base )
	{
		case 2:
			return 24;
		case 8:
			return 8;
		case 10:
			return 7;
		case 16:
			return 6;
		default:
			return 0;
	}
}




template <>
Size getPrecisionForBase<Double>( Size base )
{
	switch ( base )
	{
		case 2:
			return 53;
		case 8:
			return 18;
		case 10:
			return 16;
		case 16:
			return 13;
		default:
			return 0;
	}
}





template < typename CharType > template < typename ValueType >
typename GenericString<CharType>::SharedString* GenericString<CharType>::
			fromFloatingPointType( ValueType value, Size numDecimalDigits, Size precision, Size base, Bool allowScientific )
{
	///******************************************************************************
	//	Catch Numeric Limit Values, Handle Separately
	
	if ( math::isInfinity(value) )
	{
		positiveInfinityString.string.referenceCount++;
		return &positiveInfinityString.string;
	}
	else if ( math::isNegativeInfinity(value) )
	{
		negativeInfinityString.string.referenceCount++;
		return &negativeInfinityString.string;
	}
	else if ( math::isNAN(value) )
	{
		nanString.string.referenceCount++;
		return &nanString.string;
	}
	else
	{
		//******************************************************************************
		//	Determine What Floating Point Representation to use (normal or scientific)
		
		Index isNegative = value < ValueType(0);
		ValueType floatBase = ValueType(base);
		ValueType remainder = math::abs( value );
		ValueType exponent = remainder > ValueType(0) ? math::floor(math::log( remainder, floatBase )) : ValueType(0);
		
		Size numDigits = 1;
		Size numExponentDigits = 0;
		ValueType power = math::pow( floatBase, exponent );
		
		if ( allowScientific && exponent >= (ValueType)precision )
		{
			// Display the number in scientific notation, it is larger than base^precision.
			numExponentDigits = (Size)math::log( exponent, floatBase ) + 1;
			numDecimalDigits = math::min( numDecimalDigits, precision - 1 );
		}
		else if ( exponent < ValueType(0) )
		{
			// Display the number in scientific notation if it is smaller than half the number of decimal digits.
			if ( allowScientific && exponent < -ValueType(0.5)*ValueType(numDecimalDigits) )
			{
				numExponentDigits = (Size)math::log( -exponent, floatBase ) + 1;
				numDecimalDigits = math::min( numDecimalDigits, precision - 1 );
			}
			else
				power = 1;
		}
		else
		{
			numDigits = (Size)exponent + 1;
			numDecimalDigits = math::min( numDecimalDigits, precision - numDigits );
		}
		
		//******************************************************************************
		//	Determine the length of the string and allocate the character array.
		
		Size prefixLength = getNumberPrefixLengthForBase( base );
		
		Size stringLength = isNegative + prefixLength + numDigits + // minus sign, base prefix, significant digits.
							(numDecimalDigits != 0) + numDecimalDigits + 1; // decimal point and decimal digits, null terminator.
		
		if ( numExponentDigits > 0 )
			stringLength += (exponent < ValueType(0)) + numExponentDigits + 1;
		
		SharedString* string = allocateString( stringLength );
		
		CharType* currentCharacter = string->getCharacters();
		const CharType* const stringStart = currentCharacter;
		const CharType* const stringEnd = currentCharacter + stringLength - 1;
		const CharType* decimalPlacePointer = NULL;
		const CharType* exponentPointer = NULL;
		
		if ( numDecimalDigits != 0 )
		{
			decimalPlacePointer = stringStart + isNegative + prefixLength + numDigits;
			
			if ( numExponentDigits != 0 )
				exponentPointer = decimalPlacePointer + 1 + numDecimalDigits;
		}
		else if ( numExponentDigits != 0 )
			exponentPointer = stringStart + isNegative + prefixLength + numDigits;
		
		//******************************************************************************
		//	Add the number's prefix (sign and base prefix)
		
		//	If the number is negative, add a minus sign.
		*currentCharacter = '-';
		currentCharacter += isNegative;
		
		//	If the number has a prefix to indicate its base, add it.
		getNumberPrefixForBase( base, currentCharacter );
		currentCharacter += prefixLength;
		
		//******************************************************************************
		//	Generate the digits of the number
		
		for ( ; currentCharacter != stringEnd; currentCharacter++ )
		{
			if ( currentCharacter == decimalPlacePointer )
			{
				*currentCharacter = '.';
				continue;
			}
			else if ( currentCharacter == exponentPointer )
			{
				if ( base < 15 )
					*currentCharacter = 'e';
				else
					*currentCharacter = 'p';
				
				if ( exponent < ValueType(0) )
				{
					currentCharacter++;
					*currentCharacter = '-';
				}
				
				remainder = math::abs(exponent);
				power = math::pow( floatBase, ValueType(numExponentDigits - 1) );
				continue;
			}
			else
			{
				int quotient = int(remainder / power);
				remainder = remainder - ValueType(quotient)*power;
				power /= floatBase;
				
				*currentCharacter = getCharForInt<CharType>( quotient, base );
			}
		}
		
		//	Give the string a null terminator.
		*currentCharacter = '\0';
		
		return string;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer to String Conversion Forwarding Methods
//############		
//##########################################################################################
//##########################################################################################





template <>
GenericString<Char>::SharedString* GenericString<Char>:: fromFloatType( Float value, Size numDecimalPlaces, Size base,
																	Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Float>(base), base,
								allowScientific );
}




template <>
GenericString<Char>::SharedString* GenericString<Char>:: fromFloatType( Double value, Size numDecimalPlaces, Size base,
																	Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Double>(base), base,
								allowScientific );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Float to String Conversion Forwarding Methods (UTF-8)
//############		
//##########################################################################################
//##########################################################################################





template <>
GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: fromFloatType( Float value, Size numDecimalPlaces, Size base,
																			Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Float>(base), base,
								allowScientific );
}




template <>
GenericString<UTF8Char>::SharedString* GenericString<UTF8Char>:: fromFloatType( Double value, Size numDecimalPlaces, Size base,
																			Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Double>(base), base,
								allowScientific );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Float to String Conversion Forwarding Methods (UTF-16)
//############		
//##########################################################################################
//##########################################################################################





template <>
GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: fromFloatType( Float value, Size numDecimalPlaces, Size base,
																				Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Float>(base), base,
								allowScientific );
}




template <>
GenericString<UTF16Char>::SharedString* GenericString<UTF16Char>:: fromFloatType( Double value, Size numDecimalPlaces, Size base,
																				Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Double>(base), base,
								allowScientific );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Float to String Conversion Forwarding Methods (UTF-32)
//############		
//##########################################################################################
//##########################################################################################





template <>
GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: fromFloatType( Float value, Size numDecimalPlaces, Size base,
																				Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Float>(base), base,
								allowScientific );
}




template <>
GenericString<UTF32Char>::SharedString* GenericString<UTF32Char>:: fromFloatType( Double value, Size numDecimalPlaces, Size base,
																				Bool allowScientific )
{
	return fromFloatingPointType( value, numDecimalPlaces, getPrecisionForBase<Double>(base), base,
								allowScientific );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String to Number Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
OM_INLINE Bool isWhitespace( const CharType character )
{
	return character == ' ' || character == '\t' || character == '\n' || character == '\r';
}




template < typename CharType1, typename CharType2 >
OM_NO_INLINE static Bool stringEqualsIgnoreCase( const CharType1* string1, const CharType2* string2, Size number )
{
	Index currentIndex = 0;
	
	while ( *string1 && *string2 )
	{
		if ( GenericString<CharType1>::toLowerCase(*string1) != 
			GenericString<CharType2>::toLowerCase(*string2) )
			return false;
		
		string1++;
		string2++;
		currentIndex++;
		
		// The strings have been equal up to the number of characters that we are supposed to check.
		if ( currentIndex == number )
			return true;
	}
	
	return *string1 == '\0' && *string2 == '\0';
}




template < typename CharType >
template < typename ValueType >
Bool GenericString<CharType>:: parseSimpleNumber( const CharType* numberStart, const CharType* numberEnd,
												Size base, ValueType& number )
{
	ValueType result = ValueType(0);
	ValueType tBase = ValueType(base);
	
	//***************************************************************
	// Parse the number backwards.
	
	const CharType* currentCharacter = numberEnd - 1;
	const CharType* reverseEnd = numberStart - 1;
	ValueType currentPower = ValueType(1);
	
	for ( ; currentCharacter != reverseEnd; currentCharacter-- )
	{
		if ( *currentCharacter == '.' )
		{
			// This character is the decimal point, divide the current
			// result by the current power so that the current result
			// is a number from 0 to 1.
			result /= currentPower;
			
			// Set the current power to be one, decrement the character,
			// and continue to the next character.
			currentPower = ValueType(1);
			
			continue;
		}
		else if ( *currentCharacter < '0' || 
					(base == 2 && *currentCharacter > '1') ||
					(base == 8 && *currentCharacter > '7') ||
					(base == 10 && *currentCharacter > '9') ||
					(base == 16 && !(*currentCharacter < '9' || 
										(*currentCharacter >= 'a' && *currentCharacter <= 'f') ||
										(*currentCharacter >= 'A' && *currentCharacter <= 'F')) ) )
		{
			// There was an invalid character in the number, return that there was a parse error.
			return false;
		}
		
		if ( base == 16 )
		{
			CharType c = *currentCharacter;
			
			// Convert the hexadecimal character to an integer
			c = c <= '9' ? c - 48 : (c >= 'a' && c <= 'f' ? c - 87 : c >= 'A' && c <= 'F' ? c - 55 : 0);
			
			result += currentPower*ValueType(c);
		}
		else
			result += currentPower*ValueType( *currentCharacter - 48 );
		
		currentPower *= tBase;
	}
	
	number = result;
	
	return true;
}



template < typename CharType >
template < typename ValueType >
Bool GenericString<CharType>:: convertStringToNumber( const CharType* stringStart, const CharType* stringEnd, ValueType& result )
{
	//***************************************************************
	// Skip any leading and trailing whitespace in the string.
	
	while ( isWhitespace( *stringStart ) && stringStart != stringEnd )
		stringStart++;
	
	const CharType* currentCharacter = stringStart;
	
	while ( !isWhitespace( *currentCharacter ) && *currentCharacter != '\0' && currentCharacter != stringEnd )
		currentCharacter++;
	
	stringEnd = currentCharacter;
	currentCharacter = stringStart;
	
	//***************************************************************
	// Check to see if the number is negative.
	
	Bool isNegative = false;
	
	if ( *currentCharacter == '-' )
	{
		isNegative = true;
		currentCharacter++;
	}
	else if ( *currentCharacter == '+' )
		currentCharacter++;
	
	//***************************************************************
	// Check to see if the number is infinite or NaN
	
	if ( stringEqualsIgnoreCase( currentCharacter, "infinity", 8 ) )
	{
		result = isNegative ? math::negativeInfinity<ValueType>() : math::infinity<ValueType>();
		return true;
	}
	else if ( stringEqualsIgnoreCase( currentCharacter, "nan", 3 ) )
	{
		result = math::nan<ValueType>();
		return true;
	}
	
	//***************************************************************
	// Parse the base prefix for the string if it has one.
	
	Size base;
	
	if ( *currentCharacter == '0' )
	{
		currentCharacter++;
		
		// do we have a single zero here and not an octal or hexadecimal integer?
		if ( currentCharacter == stringEnd )
		{
			result = ValueType(0);
			return true;
		}
		
		if ( *currentCharacter == 'b' || *currentCharacter == 'B' )
		{
			base = 2;
			currentCharacter++;
		}
		else if ( *currentCharacter == 'x' || *currentCharacter == 'X' )
		{
			base = 16;
			currentCharacter++;
		}
		else if ( *currentCharacter == '.' )
		{
			base = 10;
			currentCharacter--;
		}
		else
			base = 8;
	}
	else
		base = 10;
	
	// Make sure that the end of the string was not reached.
	if ( currentCharacter == stringEnd )
		return false;
	
	//***************************************************************
	// Find the exponent of the number if there is one.
	
	// Start the search one digit after the current character (so that a mantissa is required).
	const CharType* exponentCharacter = currentCharacter + 1;
	
	while ( exponentCharacter != stringEnd )
	{
		if ( base > 14 )
		{
			if ( *exponentCharacter == 'p' || *exponentCharacter == 'P' )
				break;
		}
		else if ( *exponentCharacter == 'e' || *exponentCharacter == 'E' )
			break;
		
		exponentCharacter++;
	}
	
	const CharType* exponentStart = NULL;
	Bool exponentIsNegative = false;
	
	if ( exponentCharacter != stringEnd )
	{
		exponentStart = exponentCharacter + 1;
		
		// Check to see if the exponent is negative.
		if ( *exponentStart == '-' )
		{
			exponentIsNegative = true;
			exponentStart++;
		}
		else if ( *exponentStart == '+' )
			exponentStart++;
	}
	
	//***************************************************************
	// Parse the mantissa and exponent of the number.
	
	ValueType mantissa;
	
	if ( !parseSimpleNumber( currentCharacter, exponentCharacter, base, mantissa ) )
		return false;
	
	if ( isNegative )
		mantissa = -mantissa;
	
	if ( exponentStart != NULL )
	{
		ValueType exponent;
		
		if ( !parseSimpleNumber( exponentStart, stringEnd, base, exponent ) )
			return false;
		
		if ( exponentIsNegative )
			exponent = -exponent;
		
		result = mantissa*math::pow( ValueType(base), exponent );
	}
	else
		result = mantissa;
	
	return true;
}




template < typename CharType >
Bool GenericString<CharType>:: convertStringToBoolean( const CharType* stringStart, const CharType* stringEnd,
													Bool& result )
{
	// Check to see if the string is equal to "true" or "false"
	if ( stringEqualsIgnoreCase( stringStart, "true", 4 ) )
	{
		result = true;
		return true;
	}
	else if ( stringEqualsIgnoreCase( stringStart, "false", 5 ) )
	{
		result = false;
		return true;
	}
	else
	{
		Int value;
		
		if ( convertStringToNumber( stringStart, stringEnd, value ) )
		{
			result = value != Int(0);
			return true;
		}
		
		return false;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		ASCII String to Number Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool GenericString<Char>:: toBool( Bool& value ) const
{
	return convertStringToBoolean( string, string + this->getLength(), value );
}




template <>
Bool GenericString<Char>:: toFloat( Float& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




template <>
Bool GenericString<Char>:: toDouble( Double& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-8 String to Number Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool GenericString<UTF8Char>:: toBool( Bool& value ) const
{
	return convertStringToBoolean( string, string + this->getLength(), value );
}




template <>
Bool GenericString<UTF8Char>:: toFloat( Float& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




template <>
Bool GenericString<UTF8Char>:: toDouble( Double& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-16 String to Number Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool GenericString<UTF16Char>:: toBool( Bool& value ) const
{
	return convertStringToBoolean( string, string + this->getLength(), value );
}




template <>
Bool GenericString<UTF16Char>:: toFloat( Float& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




template <>
Bool GenericString<UTF16Char>:: toDouble( Double& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-32 String to Number Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool GenericString<UTF32Char>:: toBool( Bool& value ) const
{
	return convertStringToBoolean( string, string + this->getLength(), value );
}




template <>
Bool GenericString<UTF32Char>:: toFloat( Float& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




template <>
Bool GenericString<UTF32Char>:: toDouble( Double& value ) const
{
	return convertStringToNumber( string, string + this->getLength(), value );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String to Number Conversion Test Forwarding Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool GenericString<Char>:: isANumber( const Char* string, Size length )
{
	Float value;
	return convertStringToNumber( string, string + length - 1, value );
}




template <>
Bool GenericString<UTF8Char>:: isANumber( const UTF8Char* string, Size length )
{
	Float value;
	return convertStringToNumber( string, string + length - 1, value );
}




template <>
Bool GenericString<UTF16Char>:: isANumber( const UTF16Char* string, Size length )
{
	Float value;
	return convertStringToNumber( string, string + length - 1, value );
}




template <>
Bool GenericString<UTF32Char>:: isANumber( const UTF32Char* string, Size length )
{
	Float value;
	return convertStringToNumber( string, string + length - 1, value );
}


 

//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
