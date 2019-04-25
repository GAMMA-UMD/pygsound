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

#ifndef INCLUDE_OM_SOUND_MIDI_DECODER_H
#define INCLUDE_OM_SOUND_MIDI_DECODER_H


#include "omSoundIOConfig.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that decodes MIDI file data into a stream of MIDI events.
class MIDIDecoder : public MIDIInputStream
{
	public:
		
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not seeking is allowed in this input stream.
			/**
			  * The defaut implementation returns FALSE, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream's current position can be moved by the specified time offset.
			/**
			  * The defaut implementation returns FALSE, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Bool canSeek( const Time& timeOffset ) const;
			
			
			/// Move the current position in the stream by the specified signed time offset in seconds.
			/**
			  * This method attempts to seek the position in the stream by the specified time in seconds.
			  * The method returns the signed amount that the position in the stream was changed
			  * by. Thus, if seeking is not allowed, 0 is returned. Otherwise, the stream should
			  * seek as far as possible in the specified direction and return the actual change
			  * in position.
			  *
			  * The defaut implementation returns 0, disabling seeking. Override
			  * the function to allow seeking of input streams.
			  */
			virtual Time seek( const Time& timeOffset );
			
			
		//********************************************************************************
		//******	Stream Position Accessor Methods
			
			
			/// Return the number of seconds remaining in the MIDI input stream.
			/**
			  * The value returned must only be a lower bound on the total time left in the stream.
			  * events in the stream. This behavior is allowed in order to support never-ending streams
			  * which might not have a defined endpoint.
			  */
			virtual Time getTimeRemaining() const;
			
			
			/// Return the current position of the stream in seconds relative to the start of the stream.
			/**
			  * The returned value indicates the time offset of the current read
			  * position within the MIDI stream. For unbounded streams, this indicates
			  * the number of samples that have been read by the stream since it was opened.
			  */
			virtual Time getTime() const;
			
			
	protected:
		
		//********************************************************************************
		//******	Protected MIDI Stream Methods
			
			
			/// Read MIDI events into the buffer for the next time interval of the specified length.
			/**
			  * This method attempts to read events for the specified time interval from the
			  * stream into the buffer and then returns the total number of events that
			  * were successfully read. The stream's current position is incremented by the
			  * return value.
			  *
			  * The timestamps of the returned MIDI events are specified relative to the start
			  * of the time interval.
			  */
			virtual Size readEvents( MIDIBuffer& buffer, const Time& length );
			
			
	protected:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			
};




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_DECODER_H
