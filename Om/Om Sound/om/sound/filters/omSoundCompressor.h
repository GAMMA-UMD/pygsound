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

#ifndef INCLUDE_OM_SOUND_COMPRESSOR_H
#define INCLUDE_OM_SOUND_COMPRESSOR_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that reduces the level of sound above a certain threshold.
/**
  * This compressor class uses an arbitrary-length RMS peak sensing function to
  * determine the envelope level at each sample. If the envelope is above a user-defined
  * threshold, the compressor applies gain reduction to the sound at the compressor's
  * logarithmic compression ratio. The compressor also has a variable-hardness
  * knee which allows the user to smooth the transition from compressed to non-
  * compressed audio.
  *
  * This compressor can also be used as a limiter by setting the ratio to be equal
  * to positive infinity.
  */
class Compressor : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new compressor with the default compression parameters.
			/**
			  * These are - threshold: -6dB, ratio: 2:1, knee: 0dB, attack: 15ms, release: 50ms,
			  * input gain: 0dB, output gain: 0dB, RMS time of 0 seconds (peak detection),
			  * with unlinked channels.
			  */
			Compressor();
			
			
			/// Create a new compressor with specified linear threshold, ratio, attack, and release.
			/**
			  * This compressor uses peak-sensing detection (RMS time of 0), and has unlinked
			  * channels. The compressor has the default knee of 0dB. The input and output
			  * gains of the compressor are 0dB. All gain and threshold values are specified
			  * on a linear scale.
			  */
			Compressor( Gain threshold, Float ratio, Float attack, Float release );
			
			
		//********************************************************************************
		//******	Input Gain Accessor Methods
			
			
			/// Return the current linear input gain factor of this compressor.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * compressor. This allows the user to scale the input to match the compressor
			  * without having to change the compressor threshold.
			  */
			OM_INLINE Gain getInputGain() const
			{
				return targetInputGain;
			}
			
			
			/// Return the current input gain factor in decibels of this compressor.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * compressor. This allows the user to scale the input to match the compressor
			  * without having to change the compressor threshold.
			  */
			OM_INLINE Gain getInputGainDB() const
			{
				return math::linearToDB( targetInputGain );
			}
			
			
			/// Set the target linear input gain for compressor.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * compressor. This allows the user to scale the input to match the compressor
			  * without having to change the compressor threshold.
			  */
			OM_INLINE void setInputGain( Gain newInputGain )
			{
				lockMutex();
				targetInputGain = newInputGain;
				unlockMutex();
			}
			
			
			/// Set the target input gain in decibels for this compressor.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * compressor. This allows the user to scale the input to match the compressor
			  * without having to change the compressor threshold.
			  */
			OM_INLINE void setInputGainDB( Gain newDBInputGain )
			{
				lockMutex();
				targetInputGain = math::dbToLinear( newDBInputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Output Gain Accessor Methods
			
			
			/// Return the current linear output gain factor of this compressor.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * compressor. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE Gain getOutputGain() const
			{
				return targetOutputGain;
			}
			
			
			/// Return the current output gain factor in decibels of this compressor.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * compressor. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE Gain getOutputGainDB() const
			{
				return math::linearToDB( targetOutputGain );
			}
			
			
			/// Set the target linear output gain for this compressor.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * compressor. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE void setOutputGain( Gain newOutputGain )
			{
				lockMutex();
				targetOutputGain = newOutputGain;
				unlockMutex();
			}
			
			
			/// Set the target output gain in decibels for this compressor.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * compressor. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE void setOutputGainDB( Gain newDBOutputGain )
			{
				lockMutex();
				targetOutputGain = math::dbToLinear( newDBOutputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Output Mix Accessor Methods
			
			
			/// Return the ratio of input signal to compressed signal sent to the output of the compressor.
			/**
			  * Valid mix values are in the range [0,1].
			  * A mix value of 1 indicates that only the output of the compressor should be
			  * heard at the output, while a value of 0 indicates that only the input of the
			  * compressor should be heard at the output. A value of 0.5 indicates that both
			  * should be mixed together equally at -6dB.
			  */
			OM_INLINE Gain getMix() const
			{
				return targetMix;
			}
			
			
			/// Set the ratio of input signal to compressed signal sent to the output of the compressor.
			/**
			  * Valid mix values are in the range [0,1].
			  * A mix value of 1 indicates that only the output of the compressor should be
			  * heard at the output, while a value of 0 indicates that only the input of the
			  * compressor should be heard at the output. A value of 0.5 indicates that both
			  * should be mixed together equally at -6dB.
			  *
			  * The new mix value is clamped to the valid range of [0,1].
			  */
			OM_INLINE void setMix( Gain newMix )
			{
				lockMutex();
				targetMix = math::clamp( newMix, Gain(0), Gain(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Threshold Accessor Methods
			
			
			/// Return the linear full-scale value above which the compressor applies gain reduction.
			OM_INLINE Gain getThreshold() const
			{
				return targetThreshold;
			}
			
			
			/// Return the logarithmic full-scale value above which the compressor applies gain reduction.
			OM_INLINE Gain getThresholdDB() const
			{
				return math::linearToDB( targetThreshold );
			}
			
			
			/// Set the linear full-scale value above which the compressor applies gain reduction.
			/**
			  * The value is clamped to the valid range of [0,infinity] before being stored.
			  */
			OM_INLINE void setThreshold( Gain newThreshold )
			{
				lockMutex();
				targetThreshold = math::max( newThreshold, Gain(0) );
				unlockMutex();
			}
			
			
			/// Set the logarithmic full-scale value above which the compressor applies gain reduction.
			OM_INLINE void setThresholdDB( Gain newThresholdDB )
			{
				lockMutex();
				targetThreshold = math::dbToLinear( newThresholdDB );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Ratio Accessor Methods
			
			
			/// Return the compression ratio that the compressor is using.
			/**
			  * This value is expressed as a ratio of input to output gain above
			  * the compression threshold, expressed in decibels. For instance, a
			  * ratio of 2 indicates that for ever 2 decibels that the signal is over
			  * the threshold, the output signal will only be 1 decibel over the threshold.
			  * Thus, higher ratios indicate harder compression. A ratio of +infinity
			  * is equivalent to a brickwall limiter.
			  */
			OM_INLINE Float getRatio() const
			{
				return targetRatio;
			}
			
			
			/// Set the compression ratio that the compressor is using.
			/**
			  * This value is expressed as a ratio of input to output gain above
			  * the compression threshold, expressed in decibels. For instance, a
			  * ratio of 2 indicates that for ever 2 decibels that the signal is over
			  * the threshold, the output signal will only be 1 decibel over the threshold.
			  * Thus, higher ratios indicate harder compression. A ratio of +infinity
			  * is equivalent to a brickwall limiter.
			  *
			  * The new ratio is clamped to the range of [1,+infinity].
			  */
			OM_INLINE void setRatio( Float newRatio )
			{
				lockMutex();
				targetRatio = math::max( newRatio, Float(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Knee Accessor Methods
			
			
			/// Return the knee radius of this compressor in decibels.
			/**
			  * This is the amount below the compressor's threshold at which the compressor first
			  * starts compressing, as well as the amount above the compressor's threshold where
			  * the actual compressor ratio starts to be used. A higher knee will result in a compressor
			  * that starts to apply gain reduction to envelopes that approach the threshold, resulting
			  * in a smoother transition from no gain reduction to full gain reduction.
			  */
			OM_INLINE Gain getKnee() const
			{
				return targetKnee;
			}
			
			
			/// Set the knee radius of this compressor in decibels.
			/**
			  * This is the amount below the compressor's threshold at which the compressor first
			  * starts compressing, as well as the amount above the compressor's threshold where
			  * the actual compressor ratio starts to be used. A higher knee will result in a compressor
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
			
			
			/// Return the attack of this compressor in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the compressor's
			  * detection envelope to respond to a sudden increase in signal level. Thus,
			  * a very small attack softens transients more than a slower attack which 
			  * lets the transients through the compressor.
			  */
			OM_INLINE Float getAttack() const
			{
				return attack;
			}
			
			
			/// Set the attack of this compressor in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the compressor's
			  * detection envelope to respond to a sudden increase in signal level. Thus,
			  * a very small attack softens transients more than a slower attack which 
			  * lets the transients through the compressor.
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
			
			
			/// Return the release of this compressor in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the compressor's
			  * detection envelope to respond to a sudden decrease in signal level. Thus,
			  * a very short release doesn't compress the signal after a transient for as
			  * long as a longer release. Beware, very short release times (< 5ms) can result
			  * in audible distortion.
			  */
			OM_INLINE Float getRelease() const
			{
				return release;
			}
			
			
			/// Set the release of this compressor in seconds.
			/**
			  * This value indicates the time in seconds that it takes for the compressor's
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
		//******	RMS Time Accessor Methods
			
			
			/// Return the RMS averaging time for the compressor, expressed in seconds.
			/**
			  * This value indicates the total time in seconds over which the compressor
			  * is applying an RMS averaging function. The default value is 0, indicating
			  * that no RMS detection is desired, peak detection is used instead. A compressor
			  * with a longer RMS time will compress the input signal more smoothly than
			  * peak detection but may not respond to transients as quickly.
			  */
			OM_INLINE Float getRMSTime() const
			{
				return rmsTime;
			}
			
			
			/// Set the RMS averaging time for the compressor, expressed in seconds.
			/**
			  * This value indicates the total time in seconds over which the compressor
			  * is applying an RMS averaging function. The default value is 0, indicating
			  * that no RMS detection is desired, peak detection is used instead. A compressor
			  * with a longer RMS time will compress the input signal more smoothly than
			  * peak detection but may not respond to transients as quickly.
			  *
			  * The new RMS averaging time is clamped to the valid range of [0,+infinity].
			  */
			OM_INLINE void setRMSTime( Float newRMSTime )
			{
				lockMutex();
				rmsTime = math::max( newRMSTime, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Channel Link Status Accessor Methods
			
			
			/// Return whether or not all channels in the compressor are linked together.
			/**
			  * If the value is TRUE, all channels are compressed by the maximum compression
			  * amount selected from all channel envelopes. This allows the compressor
			  * to maintain the stereo image of the audio when compressing hard-panned sounds.
			  */
			OM_INLINE Bool getChannelsAreLinked() const
			{
				return linkChannels;
			}
			
			
			/// Set whether or not all channels in the compressor are linked together.
			/**
			  * If the value is TRUE, all channels are compressed by the maximum compression
			  * amount selected from all channel envelopes. This allows the compressor
			  * to maintain the stereo image of the audio when compressing hard-panned sounds.
			  */
			OM_INLINE void setChannelsAreLinked( Bool newChannelsAreLinked )
			{
				lockMutex();
				linkChannels = newChannelsAreLinked;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Gain Reduction Accessor Methods
			
			
			/// Return the current gain reduction of the compressor in decibels.
			/**
			  * This value can be used as a way for humans to visualize how much the
			  * compressor is compressing at any given time.
			  */
			OM_INLINE Gain getGainReductionDB() const
			{
				return currentReduction;
			}
			
			
			/// Return the current gain reduction of the compressor on a linear scale.
			/**
			  * This value can be used as a way for humans to visualize how much the
			  * compressor is compressing at any given time.
			  */
			OM_INLINE Gain getGainReduction() const
			{
				return math::dbToLinear( currentReduction );
			}
			
			
		//********************************************************************************
		//******	Transfer Function Accessor Methods
			
			
			/// Evaluate the transfer function of the compressor for an envelope with the specified amplitude.
			/**
			  * This method returns the output gain applied 
			  */
			Gain evaluateTransferFunction( Gain input ) const;
			
			
			/// Evaluate the transfer function of the compressor for an envelope with the specified amplitude.
			OM_INLINE Gain evaluateTransferFunctionDB( Gain input ) const
			{
				return math::linearToDB( this->evaluateTransferFunction( math::dbToLinear( input ) ) );
			}
			
			
		//********************************************************************************
		//******	Input and Output Accessor Methods
			
			
			/// Return a human-readable name of the compressor input at the specified index.
			/**
			  * The compressor has 2 inputs:
			  * - 0: the compressor's main input, the source of the signal that is going to be compressed.
			  * - 1: the compressor's sidechain input, the main input is compressed using this input if provided.
			  *
			  * The main input's name is "Main Input", while the sidechain's name is "Sidechain".
			  */
			virtual UTF8String getInputName( Index inputIndex ) const;
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this compressor.
			/**
			  * The method returns the string "Compressor".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this compressor.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this compressor.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DYNAMICS.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this compressor can process audio data in-place.
			/**
			  * This method always returns TRUE, compressors can process audio data in-place.
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
			
			
			/// A string indicating the human-readable name of this compressor.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this compressor.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this compressor.
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
			
			
			/// Do compression processing on the input buffer and place the results in the output buffer.
			template < Bool interpolateChanges, Bool rmsEnabled >
			OM_FORCE_INLINE void compress( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
										Gain envelopeAttack, Gain envelopeRelease,
										Gain inputGainChangePerSample, Gain outputGainChangePerSample,
										Gain mixChangePerSample, Gain thresholdChangePerSample,
										Gain kneeChangePerSample, Float ratioChangePerSample );
			
			
			/// Return the negative gain reduction in decibels for the specified signal level and compression parameters.
			OM_FORCE_INLINE static Gain getDBReduction( Float level, Gain threshold, Float ratio,
														Float kneeMin, Float kneeMax, Float knee )
			{
				Float reductionConstant = (Float(1) - ratio)/ratio;
				Gain dbOver = math::linearToDB( level / threshold );
				
				if ( knee > Float(0) && level < kneeMax )
				{
					Float x = (dbOver + knee)/knee;
					return knee*reductionConstant*x*x*Float(0.25);
				}
				else
					return dbOver*reductionConstant;
			}
			
			
			/// Return the negative gain reduction in decibels for the specified signal level and compression parameters.
			OM_FORCE_INLINE static Gain getDBReduction2( Float level, Gain threshold, Float reductionConstant,
														Float kneeMin, Float kneeMax, Float knee )
			{
				Gain dbOver = math::linearToDB( level / threshold );
				
				if ( knee > Float(0) && level < kneeMax )
				{
					Float x = (dbOver + knee)/knee;
					return knee*reductionConstant*x*x*Float(0.25);
				}
				else
					return dbOver*reductionConstant;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The threshold, given as a linear full-scale value, at which compression starts to occur.
			Gain threshold;
			
			
			/// The target threshold, used to smooth changes in the threshold parameter.
			Gain targetThreshold;
			
			
			/// The ratio at which the compressor applies gain reduction to signals above the threshold.
			Float ratio;
			
			
			/// The target ratio of the compressor, used to smooth ratio parameter changes.
			Float targetRatio;
			
			
			/// The radius of the compressor's knee in decibels.
			/**
			  * This is the amount below the compressor's threshold at which the compressor first
			  * starts compressing, as well as the amount above the compressor's threshold where
			  * the actual compressor ratio starts to be used. A higher knee will result in a compressor
			  * that starts to apply gain reduction to envelopes that approach the threshold, resulting
			  * in a smoother transition from no gain reduction to full gain reduction.
			  */
			Gain knee;
			
			
			/// The target knee for this compressor, used to smooth knee parameter changes.
			Gain targetKnee;
			
			
			/// The linear gain applied to the signal before it goes through the compressor.
			Gain inputGain;
			
			
			/// The target input gain of the compressor, used to smooth input gain parameter changes.
			Gain targetInputGain;
			
			
			/// The linear gain applied to the signal after it has been compressed to restore signal level.
			Gain outputGain;
			
			
			/// The target output gain of the compressor, used to smooth output gain parameter changes.
			Gain targetOutputGain;
			
			
			/// The ratio of input signal to compressed signal sent to the output of the compressor.
			/**
			  * The mix factor determines the ratio of the input signal (post input gain) to the
			  * compressed signal that is sent to the final output buffer. Thus, a mix factor
			  * of 1 indicates only the compressed signal is sent to the output. Likewise, a mix
			  * factor of 0 indicates that only the input signal is sent to the output.
			  */
			Gain mix;
			
			
			/// The target mix factor of the compressor, used to smooth mix parameter changes.
			Gain targetMix;
			
			
			/// The time in seconds that the compressor envelope takes to respond to an increase in level.
			Float attack;
			
			
			/// The time in seconds that the compressor envelope takes to respond to a decrease in level.
			Float release;
			
			
			/// An array of envelope values for each of the channels that this compressor is processing.
			Array<Float> envelope;
			
			
			/// The time in seconds for which we are computing the RMS level of the input signal.
			Float rmsTime;
			
			
			/// The active length of the RMS buffer in samples.
			Size rmsLengthInSamples;
			
			
			/// The sum of the squares of the active RMS samples.
			Array<Float> rmsSumSquares;
			
			
			/// The current sample index within the RMS buffer.
			Size currentRMSIndex;
			
			
			/// A buffer used to store the last N samples, used in RMS level detection.
			SoundBuffer rmsBuffer;
			
			
			/// The current gain reduction of the compressor, expressed in decibels.
			Gain currentReduction;
			
			
			/// A boolean value indicating whether or not all channels processed should be linked.
			/**
			  * This means that the same compression amount is applied to all channels. The
			  * compressor finds the channel which needs the most gain reduction and uses
			  * that gain reduction for all other channels. This feature allows the compressor
			  * to maintain the original stereo (or multichannel) balance between channels.
			  */
			Bool linkChannels;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_COMPRESSOR_H
