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


#if defined(OM_PLATFORM_WINDOWS)


// Include platform-specific headers
#define NOMINMAX

#include <Windows.h>
#include <MmDeviceapi.h>
#include <Audioclient.h>
#include <Audiopolicy.h>
#include <Functiondiscoverykeys_devpkey.h>

#define ERROR_CREATING_DRIVER_THREAD "An error was encountered while creating an audio device driver thread"
#define ERROR_CREATING_SIGNAL_EVENT "An error was encountered while creating a signaling event object."
#define ERROR_CREATING_DEVICE_PROPERTY_STORE "An error was encountered while querying an audio device property store."
#define ERROR_CREATING_DEVICE_ENUMERATOR "An error was encountered while creating an audio device enumerator object."
#define ERROR_CREATING_AUDIO_CLIENT "An error was encountered while creating an audio client for an audio device."
#define ERROR_INITIALIZING_AUDIO_CLIENT "An error was encountered while intializing an audio client object."
#define ERROR_GETTING_INPUT_CLIENT "An error was encountered while getting an interface to the audio input system"
#define ERROR_GETTING_OUTPUT_CLIENT "An error was encountered while getting an interface to the audio output system"
#define ERROR_WAITING_FOR_OUTPUT_BUFFER "An error was encountered while waiting for a free audio output buffer."
#define ERROR_GETTING_INPUT_PACKET_SIZE "An error was encountered while querying the size of an input audio packet."
#define ERROR_GETTING_BUFFER_SIZE "An error was encountered while querying the size of an audio output buffer."
#define ERROR_GETTING_BUFFER_PADDING "An error was encountered while querying the current padding of an audio output buffer."
#define ERROR_GETTING_BUFFER_FROM_DEVICE "An error was encountered while getting an audio output buffer from the device."
#define ERROR_RELEASING_BUFFER_TO_DEVICE "An error was encountered while releasing an audio output buffer to the device."
#define ERROR_UNSUPPORTED_SAMPLE_TYPE "Unsupported sample type for sound device."



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
#define ERROR_QUERYING_DEVICE_NAME_LENGTH "An error was encountered while querying the length of the name of an audio device."
#define ERROR_QUERYING_DEVICE_MANUFACTURER "An error was encountered while querying the name of an audio device's manufacturer."
#define ERROR_QUERYING_DEVICE_MANUFACTURER_LENGTH "An error was encountered while querying the length of the name of an audio device's manufacturer."
#define ERROR_QUERYING_DEVICE_INPUT_CHANNEL_NAME "An error was encountered while querying the name of an audio input channel."
#define ERROR_QUERYING_DEVICE_INPUT_CHANNEL_NAME_LENGTH "An error was encountered while querying the length of the name of an audio input channel."
#define ERROR_QUERYING_DEVICE_OUTPUT_CHANNEL_NAME "An error was encountered while querying the name of an audio output channel."
#define ERROR_QUERYING_DEVICE_OUTPUT_CHANNEL_NAME_LENGTH "An error was encountered while querying the length of the name of an audio output channel."
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
//############		Sound Output Device Wrapper Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundDevice:: Wrapper
{
	public:
		
		//********************************************************************************
		//******	Public Class Declarations
			
			class DeviceChangeNotifier;
			
			
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new sound device wrapper, initializing all internal state to NULL.
			OM_INLINE Wrapper()
				:	deviceEnumerator( NULL ),
					device( NULL ),
					audioClient( NULL ),
					audioRenderClient( NULL ),
					audioCaptureClient( NULL ),
					deviceChangeNotifier( NULL ),
					driverThread( NULL ),
					bufferFreeEvent( NULL )
			{
			}
			
			
		//********************************************************************************
		//******	Public Methods
			
			
			/// Clean up any device related state.
			void releaseDevice()
			{
				// Release the audio client object.
				if ( audioClient != NULL )
				{
					audioClient->Release();
					audioClient = NULL;
				}
				
				// Release the device.
				if ( device != NULL )
				{
					device->Release();
					device = NULL;
				}
				
				// Release the device enumerator.
				if ( deviceEnumerator != NULL )
				{
					deviceEnumerator->Release();
					deviceEnumerator = NULL;
				}
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A class that enumerates all of the connected system audio devices.
			IMMDeviceEnumerator* deviceEnumerator;
			
			
			/// A pointer to the system audio device that is being used.
			IMMDevice* device;
			
			
			/// A client interface for the device which handles most interactions with the device.
			IAudioClient* audioClient;
			
			
			/// While the device IO thread is running, this points to the current audio rendering client object.
			IAudioRenderClient* audioRenderClient;
			
			
			/// While the device IO thread is running, this points to the current audio capturing client object.
			IAudioCaptureClient* audioCaptureClient;
			
			
			/// A pointer to a class that notifies the device if it is removed or changes configuration.
			DeviceChangeNotifier* deviceChangeNotifier;
			
			
			/// A handle to the thread which drives the audio rendering pipeline.
			HANDLE driverThread;
			
			
			/// A handle to an event object that signals when an output buffer is available.
			HANDLE bufferFreeEvent;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			friend class SoundDevice;
			
			
		//********************************************************************************
		//******	Private Static Audio IO Callback Methods
			
			
			/// The entry function for the windows audio device rendering thread.
			static DWORD WINAPI driverThreadEntry( LPVOID lpParam );
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Device Change Notifier Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundDevice:: Wrapper:: DeviceChangeNotifier : public IMMNotificationClient
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			DeviceChangeNotifier( SoundDevice* newDevice )
				:	referenceCount( 1 ),
					device( newDevice )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			~DeviceChangeNotifier()
			{
			}
			
			
		//********************************************************************************
		//******	IUnknown Object Management Methods
			
			
			ULONG STDMETHODCALLTYPE AddRef()
			{
				return InterlockedIncrement(&referenceCount);
			}
			
			
			ULONG STDMETHODCALLTYPE Release()
			{
				ULONG ulRef = InterlockedDecrement(&referenceCount);
				
				if ( ulRef == 0 )
					delete this;
				
				return ulRef;
			}
			
			
			HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, VOID **ppvInterface )
			{
				if (IID_IUnknown == riid)
				{
					AddRef();
					*ppvInterface = (IUnknown*)this;
				}
				else if (__uuidof(IMMNotificationClient) == riid)
				{
					AddRef();
					*ppvInterface = (IMMNotificationClient*)this;
				}
				else
				{
					*ppvInterface = NULL;
					return E_NOINTERFACE;
				}
				
				return S_OK;
			}
			
			
		//********************************************************************************
		//******	Device Event Callback Methods
			
			
			HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged( EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId )
			{
				return S_OK;
			}
			
			
			HRESULT STDMETHODCALLTYPE OnDeviceAdded( LPCWSTR pwstrDeviceId )
			{
				return S_OK;
			}
			
			
			HRESULT STDMETHODCALLTYPE OnDeviceRemoved( LPCWSTR deviceID )
			{
				if ( device != NULL && wcscmp( (const wchar_t*)device->getID().getIDString().getPointer(), deviceID ) == 0 )
					device->refreshDeviceStatus();
				
				return S_OK;
			}
			
			
			HRESULT STDMETHODCALLTYPE OnDeviceStateChanged( LPCWSTR deviceID, DWORD dwNewState )
			{
				if ( device != NULL && wcscmp( (const wchar_t*)device->getID().getIDString().getPointer(), deviceID ) == 0 )
					device->refreshDeviceStatus();
				
				return S_OK;
			}
			
			
			HRESULT STDMETHODCALLTYPE OnPropertyValueChanged( LPCWSTR pwstrDeviceId, const PROPERTYKEY key )
			{
				return S_OK;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of references that exist to this callback object.
			LONG referenceCount;
			
			
			/// A pointer to the object that is the device for this device change notifier.
			SoundDevice* device;
			
			
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
	
	//****************************************************************
	// Create an event object that signals when an output buffer is free.
	
	wrapper->bufferFreeEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	
	if ( wrapper->bufferFreeEvent == NULL )
	{
		REPORT_ERROR( ERROR_CREATING_SIGNAL_EVENT );
		return false;
	}
	
	// Register the event with the audio client so that it signals the event whenever a buffer
	// become available.
	HRESULT result = wrapper->audioClient->SetEventHandle( wrapper->bufferFreeEvent );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( "An error was encountered while registering an audio output buffer free signal event." );
		return false;
	}
	
	//****************************************************************
	// Get an interface to the audio IO systems from the audio client interface.
	
	// Get an interface to the audio input system for this device.
	if ( this->isInput() )
	{
		result = wrapper->audioClient->GetService( __uuidof(IAudioCaptureClient), (void**)&wrapper->audioCaptureClient );
		
		OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_GETTING_INPUT_CLIENT, result );
	}
	else
		wrapper->audioCaptureClient = NULL;
	
	// Get an interface to the audio output system for this device.
	if ( this->isOutput() )
	{
		result = wrapper->audioClient->GetService( __uuidof(IAudioRenderClient), (void**)&wrapper->audioRenderClient );
		
		OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_GETTING_OUTPUT_CLIENT, result );
	}
	else
		wrapper->audioRenderClient = NULL;
	
	//****************************************************************
	// Create the driver thread and start the IO stream.
	
	wrapper->driverThread = CreateThread( NULL, 0, Wrapper::driverThreadEntry, (LPVOID)this, 0, NULL );
	
	OM_ASSERT_MESSAGE( wrapper->driverThread != NULL, ERROR_CREATING_DRIVER_THREAD );
	
	// Make the driver thread a time-critical thread with high priority.
	SetThreadPriority( wrapper->driverThread, THREAD_PRIORITY_TIME_CRITICAL );
	
	// Start the audio stream.
	result = wrapper->audioClient->Start();
	
	OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_STARTING_DEVICE_IO, result );
	
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
	
	// Stop the output stream.
	HRESULT result = wrapper->audioClient->Stop();
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_STOPPING_DEVICE_IO );
		return false;
	}

	running = false;
	
	// Wait for the driver thread to finish.
	WaitForSingleObject( wrapper->driverThread, INFINITE );
	
	// Destroy the driver thread object.
	CloseHandle( wrapper->driverThread );
	
	// Release the audio capture client object.
	if ( wrapper->audioCaptureClient != NULL )
		wrapper->audioCaptureClient->Release();
	
	// Release the audio render client object.
	if ( wrapper->audioRenderClient != NULL )
		wrapper->audioRenderClient->Release();
	
	// Destroy the buffer-free event object.
	CloseHandle( wrapper->bufferFreeEvent );
	
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
	// If the device is invalid, do nothing and return an empty name.
	if ( !valid || inputChannelIndex >= numInputChannels )
		return UTF8String();
	
	return UTF8String("Input ") + inputChannelIndex;
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
	// If the device is invalid or index out of bounds, do nothing and return an empty name.
	if ( !valid || outputChannelIndex >= numOutputChannels )
		return UTF8String();
	
	return UTF8String("Output ") + outputChannelIndex;
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
		return Float(0);
	
	// Get the current stream format for this device.
	WAVEFORMATEXTENSIBLE* streamFormat;
	
	wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	SampleRate sampleRate = SampleRate(streamFormat->Format.nSamplesPerSec);
	
	// Release the stream format object.
	CoTaskMemFree( streamFormat );
	
	return sampleRate;
}




