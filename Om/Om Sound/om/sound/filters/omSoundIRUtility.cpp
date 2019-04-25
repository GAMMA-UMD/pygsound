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

#include "omSoundIRUtility.h"


#define PARAMETER_INDEX_CHANNEL_COUNT 0
#define PARAMETER_INDEX_SAMPLE_RATE 1
#define PARAMETER_INDEX_IR_LENGTH 2
#define PARAMETER_INDEX_STIMULUS_LENGTH 3
#define PARAMETER_INDEX_MEASUREMENT_COUNT 4
#define PARAMETER_COUNT 5

#define PARAMETER_NAME_CHANNEL_COUNT "Channel Count"
#define PARAMETER_NAME_SAMPLE_RATE "Sample Rate"
#define PARAMETER_NAME_IR_LENGTH "IR Length"
#define PARAMETER_NAME_STIMULUS_LENGTH "Stimulus Length"
#define PARAMETER_NAME_MEASUREMENT_COUNT "Measurement Count"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String IRUtility:: NAME( "IR Utility" );
const UTF8String IRUtility:: MANUFACTURER( "Om Sound" );
const FilterVersion IRUtility:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




IRUtility:: IRUtility()
	:	channelCount( 1 ),
		sampleRate( 44100.0 ),
		irLength( 1.0 ),
		irLengthInSamples( 0 ),
		stimulusLength( 0.1 ),
		measurementLength( 0 ),
		measurementCount( 1 ),
		measurementIndex( 0 ),
		measurementPosition( 0 ),
		golayLength( 0 ),
		measuring( false ),
		deconvolution( false )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Measurement Methods
//############		
//##########################################################################################
//##########################################################################################




void IRUtility:: start()
{
	lockMutex();
	
	// Compute the length of the IR in the requested sample rate.
	irLengthInSamples = math::max( Size(1), (Size)math::ceiling( irLength.getSeconds() * sampleRate ) );
	
	// Compute the length of the golay codes.
	const Size stimulusLengthInSamples = math::max( Size(2), (Size)math::ceiling( stimulusLength.getSeconds() * sampleRate ) );
	const Size newGolayLength = math::isPowerOfTwo(stimulusLengthInSamples) ? stimulusLengthInSamples : 
													math::nextPowerOfTwo( stimulusLengthInSamples );
	
	// Generate the golay codes.
	if ( newGolayLength != golayLength )
	{
		// Initialize the golay code computation.
		if ( golayLength == 0 )
		{
			golay.allocate( 2, newGolayLength );
			golay[0][0] = 1;
			golay[0][1] = 1;
			golay[1][0] = 1;
			golay[1][1] = -1;
			golayLength = 2;
		}
		else
			golay.setSizeCopy( 2, newGolayLength );
		
		// Iterate until the golay codes are long enough.
		while ( golayLength < newGolayLength )
		{
			util::copy( golay[0] + golayLength, golay[1], golayLength );
			math::multiply( golay[1] + golayLength, golay[1], Float(-1), golayLength );
			util::copy( golay[1], golay[0], golayLength );
			
			golayLength *= 2;
		}
		
		golayLength = newGolayLength;
	}
	
	// The measurement consists of 2 samples, each of length 2*N-1.
	// Each sample consists of a golay code of length N and N-1 samples of code response.
	measurementLength = 2*(golayLength + irLengthInSamples);
	
	// Make sure the recorded response buffer is large enough.
	response.allocate( channelCount, measurementLength );
	
	// Initialize the measurement state.
	measurementIndex = 0;
	measurementPosition = 0;
	measuring = true;
	deconvolution = false;
	
	unlockMutex();
}




