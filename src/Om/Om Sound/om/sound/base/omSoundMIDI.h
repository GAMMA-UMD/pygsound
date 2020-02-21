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

#ifndef INCLUDE_OM_SOUND_MIDI_H
#define INCLUDE_OM_SOUND_MIDI_H


#include "omSoundBaseConfig.h"


#include "omSoundMIDIInputStream.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a MIDI sequence.
class MIDI : public MIDIInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors//********************************************************************************
			
			
		//********************************************************************************
		//******	Seeking Methods
			
			
			/// Return whether or not seeking is allowed in this input stream.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this stream's current position can be moved by the specified time offset.
			virtual Bool canSeek( const Time& timeOffset ) const;
			
			
			/// Move the current position in the stream by the specified signed time offset in seconds.
			virtual Time seek( const Time& timeOffset );
			
			
		//********************************************************************************
		//******	Stream Position Accessor Methods
			
			
			/// Return the number of seconds remaining in the MIDI input stream.
			virtual Time getTimeRemaining() const;
			
			
			/// Return the current position of the stream in seconds relative to the start of the stream.
			virtual Time getTime() const;
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a MIDI resource.
			static const om::resources::ResourceType RESOURCE_TYPE;
			
			
			
	protected:
		
		//********************************************************************************
		//******	Protected MIDI Stream Methods
			
			
			/// Read the specified number of MIDI events from the input stream into the output buffer.
			virtual Size readEvents( MIDIBuffer& buffer, const Time& length );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::sound::base::MIDI, om::sound::base::MIDI::RESOURCE_TYPE );


#endif // INCLUDE_OM_SOUND_MIDI_H
