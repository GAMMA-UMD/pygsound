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

#include "omSoundBitcrusher.h"


#define PARAMETER_INDEX_INPUT_GAIN 0
#define PARAMETER_INDEX_OUTPUT_GAIN 1
#define PARAMETER_INDEX_MIX 2
#define PARAMETER_INDEX_CLIP_MODE 3
#define PARAMETER_INDEX_CLIP_THRESHOLD 4
#define PARAMETER_INDEX_BIT_REDUCTION_ENABLED 5
#define PARAMETER_INDEX_BIT_RESOLUTION 6
#define PARAMETER_INDEX_DITHER_ENABLED 7
#define PARAMETER_INDEX_DITHER_AMOUNT 8
#define PARAMETER_INDEX_DOWNSAMPLING 9
#define PARAMETER_INDEX_LOW_PASS_FILTER_ENABLED 10
#define PARAMETER_INDEX_LOW_PASS_FILTER_FREQUENCY 11
#define PARAMETER_INDEX_LOW_PASS_FILTER_ORDER 12
#define PARAMETER_COUNT 13

#define PARAMETER_NAME_INPUT_GAIN "Input Gain"
#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"
#define PARAMETER_NAME_MIX "Mix"
#define PARAMETER_NAME_CLIP_MODE "Clip Mode"
#define PARAMETER_NAME_CLIP_THRESHOLD "Clip Threshold"
#define PARAMETER_NAME_BIT_REDUCTION_ENABLED "Bit Reduction Enabled"
#define PARAMETER_NAME_BIT_RESOLUTION "Bit Resolution"
#define PARAMETER_NAME_DITHER_ENABLED "Dither Enabled"
#define PARAMETER_NAME_DITHER_AMOUNT "Dither Amount"
#define PARAMETER_NAME_DOWNSAMPLING "Downsampling"
#define PARAMETER_NAME_LOW_PASS_FILTER_ENABLED "Low-Pass Filter Enabled"
#define PARAMETER_NAME_LOW_PASS_FILTER_FREQUENCY "LPF Frequency"
#define PARAMETER_NAME_LOW_PASS_FILTER_ORDER "LPF Order"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Bitcrusher:: NAME( "Bitcrusher" );
const UTF8String Bitcrusher:: MANUFACTURER( "Om Sound" );
const FilterVersion Bitcrusher:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Bitcrusher:: Bitcrusher()
	:	SoundFilter( 1, 1 ),
		inputGain( 1 ),
		targetInputGain( 1 ),
		outputGain( 1 ),
		targetOutputGain( 1 ),
		mix( 1 ),
		targetMix( 1 ),
		clipMode( HARD ),
		threshold( 1 ),
		targetThreshold( 1 ),
		bitResolution( 24 ),
		dither( 1 ),
		targetDither( 1 ),
		downsampling( 1 ),
		downsampleRemainder( 0 ),
		lastSamples(),
		lowPassFrequency( 4500 ),
		lowPassOrder( 4 ),
		lowPass( NULL ),
		lowPassEnabled( false ),
		bitReduceEnabled( true ),
		ditherEnabled( false )
{
}




Bitcrusher:: Bitcrusher( const Bitcrusher& other )
	:	SoundFilter( 1, 1 ),
		inputGain( other.inputGain ),
		targetInputGain( other.targetInputGain ),
		outputGain( other.outputGain ),
		targetOutputGain( other.targetOutputGain ),
		mix( other.mix ),
		targetMix( other.targetMix ),
		clipMode( other.clipMode ),
		threshold( other.threshold ),
		targetThreshold( other.targetThreshold ),
		bitResolution( other.bitResolution ),
		dither( other.dither ),
		targetDither( other.targetDither ),
		downsampling( other.downsampling ),
		downsampleRemainder( other.downsampleRemainder ),
		lastSamples( other.lastSamples ),
		lowPass( NULL ),
		lowPassFrequency( other.lowPassFrequency ),
		lowPassOrder( other.lowPassOrder ),
		lowPassEnabled( other.lowPassEnabled ),
		bitReduceEnabled( other.bitReduceEnabled ),
		ditherEnabled( other.ditherEnabled )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Bitcrusher:: ~Bitcrusher()
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




