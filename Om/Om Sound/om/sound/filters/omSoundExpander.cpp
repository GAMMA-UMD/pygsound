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

#include "omSoundExpander.h"


#define PARAMETER_INDEX_THRESHOLD 0
#define PARAMETER_INDEX_RATIO 1
#define PARAMETER_INDEX_KNEE 2
#define PARAMETER_INDEX_ATTACK 3
#define PARAMETER_INDEX_HOLD 4
#define PARAMETER_INDEX_RELEASE 5
#define PARAMETER_INDEX_CHANNEL_LINK 6
#define PARAMETER_INDEX_GAIN_REDUCTION 7
#define PARAMETER_COUNT 8

#define PARAMETER_NAME_THRESHOLD "Threshold"
#define PARAMETER_NAME_RATIO "Ratio"
#define PARAMETER_NAME_KNEE "Knee"
#define PARAMETER_NAME_ATTACK "Attack"
#define PARAMETER_NAME_HOLD "Hold"
#define PARAMETER_NAME_RELEASE "Release"
#define PARAMETER_NAME_CHANNEL_LINK "Channel Link"
#define PARAMETER_NAME_GAIN_REDUCTION "Gain Reduction"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Expander:: NAME( "Expander" );
const UTF8String Expander:: MANUFACTURER( "Om Sound" );
const FilterVersion Expander:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Expander:: Expander()
	:	threshold( 0.0f ),
		targetThreshold( 0.0f ),
		ratio( 2.0f ),
		targetRatio( 2.0f ),
		knee( 0.0f ),
		targetKnee( 0.0f ),
		attack( 0.001f ),
		hold( 0.0f ),
		release( 0.1f ),
		currentReduction( 0.0f ),
		linkChannels( false )
{
}




Expander:: Expander( Gain newThreshold, Float newRatio, Float newAttack, Float newHold, Float newRelease )
	:	threshold( math::max( newThreshold, Gain(0) ) ),
		ratio( math::max( newRatio, Float(1) ) ),
		knee( 0.0f ),
		targetKnee( 0.0f ),
		attack( math::max( newAttack, Float(0) ) ),
		hold( math::max( newHold, Float(0) ) ),
		release( math::max( newRelease, Float(0) ) ),
		currentReduction( 0.0f ),
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




