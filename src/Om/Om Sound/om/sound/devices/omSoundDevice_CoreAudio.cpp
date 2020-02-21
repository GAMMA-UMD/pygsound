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

#include "omSoundDevice.h"


#if defined(OM_PLATFORM_APPLE)


// Include platform-specific headers
#include <CoreServices/CoreServices.h>
#include <CoreAudio/CoreAudio.h>
#include <unistd.h>
#include <AudioUnit/AudioUnit.h>


#define ERROR_CORE_AUDIO_HAL_RUNLOOP "An error was encountered while creating a CoreAudio HAL run loop."


#define ERROR_STARTING_DEVICE_IO "An error was encountered while trying to start audio IO for a device."
#define ERROR_STOPPING_DEVICE_IO "An error was encountered while trying to stop audio IO for a device."
#define ERROR_REGISTERING_DEVICE_CALLBACK "An error was encountered while registering an audio device change callback."
#define ERROR_REGISTERING_DEVICE_IO_CALLBACK "An error was encountered while registering the audio IO callback for a device."
#define ERROR_REGISTERING_STREAM_CONFIGURATION_CALLBACK "An error was encountered while registering an audio device stream configuration change callback."
#define ERROR_UNREGISTERING_DEVICE_CALLBACK "An error was encountered while unregistering an audio device change callback."
#define ERROR_UNREGISTERING_DEVICE_IO_CALLBACK "An error was encountered while unregistering the audio IO callback for a device."
#define ERROR_UNREGISTERING_STREAM_CONFIGURATION_CALLBACK "An error was encountered while unregistering an audio device stream configuration change callback."
#define ERROR_QUERYING_DEVICES "An error was encountered while querying the connected audio devices."
#define ERROR_QUERYING_NUMBER_DEVICES "An error was encountered while querying the number of connected audio devices."
#define ERROR_QUERYING_DEVICE "An error was encountered while querying a connected audio device."
#define ERROR_QUERYING_DEFAULT_INPUT_DEVICE "An error was encountered while querying the default input audio device."
#define ERROR_QUERYING_DEFAULT_OUTPUT_DEVICE "An error was encountered while querying the default output audio device."
#define ERROR_QUERYING_NATIVE_SAMPLE_RATES "An error was encountered while querying an audio device for available sample rates."
#define ERROR_QUERYING_NUMBER_NATIVE_SAMPLE_RATES "An error was encountered while querying an audio device for the number of available sample rates."
#define ERROR_QUERYING_INPUT_STREAM_CONFIGURATION "An error was encountered while getting a device's input stream configuration."
#define ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION "An error was encountered while getting a device's output stream configuration."
#define ERROR_QUERYING_DEVICE_NAME "An error was encountered while querying the name of an audio device."
#define ERROR_QUERYING_DEVICE_MANUFACTURER "An error was encountered while querying the name of an audio device's manufacturer."
#define ERROR_QUERYING_DEVICE_INPUT_CHANNEL_NAME "An error was encountered while querying the name of an audio input channel."
#define ERROR_QUERYING_DEVICE_OUTPUT_CHANNEL_NAME "An error was encountered while querying the name of an audio output channel."
#define ERROR_QUERYING_SAMPLE_RATE "An error was encountered while querying an audio device's current sample rate."
#define ERROR_QUERYING_SAMPLE_RATE_WRITE_STATUS "An error was encountered while determining whether or not the sample\
 rate for an audio device can be changed."
#define ERROR_QUERYING_LATENCY "An error was encountered while querying an audio device's current latency."


#define REPORT_ERROR( X ) (Console << X)


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		String Conversion Helper Method
//############		
//##########################################################################################
//##########################################################################################




