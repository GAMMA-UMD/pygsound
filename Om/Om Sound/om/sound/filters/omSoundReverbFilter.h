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

#ifndef INCLUDE_OM_SOUND_REVERB_FILTER_H
#define INCLUDE_OM_SOUND_REVERB_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"
#include "omSoundCutoffFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that provides a basic reverberation effect.
/**
  * The class uses a simple schroeder-type reverberator with frequency
  * band filtering.
  */
class ReverbFilter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new reverb filter with the default parameters.
			ReverbFilter();
			
			
			/// Create a new reverb filter with the default parameters and the specified decay time in seconds.
			ReverbFilter( Float newDecayTime );
			
			
		//********************************************************************************
		//******	Wet Gain Accessor Methods
			
			
			/// Return the current linear wet gain factor of this reverb filter.
			/**
			  * This value represents the gain applied to the reverb
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE Gain getWetGain() const
			{
				return targetWetGain;
			}
			
			
			/// Return the current wet gain factor in decibels of this reverb filter.
			/**
			  * This value represents the gain applied to the reverb
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE Gain getWetGainDB() const
			{
				return math::linearToDB( targetWetGain );
			}
			
			
			/// Set the target linear wet gain for this reverb filter.
			/**
			  * This value represents the gain applied to the reverb
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE void setWetGain( Gain newWetGain )
			{
				lockMutex();
				targetWetGain = newWetGain;
				unlockMutex();
			}
			
			
			/// Set the target wet gain in decibels for this reverb filter.
			/**
			  * This value represents the gain applied to the reverb
			  * signal before it is mixed with input signal.
			  */
			OM_INLINE void setWetGainDB( Gain newDBWetGain )
			{
				lockMutex();
				targetWetGain = math::dbToLinear( newDBWetGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Dry Gain Accessor Methods
			
			
			/// Return the current linear dry gain factor of this reverb filter.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with affected signal.
			  */
			OM_INLINE Gain getDryGain() const
			{
				return targetDryGain;
			}
			
			
			/// Return the current dry gain factor in decibels of this reverb filter.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with affected signal.
			  */
			OM_INLINE Gain getDryGainDB() const
			{
				return math::linearToDB( targetDryGain );
			}
			
			
			/// Set the target linear dry gain for this reverb filter.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with affected signal.
			  */
			OM_INLINE void setDryGain( Gain newDryGain )
			{
				lockMutex();
				targetDryGain = newDryGain;
				unlockMutex();
			}
			
			
			/// Set the target dry gain in decibels for this reverb filter.
			/**
			  * This value represents the gain applied to the input
			  * signal before it is mixed with affected signal.
			  */
			OM_INLINE void setDryGainDB( Gain newDBDryGain )
			{
				lockMutex();
				targetDryGain = math::dbToLinear( newDBDryGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Reverb Time Accessor Methods
			
			
			/// Return the decay time for this reverb filter.
			/**
			  * This is the time that it takes for an impulse's reverb tail
			  * to drop to -60 decibels below its original level, the RT60.
			  */
			OM_INLINE Float getDecayTime() const
			{
				return decayTime;
			}
			
			
			/// Set the decay time for this reverb filter.
			/**
			  * This is the time that it takes for an impulse's reverb tail
			  * to drop to -60 decibels below its original level, the RT60.
			  * 
			  * The new reverb time is clamped to the range [0,100].
			  */
			OM_INLINE void setDecayTime( Float newDecayTime )
			{
				lockMutex();
				decayTime = math::clamp( newDecayTime, Float(0), Float(100) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Reverb Density Accessor Methods
			
			
			/// Return the reverb density for this reverb filter.
			/**
			  * This is a value between 0 and 1 indicating how dense the
			  * reverb reflections are. A value of 1 indicates that the
			  * reflections are as dense as possible.
			  */
			OM_INLINE Float getDensity() const
			{
				return density;
			}
			
			
			/// Set the reverb density for this reverb filter.
			/**
			  * This is a value between 0 and 1 indicating how dense the
			  * reverb reflections are. A value of 1 indicates that the
			  * reflections are as dense as possible.
			  * 
			  * The new reverb density is clamped to the range [0,1].
			  */
			OM_INLINE void setDensity( Float newDensity )
			{
				lockMutex();
				density = math::clamp( newDensity, Float(0), Float(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	High Pass Filter Attribute Accessor Methods
			
			
			/// Return whether or not this reverb filter's high pass filter is enabled.
			OM_INLINE Bool getHighPassIsEnabled() const
			{
				return highPassEnabled;
			}
			
			
			/// Set whether or not this reverb filter's high pass filter is enabled.
			OM_INLINE void setHighPassIsEnabled( Bool newHighPassIsEnabled )
			{
				lockMutex();
				highPassEnabled = newHighPassIsEnabled;
				unlockMutex();
			}
			
			
			/// Return the high pass filter frequency of this reverb filter.
			OM_INLINE Float getHighPassFrequency() const
			{
				return highPassFrequency;
			}
			
			
			/// Set the high pass filter frequency of this reverb filter.
			/**
			  * The new high pass frequency is clamped to the range [0,infinity].
			  */
			OM_INLINE void setHighPassFrequency( Float newHighPassFrequency )
			{
				lockMutex();
				highPassFrequency = math::max( newHighPassFrequency, Float(0) );
				unlockMutex();
			}
			
			
			/// Return the high pass filter order of this reverb filter.
			OM_INLINE Size getHighPassOrder() const
			{
				return highPassOrder;
			}
			
			
			/// Set the high pass filter order of this reverb filter.
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
			
			
			/// Return whether or not this reverb filter's low pass filter is enabled.
			OM_INLINE Bool getLowPassIsEnabled() const
			{
				return lowPassEnabled;
			}
			
			
			/// Set whether or not this reverb filter's low pass filter is enabled.
			OM_INLINE void setLowPassIsEnabled( Bool newLowPassIsEnabled )
			{
				lockMutex();
				lowPassEnabled = newLowPassIsEnabled;
				unlockMutex();
			}
			
			
			/// Return the low pass filter frequency of this reverb filter.
			OM_INLINE Float getLowPassFrequency() const
			{
				return lowPassFrequency;
			}
			
			
			/// Set the low pass filter frequency of this reverb filter.
			/**
			  * The new low pass frequency is clamped to the range [0,infinity].
			  */
			OM_INLINE void setLowPassFrequency( Float newLowPassFrequency )
			{
				lockMutex();
				lowPassFrequency = math::max( newLowPassFrequency, Float(0) );
				unlockMutex();
			}
			
			
			/// Return the low pass filter order of this reverb filter.
			OM_INLINE Size getLowPassOrder() const
			{
				return lowPassOrder;
			}
			
			
			/// Set the low pass filter order of this reverb filter.
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
			
			
			/// Return a human-readable name for this reverb filter.
			/**
			  * The method returns the string "Reverb Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this reverb filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this reverb filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::REVERB.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this reverb filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the reverb filter parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this reverb filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this reverb filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this reverb filter.
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
			
			
			/// Apply a reverb function to the samples in the input frame and write the output to the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Process a comb filter where no parameter interpolation occurs, mixing to the output instead of replacing.
			void processCombFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
											Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
											Sample32f* delay, Gain feedbackGain );
			
			
			/// Process a comb filter where no parameter interpolation occurrs.
			void processAllPassFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
												Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
												Sample32f* delay, Gain feedbackGain );
			
			
			/// Compute and return the feedback gain necessary to produce the specified reverb time with the given delay time.
			OM_INLINE Float getFeedbackGainForRT60( Float delayTime, Float rt60 )
			{
				return math::pow( Float(0.001), delayTime / rt60 );
			}
			
			
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that encapsulates all information related to a single reverb filter channel.
			class DelayFilterChannel
			{
				public:
					
					inline DelayFilterChannel()
						:	currentDelayWriteIndex( 0 ),
							delayTime( 0 ),
							feedbackGain( 0 ),
							decayTime( 0 )
					{
					}
					
					
					/// An array of samples which represents the delay filter buffer for this channel.
					Array<Sample32f> delayBuffer;
					
					
					/// The current write position in samples within the delay buffer.
					Index currentDelayWriteIndex;
					
					
					/// The delay time for this delay filter channel.
					Float delayTime;
					
					
					/// The feedbaack gain for this delay filter channel.
					Float feedbackGain;
					
					
					/// The RT60 for this delay filter channel, stored here so that we can know when to update the feedback gain.
					Float decayTime;
					
					
			};
			
			
			/// A class that encapsulates information about a single all-pass or comb filter.
			class DelayFilter
			{
				public:
					
					OM_INLINE DelayFilter()
					{
					}
					
					
					/// An array of the channels that are part of this delay filter.
					Array<DelayFilterChannel> channels;
					
			};
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of comb filters which are applied in parallel and mixed together.
			Array<DelayFilter> combFilters;
			
			
			/// An array of all pass filters which are applied in series to the output of the comb filters.
			Array<DelayFilter> allPassFilters;
			
			
			/// The current output gain for the wet affected signal for this reverb filter.
			Gain wetGain;
			
			
			/// The target wet gain factor, used to smooth changes in the wet gain.
			Gain targetWetGain;
			
			
			/// The current output gain for the dry unaffected signal for this reverb filter.
			Gain dryGain;
			
			
			/// The target dry gain factor, used to smooth changes in the dry gain.
			Gain targetDryGain;
			
			
			/// The decay time for this reverb filter.
			/**
			  * This is the time that it takes for an impulse's reverb tail
			  * to drop to -60 decibels below its original level, the RT60.
			  */
			Float decayTime;
			
			
			/// A value between 0 and 1 indicating how dense the reverb reflections are.
			Float density;
			
			
			/// The frequency at which the high pass filter for the reverb is at -3dB.
			Float highPassFrequency;
			
			
			/// The order of the reverb's high pass filter that determines its slope.
			Size highPassOrder;
			
			
			/// A high-pass filter used to smooth the output of the reverb.
			CutoffFilter* highPass;
			
			
			/// The frequency at which the low pass filter for the reverb is at -3dB.
			Float lowPassFrequency;
			
			
			/// The order of the reverb's low pass filter that determines its slope.
			Size lowPassOrder;
			
			
			/// A low-pass filter used to smooth the output of the reverb.
			CutoffFilter* lowPass;
			
			
			/// A boolean value indicating whether or not this reverb's low-pass filter is enabled.
			Bool lowPassEnabled;
			
			
			/// A boolean value indicating whether or not this reverb's high-pass filter is enabled.
			Bool highPassEnabled;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The maximum allowed number of series all pass filters for this reverb filter.
			static const Size MAX_NUMBER_OF_ALL_PASS_FILTERS = 5;
			
			/// The maximum allowed number of parallel comb filters for this reverb filter.
			static const Size MAX_NUMBER_OF_COMB_FILTERS = 10;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_REVERB_FILTER_H
