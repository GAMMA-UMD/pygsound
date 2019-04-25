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

#include "omSoundCutoffFilter.h"


#define PARAMETER_INDEX_TYPE 0
#define PARAMETER_INDEX_DIRECTION 1
#define PARAMETER_INDEX_ORDER 2
#define PARAMETER_INDEX_FREQUENCY 3
#define PARAMETER_INDEX_RIPPLE 4
#define PARAMETER_COUNT 5

#define PARAMETER_NAME_TYPE "Type"
#define PARAMETER_NAME_DIRECTION "Direction"
#define PARAMETER_NAME_ORDER "Order"
#define PARAMETER_NAME_FREQUENCY "Frequency"
#define PARAMETER_NAME_RIPPLE "Ripple"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String CutoffFilter:: NAME( "Cutoff Filter" );
const UTF8String CutoffFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion CutoffFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




CutoffFilter:: CutoffFilter()
	:	filterType( BUTTERWORTH ),
		filterDirection( HIGH_PASS ),
		filterOrder( 1 ),
		cornerFrequency( 0 ),
		ripple( 1 ),
		sampleRate( 0 )
{
}




CutoffFilter:: CutoffFilter( Type newFilterType, Direction newFilterDirection, Size newFilterOrder, Float newCornerFrequency )
	:	filterType( newFilterType ),
		filterDirection( newFilterDirection ),
		filterOrder( math::clamp( newFilterOrder, Size(1), MAXIMUM_FILTER_ORDER ) ),
		cornerFrequency( math::max( newCornerFrequency, Float(0) ) ),
		ripple( 1 ),
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




UTF8String CutoffFilter:: getName() const
{
	return NAME;
}




UTF8String CutoffFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion CutoffFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory CutoffFilter:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




Bool CutoffFilter:: allowsInPlaceProcessing() const
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




Size CutoffFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool CutoffFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_TYPE:
			info = FilterParameterInfo( PARAMETER_INDEX_TYPE, PARAMETER_NAME_TYPE,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(BUTTERWORTH), Int64(CHEBYSHEV_I), Int64(BUTTERWORTH),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_DIRECTION:
			info = FilterParameterInfo( PARAMETER_INDEX_DIRECTION, PARAMETER_NAME_DIRECTION,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(HIGH_PASS), Int64(LOW_PASS), Int64(HIGH_PASS),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_ORDER:
			info = FilterParameterInfo( PARAMETER_INDEX_ORDER, PARAMETER_NAME_ORDER,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										(Int64)1, (Int64)10, (Int64)1,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY, PARAMETER_NAME_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 1000.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_RIPPLE:
			info = FilterParameterInfo( PARAMETER_INDEX_RIPPLE, PARAMETER_NAME_RIPPLE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										0.1f, 12.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
	}
	
	return false;
}




Bool CutoffFilter:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_TYPE:
			if ( value.getValue( enumValue ) )
			{
				switch ( enumValue )
				{
					case BUTTERWORTH:			name = "Butterworth";			break;
					case LINKWITZ_RILEY:		name = "Linkwitz-Riley";		break;
					case CHEBYSHEV_I:			name = "Chebyshev Type I";		break;
					default:
						return false;
				}
				return true;
			}
			break;
		case PARAMETER_INDEX_DIRECTION:
			if ( value.getValue( enumValue ) )
			{
				switch ( enumValue )
				{
					case HIGH_PASS:		name = "High Pass";		break;
					case LOW_PASS:		name = "Low Pass";		break;
					default:
						return false;
				}
				return true;
			}
			break;
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




Bool CutoffFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_TYPE:
			value = FilterParameter( (Int64)this->getType() );
			return true;
		case PARAMETER_INDEX_DIRECTION:
			value = FilterParameter( (Int64)this->getDirection() );
			return true;
		case PARAMETER_INDEX_ORDER:
			value = FilterParameter( (Int64)this->getOrder() );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			value = FilterParameter( this->getFrequency() );
			return true;
		case PARAMETER_INDEX_RIPPLE:
			value = FilterParameter( this->getRipple() );
			return true;
	}
	
	return false;
}




