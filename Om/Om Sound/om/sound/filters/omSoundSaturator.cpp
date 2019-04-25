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

#include "omSoundSaturator.h"


#define PARAMETER_INDEX_INPUT_GAIN 0
#define PARAMETER_INDEX_OUTPUT_GAIN 1
#define PARAMETER_INDEX_CROSSOVER_ENABLED 2
#define PARAMETER_INDEX_CROSSOVER_FREQUENCY 3
#define PARAMETER_INDEX_CROSSOVER_ORDER 4
#define PARAMETER_INDEX_LOW_EFFECT_ENABLED 5
#define PARAMETER_INDEX_LOW_FILTER_ENABLED 6
#define PARAMETER_INDEX_LOW_SOLO 7
#define PARAMETER_INDEX_LOW_DRIVE 8
#define PARAMETER_INDEX_LOW_OUTPUT_GAIN 9
#define PARAMETER_INDEX_HIGH_EFFECT_ENABLED 10
#define PARAMETER_INDEX_HIGH_SOLO 11
#define PARAMETER_INDEX_HIGH_DRIVE 12
#define PARAMETER_INDEX_HIGH_OUTPUT_GAIN 13
#define PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED 14
#define PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY 15
#define PARAMETER_INDEX_LOW_PASS_FILTER_ORDER 16
#define PARAMETER_COUNT 17

#define PARAMETER_NAME_INPUT_GAIN "Input Gain"
#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"
#define PARAMETER_NAME_CROSSOVER_ENABLED "Crossover Enabled"
#define PARAMETER_NAME_CROSSOVER_FREQUENCY "Crossover Frequency"
#define PARAMETER_NAME_CROSSOVER_ORDER "Crossover Order"
#define PARAMETER_NAME_LOW_EFFECT_ENABLED "Low Effect Enabled"
#define PARAMETER_NAME_LOW_FILTER_ENABLED "Low Filter Enabled"
#define PARAMETER_NAME_LOW_SOLO "Low Solo"
#define PARAMETER_NAME_LOW_DRIVE "Low Drive"
#define PARAMETER_NAME_LOW_OUTPUT_GAIN "Low Output Gain"
#define PARAMETER_NAME_HIGH_EFFECT_ENABLED "High Effect Enabled"
#define PARAMETER_NAME_HIGH_SOLO "High Solo"
#define PARAMETER_NAME_HIGH_DRIVE "High Drive"
#define PARAMETER_NAME_HIGH_OUTPUT_GAIN "High Output Gain"
#define PARAMETER_NAME_LOW_PASS_FILTER_ENABLED "Low Pass Filter Enabled"
#define PARAMETER_NAME_LOW_PASS_FILTER_FREQUENCY "Low Pass Filter Frequency"
#define PARAMETER_NAME_LOW_PASS_FILTER_ORDER "Low Pass Filter Order"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Saturator:: NAME( "Saturator" );
const UTF8String Saturator:: MANUFACTURER( "Om Sound" );
const FilterVersion Saturator:: VERSION( 1, 0, 0 );
const Float Saturator:: MIN_HARDNESS = 0.0f;
const Float Saturator:: MAX_HARDNESS = Float(1) - math::epsilon<Float>();


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Saturator:: Saturator()
	:	SoundFilter( 1, 1 ),
		preLowPass( CutoffFilter::BUTTERWORTH, CutoffFilter::LOW_PASS, 2, 160 ),
		postLowPass( CutoffFilter::BUTTERWORTH, CutoffFilter::LOW_PASS, 2, 160 ),
		highPass( CutoffFilter::LINKWITZ_RILEY, CutoffFilter::HIGH_PASS, 4, 160 ),
		finalLowPass( NULL ),
		inputGain( 1 ),
		targetInputGain( 1 ),
		outputGain( 1 ),
		targetOutputGain( 1 ),
		lowDrive( 1 ),
		targetLowDrive( 1 ),
		lowOutputGain( 1 ),
		targetLowOutputGain( 1 ),
		highDrive( 1 ),
		targetHighDrive( 1 ),
		highOutputGain( 1 ),
		targetHighOutputGain( 1 ),
		crossoverFrequency( 160 ),
		crossoverOrder( 4 ),
		lowPassFrequency( 4500 ),
		lowPassOrder( 4 ),
		lowPassEnabled( false ),
		lowEffectEnabled( true ),
		lowFilterEnabled( true ),
		lowSolo( false ),
		highEffectEnabled( false ),
		highSolo( false ),
		crossoverEnabled( true )
{
	preLowPass.setIsSynchronized( false );
	postLowPass.setIsSynchronized( false );
	highPass.setIsSynchronized( false );
}




