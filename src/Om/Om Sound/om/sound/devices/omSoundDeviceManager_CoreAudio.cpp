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

#include "omSoundDeviceManager.h"


#if defined(OM_PLATFORM_APPLE)

// Include platform-specific headers
#include <CoreServices/CoreServices.h>
#include <CoreAudio/CoreAudio.h>
#include <unistd.h>
#include <AudioUnit/AudioUnit.h>

#define ERROR_CORE_AUDIO_HAL_RUNLOOP "An error was encountered while creating a CoreAudio HAL run loop."

// Define the error strings for different types of deivce errors
#define ERROR_REGISTERING_DEVICE_CALLBACK "An error was encountered while registering an audio device change callback."
#define ERROR_REGISTERING_DEFAULT_INPUT_DEVICE_CALLBACK "An error was encountered while registering the default audio input device change callback."
#define ERROR_REGISTERING_DEFAULT_OUTPUT_DEVICE_CALLBACK "An error was encountered while registering the default audio output device change callback."
#define ERROR_UNREGISTERING_DEVICE_CALLBACK "An error was encountered while unregistering an audio device change callback."
#define ERROR_UNREGISTERING_DEFAULT_INPUT_DEVICE_CALLBACK "An error was encountered while unregistering the default audio input device change callback."
#define ERROR_UNREGISTERING_DEFAULT_OUTPUT_DEVICE_CALLBACK "An error was encountered while unregistering the default audio output device change callback."
#define ERROR_QUERYING_DEVICES "An error was encountered while querying the connected audio devices."
#define ERROR_QUERYING_NUMBER_DEVICES "An error was encountered while querying the number of connected audio devices."
#define ERROR_QUERYING_DEVICE "An error was encountered while querying a connected audio device."
#define ERROR_QUERYING_DEFAULT_INPUT_DEVICE "An error was encountered while querying the default input audio device."
#define ERROR_QUERYING_DEFAULT_OUTPUT_DEVICE "An error was encountered while querying the default output audio device."
#define ERROR_QUERYING_DEVICE_NAME "An error was encountered while querying the name of an audio device."


#define REPORT_ERROR( X ) (Console << X)


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Sound Device Manager Wrapper Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundDeviceManager:: Wrapper
{
	public:
		
		//********************************************************************************
		//******	Public Device Change Callback Method
			
			
			static OSStatus devicesChangeCallback( AudioObjectID deviceID, ::UInt32 numChangedProperties,
												const AudioObjectPropertyAddress properties[], void* clientData )
			{
				SoundDeviceManager* manager = (SoundDeviceManager*)clientData;
				
				if ( manager != NULL )
				{
					for ( Index i = 0; i < numChangedProperties; i++ )
					{
						switch ( properties[i].mSelector )
						{
							case kAudioHardwarePropertyDevices:
								manager->refreshDevices();
								break;
							case kAudioHardwarePropertyDefaultInputDevice:
								manager->refreshDefaultInputDevice();
								break;
							case kAudioHardwarePropertyDefaultOutputDevice:
								manager->refreshDefaultOutputDevice();
								break;
						}
					}
				}
				
				return noErr;
			}
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Device Manager Initialization Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDeviceManager:: createManager()
{
	// No initialization necessary.
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Manager Destruction Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDeviceManager:: destroyManager()
{
	// No destruction necessary.
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Update Callback Registration Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDeviceManager:: registerDeviceUpdateCallbacks()
{
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioHardwarePropertyDevices,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster };
	
	//****************************************************************
	// Add the function that listens to changes in the connected audio devices.
	
	OSStatus error = AudioObjectAddPropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::devicesChangeCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_REGISTERING_DEVICE_CALLBACK );
		return false;
	}
	
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultInputDevice;
	
	// Add the function that listens to changes in the default audio input device.
	error = AudioObjectAddPropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::devicesChangeCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_REGISTERING_DEFAULT_INPUT_DEVICE_CALLBACK );
		return false;
	}
	
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
	
	// Add the function that listens to changes in the default audio output device.
	error = AudioObjectAddPropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::devicesChangeCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_REGISTERING_DEFAULT_OUTPUT_DEVICE_CALLBACK );
		return false;
	}
	
	//****************************************************************
	// Initialize the CoreAudio event run loop.
	
	propertyAddress.mSelector = kAudioHardwarePropertyRunLoop;
	
	CFRunLoopRef runLoop = NULL;
	
	// Tell the CoreAudio HAL to create its own run loop by passing a NULL run loop.
	error = AudioObjectSetPropertyData( kAudioObjectSystemObject, &propertyAddress, 0, NULL, sizeof(CFRunLoopRef), &runLoop );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_CORE_AUDIO_HAL_RUNLOOP );
		return false;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Update Callback Unregistration Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDeviceManager:: unregisterDeviceUpdateCallbacks()
{
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioHardwarePropertyDevices,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster };
	
	// Remove the function that listens to changes in the connected audio devices.
	OSStatus error = AudioObjectRemovePropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::devicesChangeCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_UNREGISTERING_DEVICE_CALLBACK );
		return false;
	}
	
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultInputDevice;
	
	// Remove the function that listens to changes in the default audio input device.
	error = AudioObjectRemovePropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::devicesChangeCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_UNREGISTERING_DEFAULT_INPUT_DEVICE_CALLBACK );
		return false;
	}
	
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
	
	// Remove the function that listens to changes in the default audio output device.
	error = AudioObjectRemovePropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::devicesChangeCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_UNREGISTERING_DEFAULT_OUTPUT_DEVICE_CALLBACK );
		return false;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Refresh Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDeviceManager:: refreshDevices()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	OSStatus error;
	
	//****************************************************************
	//	Get handles to all of the connected audio devices
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioHardwarePropertyDevices,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster };
	
	// Get the number of bytes needed to hold the buffer of devices
	::UInt32 numBytesInDeviceArray;
	error = AudioObjectGetPropertyDataSize( kAudioObjectSystemObject, &propertyAddress, 0, NULL, &numBytesInDeviceArray ); 
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_NUMBER_DEVICES );
		return false;
	}
	
	// Allocate a temporary array of device IDs.
	Size numDevices = numBytesInDeviceArray / sizeof(AudioDeviceID);
	AudioDeviceID* systemDevices = om::util::allocate<AudioDeviceID>( numDevices );
	
	// Get the device IDs in the array.
	error = AudioObjectGetPropertyData( kAudioObjectSystemObject, &propertyAddress, 0, NULL,
										&numBytesInDeviceArray, systemDevices );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEVICES );
		return false;
	}
	
	//****************************************************************
	// Check to see if any devices have been removed or added.
	
	ArrayList<SoundDeviceID> addedDevices;
	ArrayList<SoundDeviceID> removedDevices;
	
	// Find the devices that have been removed.
	for ( Index i = 0; i < devices.getSize(); )
	{
		Bool foundDevice = false;
		
		for ( Index j = 0; j < numDevices; j++ )
		{
			if ( devices[i] == systemDevices[j] )
			{
				foundDevice = true;
				break;
			}
		}
		
		if ( !foundDevice )
		{
			// Device no longer exists, notify the client and remove it from the array.
			removedDevices.add( devices[i] );
			devices.removeAtIndexUnordered( i );
			continue;
		}
		
		i++;
	}
	
	// Find the devices that have been added.
	for ( Index i = 0; i < numDevices; i++ )
	{
		SoundDeviceID newID( systemDevices[i] );
		
		if ( !devices.contains( newID ) )
		{
			addedDevices.add( newID );
			devices.add( newID );
		}
	}
	
	// Deallocate the temporary array of device IDs.
	om::util::deallocate( systemDevices );
		
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	
	//****************************************************************
	// Notify the callbacks that certain devices have been removed or added.
	
	delegateChangeMutex.lock();
	
	if ( delegate.deviceAdded.isSet() )
	{
		for ( Index i = 0; i < addedDevices.getSize(); i++ )
			delegate.deviceAdded( *this, addedDevices[i] );
	}
	
	if ( delegate.deviceRemoved.isSet() )
	{
		for ( Index i = 0; i < removedDevices.getSize(); i++ )
			delegate.deviceRemoved( *this, removedDevices[i] );
	}
	
	delegateChangeMutex.unlock();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Default Input Device Refresh Method
