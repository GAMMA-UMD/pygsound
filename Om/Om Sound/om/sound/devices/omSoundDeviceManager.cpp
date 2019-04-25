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


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundDeviceManager:: SoundDeviceManager()
	:	defaultInputDeviceIndex( 0 ),
		defaultOutputDeviceIndex( 0 ),
		hasCachedDevices( false ),
		wrapper( NULL )
{
	createManager();
	registerDeviceUpdateCallbacks();
}




SoundDeviceManager:: SoundDeviceManager( const SoundDeviceManager& other )
	:	defaultInputDeviceIndex( 0 ),
		defaultOutputDeviceIndex( 0 ),
		hasCachedDevices( false ),
		wrapper( NULL )
{
	createManager();
	registerDeviceUpdateCallbacks();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundDeviceManager:: ~SoundDeviceManager()
{
	//unregisterDeviceUpdateCallbacks();
	
	destroyManager();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundDeviceManager& SoundDeviceManager:: operator = ( const SoundDeviceManager& other )
{
	if ( this != &other )
	{
		unregisterDeviceUpdateCallbacks();
		destroyManager();
		
		defaultInputDeviceIndex = 0;
		defaultOutputDeviceIndex = 0;
		devices.clear();
		hasCachedDevices = false;
		wrapper = NULL;
		
		createManager();
		registerDeviceUpdateCallbacks();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Update Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundDeviceManager:: refresh()
{
	// Refresh the devices.
	refreshDevices();
	refreshDefaultInputDevice();
	refreshDefaultOutputDevice();
	
	hasCachedDevices = true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundDeviceManager:: getDeviceCount() const
{
	// Make sure that the list of available devices has been cached.
	if ( !hasCachedDevices )
		const_cast<SoundDeviceManager*>(this)->cacheDevices();
	
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	Size numDevices = devices.getSize();
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	return numDevices;
}




SoundDeviceID SoundDeviceManager:: getDeviceID( Index deviceIndex ) const
{
	// Make sure that the list of available devices has been cached.
	if ( !hasCachedDevices )
		const_cast<SoundDeviceManager*>(this)->cacheDevices();
	
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	if ( deviceIndex >= devices.getSize() )
		return SoundDeviceID::INVALID_DEVICE;
	
	// Determine the device ID for the requested device.
	SoundDeviceID deviceID = devices[deviceIndex];
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	return deviceID;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Default Device Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundDeviceID SoundDeviceManager:: getDefaultInputDeviceID() const
{
	// Make sure that the list of available devices has been cached.
	if ( !hasCachedDevices )
		const_cast<SoundDeviceManager*>(this)->cacheDevices();
	
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	// Determine whether or not the device at that index is valid.
	Bool validDeviceIndex = defaultInputDeviceIndex < devices.getSize();
	
	// Determine the device ID for the requested device.
	SoundDeviceID deviceID = validDeviceIndex ? devices[defaultInputDeviceIndex] : 
												SoundDeviceID::INVALID_DEVICE;
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	return deviceID;
}




SoundDeviceID SoundDeviceManager:: getDefaultOutputDeviceID() const
{
	// Make sure that the list of available devices has been cached.
	if ( !hasCachedDevices )
		const_cast<SoundDeviceManager*>(this)->cacheDevices();
	
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	// Determine whether or not the device at that index is valid.
	Bool validDeviceIndex = defaultOutputDeviceIndex < devices.getSize();
	
	// Determine the device ID for the requested device.
	SoundDeviceID deviceID = validDeviceIndex ? devices[defaultOutputDeviceIndex] : 
												SoundDeviceID::INVALID_DEVICE;
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	return deviceID;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Delegate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundDeviceManager:: setDelegate( const SoundDeviceManagerDelegate& newDelegate )
{
	// Lock the mutex which avoids unsafe access to the delegate methods for this device manager.
	delegateChangeMutex.lock();
	
	// Store the new delegate.
	delegate = newDelegate;
	
	// Unlock the mutex which avoids unsafe access to the delegate methods for this device manager.
	delegateChangeMutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Cache Method
//############		
//##########################################################################################
//##########################################################################################




void SoundDeviceManager:: cacheDevices()
{
	if ( !hasCachedDevices )
	{
		// Refresh internal lists which store the lists of available devices.
		refreshDevices();
		refreshDefaultInputDevice();
		refreshDefaultOutputDevice();
		
		hasCachedDevices = true;
	}
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
