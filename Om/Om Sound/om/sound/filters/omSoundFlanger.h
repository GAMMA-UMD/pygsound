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

#ifndef INCLUDE_OM_SOUND_FLANGER_H
#define INCLUDE_OM_SOUND_FLANGER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that periodically varies the phase of an input signal.
/**
  * A Flanger filter takes the input sound and modulates the phase of that
  * sound with a repeating wave function, LFO, mixing the output with the input signal.
  * There is a feedback path from the output to input which produces a more resonant output.
  * This class supports several flanger wave types: sinusoidal, square, saw, and triangle.
  */
class Flanger : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sine-based flanger filter with a depth of 6dB and frequency of 1Hz.
			Flanger();
			
			
			/// Create a flanger with the specified frequency, depth, feedback, and mix values.
			Flanger( Float newFrequency, Float newDepth, Gain newFeedback, Gain newMix );
			
			
		//********************************************************************************
		//******	Flanger Frequency Accessor Methods
			
			
			/// Return the frequency of this flanger's modulation wave in hertz.
			OM_INLINE Float getFrequency() const
			{
				return targetFrequency;
			}
			
			
			/// Set the frequency of this flanger's modulation wave in hertz.
			OM_INLINE void setFrequency( Float newFrequency )
			{
				lockMutex();
				targetFrequency = math::max( newFrequency, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Flanger Depth Accessor Methods
			
			
			/// Return the intensity of the flanger modulation.
			/**
			  * This is a value between 0 and 1 which indicates the unitless amount that the flanger
			  * effect should affect the input signal by. A higher depth affects the signal more
			  * by producing more frequency modulation.
			  */
			OM_INLINE Float getDepth() const
			{
				return targetDepth;
			}
			
			
			/// Set the maximum attenuation of the flanger modulation in decibels.
			/**
			  * This is a value between 0 and 1 which indicates the unitless amount that the flanger
			  * effect should affect the input signal by. A higher depth affects the signal more
			  * by producing more frequency modulation.
			  *
			  * The new depth value is clamped to the range of [0,1].
			  */
			OM_INLINE void setDepth( Float newDepth )
			{
				lockMutex();
				targetDepth = math::clamp( newDepth, Float(0), Float(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Flanger Feedback Accessor Methods
			
			
			/// Return the fraction of the effected sound that is sent back to the flanger's input.
			/**
			  * This is a value between -1 and 1 which indicates how much of the flanger effect
			  * output (pre mix-control) gets sent back to the flanger's input. A higher feedback
			  * value will produce a more extreme resonnant effect. A negative feedback indicates
			  * that the feedbacked audio is inverted.
			  */
			OM_INLINE Gain getFeedback() const
			{
				return targetFeedback;
			}
			
			
			/// Set the fraction of the effected sound that is sent back to the flanger's input.
			/**
			  * This is a value between -1 and 1 which indicates how much of the flanger effect
			  * output (pre mix-control) gets sent back to the flanger's input. A higher feedback
			  * value will produce a more extreme resonnant effect. A negative feedback indicates
			  * that the feedbacked audio is inverted.
			  *
			  * The new feedback value is clamped to the range of [-1,1].
			  */
			OM_INLINE void setFeedback( Gain newFeedback )
			{
				lockMutex();
				targetFeedback = math::clamp( newFeedback, Gain(-1), Gain(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Flanger Mix Accessor Methods
			
			
			/// Return the fraction of the final output sound that is the flanger effect.
			/**
			  * This is a value between 0 and 1 which indicates the mix between the input
			  * signal and the effect output that is produced. A value of 0 results in only
			  * the input being present, while a value of 1 results in only the output being
			  * preset.
			  */
			OM_INLINE Float getMix() const
			{
				return targetMix;
			}
			
			
			/// Set the fraction of the final output sound that is the flanger effect.
			/**
			  * This is a value between 0 and 1 which indicates the mix between the input
			  * signal and the effect output that is produced. A value of 0 results in only
			  * the input being present, while a value of 1 results in only the output being
			  * preset.
			  *
			  * The new mix value is clamped to the range of [0,1].
			  */
			OM_INLINE void setMix( Float newMix )
			{
				lockMutex();
				targetMix = math::clamp( newMix, Float(0), Float(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Flanger Frequency Accessor Methods
			
			
			/// Return the modulation phase offset of the channel with the specified index.
			/**
			  * This value, specified in degrees, indicates how much the phase of the channel
			  * should be shifted by. This parameter allows the creation of stereo (or higher)
			  * modulation effects. For example, if the phase of the left channel is 0 and the phase
			  * of the right channel is 180, the channels' modulation will be completely out-of-phase.
			  */
			OM_INLINE Float getChannelPhase( Index channelIndex ) const
			{
				if ( channelIndex < channelPhase.getSize() )
					return math::radiansToDegrees( channelPhase[channelIndex] );
				else
					return math::radiansToDegrees( globalChannelPhase );
			}
			
			
			/// Set the modulation phase offset of the channel with the specified index.
			/**
			  * This value, specified in degrees, indicates how much the phase of the channel
			  * should be shifted by. This parameter allows the creation of stereo (or higher)
			  * modulation effects. For example, if the phase of the left channel is 0 and the phase
			  * of the right channel is 180, the channels' modulation will be completely out-of-phase.
			  *
			  * The input phase value is clamped so that the new phase value lies between -180 and 180 degrees.
			  */
			void setChannelPhase( Index channelIndex, Float newPhase );
			
			
			/// Set the modulation phase offset for all channels.
			/**
			  * Doing this brings all channels into phase with each other (regardless of what phase that is).
			  *
			  * The input phase value is clamped so that the new phase value lies between -180 and 180 degrees.
			  */
			void setChannelPhase( Float newPhase );
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this flanger.
			/**
			  * The method returns the string "Flanger".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this flanger.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this flanger.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::MODULATION.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this flanger can process audio data in-place.
			/**
			  * This method always returns TRUE, flangers can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Filter Latency Accessor Method
			
			
			/// Return a Time value indicating the latency of this flanger effect in seconds.
			/**
			  * The latency of the flanger is equal to half the maximum delay amount of the
			  * effect, usually just a few milliseconds.
			  */
			virtual Time getLatency() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this flanger has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the flanger parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this flanger.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this flanger.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this flanger.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Filter Parameter Value Accessor Methods
			
			
			/// Place the value of the parameter at the specified index in the output parameter.
			virtual Bool getParameterValue( Index parameterIndex, FilterParameter& value ) const;
			
			
			/// Attempt to set the parameter value at the specified index.
			virtual Bool setParameterValue( Index parameterIndex, const FilterParameter& value );
			
			
		//********************************************************************************
		//******	Private Filter Processing Methods
			
			
			/// Fill the output frame with the amplitude modulated input sound.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Wave Generation Methods
			
			
			/// Compute the value of a cosine wave, given the specified phase value in radians.
			/**
			  * The output of this function is biased so that the sine wave has minima
			  * and maxima at y=0 and y=1.
			  */
			OM_FORCE_INLINE static Sample32f cosine( Float phase )
			{
				return Float(0.5)*(math::cos( phase + math::pi<Float>() ) + Float(1));
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The frequency of the flanger's modulation wave in hertz.
			Float frequency;
			
			
			/// The target frequency of the flanger's modulation wave in hertz.
			/**
			  * This value allows the flanger to do smooth transitions between
			  * different modulation frequencies.
			  */
			Float targetFrequency;
			
			
			/// The intensity of the flanger modulation.
			/**
			  * This is a value between 0 and 1 which indicates the unitless amount that the vibrato
			  * effect should affect the input signal by. A higher depth affects the signal more
			  * by producing more frequency modulation.
			  */
			Float depth;
			
			
			/// The target depth for this flanger.
			/**
			  * This value allows the flanger to do smooth transitions between
			  * different depths.
			  */
			Float targetDepth;
			
			
			/// The fraction of the effected sound that is sent back to the flanger's input.
			/**
			  * This is a value between -1 and 1 which indicates how much of the flanger effect
			  * output (pre mix-control) gets sent back to the flanger's input. A higher feedback
			  * value will produce a more extreme resonnant effect. A negative feedback indicates
			  * that the feedbacked audio is inverted.
			  */
			Gain feedback;
			
			
			/// The target feedback for this flanger.
			/**
			  * This value allows the flanger to do smooth transitions between
			  * different feedback values.
			  */
			Gain targetFeedback;
			
			
			/// The fraction of the final output sound that is the flanger effect.
			/**
			  * This is a value between 0 and 1 which indicates the mix between the input
			  * signal and the effect output that is produced. A value of 0 results in only
			  * the input being present, while a value of 1 results in only the output being
			  * preset.
			  */
			Gain mix;
			
			
			/// The target mix for this flanger.
			/**
			  * This value allows the flanger to do smooth transitions between
			  * different mix values.
			  */
			Gain targetMix;
			
			
			/// The modulation phase offset of each channel (in radians).
			/**
			  * This allows different channels to be in different phases,
			  * creating a stereo (or higher) flanger effect.
			  */
			Array<Float> channelPhase;
			
			
			/// The channel phase offset to use for all channels for which the phase has not been set.
			Float globalChannelPhase;
			
			
			/// The current phase of the flanger's modulation wave (in radians).
			Float phase;
			
			
			/// The maximum delay time in seconds that the flanger effect can use.
			Float maxDelayTime;
			
			
			/// A buffer that is used to hold a delayed copy of the input sound so that it can be frequency modulated.
			SoundBuffer delayBuffer;
			
			
			/// The number of currently valid samples in the delay buffer.
			Size delayBufferSize;
			
			
			/// The current write position for input to the delay buffer.
			Index currentDelayWriteIndex;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FLANGER_H
