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

#include "omPrintStream.h"


#include <stdio.h>


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Print Stream Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void PrintStream:: flush()
{
	fflush( stdout );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Protected Character Write Methods (Overidden from StringOutputStream)
//############		
//##########################################################################################
//##########################################################################################




Size PrintStream:: writeChars( const Char* characters, Size number )
{
	return fwrite( characters, sizeof(Char), number, stdout );
}




Size PrintStream:: writeUTF8Chars( const UTF8Char* characters, Size number )
{
	return fwrite( (const char*)characters, sizeof(UTF8Char), number, stdout );
}




Size PrintStream:: writeUTF16Chars( const UTF16Char* characters, Size number )
{
	return fwrite( (const char*)characters, sizeof(UTF16Char), number, stdout );
}




Size PrintStream:: writeUTF32Chars( const UTF32Char* characters, Size number )
{
	return fwrite( (const char*)characters, sizeof(UTF32Char), number, stdout );
}




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
