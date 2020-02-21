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

#ifndef INCLUDE_OM_STRING_INPUT_STREAM_H
#define INCLUDE_OM_STRING_INPUT_STREAM_H


#include "omIOConfig.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that abstracts a source for a stream of character information.
class StringInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a StringInputStream with the native endianness.
			OM_INLINE StringInputStream()
			{
			}
			
			
			/// Create a StringInputStream with the specified input endianness.
			OM_INLINE StringInputStream( data::Endianness newEndianness )
				:	endianness( newEndianness )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an input stream and free all of it's resources (close it).
			virtual ~StringInputStream()
			{
			}
			
			
		//********************************************************************************
		//******	Remaining Characters Size Accessor Methods
			
			
			/// Return the number of ASCII characters remaining in the stream.
			/**
			  * The value returned must only be a lower bound on the number of characters
			  * remaining in the stream. If there are characters remaining, it must return
			  * at least 1.
			  */
			virtual LargeSize getCharactersRemaining() const = 0;
			
			
			/// Return whether or not there are bytes remaining in the stream.
			OM_INLINE Bool hasCharactersRemaining() const
			{
				return this->getCharactersRemaining() > LargeSize(0);
			}
			
			
		//********************************************************************************
		//******	Character/String Read Methods
			
			
			/// Read a single ASCII character and place it in the input parameter.
			/**
			  * If the read operation was successful, return TRUE, otherwise return
			  * FALSE.
			  */
			OM_INLINE Bool readASCII( Char& value )
			{
				return this->readChars( &value, 1 ) == 1;
			}
			
			
			/// Read a single UTF32 character and place it in the input parameter.
			/**
			  * If the read operation was successful, return TRUE, otherwise return
			  * FALSE.
			  */
			OM_INLINE Bool readUTF32( UTF32Char& value )
			{
				return this->readUTF32Chars( &value, 1 ) == 1;
			}
			
			
			/// Read the specified number of ASCII characters and place them in the output buffer.
			/**
			  * The number of characters successfully read is returned.
			  */
			OM_INLINE Size readASCII( Char* buffer, Size numBytes )
			{
				if ( buffer == NULL )
					return 0;
				
				return readChars( buffer, numBytes );
			}
			
			
			/// Read the specified number of UTF-8 characters and place them in the output buffer.
			/**
			  * The number of unicode code points successfully read is returned. The number of
			  * code points read is limited to the specified buffer capacity. The number of 
			  * full unicode characters actually read is placed in the input/output paramter
			  * numChars.
			  */
			OM_INLINE Size readUTF8( UTF8Char* buffer, Size numChars, Size capacity )
			{
				if ( buffer == NULL )
					return 0;
				
				return readUTF8Chars( buffer, numChars, capacity );
			}
			
			
			/// Read the specified number of UTF-16 characters and place them in the output buffer.
			/**
			  * The number of unicode code points successfully read is returned. The number of
			  * code points read is limited to the specified buffer capacity. The number of 
			  * full unicode characters actually read is placed in the input/output paramter
			  * numChars.
			  */
			OM_INLINE Size readUTF16( UTF16Char* buffer, Size numChars, Size capacity )
			{
				if ( buffer == NULL )
					return 0;
				
				return readUTF16Chars( buffer, numChars, capacity );
			}
			
			
			/// Read the specified number of UTF-16 characters and place them in the output buffer.
			/**
			  * The number of characters successfully read is returned.
			  */
			OM_INLINE Size readUTF32( UTF32Char* buffer, Size numChars )
			{
				if ( buffer == NULL )
					return 0;
				
				return readUTF32Chars( buffer, numChars );
			}
			
			
			/// Read the specified number of characters from the stream and place them in the specified string buffer.
			Size readASCII( data::StringBuffer& buffer, Size numChars );
			
			
			/// Read the specified number of characters from the stream and place them in the specified UTF-8 string buffer.
			/**
			  * The number of unicode code points read is returned and the number of characters actually
			  * read is placed in the input/output paramter numChars.
			  */
			Size readUTF8( data::UTF8StringBuffer& buffer, Size numChars );
			
			
			/// Read the specified number of characters from the stream and place them in the specified UTF-16 string buffer.
			/**
			  * The number of unicode code points read is returned and the number of characters actually
			  * read is placed in the input/output paramter numChars.
			  */
			Size readUTF16( data::UTF16StringBuffer& buffer, Size numChars );
			
			
			/// Read the specified number of characters from the stream and place them in the specified UTF-32 string buffer.
			/**
			  * The number of characters read is returned.
			  */
			Size readUTF32( data::UTF32StringBuffer& buffer, Size numChars );
			
			
			data::String readAllASCII();
			
			
			data::UTF8String readAllUTF8();
			
			
			OM_INLINE data::UTF16String readAllUTF16();
			
			
			OM_INLINE data::UTF32String readAllUTF32();
			
			
		//********************************************************************************
		//******	Endian-ness Accessor Methods
			
			
			/// Get the endianness to assume when reading from the stream.
			OM_INLINE data::Endianness getEndianness() const
			{
				return endianness;
			}
			
			
			/// Set the endianness to assume when reading from the stream.
			OM_INLINE void setEndianness( data::Endianness newEndianness )
			{
				endianness = newEndianness;
			}
			
			
	protected:
		
		//********************************************************************************
		//******	Protected String Read Method
			
			
			/// Read the specified number of characters from the stream and place them in the specified output buffer.
			virtual Size readChars( Char* buffer, Size number ) = 0;
			
			
			/// Read the specified number of UTF-8 characters from the stream and place them in the specified output buffer.
			/**
			  * If the number of unicode code points exceeds the capacity of the buffer, as many characters
			  * are read as possible. The number of code points read is returned.
			  */
			virtual Size readUTF8Chars( UTF8Char* buffer, Size numChars, Size capacity ) = 0;
			
			
			/// Read the specified number of UTF-16 characters from the stream and place them in the specified output buffer.
			/**
			  * If the number of unicode code points exceeds the capacity of the buffer, as many characters
			  * are read as possible. The number of code points read is returned.
			  */
			virtual Size readUTF16Chars( UTF16Char* buffer, Size numChars, Size capacity ) = 0;
			
			
			/// Read the specified number of UTF-32 characters from the stream and place them in the specified output buffer.
			/**
			  * If the number of unicode code points exceeds the capacity of the buffer, as many characters
			  * are read as possible. The number of code points read is returned.
			  */
			virtual Size readUTF32Chars( UTF32Char* buffer, Size numChars ) = 0;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Convert the specified number of characters in a buffer to native endiannness.
			template < typename CharType >
			OM_INLINE void convertEndianness( CharType* buffer, Size number )
			{
				const CharType* const bufferEnd = buffer + number;
				
				while ( buffer != bufferEnd )
				{
					*buffer = endianness.convertToNative( *buffer );
					buffer++;
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The endianness in which the data being read from the stream is assumed to be in.
			/**
			  * Upon reading primitive types from the stream, they are converted to the platform's
			  * native endianness before being returned to the user.
			  */
			data::Endianness endianness;
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_STRING_INPUT_STREAM_H
