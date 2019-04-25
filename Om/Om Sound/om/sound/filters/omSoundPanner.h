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

#ifndef INCLUDE_OM_SOUND_PANNER_H
#define INCLUDE_OM_SOUND_PANNER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that pans input sound around a 360 degree sound panning field.
/**
  * This class takes the input channel layout and pans that layout around
  * the output channel layout based on a 3D panning direction.
  */
class Panner : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound panner with the default panning direction (forward).
			OM_INLINE Panner()
				:	SoundFilter( 1, 1 ),
					pan()
			{
			}
			
			
			/// Create a new sound panner with the specified panning direction.
			OM_INLINE Panner( const PanDirection& newPan )
				:	SoundFilter( 1, 1 ),
					pan( newPan )
			{
			}
			
			
		//********************************************************************************
		//******	Pan Accessor Methods
			
			
			/// Return the current panning direction of this sound panner.
			OM_INLINE const PanDirection& getPan() const
			{
				return pan;
			}
			
			
			/// Set the current panning direction of this sound panner.
			OM_INLINE void setPan( const PanDirection& newPan )
			{
				lockMutex();
				pan = newPan;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this sound panner.
			/**
			  * The method returns the string "Panner".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this sound panner.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this sound panner.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::IMAGING.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this panner.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this panner.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this panner.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Private Filter Processing Method
			
			
			/// Pan the input channel layout to the output channel layout.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object representing the current panning direction of this sound panner.
			PanDirection pan;
			
			
			/// An object that holds the current channel matrix mixing gains for each channel pairing.
			ChannelMixMatrix channelGains;
			
			
			/// An object that holds the target channel matrix mixing gains for each channel pairing.
			ChannelMixMatrix targetChannelGains;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PANNER_H