static void convertCFStringToUTF8( CFStringRef string, UTF8String& result )
{
	// Try to get the string efficiently as a UTF-16 string.
	const ::UniChar* utf16 = CFStringGetCharactersPtr( string );
	
	if ( utf16 )
		result = UTF8String( (const UTF16Char*)utf16 );
	else
	{
		// Get the length of the string as UTF-16.
		CFIndex utf16Length = CFStringGetLength( string );
		
		// Allocate a temporary buffer of UTF-16 characters.
		::UniChar* tempBuffer = util::allocate< ::UniChar >( utf16Length );
		
		// Copy the characters into the temporary buffer.
		CFStringGetCharacters( string, CFRangeMake( 0, utf16Length ), tempBuffer );
		
		// Create a new string.
		result = UTF8String( (const UTF16Char*)tempBuffer, utf16Length );
		
		// Deallocate the temporary buffer.
		util::deallocate( tempBuffer );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Device Wrapper Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundDevice:: Wrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE Wrapper()
				:	ioProcID( 0 ),
					hasLastFrameTime( false ),
					lastFrameTime()
			{
			}
			
			
		//********************************************************************************
		//******	Public Device Change Callback Method
			
			
			static OSStatus deviceEventCallback( AudioObjectID deviceID, ::UInt32 numChangedProperties,
												const AudioObjectPropertyAddress properties[], void* clientData )
			{
				SoundDevice* device = (SoundDevice*)clientData;
				
				if ( device != NULL )
				{
					// Make sure that this event is meant for this device pointer.
					if ( deviceID != kAudioObjectSystemObject && deviceID != device->deviceID )
						return noErr;
					
					for ( Index i = 0; i < numChangedProperties; i++ )
					{
						switch ( properties[i].mSelector )
						{
							case kAudioHardwarePropertyDevices:
								{
									// Enumerate the connected devices and see if this device is still connected.
									device->refreshDeviceStatus();
								}
							case kAudioDevicePropertyStreamConfiguration:
								{
									if ( properties[i].mScope == kAudioDevicePropertyScopeInput )
									{
										// Update the input stream configuration.
										device->refreshInputStreamConfiguration();
									}
									else if ( properties[i].mScope == kAudioDevicePropertyScopeOutput )
									{
										// Update the output stream configuration.
										device->refreshOutputStreamConfiguration();
									}
								}
						}
					}
				}
				
				return noErr;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The internal ID of the CoreAudio IO procedure.
			AudioDeviceIOProcID ioProcID;
			
			
			// A boolean value indicating whether or not the timestamp of the last processing frame is saved.
			Bool hasLastFrameTime;
			
			
			// The current system time of the start of the last processing frame.
			Time lastFrameTime;
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Audio IO Callback Methods
			
			/// A function that handles providing data to the CoreAudio device driver.
			static OSStatus audioIOCallback( AudioDeviceID deviceID, const AudioTimeStamp* currentTime,
											const AudioBufferList* inputData, const AudioTimeStamp* inputTime,
											AudioBufferList* outputData, const AudioTimeStamp* outputTime,
											void* clientData );
			
			
			friend class SoundDevice;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Output Start/Stop Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: start()
{
	// If the device is already running or is invalid, there is nothing to be done.
	if ( running )
		return true;
	
	if ( !valid )
		return false;
	
	// Register the audio output callback function
	OSStatus error = AudioDeviceCreateIOProcID( deviceID,
												Wrapper::audioIOCallback, this, &wrapper->ioProcID );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_REGISTERING_DEVICE_IO_CALLBACK );
		return false;
	}
	
	// Start outputing audio.
	error = AudioDeviceStart( deviceID, wrapper->ioProcID );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_STARTING_DEVICE_IO );
		return false;
	}
	
	// Set a flag which indicates that the device is currently running.
	running = true;
	
	return true;
}




Bool SoundDevice:: stop()
{
	// If the device isn't currently running or is invalid, there is nothing to be done.
	if ( !running )
		return true;
	
	if ( !valid )
		return false;
	
	// Stop outputing audio.
	OSStatus error = AudioDeviceStop( deviceID, wrapper->ioProcID );
	
	if ( error == kAudioHardwareBadDeviceError )
	{
		// The device was probably removed and the method is called from the destructor.
		running = false;
		return false;
	}
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_STOPPING_DEVICE_IO );
		return false;
	}
	
	// Unregister the audio output callback function.
	error = AudioDeviceDestroyIOProcID( deviceID, wrapper->ioProcID );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_UNREGISTERING_DEVICE_IO_CALLBACK );
		return false;
	}
	
	running = false;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Input Channel Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String SoundDevice:: getInputChannelName( Index inputChannelIndex ) const
{
	// If the device is invalid or channel index is output of bounds, do nothing and return an empty name.
	if ( !valid || inputChannelIndex >= numInputChannels )
		return UTF8String();
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioObjectPropertyElementName,
		kAudioDevicePropertyScopeInput,
		inputChannelIndex + 1 };
	
	// If channel names are not supported, return a placeholder name.
	if ( !AudioObjectHasProperty( deviceID, &propertyAddress ) )
		return UTF8String("Input ") + inputChannelIndex;
	
	//**************************************************************************
	// Get the name of this input channel.
	
	CFStringRef channelName = NULL;
	::UInt32 dataSize = sizeof(CFStringRef);
	OSStatus error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL, &dataSize, &channelName );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_INPUT_CHANNEL_NAME );
		return UTF8String("Input ") + inputChannelIndex;
	}
	
	// Convert the name to a UTF8String.
	UTF8String channelNameUTF8;
	convertCFStringToUTF8( channelName, channelNameUTF8 );
	
	// Release the string.
	CFRelease( channelName );
	
	// Check to make sure that the driver didn't return an empty string.
	if ( channelNameUTF8 == "" )
		return UTF8String("Input ") + inputChannelIndex;
	
	return channelNameUTF8;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Channel Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String SoundDevice:: getOutputChannelName( Index outputChannelIndex ) const
{
	// If the device is invalid or channel index is output of bounds, do nothing and return an empty name.
	if ( !valid || outputChannelIndex >= numOutputChannels )
		return UTF8String();
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioObjectPropertyElementName,
		kAudioDevicePropertyScopeOutput,
		outputChannelIndex + 1 };
	
	// If channel names are not supported, return a placeholder name.
	if ( !AudioObjectHasProperty( deviceID, &propertyAddress ) )
		return UTF8String("Output ") + outputChannelIndex;
	
	//**************************************************************************
	// Get the name of this output channel.
	
	CFStringRef channelName = NULL;
	::UInt32 dataSize = sizeof(CFStringRef);
	OSStatus error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL, &dataSize, &channelName );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_OUTPUT_CHANNEL_NAME );
		return UTF8String("Output ") + outputChannelIndex;
	}
	
	// Convert the name to a UTF8String.
	UTF8String channelNameUTF8;
	convertCFStringToUTF8( channelName, channelNameUTF8 );
	
	// Release the string.
	CFRelease( channelName );
	
	// Check to make sure that the driver didn't return an empty string.
	if ( channelNameUTF8 == "" )
		return UTF8String("Output ") + outputChannelIndex;
	
	return channelNameUTF8;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Rate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SampleRate SoundDevice:: getSampleRate() const
{
	// If the device is invalid, do nothing and return a sample rate of 0.
	if ( !valid )
		return SampleRate(0);
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioDevicePropertyNominalSampleRate,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster };
	
	::UInt32 propertySize = sizeof(::Float64);
	::Float64 sampleRate;
	
	// Query the current sample rate of the device.
	OSStatus error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
												&propertySize, &sampleRate );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_SAMPLE_RATE );
		return SampleRate(0);
	}
	
	return SampleRate(sampleRate);
}