Gain Expander:: evaluateTransferFunction( Gain e ) const
{
	// Compute the minimum and maximum knee thresholds on a linear scale.
	Gain kneeMin = math::dbToLinear( math::linearToDB( threshold ) - knee );
	Gain kneeMax = math::dbToLinear( math::linearToDB( threshold ) + knee );
	
	if ( e > kneeMax )
		return e;
	else
	{
		const Float reductionConstant = Float(1) - ratio;
		Gain dbUnder = math::linearToDB( threshold / e );
		Gain dbReduction;
		
		if ( knee > Float(0) && e > kneeMin )
		{
			Float x = (dbUnder + knee)/knee;
			dbReduction = knee*reductionConstant*x*x*Float(0.25);
		}
		else
			dbReduction = dbUnder*reductionConstant;
		
		return e*math::dbToLinear( dbReduction );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Input and Output Name Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Expander:: getInputName( Index inputIndex ) const
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




UTF8String Expander:: getName() const
{
	return NAME;
}




UTF8String Expander:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Expander:: getVersion() const
{
	return VERSION;
}




FilterCategory Expander:: getCategory() const
{
	return FilterCategory::DYNAMICS;
}




Bool Expander:: allowsInPlaceProcessing() const
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




Size Expander:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Expander:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_THRESHOLD:
			info = FilterParameterInfo( PARAMETER_INDEX_THRESHOLD, PARAMETER_NAME_THRESHOLD,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-50.0f, 6.0f, -50.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_RATIO:
			info = FilterParameterInfo( PARAMETER_INDEX_RATIO, PARAMETER_NAME_RATIO,
										FilterParameterType::FLOAT,
										FilterParameterUnits::RATIO, FilterParameterCurve::LINEAR,
										1.0f, 3.0f, 2.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_KNEE:
			info = FilterParameterInfo( PARAMETER_INDEX_KNEE, PARAMETER_NAME_KNEE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										0.0f, 24.0f, 6.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_ATTACK:
			info = FilterParameterInfo( PARAMETER_INDEX_ATTACK, PARAMETER_NAME_ATTACK,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::SQUARE,
										0.0f, 2000.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HOLD:
			info = FilterParameterInfo( PARAMETER_INDEX_HOLD, PARAMETER_NAME_HOLD,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::CUBE,
										0.0f, 2000.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_RELEASE:
			info = FilterParameterInfo( PARAMETER_INDEX_RELEASE, PARAMETER_NAME_RELEASE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::CUBE,
										2.0f, 5000.0f, 100.0f,
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




Bool Expander:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch( parameterIndex )
	{
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
		case PARAMETER_INDEX_HOLD:
			value = FilterParameter( this->getHold()*1000.0f );
			return true;
		case PARAMETER_INDEX_RELEASE:
			value = FilterParameter( this->getRelease()*1000.0f );
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




Bool Expander:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Float floatValue;
	Bool booleanValue;
	
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_THRESHOLD:
			if ( value.getValue( floatValue ) )
			{
				this->setThresholdDB( floatValue );
				return true;
			}
			else
				return false;
		case PARAMETER_INDEX_RATIO:
			if ( value.getValue( floatValue ) )
			{
				this->setRatio( floatValue );
				return true;
			}
			else
				return false;
		case PARAMETER_INDEX_KNEE:
			if ( value.getValue( floatValue ) )
			{
				this->setKnee( floatValue );
				return true;
			}
			else
				return false;
		case PARAMETER_INDEX_ATTACK:
			if ( value.getValue( floatValue ) )
			{
				this->setAttack( floatValue*0.001f );
				return true;
			}
			else
				return false;
		case PARAMETER_INDEX_HOLD:
			if ( value.getValue( floatValue ) )
			{
				this->setHold( floatValue*0.001f );
				return true;
			}
			else
				return false;
		case PARAMETER_INDEX_RELEASE:
			if ( value.getValue( floatValue ) )
			{
				this->setRelease( floatValue*0.001f );
				return true;
			}
			else
				return false;
		case PARAMETER_INDEX_CHANNEL_LINK:
			if ( value.getValue( booleanValue ) )
			{
				this->setChannelsAreLinked( booleanValue );
				return true;
			}
			else
				return false;
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




void Expander:: resetStream()
{
	// Reset the envelope.
	envelope.setAll( 0 );
	holdTime.setAll( 0 );
	currentReduction = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Expander Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Expander:: processFrame( const SoundFrame& inputFrame,
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
	// Compute basic expansion parameters.
	
	// Get data about the input buffer.
	const Size numChannels = inputBuffer->getChannelCount();
	
	// Make sure that there are enough channels in the envelope data.
	if ( envelope.getSize() < numChannels )
		envelope.setSize( numChannels );
	
	// Make sure that there are enough channels in the envelope hold data.
	if ( holdTime.getSize() < numChannels )
		holdTime.setSize( numChannels );
	
	// Compute the envelope attack and release parameters.
	const Gain envelopeAttack = math::pow( Float(0.3), Float(1.0/(attack*sampleRate)) );
	const Gain envelopeRelease = math::pow( Float(0.5), Float(1.0/(release*sampleRate)) );
	
	//*************************************************************************************
	// Compute how much certain parameters have changed since the last buffer frame.
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		threshold = targetThreshold;
		knee = targetKnee;
		ratio = targetRatio;
	}
	
	const Float inverseNumSamples = Float(1) / Float(numSamples);
	
	// Compute how much the interpolated parameters will change for each sample.
	const Gain thresholdChangePerSample = Gain(0.5)*(targetThreshold - threshold)*inverseNumSamples;
	const Gain kneeChangePerSample = Gain(0.5)*(targetKnee - knee)*inverseNumSamples;
	const Float ratioChangePerSample = Float(0.5)*(targetRatio - ratio)*inverseNumSamples;
	
	//*************************************************************************************
	
	const Gain gainChangeThreshold = math::epsilon<Gain>()*Gain(10);
	const Float floatChangeThreshold = math::epsilon<Float>()*Float(10);
	
	if ( math::abs(thresholdChangePerSample) < gainChangeThreshold &&
		math::abs(kneeChangePerSample) < gainChangeThreshold &&
		math::abs(ratioChangePerSample) < floatChangeThreshold )
	{
		expandNoChanges( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease );
	}
	else
	{
		// At least on of the parameters changed, use the most expensive processing method.
		// We don't differentiate between RMS and peak detection because they are equivalent
		// when the RMS buffer length is 1.
		expand( *inputBuffer, *outputBuffer, numSamples, envelopeAttack, envelopeRelease,
				thresholdChangePerSample, kneeChangePerSample, ratioChangePerSample );
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		No Parameter Changes Processing Method
//############		
//##########################################################################################
//##########################################################################################




void Expander:: expandNoChanges( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
								Gain envelopeAttack, Gain envelopeRelease )
{
	const Size numChannels = inputBuffer.getChannelCount();
	
	const Float reductionConstant = Float(1) - ratio;
	
	// Compute the minimum and maximum knee thresholds on a linear scale.
	Gain kneeMin = math::dbToLinear( math::linearToDB( threshold ) - knee );
	Gain kneeMax = math::dbToLinear( math::linearToDB( threshold ) + knee );
	
	// Compute the length in seconds of each sample.
	Float timePerSample = Float(1) / Float(inputBuffer.getSampleRate());
	
	// The total number of samples (from all channels) where gain reduction was applied.
	Size numReductionSamples = 0;
	
	// The sum of all gain reduction that has been applied, in decibels.
	// This is used to calculate the average gain reduction for the processing frame.
	Gain reductionTotal = 0;
	
	if ( linkChannels )
	{
		for ( Index i = 0; i < numSamples; i++ )
		{
			Gain maxReduction = 0;
			
			// Find the highest amount of gain reduction for all channels.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Sample32f inputSample = inputBuffer.getChannel(c)[i];
				
				// Compute the level for this sample.
				Float level = math::abs( (Float)inputSample );
				Float& e = envelope[c];
				Float& h = holdTime[c];
				
				// Update the envelope level for this sample.
				if ( level > e )
				{
					e = envelopeAttack*(e - level) + level;
					h = 0;
				}
				else
				{
					if ( h > hold )
						e = envelopeRelease*(e - level) + level;
					
					h += timePerSample;
				}
				
				// Detect if the envelope is below the threshold, and if so, apply gain reduction.
				if ( e < kneeMax )
				{
					Gain dbUnder = math::linearToDB( threshold / e );
					
					if ( knee > Float(0) && e > kneeMin )
					{
						Float x = (dbUnder + knee)/knee;
						maxReduction = math::min( maxReduction, knee*reductionConstant*x*x*Float(0.25) );
					}
					else
						maxReduction = math::min( maxReduction, dbUnder*reductionConstant );
				}
			}
			
			// Add the current reduction in dB to the total reduction.
			reductionTotal += maxReduction;
			
			if ( maxReduction < 0 )
				numReductionSamples++;
			
			// Compute the final gain which should be applied to all channels.
			Gain finalGain = math::dbToLinear( maxReduction );
			
			// Apply the final gain to all channels.
			for ( Index c = 0; c < numChannels; c++ )
				outputBuffer.getChannel(c)[i] = inputBuffer.getChannel(c)[i]*finalGain;
		}
	}
	else
	{
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Sample32f* input = inputBuffer.getChannel(c);
			const Sample32f* const inputEnd = input + numSamples;
			Sample32f* output = outputBuffer.getChannel(c);
			Float& e = envelope[c];
			Float& h = holdTime[c];
			
			while ( input != inputEnd )
			{
				Sample32f inputSample = *input;
				
				// Compute the level for this sample.
				Float level = math::abs( (Float)inputSample );
				
				// Update the envelope level for this sample.
				if ( level > e )
				{
					e = envelopeAttack*(e - level) + level;
					h = 0;
				}
				else
				{
					if ( h > hold )
						e = envelopeRelease*(e - level) + level;
					
					h += timePerSample;
				}
				
				// Detect if the envelope is below the threshold, and if so, apply gain reduction.
				if ( e > kneeMax )
					*output = inputSample;
				else
				{
					Gain dbUnder = math::linearToDB( threshold / e );
					Gain dbReduction;
					
					if ( knee > Float(0) && e > kneeMin )
					{
						Float x = (dbUnder + knee)/knee;
						dbReduction = knee*reductionConstant*x*x*Float(0.25);
					}
					else
						dbReduction = dbUnder*reductionConstant;
					
					// Add the current reduction in dB to the total reduction.
					reductionTotal += dbReduction;
					numReductionSamples++;
					
					*output = inputSample*math::dbToLinear( dbReduction );
				}
				
				input++;
				output++;
			}
		}
	}
	
	// Update the current average gain reduction amount.
	if ( numReductionSamples > 0 )
		currentReduction = -reductionTotal / numReductionSamples;
	else
		currentReduction = 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Parameter Changes Processing Method
//############		
//##########################################################################################
//##########################################################################################




void Expander:: expand( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
						Gain envelopeAttack, Gain envelopeRelease,
						Gain thresholdChangePerSample, Gain kneeChangePerSample,
						Float ratioChangePerSample )
{
	const Size numChannels = inputBuffer.getChannelCount();
	
	// Compute the length in seconds of each sample.
	Float timePerSample = Float(1) / Float(inputBuffer.getSampleRate());
	
	// The total number of samples (from all channels) where gain reduction was applied.
	Size numReductionSamples = 0;
	
	// The sum of all gain reduction that has been applied, in decibels.
	// This is used to calculate the average gain reduction for the processing frame.
	Gain reductionTotal = 0;
	
	
	if ( linkChannels )
	{
		Gain currentThreshold = threshold;
		Gain currentKnee = knee;
		Float currentRatio = ratio;
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			Gain maxReduction = 0;
			
			// Compute a constant which converts from gain over the threshold to output gain reduction.
			const Float reductionConstant = Float(1) - currentRatio;
			
			// Compute the minimum and maximum knee thresholds on a linear scale.
			Gain kneeMin = math::dbToLinear( math::linearToDB( currentThreshold ) - currentKnee );
			Gain kneeMax = math::dbToLinear( math::linearToDB( currentThreshold ) + currentKnee );
			
			
			// Find the highest amount of gain reduction for all channels.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Sample32f inputSample = inputBuffer.getChannel(c)[i];
				
				// Compute the peak level for this sample.
				Float level = math::abs( (Float)inputSample );
				Float& e = envelope[c];
				Float& h = holdTime[c];
				
				// Update the envelope level for this sample.
				if ( level > e )
				{
					e = envelopeAttack*(e - level) + level;
					h = 0;
				}
				else
				{
					if ( h > hold )
						e = envelopeRelease*(e - level) + level;
					
					h += timePerSample;
				}
				
				// Detect if the envelope is over the threshold, and if so, apply gain reduction.
				if ( e < kneeMax )
				{
					Gain dbUnder = math::linearToDB( currentThreshold / e );
					
					if ( currentKnee > Float(0) && e > kneeMin )
					{
						Float x = (dbUnder + currentKnee)/currentKnee;
						maxReduction = math::min( maxReduction, currentKnee*reductionConstant*x*x*Float(0.25) );
					}
					else
						maxReduction = math::min( maxReduction, dbUnder*reductionConstant );
				}
			}
			
			// Add the current reduction in dB to the total reduction.
			reductionTotal += maxReduction;
			
			if ( maxReduction < 0 )
				numReductionSamples++;
			
			// Compute the final gain which should be applied to all channels.
			Gain finalGain = math::dbToLinear( maxReduction );
			
			// Apply the final gain to all channels.
			for ( Index c = 0; c < numChannels; c++ )
				outputBuffer.getChannel(c)[i] = finalGain*inputBuffer.getChannel(c)[i];
			
			// Update the current state for interpolated parameters.
			currentThreshold += thresholdChangePerSample;
			currentKnee += kneeChangePerSample;
			currentRatio += ratioChangePerSample;
		}
		
		// Store the final values for interpolated parameters.
		threshold = currentThreshold;
		knee = currentKnee;
		ratio = currentRatio;
	}
	else
	{
		Gain currentThreshold;
		Gain currentKnee;
		Float currentRatio;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Sample32f* input = inputBuffer.getChannel(c);
			const Sample32f* const inputEnd = input + numSamples;
			Sample32f* output = outputBuffer.getChannel(c);
			Float& e = envelope[c];
			Float& h = holdTime[c];
			
			currentThreshold = threshold;
			currentKnee = knee;
			currentRatio = ratio;
			
			while ( input != inputEnd )
			{
				Sample32f inputSample = *input;
				
				// Compute the peak level for this sample.
				Float level = math::abs( (Float)inputSample );
				
				// Update the envelope level for this sample.
				if ( level > e )
				{
					e = envelopeAttack*(e - level) + level;
					h = 0;
				}
				else
				{
					if ( h > hold )
						e = envelopeRelease*(e - level) + level;
					
					h += timePerSample;
				}
				
				// Compute the minimum and maximum knee thresholds on a linear scale.
				Gain kneeMin = math::dbToLinear( math::linearToDB( currentThreshold ) - currentKnee );
				Gain kneeMax = math::dbToLinear( math::linearToDB( currentThreshold ) + currentKnee );
				
				// Detect if the envelope is below the threshold, and if so, apply gain reduction.
				if ( e > kneeMax )
					*output = inputSample;
				else
				{
					const Float reductionConstant = Float(1) - currentRatio;
					Gain dbUnder = math::linearToDB( currentThreshold / e );
					Gain dbReduction;
					
					if ( knee > Float(0) && e > kneeMin )
					{
						Float x = (dbUnder + currentKnee)/currentKnee;
						dbReduction = currentKnee*reductionConstant*x*x*Float(0.25);
					}
					else
						dbReduction = dbUnder*reductionConstant;
					
					// Add the current reduction in dB to the total reduction.
					reductionTotal += dbReduction;
					numReductionSamples++;
					
					*output = inputSample*math::dbToLinear( dbReduction );
				}
				
				input++;
				output++;
			
				// Update the current state for interpolated parameters.
				currentThreshold += thresholdChangePerSample;
				currentKnee += kneeChangePerSample;
				currentRatio += ratioChangePerSample;
			}
		}
		
		// Store the final values for interpolated parameters.
		threshold = currentThreshold;
		knee = currentKnee;
		ratio = currentRatio;
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
