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

#include "omSoundPitchShifter.h"


#define PARAMETER_INDEX_SHIFT 0
#define PARAMETER_INDEX_BUFFER_LENGTH 1
#define PARAMETER_INDEX_OVERSAMPLING 2
#define PARAMETER_INDEX_MIX 3
#define PARAMETER_COUNT 4

#define PARAMETER_NAME_SHIFT "Shift"
#define PARAMETER_NAME_BUFFER_LENGTH "Buffer Length"
#define PARAMETER_NAME_OVERSAMPLING "Oversampling"
#define PARAMETER_NAME_MIX "Mix"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String PitchShifter:: NAME( "Pitch Shifter" );
const UTF8String PitchShifter:: MANUFACTURER( "Om Sound" );
const FilterVersion PitchShifter:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




PitchShifter:: PitchShifter()
	:	SoundFilter( 1, 1 ),
		shift( 1 ),
		mix( 1 ),
		targetMix( 1 ),
		bufferLength( 0.020f ),
		oversampling( 4 ),
		currentInputWritePosition( 0 ),
		currentFFTReadPosition( 0 ),
		currentFFTWritePosition( 0 ),
		currentInputReadPosition( 0 ),
		currentOutputReadPosition( 0 ),
		lastSampleRate( 0 )
{
}




