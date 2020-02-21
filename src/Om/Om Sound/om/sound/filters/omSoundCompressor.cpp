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

#include "omSoundCompressor.h"


#define PARAMETER_INDEX_INPUT_GAIN 0
#define PARAMETER_INDEX_OUTPUT_GAIN 1
#define PARAMETER_INDEX_MIX 2
#define PARAMETER_INDEX_THRESHOLD 3
#define PARAMETER_INDEX_RATIO 4
#define PARAMETER_INDEX_KNEE 5
#define PARAMETER_INDEX_ATTACK 6
#define PARAMETER_INDEX_RELEASE 7
#define PARAMETER_INDEX_RMS_TIME 8
#define PARAMETER_INDEX_CHANNEL_LINK 9
#define PARAMETER_INDEX_GAIN_REDUCTION 10
#define PARAMETER_COUNT 11

#define PARAMETER_NAME_INPUT_GAIN "Input Gain"
#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"
#define PARAMETER_NAME_MIX "Mix"
#define PARAMETER_NAME_THRESHOLD "Threshold"
#define PARAMETER_NAME_RATIO "Ratio"
#define PARAMETER_NAME_KNEE "Knee"
#define PARAMETER_NAME_ATTACK "Attack"
#define PARAMETER_NAME_RELEASE "Release"
#define PARAMETER_NAME_RMS_TIME "RMS Time"
#define PARAMETER_NAME_CHANNEL_LINK "Link Channels"
#define PARAMETER_NAME_GAIN_REDUCTION "Gain Reduction"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Compressor:: NAME( "Compressor" );
const UTF8String Compressor:: MANUFACTURER( "Om Sound" );
const FilterVersion Compressor:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Compressor:: Compressor()
	:	SoundFilter( 2, 1 ),
		threshold( 1.0f ),
		targetThreshold( 1.0f ),
		ratio( 2.0f ),
		targetRatio( 2.0f ),
		knee( 0.0f ),
		targetKnee( 0.0f ),
		inputGain( 1.0f ),
		targetInputGain( 1.0f ),
		outputGain( 1.0f ),
		targetOutputGain( 1.0f ),
		mix( 1.0f ),
		targetMix( 1.0f ),
		attack( 0.015f ),
		release( 0.05f ),
		rmsTime( 0.0f ),
		rmsLengthInSamples( 0 ),
		currentRMSIndex( 0 ),
		currentReduction( 0 ),
		linkChannels( false )
{
}




