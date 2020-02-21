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

#include "omSoundBandFilter.h"


#define PARAMETER_INDEX_TYPE 0
#define PARAMETER_INDEX_DIRECTION 1
#define PARAMETER_INDEX_ORDER 2
#define PARAMETER_INDEX_FREQUENCY_1 3
#define PARAMETER_INDEX_FREQUENCY_2 4
#define PARAMETER_INDEX_RIPPLE 5
#define PARAMETER_COUNT 6

#define PARAMETER_NAME_TYPE "Type"
#define PARAMETER_NAME_DIRECTION "Direction"
#define PARAMETER_NAME_ORDER "Order"
#define PARAMETER_NAME_FREQUENCY_1 "Frequency 1"
#define PARAMETER_NAME_FREQUENCY_2 "Frequency 2"
#define PARAMETER_NAME_RIPPLE "Ripple"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String BandFilter:: NAME( "Band Filter" );
const UTF8String BandFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion BandFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




BandFilter:: BandFilter()
	:	filterDirection( BAND_PASS ),
		highPass( CutoffFilter::BUTTERWORTH, CutoffFilter::HIGH_PASS, 1, 0.0f ),
		lowPass( CutoffFilter::BUTTERWORTH, CutoffFilter::LOW_PASS, 1, 20000.0f ),
		frequency1( 0.0f ),
		frequency2( 20000.0f )
{
	highPass.setIsSynchronized( false );
	lowPass.setIsSynchronized( false );
}




BandFilter:: BandFilter( Type newFilterType, Direction newFilterDirection, Size newFilterOrder,
						Float newFrequency1, Float newFrequency2 )
	:	filterDirection( newFilterDirection ),
		highPass( convertFilterType(newFilterType), CutoffFilter::HIGH_PASS, newFilterOrder, 0.0f ),
		lowPass( convertFilterType(newFilterType), CutoffFilter::LOW_PASS, newFilterOrder, 20000.0f ),
		frequency1( newFrequency1 ),
		frequency2( newFrequency2 )
{
	highPass.setIsSynchronized( false );
	lowPass.setIsSynchronized( false );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String BandFilter:: getName() const
{
	return NAME;
}




UTF8String BandFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion BandFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory BandFilter:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




Bool BandFilter:: allowsInPlaceProcessing() const
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




Size BandFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool BandFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
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
										Int64(BAND_PASS), Int64(BAND_REJECT), Int64(BAND_PASS),
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
		case PARAMETER_INDEX_FREQUENCY_1:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY_1, PARAMETER_NAME_FREQUENCY_1,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 1000.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FREQUENCY_2:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY_2, PARAMETER_NAME_FREQUENCY_2,
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




Bool BandFilter:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
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
					case BAND_PASS:		name = "Band Pass";		break;
					case BAND_REJECT:	name = "Band Reject";	break;
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




Bool BandFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
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
		case PARAMETER_INDEX_FREQUENCY_1:
			value = FilterParameter( this->getFrequency1() );
			return true;
		case PARAMETER_INDEX_FREQUENCY_2:
			value = FilterParameter( this->getFrequency2() );
			return true;
		case PARAMETER_INDEX_RIPPLE:
			value = FilterParameter( this->getRipple() );
			return true;
	}
	
	return false;
}




Bool BandFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
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
			if ( value.getValue( enumValue ) )
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
		case PARAMETER_INDEX_FREQUENCY_1:
			if ( value.getValue( floatValue ) )
			{
				this->setFrequency1( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_FREQUENCY_2:
			if ( value.getValue( floatValue ) )
			{
				this->setFrequency2( floatValue );
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




void BandFilter:: resetStream()
{
	// Reset the cutoff filters.
	highPass.reset();
	lowPass.reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult BandFilter:: processFrame( const SoundFrame& inputFrame,
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
	// Determine the correct order of the frequencies.
	
	Float lowFrequency;
	Float highFrequency;
	
	if ( frequency1 <= frequency2 )
	{
		lowFrequency = frequency1;
		highFrequency = frequency2;
	}
	else
	{
		lowFrequency = frequency2;
		highFrequency = frequency1;
	}
	
	//*************************************************************************
	// Apply the high pass and low pass filters that make up this band filter.
	
	if ( filterDirection == BAND_PASS )
	{
		// Make sure that the high pass filter has the low frequency and the
		// low pass filter has the high frequency.
		if ( highPass.getFrequency() != lowFrequency )
			highPass.setFrequency( lowFrequency );
		
		if ( lowPass.getFrequency() != highFrequency )
			lowPass.setFrequency( highFrequency );
		
		// Process the filters in series to produce the band-limited output.
		highPass.process( *inputBuffer, *outputBuffer, numSamples );
		lowPass.process( *outputBuffer, *outputBuffer, numSamples );
	}
	else
	{
		// Make sure that the high pass filter has the high frequency and the
		// low pass filter has the low frequency.
		if ( highPass.getFrequency() != highFrequency )
			highPass.setFrequency( highFrequency );
		
		if ( lowPass.getFrequency() != lowFrequency )
			lowPass.setFrequency( lowFrequency );
		
		// Get a shared temporary sound buffer to use to mix the output.
		SharedSoundBuffer sharedBuffer = SharedBufferPool::getGlobalBuffer( inputBuffer->getChannelCount(),
																		numSamples, inputBuffer->getSampleRate() );
		
		// Process the filters in series to produce the band-limited output.
		highPass.process( *inputBuffer, sharedBuffer.getBuffer(), numSamples );
		lowPass.process( *inputBuffer, *outputBuffer, numSamples );
		
		// Mix the shared buffer to the output buffer.
		sharedBuffer.getBuffer().mixTo( *outputBuffer );
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
