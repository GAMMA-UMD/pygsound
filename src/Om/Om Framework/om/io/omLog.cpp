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

#include "omLog.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


Log Console;


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Log:: Log()
	:	stream( NULL )
{
}




Log:: Log( StringOutputStream* newStream )
	:	stream( newStream )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Pointer Output Operator
//############		
//##########################################################################################
//##########################################################################################




Log& Log:: operator < ( const void* pointer )
{
	if ( pointer == NULL )
		writeChars( "NULL", 4 );
	else
	{
		data::String pointerString( PointerInt(pointer), 16 );
		writeChars( pointerString.getCString(), pointerString.getLength() );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Flush Method
//############		
//##########################################################################################
//##########################################################################################




void Log:: flush()
{
	if ( stream )
		return stream->flush();
	else
		fflush( stdout );
}




//##########################################################################################
//##########################################################################################
//############		
//############		String Output Stream Write Methods
//############		
//##########################################################################################
//##########################################################################################




Size Log:: writeChars( const Char* characters, Size number )
{
	if ( stream )
		return stream->writeASCII( characters, number );
	else
		return fwrite( characters, sizeof(Char), number, stdout );
}




Size Log:: writeUTF8Chars( const UTF8Char* characters, Size number )
{
	if ( stream )
		return stream->writeUTF8( characters, number );
	else
		return fwrite( (const char*)characters, sizeof(UTF8Char), number, stdout );
}




Size Log:: writeUTF16Chars( const UTF16Char* characters, Size number )
{
	if ( stream )
		return stream->writeUTF16( characters, number );
	else
		return fwrite( (const char*)characters, sizeof(UTF16Char), number, stdout );
}




Size Log:: writeUTF32Chars( const UTF32Char* characters, Size number )
{
	if ( stream )
		return stream->writeUTF32( characters, number );
	else
		return fwrite( (const char*)characters, sizeof(UTF32Char), number, stdout );
}




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
