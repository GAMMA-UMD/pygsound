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

#ifndef INCLUDE_OM_SOUND_CROSSOVER_H
#define INCLUDE_OM_SOUND_CROSSOVER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"
#include "omSoundCutoffFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that is used to filter input audio data into an arbitrary number of frequency band outputs.
/**
  * The Crossover class uses a series of Linkwitz-Riley order crossover filters to
  * split a stream of input audio into an arbitrary number of frequency bands whose
  * corner frequencies can be between 0Hz and the Nyquist Frequency for the current operating
  * sample rate.
  *
  * Each crossover filter (a high-pass, low-pass pair) can have any even order N.
  * Special care is taken to keep the outputs of the crossover in phase and all-pass
  * at all frequencies.
  *
  * The crossover keeps an internal list of the crossover filters, sorted by
  * frequency. Therefore, if you add filters to the crossover in arbitrary order, they
  * are automatically sorted, so don't expect the filters to be stored in the order
  * in which they were added.
  */
class Crossover : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default crossover with no split frequencies and one full-range frequency band output.
			Crossover();
			
			
			/// Create an exact copy of the specified crossover.
			Crossover( const Crossover& newCrossover );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a crossover and release all of its resources.
			~Crossover();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of the specified crossover object to this crossover.
			Crossover& operator = ( const Crossover& other );
			
			
		//********************************************************************************
		//******	Crossover Filter Accessor Methods
			
			
			/// Return the total number of crossover frequencies that this crossover has.
			/**
			  * This value is 0 for a crossover with no crossover points and is
			  * equal to (n-1) for a crossover with (n) frequency bands.
			  */
			OM_INLINE Size getFilterCount() const
			{
				return points.getSize();
			}
			
			
			/// Add a new crossover frequency to this Crossover, specifying the filter order.
			/**
			  * If the specified frequency is not within the valid range of 0 to the
			  * Nyquist Frequency, this method has no effect and FALSE is returned. Otherwise,
			  * the frequency is added to the Crossover and TRUE is returned. If the method 
			  * succeeds, the resulting Crossover will have one more output frequency band
			  * than it had before.
			  *
			  * This version of this method allows the user to specify the filter
			  * order used for the new crossover filter. Order values are clamped to the range [2,8].
			  * Since Linkwitz-Riley crossover filters are used, the actual filter order will be
			  * the next even number greater than or equal to the specified filter order.
			  * 
			  * @param newFrequency - the crossover frequency to add to this Crossover.
			  * @param newFilterOrder - the crossover filter order to use.
			  * @return whether or not the specified crossover frequency was added successfully.
			  */
			Bool addFilter( Float newFrequency, Size newFilterOrder = Size(4) );
			
			
			/// Remove the crossover point filter at the specified index in this Crossover.
			/**
			  * If the specified filter index is valid, the crossover filter at that index
			  * is removed and the number of output frequency bands for the crossover is reduced
			  * by one. Otherwise, the method has no effect.
			  */
			void removeFilter( Index filterIndex );
			
			
			/// Remove all crossover filters from this Crossover.
			/**
			  * This method resets the Crossover to its initial state with
			  * only one full-range output.
			  */
			void clearFilters();
			
			
		//********************************************************************************
		//******	Frequency Band Accessor Methods
			
			
			/// Return the total number of frequency bands that this crossover produces.
			/**
			  * This value is 1 for a crossover with no crossover points, with one additional
			  * band for each crossover split frequency.
			  */
			OM_INLINE Size getBandCount() const
			{
				return points.getSize() + 1;
			}
			
			
			/// Return a 1D range value indicating the range of frequencies for the specified frequency band index.
			OM_INLINE AABB1f getBandRange( Index bandIndex ) const
			{
				if ( bandIndex > points.getSize() )
					return AABB1f();
				else if ( bandIndex == 0 )
					return AABB1f( 0, points[bandIndex].frequency );
				else if ( bandIndex == points.getSize() )
					return AABB1f( points[bandIndex - 1].frequency, math::max<Float>() );
				else
					return AABB1f( points[bandIndex - 1].frequency, points[bandIndex].frequency );
			}
			
			
		//********************************************************************************
		//******	Crossover Filter Frequency Accessor Methods
			
			
			/// Return the frequency in hertz of the crossover filter at the specified index.
			OM_INLINE Float getFilterFrequency( Index filterIndex ) const
			{
				if ( filterIndex < points.getSize() )
					return points[filterIndex].frequency;
				else
					return Float(0);
			}
			
			
			/// Set the frequency in hertz of the crossover filter at the specified index.
			/**
			  * This method resorts the crossover points based on the specified frequency
			  * change so that they remain sorted.
			  *
			  * The method returns whether or not the filter frequency was able to be changed.
			  * It can fail if the given filter index or frequency is invalid.
			  */
			Bool setFilterFrequency( Index filterIndex, Float newFrequency );
			
			
		//********************************************************************************
		//******	Crossover Filter Order Accessor Methods
			
			
			/// Return the order of the crossover filter at the specified index.
			OM_INLINE Size getFilterOrder( Index filterIndex ) const
			{
				if ( filterIndex < points.getSize() )
					return points[filterIndex].order;
				else
					return 0;
			}
			
			
			/// Set the order of the crossover filter at the specified index.
			Bool setFilterOrder( Index filterIndex, Size newOrder );
			
			
		//********************************************************************************
		//******	Input Type Accessor Methods
			
			
			/// Return a boolean value indicating whether or not the crossover is given a multiband multi-input format.
			/**
			  * If this value is TRUE, the crossover will use the audio from each input for the corresponding output,
			  * rather than using only the first input. The number of inputs in this case will be the same as the number
			  * of outputs. If this value is FALSE, the first input is used to feed all frequency band outputs.
			  */
			OM_INLINE Bool getIsMultiInput() const
			{
				return multibandInput;
			}
			
			
			/// Return a boolean value indicating whether or not the crossover is given a multiband multi-input format.
			/**
			  * If this value is TRUE, the crossover will use the audio from each input for the corresponding output,
			  * rather than using only the first input. The number of inputs in this case will be the same as the number
			  * of outputs. If this value is FALSE, the first input is used to feed all frequency band outputs.
			  */
			Bool setIsMultiInput( Bool newIsMultiInput );
			
			
		//********************************************************************************
		//******	Input and Output Name Accessor Methods
			
			
			/// Return a human-readable name of the crossover filter output at the specified index.
			/**
			  * This method returns the string "Output N" where N is the index of the frequency
			  * band, starting at 0.
			  */
			virtual UTF8String getOutputName( Index outputIndex ) const;
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this cutoff filter.
			/**
			  * The method returns the string "Crossover".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this crossover filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this crossover filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::EQUALIZER.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this crossover filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this crossover filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this crossover filter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Crossover Filter Class Declaration
			
			
			/// A class that contains information related to a single crossover point.
			class CrossoverPoint
			{
				public:
					
					/// Create a new crossover point object with the specified split frequency and filter order.
					OM_INLINE CrossoverPoint( Float newFrequency, Size newOrder )
						:	frequency( newFrequency ),
							order( newOrder )
					{
					}
					
					
					/// The corner frequency of this crossover filter.
					Float frequency;
					
					
					/// The filter order of this crossover point, usually an even integer betwee 2 and 8.
					Size order;
					
					
			};
			
			
			/// A class that stores information for a single crossover output frequency band.
			class FrequencyBand
			{
				public:
					
					/// Create a new frequency band object with no crossover filters.
					OM_INLINE FrequencyBand()
					{
					}
					
					
					/// A list of filters that are applied to the input to produce this frequency band.
					ArrayList<CutoffFilter*> filters;
					
					
			};
			
			
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
			
			
			/// Apply this crossover to the samples in the input frame and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Return whether or not the specified CutoffFilter has an odd order and needs frequency band inversion.
			OM_INLINE static Bool filterNeedsInversion( const CutoffFilter& filter )
			{
				return filter.getDirection() == CutoffFilter::HIGH_PASS && 
						((math::nextMultiple( filter.getOrder(), Size(2) ) / 2) % 2) == 1;
			}
			
			
			/// Update the frequency bands for the crossover so that they have the correct filters.
			void updateFrequencyBands();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the crossover frequency split points and filter orders for this crossover.
			ArrayList<CrossoverPoint> points;
			
			
			/// A list of information for each output frequency band of this crossover.
			ArrayList<FrequencyBand> frequencyBands;
			
			
			/// A boolean value indicating whether or not the crossover is given a multiband multi-input format.
			/**
			  * If this value is TRUE, the crossover will use the audio from each input for the corresponding output,
			  * rather than using only the first input. The number of inputs in this case will be the same as the number
			  * of outputs. If this value is FALSE, the first input is used to feed all frequency band outputs.
			  */
			Bool multibandInput;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CROSSOVER_H