Bool SoundDevice:: setSampleRate( SampleRate newSampleRate )
{
	// If the device is invalid, do nothing and return FALSE.
	if ( !valid )
		return false;
	
	// Acquire the mutex which indicates that audio is currently being output.
	ioMutex.lock();
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioDevicePropertyNominalSampleRate,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster };
	
	// Make sure that the sample rate of the device can be changed.
	::Boolean isWritable;
	
	OSStatus error = AudioObjectIsPropertySettable( deviceID, &propertyAddress, &isWritable );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_SAMPLE_RATE_WRITE_STATUS );
		return false;
	}
	
	// If the sample rate of the device cannot be changed, return false.
	if ( !isWritable )
		return false;
	
	// Convert the incoming sample rate to the necessary type.
	::Float64 sampleRate = ::Float64(newSampleRate);
	
	// Try setting the sample rate of the device to the new value.
	error = AudioObjectSetPropertyData( deviceID, &propertyAddress, 0, NULL,
										sizeof(::Float64), &sampleRate );
	
	// Release the mutex which indicates that audio is currently being output.
	ioMutex.unlock();
	
	if ( error == kAudioHardwareNoError )
	{
		// The sample rate change operation was successful, inform the sample rate converter object.
		resampler.setOutputSampleRate( newSampleRate );
		
		return true;
	}
	else
		return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Latency Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Time SoundDevice:: getInputLatency() const
{
	// If the device is invalid, do nothing and return a latency of 0.
	if ( !valid )
		return Time();
	
	//********************************************************************
	// Get the device's input latency.
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioDevicePropertyLatency,
		kAudioDevicePropertyScopeInput,
		kAudioObjectPropertyElementMaster };
	
	::UInt32 propertySize = sizeof(::UInt32);
	::UInt32 deviceSampleLatency = 0;
	
	// Query the current input latency of the device.
	OSStatus error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
												&propertySize, &deviceSampleLatency );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	//********************************************************************
	// Get the device's safety offset.
	
	propertyAddress.mSelector = kAudioDevicePropertySafetyOffset;
	propertySize = sizeof(::UInt32);
	::UInt32 deviceSafetyOffset = 0;
	
	// Query the current input latency of the device.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&propertySize, &deviceSafetyOffset );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	//********************************************************************
	// Get the stream's latency.
	
	// Get a stream ID from the device to get the latency.
	propertyAddress.mSelector = kAudioDevicePropertyStreams;
	propertySize = sizeof(AudioStreamID);
	AudioStreamID streamID;
	
	::UInt32 streamSampleLatency = 0;
	
	// Query the current input stream latency of the device.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&propertySize, &streamID );
	
	if ( error == kAudioHardwareNoError )
	{
		propertyAddress.mSelector = kAudioStreamPropertyLatency;
		
		if ( AudioObjectHasProperty( streamID, &propertyAddress ) )
		{
			propertySize = sizeof(::UInt32);
			error = AudioObjectGetPropertyData( streamID, &propertyAddress, 0, NULL,
												&propertySize, &streamSampleLatency );
		}
	}
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	//********************************************************************
	// Get the buffer frame size latency.
	
	propertyAddress.mSelector = kAudioDevicePropertyBufferFrameSize;
	propertySize = sizeof(UInt32);
	::UInt32 bufferSampleLatency;
	
	// Query the current output stream latency of the device.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&propertySize, &bufferSampleLatency );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	return Time( (deviceSampleLatency + deviceSafetyOffset + streamSampleLatency + bufferSampleLatency) / this->getSampleRate() );
}




