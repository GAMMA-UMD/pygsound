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

#include "omSoundMIDIDeviceManager.h"


#if defined(OM_PLATFORM_APPLE)


// Include platform-specific headers
#include <CoreMIDI/CoreMIDI.h>


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




class MIDIDeviceManager:: Wrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new default-initialized MIDI device manager wrapper.
			OM_INLINE Wrapper()
				:	client( NULL ),
					runLoop( NULL )
			{
			}
			
			
		//********************************************************************************
		//******	Public Device Change Callback Method
			
			
			/// Respond to an event that occurred, indicating a change in the MIDI configuration.
			static void devicesChangeCallback( const MIDINotification* message, void* userData )
			{
				// Get a pointer to the manager.
				MIDIDeviceManager* manager = (MIDIDeviceManager*)userData;
				
				if ( manager != NULL )
				{
					switch ( message->messageID )
					{
						case kMIDIMsgSetupChanged:
							manager->refreshDevices();
							break;
					}
				}
			}
			
			
			/// A method called when entering the thread which handles device update events.
			void deviceUpdateThreadEntry( MIDIDeviceManager* manager )
			{
				/// Create a MIDI client which handles events for the device manager.
				OSStatus status = MIDIClientCreate( CFSTR("Rim Sound MIDI Device Manager"),
													devicesChangeCallback, manager, &client );
				
				// Signal that the MIDI client was created.
				clientCreatedSignal.signal();
				
				// Make sure the client was created successfully.
				if ( status != noErr )
					return;
				
				// Store a reference to this thread's run loop object.
				runLoop = CFRunLoopGetCurrent();
				
				// Make sure that the run loop is not prematurely destroyed.
				CFRetain( runLoop );
				
				// Start the run loop. This runs until we call CFRunLoopStop().
				CFRunLoopRun();
				
				// Release the reference to the run loop.
				CFRelease( runLoop );
				runLoop = NULL;
				
				// Release the MIDI client object.
				MIDIClientDispose( client );
				client = NULL;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A handle to the MIDI client which monitors for device add/remove events.
			MIDIClientRef client;
			
			
			/// A thread that is created to listen to device add/remove events.
			FunctionThread<void(MIDIDeviceManager*)> runLoopThread;
			
			
			/// A thread signal that is used to signal when a MIDI client has been created on the device listening thread.
			Signal clientCreatedSignal;
			
			
			/// A run loop which dispatches events related to MIDI devices.
			CFRunLoopRef runLoop;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Device Manager Initialization Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDeviceManager:: createManager()
{
	wrapper = om::util::construct<Wrapper>();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Manager Destruction Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDeviceManager:: destroyManager()
{
	om::util::destruct( wrapper );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Update Callback Registration Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDeviceManager:: registerDeviceUpdateCallbacks()
{
	// Don't bother if the run loop thread has already been created.
	if ( wrapper->runLoopThread.isRunning() )
		return true;
	
	// Start the run loop thread.
	wrapper->runLoopThread.start( FunctionCall<void(MIDIDeviceManager*)>(
								Function<void(MIDIDeviceManager*)>( &Wrapper::deviceUpdateThreadEntry, wrapper ), this ) );
	
	// Wait for the MIDI client to be created.
	// Not sure why this is necessary, but if we don't do this, the MIDI client isn't able
	// to add its source to the run loop for the run loop thread.
	wrapper->clientCreatedSignal.wait();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Update Callback Unregistration Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDeviceManager:: unregisterDeviceUpdateCallbacks()
{
	// Stop the currently running run loop thread.
	if ( wrapper->runLoopThread.isRunning() && wrapper->runLoop != NULL )
	{
		CFRunLoopStop( wrapper->runLoop );
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




Bool MIDIDeviceManager:: refreshDevices()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	// Keep a list of the added and removed devices so that we can notify the user of the changes.
	ArrayList<MIDIDeviceID> addedDevices;
	ArrayList<MIDIDeviceID> removedDevices;
	
	//****************************************************************
	// Get handles to all of the connected MIDI devices
	
	// Get the number of source devices there are (inputs).
	const ItemCount numSources = MIDIGetNumberOfSources();
	
	// Get the number of destination devices there are (outputs).
	const ItemCount numDestinations = MIDIGetNumberOfDestinations();
	
	// Find any input devices that have been removed from the system
	// by iterating over the current list of devices and finding the ones
	// that are no longer present in the list of devices.
	for ( Index i = 0; i < inputDevices.getSize(); )
	{
		const MIDIDeviceID& deviceID = inputDevices[i];
		Bool foundDevice = false;
		
		// Find the device for this ID in the list of sources.
		for ( ItemCount j = 0; j < numSources; j++ )
		{
			if ( deviceID == MIDIDeviceID( MIDIGetSource( j ), true, false ) )
				foundDevice = true;
		}
		
		// If the device was not found, indicate to the user that the device was removed.
		if ( !foundDevice )
		{
			removedDevices.add( deviceID );
			inputDevices.removeAtIndexUnordered( i );
			continue;
		}
		
		i++;
	}
	
	// Find any output devices that have been removed from the system
	// by iterating over the current list of devices and finding the ones
	// that are no longer present in the list of devices.
	for ( Index i = 0; i < outputDevices.getSize(); )
	{
		const MIDIDeviceID& deviceID = outputDevices[i];
		Bool foundDevice = false;
		
		// Find the device for this ID in the list of destinations.
		for ( ItemCount j = 0; j < numDestinations; j++ )
		{
			if ( deviceID == MIDIDeviceID( MIDIGetDestination(j), false, true ) )
				foundDevice = true;
		}
		
		// If the device was not found, indicate to the user that the device was removed.
		if ( !foundDevice )
		{
			removedDevices.add( deviceID );
			outputDevices.removeAtIndexUnordered( i );
			continue;
		}
		
		i++;
	}
	
	// Iterate over the source devices and update the list of devices.
	for ( ItemCount i = 0; i < numSources; i++ )
	{
		MIDIDeviceID deviceID( MIDIGetSource( i ), true, false );
		
		// If the device was not found, indicate to the user that the device was added.
		if ( !inputDevices.contains( deviceID ) )
		{
			addedDevices.add( deviceID );
			inputDevices.add( deviceID );
			continue;
		}
	}
	
	// Iterate over the destination devices and update the list of devices.
	for ( ItemCount i = 0; i < numDestinations; i++ )
	{
		MIDIDeviceID deviceID( MIDIGetDestination( i ), false, true );
		
		// If the device was not found, indicate to the user that the device was added.
		if ( !outputDevices.contains( deviceID ) )
		{
			addedDevices.add( deviceID );
			outputDevices.add( deviceID );
			continue;
		}
	}
	
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




Bool MIDIDeviceManager:: refreshDefaultInputDevice()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
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




Bool MIDIDeviceManager:: refreshDefaultOutputDevice()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
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