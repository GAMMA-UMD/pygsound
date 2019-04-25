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

#include "omSoundTremolo.h"


#define PARAMETER_INDEX_WAVE_TYPE 0
#define PARAMETER_INDEX_FREQUENCY 1
#define PARAMETER_INDEX_DEPTH 2
#define PARAMETER_INDEX_SMOOTHING 3
#define PARAMETER_INDEX_CHANNEL_PHASE_0 4
#define PARAMETER_INDEX_CHANNEL_PHASE_1 5
#define PARAMETER_INDEX_CHANNEL_PHASE_2 6
#define PARAMETER_INDEX_CHANNEL_PHASE_3 7
#define PARAMETER_INDEX_CHANNEL_PHASE_4 8
#define PARAMETER_INDEX_CHANNEL_PHASE_5 9
#define PARAMETER_INDEX_CHANNEL_PHASE_6 10
#define PARAMETER_INDEX_CHANNEL_PHASE_7 11
#define PARAMETER_COUNT 12


#define PARAMETER_NAME_WAVE_TYPE "Wave Type"
#define PARAMETER_NAME_FREQUENCY "Frequency"
#define PARAMETER_NAME_DEPTH "Depth"
#define PARAMETER_NAME_SMOOTHING "Smoothing"
#define PARAMETER_NAME_CHANNEL_PHASE_0 "Left Channel Phase"
#define PARAMETER_NAME_CHANNEL_PHASE_1 "Right Channel Phase"
#define PARAMETER_NAME_CHANNEL_PHASE_2 "Channel 3 Phase"
#define PARAMETER_NAME_CHANNEL_PHASE_3 "Channel 4 Phase"
#define PARAMETER_NAME_CHANNEL_PHASE_4 "Channel 5 Phase"
#define PARAMETER_NAME_CHANNEL_PHASE_5 "Channel 6 Phase"
#define PARAMETER_NAME_CHANNEL_PHASE_6 "Channel 7 Phase"
#define PARAMETER_NAME_CHANNEL_PHASE_7 "Channel 8 Phase"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Tremolo:: NAME( "Tremolo" );
const UTF8String Tremolo:: MANUFACTURER( "Om Sound" );
const FilterVersion Tremolo:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Tremolo:: Tremolo()
	:	SoundFilter( 1, 1 ),
		type( SINE ),
		frequency( 1.0 ),
		targetFrequency( 1.0 ),
		depth( 0.5 ),
		targetDepth( 0.5 ),
		smoothing( 0 ),
		envelope( 0 ),
		globalChannelPhase( 0 ),
		phase( 0 )
{
}




