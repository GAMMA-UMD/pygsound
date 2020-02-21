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

#include "omSoundFlanger.h"


#define PARAMETER_INDEX_FREQUENCY 0
#define PARAMETER_INDEX_DEPTH 1
#define PARAMETER_INDEX_FEEDBACK 2
#define PARAMETER_INDEX_MIX 3
#define PARAMETER_INDEX_CHANNEL_PHASE_0 4
#define PARAMETER_INDEX_CHANNEL_PHASE_1 5
#define PARAMETER_INDEX_CHANNEL_PHASE_2 6
#define PARAMETER_INDEX_CHANNEL_PHASE_3 7
#define PARAMETER_INDEX_CHANNEL_PHASE_4 8
#define PARAMETER_INDEX_CHANNEL_PHASE_5 9
#define PARAMETER_INDEX_CHANNEL_PHASE_6 10
#define PARAMETER_INDEX_CHANNEL_PHASE_7 11
#define PARAMETER_COUNT 12


#define PARAMETER_NAME_FREQUENCY "Frequency"
#define PARAMETER_NAME_DEPTH "Depth"
#define PARAMETER_NAME_FEEDBACK "Feedback"
#define PARAMETER_NAME_MIX "Mix"
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


const UTF8String Flanger:: NAME( "Flanger" );
const UTF8String Flanger:: MANUFACTURER( "Om Sound" );
const FilterVersion Flanger:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Flanger:: Flanger()
	:	SoundFilter( 1, 1 ),
		frequency( 0.1f ),
		targetFrequency( 0.1f ),
		depth( 0.5f ),
		targetDepth( 0.5f ),
		feedback( 0.5f ),
		targetFeedback( 0.5f ),
		mix( 1.0f ),
		targetMix( 1.0f ),
		globalChannelPhase( 0 ),
		phase( 0 ),
		maxDelayTime( 0.015f ),
		currentDelayWriteIndex( 0 ),
		delayBufferSize( 0 )
{
}




