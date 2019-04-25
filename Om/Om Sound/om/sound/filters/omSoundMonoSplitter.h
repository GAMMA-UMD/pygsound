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

#ifndef INCLUDE_OM_SOUND_MONO_SPLITTER_H
#define INCLUDE_OM_SOUND_MONO_SPLITTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that copies a single input channel into multiple output channels.
/**
  * This class takes the first channel of its input buffer and copies to a
  * user-defined number of channels in the output buffer.
  */
class MonoSplitter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new mono splitter with the default number of output channels, 1.
			OM_INLINE MonoSplitter()
				:	SoundFilter( 1, 1 ),
					numOutputChannels( 1 )
			{
			}
			
			
			/// Create a new mono splitter which has the specified number of output channels.
			OM_INLINE MonoSplitter( Size newNumOutputChannels )
				:	SoundFilter( 1, 1 ),
					numOutputChannels( math::max( newNumOutputChannels, Size(1) ) )
			{
			}
			
			
		//********************************************************************************
		//******	Output Channel Count Accessor Methods
			
			
			/// Return the total number of output channels that this mono splitter has.
			/**
			  * This is the number of channels that the first input channel is split into.
			  */
			OM_INLINE Size getChannelCount() const
			{
				return numOutputChannels;
			}
			
			
			/// Set the total number of output channels that this mono splitter has.
			/**
			  * This is the number of channels that the first input channel is split into.
			  *
			  * The specified number of channels is clamped to be in the range [1,infinity].
			  */
			OM_INLINE void setChannelCount( Size newNumOutputChannels )
			{
				numOutputChannels = math::max( newNumOutputChannels, Size(1) );
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this mono splitter.
			/**
			  * The method returns the string "Mono Splitter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this mono splitter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this mono splitter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::IMAGING.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this splitter can process audio data in-place.
			/**
			  * This method always returns TRUE, splitters can process audio data in-place.
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
			
			
			/// A string indicating the human-readable name of this mono splitter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this mono splitter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this mono splitter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Filter Parameter Value Accessor Methods
			
			
			/// Place the value of the parameter at the specified index in the output parameter.
			virtual Bool getParameterValue( Index parameterIndex, FilterParameter& value ) const;
			
			
			/// Attempt to set the parameter value at the specified index.
			virtual Bool setParameterValue( Index parameterIndex, const FilterParameter& value );
			
			
		//********************************************************************************
		//******	Private Filter Processing Method
			
			
			/// Split the sound in the first input buffer channel to as many output channels as necessary.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of channels into which the first input buffer channel is being split.
			Size numOutputChannels;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_MONO_SPLITTER_H