Time SoundDevice:: getOutputLatency() const
{
	// If the device is invalid, do nothing and return a latency of 0.
	if ( !valid )
		return Time();
	
	//********************************************************************
	// Get the device's output latency.
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress = {
		kAudioDevicePropertyLatency,
		kAudioDevicePropertyScopeOutput,
		kAudioObjectPropertyElementMaster };
	
	::UInt32 propertySize = sizeof(::UInt32);
	::UInt32 deviceSampleLatency = 0;
	
	// Query the current input latency of the device.
	OSStatus error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
												&propertySize, &deviceSampleLatency );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	//********************************************************************
	// Get the device's safety offset.
	
	propertyAddress.mSelector = kAudioDevicePropertySafetyOffset;
	propertySize = sizeof(::UInt32);
	::UInt32 deviceSafetyOffset = 0;
	
	// Query the current input latency of the device.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&propertySize, &deviceSafetyOffset );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	//********************************************************************
	// Get the stream's latency.
	
	// Get a stream ID from the device to get the latency.
	propertyAddress.mSelector = kAudioDevicePropertyStreams;
	propertySize = sizeof(AudioStreamID);
	AudioStreamID streamID;
	
	::UInt32 streamSampleLatency = 0;
	
	// Query the current output stream latency of the device.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&propertySize, &streamID );
	
	if ( error == kAudioHardwareNoError )
	{
		propertyAddress.mSelector = kAudioStreamPropertyLatency;
		
		if ( AudioObjectHasProperty( streamID, &propertyAddress ) )
		{
			propertySize = sizeof(::UInt32);
			error = AudioObjectGetPropertyData( streamID, &propertyAddress, 0, NULL,
												&propertySize, &streamSampleLatency );
		}
	}
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	//********************************************************************
	// Get the buffer frame size latency.
	
	propertyAddress.mSelector = kAudioDevicePropertyBufferFrameSize;
	propertySize = sizeof(UInt32);
	::UInt32 bufferSampleLatency;
	
	// Query the current output stream latency of the device.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&propertySize, &bufferSampleLatency );
	
	// Make sure that no error has occurred.
	if ( error != kAudioHardwareNoError )
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
	
	return Time( (deviceSampleLatency + deviceSafetyOffset + streamSampleLatency + bufferSampleLatency) / this->getSampleRate() );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Is Default Input Device Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: isDefaultInput() const
{
	// If the device is not valid, it can't be the default input.
	if ( !valid )
		return false;
	
	//****************************************************************
	//	Get a handle to the default input device
	
	::UInt32 propertySize = sizeof(AudioDeviceID);
	AudioDeviceID defaultInputDevice;
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultInputDevice;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	OSStatus error = AudioObjectGetPropertyData( kAudioObjectSystemObject, &propertyAddress, 0, NULL,
												&propertySize, &defaultInputDevice );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_INPUT_DEVICE );
		return false;
	}
	
	return deviceID == defaultInputDevice;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Is Default Output Device Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: isDefaultOutput() const
{
	// If the device is not valid, it can't be the default output.
	if ( !valid )
		return false;
	
	//****************************************************************
	//	Get a handle to the default output device
	
	::UInt32 propertySize = sizeof(AudioDeviceID);
	AudioDeviceID defaultOutputDevice;
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	OSStatus error = AudioObjectGetPropertyData( kAudioObjectSystemObject, &propertyAddress, 0, NULL,
												&propertySize, &defaultOutputDevice );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_OUTPUT_DEVICE );
		return false;
	}
	
	return deviceID == defaultOutputDevice;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Data Initialization Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: createDevice()
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




Bool SoundDevice:: destroyDevice()
{
	// Destroy the wrapper object.
	if ( wrapper != NULL )
		om::util::destruct( wrapper );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Status Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: refreshDeviceStatus()
{
	// Mark the device as invalid to start.
	valid = false;
	
	// If the device ID is invalid, then the device must be invalid.
	if ( deviceID == SoundDeviceID::INVALID_DEVICE )
		return valid;
	
	//****************************************************************
	// Find the device ID in the list of connected devices.
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioHardwarePropertyDevices;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	// Get the number of bytes needed to hold the buffer of devices
	::UInt32 numBytesInDeviceArray;
	
	OSStatus error = AudioObjectGetPropertyDataSize( kAudioObjectSystemObject, &propertyAddress, 0, NULL, &numBytesInDeviceArray ); 
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
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
		REPORT_ERROR( ERROR_QUERYING_DEVICES );
		return false;
	}
	
	// Check to see if this device's ID exists in the array of connected devices.
	for ( Index i = 0; i < numDevices; i++ )
	{
		if ( deviceID == systemDevices[i] )
		{
			valid = true;
			break;
		}
	}
	
	// Deallocate the temporary array of device IDs.
	om::util::deallocate(systemDevices);
	
	return valid;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Input Stream Configuration Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: refreshInputStreamConfiguration()
{
	// Start by setting the number of channels to 0.
	numInputChannels = 0;
	
	// Don't refresh anything for invalid devices.
	if ( !valid )
		return false;
	
	//**************************************************************************
	// Get the number of input channels that the device has.
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioDevicePropertyStreamConfiguration;
	propertyAddress.mScope = kAudioDevicePropertyScopeInput;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	// Allocate a temporary audio buffer list to hold the stream configuration.
	::UInt32 bufferListSize = sizeof(AudioBufferList);
	
	// Get the output stream configuration.
	OSStatus error = AudioObjectGetPropertyDataSize( deviceID, &propertyAddress, 0, NULL,
													&bufferListSize );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_INPUT_STREAM_CONFIGURATION );
		return false;
	}
	
	// Allocate a temporary audio buffer list to hold the stream configuration.
	AudioBufferList* bufferList = (AudioBufferList*)om::util::allocate<UByte>( bufferListSize );
	
	// Get the input stream configuration.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&bufferListSize, bufferList );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		// Deallocate the temporary stream configuration buffer.
		om::util::deallocate( bufferList );
		
		REPORT_ERROR( ERROR_QUERYING_INPUT_STREAM_CONFIGURATION );
		return false;
	}
	
	// For each stream of the device, add the number of channels in that stream to
	// the total number of channels.
	for ( Index i = 0; i < bufferList->mNumberBuffers; i++ )
		numInputChannels += bufferList->mBuffers[i].mNumberChannels;
	
	// Deallocate the temporary stream configuration buffer.
	om::util::deallocate( bufferList );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Output Stream Configuration Update Method
