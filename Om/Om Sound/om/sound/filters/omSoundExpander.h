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

#ifndef INCLUDE_OM_SOUND_EXPANDER_H
#define INCLUDE_OM_SOUND_EXPANDER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that reduces the level of sound that is below a certain threshold.
/**
  * This expander class uses peak sensing to determine an envelope level at
  * each sample. If the envelope is below a user-defined
  * threshold, the expander applies gain reduction to the sound at the expander's
  * logarithmic compression ratio. The expander also has a variable-hardness
  * knee which allows the user to smooth the transition from gain reduction to no
  * gain reduction.
  *
  * This expander can also be used as a true noise gate by setting the ratio to be equal
  * to positive infinity.
  */
class Expander : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new expander with the default compression parameters.
			/**
			  * These are - threshold: -6dB, ratio: 2:1, knee: 0dB, attack: 2ms,
			  * hold: 100ms, release: 300ms, with unlinked channels.
			  */
			Expander();
			
			
			/// Create a new expander with specified threshold, ratio, attack, hold, and release.
			/**
			  * This expander uses peak-sensing detection and has unlinked
			  * channels. The expander has the default knee of 0dB. All gain and threshold
			  * values are specified on a linear scale. The attack, hold, and release times
			  * are specified in seconds.
			  */
			Expander( Gain threshold, Float ratio, Float attack, Float hold, Float release );
			
			
		//********************************************************************************
		//******	Threshold Accessor Methods
			
			
			/// Return the linear full-scale value below which the expander applies gain reduction.
			OM_INLINE Gain getThreshold() const
			{
				return targetThreshold;
			}
			
			
			/// Return the logarithmic full-scale value below which the expander applies gain reduction.
			OM_INLINE Gain getThresholdDB() const
			{
				return math::linearToDB( targetThreshold );
			}
			
			
			/// Set the linear full-scale value below which the expander applies gain reduction.
			/**
			  * The value is clamped to the valid range of [0,infinity] before being stored.
			  */
			OM_INLINE void setThreshold( Gain newThreshold )
			{
				lockMutex();
				targetThreshold = math::max( newThreshold, Gain(0) );
				unlockMutex();
			}
			
			
			/// Set the logarithmic full-scale value below which the expander applies gain reduction.
			OM_INLINE void setThresholdDB( Gain newThresholdDB )
			{
				lockMutex();
				targetThreshold = math::dbToLinear( newThresholdDB );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Ratio Accessor Methods
			
			
			/// Return the downward expansion ratio that the expander is using.
			/**
			  * This value is expressed as a ratio of input to output gain below
			  * the compression threshold, expressed in decibels. For instance, a
			  * ratio of 2 indicates that for ever 1 decibels that the signal is below
			  * the threshold, the output signal will be attenuated by 2 dB.
			  * Thus, higher ratios indicate more extreme expansion. A ratio of +infinity
			  * is equivalent to a hard noise gate.
			  */
			OM_INLINE Float getRatio() const
			{
				return targetRatio;
			}
			
			
			/// Set the downward expansion ratio that the expander is using.
			/**
			  * This value is expressed as a ratio of input to output gain below
			  * the compression threshold, expressed in decibels. For instance, a
			  * ratio of 2 indicates that for ever 1 decibels that the signal is below
			  * the threshold, the output signal will be attenuated by 2 dB.
			  * Thus, higher ratios indicate more extreme expansion. A ratio of +infinity
			  * is equivalent to a hard noise gate.
			  *
			  * The new ratio is clamped to the range of [1,100].
			  */
			OM_INLINE void setRatio( Float newRatio )
			{
				lockMutex();
				targetRatio = math::clamp( newRatio, Float(1), Float(100) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Knee Accessor Methods
			
			
			/// Return the knee radius of this expander in decibels.
			/**
			  * This is the amount above the expander's threshold at which the expander first
			  * starts reducing level, as well as the amount below the expander's threshold where
			  * the actual expander ratio starts to be used. A higher knee will result it an expander
			  * that starts to apply gain reduction to envelopes that approach the threshold, resulting
			  * in a smoother transition from no gain reduction to full gain reduction.
			  */
			OM_INLINE Gain getKnee() const
			{
				return targetKnee;
			}
			
			
			/// Set the knee radius of this expander in decibels.
			/**
			  * This is the amount above the expander's threshold at which the expander first
			  * starts reducing level, as well as the amount below the expander's threshold where
			  * the actual expander ratio starts to be used. A higher knee will result it an expander
			  * that starts to apply gain reduction to envelopes that approach the threshold, resulting
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
			
			
			/// Return the attack of this expander in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the expander's
			  * detection envelope to respond to a sudden increase in signal level. Thus,
			  * a very small attack softens transients more than a slower attack which 
			  * lets the transients through the expander.
			  */
			OM_INLINE Float getAttack() const
			{
				return attack;
			}
			
			
			/// Set the attack of this expander in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the expander's
			  * detection envelope to respond to a sudden increase in signal level. Thus,
			  * a very small attack softens transients more than a slower attack which 
			  * lets the transients through the expander.
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
		//******	Hold Accessor Methods
			
			
			/// Return the hold time of this expander in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the expander's
			  * detection envelope to start responding to a sudden decrease in signal level. Thus,
			  * the hold time is a window after an initial transient in which the expander's
			  * envelope doesn't decrease, resulting in no further gain reduction until the
			  * hold time is expired, at which time the envelope release begins.
			  */
			OM_INLINE Float getHold() const
			{
				return hold;
			}
			
			
			/// Set the hold time of this expander in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the expander's
			  * detection envelope to start responding to a sudden decrease in signal level. Thus,
			  * the hold time is a window after an initial transient in which the expander's
			  * envelope doesn't decrease, resulting in no further gain reduction until the
			  * hold time is expired, at which time the envelope release begins.
			  *
			  * The new hold value is clamped to the range of [0,+infinity].
			  */
			OM_INLINE void setHold( Float newHold )
			{
				lockMutex();
				hold = math::max( newHold, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Release Accessor Methods
			
			
			/// Return the release of this expander in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the expander's
			  * detection envelope to respond to a sudden decrease in signal level. Thus,
			  * a very short release doesn't compress the signal after a transient for as
			  * long as a longer release. Beware, very short release times (< 5ms) can result
			  * in audible distortion.
			  */
			OM_INLINE Float getRelease() const
			{
				return release;
			}
			
			
			/// Set the release of this expander in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the expander's
			  * detection envelope to respond to a sudden decrease in signal level. Thus,
			  * a very short release doesn't compress the signal after a transient for as
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
			
			
			/// Return whether or not all channels in the expander are linked together.
			/**
			  * If the value is TRUE, all channels are reduced by the maximum
			  * amount selected from all channel envelopes. This allows the expander
			  * to maintain the stereo image of the audio when expanding hard-panned sounds.
			  */
			OM_INLINE Bool getChannelsAreLinked() const
			{
				return linkChannels;
			}
			
			
			/// Set whether or not all channels in the expander are linked together.
			/**
			  * If the value is TRUE, all channels are reduced by the maximum
			  * amount selected from all channel envelopes. This allows the expander
			  * to maintain the stereo image of the audio when expanding hard-panned sounds.
			  */
			OM_INLINE void setChannelsAreLinked( Bool newChannelsAreLinked )
			{
				lockMutex();
				linkChannels = newChannelsAreLinked;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Gain Reduction Accessor Methods
			
			
			/// Return the current gain reduction of the expander in decibels.
			/**
			  * This value can be used as a way for humans to visualize how much the
			  * expander is reducing the signal at any given time.
			  */
			OM_INLINE Gain getGainReductionDB() const
			{
				return currentReduction;
			}
			
			
			/// Return the current gain reduction of the expander on a linear scale.
			/**
			  * This value can be used as a way for humans to visualize how much the
			  * expander is reducing the signal at any given time.
			  */
			OM_INLINE Gain getGainReduction() const
			{
				return math::dbToLinear( currentReduction );
			}
			
			
		//********************************************************************************
		//******	Transfer Function Accessor Methods
			
			
			/// Evaluate the transfer function of the expander for an envelope with the specified amplitude.
			Gain evaluateTransferFunction( Gain input ) const;
			
			
			/// Evaluate the transfer function of the expander for an envelope with the specified amplitude in decibels.
			OM_INLINE Gain evaluateTransferFunctionDB( Gain input ) const
			{
				return math::linearToDB( this->evaluateTransferFunction( math::dbToLinear( input ) ) );
			}
			
			
		//********************************************************************************
		//******	Input and Output Accessor Methods
			
			
			/// Return a human-readable name of the expander input at the specified index.
			/**
			  * The expander has 2 inputs:
			  * - 0: the expander's main input, the source of the signal that is going to be expanded.
			  * - 1: the expander's sidechain input, the main input is expanded using this input if provided.
			  *
			  * The main input's name is "Main Input", while the sidechain's name is "Sidechain".
			  */
			virtual UTF8String getInputName( Index inputIndex ) const;
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this expander.
			/**
			  * The method returns the string "Expander".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this expander.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this expander.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DYNAMICS.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this expander can process audio data in-place.
			/**
			  * This method always returns TRUE, expanders can process audio data in-place.
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
			
			
			/// A string indicating the human-readable name of this expander.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this expander.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this expander.
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
			
			
			/// Downward expand soft passages in the input frame sound and write the result to the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Do expansion processing on the input buffer and place the results in the output buffer.
			/**
			  * This method assumes that none of the expansion parameters changed since the last frame
			  * and thus we can save time by not having to interpolate the parameters.
			  */
			OM_INLINE void expandNoChanges( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
											Gain envelopeAttack, Gain envelopeRelease );
			
			
			/// Do expansion processing on the input buffer and place the results in the output buffer.
			/**
			  * This method allows any of the expansion parameters to change and automatically interpolates
			  * them based on the given changes per sample.
			  */
			OM_INLINE void expand( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
									Gain envelopeAttack, Gain envelopeRelease,
									Gain thresholdChangePerSample, Gain kneeChangePerSample,
									Float ratioChangePerSample );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The threshold, given as a linear full-scale value, below which expansion starts to occur.
			Gain threshold;
			
			
			/// The target threshold, used to smooth changes in the threshold parameter.
			Gain targetThreshold;
			
			
			/// The ratio at which the expander applies gain reduction to signals below the threshold.
			Float ratio;
			
			
			/// The target ratio of the expander, used to smooth ratio parameter changes.
			Float targetRatio;
			
			
			/// The radius of the expander's knee in decibels.
			/**
			  * This is the amount above the expander's threshold at which the expander first
			  * starts reducing level, as well as the amount below the expander's threshold where
			  * the actual expander ratio starts to be used. A higher knee will result it an expander
			  * that starts to apply gain reduction to envelopes that approach the threshold, resulting
			  * in a smoother transition from no gain reduction to full gain reduction.
			  */
			Gain knee;
			
			
			/// The target knee for this expander, used to smooth knee parameter changes.
			Gain targetKnee;
			
			
			/// The time in seconds that the expander envelope takes to respond to an increase in level.
			Float attack;
			
			
			/// The time in seconds after the hold time that the expander envelope takes to respond to a decrease in level.
			Float release;
			
			
			/// The time in seconds that it takes for the expander envelope to move into its release phase.
			Float hold;
			
			
			/// The amount of time in seconds that each envelope channel has been in the 'hold' phase.
			Array<Float> holdTime;
			
			
			/// An array of current envelope values for each of the channels that this expander is processing.
			Array<Float> envelope;
			
			
			/// The current gain reduction of the expander, expressed in decibels.
			Gain currentReduction;
			
			
			/// A boolean value indicating whether or not all channels processed should be linked.
			/**
			  * This means that the same gain reduction amount is applied to all channels. The
			  * expander finds the channel which needs the most gain reduction and uses
			  * that gain reduction for all other channels. This feature allows the expander
			  * to maintain the original stereo (or multichannel) balance between channels.
			  */
			Bool linkChannels;
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_EXPANDER_H
