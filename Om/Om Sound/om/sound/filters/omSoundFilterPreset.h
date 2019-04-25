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

#ifndef INCLUDE_OM_SOUND_FILTER_PRESET_H
#define INCLUDE_OM_SOUND_FILTER_PRESET_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilterState.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a preset configuration for a SoundFilter.
/**
  * A sound filter preset contains a FilterState object that stores the
  * preset configuration, as well as a human-readable name associated with the preset.
  */
class FilterPreset
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound filter preset with no name or data stored.
			OM_INLINE FilterPreset()
				:	state(),
					name()
			{
			}
			
			
			/// Create a new sound filter preset with the specified name and no data stored.
			OM_INLINE FilterPreset( const UTF8String& newName )
				:	state(),
					name( newName )
			{
			}
			
			
			/// Create a new sound filter preset with the specified name and filter state structure.
			OM_INLINE FilterPreset( const UTF8String& newName, const FilterState& newState )
				:	state( newState ),
					name( newName )
			{
			}
			
			
		//********************************************************************************
		//******	State Accessor Methods
			
			
			/// Return a reference to the FilterState object that contains the information for this preset.
			OM_INLINE FilterState& getState()
			{
				return state;
			}
			
			
			/// Return a reference to the FilterState object that contains the information for this preset.
			OM_INLINE const FilterState& getState() const
			{
				return state;
			}
			
			
			/// Set the FilterState object that contains the information for this preset.
			OM_INLINE void setState( const FilterState& newState )
			{
				state = newState;
			}
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return the human-readable name of this filter preset.
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set the human-readable name of this filter preset.
			OM_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that stores the filter state associated with this preset.
			FilterState state;
			
			
			/// The name associated with this preset.
			UTF8String name;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_PRESET_H
