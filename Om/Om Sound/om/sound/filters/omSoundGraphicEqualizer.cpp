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

#include "omSoundGraphicEqualizer.h"


#define PARAMETER_INDEX_OUTPUT_GAIN 0
#define PARAMETER_INDEX_FILTER_Q 1
#define PARAMETER_INDEX_FILTER_BANDWIDTH 2
#define PARAMETER_INDEX_FILTER_GAIN 3
#define PARAMETER_COUNT 34

#define PARAMETER_NAME_OUTPUT_GAIN "OutputGain"
#define PARAMETER_NAME_FILTER_Q "Filter Q"
#define PARAMETER_NAME_FILTER_BANDWIDTH "Filter Bandwidth"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String GraphicEqualizer:: NAME( "Graphic Equalizer" );
const UTF8String GraphicEqualizer:: MANUFACTURER( "Om Sound" );
const FilterVersion GraphicEqualizer:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




GraphicEqualizer:: GraphicEqualizer()
{
	// Initialize each filter with the correct frequency and bandwidth.
	for ( Index i = 0; i < filters.getSize(); i++ )
	{
		filters[i].setFrequency( FILTER_FREQUENCIES[i] );
		filters[i].setBandwidth( 1.0f/3.0f );
		
		// Disable thread synchronization for internal filters.
		filters[i].setIsSynchronized( false );
	}
	
	gainFilter.setIsSynchronized( false );
}



