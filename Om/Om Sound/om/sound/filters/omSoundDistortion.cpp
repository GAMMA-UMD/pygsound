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

#include "omSoundDistortion.h"


#define PARAMETER_INDEX_INPUT_GAIN 0
#define PARAMETER_INDEX_OUTPUT_GAIN 1
#define PARAMETER_INDEX_MIX 2
#define PARAMETER_INDEX_TYPE 3
#define PARAMETER_INDEX_THRESHOLD 4
#define PARAMETER_INDEX_HARDNESS 5
#define PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED 6
#define PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY 7
#define PARAMETER_INDEX_LOW_PASS_FILTER_ORDER 8
#define PARAMETER_COUNT 9

#define PARAMETER_NAME_INPUT_GAIN "Input Gain"
#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"
#define PARAMETER_NAME_MIX "Mix"
#define PARAMETER_NAME_TYPE "Type"
#define PARAMETER_NAME_THRESHOLD "Threshold"
#define PARAMETER_NAME_HARDNESS "Hardness"
#define PARAMETER_NAME_LOW_PASS_FILTER_ENABLED "Low-Pass Filter Enabled"
#define PARAMETER_NAME_LOW_PASS_FILTER_FREQUENCY "LPF Frequency"
#define PARAMETER_NAME_LOW_PASS_FILTER_ORDER "LPF Order"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Distortion:: NAME( "Distortion" );
const UTF8String Distortion:: MANUFACTURER( "Om Sound" );
const FilterVersion Distortion:: VERSION( 1, 0, 0 );
const Float Distortion:: MIN_HARDNESS = 0.0f;
const Float Distortion:: MAX_HARDNESS = Float(1) - math::epsilon<Float>();


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Distortion:: Distortion()
	:	SoundFilter( 1, 1 ),
		type( SOFT ),
		inputGain( 1 ),
		targetInputGain( 1 ),
		outputGain( 1 ),
		targetOutputGain( 1 ),
		mix( 1 ),
		targetMix( 1 ),
		threshold( 1 ),
		targetThreshold( 1 ),
		hardness( 1 ),
		targetHardness( 1 ),
		lowPassFrequency( 4500 ),
		lowPassOrder( 4 ),
		lowPass( NULL ),
		lowPassEnabled( false )
{
}




Distortion:: Distortion( Type newType )
	:	SoundFilter( 1, 1 ),
		type( newType ),
		inputGain( 1 ),
		targetInputGain( 1 ),
		outputGain( 1 ),
		targetOutputGain( 1 ),
		mix( 1 ),
		targetMix( 1 ),
		threshold( 1 ),
		targetThreshold( 1 ),
		hardness( 1 ),
		targetHardness( 1 ),
		lowPassFrequency( 4500 ),
		lowPassOrder( 4 ),
		lowPass( NULL ),
		lowPassEnabled( false )
{
}




