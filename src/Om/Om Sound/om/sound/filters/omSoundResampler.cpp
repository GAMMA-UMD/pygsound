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

#include "omSoundResampler.h"


#include "omSoundCutoffFilter.h"


#define PARAMETER_INDEX_CONVERSION_TYPE 0
#define PARAMETER_INDEX_OUTPUT_SAMPLE_RATE 1
#define PARAMETER_COUNT 2

#define PARAMETER_NAME_CONVERSION_TYPE "Conversion Type"
#define PARAMETER_NAME_OUTPUT_SAMPLE_RATE "Output Sample Rate"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Resampler:: NAME( "Resampler" );
const UTF8String Resampler:: MANUFACTURER( "Om Sound" );
const FilterVersion Resampler:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Resampler:: Resampler()
	:	SoundFilter( 1, 1 ),
		conversionType( BEST ),
		outputSampleRate( 44100 ),
		interpolationSampleOffset( 0 ),
		lowPass( NULL )
{
}




Resampler:: Resampler( Type newConversionType )
	:	SoundFilter( 1, 1 ),
		conversionType( newConversionType ),
		outputSampleRate( 44100 ),
		interpolationSampleOffset( 0 ),
		lowPass( NULL )
{
}




Resampler:: Resampler( Type newConversionType, SampleRate newOutputSampleRate )
	:	SoundFilter( 1, 1 ),
		conversionType( newConversionType ),
		outputSampleRate( newOutputSampleRate ),
		interpolationSampleOffset( 0 ),
		lowPass( NULL )
{
}




