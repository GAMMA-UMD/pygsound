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

#include "omSoundDelay.h"


#define PARAMETER_INDEX_DELAY_TYPE 0
#define PARAMETER_INDEX_DELAY_TIME 1
#define PARAMETER_INDEX_FEEDBACK_GAIN 2
#define PARAMETER_INDEX_DECAY_TIME 3
#define PARAMETER_INDEX_DELAY_GAIN 4
#define PARAMETER_INDEX_DRY_GAIN 5
#define PARAMETER_INDEX_FREEZE_DELAY 6
#define PARAMETER_INDEX_HIGH_PASS_FILTER_ENABLED 7
#define PARAMETER_INDEX_HIGH_PASS_FILTER_FREQUENCY 8
#define PARAMETER_INDEX_HIGH_PASS_FILTER_ORDER 9
#define PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED 10
#define PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY 11
#define PARAMETER_INDEX_LOW_PASS_FILTER_ORDER 12
#define PARAMETER_COUNT 13

#define PARAMETER_NAME_DELAY_TYPE "Delay Type"
#define PARAMETER_NAME_DELAY_TIME "Delay Time"
#define PARAMETER_NAME_FEEDBACK_GAIN "Feedback Gain"
#define PARAMETER_NAME_DECAY_TIME "Decay Time"
#define PARAMETER_NAME_DELAY_GAIN "Delay Gain"
#define PARAMETER_NAME_DRY_GAIN "Dry Gain"
#define PARAMETER_NAME_FREEZE_DELAY "Freeze Delay"
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


const UTF8String Delay:: NAME( "Delay" );
const UTF8String Delay:: MANUFACTURER( "Om Sound" );
const FilterVersion Delay:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Delay:: Delay()
	:	SoundFilter( 1, 1 ),
		type( COMB ),
		delayBufferSize( 0 ),
		currentDelayWriteIndex( 0 ),
		delayTime( 0 ),
		targetDelayTime( 0.5 ),
		feedbackGain( 0 ),
		targetFeedbackGain( 0 ),
		delayGain( 1 ),
		targetDelayGain( 1 ),
		dryGain( 1 ),
		targetDryGain( 1 ),
		highPass( NULL ),
		highPassFrequency( 200 ),
		highPassOrder( 2 ),
		lowPass( NULL ),
		lowPassFrequency( 4000 ),
		lowPassOrder( 2 ),
		lowPassEnabled( false ),
		highPassEnabled( false ),
		delayFrozen( false )
{
}




Delay:: Delay( DelayType newType, Float newDelayTime, Gain newFeedbackGain, Gain newDelayGain, Gain newDryGain )
	:	SoundFilter( 1, 1 ),
		type( newType ),
		delayBufferSize( 0 ),
		currentDelayWriteIndex( 0 ),
		delayTime( 0 ),
		targetDelayTime( math::max( newDelayTime, Float(0) ) ),
		feedbackGain( math::clamp( newFeedbackGain, Gain(-0.99999), Gain(0.99999) ) ),
		delayGain( newDelayGain ),
		targetDelayGain( newDelayGain ),
		dryGain( newDryGain ),
		targetDryGain( newDryGain ),
		highPass( NULL ),
		highPassFrequency( 200 ),
		highPassOrder( 2 ),
		lowPass( NULL ),
		lowPassFrequency( 4000 ),
		lowPassOrder( 2 ),
		lowPassEnabled( false ),
		highPassEnabled( false ),
		delayFrozen( false )
{
	targetFeedbackGain = newFeedbackGain;
}




