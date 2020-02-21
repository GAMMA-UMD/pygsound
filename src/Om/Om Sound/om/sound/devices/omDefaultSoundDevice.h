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

#ifndef INCLUDE_OM_DEFAULT_SOUND_DEVICE_H
#define INCLUDE_OM_DEFAULT_SOUND_DEVICE_H


#include "omSoundDevicesConfig.h"


#include "omSoundDeviceID.h"
#include "omSoundDeviceManager.h"
#include "omSoundDevice.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that maintains a default input and output device abstraction layer.
/**
  * This class automatically keeps track of the current default system input and
  * output devices and acts as a wrapper for those devices with the same interface
  * as the SoundDevice class. This allows the user to not have to manage this information
  * themselves and simplifies basic audio application development.
  */
class DefaultSoundDevice
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default sound device with the current default input and output devices.
			DefaultSoundDevice();
			
			
			/// Create a copy of the specified default sound device.
			DefaultSoundDevice( const DefaultSoundDevice& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a DefaultSoundDevice object, stopping the input/output of any audio.
			~DefaultSoundDevice();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state from one DefaultSoundDevice to this object.
			DefaultSoundDevice& operator = ( const DefaultSoundDevice& other );
			
			
		//********************************************************************************
		//******	Sound IO Start/Stop Methods
			
			
			/// Start sending audio to the device.
			/**
			  * If this device has no output callback, zeroes are sent to the device until
			  * a callback function is bound to the device. If the device is invalid,
			  * this method has no effect.
			  *
			  * This method has the effect of starting a new audio rendering thread which
			  * will then handle requesting audio data from the output callback function
			  * until the callback function is changed or removed or the device's output
			  * is stopped using the stop() method.
			  */
			void start();
			
			
			/// Stop sending/receiving audio data to the device.
			/**
			  * If the device is currently outputing audio, the output of further audio
			  * is stopped. Otherwise, the method has no effect. If the device is invalid,
			  * this method has no effect.
			  *
			  * This method has the effect of stopping the audio rendering thread that was
			  * started in the start() method.
			  */
			void stop();
			
			
			/// Return whether or not the device is currently sending/receiving audio.
			/**
			  * If audio is currently being requested and sent to the device, TRUE is returned.
			  * Otherwise, FALSE is returned. If the device is invalid, FALSE is always
			  * returned.
			  * 
			  * @return whether or not the device is currently outputing audio.
			  */
			Bool isRunning() const;
			
			
		//********************************************************************************
		//******	Device Input Channel Accessor Methods
			
			
			/// Get the number of input channels that this device has.
			/**
			  * If the device is invalid, this method always returns 0.
			  */
			Size getInputChannelCount() const;
			
			
			/// Return a human-readable name for the input channel at the specified index.
			/**
			  * This is a string provided by the device driver which names the input channel
			  * with the given index. If an invalid channel index is specified, an empty
			  * string is returned.
			  */
			UTF8String getInputChannelName( Index inputChannelIndex ) const;
			
			
		//********************************************************************************
		//******	Device Output Channel Accessor Methods
			
			
			/// Get the number of output channels that this device has.
			/**
			  * If the device is invalid, this method always returns 0.
			  */
			Size getOutputChannelCount() const;
			
			
			/// Return a human-readable name for the output channel at the specified index.
			/**
			  * This is a string provided by the device driver which names the output channel
			  * with the given index. If an invalid channel index is specified, an empty
			  * string is returned.
			  */
			UTF8String getOutputChannelName( Index outputChannelIndex ) const;
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Get the current sampling rate for the default input device.
			/**
			  * This is the sample rate of the device's clock.
			  * 
			  * @return the current sample rate of the default input device's clock.
			  */
			SampleRate getInputSampleRate() const;
			
			
			/// Get the current sampling rate for the default output device.
			/**
			  * This is the sample rate of the device's clock.
			  * 
			  * @return the current sample rate of the default output device's clock.
			  */
			SampleRate getOutputSampleRate() const;
			
			
		//********************************************************************************
		//******	Latency Accessor Methods
			
			
			/// Return the one-way input latency in seconds of this sound device.
			/**
			  * This is the total time that it takes for the sound device to
			  * preset input, given an analogue input signal.
			  */
			Time getInputLatency() const;
			
			
			/// Return the one-way output latency in seconds of this sound device.
			/**
			  * This is the total time that it takes for the sound device to
			  * produce output, given input audio data.
			  */
			Time getOutputLatency() const;
			
			
		//********************************************************************************
		//******	Device Name Accessor Method
			
			
			/// Get a string representing the name of the default input device.
			UTF8String getInputName() const;
			
			
			/// Get a string representing the name of the default input device's manufacturer.
			UTF8String getInputManufacturer() const;
			
			
			/// Get a string representing the name of the default input device.
			UTF8String getOutputName() const;
			
			
			/// Get a string representing the name of the default input device's manufacturer.
			UTF8String getOutputManufacturer() const;
			
			
		//********************************************************************************
		//******	Delegate Accessor Methods
			
			
			/// Return a reference to the delegate object that is responding to events for this device manager.
			OM_INLINE const SoundDeviceDelegate& getDelegate() const
			{
				return delegate;
			}
			
			
			/// Replace the delegate object that is responding to events for this device manager.
			void setDelegate( const SoundDeviceDelegate& newDelegate );
			
			
		//********************************************************************************
		//******	Device Status Accessor Method
			
			
			/// Return whether or not this device represents a valid device.
			/**
			  * If a SoundDevice is created with a SoundDeviceID that does not
			  * represent a valid system audio device or if a device is removed after
			  * it is created, the SoundDevice is marked as invalid and this
			  * method will return FALSE. Otherwise, if the device is valid, the method
			  * returns TRUE.
			  *
			  * If a device is invalid, the output callback method will not be called anymore
			  * and the application should switch to a different device. The application
			  * should periodically check the return value of this function to see if the
			  * device has been removed.
			  */
			Bool isValid() const;
			
			
			/// Return whether or not this device is an input device.
			/**
			  * If this is true, the device will have at least one output channel.
			  * Otherwise, the device should have 0 output channels.
			  */
			Bool isInput() const;

			
			
			/// Return whether or not this device is an output device.
			/**
			  * If this is true, the device will have at least one output channel.
			  * Otherwise, the device should have 0 output channels.
			  */
			Bool isOutput() const;
			
			
			/// Return whether or not this device represents the current default system input device.
			/**
			  * This method will return TRUE unless there is no default input device or
			  * if the default input device has no channels.
			  */
			OM_INLINE Bool isDefaultInput() const
			{
				return this->isInput();
			}
			
			
			/// Return whether or not this device represents the current default system output device.
			/**
			  * This method will return TRUE unless there is no default output device or
			  * if the default output device has no channels.
			  */
			OM_INLINE Bool isDefaultOutput() const
			{
				return this->isOutput();
			}
			
			
		//********************************************************************************
		//******	CPU Usage Accessor Methods
			
			
			/// Return a value indicating the fraction of available CPU time being used to process audio for the last frame.
			/**
			  * This value lies in the range [0,1] where 0 indicates that no time is used, and 1 indicates
			  * that 100% of the available time is used. Going over 100% of the available time means
			  * that the audio processing thread has stalled, producing clicks or pops in the audio
			  * due to dropped frames.
			  *
			  * This is the CPU usage amount for the last processed frame of audio. Use this value
			  * to obtain an instantaneous usage metric.
			  */
			Float getCurrentCPUUsage() const;
			
			
			/// Return a value indicating the average fraction of available CPU time being used to process audio.
			/**
			  * This value lies in the range [0,1] where 0 indicates that no time is used, and 1 indicates
			  * that 100% of the available time is used. Going over 100% of the available time means
			  * that the audio processing thread has stalled, producing clicks or pops in the audio
			  * due to dropped frames.
			  *
			  * This average value is computed using an envelope filter with a fast attack time and a
			  * release time of half a second. This value is computed to give a long-time indication of the
			  * CPU usage over many processing frames.
			  */
			Float getAverageCPUUsage() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Start the system polling thread and initialize other class data.
			void initialize();
			
			
			/// Stop the system polling thread and clean up other class data.
			void deinitialize();
			
			
			/// The function called when starting a new default device system polling thread.
			void pollingThreadEntryPoint();
			
			
			/// The callback function called by the device manager whenever a device is removed.
			void deviceRemoved( SoundDeviceManager& manager, const SoundDeviceID& oldID );
			
			
			/// Refresh the current default input device. This method is not synchronized, so sync it elsewhere!
			void refreshDefaultInput();
			
			
			/// Refresh the current default output device. This method is not synchronized, so sync it elsewhere!
			void refreshDefaultOutput();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that manages the list of devices.
			SoundDeviceManager deviceManager;
			
			
			/// A pointer to a SoundDevice object for the default audio input device.
			SoundDevice* input;
			
			
			/// A pointer to a SoundDevice object for the default audio output device.
			SoundDevice* output;
			
			
			/// A delegate object that responds to events for this sound device.
			SoundDeviceDelegate delegate;
			
			
			/// A thread object for the thread which polls the system to see if the default devices have changed.
			threads::Thread pollingThread;
			
			
			/// A mutex which synchronizes changes to the input and output devices.
			mutable threads::Mutex deviceChangeMutex;
			
			
			/// Whether or not the IO thread is currently running.
			Bool running;
			
			
			/// A boolean flag which allows the system polling thread to know when to stop.
			Bool shouldStopPolling;
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_DEFAULT_SOUND_DEVICE_H
