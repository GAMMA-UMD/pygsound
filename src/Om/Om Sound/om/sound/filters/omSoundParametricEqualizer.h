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

#ifndef INCLUDE_OM_SOUND_PARAMETRIC_EQUALIZER_H
#define INCLUDE_OM_SOUND_PARAMETRIC_EQUALIZER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"
#include "omSoundParametricFilter.h"
#include "omSoundCutoffFilter.h"
#include "omSoundShelfFilter.h"
#include "omSoundGainFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that provides a basic 5-band parametric EQ with additional high/low shelf/pass filters.
class ParametricEqualizer : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default parametric equalizer with 5 parametric filter bands.
			ParametricEqualizer();
			
			
		//********************************************************************************
		//******	Output Gain Accessor Methods
			
			
			/// Return the linear output gain for this parametric equalizer.
			OM_INLINE Gain getOutputGain() const
			{
				return gainFilter.getGain();
			}
			
			
			/// Return the output gain in decibels for this parametric equalizer.
			OM_INLINE Gain getOutputGainDB() const
			{
				return gainFilter.getGainDB();
			}
			
			
			/// Set the linear output gain for this parametric equalizer.
			OM_INLINE void setOutputGain( Gain newGain )
			{
				lockMutex();
				gainFilter.setGain( newGain );
				unlockMutex();
			}
			
			
			/// Set the output gain in decibels for this parametric equalizer.
			OM_INLINE void setOutputGainDB( Gain newGain )
			{
				lockMutex();
				gainFilter.setGainDB( newGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Parametric Filter Accessor Methods
			
			
			/// Return the number of parametric EQ filters that make up this parametric equalizer.
			OM_INLINE Size getParametricCount() const
			{
				return parametrics.getSize();
			}
			
			
			/// Set the number of parametric EQ filters that make up this parametric equalizer.
			/**
			  * If the specified new number of parametric filters is less than the old number,
			  * the unnecessary filters are removed and deleted. If the new number is greater,
			  * the new frequency bands are initialzed to have the center frequency of 1000Hz.
			  */
			OM_INLINE void setParametricCount( Size newNumberOfParametrics )
			{
				lockMutex();
				parametrics.setSize( newNumberOfParametrics );
				unlockMutex();
			}
			
			
			/// Return whether or not the parametric filter within this equalizer at the specified index is enabled.
			OM_INLINE Bool getParametricIsEnabled( Index parametricIndex ) const
			{
				if ( parametricIndex < parametrics.getSize() )
					return parametrics[parametricIndex].isEnabled;
				else
					return false;
			}
			
			
			/// Set whether or not the parametric filter within this equalizer at the specified index is enabled.
			OM_INLINE void setParametricIsEnabled( Index parametricIndex, Bool newIsEnabled )
			{
				lockMutex();
				if ( parametricIndex < parametrics.getSize() )
					parametrics[parametricIndex].isEnabled = newIsEnabled;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Parametric Filter Gain Accessor Methods
			
			
			/// Return the linear gain of the parametric filter within this equalizer at the specified index.
			OM_INLINE Gain getParametricGain( Index parametricIndex ) const
			{
				if ( parametricIndex < parametrics.getSize() )
					return parametrics[parametricIndex].filter.getGain();
				else
					return Gain(0);
			}
			
			
			/// Return the gain in decibels of the parametric filter within this equalizer at the specified index.
			OM_INLINE Gain getParametricGainDB( Index parametricIndex ) const
			{
				if ( parametricIndex < parametrics.getSize() )
					return parametrics[parametricIndex].filter.getGainDB();
				else
					return math::negativeInfinity<Gain>();
			}
			
			
			/// Set the linear gain of the parametric filter within this equalizer at the specified index.
			OM_INLINE void setParametricGain( Index parametricIndex, Gain newGain )
			{
				lockMutex();
				if ( parametricIndex < parametrics.getSize() )
					parametrics[parametricIndex].filter.setGain( newGain );
				unlockMutex();
			}
			
			
			/// Set the gain in decibels of the parametric filter within this equalizer at the specified index.
			OM_INLINE void setParametricGainDB( Index parametricIndex, Gain newGain )
			{
				lockMutex();
				if ( parametricIndex < parametrics.getSize() )
					parametrics[parametricIndex].filter.setGainDB( newGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Parametric Filter Frequency Accessor Methods
			
			
			/// Return the center frequency of the parametric filter within this equalizer at the specified index.
			OM_INLINE Float getParametricFrequency( Index parametricIndex ) const
			{
				if ( parametricIndex < parametrics.getSize() )
					return parametrics[parametricIndex].filter.getFrequency();
				else
					return Float(0);
			}
			
			
			/// Set the center frequency of the parametric filter within this equalizer at the specified index.
			OM_INLINE void setParametricFrequency( Index parametricIndex, Float newFrequency )
			{
				lockMutex();
				if ( parametricIndex < parametrics.getSize() )
					parametrics[parametricIndex].filter.setFrequency( newFrequency );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Parametric Filter Bandwidth Accessor Methods
			
			
			/// Return the Q factor of the parametric filter within this equalizer at the specified index.
			OM_INLINE Float getParametricQ( Index parametricIndex ) const
			{
				if ( parametricIndex < parametrics.getSize() )
					return parametrics[parametricIndex].filter.getQ();
				else
					return Float(0);
			}
			
			
			/// Set the Q factor of the parametric filter within this equalizer at the specified index.
			OM_INLINE void setParametricQ( Index parametricIndex, Float newQ )
			{
				lockMutex();
				if ( parametricIndex < parametrics.getSize() )
					parametrics[parametricIndex].filter.setQ( newQ );
				unlockMutex();
			}
			
			
			/// Return the bandwidth in octaves of the parametric filter within this equalizer at the specified index.
			OM_INLINE Float getParametricBandwidth( Index parametricIndex ) const
			{
				if ( parametricIndex < parametrics.getSize() )
					return parametrics[parametricIndex].filter.getBandwidth();
				else
					return Float(0);
			}
			
			
			/// Set the bandwidth in octaves of the parametric filter within this equalizer at the specified index.
			OM_INLINE void setParametricBandwidth( Index parametricIndex, Float newBandwidth )
			{
				lockMutex();
				if ( parametricIndex < parametrics.getSize() )
					parametrics[parametricIndex].filter.setBandwidth( newBandwidth );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	High-Pass Filter Frequency Accessor Methods
			
			
			/// Return the corner frequency of this parametric equalizer's high pass filter.
			OM_INLINE Float getHighPassFrequency() const
			{
				return highPass.getFrequency();
			}
			
			
			/// Set the corner frequency of this parametric equalizer's high pass filter.
			OM_INLINE void setHighPassFrequency( Float newFrequency )
			{
				lockMutex();
				highPass.setFrequency( newFrequency );
				unlockMutex();
			}
			
			
			/// Return the order of this parametric equalizer's high pass filter.
			OM_INLINE Size getHighPassOrder() const
			{
				return highPass.getOrder();
			}
			
			
			/// Set the order of this parametric equalizer's high pass filter.
			OM_INLINE void setHighPassOrder( Size newOrder )
			{
				lockMutex();
				highPass.setOrder( newOrder );
				unlockMutex();
			}
			
			
			/// Return whether or not the high pass filter of this parametric equalizer is enabled.
			OM_INLINE Bool getHighPassIsEnabled() const
			{
				return highPassEnabled;
			}
			
			
			/// Set whether or not the high pass filter of this parametric equalizer is enabled.
			OM_INLINE void setHighPassIsEnabled( Bool newIsEnabled )
			{
				lockMutex();
				highPassEnabled = newIsEnabled;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Low-Pass Filter Attribute Accessor Methods
			
			
			/// Return the corner frequency of this parametric equalizer's low pass filter.
			OM_INLINE Float getLowPassFrequency() const
			{
				return lowPass.getFrequency();
			}
			
			
			/// Set the corner frequency of this parametric equalizer's low pass filter.
			OM_INLINE void setLowPassFrequency( Float newFrequency )
			{
				lockMutex();
				lowPass.setFrequency( newFrequency );
				unlockMutex();
			}
			
			
			/// Return the order of this parametric equalizer's low pass filter.
			OM_INLINE Size getLowPassOrder() const
			{
				return lowPass.getOrder();
			}
			
			
			/// Set the order of this parametric equalizer's low pass filter.
			OM_INLINE void setLowPassOrder( Size newOrder )
			{
				lockMutex();
				lowPass.setOrder( newOrder );
				unlockMutex();
			}
			
			
			/// Return whether or not the low pass filter of this parametric equalizer is enabled.
			OM_INLINE Bool getLowPassIsEnabled() const
			{
				return lowPassEnabled;
			}
			
			
			/// Set whether or not the low pass filter of this parametric equalizer is enabled.
			OM_INLINE void setLowPassIsEnabled( Bool newIsEnabled )
			{
				lockMutex();
				lowPassEnabled = newIsEnabled;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Low Shelf Filter Attribute Accessor Methods
			
			
			/// Return the corner frequency of this parametric equalizer's low shelf filter.
			OM_INLINE Float getLowShelfFrequency() const
			{
				return lowShelf.getFrequency();
			}
			
			
			/// Set the corner frequency of this parametric equalizer's low shelf filter.
			OM_INLINE void setLowShelfFrequency( Float newFrequency )
			{
				lockMutex();
				lowShelf.setFrequency( newFrequency );
				unlockMutex();
			}
			
			
			/// Return the linear gain of this parametric equalizer's low shelf filter.
			OM_INLINE Gain getLowShelfGain() const
			{
				return lowShelf.getGain();
			}
			
			
			/// Return the gain in decibels of this parametric equalizer's low shelf filter.
			OM_INLINE Gain getLowShelfGainDB() const
			{
				return lowShelf.getGainDB();
			}
			
			
			/// Set the linear gain of this parametric equalizer's low shelf filter.
			OM_INLINE void setLowShelfGain( Gain newGain )
			{
				lockMutex();
				lowShelf.setGain( newGain );
				unlockMutex();
			}
			
			
			/// Set the gain in decibels of this parametric equalizer's low shelf filter.
			OM_INLINE void setLowShelfGainDB( Gain newGain )
			{
				lockMutex();
				lowShelf.setGainDB( newGain );
				unlockMutex();
			}
			
			
			/// Return the slope of this parametric equalizer's low shelf filter.
			OM_INLINE Float getLowShelfSlope() const
			{
				return lowShelf.getGain();
			}

			
			/// Set the slope of this parametric equalizer's low shelf filter.
			OM_INLINE void setLowShelfSlope( Float newSlope )
			{
				lockMutex();
				lowShelf.setSlope( newSlope );
				unlockMutex();
			}
			
			
			/// Return whether or not the low shelf filter of this parametric equalizer is enabled.
			OM_INLINE Bool getLowShelfIsEnabled() const
			{
				return lowShelfEnabled;
			}
			
			
			/// Set whether or not the low shelf filter of this parametric equalizer is enabled.
			OM_INLINE void setLowShelfIsEnabled( Bool newIsEnabled )
			{
				lockMutex();
				lowShelfEnabled = newIsEnabled;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	High Shelf Filter Attribute Accessor Methods
			
			
			/// Return the corner frequency of this parametric equalizer's high shelf filter.
			OM_INLINE Float getHighShelfFrequency() const
			{
				return highShelf.getFrequency();
			}
			
			
			/// Set the corner frequency of this parametric equalizer's high shelf filter.
			OM_INLINE void setHighShelfFrequency( Float newFrequency )
			{
				lockMutex();
				highShelf.setFrequency( newFrequency );
				unlockMutex();
			}
			
			
			/// Return the linear gain of this parametric equalizer's high shelf filter.
			OM_INLINE Gain getHighShelfGain() const
			{
				return highShelf.getGain();
			}
			
			
			/// Return the gain in decibels of this parametric equalizer's high shelf filter.
			OM_INLINE Gain getHighShelfGainDB() const
			{
				return highShelf.getGainDB();
			}
			
			
			/// Set the linear gain of this parametric equalizer's high shelf filter.
			OM_INLINE void setHighShelfGain( Gain newGain )
			{
				lockMutex();
				highShelf.setGain( newGain );
				unlockMutex();
			}
			
			
			/// Set the gain in decibels of this parametric equalizer's high shelf filter.
			OM_INLINE void setHighShelfGainDB( Gain newGain )
			{
				lockMutex();
				highShelf.setGainDB( newGain );
				unlockMutex();
			}
			
			
			/// Return the slope of this parametric equalizer's high shelf filter.
			OM_INLINE Float getHighShelfSlope() const
			{
				return highShelf.getGain();
			}

			
			/// Set the slope of this parametric equalizer's high shelf filter.
			OM_INLINE void setHighShelfSlope( Float newSlope )
			{
				lockMutex();
				highShelf.setSlope( newSlope );
				unlockMutex();
			}
			
			
			/// Return whether or not the high shelf filter of this parametric equalizer is enabled.
			OM_INLINE Bool getHighShelfIsEnabled() const
			{
				return highShelfEnabled;
			}
			
			
			/// Set whether or not the high shelf filter of this parametric equalizer is enabled.
			OM_INLINE void setHighShelfIsEnabled( Bool newIsEnabled )
			{
				lockMutex();
				highShelfEnabled = newIsEnabled;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this parametric equalizer.
			/**
			  * The method returns the string "Parametric Equalizer".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this parametric equalizer.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this parametric equalizer.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::EQUALIZER.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this parametric equalizer can process audio data in-place.
			/**
			  * This method always returns TRUE, parameteric equalizers can process audio data in-place.
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
			
			
			/// A string indicating the human-readable name of this parametric equalizer.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this parametric equalizer.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this parametric equalizer.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A class that holds information about a single band of parametric EQ.
			class ParametricFilterBand
			{
				public:
					
					/// Create a new parametric filter band, enabled by default.
					OM_INLINE ParametricFilterBand()
						:	filter(),
							isEnabled( true )
					{
					}
					
					
					/// The parametric filter associated with this frequency band.
					ParametricFilter filter;
					
					
					/// A boolean value indicating whether or not this frequency band is enabled.
					Bool isEnabled;
					
					
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
			
			
			/// Return whether or not the specified linear gain value is very close to unity gain.
			OM_INLINE static Bool gainIsUnity( Gain linearGain )
			{
				return math::abs( Gain(1) - linearGain ) < 2*math::epsilon<Gain>();
			}
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Define the default number of parametric filters that should make up a parametric equalizer.
			static const Size DEFAULT_NUMBER_OF_PARAMETRIC_FILTERS = 5;
			
			
			/// Define the default center frequencies of the parametric filters that make up this equalizer.
			static const Float DEFAULT_PARAMETRIC_FREQUENCIES[DEFAULT_NUMBER_OF_PARAMETRIC_FILTERS];
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A high pass filter for this parametric equalizer.
			CutoffFilter highPass;
			
			
			/// A low shelf filter for this parametric equalizer.
			ShelfFilter lowShelf;
			
			
			/// An array of the parametric filters that make up this parametric equalizer.
			Array<ParametricFilterBand> parametrics;
			
			
			/// A high shelf filter for this parametric equalizer.
			ShelfFilter highShelf;
			
			
			/// A low pass filter for this parametric equalizer.
			CutoffFilter lowPass;
			
			
			/// A master gain filter for this parametric equalizer.
			GainFilter gainFilter;
			
			
			/// A boolean value indicating whether or not the high pass filter is enabled.
			Bool highPassEnabled;
			
			
			/// A boolean value indicating whether or not the low pass filter is enabled.
			Bool lowPassEnabled;
			
			
			/// A boolean value indicating whether or not the low shelf filter is enabled.
			Bool lowShelfEnabled;
			
			
			/// A boolean value indicating whether or not the high shelf filter is enabled.
			Bool highShelfEnabled;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PARAMETRIC_EQUALIZER_H