//############		
//##########################################################################################
//##########################################################################################





Bool SoundDevice:: refreshOutputStreamConfiguration()
{
	// Start by setting the number of channels to 0.
	numOutputChannels = 0;
	
	// Don't refresh anything for invalid devices.
	if ( !valid )
		return false;
	
	//**************************************************************************
	// Get the number of output channels that the device has.
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioDevicePropertyStreamConfiguration;
	propertyAddress.mScope = kAudioDevicePropertyScopeOutput;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	// Allocate a temporary audio buffer list to hold the stream configuration.
	::UInt32 bufferListSize = sizeof(AudioBufferList);
	
	// Get the output stream configuration.
	OSStatus error = AudioObjectGetPropertyDataSize( deviceID, &propertyAddress, 0, NULL,
													&bufferListSize );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION );
		return false;
	}
	
	// Allocate a temporary audio buffer list to hold the stream configuration.
	AudioBufferList* bufferList = (AudioBufferList*)om::util::allocate<UByte>( bufferListSize );
	
	// Get the output stream configuration.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&bufferListSize, bufferList );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		// Deallocate the temporary stream configuration buffer.
		om::util::deallocate( bufferList );
		
		REPORT_ERROR( ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION );
		return false;
	}
	
	// For each stream of the device, add the number of channels in that stream to
	// the total number of channels.
	for ( Index i = 0; i < bufferList->mNumberBuffers; i++ )
		numOutputChannels += bufferList->mBuffers[i].mNumberChannels;
	
	// Deallocate the temporary stream configuration buffer.
	om::util::deallocate( bufferList );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Native Sample Rates Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: refreshNativeSampleRates()
{
	// Make sure that the list of native sample rates is empty.
	nativeSampleRates.clear();
	
	// Don't refresh anything for invalid devices.
	if ( !valid )
		return false;
	
	//**************************************************************************
	// Get the native sample rates for this device
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioDevicePropertyAvailableNominalSampleRates;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	// Get the number of samples rates.
	::UInt32 propertySize;
	
	OSStatus error = AudioObjectGetPropertyDataSize( deviceID, &propertyAddress, 0, NULL,
													&propertySize );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_NUMBER_NATIVE_SAMPLE_RATES );
		return false;
	}
	
	Size numSamplesRates = propertySize / sizeof(AudioValueRange);
	
	// Allocate an array of value range structures to hold the valid device sample rates.
	AudioValueRange* sampleRates = om::util::allocate<AudioValueRange>( numSamplesRates );
	
	// Query the native sample rates for the device.
	error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL,
										&propertySize, sampleRates );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		// Deallocate the array of sample rates.
		om::util::deallocate( sampleRates );
		
		REPORT_ERROR( ERROR_QUERYING_NATIVE_SAMPLE_RATES );
		return false;
	}
	
	// Add all sample rates in the temporary array to the list of native sample rates.
	for ( Index i = 0; i < numSamplesRates; i++ )
	{
		// Use the minimum sample rate value, not sure why these are specified as a range.
		nativeSampleRates.add( sampleRates[i].mMinimum );
	}
	
	// Deallocate the array of sample rates.
	om::util::deallocate( sampleRates );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Name Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: refreshName()
{
	// Don't refresh anything for invalid devices.
	if ( !valid )
	{
		name = UTF8String();
		return false;
	}
	
	//**************************************************************************
	// Get the name of this device.
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioDevicePropertyDeviceNameCFString;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	// Get the device's name.
	CFStringRef deviceName = NULL;
	::UInt32 dataSize = sizeof(CFStringRef);
	OSStatus error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL, &dataSize, &deviceName );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	// Convert the name to a UTF8String.
	convertCFStringToUTF8( deviceName, name );
	
	// Release the string.
	CFRelease( deviceName );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Manufacturer Name Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: refreshManufacturer()
{
	// Don't refresh anything for invalid devices.
	if ( !valid )
	{
		manufacturer = UTF8String();
		return false;
	}
	
	//**************************************************************************
	// Get the name of this device's manufacturer.
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioDevicePropertyDeviceManufacturerCFString;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	// Get the device's name.
	CFStringRef manufacturerName = NULL;
	::UInt32 dataSize = sizeof(CFStringRef);
	OSStatus error = AudioObjectGetPropertyData( deviceID, &propertyAddress, 0, NULL, &dataSize, &manufacturerName );
	
	// Make sure that there was no error.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_MANUFACTURER );
		return false;
	}
	
	// Convert the name to a UTF8String.
	convertCFStringToUTF8( manufacturerName, manufacturer );
	
	// Release the string.
	CFRelease( manufacturerName );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Update Callback Registration Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundDevice:: registerDeviceUpdateCallbacks()
{
	return false;
	
	if ( !valid )
		return false;
	
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioHardwarePropertyDevices;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	//****************************************************************
	// Add the function that listens to changes in the connected audio devices.
	
	OSStatus error = AudioObjectAddPropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::deviceEventCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_REGISTERING_DEVICE_CALLBACK );
		return false;
	}
	
	//****************************************************************
	// Add the function that listens to changes in the device's IO stream configurations.
	
	propertyAddress.mSelector = kAudioDevicePropertyStreamConfiguration;
	propertyAddress.mScope = kAudioDevicePropertyScopeInput;
	
	error = AudioObjectAddPropertyListener( deviceID, &propertyAddress, Wrapper::deviceEventCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_REGISTERING_STREAM_CONFIGURATION_CALLBACK );
		return false;
	}
	
	propertyAddress.mScope = kAudioDevicePropertyScopeOutput;
	
	error = AudioObjectAddPropertyListener( deviceID, &propertyAddress, Wrapper::deviceEventCallback, this );
	
	// Make sure that an error has not been encountered.
	if ( error != kAudioHardwareNoError )
	{
		REPORT_ERROR( ERROR_REGISTERING_STREAM_CONFIGURATION_CALLBACK );
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




Bool SoundDevice:: unregisterDeviceUpdateCallbacks()
{
	// Create an object that indicates the type of property that we are accessing.
	AudioObjectPropertyAddress propertyAddress;
	propertyAddress.mSelector = kAudioHardwarePropertyDevices;
	propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAddress.mElement = kAudioObjectPropertyElementMaster;
	
	// Remove the function that listens to changes in the connected audio devices.
	OSStatus error = AudioObjectRemovePropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::deviceEventCallback, this );
	
	// Make sure that an error has not been encountered.
	OM_ASSERT_MESSAGE_CODE( error == kAudioHardwareNoError, ERROR_UNREGISTERING_DEVICE_CALLBACK, error );
	
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultInputDevice;
	
	// Remove the function that listens to changes in the default audio input device.
	error = AudioObjectRemovePropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::deviceEventCallback, this );
	
	// Make sure that an error has not been encountered.
	OM_ASSERT_MESSAGE_CODE( error == kAudioHardwareNoError, ERROR_UNREGISTERING_STREAM_CONFIGURATION_CALLBACK, error );
	
	propertyAddress.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
	
	// Remove the function that listens to changes in the default audio output device.
	error = AudioObjectRemovePropertyListener( kAudioObjectSystemObject, &propertyAddress, Wrapper::deviceEventCallback, this );
	
	// Make sure that an error has not been encountered.
	OM_ASSERT_MESSAGE_CODE( error == kAudioHardwareNoError, ERROR_UNREGISTERING_STREAM_CONFIGURATION_CALLBACK, error );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Audio Output Buffer Copy Methods
