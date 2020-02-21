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

#ifndef INCLUDE_OM_SOUND_MIDI_DEVICE_ID_H
#define INCLUDE_OM_SOUND_MIDI_DEVICE_ID_H


#include "omSoundDevicesConfig.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that is used to encapsulate a unique identifier for a system MIDI device.
/**
  * This opaque type uses a platform-dependent internal representation which uniquelly
  * identifies a MIDI device.
  */
class MIDIDeviceID
{
	public:
		
		//********************************************************************************
		//******	Public Constants
			
			
			/// Define an instance of MIDIDeviceID that represents an invalid device.
			static const MIDIDeviceID INVALID_DEVICE;
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this device ID represents the same device as another.
			OM_INLINE Bool operator == ( const MIDIDeviceID& other ) const
			{
				return deviceID == other.deviceID && input == other.input && output == other.output;
			}
			
			
			/// Return whether or not this device ID represents a different device than another.
			OM_INLINE Bool operator != ( const MIDIDeviceID& other ) const
			{
				return deviceID != other.deviceID || input != other.input || output != other.output;
			}
			
			
		//********************************************************************************
		//******	Device Status Accessor Methods
			
			
			/// Return whether or not this MIDIDeviceID represents a valid device.
			/**
			  * This condition is met whenever the device ID is not equal to INVALID_DEVICE_ID.
			  */
			OM_INLINE Bool isValid() const
			{
				return deviceID != INVALID_DEVICE_ID;
			}
			
			
			/// Return whether or not this device ID represents a device capable of MIDI input.
			OM_INLINE Bool isInput() const
			{
				return input;
			}
			
			
			/// Return whether or not this device ID represents a device capable of MIDI output.
			OM_INLINE Bool isOutput() const
			{
				return output;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Constructor
			
			
#if defined(OM_PLATFORM_APPLE)
	#if defined(OM_PLATFORM_64_BIT)
			/// Create a MIDIDeviceID object that represents the device with the specified device ID.
			OM_INLINE MIDIDeviceID( UInt32 newDeviceID, Bool newIsInput, Bool newIsOutput )
				:	deviceID( newDeviceID ),
					input( newIsInput ),
					output( newIsOutput )
			{
			}
	#else
			/// Create a MIDIDeviceID object that represents the device with the specified device ID.
			OM_INLINE MIDIDeviceID( void* newDeviceID, Bool newIsInput, Bool newIsOutput )
				:	deviceID( newDeviceID ),
					input( newIsInput ),
					output( newIsOutput )
			{
			}
	#endif

#elif defined(OM_PLATFORM_WINDOWS)
			OM_INLINE MIDIDeviceID( UInt newDeviceID, Bool newIsInput, Bool newIsOutput )
				:	deviceID( newDeviceID ),
					input( newIsInput ),
					output( newIsOutput )
			{
			}
// added Linux definition by Zhenyu Tang on 11/25/2018
#elif defined(OM_PLATFORM_LINUX)
			OM_INLINE MIDIDeviceID( void* newDeviceID, Bool newIsInput, Bool newIsOutput )
				:	deviceID( newDeviceID ),
					input( newIsInput ),
					output( newIsOutput )
			{
			}
#endif
//----------------------------------------------------
			
			
		//********************************************************************************
		//******	Private Conversion Methods
			
			
#if defined(OM_PLATFORM_APPLE)
	#if defined(OM_PLATFORM_64_BIT)
			/// Convert this MIDIDeviceID object to a 32-bit integer which uniquely represents a MIDI device.
			OM_INLINE UInt32 getInternalID() const
			{
				return deviceID;
			}
	#else
			/// Convert this MIDIDeviceID object to a pointer which uniquely represents a MIDI device.
			OM_INLINE void* getInternalID() const
			{
				return deviceID;
			}
	#endif

#elif defined(OM_PLATFORM_WINDOWS)
			/// Convert this MIDIDeviceID object to an integer which uniquely represents a MIDI device.
			OM_INLINE UInt getInternalID() const
			{
				return deviceID;
			}
#endif
			
			
#if defined(OM_PLATFORM_APPLE)
	#if defined(OM_PLATFORM_64_BIT)
			/// Convert this MIDIDeviceID object to a 32-bit integer which uniquely represents a MIDI device.
			OM_INLINE operator UInt32 () const
			{
				return deviceID;
			}
	#else
			/// Convert this MIDIDeviceID object to a pointer which uniquely represents a MIDI device.
			OM_INLINE operator void* () const
			{
				return deviceID;
			}
	#endif

#elif defined(OM_PLATFORM_WINDOWS)
			/// Convert this MIDIDeviceID object to an integer which uniquely represents a MIDI device.
			OM_INLINE operator UInt () const
			{
				return deviceID;
			}
#endif
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
#if defined(OM_PLATFORM_APPLE)
	#if defined(OM_PLATFORM_64_BIT)
			/// The underlying representation of a MIDIDeviceID on 64-bit Mac OS X, a 32-bit integer.
			UInt32 deviceID;
			
			/// The reserved ID used to indicate an invalid device.
			static UInt32 INVALID_DEVICE_ID;
	#else
			/// The underlying representation of a MIDIDeviceID on 32-bit Mac OS X, a pointer.
			void* deviceID;
			
			/// The reserved ID used to indicate an invalid device.
			static void* INVALID_DEVICE_ID;
	#endif

#elif defined(OM_PLATFORM_WINDOWS)
			/// The underlying representation of a MIDIDeviceID on Windows, an unsigned integer.
			UInt deviceID;
			
			/// The reserved ID used to indicate an invalid device.
			static UInt INVALID_DEVICE_ID;
#endif
			
			/// A boolean value indicating whether or not this device ID represents an input device.
			Bool input;
			
			
			/// A boolean value indicating whether or not this device ID represents an output device.
			Bool output;
			
// added Linux definition by Zhenyu Tang on 11/25/2018
#if defined(OM_PLATFORM_LINUX)
			/// The underlying representation of a MIDIDeviceID on Linux, a pointer.
			void* deviceID;
			static void* INVALID_DEVICE_ID;
#endif
//----------------------------------------------------
			
		//********************************************************************************
		//******	Private Friend Declaration
			
			
			/// Declare the MIDIDeviceManager a friend so that it can construct MIDIDeviceID objects.
			friend class MIDIDeviceManager;
			
			
			/// Declare the MIDIDevice a friend so that it can construct MIDIDeviceID objects.
			friend class MIDIDevice;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_DEVICE_ID_H
