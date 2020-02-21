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

#include "omSoundMultichannelDelay.h"


#define PARAMETER_INDEX_DELAY_TYPE 0
#define PARAMETER_INDEX_DELAY_TIME 1
#define PARAMETER_INDEX_FEEDBACK_GAIN 2
#define PARAMETER_INDEX_DECAY_TIME 3
#define PARAMETER_INDEX_DELAY_GAIN 4
#define PARAMETER_INDEX_DRY_GAIN 5
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_0 6
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_1 7
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_2 8
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_3 9
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_4 10
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_5 11
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_6 12
#define PARAMETER_INDEX_CHANNEL_DELAY_TIME_7 13
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_0 14
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_1 15
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_2 16
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_3 17
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_4 18
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_5 19
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_6 20
#define PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_7 21
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_0 22
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_1 23
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_2 24
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_3 25
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_4 26
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_5 27
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_6 28
#define PARAMETER_INDEX_CHANNEL_DECAY_TIME_7 29
#define PARAMETER_COUNT 30

#define PARAMETER_NAME_DELAY_TYPE "Delay Type"
#define PARAMETER_NAME_DELAY_TIME "Delay Time"
#define PARAMETER_NAME_DECAY_TIME "Decay Time"
#define PARAMETER_NAME_FEEDBACK_GAIN "Feedback Gain"
#define PARAMETER_NAME_DELAY_GAIN "Delay Gain"
#define PARAMETER_NAME_DRY_GAIN "Dry Gain"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_0 "Channel 1 Delay Time"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_1 "Channel 2 Delay Time"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_2 "Channel 3 Delay Time"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_3 "Channel 4 Delay Time"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_4 "Channel 5 Delay Time"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_5 "Channel 6 Delay Time"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_6 "Channel 7 Delay Time"
#define PARAMETER_NAME_CHANNEL_DELAY_TIME_7 "Channel 8 Delay Time"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_0 "Channel 1 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_1 "Channel 2 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_2 "Channel 3 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_3 "Channel 4 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_4 "Channel 5 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_5 "Channel 6 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_6 "Channel 7 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_FEEDBACK_GAIN_7 "Channel 8 Feedback Gain"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_0 "Channel 1 Decay Time"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_1 "Channel 2 Decay Time"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_2 "Channel 3 Decay Time"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_3 "Channel 4 Decay Time"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_4 "Channel 5 Decay Time"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_5 "Channel 6 Decay Time"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_6 "Channel 7 Decay Time"
#define PARAMETER_NAME_CHANNEL_DECAY_TIME_7 "Channel 8 Decay Time"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String MultichannelDelay:: NAME( "Multichannel Delay" );
const UTF8String MultichannelDelay:: MANUFACTURER( "Om Sound" );
const FilterVersion MultichannelDelay:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




MultichannelDelay:: MultichannelDelay()
	:	SoundFilter( 1, 1 ),
		type( COMB ),
		globalChannel(),
		delayGain( 1 ),
		targetDelayGain( 1 ),
		dryGain( 1 ),
		targetDryGain( 1 )
{
}




MultichannelDelay:: MultichannelDelay( Float newDelayTime, Gain newFeedbackGain,
													Gain newDelayGain, Gain newDryGain )
	:	SoundFilter( 1, 1 ),
		type( COMB ),
		globalChannel( newDelayTime, newFeedbackGain ),
		delayGain( newDelayGain ),
		targetDelayGain( newDelayGain ),
		dryGain( newDryGain ),
		targetDryGain( newDryGain )
{
}




MultichannelDelay:: MultichannelDelay( DelayType newType, Float newDelayTime,
												Gain newFeedbackGain, Gain newDelayGain, Gain newDryGain )
	:	SoundFilter( 1, 1 ),
		type( COMB ),
		globalChannel( newDelayTime, newFeedbackGain ),
		delayGain( newDelayGain ),
		targetDelayGain( newDelayGain ),
		dryGain( newDryGain ),
		targetDryGain( newDryGain )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Delay Time Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void MultichannelDelay:: setDelayTime( Index channelIndex, Float newDelayTime )
{
	lockMutex();
	
	// Enlarge the size of the channel array if necessary.
	if ( channelIndex >= channels.getSize() )
		channels.setSize( channelIndex + 1, globalChannel );
	
	channels[channelIndex].targetDelayTime = math::max( newDelayTime, Float(0) );
	
	unlockMutex();
}




