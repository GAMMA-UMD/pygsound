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

#include "omSoundHDRFilter.h"


#define PARAMETER_INDEX_INPUT_GAIN 0
#define PARAMETER_INDEX_OUTPUT_GAIN 1
#define PARAMETER_COUNT 2

#define PARAMETER_NAME_INPUT_GAIN "Input Gain"
#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String HDRFilter:: NAME( "HDR Filter" );
const UTF8String HDRFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion HDRFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




HDRFilter:: HDRFilter()
	:	SoundFilter( 1, 1 )
{
	// A first compressor that reduces transients with moderate attack/release.
	comp1.setThresholdDB( -6.0f );
	comp1.setKnee( 0.0f );
	comp1.setRatio( 3.0f );
	comp1.setAttack( 0.005f );
	comp1.setRelease( 0.02f );
	comp1.setChannelsAreLinked( false );
	comp1.setIsSynchronized( false );
	
	// A second compressor that is used to adjust to slow changes in dynamic range.
	comp2.setThresholdDB( -6.0f );
	comp2.setKnee( 6.0f );
	comp2.setRatio( 4.0f );
	comp2.setAttack( 0.2f );
	comp2.setRelease( 0.2f );
	comp2.setChannelsAreLinked( false );
	comp2.setIsSynchronized( false );
	
	/// A third compressor that reduces fast transients.
	comp3.setThresholdDB( -3.0f );
	comp3.setKnee( 2.0f );
	comp3.setRatio( 6.0f );
	comp3.setAttack( 0.0002f );
	comp3.setRelease( 0.01f );
	comp3.setChannelsAreLinked( false );
	comp3.setIsSynchronized( false );
	
	// A limiter that prevents clipping.
	limiter.setThresholdDB( 0.0f );
	limiter.setRelease( 0.02f );
	limiter.setKnee( 0.0f );
	limiter.setChannelsAreLinked( false );
	limiter.setOutputGainDB( -0.1f );
	limiter.setIsSynchronized( false );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String HDRFilter:: getName() const
{
	return NAME;
}




UTF8String HDRFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion HDRFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory HDRFilter:: getCategory() const
{
	return FilterCategory::DYNAMICS;
}




Bool HDRFilter:: allowsInPlaceProcessing() const
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




Size HDRFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool HDRFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	if ( parameterIndex == PARAMETER_INDEX_INPUT_GAIN )
	{
		info = FilterParameterInfo( PARAMETER_INDEX_INPUT_GAIN, PARAMETER_NAME_INPUT_GAIN,
									FilterParameterType::FLOAT,
									FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
									-50.0f, 24.0f, 0.0f,
									FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
		return true;
	}
	else if ( parameterIndex == PARAMETER_INDEX_OUTPUT_GAIN )
	{
		info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_GAIN, PARAMETER_NAME_OUTPUT_GAIN,
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




Bool HDRFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	if ( parameterIndex == PARAMETER_INDEX_INPUT_GAIN )
	{
		value = FilterParameter( this->getInputGainDB() );
		return true;
	}
	else if ( parameterIndex == PARAMETER_INDEX_OUTPUT_GAIN )
	{
		value = FilterParameter( this->getOutputGainDB() );
		return true;
	}
	
	return false;
}




Bool HDRFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
	
	if ( parameterIndex == PARAMETER_INDEX_INPUT_GAIN && value.getValue(gainValue) )
	{
		this->setInputGainDB( gainValue );
		return true;
	}
	else if ( parameterIndex == PARAMETER_INDEX_OUTPUT_GAIN && value.getValue(gainValue) )
	{
		this->setOutputGainDB( gainValue );
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult HDRFilter:: processFrame( const SoundFrame& inputFrame,
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
	
	// Process the first compressor from the input to the output buffer.
	// The buffers can be aliased, in-place processing is allowed for compressors and limiters.
	numSamples = comp1.process( *inputBuffer, *outputBuffer, numSamples );
	
	// Process the remaining filters in-place in the output buffer.
	numSamples = comp2.process( *outputBuffer, *outputBuffer, numSamples );
	numSamples = comp3.process( *outputBuffer, *outputBuffer, numSamples );
	numSamples = limiter.process( *outputBuffer, *outputBuffer, numSamples );
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
