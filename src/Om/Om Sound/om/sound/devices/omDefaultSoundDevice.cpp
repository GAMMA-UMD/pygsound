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

#include "omDefaultSoundDevice.h"


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




DefaultSoundDevice:: DefaultSoundDevice()
	:	shouldStopPolling( false ),
		running( false ),
		input( NULL ),
		output( NULL )
{
	initialize();
}




DefaultSoundDevice:: DefaultSoundDevice( const DefaultSoundDevice& other )
	:	shouldStopPolling( false ),
		running( false ),
		input( NULL ),
		output( NULL )
{
	initialize();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




DefaultSoundDevice:: ~DefaultSoundDevice()
{
	deinitialize();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




DefaultSoundDevice& DefaultSoundDevice:: operator = ( const DefaultSoundDevice& other )
{
	if ( this != &other )
	{
		// Set the new delegate.
		this->setDelegate( other.getDelegate() );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound IO Start/Stop Methods
//############		
//##########################################################################################
//##########################################################################################




void DefaultSoundDevice:: start()
{
	if ( running )
		return;
	
	deviceChangeMutex.lock();
	
	if ( input != NULL )
		input->start();
	
	if ( output != NULL )
		output->start();
	
	running = true;
	
	deviceChangeMutex.unlock();
}




void DefaultSoundDevice:: stop()
{
	if ( !running )
		return;
	
	deviceChangeMutex.lock();

	if ( input != NULL )
		input->stop();
	
	if ( output != NULL )
		output->stop();
	
	running = false;
	
	deviceChangeMutex.unlock();
}




Bool DefaultSoundDevice:: isRunning() const
{
	return running;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Input Channel Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size DefaultSoundDevice:: getInputChannelCount() const
{
	deviceChangeMutex.lock();
	
	Size result = input != NULL ? input->getInputChannelCount() : 0;
	
	deviceChangeMutex.unlock();
	
	return result;
}




UTF8String DefaultSoundDevice:: getInputChannelName( Index inputChannelIndex ) const
{
	deviceChangeMutex.lock();
	
	UTF8String result = input != NULL ? input->getInputChannelName( inputChannelIndex ) : UTF8String();
	
	deviceChangeMutex.unlock();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Output Channel Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size DefaultSoundDevice:: getOutputChannelCount() const
{
	deviceChangeMutex.lock();
	
	Size result = output != NULL ? output->getOutputChannelCount() : 0;
	
	deviceChangeMutex.unlock();
	
	return result;
}




UTF8String DefaultSoundDevice:: getOutputChannelName( Index outputChannelIndex ) const
{
	deviceChangeMutex.lock();
	
	UTF8String result = output != NULL ? output->getOutputChannelName( outputChannelIndex ) : UTF8String();
	
	deviceChangeMutex.unlock();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Rate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SampleRate DefaultSoundDevice:: getInputSampleRate() const
{
	deviceChangeMutex.lock();
	
	SampleRate result = input != NULL ? input->getSampleRate() : 0;
	
	deviceChangeMutex.unlock();
	
	return result;
}




SampleRate DefaultSoundDevice:: getOutputSampleRate() const
{
	deviceChangeMutex.lock();
	
	SampleRate result = output != NULL ? output->getSampleRate() : 0;
	
	deviceChangeMutex.unlock();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Latency Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Time DefaultSoundDevice:: getInputLatency() const
{
	deviceChangeMutex.lock();
	
	Time result = input != NULL ? input->getInputLatency() : Time();
	
	deviceChangeMutex.unlock();
	
	return result;
}




Time DefaultSoundDevice:: getOutputLatency() const
{
	deviceChangeMutex.lock();
	
	Time result = output != NULL ? output->getOutputLatency() : Time();
	
	deviceChangeMutex.unlock();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Device Name Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String DefaultSoundDevice:: getInputName() const
{
	deviceChangeMutex.lock();
	
	UTF8String result = input != NULL ? input->getName() : UTF8String();
	
	deviceChangeMutex.unlock();
	
	return result;
}




UTF8String DefaultSoundDevice:: getInputManufacturer() const
{
	deviceChangeMutex.lock();
	
	UTF8String result = input != NULL ? input->getManufacturer() : UTF8String();
	
	deviceChangeMutex.unlock();
	
	return result;
}




UTF8String DefaultSoundDevice:: getOutputName() const
{
	deviceChangeMutex.lock();
	
	UTF8String result = output != NULL ? output->getName() : UTF8String();
	
	deviceChangeMutex.unlock();
	
	return result;
}




UTF8String DefaultSoundDevice:: getOutputManufacturer() const
{
	deviceChangeMutex.lock();
	
	UTF8String result = output != NULL ? output->getManufacturer() : UTF8String();
	
	deviceChangeMutex.unlock();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Delegate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void DefaultSoundDevice:: setDelegate( const SoundDeviceDelegate& newDelegate )
{
	deviceChangeMutex.lock();
	
	// Store the delegate locally.
	delegate = newDelegate;
	
	// Store the input delegate in the input device.
	SoundDeviceDelegate inputDelegate = newDelegate;
	inputDelegate.outputCallback = SoundOutputCallback();
	
	if ( input != NULL )
		input->setDelegate( inputDelegate );
	
	// Store the output delegate in the output device.
	SoundDeviceDelegate outputDelegate = newDelegate;
	outputDelegate.inputCallback = SoundInputCallback();
	
	if ( output != NULL )
		output->setDelegate( outputDelegate );
	
	deviceChangeMutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############		
//############		CPU Usage Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Float DefaultSoundDevice:: getCurrentCPUUsage() const
{
	if ( output != NULL )
		return output->getCurrentCPUUsage();
	else
		return Float(0);
}




Float DefaultSoundDevice:: getAverageCPUUsage() const
{
	if ( output != NULL )
		return output->getAverageCPUUsage();
	else
		return Float(0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Private Class Initialization Method
//############		
//##########################################################################################
//##########################################################################################




void DefaultSoundDevice:: initialize()
{
	deviceChangeMutex.lock();
	
	// Initialize the delegate object for the device manager.
	SoundDeviceManagerDelegate managerDelegate;
	
	managerDelegate.deviceRemoved = Function<void ( SoundDeviceManager&, const SoundDeviceID& )>( &DefaultSoundDevice::deviceRemoved, this );
	
	// Set the delegate for the device manager.
	deviceManager.setDelegate( managerDelegate );
	
	//*******************************************************************************
	
	// Initialize the default input device.
	SoundDeviceID defaultInputID = deviceManager.getDefaultInputDeviceID();
	
	if ( defaultInputID == SoundDeviceID::INVALID_DEVICE )
		input = NULL;
	else
		input = om::util::construct<SoundDevice>( defaultInputID );
	
	// Initialize the default output device.
	SoundDeviceID defaultOutputID = deviceManager.getDefaultOutputDeviceID();
	
	if ( defaultOutputID == SoundDeviceID::INVALID_DEVICE )
		output = NULL;
	else
		output = om::util::construct<SoundDevice>( defaultOutputID );
	
	// Make sure that the polling thread doesn't stop.
	shouldStopPolling = false;
	
	// Start the system polling thread.
	pollingThread.start( FunctionCall<void()>( Function<void()>( &DefaultSoundDevice::pollingThreadEntryPoint, this ) ) );
	
	deviceChangeMutex.unlock();
}




void DefaultSoundDevice:: deinitialize()
{
	// Remove the device removed callback.
	//deviceManager.removeDeviceRemovedCallback();
	
	// Wait for the polling thread to stop.
	shouldStopPolling = true;
	pollingThread.join();
	
	deviceChangeMutex.lock();

	// Destroy the default input and output device objects.
	if ( input != NULL )
		om::util::destruct( input );
	
	if ( output != NULL )
		om::util::destruct( output );
	
	deviceChangeMutex.unlock();
}




void DefaultSoundDevice:: pollingThreadEntryPoint()
{
	while ( !shouldStopPolling )
	{
		deviceChangeMutex.lock();
		
		// Check to make sure that the current input device is the default input device.
		if ( input == NULL || !input->isDefaultInput() )
			this->refreshDefaultInput();
		
		// Check to make sure that the current output device is the default output device.
		if ( output == NULL || !output->isDefaultOutput() )
			this->refreshDefaultOutput();
		
		if ( shouldStopPolling )
		{
			deviceChangeMutex.unlock();
			break;
		}
		
		deviceChangeMutex.unlock();
		
		// Sleep for 100 milliseconds.
		threads::Thread::sleepMs(100);
	}
}




void DefaultSoundDevice:: deviceRemoved( SoundDeviceManager& manager, const SoundDeviceID& oldID )
{
	deviceChangeMutex.lock();
	
	// Check to see if either of the default input device was removed.
	if ( input != NULL && input->getID() == oldID )
		this->refreshDefaultInput();
	
	// Check to see if either of the default output device was removed.
	if ( output != NULL && output->getID() == oldID )
		this->refreshDefaultOutput();
	
	deviceChangeMutex.unlock();
}




void DefaultSoundDevice:: refreshDefaultInput()
{
	SoundDeviceID defaultInputID = deviceManager.getDefaultInputDeviceID();
	
	// Was there a mistake in refreshing the device?
	if ( input != NULL && defaultInputID == input->getID() )
		return;
	
	// This should cause the old device to be deleted, but here we're going to ignore it
	// because otherwise the callback information gets lost sometimes.
	// This is probably because the system hasn't assigned a default device yet after a device was removed.
	if ( defaultInputID == SoundDeviceID::INVALID_DEVICE )
		return;
	
	SoundDevice* newInput = om::util::construct<SoundDevice>( defaultInputID );
	
	if ( input != NULL )
	{
		// Make sure the new input device has the same delegate.
		newInput->setDelegate( input->getDelegate() );
		
		// Destroy the old input device.
		om::util::destruct( input );
	}
	
	input = newInput;
	
	if ( running )
		input->start();
}




void DefaultSoundDevice:: refreshDefaultOutput()
{
	SoundDeviceID defaultOutputID = deviceManager.getDefaultOutputDeviceID();
	
	// Was there a mistake in refreshing the device?
	if ( output != NULL && defaultOutputID == output->getID() )
		return;
	
	// This should cause the old device to be deleted, but here we're going to ignore it
	// because otherwise the callback information gets lost sometimes.
	// This is probably because the system hasn't assigned a default device yet after a device was removed.
	if ( defaultOutputID == SoundDeviceID::INVALID_DEVICE )
		return;
	
	SoundDevice* newOutput = om::util::construct<SoundDevice>( defaultOutputID );
	
	if ( output != NULL )
	{
		// Make sure the new input device has the same delegate.
		newOutput->setDelegate( output->getDelegate() );
		
		// Destroy the old output device.
		om::util::destruct( output );
	}
	
	output = newOutput;
	
	if ( running )
		output->start();
}




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