Bool CutoffFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 enumValue;
	Float floatValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_TYPE:
			if ( value.getValue( enumValue ) &&
				(enumValue == BUTTERWORTH || enumValue == LINKWITZ_RILEY || enumValue == CHEBYSHEV_I) )
			{
				this->setType( (Type)enumValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_DIRECTION:
			if ( value.getValue( enumValue ) &&
				(enumValue == HIGH_PASS || enumValue == LOW_PASS) )
			{
				this->setDirection( (Direction)enumValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_ORDER:
			if ( value.getValue( enumValue ) )
			{
				this->setOrder( (Size)enumValue );
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
		case PARAMETER_INDEX_RIPPLE:
			if ( value.getValue( floatValue ) )
			{
				this->setRipple( floatValue );
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




void CutoffFilter:: resetStream()
{
	// Reset the channel histories.
	for ( Index i = 0; i < filterSections.getSize(); i++ )
	{
		Array<ChannelHistory>& histories = filterSections[i].channelHistory;
		
		for ( Index c = 0; c < histories.getSize(); c++ )
		{
			ChannelHistory& history = histories[c];
			
			history.reset();
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult CutoffFilter:: processFrame( const SoundFrame& inputFrame,
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
	
	//******************************************************************************
	
	const Float nyquistFrequency = 0.5f*Float(inputSampleRate);
	
	// If the corner frequency is at either end of the spectrum, avoid applying
	// the filter and either pass through the input or zero the output.
	if ( filterDirection == HIGH_PASS )
	{
		if ( cornerFrequency <= math::epsilon<Float>() )
		{
			inputBuffer->copyTo( *outputBuffer, numSamples );
			return numSamples;
		}
		else if ( cornerFrequency >= nyquistFrequency )
		{
			outputBuffer->zero( 0, numSamples );
			return numSamples;
		}
	}
	else if ( filterDirection == LOW_PASS )
	{
		if ( cornerFrequency >= nyquistFrequency )
		{
			inputBuffer->copyTo( *outputBuffer, numSamples );
			return numSamples;
		}
		else if ( cornerFrequency <= math::epsilon<Float>() )
		{
			outputBuffer->zero( 0, numSamples );
			return numSamples;
		}
	}
	
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
	
	switch ( filterType )
	{
		// Use a special cascaded processing method for LR filters.
		case LINKWITZ_RILEY:
			processLinkwitzRiley( *inputBuffer, *outputBuffer, numSamples );
			break;
		
		default:
			processFilterCascade( *inputBuffer, *outputBuffer, numSamples, filterOrder, filterSections );
			break;
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Generic Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




void CutoffFilter:: processFilterCascade( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
										Size filterOrder, SecondOrderFilter* filterSections )
{
	// Compute the total number of cascaded second order sections (order / 2)
	Size num2ndOrderSections = filterOrder >> 1;
	
	// Determine if there is a single 1st order section (only for odd orders)
	Bool has1stOrderSection = (num2ndOrderSections << 1) != filterOrder;
	
	// Get the total channel count.
	const Size numChannels = inputBuffer.getChannelCount();
	
	//*************************************************************************
	// Process each cascaded filter.
	
	Index filterIndex = 0;
	
	for ( ; filterIndex < num2ndOrderSections; filterIndex++ )
	{
		SecondOrderFilter& filter = filterSections[filterIndex];
		const SoundBuffer& currentInput = filterIndex == 0 ? inputBuffer : outputBuffer;
		
		// Make sure that the channel count for the filter's history is correct.
		if ( filter.channelHistory.getSize() < numChannels )
			filter.channelHistory.setSize( numChannels );
		
		// Process each channel of this second order section.
		for ( Index c = 0; c < numChannels; c++ )
		{
			process2ndOrderFilter( currentInput.getChannel(c), outputBuffer.getChannel(c), numSamples,
									filter.a, filter.b, filter.channelHistory[c].inputHistory,
									filter.channelHistory[c].outputHistory );
		}
	}
	
	// Process the final 1st order filter if applicable.
	if ( has1stOrderSection )
	{
		SecondOrderFilter& filter = filterSections[filterIndex];
		const SoundBuffer& currentInput = filterIndex == 0 ? inputBuffer : outputBuffer;
		
		// Make sure that the channel count for the filter's history is correct.
		if ( filter.channelHistory.getSize() < numChannels )
			filter.channelHistory.setSize( numChannels );
		
		// Process each channel of this first order section.
		for ( Index c = 0; c < numChannels; c++ )
		{
			process1stOrderFilter( currentInput.getChannel(c), outputBuffer.getChannel(c), numSamples,
									filter.a, filter.b, filter.channelHistory[c].inputHistory,
									filter.channelHistory[c].outputHistory );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Linkwitz-Riley Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




void CutoffFilter:: processLinkwitzRiley( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples )
{
	// Compute the order of the butterworth filters that are necessary
	// to produce a linkwitz riley filter with this order.
	Size butterworthOrder = (filterOrder >> 1) + (filterOrder % 2);
	
	// Compute the total number of cascaded second order sections per butterworth filter.
	Size num2ndOrderSections = butterworthOrder >> 1;
	
	// Compute the total number of filters that are needed for each butterworth filter.
	Size numButterworthSections = num2ndOrderSections + (butterworthOrder - (num2ndOrderSections << 1));
	
	//*************************************************************************
	// Compute the butterworth filters that makes up this linkwitz riley filter.
	
	processFilterCascade( inputBuffer, outputBuffer, numSamples, butterworthOrder, filterSections );
	processFilterCascade( outputBuffer, outputBuffer, numSamples, butterworthOrder, &filterSections[numButterworthSections] );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specific Order Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




void CutoffFilter:: process1stOrderFilter( const Sample32f* input, Sample32f* output, Size numSamples,
											const Float a[2], const Float b[1],
											Float inputHistory[1], Float outputHistory[1] )
{
	const Sample32f* inputEnd = input + numSamples;
	
	while ( input != inputEnd )
	{
		Float in = a[0]*(*input);
		
		*output = in + a[1]*inputHistory[0] - b[0]*outputHistory[0];
		
		// Update the history information
		inputHistory[0] = in;
		outputHistory[0] = *output;
		
		input++;
		output++;
	}
}




void CutoffFilter:: process2ndOrderFilter( const Sample32f* input, Sample32f* output, Size numSamples,
											const Float a[3], const Float b[2],
											Float inputHistory[2], Float outputHistory[2] )
{
	const Sample32f* inputEnd = input + numSamples;
	
	while ( input != inputEnd )
	{
		Float in = a[0]*(*input);
		
		*output = (in - b[0]*outputHistory[0]) + (a[1]*inputHistory[0] - b[1]*outputHistory[1]) + a[2]*inputHistory[1];
		
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




void CutoffFilter:: recalculateCoefficients()
{
	if ( filterType == LINKWITZ_RILEY )
	{
		// Compute the order of the butterworth filters that are necessary
		// to produce a linkwitz riley filter with this order.
		Size butterworthOrder = (filterOrder >> 1) + (filterOrder % 2);
		
		// Compute the total number of cascaded second order sections per butterworth filter.
		Size num2ndOrderSections = butterworthOrder >> 1;
		
		// Compute the total number of filters that are needed for each butterworth filter.
		Size numButterworthSections = num2ndOrderSections + (butterworthOrder - (num2ndOrderSections << 1));
		Size totalNumFilters = 2*numButterworthSections;
		
		// Make sure the array of filters is large enough.
		if ( filterSections.getSize() < totalNumFilters )
			filterSections.setSize( totalNumFilters );
		
		// Compute the Butterworth filter coefficients directly for the first butterworth filter.
		getButterworthCoefficients( filterDirection, butterworthOrder, cornerFrequency, sampleRate, filterSections );
		
		// Compute the Butterworth filter coefficients directly for the second butterworth filter.
		getButterworthCoefficients( filterDirection, butterworthOrder, cornerFrequency, sampleRate,
									&filterSections[numButterworthSections] );
	}
	else
	{
		// Compute the total number of cascaded second order sections (order / 2)
		Size num2ndOrderSections = filterOrder >> 1;
		
		// Compute the total number of filters that are needed.
		Size totalNumFilters = num2ndOrderSections + (filterOrder - (num2ndOrderSections << 1));
		
		// Make sure the array of filters is large enough.
		if ( filterSections.getSize() < totalNumFilters )
			filterSections.setSize( totalNumFilters );
		
		switch ( filterType )
		{
			case BUTTERWORTH:
				getButterworthCoefficients( filterDirection, filterOrder, cornerFrequency, sampleRate, filterSections );
				break;
			
			case CHEBYSHEV_I:
				getChebyshev1Coefficients( filterDirection, filterOrder, cornerFrequency, ripple, sampleRate, filterSections );
				break;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Butterworth Filter Coefficient Generation Method
//############		
//##########################################################################################
//##########################################################################################




void CutoffFilter:: getButterworthCoefficients( Direction direction, Size order, Float cornerFrequency,
												SampleRate sampleRate, SecondOrderFilter* filterSections )
{
	// Compute the total number of cascaded second order sections (order / 2)
	Size num2ndOrderSections = order >> 1;
	
	// Determine if there is a single 1st order section (only for odd orders)
	Bool has1stOrderSection = (num2ndOrderSections << 1) != order;
	
	// Sanitize the corner frequency so that it is between 0 and the nyquist frequency.
	Float frequencyRatio = math::clamp( Float(cornerFrequency / sampleRate),
										Float(0), Float(0.499) );
	
	// Compute the constant w0 that is the same for all filters.
	Float w0;
	
	if ( direction == HIGH_PASS )
		w0 = math::tan( math::pi<Float>()*frequencyRatio );
	else
		w0 = 1 / math::tan( math::pi<Float>()*frequencyRatio );
	
	//*************************************************************************
	// Compute the coefficients of the second order sections.
	
	Index filterIndex = 0;
	Float orderMinusOne = Float(order - 1);
	Float orderTimesTwo = Float(2)*order;
	
	for ( ; filterIndex < num2ndOrderSections; filterIndex++ )
	{
		SecondOrderFilter& filter = filterSections[filterIndex];
		/*
		2nd order coefficients:
			A = B0 + B1*w0 + B2*w0^2
			a0 = (A0 + A1*w0 + A2*w0^2) / A
			a1 = 2*(A0 - A2*w0^2) / A
			a2 = (A0 - A1*w0 + A2*w0^2) / A
			b0 = 2*(B0 - B2*w0^2) / A
			b1 = (B0 - B1*w0 + B2*w0^2) / A
			
			where for Butterworth filters:
			A0 = B0 = B2 = 1
			A1 = A2 = 0
			B1 = ?
		*/
		
		// Compute the linear coefficient of the butterworth polynomial factor (s^2 + B*s + 1)
		Float B1 = -2.0f * math::cos( math::pi<Float>()*Float(2*(filterIndex + 1) + orderMinusOne)/orderTimesTwo );
		Float w0squared = w0*w0;
		Float A = 1 + B1*w0 + w0squared;
		
		filter.a[0] = 1 / A;
		filter.a[1] = 2;
		filter.a[2] = 1;
		filter.b[0] = 2*(1 - w0squared)*filter.a[0];
		filter.b[1] = (1 - B1*w0 + w0squared)*filter.a[0];
	}
	
	//*************************************************************************
	// Compute the coefficients of the first order section (if applicable)
	
	if ( has1stOrderSection )
	{
		SecondOrderFilter& filter = filterSections[filterIndex];
		/*
		1st order coefficients:
			A = B0 + B1*w0
			a0 = (A0 + A1*w0) / A
			a1 = (A0 - A1*w0) / A
			b0 = (B0 - B1*w0) / A
			
			where for Butterworth filters:
			A0 = B0 = B1 = 1
			A1 = 0
		*/
		
		Float A = 1 + w0;
		
		filter.a[0] = 1 / A;
		filter.a[1] = 1;
		filter.b[0] = (1 - w0)*filter.a[0];
	}
	
	//*************************************************************************
	// Negate the odd-numbered coefficients of each filter if this is a high pass filter.
	
	if ( direction == HIGH_PASS )
	{
		Size totalNumFilters = num2ndOrderSections + has1stOrderSection;
		
		for ( Index i = 0; i < totalNumFilters; i++ )
		{
			SecondOrderFilter& filter = filterSections[i];
			filter.a[1] = -filter.a[1];
			filter.b[0] = -filter.b[0];
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Chebyshev Filter Coefficient Generation Method
//############		
//##########################################################################################
//##########################################################################################




void CutoffFilter:: getChebyshev1Coefficients( Direction direction, Size order, Float cornerFrequency,
											Float ripple, SampleRate sampleRate, SecondOrderFilter* filterSections )
{
	// Compute the total number of cascaded second order sections (order / 2)
	Size num2ndOrderSections = order >> 1;
	
	// Determine if there is a single 1st order section (only for odd orders)
	Bool has1stOrderSection = (num2ndOrderSections << 1) != order;
	
	// Sanitize the corner frequency so that it is between 0 and the nyquist frequency.
	cornerFrequency = math::clamp( cornerFrequency, math::epsilon<Float>(),
									Float(sampleRate)/Float(2 + math::epsilon<Float>()) );
	
	// Compute the constant w0 that is the same for all filters.
	Float w0;
	
	if ( direction == HIGH_PASS )
		w0 = math::tan( math::pi<Float>()*Float(cornerFrequency / sampleRate) );
	else
		w0 = 1 / math::tan( math::pi<Float>()*Float(cornerFrequency / sampleRate) );
	
	// Compute the parameter h that is the same for all filters
	Float e = math::sqrt( math::pow( Float(10), ripple / Float(10) ) - Float(1) );
	Float h = math::tanh( math::asinh(Float(1) / e) / Float(order) );
	
	//*************************************************************************
	// Compute the coefficients of the second order sections.
	
	Index filterIndex = 0;
	
	for ( ; filterIndex < num2ndOrderSections; filterIndex++ )
	{
		SecondOrderFilter& filter = filterSections[filterIndex];
		/*
		2nd order coefficients:
			A = B0 + B1*w0 + B2*w0^2
			a0 = (A0 + A1*w0 + A2*w0^2) / A
			a1 = 2*(A0 - A2*w0^2) / A
			a2 = (A0 - A1*w0 + A2*w0^2) / A
			b0 = 2*(B0 - B2*w0^2) / A
			b1 = (B0 - B1*w0 + B2*w0^2) / A
			
			where for Chebyshev filters:
			A0 = B0 = 1
			A1 = A2 = 0
			B1 = ?
			B2 = ?
		*/
		
		Float oi = math::pi<Float>()*Float(2*(filterIndex + 1) - 1)/Float(2*order);
		Float ai = math::sqrt( 1/(1 - h*h) - math::square(math::sin(oi)) );
		Float bi = Float(0.5)*math::sqrt( Float(1) + Float(1)/(h*h*math::square(math::tan(oi))) );
		
		// Compute the coefficients of the terms of the chebyshev polynomial.
		Float B1 = Float(1) / (ai*bi);
		Float B2 = Float(1) / (ai*ai);
		
		Float w0squared = w0*w0;
		Float A = 1 + B1*w0 + B2*w0squared;
		
		filter.a[0] = 1 / A;
		filter.a[1] = 2;
		filter.a[2] = 1;
		filter.b[0] = 2*(1 - B2*w0squared)*filter.a[0];
		filter.b[1] = (1 - B1*w0 + B2*w0squared)*filter.a[0];
	}
	
	//*************************************************************************
	// Compute the coefficients of the first order section (if applicable)
	
	if ( has1stOrderSection )
	{
		SecondOrderFilter& filter = filterSections[filterIndex];
		/*
		1st order coefficients:
			A = B0 + B1*w0
			a0 = (A0 + A1*w0) / A
			a1 = (A0 - A1*w0) / A
			b0 = (B0 - B1*w0) / A
			
			where for Butterworth filters:
			A0 = B0 = B1 = 1
			A1 = 0
		*/
		
		Float ai = h / math::sqrt( 1 - h*h );
		Float B1 = 1 / ai;
		
		Float A = 1 + B1*w0;
		
		filter.a[0] = 1 / A;
		filter.a[1] = 1;
		filter.b[0] = (1 - B1*w0)*filter.a[0];
	}
	
	//*************************************************************************
	// Negate the odd-numbered coefficients of each filter if this is a high pass filter.
	
	if ( direction == HIGH_PASS )
	{
		Size totalNumFilters = num2ndOrderSections + has1stOrderSection;
		
		for ( Index i = 0; i < totalNumFilters; i++ )
		{
			SecondOrderFilter& filter = filterSections[i];
			filter.a[1] = -filter.a[1];
			filter.b[0] = -filter.b[0];
		}
	}
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