//##########################################################################################
//##########################################################################################
//############		
//############		Filter Gain Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void GraphicEqualizer:: setFlat()
{
	lockMutex();
	
	for ( Index i = 0; i < filters.getSize(); i++ )
		filters[i].setGain( 1 );
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Bandwidth Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void GraphicEqualizer:: setFilterQ( Float newQ )
{
	lockMutex();
	
	for ( Index i = 0; i < filters.getSize(); i++ )
		filters[i].setQ( newQ );
	
	unlockMutex();
}




void GraphicEqualizer:: setFilterBandwidth( Float newBandwidth )
{
	lockMutex();
	
	// Calculate the new Q first because this is the same for all filters and is expensive to compute.
	Float twoToTheB = math::pow( Float(2), math::max( newBandwidth, Float(0) ) );
	Float newQ = math::sqrt(twoToTheB) / (twoToTheB - 1);
	
	for ( Index i = 0; i < filters.getSize(); i++ )
		filters[i].setQ( newQ );
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String GraphicEqualizer:: getName() const
{
	return NAME;
}




UTF8String GraphicEqualizer:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion GraphicEqualizer:: getVersion() const
{
	return VERSION;
}




FilterCategory GraphicEqualizer:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




Bool GraphicEqualizer:: allowsInPlaceProcessing() const
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




Size GraphicEqualizer:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool GraphicEqualizer:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_GAIN, PARAMETER_NAME_OUTPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 20.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FILTER_Q:
			info = FilterParameterInfo( PARAMETER_INDEX_FILTER_Q, PARAMETER_NAME_FILTER_Q,
										FilterParameterType::FLOAT,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::SQUARE,
										1.414f, 30.0f, 4.3189f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FILTER_BANDWIDTH:
			info = FilterParameterInfo( PARAMETER_INDEX_FILTER_BANDWIDTH, PARAMETER_NAME_FILTER_BANDWIDTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::OCTAVES, FilterParameterCurve::SQUARE,
										0.05f, 1.0f, 1.0f/3.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		default:
			if ( parameterIndex >= PARAMETER_INDEX_FILTER_GAIN && parameterIndex < PARAMETER_COUNT )
			{
				Index filterIndex = parameterIndex - PARAMETER_INDEX_FILTER_GAIN;
				Float filterFrequency = filters[filterIndex].getFrequency();
				UTF8StringBuffer parameterNameBuffer;
				
				if ( filterFrequency >= 1000.0f )
				{
					Float kHz = filterFrequency/1000.0f;
					
					if ( math::floor(kHz) == kHz )
						parameterNameBuffer << UTF8String( kHz, 0 );
					else
						parameterNameBuffer << UTF8String( kHz, 2 );
					
					parameterNameBuffer << " kHz";
				}
				else
				{
					if ( math::floor(filterFrequency) == filterFrequency )
						parameterNameBuffer << UTF8String( filterFrequency, 0 );
					else
						parameterNameBuffer << UTF8String( filterFrequency, 1 );
					
					parameterNameBuffer << " Hz";
				}
				
				info = FilterParameterInfo( (UInt32)parameterIndex, parameterNameBuffer.toString(),
											FilterParameterType::FLOAT,
											FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
											-20.0f, 20.0f, 0.0f,
											FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
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




Bool GraphicEqualizer:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_GAIN:
			value = FilterParameter( this->getOutputGainDB() );
			return true;
		case PARAMETER_INDEX_FILTER_Q:
			value = FilterParameter( this->getFilterQ() );
			return true;
		case PARAMETER_INDEX_FILTER_BANDWIDTH:
			value = FilterParameter( this->getFilterBandwidth() );
			return true;
		default:
			if ( parameterIndex >= PARAMETER_INDEX_FILTER_GAIN && parameterIndex < PARAMETER_COUNT )
			{
				Index filterIndex = parameterIndex - PARAMETER_INDEX_FILTER_GAIN;
				value = FilterParameter( this->getFilterGainDB( filterIndex ) );
				return true;
			}
			break;
	}
	
	return false;
}




Bool GraphicEqualizer:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
	Float floatValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setOutputGainDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_FILTER_Q:
			if ( value.getValue( floatValue ) )
			{
				this->setFilterQ( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_FILTER_BANDWIDTH:
			if ( value.getValue( floatValue ) )
			{
				this->setFilterBandwidth( floatValue );
				return true;
			}
			break;
		default:
			if ( parameterIndex >= PARAMETER_INDEX_FILTER_GAIN && parameterIndex < PARAMETER_COUNT )
			{
				if ( value.getValue( gainValue ) )
				{
					Index filterIndex = parameterIndex - PARAMETER_INDEX_FILTER_GAIN;
					this->setFilterGainDB( filterIndex, gainValue );
					return true;
				}
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




void GraphicEqualizer:: resetStream()
{
	// Reset the graphic equalizer filters.
	for ( Index c = 0; c < filters.getSize(); c++ )
		filters[c].reset();
	
	gainFilter.reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult GraphicEqualizer:: processFrame( const SoundFrame& inputFrame,
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
	// Do the main filter processing.
	
	// Apply the first filter separately to get the audio into the output buffer.
	// Every filter after this can be done in-place in the output buffer.
	filters[0].process( *inputBuffer, *outputBuffer, numSamples );
	
	// Apply each additional parametric filter in series.
	for ( Index i = 1; i < filters.getSize(); i++ )
	{
		// Check to see if the gain for the filter is 0dB. If so, skip it.
		if ( math::abs(Gain(1) - filters[i].getGain()) > math::epsilon<Gain>() )
			filters[i].process( *outputBuffer, *outputBuffer, numSamples );
	}
	
	// Apply the output gain if the gain is not unity.
	if ( math::abs(Gain(1) - gainFilter.getGain()) > math::epsilon<Gain>() )
		gainFilter.process( *outputBuffer, *outputBuffer, numSamples );
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Center Frequency Definition
//############		
//##########################################################################################
//##########################################################################################




const Float GraphicEqualizer:: FILTER_FREQUENCIES[31] = {
	20.0f,
	25.0f,
	31.5f,
	40.0f,
	50.0f,
	63.0f,
	80.0f,
	100.0f,
	125.0f,
	160.0f,
	200.0f,
	250.0f,
	315.0f,
	400.0f,
	500.0f,
	630.0f,
	800.0f,
	1000.0f,
	1250.0f,
	1600.0f,
	2000.0f,
	2500.0f,
	3150.0f,
	4000.0f,
	5000.0f,
	6300.0f,
	8000.0f,
	10000.0f,
	12500.0f,
	16000.0f,
	20000.0f
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