Resampler:: Resampler( const Resampler& other )
	:	SoundFilter( 1, 1 ),
		conversionType( other.conversionType ),
		outputSampleRate( other.outputSampleRate ),
		interpolationSampleOffset( other.interpolationSampleOffset ),
		lowPass( NULL )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Resampler:: ~Resampler()
{
	if ( lowPass != NULL )
		om::util::destruct( lowPass );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Resampler& Resampler:: operator = ( const Resampler& other )
{
	if ( this != &other )
	{
		conversionType = other.conversionType;
		outputSampleRate = other.outputSampleRate;
		interpolationSampleOffset = other.interpolationSampleOffset;
		
		// Low pass filter doesn't need to be copied because it is the same in both objects.
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Resampler:: getName() const
{
	return NAME;
}




UTF8String Resampler:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Resampler:: getVersion() const
{
	return VERSION;
}




FilterCategory Resampler:: getCategory() const
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




Size Resampler:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Resampler:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_CONVERSION_TYPE:
			info = FilterParameterInfo( PARAMETER_INDEX_CONVERSION_TYPE, PARAMETER_NAME_CONVERSION_TYPE,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(BEST), Int64(INTERPOLATE_FILTERED), Int64(BEST),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_OUTPUT_SAMPLE_RATE:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_SAMPLE_RATE, PARAMETER_NAME_OUTPUT_SAMPLE_RATE,
										FilterParameterType::DOUBLE,
										FilterParameterUnits::SAMPLE_RATE, FilterParameterCurve::LINEAR,
										0.0, 192000.0, 44100.0,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
	}
	
	return false;
}




Bool Resampler:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	if ( parameterIndex == PARAMETER_INDEX_CONVERSION_TYPE && value.getValue( enumValue ) )
	{
		switch ( enumValue )
		{
			case BEST:					name = "Best";						break;
			case FASTEST:				name = "Fastest";					break;
			case INTERPOLATE:			name = "Interpolation";				break;
			case INTERPOLATE_FILTERED:	name = "Filtered Interpolation";	break;
			default:
				return false;
		}
		
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




Bool Resampler:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_CONVERSION_TYPE:
			value = FilterParameter( FilterParameterType::ENUMERATION, FilterParameterValue((Int64)this->getType()) );
			return true;
		case PARAMETER_INDEX_OUTPUT_SAMPLE_RATE:
			value = FilterParameter( this->getOutputSampleRate() );
			return true;
	}
	
	return false;
}




Bool Resampler:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 enumValue;
	SampleRate sampleRateValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_CONVERSION_TYPE:
			if ( value.getValue( enumValue ) &&
				(enumValue == BEST || enumValue == FASTEST || enumValue == INTERPOLATE ||
				enumValue == INTERPOLATE_FILTERED) )
			{
				this->setType( (Type)enumValue );
				return true;
			}
			else
				return false;
		case PARAMETER_INDEX_OUTPUT_SAMPLE_RATE:
			if ( value.getValue( sampleRateValue ) )
			{
				this->setOutputSampleRate( sampleRateValue );
				return true;
			}
			else
				return false;
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




void Resampler:: resetStream()
{
	// Reset the last input sample history.
	lastInputSample.setAll( Sample32f(0) );
	interpolationSampleOffset = 0;
	
	// Reset the low pass filter if it exists.
	if ( lowPass != NULL )
		lowPass->reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Resampler:: processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numInputSamples )
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
	if ( inputSampleRate <= SampleRate(0) || outputSampleRate <= SampleRate(0) )
		return SoundResult::ERROR;
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//******************************************************************************
	
	// Determine the maximum number of output samples that the output buffer should hold.
	Size numOutputSamples = (Size)math::ceiling( numInputSamples*(outputSampleRate / inputSampleRate) );
	
	// Make sure that the output buffer has the right size and format.
	inputBuffer->copyFormatTo( *outputBuffer, numOutputSamples );
	
	// Make sure that the output buffer has the correct output sample rate.
	outputBuffer->setSampleRate( outputSampleRate );
	
	//******************************************************************************
	
	// If the input and output sample rates are equal, copy the input buffer to the output buffer and return.
	// Don't do any conversion.
	if ( inputSampleRate == outputSampleRate )
	{
		inputBuffer->copyTo( *outputBuffer, numInputSamples );
		return numInputSamples;
	}
	
	// Determine which conversion type to use.
	switch ( conversionType )
	{
		case FASTEST:
		case INTERPOLATE:
		{
			// Do a linear interpolation of the samples in the buffers.
			numOutputSamples = interpolateBuffers( *inputBuffer, *outputBuffer, numInputSamples );
		}
		break;
		
		default:
		case BEST:
		case INTERPOLATE_FILTERED:
		{
			// Make sure that the low pass filter is instantiated.
			Float lowPassFrequency = inputSampleRate < outputSampleRate ? Float(0.5*inputSampleRate) : Float(0.5*outputSampleRate);
			
			if ( lowPass == NULL )
			{
				// Use a 16th order butterworth low pass filter.
				lowPass = om::util::construct<CutoffFilter>( CutoffFilter::BUTTERWORTH, CutoffFilter::LOW_PASS,
															16, lowPassFrequency );
				lowPass->setIsSynchronized( false );
			}
			else if ( lowPass->getFrequency() != lowPassFrequency )
				lowPass->setFrequency( lowPassFrequency );
			
			
			if ( inputSampleRate < outputSampleRate )
			{
				// Do a linear interpolation of the samples in the buffers.
				numOutputSamples = interpolateBuffers( *inputBuffer, *outputBuffer, numInputSamples );
				
				// Low pass filter the output
				numOutputSamples = lowPass->process( *outputBuffer, *outputBuffer, numOutputSamples );
			}
			else
			{
				// Get a temporary shared buffer which holds the output of the low pass filter.
				SharedSoundBuffer sharedBuffer = SharedBufferPool::getGlobalBuffer( inputBuffer->getChannelCount(),
																					numInputSamples, inputSampleRate );
				
				// Low pass filter the input
				numInputSamples = lowPass->process( *inputBuffer, sharedBuffer.getBuffer(), numInputSamples );
				
				// Do a linear interpolation of the samples in the buffers.
				numOutputSamples = interpolateBuffers( sharedBuffer.getBuffer(), *outputBuffer, numInputSamples );
			}
			
			break;
		}
	}
	
	return numOutputSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Linear Sample Rate Conversion Method
//############		
//##########################################################################################
//##########################################################################################




Size Resampler:: interpolateBuffers( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numInputSamples )
{
	// Compute how much the input pointer should advance (in floating point units) for each output sample.
	Float inputIncrement = Float(inputBuffer.getSampleRate()/outputBuffer.getSampleRate());
	
	// Declare a variable to hold the number of output samples that is computed in the conversion loop.
	Size numOutputSamples;
	Size numChannels = inputBuffer.getChannelCount();
	
	// Make sure that the buffer that holds the last input sample (from the previous buffer frame)
	// has at least enough channels for this buffer frame.
	if ( lastInputSample.getSize() < numChannels )
	{
		// If not, increase the number of channels and zero-out the buffer.
		lastInputSample.setSize( numChannels, Sample32f(0) );
	}
	
	// The interpolation parameter. This is the position of the floating-point input pointer
	// between the last input sample and the next input sample (between 0 and 1).
	Float a;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		// Initialize loop invariants.
		const Sample32f* const inputStart = inputBuffer.getChannel(c);
		const Sample32f* input = inputStart;
		const Sample32f* const inputEnd = input + numInputSamples;
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputStart = output;
		
		// Get the last input sample for this channel. This is from the previous buffer processing frame.
		Sample32f lastInput = lastInputSample[c];
		
		// Get the leftover input sample offset from the previous frame.
		a = interpolationSampleOffset;
		
		// Advance the input pointer until the interpolation parameter lies between 0 and 1.
		while ( a >= Float(1) )
		{
			a -= Float(1);
			lastInput = *input;
			input++;
		}
		
		// Generate the interpolated samples.
		while ( input < inputEnd )
		{
			// Linearly interpolate between the last and current input samples.
			*output = (*input - lastInput)*a + lastInput;
			
			// Increment the output pointer and the interpolation paramter.
			output++;
			a += inputIncrement;
			
			// Advance the input pointer until the interpolation parameter lies between 0 and 1.
			while ( a >= Float(1) )
			{
				a -= Float(1);
				lastInput = *input;
				input++;
			}
		}
		
		// Test to see if the interpolation parameter needs to be increased.
		// This can happen when downsampling, when the input pointer is more than 1 sample
		// into the next frame's input buffer.
		if ( input > inputEnd )
			a += (input - inputEnd);
		
		// Save the last input sample from this channel for the next buffer processing frame.
		lastInputSample[c] = inputStart[numInputSamples - 1];
		
		// Compute the total number of output samples that were generated.
		numOutputSamples = output - outputStart;
	}
	
	// Store the remaining sample position offset for the next buffer processing frame.
	interpolationSampleOffset = a;
	
	return numOutputSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
