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

#include "omStringInputStream.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		String Buffer Reading Methods
//############		
//##########################################################################################
//##########################################################################################




Size StringInputStream:: readASCII( data::StringBuffer& buffer, Size numChars )
{
	Size necessaryCapacity = buffer.nextElement - buffer.buffer + numChars;
	
	if ( necessaryCapacity > buffer.getCapacity() )
		buffer.increaseCapacity( necessaryCapacity );
	
	Size numberRead = this->readChars( buffer.nextElement, numChars );
	
	buffer.nextElement += numberRead;
	buffer.nextElement[1] = '\0';
	
	return numberRead;
}




Size StringInputStream:: readUTF8( data::UTF8StringBuffer& buffer, Size numChars )
{
	Size necessaryCapacity = buffer.nextElement - buffer.buffer + numChars*2;
	
	if ( necessaryCapacity > buffer.getCapacity() )
		buffer.increaseCapacity( necessaryCapacity );
	
	Size numberRead = this->readUTF8Chars( buffer.nextElement, numChars, numChars*2 );
	
	buffer.nextElement += numberRead;
	buffer.nextElement[1] = '\0';
	
	return numberRead;
}




Size StringInputStream:: readUTF16( data::UTF16StringBuffer& buffer, Size numChars )
{
	Size necessaryCapacity = buffer.nextElement - buffer.buffer + numChars;
	
	if ( necessaryCapacity > buffer.getCapacity() )
		buffer.increaseCapacity( necessaryCapacity );
	
	Size numberRead = this->readUTF16Chars( buffer.nextElement, numChars, numChars );
	
	if ( !endianness.isNative() )
		this->convertEndianness( buffer.nextElement, numberRead );
	
	buffer.nextElement += numberRead;
	buffer.nextElement[1] = '\0';
	
	return numberRead;
}




Size StringInputStream:: readUTF32( data::UTF32StringBuffer& buffer, Size numChars )
{
	Size necessaryCapacity = buffer.nextElement - buffer.buffer + numChars;
	
	if ( necessaryCapacity > buffer.getCapacity() )
		buffer.increaseCapacity( necessaryCapacity );
	
	Size numberRead = this->readUTF32Chars( buffer.nextElement, numChars );
	
	if ( !endianness.isNative() )
		this->convertEndianness( buffer.nextElement, numberRead );
	
	buffer.nextElement += numberRead;
	buffer.nextElement[1] = '\0';
	
	return numberRead;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Entire String Reading Methods
//############		
//##########################################################################################
//##########################################################################################




data::String StringInputStream:: readAllASCII()
{
	Size charsRemaining = (Size)this->getCharactersRemaining();
	data::StringBuffer buffer( charsRemaining );
	
	while ( charsRemaining > 0 )
	{
		this->readASCII( buffer, charsRemaining );
		charsRemaining = (Size)this->getCharactersRemaining();
	}
	
	return buffer.toString();
}




data::UTF8String StringInputStream:: readAllUTF8()
{
	Size charsRemaining = (Size)this->getCharactersRemaining();
	data::UTF8StringBuffer buffer( charsRemaining );
	
	while ( charsRemaining > 0 )
	{
		this->readUTF8( buffer, charsRemaining );
		charsRemaining = (Size)this->getCharactersRemaining();
	}
	
	return buffer.toString();
}




data::UTF16String StringInputStream:: readAllUTF16()
{
	Size charsRemaining = (Size)this->getCharactersRemaining();
	data::UTF16StringBuffer buffer( charsRemaining );
	
	while ( charsRemaining > 0 )
	{
		this->readUTF16( buffer, charsRemaining );
		charsRemaining = (Size)this->getCharactersRemaining();
	}
	
	return buffer.toString();
}




data::UTF32String StringInputStream:: readAllUTF32()
{
	Size charsRemaining = (Size)this->getCharactersRemaining();
	data::UTF32StringBuffer buffer( charsRemaining );
	
	while ( charsRemaining > 0 )
	{
		this->readUTF32( buffer, charsRemaining );
		charsRemaining = (Size)this->getCharactersRemaining();
	}
	
	return buffer.toString();
}




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