Bool SoundDevice:: setSampleRate( SampleRate newSampleRate )
{
	// If the device is invalid, do nothing and return FALSE.
	if ( !valid )
		return false;
	
	
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
	// Just get the output latency because WASAPI doesn't differentiate between input and output latencies.
	return this->getOutputLatency();
}




Time SoundDevice:: getOutputLatency() const
{
	// If the device is invalid, do nothing and return a latency of 0.
	if ( !valid )
		return Time();
	
	REFERENCE_TIME latency100Nanos;
	
	HRESULT result = wrapper->audioClient->GetStreamLatency( &latency100Nanos );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_LATENCY );
		return Time();
	}
	
	// Convert time to nanoseconds and return.
	return Time( Int64(latency100Nanos*100) );
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
	// Check to see if this device is the default system input device.
	
	IMMDevice* device;
	HRESULT  result = wrapper->deviceEnumerator->GetDefaultAudioEndpoint( eCapture, eMultimedia, &device );
	
	// If there was an error because no default device was found, return that this device isn't default.
	if ( result != S_OK && result == E_NOTFOUND )
		return false;
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_INPUT_DEVICE );
		return false;
	}
	
	// Get the name of the default input device.
	LPWSTR defaultDeviceName;
	result = device->GetId( &defaultDeviceName );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	// Release the default device.
	device->Release();
	
	// Get the name of this device.
	LPWSTR deviceName;
	result = wrapper->device->GetId( &deviceName );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	// Check to see if the devices have the same name.
	Bool isDefaultInput = wcscmp( defaultDeviceName, deviceName ) == 0;
	
	// Release the names of the two devices.
	CoTaskMemFree( defaultDeviceName );
	CoTaskMemFree( deviceName );
	
	return isDefaultInput;
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
	// Check to see if this device is the default system output device.
	
	IMMDevice* device;
	HRESULT  result = wrapper->deviceEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &device );
	
	// If there was an error because no default device was found, return that this device isn't default.
	if ( result != S_OK && result == E_NOTFOUND )
		return false;
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_OUTPUT_DEVICE );
		return false;
	}
	
	// Get the name of the default output device.
	LPWSTR defaultDeviceName;
	result = device->GetId( &defaultDeviceName );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	// Release the default device.
	device->Release();
	
	// Get the name of this device.
	LPWSTR deviceName;
	result = wrapper->device->GetId( &deviceName );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	// Check to see if the devices have the same name.
	Bool isDefaultOutput = wcscmp( defaultDeviceName, deviceName ) == 0;
	
	// Release the names of the two devices.
	CoTaskMemFree( defaultDeviceName );
	CoTaskMemFree( deviceName );
	
	return isDefaultOutput;
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
	{
		// Release the device.
		wrapper->releaseDevice();
		
		om::util::destruct( wrapper );
	}
	
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
	// If the device ID is invalid, then the device must be invalid.
	if ( deviceID == SoundDeviceID::INVALID_DEVICE )
	{
		valid = false;
		return false;
	}
	
	//****************************************************************
	// Create an object that enumerates the connected devices.
	
	HRESULT result;

	if ( wrapper->deviceEnumerator == NULL )
	{
		CoInitialize( NULL );
		
		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		
		result = CoCreateInstance( CLSID_MMDeviceEnumerator, NULL,
									CLSCTX_ALL, IID_IMMDeviceEnumerator,
									(void**)&wrapper->deviceEnumerator );
		
		if ( result != S_OK )
		{
			REPORT_ERROR( ERROR_CREATING_DEVICE_ENUMERATOR );
			return false;
		}
	}
	
	//****************************************************************
	// Test to see if the device with this ID string exists.
	
	result = wrapper->deviceEnumerator->GetDevice( (LPWSTR)deviceID.getIDString().getPointer(), &wrapper->device );
	
	if ( result != S_OK )
	{
		// Clean up the device state that has been initialized so far.
		wrapper->releaseDevice();
		
		REPORT_ERROR( ERROR_QUERYING_DEVICE );
		valid = false;
		return false;
	}
	
	// Check to see if the device is actually active or just lingering because it was unplugged.
	DWORD deviceState;

	result = wrapper->device->GetState( &deviceState );
	
	// Ignore devices that are not active.
	if ( result != S_OK || deviceState != DEVICE_STATE_ACTIVE )
	{
		// Clean up the device state that has been initialized so far.
		wrapper->releaseDevice();
		
		REPORT_ERROR( ERROR_QUERYING_DEVICE );
		valid = false;
		return false;
	}

	//****************************************************************
	// The device was just found! Initialize some objects that help us interface with it.
	
	// Get the audio client interface for this device.
	result = wrapper->device->Activate( __uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&wrapper->audioClient );
	
	if ( result != S_OK )
	{
		// Clean up the device state that has been initialized so far.
		wrapper->releaseDevice();
		
		REPORT_ERROR( ERROR_CREATING_AUDIO_CLIENT );
		valid = false;
		return false;
	}
	
	//****************************************************************
	// Initialize the audio client object.
	
	// Get the current stream format for this device.
	WAVEFORMATEX* streamFormat;
	
	result = wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	if ( result != S_OK )
	{
		// Clean up the device state that has been initialized so far.
		wrapper->releaseDevice();
		
		REPORT_ERROR( ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION );
		valid = false;
		return false;
	}
	
	// Don't specify a device period because we are using shared mode.
	// Get the length of the audio device's minimum period in reference time units (100's of nanoseconds).
	REFERENCE_TIME devicePeriod;
	
	wrapper->audioClient->GetDevicePeriod( NULL, &devicePeriod );
	
	// Initialize audio client object to use shared mode with the above parameters.
	result = wrapper->audioClient->Initialize( AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
												devicePeriod, 0, streamFormat, NULL );
	
	if ( result != S_OK )
	{
		// Free the stream format structure.
		CoTaskMemFree( streamFormat );
		
		// Clean up the device state that has been initialized so far.
		wrapper->releaseDevice();
		
		REPORT_ERROR( ERROR_INITIALIZING_AUDIO_CLIENT );
		valid = false;
		
		return false;
	}
	
	// Free the stream format structure.
	CoTaskMemFree( streamFormat );
	
	// Set the device to be valid.
	valid = true;
	
	return true;
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
	// Test to see if this is an input device.
	
	HRESULT result = wrapper->audioClient->GetService( __uuidof(IAudioCaptureClient), (void**)&wrapper->audioCaptureClient );
	
	// Check to make sure this is an input device.
	if ( result == AUDCLNT_E_WRONG_ENDPOINT_TYPE )
		return true;
	else if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_GETTING_INPUT_CLIENT );
		return false;
	}
	
	// Release the capture client, we just needed to see if input was supported.
	wrapper->audioCaptureClient->Release();
	
	//**************************************************************************
	// Get the input stream format for this device.
	// We are using 'shared' mode in the WASAPI, so there is only one valid input format.
	
	// Get the current stream format for this device.
	WAVEFORMATEXTENSIBLE* streamFormat;
	
	result = wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION );
		return false;
	}
	
	// Get the number of channels that this device has.
	numInputChannels = streamFormat->Format.nChannels;
	
	// Release the stream format object.
	CoTaskMemFree( streamFormat );
	
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
	// Test to see if this is an output device.
	
	IAudioRenderClient* renderClient;

	HRESULT result = wrapper->audioClient->GetService( __uuidof(IAudioRenderClient), (void**)&renderClient );
	
	// Check to make sure this is an output device.
	if ( result == AUDCLNT_E_WRONG_ENDPOINT_TYPE )
		return true;
	else if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_GETTING_OUTPUT_CLIENT );
		return false;
	}
	
	// Release the rendering client, we just needed to see if output was supported.
	renderClient->Release();
	
	//**************************************************************************
	// Get the output stream format for this device.
	// We are using 'shared' mode in the WASAPI, so there is only one valid output format.
	
	// Get the current stream format for this device.
	WAVEFORMATEXTENSIBLE* streamFormat;
	
	result = wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION );
		return false;
	}
	
	// Get the number of channels that this device has.
	numOutputChannels = streamFormat->Format.nChannels;
	
	// Release the stream format object.
	CoTaskMemFree( streamFormat );
	
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
	// Get the sample rate of this device.
	// We are using 'shared' mode in the WASAPI, so there is only one valid sample rate.
	
	// Get the current stream format for this device.
	WAVEFORMATEXTENSIBLE* streamFormat;
	
	HRESULT result = wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION );
		return false;
	}
	
	// Add the current sample rate to the list of native sample rates.
	nativeSampleRates.add( Float(streamFormat->Format.nSamplesPerSec) );
	
	// Release the stream format object.
	CoTaskMemFree( streamFormat );
	
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
	// Open a way to access properties of the device.
	
	IPropertyStore* deviceProperties;
	
	HRESULT result = wrapper->device->OpenPropertyStore( STGM_READ, &deviceProperties );
	
	if ( result != S_OK )
	{
		// Set the name to be the empty string.
		name = UTF8String();
		
		REPORT_ERROR( ERROR_CREATING_DEVICE_PROPERTY_STORE );
		return false;
	}
	
	//**************************************************************************
	// Get the name of this device.
	
	// Create a structure to hold the name of the device.
	PROPVARIANT propertyValue;
	PropVariantInit( &propertyValue );
	
	// Get the name of the device.
	result = deviceProperties->GetValue( PKEY_Device_FriendlyName, &propertyValue );
	
	if ( result != S_OK )
	{
		// Clear the structure that was used to query the name string.
		PropVariantClear( &propertyValue );
		
		// Release the device property store.
		deviceProperties->Release();
		
		// Set the name to be the empty string.
		name = UTF8String();
		
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	// Get the length of the name of the device.
	int nameLength = WideCharToMultiByte( CP_ACP, 0, propertyValue.pwszVal, -1, NULL, 0, NULL, NULL );
	
	// Allocate an array of characters which will hold the device's name.
	Char* cString = om::util::allocate<Char>( nameLength / sizeof(char) );
	
	// Get the device's name string in the temporary buffer.
	int bytesWritten = WideCharToMultiByte( CP_ACP, 0, propertyValue.pwszVal, -1, cString, nameLength, NULL, NULL );
	
	// Set the name string object to contain the contents of this device name string.
	name = UTF8String( (const UTF8Char*)cString, (Size)nameLength );
	
	// Deallocate the temporary array of characters which held the name string.
	om::util::deallocate( cString );
	
	// Clear the structure that was used to query the name string.
	PropVariantClear( &propertyValue );
	
	// Release the device property store.
	deviceProperties->Release();
	
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
	
	// No way to access manufacturer on windows, just set the manufacturer string to be empty.
	manufacturer = UTF8String();
	
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
	if ( !valid )
		return false;
	
	//****************************************************************
	// Create and register an object that receives device change notifications
	
	if ( wrapper->deviceChangeNotifier != NULL )
	{
		wrapper->deviceChangeNotifier->Release();
		wrapper->deviceChangeNotifier = NULL;
	}
	
	wrapper->deviceChangeNotifier = om::util::construct<Wrapper::DeviceChangeNotifier>( this );
	
	HRESULT result = wrapper->deviceEnumerator->RegisterEndpointNotificationCallback( wrapper->deviceChangeNotifier );
	
	// Make sure that there was no error.
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_REGISTERING_DEVICE_CALLBACK );
		return false;
	}
	
	return false;
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
	//****************************************************************
	// Unregister and release an object that receives device change notifications
	
	if ( wrapper->deviceChangeNotifier != NULL )
	{
		HRESULT result = wrapper->deviceEnumerator->UnregisterEndpointNotificationCallback( wrapper->deviceChangeNotifier );
		
		// Make sure that there was no error.
		if ( result != S_OK )
		{
			REPORT_ERROR( ERROR_UNREGISTERING_DEVICE_CALLBACK );
			return false;
		}
		
		wrapper->deviceChangeNotifier->Release();
		wrapper->deviceChangeNotifier = NULL;
	}
	
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
								Sample32f* outputBuffer, Index outputStartIndex, Size numOutputChannels )
{
	const Size numInputChannels = inputBuffer.getChannelCount();
	const Index outputOffset = outputStartIndex*numOutputChannels;
	
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		// Check to see if there are enough channels in the client-provided buffer.
		if ( c < numInputChannels )
		{
			// Write data from the client output buffer.
			const Sample32f* input = inputBuffer.getChannel(c) + inputStartIndex;
			const Sample32f* const inputEnd = input + numInputSamples;
			Sample32f* output = outputBuffer + outputOffset + c;
			
			while ( input != inputEnd )
			{
				*output = *input;
				output += numOutputChannels;
				input++;
			}
		}
		else
		{
			// Write zeros to the rest of the output buffer channels.
			Sample32f* output = outputBuffer + outputOffset + c;
			const Sample32f* const outputEnd = output + numInputSamples*numOutputChannels;
			
			while ( output != outputEnd )
			{
				*output = Sample32f(0);
				output += numOutputChannels;
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




static void zeroOutputBuffer( Sample32f* outputBuffer, Index outputStartIndex, Size numSamples, Size numOutputChannels )
{
	const Index outputOffset = outputStartIndex*numOutputChannels;
	
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		// Write zeros to the rest of the output buffer channels.
		Sample32f* output = outputBuffer + outputOffset + c;
		const Sample32f* const outputEnd = output + numSamples*numOutputChannels;
		
		while ( output != outputEnd )
		{
			*output = Sample32f(0);
			output += numOutputChannels;
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




DWORD WINAPI SoundDevice:: Wrapper:: driverThreadEntry( LPVOID lpParam )
{
	SoundDevice* device = (SoundDevice*)lpParam;
	Wrapper* wrapper = device->wrapper;
	Bool isRunning = true;
	SampleRate deviceSampleRate;
	SampleType sampleType;
	Size numChannels;
	
	// Get the current stream format for this device.
	WAVEFORMATEX* streamFormat;
	
	HRESULT result = wrapper->audioClient->GetMixFormat( (WAVEFORMATEX**)&streamFormat );
	
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_QUERYING_OUTPUT_STREAM_CONFIGURATION );
		return 0;
	}
	
	numChannels = streamFormat->nChannels;
	deviceSampleRate = SampleRate(streamFormat->nSamplesPerSec);
	
	if ( streamFormat->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
		(streamFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
		((WAVEFORMATEXTENSIBLE*)streamFormat)->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) )
	{
		sampleType = SampleType::SAMPLE_32F;
	}
	else
	{
		// Determine the integer output format. If it is not supported, return from the function.
		switch ( streamFormat->wBitsPerSample )
		{
			case 8: sampleType = SampleType::SAMPLE_8; break;
			case 16: sampleType = SampleType::SAMPLE_16; break;
			case 24: sampleType = SampleType::SAMPLE_24; break;
			case 32: sampleType = SampleType::SAMPLE_32; break;
			case 64: sampleType = SampleType::SAMPLE_64; break;
			default:
				return 0;
		}
	}
	
	// Free the stream format structure.
	CoTaskMemFree( streamFormat );
	
	
	//************************************************************************************
	
	// A boolean value indicating whether or not the timestamp of the last processing frame is saved.
	Bool hasLastFrameTime = false;
	
	// The current system time of the start of the last processing frame.
	Time lastFrameTime;
	
	// Do audio IO while the device is still running.
	while ( isRunning )
	{
		// Wait until there is a buffer available for output.
		DWORD waitStatus = WaitForSingleObject( wrapper->bufferFreeEvent, INFINITE );
		
		// Check for an error, if so, try again.
		if ( waitStatus != WAIT_OBJECT_0 )
		{
			REPORT_ERROR( ERROR_WAITING_FOR_OUTPUT_BUFFER );
			
			isRunning = device->isRunning();
			continue;
		}
		
		// Acquire the mutex which indicates that audio is currently being output.
		device->ioMutex.lock();
		
		// Get the absolute time at the start of the frame.
		Time frameStartTime = Time::getCurrent();
		
		//*******************************************************************************
		
		// Get the size of the output buffer in sample frames.
		UINT32 bufferSize;
		
		result = wrapper->audioClient->GetBufferSize( &bufferSize );
		
		// Check for an error, if so, try again.
		if ( result != S_OK )
		{
			REPORT_ERROR( ERROR_GETTING_BUFFER_SIZE );
			
			isRunning = device->isRunning();
			continue;
		}
		
		// Get the padding of the buffer in sample frames.
		UINT32 bufferPadding;
		
		result = wrapper->audioClient->GetCurrentPadding( &bufferPadding );
		
		// Check for an error, if so, try again.
		if ( result != S_OK )
		{
			REPORT_ERROR( ERROR_GETTING_BUFFER_PADDING );
			
			isRunning = device->isRunning();
			continue;
		}
		
		UINT32 bufferSizeInSamples = bufferSize - bufferPadding;
		
		//*******************************************************************************
		
		const SoundDeviceDelegate& delegate = device->getDelegate();
		
		// Is input enabled for this device?
		if ( wrapper->audioCaptureClient != NULL && delegate.inputCallback.isSet() )
		{
			// Determine the size of the next packet of audio input data.
			UINT32 packetLength;
			
			result = wrapper->audioCaptureClient->GetNextPacketSize( &packetLength );
			
			OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_GETTING_INPUT_PACKET_SIZE, result );
			
			// Continue getting input data while there is data available.
			while ( packetLength > 0 )
			{
				BYTE* bufferData;
				UINT32 numInputSamples;
				DWORD flags;
				
				// Get the buffer containing the new input data.
				result = wrapper->audioCaptureClient->GetBuffer( &bufferData, &numInputSamples,
																&flags, NULL, NULL );
				
				OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_GETTING_BUFFER_FROM_DEVICE, result );
				
				SoundBuffer* inputBuffer = &device->ioBuffer;
				Size numInputChannels = device->numInputChannels;
				
				// Make sure that the input buffer has the right sample rate.
				if ( inputBuffer->getSampleRate() != deviceSampleRate )
					inputBuffer->setSampleRate( deviceSampleRate );
				
				// Make sure that the input buffer is large enough to hold all of the samples/channels.
				if ( inputBuffer->getSize() < numInputSamples )
					inputBuffer->setSize( numInputSamples );
				
				// Make sure that the input buffer is large enough to hold all of the samples/channels.
				if ( inputBuffer->getChannelCount() != numInputChannels )
					inputBuffer->setChannelCount( numInputChannels );
				
				// Copy the samples from the CoreAudio interleaved buffers to another buffer.
				for ( Index c = 0; c < numInputChannels; c++ )
				{
					const Sample32f* source = (Sample32f*)bufferData + c;
					const Sample32f* const sourceEnd = source + numInputSamples*numInputChannels;
					Sample32f* destination = inputBuffer->getChannel(c);
					
					while ( source < sourceEnd )
					{
						*destination = *source;
						source += numInputChannels;
						destination++;
					}
				}
				
				// Send the new input samples to the input callback function.
				delegate.inputCallback( *device, *inputBuffer, numInputSamples, frameStartTime );
				
				// Release the buffer back to the device
				result = wrapper->audioCaptureClient->ReleaseBuffer( numInputSamples );
				
				OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_RELEASING_BUFFER_TO_DEVICE, result );
				
				// Get the next input audio packet size, in case there is any more.
				result = wrapper->audioCaptureClient->GetNextPacketSize( &packetLength );
				
				OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_GETTING_INPUT_PACKET_SIZE, result );
			}
		}
		
		//*******************************************************************************
		
		Size numSamplesWritten = 0;
		
		// Is output enabled for this device?
		if ( wrapper->audioRenderClient != NULL && delegate.outputCallback.isSet() )
		{
			// Get the output buffer.
			BYTE* bufferData;
			
			result = wrapper->audioRenderClient->GetBuffer( bufferSizeInSamples, &bufferData );
			
			OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_GETTING_BUFFER_FROM_DEVICE, result );
			
			//************************************************************************************
			// Prepare the output buffer so that it can receive the output audio.
			
			// The number of samples to read from the output audio source.
			Size numSamplesToOutput = bufferSizeInSamples;
			
			while ( numSamplesWritten < numSamplesToOutput )
			{
				Size numSamplesRemaining = numSamplesToOutput - numSamplesWritten;
				
				// Check to see if there are old samples in the sample rate conversion buffer.
				if ( device->samplesInConverterBuffer > 0 )
				{
					Size numConverterSamples = math::min( device->samplesInConverterBuffer, numSamplesRemaining );
					
					copyToOutputBuffer( device->sampleRateConversionBuffer, device->converterBufferStart, numConverterSamples,
										(Sample32f*)bufferData, numSamplesWritten, device->numOutputChannels );
					
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
					if ( outputBuffer->getChannelCount() != device->numOutputChannels )
						outputBuffer->setChannelCount( device->numOutputChannels );
					
					//************************************************************************************
					// Get the output audio from the client and potentially convert its sample rate.
					
					// Zero the output buffer so that there won't be garbage played if the client doesn't write anything.
					outputBuffer->zero( 0, numSamplesToOutput );
					
					// Request the desired number of samples from the output callback function.
					Size numOutputSamples = delegate.outputCallback( *device, *outputBuffer, numSamplesToRequest, frameStartTime );
					
					// Detect when the client doesn't provide any more audio.
					if ( numOutputSamples == 0 )
					{
						// Write zeros to the rest of the buffer and return from the function.
						zeroOutputBuffer( (Sample32f*)bufferData, numSamplesWritten, numSamplesToRequest, device->numOutputChannels );
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
					// Copy the output buffer to the WASAPI output buffer channels.
					
					// Place each input channel into the output buffer in interleaved format.
					copyToOutputBuffer( *outputBuffer, 0, numOutputSamples, (Sample32f*)bufferData, numSamplesWritten,
										device->numOutputChannels );
					
					numSamplesWritten += numOutputSamples;
				}
			}
			
			// Release the output buffer to the device.
			if ( numSamplesWritten > 0 )
				result = wrapper->audioRenderClient->ReleaseBuffer( UINT32(numSamplesWritten), 0 );
			else
				result = wrapper->audioRenderClient->ReleaseBuffer( bufferSizeInSamples, AUDCLNT_BUFFERFLAGS_SILENT );
			
			OM_ASSERT_MESSAGE_CODE( result == S_OK, ERROR_RELEASING_BUFFER_TO_DEVICE, result );
		}
		
		// Get the current absolute time.
		Time currentTime = Time::getCurrent();
		
		// Update the current timing information.
		if ( hasLastFrameTime && bufferSizeInSamples > 0 )
		{
			// Update the CPU usage for this frame.
			Double frameTime = currentTime - frameStartTime;
			Double bufferTime = bufferSizeInSamples / deviceSampleRate;
			
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
			if ( delegate.processOverload.isSet() &&
				(currentTime - lastFrameTime) > bufferTimeThreshold && device->currentCPUUsage > Float(1) )
			{
				delegate.processOverload( *device );
			}
		}
		else
		{
			// No samples were processed or doesn't have a previous time, so set the CPU usage to 0.
			device->currentCPUUsage = 0;
		}
		
		// Save the current frame time as the last one.
		lastFrameTime = currentTime;
		hasLastFrameTime = true;
		
		// Release the mutex which indicates that audio is currently being output.
		device->ioMutex.unlock();
		
		isRunning = device->isRunning();
	}
	
	return 0;
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_WINDOWS