Distortion:: Distortion( const Distortion& other )
	:	SoundFilter( 1, 1 ),
		type( other.type ),
		inputGain( other.inputGain ),
		targetInputGain( other.targetInputGain ),
		outputGain( other.outputGain ),
		targetOutputGain( other.targetOutputGain ),
		mix( other.mix ),
		targetMix( other.targetMix ),
		threshold( other.threshold ),
		targetThreshold( other.targetThreshold ),
		hardness( other.hardness ),
		targetHardness( other.targetHardness ),
		lowPassFrequency( other.lowPassFrequency ),
		lowPassOrder( other.lowPassOrder ),
		lowPass( NULL ),
		lowPassEnabled( other.lowPassEnabled )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Distortion:: ~Distortion()
{
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




Distortion& Distortion:: operator = ( const Distortion& other )
{
	if ( this != &other )
	{
		type = other.type;
		hardness = other.hardness;
		targetHardness = other.targetHardness;
		inputGain = other.inputGain;
		targetInputGain = other.targetInputGain;
		outputGain = other.outputGain;
		targetOutputGain = other.targetOutputGain;
		lowPassFrequency = other.lowPassFrequency;
		lowPassOrder = other.lowPassOrder;
		lowPassEnabled = other.lowPassEnabled;
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




UTF8String Distortion:: getName() const
{
	return NAME;
}




UTF8String Distortion:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Distortion:: getVersion() const
{
	return VERSION;
}




FilterCategory Distortion:: getCategory() const
{
	return FilterCategory::DISTORTION;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size Distortion:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Distortion:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_INPUT_GAIN, PARAMETER_NAME_INPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 50.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_GAIN, PARAMETER_NAME_OUTPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-30.0f, 20.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_MIX:
			info = FilterParameterInfo( PARAMETER_INDEX_MIX, PARAMETER_NAME_MIX,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::LINEAR,
										0.0f, 100.0f, 100.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_TYPE:
			info = FilterParameterInfo( PARAMETER_INDEX_TYPE, PARAMETER_NAME_TYPE,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(SOFT), Int64(BREAKUP_2), Int64(SOFT),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_THRESHOLD:
			info = FilterParameterInfo( PARAMETER_INDEX_THRESHOLD, PARAMETER_NAME_THRESHOLD,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-30.0f, 6.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HARDNESS:
			info = FilterParameterInfo( PARAMETER_INDEX_HARDNESS, PARAMETER_NAME_HARDNESS,
										FilterParameterType::FLOAT,
										FilterParameterUnits::GENERIC, FilterParameterCurve::LINEAR,
										0.0f, 1.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED, PARAMETER_NAME_LOW_PASS_FILTER_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY, PARAMETER_NAME_LOW_PASS_FILTER_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 4500.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FILTER_ORDER:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_FILTER_ORDER, PARAMETER_NAME_LOW_PASS_FILTER_ORDER,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(8), Int64(4),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
	}
	
	return false;
}




Bool Distortion:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	if ( parameterIndex == PARAMETER_INDEX_TYPE && value.getValue( enumValue ) )
	{
		switch ( enumValue )
		{
			case SOFT:			name = "Soft";		break;
			case HARD:			name = "Hard";		break;
			case BREAKUP_1:		name = "Breakup 1";	break;
			case BREAKUP_2:		name = "Breakup 2";	break;
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




Bool Distortion:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_GAIN:
			value = FilterParameter( this->getInputGainDB() );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			value = FilterParameter( this->getOutputGainDB() );
			return true;
		case PARAMETER_INDEX_MIX:
			value = FilterParameter( 100.0f*this->getMix() );
			return true;
		case PARAMETER_INDEX_TYPE:
			value = FilterParameter( (Int64)this->getType() );
			return true;
		case PARAMETER_INDEX_THRESHOLD:
			value = FilterParameter( this->getThresholdDB() );
			return true;
		case PARAMETER_INDEX_HARDNESS:
			value = FilterParameter( this->getHardness() );
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




Bool Distortion:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
	Float floatValue;
	Int64 enumValue;
	Int64 intValue;
	Bool booleanValue;
	
	switch ( parameterIndex )
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
		case PARAMETER_INDEX_TYPE:
			if ( value.getValue( enumValue ) && enumValue >= SOFT && enumValue <= BREAKUP_2 )
			{
				this->setType( (Type)enumValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_THRESHOLD:
			if ( value.getValue( gainValue ) )
			{
				this->setThresholdDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HARDNESS:
			if ( value.getValue( floatValue ) )
			{
				this->setHardness( floatValue );
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




void Distortion:: resetStream()
{
	if ( lowPass != NULL )
		lowPass->reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Distortion:: processFrame( const SoundFrame& inputFrame,
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
	
	//******************************************************************************
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		inputGain = targetInputGain;
		outputGain = targetOutputGain;
		mix = targetMix;
		threshold = targetThreshold;
		hardness = targetHardness;
	}
	
	// Compute how much the gain factors should change per sample.
	Gain inputGainChangePerSample = Gain(0.5)*(targetInputGain - inputGain) / numSamples;
	Gain outputGainChangePerSample = Gain(0.5)*(targetOutputGain - outputGain) / numSamples;
	Float mixChangePerSample = Float(0.5)*(targetMix - mix) / numSamples;
	Float thresholdChangePerSample = Float(0.5)*(targetThreshold - threshold) / numSamples;
	Float hardnessChangePerSample = Float(0.5)*(targetHardness - hardness) / numSamples;
	
	// Check to see if we should interpolate parameters.
	if ( math::abs(inputGainChangePerSample) < math::epsilon<Gain>() && 
		math::abs(thresholdChangePerSample) < math::epsilon<Float>() && 
		math::abs(hardnessChangePerSample) < math::epsilon<Float>() )
	{
		// Use a processing method that doesn't interpolate parameters, since this is
		// more efficient and is the general case.
		switch ( type )
		{
			case SOFT:
				processDistortion<softClip>( *inputBuffer, *outputBuffer, numSamples );
				break;
			
			case HARD:
				processDistortion<hardClip>( *inputBuffer, *outputBuffer, numSamples );
				break;
			
			case BREAKUP_1:
				processDistortion<breakup1>( *inputBuffer, *outputBuffer, numSamples );
				break;
			
			case BREAKUP_2:
				processDistortion<breakup2>( *inputBuffer, *outputBuffer, numSamples );
				break;
		}
	}
	else
	{
		switch ( type )
		{
			case SOFT:
				processDistortion<softClip>( *inputBuffer, *outputBuffer, numSamples,
											inputGainChangePerSample, thresholdChangePerSample, hardnessChangePerSample );
				break;
			
			case HARD:
				processDistortion<hardClip>( *inputBuffer, *outputBuffer, numSamples,
											inputGainChangePerSample, thresholdChangePerSample, hardnessChangePerSample );
				break;
			
			case BREAKUP_1:
				processDistortion<breakup1>( *inputBuffer, *outputBuffer, numSamples,
											inputGainChangePerSample, thresholdChangePerSample, hardnessChangePerSample );
				break;
			
			case BREAKUP_2:
				processDistortion<breakup2>( *inputBuffer, *outputBuffer, numSamples,
											inputGainChangePerSample, thresholdChangePerSample, hardnessChangePerSample );
				break;
		}
	}
	
	//******************************************************************************
	// Process the low pass filter for the distortion filter if applicable.
	
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
	
	//******************************************************************************
	// Mix the distorted signal with the input based on the mix parameter.
	
	{
		const Size numChannels = inputBuffer->getChannelCount();
		Gain currentOutputGain;
		Gain currentMix;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Sample32f* input = inputBuffer->getChannel(c);
			Sample32f* output = outputBuffer->getChannel(c);
			const Sample32f* const outputEnd = output + numSamples;
			
			currentOutputGain = outputGain;
			currentMix = mix;
			
			while ( output != outputEnd )
			{
				*output = currentOutputGain*(currentMix*(*output) + (Gain(1) - currentMix)*(*input));
				
				input++;
				output++;
				currentOutputGain += outputGainChangePerSample;
				currentMix += mixChangePerSample;
			}
		}
		
		outputGain = currentOutputGain;
		mix = currentMix;
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Distortion Processing Method
//############		
//##########################################################################################
//##########################################################################################




template < Float (*clippingFunction)( Float /*input*/, Float /*threshold*/, Float /*inverseThreshold*/, Float /*hardness*/,
									Float /*inverseHardness*/, Float /*hardnessThreshold*/, Float /*hardnessOffset*/ ) >
void Distortion:: processDistortion( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples )
{
	const Size numChannels = inputBuffer.getChannelCount();
	
	// Compute derrived static hardness parameters.
	Float inverseHardness = Float(1) / hardness;
	Float hardnessThreshold = Float(1) - inverseHardness;
	Float hardnessOffset = hardness*hardnessThreshold;
	
	Float inverseThreshold = Float(1) / threshold;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		while ( output != outputEnd )
		{
			Float in = *input;
			
			*output = clippingFunction( inputGain*in,
										threshold, inverseThreshold,
										hardness, inverseHardness,
										hardnessThreshold, hardnessOffset );
			
			input++;
			output++;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Interpolated Distortion Processing Method
//############		
//##########################################################################################
//##########################################################################################




template < Float (*clippingFunction)( Float /*input*/, Float /*threshold*/, Float /*inverseThreshold*/, Float /*hardness*/,
									Float /*inverseHardness*/, Float /*hardnessThreshold*/, Float /*hardnessOffset*/ ) >
void Distortion:: processDistortion( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
									Gain inputGainChangePerSample, Float thresholdChangePerSample, Float hardnessChangePerSample )
{
	const Size numChannels = inputBuffer.getChannelCount();
	Gain currentInputGain;
	Float currentThreshold;
	Gain currentHardness;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		currentInputGain = inputGain;
		currentThreshold = threshold;
		currentHardness = hardness;
		
		while ( output != outputEnd )
		{
			Float inverseHardness = Float(1) / currentHardness;
			Float hardnessThreshold = Float(1) - inverseHardness;
			Float hardnessOffset = currentHardness*hardnessThreshold;
			
			*output = clippingFunction( currentInputGain*(*input),
										currentThreshold, Float(1) / currentThreshold,
										currentHardness, inverseHardness,
										hardnessThreshold, hardnessOffset );
			
			input++;
			output++;
			currentInputGain += inputGainChangePerSample;
			currentThreshold += thresholdChangePerSample;
			currentHardness += hardnessChangePerSample;
		}
	}
	
	inputGain = currentInputGain;
	threshold = currentThreshold;
	hardness = currentHardness;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
