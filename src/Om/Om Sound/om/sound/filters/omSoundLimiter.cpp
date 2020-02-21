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

#include "omSoundLimiter.h"


#define PARAMETER_INDEX_INPUT_GAIN 0
#define PARAMETER_INDEX_OUTPUT_GAIN 1
#define PARAMETER_INDEX_THRESHOLD 2
#define PARAMETER_INDEX_KNEE 3
#define PARAMETER_INDEX_ATTACK 4
#define PARAMETER_INDEX_RELEASE 5
#define PARAMETER_INDEX_CHANNEL_LINK 6
#define PARAMETER_INDEX_SATURATE_OUTPUT 7
#define PARAMETER_INDEX_SATURATION_KNEE 8
#define PARAMETER_INDEX_GAIN_REDUCTION 9
#define PARAMETER_COUNT 10

#define PARAMETER_NAME_INPUT_GAIN "Input Gain"
#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"
#define PARAMETER_NAME_THRESHOLD "Threshold"
#define PARAMETER_NAME_KNEE "Knee"
#define PARAMETER_NAME_ATTACK "Attack"
#define PARAMETER_NAME_RELEASE "Release"
#define PARAMETER_NAME_CHANNEL_LINK "Link Channels"
#define PARAMETER_NAME_SATURATE_OUTPUT "Saturate Output"
#define PARAMETER_NAME_SATURATION_KNEE "Saturation Knee"
#define PARAMETER_NAME_GAIN_REDUCTION "Gain Reduction"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Limiter:: NAME( "Limiter" );
const UTF8String Limiter:: MANUFACTURER( "Om Sound" );
const FilterVersion Limiter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Limiter:: Limiter()
	:	SoundFilter( 1, 1 ),
		threshold( 1.0f ),
		targetThreshold( 1.0f ),
		inputGain( 1.0f ),
		targetInputGain( 1.0f ),
		outputGain( 1.0f ),
		targetOutputGain( 1.0f ),
		knee( 3.0f ),
		targetKnee( 3.0f ),
		attack( 0.0f ),
		release( 0.005f ),
		saturationKnee( 1.0f ),
		targetSaturationKnee( 1.0f ),
		currentReduction( 0.0f ),
		linkChannels( false ),
		saturateOutput( false )
{
}