void MultichannelDelay:: setDelayTime( Float newDelayTime )
{
	lockMutex();
	
	Float delay = math::max( newDelayTime, Float(0) );
	const Size numChannels = channels.getSize();
	
	for ( Index i = 0; i < numChannels; i++ )
		channels[i].targetDelayTime = delay;
	
	globalChannel.targetDelayTime = globalChannel.delayTime = delay;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decay Time Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void MultichannelDelay:: setDecayTime( Index channelIndex, Float newDecayTime )
{
	lockMutex();
	
	// Enlarge the size of the channel array if necessary.
	if ( channelIndex >= channels.getSize() )
		channels.setSize( channelIndex + 1, globalChannel );
	
	Float desiredGain = math::pow( Float(0.001),
				channels[channelIndex].targetDelayTime / math::max( newDecayTime, math::epsilon<Float>() ) );
	
	channels[channelIndex].targetFeedbackGain = math::clamp( desiredGain, Float(-0.999), Float(0.999) );
	
	unlockMutex();
}




void MultichannelDelay:: setDecayTime( Float newDecayTime )
{
	lockMutex();
	
	const Size numChannels = channels.getSize();
	
	for ( Index i = 0; i < numChannels; i++ )
	{
		Float desiredGain = math::pow( Float(0.001),
										channels[i].targetDelayTime / math::max( newDecayTime, math::epsilon<Float>() ) );
		
		channels[i].targetFeedbackGain = math::clamp( desiredGain, Float(-0.999), Float(0.999) );
	}
	
	Float desiredGain = math::pow( Float(0.001),
									globalChannel.targetDelayTime / math::max( newDecayTime, math::epsilon<Float>() ) );
	
	globalChannel.feedbackGain = math::clamp( desiredGain, Float(-0.999), Float(0.999) );
	globalChannel.targetFeedbackGain = globalChannel.feedbackGain;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Feedback Gain Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void MultichannelDelay:: setFeedbackGain( Index channelIndex, Gain newFeedbackGain )
{
	lockMutex();
	
	// Enlarge the size of the channel array if necessary.
	if ( channelIndex >= channels.getSize() )
		channels.setSize( channelIndex + 1, globalChannel );
	
	channels[channelIndex].targetFeedbackGain = math::clamp( newFeedbackGain, Gain(-0.999), Gain(0.999) );
	
	unlockMutex();
}




void MultichannelDelay:: setFeedbackGain( Gain newFeedbackGain )
{
	lockMutex();
	
	Gain feedback = math::clamp( newFeedbackGain, Gain(-0.999), Gain(0.999) );
	const Size numChannels = channels.getSize();
	
	for ( Index i = 0; i < numChannels; i++ )
		channels[i].targetFeedbackGain = feedback;
	
	globalChannel.feedbackGain = newFeedbackGain;
	globalChannel.targetFeedbackGain = newFeedbackGain;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Phase Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void MultichannelDelay:: setChannelPhase( Index channelIndex, Float newPhase )
{
	lockMutex();
	
	// Enlarge the size of the channel array if necessary.
	if ( channelIndex >= channels.getSize() )
		channels.setSize( channelIndex + 1, globalChannel );
	
	channels[channelIndex].phase = math::clamp( newPhase, Float(-180), Float(180) ) / Float(180);
	
	unlockMutex();
}




void MultichannelDelay:: setChannelPhase( Float newPhase )
{
	lockMutex();
	
	// Santize and convert the input value to the internal format.
	Float phase = math::clamp( newPhase, Float(-180), Float(180) ) / Float(180);
	
	const Size numChannels = channels.getSize();
	
	for ( Index i = 0; i < numChannels; i++ )
		channels[i].phase = phase;
	
	globalChannel.phase = phase;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String MultichannelDelay:: getName() const
{
	return NAME;
}




UTF8String MultichannelDelay:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion MultichannelDelay:: getVersion() const
{
	return VERSION;
}




FilterCategory MultichannelDelay:: getCategory() const
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




Size MultichannelDelay:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool MultichannelDelay:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
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
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::SQUARE,
										0.0f, 10000.0f, 500.0f,
										FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FEEDBACK_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_FEEDBACK_GAIN, PARAMETER_NAME_FEEDBACK_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, -0.01f, -6.0f,
										FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DECAY_TIME:
			info = FilterParameterInfo( PARAMETER_INDEX_DECAY_TIME, PARAMETER_NAME_DECAY_TIME,
										FilterParameterType::FLOAT,
										FilterParameterUnits::SECONDS, FilterParameterCurve::SQUARE,
										0.0f, 50.0f, 0.0f,
										FilterParameterFlags::WRITE_ACCESS );
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
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_0:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_1:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_2:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_3:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_4:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_5:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_6:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_7:
		{
			Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_DELAY_TIME_0;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Channel ") + (channelIndex + 1) + " Delay Time",
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::SQUARE,
										0.0f, 10000.0f, 500.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_0:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_1:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_2:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_3:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_4:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_5:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_6:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_7:
		{
			Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_0;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Channel ") + (channelIndex + 1) + " Feedback Gain",
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, -0.01f, -6.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_0:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_1:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_2:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_3:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_4:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_5:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_6:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_7:
		{
			Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_DECAY_TIME_0;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Channel ") + (channelIndex + 1) + " Decay Time",
										FilterParameterType::FLOAT,
										FilterParameterUnits::SECONDS, FilterParameterCurve::SQUARE,
										0.0f, 50.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
	}
	
	return false;
}




Bool MultichannelDelay:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
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




Bool MultichannelDelay:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_DELAY_TYPE:
			value = FilterParameter( (Int64)this->getType() );
			return true;
		case PARAMETER_INDEX_DELAY_GAIN:
			value = FilterParameter( this->getDelayGainDB() );
			return true;
		case PARAMETER_INDEX_DRY_GAIN:
			value = FilterParameter( this->getDryGainDB() );
			return true;
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_0:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_1:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_2:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_3:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_4:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_5:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_6:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_7:
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_DELAY_TIME_0;
				
				value = FilterParameter( this->getDelayTime( channelIndex )*1000.0f );
				return true;
			}
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_0:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_1:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_2:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_3:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_4:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_5:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_6:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_7:
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_0;
				
				value = FilterParameter( this->getFeedbackGainDB( channelIndex ) );
				return true;
			}
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_0:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_1:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_2:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_3:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_4:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_5:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_6:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_7:
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_DECAY_TIME_0;
				
				value = FilterParameter( this->getDecayTime( channelIndex ) );
				return true;
			}
	}
	
	return false;
}




