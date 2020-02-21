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

#include "omCompressedOutputStream.h"


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




class CompressedOutputStream:: Wrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new compressed output stream wrapper.
			OM_INLINE Wrapper()
				:	numInputBytes( 0 )
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
			
			
			/// The zlib stream that is used to compress data.
			z_stream stream;
			
			
			/// The input buffer for zlib compression, where uncompressed data is written.
			PODArray<UByte> inputBuffer;
			
			
			/// The output buffer for zlib compression, where compressed data is buffered.
			PODArray<UByte> outputBuffer;
			
			
			/// The number of input bytes that are currently in the input buffer.
			Size numInputBytes;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




CompressedOutputStream:: CompressedOutputStream( DataOutputStream* newOutputStream )
	:	wrapper( NULL ),
		outputStream( newOutputStream ),
		settings(),
		open( false )
{
}




CompressedOutputStream:: CompressedOutputStream( DataOutputStream* newOutputStream, const CompressionSettings& newSettings )
	:	wrapper( NULL ),
		outputStream( newOutputStream ),
		settings( newSettings ),
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




CompressedOutputStream:: ~CompressedOutputStream()
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




Bool CompressedOutputStream:: start()
{
	if ( open )
		return false;
	
	return this->openStream();
}




Bool CompressedOutputStream:: stop()
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