Tremolo:: Tremolo( WaveType newType, Float newFrequency, Gain newDepth )
	:	SoundFilter( 1, 1 ),
		type( newType ),
		frequency( math::max( newFrequency, Float(0) ) ),
		depth( math::dbToLinear( -math::abs(newDepth) ) ),
		smoothing( 0 ),
		envelope( 0 ),
		globalChannelPhase( 0 ),
		phase( 0 )
{
	targetFrequency = frequency;
	targetDepth = depth;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Phase Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void Tremolo:: setChannelPhase( Index channelIndex, Float newPhase )
{
	lockMutex();
	
	// Enlarge the size of the channel phase array if necessary.
	if ( channelIndex >= channelPhase.getSize() )
		channelPhase.setSize( channelIndex + 1, globalChannelPhase );
	
	// Santize and convert the input value to radians.
	channelPhase[channelIndex] = math::degreesToRadians( math::clamp( newPhase, Float(-180), Float(180) ) );
	
	unlockMutex();
}




void Tremolo:: setChannelPhase( Float newPhase )
{
	lockMutex();
	
	// Santize and convert the input value to radians.
	Float phaseValue = math::degreesToRadians( math::clamp( newPhase, Float(-180), Float(180) ) );
	
	channelPhase.setAll( phaseValue );
	globalChannelPhase = phaseValue;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tremolo Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Tremolo:: getName() const
{
	return NAME;
}




UTF8String Tremolo:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Tremolo:: getVersion() const
{
	return VERSION;
}




FilterCategory Tremolo:: getCategory() const
{
	return FilterCategory::MODULATION;
}




Bool Tremolo:: allowsInPlaceProcessing() const
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





Size Tremolo:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Tremolo:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_WAVE_TYPE:
			info = FilterParameterInfo( PARAMETER_INDEX_WAVE_TYPE, PARAMETER_NAME_WAVE_TYPE,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(SINE), Int64(TRIANGLE), Int64(SINE),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY, PARAMETER_NAME_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::SQUARE,
										0.05f, 50.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DEPTH:
			info = FilterParameterInfo( PARAMETER_INDEX_DEPTH, PARAMETER_NAME_DEPTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::SQUARE,
										0.1f, 50.0f, 3.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_SMOOTHING:
			info = FilterParameterInfo( PARAMETER_INDEX_SMOOTHING, PARAMETER_NAME_SMOOTHING,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::SQUARE,
										0.01f, 100.0f, 0.01f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_CHANNEL_PHASE_0:
		case PARAMETER_INDEX_CHANNEL_PHASE_1:
		case PARAMETER_INDEX_CHANNEL_PHASE_2:
		case PARAMETER_INDEX_CHANNEL_PHASE_3:
		case PARAMETER_INDEX_CHANNEL_PHASE_4:
		case PARAMETER_INDEX_CHANNEL_PHASE_5:
		case PARAMETER_INDEX_CHANNEL_PHASE_6:
		case PARAMETER_INDEX_CHANNEL_PHASE_7:
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_PHASE_0;
				
				info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Channel ") + (channelIndex + 1) + " Phase",
											FilterParameterType::FLOAT,
											FilterParameterUnits::DEGREES, FilterParameterCurve::LINEAR,
											-180.0f, 180.0f, 0.0f,
											FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			}
			return true;
	}
	
	return false;
}




Bool Tremolo:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	if ( parameterIndex == PARAMETER_INDEX_WAVE_TYPE && value.getValue( enumValue ) )
	{
		switch ( enumValue )
		{
			case SINE:			name = "Sine";			break;
			case SQUARE:		name = "Square";		break;
			case SAW:			name = "Saw";			break;
			case TRIANGLE:		name = "Triangle";		break;
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




Bool Tremolo:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_WAVE_TYPE:
			value = FilterParameter( (Int64)this->getType() );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			value = FilterParameter( this->getFrequency() );
			return true;
		case PARAMETER_INDEX_DEPTH:
			value = FilterParameter( this->getDepth() );
			return true;
		case PARAMETER_INDEX_SMOOTHING:
			value = FilterParameter( this->getSmoothing()*100.0f );
			return true;
		case PARAMETER_INDEX_CHANNEL_PHASE_0:
		case PARAMETER_INDEX_CHANNEL_PHASE_1:
		case PARAMETER_INDEX_CHANNEL_PHASE_2:
		case PARAMETER_INDEX_CHANNEL_PHASE_3:
		case PARAMETER_INDEX_CHANNEL_PHASE_4:
		case PARAMETER_INDEX_CHANNEL_PHASE_5:
		case PARAMETER_INDEX_CHANNEL_PHASE_6:
		case PARAMETER_INDEX_CHANNEL_PHASE_7:
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_PHASE_0;
				
				value = FilterParameter( this->getChannelPhase( channelIndex ) );
				return true;
			}
	}
	
	return false;
}




Bool Tremolo:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 enumValue;
	Gain gainValue;
	Float floatValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_WAVE_TYPE:
			if ( value.getValue( enumValue ) &&
				(enumValue == SINE || enumValue == SQUARE || enumValue == SAW || enumValue == TRIANGLE) )
			{
				this->setType( (WaveType)enumValue );
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
		case PARAMETER_INDEX_DEPTH:
			if ( value.getValue( gainValue ) )
			{
				this->setDepth( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_SMOOTHING:
			if ( value.getValue( floatValue ) )
			{
				this->setSmoothing( floatValue*0.01f );
				return true;
			}
			break;
		case PARAMETER_INDEX_CHANNEL_PHASE_0:
		case PARAMETER_INDEX_CHANNEL_PHASE_1:
		case PARAMETER_INDEX_CHANNEL_PHASE_2:
		case PARAMETER_INDEX_CHANNEL_PHASE_3:
		case PARAMETER_INDEX_CHANNEL_PHASE_4:
		case PARAMETER_INDEX_CHANNEL_PHASE_5:
		case PARAMETER_INDEX_CHANNEL_PHASE_6:
		case PARAMETER_INDEX_CHANNEL_PHASE_7:
			if ( value.getValue( floatValue ) )
			{
				Index channelIndex = parameterIndex - PARAMETER_INDEX_CHANNEL_PHASE_0;
				
				this->setChannelPhase( channelIndex, floatValue );
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




void Tremolo:: resetStream()
{
	// Reset the tremolo envelope.
	envelope.setAll( 0 );
	phase = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Tremolo:: processFrame( const SoundFrame& inputFrame,
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
	
	const SampleRate sampleRate = inputBuffer->getSampleRate();
	
	// If the input sample rate is zero, return that an error occurred.
	if ( sampleRate <= SampleRate(0) )
		return SoundResult::ERROR;
	
	// Make sure that the output buffer has the right size and format.
	inputBuffer->copyFormatTo( *outputBuffer, numSamples );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************************
	
	const Size numChannels = inputBuffer->getChannelCount();
	
	if ( envelope.getSize() < numChannels )
		envelope.setSize( numChannels );
	
	if ( channelPhase.getSize() < numChannels )
		channelPhase.setSize( numChannels, globalChannelPhase );
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		frequency = targetFrequency;
		depth = targetDepth;
	}
	
	Float inverseNumSamples = Float(1) / numSamples;
	
	// Compute the change in some parameters.
	Float frequencyChangePerSample = Float(0.5)*(targetFrequency - frequency)*inverseNumSamples;
	Gain depthChangePerSample = Gain(0.5)*(targetDepth - depth)*inverseNumSamples;
	
	// Compute the envelope attack and release parameters.
	const Gain envelopeChange = math::pow( Float(0.001), Float(1.0/(smoothing*(Float(1)/frequency)*sampleRate)) );
	
	// Determine which type of modulation to use.
	switch ( type )
	{
		case SINE:
			modulate<cosine>( *inputBuffer, *outputBuffer, numSamples,
							frequencyChangePerSample, depthChangePerSample, envelopeChange );
			break;
		case SQUARE:
			modulate<square>( *inputBuffer, *outputBuffer, numSamples,
							frequencyChangePerSample, depthChangePerSample, envelopeChange );
			break;
		case SAW:
			modulate<saw>( *inputBuffer, *outputBuffer, numSamples, 
							frequencyChangePerSample, depthChangePerSample, envelopeChange );
			break;
		case TRIANGLE:
			modulate<triangle>( *inputBuffer, *outputBuffer, numSamples, 
							frequencyChangePerSample, depthChangePerSample, envelopeChange );
			break;
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Wave Generation Method
//############		
//##########################################################################################
//##########################################################################################




template < Sample32f (*waveFunction)( Float ) >
void Tremolo:: modulate( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
						Float frequencyChangePerSample, Gain depthChangePerSample, Gain envelopeChange )
{
	// Compute the constant part of how much the phase of the wave changes per sample.
	const Float twoPi = Float(2)*math::pi<Float>();
	const Float phaseIncrementConstant = Float(twoPi / outputBuffer.getSampleRate());
	
	// Declare current parameter interpolation state variables outside of the processing
	// loop so that we can get their final values.
	Float currentFrequency;
	Gain currentDepth;
	Float currentPhase;
	Gain currentEnvelope;
	
	const Size numChannels = outputBuffer.getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		// Get loop invariants.
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		// Reset the current interpolated parameters for this channel.
		currentFrequency = frequency;
		currentDepth = depth;
		currentPhase = phase;
		currentEnvelope = envelope[c];
		
		Float phaseOffset = c < channelPhase.getSize() ? channelPhase[c] : globalChannelPhase;
		
		if ( phaseOffset < Float(0) )
			phaseOffset += twoPi;
		
		currentPhase += phaseOffset;
		
		// Compute the starting phase increment for the current frequency.
		Float phaseIncrement = phaseIncrementConstant*currentFrequency;
		
		// Generate the wave samples and interpolate parameters.
		while ( output != outputEnd )
		{
			Float wave = waveFunction( currentPhase );
			
			currentEnvelope = envelopeChange*(currentEnvelope - wave) + wave;
			Gain currentGain = (Float(1) - currentEnvelope) + currentDepth*currentEnvelope;
			
			*output = currentGain*(*input);
			
			// Update parameter interpolation state.
			currentDepth += depthChangePerSample;
			currentFrequency += frequencyChangePerSample;
			currentPhase += phaseIncrement;
			phaseIncrement = phaseIncrementConstant*currentFrequency;
			
			// Update the input and output pointers.
			input++;
			output++;
		}
		
		currentPhase -= phaseOffset;
		envelope[c] = currentEnvelope;
	}
	
	
	// Save the final output gain, frequency and phase values for the next buffer output frame.
	frequency = currentFrequency;
	depth = currentDepth;
	
	// Make sure that the phase stays in a constrained range due to floating point
	// precision problems which can cause frequency drift.
	phase = math::mod( currentPhase, twoPi );
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
