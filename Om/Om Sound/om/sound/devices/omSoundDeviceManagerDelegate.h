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

#ifndef INCLUDE_OM_SOUND_DEVICE_MANAGER_DELEGATE_H
#define INCLUDE_OM_SOUND_DEVICE_MANAGER_DELEGATE_H


#include "omSoundDevicesConfig.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class SoundDeviceManager;
class SoundDeviceID;




//********************************************************************************
/// A class that contains function objects that recieve SoundDeviceManager events.
/**
  * Any device-manager-related event that might be processed has an appropriate callback
  * function object. Each callback function is called by the device manager
  * whenever such an event is received. If a callback function in the delegate
  * is not initialized, the device manager simply ignores it.
  */
class SoundDeviceManagerDelegate
{
	public:
		
		//********************************************************************************
		//******	Sound Device Manager Delegate Callback Functions
			
			
			/// A function object that is called whenever a new sound device is discovered by the system.
			/**
			  * This callback is called whenever a new sound device is connected or added to the
			  * system. This allows the user to use new sound devices as they are discovered.
			  *
			  * This method will be called from a separate thread
			  * so any data accessed in the callback function should use proper thread synchonization
			  * to avoid unsafe access.
			  */
			Function<void ( SoundDeviceManager& manager, const SoundDeviceID& deviceID )> deviceAdded;
			
			
			/// A function object that is called whenever a sound device is removed from the system.
			/**
			  * This callback is called whenever a sound device is disconnected or removed from the
			  * system. This allows the user to know when devices are disconnected and to discontinue use
			  * of those devices.
			  *
			  * This method will be called from a separate thread
			  * so any data accessed in the callback function should use proper thread synchonization
			  * to avoid unsafe access.
			  */
			Function<void ( SoundDeviceManager& manager, const SoundDeviceID& deviceID )> deviceRemoved;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_DEVICE_MANAGER_DELEGATE_H