Delay:: Delay( const Delay& other )
	:	SoundFilter( 1, 1 ),
		type( other.type ),
		delayBufferSize( other.delayBufferSize ),
		currentDelayWriteIndex( other.currentDelayWriteIndex ),
		delayTime( other.delayTime ),
		targetDelayTime( other.targetDelayTime ),
		feedbackGain( other.feedbackGain ),
		targetFeedbackGain( other.targetFeedbackGain ),
		delayGain( other.delayGain ),
		targetDelayGain( other.targetDelayGain ),
		dryGain( other.dryGain ),
		targetDryGain( other.targetDryGain ),
		highPass( NULL ),
		highPassFrequency( other.highPassFrequency ),
		highPassOrder( other.highPassOrder ),
		lowPass( NULL ),
		lowPassFrequency( other.lowPassFrequency ),
		lowPassOrder( other.lowPassOrder ),
		lowPassEnabled( other.lowPassEnabled ),
		highPassEnabled( other.highPassEnabled ),
		delayFrozen( other.delayFrozen )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Delay:: ~Delay()
{
	if ( highPass != NULL )
		om::util::destruct( highPass );
	
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





Delay& Delay:: operator = ( const Delay& other )
{
	if ( this != &other )
	{
		type = other.type;
		delayBufferSize = other.delayBufferSize;
		currentDelayWriteIndex = other.currentDelayWriteIndex;
		delayTime = other.delayTime;
		targetDelayTime = other.targetDelayTime;
		feedbackGain = other.feedbackGain;
		targetFeedbackGain = other.targetFeedbackGain;
		delayGain = other.delayGain;
		targetDelayGain = other.targetDelayGain;
		dryGain = other.dryGain;
		targetDryGain = other.targetDryGain;
		highPassFrequency = other.highPassFrequency;
		highPassOrder = other.highPassOrder;
		lowPassFrequency = other.lowPassFrequency;
		lowPassOrder = other.lowPassOrder;
		lowPassEnabled = other.lowPassEnabled;
		highPassEnabled = other.highPassEnabled;
		delayFrozen = other.delayFrozen;
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




UTF8String Delay:: getName() const
{
	return NAME;
}




UTF8String Delay:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Delay:: getVersion() const
{
	return VERSION;
}




FilterCategory Delay:: getCategory() const
{
	return FilterCategory::DELAY;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size Delay:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Delay:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_DELAY_TYPE:
			info = FilterParameterInfo( PARAMETER_INDEX_DELAY_TYPE, PARAMETER_NAME_DELAY_TYPE,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(COMB), Int64(ALL_PASS), Int64(COMB),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_DELAY_TIME:
			info = FilterParameterInfo( PARAMETER_INDEX_DELAY_TIME, PARAMETER_NAME_DELAY_TIME,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::CUBE,
										0.0f, 10000.0f, 500.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FEEDBACK_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_FEEDBACK_GAIN, PARAMETER_NAME_FEEDBACK_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, -0.01f, -6.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DECAY_TIME:
			info = FilterParameterInfo( PARAMETER_INDEX_DECAY_TIME, PARAMETER_NAME_DECAY_TIME,
										FilterParameterType::FLOAT,
										FilterParameterUnits::SECONDS, FilterParameterCurve::SQUARE,
										0.0f, 50.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DELAY_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_DELAY_GAIN, PARAMETER_NAME_DELAY_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, 6.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DRY_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_DRY_GAIN, PARAMETER_NAME_DRY_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, 6.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FREEZE_DELAY:
			info = FilterParameterInfo( PARAMETER_INDEX_FREEZE_DELAY, PARAMETER_NAME_FREEZE_DELAY,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
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




Bool Delay:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	if ( parameterIndex == PARAMETER_INDEX_DELAY_TYPE && value.getValue( enumValue ) )
	{
		switch ( enumValue )
		{
			case COMB:			name = "Comb";			break;
			case ALL_PASS:		name = "All-Pass";		break;
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




Bool Delay:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_DELAY_TYPE:
			value = FilterParameter( (Int64)this->getType() );
			return true;
		case PARAMETER_INDEX_DELAY_TIME:
			value = FilterParameter( this->getDelayTime()*1000.0f );
			return true;
		case PARAMETER_INDEX_FEEDBACK_GAIN:
			value = FilterParameter( this->getFeedbackGainDB() );
			return true;
		case PARAMETER_INDEX_DECAY_TIME:
			value = FilterParameter( this->getDecayTime() );
			return true;
		case PARAMETER_INDEX_DELAY_GAIN:
			value = FilterParameter( this->getDelayGainDB() );
			return true;
		case PARAMETER_INDEX_DRY_GAIN:
			value = FilterParameter( this->getDryGainDB() );
			return true;
		case PARAMETER_INDEX_FREEZE_DELAY:
			value = FilterParameter( this->getDelayIsFrozen() );
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




Bool Delay:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 enumValue;
	Gain gainValue;
	Float floatValue;
	Int64 intValue;
	Bool booleanValue;
	
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_DELAY_TYPE:
			if ( value.getValue( enumValue ) &&
				(enumValue == COMB || enumValue == ALL_PASS) )
			{
				this->setType( (DelayType)enumValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_DELAY_TIME:
			if ( value.getValue( floatValue ) )
			{
				this->setDelayTime( floatValue*0.001f );
				return true;
			}
			break;
		case PARAMETER_INDEX_FEEDBACK_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setFeedbackGainDB( gainValue );
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
		case PARAMETER_INDEX_DELAY_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setDelayGainDB( gainValue );
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
		case PARAMETER_INDEX_FREEZE_DELAY:
			if ( value.getValue( booleanValue ) )
			{
				this->setDelayIsFrozen( booleanValue );
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
//############		Filter Reset Method
//############		
//##########################################################################################
//##########################################################################################




void Delay:: resetStream()
{
	delayBuffer.zero();
	currentDelayWriteIndex = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Delay:: processFrame( const SoundFrame& inputFrame,
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
	
	//*****************************************************************************
	
	// Compute the necessary size for the delay buffer. Must be at least 1 sample long.
	Size newDelayBufferSize = math::max( Size(targetDelayTime*inputSampleRate), Size(1) );
	
	if ( delayBufferSize < newDelayBufferSize )
	{
		// The delay time is increasing and the delay buffer might need to be enlarged.
		if ( delayBuffer.getSize() < newDelayBufferSize )
			delayBuffer.setSize( newDelayBufferSize );
		
		// Zero the new samples in the delay buffer.
		delayBuffer.zero( delayBufferSize, newDelayBufferSize - delayBufferSize );
	}
	else
	{
		// The delay time is decreasing.
		
		// If the current delay index is pointing to a sample that is no longer
		// part of the delay buffer, reset it to the beginning of the delay buffer.
		if ( currentDelayWriteIndex >= newDelayBufferSize )
			currentDelayWriteIndex = 0;
	}
	
	// Update the current delay buffer size and the new delay tiem.
	delayBufferSize = newDelayBufferSize;
	delayTime = targetDelayTime;
	
	// Determine the number of channels that are being processed.
	const Size numChannels = inputBuffer->getChannelCount();
	
	// Make sure that the delay buffer has enough channels.
	if ( delayBuffer.getChannelCount() < numChannels )
	{
		delayBuffer.setChannelCount( numChannels );
		delayBuffer.zero();
	}
	
	//*****************************************************************************
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		feedbackGain = targetFeedbackGain;
		delayGain = targetDelayGain;
		dryGain = targetDryGain;
	}
	
	// Compute how much each interpolated parameter changes per sample.
	Float inverseNumSamples = Float(1) / numSamples;
	
	// Determine the change in the parameters of the delay filter.
	Gain feedbackGainChangePerSample = Gain(0.5)*(targetFeedbackGain - feedbackGain)*inverseNumSamples;
	Gain delayGainChangePerSample = Gain(0.5)*(targetDelayGain - delayGain)*inverseNumSamples;
	Gain dryGainChangePerSample = Gain(0.5)*(targetDryGain - dryGain)*inverseNumSamples;
	
	//*****************************************************************************
	// Process the wet signal into the output buffer.
	
	if ( type == COMB )
	{
		if ( delayFrozen )
		{
			processDelayChanges<COMB,true>( *inputBuffer, *outputBuffer, numSamples,
											delayBuffer, delayBufferSize, currentDelayWriteIndex,
											feedbackGain, feedbackGainChangePerSample,
											delayGain, delayGainChangePerSample );
		}
		else
		{
			processDelayChanges<COMB,false>( *inputBuffer, *outputBuffer, numSamples,
											delayBuffer, delayBufferSize, currentDelayWriteIndex,
											feedbackGain, feedbackGainChangePerSample,
											delayGain, delayGainChangePerSample );
		}
	}
	else if ( type == ALL_PASS )
	{
		if ( delayFrozen )
		{
			processDelayChanges<ALL_PASS,true>( *inputBuffer, *outputBuffer, numSamples,
											delayBuffer, delayBufferSize, currentDelayWriteIndex,
											feedbackGain, feedbackGainChangePerSample,
											delayGain, delayGainChangePerSample );
		}
		else
		{
			processDelayChanges<ALL_PASS,false>( *inputBuffer, *outputBuffer, numSamples,
											delayBuffer, delayBufferSize, currentDelayWriteIndex,
											feedbackGain, feedbackGainChangePerSample,
											delayGain, delayGainChangePerSample );
		}
	}
	
	
	//*****************************************************************************
	// Apply the high pass and low pass filters if necessary to the wet signal
	
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
		highPass->process( *outputBuffer, *outputBuffer, numSamples );
	}
	
	
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
		lowPass->process( *outputBuffer, *outputBuffer, numSamples );
	}
	
	
	//*****************************************************************************
	// Mix the wet signal with the dry signal.
	
	{
		Gain currentDryGain;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			// Setup iteration for input and output buffers.
			const Sample32f* input = inputBuffer->getChannel(c);
			Sample32f* output = outputBuffer->getChannel(c);
			const Sample32f* const outputEnd = output + numSamples;
			
			currentDryGain = dryGain;
			
			while ( output != outputEnd )
			{
				*output += (*input)*currentDryGain;
				
				input++;
				output++;
				currentDryGain += dryGainChangePerSample;
			}
		}
		
		dryGain = currentDryGain;
	}
	
	//*****************************************************************************
	
	currentDelayWriteIndex = (currentDelayWriteIndex + numSamples) % delayBufferSize;
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Delay Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




template < Delay::DelayType delayType, Bool frozen >
void Delay:: processDelayChanges( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
								SoundBuffer& delayBuffer, Size delayBufferSize, Index currentDelayWriteIndex,
								Gain& feedbackGain, Gain feedbackGainChangePerSample,
								Gain& delayGain, Gain delayGainChangePerSample )
{
	const Size numChannels = inputBuffer.getChannelCount();
	Gain currentFeedbackGain;
	Gain currentDelayGain;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		// Setup iteration for input and output buffers.
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		// Setup iteration for the delay buffer.
		Sample32f* const delayBufferStart = delayBuffer.getChannel(c);
		const Sample32f* const delayBufferEnd = delayBufferStart + delayBufferSize;
		Sample32f* delay = delayBufferStart + currentDelayWriteIndex;
		
		// Reset interpolation parameters.
		currentFeedbackGain = feedbackGain;
		currentDelayGain = delayGain;
		
		while ( output < outputEnd )
		{
			if ( delay >= delayBufferEnd )
				delay = delayBufferStart;
			
			if ( delayType == COMB )
			{
				Sample32f delaySample = *delay;
				
				// Compute the new delay value.
				if ( !frozen )
					*delay = delaySample*currentFeedbackGain + *input;
				
				// Set the output value.
				*output = currentDelayGain*delaySample;
			}
			else if ( delayType == ALL_PASS )
			{
				Sample32f delaySample = *delay;
				
				// Compute the new delay value.
				if ( !frozen )
					*delay = delaySample*currentFeedbackGain + (*input);
				
				// Compute the output sample.
				*output = currentDelayGain*(delaySample - (*input)*currentFeedbackGain);
			}
			
			// Increment the input, output, and delay pointers.
			input++;
			output++;
			delay++;
			
			// Update the parameters with the new values for the next sample.
			currentFeedbackGain += feedbackGainChangePerSample;
			currentDelayGain += delayGainChangePerSample;
		}
	}
	
	feedbackGain = currentFeedbackGain;
	delayGain = currentDelayGain;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
