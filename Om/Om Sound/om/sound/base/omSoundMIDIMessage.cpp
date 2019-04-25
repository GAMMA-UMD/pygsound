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

#include "omSoundMIDIMessage.h"


// 4-bit message type codes, from midi.org
#define MIDI_NOTE_OFF 8
#define MIDI_NOTE_ON 9
#define MIDI_AFTERTOUCH 10
#define MIDI_CONTROL_CHANGE 11
#define MIDI_PROGRAM_CHANGE 12
#define MIDI_CHANNEL_PRESSURE 13
#define MIDI_PITCH_WHEEL 14
#define MIDI_SYSTEM 15


// System message type codes.
#define MIDI_START 10
#define MIDI_CONTINUE 11
#define MIDI_STOP 14


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Private Static Helper Methods
//############		
//##########################################################################################
//##########################################################################################




OM_FORCE_INLINE static UByte midiDataToByte( Float data )
{
	return UByte(math::clamp( data, 0.0f, 1.0f )*127);
}




OM_FORCE_INLINE static Float midiDataToFloat( UByte data )
{
	return Float(data) / Float(127);
}




OM_FORCE_INLINE static UByte makeMIDIStatus( UByte typeCode, UByte channel )
{
	return (typeCode << 4) | channel;
}




//##########################################################################################
//##########################################################################################
//############		
//############		MIDI Message Parse Function
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIMessage:: fromBytes( const UByte* data, MIDIMessage& message, Size& messageLength )
{
	// Get the first byte
	UByte byte0 = data[0];
	UByte messageType = byte0 >> 4;
	UByte channel = byte0 & 0x0F;
	
	switch ( messageType )
	{
		case MIDI_NOTE_ON:
		{
			UByte note = data[1] & 0x7F;
			UByte velocity = data[2] & 0x7F;
			MIDIMessage::Type type = NOTE_ON;
			
			// Generate a note-off message instead, if the velocity is 0.
			if ( velocity == 0 )
				type = NOTE_OFF;
			
			message = MIDIMessage( type, channel, note, midiDataToFloat(velocity) );
			messageLength = 3;
		}
		break;
		
		case MIDI_NOTE_OFF:
		{
			UByte note = data[1] & 0x7F;
			UByte velocity = data[2] & 0x7F;
			
			message = MIDIMessage( NOTE_OFF, channel, note, midiDataToFloat(velocity) );
			messageLength = 3;
		}
		break;
		
		case MIDI_AFTERTOUCH:
		{
			UByte note = data[1] & 0x7F;
			UByte pressure = data[2] & 0x7F;
			
			message = MIDIMessage( AFTERTOUCH, channel, note, midiDataToFloat(pressure) );
			messageLength = 3;
		}
		break;
		
		case MIDI_CONTROL_CHANGE:
		{
			UByte controller = data[1] & 0x7F;
			UByte value = data[2] & 0x7F;
			
			message = MIDIMessage( CONTROL_CHANGE, channel, controller, midiDataToFloat(value) );
			messageLength = 3;
		}
		break;
		
		case MIDI_PROGRAM_CHANGE:
		{
			UByte program = data[1] & 0x7F;
			
			message = MIDIMessage( PROGRAM_CHANGE, channel, program );
			messageLength = 2;
		}
		break;
		
		case MIDI_CHANNEL_PRESSURE:
		{
			UByte pressure = data[1] & 0x7F;
			
			message = MIDIMessage( CHANNEL_PRESSURE, channel, midiDataToFloat(pressure) );
			messageLength = 2;
		}
		break;
		
		case MIDI_PITCH_WHEEL:
		{
			UInt pitch = UInt(data[1] & 0x7F) |
						(UInt(data[2] & 0x7F) << 7);
			const UInt centered = 0x2000;
			
			// Convert the pitch to floating point between 0 and 1.
			Float pitchFloat = Float(pitch) - Float(centered);
			
			if ( pitchFloat > 0 )
				pitchFloat /= Float(centered - 1);
			else
				pitchFloat /= Float(centered);
			
			message = MIDIMessage( PITCH_WHEEL, channel, pitchFloat );
			messageLength = 3;
		}
		break;
		
		case MIDI_SYSTEM:
		{
			switch ( channel )
			{
				case MIDI_START:
					message = MIDIMessage( START );
					break;
				case MIDI_STOP:
					message = MIDIMessage( STOP );
					break;
				case MIDI_CONTINUE:
					message = MIDIMessage( CONTINUE );
					break;
				default:
					return false;
			}
			
			messageLength = 1;
		}
		break;
		
		default:
			return false;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		MIDI Message Conversion Function
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIMessage:: toBytes( const MIDIMessage& message, UByte* bytes, Size& messageLengthInBytes )
{
	if ( bytes == NULL )
		return false;
	
	UByte channel = (message.getChannel() & 0xF);
	
	switch ( message.getType() )
	{
		case NOTE_ON:
		{
			bytes[0] = makeMIDIStatus( MIDI_NOTE_ON, channel );
			bytes[1] = message.getData1() & 0x7F;
			bytes[2] = midiDataToByte( message.getData2() );
			messageLengthInBytes = 3;
		}
		break;
		
		case NOTE_OFF:
		{
			bytes[0] = makeMIDIStatus( MIDI_NOTE_OFF, channel );
			bytes[1] = message.getData1() & 0x7F;
			bytes[2] = midiDataToByte( message.getData2() );
			messageLengthInBytes = 3;
		}
		break;
		
		case CONTROL_CHANGE:
		{
			bytes[0] = makeMIDIStatus( MIDI_CONTROL_CHANGE, channel );
			bytes[1] = message.getData1() & 0x7F;
			bytes[2] = midiDataToByte( message.getData2() );
			messageLengthInBytes = 3;
		}
		break;
		
		case AFTERTOUCH:
		{
			bytes[0] = makeMIDIStatus( MIDI_AFTERTOUCH, channel );
			bytes[1] = message.getData1() & 0x7F;
			bytes[2] = midiDataToByte( message.getData2() );
			messageLengthInBytes = 3;
		}
		break;
		
		case CHANNEL_PRESSURE:
		{
			bytes[0] = makeMIDIStatus( MIDI_CHANNEL_PRESSURE, channel );
			bytes[1] = midiDataToByte( message.getData2() );
			messageLengthInBytes = 2;
		}
		break;
		
		case PITCH_WHEEL:
		{
			bytes[0] = makeMIDIStatus( MIDI_PITCH_WHEEL, channel );
			
			const UInt centered = 0x2000;
			Float pitchFloat = message.getData2();
			
			if ( pitchFloat > 0 )
				pitchFloat *= Float(centered - 1);
			else
				pitchFloat *= Float(centered);
			
			UInt pitch = UInt(pitchFloat + Float(centered));
			
			// Pack the 14-bit pitch value into 2 bytes.
			bytes[1] = (pitch & 0x7F);
			bytes[2] = (pitch & 0x3F80) >> 7;
			
			messageLengthInBytes = 3;
		}
		break;
		
		case PROGRAM_CHANGE:
		{
			bytes[0] = makeMIDIStatus( MIDI_PROGRAM_CHANGE, channel );
			bytes[1] = message.getData1() & 0x7F;
			messageLengthInBytes = 2;
		}
		break;
		
		case START:
		{
			bytes[0] = makeMIDIStatus( MIDI_SYSTEM, MIDI_START );
			messageLengthInBytes = 1;
		}
		break;
		
		case STOP:
		{
			bytes[0] = makeMIDIStatus( MIDI_SYSTEM, MIDI_STOP );
			messageLengthInBytes = 1;
		}
		break;
		
		case CONTINUE:
		{
			bytes[0] = makeMIDIStatus( MIDI_SYSTEM, MIDI_CONTINUE );
			messageLengthInBytes = 1;
		}
		break;
		
		default:
			return false;
	}
	
	return true;
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