Flanger:: Flanger( Float newFrequency, Float newDepth, Gain newFeedback, Gain newMix  )
	:	SoundFilter( 1, 1 ),
		frequency( math::max( newFrequency, Float(0) ) ),
		depth( math::clamp( newDepth, Float(0), Float(1) ) ),
		feedback( math::clamp( newFeedback, Gain(-1), Gain(1) ) ),
		mix( math::clamp( newMix, Gain(0), Gain(1) ) ),
		targetMix( 0.5 ),
		globalChannelPhase( 0 ),
		phase( 0 ),
		maxDelayTime( 0.015f ),
		currentDelayWriteIndex( 0 ),
		delayBufferSize( 0 )
{
	targetFrequency = frequency;
	targetDepth = depth;
	targetFeedback = feedback;
	targetMix = mix;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Phase Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void Flanger:: setChannelPhase( Index channelIndex, Float newPhase )
{
	lockMutex();
	
	// Enlarge the size of the channel phase array if necessary.
	if ( channelIndex >= channelPhase.getSize() )
		channelPhase.setSize( channelIndex + 1, globalChannelPhase );
	
	// Santize and convert the input value to radians.
	channelPhase[channelIndex] = math::degreesToRadians( math::clamp( newPhase, Float(-180), Float(180) ) );
	
	unlockMutex();
}




void Flanger:: setChannelPhase( Float newPhase )
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
//############		Flanger Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Flanger:: getName() const
{
	return NAME;
}




UTF8String Flanger:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Flanger:: getVersion() const
{
	return VERSION;
}




FilterCategory Flanger:: getCategory() const
{
	return FilterCategory::MODULATION;
}




Bool Flanger:: allowsInPlaceProcessing() const
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Latency Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Time Flanger:: getLatency() const
{
	return Time( Double(maxDelayTime*0.5) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################





Size Flanger:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Flanger:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY, PARAMETER_NAME_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::CUBE,
										0.05f, 50.0f, 0.1f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DEPTH:
			info = FilterParameterInfo( PARAMETER_INDEX_DEPTH, PARAMETER_NAME_DEPTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::LINEAR,
										0.0f, 100.0f, 50.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FEEDBACK:
			info = FilterParameterInfo( PARAMETER_INDEX_FEEDBACK, PARAMETER_NAME_FEEDBACK,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::LINEAR,
										-100.0f, 100.0f, 50.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_MIX:
			info = FilterParameterInfo( PARAMETER_INDEX_MIX, PARAMETER_NAME_MIX,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::LINEAR,
										0.0f, 100.0f, 100.0f,
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




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Value Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Flanger:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_FREQUENCY:
			value = FilterParameter( this->getFrequency() );
			return true;
		case PARAMETER_INDEX_DEPTH:
			value = FilterParameter( this->getDepth()*100.0f );
			return true;
		case PARAMETER_INDEX_FEEDBACK:
			value = FilterParameter( this->getFeedback()*100.0f );
			return true;
		case PARAMETER_INDEX_MIX:
			value = FilterParameter( this->getMix()*100.0f );
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




Bool Flanger:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
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
		case PARAMETER_INDEX_DEPTH:
			if ( value.getValue( floatValue ) )
			{
				this->setDepth( floatValue*0.01f );
				return true;
			}
			break;
		case PARAMETER_INDEX_FEEDBACK:
			if ( value.getValue( gainValue ) )
			{
				this->setFeedback( gainValue*0.01f );
				return true;
			}
			break;
		case PARAMETER_INDEX_MIX:
			if ( value.getValue( gainValue ) )
			{
				this->setMix( gainValue*0.01f );
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
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Flanger:: processFrame( const SoundFrame& inputFrame,
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
	
	if ( channelPhase.getSize() < numChannels )
		channelPhase.setSize( numChannels, globalChannelPhase );
	
	// Make sure the delay buffer has the right size for the current sample rate.
	delayBufferSize = math::max( Size(2*maxDelayTime*sampleRate), Size(1) );
	Size halfDelayBufferSize = delayBufferSize / 2;
	Size quarterDelayBufferSize = delayBufferSize / 4;
	
	if ( delayBuffer.getSize() < delayBufferSize )
		delayBuffer.setSize( delayBufferSize );
	
	if ( delayBuffer.getChannelCount() < numChannels )
		delayBuffer.setChannelCount( numChannels );
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		frequency = targetFrequency;
		depth = targetDepth;
		feedback = targetFeedback;
		mix = targetMix;
		phase = 0;
		
		currentDelayWriteIndex = 0;
		
		// Zero the delay buffer.
		delayBuffer.zero();
	}
	
	Float inverseNumSamples = Float(1) / numSamples;
	
	// Compute the change in some parameters.
	Float frequencyChangePerSample = Float(0.5)*(targetFrequency - frequency)*inverseNumSamples;
	Gain depthChangePerSample = Gain(0.5)*(targetDepth - depth)*inverseNumSamples;
	Float feedbackChangePerSample = Gain(0.5)*(targetFeedback - feedback)*inverseNumSamples;
	Gain mixChangePerSample = Gain(0.5)*(targetMix - mix)*inverseNumSamples;
	
	
	// Compute the constant part of how much the phase of the wave changes per sample.
	const Float twoPi = Float(2)*math::pi<Float>();
	const Float phaseIncrementConstant = Float(twoPi / sampleRate);
	
	// Declare current parameter interpolation state variables outside of the processing
	// loop so that we can get their final values.
	Float currentFrequency;
	Float currentDepth;
	Gain currentFeedback;
	Gain currentMix;
	Float currentPhase;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		// Compute read/write sample pointers.
		const Sample32f* input = inputBuffer->getChannel(c);
		Sample32f* output = outputBuffer->getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		Sample32f* const delayStart = delayBuffer.getChannel(c);
		const Sample32f* const delayEnd = delayStart + delayBufferSize;
		
		Index currentDelayWrite = currentDelayWriteIndex;
		
		// Reset the current interpolated parameters for this channel.
		currentFrequency = frequency;
		currentDepth = depth;
		currentFeedback = feedback;
		currentMix = mix;
		currentPhase = phase;
		
		Float phaseOffset = c < channelPhase.getSize() ? channelPhase[c] : globalChannelPhase;
		
		if ( phaseOffset < Float(0) )
			phaseOffset += twoPi;
		
		currentPhase += phaseOffset;
		
		// Compute the starting phase increment for the current frequency.
		Float phaseIncrement = phaseIncrementConstant*currentFrequency;
		
		// Generate the wave samples and interpolate parameters.
		while ( output != outputEnd )
		{
			// Compute the current modulation envelope.
			Float wave = cosine( currentPhase );
			
			// Compute the current delay read position.
			Float currentDelay = halfDelayBufferSize*currentDepth*wave;
			Float a = currentDelay - math::floor(currentDelay);
			
			Index delayReadIndex = (((currentDelayWrite + delayBufferSize) - Size(currentDelay)) - quarterDelayBufferSize) % delayBufferSize;
			
			const Sample32f* delayRead1 = delayStart + delayReadIndex;
			const Sample32f* delayRead2 = delayRead1 + 1;
			
			if ( delayRead2 >= delayEnd )
				delayRead2 = delayStart;
			
			// Read the output sample from the interpolated position.
			Sample32f delaySample = a*(*delayRead1) + (1 - a)*(*delayRead2);
			
			// Write the input sample to the current delay write position.
			delayStart[currentDelayWrite % delayBufferSize] = (*input) + currentFeedback*delaySample;
			
			*output = currentMix*delaySample + (Gain(1) - currentMix)*(*input);
			
			// Update parameter interpolation state.
			currentFrequency += frequencyChangePerSample;
			currentDepth += depthChangePerSample;
			currentFeedback += feedbackChangePerSample;
			currentMix += mixChangePerSample;
			
			currentPhase += phaseIncrement;
			phaseIncrement = phaseIncrementConstant*currentFrequency;
			
			// Update the input and output pointers.
			input++;
			output++;
			currentDelayWrite++;
			
			// Make sure the delay write position isn't past the end of the delay buffer.
			if ( currentDelayWrite == delayBufferSize )
				currentDelayWrite = 0;
		}
		
		currentPhase -= phaseOffset;
	}
	
	currentDelayWriteIndex = (currentDelayWriteIndex + numSamples) % delayBufferSize;
	
	// Save the final output gain, frequency and phase values for the next buffer output frame.
	frequency = currentFrequency;
	depth = currentDepth;
	feedback = currentFeedback;
	mix = currentMix;
	
	// Make sure that the phase stays in a constrained range due to floating point
	// precision problems which can cause frequency drift.
	phase = math::mod( currentPhase, twoPi );
	
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
