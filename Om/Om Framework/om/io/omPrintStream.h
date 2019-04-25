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

#ifndef INCLUDE_OM_PRINT_STREAM_H
#define INCLUDE_OM_PRINT_STREAM_H


#include "omIOConfig.h"


#include "omStringOutputStream.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that allows the user to print messages/data to standard output.
/** 
  * It is a wrapper for C standard output that implements the StringOutputStream interface.
  */
class PrintStream : public StringOutputStream
{
	public:
		
		//********************************************************************************
		//******	Flush Method
			
			
			/// Flush the print stream, sending all internally buffered output to standard output.
			/** 
			  * This method causes all currently pending output data to be sent to
			  * C standard output. This method ensures that this is done and that all internal
			  * data buffers are emptied if they have any contents.
			  */
			virtual void flush();
			
			
	protected:
		
		//********************************************************************************
		//******	Protected String Write Methods
			
			
			/// Write the specified number of characters from the character buffer and return the number written.
			virtual Size writeChars( const Char* characters, Size number );
			
			
			/// Write the specified number of UTF-8 characters from the character buffer and return the number written.
			virtual Size writeUTF8Chars( const UTF8Char* characters, Size number );
			
			
			/// Write the specified number of UTF-16 characters from the character buffer and return the number written.
			virtual Size writeUTF16Chars( const UTF16Char* characters, Size number );
			
			
			/// Write the specified number of UTF-32 characters from the character buffer and return the number written.
			virtual Size writeUTF32Chars( const UTF32Char* characters, Size number );
			
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_PRINT_STREAM_H
