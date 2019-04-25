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

#ifndef INCLUDE_OM_SOUND_CUTOFF_FILTER_H
#define INCLUDE_OM_SOUND_CUTOFF_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements high-pass and low-pass EQ filters of various types and filter orders.
class CutoffFilter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Cutoff Filter Type Enum Declaration
			
			
			/// An enum type that denotes a certain class of cutoff filter.
			enum Type
			{
				/// A type of cutoff filter that uses a Bufferworth design.
				/**
				  * A Butterworth filter is a type of filter that is designed to be as flat as 
				  * possible in the passband with no ripple in the stopband. The filter is -3dB at the corner
				  * frequency.
				  */
				BUTTERWORTH = 0,
				
				/// A type of cutoff filter that uses a Linkwitz-Riley design.
				/**
				  * A Linkwitz-Riley filter is a type of filter that is designed to be allpass when
				  * summed with a corresponding opposite filter at the crossover frequency.
				  * The filter is -6dB at the corner frequency.
				  *
				  * Linkwitz-Riley filters only support orders 2, 4, 6, and 8 because of their special properties.
				  * Attempting to use an invalid order will result in the next highest valid order being
				  * used.
				  */
				LINKWITZ_RILEY = 1,
				
				/// A type of cutoff filter that uses a Bessel design.
				/**
				  * A Bessel filter is a type of filter that is designed to have a maximally
				  * linear phase response. The filter is -3dB at the corner frequency.
				  */
				//BESSEL,
				
				/// A type of cutoff filter that uses a Chebyshev type I design.
				/**
				  * A Chebyshev type I filter is a filter that has a steeper rolloff but at the expense
				  * of ripple in the passband.
				  */
				CHEBYSHEV_I = 2,
				
				/// A type of cutoff filter that uses a Chebyshev type II design.
				/**
				  * A Chebyshev type II filter is a filter that has a steeper rolloff but at the expense
				  * of ripple in the stopband.
				  */
				//CHEBYSHEV_II
			};
			
			
		//********************************************************************************
		//******	Filter Direction Enum Declaration
			
			
			/// An enum type that specifies if a filter is high-pass or low-pass.
			enum Direction
			{
				/// A type of filter that filters out all frequencies below the cutoff frequency.
				HIGH_PASS = 0,
				
				/// A type of filter that filters out all frequencies above the cutoff frequency.
				LOW_PASS = 1
				
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default 1st order butterworth cutoff filter with corner frequency at 0 Hz.
			/**
			  * Since 0 Hz is not a valid corner frequency, this filter does not do anything to
			  * the input audio.
			  */
			CutoffFilter();
			
			
			/// Create a cutoff filter with the specified type, direction, order, and corner frequency.
			/**
			  * The filter order is clamped between 1 and the maximum allowed filter order,
			  * and the corner frequency is clamped to the range of [0,+infinity].
			  */
			CutoffFilter( Type newFilterType, Direction newFilterDirection,
						Size newFilterOrder, Float newCornerFrequency );
			
			
		//********************************************************************************
		//******	Filter Type Accessor Methods
			
			
			/// Return the type of filter that is being used.
			/**
			  * Since different types of filters have different characteristics in frequency
			  * and phase response, this value allows the user to pick the filter type best
			  * suited for their needs.
			  */
			OM_INLINE Type getType() const
			{
				return filterType;
			}
			
			
			/// Set the type of filter that is being used.
			/**
			  * Since different types of filters have different characteristics in frequency
			  * and phase response, this value allows the user to pick the filter type best
			  * suited for their needs.
			  */
			OM_INLINE void setType( Type newFilterType )
			{
				lockMutex();
				filterType = newFilterType;
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Direction Accessor Methods
			
			
			/// Return the direction of the filter that is being used.
			/**
			  * This value determines whether the filter behaves as a high-pass
			  * or low-pass filter.
			  */
			OM_INLINE Direction getDirection() const
			{
				return filterDirection;
			}
			
			
			/// Set the type of filter that is being used.
			/**
			  * This value determines whether the filter behaves as a high-pass
			  * or low-pass filter.
			  */
			OM_INLINE void setDirection( Direction newFilterDirection )
			{
				lockMutex();
				filterDirection = newFilterDirection;
				recalculateCoefficients();
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Order Accessor Methods
			
			
			/// Return the order of this cutoff filter.
			OM_INLINE Size getOrder() const
			{
				return filterOrder;
			}
			
			
			/// Set the order of this cutoff filter.
			/**
			  * If the specified order is not supported by this filter, the closest
			  * order to the desired order is used.
			  *
			  * The new filter order is clamped betwee 1 and the maximum allowed filter order.
			  */
			OM_INLINE void setOrder( Size newFilterOrder )
			{
				lockMutex();
				filterOrder = math::clamp( newFilterOrder, Size(1), MAXIMUM_FILTER_ORDER );
				recalculateCoefficients();
				unlockMutex();
			}
			
			
			/// Return the maximum filter order allowed.
			/**
			  * All created filters will have an order less than or equal to this value
			  * and it is impossible to set the order of a filter to be greater than this
			  * value.
			  */
			OM_INLINE Size getMaximumOrder() const
			{
				return MAXIMUM_FILTER_ORDER;
			}
			
			
		//********************************************************************************
		//******	Corner Frequency Accessor Methods
			
			
			/// Return the corner frequency of this cutoff filter.
			/**
			  * This is the frequency at which the frequency begins to be cut off by the
			  * filter. This is usually the point at which the filter is -3dB down, but 
			  * can be -6dB or other for some filter types.
			  */
			OM_INLINE Float getFrequency() const
			{
				return cornerFrequency;
			}
			
			
			/// Set the corner frequency of this cutoff filter.
			/**
			  * This is the frequency at which the frequency begins to be cut off by the
			  * filter. This is usually the point at which the filter is -3dB down, but 
			  * can be -6dB or other for some filter types.
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
		//******	Filter Ripple Accessor Methods
			
			
			/// Return the ripple of this cutoff filter in dB.
			/**
			  * This parameter is only used by the Chebyshev type I and type II filters.
			  * It determines the amount of ripple in the passband (for type I) or in
			  * the stopband (for type II). A smaller ripple results in a slower
			  * rolloff in the frequency response for any given filter order.
			  *
			  * The ripple amount is initially equal to 1 dB and must be greater than 0.
			  */
			OM_INLINE Float getRipple() const
			{
				return ripple;
			}
			
			
			/// Set the ripple of this cutoff filter in dB.
			/**
			  * This parameter is only used by the Chebyshev type I and type II filters.
			  * It determines the amount of ripple in the passband (for type I) or in
			  * the stopband (for type II). A smaller ripple results in a slower
			  * rolloff in the frequency response for any given filter order.
			  *
			  * The ripple amount is initially equal to 1 dB and is clamped to be greater than 0.
			  */
			OM_INLINE void setRipple( Float newRipple )
			{
				lockMutex();
				
				ripple = math::max( newRipple, Float(0) );
				
				if ( filterType == CHEBYSHEV_I )
					recalculateCoefficients();
				
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this cutoff filter.
			/**
			  * The method returns the string "Cutoff Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this cutoff filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this cutoff filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::EQUALIZER.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this cutoff filter can process audio data in-place.
			/**
			  * This method always returns TRUE, cutoff filters can process audio data in-place.
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
			
			
			/// A string indicating the human-readable name of this cutoff filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this cutoff filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this cutoff filter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Maximum Filter Order Declaration
			
			
			/// Define the maximum allowed filter order for this cutoff filter class.
			static const Size MAXIMUM_FILTER_ORDER = 100;
			
			
		//********************************************************************************
		//******	Private Filter Channel Class Declaration
			
			
			/// A class that contains a history of the last 2 input and output samples for a second order filter.
			class ChannelHistory
			{
				public:
					
					OM_INLINE ChannelHistory()
					{
						this->reset();
					}
					
					OM_INLINE void reset()
					{
						inputHistory[1] = inputHistory[0] = Float(0);
						outputHistory[1] = outputHistory[0] = Float(0);
					}
					
					/// An array of the last 2 input samples for a filter with order 2.
					Float inputHistory[2];
					
					
					/// An array of the last 2 output samples for a filter with order 2.
					Float outputHistory[2];
					
					
			};
			
			
			/// A class that contains coefficients for a 2nd order IIR filter and channel history information.
			class SecondOrderFilter
			{
				public:
					
					OM_INLINE SecondOrderFilter()
					{
					}
					
					
					/// The 'a' (numerator) coefficients of the z-domain transfer function.
					Float a[3];
					
					
					/// The 'b' (denominator) coefficients of the z-domain transfer function.
					Float b[2];
					
					
					/// An array of input and output history information for each channel of this filter.
					Array<ChannelHistory> channelHistory;
					
					
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
			
			
			/// Apply this cutoff filter to the samples in the input frame and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply the current filter to the specified buffers of data using a generic method
			static void processFilterCascade( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
											Size filterOrder, SecondOrderFilter* filterSections );
			
			
			/// Apply the current linkwitz filter to the specified buffers of data using two butterworth filters in series.
			void processLinkwitzRiley( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples );
			
			
			/// Apply a first order filter to the specified sample arrays.
			OM_FORCE_INLINE static void process1stOrderFilter( const Sample32f* input, Sample32f* output,
												Size numSamples, const Float a[2], const Float b[1],
												Float inputHistory[1], Float outputHistory[1] );
			
			
			/// Apply a second order filter to the specified sample arrays.
			OM_FORCE_INLINE static void process2ndOrderFilter( const Sample32f* input, Sample32f* output,
												Size numSamples, const Float a[3], const Float b[2],
												Float inputHistory[2], Float outputHistory[2] );
			
			
		//********************************************************************************
		//******	Private Filter Coefficient Calculation Methods
			
			
			/// Recalculate the filter coefficients for the current filter type, order, and sample rate.
			void recalculateCoefficients();
			
			
			/// Get the coefficients for a Butterworth cutoff filter with the specified parameters.
			/**
			  * This method computes the coefficients for a Butterworth cutoff filter with the
			  * specified filter direction, order, corner frequency (-3dB point), and sample rate.
			  *
			  * The filter sections output array must not be NULL and must have space to hold
			  * ceiling(n/2) filters for a filter of order n.
			  */
			static void getButterworthCoefficients( Direction direction, Size order, Float cornerFrequency,
													SampleRate sampleRate, SecondOrderFilter* filterSections );
			
			
			/// Get the coefficients for a Chebyshev cutoff filter with the specified parameters.
			/**
			  * This method computes the coefficients for a Chebyshev cutoff filter with the
			  * specified filter direction, order, corner frequency (-3dB point), and sample rate.
			  *
			  * The filter sections output array must not be NULL and must have space to hold
			  * ceiling(n/2) filters for a filter of order n.
			  */
			static void getChebyshev1Coefficients( Direction direction, Size order, Float cornerFrequency,
												Float ripple, SampleRate sampleRate, SecondOrderFilter* filterSections );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum representing the type of cutoff filter that is being applied.
			/**
			  * Since different types of filters have different characteristics in frequency
			  * and phase response, this value allows the user to pick the filter type best
			  * suited for their needs.
			  */
			Type filterType;
			
			
			/// An enum representing the direction of this cutoff filter.
			/**
			  * This value specifies whether the filter is a high-pass or low-pass filter.
			  */
			Direction filterDirection;
			
			
			/// The order of the cutoff filter, from 1 up to MAXIMUM_FILTER_ORDER.
			/**
			  * Thie value affects the slope of the filter below the cutoff filter.
			  * A filter of order 1 means that the slope of the filter asymptotically reaches
			  * 6dB per octave. Each additional order of filer increases the slope of the filter
			  * by an additional 6dB per octave. Since some filter types (such as Linkwitz-Riley)
			  * do not support certain filter orders (odd numbered, for instance), those filters
			  * will use the next highest supported filter order as a fallback in those cases.
			  */
			Size filterOrder;
			
			
			/// The frequency in hertz of the corner frequency of the cutoff filter.
			/**
			  * This is the frequency at which the frequency begins to be cut off by the
			  * filter. This is usually the point at which the filter is -3dB down, but 
			  * can be -6dB or other for some filter types.
			  */
			Float cornerFrequency;
			
			
			/// The ripple (in dB) of the filter if it is a Chebyshev filter.
			Float ripple;
			
			
			/// The sample rate of the last sample buffer processed.
			/**
			  * This value is used to detect when the sample rate of the audio stream has changed,
			  * and thus recalculate filter coefficients.
			  */
			SampleRate sampleRate;
			
			
			/// An array of cascaded 2nd order filter sections for this cutoff filter.
			Array<SecondOrderFilter> filterSections;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CUTOFF_FILTER_H
