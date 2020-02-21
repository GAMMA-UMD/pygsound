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

#ifndef INCLUDE_OM_STRING_OUTPUT_STREAM_H
#define INCLUDE_OM_STRING_OUTPUT_STREAM_H


#include "omIOConfig.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that abstracts a destination for a stream of character information.
class StringOutputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a StringOutputStream with the native output endianness.
			OM_INLINE StringOutputStream()
			{
			}
			
			
			/// Create a StringOutputStream with the specified output endianness.
			OM_INLINE StringOutputStream( data::Endianness newEndianness )
				:	endianness( newEndianness )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an output stream and free all of it's resources (close it).
			virtual ~StringOutputStream()
			{
			}
			
			
		//********************************************************************************
		//******	ASCII String Write Methods
			
			
			/// Write one ASCII character to the output stream.
			OM_INLINE Bool writeASCII( Char character )
			{
				return this->writeChars( &character, Size(1) ) == Size(1);
			}
			
			
			/// Write characters from the buffer until a NULl terminator is reached and return the number written.
			OM_INLINE Size writeASCII( const Char* characters )
			{
				return this->writeChars( characters, data::String::getLength( characters ) );
			}
			
			
			/// Write the specified number of characters from the buffer and return the number written.
			OM_INLINE Size writeASCII( const Char* characters, Size numCharacters )
			{
				return this->writeChars( characters, numCharacters );
			}
			
			
			/// Write the specified string to the output string and return the number of characters written.
			OM_INLINE Size writeASCII( const data::String& string )
			{
				return this->writeChars( string.getCString(), string.getLength() );
			}
			
			
		//********************************************************************************
		//******	UTF-8 String Write Methods
			
			
			/// Write one UTF-8 character to the output stream.
			OM_INLINE Bool writeUTF8( UTF8Char character )
			{
				return this->writeUTF8Chars( &character, Size(1) ) == Size(1);
			}
			
			
			/// Write characters from the buffer until a NULl terminator is reached and return the number written.
			OM_INLINE Size writeUTF8( const UTF8Char* characters )
			{
				return this->writeUTF8Chars( characters, data::UTF8String::getLength( characters ) );
			}
			
			
			/// Write the specified number of characters from the buffer and return the number written.
			OM_INLINE Size writeUTF8( const UTF8Char* characters, Size numCharacters )
			{
				return this->writeUTF8Chars( characters, numCharacters );
			}
			
			
			/// Write the specified string to the output string and return the number of characters written.
			OM_INLINE Size writeUTF8( const data::UTF8String& string )
			{
				return this->writeUTF8Chars( string.getCString(), string.getLength() );
			}
			
			
		//********************************************************************************
		//******	UTF-16 String Write Methods
			
			
			/// Write one UTF-16 character to the output stream.
			OM_INLINE Bool writeUTF16( UTF16Char character )
			{
				character = endianness.convertToNative( character );
				return this->writeUTF16Chars( &character, Size(1) ) == Size(1);
			}
			
			
			/// Write characters from the buffer until a NULL terminator is reached and return the number written.
			OM_INLINE Size writeUTF16( const UTF16Char* characters )
			{
				const UTF16Char* character = characters;
				Size numWritten = 0;
				
				while ( *character )
				{
					numWritten += this->writeUTF16( *character ) ? 1 : 0;
					character++;
				}
				
				return numWritten;
			}
			
			
			/// Write the specified number of characters from the buffer and return the number written.
			OM_INLINE Size writeUTF16( const UTF16Char* characters, Size numCharacters )
			{
				const UTF16Char* character = characters;
				const UTF16Char* const charactersEnd = characters + numCharacters;
				Size numWritten = 0;
				
				while ( character != charactersEnd )
				{
					numWritten += this->writeUTF16( *character ) ? 1 : 0;
					character++;
				}
				
				return numWritten;
			}
			
			
			/// Write the specified string to the output string and return the number of characters written.
			OM_INLINE Size writeUTF16( const data::UTF16String& string )
			{
				return this->writeUTF16( string.getCString(), string.getLength() );
			}
			
			
		//********************************************************************************
		//******	UTF-32 String Write Methods
			
			
			/// Write one UTF-32 character to the output stream.
			OM_INLINE Bool writeUTF32( UTF32Char character )
			{
				character = endianness.convertToNative( character );
				return this->writeUTF32Chars( &character, Size(1) ) == Size(1);
			}
			
			
			/// Write characters from the buffer until a NULl terminator is reached and return the number written.
			OM_INLINE Size writeUTF32( const UTF32Char* characters )
			{
				const UTF32Char* character = characters;
				Size numWritten = 0;
				
				while ( *character )
				{
					numWritten += this->writeUTF32( *character ) ? 1 : 0;
					character++;
				}
				
				return numWritten;
			}
			
			
			/// Write the specified number of characters from the buffer and return the number written.
			OM_INLINE Size writeUTF32( const UTF32Char* characters, Size numCharacters )
			{
				const UTF32Char* character = characters;
				const UTF32Char* const charactersEnd = characters + numCharacters;
				Size numWritten = 0;
				
				while ( character != charactersEnd )
				{
					numWritten += this->writeUTF32( *character ) ? 1 : 0;
					character++;
				}
				
				return numWritten;
			}
			
			
			/// Write the specified string to the output string and return the number of characters written.
			OM_INLINE Size writeUTF32( const data::UTF32String& string )
			{
				return this->writeUTF32( string.getCString(), string.getLength() );
			}
			
			
		//********************************************************************************
		//******	Flush Method
			
			
			/// Flush the output stream, sending all internally buffered output to it's destination.
			/** 
			  * This method causes all currently pending output data to be sent to it's
			  * final destination. This method ensures that this is done and that all internal
			  * data buffers are emptied if they have any contents.
			  */
			virtual void flush() = 0;
			
			
		//********************************************************************************
		//******	Endian-ness Accessor Methods
			
			
			/// Get the current endianness of the wide characters being written to the stream.
			OM_INLINE data::Endianness getEndianness() const
			{
				return endianness;
			}
			
			
			/// Set the stream to write wide characters in the specified endian format.
			OM_INLINE void setEndianness( data::Endianness newEndianness )
			{
				endianness = newEndianness;
			}
			
			
	protected:
			
		//********************************************************************************
		//******	Protected String Write Methods
			
			
			/// Write the specified number of characters from the character buffer and return the number written.
			virtual Size writeChars( const Char* characters, Size number ) = 0;
			
			
			/// Write the specified number of UTF-8 characters from the character buffer and return the number written.
			virtual Size writeUTF8Chars( const UTF8Char* characters, Size number ) = 0;
			
			
			/// Write the specified number of UTF-16 characters from the character buffer and return the number written.
			virtual Size writeUTF16Chars( const UTF16Char* characters, Size number ) = 0;
			
			
			/// Write the specified number of UTF-32 characters from the character buffer and return the number written.
			virtual Size writeUTF32Chars( const UTF32Char* characters, Size number ) = 0;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The endianness in which the data being written to the stream is converted to.
			/**
			  * Wide characters are converted from the native platform endianness to this
			  * endianness before they are written to a string output stream.
			  */
			data::Endianness endianness;
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_STRING_OUTPUT_STREAM_H
