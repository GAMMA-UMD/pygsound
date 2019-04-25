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

#ifndef INCLUDE_OM_SOUND_MIDI_BUFFER_H
#define INCLUDE_OM_SOUND_MIDI_BUFFER_H


#include "omSoundBaseConfig.h"


#include "omSoundMIDIEvent.h"
#include "omSoundMIDITime.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a sequence of MIDI events.
/**
  * A MIDI buffer keeps a small local storage space for events which avoids
  * allocating any extra memory when the total number of events in a buffer
  * small (less than a few).
  */
class MIDIBuffer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty MIDI buffer with the default initial capacity.
			MIDIBuffer();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a MIDI buffer, deallocating its internal array of events.
			~MIDIBuffer();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of the specified MIDI buffer to this one.
			MIDIBuffer& operator = ( const MIDIBuffer& other );
			
			
		//********************************************************************************
		//******	Event Accessor Methods
			
			
			/// Return the total number of valid events that are part of this MIDI buffer.
			OM_INLINE Size getEventCount() const
			{
				return events.getSize();
			}
			
			
			/// Return a reference to the MIDI event at the specified index in this MIDI buffer.
			OM_INLINE MIDIEvent& getEvent( Index eventIndex )
			{
				OM_DEBUG_ASSERT_MESSAGE( eventIndex < events.getSize(), "Invalid MIDI event buffer index" );
				
				return events[eventIndex];
			}
			
			
			/// Return a const reference to the MIDI event at the specified index in this MIDI buffer.
			OM_INLINE const MIDIEvent& getEvent( Index eventIndex ) const
			{
				OM_DEBUG_ASSERT_MESSAGE( eventIndex < events.getSize(), "Invalid MIDI event buffer index" );
				
				return events[eventIndex];
			}
			
			
			/// Add a new MIDI event to the end of this MIDI buffer.
			OM_INLINE void addEvent( const MIDIEvent& newEvent )
			{
				events.add( newEvent );
			}
			
			
			/// Remove all events from this MIDI buffer.
			OM_INLINE void clearEvents()
			{
				events.clear();
			}
			
			
		//********************************************************************************
		//******	Buffer MIDI Time Accessor Methods
			
			
			/// Return the musical time within the MIDI sequence for the start of this frame.
			OM_INLINE const MIDITime& getTime() const
			{
				return time;
			}
			
			
			/// Set the musical time within the MIDI sequence for the start of this frame.
			OM_INLINE void setTime( const MIDITime& newTime )
			{
				time = newTime;
			}
			
			
		//********************************************************************************
		//******	Buffer Copy Method
			
			
			/// Copy the entire contents of this buffer to another MIDI buffer, including the MIDI time.
			void copyTo( MIDIBuffer& other ) const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Define the size of the fixed-size array of MIDI events that is part of a MIDIBuffer.
			static const Size FIXED_EVENT_ARRAY_SIZE = 2;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An internal list of events.
			ShortArrayList<MIDIEvent,FIXED_EVENT_ARRAY_SIZE> events;
			
			
			/// An object representing the musical time within the MIDI sequence for the start of this buffer.
			MIDITime time;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_BUFFER_H
