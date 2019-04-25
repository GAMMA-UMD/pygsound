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

#ifndef INCLUDE_OM_SOUND_HDR_FILTER_H
#define INCLUDE_OM_SOUND_HDR_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"
#include "omSoundCompressor.h"
#include "omSoundLimiter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that maps a high-dynamic-range input audio source to a low-dynamic-range output.
/**
  * This class applies specially-tuned compression and limiting to reduce the
  * dynamic range of sound that is over the 0dBFS (clipping) threshold in a 
  * way that approximates how human hearing adjusts to dynamic changes in
  * sound level.
  */
class HDRFilter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new HDR filter with the default parameters.
			HDRFilter();
			
			
		//********************************************************************************
		//******	Input Gain Accessor Methods
			
			
			/// Return the current linear input gain factor of this HDR filter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * HDR filter. This allows the user to scale the input to match the HDR filter
			  * without having to change the HDR filter threshold.
			  */
			OM_INLINE Gain getInputGain() const
			{
				return comp1.getInputGain();
			}
			
			
			/// Return the current input gain factor in decibels of this HDR filter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * HDR filter. This allows the user to scale the input to match the HDR filter
			  * without having to change the HDR filter threshold.
			  */
			OM_INLINE Gain getInputGainDB() const
			{
				return comp1.getInputGainDB();
			}
			
			
			/// Set the target linear input gain for HDR filter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * HDR filter. This allows the user to scale the input to match the HDR filter
			  * without having to change the HDR filter threshold.
			  */
			OM_INLINE void setInputGain( Gain newInputGain )
			{
				lockMutex();
				comp1.setInputGain( newInputGain );
				unlockMutex();
			}
			
			
			/// Set the target input gain in decibels for this HDR filter.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * HDR filter. This allows the user to scale the input to match the HDR filter
			  * without having to change the HDR filter threshold.
			  */
			OM_INLINE void setInputGainDB( Gain newDBInputGain )
			{
				lockMutex();
				comp1.setInputGainDB( newDBInputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Output Gain Accessor Methods
			
			
			/// Return the current linear output gain factor of this HDR filter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * HDR filter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE Gain getOutputGain() const
			{
				return limiter.getOutputGain();
			}
			
			
			/// Return the current output gain factor in decibels of this HDR filter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * HDR filter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE Gain getOutputGainDB() const
			{
				return limiter.getOutputGainDB();
			}
			
			
			/// Set the target linear output gain for this HDR filter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * HDR filter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE void setOutputGain( Gain newOutputGain )
			{
				lockMutex();
				limiter.setOutputGain( newOutputGain );
				unlockMutex();
			}
			
			
			/// Set the target output gain in decibels for this HDR filter.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * HDR filter. This value is used to apply make-up gain to the signal
			  * after is has been compressed.
			  */
			OM_INLINE void setOutputGainDB( Gain newDBOutputGain )
			{
				lockMutex();
				limiter.setOutputGainDB( newDBOutputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this HDR filter.
			/**
			  * The method returns the string "HDR Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this HDR filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this HDR filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DYNAMICS.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this HDR filter can process audio data in-place.
			/**
			  * This method always returns TRUE, HDR filters can process audio data in-place.
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
			
			
			/// A string indicating the human-readable name of this HDR filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this HDR filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this HDR filter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Filter Parameter Value Accessor Methods
			
			
			/// Place the value of the parameter at the specified index in the output parameter.
			virtual Bool getParameterValue( Index parameterIndex, FilterParameter& value ) const;
			
			
			/// Attempt to set the parameter value at the specified index.
			virtual Bool setParameterValue( Index parameterIndex, const FilterParameter& value );
			
			
		//********************************************************************************
		//******	Private Filter Processing Methods
			
			
			/// Apply this HDR filter to the input frame and put the result in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
												SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A first compressor that reduces transients with moderate attack/release.
			Compressor comp1;
			
			
			/// A second compressor that is used to adjust to slow changes in dynamic range.
			Compressor comp2;
			
			
			/// A third compressor that reduces fast transients.
			Compressor comp3;
			
			
			/// A limiter that prevents clipping.
			Limiter limiter;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_HDR_FILTER_H
