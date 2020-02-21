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

#include "omFileWriter.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




FileWriter:: FileWriter( const Char* filePath )
	:	file( filePath != NULL ? fs::Path( filePath ) : fs::Path() ),
		stream( NULL )
{
}




FileWriter:: FileWriter( const fs::UTF8String& filePath )
	:	file( fs::Path( filePath ) ),
		stream( NULL )
{
}




FileWriter:: FileWriter( const fs::Path& filePath )
	:	file( filePath ),
		stream( NULL )
{
}




FileWriter:: FileWriter( const fs::File& file )
	:	file( file ),
		stream( NULL )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructors
//############		
//##########################################################################################
//##########################################################################################




FileWriter:: ~FileWriter()
{
	if ( isOpen() )
		close();
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Open Method
//############		
//##########################################################################################
//##########################################################################################





Bool FileWriter:: open()
{
	// Exit the function if the file is already open.
	if ( isOpen() )
		return true;
	
	// If the file doesn't exist, try to create it.
	if ( !file.exists() )
	{
		// If the creation was not successful, return failure.
		if ( !file.create() )
			return false;
	}
	
	// open the file for binary read-only access.
	stream = std::fopen( (const char*)file.getPathString().getCString(), "rb+" );
	
	// if the result is NULL, then there was an error opening the file.
	if ( stream == NULL )
		return false;
	else
		return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Close Method
//############		
//##########################################################################################
//##########################################################################################




Bool FileWriter:: close()
{
	if ( !isOpen() )
		return false;
	
	int result = fclose( stream );
	
	// if the result is not 0, then there was an error in closing the file.
	if ( result != 0 )
		return false;
	
	stream = NULL;
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Flush Method
//############		
//##########################################################################################
//##########################################################################################




void FileWriter:: flush()
{
	if ( !fileExists() )
		return;
	
	if ( !isOpen() )
		return;
	
	fflush( stream );
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Erase Method
//############		
//##########################################################################################
//##########################################################################################




Bool FileWriter:: erase()
{
	if ( isOpen() )
	{
		close();
		stream = std::fopen( (const char*)file.getPathString().getCString(), "wb+" );
		
		return stream != NULL;
	}
	else
		return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Seek Methods
//############		
//##########################################################################################
//##########################################################################################




Bool FileWriter:: canSeek() const
{
	return this->isOpen();
}




Bool FileWriter:: canSeek( Int64 relativeOffset ) const
{
	if ( !this->isOpen() )
		return false;
	
	if ( relativeOffset > 0 )
		return this->getPosition() + relativeOffset < file.getSize();
	else if ( relativeOffset < 0 )
		return this->getPosition() > LargeIndex(-relativeOffset);
	else
		return true;
}




Int64 FileWriter:: seek( Int64 byteOffset )
{
	if ( !this->isOpen() )
		return 0;
	
	if ( fseek( stream, (long)byteOffset, SEEK_CUR ) == 0 )
		return byteOffset;
	else
		return 0;
}




LargeIndex FileWriter:: seekAbsolute( LargeIndex newFilePosition )
{
	if ( this->isOpen() )
		fseek( stream, (long)newFilePosition, SEEK_SET );
	
	return ftell( stream );
}




Bool FileWriter:: seekStart()
{
	if ( !this->isOpen() )
		return false;
	
	return fseek( stream, 0L, SEEK_SET ) == 0;
}




Bool FileWriter:: seekEnd()
{
	if ( !this->isOpen() )
		return false;
	
	return fseek( stream, 0L, SEEK_END ) == 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeIndex FileWriter:: getPosition() const
{
	if ( !isOpen() )
		return 0;
	
	return (LargeIndex)ftell( stream );
}




Bool FileWriter:: isAtEndOfFile() const
{
	if ( !isOpen() )
		return 0;
	
	return !(feof( stream ) == 0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Read Methods Overridden from StringInputStream/DataInputStream
//############		
//##########################################################################################
//##########################################################################################




Size FileWriter:: writeChars( const Char* characters, Size number )
{
	if ( !isOpen() )
		return 0;
	
	return (Size)fwrite( characters, sizeof(Char), number, stream );
}




Size FileWriter:: writeUTF8Chars( const UTF8Char* characters, Size number )
{
	if ( !isOpen() )
		return 0;
	
	return (Size)fwrite( characters, sizeof(UTF8Char), number, stream );
}




Size FileWriter:: writeUTF16Chars( const UTF16Char* characters, Size number )
{
	if ( !isOpen() )
		return 0;
	
	return (Size)fwrite( characters, sizeof(UTF16Char), number, stream );
}




Size FileWriter:: writeUTF32Chars( const UTF32Char* characters, Size number )
{
	if ( !isOpen() )
		return 0;
	
	return (Size)fwrite( characters, sizeof(UTF32Char), number, stream );
}




Size FileWriter:: writeData( const UByte* buffer, Size numBytes )
{
	if ( !isOpen() )
		return 0;
	
	return (Size)fwrite( buffer, sizeof(UByte), numBytes, stream );
}




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