//############		
//##########################################################################################
//##########################################################################################




static void copyToOutputBuffer( const SoundBuffer& inputBuffer, Index inputStartIndex, Size numInputSamples,
								AudioBufferList* outputData, Index outputStartIndex )
{
	const Size numInputChannels = inputBuffer.getChannelCount();
	
	// Place each input channel into the output buffer in interleaved format.
	// Find the buffer corresponding to this output channel index.
	for ( Index i = 0, c = 0; i < outputData->mNumberBuffers; i++ )
	{
		Size numBufferChannels = outputData->mBuffers[i].mNumberChannels;
		Index bufferStartOffset = outputStartIndex*numBufferChannels;
		
		for ( Index j = 0; j < numBufferChannels; j++, c++ )
		{
			// Check to see if there are enough channels in the client-provided buffer.
			if ( c < numInputChannels )
			{
				// Write data from the client output buffer.
				const Sample32f* input = inputBuffer.getChannel(c) + inputStartIndex;
				const Sample32f* const inputEnd = input + numInputSamples;
				Sample32f* output = (Sample32f*)outputData->mBuffers[i].mData + j + bufferStartOffset;
				
				while ( input != inputEnd )
				{
					*output = *input;
					output += numBufferChannels;
					input++;
				}
			}
			else
			{
				// Write zeros to the rest of the output buffer channels.
				Sample32f* output = (Sample32f*)outputData->mBuffers[i].mData + j + bufferStartOffset;
				Sample32f* outputEnd = output + numBufferChannels*numInputSamples;
				
				while ( output < outputEnd )
				{
					*output = Sample32f(0);
					output += numBufferChannels;
				}
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Audio Output Buffer Zero Methods
//############		
//##########################################################################################
//##########################################################################################




static void zeroOutputBuffer( AudioBufferList* outputData, Index outputStartIndex, Size numSamples )
{
	// Place each input channel into the output buffer in interleaved format.
	// Find the buffer corresponding to this output channel index.
	for ( Index i = 0, c = 0; i < outputData->mNumberBuffers; i++ )
	{
		Size numBufferChannels = outputData->mBuffers[i].mNumberChannels;
		Index bufferStartOffset = outputStartIndex*numBufferChannels;
		
		for ( Index j = 0; j < numBufferChannels; j++, c++ )
		{
			// Write zeros to the rest of the output buffer channels.
			Sample32f* output = (Sample32f*)outputData->mBuffers[i].mData + j + bufferStartOffset;
			Sample32f* outputEnd = output + numBufferChannels*numSamples;
			
			while ( output < outputEnd )
			{
				*output = Sample32f(0);
				output += numBufferChannels;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Audio Output Callback Method
//############		
//##########################################################################################
//##########################################################################################




OSStatus SoundDevice:: Wrapper:: audioIOCallback( AudioDeviceID deviceID,
											const AudioTimeStamp* timeStamp,
											const AudioBufferList* inputData, const AudioTimeStamp* inputTime,
											AudioBufferList* outputData, const AudioTimeStamp* outputTime,
											void* clientData )
{
	// If there is no client data pointer, return. (this shouldn't happen)
	if ( clientData == NULL )
		return noErr;
	
	// Get a pointer to the device object for which we are producing audio.
	SoundDevice* device = (SoundDevice*)clientData;
	Wrapper* wrapper = device->wrapper;
	
	// If the device isn't currently running, do nothing and return from the function.
	if ( !device->isRunning() )
		return noErr;
	
	// Acquire the mutex which indicates that audio is currently being sent or received.
	device->ioMutex.lock();
	
	// Get the absolute time at the start of the frame.
	Time frameStartTime = Time::getCurrent();
	
	SampleRate deviceSampleRate = device->getSampleRate();
	const SoundDeviceDelegate& delegate = device->getDelegate();
	
	// Get the number of input and output channels there are for this device.
	const Size numInputChannels = device->getInputChannelCount();
	const Size numOutputChannels = device->getOutputChannelCount();
	
	//************************************************************************************
	// Check to see if the input function object is NULL, if so, skip sending input to the client.
	
	if ( delegate.inputCallback.isSet() && numInputChannels > 0 )
	{
		SoundBuffer* inputBuffer = &device->ioBuffer;
		
		// Calculate the number of samples to read from the input source.
		Size numSamplesToInput = inputData->mBuffers[0].mDataByteSize /
									(inputData->mBuffers[0].mNumberChannels*sizeof(Sample32f));
		
		// Make sure that the input buffer has the right sample rate.
		if ( inputBuffer->getSampleRate() != deviceSampleRate )
			inputBuffer->setSampleRate( deviceSampleRate );
		
		// Make sure that the input buffer is large enough to hold all of the samples/channels.
		if ( inputBuffer->getSize() < numSamplesToInput )
			inputBuffer->setSize( numSamplesToInput );
		
		// Make sure that the input buffer is large enough to hold all of the samples/channels.
		if ( inputBuffer->getChannelCount() != numInputChannels )
			inputBuffer->setChannelCount( numInputChannels );
		
		// Copy the samples from the CoreAudio interleaved buffers to another buffer.
		for ( Index c = 0; c < numInputChannels; )
		{
			for ( Index i = 0; i < inputData->mNumberBuffers; i++ )
			{
				Size numBufferChannels = inputData->mBuffers[i].mNumberChannels;
				
				for ( Index j = 0; j < numBufferChannels; j++, c++ )
				{
					const Sample32f* source = ((const Sample32f*)inputData->mBuffers[i].mData) + j;
					const Sample32f* const sourceEnd = source + numSamplesToInput*numBufferChannels;
					Sample32f* destination = inputBuffer->getChannel(c);
					
					while ( source != sourceEnd )
					{
						*destination = *source;
						source += numBufferChannels;
						destination++;
					}
				}
			}
		}
		
		// Send the new input samples to the input callback function.
		delegate.inputCallback( *device, *inputBuffer, numSamplesToInput, frameStartTime );
	}
	
	
	//************************************************************************************
	// Check to see if the output function object is NULL, if so, abort output.
	
	Size numSamplesToOutput = 0;
	
	if ( delegate.outputCallback.isSet() && numOutputChannels > 0 )
	{
		// Calculate the number of samples to read from the input source.
		numSamplesToOutput = outputData->mBuffers[0].mDataByteSize /
										(outputData->mBuffers[0].mNumberChannels*sizeof(Sample32f));
		
		Size numSamplesWritten = 0;
		
		while ( numSamplesWritten < numSamplesToOutput )
		{
			Size numSamplesRemaining = numSamplesToOutput - numSamplesWritten;
			
			// Check to see if there are old samples in the sample rate conversion buffer.
			if ( device->samplesInConverterBuffer > 0 )
			{
				Size numConverterSamples = math::min( device->samplesInConverterBuffer, numSamplesRemaining );
				
				copyToOutputBuffer( device->sampleRateConversionBuffer, device->converterBufferStart, numConverterSamples,
									outputData, numSamplesWritten );
				
				numSamplesWritten += numConverterSamples;
				
				// Did we use all of the samples in the buffer? If so, reset the valid sample locations.
				if ( numConverterSamples == device->samplesInConverterBuffer )
				{
					device->converterBufferStart = 0;
					device->samplesInConverterBuffer = 0;
				}
				else
				{
					device->converterBufferStart += numConverterSamples;
					device->samplesInConverterBuffer -= numConverterSamples;
				}
			}
			else
			{
				Size numSamplesToRequest = numSamplesRemaining;
				SoundBuffer* outputBuffer = &device->ioBuffer;
				
				// Make sure that the output buffer has the right sample rate.
				if ( outputBuffer->getSampleRate() != deviceSampleRate )
					outputBuffer->setSampleRate( deviceSampleRate );
				
				// Make sure that the output buffer is large enough to hold all of the samples/channels.
				if ( outputBuffer->getSize() < numSamplesToRequest )
					outputBuffer->setSize( numSamplesToRequest );
				
				// Make sure that the output buffer is large enough to hold all of the samples/channels.
				if ( outputBuffer->getChannelCount() != numOutputChannels )
					outputBuffer->setChannelCount( numOutputChannels );
				
				//************************************************************************************
				// Get the output audio from the client and potentially convert its sample rate.
				
				// Zero the output buffer so that there won't be garbage played if the client doesn't write anything.
				outputBuffer->zero( 0, numSamplesToRequest );
				
				// Request the desired number of samples from the output callback function.
				Size numOutputSamples = delegate.outputCallback( *device, *outputBuffer, numSamplesToRequest, frameStartTime );
				
				// Detect when the client doesn't provide any more audio.
				if ( numOutputSamples == 0 )
				{
					// Write zeros to the rest of the buffer and return from the function.
					zeroOutputBuffer( outputData, numSamplesWritten, numSamplesToRequest );
					break;
				}
				
				// Check to see if the data was provided in a different sample rate than the device's rate.
				if ( outputBuffer->getSampleRate() != deviceSampleRate )
				{
					// If so, convert the audio in the input buffer to the device's sample rate.
					// Make sure that the sample rate converter has the correct sample rate.
					device->resampler.setOutputSampleRate( deviceSampleRate );
					
					Size numConverterSamples = device->resampler.process( *outputBuffer,
																device->sampleRateConversionBuffer, numOutputSamples );
					
					// Change the pointer to the input buffer so that it points to the sample rate converter output buffer.
					outputBuffer = &device->sampleRateConversionBuffer;
					
					if ( numConverterSamples > numSamplesToRequest )
					{
						device->samplesInConverterBuffer = numConverterSamples - numSamplesToRequest;
						device->converterBufferStart = numOutputSamples;
					}
					else
						numOutputSamples = numConverterSamples;
				}
				
				//************************************************************************************
				// Copy the output buffer to the CoreAudio output buffer channels.
				
				// Place each input channel into the output buffer in interleaved format.
				copyToOutputBuffer( *outputBuffer, 0, numOutputSamples, outputData, numSamplesWritten );
				
				numSamplesWritten += numOutputSamples;
			}
		}
	}
	
	// Get the current absolute time.
	Time currentTime = Time::getCurrent();
	
	// Update the current timing information.
	if ( wrapper->hasLastFrameTime && numSamplesToOutput > 0 )
	{
		// Update the CPU usage for this frame.
		Double frameTime = currentTime - frameStartTime;
		Double bufferTime = numSamplesToOutput / deviceSampleRate;
		
		device->currentCPUUsage = Float(frameTime / bufferTime);
		
		// Compute the envelope parameters for the CPU usage averaging.
		const Float attackTime = 0.01f;
		const Float releaseTime = 0.5f;
		const Float envelopeAttack = math::pow( Float(0.1), Float(1.0/(attackTime / bufferTime)) );
		const Float envelopeRelease = math::pow( Float(0.1), Float(1.0/(releaseTime / bufferTime)) );
		
		// Update the average CPU usage.
		if ( device->currentCPUUsage > device->averageCPUUsage )
			device->averageCPUUsage = device->currentCPUUsage + envelopeAttack*(device->averageCPUUsage - device->currentCPUUsage);
		else
			device->averageCPUUsage = device->currentCPUUsage + envelopeRelease*(device->averageCPUUsage - device->currentCPUUsage);
		
		// Compute the time for a buffer, with a little extra to be sure we skipped a frame.
		const Time bufferTimeThreshold( 1.99*bufferTime );
		
		// Notify the user of an overload if the time between the end of the last frame and the end
		// of this one exceeds the real-time requirements based on the buffer size/sample rate.
		if ( delegate.processOverload.isSet() && (currentTime - wrapper->lastFrameTime) > bufferTimeThreshold )
			delegate.processOverload( *device );
	}
	else
	{
		// No samples were processed or doesn't have a previous time, so set the current CPU usage to 0.
		device->currentCPUUsage = 0;
	}
	
	wrapper->lastFrameTime = currentTime;
	wrapper->hasLastFrameTime = true;
	
	// Release the mutex which indicates that audio is currently being output.
	device->ioMutex.unlock();
	
	return noErr;
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_APPLE
