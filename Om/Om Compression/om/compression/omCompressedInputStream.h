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

#ifndef INCLUDE_OM_COMPRESSED_INPUT_STREAM_H
#define INCLUDE_OM_COMPRESSED_INPUT_STREAM_H


#include "omCompressionConfig.h"


//##########################################################################################
//**************************  Start Om Compression Namespace  ******************************
OM_COMPRESSION_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that decodes a read-only stream of compressed data.
class CompressedInputStream : public DataInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new compressed input stream that uses the specified data input stream as its destination.
			/**
			  * The default compression settings are used.
			  */
			CompressedInputStream( DataInputStream* newInputStream );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an input stream and free all of its resources (close it).
			~CompressedInputStream();
			
			
		//********************************************************************************
		//******	Compression Start/Stop Methods
			
			
			/// Start uncompressing input data.
			Bool start();
			
			
			/// Stop uncompressing input data and finish the compression stream.
			Bool stop();
			
			
		//********************************************************************************
		//******	Data Write Methods
			
			
			/// Read the specified number of bytes of data from the buffer to the stream.
			virtual Size readData( UByte* data, Size number );
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not this type of stream allows seeking.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			/**
			  * Only forward (positive) seeking is supported when the stream is reading compressed data.
			  */
			virtual Bool canSeek( Int64 relativeOffset ) const;
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			/**
			  * Only forward (positive) seeking is supported when the stream is reading compressed data.
			  */
			virtual Int64 seek( Int64 relativeOffset );
			
			
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			/// Return the number of bytes remaining in the stream.
			/**
			  * The value returned must only be a lower bound on the number of bytes
			  * remaining in the stream. If there are bytes remaining, it must return
			  * at least 1.
			  */
			virtual LargeSize getBytesRemaining() const;
			
			
			/// Return the absolute position in bytes in the uncompressed stream of data.
			virtual LargeIndex getPosition() const;
			
			
			/// Return the current position in the compressed input data stream.
			LargeIndex getCompressedPosition() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			///  A class that wraps internal library-specific data.
			class Wrapper;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Declared private to prevent copying.
			OM_INLINE CompressedInputStream( const CompressedInputStream& other )
			{
			}
			
			
			/// Declared private to prevent copying.
			OM_INLINE CompressedInputStream& operator = ( const CompressedInputStream& other )
			{
				return *this;
			}
			
			
			/// Initialize the decompression stream.
			Bool openStream();
			
			
			/// Cleanup the decompression stream.
			Bool closeStream();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to an object that wraps internal platform-specific state.
			Wrapper* wrapper;
			
			
			/// A pointer to the data input stream where the compressed input of this stream is read.
			DataInputStream* inputStream;
			
			
			/// The uncompressed stream position;
			LargeIndex streamPosition;
			
			
			/// A boolean value indicating whether or not the compressed input stream is initialized properly.
			Bool open;
			
			
			
};




//##########################################################################################
//**************************  End Om Compression Namespace  ********************************
OM_COMPRESSION_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_COMPRESSED_INPUT_STREAM_H
