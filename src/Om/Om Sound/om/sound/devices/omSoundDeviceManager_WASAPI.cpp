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


#if defined(OM_PLATFORM_WINDOWS)

// Include platform-specific headers
#include <Windows.h>
#include <MmDeviceapi.h>
#include <Audioclient.h>
#include <Audiopolicy.h>

#define ERROR_CREATING_DEVICE_ENUMERATOR "An error was encountered while creating an audio device enumerator object."

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
//############		Device Change Notifier Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundDeviceManager:: DeviceChangeNotifier : public IMMNotificationClient
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			DeviceChangeNotifier( SoundDeviceManager* newManager )
				:	referenceCount( 1 ),
					manager( newManager )
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
				if ( manager != NULL )
				{
					if ( flow == eRender )
						manager->refreshDefaultOutputDevice();
					else if ( flow == eCapture )
						manager->refreshDefaultInputDevice();
				}
				
				return S_OK;
			}
			
			
			HRESULT STDMETHODCALLTYPE OnDeviceAdded( LPCWSTR pwstrDeviceId )
			{
				if ( manager != NULL )
					manager->refreshDevices();
				
				return S_OK;
			}
			
			
			HRESULT STDMETHODCALLTYPE OnDeviceRemoved( LPCWSTR pwstrDeviceId )
			{
				if ( manager != NULL )
					manager->refreshDevices();
				
				return S_OK;
			}
			
			
			HRESULT STDMETHODCALLTYPE OnDeviceStateChanged( LPCWSTR pwstrDeviceId, DWORD dwNewState )
			{
				if ( manager != NULL )
					manager->refreshDevices();
				
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
			
			
			/// A pointer to the object that is the device manager for this device change notifier.
			SoundDeviceManager* manager;
			
			
};




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
		//******	Constructor
			
			
			OM_INLINE Wrapper()
				:	deviceChangeNotifier( NULL ),
					deviceEnumerator( NULL ),
					deviceCollection( NULL )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			DeviceChangeNotifier* deviceChangeNotifier;
			
			IMMDeviceEnumerator* deviceEnumerator;
			
			IMMDeviceCollection* deviceCollection;
			
			
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
	// Create a wrapper object.
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




Bool SoundDeviceManager:: destroyManager()
{
	// Release objects which stored the list of devices.
	wrapper->deviceCollection->Release();
	wrapper->deviceEnumerator->Release();
	
	// Destroy the wrapper object.
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




Bool SoundDeviceManager:: registerDeviceUpdateCallbacks()
{
	//****************************************************************
	// Create an object that enumerates the connected devices.
	
	CoInitializeEx( NULL, COINIT_MULTITHREADED );
	
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	
	HRESULT result = CoCreateInstance( CLSID_MMDeviceEnumerator, NULL,
										CLSCTX_ALL, IID_IMMDeviceEnumerator,
										(void**)&wrapper->deviceEnumerator );
	
	// Make sure that an error has not been encountered.
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_CREATING_DEVICE_ENUMERATOR );
		return false;
	}
	
	//****************************************************************
	// Create and register an object that receives device change notifications
	
	if ( wrapper->deviceChangeNotifier != NULL )
	{
		wrapper->deviceChangeNotifier->Release();
		wrapper->deviceChangeNotifier = NULL;
	}
	
	wrapper->deviceChangeNotifier = new DeviceChangeNotifier( this );
	
	result = wrapper->deviceEnumerator->RegisterEndpointNotificationCallback( wrapper->deviceChangeNotifier );
	
	// Make sure that an error has not been encountered.
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_REGISTERING_DEVICE_CALLBACK );
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
	//****************************************************************
	// Unregister and release an object that receives device change notifications
	
	HRESULT result = wrapper->deviceEnumerator->UnregisterEndpointNotificationCallback( wrapper->deviceChangeNotifier );
	
	// Make sure that an error has not been encountered.
	if ( result != S_OK )
	{
		REPORT_ERROR( ERROR_UNREGISTERING_DEVICE_CALLBACK );
		return false;
	}
	
	wrapper->deviceChangeNotifier->Release();
	
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
	
	//****************************************************************
	// Enumerate the connected audio devices.
	
	// Release the previous device collection if it existed.
	if ( wrapper->deviceCollection != NULL )
	{
		wrapper->deviceCollection->Release();
		wrapper->deviceCollection = NULL;
	}
	
	// Get the collection of devices.
	HRESULT result = wrapper->deviceEnumerator->EnumAudioEndpoints( eAll, DEVICE_STATE_ACTIVE, &wrapper->deviceCollection );
	
	if ( result != S_OK )
	{
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEVICES );
		return false;
	}
	
	// Get the number of devices.
	UINT numDevices;
	result = wrapper->deviceCollection->GetCount( &numDevices );
	
	// Make sure that an error has not been encountered.
	if ( result != S_OK )
	{
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_NUMBER_DEVICES );
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
		const wchar_t* currentDeviceID = (const wchar_t*)devices[i].getIDString().getPointer();
		
		for ( Index j = 0; j < numDevices; j++ )
		{
			// Get the device at the current index.
			IMMDevice* otherDevice;
			result = wrapper->deviceCollection->Item( (UINT)j, &otherDevice );
			
			// Make sure that there was no error, if so, skip this device.
			if ( result != S_OK )
			{
				REPORT_ERROR( ERROR_QUERYING_DEVICE );
				continue;
			}
			
			// Get the ID of the current device.
			LPWSTR otherDeviceID;
			result = otherDevice->GetId( &otherDeviceID );
			
			// Make sure that there was no error, if so, skip this device.
			if ( result != S_OK )
			{
				REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
				continue;
			}
			
			// Test to see if the devices are equal.
			if ( wcscmp( currentDeviceID, otherDeviceID ) == 0 )
			{
				foundDevice = true;
				break;
			}
			
			// Release the current device's name.
			CoTaskMemFree( otherDeviceID );
		}
		
		// We didn't find a previously existing device, it must have been removed.
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
		// Get the device at the current index.
		IMMDevice* otherDevice;
		result = wrapper->deviceCollection->Item( (UINT)i, &otherDevice );
		
		// Make sure that there was no error, if so, skip this device.
		if ( result != S_OK )
		{
			REPORT_ERROR( ERROR_QUERYING_DEVICE );
			continue;
		}
		
		// Get the ID of the current device.
		LPWSTR otherDeviceID;
		result = otherDevice->GetId( &otherDeviceID );
		
		// Make sure that there was no error, if so, skip this device.
		if ( result != S_OK )
		{
			REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
			continue;
		}
		
		SoundDeviceID newID( (const UTF16Char*)otherDeviceID );
		
		// Release the current device's name.
		CoTaskMemFree( otherDeviceID );
		
		if ( !devices.contains( newID ) )
		{
			// If the device is new, add it.
			addedDevices.add( newID );
			devices.add( newID );
		}
	}
	
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