Limiter:: Limiter( Gain newThreshold, Gain newInputGain, Gain newOutputGain, Float newRelease )
	:	SoundFilter( 1, 1 ),
		threshold( math::max( newThreshold, Gain(0) ) ),
		inputGain( 1.0f ),
		targetInputGain( 1.0f ),
		outputGain( 1.0f ),
		targetOutputGain( 1.0f ),
		knee( 3.0f ),
		targetKnee( 3.0f ),
		attack( 0.0f ),
		release( math::max( newRelease, Float(0) ) ),
		saturationKnee( 1.0f ),
		targetSaturationKnee( 1.0f ),
		currentReduction( 0.0f ),
		linkChannels( false ),
		saturateOutput( false )
{
	targetThreshold = threshold;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Limiter:: getName() const
{
	return NAME;
}




UTF8String Limiter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Limiter:: getVersion() const
{
	return VERSION;
}




FilterCategory Limiter:: getCategory() const
{
	return FilterCategory::DYNAMICS;
}




Bool Limiter:: allowsInPlaceProcessing() const
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




Size Limiter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Limiter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_INPUT_GAIN, PARAMETER_NAME_INPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 30.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_GAIN, PARAMETER_NAME_OUTPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 30.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_THRESHOLD:
			info = FilterParameterInfo( PARAMETER_INDEX_THRESHOLD, PARAMETER_NAME_THRESHOLD,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, 6.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_KNEE:
			info = FilterParameterInfo( PARAMETER_INDEX_KNEE, PARAMETER_NAME_KNEE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::SQUARE,
										0.0f, 24.0f, 3.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_ATTACK:
			info = FilterParameterInfo( PARAMETER_INDEX_ATTACK, PARAMETER_NAME_ATTACK,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::SQUARE,
										0.0f, 10.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_RELEASE:
			info = FilterParameterInfo( PARAMETER_INDEX_RELEASE, PARAMETER_NAME_RELEASE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::SQUARE,
										5.0f, 200.0f, 5.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_CHANNEL_LINK:
			info = FilterParameterInfo( PARAMETER_INDEX_CHANNEL_LINK, PARAMETER_NAME_CHANNEL_LINK,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_SATURATE_OUTPUT:
			info = FilterParameterInfo( PARAMETER_INDEX_SATURATE_OUTPUT, PARAMETER_NAME_SATURATE_OUTPUT,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_SATURATION_KNEE:
			info = FilterParameterInfo( PARAMETER_INDEX_SATURATION_KNEE, PARAMETER_NAME_SATURATION_KNEE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::SQUARE,
										0.01f, 24.0f, 24.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_GAIN_REDUCTION:
			info = FilterParameterInfo( PARAMETER_INDEX_GAIN_REDUCTION, PARAMETER_NAME_GAIN_REDUCTION,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										0.0f, 50.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS );
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




Bool Limiter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_GAIN:
			value = FilterParameter( this->getInputGainDB() );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			value = FilterParameter( this->getOutputGainDB() );
			return true;
		case PARAMETER_INDEX_THRESHOLD:
			value = FilterParameter( this->getThresholdDB() );
			return true;
		case PARAMETER_INDEX_KNEE:
			value = FilterParameter( this->getKnee() );
			return true;
		case PARAMETER_INDEX_ATTACK:
			value = FilterParameter( this->getAttack()*1000.0f );
			return true;
		case PARAMETER_INDEX_RELEASE:
			value = FilterParameter( this->getRelease()*1000.0f );
			return true;
		case PARAMETER_INDEX_CHANNEL_LINK:
			value = FilterParameter( this->getChannelsAreLinked() );
			return true;
		case PARAMETER_INDEX_SATURATE_OUTPUT:
			value = FilterParameter( this->getSaturationIsEnabled() );
			return true;
		case PARAMETER_INDEX_SATURATION_KNEE:
			value = FilterParameter( this->getSaturationKnee() );
			return true;
		case PARAMETER_INDEX_GAIN_REDUCTION:
			value = FilterParameter( this->getGainReductionDB() );
			return true;
	}
	
	return false;
}




Bool Limiter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
	Float floatValue;
	Bool booleanValue;
	
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setInputGainDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setOutputGainDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_THRESHOLD:
			if ( value.getValue( floatValue ) )
			{
				this->setThresholdDB( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_KNEE:
			if ( value.getValue( floatValue ) )
			{
				this->setKnee( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_ATTACK:
			if ( value.getValue( floatValue ) )
			{
				this->setAttack( floatValue*0.001f );
				return true;
			}
			break;
		case PARAMETER_INDEX_RELEASE:
			if ( value.getValue( floatValue ) )
			{
				this->setRelease( floatValue*0.001f );
				return true;
			}
			break;
		case PARAMETER_INDEX_CHANNEL_LINK:
			if ( value.getValue( booleanValue ) )
			{
				this->setChannelsAreLinked( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_SATURATE_OUTPUT:
			if ( value.getValue( booleanValue ) )
			{
				this->setSaturationIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_SATURATION_KNEE:
			if ( value.getValue( floatValue ) )
			{
				this->setSaturationKnee( floatValue );
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




void Limiter:: resetStream()
{
	// Reset the envelope.
	envelope.setAll( 0 );
	currentReduction = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Limiter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Limiter:: processFrame( const SoundFrame& inputFrame,
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
	// Compute basic compression parameters.
	
	// Get data about the input buffer.
	const Size numChannels = inputBuffer->getChannelCount();
	
	// Make sure that there are enough channels in the envelope data.
	if ( envelope.getSize() < numChannels )
		envelope.setSize( numChannels );
	
	// Compute the envelope attack and release parameters.
	const Gain envelopeAttack = math::pow( Float(0.3), Float(1.0/(attack*sampleRate)) );
	const Gain envelopeRelease = math::pow( Float(0.5), Float(1.0/(release*sampleRate)) );
	
	//*************************************************************************************
	// Compute how much certain parameters have changed since the last buffer frame.
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		inputGain = targetInputGain;
		outputGain = targetOutputGain;
		threshold = targetThreshold;
		knee = targetKnee;
		saturationKnee = targetSaturationKnee;
	}
	
	const Float inverseNumSamples = Float(1) / Float(numSamples);
	
	// Compute how much the interpolated parameters will change for each sample.
	const Gain inputGainChangePerSample = Gain(0.5)*(targetInputGain - inputGain)*inverseNumSamples;
	const Gain outputGainChangePerSample = Gain(0.5)*(targetOutputGain - outputGain)*inverseNumSamples;
	const Gain thresholdChangePerSample = Gain(0.5)*(targetThreshold - threshold)*inverseNumSamples;
	const Gain kneeChangePerSample = Gain(0.5)*(targetKnee - knee)*inverseNumSamples;
	const Gain saturationKneeChangePerSample = Gain(0.5)*(targetSaturationKnee - saturationKnee)*inverseNumSamples;
	
	//*************************************************************************************
	
	const Gain gainChangeThreshold = math::epsilon<Gain>()*Gain(100);
	
	if ( math::abs(inputGainChangePerSample) < gainChangeThreshold &&
		math::abs(outputGainChangePerSample) < gainChangeThreshold &&
		math::abs(thresholdChangePerSample) < gainChangeThreshold &&
		math::abs(kneeChangePerSample) < gainChangeThreshold &&
		math::abs(saturationKneeChangePerSample) < gainChangeThreshold )
	{
		// Use a limiting method that doesn't interpolate parameters.
		if ( saturateOutput )
		{
			limit<false,true>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
							inputGainChangePerSample, outputGainChangePerSample, thresholdChangePerSample,
							kneeChangePerSample, saturationKneeChangePerSample );
		}
		else
		{
			limit<false,false>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
								inputGainChangePerSample, outputGainChangePerSample, thresholdChangePerSample,
								kneeChangePerSample, saturationKneeChangePerSample );
		}
	}
	else
	{
		// At least one of the parameters changed, use the most expensive processing method.
		if ( saturateOutput )
		{
			limit<true,true>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
							inputGainChangePerSample, outputGainChangePerSample, thresholdChangePerSample,
							kneeChangePerSample, saturationKneeChangePerSample );
		}
		else
		{
			limit<true,false>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
								inputGainChangePerSample, outputGainChangePerSample, thresholdChangePerSample,
								kneeChangePerSample, saturationKneeChangePerSample );
		}
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Limiting Processing Method
//############		
//##########################################################################################
//##########################################################################################




template < Bool interpolateChanges, Bool saturationEnabled >
void Limiter:: limit( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
					Gain envelopeAttack, Gain envelopeRelease,
					Gain inputGainChangePerSample, Gain outputGainChangePerSample,
					Gain thresholdChangePerSample, Gain kneeChangePerSample, Gain saturationKneeChangePerSample )
{
	const Size numChannels = inputBuffer.getChannelCount();
	
	// The total number of samples (from all channels) where gain reduction was applied.
	Size numReductionSamples = 0;
	
	// The sum of all gain reduction that has been applied, in decibels.
	// This is used to calculate the average gain reduction for the processing frame.
	Gain reductionTotal = 0;
	
	//****************************************************************************
	
	if ( linkChannels )
	{
		Gain currentInputGain = inputGain;
		Gain currentOutputGain = outputGain;
		Gain currentThreshold = threshold;
		Gain currentKnee = knee;
		Gain currentSaturationKnee = saturationKnee;
		
		// Temporary variables that help apply the knee and gain reduction.
		Gain linearKnee, kneeMin, kneeMax;
		
		// Declare dependent saturation parameters.
		Gain linearSaturationKnee, saturationThreshold, inverseSaturationHardness,
			saturationHardness, saturationOffset;
		
		if ( !interpolateChanges )
		{
			// Compute the minimum and maximum knee thresholds on a linear scale.
			linearKnee = math::dbToLinear( currentKnee );
			kneeMin = currentThreshold/linearKnee;
			kneeMax = currentThreshold*linearKnee;
			
			linearSaturationKnee = math::dbToLinear( currentSaturationKnee );
			saturationThreshold = 1 / linearSaturationKnee;
			inverseSaturationHardness = Gain(1) - saturationThreshold;
			saturationHardness = Gain(1) / inverseSaturationHardness;
			saturationOffset = saturationHardness*saturationThreshold;
		}
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			Gain maxReduction = 0;
			
			// Recompute the knee constants for this sample.
			if ( interpolateChanges )
			{
				// Compute the minimum and maximum knee thresholds on a linear scale.
				linearKnee = math::dbToLinear( currentKnee );
				kneeMin = currentThreshold/linearKnee;
				kneeMax = currentThreshold*linearKnee;
				
				linearSaturationKnee = math::dbToLinear( currentSaturationKnee );
				saturationThreshold = 1 / linearSaturationKnee;
				inverseSaturationHardness = Gain(1) - saturationThreshold;
				saturationHardness = Gain(1) / inverseSaturationHardness;
				saturationOffset = saturationHardness*saturationThreshold;
			}
			
			// Find the highest amount of gain reduction for all channels.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Float inputSample = inputBuffer.getChannel(c)[i]*currentInputGain;
				
				// Compute the level for this sample.
				Float level = math::abs( inputSample );
				Float& e = envelope[c];
				
				// Update the envelope level for this sample.
				if ( level > e )
					e = level + envelopeAttack*(e - level);
				else
					e = level + envelopeRelease*(e - level);
				
				//****************************************************************************
				
				// Detect if the envelope is over the threshold, and if so, apply gain reduction.
				if ( e > kneeMin )
				{
					// Compute the reduction for this channel.
					Gain dbReduction = getDBReduction( e, currentThreshold, kneeMin, kneeMax, currentKnee );
					
					// Find the largest (most negative) reduction amount.
					maxReduction = math::min( maxReduction, dbReduction );
				}
			}
			
			//****************************************************************************
			// Compute the output for this sample.
			
			// Add the current reduction in dB to the total reduction.
			reductionTotal += maxReduction;
			numReductionSamples++;
			
			// Compute the final gain which should be applied to all channels.
			Gain finalGain = math::dbToLinear( maxReduction );
			
			// Apply the final gain to all channels.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Float outputSample = inputBuffer.getChannel(c)[i]*inputGain*finalGain;
				
				if ( saturationEnabled )
				{
					outputSample /= currentThreshold;
					
					if ( outputSample > saturationThreshold )
					{
						outputSample = inverseSaturationHardness*
										math::tanh(saturationHardness*outputSample - saturationOffset) + saturationThreshold;
					}
					else if ( outputSample < -saturationThreshold )
					{
						outputSample = inverseSaturationHardness*
										math::tanh(saturationHardness*outputSample + saturationOffset) - saturationThreshold;
					}
					
					outputSample *= currentThreshold;
				}
				
				outputBuffer.getChannel(c)[i] = currentOutputGain*outputSample;
			}
			
			//****************************************************************************
			// Increment values that need updating.
			
			// Update the current state for interpolated parameters.
			if ( interpolateChanges )
			{
				currentInputGain += inputGainChangePerSample;
				currentOutputGain += outputGainChangePerSample;
				currentThreshold += thresholdChangePerSample;
				currentKnee += kneeChangePerSample;
				currentSaturationKnee += saturationKneeChangePerSample;
			}
		}
		
		// Store the final values for interpolated parameters.
		if ( interpolateChanges )
		{
			inputGain = currentInputGain;
			outputGain = currentOutputGain;
			threshold = currentThreshold;
			knee = currentKnee;
			saturationKnee = currentSaturationKnee;
		}
	}
	else
	{
		Gain currentInputGain, currentOutputGain, currentThreshold, currentKnee, currentSaturationKnee;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Sample32f* input = inputBuffer.getChannel(c);
			const Sample32f* const inputEnd = input + numSamples;
			Sample32f* output = outputBuffer.getChannel(c);
			Float& e = envelope[c];
			
			currentInputGain = inputGain;
			currentOutputGain = outputGain;
			currentThreshold = threshold;
			currentKnee = knee;
			currentSaturationKnee = saturationKnee;
			
			// Declare dependent saturation parameters.
			Gain linearSaturationKnee, saturationThreshold, inverseSaturationHardness,
				saturationHardness, saturationOffset;
			
			// Compute dependent saturation parameters.
			if ( !interpolateChanges )
			{
				linearSaturationKnee = math::dbToLinear( currentSaturationKnee );
				saturationThreshold = 1 / linearSaturationKnee;
				inverseSaturationHardness = Gain(1) - saturationThreshold;
				saturationHardness = Gain(1) / inverseSaturationHardness;
				saturationOffset = saturationHardness*saturationThreshold;
			}
			
			//****************************************************************************
			
			while ( input != inputEnd )
			{
				Float inputSample = *input*currentInputGain;
				
				// Compute the level for this sample.
				Float level = math::abs( inputSample );
				
				// Update the envelope level for this sample.
				if ( level > e )
					e = level + envelopeAttack*(e - level);
				else
					e = level + envelopeRelease*(e - level);
				
				//****************************************************************************
				
				// Compute the minimum and maximum knee thresholds on a linear scale.
				Gain linearKnee = math::dbToLinear( currentKnee );
				Gain kneeMin = currentThreshold/linearKnee;
				
				Float outputSample = inputSample;
				
				// Detect if the envelope is over the threshold, and if so, apply gain reduction.
				if ( e >= kneeMin )
				{
					Gain kneeMax = currentThreshold*linearKnee;
					Gain dbReduction = getDBReduction( e, currentThreshold, kneeMin, kneeMax, currentKnee );
					
					outputSample = inputSample*math::dbToLinear( dbReduction );
					
					// Add the current reduction in dB to the total reduction.
					reductionTotal += dbReduction;
				}
				
				// Update dependent saturation parameters.
				if ( interpolateChanges )
				{
					linearSaturationKnee = math::dbToLinear( currentSaturationKnee );
					saturationThreshold = 1 / linearSaturationKnee;
					inverseSaturationHardness = Gain(1) - saturationThreshold;
					saturationHardness = Gain(1) / inverseSaturationHardness;
					saturationOffset = saturationHardness*saturationThreshold;
				}
				
				// Saturate the output of the limiter if necessary.
				if ( saturationEnabled )
				{
					outputSample /= currentThreshold;
					
					if ( outputSample > saturationThreshold )
					{
						outputSample = inverseSaturationHardness*
										math::tanh(saturationHardness*outputSample - saturationOffset) + saturationThreshold;
					}
					else if ( outputSample < -saturationThreshold )
					{
						outputSample = inverseSaturationHardness*
										math::tanh(saturationHardness*outputSample + saturationOffset) - saturationThreshold;
					}
					
					outputSample *= currentThreshold;
				}
				
				*output = currentOutputGain*outputSample;
				
				numReductionSamples++;
				input++;
				output++;
				
				// Update the current state for interpolated parameters.
				if ( interpolateChanges )
				{
					currentInputGain += inputGainChangePerSample;
					currentOutputGain += outputGainChangePerSample;
					currentThreshold += thresholdChangePerSample;
					currentKnee += kneeChangePerSample;
					currentSaturationKnee += saturationKneeChangePerSample;
				}
			}
		}
		
		// Store the final values for interpolated parameters.
		if ( interpolateChanges )
		{
			inputGain = currentInputGain;
			outputGain = currentOutputGain;
			threshold = currentThreshold;
			knee = currentKnee;
			saturationKnee = currentSaturationKnee;
		}
	}
	
	// Update the current average gain reduction amount.
	if ( numReductionSamples > 0 )
		currentReduction = -reductionTotal / numReductionSamples;
	else
		currentReduction = 0;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
