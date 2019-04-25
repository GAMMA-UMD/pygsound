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


#if defined(OM_PLATFORM_WINDOWS)

// Include platform-specific headers
#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include <mmsystem.h>

#define UTF8_FROM_TCHAR( string ) \
			(sizeof(TCHAR) == sizeof(UTF8Char) ? UTF8String((const UTF8Char*)string) : \
			sizeof(TCHAR) == sizeof(UTF16Char) ? UTF8String((const UTF16Char*)string) : \
			sizeof(TCHAR) == sizeof(UTF32Char) ? UTF8String((const UTF32Char*)string) : \
			UTF8String()) 


#define REPORT_ERROR( X ) (Console << X)


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		MIDI Device Wrapper Class Definition
//############		
//##########################################################################################
//##########################################################################################




class MIDIDevice:: Wrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE Wrapper()
				:	inputDevice( NULL ),
					outputDevice( NULL )
			{
			}
			
			
		//********************************************************************************
		//******	MIDI Input Callback Method
			
			
			/// A callback method that is called whenever MIDI input is detected.
			static void CALLBACK midiInputCallback( HMIDIIN inputDeviceHandle, UINT messageType, DWORD_PTR userData,
													DWORD parameter1, DWORD parameter2 )
			{
				MIDIDevice* device = (MIDIDevice*)userData;
				
				if ( device == NULL )
					return;
				
				// Get the device's delegate and wrapper.
				Wrapper* wrapper = device->wrapper;
				const MIDIDeviceDelegate& delegate = device->getDelegate();
				
				if ( messageType == MIM_DATA )
				{
					// The MIDI message packed into a 32-bit value.
					UInt32 midiMessage = parameter1;
					
					// Timestamp of the message as the number of milliseconds since the call to midiInStart().
					UInt32 timestamp = parameter2;
					
					UByte messageData[3];
					messageData[0] = (midiMessage & 0xFF);
					messageData[1] = (midiMessage & 0xFF00) >> 8;
					messageData[2] = (midiMessage & 0xFF0000) >> 16;
					
					MIDIMessage message;
					Size messageLength = 0;
					
					if ( MIDIMessage::fromBytes( messageData, message, messageLength ) )
					{
						// Create a MIDI buffer to hold the message.
						MIDIBuffer eventBuffer;
						
						// Compute the absolute time of this MIDI event.
						Time eventTime = wrapper->startTime + Time( Int64(timestamp)*Int64(1000000) );
						
						eventBuffer.addEvent( MIDIEvent( message, eventTime ) );
						
						// Send the MIDI buffer to the device delegate.
						if ( delegate.inputCallback.isSet() )
							delegate.inputCallback( *device, eventBuffer );
					}
				}
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A handle to the input device that this device represents.
			HMIDIIN inputDevice;
			
			
			/// A handle to the output device that this device represents.
			HMIDIOUT outputDevice;
			
			
			/// The absolute time at which the MIDI device was started.
			Time startTime;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Device Start Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: start()
{
	// If the device is already running or is invalid, there is nothing to be done.
	if ( running )
		return true;
	
	if ( !valid )
		return false;
	
	if ( this->isInput() )
	{
		// Open the input device.
		MMRESULT result = midiInOpen( &wrapper->inputDevice, deviceID.getInternalID(),
									(DWORD_PTR)Wrapper::midiInputCallback, (DWORD_PTR)this, CALLBACK_FUNCTION );
		
		if ( result != MMSYSERR_NOERROR )
			return false;
		
		// Start the input device.
		result = midiInStart( wrapper->inputDevice );
		
		// Check to make sure there was no error.
		if ( result != MMSYSERR_NOERROR )
		{
			midiInClose( wrapper->inputDevice );
			wrapper->inputDevice = NULL;
			return false;
		}
		
		// Get the current system time.
		wrapper->startTime = Time::getCurrent();
	}
	
	
	if ( this->isOutput() )
	{
		MMRESULT result = midiOutOpen( &wrapper->outputDevice, deviceID.getInternalID(),
									NULL, NULL, CALLBACK_NULL );
		
		if ( result != MMSYSERR_NOERROR )
			return false;
	}
	
	// Indicate that the device is currently running.
	running = true;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Stop Method
//############		
//##########################################################################################
//##########################################################################################




void MIDIDevice:: stop()
{
	// Don't bother stopping the device if it is not valid or not running.
	if ( !valid || !running )
		return;
	
	if ( this->isInput() && wrapper->inputDevice != NULL )
	{
		midiInClose( wrapper->inputDevice );
		wrapper->inputDevice = NULL;
	}
	
	
	if ( this->isOutput() && wrapper->outputDevice != NULL )
	{
		midiOutClose( wrapper->outputDevice );
		wrapper->outputDevice = NULL;
	}
	
	// Indicate that the device is no longer running.
	running = false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		MIDI Output Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: write( const MIDIEvent& event )
{
	if ( !this->isOutput() || !running || wrapper->outputDevice == NULL )
		return false;
	
	UByte data[3];
	Size messageLengthInBytes = 0;
	
	// Convert the MIDI message to a byte stream.
	if ( !MIDIMessage::toBytes( event.getMessage(), data, messageLengthInBytes ) )
		return false;
	
	// Pack the bytes into a DWORD.
	DWORD packedMessage = 0;
	
	if ( messageLengthInBytes > 0 )
		packedMessage |= DWORD(data[0]);
	
	if ( messageLengthInBytes > 1 )
		packedMessage |= (DWORD(data[1]) << 8);
	
	if ( messageLengthInBytes > 2 )
		packedMessage |= (DWORD(data[2]) << 16);
	
	// Send the message to the output device.
	MMRESULT result = midiOutShortMsg( wrapper->outputDevice, packedMessage );
	
	// Return whether or not the operation was successful.
	return result == MMSYSERR_NOERROR;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Data Initialization Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: createDevice()
{
	// Create a new wrapper object.
	wrapper = om::util::construct<Wrapper>();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Data Destruction Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: destroyDevice()
{
	// Destroy the wrapper object.
	if ( wrapper != NULL )
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




Bool MIDIDevice:: registerDeviceUpdateCallbacks()
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Update Callback Unregistration Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: unregisterDeviceUpdateCallbacks()
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Status Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: refreshDeviceStatus()
{
	// Mark the device as invalid to start.
	valid = false;
	
	// If the device ID is invalid, then the device must be invalid.
	if ( deviceID == MIDIDeviceID::INVALID_DEVICE )
		return false;
	
	if ( deviceID.isInput() )
	{
		if ( deviceID.getInternalID() >= midiInGetNumDevs() )
			return false;
	}
	
	if ( deviceID.isOutput() )
	{
		if ( deviceID.getInternalID() >= midiOutGetNumDevs() )
			return false;
	}
	
	valid = true;
	
	return valid;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Name Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: refreshName()
{
	if ( deviceID.isInput() )
	{
		MIDIINCAPS deviceData;
		MMRESULT result = midiInGetDevCaps( deviceID.getInternalID(), &deviceData, sizeof(MIDIINCAPS) );
		
		if ( result != MMSYSERR_NOERROR )
			return false;
		
		name = UTF8_FROM_TCHAR( deviceData.szPname );
	}
	
	if ( deviceID.isOutput() )
	{
		MIDIOUTCAPS deviceData;
		MMRESULT result = midiOutGetDevCaps( deviceID.getInternalID(), &deviceData, sizeof(MIDIOUTCAPS) );
		
		if ( result != MMSYSERR_NOERROR )
			return false;
		
		name = UTF8_FROM_TCHAR( deviceData.szPname );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Manufacturer Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool MIDIDevice:: refreshManufacturer()
{
	return true;
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_WINDOWS
