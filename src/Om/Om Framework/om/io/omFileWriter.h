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

#ifndef INCLUDE_OM_FILE_WRITER_H
#define INCLUDE_OM_FILE_WRITER_H


#include "omIOConfig.h"


#include "../omFileSystem.h"


#include "omDataOutputStream.h"
#include "omStringOutputStream.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that allows the user to easily write to a file.
/** 
  * This purpose of this class is to write to a file in an
  * object oriented and flexible manner. It allows the user
  * to write individual bytes (characters), a sequence of characters,
  * and raw data. One can open and close the file writer, and
  * manipulate it's position in the file by seeking an absolute
  * position or moving relatively. This class can also create a file
  * if it does not initially exist when the file writer is instantiated.
  * It wraps C's standard file in/out.
  */
class FileWriter : public DataOutputStream, public StringOutputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a FileWriter object that should write to the file at the specified path string.
			FileWriter( const Char* filePath );
			
			
			/// Create a FileWriter object that should write to the file at the specified path string.
			FileWriter( const fs::UTF8String& filePath );
			
			
			/// Create a FileWriter object that should write to the file at the specified path.
			FileWriter( const fs::Path& filePath );
			
			
			/// Create a FileWriter object that should write to the specified file.
			FileWriter( const fs::File& file );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a file reader and free all of it's resources (close the file).
			~FileWriter();
			
			
		//********************************************************************************
		//******	File Attribute Accessor Methods
			
			
			/// Get the file object that this file writer is reading from.
			/** 
			  * This method returns a constant reference to a file object representing
			  * the file that this file writer is associated with.
			  * 
			  * @return the file this file writer is associated with.
			  */
			OM_INLINE const fs::File& getFile() const
			{
				return file;
			}
			
			
			/// Get the path to the file that this file writer is reading.
			/** 
			  * This method returns a constant reference to a string representing
			  * the path to the file that this file writer is associated with.
			  * 
			  * @return the path to the file this file writer is associated with.
			  */
			OM_INLINE const fs::Path& getURL() const
			{
				return file.getPath();
			}
			
			
			/// Get the size of the file in bytes.
			/** 
			  * This method queries and returns the size of the file
			  * in bytes. The file does not have to be open to do this, but
			  * it does have to exist. If the file does not exist, the method returns
			  * a size of 0.
			  * 
			  * @return the total size of the file in bytes.
			  */
			OM_INLINE LargeSize getFileSize() const
			{
				return file.getSize();
			}
			
			
			/// Get whether or not the file associated with this writer exists.
			/** 
			  * This method checks whether or not the file pointed to by the
			  * path queried by getURL() exists. It then returns TRUE
			  * if the file exists or returns FALSE if the file does not exist.
			  * 
			  * @return whether or not the file associated with this file writer exists.
			  */
			OM_INLINE Bool fileExists() const
			{
				return file.exists();
			}
			
			
		//********************************************************************************
		//******	File Writer Open/Close Methods
			
			
			/// Open the file writer, allocating whatever resources needed to do so.
			Bool open();
			
			
			/// Close the file writer, freeing all resources used during writing.
			/**
			  * This method closes the file writer, and ensures that all resources
			  * that it used to perform output are freed (such as files, etc.).
			  * If the file writer is currently open, then this method guarantees that
			  * the writer is closed. If the file was not able to be closed or
			  * was not already open, the method returns FALSE indicating the error.
			  * Otherwise, the method returns TRUE if it succeeds.
			  */
			Bool close();
			
			
			/// Return whether or not the file writer's file is open.
			/** 
			  * This method gets a boolean value from the file writer indicating
			  * whether or not the file is currently open. If the file is open,
			  * then TRUE is returned, and if it is closed, FALSE is returned.
			  * 
			  * @return whether or not the file writer is open
			  */
			OM_INLINE Bool isOpen() const
			{
				return stream != NULL;
			}
			
			
		//********************************************************************************
		//******	File Writing Methods
			
			
			/// Write the specified number of bytes of data from the buffer to the stream.
			virtual Size writeData( const UByte* data, Size number );
			
			
		//********************************************************************************
		//******	File Writer Flush Method
			
			
			/// Flush the file stream, sending all internally buffered output to the file.
			/** 
			  * This method causes all currently pending output data to be sent to
			  * the file. This method ensures that this is done and that all internal
			  * data buffers are emptied if they have any contents. If this method is called
			  * when the file is not open, then a IOException is thrown indicating
			  * this mistake.
			  */
			virtual void flush();
			
			
		//********************************************************************************
		//******	File Erase Methods
			
			
			/// Erase the file associated with this file writer.
			/**
			  * This method erases the entire contents of the file being written
			  * and resets the current write pointer to the begining of the file.
			  * The method returns whether or not the erase operation was successful.
			  * Erasing a file can fail if the file is not open or if the file is not
			  * valid.
			  */
			Bool erase();
			
			
		//********************************************************************************
		//******	Seek/Move Methods
			
			
			/// Return whether or not this file writer allows seeking.
			/**
			  * If the file that is being written is open and valid, this
			  * method returns TRUE. Otherwise, the method returns false indicating
			  * that seeking is not allowed.
			  */
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			/**
			  * Since some streams may not support rewinding, this method can be used
			  * to determine if a given seek operation can succeed. The method can also
			  * be used to determine if the end of a stream has been reached, a seek past
			  * the end of a file will fail.
			  */
			virtual Bool canSeek( Int64 relativeOffset ) const;
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			/**
			  * The method attempts to seek in the stream by the specified amount and
			  * returns the signed amount that the position in the stream was changed by
			  * in bytes. A negative offset indicates that the position should be moved in
			  * reverse and a positive offset indicates that the position should be moved
			  * forwards.
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
			  * This method moves the position in the file of the file writer
			  * to the beginning of the file. The method returns whether or not
			  * the seek operation was successful. The seek operation can fail
			  * if seeking is not allowed or the file is not open.
			  */
			Bool seekStart();
			
			
			/// Seek to the end of the file.
			/** 
			  * This method sets the current position in the file of the file
			  * writer to be the end of the file. The method returns whether or not
			  * the seek operation was successful. The seek operation can fail
			  * if seeking is not allowed or if the file is not open.
			  */
			Bool seekEnd();
			
			
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			/// Get the absolute position in the file of the file writer in bytes.
			/** 
			  * This method queries and returns the current position in
			  * the file of the file writer. Positions within a file
			  * are specified with 0 representing the beginning of the file, 
			  * and each positive increment of 1 representing a position 1 more byte
			  * further in the file. If the file is not open when the method
			  * is called, then a IOException is thrown.
			  * 
			  * @return the current position in the file of the file writer.
			  */
			virtual LargeIndex getPosition() const;
			
			
			/// Get whether or not the file writer is at the end of the file.
			/**
			  * This method queries whether or not the file writer is at the
			  * end of the file. If it is, then TRUE is returned, otherwise
			  * FALSE is returned. If the file is not open when the method is
			  * called, then a IOException is thrown.
			  * 
			  * @return whether or not the file writer is at the end of the file.
			  */
			Bool isAtEndOfFile() const;
			
			
	protected:
			
		//********************************************************************************
		//******	Protected Write Methods (Declared in String/DataOutputStream)
			
			
			/// Write the specified number of characters from the character buffer and return the number written.
			virtual Size writeChars( const Char* characters, Size number );
			
			
			/// Write the specified number of UTF-8 characters from the character buffer and return the number written.
			virtual Size writeUTF8Chars( const UTF8Char* characters, Size number );
			
			
			/// Write the specified number of UTF-16 characters from the character buffer and return the number written.
			virtual Size writeUTF16Chars( const UTF16Char* characters, Size number );
			
			
			/// Write the specified number of UTF-32 characters from the character buffer and return the number written.
			virtual Size writeUTF32Chars( const UTF32Char* characters, Size number );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The C standard stream object for the file the writer is writing to.
			std::FILE* stream;
			
			
			/// A file object representing the file we are writing to.
			fs::File file;
			
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FILE_WRITER_H
