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

#include "omBinaryDecoder.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




BinaryDecoder:: BinaryDecoder()
	:	format(),
		dataStart( NULL ),
		dataEnd( NULL ),
		data( NULL )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Reading Methods
//############		
//##########################################################################################
//##########################################################################################




data::String BinaryDecoder:: readASCIIString()
{
	// Read the string's length.
	UInt64 stringLength = 0;
	
	if ( !this->read( stringLength ) )
		return data::String();
	
	// Compute the padded length according to the alignment.
	const Size paddedLength = padSize( stringLength*sizeof(Char) );
	const UByte* newData = data + paddedLength*sizeof(Char);
	
	if ( newData > dataEnd )
		return data::String();
	
	// Read the string's characters.
	data::String result( (const Char*)data, stringLength );
	data = newData;
	
	return result;
}




data::UTF8String BinaryDecoder:: readUTF8String()
{
	// Read the string's length.
	UInt64 stringLength = 0;
	
	if ( !this->read( stringLength ) )
		return data::UTF8String();
	
	// Compute the padded length according to the alignment.
	const Size paddedLength = padSize( stringLength*sizeof(UTF8Char) );
	const UByte* newData = data + paddedLength*sizeof(UTF8Char);
	
	if ( newData > dataEnd )
		return data::UTF8String();
	
	// Read the string's characters.
	data::UTF8String result( (const UTF8Char*)data, stringLength );
	data = newData;
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Data Reading Methods
//############		
//##########################################################################################
//##########################################################################################




Bool BinaryDecoder:: read( UByte* buffer, Size number )
{
	const UByte* newData = data + number;
	
	if ( newData > dataEnd )
		return false;
	
	util::copyPOD( buffer, data, number );
	data = newData;
	
	return true;
}




Size BinaryDecoder:: readData( UByte* buffer, Size number )
{
	if ( this->read( buffer, number ) )
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




Bool BinaryDecoder:: canSeek() const
{
	return true;
}




Bool BinaryDecoder:: canSeek( Int64 relativeOffset ) const
{
	const UByte* newData = data + relativeOffset;
	
	if ( newData < dataStart || newData > dataEnd )
		return false;
	
	return true;
}




Int64 BinaryDecoder:: seek( Int64 relativeOffset )
{
	const UByte* oldData = data;
	const UByte* newData = data + relativeOffset;
	
	if ( newData > dataEnd )
		data = dataEnd;
	else if ( newData < dataStart )
		data = dataStart;
	else
		data = newData;
	
	return data - oldData;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Read Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeSize BinaryDecoder:: getBytesRemaining() const
{
	return dataEnd - data;
}




LargeIndex BinaryDecoder:: getPosition() const
{
	return data - dataStart;
}




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
