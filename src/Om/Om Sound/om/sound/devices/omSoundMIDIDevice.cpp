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

#include "omSoundMIDIDevice.h"


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




MIDIDevice:: MIDIDevice( const MIDIDeviceID& newDeviceID )
	:	deviceID( newDeviceID ),
		wrapper( NULL ),
		valid( false ),
		running( false )
{
	// Initialize data about this device.
	initializeDeviceData();
}




MIDIDevice:: MIDIDevice( const MIDIDevice& other )
	:	deviceID( other.deviceID ),
		wrapper( NULL ),
		valid( false ),
		running( false )
{
	// Use the same source of output audio for this device object.
	this->setDelegate( other.getDelegate() );
	
	// Initialize data about this device.
	initializeDeviceData();
	
	// If the other device object was outputing audio, start outputing audio from this device.
	if ( other.isRunning() )
		this->start();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




MIDIDevice:: ~MIDIDevice()
{
	// Stop outputing audio if the device is currently running.
	if ( running )
		this->stop();
	
	// Unregister callbacks that notify this class when the device's properties change.
	this->unregisterDeviceUpdateCallbacks();
	
	// Clean up platform-specific data.
	destroyDevice();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




MIDIDevice& MIDIDevice:: operator = ( const MIDIDevice& other )
{
	if ( this != &other )
	{
		// Stop outputing audio if the device is currently running.
		if ( running )
			this->stop();
		
		// Unregister callbacks that notify this class when the device's properties change.
		this->unregisterDeviceUpdateCallbacks();
		
		// Clean up platform-specific data.
		destroyDevice();
		
		//*********************************************************************
		
		// Store the new device ID.
		deviceID = other.deviceID;
		
		// Use the same source of output audio for this device object.
		this->setDelegate( other.getDelegate() );
		
		// Initialize data about the device.
		initializeDeviceData();
		
		// If the other device object was outputing audio, start outputing audio from this device.
		if ( other.isRunning() )
			this->start();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Running Status Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: isRunning() const
{
	return running;
}




//##########################################################################################
//##########################################################################################
//############		
//############		MIDI Buffer Output Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: write( const MIDIBuffer& eventBuffer )
{
	if ( !this->isOutput() )
		return false;
	
	Bool result = true;
	const Size numEvents = eventBuffer.getEventCount();
	
	for ( Index i = 0; i < numEvents; i++ )
		result &= this->write( eventBuffer.getEvent(i) );
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Delegate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void MIDIDevice:: setDelegate( const MIDIDeviceDelegate& newDelegate )
{
	// Acquire the mutex which keeps us from setting the callback when audio is being sent to the device.
	ioMutex.lock();
	
	// Store the new delegate.
	delegate = newDelegate;
	
	// Release the mutex which keeps us from setting the callback when audio is being sent to the device.
	ioMutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Data Initialization Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: initializeDeviceData()
{
	Bool result = true;
	
	// Initialize the device.
	result &= this->createDevice();
	
	// Determine whether or not this sound device is valid.
	result &= this->refreshDeviceStatus();
	
	// Get the name of this device.
	result &= this->refreshName();
	
	// Get the name of this device's manufacturer.
	result &= this->refreshManufacturer();
	
	// Register callbacks that notify this class when the device's properties change.
	result &= this->registerDeviceUpdateCallbacks();
	
	return result;
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

