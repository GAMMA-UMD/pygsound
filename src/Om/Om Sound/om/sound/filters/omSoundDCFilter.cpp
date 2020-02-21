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

#include "omSoundDCFilter.h"


#define PARAMETER_INDEX_FEEDBACK 0
#define PARAMETER_COUNT 1

#define PARAMETER_NAME_FEEDBACK "Feedback"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String DCFilter:: NAME( "DC Filter" );
const UTF8String DCFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion DCFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




DCFilter:: DCFilter()
	:	feedback( 0.999 )
{
}




DCFilter:: DCFilter( Float newFeedback )
{
	this->setFeedback( newFeedback );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String DCFilter:: getName() const
{
	return NAME;
}




UTF8String DCFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion DCFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory DCFilter:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




Bool DCFilter:: allowsInPlaceProcessing() const
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




Size DCFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool DCFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FEEDBACK:
			info = FilterParameterInfo( PARAMETER_INDEX_FEEDBACK, PARAMETER_NAME_FEEDBACK,
										FilterParameterType::FLOAT,
										FilterParameterUnits::GENERIC, FilterParameterCurve::LOGARITHMIC,
										0.0f, 1.0f, 0.999f,
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




Bool DCFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FEEDBACK:
			value = FilterParameter( this->getFeedback() );
			return true;
	}
	
	return false;
}




Bool DCFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Float floatValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FEEDBACK:
			if ( value.getValue( floatValue ) )
			{
				this->setFeedback( floatValue );
				return true;
			}
			break;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Method
//############		
//##########################################################################################
//##########################################################################################




void DCFilter:: resetStream()
{
	// Reset the channel histories.
	const Size numChannels = channelHistory.getSize();
	
	for ( Index c = 0; c < numChannels; c++ )
		channelHistory[c].reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult DCFilter:: processFrame( const SoundFrame& inputFrame,
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
	
	const SampleRate inputSampleRate = inputBuffer->getSampleRate();
	
	// If the input sample rate is zero, return that an error occurred.
	if ( inputSampleRate <= SampleRate(0) )
		return SoundResult::ERROR;
	
	// Make sure that the output buffer has the right size and format.
	inputBuffer->copyFormatTo( *outputBuffer, numSamples );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	const Size numChannels = inputBuffer->getChannelCount();
	
	// Make sure the channel history is big enough.
	if ( channelHistory.getSize() < numChannels )
		channelHistory.setSize( numChannels );
	
	//*************************************************************************
	// Do the main filter processing.
	
	const Float a = feedback;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer->getChannel(c);
		Sample32f* output = outputBuffer->getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		ChannelHistory& history = channelHistory[c];
		Float lastInput = history.input;
		Float lastOutput = history.output;
		
		while ( output != outputEnd )
		{
			const Float out = (*input - lastInput) + a*lastOutput;
			lastInput = *input;
			lastOutput = out;
			*output = out;
			input++;
			output++;
		}
		
		history.input = lastInput;
		history.output = lastOutput;
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
