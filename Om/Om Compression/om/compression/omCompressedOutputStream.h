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

#ifndef INCLUDE_OM_COMPRESSED_OUTPUT_STREAM_H
#define INCLUDE_OM_COMPRESSED_OUTPUT_STREAM_H


#include "omCompressionConfig.h"


#include "omCompressionSettings.h"


//##########################################################################################
//**************************  Start Om Compression Namespace  ******************************
OM_COMPRESSION_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encodes a write-only stream of compressed data.
class CompressedOutputStream : public DataOutputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new compressed output stream that uses the specified data output stream as its destination.
			/**
			  * The default compression settings are used.
			  */
			CompressedOutputStream( DataOutputStream* newOutputStream );
			
			
			/// Create a new compressed output stream that uses the specified data output stream as its destination.
			CompressedOutputStream( DataOutputStream* newOutputStream, const CompressionSettings& settings );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an output stream and free all of its resources (close it).
			~CompressedOutputStream();
			
			
		//********************************************************************************
		//******	Compression Start/Stop Methods
			
			
			/// Start compressing output data.
			Bool start();
			
			
			/// Stop compressing output data, flush buffered data, and finish the compression stream.
			Bool stop();
			
			
		//********************************************************************************
		//******	Data Write Methods
			
			
			/// Write the specified number of bytes of data from the buffer to the stream.
			virtual Size writeData( const UByte* data, Size number );
			
			
		//********************************************************************************
		//******	Flush Methods
			
			
			/// Flush buffered data to the output stream and compress it.
			virtual void flush();
			
			
			/// Flush buffered data to the output stream and restart the compression process.
			/**
			  * This allows decompression to be restarted from the current compressed position.
			  * This can allow random access to block-compressed files.
			  */
			void restart();
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not this type of stream allows seeking.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			virtual Bool canSeek( Int64 relativeOffset ) const;
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			virtual Int64 seek( Int64 relativeOffset );
			
			
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			/// Return the absolute position in bytes in the uncompressed stream of data.
			virtual LargeIndex getPosition() const;
			
			
			/// Return the current position in the compressed output data stream.
			LargeIndex getCompressedPosition() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			///  A class that wraps internal library-specific data.
			class Wrapper;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Declared private to prevent copying.
			OM_INLINE CompressedOutputStream( const CompressedOutputStream& other )
			{
			}
			
			
			/// Declared private to prevent copying.
			OM_INLINE CompressedOutputStream& operator = ( const CompressedOutputStream& other )
			{
				return *this;
			}
			
			
			/// Initialize the compression stream.
			Bool openStream();
			
			
			/// Cleanup the compression stream.
			Bool closeStream();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to an object that wraps internal platform-specific state.
			Wrapper* wrapper;
			
			
			/// A pointer to the data output stream where the compressed output of this stream is written.
			DataOutputStream* outputStream;
			
			
			/// The compression settings for this stream.
			CompressionSettings settings;
			
			
			/// The uncompressed stream position;
			LargeIndex streamPosition;
			
			
			/// A boolean value indicating whether or not the compressed output stream is initialized properly.
			Bool open;
			
			
			
};




//##########################################################################################
//**************************  End Om Compression Namespace  ********************************
OM_COMPRESSION_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_COMPRESSED_OUTPUT_STREAM_H
