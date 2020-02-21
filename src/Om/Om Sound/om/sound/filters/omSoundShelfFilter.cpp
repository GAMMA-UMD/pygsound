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

#include "omSoundShelfFilter.h"


#define PARAMETER_INDEX_DIRECTION 0
#define PARAMETER_INDEX_FREQUENCY 1
#define PARAMETER_INDEX_SLOPE 2
#define PARAMETER_INDEX_GAIN 3
#define PARAMETER_COUNT 4

#define PARAMETER_NAME_DIRECTION "Direction"
#define PARAMETER_NAME_FREQUENCY "Frequency"
#define PARAMETER_NAME_SLOPE "Slope"
#define PARAMETER_NAME_GAIN "Gain"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String ShelfFilter:: NAME( "Shelf Filter" );
const UTF8String ShelfFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion ShelfFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ShelfFilter:: ShelfFilter()
	:	filterDirection( LOW_SHELF ),
		cornerFrequency( 0 ),
		slope( 1 ),
		gain( 1 ),
		sampleRate( 0 )
{
}




ShelfFilter:: ShelfFilter( Direction newFilterDirection, Float newCornerFrequency, Gain newGain )
	:	filterDirection( newFilterDirection ),
		cornerFrequency( math::max( newCornerFrequency, Float(0) ) ),
		slope( 1 ),
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




UTF8String ShelfFilter:: getName() const
{
	return NAME;
}




UTF8String ShelfFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion ShelfFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory ShelfFilter:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




Bool ShelfFilter:: allowsInPlaceProcessing() const
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




Size ShelfFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool ShelfFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_DIRECTION:
			info = FilterParameterInfo( PARAMETER_INDEX_DIRECTION, PARAMETER_NAME_DIRECTION,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(LOW_SHELF), Int64(HIGH_SHELF), Int64(LOW_SHELF),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY, PARAMETER_NAME_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 1000.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_SLOPE:
			info = FilterParameterInfo( PARAMETER_INDEX_SLOPE, PARAMETER_NAME_SLOPE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LOGARITHMIC,
										0.1f, 10.0f, 1.0f,
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




Bool ShelfFilter:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	if ( parameterIndex == PARAMETER_INDEX_DIRECTION && value.getValue( enumValue ) )
	{
		switch ( enumValue )
		{
			case LOW_SHELF:		name = "Low Shelf";		break;
			case HIGH_SHELF:	name = "High Shelf";	break;
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




Bool ShelfFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_DIRECTION:
			value = FilterParameter( (Int64)this->getDirection() );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			value = FilterParameter( this->getFrequency() );
			return true;
		case PARAMETER_INDEX_SLOPE:
			value = FilterParameter( this->getSlope() );
			return true;
		case PARAMETER_INDEX_GAIN:
			value = FilterParameter( this->getGainDB() );
			return true;
	}
	
	return false;
}




Bool ShelfFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 enumValue;
	Gain gainValue;
	Float floatValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_DIRECTION:
			if ( value.getValue( enumValue ) &&
				(enumValue == LOW_SHELF || enumValue == HIGH_SHELF) )
			{
				this->setDirection( (Direction)enumValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_SLOPE:
			if ( value.getValue( floatValue ) )
			{
				this->setSlope( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setGainDB( gainValue );
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




void ShelfFilter:: resetStream()
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




SoundResult ShelfFilter:: processFrame( const SoundFrame& inputFrame,
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
	
	// If the gain is very close to zero, don't apply a filter and pass through the input to output.
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




void ShelfFilter:: process2ndOrderFilter( const Sample32f* input, Sample32f* output, Size numSamples,
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




void ShelfFilter:: recalculateCoefficients()
{
	// Sanitize the corner frequency so that it is between 0 and the nyquist frequency.
	Float frequencyRatio = math::clamp( Float(cornerFrequency / sampleRate),
										Float(0), Float(0.4999) );
	
	// Compute the constant w0 that is the same for all filters.
	Float w0 = 1 / math::tan( math::pi<Float>()*frequencyRatio );
	
	Float g = filterDirection == LOW_SHELF ? gain : 1/gain;
	
	//*************************************************************************
	
	/*
	Transfer function for a 2nd order shelving filter:
		
		H(s) = (A + G*s + s^2) / (1 + G*s + A*s^2)
		
		where:
		A = sqrt( linear gain )
		G = A / Q
		Q = 'slope' * sqrt( linear gain )
		
	2nd order coefficients:
		B = B0 + B1*w0 + B2*w0^2
		a0 = (A0 + A1*w0 + A2*w0^2) / B
		a1 = 2*(A0 - A2*w0^2) / B
		a2 = (A0 - A1*w0 + A2*w0^2) / B
		b0 = 2*(B0 - B2*w0^2) / B
		b1 = (B0 - B1*w0 + B2*w0^2) / B
	*/
	
	Float A = math::sqrt( g );
	Float scale = math::sqrt( gain );
	Float Q = slope*scale;
	Float G = A / Q;
	Float w0squared = w0*w0;
	Float invB = 1 / (1 + G*w0 + A*w0squared);
	
	a[0] = scale*(A + G*w0 + w0squared) * invB;
	a[1] = scale*2*(A - w0squared) * invB;
	a[2] = scale*(A - G*w0 + w0squared) * invB;
	b[0] = 2*(1 - A*w0squared) * invB;
	b[1] = (1 - G*w0 + A*w0squared) * invB;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
