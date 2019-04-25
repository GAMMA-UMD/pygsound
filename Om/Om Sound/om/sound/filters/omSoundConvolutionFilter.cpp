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

#include "omSoundConvolutionFilter.h"


#define PARAMETER_COUNT 0


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String ConvolutionFilter:: NAME( "Convolution Filter" );
const UTF8String ConvolutionFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion ConvolutionFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ConvolutionFilter:: ConvolutionFilter()
	:	SoundFilter( 1, 1 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String ConvolutionFilter:: getName() const
{
	return NAME;
}




UTF8String ConvolutionFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion ConvolutionFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory ConvolutionFilter:: getCategory() const
{
	return FilterCategory::UTILITY;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size ConvolutionFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool ConvolutionFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	/*
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_SHIFT:
			info = FilterParameterInfo( PARAMETER_INDEX_SHIFT, PARAMETER_NAME_SHIFT,
										FilterParameterType::FLOAT,
										FilterParameterUnits::SEMITONES, FilterParameterCurve::LINEAR,
										Float(-24), Float(24), Float(0),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
	}
	*/
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Value Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ConvolutionFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{/*
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_SHIFT:
			value = FilterParameter( this->getSemitoneShift() );
			return true;
	}
	*/
	return false;
}




Bool ConvolutionFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	//Float floatValue;
	//Int64 intValue;
	/*
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_SHIFT:
			if ( value.getValue( floatValue ) )
			{
				this->setSemitoneShift( floatValue );
				return true;
			}
			break;
	}
	*/
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Method
//############		
//##########################################################################################
//##########################################################################################




void ConvolutionFilter:: resetStream()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Impulse Response Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ConvolutionFilter:: setImpulseResponse( const SoundBuffer& newIR )
{
	// Don't accept bad impulse responses.
	if ( newIR.getChannelCount() == 0 || newIR.getSize() == 0 || newIR.getSampleRate() == 0 )
		return false;
	
	// Lock the synchronization mutex.
	lockMutex();
	
	
	
	// Unlock the synchronization mutex.
	unlockMutex();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult ConvolutionFilter:: processFrame( const SoundFrame& inputFrame,
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
	/*
	// If the IR is not initialized, pass through sound from input to output.
	if ( fdls.getSize() == 0 )
	{
		inputBuffer->copyTo( *outputBuffer, numSamples );
		return numSamples;
	}
	
	//******************************************************************************
	
	const SampleRate sampleRate = inputBuffer->getSampleRate();
	
	// Reset parameter interpolation if this is the first processing frame or the sample rate changed.
	if ( isFirstFrame() || lastSampleRate != sampleRate )
		ConvolutionFilter::resetStream();
	
	lastSampleRate = sampleRate;
	
	//******************************************************************************
	
	//renderConvolution( *inputBuffer, *outputBuffer, numSamples );
	*/
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