Bool SoundDeviceManager:: refreshDefaultInputDevice()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	//****************************************************************
	//	Get a handle to the default input device
	
	IMMDevice* device;
	HRESULT  result = wrapper->deviceEnumerator->GetDefaultAudioEndpoint( eCapture, eMultimedia, &device );
	
	// If there was an error because no default device was found, recover.
	if ( result != S_OK && result == E_NOTFOUND )
	{
		// Set the default input device to be the first if there was no default.
		defaultInputDeviceIndex = 0;
		deviceChangeMutex.unlock();
		return false;
	}
	
	// Check for other unrecoverable errors.
	if ( result != S_OK )
	{
		defaultInputDeviceIndex = 0;
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_INPUT_DEVICE );
		return false;
	}
	
	
	//****************************************************************
	// Get the name of the default input device.
	
	LPWSTR defaultDeviceName;
	result = device->GetId( &defaultDeviceName );
	
	// Make sure that an error has not been encountered.
	if ( result != S_OK )
	{
		defaultInputDeviceIndex = 0;
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	SoundDeviceID defaultDeviceID( (const UTF16Char*)defaultDeviceName );
	
	// Release the default device's name.
	CoTaskMemFree( defaultDeviceName );
	
	// Release the default device.
	device->Release();
	
	//****************************************************************
	
	// Get the index of the default device in this device manager's list of internal device IDs.
	if ( !devices.getIndex( defaultDeviceID, defaultInputDeviceIndex ) )
	{
		// Set the default input device to be the first if there was no default.
		defaultInputDeviceIndex = 0;
	}
	
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




Bool SoundDeviceManager:: refreshDefaultOutputDevice()
{
	// Acquire a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.lock();
	
	
	//****************************************************************
	//	Get a handle to the default output device
	
	IMMDevice* device;
	HRESULT  result = wrapper->deviceEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &device );
	
	// If there was an error because no device was found, recover.
	if ( result != S_OK && result == E_NOTFOUND )
	{
		// Set the default output device to be the first if there was no default.
		defaultOutputDeviceIndex = 0;
		deviceChangeMutex.unlock();
		return false;
	}
	
	// Check for other unrecoverable errors.
	if ( result != S_OK )
	{
		defaultOutputDeviceIndex = 0;
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEFAULT_OUTPUT_DEVICE );
		return false;
	}
	
	//****************************************************************
	// Get the name of the default input device.
	
	LPWSTR defaultDeviceName;
	result = device->GetId( &defaultDeviceName );
	
	// Make sure that an error has not been encountered.
	if ( result != S_OK )
	{
		defaultOutputDeviceIndex = 0;
		deviceChangeMutex.unlock();
		REPORT_ERROR( ERROR_QUERYING_DEVICE_NAME );
		return false;
	}
	
	SoundDeviceID defaultDeviceID( (const UTF16Char*)defaultDeviceName );
	
	// Release the default device's name.
	CoTaskMemFree( defaultDeviceName );
	
	// Release the default device.
	device->Release();
	
	//****************************************************************
	
	// Get the index of the default device in this device manager's list of internal device IDs.
	if ( !devices.getIndex( defaultDeviceID, defaultOutputDeviceIndex ) )
	{
		// Set the default output device to be the first if there was no default.
		defaultOutputDeviceIndex = 0;
	}
	
	// Release a mutex which indicates that the number of devices is currently changing or being queried.
	deviceChangeMutex.unlock();
	
	return true;
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // OM_PLATFORM_WINDOWS
