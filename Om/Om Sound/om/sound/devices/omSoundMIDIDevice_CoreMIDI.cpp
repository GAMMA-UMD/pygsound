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


#if defined(OM_PLATFORM_APPLE)


// Include platform-specific headers
#include <CoreMIDI/CoreMIDI.h>
#include <CoreAudio/CoreAudio.h>


// 4-bit message type codes, from midi.org
#define MIDI_NOTE_OFF 8
#define MIDI_NOTE_ON 9
#define MIDI_AFTERTOUCH 10
#define MIDI_CONTROL_CHANGE 11
#define MIDI_PROGRAM_CHANGE 12
#define MIDI_CHANNEL_PRESSURE 13
#define MIDI_PITCH_WHEEL 14
#define MIDI_SYSTEM 15


// System message type codes.
#define MIDI_START 10
#define MIDI_CONTINUE 11
#define MIDI_STOP 14


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Local Helper Functions
//############		
//##########################################################################################
//##########################################################################################




static Bool convertCFStringToUTF8( CFStringRef string, UTF8String& output )
{
	// Determine how many bytes to allocate.
	CFIndex maxSize = CFStringGetMaximumSizeForEncoding( CFStringGetLength(string), kCFStringEncodingUTF8 );
	
	// Allocate a temorary buffer of characters to hold the string.
	Char* cString = om::util::allocate<Char>( maxSize );
	
	// Copy the string to the temorary buffer.
	Boolean result = CFStringGetCString( string, cString, maxSize, kCFStringEncodingUTF8 );
	
	// Create the UTF8 string.
	output = UTF8String( (const UTF8Char*)cString );
	
	// Deallocate the temporary buffer.
	om::util::deallocate( cString );
	
	return result;
}




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
				:	client( NULL ),
					inputPort( NULL ),
					outputPort( NULL )
			{
			}
			
			
		//********************************************************************************
		//******	Public Device Change Callback Method
			
			
			/// Respond to an event that occurred, indicating a change in the MIDI configuration.
			static void devicesChangeCallback( const MIDINotification* message, void* userData )
			{
				// Get a pointer to the device.
				MIDIDevice* device = (MIDIDevice*)userData;
				
				if ( device != NULL )
				{
					// Get the device's delegate.
					const MIDIDeviceDelegate& delegate = device->getDelegate();
					
					switch ( message->messageID )
					{
						case kMIDIMsgObjectAdded:
						{
							const MIDIObjectAddRemoveNotification* m = (const MIDIObjectAddRemoveNotification*)message;
							
							// Check to see if this device was removed. If so, notify the user.
							if ( m->child == device->deviceID )
							{
								if ( delegate.added.isSet() )
									delegate.added( *device );
							}
						}
						break;
						
						case kMIDIMsgObjectRemoved:
						{
							const MIDIObjectAddRemoveNotification* m = (const MIDIObjectAddRemoveNotification*)message;
							
							// Check to see if this device was removed. If so, notify the user.
							if ( m->child == device->deviceID )
							{
								if ( delegate.removed.isSet() )
									delegate.removed( *device );
							}
						}
						break;
					}
				}
			}
			
			
		//********************************************************************************
		//******	MIDI Input Callback Method
			
			
			/// Respond to an event that occurred, indicating a change in the MIDI configuration.
			static void midiInputCallback( const MIDIPacketList* packetList, void* userData, void* userData2 )
			{
				// Get a pointer to the device.
				MIDIDevice* device = (MIDIDevice*)userData;
				
				if ( device == NULL )
					return;
				
				// Get the device's delegate.
				const MIDIDeviceDelegate& delegate = device->getDelegate();
				
				// Don't bother decoding the packets if the delegate method is not set.
				if ( delegate.inputCallback.isNull() )
					return;
				
				// Create a buffer to hold the messages.
				MIDIBuffer messageBuffer;
				
				// Iterate over the MIDI packets.
				const Size numPackets = packetList->numPackets;
				const MIDIPacket* packet = &packetList->packet[0];
				
				for ( Index i = 0; i < numPackets; i++ )
				{
					const Size packetLength = packet->length;
					const UByte* packetData = (const UByte*)packet->data;
					Index byteIndex = 0;
					
					UInt64 currentHostTime = AudioGetCurrentHostTime();
					Time packetTime = Time::getCurrent();
					
					if ( packet->timeStamp > currentHostTime )
						packetTime += Time(Int64(AudioConvertHostTimeToNanos( packet->timeStamp - currentHostTime )));
					
					// Parse the MIDI data packet, one mesage for each loop.
					while ( byteIndex < packetLength )
					{
						MIDIMessage message;
						Size messageLength = 0;
						
						if ( MIDIMessage::fromBytes( packetData + byteIndex, message, messageLength ) )
						{
							messageBuffer.addEvent( MIDIEvent( message, packetTime ) );
							byteIndex += messageLength;
						}
						else
							byteIndex++;
					}
					
					packet = MIDIPacketNext( packet );
				}
				
				// Send the message buffer to the user.
				delegate.inputCallback( *device, messageBuffer );
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A handle to the MIDI client for this device.
			MIDIClientRef client;
			
			
			/// An object that represents a handle to the input port for this MIDI device.
			MIDIPortRef inputPort;
			
			
			/// An object that represents a handle to the output port for this MIDI device.
			MIDIPortRef outputPort;
			
			
			
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
	
	if ( !valid || !wrapper->client )
		return false;
	
	Bool success = true;
	
	if ( this->isInput() )
	{
		// Create an input port for the device.
		OSStatus status = MIDIInputPortCreate( wrapper->client, CFSTR("Rim Sound MIDI Input Port"),
												Wrapper::midiInputCallback, this, &wrapper->inputPort );
		
		// Make sure that no error occurred.
		if ( status != noErr )
			return false;
		
		// Connect the input port to the device.
		status = MIDIPortConnectSource( wrapper->inputPort, (MIDIEndpointRef)deviceID.getInternalID(), NULL );
		
		// Make sure that no error occurred.
		if ( status != noErr )
		{
			// If so, close the input port.
			MIDIPortDispose( wrapper->inputPort );
			wrapper->inputPort = NULL;
			success = false;
		}
	}
	
	if ( this->isOutput() )
	{
		// Create an input port for the device.
		OSStatus status = MIDIOutputPortCreate( wrapper->client, CFSTR("Rim Sound MIDI Output Port"),
												&wrapper->outputPort );
		
		// Make sure that no error occurred.
		if ( status != noErr )
			return false;
	}
	
	// Indicate that the device is currently running.
	running = success;
	
	return success;
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
	
	if ( this->isInput() )
	{
		// Disconnect the MIDI source.
		MIDIPortDisconnectSource( wrapper->inputPort, (MIDIEndpointRef)deviceID.getInternalID() );
		
		// Destroy the input port.
		MIDIPortDispose( wrapper->inputPort );
		wrapper->inputPort = NULL;
	}
	
	if ( this->isOutput() )
	{
		// Destroy the output port.
		MIDIPortDispose( wrapper->inputPort );
		wrapper->outputPort = NULL;
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
	if ( !this->isOutput() || !running || !wrapper->outputPort )
		return false;
	
	UByte data[3];
	Size messageLengthInBytes = 0;
	
	// Convert the MIDI message to a byte stream.
	if ( !MIDIMessage::toBytes( event.getMessage(), data, messageLengthInBytes ) )
		return false;
	
	// Put the message into a MIDI packet.
	MIDIPacketList packetList;
	packetList.numPackets = 1;
	
	// Compute the timestamp for the message.
	Time currentTime = Time::getCurrent();
	
	// Is the event scheduled for the future?
	if ( event.getTime() > currentTime )
	{
		// If so, compute the host time for the event.
		packetList.packet[0].timeStamp = AudioGetCurrentHostTime() +
										AudioConvertNanosToHostTime( (event.getTime() - currentTime).getNanoseconds() );
	}
	else
	{
		// Otherwise, play the event ASAP.
		packetList.packet[0].timeStamp = 0; 
	}
	
	packetList.packet[0].length = messageLengthInBytes;
	
	for ( Index i = 0; i < messageLengthInBytes; i++ )
		packetList.packet[0].data[i] = data[i];
	
	// Send the message to the output device.
	OSStatus result = MIDISend( wrapper->outputPort, (MIDIEndpointRef)deviceID.getInternalID(), &packetList );
	
	// Return whether or not the operation was successful.
	return result == noErr;
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
	
	/// Create a MIDI client which handles events for the device.
	OSStatus result = MIDIClientCreate( CFSTR("Rim Sound MIDI Device"), Wrapper::devicesChangeCallback, this, &wrapper->client );
	
	if ( result != noErr )
		return false;
	
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
	// Destroy the MIDI client.
	if ( wrapper->client )
		MIDIClientDispose( wrapper->client );
	
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
		return valid;
	
	
	
	
	
	
	
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
	// Don't use invalid devices.
	if ( !valid )
		return false;
	
	// Get a CF string for the device's human-readable name.
	CFStringRef nameString;
	
	MIDIObjectGetStringProperty( deviceID, kMIDIPropertyDisplayName, &nameString );
	
	// Convert the name string to a string format we can use.
	Bool result = convertCFStringToUTF8( nameString, name );
	
	// Release the name string.
	CFRelease( nameString );
	
	return result;
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
	// Don't use invalid devices.
	if ( !valid )
		return false;
	
	// Get a CF string for the device's manufacturer.
	CFStringRef manufacturerString;
	
	MIDIObjectGetStringProperty( deviceID, kMIDIPropertyManufacturer, &manufacturerString );
	
	// Convert the manufacturer string to a string format we can use.
	Bool result = convertCFStringToUTF8( manufacturerString, manufacturer );
	
	// Release the manufacturer string.
	CFRelease( manufacturerString );
	
	return result;
}





//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_APPLE
