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

#ifndef INCLUDE_OM_SOUND_SHELF_FILTER_H
#define INCLUDE_OM_SOUND_SHELF_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements high-shelf and low-shelf EQ filters.
class ShelfFilter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Filter Direction Enum Declaration
			
			
			/// An enum type that specifies if a filter is a high shelf or low shelf.
			enum Direction
			{
				/// A type of filter that changes the response of all frequencies below the filter frequency.
				LOW_SHELF = 0,
				
				/// A type of filter that changes the response of all frequencies above the filter frequency.
				HIGH_SHELF = 1,
				
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default low shelf filter with corner frequency at 100 Hz and 0dB gain.
			ShelfFilter();
			
			
			/// Create a shelf filter with the specified direction, corner frequency, and gain.
			/**
			  * The corner frequency is clamped to the range of [0,+infinity].
			  */
			ShelfFilter( Direction newFilterDirection, Float newCornerFrequency, Gain newGain );
			
			
		//********************************************************************************
		//******	Filter Direction Accessor Methods
			
			
			/// Return the direction of the filter that is being used.
			/**
			  * This value determines whether the filter behaves as a high-shelf
			  * or low-shelf filter.
			  */
			OM_INLINE Direction getDirection() const
			{
				return filterDirection;
			}
			
			
			/// Set the type of filter that is being used.
			/**
			  * This value determines whether the filter behaves as a high-shelf
			  * or low-shelf filter.
			  */
			OM_INLINE void setDirection( Direction newFilterDirection )
			{
				lockMutex();
				filterDirection = newFilterDirection;
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Corner Frequency Accessor Methods
			
			
			/// Return the corner frequency of this shelving filter.
			/**
			  * This is the frequency at which the frequency begins to be cut off by the
			  * filter. This is usually the point at which the filter is -3dB down.
			  */
			OM_INLINE Float getFrequency() const
			{
				return cornerFrequency;
			}
			
			
			/// Set the corner frequency of this shelving filter.
			/**
			  * This is the frequency at which the frequency begins to be cut off by the
			  * filter. This is usually the point at which the filter is -3dB down.
			  *
			  * The new corner frequency is clamped to be in the range [0,+infinity].
			  */
			OM_INLINE void setFrequency( Float newCornerFrequency )
			{
				lockMutex();
				cornerFrequency = math::max( newCornerFrequency, Float(0) );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Gain Accessor Methods
			
			
			/// Return the linear gain of this shelving filter.
			OM_INLINE Gain getGain() const
			{
				return gain;
			}
			
			
			/// Return the gain in decibels of this shelving filter.
			OM_INLINE Gain getGainDB() const
			{
				return math::linearToDB( gain );
			}
			
			
			/// Set the linear gain of this shelving filter.
			OM_INLINE void setGain( Gain newGain )
			{
				lockMutex();
				gain = math::max( newGain, Float(0) );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
			/// Set the gain in decibels of this shelving filter.
			OM_INLINE void setGainDB( Gain newGain )
			{
				lockMutex();
				gain = math::dbToLinear( newGain );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Slope Accessor Methods
			
			
			/// Return the slope of this shelf filter.
			/**
			  * This value controls the slope of the transition from the passband to the stopband.
			  * The default slope of 1 indicates that the change is as fast as possible without filter
			  * overshoot on either side of the transition. A value greater than 1 causes filter overshoot
			  * but a faster transition, while a value less than 1 causes a slower filter transition.
			  */
			OM_INLINE Float getSlope() const
			{
				return slope;
			}
			
			
			/// Set the slope of this shelf filter.
			/**
			  * This value controls the slope of the transition from the passband to the stopband.
			  * The default slope of 1 indicates that the change is as fast as possible without filter
			  * overshoot on either side of the transition. A value greater than 1 causes filter overshoot
			  * but a faster transition, while a value less than 1 causes a slower filter transition.
			  *
			  * The new slope value is clamped to the range [0, +infinity].
			  */
			OM_INLINE void setSlope( Float newSlope )
			{
				lockMutex();
				slope = math::max( newSlope, Float(0) );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this shelving filter.
			/**
			  * The method returns the string "High-Pass Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this shelving filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this shelving filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::EQUALIZER.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this band filter can process audio data in-place.
			/**
			  * This method always returns TRUE, band filters can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
			/// Get any special name associated with the specified value of an indexed parameter.
			virtual Bool getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this shelf filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this shelf filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this shelf filter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Private Filter Channel Class Declaration
			
			
			/// A class that contains a history of the last input and output sample for a 1st order filter.
			class ChannelHistory
			{
				public:
					
					OM_INLINE ChannelHistory()
						:	inputHistory( Sample32f(0) ),
							outputHistory( Sample32f(0) )
					{
					}
					
					
					/// An array of the last 2 input samples for a filter with order 2.
					StaticArray<Sample32f,2> inputHistory;
					
					
					/// An array of the last 2 output samples for a filter with order 2.
					StaticArray<Sample32f,2> outputHistory;
					
					
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
			
			
			/// Apply this shelving filter to the samples in the input frame and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply a second order filter to the specified sample arrays.
			OM_FORCE_INLINE void process2ndOrderFilter( const Sample32f* input, Sample32f* output,
												Size numSamples, const Float* a, const Float* b,
												Sample32f* inputHistory, Sample32f* outputHistory );
			
			
		//********************************************************************************
		//******	Private Filter Coefficient Calculation Methods
			
			
			/// Recalculate the filter coefficients for the current filter direction, frequency, and sample rate.
			void recalculateCoefficients();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum representing the direction of this shelving filter.
			/**
			  * This value specifies whether the filter is a high self or low shelf filter.
			  */
			Direction filterDirection;
			
			
			/// The frequency in hertz of the corner frequency of the shelving filter.
			/**
			  * This is the frequency at which the frequency begins to be cut off by the
			  * filter. This is usually the point at which the filter is -3dB down, but 
			  * can be -6dB or other for some filter types.
			  */
			Float cornerFrequency;
			
			
			/// The slope of this shelf filter.
			/**
			  * This value controls the slope of the transition from the passband to the stopband.
			  * The default slope of 1 indicates that the change is as fast as possible without filter
			  * overshoot on either side of the transition. A value greater than 1 causes filter overshoot
			  * but a faster transition, while a value less than 1 causes a slower filter transition.
			  */
			Float slope;
			
			
			/// The linear gain of the shelf filter.
			Gain gain;
			
			
			/// The sample rate of the last sample buffer processed.
			/**
			  * This value is used to detect when the sample rate of the audio stream has changed,
			  * and thus recalculate filter coefficients.
			  */
			SampleRate sampleRate;
			
			
			/// The 'a' (numerator) coefficients of the z-domain transfer function.
			StaticArray<Float,3> a;
			
			
			/// The 'b' (denominator) coefficients of the z-domain transfer function.
			StaticArray<Float,2> b;
			
			
			/// An array of input and output history information for each channel of this filter.
			Array<ChannelHistory> channelHistory;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SHELF_FILTER_H
