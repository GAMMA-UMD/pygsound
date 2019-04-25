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

#include "omSoundParametricFilter.h"

#define PARAMETER_INDEX_FREQUENCY 0
#define PARAMETER_INDEX_Q 1
#define PARAMETER_INDEX_BANDWIDTH 2
#define PARAMETER_INDEX_GAIN 3
#define PARAMETER_COUNT 4

#define PARAMETER_NAME_FREQUENCY "Frequency"
#define PARAMETER_NAME_Q "Q"
#define PARAMETER_NAME_BANDWIDTH "Bandwidth"
#define PARAMETER_NAME_GAIN "Gain"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String ParametricFilter:: NAME( "Parameteric Filter" );
const UTF8String ParametricFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion ParametricFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ParametricFilter:: ParametricFilter()
	:	centerFrequency( 1000 ),
		gain( 1 ),
		q( math::sqrt(2.0f) ),
		sampleRate( 0 )
{
}




ParametricFilter:: ParametricFilter( Float newCenterFrequency, Float newQ, Gain newGain )
	:	centerFrequency( math::max( newCenterFrequency, Float(0) ) ),
		q( math::max( newQ, Float(0) ) ),
		gain( math::max( newGain, Float(0) ) ),
		sampleRate( 0 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String ParametricFilter:: getName() const
{
	return NAME;
}




UTF8String ParametricFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion ParametricFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory ParametricFilter:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




Bool ParametricFilter:: allowsInPlaceProcessing() const
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




Size ParametricFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool ParametricFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY, PARAMETER_NAME_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 1000.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_Q:
			info = FilterParameterInfo( PARAMETER_INDEX_Q, PARAMETER_NAME_Q,
										FilterParameterType::FLOAT,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::SQUARE,
										0.05f, 30.0f, 1.414f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_BANDWIDTH:
			info = FilterParameterInfo( PARAMETER_INDEX_BANDWIDTH, PARAMETER_NAME_BANDWIDTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::OCTAVES, FilterParameterCurve::SQUARE,
										0.05f, 8.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_GAIN, PARAMETER_NAME_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 20.0f, 0.0f,
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




Bool ParametricFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FREQUENCY:
			value = FilterParameter( this->getFrequency() );
			return true;
		case PARAMETER_INDEX_Q:
			value = FilterParameter( this->getQ() );
			return true;
		case PARAMETER_INDEX_BANDWIDTH:
			value = FilterParameter( this->getBandwidth() );
			return true;
		case PARAMETER_INDEX_GAIN:
			value = FilterParameter( this->getGainDB() );
			return true;
	}
	
	return false;
}




Bool ParametricFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Float floatValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_Q:
			if ( value.getValue( floatValue ) )
			{
				this->setQ( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_BANDWIDTH:
			if ( value.getValue( floatValue ) )
			{
				this->setBandwidth( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_GAIN:
			if ( value.getValue( floatValue ) )
			{
				this->setGainDB( floatValue );
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




void ParametricFilter:: resetStream()
{
	// Reset the channel histories.
	for ( Index c = 0; c < channelHistory.getSize(); c++ )
	{
		ChannelHistory& history = channelHistory[c];
		
		history.inputHistory.setAll( Sample32f(0) );
		history.outputHistory.setAll( Sample32f(0) );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult ParametricFilter:: processFrame( const SoundFrame& inputFrame,
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
	
	//*************************************************************************
	
	// If the corner frequency is very close to zero, don't apply a filter and pass through the input to output.
	if ( math::abs(Gain(1) - gain) <= math::epsilon<Float>() )
	{
		inputBuffer->copyTo( *outputBuffer, numSamples );
		return numSamples;
	}
	
	//*************************************************************************
	// Update the number of channels in the history if necessary.
	
	const Size numChannels = inputBuffer->getChannelCount();
	
	if ( channelHistory.getSize() < numChannels )
		channelHistory.setSize( numChannels );
	
	//*************************************************************************
	// Recalculate filter coefficients if necessary.
	
	if ( sampleRate != inputSampleRate )
	{
		// Recalculate the filter coefficients using the new sample rate.
		sampleRate = inputSampleRate;
		recalculateCoefficients();
	}
	
	//*************************************************************************
	// Do the main filter processing.
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		process2ndOrderFilter( inputBuffer->getChannel(c), outputBuffer->getChannel(c), numSamples,
							a, b, channelHistory[c].inputHistory, channelHistory[c].outputHistory );
	}
	
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specific Order Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




void ParametricFilter:: process2ndOrderFilter( const Sample32f* input, Sample32f* output, Size numSamples,
											const Float* a, const Float* b,
											Sample32f* inputHistory, Sample32f* outputHistory )
{
	const Sample32f* inputEnd = input + numSamples;
	
	while ( input != inputEnd )
	{
		Sample32f in = (*input);
		
		*output = a[0]*in + a[1]*inputHistory[0] + a[2]*inputHistory[1]
					- b[0]*outputHistory[0] - b[1]*outputHistory[1];
		
		// Update the history information.
		inputHistory[1] = inputHistory[0];
		inputHistory[0] = in;
		
		outputHistory[1] = outputHistory[0];
		outputHistory[0] = *output;
		
		input++;
		output++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Coefficient Recalculation Method
//############		
//##########################################################################################
//##########################################################################################




void ParametricFilter:: recalculateCoefficients()
{
	// Sanitize the corner frequency so that it is between 0 and the nyquist frequency.
	Float frequencyRatio = math::clamp( Float(centerFrequency / sampleRate),
										Float(0), Float(0.4999) );
	
	// Compute the constant w0 that is the same for all filters.
	Float w0 = 1 / math::tan( math::pi<Float>()*frequencyRatio );
	
	//*************************************************************************
	
	/*
	Transfer function for a 2nd order parametric peaking filter:
		
		H(s) = (1 + (g/Q)*s + s^2) / (1 + (1/Q)*s + s^2)
		
		where:
		Q is the 'quality' factor of the filter.
		g is the linear gain factor of the filter.
		
	2nd order coefficients:
		B = B0 + B1*w0 + B2*w0^2
		a0 = (A0 + A1*w0 + A2*w0^2) / B
		a1 = 2*(A0 - A2*w0^2) / B
		a2 = (A0 - A1*w0 + A2*w0^2) / B
		b0 = 2*(B0 - B2*w0^2) / B
		b1 = (B0 - B1*w0 + B2*w0^2) / B
		
	so:
		A0 = B0 = A2 = B2 = 1
		A1 = g / Q
		B1 = 1 / Q
	*/
	
	Float w0squared = w0*w0;
	Float B1 = 1 / q;
	Float A1 = gain*B1;
	Float invB = 1 / (1 + B1*w0 + w0squared);
	
	a[0] = (1 + A1*w0 + w0squared) * invB;
	a[1] = 2*(1 - w0squared) * invB;
	a[2] = (1 - A1*w0 + w0squared) * invB;
	b[0] = 2*(1 - w0squared) * invB;
	b[1] = (1 - B1*w0 + w0squared) * invB;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
