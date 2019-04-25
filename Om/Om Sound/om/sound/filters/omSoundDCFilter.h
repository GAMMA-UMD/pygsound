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

#ifndef INCLUDE_OM_SOUND_DC_FILTER_H
#define INCLUDE_OM_SOUND_DC_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements a DC-blocking filter that filters out frequencies near 0Hz.
class DCFilter : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a DC filter with the default feedback gain.
			DCFilter();
			
			
			/// Create a DC filter with the specified feedback gain.
			DCFilter( Float newFeedback );
			
			
		//********************************************************************************
		//******	Feedback Gain Accessor Methods
			
			
			/// Return the feedback gain of this DC filter.
			/**
			  * This value lies in the range [0,1]. The closer to 1, the
			  * more the filter becomes an ideal DC filter that blocks only the 0Hz frequency.
			  */
			OM_INLINE Float getFeedback() const
			{
				return feedback;
			}
			
			
			/// Set the feedback gain of this DC filter.
			/**
			  * This value lies in the range [0,1]. The closer to 1, the
			  * more the filter becomes an ideal DC filter that blocks only the 0Hz frequency.
			  *
			  * The new feedback gain is clamped to be in the range [0,+infinity].
			  */
			OM_INLINE void setFeedback( Float newFeedback )
			{
				lockMutex();
				feedback = math::clamp( newFeedback, Float(0), Float(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this DC filter.
			/**
			  * The method returns the string "DC Filter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this DC filter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this DC filter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::EQUALIZER.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this DC filter can process audio data in-place.
			/**
			  * This method always returns TRUE, DC filters can process audio data in-place.
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
			
			
			/// A string indicating the human-readable name of this DC filter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this DC filter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this DC filter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Private Filter Channel Class Declaration
			
			
			/// A class that contains a history of the last input and output samples for a first order DC filter.
			class ChannelHistory
			{
				public:
					
					OM_INLINE ChannelHistory()
					{
						this->reset();
					}
					
					OM_INLINE void reset()
					{
						input = Float(0);
						output = Float(0);
					}
					
					/// An array of the last 2 input samples for a filter with order 2.
					Float input;
					
					
					/// An array of the last 2 output samples for a filter with order 2.
					Float output;
					
					
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
			
			
			/// Apply this DC filter to the samples in the input frame and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The feedback gain parameter for this DC filter in the range [0,1].
			Float feedback;
			
			
			/// An array of input and output history information for each channel of this filter.
			Array<ChannelHistory> channelHistory;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_DC_FILTER_H
