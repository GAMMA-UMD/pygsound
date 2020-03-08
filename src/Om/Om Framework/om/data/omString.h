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

#ifndef INCLUDE_OM_STRING_H
#define INCLUDE_OM_STRING_H


#include "omDataConfig.h"


#include "omHashCode.h"
#include "../threads/omAtomics.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


template < typename CharType >
class GenericStringBuffer;




//********************************************************************************
/// A string class supporting unicode and ASCII character sets.
/**
  * The GenericString class is a template class that allows the user to use any of
  * four different specializations: Char for ASCII strings, UTF8Char for UTF-8 strings,
  * UTF16Char for UTF-16 strings, and UTF32Char for UTF-32 strings. In addition to this
  * functionality, it provides comprehensive number-to-string and string-to-number conversion
  * using constructors and cast operators. All strings are immutable after creation.
  * Use the GenericStringBuffer class to efficiently compose strings.
  *
  * Strings are reference-counted, so the overhead of copying a string object
  * is very small.
  */
template < typename CharType >
class GenericString
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty string.
			OM_INLINE GenericString()
				:	shared( &nullString.string )
			{
				shared->referenceCount++;
				string = shared->getCharacters();
			}
			
			
			/// Create a string consisting of a single character.
			OM_INLINE GenericString( Char character )
				:	shared( allocateString( 2 ) )
			{
				string = shared->getCharacters();
				string[0] = character;
				string[1] = '\0';
			}
			
			
			/// Create a string from a NULL-terminated character array.
			GenericString( const Char* array );
			
			
			/// Create a string from a NULL-terminated character array.
			GenericString( const UTF8Char* array );
			
			
			/// Create a string from a NULL-terminated character array.
			GenericString( const UTF16Char* array );
			
			
			/// Create a string from a NULL-terminated character array.
			GenericString( const UTF32Char* array );
			
			
			/// Create a string from a character array with the specified length.
			GenericString( const Char* array, Size length );
			
			
			/// Create a string from a character array with the specified length.
			GenericString( const UTF8Char* array, Size length );
			
			
			/// Create a string from a character array with the specified length.
			GenericString( const UTF16Char* array, Size length );
			
			
			/// Create a string from a character array with the specified length.
			GenericString( const UTF32Char* array, Size length );
			
			
			/// Create a copy of the specified other string.
			OM_INLINE GenericString( const GenericString& other )
				:	shared( other.shared ),
					string( other.string )
			{
				shared->referenceCount++;
			}
			
			
			/// Create a copy of the specified other string with a different character type.
			template < typename OtherCharType >
			explicit GenericString( const GenericString<OtherCharType>& other );
			
			
		//********************************************************************************
		//******	Number-To-String Conversion Constructors
			
			
			/// Create a new boolean literal string for the specified value.
			OM_INLINE explicit GenericString( Bool boolean )
				:	shared( boolean ? (SharedString*)&trueString : (SharedString*)&falseString )
			{
				shared->referenceCount++;
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( Short number, Size base = 10 )
				:	shared( fromIntegerType( (Int32)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( UShort number, Size base = 10 )
				:	shared( fromIntegerType( (UInt32)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( Int number, Size base = 10 )
				:	shared( fromIntegerType( (Int32)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( UInt number, Size base = 10 )
				:	shared( fromIntegerType( (UInt32)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( Long number, Size base = 10 )
				:	shared( fromIntegerType( (Int32)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( ULong number, Size base = 10 )
				:	shared( fromIntegerType( (UInt32)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( LongLong number, Size base = 10 )
				:	shared( fromIntegerType( (Int64)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified integer number in the given base system.
			OM_INLINE explicit GenericString( ULongLong number, Size base = 10 )
				:	shared( fromIntegerType( (UInt64)number, base ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified floating-point number with default formating.
			/**
			  * The number is displayed with the maximum precision for its type in base 10,
			  * and scientific notation is automatically used when necessary.
			  */
			OM_INLINE explicit GenericString( Float number )
				:	shared( fromFloatType( number, 6, 10, true ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified floating-point number in the given base system.
			/**
			  * This method allows the user to determine the number of decimal places to display,
			  * the base system, and whether or not the allow scientific notation.
			  */
			OM_INLINE explicit GenericString( Float number, Size numDecimalPlaces, Size base = 10, Bool allowScientific = true )
				:	shared( fromFloatType( number, numDecimalPlaces, base, allowScientific ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified double floating-point number with default formating.
			/**
			  * The number is displayed with the maximum precision for its type in base 10,
			  * and scientific notation is automatically used when necessary.
			  */
			OM_INLINE explicit GenericString( Double number )
				:	shared( fromFloatType( number, 15, 10, true ) )
			{
				string = shared->getCharacters();
			}
			
			
			/// Create a string which represents the specified double floating-point number in the given base system.
			/**
			  * This method allows the user to determine the number of decimal places to display,
			  * the base system, and whether or not the allow scientific notation.
			  */
			OM_INLINE explicit GenericString( Double number, Size numDecimalPlaces, Size base = 10, Bool allowScientific = true )
				:	shared( fromFloatType( number, numDecimalPlaces, base, allowScientific ) )
			{
				string = shared->getCharacters();
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this string, releasing its internal character buffer if the reference count is 0.
			OM_INLINE ~GenericString()
			{
				deallocateString( shared );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another string to this string.
			/**
			  * The reference count of the other string is increased by one
			  * and it replaces the current string of this object, deallocating
			  * the previous string if its reference count reaches 0.
			  */
			OM_INLINE GenericString& operator = ( const GenericString& other )
			{
				if ( shared != other.shared )
				{
					deallocateString( shared );
					
					shared = other.shared;
					shared->referenceCount++;
					string = shared->getCharacters();
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	String Equality Comparison Methods
			
			
			/// Return whether or not this string is exactly equal to another.
			OM_INLINE Bool equals( const GenericString& other ) const
			{
				return shared == other.shared ||
						(shared->length == other.shared->length && GenericString::equals( string, other.string ));
			}
			
			
			/// Return whether or not this string is equal to another if letter case is ignored.
			OM_INLINE Bool equalsIgnoreCase( const GenericString& other ) const
			{
				return shared == other.shared ||
						(shared->length == other.shared->length && GenericString::equalsIgnoreCase( string, other.string ));
			}
			
			
			/// Return whether or not this string is exactly equal to a NULL-terminated character string.
			OM_INLINE Bool equals( const CharType* characters ) const
			{
				return GenericString::equals( string, characters );
			}
			
			
			/// Return whether or not this string is exactly equal to a character string of the given length.
			OM_INLINE Bool equals( const CharType* characters, Size numCharacters ) const
			{
				return (shared->length - 1) == numCharacters &&
						GenericString::equals( string, characters, numCharacters );
			}
			
			
			/// Return whether or not this string is equal to a NULL-terminated character string if letter case is ignored.
			OM_INLINE Bool equalsIgnoreCase( const CharType* characters ) const
			{
				return GenericString::equalsIgnoreCase( string, characters );
			}
			
			
			/// Return whether or not this string is equal to a character string if letter case is ignored.
			OM_INLINE Bool equalsIgnoreCase( const CharType* characters, Size numCharacters ) const
			{
				return (shared->length - 1) == numCharacters &&
						GenericString::equalsIgnoreCase( string, characters, numCharacters );
			}
			
			
		//********************************************************************************
		//******	String Equality Comparison Operators
			
			
			/// Return whether or not this string is exactly equal to another.
			OM_INLINE Bool operator == ( const GenericString& other ) const
			{
				return shared == other.shared ||
						(shared->length == other.shared->length && GenericString::equals( string, other.string ));
			}
			
			
			/// Return whether or not this string is not equal to another.
			OM_INLINE Bool operator != ( const GenericString& other ) const
			{
				return shared != other.shared &&
						(shared->length != other.shared->length || !GenericString::equals( string, other.string ));
			}
			
			
			/// Return whether or not this string is exactly equal to a NULL-terminated character string.
			OM_INLINE Bool operator == ( const CharType* other ) const
			{
				return GenericString::equals( string, string );
			}
			
			
			/// Return whether or not this string is not equal to a NULL-terminated character string.
			OM_INLINE Bool operator != ( const CharType* other ) const
			{
				return !GenericString::equals( string, string );
			}
			
			
		//********************************************************************************
		//******	String Sorting Comparison Methods
			
			
			/// Return an integer indicating the lexical order of this string when compared to another.
			/**
			  * If this string should come before the other, -1 is returned.
			  * If this string is equal to the other, 0 is returned.
			  * If this string should come after the other, 1 is returned.
			  */
			OM_INLINE Int compareTo( const GenericString& other ) const
			{
				return GenericString::compare( string, other.string );
			}
			
			
			/// Return an integer indicating the lexical order of this string when compared to another, ignoring letter case.
			/**
			  * If this string should come before the other, -1 is returned.
			  * If this string is equal to the other, 0 is returned.
			  * If this string should come after the other, 1 is returned.
			  */
			OM_INLINE Int compareToIgnoreCase( const GenericString& other ) const
			{
				return GenericString::compareIgnoreCase( string, other.string );
			}
			
			
			/// Return an integer indicating the lexical order of this string when compared to a C-string.
			/**
			  * If this string should come before the other, -1 is returned.
			  * If this string is equal to the other, 0 is returned.
			  * If this string should come after the other, 1 is returned.
			  */
			OM_INLINE Int compareTo( const CharType* characters ) const
			{
				return GenericString::compare( string, characters );
			}
			
			
			/// Return an integer indicating the lexical order of this string when compared to a C-string, ignoring letter case.
			/**
			  * If this string should come before the other, -1 is returned.
			  * If this string is equal to the other, 0 is returned.
			  * If this string should come after the other, 1 is returned.
			  */
			OM_INLINE Int compareToIgnoreCase( const CharType* characters ) const
			{
				return GenericString::compareIgnoreCase( string, characters );
			}
			
			
			/// Return an integer indicating the lexical order of a C-string when compared to a C-string.
			static Int compare( const CharType* string1, const CharType* string2 );
			
			
			/// Return an integer indicating the lexical order of a C-string when compared to a C-string, ignoring letter case.
			static Int compareIgnoreCase( const CharType* string1, const CharType* string2 );
			
			
		//********************************************************************************
		//******	String Sorting Operators
			
			
			/// Return whether or not this string comes before another one in the lexical order.
			OM_INLINE Bool operator < ( const GenericString& other ) const
			{
				return GenericString::compare( string, other.string ) == -1;
			}
			
			
			/// Return whether or not this string comes after another one in the lexical order.
			OM_INLINE Bool operator > ( const GenericString& other ) const
			{
				return GenericString::compare( string, other.string ) == 1;
			}
			
			
			/// Return whether or not this string comes before or is the same as another one in the lexical order.
			OM_INLINE Bool operator <= ( const GenericString& other ) const
			{
				return GenericString::compare( string, other.string ) <= 0;
			}
			
			
			/// Return whether or not this string comes after or is the same as another one in the lexical order.
			OM_INLINE Bool operator >= ( const GenericString& other ) const
			{
				return GenericString::compare( string, other.string ) >= 0;
			}
			
			
			/// Return whether or not this string comes before a C-string in the lexical order.
			OM_INLINE Bool operator < ( const CharType* characters ) const
			{
				return GenericString::compare( string, characters ) == -1;
			}
			
			
			/// Return whether or not this string comes after a C-string in the lexical order.
			OM_INLINE Bool operator > ( const CharType* characters ) const
			{
				return GenericString::compare( string, characters ) == 1;
			}
			
			
			/// Return whether or not this string comes before or is the same as a C-string in the lexical order.
			OM_INLINE Bool operator <= ( const CharType* characters ) const
			{
				return GenericString::compare( string, characters ) < 1;
			}
			
			
			/// Return whether or not this string comes after or is the same as a C-string in the lexical order.
			OM_INLINE Bool operator >= ( const CharType* characters ) const
			{
				return GenericString::compare( string, characters ) > -1;
			}
			
			
			/// Return whether or not this string comes before another one in the lexical order, ignoring letter case.
			OM_INLINE Bool operator << ( const GenericString& other ) const
			{
				return GenericString::compareIgnoreCase( string, other.string ) == -1;
			}
			
			
			/// Return whether or not this string comes after another one in the lexical order, ignoring letter case.
			OM_INLINE Bool operator >> ( const GenericString& other ) const
			{
				return GenericString::compareIgnoreCase( string, other.string ) == 1;
			}
			
			
			/// Return whether or not this string comes before or is the same as another one in the lexical order, ignoring letter case.
			OM_INLINE Bool operator <<= ( const GenericString& other ) const
			{
				return GenericString::compareIgnoreCase( string, other.string ) < 1;
			}
			
			
			/// Return whether or not this string comes after or is the same as another one in the lexical order, ignoring letter case.
			OM_INLINE Bool operator >>= ( const GenericString& other ) const
			{
				return GenericString::compareIgnoreCase( string, other.string ) > -1;
			}
			
			
			/// Return whether or not this string comes before a C-string in the lexical order, ignoring letter case.
			OM_INLINE Bool operator << ( const CharType* characters ) const
			{
				return GenericString::compareIgnoreCase( string, characters ) == -1;
			}
			
			
			/// Return whether or not this string comes before a C-string in the lexical order, ignoring letter case.
			OM_INLINE Bool operator >> ( const CharType* characters ) const
			{
				return GenericString::compareIgnoreCase( string, characters ) == 1;
			}
			
			
			/// Return whether or not this string comes before or is the same as a C-string in the lexical order, ignoring letter case.
			OM_INLINE Bool operator <<= ( const CharType* characters ) const
			{
				return GenericString::compareIgnoreCase( string, characters ) < 1;
			}
			
			
			/// Return whether or not this string comes after or is the same as a C-string in the lexical order, ignoring letter case.
			OM_INLINE Bool operator >>= ( const CharType* characters ) const
			{
				return GenericString::compareIgnoreCase( string, characters ) > -1;
			}
			
			
		//********************************************************************************
		//******	Substring Methods
			
			
			/// Return whether or not this string contains another string as a substring.
			OM_INLINE Bool contains( const GenericString& other ) const
			{
				return shared == other.shared ||
						(shared->length >= other.shared->length && GenericString::contains( string, other.string ));
			}
			
			
			/// Return whether or not this string contains another string as a substring if letter case is ignored.
			OM_INLINE Bool containsIgnoreCase( const GenericString& other ) const
			{
				return shared == other.shared ||
						(shared->length >= other.shared->length && GenericString::containsIgnoreCase( string, other.string ));
			}
			
			
			/// Return whether or not this string contains another NULL-terminated string as a substring.
			OM_INLINE Bool contains( const CharType* characters ) const
			{
				return GenericString::contains( string, characters );
			}
			
			
			/// Return whether or not this string contains another NULL-terminated string as a substring if letter case is ignored.
			OM_INLINE Bool containsIgnoreCase( const CharType* characters ) const
			{
				return GenericString::containsIgnoreCase( string, characters );
			}
			
			
			/// Return whether or not this string contains another string as a substring.
			OM_INLINE Bool contains( const CharType* characters, Size numCharacters ) const
			{
				return (shared->length - 1) >= numCharacters &&
						GenericString::contains( string, characters, numCharacters );
			}
			
			
			/// Return whether or not this string contains another string as a substring if letter case is ignored.
			OM_INLINE Bool containsIgnoreCase( const CharType* characters, Size numCharacters ) const
			{
				return (shared->length - 1) >= numCharacters &&
						GenericString::containsIgnoreCase( string, characters, numCharacters );
			}
			
			
			/// Return whether or not a NULL-terminated string contains another NULL-terminated string as a substring.
			static Bool contains( const CharType* string1, const CharType* string2 );
			
			
			/// Return whether or not a NULL-terminated string contains another string as a substring.
			static Bool contains( const CharType* string1, const CharType* string2, Size length );
			
			
			/// Return whether or not a NULL-terminated string contains another NULL-terminated string as a substring if letter case is ignored.
			static Bool containsIgnoreCase( const CharType* string1, const CharType* string2 );
			
			
			/// Return whether or not a NULL-terminated string contains another string as a substring if letter case is ignored.
			static Bool containsIgnoreCase( const CharType* string1, const CharType* string2, Size length );
			
			
		//********************************************************************************
		//******	Static String Comparison Methods
			
			
			static Bool equals( const CharType* string1, const CharType* string2 );
			static Bool equals( const CharType* string1, const CharType* string2, Size length );
			static Bool equalsIgnoreCase( const CharType* string1, const CharType* string2 );
			static Bool equalsIgnoreCase( const CharType* string1, const CharType* string2, Size length );
			
			
		//********************************************************************************
		//******	Concatenation Operators
			
			
			GenericString operator + ( const GenericString& other ) const;
			
			
			OM_INLINE GenericString operator + ( const Char* characters ) const
			{
				return (*this) + GenericString(characters);
			}
			
			
			OM_INLINE GenericString operator + ( Char character ) const
			{
				return (*this) + GenericString(character);
			}
			
			
			OM_INLINE GenericString operator + ( Short value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
			OM_INLINE GenericString operator + ( UShort value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
			OM_INLINE GenericString operator + ( Int value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
			OM_INLINE GenericString operator + ( UInt value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
			OM_INLINE GenericString operator + ( Long value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
			OM_INLINE GenericString operator + ( ULong value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
			OM_INLINE GenericString operator + ( LongLong value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
			OM_INLINE GenericString operator + ( ULongLong value ) const
			{
				return (*this) + GenericString(value);
			}
			
			
		//********************************************************************************
		//******	String to Number Conversion Operators
			
			
			/// Cast this string to an integer, returning math::nan<Int32>() if the conversion fails.
			OM_INLINE operator Int () const
			{
				return (Long)*this;
			}
			
			
			/// Cast this string to an integer, returning math::nan<UInt32>() if the conversion fails.
			OM_INLINE operator UInt () const
			{
				return (ULong)*this;
			}
			
			
			/// Cast this string to an integer, returning math::nan<Int32>() if the conversion fails.
			OM_INLINE operator Long () const
			{
				Int32 value;
				if ( this->toInt32( value ) )
					return value;
				else
					return math::nan<Int32>();
			}
			
			
			/// Cast this string to an integer, returning math::nan<UInt32>() if the conversion fails.
			OM_INLINE operator ULong () const
			{
				UInt32 value;
				if ( this->toUInt32( value ) )
					return value;
				else
					return math::nan<UInt32>();
			}
			
			
			/// Cast this string to an integer, returning math::nan<Int64>() if the conversion fails.
			OM_INLINE operator LongLong () const
			{
				Int64 value;
				if ( this->toInt64( value ) )
					return value;
				else
					return math::nan<Int64>();
			}
			
			
			/// Cast this string to an integer, returning math::nan<UInt64>() if the conversion fails.
			OM_INLINE operator ULongLong () const
			{
				UInt64 value;
				if ( this->toUInt64( value ) )
					return value;
				else
					return math::nan<UInt64>();
			}
			
			
			/// Cast this string to a floating-point number, returning math::nan<Float>() if the conversion fails.
			OM_INLINE operator Float () const
			{
				Float value;
				if ( this->toFloat( value ) )
					return value;
				else
					return math::nan<Float>();
			}
			
			
			/// Cast this string to a double floating-point number, returning math::nan<Double>() if the conversion fails.
			OM_INLINE operator Double () const
			{
				Double value;
				if ( this->toDouble( value ) )
					return value;
				else
					return math::nan<Double>();
			}
			
			
		//********************************************************************************
		//******	String to Number Conversion Methods
			
			
			/// Convert this string to a boolean value, returning whether or not the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			Bool toBool( Bool& value ) const;
			
			
			/// Convert this string to a float value, returning whether or not the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			Bool toFloat( Float& value ) const;
			
			
			/// Convert this string to a double value, returning whether or not the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			Bool toDouble( Double& value ) const;
			
			
			/// Convert this string to an integer value, returning whether or not the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			OM_INLINE Bool toInt( Int& value ) const
			{
				Double d;
				
				if ( this->toDouble( d ) )
				{
					value = safeTypeConversion<Int, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert this string to an unsigned integer value, returning TRUE if the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			Bool toUInt( UInt32& value ) const
			{
				Double d;
				
				if ( this->toDouble( d ) )
				{
					value = safeTypeConversion<UInt, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert this string to a 32-bit integer value, returning whether or not the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			OM_INLINE Bool toInt32( Int32& value ) const
			{
				Double d;
				
				if ( this->toDouble( d ) )
				{
					value = safeTypeConversion<Int32, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert this string to a 32-bit unsigned integer value, returning TRUE if the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			OM_INLINE Bool toUInt32( UInt32& value ) const
			{
				Double d;
				
				if ( this->toDouble( d ) )
				{
					value = safeTypeConversion<UInt32, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert this string to a 64-bit integer value, returning whether or not the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			OM_INLINE Bool toInt64( Int64& value ) const
			{
				Double d;
				
				if ( this->toDouble( d ) )
				{
					value = safeTypeConversion<Int64, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert this string to a 64-bit unsigned integer value, returning TRUE if the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  */
			OM_INLINE Bool toUInt64( UInt64& value ) const
			{
				Double d;
				
				if ( this->toDouble( d ) )
				{
					value = safeTypeConversion<UInt64, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert this string to a templated number value, returning TRUE if the conversion was successful.
			/**
			  * If the conversion is successful, the converted value is placed in the output parameter.
			  * This method will not compile for non-primitive types.
			  */
			template < typename T >
			OM_INLINE Bool toNumber( T& value ) const
			{
				Double d;
				
				if ( this->toDouble( d ) )
				{
					value = safeTypeConversion<T, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Static String to Number Conversion Methods
			
			
			/// Convert a string specified by starting and ending pointers to a boolean value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToBool( const CharType* stringStart, const CharType* stringEnd,
										Bool& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				return convertStringToBoolean( stringStart, stringEnd, result );
			}
			
			
			/// Convert a string specified by starting and ending pointers to an integer value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToInt( const CharType* stringStart, const CharType* stringEnd,
										Int& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				Double d;
				
				if ( convertStringToNumber( stringStart, stringEnd, d ) )
				{
					result = safeTypeConversion<Int, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert a string specified by starting and ending pointers to an integer value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToInt32( const CharType* stringStart, const CharType* stringEnd,
										Int32& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				Double d;
				
				if ( convertStringToNumber( stringStart, stringEnd, d ) )
				{
					result = safeTypeConversion<Int32, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert a string specified by starting and ending pointers to an integer value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToUInt32( const CharType* stringStart, const CharType* stringEnd,
										UInt32& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				Double d;
				
				if ( convertStringToNumber( stringStart, stringEnd, d ) )
				{
					result = safeTypeConversion<UInt32, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert a string specified by starting and ending pointers to an integer value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToInt64( const CharType* stringStart, const CharType* stringEnd,
										Int64& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				Double d;
				
				if ( convertStringToNumber( stringStart, stringEnd, d ) )
				{
					result = safeTypeConversion<Int64, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert a string specified by starting and ending pointers to an integer value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToUInt64( const CharType* stringStart, const CharType* stringEnd,
										UInt64& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				Double d;
				
				if ( convertStringToNumber( stringStart, stringEnd, d ) )
				{
					result = safeTypeConversion<UInt64, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
			/// Convert a string specified by starting and ending pointers to a float value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToFloat( const CharType* stringStart, const CharType* stringEnd,
										Float& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				return convertStringToNumber( stringStart, stringEnd, result );
			}
			
			
			/// Convert a string specified by starting and ending pointers to a float value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			static Bool convertToDouble( const CharType* stringStart, const CharType* stringEnd,
										Double& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				return convertStringToNumber( stringStart, stringEnd, result );
			}
			
			
			/// Convert a string specified by starting and ending pointers to a number value.
			/**
			  * The method returns whether or not the conversion was successful.
			  */
			template < typename T >
			static Bool convertToNumber( const CharType* stringStart, const CharType* stringEnd,
										T& result )
			{
				if ( !stringStart || stringStart >= stringEnd )
					return false;
				
				Double d;
				
				if ( convertStringToNumber( stringStart, stringEnd, d ) )
				{
					result = safeTypeConversion<T, Double>( d );
					return true;
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	String to Number Conversion Test Methods
			
			
			/// Return whether or not this string represents a valid number.
			OM_INLINE Bool isANumber() const
			{
				return isANumber( string, shared->length );
			}
			
			
		//********************************************************************************
		//******	Character Accessor Methods
			
			
			/// Return the character at the specified index in this string.
			/**
			  * A debug assertion is raised if the index is invalid.
			  */
			OM_INLINE CharType get( Index index ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( index < length - 1, "String index is out-of-bounds" );
				return string[index];
			}
			
			
			/// Return the character at the specified index in this string.
			/**
			  * A debug assertion is raised if the index is invalid.
			  */
			OM_INLINE CharType operator [] ( Index index ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( index < length - 1, "String index is out-of-bounds" );
				return string[index];
			}
			
			
			/// Return the character at the specified index in this string.
			/**
			  * A debug assertion is raised if the index is invalid.
			  */
			OM_INLINE CharType operator () ( Index index ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( index < length - 1, "String index is out-of-bounds" );
				return string[index];
			}
			
			
		//********************************************************************************
		//******	Substring Accessor Method
			
			
			/// Return a sub-string of this string, specified by the start index and number of code points (characters).
			OM_INLINE GenericString getSubString( Index start, Size number ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( start < length - 1, "Substring start index is out of string bounds" );
				OM_DEBUG_ASSERT_MESSAGE( start + number < length, "Substring end index is out of string bounds" );
				
				return GenericString( string + start, number );
			}
			
			
		//********************************************************************************
		//******	String Character Array Accessor Method
			
			
			/// Return a pointer to a NULL-terminated character array representing this string.
			OM_INLINE const CharType* getCString() const
			{
				return string;
			}
			
			
			/// Return a pointer to a NULL-terminated character array representing this string.
			OM_INLINE const CharType* getPointer() const
			{
				return string;
			}
			
			
			/// Return a pointer to a NULL-terminated character array representing this string.
			OM_INLINE operator const CharType* () const
			{
				return string;
			}
			
			
		//********************************************************************************
		//******	String Length Accessor Methods
			
			
			/// Return the number of character code points that are part of this string, not including the NULL terminator.
			OM_FORCE_INLINE Size getLength() const
			{
				return shared->length - 1;
			}
			
			
			/// Return the actual length of this string in characters.
			/**
			  * This method parses the multi-byte encoded string and determines
			  * the actual number of characters that it contains (which will be less
			  * than or equal to the number of code points).
			  */
			Size getLengthInCharacters() const;
			
			
			/// Return the number of code points that are part of the specified NULL-terminated string.
			/**
			  * The value returned does not include the NULL termination character.
			  */
			static Size getLength( const CharType* characters );
			
			
			/// Return the number of characters that are part of the specified NULL-terminated string.
			/**
			  * This method parses the multi-byte encoded string and determines
			  * the actual number of characters that it contains (which will be less
			  * than or equal to the number of code points).
			  */
			static Size getLengthInCharacters( const CharType* characters );
			
			
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			/// Return a hash code for this string.
			OM_INLINE Hash getHashCode() const
			{
				Hash& hash = shared->hashCode;
				
				if ( hash != 0 )
					return hash;
				else
				{
					hash = HashCode( string, shared->length );
					return hash;
				}
			}
			
			
		//********************************************************************************
		//******	Static Character Trait Accessor Methods
			
			
			/// Convert this string to lower case.
			GenericString toLowerCase() const;
			
			
			/// Convert this string to lower case.
			GenericString toUpperCase() const;
			
			
			// If the specified character is an upper-case character, convert it a lower-case one.
			OM_INLINE static CharType toLowerCase( CharType character )
			{
				if ( character >= 'A' && character <= 'Z' )
					return character + 32;
				else
					return character;
			}
			
			
			// If the specified character is a lower-case character, convert it an upper-case one.
			OM_INLINE static CharType toUpperCase( CharType character )
			{
				if ( character >= 'a' && character <= 'z' )
					return character - 32;
				else
					return character;
			}
			
			
			/// Return whether or not the specified character is a letter character.
			OM_INLINE static Bool isLetter( CharType character )
			{
				return	(character >= 'A' && character <= 'Z') || 
						(character >= 'a' && character <= 'z');
			}
			
			
			/// Return whether or not the specified character is an upper-case character.
			OM_INLINE static Bool isUpperCase( CharType character )
			{
				return	character >= 'A' && character <= 'Z';
			}
			
			
			/// Return whether or not the specified character is a lower-case character.
			OM_INLINE static Bool isLowerCase( CharType character )
			{
				return	character >= 'a' && character <= 'z';
			}
			
			
			/// Return whether or not the specified character is a decimal digit (0 to 9).
			OM_INLINE static Bool isDigit( CharType character )
			{
				return character >= '0' && character <= '9';
			}
			
			
			/// Return whether or not the specified character is an octal digit (0 to 7).
			OM_INLINE static Bool isOctalDigit( CharType character )
			{
				return character >= '0' && character <= '7';
			}
			
			
			/// Return whether or not the specified character is an binary digit (0 or 1).
			OM_INLINE static Bool isBinaryDigit( CharType character )
			{
				return character == '0' || character == '1';
			}
			
			
			/// Return whether or not the specified character is a hexidecimal digit (0 to 9, A to F).
			OM_INLINE static Bool isHexDigit( CharType character )
			{
				return	(character >= '0' && character <= '9') ||
						(character >= 'A' && character <= 'F') ||
						(character >= 'a' && character <= 'f');
			}
			
			
			/// Return whether or not the specified character is a whitespace character (i.e. space, tab, new line, etc).
			OM_INLINE static Bool isWhitespace( CharType character )
			{
				return character == ' ' || character == '\t' || character == '\n' || character == '\r';
			}
			
			
			/// Return whether or not the specified character is a puntuation character (i.e. period, comma, semicolon, etc).
			OM_INLINE static Bool isPunctuation( CharType c )
			{
				return c == '.' || c == ',' || c == '-' || c == '+' || c == ':' || c == ';' || c == '<' || c == '>' ||
						c == '=' || c == '?' || c == '!' || c == '`' || c == '/' || c == '\\' || c == '*' || c == '\'' ||
						c == '"';
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			template < typename OtherCharType >
			friend class GenericString;
			
			
			/// A reference-counted string class that has contiguously-allocated character data.
			class SharedString
			{
				public:
					
					/// Create a new shared string which has the given length and uninitialized characters.
					OM_INLINE SharedString( Size newLength )
						:	length( newLength ),
							referenceCount( 1 ),
							hashCode( 0 )
					{
					}
					
					
					/// Return a pointer to the characters that are part of this shared string object.
					/**
					  * The characters are allocated contiguously after the end of the object.
					  */
					OM_INLINE CharType* getCharacters()
					{
						return (CharType*)((UByte*)this + sizeof(SharedString));
					}
					
					
					/// The length in characters (including the NULL terminator) of this string.
					Size length;
					
					/// The number of strings that reference this shared string.
					/**
					  * When this number reaches 0, the string is deallocated.
					  */
					threads::Atomic<Size> referenceCount;
					
					/// A hash code for this string, lazily computed when it is first needed.
					/**
					  * A value of 0 indicates that the hash code has not been computed.
					  */
					Hash hashCode;
					
			};
			
			
			/// A template class used to define shared strings of static length.
			template < Size bufferSize >
			class SharedStaticString
			{
				public:
					
					OM_INLINE SharedStaticString( const Char* characters )
						:	string( bufferSize )
					{
						util::convert( string.getCharacters(), characters, bufferSize );
					}
					
					SharedString string;
					
					/// A storage buffer of characters.
					CharType storage[bufferSize];
					
			};
			
			
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Create a new string with the specified shared string.
			OM_INLINE GenericString( SharedString* sharedString )
				:	shared( sharedString )
			{
				string = shared->getCharacters();
			}
			
			
		//********************************************************************************
		//******	String Allocation Methods
			
			
			static SharedString* allocateString( Size length )
			{
				Size sharedStringSize = sizeof(SharedString) + sizeof(CharType)*length;
				UByte* sharedStringBytes = util::allocate<UByte>( sharedStringSize );
				
				new ( sharedStringBytes ) SharedString( length );
				
				return (SharedString*)sharedStringBytes;
			}
			
			
			static void deallocateString( SharedString* sharedString )
			{
				if ( --sharedString->referenceCount == Size(0) )
					util::deallocate( (UByte*)sharedString );
			}
			
			
		//********************************************************************************
		//******	Private Static Unicode Conversion Methods
			
			
			template < typename CharType2 >
			OM_INLINE static SharedString* convertUnicode( const CharType2* unicodeString );
			
			
			template < typename CharType2 >
			OM_INLINE static SharedString* convertUnicode( const CharType2* unicodeString, const CharType2* unicodeStringEnd );
			
			
			
		//********************************************************************************
		//******	Private Static String Concatenation Methods
			
			
			OM_INLINE static SharedString* concatenateStrings( const CharType* string1, Size length1,
																const CharType* string2, Size length2 );
			
			
		//********************************************************************************
		//******	Private Static String-Number Conversion Methods
			
			
			static SharedString* fromIntegerType( Int32 value, Size base );
			static SharedString* fromIntegerType( UInt32 value, Size base );
			static SharedString* fromIntegerType( Int64 value, Size base );
			static SharedString* fromIntegerType( UInt64 value, Size base );
			
			
			template < typename ValueType >
			OM_INLINE static SharedString* fromSignedIntegerType( ValueType value, Size base );
			
			template < typename ValueType >
			OM_INLINE static SharedString* fromUnsignedIntegerType( ValueType value, Size base );
			
			
		//********************************************************************************
		//******	Private Static String-Number Conversion Methods
			
			
			static SharedString* fromFloatType( Float value, Size numDecimalPlaces, Size base,
												Bool allowScientific );
			static SharedString* fromFloatType( Double value, Size numDecimalPlaces, Size base,
												Bool allowScientific );
			
			template < typename ValueType >
			OM_INLINE static SharedString* fromFloatingPointType( ValueType value, Size numDecimalDigits,
																Size precision, Size base, Bool allowScientific );
			
			
		//********************************************************************************
		//******	Private Static String-Number Conversion Methods
			
			
			template < typename ValueType >
			OM_NO_INLINE static Bool parseSimpleNumber( const CharType* numberStart, const CharType* numberEnd,
														Size base, ValueType& number );
			
			template < typename ValueType >
			OM_NO_INLINE static Bool convertStringToNumber( const CharType* stringStart, const CharType* stringEnd,
															ValueType& result );
			
			
			OM_NO_INLINE static Bool convertStringToBoolean( const CharType* stringStart, const CharType* stringEnd,
															Bool& result );
			
			
			static Bool isANumber( const CharType* string, Size length );
			
			
			template < typename ValueType, typename FloatType >
			OM_FORCE_INLINE static ValueType safeTypeConversion( FloatType value )
			{
				if ( value < (FloatType)math::min<ValueType>() )
					return math::min<ValueType>();
				else if ( value > (FloatType)math::max<ValueType>() )
					return math::max<ValueType>();
				else
					return (ValueType)value;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the character buffer for this string.
			CharType* string;
			
			
			/// A pointer to the shared string object that stores this string's reference-counted characters.
			SharedString* shared;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// A static shared string which represents the null string (that has 0 characters, with a null terminator).
			static SharedStaticString<1> nullString;
			
			
			/// A static shared string which represents the boolean 'true' literal string.
			static SharedStaticString<5> trueString;
			
			
			/// A static shared string which represents the boolean 'false' literal string.
			static SharedStaticString<6> falseString;
			
			
			/// A static shared string which represents the +infinity literal string.
			static SharedStaticString<9> positiveInfinityString;
			
			
			/// A static shared string which represents the -infinity literal string.
			static SharedStaticString<10> negativeInfinityString;
			
			
			/// A static shared string which represents the not-a-number literal string.
			static SharedStaticString<4> nanString;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Comparison Operators
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
OM_INLINE GenericString<CharType> operator == ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::equals( characters, string );
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator != ( const CharType* characters, const GenericString<CharType>& string )
{
	return !GenericString<CharType>::equals( characters, string );
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator < ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compare( characters, string ) == -1;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator > ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compare( characters, string ) == 1;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator <= ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compare( characters, string ) < 1;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator >= ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compare( characters, string ) > -1;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator << ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compareIgnoreCase( characters, string ) == -1;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator >> ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compareIgnoreCase( characters, string ) == 1;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator <<= ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compareIgnoreCase( characters, string ) < 1;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator >>= ( const CharType* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>::compareIgnoreCase( characters, string ) > -1;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Concatenation Operators
//############		
//##########################################################################################
//##########################################################################################




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( const Char* characters, const GenericString<CharType>& string )
{
	return GenericString<CharType>( characters ) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( Char character, const GenericString<CharType>& string )
{
	return GenericString<CharType>( character ) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( Short value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( UShort value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( Int value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( UInt value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( Long value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( ULong value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( LongLong value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




template < typename CharType >
OM_INLINE GenericString<CharType> operator + ( ULongLong value, const GenericString<CharType>& string )
{
	return GenericString<CharType>(value) + string;
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Type Definitions
//############		
//##########################################################################################
//##########################################################################################




/// A class that represents a standard NULL-terminated ASCII-encoded string of characters.
typedef GenericString<Char> ASCIIString;




/// A class that represents a standard NULL-terminated ASCII-encoded string of characters.
typedef ASCIIString String;




/// A class that represents a NULL-terminated UTF8-encoded string of characters.
typedef GenericString<UTF8Char> UTF8String;




/// A class that represents a NULL-terminated UTF16-encoded string of characters.
typedef GenericString<UTF16Char> UTF16String;




/// A class that represents a NULL-terminated UTF32-encoded string of characters.
typedef GenericString<UTF32Char> UTF32String;




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_STRING_H