Compressor:: Compressor( Gain newThreshold, Float newRatio, Float newAttack, Float newRelease )
	:	SoundFilter( 2, 1 ),
		threshold( math::max( newThreshold, Gain(0) ) ),
		ratio( math::max( newRatio, Float(1) ) ),
		knee( 0.0f ),
		targetKnee( 0.0f ),
		inputGain( 1.0f ),
		targetInputGain( 1.0f ),
		outputGain( 1.0f ),
		targetOutputGain( 1.0f ),
		mix( 1.0f ),
		targetMix( 1.0f ),
		attack( math::max( newAttack, Float(0) ) ),
		release( math::max( newRelease, Float(0) ) ),
		rmsTime( 0.0f ),
		rmsLengthInSamples( 0 ),
		currentRMSIndex( 0 ),
		currentReduction( 0 ),
		linkChannels( false )
{
	targetThreshold = threshold;
	targetRatio = ratio;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Transfer Function Evaluation Method
//############		
//##########################################################################################
//##########################################################################################




Gain Compressor:: evaluateTransferFunction( Gain e ) const
{
	// Compute the minimum and maximum knee thresholds on a linear scale.
	Gain kneeMin = math::dbToLinear( math::linearToDB( threshold ) - knee );
	Gain kneeMax = math::dbToLinear( math::linearToDB( threshold ) + knee );
	
	if ( e < kneeMin )
		return e*outputGain;
	else
	{
		Gain dbReduction = getDBReduction( e, threshold, ratio, kneeMin, kneeMax, knee );
		
		return e*outputGain*math::dbToLinear( dbReduction );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Input and Output Name Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Compressor:: getInputName( Index inputIndex ) const
{
	if ( inputIndex == 0 )
		return UTF8String("Main Input");
	else if ( inputIndex == 1 )
		return UTF8String("Sidechain");
	else
		return UTF8String();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Compressor:: getName() const
{
	return NAME;
}




UTF8String Compressor:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Compressor:: getVersion() const
{
	return VERSION;
}




FilterCategory Compressor:: getCategory() const
{
	return FilterCategory::DYNAMICS;
}




Bool Compressor:: allowsInPlaceProcessing() const
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




Size Compressor:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Compressor:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
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
		case PARAMETER_INDEX_MIX:
			info = FilterParameterInfo( PARAMETER_INDEX_MIX, PARAMETER_NAME_MIX,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::LINEAR,
										0.0f, 100.0f, 100.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_THRESHOLD:
			info = FilterParameterInfo( PARAMETER_INDEX_THRESHOLD, PARAMETER_NAME_THRESHOLD,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, 6.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_RATIO:
			info = FilterParameterInfo( PARAMETER_INDEX_RATIO, PARAMETER_NAME_RATIO,
										FilterParameterType::FLOAT,
										FilterParameterUnits::RATIO, FilterParameterCurve::SQUARE,
										1.0f, 10.0f, 2.0f,
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
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::CUBE,
										0.5f, 2000.0f, 15.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_RELEASE:
			info = FilterParameterInfo( PARAMETER_INDEX_RELEASE, PARAMETER_NAME_RELEASE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::CUBE,
										5.0f, 5000.0f, 20.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_RMS_TIME:
			info = FilterParameterInfo( PARAMETER_INDEX_RMS_TIME, PARAMETER_NAME_RMS_TIME,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::SQUARE,
										0.0f, 200.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_CHANNEL_LINK:
			info = FilterParameterInfo( PARAMETER_INDEX_CHANNEL_LINK, PARAMETER_NAME_CHANNEL_LINK,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
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




Bool Compressor:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_GAIN:
			value = FilterParameter( this->getInputGainDB() );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			value = FilterParameter( this->getOutputGainDB() );
			return true;
		case PARAMETER_INDEX_MIX:
			value = FilterParameter( this->getMix()*100.0f );
			return true;
		case PARAMETER_INDEX_THRESHOLD:
			value = FilterParameter( this->getThresholdDB() );
			return true;
		case PARAMETER_INDEX_RATIO:
			value = FilterParameter( this->getRatio() );
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
		case PARAMETER_INDEX_RMS_TIME:
			value = FilterParameter( this->getRMSTime()*1000.0f );
			return true;
		case PARAMETER_INDEX_CHANNEL_LINK:
			value = FilterParameter( this->getChannelsAreLinked() );
			return true;
		case PARAMETER_INDEX_GAIN_REDUCTION:
			value = FilterParameter( this->getGainReductionDB() );
			return true;
	}
	
	return false;
}




Bool Compressor:: setParameterValue( Index parameterIndex, const FilterParameter& value )
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
		case PARAMETER_INDEX_MIX:
			if ( value.getValue( floatValue ) )
			{
				this->setMix( floatValue*0.01f );
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
		case PARAMETER_INDEX_RATIO:
			if ( value.getValue( floatValue ) )
			{
				this->setRatio( floatValue );
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
		case PARAMETER_INDEX_RMS_TIME:
			if ( value.getValue( floatValue ) )
			{
				this->setRMSTime( floatValue*0.001f );
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




void Compressor:: resetStream()
{
	// Reset the envelope.
	envelope.setAll( 0 );
	rmsSumSquares.setAll( 0 );
	rmsBuffer.zero();
	currentReduction = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Compressor Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Compressor:: processFrame( const SoundFrame& inputFrame,
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
	// Update the state of the RMS sample buffer.
	
	// Compute the new size of the RMS buffer.
	Size newRMSLengthInSamples = math::max( Size(rmsTime*sampleRate), Size(1) );
	
	// If the size of the buffer has changed, restart the RMS computation.
	if ( newRMSLengthInSamples != rmsLengthInSamples )
	{
		if ( rmsBuffer.getSize() < newRMSLengthInSamples )
			rmsBuffer.setSize( newRMSLengthInSamples );
		
		if ( rmsBuffer.getChannelCount() < numChannels )
			rmsBuffer.setChannelCount( numChannels );
		
		// Zero the output buffer and restart the RMS computation.
		rmsBuffer.zero( 0, newRMSLengthInSamples );
		rmsLengthInSamples = newRMSLengthInSamples;
		rmsSumSquares.setAll( 0 );
		currentRMSIndex = 0;
	}
	
	//*************************************************************************************
	// Compute how much certain parameters have changed since the last buffer frame.
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		inputGain = targetInputGain;
		outputGain = targetOutputGain;
		mix = targetMix;
		threshold = targetThreshold;
		knee = targetKnee;
		ratio = targetRatio;
	}
	
	const Float inverseNumSamples = Float(1) / Float(numSamples);
	
	// Compute how much the interpolated parameters will change for each sample.
	const Gain inputGainChangePerSample = Gain(0.5)*(targetInputGain - inputGain)*inverseNumSamples;
	const Gain outputGainChangePerSample = Gain(0.5)*(targetOutputGain - outputGain)*inverseNumSamples;
	const Gain mixChangePerSample = Gain(0.5)*(targetMix - mix)*inverseNumSamples;
	const Gain thresholdChangePerSample = Gain(0.5)*(targetThreshold - threshold)*inverseNumSamples;
	const Gain kneeChangePerSample = Gain(0.5)*(targetKnee - knee)*inverseNumSamples;
	const Float ratioChangePerSample = Gain(0.5)*(targetRatio - ratio)*inverseNumSamples;
	
	//*************************************************************************************
	
	const Gain gainChangeThreshold = math::epsilon<Gain>()*Gain(100);
	const Float floatChangeThreshold = math::epsilon<Float>()*Float(100);
	
	if ( math::abs(inputGainChangePerSample) < gainChangeThreshold &&
		math::abs(outputGainChangePerSample) < gainChangeThreshold &&
		math::abs(mixChangePerSample) < gainChangeThreshold &&
		math::abs(thresholdChangePerSample) < gainChangeThreshold &&
		math::abs(kneeChangePerSample) < gainChangeThreshold &&
		math::abs(ratioChangePerSample) < floatChangeThreshold )
	{
		// If there is an RMS buffer longer than 1 sample (the minimum), use RMS peak detection.
		// Otherwise, use normal peak detection because it is faster for the same results.
		if ( rmsLengthInSamples > Size(1) )
			compress<false,true>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
								inputGainChangePerSample, outputGainChangePerSample, mixChangePerSample,
								thresholdChangePerSample, kneeChangePerSample, ratioChangePerSample );
		else
			compress<false,false>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
								inputGainChangePerSample, outputGainChangePerSample, mixChangePerSample,
								thresholdChangePerSample, kneeChangePerSample, ratioChangePerSample );
	}
	else
	{
		// At least on of the parameters changed, use the most expensive processing method.
		// We don't differentiate between RMS and peak detection because they are equivalent
		// when the RMS buffer length is 1.
		if ( rmsLengthInSamples > Size(1) )
			compress<true,true>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
								inputGainChangePerSample, outputGainChangePerSample, mixChangePerSample,
								thresholdChangePerSample, kneeChangePerSample, ratioChangePerSample );
		else
			compress<true,false>( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
								inputGainChangePerSample, outputGainChangePerSample, mixChangePerSample,
								thresholdChangePerSample, kneeChangePerSample, ratioChangePerSample );
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Compression Processing Method
//############		
//##########################################################################################
//##########################################################################################




template < Bool interpolateChanges, Bool rmsEnabled >
void Compressor:: compress( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
							Gain envelopeAttack, Gain envelopeRelease,
							Gain inputGainChangePerSample, Gain outputGainChangePerSample,
							Gain mixChangePerSample, Gain thresholdChangePerSample,
							Gain kneeChangePerSample, Float ratioChangePerSample )
{
	const Size numChannels = inputBuffer.getChannelCount();
	Float inverseRMSLength;
	
	if ( rmsEnabled )
	{
		if ( rmsSumSquares.getSize() < numChannels )
			rmsSumSquares.setSize( numChannels );
		
		inverseRMSLength = Float(1) / rmsLengthInSamples;
	}
	
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
		Gain currentMix = mix;
		Gain currentThreshold = threshold;
		Gain currentKnee = knee;
		Float currentRatio = ratio;
		
		// Temporary variables that help apply the knee and gain reduction.
		Float reductionConstant;
		Gain linearKnee, kneeMin, kneeMax;
		
		if ( !interpolateChanges )
		{
			// Compute a constant which converts from gain over the threshold to output gain reduction.
			reductionConstant = (Float(1) - currentRatio)/currentRatio;
			
			// Compute the minimum and maximum knee thresholds on a linear scale.
			linearKnee = math::dbToLinear( currentKnee );
			kneeMin = currentThreshold/linearKnee;
			kneeMax = currentThreshold*linearKnee;
		}
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			Gain maxReduction = 0;
			
			// Recompute the knee and gain reduction constants for this sample.
			if ( interpolateChanges )
			{
				// Compute a constant which converts from gain over the threshold to output gain reduction.
				reductionConstant = (Float(1) - currentRatio)/currentRatio;
				
				// Compute the minimum and maximum knee thresholds on a linear scale.
				linearKnee = math::dbToLinear( currentKnee );
				kneeMin = currentThreshold/linearKnee;
				kneeMax = currentThreshold*linearKnee;
			}
			
			//****************************************************************************
			
			// Find the highest amount of gain reduction for all channels.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Float inputSample = inputBuffer.getChannel(c)[i]*currentInputGain;
				Float level;
				
				if ( rmsEnabled )
				{
					if ( currentRMSIndex >= rmsLengthInSamples )
						currentRMSIndex = 0;
					
					Sample32f* rms = rmsBuffer.getChannel(c) + currentRMSIndex;
					Float& sumSquares = rmsSumSquares[c];
					
					// Compute the new sum-of-squares for the current sample.
					Float sampleSquared = inputSample*inputSample;
					sumSquares = math::max( sumSquares - *rms, Float(0) ) + sampleSquared;
					*rms = sampleSquared;
					
					// Compute the RMS level for this sample.
					level = math::sqrt( sumSquares*inverseRMSLength );
				}
				else
					level = math::abs( inputSample );
				
				//****************************************************************************
				// Update the envelope level for this sample.
				
				Float& e = envelope[c];
				
				if ( level > e )
					e = level + envelopeAttack*(e - level);
				else
					e = level + envelopeRelease*(e - level);
				
				//****************************************************************************
				// Detect if the envelope is over the threshold, and if so, apply gain reduction.
				
				if ( e > kneeMin )
				{
					// Compute the reduction for this channel.
					Gain dbReduction = getDBReduction2( e, currentThreshold, reductionConstant,
														kneeMin, kneeMax, currentKnee );
					
					// Find the largest (most negative) reduction amount.
					maxReduction = math::min( maxReduction, dbReduction );
				}
			}
			
			// Add the current reduction in dB to the total reduction.
			reductionTotal += maxReduction;
			numReductionSamples++;
			
			// Compute the final gain which should be applied to all channels.
			Gain finalGain = currentOutputGain*currentMix*math::dbToLinear( maxReduction );
			
			// Compute how much gain is applied to the input when mixing with the output.
			Gain inputMix = Gain(1) - currentMix;
			
			//****************************************************************************
			// Compute the output for this sample.
			
			// Apply the final gain to all channels.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Sample32f inputSample = inputBuffer.getChannel(c)[i]*currentInputGain;
				outputBuffer.getChannel(c)[i] = inputSample*inputMix + inputSample*finalGain;
			}
			
			//****************************************************************************
			// Increment values that need updating.
			
			// Update the current state for interpolated parameters.
			if ( interpolateChanges )
			{
				currentInputGain += inputGainChangePerSample;
				currentOutputGain += outputGainChangePerSample;
				currentMix += mixChangePerSample;
				currentThreshold += thresholdChangePerSample;
				currentKnee += kneeChangePerSample;
				currentRatio += ratioChangePerSample;
			}
		}
		
		// Store the final values for interpolated parameters.
		if ( interpolateChanges )
		{
			inputGain = currentInputGain;
			outputGain = currentOutputGain;
			mix = currentMix;
			threshold = currentThreshold;
			knee = currentKnee;
			ratio = currentRatio;
		}
	}
	else
	{
		Gain currentInputGain;
		Gain currentOutputGain;
		Gain currentMix;
		Gain currentThreshold;
		Gain currentKnee;
		Float currentRatio;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Sample32f* input = inputBuffer.getChannel(c);
			const Sample32f* const inputEnd = input + numSamples;
			Sample32f* output = outputBuffer.getChannel(c);
			Float& e = envelope[c];
			
			Sample32f* rmsStart, *rmsEnd;
			Sample32f* rms;
			Float* sumSquares;
			
			if ( rmsEnabled )
			{
				rmsStart = rmsBuffer.getChannel(c);
				rmsEnd = rmsStart + rmsLengthInSamples;
				rms = rmsStart + currentRMSIndex;
				sumSquares = &rmsSumSquares[c];
			}
			
			currentInputGain = inputGain;
			currentOutputGain = outputGain;
			currentMix = mix;
			currentThreshold = threshold;
			currentKnee = knee;
			currentRatio = ratio;
			
			//****************************************************************************
			
			while ( input != inputEnd )
			{
				// Compute the input sample value.
				Float inputSample = currentInputGain*(*input);
				Float level;
				
				// Do RMS processing if necessary.
				if ( rmsEnabled )
				{
					if ( rms == rmsEnd )
						rms = rmsStart;
					
					// Compute the new sum-of-squares for the current sample.
					Float sampleSquared = inputSample*inputSample;
					*sumSquares = math::max( *sumSquares - *rms, Float(0) ) + sampleSquared;
					*rms = sampleSquared;
					rms++;
					
					// Compute the RMS level for this sample.
					level = math::sqrt( (*sumSquares)*inverseRMSLength );
				}
				else
					level = math::abs( inputSample );
				
				//****************************************************************************
				// Update the envelope level for this sample.
				
				if ( level > e )
					e = level + envelopeAttack*(e - level);
				else
					e = level + envelopeRelease*(e - level);
				
				//****************************************************************************
				// Compute the output for this sample.
				
				// Compute the minimum and maximum knee thresholds on a linear scale.
				Gain linearKnee = math::dbToLinear( currentKnee );
				Gain kneeMin = currentThreshold/linearKnee;
				
				// Compute how much gain is applied to the input when mixing with the output.
				Gain inputMix = Gain(1) - currentMix;
				
				// If the envelope is under the knee's lower threshold, no gain reduction occurs.
				if ( e < kneeMin )
					*output = inputSample*(inputMix + currentMix*currentOutputGain);
				else
				{
					// Envelope is over the knee threshold, compute the gain reduction.
					Gain kneeMax = currentThreshold*linearKnee;
					Gain dbReduction = getDBReduction( e, currentThreshold, currentRatio,
														kneeMin, kneeMax, currentKnee );
					
					*output = inputSample*(inputMix + currentMix*currentOutputGain*math::dbToLinear( dbReduction ));
					
					// Add the current reduction in dB to the total reduction.
					reductionTotal += dbReduction;
				}
				
				//****************************************************************************
				// Increment values that need updating.
				
				numReductionSamples++;
				input++;
				output++;
				
				// Update the current state for interpolated parameters.
				if ( interpolateChanges )
				{
					currentInputGain += inputGainChangePerSample;
					currentOutputGain += outputGainChangePerSample;
					currentMix += mixChangePerSample;
					currentThreshold += thresholdChangePerSample;
					currentKnee += kneeChangePerSample;
					currentRatio += ratioChangePerSample;
				}
			}
		}
		
		//****************************************************************************
		
		// Update the current position in the RMS buffer.
		currentRMSIndex = (currentRMSIndex + numSamples) % rmsLengthInSamples;
		
		// Store the final values for interpolated parameters.
		if ( interpolateChanges )
		{
			inputGain = currentInputGain;
			outputGain = currentOutputGain;
			mix = currentMix;
			threshold = currentThreshold;
			knee = currentKnee;
			ratio = currentRatio;
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
