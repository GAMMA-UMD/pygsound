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

#ifndef INCLUDE_OM_SOUND_DEVICE_ID_H
#define INCLUDE_OM_SOUND_DEVICE_ID_H


#include "omSoundDevicesConfig.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that is used to encapsulate a unique identifier for a system sound device.
/**
  * This opaque type uses a platform-dependent internal representation which uniquelly
  * identifies a sound device.
  */
class SoundDeviceID
{
	public:
		
		//********************************************************************************
		//******	Public Constants
			
			
			/// Define an instance of SoundDeviceID that represents an invalid device.
			static const SoundDeviceID INVALID_DEVICE;
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this device ID represents the same device as another.
			OM_INLINE Bool operator == ( const SoundDeviceID& other ) const
			{
				return deviceID == other.deviceID;
			}
			
			
			/// Return whether or not this device ID represents a different device than another.
			OM_INLINE Bool operator != ( const SoundDeviceID& other ) const
			{
				return deviceID != other.deviceID;
			}
			
			
		//********************************************************************************
		//******	Device Status Accessor Method
			
			
			/// Return whether or not this SoundDeviceID represents a valid device.
			/**
			  * This condition is met whenever the device ID is not equal to INVALID_DEVICE_ID.
			  */
			OM_INLINE Bool isValid() const
			{
				return deviceID != INVALID_DEVICE_ID;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructor
			
			
#if defined(OM_PLATFORM_WINDOWS)
			/// Create a SoundDeviceID object that represents the device with the specified device ID.
			OM_INLINE explicit SoundDeviceID( const UTF16String& newDeviceID )
				:	deviceID( newDeviceID )
			{
			}
			
#else
			/// Create a SoundDeviceID object that represents the device with the specified device ID.
			OM_INLINE explicit SoundDeviceID( UInt newDeviceID )
				:	deviceID( newDeviceID )
			{
			}
#endif
			
			
		//********************************************************************************
		//******	Private Conversion Methods
			
			
#if defined(OM_PLATFORM_WINDOWS)
			/// Return a const reference to the wide-character string uniquely representing a sound device.
			OM_INLINE const UTF16String& getIDString() const
			{
				return deviceID;
			}
#else
			/// Convert this SoundDeviceID object to an unsigned integer which uniquely represents a sound device.
			OM_INLINE operator UInt () const
			{
				return deviceID;
			}
#endif
			
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
#if defined(OM_PLATFORM_WINDOWS)
			
			/// The underlying representation of a SoundDeviceID on windows, a UTF-16 encoded ID string.
			UTF16String deviceID;
			
			/// The reserved ID used to indicate an invalid device.
			static const UTF16String INVALID_DEVICE_ID;
			
#else
			
			/// The underlying representation of a SoundDeviceID, an unsigned integer.
			UInt deviceID;
			
			/// The reserved ID used to indicate an invalid device.
			static const UInt INVALID_DEVICE_ID = 0xFFFFFFFF;
			
#endif
			
			
			
		//********************************************************************************
		//******	Private Friend Declaration
			
			
			/// Declare the SoundDeviceManager a friend so that it can construct SoundDeviceID objects.
			friend class SoundDeviceManager;
			
			
			/// Declare the SoundDevice a friend so that it can construct SoundDeviceID objects.
			friend class SoundDevice;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_DEVICE_ID_H
