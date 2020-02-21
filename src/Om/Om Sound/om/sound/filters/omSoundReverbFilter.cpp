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

#include "omSoundReverbFilter.h"


#define PARAMETER_INDEX_WET_GAIN 0
#define PARAMETER_INDEX_DRY_GAIN 1
#define PARAMETER_INDEX_DECAY_TIME 2
#define PARAMETER_INDEX_DENSITY 3
#define PARAMETER_INDEX_HIGH_PASS_FILTER_ENABLED 4
#define PARAMETER_INDEX_HIGH_PASS_FILTER_FREQUENCY 5
#define PARAMETER_INDEX_HIGH_PASS_FILTER_ORDER 6
#define PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED 7
#define PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY 8
#define PARAMETER_INDEX_LOW_PASS_FILTER_ORDER 9
#define PARAMETER_COUNT 10


#define PARAMETER_NAME_WET_GAIN "Wet Gain"
#define PARAMETER_NAME_DRY_GAIN "Dry Gain"
#define PARAMETER_NAME_DECAY_TIME "Decay Time"
#define PARAMETER_NAME_DENSITY "Density"
#define PARAMETER_NAME_HIGH_PASS_FILTER_ENABLED "High-Pass Filter Enabled"
#define PARAMETER_NAME_HIGH_PASS_FILTER_FREQUENCY "HPF Frequency"
#define PARAMETER_NAME_HIGH_PASS_FILTER_ORDER "HPF Order"
#define PARAMETER_NAME_LOW_PASS_FILTER_ENABLED "Low-Pass Filter Enabled"
#define PARAMETER_NAME_LOW_PASS_FILTER_FREQUENCY "LPF Frequency"
#define PARAMETER_NAME_LOW_PASS_FILTER_ORDER "LPF Order"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String ReverbFilter:: NAME( "Reverb Filter" );
const UTF8String ReverbFilter:: MANUFACTURER( "Om Sound" );
const FilterVersion ReverbFilter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ReverbFilter:: ReverbFilter()
	:	SoundFilter( 1, 1 ),
		wetGain( 1 ),
		targetWetGain( 1 ),
		dryGain( 1 ),
		targetDryGain( 1 ),
		decayTime( 2.0f ),
		density( 1.0f ),
		highPassFrequency( 200 ),
		highPassOrder( 2 ),
		highPass( NULL ),
		highPassEnabled( true ),
		lowPassFrequency( 2000 ),
		lowPassOrder( 2 ),
		lowPass( NULL ),
		lowPassEnabled( true )
{
}




