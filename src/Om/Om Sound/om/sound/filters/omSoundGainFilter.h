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

#ifndef INCLUDE_OM_SOUND_GAIN_FILTER_H
#define INCLUDE_OM_SOUND_GAIN_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that applies a simple gain factor to input audio.
class GainFilter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new gain filter with unity gain (gain = 1).
			/**
			  * The new gain filter doesn't do anything to input audio since
			  * it is a unity gain. Set the gain factor to modify the output gain
			  * to something else.
			  */
			OM_INLINE GainFilter()
				:	SoundFilter( 1, 1 ),
					gain( 1 ),
					targetGain( 1 )
			{
			}
			
			
			/// Create a new gain filter with the specified linear gain factor.
			OM_INLINE GainFilter( Gain newGain )
				:	SoundFilter( 1, 1 ),
					gain( newGain ),
					targetGain( newGain )
			{
			}
			
			
		//********************************************************************************
		//******	Gain Accessor Methods
			
			
			/// Return the current linear gain factor of this gain filter.
			OM_INLINE Gain getGain() const
			{
				return targetGain;
			}
			
			
			/// Return the current gain for this gain filter in decibels.
			OM_INLINE Gain getGainDB() const
			{
				return math::linearToDB( targetGain );
			}
			
			
			/// Set the target linear gain for this gain filter.
			OM_INLINE void setGain( Gain newGain )
			{
				lockMutex();
				targetGain = newGain;
				unlockMutex();
			}
			
			
			/// Set the target gain for this gain filter in decibels.
			OM_INLINE void setGainDB( Gain newDBGain )
			{
				lockMutex();
				targetGain = math::dbToLinear( newDBGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this gain filter.
			/**
			  * The method returns the string "Gain Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this gain filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this gain filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DYNAMICS.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this gain filter can process audio data in-place.
			/**
			  * This method always returns TRUE, gain filter can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the filter parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
		//********************************************************************************
		//******	Filter Preset Accessor Methods
			
			
			/// Return the number of standard configuration presets that this gain filter has.
			virtual Size getPresetCount() const;
			
			
			/// Get the standard preset for this gain filter with the specified index.
			virtual Bool getPreset( Index presetIndex, FilterPreset& preset ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this gain filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this gain filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this gain filter.
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
			
			
			/// Multiply the samples in the input frame by this gain filter's gain factor and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The current linear gain factor applied to all input audio.
			Gain gain;
			
			
			/// The target output gain for this gain filter.
			/**
			  * This value allows smooth changes between different gain values.
			  */
			Gain targetGain;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_GAIN_FILTER_H
