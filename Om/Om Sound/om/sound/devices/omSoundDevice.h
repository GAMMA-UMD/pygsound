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

#ifndef INCLUDE_OM_SOUND_DEVICE_H
#define INCLUDE_OM_SOUND_DEVICE_H


#include "omSoundDevicesConfig.h"


#include "omSoundDeviceID.h"
#include "omSoundDeviceDelegate.h"
#include "../filters/omSoundResampler.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents a system sound device.
/**
  * A SoundDevice provides an easy-to-use platform-independent interface for
  * sending audio to an audio device. It allows the user to access commonly
  * needed parameters such as the device's sample rate and name.
  *
  * The class also provides automatic sample rate conversion if the input audio
  * sample rate is not the same as the device's current sample rate. If one does not
  * wish to incurr a performance penalty from the sample rate conversion, the class
  * also allows the user to attempt to set the device's sample rate.
  */
class SoundDevice
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a SoundDevice for the specified SoundDeviceID.
			SoundDevice( const SoundDeviceID& newDeviceID );
			
			
			/// Create a copy of the specified SoundDevice object.
			SoundDevice( const SoundDevice& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a SoundDevice object, stopping the input/output of any audio.
			~SoundDevice();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state from one SoundDevice to this object.
			SoundDevice& operator = ( const SoundDevice& other );
			
			
		//********************************************************************************
		//******	Sound Output Start/Stop Methods
			
			
			/// Start sending audio to the device.
			/**
			  * If this device has no output callback, zeroes are sent to the device until
			  * a callback function is bound to the device. If the device is invalid or 
			  * if an error occurs, FALSE is returned indicating that the method had no effect.
			  * If TRUE is returned, the device was started successfully.
			  *
			  * This method has the effect of starting a new audio rendering thread which
			  * will then handle requesting audio data from the output callback function
			  * until the callback function is changed or removed or the device's output
			  * is stopped using the stop() method.
			  */
			Bool start();
			
			
			/// Stop sending/receiving audio data to the device.
			/**
			  * If the device is currently outputing audio, the output of further audio
			  * is stopped. Otherwise, the method has no effect. If the device is invalid,
			  * this method has no effect.
			  *
			  * This method has the effect of stopping the audio rendering thread that was
			  * started in the start() method.
			  */
			Bool stop();
			
			
			/// Return whether or not the device is currently sending/receiving audio.
			/**
			  * If audio is currently being requested and sent to the device, TRUE is returned.
			  * Otherwise, FALSE is returned. If the device is invalid, FALSE is always
			  * returned.
			  * 
			  * @return whether or not the device is currently outputing audio.
			  */
			OM_INLINE Bool isRunning() const
			{
				return running;
			}
			
			
		//********************************************************************************
		//******	Device Input Channel Accessor Methods
			
			
			/// Get the number of input channels that this device has.
			/**
			  * If the device is invalid, this method always returns 0.
			  */
			OM_INLINE Size getInputChannelCount() const
			{
				return numInputChannels;
			}
			
			
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
			OM_INLINE Size getOutputChannelCount() const
			{
				return numOutputChannels;
			}
			
			
			/// Return a human-readable name for the output channel at the specified index.
			/**
			  * This is a string provided by the device driver which names the output channel
			  * with the given index. If an invalid channel index is specified, an empty
			  * string is returned.
			  */
			UTF8String getOutputChannelName( Index outputChannelIndex ) const;
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Get the current sample rate at which audio is being sent to the device.
			/**
			  * This is the sample rate of the device's clock. Any input audio that doesn't
			  * match this sample rate is automatically converted to this sample rate.
			  * If the device is invalid, a sample rate of 0 is returned.
			  * 
			  * @return the current sample rate of the device's clock.
			  */
			SampleRate getSampleRate() const;
			
			
			/// Set the current sample rate at which audio should be sent to the device.
			/**
			  * This method sets the sample rate of the device's clock. If the specified
			  * new sample rate is not a natively supported sample rate or if there was an
			  * error in setting the new sample rate, FALSE is returned and this method has
			  * no effect on the device's state. Otherwise, if the sample rate is a native format,
			  * TRUE is returned and the device's output sample rate is changed.
			  * If the device is invalid, FALSE is returned and the method has no effect.
			  * 
			  * @param newSampleRate - the sample rate to which the device's clock will be set to.
			  * @return whether or not the sample rate change operation was successful.
			  */
			Bool setSampleRate( SampleRate newSampleRate );
			
			
			/// Return whether or not the specified sample rate is a native sample rate for this device.
			/**
			  * For a sample rate to be native, no sample rate conversion is necessary before
			  * sending the audio to the device if it is of that sampling rate.
			  * 
			  * @param sampleRate - the sample rate to test to see if it is a native format.
			  * @return whether or not the specified sample rate is a native format for this device.
			  */
			OM_INLINE Bool isNativeSampleRate( SampleRate sampleRate )
			{
				return nativeSampleRates.contains( sampleRate );
			}
			
			
			/// Return a list of the native sampling rates for this output audio device.
			/**
			  * For a sample rate to be native, no sample rate conversion is necessary before
			  * sending the audio to the device if it is of that sampling rate.
			  */
			OM_INLINE const ArrayList<SampleRate>& getNativeSampleRates() const
			{
				return nativeSampleRates;
			}
			
			
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
			
			
			/// Get a string representing the name of this device.
			/**
			  * This name is usually specified by the hardware driver as a human-readable
			  * identifier for the device. If the device is not valid, the empty string
			  * is returned.
			  * 
			  * @return the name of this device.
			  */
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Get a string representing the name of this device's manufacturer.
			/**
			  * This name is usually specified by the hardware driver as a human-readable
			  * identifier for the device's manufacturer. If the device is not valid, the empty string
			  * is returned.
			  * 
			  * @return the name of this device's manufacturer.
			  */
			OM_INLINE const UTF8String& getManufacturer() const
			{
				return manufacturer;
			}
			
			
		//********************************************************************************
		//******	Device ID Accessor Method
			
			
			/// Return an object that uniquely identifies this sound device.
			OM_INLINE SoundDeviceID getID() const
			{
				return deviceID;
			}
			
			
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
			OM_INLINE Bool isValid() const
			{
				return valid;
			}
			
			
			/// Return whether or not this device is an input device.
			/**
			  * If this is true, the device will have at least one output channel.
			  * Otherwise, the device should have 0 output channels.
			  */
			OM_INLINE Bool isInput() const
			{
				return numInputChannels > 0;
			}

			
			
			/// Return whether or not this device is an output device.
			/**
			  * If this is true, the device will have at least one output channel.
			  * Otherwise, the device should have 0 output channels.
			  */
			OM_INLINE Bool isOutput() const
			{
				return numOutputChannels > 0;
			}
			
			
			/// Return whether or not this device represents the current default system input device.
			Bool isDefaultInput() const;
			
			
			/// Return whether or not this device represents the current default system output device.
			Bool isDefaultOutput() const;
			
			
		//********************************************************************************
		//******	CPU Usage Accessor Method
			
			
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
			OM_INLINE Float getCurrentCPUUsage() const
			{
				return currentCPUUsage;
			}
			
			
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
			OM_INLINE Float getAverageCPUUsage() const
			{
				return averageCPUUsage;
			}
			
			
		//********************************************************************************
		//******	Delegate Accessor Methods
			
			
			/// Return a reference to the delegate object that is responding to events for this device manager.
			OM_INLINE const SoundDeviceDelegate& getDelegate() const
			{
				return delegate;
			}
			
			
			/// Replace the delegate object that is responding to events for this device manager.
			void setDelegate( const SoundDeviceDelegate& newDelegate );
			
			
	private:
		
		//********************************************************************************
		//******	Private Wrapper Class Declaration
			
			
			/// A class that encapsulates internal data needed by the SoundDevice object.
			class Wrapper;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Initialize a newly created device.
			Bool initializeDeviceData();
			
			
		//********************************************************************************
		//******	Platform-Specific Methods
			
			
			/// Initialize up any platform-specific data for a newly-created device.
			Bool createDevice();
			
			
			/// Clean up any platform-specific data before a device is destroyed.
			Bool destroyDevice();
			
			
			/// Register callback functions that tell the device when its attributes change.
			Bool registerDeviceUpdateCallbacks();
			
			
			/// Unregister callback functions that tell the device when its attributes change.
			Bool unregisterDeviceUpdateCallbacks();
			
			
			/// Determine whether or not this sound device is still valid.
			Bool refreshDeviceStatus();
			
			
			/// Refresh the configuration of the device's input stream.
			Bool refreshInputStreamConfiguration();
			
			
			/// Refresh the configuration of the device's output stream.
			Bool refreshOutputStreamConfiguration();
			
			
			/// Refresh the native sample rates of this device.
			Bool refreshNativeSampleRates();
			
			
			/// Refresh the name of the sound device.
			Bool refreshName();
			
			
			/// Refresh the manufacturer name of the sound device.
			Bool refreshManufacturer();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that represents a unique identifier for this sound device.
			SoundDeviceID deviceID;
			
			
			/// An object that handles events for this sound device.
			SoundDeviceDelegate delegate;
			
			
			/// A list of the natively supported sample rates of this SoundDevice.
			ArrayList<SampleRate> nativeSampleRates;
			
			
			/// The device-provided name of this SoundDevice.
			UTF8String name;
			
			
			/// The device-provided manufacturer name of this SoundDevice.
			UTF8String manufacturer;
			
			
			/// A mutex object that handles output synchronization with device parameter changes.
			mutable threads::Mutex ioMutex;
			
			
			/// A class that handles sample rate conversion for this device.
			filters::Resampler resampler;
			
			
			/// A buffer of audio data that holds audio data requested from the client.
			SoundBuffer ioBuffer;
			
			
			/// A buffer of audio data that is used hold the results of (possible) sample rate conversion.
			SoundBuffer sampleRateConversionBuffer;
			
			
			/// The number of input channels that this device has.
			Size numInputChannels;
			
			
			/// The number of output channels that this device has.
			Size numOutputChannels;
			
			
			/// The index of the first valid sample in the sample rate conversion buffer.
			Index converterBufferStart;
			
			
			/// The number of samples of valid audio that are buffered in the sample rate conversion buffer.
			Size samplesInConverterBuffer;
			
			
			/// A value indicating the fraction of available CPU time being used to process audio for the last frame.
			Float currentCPUUsage;
			
			
			/// A value indicating the average fraction of available CPU time being used to process audio on the rendering thread.
			/**
			  * This average value is computed using an envelope filter with a fast attack time and a
			  * release time of half a second. This value is computed to give a long-time indication of the
			  * CPU usage over many processing frames.
			  */
			Float averageCPUUsage;
			
			
			/// A pointer to a class that wraps internal state of this SoundDevice.
			Wrapper* wrapper;
			
			
			/// A boolean value indicating whether or not the device is currently valid for use.
			Bool valid;
			
			
			/// A boolean value indicating whether or not the device is currently outputing audio.
			Bool running;
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_DEVICE_H
