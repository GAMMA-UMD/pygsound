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

#ifndef INCLUDE_OM_DATA_OUTPUT_STREAM_H
#define INCLUDE_OM_DATA_OUTPUT_STREAM_H


#include "omIOConfig.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class/interface which represents an abstract write-only stream of data.
class DataOutputStream
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an output stream and free all of its resources (close it).
			virtual ~DataOutputStream()
			{
			}
			
			
		//********************************************************************************
		//******	Data Write Methods
			
			
			/// Write as much of the specified data array to the stream and return the number of bytes written.
			OM_INLINE Size writeData( const data::Data& data )
			{
				return this->writeData( data.getPointer(), data.getSize() );
			}
			
			
			/// Write as much of the specified data array to the stream and return the number of bytes written.
			OM_INLINE Size writeData( const data::DataBuffer& dataBuffer )
			{
				return this->writeData( dataBuffer.getPointer(), dataBuffer.getSize() );
			}
			
			
			/// Write the specified number of bytes of data from the buffer to the stream.
			/**
			  * The method returns the number of bytes that were actually written.
			  */
			virtual Size writeData( const UByte* data, Size number ) = 0;
			
			
		//********************************************************************************
		//******	Flush Method
			
			
			/// Flush the output stream, sending all internally buffered output to it's destination.
			/** 
			  * This method causes all currently pending output data to be sent to it's
			  * final destination. This method ensures that this is done and that all internal
			  * data buffers are emptied if they have any contents.
			  */
			virtual void flush() = 0;
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not this type of stream allows seeking.
			/**
			  * Some types of IO (like files) allow seeking, but others, especially those
			  * over networks don't allow streaming. This method allows the user to detect
			  * that situation.
			  */
			virtual Bool canSeek() const = 0;
			
			
			/// Return whether or not this stream can seek by the specified amount in bytes.
			/**
			  * Since some streams may not support rewinding, this method can be used
			  * to determine if a given seek operation can succeed. The method can also
			  * be used to determine if the end of a stream has been reached, a seek past
			  * the end of a file will fail.
			  */
			virtual Bool canSeek( Int64 relativeOffset ) const = 0;
			
			
			/// Move the current position in the stream by the specified relative signed offset in bytes.
			/**
			  * The method attempts to seek in the stream by the specified amount and
			  * returns the signed amount that the position in the stream was changed by
			  * in bytes. A negative offset indicates that the position should be moved in
			  * reverse and a positive offset indicates that the position should be moved
			  * forwards.
			  */
			virtual Int64 seek( Int64 relativeOffset ) = 0;
			
			
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			/// Return the absolute position in the stream in bytes.
			virtual LargeIndex getPosition() const = 0;
			
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DATA_OUTPUT_STREAM_H