ReverbFilter:: ReverbFilter( Float newDecayTime )
	:	SoundFilter( 1, 1 ),
		wetGain( 1 ),
		targetWetGain( 1 ),
		dryGain( 1 ),
		targetDryGain( 1 ),
		decayTime( 2.0f ),
		density( 1.0f ),
		highPassFrequency( 400 ),
		highPassOrder( 2 ),
		highPass( NULL ),
		highPassEnabled( false ),
		lowPassFrequency( 2000 ),
		lowPassOrder( 2 ),
		lowPass( NULL ),
		lowPassEnabled( true )
{
	this->setDecayTime( newDecayTime );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String ReverbFilter:: getName() const
{
	return NAME;
}




UTF8String ReverbFilter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion ReverbFilter:: getVersion() const
{
	return VERSION;
}




FilterCategory ReverbFilter:: getCategory() const
{
	return FilterCategory::REVERB;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size ReverbFilter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool ReverbFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_WET_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_WET_GAIN, PARAMETER_NAME_WET_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, 12.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DRY_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_DRY_GAIN, PARAMETER_NAME_DRY_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, 12.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DECAY_TIME:
			info = FilterParameterInfo( PARAMETER_INDEX_DECAY_TIME, PARAMETER_NAME_DECAY_TIME,
										FilterParameterType::FLOAT,
										FilterParameterUnits::SECONDS, FilterParameterCurve::SQUARE,
										0.1f, 20.0f, 2.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DENSITY:
			info = FilterParameterInfo( PARAMETER_INDEX_DENSITY, PARAMETER_NAME_DENSITY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										0.0f, 1.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_PASS_FILTER_ENABLED, PARAMETER_NAME_HIGH_PASS_FILTER_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_PASS_FILTER_FREQUENCY, PARAMETER_NAME_HIGH_PASS_FILTER_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 200.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_ORDER:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_PASS_FILTER_ORDER, PARAMETER_NAME_HIGH_PASS_FILTER_ORDER,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(8), Int64(2),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED, PARAMETER_NAME_LOW_PASS_FILTER_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY, PARAMETER_NAME_LOW_PASS_FILTER_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 2000.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ORDER:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_FILTER_ORDER, PARAMETER_NAME_LOW_PASS_FILTER_ORDER,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(8), Int64(2),
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




Bool ReverbFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_WET_GAIN:
			value = FilterParameter( this->getWetGainDB() );
			return true;
		case PARAMETER_INDEX_DRY_GAIN:
			value = FilterParameter( this->getDryGainDB() );
			return true;
		case PARAMETER_INDEX_DECAY_TIME:
			value = FilterParameter( this->getDecayTime() );
			return true;
		case PARAMETER_INDEX_DENSITY:
			value = FilterParameter( this->getDensity() );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_ENABLED:
			value = FilterParameter( this->getHighPassIsEnabled() );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_FREQUENCY:
			value = FilterParameter( this->getHighPassFrequency() );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_ORDER:
			value = FilterParameter( (Int64)this->getHighPassOrder() );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED:
			value = FilterParameter( this->getLowPassIsEnabled() );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY:
			value = FilterParameter( this->getLowPassFrequency() );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ORDER:
			value = FilterParameter( (Int64)this->getLowPassOrder() );
			return true;
	}
	
	return false;
}




Bool ReverbFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
	Float floatValue;
	Int64 intValue;
	Bool booleanValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_WET_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setWetGainDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_DRY_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setDryGainDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_DECAY_TIME:
			if ( value.getValue( floatValue ) )
			{
				this->setDecayTime( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_DENSITY:
			if ( value.getValue( floatValue ) )
			{
				this->setDensity( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setHighPassIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setHighPassFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_PASS_FILTER_ORDER:
			if ( value.getValue( intValue ) )
			{
				this->setHighPassOrder( Size(intValue) );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setLowPassIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setLowPassFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ORDER:
			if ( value.getValue( intValue ) )
			{
				this->setLowPassOrder( Size(intValue) );
				return true;
			}
			break;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Methods
//############		
//##########################################################################################
//##########################################################################################




void ReverbFilter:: resetStream()
{
	if ( lowPass != NULL )
		lowPass->reset();
	
	if ( highPass != NULL )
		highPass->reset();
	
	// Reset all delay buffers.
	for ( Index i = 0; i < combFilters.getSize(); i++ )
	{
		DelayFilter& combFilter = combFilters[i];
		const Size numChannels = combFilter.channels.getSize();
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			DelayFilterChannel& channel = combFilter.channels[c];
			
			channel.delayBuffer.setAll( 0 );
			channel.currentDelayWriteIndex = 0;
		}
	}
	
	for ( Index i = 0; i < allPassFilters.getSize(); i++ )
	{
		DelayFilter& allPassFilter = allPassFilters[i];
		const Size numChannels = allPassFilter.channels.getSize();
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			DelayFilterChannel& channel = allPassFilter.channels[c];
			
			channel.delayBuffer.setAll( 0 );
			channel.currentDelayWriteIndex = 0;
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




SoundResult ReverbFilter:: processFrame( const SoundFrame& inputFrame,
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
	
	const Size numChannels = inputBuffer->getChannelCount();
	const SampleRate sampleRate = inputBuffer->getSampleRate();
	
	//******************************************************************************
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		wetGain = targetWetGain;
		dryGain = targetDryGain;
	}
	
	//******************************************************************************
	// Process the parallel comb filters of this reverberator
	
	// Get a temporary buffer and zero its contents.
	SharedSoundBuffer sharedTempBuffer = SharedBufferPool::getGlobalBuffer( numChannels, numSamples, sampleRate );
	SoundBuffer& tempBuffer = sharedTempBuffer.getBuffer();
	tempBuffer.zero( 0, numSamples );
	
	
	// Compute the required number of comb filters based on the density.
	const Size numCombFilters = math::clamp( Size(math::round(density*MAX_NUMBER_OF_COMB_FILTERS)),
												Size(1), MAX_NUMBER_OF_COMB_FILTERS);
	
	// Make sure there are enough comb filters and that they all have enough channels.
	if ( combFilters.getSize() < numCombFilters )
		combFilters.setSize( numCombFilters );
	
	// Process each comb filter.
	for ( Index i = 0; i < numCombFilters; i++ )
	{
		DelayFilter& combFilter = combFilters[i];
		
		// Make sure this comb filter has enough channels.
		if ( combFilter.channels.getSize() < numChannels )
			combFilter.channels.setSize( numChannels );
		
		// Process each channel.
		for ( Index c = 0; c < numChannels; c++ )
		{
			DelayFilterChannel& channel = combFilter.channels[c];
			
			// If the delay time is zero, initialize the channel to a random delay time.
			if ( channel.delayTime < math::epsilon<Float>() )
			{
				channel.delayTime = math::random( Float(0.02), Float(0.05) );
			}
			
			if ( channel.decayTime != decayTime )
			{
				channel.feedbackGain = getFeedbackGainForRT60( channel.delayTime, decayTime );
				channel.decayTime = decayTime;
			}
			
			// Compute the required delay buffer size for this channel.
			Size delayBufferSize = Size(sampleRate*channel.delayTime);
			
			// Make sure the channel's delay buffer is big enough.
			if ( channel.delayBuffer.getSize() < delayBufferSize )
				channel.delayBuffer.setSize( delayBufferSize );
			
			// Make sure the delay write index is valid.
			if ( channel.currentDelayWriteIndex >= delayBufferSize )
				channel.currentDelayWriteIndex = 0;
			
			
			const Sample32f* input = inputBuffer->getChannel(c);
			Sample32f* output = tempBuffer.getChannel(c);
			Sample32f* const delayStart = channel.delayBuffer;
			Sample32f* const delayEnd = delayStart + delayBufferSize;
			Sample32f* delay = delayStart + channel.currentDelayWriteIndex;
			
			processCombFilterNoChanges( input, output, numSamples,
										delayStart, delayEnd, delay, channel.feedbackGain );
			
			// Update the current delay write index.
			channel.currentDelayWriteIndex = (channel.currentDelayWriteIndex + numSamples) % delayBufferSize;
		}
	}
	
	
	//******************************************************************************
	// Process the low pass filter for the reverb filter if applicable.
	
	if ( lowPassEnabled )
	{
		// Create the low pass filter if it has not been created yet.
		if ( lowPass == NULL )
		{
			lowPass = om::util::construct<CutoffFilter>( CutoffFilter::BUTTERWORTH, CutoffFilter::LOW_PASS,
														lowPassOrder, lowPassFrequency );
			lowPass->setIsSynchronized( false );
		}
		else
		{
			// Update the filter frequencies if necessary.
			if ( lowPass->getFrequency() != lowPassFrequency )
				lowPass->setFrequency( lowPassFrequency );
			
			if ( lowPass->getOrder() != lowPassOrder )
				lowPass->setOrder( lowPassOrder );
		}
		
		/// Process the low pass filter on the output buffer.
		lowPass->process( tempBuffer, tempBuffer, numSamples );
	}
	
	
	//******************************************************************************
	// Process the series all-pass filters of this reverberator
	
	// Compute the required number of all pass filters based on the density.
	const Size numAllPassFilters = Size(math::clamp( math::round(density*MAX_NUMBER_OF_ALL_PASS_FILTERS),
													Float(1), Float(MAX_NUMBER_OF_ALL_PASS_FILTERS) ));
	
	// Make sure there are enough comb filters and that they all have enough channels.
	if ( allPassFilters.getSize() < numAllPassFilters )
		allPassFilters.setSize( numAllPassFilters );
	
	// Process each all pass filter.
	for ( Index i = 0; i < numAllPassFilters; i++ )
	{
		DelayFilter& allPassFilter = allPassFilters[i];
		
		// Make sure this all-pass filter has enough channels.
		if ( allPassFilter.channels.getSize() < numChannels )
			allPassFilter.channels.setSize( numChannels );
		
		// Process each channel.
		for ( Index c = 0; c < numChannels; c++ )
		{
			DelayFilterChannel& channel = allPassFilter.channels[c];
			
			// If the delay time is zero, initialize the channel to a random delay time.
			if ( channel.delayTime < math::epsilon<Float>() )
			{
				channel.delayTime = math::random( Float(0.005), Float(0.02) );
				channel.feedbackGain = getFeedbackGainForRT60( channel.delayTime, 0.1f );
			}
			
			// Compute the required delay buffer size for this channel.
			Size delayBufferSize = Size(sampleRate*channel.delayTime);
			
			// Make sure the channel's delay buffer is big enough.
			if ( channel.delayBuffer.getSize() < delayBufferSize )
				channel.delayBuffer.setSize( delayBufferSize );
			
			// Make sure the delay write index is valid.
			if ( channel.currentDelayWriteIndex >= delayBufferSize )
				channel.currentDelayWriteIndex = 0;
			
			const Sample32f* input = tempBuffer.getChannel(c);
			Sample32f* output = tempBuffer.getChannel(c);
			Sample32f* const delayStart = channel.delayBuffer;
			Sample32f* const delayEnd = delayStart + delayBufferSize;
			Sample32f* delay = delayStart + channel.currentDelayWriteIndex;
			
			processAllPassFilterNoChanges( input, output, numSamples,
											delayStart, delayEnd, delay, channel.feedbackGain );
			
			// Update the current delay write index.
			channel.currentDelayWriteIndex = (channel.currentDelayWriteIndex + numSamples) % delayBufferSize;
		}
	}
	
	//******************************************************************************
	// Process the high pass filter for the reverb filter if applicable.
	
	if ( highPassEnabled )
	{
		// Create the high pass filter if it has not been created yet.
		if ( highPass == NULL )
		{
			highPass = om::util::construct<CutoffFilter>( CutoffFilter::BUTTERWORTH, CutoffFilter::HIGH_PASS,
														highPassOrder, highPassFrequency );
			highPass->setIsSynchronized( false );
		}
		else
		{
			// Update the filter frequencies if necessary.
			if ( highPass->getFrequency() != highPassFrequency )
				highPass->setFrequency( highPassFrequency );
			
			if ( highPass->getOrder() != highPassOrder )
				highPass->setOrder( highPassOrder );
		}
		
		/// Process the high pass filter on the output buffer.
		highPass->process( tempBuffer, tempBuffer, numSamples );
	}
	
	//******************************************************************************
	// Mix the dry signal with the wet output.
	
	// Compute how much the gain factors should change per sample.
	Gain wetGainChangePerSample = Gain(0.5)*(targetWetGain - wetGain) / numSamples;
	Gain dryGainChangePerSample = Gain(0.5)*(targetDryGain - dryGain) / numSamples;
	
	Gain currentWetGain = wetGain;
	Gain currentDryGain = dryGain;
	
	// A gain factor which compensates for the amplitude increase caused by the comb filters.
	Gain densityGain = Gain(1) / Gain(numCombFilters);
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* dry = inputBuffer->getChannel(c);
		const Sample32f* wet = tempBuffer.getChannel(c);
		Sample32f* output = outputBuffer->getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		currentWetGain = wetGain;
		currentDryGain = dryGain;
		
		while ( output != outputEnd )
		{
			*output = (*dry)*currentDryGain + (*wet)*currentWetGain*densityGain;
			
			output++;
			dry++;
			wet++;
			
			currentWetGain += wetGainChangePerSample;
			currentDryGain += dryGainChangePerSample;
		}
	}
	
	wetGain = currentWetGain;
	dryGain = currentDryGain;
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Comb Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




void ReverbFilter:: processCombFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
												Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
												Sample32f* delay, Gain feedbackGain )
{
	const Sample32f* const outputEnd = output + numSamples;
	
	while ( output < outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		// Compute the next output sample by combining the delay sample and input sample.
		Sample32f newSample = *delay;
		
		// Compute the new delay value.
		*delay = (*delay)*feedbackGain + *input;
		
		// Mix the output value.
		*output += newSample;
		
		// Increment the input, output, and delay pointers.
		input++;
		output++;
		delay++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		All-Pass Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




void ReverbFilter:: processAllPassFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
													Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
													Sample32f* delay, Gain feedbackGain )
{
	const Sample32f* const outputEnd = output + numSamples;
	
	while ( output < outputEnd )
	{
		if ( delay >= delayBufferEnd )
			delay = delayBufferStart;
		
		Sample32f delaySample = *delay;
		
		// Compute the new delay value.
		*delay = delaySample*feedbackGain + *input;
		
		// Compute the output sample.
		*output = delaySample - (*delay)*feedbackGain;
		
		// Increment the input, output, and delay pointers.
		input++;
		output++;
		delay++;
	}
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