void IRUtility:: stop()
{
	lockMutex();
	
	measuring = false;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Deconvolution Method
//############		
//##########################################################################################
//##########################################################################################




Bool IRUtility:: deconvolve( SoundBuffer& ir ) const
{
	if ( !deconvolution )
		return false;
	
	// Initialize the output IR buffer.
	ir.setChannelCount( channelCount );
	ir.setSampleRate( sampleRate );
	ir.setSize( irLengthInSamples );
	
	const Index response0End = golayLength + irLengthInSamples;
	
	// Generate the IR for this measurement and accumulate it in the output IR buffer.
	// Compute the normalization factor for the IR.
	const Float normalize = Float(1) / (Float(2*measurementCount)*Float(golayLength));
	
	for ( Index c = 0; c < channelCount; c++ )
	{
		Sample32f* irC = ir.getChannel(c);
		
		for ( Index i = 0; i < irLengthInSamples; i++ )
		{
			Sample32f out = 0;
			
			for ( Index k = 0; k < golayLength; k++ )
				out += golay[0][k]*response[c][k+i] + golay[1][k]*response[c][k + i + response0End];
			
			irC[i] = out*normalize;
		}
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Count Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void IRUtility:: setChannelCount( Size newChannelCount )
{
	lockMutex();
	
	if ( measuring )
	{
		unlockMutex();
		stop();
		lockMutex();
	}
	
	channelCount = math::max( Size(1), newChannelCount );
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		IR Length Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void IRUtility:: setIRLength( const Time& newIRLength )
{
	lockMutex();
	
	if ( measuring )
	{
		unlockMutex();
		stop();
		lockMutex();
	}
	
	irLength = newIRLength;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Rate Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void IRUtility:: setSampleRate( SampleRate newSampleRate )
{
	lockMutex();
	
	if ( measuring )
	{
		unlockMutex();
		stop();
		lockMutex();
	}
	
	sampleRate = math::max( SampleRate(0), newSampleRate );
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stimulus Length Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void IRUtility:: setStimulusLength( const Time& newStimulusLength )
{
	lockMutex();
	
	if ( measuring )
	{
		unlockMutex();
		stop();
		lockMutex();
	}
	
	stimulusLength = newStimulusLength;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Measurement Count Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void IRUtility:: setMeasurementCount( Size newMeasurementCount )
{
	lockMutex();
	
	if ( measuring )
	{
		unlockMutex();
		stop();
		lockMutex();
	}
	
	measurementCount = math::max( Size(1), newMeasurementCount );
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String IRUtility:: getName() const
{
	return NAME;
}




UTF8String IRUtility:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion IRUtility:: getVersion() const
{
	return VERSION;
}




FilterCategory IRUtility:: getCategory() const
{
	return FilterCategory::ANALYSIS;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size IRUtility:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool IRUtility:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_CHANNEL_COUNT:
			info = FilterParameterInfo( PARAMETER_INDEX_CHANNEL_COUNT, PARAMETER_NAME_CHANNEL_COUNT,
										FilterParameterType::INTEGER,
										FilterParameterUnits::GENERIC, FilterParameterCurve::LINEAR,
										Int64(1), Int64(8), Int64(1),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_SAMPLE_RATE:
			info = FilterParameterInfo( PARAMETER_INDEX_SAMPLE_RATE, PARAMETER_NAME_SAMPLE_RATE,
										FilterParameterType::INTEGER,
										FilterParameterUnits::GENERIC, FilterParameterCurve::LINEAR,
										Int64(1), Int64(8), Int64(1),
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_IR_LENGTH:
			info = FilterParameterInfo( PARAMETER_INDEX_IR_LENGTH, PARAMETER_NAME_IR_LENGTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::GENERIC, FilterParameterCurve::SQUARE,
										0.0f, 10.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_STIMULUS_LENGTH:
			info = FilterParameterInfo( PARAMETER_INDEX_STIMULUS_LENGTH, PARAMETER_NAME_STIMULUS_LENGTH,
										FilterParameterType::FLOAT,
										FilterParameterUnits::GENERIC, FilterParameterCurve::SQUARE,
										0.0f, 10.0f, 0.1f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_MEASUREMENT_COUNT:
			info = FilterParameterInfo( PARAMETER_INDEX_MEASUREMENT_COUNT, PARAMETER_NAME_MEASUREMENT_COUNT,
										FilterParameterType::INTEGER,
										FilterParameterUnits::GENERIC, FilterParameterCurve::LINEAR,
										Int64(1), Int64(10), Int64(1),
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




Bool IRUtility:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_CHANNEL_COUNT:
			value = FilterParameter( (Int64)this->getChannelCount() );
			return true;
		case PARAMETER_INDEX_SAMPLE_RATE:
			value = FilterParameter( this->getSampleRate() );
			return true;
		case PARAMETER_INDEX_IR_LENGTH:
			value = FilterParameter( (Float)this->getIRLength() );
			return true;
		case PARAMETER_INDEX_STIMULUS_LENGTH:
			value = FilterParameter( (Float)this->getStimulusLength() );
			return true;
		case PARAMETER_INDEX_MEASUREMENT_COUNT:
			value = FilterParameter( (Int64)this->getMeasurementCount() );
			return true;
	}
	
	return false;
}




Bool IRUtility:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Int64 intValue;
	Float floatValue;
	Double doubleValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_CHANNEL_COUNT:
			if ( value.getValue( intValue ) )
			{
				this->setChannelCount( (Size)intValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_SAMPLE_RATE:
			if ( value.getValue( doubleValue ) )
			{
				this->setSampleRate( doubleValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_IR_LENGTH:
			if ( value.getValue( floatValue ) )
			{
				this->setIRLength( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_STIMULUS_LENGTH:
			if ( value.getValue( floatValue ) )
			{
				this->setStimulusLength( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_MEASUREMENT_COUNT:
			if ( value.getValue( intValue ) )
			{
				this->setMeasurementCount( (Size)intValue );
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




void IRUtility:: resetStream()
{
	if ( measuring )
		stop();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult IRUtility:: processFrame( const SoundFrame& inputFrame,
												SoundFrame& outputFrame, Size numSamples )
{
	SoundBuffer* outputBuffer;
	
	// If there is no output buffer or if it is NULL, return that no samples were processed.
	if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
		return 0;
	
	const SoundBuffer* inputBuffer;
	
	// If there is no input buffer or if it is NULL, return that an error occurred.
	if ( inputFrame.getBufferCount() == 0 || (inputBuffer = inputFrame.getBuffer(0)) == NULL )
		return SoundResult::ERROR;
	
	const Size numInputChannels = math::min( inputBuffer->getChannelCount(), channelCount );
	
	//*************************************************************************
	// Fill the output buffer with the stimulus signal.
	
	// Make sure the output buffer has the correct format.
	outputBuffer->setChannelCount( channelCount );
	outputBuffer->setSampleRate( sampleRate );
	
	// Process the samples until the output buffer is filled.
	Size numSamplesProcessed = 0;
	
	if ( measuring )
	{
		// Compute the sample indices of measurement events.
		const Index golay0End = golayLength;
		const Index response0End = golay0End + irLengthInSamples;
		const Index golay1End = response0End + golayLength;
		const Index response1End = golay1End + irLengthInSamples;
		
		while ( numSamplesProcessed < numSamples )
		{
			Size numSamplesToProcess = numSamples - numSamplesProcessed;
			
			if ( measurementPosition < golay0End )
			{
				// Generate the first golay code.
				numSamplesToProcess = math::min( numSamplesToProcess, golay0End - measurementPosition );
				
				for ( Index c = 0; c < channelCount; c++ )
				{
					util::copy( outputBuffer->getChannel(c) + numSamplesProcessed,
								golay[0] + measurementPosition, numSamplesToProcess );
				}
			}
			else if ( measurementPosition < response0End )
			{
				// Generate silence.
				numSamplesToProcess = math::min( numSamplesToProcess, response0End - measurementPosition );
				
				for ( Index c = 0; c < channelCount; c++ )
					util::zeroPOD( outputBuffer->getChannel(c) + numSamplesProcessed, numSamplesToProcess );
			}
			else if ( measurementPosition < golay1End )
			{
				// Generate the second golay code.
				numSamplesToProcess = math::min( numSamplesToProcess, golay1End - measurementPosition );
				
				for ( Index c = 0; c < channelCount; c++ )
				{
					util::copy( outputBuffer->getChannel(c) + numSamplesProcessed,
								golay[1] + (measurementPosition - response0End), numSamplesToProcess );
				}
			}
			else if ( measurementPosition < response1End )
			{
				// Generate silence.
				numSamplesToProcess = math::min( numSamplesToProcess, response1End - measurementPosition );
				
				for ( Index c = 0; c < channelCount; c++ )
					util::zeroPOD( outputBuffer->getChannel(c) + numSamplesProcessed, numSamplesToProcess );
			}
			
			// Copy the input audio to the response recording.
			for ( Index c = 0; c < channelCount; c++ )
			{
				if ( measurementIndex == 0 )
				{
					if ( c < numInputChannels )
						util::copyPOD( response[c] + measurementPosition,
									inputBuffer->getChannel(c) + numSamplesProcessed, numSamplesToProcess );
					else
						util::zeroPOD( response[c] + measurementPosition, numSamplesToProcess );
				}
				else
				{
					if ( c < numInputChannels )
						math::add( response[c] + measurementPosition,
									inputBuffer->getChannel(c) + numSamplesProcessed, numSamplesToProcess );
				}
			}
			
			// Advance the measurement position.
			measurementPosition += numSamplesToProcess;
			numSamplesProcessed += numSamplesToProcess;
			
			// Determine whether or not the measurement is finished.
			if ( measurementPosition == measurementLength )
			{
				// Move to the next measurement.
				measurementPosition = 0;
				measurementIndex++;
				
				// Stop measuring if this was the last measurement.
				if ( measurementIndex == measurementCount )
				{
					measuring = false;
					deconvolution = true;
					break;
				}
			}
		}
	}
	
	
	if ( !measuring )
	{
		// Generate silence.
		for ( Index c = 0; c < channelCount; c++ )
			util::zeroPOD( outputBuffer->getChannel(c) + numSamplesProcessed, numSamples - numSamplesProcessed );
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
