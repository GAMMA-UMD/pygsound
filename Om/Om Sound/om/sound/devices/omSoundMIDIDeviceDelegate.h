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

#ifndef INCLUDE_OM_SOUND_MIDI_DEVICE_DELEGATE_H
#define INCLUDE_OM_SOUND_MIDI_DEVICE_DELEGATE_H


#include "omSoundDevicesConfig.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class MIDIDevice;



/// Define a short name for the type of the function object that is used to recieve MIDI from MIDIDevice objects.
/**
  * The implementor of the function can then use the given MIDI messages to perform some action.
  */
typedef Function<void ( MIDIDevice& device, const MIDIBuffer& messages )> MIDIInputCallback;




//********************************************************************************
/// A class that contains function objects that recieve MIDIDevice events.
/**
  * Any device-related event that might be processed has an appropriate callback
  * function object. Each callback function is called by the device
  * whenever such an event is received. If a callback function in the delegate
  * is not initialized, the device simply ignores it.
  */
class MIDIDeviceDelegate
{
	public:
		
		//********************************************************************************
		//******	MIDI Device Delegate Callback Functions
			
			
			/// A function object that is called whenever the device provides input MIDI messages.
			MIDIInputCallback inputCallback;
			
			
			/// A function object that is called whenever the MIDI device is added to the system.
			Function<void ( MIDIDevice& device )> added;
			
			
			/// A function object that is called whenever the MIDI device is removed from the system.
			Function<void ( MIDIDevice& device )> removed;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_DEVICE_DELEGATE_H
