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

#include "omStringIterator.h"


//##########################################################################################
//*******************************   Start Data Namespace   *********************************
OM_DATA_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		UTF-8 Specialization Methods
//############		
//##########################################################################################
//##########################################################################################




void GenericStringIterator<UTF8Char>:: advanceMultiByteCharacter()
{
	UTF8Char c = *current;
	
	if ( (c & 0xE0) == 0xC0 )
	{
		// This is a two-byte UTF-8 character, skip two bytes.
		current += 2;
	}
	else if ( (c & 0xF0) == 0xE0 )
	{
		// This is a three-byte UTF-8 character, skip three bytes.
		current += 3;
	}
	else if ( (c & 0xF8) == 0xF0 )
	{
		// This is a four-byte UTF-8 character, skip four bytes.
		current += 4;
	}
	else
	{
		// The unicode character is malformed. Skip to the next, hopefully finding a valid code point.
		current++;
	}
}




UTF32Char GenericStringIterator<UTF8Char>:: getMultiByteCharacter() const
{
	UTF8Char c = *current;
	
	if ( (c & 0xE0) == 0xC0 )
	{
		// This is a two-byte UTF-8 character, skip two bytes.
		return ((UTF32Char(c) & 0x1F) << 6) | (UTF32Char(current[1]) & 0x3F);
	}
	else if ( (c & 0xF0) == 0xE0 )
	{
		// This is a three-byte UTF-8 character, skip three bytes.
		return ((UTF32Char(c) & 0x0F) << 12) |
				((UTF32Char(current[1]) & 0x3F) << 6) |
				(UTF32Char(current[2]) & 0x3F);
	}
	else if ( (c & 0xF8) == 0xF0 )
	{
		// This is a four-byte UTF-8 character, skip four bytes.
		return ((UTF32Char(c) & 0x07) << 18) |
				((UTF32Char(current[1]) & 0x3F) << 12) |
				((UTF32Char(current[2]) & 0x3F) << 6) |
				(UTF32Char(current[3]) & 0x3F);
	}
	else
	{
		// The unicode character is malformed, return a placeholder character.
		return '?';
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		UTF-16 Specialization Methods
//############		
//##########################################################################################
//##########################################################################################




UTF32Char GenericStringIterator<UTF16Char>:: operator * () const
{
	UTF16Char c = *current;
	
	if ( c >= 0xD800 && c <= 0xDBFF )
	{
		// This is two-point UTF-16 character.
		UTF16Char c2 = current[1];
		
		if ( c2 >= 0xDC00 && c2 <= 0xDFFF )
		{
			return (((UTF32Char(c) & 0x03FF) << 10) |
					(UTF32Char(c2) & 0x03FF)) + 0x00010000;
		}
		else
		{
			// The character is malformed. Return a placeholder character.
			return '?';
		}
	}
	else if ( c >= 0xDC00 && c <= 0xDFFF )
	{
		// The character is malformed. Return a placeholder character.
		return '?';
	}
	else
	{
		// This is a single-point UTF-16 character.
		return UTF32Char(c);
	}
}




//##########################################################################################
//*******************************   End Data Namespace   ***********************************
OM_DATA_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
