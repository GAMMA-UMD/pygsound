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

#include "omSoundMixer.h"


#define PARAMETER_INDEX_INPUT_COUNT 0
#define PARAMETER_COUNT 1

#define PARAMETER_NAME_INPUT_COUNT "Input Count"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Mixer:: NAME( "Mixer" );
const UTF8String Mixer:: MANUFACTURER( "Om Sound" );
const FilterVersion Mixer:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Mixer:: getName() const
{
	return NAME;
}




UTF8String Mixer:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Mixer:: getVersion() const
{
	return VERSION;
}




FilterCategory Mixer:: getCategory() const
{
	return FilterCategory::ROUTING;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size Mixer:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Mixer:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_COUNT:
			info = FilterParameterInfo( PARAMETER_INDEX_INPUT_COUNT, PARAMETER_NAME_INPUT_COUNT,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(1000), Int64(100),
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




Bool Mixer:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_COUNT:
			value = FilterParameter( (Int64)this->getInputCount() );
			return true;
	}
	
	return false;
}




Bool Mixer:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 intValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_COUNT:
			if ( value.getValue( intValue ) )
			{
				this->setInputCount( Size(intValue) );
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




SoundResult Mixer:: processFrame( const SoundFrame& inputFrame,
										SoundFrame& outputFrame, Size numSamples )
{
	SoundBuffer* outputBuffer;
	
	// If there is no output buffer or if it is NULL, return that no samples were processed.
	if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
		return 0;
	
	// Make sure that the output buffer is big enough.
	if ( outputBuffer->getSize() < numSamples )
		outputBuffer->setSize( numSamples );
	
	// Zero the output buffer.
	outputBuffer->zero( 0, numSamples );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************
	// Mix each input buffer to the output buffer.
	
	// Determine the maximum number of inputs that the mixer can use.
	const Size numInputBuffers = math::min( this->getInputCount(), inputFrame.getBufferCount() );
	
	for ( Index i = 0; i < numInputBuffers; i++ )
	{
		const SoundBuffer* inputBuffer;
		
		// Skip NULL input buffers.
		if ( (inputBuffer = inputFrame.getBuffer(i)) == NULL )
			continue;
		
		// If the input buffer is wider than the output buffer, enlarge the output
		// buffer to hold the new input channel(s).
		if ( outputBuffer->getChannelCount() < inputBuffer->getChannelCount() )
			outputBuffer->setChannelCount( inputBuffer->getChannelCount() );
		
		inputBuffer->mixTo( *outputBuffer, math::min( inputBuffer->getSize(), numSamples ) );
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
