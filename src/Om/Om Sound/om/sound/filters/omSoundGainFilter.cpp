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

#include "omSoundGainFilter.h"


#define PARAMETER_INDEX_GAIN 0
#define PARAMETER_COUNT 1

#define PARAMETER_NAME_GAIN "Gain"


#define PRESET_INDEX_UNITY 0
#define PRESET_COUNT 1


#define PRESET_NAME_UNITY "Unity"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String GainFilter:: NAME( "Gain Filter" );
const UTF8String GainFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion GainFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String GainFilter:: getName() const
{
	return NAME;
}




UTF8String GainFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion GainFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory GainFilter:: getCategory() const
{
	return FilterCategory::DYNAMICS;
}




Bool GainFilter:: allowsInPlaceProcessing() const
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size GainFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool GainFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	if ( parameterIndex == PARAMETER_INDEX_GAIN )
	{
		info = FilterParameterInfo( PARAMETER_INDEX_GAIN, PARAMETER_NAME_GAIN,
									FilterParameterType::FLOAT,
									FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
									-50.0f, 24.0f, 0.0f,
									FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Value Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool GainFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	if ( parameterIndex == PARAMETER_INDEX_GAIN )
	{
		value = FilterParameter( this->getGainDB() );
		return true;
	}
	
	return false;
}




Bool GainFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
	
	if ( parameterIndex == PARAMETER_INDEX_GAIN && value.getValue(gainValue) )
	{
		this->setGainDB( gainValue );
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Preset Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size GainFilter:: getPresetCount() const
{
	return PRESET_COUNT;
}




Bool GainFilter:: getPreset( Index presetIndex, FilterPreset& preset ) const
{
	if ( presetIndex >= PRESET_COUNT )
		return false;
	
	// Clear the state of the preset to be sure.
	FilterState& state = preset.getState();
	state.clear();
	
	switch ( presetIndex )
	{
		case PRESET_INDEX_UNITY:
			preset.setName( PRESET_NAME_UNITY );
			state.set( PARAMETER_INDEX_GAIN, 0.0f );
			break;
		default:
			return false;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult GainFilter:: processFrame( const SoundFrame& inputFrame,
											SoundFrame& outputFrame, Size numSamples )
{
	const SoundBuffer* inputBuffer;
	
	// If there is no input buffer or if it is NULL, return that an error occurred.
	if ( inputFrame.getBufferCount() == 0 || (inputBuffer = inputFrame.getBuffer(0)) == NULL )
		return SoundResult::ERROR;
	
	SoundBuffer* outputBuffer;
	
	// If there is no output buffer or if it is NULL, return that no samples were processed.
	if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
		return 0;
	
	// Make sure that the output buffer has the right size and format.
	inputBuffer->copyFormatTo( *outputBuffer, numSamples );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//******************************************************************************
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
		gain = targetGain;
	
	// Compute how much the gain factor should change per sample.
	Gain gainChangePerSample = Gain(0.5)*(targetGain - gain) / numSamples;
	
	Gain currentGain;
	const Size numChannels = inputBuffer->getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer->getChannel(c);
		Sample32f* output = outputBuffer->getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		currentGain = gain;
		
		while ( output != outputEnd )
		{
			*output = currentGain*(*input);
			
			input++;
			output++;
			currentGain += gainChangePerSample;
		}
	}
	
	// Set the current gain as the starting gain for the next processing frame.
	gain = currentGain;
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