//############		
//##########################################################################################
//##########################################################################################




/// Refresh the current default input device if a notification was received that it changed.
Bool SoundDeviceManager:: refreshDefaultInputDevice()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	//****************************************************************
	//	Get a handle to the default input device
	
	::UInt32 propertySize = sizeof(AudioDeviceID);
	AudioDeviceID defaultInputDevice;
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioHardwarePropertyDefaultInputDevice,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster };
	
	OSStatus error = AudioObjectGetPropertyData( kAudioObjectSystemObject, &propertyAddress, 0, NULL,
												&propertySize, &defaultInputDevice );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_INPUT_DEVICE );
		return false;
	}
	
	// Find the default input device in the cached array of devices.
	for ( Index i = 0; i < devices.getSize(); i++ )
	{
		if ( devices[i] == SoundDeviceID(defaultInputDevice) )
		{
			defaultInputDeviceIndex = i;
			
			// Release a mutex which indicates that the number of devices is currently changing or being queried.
			deviceChangeMutex.unlock();
			
			return true;
		}
	}
	
	// Set the default input device to be the first if there was no default.
	defaultInputDeviceIndex = 0;
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Default Output Device Refresh Method
//############		
//##########################################################################################
//##########################################################################################




/// Refresh the current default output device if a notification was received that it changed.
Bool SoundDeviceManager:: refreshDefaultOutputDevice()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	//****************************************************************
	//	Get a handle to the default output device
	
	::UInt32 propertySize = sizeof(AudioDeviceID);
	AudioDeviceID defaultOutputDevice;
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioHardwarePropertyDefaultOutputDevice,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster };
	
	OSStatus error = AudioObjectGetPropertyData( kAudioObjectSystemObject, &propertyAddress, 0, NULL,
												&propertySize, &defaultOutputDevice );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_OUTPUT_DEVICE );
		return false;
	}
	
	// Find the default output device in the cached array of devices.
	for ( Index i = 0; i < devices.getSize(); i++ )
	{
		if ( devices[i] == SoundDeviceID(defaultOutputDevice) )
		{
			defaultOutputDeviceIndex = i;
			
			// Release a mutex which indicates that the number of devices is currently changing or being queried.
			deviceChangeMutex.unlock();
			
			return true;
		}
	}
	
	
	// Set the default output device to be the first if there was no default.
	defaultOutputDeviceIndex = 0;
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	return true;
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_APPLE
