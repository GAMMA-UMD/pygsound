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

#ifndef INCLUDE_OM_SOUND_LIMITER_H
#define INCLUDE_OM_SOUND_LIMITER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that keeps sound from going above a limiting threshold.
class Limiter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new limiter with the default limiting parameters.
			/**
			  * These are - threshold: 0dB, release: 5ms, input gain: 0dB,
			  * output gain: 0dB, with unlinked channels.
			  */
			Limiter();
			
			
			/// Create a new limiter with the specified threshold.
			/**
			  * The other limiting parameters are - release: 5ms, input gain: 0dB,
			  * output gain: 0dB, with unlinked channels.
			  */
			Limiter( Gain threshold, Gain inputGain, Gain outputGain, Float release );
			
			
		//********************************************************************************
		//******	Input Gain Accessor Methods
			
			
			/// Return the current linear input gain factor of this limiter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * limiter. This allows the user to scale the input to match the limiter
			  * without having to change the limiter threshold.
			  */
			OM_INLINE Gain getInputGain() const
			{
				return targetInputGain;
			}
			
			
			/// Return the current input gain factor in decibels of this limiter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * limiter. This allows the user to scale the input to match the limiter
			  * without having to change the limiter threshold.
			  */
			OM_INLINE Gain getInputGainDB() const
			{
				return math::linearToDB( targetInputGain );
			}
			
			
			/// Set the target linear input gain for limiter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * limiter. This allows the user to scale the input to match the limiter
			  * without having to change the limiter threshold.
			  */
			OM_INLINE void setInputGain( Gain newInputGain )
			{
				lockMutex();
				targetInputGain = newInputGain;
				unlockMutex();
			}
			
			
			/// Set the target input gain in decibels for this limiter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * limiter. This allows the user to scale the input to match the limiter
			  * without having to change the limiter threshold.
			  */
			OM_INLINE void setInputGainDB( Gain newDBInputGain )
			{
				lockMutex();
				targetInputGain = math::dbToLinear( newDBInputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Output Gain Accessor Methods
			
			
			/// Return the current linear output gain factor of this limiter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * limiter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE Gain getOutputGain() const
			{
				return targetOutputGain;
			}
			
			
			/// Return the current output gain factor in decibels of this limiter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * limiter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE Gain getOutputGainDB() const
			{
				return math::linearToDB( targetOutputGain );
			}
			
			
			/// Set the target linear output gain for this limiter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * limiter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE void setOutputGain( Gain newOutputGain )
			{
				lockMutex();
				targetOutputGain = newOutputGain;
				unlockMutex();
			}
			
			
			/// Set the target output gain in decibels for this limiter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * limiter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE void setOutputGainDB( Gain newDBOutputGain )
			{
				lockMutex();
				targetOutputGain = math::dbToLinear( newDBOutputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Threshold Accessor Methods
			
			
			/// Return the linear full-scale value above which the limiter applies gain reduction.
			OM_INLINE Gain getThreshold() const
			{
				return targetThreshold;
			}
			
			
			/// Return the logarithmic full-scale value above which the limiter applies gain reduction.
			OM_INLINE Gain getThresholdDB() const
			{
				return math::linearToDB( targetThreshold );
			}
			
			
			/// Set the linear full-scale value above which the limiter applies gain reduction.
			/**
			  * The value is clamped to the valid range of [0,infinity] before being stored.
			  */
			OM_INLINE void setThreshold( Gain newThreshold )
			{
				lockMutex();
				targetThreshold = math::max( newThreshold, Gain(0) );
				unlockMutex();
			}
			
			
			/// Set the logarithmic full-scale value above which the limiter applies gain reduction.
			OM_INLINE void setThresholdDB( Gain newThresholdDB )
			{
				lockMutex();
				targetThreshold = math::dbToLinear( newThresholdDB );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Knee Accessor Methods
			
			
			/// Return the knee radius of this limiter in decibels.
			/**
			  * This is the amount below the limiter's threshold at which the limiter first
			  * starts limiting. A higher knee will result in a limiter that starts to apply
			  * gain reduction to envelopes that approach the threshold, resulting
			  * in a smoother transition from no gain reduction to full gain reduction.
			  */
			OM_INLINE Gain getKnee() const
			{
				return targetKnee;
			}
			
			
			/// Set the knee radius of this limiter in decibels.
			/**
			  * This is the amount below the limiter's threshold at which the limiter first
			  * starts limiting. A higher knee will result in a limiter that starts to apply
			  * gain reduction to envelopes that approach the threshold, resulting
			  * in a smoother transition from no gain reduction to full gain reduction.
			  *
			  * The new knee value is clamped to the valid range of [0,+infinity].
			  */
			OM_INLINE void setKnee( Gain newKnee )
			{
				lockMutex();
				targetKnee = math::max( newKnee, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Attack Accessor Methods
			
			
			/// Return the attack of this limiter in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the limiter's
			  * detection envelope to respond to a sudden increase in signal level. Thus,
			  * a very small attack softens transients more than a slower attack which 
			  * lets the transients through the limiter.
			  */
			OM_INLINE Float getAttack() const
			{
				return attack;
			}
			
			
			/// Set the attack of this limiter in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the limiter's
			  * detection envelope to respond to a sudden increase in signal level. Thus,
			  * a very small attack softens transients more than a slower attack which 
			  * lets the transients through the limiter.
			  *
			  * The new attack value is clamped to the range of [0,+infinity].
			  */
			OM_INLINE void setAttack( Float newAttack )
			{
				lockMutex();
				attack = math::max( newAttack, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Release Accessor Methods
			
			
			/// Return the release of this limiter in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the limiter's
			  * detection envelope to respond to a sudden decrease in signal level. Thus,
			  * a very short release doesn't compress the signal after a transient for as
			  * long as a longer release. Beware, very short release times (< 5ms) can result
			  * in audible distortion.
			  */
			OM_INLINE Float getRelease() const
			{
				return release;
			}
			
			
			/// Set the release of this limiter in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the limiter's
			  * detection envelope to respond to a sudden decrease in signal level. Thus,
			  * a very short release doesn't limit the signal after a transient for as
			  * long as a longer release. Beware, very short release times (< 5ms) can result
			  * in audible distortion.
			  *
			  * The new release value is clamped to the valid range of [0,+infinity].
			  */
			OM_INLINE void setRelease( Float newRelease )
			{
				lockMutex();
				release = math::max( newRelease, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Channel Link Status Accessor Methods
			
			
			/// Return whether or not all channels in the limiter are linked together.
			/**
			  * If the value is TRUE, all channels are limited by the maximum limiting
			  * amount selected from all channel envelopes. This allows the limiter
			  * to maintain the stereo image of the audio when limiting hard-panned sounds.
			  */
			OM_INLINE Bool getChannelsAreLinked() const
			{
				return linkChannels;
			}
			
			
			/// Set whether or not all channels in the limiter are linked together.
			/**
			  * If the value is TRUE, all channels are limited by the maximum limiting
			  * amount selected from all channel envelopes. This allows the limiter
			  * to maintain the stereo image of the audio when limiting hard-panned sounds.
			  */
			OM_INLINE void setChannelsAreLinked( Bool newChannelsAreLinked )
			{
				lockMutex();
				linkChannels = newChannelsAreLinked;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Channel Link Status Accessor Methods
			
			
			/// Return whether or not output saturation is occurring for the limiter.
			/**
			  * If this value is TRUE, a soft clipping function is applied to the output of the
			  * limiter which guarantees that the output signal will never exceed the threshold.
			  * This output saturation is usually used in combination with a slower attack to keep
			  * fast transients from going over the threshold while keeping the cleaner sound of
			  * a slow attack.
			  */
			OM_INLINE Bool getSaturationIsEnabled() const
			{
				return saturateOutput;
			}
			
			
			/// Set whether or not output saturation should occur for the limiter.
			/**
			  * If this value is TRUE, a soft clipping function is applied to the output of the
			  * limiter which guarantees that the output signal will never exceed the threshold.
			  * This output saturation is usually used in combination with a slower attack to keep
			  * fast transients from going over the threshold while keeping the cleaner sound of
			  * a slow attack.
			  */
			OM_INLINE void setSaturationIsEnabled( Bool newSaturationIsEnabled )
			{
				lockMutex();
				saturateOutput = newSaturationIsEnabled;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Saturation Knee Accessor Methods
			
			
			/// Return the knee of the output clipping function, in decibels.
			/**
			  * This is the number of decibels below the threshold where the transition from
			  * no saturation to saturation will occurr. A higher knee value indicates softer
			  * output clipping.
			  */
			OM_INLINE Gain getSaturationKnee() const
			{
				return targetSaturationKnee;
			}
			
			
			/// Set the knee of the output clipping function, in decibels.
			/**
			  * This is the number of decibels below the threshold where the transition from
			  * no saturation to saturation will occurr. A higher knee value indicates softer
			  * output clipping.
			  *
			  * The new knee value is clamped to the valid range of [0.01,+infinity].
			  */
			OM_INLINE void setSaturationKnee( Gain newSaturationKnee )
			{
				lockMutex();
				targetSaturationKnee = math::max( newSaturationKnee, Float(0.01) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Gain Reduction Accessor Methods
			
			
			/// Return the current gain reduction of the limiter in decibels.
			/**
			  * This value can be used as a way for humans to visualize how much the
			  * limiter is compressing at any given time.
			  */
			OM_INLINE Gain getGainReductionDB() const
			{
				return currentReduction;
			}
			
			
			/// Return the current gain reduction of the limiter on a linear scale.
			/**
			  * This value can be used as a way for humans to visualize how much the
			  * limiter is compressing at any given time.
			  */
			OM_INLINE Gain getGainReduction() const
			{
				return math::dbToLinear( currentReduction );
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this limiter.
			/**
			  * The method returns the string "Compressor".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this limiter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this limiter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DYNAMICS.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this limiter can process audio data in-place.
			/**
			  * This method always returns TRUE, limiters can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the filter parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this limiter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this limiter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this limiter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Filter Parameter Value Accessor Methods
			
			
			/// Place the value of the parameter at the specified index in the output parameter.
			virtual Bool getParameterValue( Index parameterIndex, FilterParameter& value ) const;
			
			
			/// Attempt to set the parameter value at the specified index.
			virtual Bool setParameterValue( Index parameterIndex, const FilterParameter& value );
			
			
		//********************************************************************************
		//******	Private Stream Reset Method
			
			
			/// A method that is called whenever the filter's stream of audio is being reset.
			/**
			  * This method allows the filter to reset all parameter interpolation
			  * and processing to its initial state to avoid coloration from previous
			  * audio or parameter values.
			  */
			virtual void resetStream();
			
			
		//********************************************************************************
		//******	Private Filter Processing Methods
			
			
			/// Do compression processing on the input frame and place the results in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Do limiting processing on the input buffer and place the results in the output buffer.
			/**
			  * This method assumes that none of the limiting parameters changed since the last frame
			  * and thus we can save time by not having to interpolate the parameters.
			  */
			OM_INLINE void limitNoChanges( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
											Gain envelopeAttack, Gain envelopeRelease );
			
			
			/// Do limiting processing on the input buffer and place the results in the output buffer.
			/**
			  * This method assumes that none of the limiting parameters changed since the last frame
			  * and thus we can save time by not having to interpolate the parameters.
			  */
			OM_INLINE void limit( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
									Gain envelopeAttack, Gain envelopeRelease,
									Gain inputGainChangePerSample, Gain outputGainChangePerSample,
									Gain thresholdChangePerSample, Gain kneeChangePerSample,
									Gain saturationKneeChangePerSample );
			
			
			/// Do limiting processing on the input buffer and place the results in the output buffer.
			template < Bool interpolateChanges, Bool saturationEnabled >
			OM_FORCE_INLINE void limit( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
										Gain envelopeAttack, Gain envelopeRelease,
										Gain inputGainChangePerSample, Gain outputGainChangePerSample,
										Gain thresholdChangePerSample, Gain kneeChangePerSample,
										Gain saturationKneeChangePerSample );
			
			
			/// Return the negative gain reduction in decibels for the specified signal level and compression parameters.
			OM_FORCE_INLINE static Gain getDBReduction( Float level, Gain threshold,
														Float kneeMin, Float kneeMax, Float knee )
			{
				Gain dbOver = math::linearToDB( level / threshold );
				
				if ( knee > Float(0) && level < kneeMax )
				{
					Float x = (dbOver + knee)/knee;
					return -knee*x*x*Float(0.25);
				}
				else
					return -dbOver;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The threshold, given as a linear full-scale value, at which compression starts to occur.
			Gain threshold;
			
			
			/// The target threshold, used to smooth changes in the threshold parameter.
			Gain targetThreshold;
			
			
			/// The linear gain applied to the signal before it goes through the limiter.
			Gain inputGain;
			
			
			/// The target input gain of the limiter, used to smooth input gain parameter changes.
			Gain targetInputGain;
			
			
			/// The linear gain applied to the signal after it has been compressed to restore signal level.
			Gain outputGain;
			
			
			/// The target output gain of the limiter, used to smooth output gain parameter changes.
			Gain targetOutputGain;
			
			
			/// The radius of the limiter's knee in decibels.
			/**
			  * This is the amount below the limiter's threshold at which the limiter first
			  * starts limiting. A higher knee will result in a limiter that starts to apply
			  * gain reduction to envelopes that approach the threshold, resulting
			  * in a smoother transition from no gain reduction to full gain reduction.
			  */
			Gain knee;
			
			
			/// The target knee for this limiter, used to smooth knee parameter changes.
			Gain targetKnee;
			
			
			/// The time in seconds that the limiter envelope takes to respond to an increase in level.
			Float attack;
			
			
			/// The time in seconds that the limiter envelope takes to respond to a decrease in level.
			Float release;
			
			
			/// The knee of the output clipping function, in decibels.
			/**
			  * This is the number of decibels below the threshold where the transition from
			  * no saturation to saturation will occurr. A higher knee value indicates softer output clipping.
			  */
			Gain saturationKnee;
			
			
			/// The target saturation knee for this limiter, used to smooth clipping knee parameter changes.
			Gain targetSaturationKnee;
			
			
			/// An array of envelope values for each of the channels that this limiter is processing.
			Array<Float> envelope;
			
			
			/// The current gain reduction of the limiter, expressed in decibels.
			Gain currentReduction;
			
			
			/// A boolean value indicating whether or not all channels processed should be linked.
			/**
			  * This means that the same compression amount is applied to all channels. The
			  * limiter finds the channel which needs the most gain reduction and uses
			  * that gain reduction for all other channels. This feature allows the limiter
			  * to maintain the original stereo (or multichannel) balance between channels.
			  */
			Bool linkChannels;
			
			
			/// A boolean value indicating whether or not output saturation should occur.
			/**
			  * If this value is TRUE, a soft clipping function is applied to the output of the
			  * limiter which guarantees that the output signal will never exceed the threshold.
			  * This output saturation is usually used in combination with a slower attack to keep
			  * fast transients from going over the threshold while keeping the cleaner sound of
			  * a slow attack.
			  */
			Bool saturateOutput;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_LIMITER_H
