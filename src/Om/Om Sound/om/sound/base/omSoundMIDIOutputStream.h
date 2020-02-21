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

#ifndef INCLUDE_OM_SOUND_MIDI_OUTPUT_STREAM_H
#define INCLUDE_OM_SOUND_MIDI_OUTPUT_STREAM_H


#include "omSoundBaseConfig.h"


#include "omSoundMIDIMessage.h"
#include "omSoundMIDIEvent.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a stream-writable destination for MIDI data.
class MIDIOutputStream
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this MIDI output stream and release any resourcees associated with it.
			virtual ~MIDIOutputStream()
			{
			}
			
			
		//********************************************************************************
		//******	Stream Writing Methods
			
			
			/// Write the specified number of MIDI events from the buffer to the output stream.
			/**
			  * This method attempts to write the specified number of MIDI events to the stream
			  * from the buffer. It then returns the total number of valid events which
			  * were written to the stream. The current write position in the stream
			  * is advanced by the number of events that are written.
			  */
			OM_INLINE Size write( const MIDIBuffer& buffer, Size numEvents )
			{
				numEvents = math::min( buffer.getEventCount(), numEvents );
				
				return this->writeEvents( buffer, numEvents );
			}
			
			
			/// Flush the MIDI output stream, sending all internally buffered events to the destination.
			/** 
			  * This method causes all currently pending output MIDI data to be sent to it's
			  * final destination. This method blocks the current thread until it ensures that
			  * this is done and that all internal data buffers are emptied if they have any contents.
			  */
			virtual void flush() = 0;
			
			
		//********************************************************************************
		//******	Public Seek Status Accessor Methods
			
			
			/// Return whether or not seeking is allowed in this input stream.
			virtual Bool canSeek() const = 0;
			
			
			/// Return whether or not this input stream's current position can be moved by the specified signed event offset.
			/**
			  * This event offset is specified as the signed number of MIDI events to move
			  * in the stream.
			  */
			virtual Bool canSeek( Int64 relativeEventOffset ) const = 0;
			
			
			/// Move the current event position in the stream by the specified signed amount of events.
			/**
			  * This method attempts to seek the position in the stream by the specified amount of MIDI events.
			  * The method returns the signed amount that the position in the stream was changed
			  * by. Thus, if seeking is not allowed, 0 is returned. Otherwise, the stream should
			  * seek as far as possible in the specified direction and return the actual change
			  * in position.
			  */
			virtual Int64 seek( Int64 relativeEventOffset ) = 0;
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Stream Methods
			
			
			/// Write the specified number of MIDI events from the buffer to the output stream.
			/**
			  * This method attempts to write the specified number of MIDI events to the stream
			  * from the buffer. It then returns the total number of valid events which
			  * were written to the stream. The current write position in the stream
			  * is advanced by the number of events that are written.
			  */
			virtual Size writeEvents( const MIDIBuffer& buffer, Size numEvents ) = 0;
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_OUTPUT_STREAM_H
