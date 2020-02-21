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

#include "omSoundSplitter.h"


#define PARAMETER_INDEX_OUTPUT_COUNT 0
#define PARAMETER_COUNT 1

#define PARAMETER_NAME_OUTPUT_COUNT "Output Count"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Splitter:: NAME( "Splitter" );
const UTF8String Splitter:: MANUFACTURER( "Om Sound" );
const FilterVersion Splitter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Splitter:: getName() const
{
	return NAME;
}




UTF8String Splitter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Splitter:: getVersion() const
{
	return VERSION;
}




FilterCategory Splitter:: getCategory() const
{
	return FilterCategory::ROUTING;
}




Bool Splitter:: allowsInPlaceProcessing() const
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




Size Splitter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Splitter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_COUNT:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_COUNT, PARAMETER_NAME_OUTPUT_COUNT,
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




Bool Splitter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_COUNT:
			value = FilterParameter( (Int64)this->getOutputCount() );
			return true;
	}
	
	return false;
}




Bool Splitter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 intValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_COUNT:
			if ( value.getValue( intValue ) )
			{
				this->setOutputCount( Size(intValue) );
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




SoundResult Splitter:: processFrame( const SoundFrame& inputFrame,
											SoundFrame& outputFrame, Size numSamples )
{
	const SoundBuffer* inputBuffer;
	
	// If there is no input buffer or if it is NULL, return that an error occurred.
	if ( inputFrame.getBufferCount() == 0 || (inputBuffer = inputFrame.getBuffer(0)) == NULL )
		return SoundResult::ERROR;
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************
	// Copy the input buffer to each output buffer.
	
	// Determine the maximum number of outputs that the splitter can use.
	const Size numOutputBuffers = math::min( this->getOutputCount(), outputFrame.getBufferCount() );
	
	for ( Index i = 0; i < numOutputBuffers; i++ )
	{
		SoundBuffer* outputBuffer;
		
		// Skip NULL output buffers.
		if ( (outputBuffer = outputFrame.getBuffer(i)) == NULL )
			continue;
		
		inputBuffer->copyFormatTo( *outputBuffer, numSamples );
		inputBuffer->copyTo( *outputBuffer, numSamples );
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
