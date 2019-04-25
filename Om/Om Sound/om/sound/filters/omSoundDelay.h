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

#ifndef INCLUDE_OM_SOUND_DELAY_H
#define INCLUDE_OM_SOUND_DELAY_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"
#include "omSoundCutoffFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that mixes input sound with a delayed version of itself.
/**
  * This class represents a generic delay-style effect. It can be switched between
  * comb filtering and all-pass delay.
  */
class Delay : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Delay Type Enum Declaration
			
			
			/// An enum type which describes the various types of delay effects that a Delay can produce.
			enum DelayType
			{
				/// The delay filter behaves as a comb filter (the same as a standard delay effect).
				COMB = 0,
				
				/// The delay filter behaves as an all-pass filter.
				ALL_PASS = 1
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a comb delay filter with 500ms delay time, 0 delay feedback, 0dB delay gain, and 0dB dry gain.
			Delay();
			
			
			/// Create a delay filter with the specified type and delay parameters.
			/**
			  * This constructor creates a filter with the specified delay time, delay
			  * feedback gain, delay output gain, and input-to-output gain.
			  */
			Delay( DelayType newType, Float newDelayTime, Gain newFeedbackGain, Gain newDelayGain, Gain newDryGain );
			
			
			/// Create an exact copy of the specified delay filter.
			Delay( const Delay& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this delay filter, releasing all internal resources.
			~Delay();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another delay filter to this one.
			Delay& operator = ( const Delay& other );
			
			
		//********************************************************************************
		//******	Delay Effect Type Accessor Methods
			
			
			/// Return the kind of delay effect that this delay filter is producing.
			OM_INLINE DelayType getType() const
			{
				return type;
			}
			
			
			/// Set the kind of delay effect that this delay filter is producing.
			OM_INLINE void setType( DelayType newType )
			{
				lockMutex();
				type = newType;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Delay Time Accessor Methods
			
			
			/// Return the delay time for this delay filter in seconds.
			OM_INLINE Float getDelayTime() const
			{
				return targetDelayTime;
			}
			
			
			/// Set the delay time for this delay filter in seconds.
			OM_INLINE void setDelayTime( Float newDelayTime )
			{
				lockMutex();
				targetDelayTime = math::max( newDelayTime, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Decay Time Accessor Methods
			
			
			/// Return the time it takes for the output of this delay filter to decay to -60dB.
			/**
			  * This method computes the decay time of the delay filter using the current values
			  * for the feedback gain and delay time of the delay filter.
			  */
			OM_INLINE Float getDecayTime() const
			{
				return targetDelayTime*math::log( Float(0.001), targetFeedbackGain );
			}
			
			
			/// Set the time it takes for the output of this delay filter to decay to -60dB.
			/**
			  * This method uses the current delay filter delay time to compute the feedback
			  * gain necessary to produce the desired decay time.
			  */
			OM_INLINE void setDecayTime( Float newDecayTime )
			{
				lockMutex();
				Float desiredGain = math::pow( Float(0.001),
							targetDelayTime / math::max( newDecayTime, math::epsilon<Float>() ) );
				
				targetFeedbackGain = math::clamp( desiredGain, Float(-0.999), Float(0.999) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Feedback Gain Accessor Methods
			
			
			/// Return the feedback gain of this delay filter.
			/**
			  * This value represents how much of each output delay sample is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value should be between -0.99999 and 0.99999 in order to ensure
			  * filter stability.
			  */
			OM_INLINE Gain getFeedbackGain() const
			{
				return targetFeedbackGain;
			}
			
			
			/// Return the feedback gain of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the output delay sample that is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value should be between -infinity and -0.00001 in order to ensure
			  * filter stability.
			  */
			OM_INLINE Gain getFeedbackGainDB() const
			{
				return math::linearToDB( targetFeedbackGain );
			}
			
			
			/// Set the feedback gain of this delay filter.
			/**
			  * This value represents how much of each output delay sample is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value is clamped to be between -0.99999 and 0.99999 in order to ensure
			  * filter stability.
			  */
			OM_INLINE void setFeedbackGain( Gain newFeedbackGain )
			{
				lockMutex();
				targetFeedbackGain = math::clamp( newFeedbackGain, Gain(-0.9999), Gain(0.9999) );
				unlockMutex();
			}
			
			
			/// Set the feedback gain of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the output delay sample that is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value should be between -infinity and -0.00001 in order to ensure
			  * filter stability.
			  */
			OM_INLINE void setFeedbackGainDB( Gain newFeedbackGain )
			{
				this->setFeedbackGain( math::dbToLinear( newFeedbackGain ) );
			}
			
			
		//********************************************************************************
		//******	Delay Gain Accessor Methods
			
			
			/// Return the linear delay gain of this delay filter.
			/**
			  * This value represents the gain applied to the delayed
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE Gain getDelayGain() const
			{
				return targetDelayGain;
			}
			
			
			/// Return the delay gain of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the delayed
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE Gain getDelayGainDB() const
			{
				return math::linearToDB( targetDelayGain );
			}
			
			
			/// Set the linear delay gain of this delay filter.
			/**
			  * This value represents the gain applied to the delayed
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE void setDelayGain( Gain newDelayGain )
			{
				lockMutex();
				targetDelayGain = newDelayGain;
				unlockMutex();
			}
			
			
			/// Set the delay gain of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the delayed
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE void setDelayGainDB( Gain newDelayGain )
			{
				lockMutex();
				targetDelayGain = math::dbToLinear( newDelayGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Input Gain Accessor Methods
			
			
			/// Return the linear dry gain of this delay filter.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with delayed signal.
			  */
			OM_INLINE Gain getDryGain() const
			{
				return targetDryGain;
			}
			
			
			/// Return the dry gain of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with delayed signal.
			  */
			OM_INLINE Gain getDryGainDB() const
			{
				return math::linearToDB( targetDryGain );
			}
			
			
			/// Set the linear dry gain of this delay filter.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with delayed signal.
			  */
			OM_INLINE void setDryGain( Gain newDryGain )
			{
				lockMutex();
				targetDryGain = newDryGain;
				unlockMutex();
			}
			
			
			/// Set the dry gain of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with delayed signal.
			  */
			OM_INLINE void setDryGainDB( Gain newDryGain )
			{
				lockMutex();
				targetDryGain = math::dbToLinear( newDryGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Delay Frozen Accessor Methods
			
			
			/// Return whether or not the delay buffer's contents for this delay filter are frozen.
			/**
			  * If TRUE, this means that writes to the delay buffer disabled, so it will repeat the
			  * same delay buffer over and over.
			  */
			OM_INLINE Bool getDelayIsFrozen() const
			{
				return delayFrozen;
			}
			
			
			/// Set whether or not the delay buffer's contents for this delay filter are frozen.
			/**
			  * If TRUE, this means that writes to the delay buffer disabled, so it will repeat the
			  * same delay buffer over and over.
			  */
			OM_INLINE void setDelayIsFrozen( Bool newDelayIsFrozen )
			{
				lockMutex();
				delayFrozen = newDelayIsFrozen;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	High Pass Filter Attribute Accessor Methods
			
			
			/// Return whether or not this delay filter's high pass filter is enabled.
			OM_INLINE Bool getHighPassIsEnabled() const
			{
				return highPassEnabled;
			}
			
			
			/// Set whether or not this delay filter's high pass filter is enabled.
			OM_INLINE void setHighPassIsEnabled( Bool newHighPassIsEnabled )
			{
				lockMutex();
				highPassEnabled = newHighPassIsEnabled;
				unlockMutex();
			}
			
			
			/// Return the high pass filter frequency of this delay filter.
			OM_INLINE Float getHighPassFrequency() const
			{
				return highPassFrequency;
			}
			
			
			/// Set the high pass filter frequency of this delay filter.
			/**
			  * The new high pass frequency is clamped to the range [0,infinity].
			  */
			OM_INLINE void setHighPassFrequency( Float newHighPassFrequency )
			{
				lockMutex();
				highPassFrequency = math::max( newHighPassFrequency, Float(0) );
				unlockMutex();
			}
			
			
			/// Return the high pass filter order of this delay filter.
			OM_INLINE Size getHighPassOrder() const
			{
				return highPassOrder;
			}
			
			
			/// Set the high pass filter order of this delay filter.
			/**
			  * The new high pass order is clamped to the range [1,100].
			  */
			OM_INLINE void setHighPassOrder( Size newHighPassOrder )
			{
				lockMutex();
				highPassOrder = math::clamp( newHighPassOrder, Size(1), Size(100) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Low Pass Filter Attribute Accessor Methods
			
			
			/// Return whether or not this delay filter's low pass filter is enabled.
			OM_INLINE Bool getLowPassIsEnabled() const
			{
				return lowPassEnabled;
			}
			
			
			/// Set whether or not this delay filter's low pass filter is enabled.
			OM_INLINE void setLowPassIsEnabled( Bool newLowPassIsEnabled )
			{
				lockMutex();
				lowPassEnabled = newLowPassIsEnabled;
				unlockMutex();
			}
			
			
			/// Return the low pass filter frequency of this delay filter.
			OM_INLINE Float getLowPassFrequency() const
			{
				return lowPassFrequency;
			}
			
			
			/// Set the low pass filter frequency of this delay filter.
			/**
			  * The new low pass frequency is clamped to the range [0,infinity].
			  */
			OM_INLINE void setLowPassFrequency( Float newLowPassFrequency )
			{
				lockMutex();
				lowPassFrequency = math::max( newLowPassFrequency, Float(0) );
				unlockMutex();
			}
			
			
			/// Return the low pass filter order of this delay filter.
			OM_INLINE Size getLowPassOrder() const
			{
				return lowPassOrder;
			}
			
			
			/// Set the low pass filter order of this delay filter.
			/**
			  * The new low pass order is clamped to the range [1,100].
			  */
			OM_INLINE void setLowPassOrder( Size newLowPassOrder )
			{
				lockMutex();
				lowPassOrder = math::clamp( newLowPassOrder, Size(1), Size(100) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this delay filter.
			/**
			  * The method returns the string "Delay".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this delay filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this delay filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DELAY.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this delay filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the delay filter parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
			/// Get any special name associated with the specified value of an indexed parameter.
			virtual Bool getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this delay filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this delay filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this delay filter.
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
			
			
			/// Apply this delay filter to the specified input frame samples and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply an all-pass delay filter to the given buffers when some parameter has changed.
			template < DelayType delayType, Bool frozen >
			OM_FORCE_INLINE static void processDelayChanges( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
															SoundBuffer& delayBuffer, Size delayBufferSize, Index currentDelayWriteIndex,
															Gain& feedbackGain, Gain feedbackGainChangePerSample,
															Gain& delayGain, Gain delayGainChangePerSample );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A buffer which holds the delayed input samples which are used to create delay filtering.
			SoundBuffer delayBuffer;
			
			
			/// An enum representing the type of delay effect that this delay filter produces.
			DelayType type;
			
			
			/// The total number of samples in the delay buffer that are valid delay samples.
			/**
			  * This value is stored separately from the delay buffer so that the buffer can
			  * have a size that is greater than or equal to the actual number of delay samples.
			  */
			Size delayBufferSize;
			
			
			/// The current write position within the delay buffer in samples.
			Index currentDelayWriteIndex;
			
			
			/// The time in seconds of the delay of this delay filter.
			Float delayTime;
			
			
			/// The target delay time for this delay filter.
			/**
			  * This is the desired delay time which was set by the user.
			  * Since instant parameter changes can be audible, this value allows the filter
			  * to slowly approach the target delay time if it changes.
			  */
			Float targetDelayTime;
			
			
			/// The feedback gain of the delay filter - how much of each output delay sample is sent back to the delay buffer.
			Gain feedbackGain;
			
			
			/// The target feedback gain for this delay filter.
			/**
			  * This is the desired value for the feedback gain which was set by the user.
			  * Since instant parameter changes can be audible, this value allows the filter
			  * to slowly approach the target feedback gain if it changes.
			  */
			Gain targetFeedbackGain;
			
			
			/// The gain applied to the delayed signal before it is mixed with input signal.
			Gain delayGain;
			
			
			/// The target delay gain for this delay filter.
			/**
			  * This is the desired value for the delay gain which was set by the user.
			  * Since instant parameter changes can be audible, this value allows the filter
			  * to slowly approach the target delay gain if it changes.
			  */
			Gain targetDelayGain;
			
			
			/// The gain applied to the input signal before it is mixed with the delayed signal.
			Gain dryGain;
			
			
			/// The target dry gain for this delay filter.
			/**
			  * This is the desired value for the dry gain which was set by the user.
			  * Since instant parameter changes can be audible, this value allows the filter
			  * to slowly approach the target dry gain if it changes.
			  */
			Gain targetDryGain;
			
			
			/// A high-pass filter used to filter the wet signal of the delay.
			CutoffFilter* highPass;
			
			
			/// The frequency at which the high pass filter for the delay is at -3dB.
			Float highPassFrequency;
			
			
			/// The order of the delay's high pass filter that determines its slope.
			Size highPassOrder;
			
			
			/// A low-pass filter used to filter the wet signal of the delay.
			CutoffFilter* lowPass;
			
			
			/// The frequency at which the low pass filter for the delay is at -3dB.
			Float lowPassFrequency;
			
			
			/// The order of the delay's low pass filter that determines its slope.
			Size lowPassOrder;
			
			
			/// A boolean value indicating whether or not this delay's low-pass filter is enabled.
			Bool lowPassEnabled;
			
			
			/// A boolean value indicating whether or not this delay's high-pass filter is enabled.
			Bool highPassEnabled;
			
			
			/// A boolean value indicating whether or not this delay's buffer is frozen.
			Bool delayFrozen;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_DELAY_H