Bool MultichannelDelay:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 enumValue;
	Gain gainValue;
	Float floatValue;
	
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
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_0:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_1:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_2:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_3:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_4:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_5:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_6:
		case PARAMETER_INDEX_CHANNEL_DELAY_TIME_7:
			if ( value.getValue( floatValue ) )
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_DELAY_TIME_0;
				
				this->setDelayTime( channelIndex, floatValue*0.001f );
				return true;
			}
			break;
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_0:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_1:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_2:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_3:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_4:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_5:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_6:
		case PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_7:
			if ( value.getValue( gainValue ) )
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_FEEDBACK_GAIN_0;
				
				this->setFeedbackGainDB( channelIndex, gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_0:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_1:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_2:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_3:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_4:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_5:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_6:
		case PARAMETER_INDEX_CHANNEL_DECAY_TIME_7:
			if ( value.getValue( floatValue ) )
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_DECAY_TIME_0;
				
				this->setDecayTime( channelIndex, floatValue );
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




void MultichannelDelay:: resetStream()
{
	for ( Index i = 0; i < channels.getSize(); i++ )
	{
		channels[i].delayBuffer.setAll( Sample32f(0) );
		channels[i].currentDelayWriteIndex = 0;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult MultichannelDelay:: processFrame( const SoundFrame& inputFrame,
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
	
	// Determine the number of channels that are being processed.
	const Size numChannels = inputBuffer->getChannelCount();
	
	// Make sure that the array of channels is large enough.
	if ( channels.getSize() < numChannels )
		channels.setSize( numChannels, globalChannel );
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		delayGain = targetDelayGain;
		dryGain = targetDryGain;
	}
	
	// Compute some quantities that are needed for all channels.
	Gain delayGainChange = targetDelayGain - delayGain;
	Gain dryGainChange = targetDryGain - dryGain;
	
	//*****************************************************************************
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		Channel& channel = channels[c];
		
		// Reset parameter interpolation if this is the first processing frame.
		if ( isFirstFrame() )
		{
			channel.feedbackGain = channel.targetFeedbackGain;
			channel.delayTime = channel.targetDelayTime;
			channel.currentDelayWriteIndex = 0;
			
			channel.delayBuffer.setAll( 0 );
		}
		
		// Compute the total signed change in the comb filter's delay time.
		Float delayChange = channel.targetDelayTime - channel.delayTime;
		
		// Does the delay time change? If so, update the delay buffer.
		if ( isFirstFrame() || math::abs(delayChange) > math::epsilon<Float>() )
		{
			// The delay time changes during this processing frame.
			// Compute the delay time at the end of this processing frame.
			Float newDelayTime = channel.targetDelayTime;
			
			// Compute the necessary size for the delay buffer. Must be at least 1 sample long.
			Size newDelayBufferSize = math::max( Size(newDelayTime*inputSampleRate), Size(1) );
			
			if ( channel.delayBufferSize < newDelayBufferSize )
			{
				// The delay time is increasing and the delay buffer might need to be enlarged.
				if ( channel.delayBuffer.getSize() < newDelayBufferSize )
					channel.delayBuffer.setSize( newDelayBufferSize, Sample32f(0) );
			}
			else
			{
				// The delay time is decreasing.
				
				// If the current delay index is pointing to a sample that is no longer
				// part of the delay buffer, reset it to the beginning of the delay buffer.
				if ( channel.currentDelayWriteIndex >= newDelayBufferSize )
					channel.currentDelayWriteIndex = 0;
			}
			
			// Update the current delay buffer size and the new delay tiem.
			channel.delayBufferSize = newDelayBufferSize;
			channel.delayTime = newDelayTime;
		}
		
		// Compute the current delay read index for the channel.
		Float phaseOffset = channel.phase*(Float(0.5)*Float(channel.delayBufferSize)) + Float(channel.delayBufferSize);
		
		Index currentDelayReadIndex = (channel.currentDelayWriteIndex + Size(phaseOffset)) % channel.delayBufferSize;
		
		//*****************************************************************************
		
		// Determine the change in each other parameters of the comb filter.
		Gain feedbackGainChange = channel.targetFeedbackGain - channel.feedbackGain;
		
		// Check to see if there is any change in these parameters.
		if ( math::abs(feedbackGainChange) < math::epsilon<Float>() &&
			math::abs(delayGainChange) < math::epsilon<Float>() &&
			math::abs(dryGainChange) < math::epsilon<Float>() )
		{
			// There is no change for any of the other comb filter parameters.
			// This allows use of the most efficient comb filter processing method.
			Sample32f* const delayBufferStart = channel.delayBuffer.getPointer();
			Sample32f* const delayBufferEnd = delayBufferStart + channel.delayBufferSize;
			const Sample32f* delayRead = delayBufferStart + currentDelayReadIndex;
			Sample32f* delayWrite = delayBufferStart + channel.currentDelayWriteIndex;
			
			if ( type == COMB )
			{
				processCombFilterNoChanges( inputBuffer->getChannel(c), outputBuffer->getChannel(c), numSamples,
										delayBufferStart, delayBufferEnd, delayRead, delayWrite,
										channel.feedbackGain, delayGain, dryGain );
			}
			else if ( type == ALL_PASS )
			{
				processAllPassFilterNoChanges( inputBuffer->getChannel(c), outputBuffer->getChannel(c), numSamples,
												delayBufferStart, delayBufferEnd, delayRead, delayWrite,
												channel.feedbackGain, delayGain, dryGain );
			}
		}
		else
		{
			// There is some change in a comb filter parameter that is not the delay time.
			// Interpolate the parameters using a slightly more expensive processing function.
			
			// Compute how much each interpolated parameter changes per sample.
			Float inverseNumSamples = Float(1) / numSamples;
			
			Gain feedbackGainChangePerSample = feedbackGainChange*inverseNumSamples;
			Gain delayGainChangePerSample = delayGainChange*inverseNumSamples;
			Gain dryGainChangePerSample = dryGainChange*inverseNumSamples;
			
			Sample32f* const delayBufferStart = channel.delayBuffer.getPointer();
			Sample32f* const delayBufferEnd = delayBufferStart + channel.delayBufferSize;
			const Sample32f* delayRead = delayBufferStart + currentDelayReadIndex;
			Sample32f* delayWrite = delayBufferStart + channel.currentDelayWriteIndex;
			
			if ( type == COMB )
			{
				processCombFilterChanges( inputBuffer->getChannel(c), outputBuffer->getChannel(c), numSamples,
										delayBufferStart, delayBufferEnd, delayRead, delayWrite,
										channel.feedbackGain, feedbackGainChangePerSample,
										delayGain, delayGainChangePerSample,
										dryGain, dryGainChangePerSample );
			}
			else if ( type == ALL_PASS )
			{
				processAllPassFilterChanges( inputBuffer->getChannel(c), outputBuffer->getChannel(c), numSamples,
											delayBufferStart, delayBufferEnd, delayRead, delayWrite,
											channel.feedbackGain, feedbackGainChangePerSample,
											delayGain, delayGainChangePerSample,
											dryGain, dryGainChangePerSample );
			}
			
			channel.feedbackGain += feedbackGainChange;
		}
		
		channel.currentDelayWriteIndex = (channel.currentDelayWriteIndex + numSamples) % channel.delayBufferSize;
	}
	
	delayGain += delayGainChange;
	dryGain += dryGainChange;
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Comb Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




void MultichannelDelay:: processCombFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
												Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
												const Sample32f* delayRead, Sample32f* delayWrite,
												Gain feedbackGain, Gain delayGain, Gain dryGain )
{
	const Sample32f* const outputEnd = output + numSamples;
	
	while ( output < outputEnd )
	{
		if ( delayRead >= delayBufferEnd )
			delayRead = delayBufferStart;
		
		if ( delayWrite >= delayBufferEnd )
			delayWrite = delayBufferStart;
		
		// Compute the next output sample by combining the delay sample and input sample.
		Sample32f newSample = (*delayRead)*delayGain + (*input)*dryGain;
		
		// Compute the new delay value.
		*delayWrite = (*delayRead)*feedbackGain + *input;
		
		// Set the output value.
		*output = newSample;
		
		// Increment the input, output, and delay pointers.
		input++;
		output++;
		delayRead++;
		delayWrite++;
	}
}




void MultichannelDelay:: processCombFilterChanges( const Sample32f* input, Sample32f* output, Size numSamples,
											Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
											const Sample32f* delayRead, Sample32f* delayWrite,
											Gain feedbackGain, Gain feedbackGainChangePerSample,
											Gain delayGain, Gain delayGainChangePerSample,
											Gain dryGain, Gain dryGainChangePerSample )
{
	const Sample32f* const outputEnd = output + numSamples;
	
	while ( output < outputEnd )
	{
		if ( delayRead >= delayBufferEnd )
			delayRead = delayBufferStart;
		
		if ( delayWrite >= delayBufferEnd )
			delayWrite = delayBufferStart;
		
		// Compute the next output sample by combining the delay sample and input sample.
		Sample32f newSample = (*delayRead)*delayGain + (*input)*dryGain;
		
		// Compute the new delay value.
		*delayWrite = (*delayRead)*feedbackGain + *input;
		
		// Set the output value.
		*output = newSample;
		
		// Increment the input, output, and delay pointers.
		input++;
		output++;
		delayRead++;
		delayWrite++;
		
		// Update the parameters with the new values for the next sample.
		feedbackGain += feedbackGainChangePerSample;
		delayGain += delayGainChangePerSample;
		dryGain += dryGainChangePerSample;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		All-Pass Filter Processing Methods
//############		
//##########################################################################################
//##########################################################################################




void MultichannelDelay:: processAllPassFilterNoChanges( const Sample32f* input, Sample32f* output, Size numSamples,
													Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
													const Sample32f* delayRead, Sample32f* delayWrite,
													Gain feedbackGain, Gain delayGain, Gain dryGain )
{
	const Sample32f* const outputEnd = output + numSamples;
	
	while ( output < outputEnd )
	{
		if ( delayRead >= delayBufferEnd )
			delayRead = delayBufferStart;
		
		if ( delayWrite >= delayBufferEnd )
			delayWrite = delayBufferStart;
		
		Sample32f delaySample = *delayRead;
		
		// Compute the new delay value.
		*delayWrite = delaySample*feedbackGain + *input;
		
		// Compute the output sample.
		*output = delaySample - (*delayWrite)*feedbackGain;
		
		// Increment the input, output, and delay pointers.
		input++;
		output++;
		delayRead++;
		delayWrite++;
	}
}




void MultichannelDelay:: processAllPassFilterChanges( const Sample32f* input, Sample32f* output, Size numSamples,
												Sample32f* const delayBufferStart, Sample32f* const delayBufferEnd,
												const Sample32f* delayRead, Sample32f* delayWrite,
												Gain feedbackGain, Gain feedbackGainChangePerSample,
												Gain delayGain, Gain delayGainChangePerSample,
												Gain dryGain, Gain dryGainChangePerSample )
{
	const Sample32f* const outputEnd = output + numSamples;
	
	while ( output < outputEnd )
	{
		if ( delayRead >= delayBufferEnd )
			delayRead = delayBufferStart;
		
		if ( delayWrite >= delayBufferEnd )
			delayWrite = delayBufferStart;
		
		Sample32f delaySample = *delayRead;
		
		// Compute the new delay value.
		*delayWrite = delaySample*feedbackGain + *input;
		
		// Compute the output sample.
		*output = delaySample - (*delayWrite)*feedbackGain;
		
		// Increment the input, output, and delay pointers.
		input++;
		output++;
		delayRead++;
		delayWrite++;
		
		// Update the parameters with the new values for the next sample.
		feedbackGain += feedbackGainChangePerSample;
		delayGain += delayGainChangePerSample;
		dryGain += dryGainChangePerSample;
	}
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
