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

#include "omSoundToneGenerator.h"


#define PARAMETER_INDEX_TONE_TYPE 0
#define PARAMETER_INDEX_OUTPUT_GAIN 1
#define PARAMETER_INDEX_FREQUENCY 2
#define PARAMETER_INDEX_BANDWIDTH 3
#define PARAMETER_COUNT 4

#define PARAMETER_NAME_TONE_TYPE "Tone Type"
#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"
#define PARAMETER_NAME_FREQUENCY "Frequency"
#define PARAMETER_NAME_BANDWIDTH "Bandwidth"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String ToneGenerator:: NAME( "Tone Generator" );
const UTF8String ToneGenerator:: MANUFACTURER( "Om Sound" );
const FilterVersion ToneGenerator:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ToneGenerator:: ToneGenerator()
	:	SoundFilter( 0, 1 ),
		type( SINE ),
		outputGain( 0.5 ),
		targetOutputGain( 0.5 ),
		frequency( 1000 ),
		targetFrequency( 1000 ),
		phase( 0 ),
		bandwidth( 1.0f / 3.0f ),
		bandFilter( NULL )
{
}




ToneGenerator:: ToneGenerator( ToneType newType )
	:	SoundFilter( 0, 1 ),
		type( newType ),
		outputGain( 0.5 ),
		targetOutputGain( 0.5 ),
		frequency( 1000 ),
		targetFrequency( 1000 ),
		phase( 0 ),
		bandwidth( 1.0f / 3.0f ),
		bandFilter( NULL )
{
}




ToneGenerator:: ToneGenerator( ToneType newType, Gain newOutputGain )
	:	SoundFilter( 0, 1 ),
		type( newType ),
		outputGain( newOutputGain ),
		targetOutputGain( newOutputGain ),
		frequency( 1000 ),
		targetFrequency( 1000 ),
		phase( 0 ),
		bandwidth( 1.0f / 3.0f ),
		bandFilter( NULL )
{
}




ToneGenerator:: ToneGenerator( ToneType newType, Gain newOutputGain, Float newFrequency )
	:	SoundFilter( 0, 1 ),
		type( newType ),
		outputGain( newOutputGain ),
		targetOutputGain( newOutputGain ),
		frequency( math::max( newFrequency, Float(0) ) ),
		phase( 0 ),
		bandwidth( 1.0f / 3.0f ),
		bandFilter( NULL )
{
	targetFrequency = frequency;
}