Size CompressedOutputStream:: writeData( const UByte* data, Size number )
{
	// If the compression stream is not open, just write directly to the output stream.
	if ( !open )
	{
		if ( outputStream != NULL )
			return outputStream->writeData( data, number );
		
		return 0;
	}
	
	// Write data until the end of the input buffer.
	Size numWritten = 0;
	
	while ( numWritten < number )
	{
		Size numToWrite = math::min( wrapper->inputBuffer.getSize() - wrapper->numInputBytes, number - numWritten );
		
		// Copy the data to the input buffer.
		util::copyPOD( &wrapper->inputBuffer[wrapper->numInputBytes], data + numWritten, numToWrite );
		wrapper->numInputBytes += numToWrite;
		numWritten += numToWrite;
		
		// Check to see if the input buffer is full.
		if ( wrapper->numInputBytes == wrapper->inputBuffer.getSize() )
		{
			// Compress the input data, then write it to the output data stream.
			// Setup the zlib stream.
			wrapper->stream.avail_in = (uInt)wrapper->numInputBytes;
			wrapper->stream.next_in = wrapper->inputBuffer.getPointer();
			
			// Compress the input data until it is consumed and written to the output.
			do
			{
				wrapper->stream.avail_out = (uInt)wrapper->outputBuffer.getSize();
				wrapper->stream.next_out = wrapper->outputBuffer.getPointer();
				
				// Compress the data.
				int result = deflate( &wrapper->stream, Z_NO_FLUSH );
				
				// If there was an error, abort.
				if ( result != Z_OK && result != Z_STREAM_END )
				{
					streamPosition += numWritten;
					return numWritten;
				}
				
				// Write the compressed data to the output stream.
				const Size compressedSize = wrapper->outputBuffer.getSize() - wrapper->stream.avail_out;
				outputStream->writeData( wrapper->outputBuffer.getPointer(), compressedSize );
			}
			while ( wrapper->stream.avail_out == 0 );
			
			// Reset the number of input bytes.
			wrapper->numInputBytes = 0;
		}
	}
	
	streamPosition += numWritten;
	
	return numWritten;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Flush Methods
//############		
//##########################################################################################
//##########################################################################################




void CompressedOutputStream:: flush()
{
	if ( !open )
	{
		if ( outputStream != NULL )
			outputStream->flush();
		
		return;
	}
	
	// Compress the input data, then write it to the output data stream.
	// Setup the zlib stream.
	wrapper->stream.avail_in = (uInt)wrapper->numInputBytes;
	wrapper->stream.next_in = wrapper->inputBuffer.getPointer();
	
	// Compress the input data until it is consumed and written to the output.
	do
	{
		wrapper->stream.avail_out = (uInt)wrapper->outputBuffer.getSize();
		wrapper->stream.next_out = wrapper->outputBuffer.getPointer();
		
		// Flush the compressed data so that compression can be restarted from this point.
		int result = deflate( &wrapper->stream, Z_SYNC_FLUSH );
		
		// If there was an error, abort.
		if ( result != Z_OK )
			return;
		
		// Write the compressed data to the output stream.
		const Size compressedSize = wrapper->outputBuffer.getSize() - wrapper->stream.avail_out;
		outputStream->writeData( wrapper->outputBuffer.getPointer(), compressedSize );
	}
	while ( wrapper->stream.avail_out == 0 );
	
	// Reset the number of input bytes.
	wrapper->numInputBytes = 0;
	
	outputStream->flush();
}




void CompressedOutputStream:: restart()
{
	if ( !open )
		return;
	
	// Compress the input data, then write it to the output data stream.
	// Setup the zlib stream.
	wrapper->stream.avail_in = (uInt)wrapper->numInputBytes;
	wrapper->stream.next_in = wrapper->inputBuffer.getPointer();
	
	// Compress the input data until it is consumed and written to the output.
	do
	{
		wrapper->stream.avail_out = (uInt)wrapper->outputBuffer.getSize();
		wrapper->stream.next_out = wrapper->outputBuffer.getPointer();
		
		// Flush the compressed data so that compression can be restarted from this point.
		int result = deflate( &wrapper->stream, Z_FULL_FLUSH );
		
		// If there was an error, abort.
		if ( result != Z_OK )
			return;
		
		// Write the compressed data to the output stream.
		const Size compressedSize = wrapper->outputBuffer.getSize() - wrapper->stream.avail_out;
		outputStream->writeData( wrapper->outputBuffer.getPointer(), compressedSize );
	}
	while ( wrapper->stream.avail_out == 0 );
	
	// Reset the number of input bytes.
	wrapper->numInputBytes = 0;
	
	outputStream->flush();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seeking Methods
//############		
//##########################################################################################
//##########################################################################################




Bool CompressedOutputStream:: canSeek() const
{
	return false;
}




/// Return whether or not this stream can seek by the specified amount in bytes.
Bool CompressedOutputStream:: canSeek( Int64 relativeOffset ) const
{
	return false;
}




/// Move the current position in the stream by the specified relative signed offset in bytes.
Int64 CompressedOutputStream:: seek( Int64 relativeOffset )
{
	return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Position Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




LargeIndex CompressedOutputStream:: getPosition() const
{
	return streamPosition;
}




LargeIndex CompressedOutputStream:: getCompressedPosition() const
{
	if ( outputStream == NULL )
		return 0;
	
	return outputStream->getPosition();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Open/Close Methods
//############		
//##########################################################################################
//##########################################################################################




Bool CompressedOutputStream:: openStream()
{
	if ( open || outputStream == NULL )
		return false;
	
	// Create the wrapper object if it has not yet been created.
	if ( wrapper == NULL )
		wrapper = util::construct<Wrapper>();
	
	// Initialize the zlib stream.
	int result = deflateInit( &wrapper->stream, math::clamp( (int)math::round(settings.getCompression()*9.0f), 0, 9 ) );
	
	// Make sure the initialization was successful.
	if ( result != Z_OK )
		return false;
	
	// Initialize the input/output buffers.
	wrapper->inputBuffer.setSize( BUFFER_SIZE );
	wrapper->outputBuffer.setSize( BUFFER_SIZE );
	wrapper->inputBuffer.allocate();
	wrapper->outputBuffer.allocate();
	wrapper->numInputBytes = 0;
	streamPosition = outputStream->getPosition();
	
	// Remember that the stream is open.
	open = true;
	
	return true;
}




Bool CompressedOutputStream:: closeStream()
{
	if ( !open )
		return false;
	
	// Flush the remaining data to the stream.
	CompressedOutputStream::flush();
	
	// Clean up the stream.
	deflateEnd( &wrapper->stream );
	
	// Remember that the stream is closed.
	open = false;
	
	return true;
}




//##########################################################################################
//**************************  End Om Compression Namespace  ********************************
OM_COMPRESSION_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
