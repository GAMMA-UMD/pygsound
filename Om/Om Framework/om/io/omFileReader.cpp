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

#include "omFileReader.h"


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




FileReader:: FileReader( const Char* filePath )
	:	file( filePath != NULL ? fs::Path( filePath ) : fs::Path() ),
		stream( NULL )
{
}




FileReader:: FileReader( const fs::UTF8String& filePath )
	:	file( fs::Path( filePath ) ),
		stream( NULL )
{
}




FileReader:: FileReader( const fs::Path& filePath )
	:	file( filePath ),
		stream( NULL )
{
}




FileReader:: FileReader( const fs::File& file )
	:	file( file ),
		stream( NULL )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Reader Open/Close Methods
//############		
//##########################################################################################
//##########################################################################################




Bool FileReader:: open()
{
	// Exit the function if the file is already open.
	if ( isOpen() )
		return true;
	
	// If the file doesn't exist, return that opening the file failed.
	if ( !file.exists() )
		return false;
	
	// open the file for binary read-only access.
	stream = std::fopen( (const char*)file.getPathString().getCString(), "rb" );
	
	// if the result is NULL, then there was an error opening the file.
	return stream != NULL;
}




Bool FileReader:: close()
{
	if ( !isOpen() )
		return false;
	
	int result = std::fclose( stream );
	
	// if the result is not 0, then there was an error in closing the file.
	if ( result != 0 )
		return false;
	
	stream = NULL;
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek/Move Methods
//############		
//##########################################################################################
//##########################################################################################




Bool FileReader:: canSeek() const
{
	return this->isOpen();
}




Bool FileReader:: canSeek( Int64 relativeOffset ) const
{
	if ( !this->isOpen() )
		return false;
	
	if ( relativeOffset > 0 )
		return LargeSize(relativeOffset) <= this->getBytesRemaining();
	else if ( relativeOffset < 0 )
		return this->getPosition() > LargeIndex(-relativeOffset);
	else
		return true;
}




Int64 FileReader:: seek( Int64 byteOffset )
{
	if ( !this->isOpen() )
		return 0;
	
	if ( std::fseek( stream, (long)byteOffset, SEEK_CUR ) == 0 )
		return byteOffset;
	else
		return 0;
}




LargeIndex FileReader:: seekAbsolute( LargeIndex newFilePosition )
{
	if ( !this->isOpen() )
		return 0;
	
	std::fseek( stream, (long)newFilePosition, SEEK_SET );
	
	return std::ftell( stream );
}




Bool FileReader:: seekStart()
{
	if ( !this->isOpen() )
		return false;
	
	return fseek( stream, 0L, SEEK_SET ) == 0;
}




Bool FileReader:: seekEnd()
{
	if ( !this->isOpen() )
		return false;
	
	return std::fseek( stream, 0L, SEEK_END ) == 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeIndex FileReader:: getPosition() const
{
	if ( !this->isOpen() )
		return 0;
	
	return (LargeIndex)std::ftell( stream );
}




Bool FileReader:: isAtEndOfFile() const
{
	if ( !this->isOpen() )
		return 0;
	
	return !(std::feof( stream ) == 0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Remaining Data Size Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeSize FileReader:: getBytesRemaining() const
{
	if ( !this->isOpen() )
		return 0;
	
	return LargeSize(file.getSize() - this->getPosition());
}




LargeSize FileReader:: getCharactersRemaining() const
{
	if ( !this->isOpen() )
		return 0;
	
	return LargeSize(file.getSize() - this->getPosition());
}




//##########################################################################################
//##########################################################################################
//############		
//############		File Read Methods Overridden from StringInputStream/DataInputStream
//############		
//##########################################################################################
//##########################################################################################





Size FileReader:: readChars( Char* buffer, Size number )
{
	if ( !this->isOpen() )
		return 0;
	
	return (Size)std::fread( buffer, sizeof(Char), number, stream );
}




Size FileReader:: readUTF8Chars( UTF8Char* buffer, Size numChars, Size capacity )
{
	if ( !this->isOpen() )
		return 0;
	
	return (Size)std::fread( buffer, sizeof(UTF8Char), numChars, stream );
}




Size FileReader:: readUTF16Chars( UTF16Char* buffer, Size numChars, Size capacity )
{
	if ( !this->isOpen() )
		return 0;
	
	return (Size)std::fread( buffer, sizeof(UTF16Char), numChars, stream );
}




Size FileReader:: readUTF32Chars( UTF32Char* buffer, Size numChars )
{
	if ( !this->isOpen() )
		return 0;
	
	return (Size)std::fread( buffer, sizeof(UTF32Char), numChars, stream );
}




Size FileReader:: readData( UByte* buffer, Size numBytes )
{
	if ( !this->isOpen() )
		return 0;
	
	return (Size)std::fread( buffer, sizeof(UByte), numBytes, stream );;
}




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