ToneGenerator:: ToneGenerator( const ToneGenerator& other )
	:	SoundFilter( 0, 1 ),
		type( other.type ),
		outputGain( other.outputGain ),
		targetOutputGain( other.targetOutputGain ),
		frequency( other.frequency ),
		targetFrequency( other.targetFrequency ),
		phase( other.phase ),
		pinkNoiseHistory( other.pinkNoiseHistory ),
		bandwidth( other.bandwidth ),
		bandFilter( other.bandFilter == NULL ? NULL : om::util::construct<BandFilter>( *other.bandFilter ) )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ToneGenerator:: ~ToneGenerator()
{
	if ( bandFilter != NULL )
		om::util::destruct( bandFilter );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ToneGenerator& ToneGenerator:: operator = ( const ToneGenerator& other )
{
	if ( this != &other )
	{
		type = other.type;
		outputGain = other.outputGain;
		targetOutputGain = other.targetOutputGain;
		frequency = other.frequency;
		targetFrequency = other.targetFrequency;
		phase = other.phase;
		pinkNoiseHistory = other.pinkNoiseHistory;
		bandwidth = other.bandwidth;
		
		if ( bandFilter != NULL )
			om::util::destruct( bandFilter );
		
		bandFilter = other.bandFilter == NULL ? NULL : om::util::construct<BandFilter>( *other.bandFilter );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Tone Generator Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String ToneGenerator:: getName() const
{
	return NAME;
}




UTF8String ToneGenerator:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion ToneGenerator:: getVersion() const
{
	return VERSION;
}




FilterCategory ToneGenerator:: getCategory() const
{
	return FilterCategory::ANALYSIS;
}




Bool ToneGenerator:: allowsInPlaceProcessing() const
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




Size ToneGenerator:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool ToneGenerator:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_TONE_TYPE:
			info = FilterParameterInfo( PARAMETER_INDEX_TONE_TYPE, PARAMETER_NAME_TONE_TYPE,
										FilterParameterType::ENUMERATION,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										Int64(SINE), Int64(PINK_NOISE_BAND), Int64(SINE),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS |
										FilterParameterFlags::NAMED_VALUES );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_GAIN, PARAMETER_NAME_OUTPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-40.0f, 0.0f, -6.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_FREQUENCY, PARAMETER_NAME_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 1000.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_BANDWIDTH:
			info = FilterParameterInfo( PARAMETER_INDEX_BANDWIDTH, PARAMETER_NAME_BANDWIDTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::OCTAVES, FilterParameterCurve::SQUARE,
										0.1f, 10.0f, 0.33333f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
	}
	
	return false;
}




Bool ToneGenerator:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	Int64 enumValue;
	
	if ( parameterIndex == PARAMETER_INDEX_TONE_TYPE && value.getValue( enumValue ) )
	{
		switch ( enumValue )
		{
			case SINE:				name = "Sine";			break;
			case SQUARE:			name = "Square";		break;
			case SAW:				name = "Saw";			break;
			case TRIANGLE:			name = "Triangle";		break;
			case WHITE_NOISE:		name = "White Noise";	break;
			case PINK_NOISE:		name = "Pink Noise";	break;
			case PINK_NOISE_BAND:	name = "Pink Noise Band";	break;
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




Bool ToneGenerator:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_TONE_TYPE:
			value = FilterParameter( (Int64)this->getType() );
			return true;
		case PARAMETER_INDEX_OUTPUT_GAIN:
			value = FilterParameter( this->getOutputGainDB() );
			return true;
		case PARAMETER_INDEX_FREQUENCY:
			value = FilterParameter( this->getFrequency() );
			return true;
		case PARAMETER_INDEX_BANDWIDTH:
			value = FilterParameter( this->getBandwidth() );
			return true;
	}
	
	return false;
}




Bool ToneGenerator:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 enumValue;
	Gain gainValue;
	Float floatValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_TONE_TYPE:
			if ( value.getValue( enumValue ) &&
				(enumValue == SINE || enumValue == SQUARE || enumValue == SAW ||
				enumValue == TRIANGLE || enumValue == WHITE_NOISE ||
				enumValue == PINK_NOISE || enumValue == PINK_NOISE_BAND) )
			{
				this->setType( (ToneType)enumValue );
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
		case PARAMETER_INDEX_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_BANDWIDTH:
			if ( value.getValue( floatValue ) )
			{
				this->setBandwidth( floatValue );
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




void ToneGenerator:: resetStream()
{
	if ( bandFilter != NULL )
		bandFilter->reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult ToneGenerator:: processFrame( const SoundFrame& inputFrame,
												SoundFrame& outputFrame, Size numSamples )
{
	SoundBuffer* outputBuffer;
	
	// If there is no output buffer or if it is NULL, return that no samples were processed.
	if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
		return 0;
	
	// Check to make sure there is at least one output channel.
	if ( outputBuffer->getChannelCount() == 0 )
	{
		// Check to see if there is an input buffer and if so, use its channel count.
		const SoundBuffer* inputBuffer;
		
		if ( inputFrame.getBufferCount() > 0 && (inputBuffer = inputFrame.getBuffer(0)) != NULL )
			outputBuffer->setChannelCount( inputBuffer->getChannelCount() );
		else
			outputBuffer->setChannelCount( 1 );
	}
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	// Check to make sure that the output buffer has the correct size.
	if ( outputBuffer->getSize() < numSamples )
		outputBuffer->setSize( numSamples );
	
	// Check to make sure the output sample rate is valid. If invalid, use a good default of 44.1 kHz.
	if ( outputBuffer->getSampleRate() <= SampleRate(0) )
	{
		// Check to see if there is an input buffer and if so, use its channel count.
		const SoundBuffer* inputBuffer;
		
		if ( inputFrame.getBufferCount() > 0 && (inputBuffer = inputFrame.getBuffer(0)) != NULL )
			outputBuffer->setSampleRate( inputBuffer->getSampleRate() );
		else
			outputBuffer->setSampleRate( 44100 );
	}
	
	//*****************************************************************************
	
	// Reset parameter interpolation if this is the first processing frame.
	if ( isFirstFrame() )
	{
		frequency = targetFrequency;
		outputGain = targetOutputGain;
		phase = 0;
		pinkNoiseHistory.setAll( 0 );
	}
	
	// Compute the change per sample for the frequency and output gain parameters.
	Float frequencyChangePerSample = Float(0.5)*(targetFrequency - frequency) / numSamples;
	Float outputGainChangePerSample = Gain(0.5)*(targetOutputGain - outputGain) / numSamples;
	
	// Determine which type of tone to generate.
	switch ( type )
	{
		case SINE:
			generateWave<sine>( *outputBuffer, numSamples, frequencyChangePerSample, outputGainChangePerSample );
			break;
		case SQUARE:
			generateWave<square>( *outputBuffer, numSamples, frequencyChangePerSample, outputGainChangePerSample );
			break;
		case SAW:
			generateWave<saw>( *outputBuffer, numSamples, frequencyChangePerSample, outputGainChangePerSample );
			break;
		case TRIANGLE:
			generateWave<triangle>( *outputBuffer, numSamples, frequencyChangePerSample, outputGainChangePerSample );
			break;
		case WHITE_NOISE:
			generateWhiteNoise( *outputBuffer, numSamples, outputGainChangePerSample );
			break;
		case PINK_NOISE:
			generatePinkNoise( *outputBuffer, numSamples, outputGainChangePerSample );
			break;
		case PINK_NOISE_BAND:
		{
			// Set the new frequency for the tone generator.
			frequency = Float(0.5)*(targetFrequency + frequency);
			
			// Compute the ratio between the band center frequency and the band limits.
			Float frequencyRatio = math::pow( Float(2), Float(0.5)*bandwidth );
			
			// Compute the frequency band limits.
			Float lowFrequency = frequency/frequencyRatio;
			Float highFrequency = frequency*frequencyRatio;
			
			// Allocate the band filter if it has not yet been allocated.
			if ( bandFilter == NULL )
			{
				bandFilter = om::util::construct<BandFilter>( BandFilter::BUTTERWORTH,
																BandFilter::BAND_PASS, 8, lowFrequency, highFrequency );
				bandFilter->setIsSynchronized( false );
			}
			else
			{
				// Make sure the band pass filter has the new frequencies.
				bandFilter->setFrequency1( lowFrequency );
				bandFilter->setFrequency2( highFrequency );
			}
			
			// Generate pink noise in the output buffer.
			generatePinkNoise( *outputBuffer, numSamples, outputGainChangePerSample );
			
			// Filter the pink noise.
			bandFilter->process( *outputBuffer, *outputBuffer, numSamples );
			
			// Compute the necessary makeup gain to normalize to full scale.
			Gain makeupGain = math::sqrt(Float(10) / bandwidth);
			Size numChannels = outputBuffer->getChannelCount();
			
			// Apply the makeup gain to the filtered noise.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Sample32f* output = outputBuffer->getChannel(c);
				const Sample32f* const outputEnd = output + numSamples;
				
				while ( output != outputEnd )
				{
					*output = makeupGain*(*output);
					output++;
				}
			}
		}
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
void ToneGenerator:: generateWave( SoundBuffer& outputBuffer, Size numSamples,
									Float frequencyChangePerSample, Gain outputGainChangePerSample )
{
	// Compute the constant part of how much the phase of the wave changes per sample.
	const Float twoPi = Float(2)*math::pi<Float>();
	const Float phaseIncrementConstant = Float(twoPi / outputBuffer.getSampleRate());
	
	// Declare current parameter interpolation state variables outside of the processing
	// loop so that we can get their final values.
	Float currentPhase;
	Float currentFrequency;
	Gain currentOutputGain;
	
	Size numChannels = outputBuffer.getChannelCount();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		// Get loop invariants.
		Sample32f* output = outputBuffer.getChannel(c);
		const Sample32f* const outputEnd = output + numSamples;
		
		// Reset the current interpolated parameters for this channel.
		currentOutputGain = outputGain;
		currentFrequency = frequency;
		currentPhase = phase;
		
		// Compute the starting phase increment for the current frequency.
		Float phaseIncrement = phaseIncrementConstant*currentFrequency;
		
		// Generate the wave samples and interpolate parameters.
		while ( output != outputEnd )
		{
			*output = currentOutputGain*waveFunction( currentPhase );
			
			currentOutputGain += outputGainChangePerSample;
			currentFrequency += frequencyChangePerSample;
			currentPhase += phaseIncrement;
			phaseIncrement = phaseIncrementConstant*currentFrequency;
			output++;
		}
	}
	
	
	// Save the final output gain, frequency and phase values for the next buffer output frame.
	outputGain = currentOutputGain;
	frequency = currentFrequency;
	
	// Make sure that the phase stays in a constrained range due to floating point
	// precision problems which can cause frequency drift.
	phase = math::mod( currentPhase, twoPi );
}




//##########################################################################################
//##########################################################################################
//############		
//############		White Noise Generation Method
//############		
//##########################################################################################
//##########################################################################################




void ToneGenerator:: generateWhiteNoise( SoundBuffer& outputBuffer, Size numSamples,
										Gain outputGainChangePerSample  )
{
	const Size numChannels = outputBuffer.getChannelCount();
	
	if ( numChannels > 0 )
	{
		Gain currentOutputGain = outputGain;
		
		{
			// Get loop invariants.
			Sample32f* output = outputBuffer.getChannel(0);
			const Sample32f* const outputEnd = output + numSamples;
			
			// Generate the white noise samples.
			while ( output != outputEnd )
			{
				*output = currentOutputGain*math::random( Float(-1.0), Float(1.0) );
				
				currentOutputGain += outputGainChangePerSample;
				output++;
			}
		}
		
		// Save the final output gain for the next buffer output frame.
		outputGain = currentOutputGain;
		
		//****************************************************************************
		// Copy the noise to the other channels.
		
		for ( Index c = 1; c < numChannels; c++ )
		{
			const Sample32f* first = outputBuffer.getChannel(0);
			Sample32f* output = outputBuffer.getChannel(c);
			const Sample32f* const outputEnd = output + numSamples;
			
			while ( output != outputEnd )
			{
				*output = *first;
				
				first++;
				output++;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Pink Noise Generation Method
//############		
//##########################################################################################
//##########################################################################################




/**
  * This method generates pink noise using the 'reference' quality method
  * by Paul Kellet, described at http://www.firstpr.com.au/dsp/pink-noise/
  */
void ToneGenerator:: generatePinkNoise( SoundBuffer& outputBuffer, Size numSamples,
										Gain outputGainChangePerSample  )
{
	const Size numChannels = outputBuffer.getChannelCount();
	
	// Generate the pink noise in the first channel of the buffer.
	if ( numChannels > 0 )
	{
		Gain currentOutputGain = outputGain;
		
		{
			// Get loop invariants.
			Sample32f* output = outputBuffer.getChannel(0);
			const Sample32f* const outputEnd = output + numSamples;
			Float* h = pinkNoiseHistory;
			
			// Generate the white noise samples.
			while ( output != outputEnd )
			{
				// Get a white noise random value.
				Float white = math::random( Float(-1.0), Float(1.0) );
				
				// Apply a set of first-order filters to the white noise sample.
				h[0] = Float(0.99886)*h[0] + white*Float(0.0555179);
				h[1] = Float(0.99332)*h[1] + white*Float(0.0750759);
				h[2] = Float(0.96900)*h[2] + white*Float(0.1538520);
				h[3] = Float(0.86650)*h[3] + white*Float(0.3104856);
				h[4] = Float(0.55000)*h[4] + white*Float(0.5329522);
				h[5] = Float(-0.7616)*h[5] - white*Float(0.0168980);
				
				// An experimentally-determined gain bias for the generated pink noise that
				// makes the output lie the range [-1,1].
				const Float levelBias = 0.1331f;
				
				// Compute a weighted average of these filters.
				*output = currentOutputGain*levelBias*(h[0] + h[1] + h[2] + h[3] + h[4] + h[5] + h[6] + white*Float(0.5362));
				
				h[6] = Float(0.115926)*white; 
				
				currentOutputGain += outputGainChangePerSample;
				output++;
			}
		}
		
		// Save the final output gain for the next buffer output frame.
		outputGain = currentOutputGain;
		
		//****************************************************************************
		// Copy the noise to the other channels.
		
		for ( Index c = 1; c < numChannels; c++ )
		{
			const Sample32f* first = outputBuffer.getChannel(0);
			Sample32f* output = outputBuffer.getChannel(c);
			const Sample32f* const outputEnd = output + numSamples;
			
			while ( output != outputEnd )
			{
				*output = *first;
				
				first++;
				output++;
			}
		}
	}
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
