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

#ifndef INCLUDE_OM_SOUND_MULTICHANNEL_DELAY_H
#define INCLUDE_OM_SOUND_MULTICHANNEL_DELAY_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that produces delay effects which can be independently changed per channel.
/**
  * This class represents a generic delay-style effect. It can be switched between
  * comb filtering and all-pass delay. This class can also be used to implement a
  * simple delay filter with basic delay time, feedback, and mix controls. It may
  * also be used as a building block for more complex effects like a Schroeder reverberator.
  *
  * Having explicit control of the delay times and feedback gains for the different
  * channels allows different echo patterns for each channel, increasing stereo imaging.
  * It is often useful to use a set of delay filters with different delay times for each channel
  * to approximate a reverb tail.
  */
class MultichannelDelay : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Delay Type Enum Declaration
			
			
			/// An enum type which describes the various types of delay effects that a Delay can produce.
			enum DelayType
			{
				/// The delay filter behaves as a delay filter (the same as a standard delay effect).
				COMB = 0,
				
				/// The delay filter behaves as an all-pass filter.
				ALL_PASS = 1
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a multichannel comb delay filter with 500ms delay time, 0 delay feedback, 0dB delay gain, and 0dB dry gain.
			MultichannelDelay();
			
			
			/// Create a multichannel comb delay filter with the specified delay parameters.
			/**
			  * This constructor creates a filter with the specified delay time, delay
			  * feedback gain, delay output gain, and input-to-output gain.
			  */
			MultichannelDelay( Float newDelayTime, Gain newFeedbackGain, Gain newDelayGain, Gain newDryGain );
			
			
			/// Create a multichannel delay filter with the specified type and delay parameters.
			/**
			  * This constructor creates a filter with the specified delay time, delay
			  * feedback gain, delay output gain, and input-to-output gain.
			  */
			MultichannelDelay( DelayType newType, Float newDelayTime, Gain newFeedbackGain,
									Gain newDelayGain, Gain newDryGain );
			
			
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
			
			
			/// Return the delay time for the specified delay filter channel in seconds.
			OM_INLINE Float getDelayTime( Index channelIndex ) const
			{
				if ( channelIndex < channels.getSize() )
					return channels[channelIndex].delayTime;
				else
					return globalChannel.delayTime;
			}
			
			
			/// Set the delay time for the specified delay filter channel in seconds.
			void setDelayTime( Index channelIndex, Float newDelayTime );
			
			
			/// Set the delay time for all of this delay filter's channels in seconds.
			void setDelayTime( Float newDelayTime );
			
			
		//********************************************************************************
		//******	Decay Time Accessor Methods
			
			
			/// Return the time it takes for the output of the specified delay filter channel to decay to -60dB.
			/**
			  * This method computes the decay time of the multichannel delay filter using the current values
			  * for the feedback gain and delay time of the multichannel delay filter.
			  */
			OM_INLINE Float getDecayTime( Index channelIndex ) const
			{
				if ( channelIndex < channels.getSize() )
					return channels[channelIndex].delayTime*math::log( Float(0.001), channels[channelIndex].feedbackGain );
				else
					return globalChannel.delayTime*math::log( Float(0.001), globalChannel.feedbackGain );
			}
			
			
			/// Set the time it takes for the output of the specified delay filter channel to decay to -60dB.
			/**
			  * This method uses the current multichannel delay filter delay time to compute the feedback
			  * gain necessary to produce the desired decay time.
			  */
			void setDecayTime( Index channelIndex, Float newDecayTime );
			
			
			/// Set the time it takes for the output of the all delay filter channels to decay to -60dB.
			/**
			  * This method uses the current multichannel delay filter delay time to compute the feedback
			  * gain necessary to produce the desired decay time.
			  */
			void setDecayTime( Float newDecayTime );
			
			
		//********************************************************************************
		//******	Feedback Gain Accessor Methods
			
			
			/// Return the feedback gain for the specified channel of this delay filter.
			/**
			  * This value represents how much of each output delay sample is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value should be between -0.99999 and 0.99999 in order to ensure
			  * filter stability.
			  */
			OM_INLINE Gain getFeedbackGain( Index channelIndex ) const
			{
				if ( channelIndex < channels.getSize() )
					return channels[channelIndex].feedbackGain;
				else
					return globalChannel.feedbackGain;
			}
			
			
			/// Return the feedback gain for the specified channel of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the output delay sample that is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value should be between -infinity and -0.00001 in order to ensure
			  * filter stability.
			  */
			OM_INLINE Gain getFeedbackGainDB( Index channelIndex ) const
			{
				return math::linearToDB( this->getFeedbackGain( channelIndex ) );
			}
			
			
			/// Set the feedback gain for the specified channel of this delay filter.
			/**
			  * This value represents how much of each output delay sample is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value is clamped to be between -0.99999 and 0.99999 in order to ensure
			  * filter stability.
			  */
			void setFeedbackGain( Index channelIndex, Gain newFeedbackGain );
			
			
			/// Set the feedback gain for the specified channel of this delay filter in decibels.
			/**
			  * This value represents the gain applied to the output delay sample that is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value should be between -infinity and -0.00001 in order to ensure
			  * filter stability.
			  */
			OM_INLINE void setFeedbackGainDB( Index channelIndex, Gain newFeedbackGain )
			{
				this->setFeedbackGain( channelIndex, math::dbToLinear( newFeedbackGain ) );
			}
			
			
			/// Set the feedback gain for all channels of this delay filter.
			/**
			  * This value represents how much of each output delay sample is sent
			  * back to the delay buffer during each pass over the delay buffer.
			  * This value is clamped to be between -0.99999 and 0.99999 in order to ensure
			  * filter stability.
			  */
			void setFeedbackGain( Gain newFeedbackGain );
			
			
			/// Set the feedback gain for all channels of this delay filter.
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
		//******	Channel Phase Accessor Methods
			
			
			/// Return the delay phase offset of the channel with the specified index.
			/**
			  * This value, specified in degrees, indicates how much the phase of the channel
			  * should be shifted by. This parameter allows the creation of ping-pong
			  * delay effects. For example, if the phase of the left channel is 0 and the phase
			  * of the right channel is 180, the channels' delay will be 50% out-of-phase, creating
			  * the classic ping-pong style delay.
			  */
			inline Float getChannelPhase( Index channelIndex ) const
			{
				if ( channelIndex < channels.getSize() )
					return Float(180)*channels[channelIndex].phase;
				else
					return Float(180)*globalChannel.phase;
			}
			
			
			/// Set the delay phase offset of the channel with the specified index.
			/**
			  * This value, specified in degrees, indicates how much the phase of the channel
			  * should be shifted by. This parameter allows the creation of ping-pong
			  * delay effects. For example, if the phase of the left channel is 0 and the phase
			  * of the right channel is 180, the channels' delay will be 50% out-of-phase, creating
			  * the classic ping-pong style delay.
			  *
			  * The input phase value is clamped so that the new phase value lies between -180 and 180 degrees.
			  */
			void setChannelPhase( Index channelIndex, Float newPhase );
			
			
			/// Set the delay phase offset for all channels.
			/**
			  * Doing this brings all channels into phase with each other (regardless of what phase that is).
			  *
			  * The input phase value is clamped so that the new phase value lies between -180 and 180 degrees.
			  */
			void setChannelPhase( Float newPhase );
			
			
		//********************************************************************************
		//******	Delay Gain Accessor Methods
			
			
			/// Return the linear delay gain of this multichannel delay filter.
			/**
			  * This value represents the gain applied to the delayed
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE Gain getDelayGain() const
			{
				return delayGain;
			}
			
			
			/// Return the delay gain of this multichannel delay filter in decibels.
			/**
			  * This value represents the gain applied to the delayed
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE Gain getDelayGainDB() const
			{
				return math::linearToDB( delayGain );
			}
			
			
			/// Set the linear delay gain of this multichannel delay filter.
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
			
			
			/// Set the delay gain of this multichannel delay filter in decibels.
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
			
			
			/// Return the linear dry gain of this multichannel delay filter.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with delayed signal.
			  */
			OM_INLINE Gain getDryGain() const
			{
				return dryGain;
			}
			
			
			/// Return the dry gain of this multichannel delay filter in decibels.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with delayed signal.
			  */
			OM_INLINE Gain getDryGainDB() const
			{
				return math::linearToDB( dryGain );
			}
			
			
			/// Set the linear dry gain of this multichannel delay filter.
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
			
			
			/// Set the dry gain of this multichannel delay filter in decibels.
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
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this multichannel delay filter.
			/**
			  * The method returns the string "Mutichannel Comb Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this multichannel delay filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this multichannel delay filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DELAY.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this multichannel delay filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the multichannel delay filter parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
			/// Get any special name associated with the specified value of an indexed parameter.
			virtual Bool getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this multichannel delay filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this multichanel delay filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this multichannel delay filter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Comb Filter Channel Data Class Declaration
			
			
			/// A class that holds channel-dependent delay filter data for a single channel.
			class Channel
			{
				public:
					
					
					/// Create a default channel with the default delay filter parameters.
					inline Channel()
						:	delayBufferSize( 0 ),
							currentDelayWriteIndex( 0 ),
							delayTime( 0 ),
							targetDelayTime( 0.5f ),
							feedbackGain( 0 ),
							targetFeedbackGain( 0 ),
							phase( 0 )
					{
					}
					
					
					/// Create a new channel with the specified delay time and feedback gain.
					inline Channel( Float newDelayTime, Gain newFeedbackGain )
						:	delayBufferSize( 0 ),
							currentDelayWriteIndex( 0 ),
							delayTime( 0 ),
							targetDelayTime( math::max( newDelayTime, Float(0) ) ),
							feedbackGain( math::clamp( newFeedbackGain, Gain(-0.999), Gain(0.999) ) ),
							phase( 0 )
					{
						targetFeedbackGain = newFeedbackGain;
					}
					
					
					
					/// An array of delay samples for this delay filter channel.
					Array<Sample32f> delayBuffer;
					
					
					/// The total number of samples in the delay buffer that are valid delay samples.
					/**
					  * This value is stored separately from the delay buffer so that the buffer can
					  * have a size that is greater than or equal to the actual number of delay samples.
					  */
					Size delayBufferSize;
					
					
					/// The current write position within the delay buffer in samples.
					Index currentDelayWriteIndex;
					
					
					/// The time in seconds of the delay of this channel of the delay filter.
					Float delayTime;
					
					
					/// The target delay time for this channel of the delay filter.
					/**
					  * This is the desired delay time which was set by the user.
					  * Since instant parameter changes can be audible, this value allows the filter
					  * to slowly approach the target delay time if it changes.
					  */
					Float targetDelayTime;
					
					
					/// The feedback gain of the delay filter channel.
					/**
					  * This indicates How much of each output delay sample is sent back to the delay buffer.
					  */
					Gain feedbackGain;
					
					
					/// The target feedback gain for this channel of the delay filter.
					/**
					  * This is the desired value for the feedback gain which was set by the user.
					  * Since instant parameter changes can be audible, this value allows the filter
					  * to slowly approach the target feedback gain if it changes.
					  */
					Gain targetFeedbackGain;
					
					
					/// The phase offset of this channel's delay, in the range [-1,1], where 0 is in phase and 1 is 180 degrees out of phase.
					Float phase;
					
					
			};
			
			
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
			
			
			/// Apply this multichannel delay filter to the specified input frame samples and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply a comb filter to the given buffers when no parameters have changed.
			OM_FORCE_INLINE static void processCombFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
															Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
															const Sample32f* delayRead, Sample32f* delayWrite,
															Gain feedbackGain, Gain delayGain, Gain dryGain );
			
			
			/// Apply a comb filter to the given buffers when some parameter has changed.
			OM_FORCE_INLINE static void processCombFilterChanges( const Sample32f* input, Sample32f* output, Size numSamples,
														Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
														const Sample32f* delayRead, Sample32f* delayWrite,
														Gain feedbackGain, Gain feedbackGainChangePerSample,
														Gain delayGain, Gain delayGainChangePerSample,
														Gain dryGain, Gain dryGainChangePerSample );
			
			
			/// Apply an all-pass filter to the given buffers when no parameters have changed.
			OM_FORCE_INLINE static void processAllPassFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
															Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
															const Sample32f* delayRead, Sample32f* delayWrite,
															Gain feedbackGain, Gain delayGain, Gain dryGain );
			
			
			/// Apply an all-pass delay filter to the given buffers when some parameter has changed.
			OM_FORCE_INLINE static void processAllPassFilterChanges( const Sample32f* input, Sample32f* output, Size numSamples,
															Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
															const Sample32f* delayRead, Sample32f* delayWrite,
															Gain feedbackGain, Gain feedbackGainChangePerSample,
															Gain delayGain, Gain delayGainChangePerSample,
															Gain dryGain, Gain dryGainChangePerSample );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of data for each channel of this delay filter.
			Array<Channel> channels;
			
			
			/// An enum representing the type of delay effect that this delay filter produces.
			DelayType type;
			
			
			/// A structure to use for all channels that haven't yet had their attributes set.
			/**
			  * Since the filter needs to be able to handle any number of filters, it must
			  * provide default parameters to use for channels that haven't been initialized.
			  * When an uninitialized channel is needed, its attributes are initialzed using the
			  * values in the global channel.
			  */
			Channel globalChannel;
			
			
			/// The gain applied to the delayed signal before it is mixed with input signal.
			Gain delayGain;
			
			
			/// The target delay gain for this multichannel delay filter.
			/**
			  * This is the desired value for the delay gain which was set by the user.
			  * Since instant parameter changes can be audible, this value allows the filter
			  * to slowly approach the target delay gain if it changes.
			  */
			Gain targetDelayGain;
			
			
			/// The gain applied to the input signal before it is mixed with the delayed signal.
			Gain dryGain;
			
			
			/// The target dry gain for this multichannel delay filter.
			/**
			  * This is the desired value for the dry gain which was set by the user.
			  * Since instant parameter changes can be audible, this value allows the filter
			  * to slowly approach the target dry gain if it changes.
			  */
			Gain targetDryGain;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MULTICHANNEL_DELAY_H
