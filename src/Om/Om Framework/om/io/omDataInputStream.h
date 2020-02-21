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

#ifndef INCLUDE_OM_DATA_INPUT_STREAM_H
#define INCLUDE_OM_DATA_INPUT_STREAM_H


#include "omIOConfig.h"


//##########################################################################################
//*******************************  Start Om IO Namespace  **********************************
OM_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class/interface which represents an abstract read-only stream of data.
class DataInputStream
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an input stream and free all of it's resources (close it).
			virtual ~DataInputStream()
			{
			}
			
			
		//********************************************************************************
		//******	Data Read Methods
			
			
			/// Read the specified number of bytes from the stream and place them in the buffer given by a pointer.
			/**
			  * The buffer must be large enough to hold the specified number of bytes. The number
			  * of bytes read can be less than the desired number if an error is encountered or the
			  * end of the stream is reached.
			  *
			  * If the method succeeds, the return value will equal the parameter numBytes and
			  * the stream will be advanced by that many bytes. Otherwise, the return value will
			  * be less than the parameter numBytes, indicating the amount that the stream position
			  * changed.
			  * 
			  * @param buffer - a pointer to a buffer where the read data should be placed.
			  * @param numBytes - the number of bytes of data to read from the stream.
			  * @return the number of bytes that were actually read and placed in the buffer.
			  */
			virtual Size readData( UByte* buffer, Size numBytes ) = 0;
			
			
			/// Read the specified number of bytes from the stream and place them in the specified data buffer.
			Size readData( data::DataBuffer& buffer, Size numBytes );
			
			
			/// Read as many bytes from the stream as possible and return them in a Data object.
			data::Data readAllData();
			
			
			/// Read as many bytes from the stream as possible and place them in the specified data buffer.
			/**
			  * The method returns the total number of bytes written to the buffer.
			  */
			Size readAllData( data::DataBuffer& buffer );
			
			
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
		//******	Remaining Data Size Accessor Methods
			
			
			/// Return whether or not there are bytes remaining in the stream.
			OM_INLINE Bool hasBytesRemaining() const
			{
				return this->getBytesRemaining() > 0;
			}
			
			
			/// Return the number of bytes remaining in the stream.
			/**
			  * The value returned must only be a lower bound on the number of bytes
			  * remaining in the stream. If there are bytes remaining, it must return
			  * at least 1.
			  */
			virtual LargeSize getBytesRemaining() const = 0;
			
			
			/// Return the current byte index within the stream relative to the beginning.
			virtual LargeIndex getPosition() const = 0;
			
			
			
};




//##########################################################################################
//*******************************  End Om IO Namespace  ************************************
OM_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DATA_INPUT_STREAM_H
