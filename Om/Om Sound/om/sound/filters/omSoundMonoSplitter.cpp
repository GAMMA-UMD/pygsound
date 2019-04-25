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

#include "omSoundMonoSplitter.h"


#define PARAMETER_INDEX_OUTPUT_CHANNEL_COUNT 0
#define PARAMETER_COUNT 1

#define PARAMETER_NAME_OUTPUT_CHANNEL_COUNT "Output Channel Count"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String MonoSplitter:: NAME( "Mono Splitter" );
const UTF8String MonoSplitter:: MANUFACTURER( "Om Sound" );
const FilterVersion MonoSplitter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String MonoSplitter:: getName() const
{
	return NAME;
}




UTF8String MonoSplitter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion MonoSplitter:: getVersion() const
{
	return VERSION;
}




FilterCategory MonoSplitter:: getCategory() const
{
	return FilterCategory::IMAGING;
}




Bool MonoSplitter:: allowsInPlaceProcessing() const
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




Size MonoSplitter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool MonoSplitter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_CHANNEL_COUNT:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_CHANNEL_COUNT, PARAMETER_NAME_OUTPUT_CHANNEL_COUNT,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(100), Int64(1),
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




Bool MonoSplitter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_CHANNEL_COUNT:
			value = FilterParameter( (Int64)this->getChannelCount() );
			return true;
	}
	
	return false;
}




Bool MonoSplitter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 intValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_CHANNEL_COUNT:
			if ( value.getValue( intValue ) )
			{
				this->setChannelCount( Size(intValue) );
				return true;
			}
			break;
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




SoundResult MonoSplitter:: processFrame( const SoundFrame& inputFrame,
												SoundFrame& outputFrame, Size numSamples )
{
	const SoundBuffer* inputBuffer;
	
	// If there is no input buffer or if it is NULL, return that an error occurred.
	if ( inputFrame.getBufferCount() == 0 || (inputBuffer = inputFrame.getBuffer(0)) == NULL )
		return SoundResult::ERROR;
	
	// If the input buffer doesn't have any channels, return that an error occurred.
	if ( inputBuffer->getChannelCount() == 0 )
		return SoundResult::ERROR;
	
	SoundBuffer* outputBuffer;
	
	// If there is no output buffer or if it is NULL, return that no samples were processed.
	if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
		return 0;
	
	// Make sure the output buffer is the right size.
	if ( outputBuffer->getSize() < numSamples )
		outputBuffer->setSize( numSamples );
	
	// Make sure the output buffer has the right number of channels.
	if ( outputBuffer->getChannelCount() != numOutputChannels )
		outputBuffer->setChannelCount( numOutputChannels );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************
	// Copy the input buffer channel to each output buffer channel.
	
	// Get information about the first input buffer channel.
	const Sample32f* const inputStart = inputBuffer->getChannel(0);
	const Sample32f* const inputEnd = inputStart + numSamples;
	
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		const Sample32f* input = inputStart;
		Sample32f* output = outputBuffer->getChannel(c);
		
		while ( input != inputEnd )
		{
			*output = *input;
			input++;
			output++;
		}
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