PitchShifter:: PitchShifter( Float newSemitoneShift )
	:	SoundFilter( 1, 1 ),
		shift( 1 ),
		mix( 1 ),
		targetMix( 1 ),
		bufferLength( 0.020f ),
		oversampling( 4 ),
		currentInputWritePosition( 0 ),
		currentFFTReadPosition( 0 ),
		currentFFTWritePosition( 0 ),
		currentInputReadPosition( 0 ),
		currentOutputReadPosition( 0 ),
		lastSampleRate( 0 )
{
	this->setSemitoneShift( newSemitoneShift );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String PitchShifter:: getName() const
{
	return NAME;
}




UTF8String PitchShifter:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion PitchShifter:: getVersion() const
{
	return VERSION;
}




FilterCategory PitchShifter:: getCategory() const
{
	return FilterCategory::PITCH;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Latency Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Time PitchShifter:: getLatency() const
{
	if ( lastSampleRate != SampleRate(0) )
	{
		// Compute the most recent latency.
		Size fftSize = math::nextPowerOfTwo( Size(bufferLength*lastSampleRate) );
		
		// Compute latency in seconds.
		return Time( fftSize / lastSampleRate );
	}
	else
		return Time( Double(bufferLength) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size PitchShifter:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool PitchShifter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_SHIFT:
			info = FilterParameterInfo( PARAMETER_INDEX_SHIFT, PARAMETER_NAME_SHIFT,
										FilterParameterType::FLOAT,
										FilterParameterUnits::SEMITONES, FilterParameterCurve::LINEAR,
										Float(-24), Float(24), Float(0),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_BUFFER_LENGTH:
			info = FilterParameterInfo( PARAMETER_INDEX_BUFFER_LENGTH, PARAMETER_NAME_BUFFER_LENGTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::MILLISECONDS, FilterParameterCurve::SQUARE,
										Float(5), Float(200), Float(20),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_OVERSAMPLING:
			info = FilterParameterInfo( PARAMETER_INDEX_OVERSAMPLING, PARAMETER_NAME_OVERSAMPLING,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										Int64(1), Int64(5), Int64(2),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_MIX:
			info = FilterParameterInfo( PARAMETER_INDEX_MIX, PARAMETER_NAME_MIX,
										FilterParameterType::FLOAT,
										FilterParameterUnits::PERCENT, FilterParameterCurve::LINEAR,
										0.0f, 100.0f, 100.0f,
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




Bool PitchShifter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_SHIFT:
			value = FilterParameter( this->getSemitoneShift() );
			return true;
		case PARAMETER_INDEX_BUFFER_LENGTH:
			value = FilterParameter( this->getBufferLength()*1000.0f );
			return true;
		case PARAMETER_INDEX_OVERSAMPLING:
			value = FilterParameter( Int64(this->getOversampling()) );
			return true;
		case PARAMETER_INDEX_MIX:
			value = FilterParameter( this->getMix()*100.0f );
			return true;
	}
	
	return false;
}




Bool PitchShifter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Float floatValue;
	Int64 intValue;
	
	switch( parameterIndex )
	{
		case PARAMETER_INDEX_SHIFT:
			if ( value.getValue( floatValue ) )
			{
				this->setSemitoneShift( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_BUFFER_LENGTH:
			if ( value.getValue( floatValue ) )
			{
				this->setBufferLength( floatValue*0.001f );
				return true;
			}
			break;
		case PARAMETER_INDEX_OVERSAMPLING:
			if ( value.getValue( intValue ) )
			{
				this->setOversampling( (Size)intValue );
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




void PitchShifter:: resetStream()
{
	// Zero the buffers.
	accumulationBuffer.zero();
	inputQueue.zero();
	outputQueue.zero();
	
	// Zero the phase values.
	for ( Index i = 0; i < channelPhases.getSize(); i++ )
	{
		channelPhases[i].lastPhases.setAll( 0 );
		channelPhases[i].phaseSum.setAll( 0 );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult PitchShifter:: processFrame( const SoundFrame& inputFrame,
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
	
	const SampleRate sampleRate = inputBuffer->getSampleRate();
	
	// Compute the required size of the FFT buffer.
	const Size fftSize = math::nextPowerOfTwo( Size(bufferLength*sampleRate) );
	
	// Compute how much the FFT frame advances for each processing step.
	const Size stepSize = fftSize / oversampling;
	
	// Half of the size of the FFT window.
	const Size halfFFTSize = fftSize / 2;
	
	// Compute the constant phase difference between adjacent FFT windows.
	const Float stepPhase = Float(2)*math::pi<Float>()*(Float(stepSize) / Float(fftSize));
	
	// Compute the difference in Hz between adjacent FFT bins.
	const Float frequencyPerBin = Float(sampleRate) / Float(fftSize);
	
	const Size numChannels = inputBuffer->getChannelCount();
	
	// Make sure that the buffers are big enough to hold an FFT frame's worth of samples and have the right format.
	inputBuffer->copyFormatTo( inputQueue, fftSize );
	inputBuffer->copyFormatTo( outputQueue, fftSize );
	inputBuffer->copyFormatTo( accumulationBuffer, fftSize );
	
	// Reset parameter interpolation if this is the first processing frame or the sample rate changed.
	if ( isFirstFrame() || lastSampleRate != sampleRate )
	{
		mix = targetMix;
		currentInputWritePosition = 0;
		currentFFTReadPosition = stepSize;
		currentFFTWritePosition = 0;
		currentInputReadPosition = stepSize;
		currentOutputReadPosition = fftSize - stepSize;
		
		// Zero the buffers.
		accumulationBuffer.zero();
		inputQueue.zero();
		outputQueue.zero();
		
		// Zero the phase values.
		for ( Index i = 0; i < channelPhases.getSize(); i++ )
		{
			channelPhases[i].lastPhases.setAll( 0 );
			channelPhases[i].phaseSum.setAll( 0 );
		}
	}
	
	lastSampleRate = sampleRate;
	
	// Make sure the FFT data buffer is big enough.
	if ( fftData.getSize() < fftSize )
		fftData.setSize( fftSize );
	
	// Make sure that the channel phase array is big enough.
	if ( channelPhases.getSize() < numChannels )
		channelPhases.setSize( numChannels );
	
	// Make sure that the shifted magnitude buffer is big enough.
	if ( shiftedMagnitudes.getSize() < (halfFFTSize + 1) )
		shiftedMagnitudes.setSize( halfFFTSize + 1 );
	
	// Make sure that the shifted magnitude buffer is big enough.
	if ( shiftedFrequencies.getSize() < (halfFFTSize + 1) )
		shiftedFrequencies.setSize( halfFFTSize + 1 );
	
	// Make sure the precomputed window function is correct.
	if ( window.getSize() != fftSize )
	{
		window.setSize( fftSize );
		
		const Float phase = Float(2)*math::pi<Float>() / Float(fftSize - 1);
		
		// Recompute the Hann window.
		for ( Index i = 0; i < fftSize; i++ )
			window[i] = -Float(0.5)*math::cos( Float(i)*phase ) + Float(0.5);
	}
	
	// Compute the change in the mix parameter per sample.
	Gain mixChangePerSample = Gain(0.5)*(targetMix - mix) / numSamples;
	
	//******************************************************************************
	
	Size numSamplesProcessed = 0;
	
	while ( numSamplesProcessed < numSamples )
	{
		// Buffer the input and output in a first-in-first-out manner.
		// This makes sure that there is enough samples available each time
		// an FFT frame is processed.
		const Size numSamplesToProcess = math::min( numSamples - numSamplesProcessed,
													math::min( stepSize,
															((currentFFTReadPosition + fftSize) - currentInputWritePosition) % fftSize ) );
		
		// Copy the input to the internal input buffer, possibly in multiple passes.
		for ( Size samplesRemaining = numSamplesToProcess; samplesRemaining > 0; )
		{
			// Write until the end of the buffer or until we run out of samples.
			Size blockSize = math::min( samplesRemaining, fftSize - currentInputWritePosition );
			
			for ( Index c = 0; c < numChannels; c++ )
			{
				const Sample32f* source = inputBuffer->getChannel(c) + numSamplesProcessed;
				Sample32f* destination = inputQueue.getChannel(c) + currentInputWritePosition;
				
				om::util::copy( destination, source, blockSize );
			}
			
			currentInputWritePosition = (currentInputWritePosition + blockSize) % fftSize;
			samplesRemaining -= blockSize;
		}
		
		//******************************************************************************
		// If the buffer is full, process an FFT frame.
		
		if ( currentInputWritePosition == currentFFTReadPosition )
		{
			for ( Index c = 0; c < numChannels; c++ )
			{
				// Get a pointer to the start of the FFT data buffer.
				ComplexSample* const fftStart = fftData;
				
				// Copy the input to the FFT buffer and apply the windowing function.
				{
					const Sample32f* const inputStart = inputQueue.getChannel(c);
					Index readPosition = currentFFTReadPosition;
					
					for ( Size i = 0; i < fftSize; )
					{
						// Read until the end of the input buffer or until we finish.
						Size blockSize = math::min( fftSize - i, fftSize - readPosition );
						
						ComplexSample* fftWrite = fftStart + i;
						const Float* w = window + i;
						const Sample32f* input = inputStart + readPosition;
						const Sample32f* const inputEnd = input + blockSize;
						
						while ( input != inputEnd )
						{
							*fftWrite = ComplexSample( (*w)*(*input) );
							fftWrite++;
							input++;
							w++;
						}
						
						readPosition = (readPosition + blockSize) % fftSize;
						i += blockSize;
					}
				}
				
				// Do the FFT to frequency domain.
				math::fft( fftStart, fftSize );
				
				//****************************************************************
				// Do frequency analysis and pitch shifting.
				
				// Set the shifted magnitude and frequency arrays to 0.
				shiftedMagnitudes.setAll( 0 );
				shiftedFrequencies.setAll( 0 );
				
				ChannelPhase& channelPhase = channelPhases[c];
				Array<Float>& lastPhases = channelPhase.lastPhases;
				Array<Float>& phaseSum = channelPhase.phaseSum;
				
				// Make sure that the last phase buffer is big enough.
				if ( lastPhases.getSize() < (halfFFTSize + 1) )
					lastPhases.setSize( halfFFTSize + 1 );
				
				// Make sure that the phase sum buffer is big enough.
				if ( phaseSum.getSize() < (halfFFTSize + 1) )
					phaseSum.setSize( halfFFTSize + 1 );
				
				for ( Index i = 0; i <= halfFFTSize; i++ )
				{
					ComplexSample sample = fftStart[i];
					
					// Compute the magnitude and phase of the sample.
					Float magnitude = Float(2)*sample.getMagnitude();
					Float phase = sample.getPhase();
					
					// Compute the phase difference from the last phase and update the stored phase.
					Float phaseDifference = phase - lastPhases[i];
					lastPhases[i] = phase;
					
					// Subtract the expected phase difference, caused by the shift in the FFT window.
					phaseDifference -= Float(i)*stepPhase;
					
					// Map the phase change to the interval [-pi,pi].
					long qpd = long(phaseDifference/math::pi<Float>());
					
					if ( qpd >= 0 )
						qpd += qpd & 1;
					else
						qpd -= qpd & 1;
					
					phaseDifference -= math::pi<Float>()*(Float)qpd;
					
					// Get the deviation from the bin frequency.
					Float frequencyDeviation = phaseDifference*Float(oversampling) / (Float(2)*math::pi<Float>());
					
					// Compute the true frequency for this bin.
					Float binFrequency = Float(i)*frequencyPerBin + frequencyDeviation*frequencyPerBin;
					
					//****************************************************************
					// Shift the bin frequency and add its contribution to the final bins.
					
					// Compute the bin index where this shifted partial should be placed.
					Index shiftIndex = Index(i*shift);
					
					// Don't use shifted partials that are above the sample rate.
					if ( shiftIndex <= halfFFTSize )
					{
						shiftedMagnitudes[shiftIndex] += magnitude;
						shiftedFrequencies[shiftIndex] = binFrequency*shift;
					}
				}
				
				//****************************************************************
				// Synthesize the shifted partials.
				
				for ( Index i = 0; i <= halfFFTSize; i++ )
				{
					// Get the magnitude and actual frequency from the shifted bins.
					Float magnitude = shiftedMagnitudes[i];
					Float binFrequency = shiftedFrequencies[i];
					
					// Subtract the bin's middle frequency to get the deviation from the bin's frequency.
					Float frequencyDeviation = binFrequency - Float(i)*frequencyPerBin;
					
					// Get the bin's deviation from the bin's frequency deviation.
					Float binDeviation = frequencyDeviation / frequencyPerBin;
					
					// Take oversampling into account and convert to phase difference.
					Float phaseDeviation = binDeviation*Float(2)*math::pi<Float>() / Float(oversampling);
					
					// Add the expected phase difference, caused by the shift in the FFT window.
					phaseDeviation += Float(i)*stepPhase;
					
					// Accumulate the change in phase to get the phase for this bin.
					phaseSum[i] += phaseDeviation;
					Float phase = phaseSum[i];
					
					// Compute the complex partial for this bin.
					fftStart[i] = ComplexSample( magnitude*math::cos(phase), magnitude*math::sin(phase) );
				}
				
				// Zero the negative frequency partials.
				for ( Index i = halfFFTSize + 1; i < fftSize; i++ )
					fftStart[i] = ComplexSample();
				
				// Do the inverse transform back to time domain.
				math::ifft( fftStart, fftSize );
				
				//****************************************************************
				// Accumulate output.
				
				const Float outputNormalize = Float(2) / (Float(halfFFTSize)*Float(oversampling));
				
				Sample32f* const accumulationStart = accumulationBuffer.getChannel(c);
				
				// Apply the window function and accumulate the output to the accumulation buffer.
				{
					Index writePosition = currentFFTWritePosition;
					
					for ( Index i = 0; i < fftSize; )
					{
						Size blockSize = math::min( fftSize - i, fftSize - writePosition );
						
						const ComplexSample* fftRead = fftStart + i;
						const Float* w = window + i;
						Sample32f* accumulation = accumulationStart + writePosition;
						const Sample32f* const accumulationEnd = accumulation + blockSize;
						
						while ( accumulation != accumulationEnd )
						{
							*accumulation += Sample32f( (*w)*outputNormalize*fftRead->r );
							fftRead++;
							accumulation++;
							w++;
						}
						
						writePosition = (writePosition + blockSize) % fftSize;
						i += blockSize;
					}
				}
				
				
				//****************************************************************
				// Copy accumulation buffer to output.
				
				{
					Sample32f* const outputStart = outputQueue.getChannel(c);
					Index writePosition = currentFFTWritePosition;
					
					for ( Size samplesRemaining = stepSize; samplesRemaining > 0; )
					{
						Size blockSize = math::min( samplesRemaining, fftSize - writePosition );
						
						Sample32f* accumulation = accumulationStart + writePosition;
						Sample32f* output = outputStart + writePosition;
						const Sample32f* const outputEnd = output + blockSize;
						
						while ( output != outputEnd )
						{
							*output = *accumulation;
							
							// Zero the accumulation buffer after the copy so that it can be reused.
							*accumulation = Sample32f(0);
							
							output++;
							accumulation++;
						}
						
						writePosition = (writePosition + blockSize) % fftSize;
						samplesRemaining -= blockSize;
					}
				}
			}
			
			// Increment the fft read and write positions by one step size.
			currentFFTReadPosition = (currentFFTReadPosition + stepSize) % fftSize;
			currentFFTWritePosition = (currentFFTWritePosition + stepSize) % fftSize;
		}
		
		//******************************************************************************
		
		Gain currentMix;
		
		// Copy the internal output buffer to the output buffer.
		for ( Size samplesRemaining = numSamplesToProcess; samplesRemaining > 0; )
		{
			Size blockSize = math::min( samplesRemaining, math::min( fftSize - currentOutputReadPosition,
																	fftSize - currentInputReadPosition ) );
			
			for ( Index c = 0; c < numChannels; c++ )
			{
				const Sample32f* input = inputQueue.getChannel(c) + currentInputReadPosition;
				const Sample32f* output = outputQueue.getChannel(c) + currentOutputReadPosition;
				Sample32f* destination = outputBuffer->getChannel(c) + numSamplesProcessed;
				const Sample32f* const destinationEnd = destination + numSamplesToProcess;
				
				currentMix = mix;
				
				while ( destination != destinationEnd )
				{
					*destination = (*output)*currentMix + (*input)*(Gain(1) - currentMix);
					destination++;
					output++;
					input++;
					
					currentMix += mixChangePerSample;
				}
			}
			
			currentOutputReadPosition = (currentOutputReadPosition + blockSize) % fftSize;
			currentInputReadPosition = (currentInputReadPosition + blockSize) % fftSize;
			samplesRemaining -= blockSize;
		}
		
		mix = currentMix;
		numSamplesProcessed += numSamplesToProcess;
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
