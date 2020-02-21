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

#ifndef INCLUDE_OM_SOUND_MIDI_DEVICE_H
#define INCLUDE_OM_SOUND_MIDI_DEVICE_H


#include "omSoundDevicesConfig.h"


#include "omSoundMIDIDeviceID.h"
#include "omSoundMIDIDeviceDelegate.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents a system MIDI device.
/**
  * The class allows the user to send and recieve MIDI events to/from MIDI hardware
  * ports.
  */
class MIDIDevice
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a MIDIDevice for the specified MIDIDeviceID.
			MIDIDevice( const MIDIDeviceID& newDeviceID );
			
			
			/// Create a copy of the specified MIDIDevice object.
			MIDIDevice( const MIDIDevice& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a MIDIDevice object, stopping the input/output of any MIDI.
			~MIDIDevice();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state from one MIDIDevice to this object.
			MIDIDevice& operator = ( const MIDIDevice& other );
			
			
		//********************************************************************************
		//******	MIDI Start/Stop Methods
			
			
			/// Start sending/receiving MIDI events to/from the device.
			/**
			  * If the device is invalid or if an error occurs, FALSE is returned indicating
			  * that the method had no effect. If TRUE is returned, the device was started successfully
			  *
			  * This method should be called before sending any messages to a MIDI output device.
			  */
			Bool start();
			
			
			/// Stop sending/receiving MIDI events to/from the device.
			/**
			  * If the device is currently receiving MIDI, the input of further MIDI events
			  * is stopped. Otherwise, the method has no effect. If the device is invalid,
			  * this method has no effect.
			  *
			  * This method has the effect of stopping the MIDI thread that was
			  * started in the start() method.
			  */
			void stop();
			
			
			/// Return whether or not the device is currently sending/receiving MIDI.
			/**
			  * If MIDI is currently being received and sent to the device, TRUE is returned.
			  * Otherwise, FALSE is returned. If the device is invalid, FALSE is always
			  * returned.
			  * 
			  * @return whether or not the device IO is currently running.
			  */
			Bool isRunning() const;
			
			
		//********************************************************************************
		//******	MIDI Output Methods
			
			
			/// Send the specified MIDI event to the output of this MIDI device.
			/**
			  * If the method fails or if the device is not an output, FALSE is returned.
			  * Otherwise, the method succeeds and TRUE is returned.
			  */
			Bool write( const MIDIEvent& event );
			
			
			/// Send the specified MIDI message buffer to the output of this MIDI device.
			/**
			  * If the method fails or if the device is not an output, FALSE is returned.
			  * Otherwise, the method succeeds and TRUE is returned.
			  */
			Bool write( const MIDIBuffer& messageBuffer );
			
			
		//********************************************************************************
		//******	Device Name Accessor Method
			
			
			/// Get a string representing the name of this device.
			/**
			  * This name is usually specified by the hardware driver as a human-readable
			  * identifier for the device. If the device is not valid, the empty string
			  * is returned.
			  * 
			  * @return the name of this device.
			  */
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Get a string representing the name of this device's manufacturer.
			/**
			  * This name is usually specified by the hardware driver as a human-readable
			  * identifier for the device's manufacturer. If the device is not valid, the empty string
			  * is returned.
			  * 
			  * @return the name of this device's manufacturer.
			  */
			OM_INLINE const UTF8String& getManufacturer() const
			{
				return manufacturer;
			}
			
			
		//********************************************************************************
		//******	Delegate Accessor Methods
			
			
			/// Return a reference to the delegate object that is responding to events for this device.
			OM_INLINE const MIDIDeviceDelegate& getDelegate() const
			{
				return delegate;
			}
			
			
			/// Replace the delegate object that is responding to events for this device.
			void setDelegate( const MIDIDeviceDelegate& newDelegate );
			
			
		//********************************************************************************
		//******	Device ID Accessor Method
			
			
			/// Return an object that uniquely identifies this MIDI device.
			/**
			  * If the device is not valid, MIDIDeviceID::INVALID_DEVICE is returned.
			  */
			OM_INLINE MIDIDeviceID getID() const
			{
				return valid ? deviceID : MIDIDeviceID::INVALID_DEVICE;
			}
			
			
		//********************************************************************************
		//******	Device Status Accessor Methods
			
			
			/// Return whether or not this device represents a valid device.
			/**
			  * If a MIDIDevice is created with a MIDIDeviceID that does not
			  * represent a valid system audio device or if a device is removed after
			  * it is created, the MIDIDevice is marked as invalid and this
			  * method will return FALSE. Otherwise, if the device is valid, the method
			  * returns TRUE.
			  *
			  * If a device is invalid, the output callback method will not be called anymore
			  * and the application should switch to a different device. The application
			  * should periodically check the return value of this function to see if the
			  * device has been removed.
			  */
			OM_INLINE Bool isValid() const
			{
				return valid;
			}
			
			
			/// Return whether or not this MIDI device is a valid input device.
			/**
			  * If the device is not valid, FALSE is returned. Otherwise, if the
			  * MIDI device is an input device, TRUE is returned.
			  */
			OM_INLINE Bool isInput() const
			{
				return valid ? deviceID.isInput() : false;
			}
			
			
			/// Return whether or not this MIDI device is a valid output device.
			/**
			  * If the device is not valid, FALSE is returned. Otherwise, if the
			  * MIDI device is an output device, TRUE is returned.
			  */
			OM_INLINE Bool isOutput() const
			{
				return valid ? deviceID.isOutput() : false;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Wrapper Class Declaration
			
			
			/// A class that encapsulates internal data needed by the MIDIDevice object.
			class Wrapper;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Initialize a newly created device.
			Bool initializeDeviceData();
			
			
		//********************************************************************************
		//******	Platform-Specific Methods
			
			
			/// Initialize up any platform-specific data for a newly-created device.
			Bool createDevice();
			
			
			/// Clean up any platform-specific data before a device is destroyed.
			Bool destroyDevice();
			
			
			/// Register callback functions that tell the device when its attributes change.
			Bool registerDeviceUpdateCallbacks();
			
			
			/// Unregister callback functions that tell the device when its attributes change.
			Bool unregisterDeviceUpdateCallbacks();
			
			
			/// Determine whether or not this MIDI device is still valid.
			Bool refreshDeviceStatus();
			
			
			/// Refresh the name of the MIDI device.
			Bool refreshName();
			
			
			/// Refresh the manufacturer name of the MIDI device.
			Bool refreshManufacturer();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that represents a unique identifier for this MIDI device.
			MIDIDeviceID deviceID;
			
			
			/// An object that responds to events for this MIDI device.
			MIDIDeviceDelegate delegate;
			
			
			/// The device-provided name of this MIDIDevice.
			UTF8String name;
			
			
			/// The device-provided manufacturer name of this MIDIDevice.
			UTF8String manufacturer;
			
			
			/// A mutex object that handles output synchronization with device parameter changes.
			mutable threads::Mutex ioMutex;
			
			
			/// A pointer to a class that wraps internal state of this MIDIDevice.
			Wrapper* wrapper;
			
			
			/// A boolean value indicating whether or not the device is currently valid for use.
			Bool valid;
			
			
			/// A boolean value indicating whether or not the device is currently processing MIDI events.
			Bool running;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MIDI_DEVICE_H