Bitcrusher& Bitcrusher:: operator = ( const Bitcrusher& other )
{
	if ( this != &other )
	{
		inputGain = other.inputGain;
		targetInputGain = other.targetInputGain;
		outputGain = other.outputGain;
		targetOutputGain = other.targetOutputGain;
		mix = other.mix;
		targetMix = other.targetMix;
		clipMode = other.clipMode;
		threshold = other.threshold;
		targetThreshold = other.targetThreshold;
		bitResolution = other.bitResolution;
		dither = other.dither;
		targetDither = other.targetDither;
		downsampling = other.downsampling;
		downsampleRemainder = other.downsampleRemainder;
		lastSamples = other.lastSamples;
		lowPassFrequency = other.lowPassFrequency;
		lowPassOrder = other.lowPassOrder;
		lowPassEnabled = other.lowPassEnabled;
		bitReduceEnabled = other.bitReduceEnabled;
		ditherEnabled = other.ditherEnabled;
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




UTF8String Bitcrusher:: getName() const
{
	return NAME;
}




UTF8String Bitcrusher:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Bitcrusher:: getVersion() const
{
	return VERSION;
}




FilterCategory Bitcrusher:: getCategory() const
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




Size Bitcrusher:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool Bitcrusher:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
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
		case PARAMETER_INDEX_CLIP_MODE:
			info = FilterParameterInfo( PARAMETER_INDEX_CLIP_MODE, PARAMETER_NAME_CLIP_MODE,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(HARD), Int64(WRAP), Int64(HARD),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_CLIP_THRESHOLD:
			info = FilterParameterInfo( PARAMETER_INDEX_CLIP_THRESHOLD, PARAMETER_NAME_CLIP_THRESHOLD,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-30.0f, 0.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_BIT_REDUCTION_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_BIT_REDUCTION_ENABLED, PARAMETER_NAME_BIT_REDUCTION_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_BIT_RESOLUTION:
			info = FilterParameterInfo( PARAMETER_INDEX_BIT_RESOLUTION, PARAMETER_NAME_BIT_RESOLUTION,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(24), Int64(24),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DITHER_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_DITHER_ENABLED, PARAMETER_NAME_DITHER_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DITHER_AMOUNT:
			info = FilterParameterInfo( PARAMETER_INDEX_DITHER_AMOUNT, PARAMETER_NAME_DITHER_AMOUNT,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::LINEAR,
										0.0f, 100.0f, 100.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_DOWNSAMPLING:
			info = FilterParameterInfo( PARAMETER_INDEX_DOWNSAMPLING, PARAMETER_NAME_DOWNSAMPLING,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(24), Int64(1),
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




Bool Bitcrusher:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	if ( parameterIndex == PARAMETER_INDEX_CLIP_MODE && value.getValue( enumValue ) )
	{
		switch ( enumValue )
		{
			case HARD:		name = "Hard";		break;
			case INVERT:	name = "Invert";	break;
			case WRAP:		name = "Wrap";		break;
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




Bool Bitcrusher:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
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
		case PARAMETER_INDEX_CLIP_MODE:
			value = FilterParameter( (Int64)this->getClipMode() );
			return true;
		case PARAMETER_INDEX_CLIP_THRESHOLD:
			value = FilterParameter( this->getThresholdDB() );
			return true;
		case PARAMETER_INDEX_BIT_REDUCTION_ENABLED:
			value = FilterParameter( this->getBitReductionIsEnabled() );
			return true;
		case PARAMETER_INDEX_BIT_RESOLUTION:
			value = FilterParameter( (Int64)this->getBitResolution() );
			return true;
		case PARAMETER_INDEX_DITHER_ENABLED:
			value = FilterParameter( this->getDitherIsEnabled() );
			return true;
		case PARAMETER_INDEX_DITHER_AMOUNT:
			value = FilterParameter( 100.0f*this->getDitherAmount() );
			return true;
		case PARAMETER_INDEX_DOWNSAMPLING:
			value = FilterParameter( (Int64)this->getDownsampling() );
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




Bool Bitcrusher:: setParameterValue( Index parameterIndex, const FilterParameter& value )
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
		case PARAMETER_INDEX_CLIP_MODE:
			if ( value.getValue( enumValue ) && enumValue >= HARD && enumValue <= WRAP )
			{
				this->setClipMode( (ClipMode)enumValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_CLIP_THRESHOLD:
			if ( value.getValue( gainValue ) )
			{
				this->setThresholdDB( gainValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_BIT_REDUCTION_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setBitReductionIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_BIT_RESOLUTION:
			if ( value.getValue( intValue ) )
			{
				this->setBitResolution( UInt(intValue) );
				return true;
			}
			break;
		case PARAMETER_INDEX_DITHER_ENABLED:
			if ( value.getValue( booleanValue ) )
			{
				this->setDitherIsEnabled( booleanValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_DITHER_AMOUNT:
			if ( value.getValue( floatValue ) )
			{
				this->setDitherAmount( floatValue*0.01f );
				return true;
			}
			break;
		case PARAMETER_INDEX_DOWNSAMPLING:
			if ( value.getValue( intValue ) )
			{
				this->setDownsampling( UInt(intValue) );
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




void Bitcrusher:: resetStream()
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




SoundResult Bitcrusher:: processFrame( const SoundFrame& inputFrame,
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
		dither = targetDither;
		
		downsampleRemainder = 0;
		lastSamples.setAll( 0 );
	}
	
	// Make sure the last sample array is big enough.
	if ( downsampling > 1 && lastSamples.getSize() < inputBuffer->getChannelCount() )
		lastSamples.setSize( inputBuffer->getChannelCount() );
	
	// Compute how much the gain factors should change per sample.
	Gain inputGainChangePerSample = Gain(0.5)*(targetInputGain - inputGain) / numSamples;
	Gain outputGainChangePerSample = Gain(0.5)*(targetOutputGain - outputGain) / numSamples;
	Float mixChangePerSample = Float(0.5)*(targetMix - mix) / numSamples;
	Float thresholdChangePerSample = Float(0.5)*(targetThreshold - threshold) / numSamples;
	Float ditherChangePerSample = Float(0.5)*(targetDither - dither) / numSamples;
	
	// Check to see if we should interpolate parameters.
	if ( math::abs(inputGainChangePerSample) < math::epsilon<Gain>() && 
		math::abs(thresholdChangePerSample) < math::epsilon<Float>() )
	{
		switch ( clipMode )
		{
			case HARD:
				processClipping<clipHard>( *inputBuffer, *outputBuffer, numSamples );
				break;
			case INVERT:
				processClipping<clipInvert>( *inputBuffer, *outputBuffer, numSamples );
				break;
			case WRAP:
				processClipping<clipWrap>( *inputBuffer, *outputBuffer, numSamples );
				break;
		}
	}
	else
	{
		switch ( clipMode )
		{
			case HARD:
				processClipping<clipHard>( *inputBuffer, *outputBuffer, numSamples,
											inputGainChangePerSample, thresholdChangePerSample );
				break;
			case INVERT:
				processClipping<clipInvert>( *inputBuffer, *outputBuffer, numSamples,
											inputGainChangePerSample, thresholdChangePerSample );
				break;
			case WRAP:
				processClipping<clipWrap>( *inputBuffer, *outputBuffer, numSamples,
											inputGainChangePerSample, thresholdChangePerSample );
				break;
		}
	}
	
	//******************************************************************************
	// Convert to integer samples and back again, applying dithering if necessary.
	
	if ( bitReduceEnabled )
	{
		if ( ditherEnabled )
		{
			if ( downsampling > 1 )
				processBitReduction<true,true,true>( *outputBuffer, numSamples, ditherChangePerSample );
			else
				processBitReduction<true,true,false>( *outputBuffer, numSamples, ditherChangePerSample );
		}
		else
		{
			if ( downsampling > 1 )
				processBitReduction<true,false,true>( *outputBuffer, numSamples, ditherChangePerSample );
			else
				processBitReduction<true,false,false>( *outputBuffer, numSamples, ditherChangePerSample );
		}
	}
	else
	{
		if ( downsampling > 1 )
			processBitReduction<false,false,true>( *outputBuffer, numSamples, ditherChangePerSample );
		else
			processBitReduction<false,false,false>( *outputBuffer, numSamples, ditherChangePerSample );
	}
	
	//******************************************************************************
	// Process the low pass filter for the bitcrusher if applicable.
	
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
//############		Bitcrusher Clipping Methods
//############		
//##########################################################################################
//##########################################################################################




template < Float (*clippingFunction)( Float /*input*/, Float /*threshold*/ ) >
void Bitcrusher:: processClipping( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples )
{
	const Size numChannels = inputBuffer.getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		while ( output != outputEnd )
		{
			*output = clippingFunction( inputGain*(*input), threshold );
			
			input++;
			output++;
		}
	}
}




template < Float (*clippingFunction)( Float /*input*/, Float /*threshold*/ ) >
void Bitcrusher:: processClipping( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
									Gain inputGainChangePerSample, Float thresholdChangePerSample )
{
	const Size numChannels = inputBuffer.getChannelCount();
	Gain currentInputGain;
	Float currentThreshold;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* input = inputBuffer.getChannel(c);
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		currentInputGain = inputGain;
		currentThreshold = threshold;
		
		while ( output != outputEnd )
		{
			*output = clippingFunction( currentInputGain*(*input), currentThreshold );
			
			input++;
			output++;
			currentInputGain += inputGainChangePerSample;
			currentThreshold += thresholdChangePerSample;
		}
	}
	
	inputGain = currentInputGain;
	threshold = currentThreshold;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Bitcrusher Bit Reduction Method
//############		
//##########################################################################################
//##########################################################################################




template < Bool reductionEnabled, Bool ditherEnabled, Bool downsampleEnabled >
void Bitcrusher:: processBitReduction( SoundBuffer& ioBuffer, Size numSamples, Float ditherChangePerSample )
{
	const Size numChannels = ioBuffer.getChannelCount();
	
	// Compute the maximum and minimum values allowed at the specified bit resolution.
	Float resolutionMax = Float(UInt32(0xFFFFFFFF) >> (32 - bitResolution));
	Float resolutionMin = resolutionMax;
	Float ditherMax = 0.5f / resolutionMax;
	Float ditherMin = -0.5f / resolutionMin;
	
	UInt numRepeats;
	Gain currentDither;
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		Sample32f* input = ioBuffer.getChannel(c) + downsampleRemainder;
		Sample32f* output = ioBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		if ( ditherEnabled )
			currentDither = dither;
		
		if ( downsampleEnabled )
			numRepeats = downsampleRemainder;
		
		Sample32f lastInput = lastSamples[c];
		
		while ( output != outputEnd )
		{
			Float in;
			
			// Determine the input source, downsampled or not.
			if ( downsampleEnabled )
			{
				if ( numRepeats == 0 )
				{
					lastInput = *input;
					numRepeats = downsampling;
					input += downsampling;
				}
				
				in = lastInput;
				numRepeats--;
			}
			else
			{
				in = *input;
				input++;
			}
			
			// Apply dithering if necessary.
			if ( ditherEnabled )
			{
				in += currentDither*math::random( ditherMin, ditherMax );
				currentDither += ditherChangePerSample;
			}
			
			// Convert to integer format and back again if necessary.
			if ( reductionEnabled )
			{
				if ( in >= 0 )
					in = math::min( math::round(in*resolutionMax), resolutionMax ) / resolutionMax;
				else
					in = math::max( math::round(in*resolutionMin), -resolutionMin ) / resolutionMin;
			}
			
			*output = in;
			output++;
		}
		
		if ( downsampleEnabled )
			lastSamples[c] = lastInput;
	}
	
	if ( ditherEnabled )
		dither = currentDither;
	
	if ( downsampleEnabled )
		downsampleRemainder = numRepeats;
	else
		downsampleRemainder = 0;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
