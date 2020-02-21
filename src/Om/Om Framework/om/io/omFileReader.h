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

#ifndef INCLUDE_OM_FILE_READER_H
#define INCLUDE_OM_FILE_READER_H


#include "omIOConfig.h"


#include "../omFileSystem.h"


#include "omDataInputStream.h"
#include "omStringInputStream.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that allows the user to easily read from a file.
/** 
  * This purpose of this class is to read from a file in an
  * object oriented and flexible manner. It allows the user
  * to read individual bytes (characters), a sequence of characters,
  * and raw data. One can open and close the file reader, and
  * manipulate it's position in the file by seeking an absolute
  * position or moving relatively. It wraps C's standard file in/out.
  */
class FileReader : public DataInputStream, public StringInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a FileReader object that should read from the file at the specified path string.
			FileReader( const Char* filePath );
			
			
			/// Create a FileReader object that should read from the file at the specified path string.
			FileReader( const fs::UTF8String& filePath );
			
			
			/// Create a FileReader object that should read from the file at the specified path.
			FileReader( const fs::Path& filePath );
			
			
			/// Create a FileReader object that should read from the specified file.
			FileReader( const fs::File& file );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a file reader and free all of it's resources (close the file).
			OM_INLINE ~FileReader()
			{
				if ( isOpen() )
					close();
			}
			
			
		//********************************************************************************
		//******	File Reader Open/Close Methods
			
			
			/// Open the file reader, allocating whatever resources needed to do so.
			/** 
			  * This method opens a stream for reading from a file, and
			  * allocates the necessary resources to do so. If the file is already
			  * open, then this method does nothing. If an error occurs during
			  * opening the file reader, such that the file is unable to be opened, then
			  * a IOException is thrown. If the file path specified in the constructor
			  * points to a file that does not exist, then a FileNotFoundException is thrown.
			  */
			Bool open();
			
			
			/// Return whether or not the file reader's file is open.
			/** 
			  * This method gets a boolean value from the file reader indicating
			  * whether or not the file is currently open. If the file is open,
			  * then TRUE is returned, and if it is closed, FALSE is returned.
			  * 
			  * @return whether or not the file reader is open
			  */
			OM_INLINE Bool isOpen() const
			{
				return stream != NULL;
			}
			
			
			/// Close the file writer, freeing all resources used during reading.
			/**
			  * This method closes the file reader, and ensures that all resources
			  * that it used to perform output are freed (such as files, etc.).
			  * If the file reader is currently open, then this method guarantees that
			  * the reader is closed. If the file is unable to be closed, the method returns
			  * FALSE. If the file reader is already closed when the method is called,
			  * then nothing is done. This method is automatically called by the class's
			  * destructor when a file reader is destroyed.
			  */
			Bool close();
			
			
		//********************************************************************************
		//******	Seek/Move Methods
			
			
			/// Return whether or not this file reader can seek within the file.
			/**
			  * This method returns TRUE if the file reader is opened and points to
			  * a valid file. Otherwise FALSE is returned.
			  */
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this reader can seek by the specified amount in bytes.
			/**
			  * Since some streams may not support rewinding, this method can be used
			  * to determine if a given seek operation can succeed. The method can also
			  * be used to determine if the end of a stream has been reached, a seek past
			  * the end of a file will fail.
			  */
			virtual Bool canSeek( Int64 relativeOffset ) const;
			
			
			/// Move the current position in the file by the specified relative signed offset in bytes.
			/**
			  * The method attempts to seek in the file by the specified amount and
			  * returns the signed amount that the position in the file was changed by
			  * in bytes. A negative offset indicates that the position should be moved in
			  * reverse and a positive offset indicates that the position should be moved
			  * forwards. The file must be open for the seek operation to succeed.
			  */
			virtual Int64 seek( Int64 relativeOffset );
			
			
			/// Seek to an absolute position in the file.
			/** 
			  * This method attempts to seek to the specified absolute
			  * position in the file, and then returns the resulting
			  * position in the file of the file reader after the method call.
			  * Positions within a file are specified with 0 representing 
			  * the beginning of the file, and each positive increment 
			  * of 1 representing a position 1 more byte
			  * further in the file. If the file is not open when the method
			  * is called, no seek operation is performed and the current position in
			  * the file is returned.
			  * 
			  * @param newFilePosition - The desired position in the file to seek to.
			  * @return the resulting position in the file after the method call.
			  */
			LargeIndex seekAbsolute( LargeIndex newFilePosition );
			
			
			/// Rewind the file pointer to the beginning of the file.
			/** 
			  * This method moves the position in the file of the file reader
			  * to the beginning of the file. The method returns whether or not
			  * the seek operation was successful. The seek operation can fail
			  * if seeking is not allowed or the file is not open.
			  */
			Bool seekStart();
			
			
			/// Seek to the end of the file.
			/** 
			  * This method sets the current position in the file of the file
			  * reader to be the end of the file. The method returns whether or not
			  * the seek operation was successful. The seek operation can fail
			  * if seeking is not allowed or the file is not open.
			  */
			Bool seekEnd();
			
			
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			/// Get the absolute position in the file of the file reader.
			/** 
			  * This method queries and returns the current position in
			  * the file of the file reader. Positions within a file
			  * are specified with 0 representing the beginning of the file, 
			  * and each positive increment of 1 representing a position 1 more byte
			  * further in the file. If the file is not open when the method
			  * is called, then a IOException is thrown.
			  * 
			  * @return the current position in the file of the file reader.
			  */
			virtual LargeIndex getPosition() const;
			
			
			/// Get whether or not the file writer is at the end of the file.
			/**
			  * This method queries whether or not the file writer is at the
			  * end of the file. If it is, then TRUE is returned, otherwise
			  * FALSE is returned. If the file is not open when the method is
			  * called, then a IOException is thrown.
			  * 
			  * @return whether or not the file reader is at the end of the file.
			  */
			Bool isAtEndOfFile() const;
			
			
		//********************************************************************************
		//******	File Attribute Accessor Methods
			
			
			/// Get the file object that this file reader is reading from.
			/** 
			  * This method returns a constant reference to a file object representing
			  * the file that this file reader is associated with.
			  * 
			  * @return the file this file reader is associated with.
			  */
			OM_INLINE const fs::File& getFile() const
			{
				return file;
			}
			
			
			/// Get the path to the file that this file reader is reading.
			/** 
			  * This method returns a constant reference to a string representing
			  * the path to the file that this file reader is associated with.
			  * 
			  * @return the path to the file this file reader is associated with.
			  */
			OM_INLINE const fs::Path& getURL() const
			{
				return file.getPath();
			}
			
			
			/// Get the size of the file in bytes.
			/** 
			  * This method queries and returns the size of the file
			  * in bytes. The file does not have to be open to do this, but
			  * it does have to exist. If the file does not exist, then
			  * a IOException is thrown.
			  * 
			  * @return the total size of the file in bytes.
			  */
			OM_INLINE LargeSize getFileSize() const
			{
				return file.getSize();
			}
			
			
			/// Get whether or not the file associated with this reader exists.
			/** 
			  * This method checks whether or not the file pointed to by the
			  * path queried by getURL() exists. It then returns TRUE
			  * if the file exists or returns FALSE if the file does not exist.
			  * 
			  * @return whether or not the file associated with this file reader exists.
			  */
			OM_INLINE Bool fileExists() const
			{
				return file.exists();
			}
			
			
		//********************************************************************************
		//******	Remaining Data Size Accessor Methods
			
			
			/// Return the number of bytes remaining in the file.
			/**
			  * The value returned must only be a lower bound on the number of bytes
			  * remaining in the stream. If there are bytes remaining, it must return
			  * at least 1.
			  */
			virtual LargeSize getBytesRemaining() const;
			
			
			/// Return the number of characters remaining in the stream.
			/**
			  * The value returned must only be a lower bound on the number of characters
			  * remaining in the stream. If there are characters remaining, it must return
			  * at least 1.
			  */
			virtual LargeSize getCharactersRemaining() const;
			
			
	protected:
			
		//********************************************************************************
		//******	Protected Read Methods (Declared in StringInputStream/DataInputStream)
			
			
			virtual Size readChars( Char* buffer, Size numChars );
			
			
			/// Read the specified number of UTF-8 characters from the stream and place them in the specified output buffer.
			/**
			  * If the number of unicode code points exceeds the capacity of the buffer, as many characters
			  * are read as possible. The number of code points read is returned.
			  */
			virtual Size readUTF8Chars( UTF8Char* buffer, Size numChars, Size capacity );
			
			
			/// Read the specified number of UTF-16 characters from the stream and place them in the specified output buffer.
			/**
			  * If the number of unicode code points exceeds the capacity of the buffer, as many characters
			  * are read as possible. The number of code points read is returned.
			  */
			virtual Size readUTF16Chars( UTF16Char* buffer, Size numChars, Size capacity );
			
			
			/// Read the specified number of UTF-32 characters from the stream and place them in the specified output buffer.
			/**
			  * If the number of unicode code points exceeds the capacity of the buffer, as many characters
			  * are read as possible. The number of code points read is returned.
			  */
			virtual Size readUTF32Chars( UTF32Char* buffer, Size numChars );
			
			
			virtual Size readData( UByte* buffer, Size numBytes );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A file object representing the file we are reading from.
			fs::File file;
			
			
			/// The C standard file stream for the file the reader is reading from.
			std::FILE* stream;
			
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FILE_READER_H
