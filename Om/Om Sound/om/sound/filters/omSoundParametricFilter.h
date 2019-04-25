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

#ifndef INCLUDE_OM_SOUND_PARAMETRIC_FILTER_H
#define INCLUDE_OM_SOUND_PARAMETRIC_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements a parametric peaking/notching EQ filter.
class ParametricFilter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default parametric filter at 1000Hz with 0dB gain.
			ParametricFilter();
			
			
			/// Create a parametric filter with the specified center frequency, q factor, and linear gain.
			/**
			  * The center frequency and gain are clamped to the range of [0,+infinity].
			  */
			ParametricFilter( Float newCenterFrequency, Float newQ, Gain newGain );
			
			
		//********************************************************************************
		//******	Corner Frequency Accessor Methods
			
			
			/// Return the center frequency of this parametric filter.
			/**
			  * This is the frequency most affected by the filter.
			  */
			OM_INLINE Float getFrequency() const
			{
				return centerFrequency;
			}
			
			
			/// Set the center frequency of this parametric filter.
			/**
			  * This is the frequency most affected by the filter.
			  * The new corner frequency is clamped to be in the range [0,+infinity].
			  */
			OM_INLINE void setFrequency( Float newCenterFrequency )
			{
				lockMutex();
				centerFrequency = math::max( newCenterFrequency, Float(0) );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Bandwith Accessor Methods
			
			
			/// Return the Q factor of this parametric filter.
			/**
			  * This value controls the width of the boost or cut that the filter produces.
			  * A smaller Q indicates a wider filter, while a larger Q indicates a narrower filter.
			  */
			OM_INLINE Float getQ() const
			{
				return q;
			}
			
			
			/// Set the Q factor of this parametric filter.
			/**
			  * This value controls the width of the boost or cut that the filter produces.
			  * A smaller Q indicates a wider filter, while a larger Q indicates a narrower filter.
			  *
			  * The new Q value is clamped to the range [0, +infinity].
			  */
			OM_INLINE void setQ( Float newQ )
			{
				lockMutex();
				q = math::max( newQ, Float(0) );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
			/// Return the octave bandwidth of this parametric filter.
			/**
			  * This value controls the width of the boost or cut that the filter produces.
			  * A larger bandwidth indicates a wider filter, while a smaller bandwidth
			  * indicates a narrower filter.
			  */
			OM_INLINE Float getBandwidth() const
			{
				return math::log2( (2*q*q + 1)/(2*q*q) + math::sqrt( math::square(((2*q*q + 1)/(q*q)))/4 - 1) );
			}
			
			
			/// Set the octave bandwidth of this parametric filter.
			/**
			  * This value controls the width of the boost or cut that the filter produces.
			  * A larger bandwidth indicates a wider filter, while a smaller bandwidth
			  * indicates a narrower filter.
			  */
			OM_INLINE void setBandwidth( Float newBandwidth )
			{
				lockMutex();
				Float twoToTheB = math::pow( Float(2), math::max( newBandwidth, Float(0) ) );
				q = math::sqrt(twoToTheB) / (twoToTheB - 1);
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Gain Accessor Methods
			
			
			/// Return the linear gain of this parametric filter.
			OM_INLINE Gain getGain() const
			{
				return gain;
			}
			
			
			/// Return the gain in decibels of this parametric filter.
			OM_INLINE Gain getGainDB() const
			{
				return math::linearToDB( gain );
			}
			
			
			/// Set the linear gain of this parametric filter.
			OM_INLINE void setGain( Gain newGain )
			{
				lockMutex();
				gain = math::max( newGain, Float(0) );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
			/// Set the gain in decibels of this parametric filter.
			OM_INLINE void setGainDB( Gain newGain )
			{
				lockMutex();
				gain = math::dbToLinear( newGain );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this parametric filter.
			/**
			  * The method returns the string "Parametric Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this parametric filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this parametric filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::EQUALIZER.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this parametric filter can process audio data in-place.
			/**
			  * This method always returns TRUE, parametric filters can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this parametric filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this parametric filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this parametric filter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Private Filter Channel Class Declaration
			
			
			/// A class that contains a history of the last 2 input and output samples for a 2nd order filter.
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
			
			
			/// Apply this parametric filter to the samples in the input frame and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply a second order filter to the specified sample arrays.
			OM_FORCE_INLINE static void process2ndOrderFilter( const Sample32f* input, Sample32f* output,
												Size numSamples, const Float* a, const Float* b,
												Sample32f* inputHistory, Sample32f* outputHistory );
			
			
		//********************************************************************************
		//******	Private Filter Coefficient Calculation Methods
			
			
			/// Recalculate the filter coefficients for the current filter frequency, gain, Q, and sample rate.
			void recalculateCoefficients();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The frequency in hertz of the center frequency of the parametric filter.
			/**
			  * This is the frequency most affected by the filter.
			  */
			Float centerFrequency;
			
			
			/// The linear gain of the parametric filter.
			Gain gain;
			
			
			/// The 'q' factor for the parametric filter.
			/**
			  * This value controls the width of the boost or cut that the filter produces.
			  * A smaller Q indicates a wider filter, while a larger Q indicates a narrower filter.
			  */
			Float q;
			
			
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


#endif // INCLUDE_OM_SOUND_PARAMETRIC_FILTER_H
