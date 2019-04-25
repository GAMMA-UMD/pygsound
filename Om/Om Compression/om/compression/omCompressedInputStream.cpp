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

#include "omCompressedInputStream.h"


#include "zlib.h"


#define BUFFER_SIZE 32768


//##########################################################################################
//**************************  Start Om Compression Namespace  ******************************
OM_COMPRESSION_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Stream Wrapper Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class CompressedInputStream:: Wrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new compressed output stream wrapper.
			OM_INLINE Wrapper()
				:	numInputBytes( 0 ),
					numOutputBytes( 0 ),
					outputStart( 0 )
			{
				stream.zalloc = Z_NULL;
				stream.zfree = Z_NULL;
				stream.opaque = Z_NULL;
				stream.avail_in = 0;
				stream.next_in = Z_NULL;
				stream.avail_out = 0;
				stream.next_out = Z_NULL;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The zlib stream that is used to decompress data.
			z_stream stream;
			
			
			/// The input buffer for zlib decompression, where compressed data is written.
			PODArray<UByte> inputBuffer;
			
			
			/// The output buffer for zlib decompression, where uncompressed data is buffered.
			PODArray<UByte> outputBuffer;
			
			
			/// The number of input bytes that are currently in the input buffer.
			Size numInputBytes;
			
			
			/// The number of output bytes that are currently in the output buffer.
			Size numOutputBytes;
			
			
			/// The start offset in bytes where the next output byte starts.
			Size outputStart;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




CompressedInputStream:: CompressedInputStream( DataInputStream* newInputStream )
	:	wrapper( NULL ),
		inputStream( newInputStream ),
		open( false )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




CompressedInputStream:: ~CompressedInputStream()
{
	if ( open )
		closeStream();
	
	if ( wrapper )
		util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Compression Start/Stop Methods
//############		
//##########################################################################################
//##########################################################################################




Bool CompressedInputStream:: start()
{
	if ( open )
		return false;
	
	return this->openStream();
}




Bool CompressedInputStream:: stop()
{
	return this->closeStream();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Data Write Methods
//############		
//##########################################################################################
//##########################################################################################




Size CompressedInputStream:: readData( UByte* data, Size number )
{
	// If the compression stream is not open, just write directly to the output stream.
	if ( !open )
	{
		if ( inputStream != NULL )
			return inputStream->readData( data, number );
		
		return 0;
	}
	
	// Read data until the end of the output buffer.
	Size numRead = 0;
	
	while ( numRead < number )
	{
		// See if we need to decompress more output.
		if ( wrapper->numOutputBytes == 0 )
		{
			// See if we need to read more compressed input for zlib.
			if ( wrapper->numInputBytes == 0 )
			{
				// Read compressed data into the input buffer.
				wrapper->numInputBytes = inputStream->readData( wrapper->inputBuffer.getPointer(),
																wrapper->inputBuffer.getSize() );
				
				// Update the zlib input state.
				wrapper->stream.avail_in = (uInt)wrapper->numInputBytes;
				wrapper->stream.next_in = wrapper->inputBuffer.getPointer();
			}
			
			// Update the zlib output state.
			wrapper->stream.avail_out = (uInt)wrapper->outputBuffer.getSize();
			wrapper->stream.next_out = wrapper->outputBuffer.getPointer();
			
			// Decompress the data.
			int result = inflate( &wrapper->stream, Z_NO_FLUSH );
			
			// If there was an error, abort.
			if ( result != Z_OK )
				return numRead;
			
			wrapper->numInputBytes = wrapper->stream.avail_in;
			wrapper->numOutputBytes = wrapper->outputBuffer.getSize() - wrapper->stream.avail_out;
			wrapper->outputStart = 0;
		}
		
		// Determine how many bytes to read from the zlib output buffer.
		const Size numToRead = math::min( wrapper->numOutputBytes, number - numRead );
		
		// Copy the data to the output.
		util::copyPOD( data + numRead, wrapper->outputBuffer.getPointer() + wrapper->outputStart, numToRead );
		
		// Update the output state.
		wrapper->outputStart += numToRead;
		wrapper->numOutputBytes -= numToRead;
		numRead += numToRead;
		streamPosition += numToRead;
	}
	
	return numRead;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seeking Methods
//############		
//##########################################################################################
//##########################################################################################




Bool CompressedInputStream:: canSeek() const
{
	if ( inputStream == NULL )
		return false;
	
	if ( !open )
		return inputStream->canSeek();
	
	return true;
}




Bool CompressedInputStream:: canSeek( Int64 relativeOffset ) const
{
	if ( inputStream == NULL )
		return false;
	
	if ( !open )
		return inputStream->canSeek( relativeOffset );
	
	if ( relativeOffset < 0 )
		return false;
	else
		return true;
}




Int64 CompressedInputStream:: seek( Int64 relativeOffset )
{
	if ( inputStream == NULL )
		return 0;
	
	if ( !open )
		return inputStream->seek( relativeOffset );
	
	if ( relativeOffset < 0 )
		return 0;
	else
	{
		Size numSkipped = 0;
		Size totalToSkip = (Size)relativeOffset;
		
		while ( numSkipped < totalToSkip )
		{
			// See if we need to decompress more output.
			if ( wrapper->numOutputBytes == 0 )
			{
				// See if we need to read more compressed input for zlib.
				if ( wrapper->numInputBytes == 0 )
				{
					// Read compressed data into the input buffer.
					wrapper->numInputBytes = inputStream->readData( wrapper->inputBuffer.getPointer(),
																	wrapper->inputBuffer.getSize() );
					
					// Update the zlib input state.
					wrapper->stream.avail_in = (uInt)wrapper->numInputBytes;
					wrapper->stream.next_in = wrapper->inputBuffer.getPointer();
				}
				
				// Update the zlib output state.
				wrapper->stream.avail_out = (uInt)wrapper->outputBuffer.getSize();
				wrapper->stream.next_out = wrapper->outputBuffer.getPointer();
				
				// Decompress the data.
				int result = inflate( &wrapper->stream, Z_NO_FLUSH );
				
				// If there was an error, abort.
				if ( result != Z_OK )
					return numSkipped;
				
				wrapper->numInputBytes = wrapper->stream.avail_in;
				wrapper->numOutputBytes = wrapper->outputBuffer.getSize() - wrapper->stream.avail_out;
				wrapper->outputStart = 0;
			}
			
			// Determine how many bytes to skip from the zlib output buffer.
			const Size numToSkip = math::min( wrapper->numOutputBytes, totalToSkip - numSkipped );
			
			// Update the output state.
			wrapper->outputStart += numToSkip;
			wrapper->numOutputBytes -= numToSkip;
			numSkipped += numToSkip;
			streamPosition += numToSkip;
		}
		
		return numSkipped;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeSize CompressedInputStream:: getBytesRemaining() const
{
	if ( inputStream == NULL )
		return 0;
	
	return inputStream->getBytesRemaining() + wrapper->numOutputBytes;
}




LargeIndex CompressedInputStream:: getPosition() const
{
	return streamPosition;
}




LargeIndex CompressedInputStream:: getCompressedPosition() const
{
	if ( inputStream == NULL )
		return 0;
	
	return inputStream->getPosition();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Open/Close Methods
//############		
//##########################################################################################
//##########################################################################################




Bool CompressedInputStream:: openStream()
{
	if ( open || inputStream == NULL )
		return false;
	
	// Create the wrapper object if it has not yet been created.
	if ( wrapper == NULL )
		wrapper = util::construct<Wrapper>();
	
	// Initialize the zlib stream.
	int result = inflateInit( &wrapper->stream );
	
	// Make sure the initialization was successful.
	if ( result != Z_OK )
		return false;
	
	// Initialize the input/output buffers.
	wrapper->inputBuffer.setSize( BUFFER_SIZE );
	wrapper->outputBuffer.setSize( BUFFER_SIZE );
	wrapper->inputBuffer.allocate();
	wrapper->outputBuffer.allocate();
	wrapper->numInputBytes = 0;
	streamPosition = inputStream->getPosition();
	
	// Remember that the stream is open.
	open = true;
	
	return true;
}




Bool CompressedInputStream:: closeStream()
{
	if ( !open )
		return false;
	
	// Clean up the stream.
	inflateEnd( &wrapper->stream );
	
	// Remember that the stream is closed.
	open = false;
	
	return true;
}




//##########################################################################################
//**************************  End Om Compression Namespace  ********************************
OM_COMPRESSION_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
