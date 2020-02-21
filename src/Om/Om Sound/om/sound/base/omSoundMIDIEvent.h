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

#ifndef INCLUDE_OM_SOUND_MIDI_EVENT_H
#define INCLUDE_OM_SOUND_MIDI_EVENT_H


#include "omSoundBaseConfig.h"


#include "omSoundMIDIMessage.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a single MIDI message that occurred at an absolute moment in time.
/**
  * This class consists of a MIDIMessage object and a relative time at which that
  * event occurs.
  */
class MIDIEvent
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default MIDI event with an undefined message and time.
			OM_INLINE MIDIEvent()
				:	message(),
					time()
			{
			}
			
			
			/// Create a MIDI event for the specified message.
			/**
			  * The time for the event is default-initialized to be 0, indicating
			  * it coincided with the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  */
			OM_INLINE MIDIEvent( const MIDIMessage& newMessage )
				:	message( newMessage ),
					time()
			{
			}
			
			
			/// Create a MIDI event for the specified message at the specified time.
			/**
			  * This time is measured relative to the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  */
			OM_INLINE MIDIEvent( const MIDIMessage& newMessage, const Time& newTime )
				:	message( newMessage ),
					time( newTime )
			{
			}
			
			
		//********************************************************************************
		//******	Message Accessor Methods
			
			
			/// Return a reference to the MIDI message associated with this event.
			OM_INLINE const MIDIMessage& getMessage() const
			{
				return message;
			}
			
			
			/// Set the MIDI message that is associated with this event.
			OM_INLINE void setMessage( const MIDIMessage& newMessage )
			{
				message = newMessage;
			}
			
			
		//********************************************************************************
		//******	Time Accessor Methods
			
			
			/// Return the relative time at which this MIDI event occurred.
			/**
			  * This time is measured relative to some reference time. For absolute-time
			  * MIDI events (real-time), the reference time is the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  * For other events, such as those that are part of a SoundFrame, the
			  * time is specified relative to the frame's time.
			  */
			OM_INLINE const Time& getTime() const
			{
				return time;
			}
			
			
			/// Set the absolute time at which this MIDI event occurred.
			/**
			  * This time is measured relative to some reference time. For absolute-time
			  * MIDI events (real-time), the reference time is the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  * For other events, such as those that are part of a SoundFrame, the
			  * time is specified relative to the frame's time.
			  */
			OM_INLINE void setTime( const Time& newTime )
			{
				time = newTime;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The MIDI message associated with this MIDI event.
			MIDIMessage message;
			
			
			/// The relative offset in time when the associated MIDI message occurred from some reference point.
			/**
			  * This time is measured relative to some reference time. For absolute-time
			  * MIDI events (real-time), the reference time is the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  * For other events, such as those that are part of a SoundFrame, the
			  * time is specified relative to the frame's time.
			  */
			Time time;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_EVENT_H