Saturator:: Saturator( const Saturator& other )
	:	SoundFilter( 1, 1 ),
		preLowPass( other.preLowPass ),
		postLowPass( other.postLowPass ),
		highPass( other.highPass ),
		finalLowPass( NULL ),
		inputGain( other.inputGain ),
		targetInputGain( other.targetInputGain ),
		outputGain( other.outputGain ),
		targetOutputGain( other.targetOutputGain ),
		lowDrive( other.lowDrive ),
		targetLowDrive( other.targetLowDrive ),
		lowOutputGain( other.lowOutputGain ),
		targetLowOutputGain( other.targetLowOutputGain ),
		highDrive( other.highDrive ),
		targetHighDrive( other.targetHighDrive ),
		highOutputGain( other.highOutputGain ),
		targetHighOutputGain( other.targetHighOutputGain ),
		crossoverFrequency( other.crossoverFrequency ),
		crossoverOrder( other.crossoverOrder ),
		lowPassFrequency( other.lowPassFrequency ),
		lowPassOrder( other.lowPassOrder ),
		lowPassEnabled( other.lowPassEnabled ),
		lowEffectEnabled( other.lowEffectEnabled ),
		lowFilterEnabled( other.lowFilterEnabled ),
		lowSolo( other.lowSolo ),
		highEffectEnabled( other.highEffectEnabled ),
		highSolo( other.highSolo ),
		crossoverEnabled( other.crossoverEnabled )
{
	preLowPass.setIsSynchronized( false );
	postLowPass.setIsSynchronized( false );
	highPass.setIsSynchronized( false );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Saturator:: ~Saturator()
{
	if ( finalLowPass != NULL )
		om::util::destruct( finalLowPass );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Saturator& Saturator:: operator = ( const Saturator& other )
{
	if ( this != &other )
	{
		preLowPass = other.preLowPass;
		postLowPass = other.postLowPass;
		highPass = other.highPass;
		inputGain = other.inputGain;
		targetInputGain = other.targetInputGain;
		outputGain = other.outputGain;
		targetOutputGain = other.targetOutputGain;
		lowDrive = other.lowDrive;
		targetLowDrive = other.targetLowDrive;
		lowOutputGain = other.lowOutputGain;
		targetLowOutputGain = other.targetLowOutputGain;
		highDrive = other.highDrive;
		targetHighDrive = other.targetHighDrive;
		highOutputGain = other.highOutputGain;
		targetHighOutputGain = other.targetHighOutputGain;
		crossoverFrequency = other.crossoverFrequency;
		crossoverOrder = other.crossoverOrder;
		lowPassFrequency = other.lowPassFrequency;
		lowPassOrder = other.lowPassOrder;
		lowPassEnabled = other.lowPassEnabled;
		lowEffectEnabled = other.lowEffectEnabled;
		lowFilterEnabled = other.lowFilterEnabled;
		lowSolo = other.lowSolo;
		highEffectEnabled = other.highEffectEnabled;
		highSolo = other.highSolo;
		crossoverEnabled = other.crossoverEnabled;
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




UTF8String Saturator:: getName() const
{
	return NAME;
}




UTF8String Saturator:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Saturator:: getVersion() const
{
	return VERSION;
}




FilterCategory Saturator:: getCategory() const
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




Size Saturator:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Saturator:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
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
		//**********************************************************************
		case PARAMETER_INDEX_CROSSOVER_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_CROSSOVER_ENABLED, PARAMETER_NAME_CROSSOVER_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_CROSSOVER_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_CROSSOVER_FREQUENCY, PARAMETER_NAME_CROSSOVER_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 160.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_CROSSOVER_ORDER:
			info = FilterParameterInfo( PARAMETER_INDEX_CROSSOVER_ORDER, PARAMETER_NAME_CROSSOVER_ORDER,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(2), Int64(8), Int64(4),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		//**********************************************************************
		case PARAMETER_INDEX_LOW_EFFECT_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_EFFECT_ENABLED, PARAMETER_NAME_LOW_EFFECT_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_FILTER_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_FILTER_ENABLED, PARAMETER_NAME_LOW_FILTER_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_SOLO:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_SOLO, PARAMETER_NAME_LOW_SOLO,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_DRIVE:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_DRIVE, PARAMETER_NAME_LOW_DRIVE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										0.0f, 50.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_OUTPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_OUTPUT_GAIN, PARAMETER_NAME_LOW_OUTPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-30.0f, 6.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		//**********************************************************************
		case PARAMETER_INDEX_HIGH_EFFECT_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_EFFECT_ENABLED, PARAMETER_NAME_HIGH_EFFECT_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_SOLO:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_SOLO, PARAMETER_NAME_HIGH_SOLO,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_DRIVE:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_DRIVE, PARAMETER_NAME_HIGH_DRIVE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										0.0f, 50.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_OUTPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_OUTPUT_GAIN, PARAMETER_NAME_HIGH_OUTPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-30.0f, 6.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		//**********************************************************************
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




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Value Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Saturator:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_INPUT_GAIN:
			value = FilterParameter( this->getInputGainDB() );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			value = FilterParameter( this->getOutputGainDB() );
			return true;
		case PARAMETER_INDEX_CROSSOVER_ENABLED:
			value = FilterParameter( this->getCrossoverIsEnabled() );
			return true;
		case PARAMETER_INDEX_CROSSOVER_FREQUENCY:
			value = FilterParameter( this->getCrossoverFrequency() );
			return true;
		case PARAMETER_INDEX_CROSSOVER_ORDER:
			value = FilterParameter( (Int64)this->getCrossoverOrder() );
			return true;
		case PARAMETER_INDEX_LOW_EFFECT_ENABLED:
			value = FilterParameter( this->getLowEffectIsEnabled() );
			return true;
		case PARAMETER_INDEX_LOW_FILTER_ENABLED:
			value = FilterParameter( this->getLowFilterIsEnabled() );
			return true;
		case PARAMETER_INDEX_LOW_SOLO:
			value = FilterParameter( this->getLowsAreSoloed() );
			return true;
		case PARAMETER_INDEX_LOW_DRIVE:
			value = FilterParameter( this->getLowDriveDB() );
			return true;
		case PARAMETER_INDEX_LOW_OUTPUT_GAIN:
			value = FilterParameter( this->getLowGainDB() );
			return true;
		case PARAMETER_INDEX_HIGH_EFFECT_ENABLED:
			value = FilterParameter( this->getHighEffectIsEnabled() );
			return true;
		case PARAMETER_INDEX_HIGH_SOLO:
			value = FilterParameter( this->getHighsAreSoloed() );
			return true;
		case PARAMETER_INDEX_HIGH_DRIVE:
			value = FilterParameter( this->getHighDriveDB() );
			return true;
		case PARAMETER_INDEX_HIGH_OUTPUT_GAIN:
			value = FilterParameter( this->getHighGainDB() );
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




Bool Saturator:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Gain gainValue;
	Float floatValue;
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
		case PARAMETER_INDEX_CROSSOVER_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setCrossoverIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_CROSSOVER_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setCrossoverFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_CROSSOVER_ORDER:
			if ( value.getValue( intValue ) )
			{
				this->setCrossoverOrder( (Size)intValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_EFFECT_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setLowEffectIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_FILTER_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setLowFilterIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_SOLO:
			if ( value.getValue( booleanValue ) )
			{
				this->setLowsAreSoloed( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_DRIVE:
			if ( value.getValue( gainValue ) )
			{
				this->setLowDriveDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_OUTPUT_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setLowGainDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_EFFECT_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setHighEffectIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_SOLO:
			if ( value.getValue( booleanValue ) )
			{
				this->setHighsAreSoloed( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_DRIVE:
			if ( value.getValue( gainValue ) )
			{
				this->setHighDriveDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_OUTPUT_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setHighGainDB( gainValue );
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




void Saturator:: resetStream()
{
	// Signal to the cutoff filters that the stream is restarting.
	preLowPass.reset();
	postLowPass.reset();
	highPass.reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Saturator:: processFrame( const SoundFrame& inputFrame,
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
	
	const Size numChannels = inputBuffer->getChannelCount();
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		inputGain = targetInputGain;
		outputGain = targetOutputGain;
		lowDrive = targetLowDrive;
		lowOutputGain = targetLowOutputGain;
		highDrive = targetHighDrive;
		highOutputGain = targetHighOutputGain;
	}
	
	//*********************************************************************************
	
	// Compute how much the input gain should change per sample.
	Gain inputGainChangePerSample = Gain(0.5)*(targetInputGain - inputGain) / numSamples;
	
	// Apply the input gain to the input buffer and place it in the 1st temporary buffer.
	applyGain( *inputBuffer, *outputBuffer, numSamples, inputGain, inputGainChangePerSample, inputGain );
	
	//*********************************************************************************
	// Process the saturation effect.
	
	if ( crossoverEnabled )
	{
		// Get a temporary sound buffer to hold the temporary results of this effect.
		SharedSoundBuffer sharedBufferHandle = SharedBufferPool::getGlobalBuffer( numChannels, numSamples,
																			inputBuffer->getSampleRate() );
		SoundBuffer& tempBuffer = sharedBufferHandle.getBuffer();
		
		//*********************************************************************************
		
		const Size halfCrossoverOrder = crossoverOrder >> 1;
		
		// Apply the first low pass filter to the input audio and place it in the temp buffer.
		if ( preLowPass.getFrequency() != crossoverFrequency )
			preLowPass.setFrequency( crossoverFrequency );
		
		if ( preLowPass.getOrder() != halfCrossoverOrder )
			preLowPass.setOrder( halfCrossoverOrder );
		
		preLowPass.process( *outputBuffer, tempBuffer, numSamples );
		
		
		if ( postLowPass.getFrequency() != crossoverFrequency )
			postLowPass.setFrequency( crossoverFrequency );
		
		if ( postLowPass.getOrder() != halfCrossoverOrder )
			postLowPass.setOrder( halfCrossoverOrder );
		
		// Apply the second low pass filter to the input if the lows shouldn't be filtered.
		if ( !lowFilterEnabled )
			postLowPass.process( tempBuffer, tempBuffer, numSamples );
		
		//*********************************************************************************
		
		// Apply the high pass filter to the input audio and place it in the output buffer.
		if ( highPass.getFrequency() != crossoverFrequency )
			highPass.setFrequency( crossoverFrequency );
		
		if ( highPass.getOrder() != crossoverOrder )
			highPass.setOrder( crossoverOrder );
		
		highPass.process( *outputBuffer, *outputBuffer, numSamples );
		
		//*********************************************************************************
		
		// Compute how much the high frequency drive should change per sample.
		Gain highDriveChangePerSample = Gain(0.5)*(targetHighDrive - highDrive) / numSamples;
		Gain highGainChangePerSample = Gain(0.5)*(targetHighOutputGain - highOutputGain) / numSamples;
		
		// Determine if the high pass filter's output needs to have its polarity inverted.
		Bool highPassIsOdd = halfCrossoverOrder % 2 == 1;
		
		if ( highPassIsOdd )
		{
			// Invert the gain parameters here because the filter is of inverse polartiy.
			highDrive = -highDrive;
			highDriveChangePerSample = -highDriveChangePerSample;
		}
		
		// Process the high frequency saturation if necessary.
		if ( highEffectEnabled )
		{
			// Do the full saturation effect.
			saturate( *outputBuffer, *outputBuffer, numSamples,
					highDrive, highDriveChangePerSample, highDrive,
					highOutputGain, highGainChangePerSample, highOutputGain );
		}
		else
		{
			// Apply the high frequency gain if the effect is disabled.
			applyGain( *outputBuffer, *outputBuffer, numSamples, highOutputGain, highGainChangePerSample, highOutputGain );
		}
		
		if ( highPassIsOdd )
		{
			// Restore the gain parameters here because the filter is of inverse polartiy.
			highDrive = -highDrive;
		}
		
		//*********************************************************************************
		
		// Compute how much the low frequency drive should change per sample.
		Gain lowDriveChangePerSample = Gain(0.5)*(targetLowDrive - lowDrive) / numSamples;
		Gain lowGainChangePerSample = Gain(0.5)*(targetLowOutputGain - lowOutputGain) / numSamples;
		
		// Process the low frequency saturation if necessary.
		if ( lowEffectEnabled )
		{
			// Do the full saturation effect.
			saturate( tempBuffer, tempBuffer, numSamples,
					lowDrive, lowDriveChangePerSample, lowDrive,
					lowOutputGain, lowGainChangePerSample, lowOutputGain );
		}
		else
		{
			// Apply the low frequency drive if the low effect is disabled.
			applyGain( tempBuffer, tempBuffer, numSamples, lowOutputGain, lowGainChangePerSample, lowOutputGain );
		}
		
		//*********************************************************************************
		
		// Apply the second low pass filter to the input audio if the lows should be filtered.
		if ( lowFilterEnabled )
			postLowPass.process( tempBuffer, tempBuffer, numSamples );
		
		//*********************************************************************************
		// Mix the temporary buffer with the output buffer to produce the final output.
		
		if ( !highSolo )
		{
			if ( lowSolo )
				tempBuffer.copyTo( *outputBuffer, numSamples );
			else
				tempBuffer.mixTo( *outputBuffer, numSamples );
		}
		else if ( lowSolo )
			tempBuffer.mixTo( *outputBuffer, numSamples );
	}
	else
	{
		// Since the crossover is disabled, just copy the target low frequency parameters.
		lowDrive = targetLowDrive;
		lowOutputGain = targetLowOutputGain;
		
		//*********************************************************************************
		
		// Compute how much the high frequency drive should change per sample.
		Gain highDriveChangePerSample = Gain(0.5)*(targetHighDrive - highDrive) / numSamples;
		Gain highGainChangePerSample = Gain(0.5)*(targetHighOutputGain - highOutputGain) / numSamples;
		
		// Process the high frequency saturation if necessary.
		if ( highEffectEnabled )
		{
			// Do the full saturation effect.
			saturate( *outputBuffer, *outputBuffer, numSamples,
					highDrive, highDriveChangePerSample, highDrive,
					highOutputGain, highGainChangePerSample, highOutputGain );
		}
		else
		{
			// Apply the low frequency drive if the low effect is disabled.
			applyGain( *outputBuffer, *outputBuffer, numSamples, highOutputGain, highGainChangePerSample, highOutputGain );
		}
	}
	
	//******************************************************************************
	// Process the low pass filter for the saturation filter if applicable.
	
	if ( lowPassEnabled )
	{
		// Create the low pass filter if it has not been created yet.
		if ( finalLowPass == NULL )
		{
			finalLowPass = om::util::construct<CutoffFilter>( CutoffFilter::BUTTERWORTH, CutoffFilter::LOW_PASS,
															lowPassOrder, lowPassFrequency );
			finalLowPass->setIsSynchronized( false );
		}
		else
		{
			// Update the filter frequencies if necessary.
			if ( finalLowPass->getFrequency() != lowPassFrequency )
				finalLowPass->setFrequency( lowPassFrequency );
			
			if ( finalLowPass->getOrder() != lowPassOrder )
				finalLowPass->setOrder( lowPassOrder );
		}
		
		/// Process the low pass filter on the output buffer.
		finalLowPass->process( *outputBuffer, *outputBuffer, numSamples );
	}
	
	//*********************************************************************************
	
	// Compute how much the input gain should change per sample.
	Gain outputGainChangePerSample = Gain(0.5)*(targetOutputGain - outputGain) / numSamples;
	
	// Apply the final output gain.
	applyGain( *outputBuffer, *outputBuffer, numSamples, outputGain, outputGainChangePerSample, outputGain );
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Processing Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void Saturator:: applyGain( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
							Gain startingGain, Gain gainChangePerSample, Gain& finalGain )
{
	const Size numChannels = inputBuffer.getChannelCount();
	Gain currentGain;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		currentGain = startingGain;
		
		while ( output != outputEnd )
		{
			*output = currentGain*(*input);
			
			input++;
			output++;
			currentGain += gainChangePerSample;
		}
	}
	
	finalGain = currentGain;
}




void Saturator:: saturate( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
							Gain drive, Gain driveChangePerSample, Gain& finalDrive,
							Gain gain, Gain gainChangePerSample, Gain& finalGain )
{
	const Size numChannels = inputBuffer.getChannelCount();
	Gain currentDrive;
	Gain currentGain;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		currentDrive = drive;
		currentGain = gain;
		
		while ( output != outputEnd )
		{
			Float in = currentDrive*(*input);
			
			*output = currentGain*math::tanh(in);
			
			input++;
			output++;
			currentDrive += driveChangePerSample;
			currentGain += gainChangePerSample;
		}
	}
	
	finalDrive = currentDrive;
	finalGain = currentGain;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
