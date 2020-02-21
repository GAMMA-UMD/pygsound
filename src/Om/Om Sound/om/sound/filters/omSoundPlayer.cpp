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

#include "omSoundPlayer.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String SoundPlayer:: NAME( "SoundPlayer" );
const UTF8String SoundPlayer:: MANUFACTURER( "Om Sound" );
const FilterVersion SoundPlayer:: VERSION( 1, 0, 0 );
const PanDirection SoundPlayer:: UNDEFINED_STREAM_PAN;


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundPlayer:: SoundPlayer()
	:	SoundFilter( 0, 1 ),
		outputSampleRate( 44100 ),
		outputChannelLayout( ChannelLayout::STEREO ),
		numPlaying( 0 ),
		maxInstanceCount( DEFAULT_MAX_INSTANCE_COUNT ),
		currentPosition( 0 ),
		globalPlayingEnabled( true )
{
}




SoundPlayer:: SoundPlayer( const ChannelLayout& newOutputChannelLayout, SampleRate newSampleRate,
							Size newMaxInstanceCount )
	:	SoundFilter( 0, 1 ),
		outputSampleRate( newSampleRate > SampleRate(0) ? newSampleRate : SampleRate(44100) ),
		outputChannelLayout( newOutputChannelLayout ),
		numPlaying( 0 ),
		maxInstanceCount( newMaxInstanceCount ),
		currentPosition( 0 ),
		globalPlayingEnabled( true )
{
}




SoundPlayer:: SoundPlayer( const SoundPlayer& other )
	:	outputSampleRate( other.outputSampleRate ),
		outputChannelLayout( other.outputChannelLayout ),
		numPlaying( other.numPlaying ),
		maxInstanceCount( other.maxInstanceCount ),
		globalPlayingEnabled( other.globalPlayingEnabled ),
		currentPosition( other.currentPosition ),
		instances( other.instances )
{
	// Go through and set all sample rate converter pointers to NULL because they can't be copied.
	for ( Index i = 0; i < instances.getSize(); i++ )
		instances[i].resampler = NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundPlayer:: ~SoundPlayer()
{
	for ( Index i = 0; i < instances.getSize(); i++ )
	{
		if ( instances[i].resampler != NULL )
			om::util::destruct( instances[i].resampler );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundPlayer& SoundPlayer:: operator = ( const SoundPlayer& other )
{
	if ( this != &other )
	{
		lockMutex();
		
		// Clean up sampler state.
		for ( Index i = 0; i < instances.getSize(); i++ )
		{
			if ( instances[i].resampler != NULL )
				om::util::destruct( instances[i].resampler );
		}
		
		instances.clear();
		
		// Assign new sampler state.
		outputSampleRate = other.outputSampleRate;
		outputChannelLayout = other.outputChannelLayout;
		numPlaying = other.numPlaying;
		maxInstanceCount = other.maxInstanceCount;
		currentPosition = other.currentPosition;
		globalPlayingEnabled = other.globalPlayingEnabled;
		instances.addAll( other.instances );
		
		// Go through and set all sample rate converter pointers to NULL because they can't be copied.
		for ( Index i = 0; i < instances.getSize(); i++ )
			instances[i].resampler = NULL;
		
		unlockMutex();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPlayer:: canSeek() const
{
	return false;
}




Bool SoundPlayer:: canSeek( Int64 relativeSampleOffset ) const
{
	return false;
}




Int64 SoundPlayer:: seek( Int64 relativeSampleOffset )
{
	return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Size Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundSize SoundPlayer:: getSamplesRemaining() const
{
	return math::max<SoundSize>();
}




SampleIndex SoundPlayer:: getPosition() const
{
	return currentPosition;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundPlayer:: getChannelCount() const
{
	return outputChannelLayout.getChannelCount();
}




SampleRate SoundPlayer:: getSampleRate() const
{
	return outputSampleRate;
}




SampleType SoundPlayer:: getNativeSampleType() const
{
	return SampleType::SAMPLE_32F;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Stream Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPlayer:: isValid() const
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String SoundPlayer:: getName() const
{
	return NAME;
}




UTF8String SoundPlayer:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion SoundPlayer:: getVersion() const
{
	return VERSION;
}




FilterCategory SoundPlayer:: getCategory() const
{
	return FilterCategory::PLAYBACK;
}




Bool SoundPlayer:: allowsInPlaceProcessing() const
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Playback Methods
//############		
//##########################################################################################
//##########################################################################################




SoundPlayer::InstanceID SoundPlayer:: play( const Instance& instance )
{
	// If the stream pointer is NULL, don't play the stream and return failure.
	if ( instance.stream == NULL )
		return 0;
	
	lockMutex();
	
	// Are there any unused stream slots?
	if ( instances.getSize() > numPlaying )
	{
		// Find the first unused stream slot and replace it.
		for ( Index i = 0; i < instances.getSize(); i++ )
		{
			InstanceInfo& instanceInfo = instances[i];
			
			if ( instanceInfo.isUnused() )
			{
				// We found an unused slot. Replace the stream parameters.
				instanceInfo.instance = instance;
				instanceInfo.instance.loop &= instance.stream->canSeek();
				instanceInfo.currentStreamPosition = 0;
				instanceInfo.playing = true;
				numPlaying++;
				
				unlockMutex();
				
				// Compute and return the ID for this stream.
				return i + 1;
			}
		}
	}
	
	// Can we add a new stream to the sampler?
	if ( instances.getSize() < maxInstanceCount )
	{
		// Create and add a new stream to the sampler.
		instances.addNew( instance );
		InstanceInfo& instanceInfo = instances.getLast();
		instanceInfo.instance.loop &= instance.stream->canSeek();
		Index streamID = instances.getSize();
		numPlaying++;
		
		unlockMutex();
		
		// Return the ID of the new stream.
		return streamID;
	}
	else if ( instances.getSize() > 0 )
	{
		// We need to replace a currently playing stream.
		// Find the stream with the smallest priority, using the oldest stream in the case
		// of a tie.
		Bool foundStream = false;
		Index smallestPriorityIndex = 0;
		Float smallestPriority = instance.priority;
		SampleIndex oldestAge = 0;
		
		for ( Index i = 0; i < instances.getSize(); i++ )
		{
			InstanceInfo& instanceInfo = instances[i];
			
			if ( instanceInfo.instance.priority < smallestPriority ||
				(instanceInfo.instance.priority == smallestPriority &&
				instanceInfo.currentStreamPosition > oldestAge) )
			{
				foundStream = true;
				smallestPriorityIndex = i;
				smallestPriority = instanceInfo.instance.priority;
				oldestAge = instanceInfo.currentStreamPosition;
			}
		}
		
		if ( foundStream )
		{
			InstanceInfo& instanceInfo = instances[smallestPriorityIndex];
			
			// If we found a stream which can be replaced, replace it.
			instanceInfo.instance = instance;
			instanceInfo.instance.loop &= instance.stream->canSeek();
			instanceInfo.currentStreamPosition = 0;
			instanceInfo.playing = true;
			
			unlockMutex();
			return smallestPriorityIndex + 1;
		}
	}
	
	unlockMutex();
	
	return 0;
}




Bool SoundPlayer:: update( InstanceID instanceID, const Instance& instance )
{
	Index instanceIndex = instanceID - 1;
	
	lockMutex();
	
	if ( instanceIndex < instances.getSize() )
	{
		InstanceInfo& instanceInfo = instances[instanceIndex];
		instanceInfo.instance = instance;
		instanceInfo.instance.loop &= instance.stream->canSeek();
		
		unlockMutex();
		return true;
	}
	
	unlockMutex();
	return false;
}




Bool SoundPlayer:: pause( InstanceID instanceID )
{
	Index instanceIndex = instanceID - 1;
	
	lockMutex();
	
	if ( instanceIndex < instances.getSize() )
	{
		instances[instanceIndex].playing = false;
		unlockMutex();
		return true;
	}
	
	unlockMutex();
	return false;
}




Bool SoundPlayer:: resume( InstanceID instanceID )
{
	Index instanceIndex = instanceID - 1;
	
	lockMutex();
	
	if ( instanceIndex < instances.getSize() && instances[instanceIndex].instance.stream != NULL )
	{
		instances[instanceIndex].playing = true;
		unlockMutex();
		return true;
	}
	
	unlockMutex();
	return false;
}




Bool SoundPlayer:: stop( InstanceID instanceID )
{
	Index instanceIndex = instanceID - 1;
	
	lockMutex();
	
	if ( instanceIndex < instances.getSize() )
	{
		instances[instanceIndex].instance.stream = NULL;
		numPlaying--;
		
		unlockMutex();
		return true;
	}
	
	unlockMutex();
	return false;
}




void SoundPlayer:: pauseAll()
{
	lockMutex();
	
	for ( Index i = 0; i < instances.getSize(); i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		
		if ( instanceInfo.instance.stream != NULL )
			instanceInfo.playing = false;
	}
	
	unlockMutex();
}




void SoundPlayer:: resumeAll()
{
	lockMutex();
	
	for ( Index i = 0; i < instances.getSize(); i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		
		if ( instanceInfo.instance.stream != NULL )
			instanceInfo.playing = true;
	}
	
	unlockMutex();
}




void SoundPlayer:: stopAll()
{
	lockMutex();
	
	for ( Index i = 0; i < instances.getSize(); i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		instanceInfo.instance.stream = NULL;
	}
	
	numPlaying = 0;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		User Playback Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPlayer:: updateUserID( Index userID, const Instance& instance )
{
	lockMutex();
	
	const Size numInstances = instances.getSize();
	Bool result = false;
	
	for ( Index i = 0; i < numInstances; i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		
		if ( instanceInfo.instance.userID == userID && instanceInfo.instance.stream != NULL )
		{
			instanceInfo.instance = instance;
			instanceInfo.instance.loop &= instance.stream->canSeek();
			result = true;
		}
	}
	
	unlockMutex();
	
	return result;
}




Bool SoundPlayer:: pauseUserID( Index userID )
{
	lockMutex();
	
	const Size numInstances = instances.getSize();
	Bool result = false;
	
	for ( Index i = 0; i < numInstances; i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		
		if ( instanceInfo.instance.userID == userID && instanceInfo.instance.stream != NULL )
		{
			instanceInfo.playing = false;
			result = true;
		}
	}
	
	unlockMutex();
	
	return result;
}




Bool SoundPlayer:: resumeUserID( Index userID )
{
	lockMutex();
	
	const Size numInstances = instances.getSize();
	Bool result = false;
	
	for ( Index i = 0; i < numInstances; i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		
		if ( instanceInfo.instance.userID == userID && instanceInfo.instance.stream != NULL )
		{
			instanceInfo.playing = true;
			result = true;
		}
	}
	
	unlockMutex();
	
	return result;
}




Bool SoundPlayer:: stopUserID( Index userID )
{
	lockMutex();
	
	const Size numInstances = instances.getSize();
	Bool result = false;
	
	for ( Index i = 0; i < numInstances; i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		
		if ( instanceInfo.instance.userID == userID && instanceInfo.instance.stream != NULL )
		{
			instanceInfo.instance.stream = NULL;
			numPlaying--;
			result = true;
		}
	}
	
	
	unlockMutex();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Channel Layout Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPlayer:: setOutputChannelLayout( const ChannelLayout& newChannelLayout )
{
	lockMutex();
	
	outputChannelLayout = newChannelLayout;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Output Sample Rate Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPlayer:: setOutputSampleRate( SampleRate newSampleRate )
{
	if ( newSampleRate <= SampleRate(0) )
		return;
	
	lockMutex();
	
	outputSampleRate = newSampleRate;
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPlayer:: resetStream()
{
	currentPosition = 0;
	
	const Size numInstances = instances.getSize();
	
	for ( Index i = 0; i < numInstances; i++ )
	{
		InstanceInfo& instance = instances[i];
		
		if ( instance.resampler )
			instance.resampler->reset();
		
		if ( instance.instance.insert )
			instance.instance.insert->reset();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult SoundPlayer:: processFrame( const SoundFrame& inputFrame, SoundFrame& outputFrame, Size numSamples )
{
	if ( outputFrame.getBufferCount() == 0 )
		return SoundResult::ERROR;
	
	SoundBuffer* outputBuffer = outputFrame.getBuffer(0);
	
	// Don't produce any sound and return there was an error if the output buffer is NULL.
	if ( outputBuffer == NULL )
		return SoundResult::ERROR;
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************
	
	// Zero the output buffer.
	outputBuffer->zero( 0, numSamples );
	
	// If the sampler doesn't have global playback enabled, return.
	if ( !globalPlayingEnabled )
		return numSamples;
	
	// Configure the output buffer's sample rate and channel layout.
	outputBuffer->setSampleRate( outputSampleRate );
	outputBuffer->setLayout( outputChannelLayout );
	
	const Size numOutputChannels = outputChannelLayout.getChannelCount();
	
	// Get a temporary buffer to hold the output of each stream.
	SharedSoundBuffer streamOutputBuffer = SharedBufferPool::getGlobalBuffer();
	SharedSoundBuffer filterOutputBuffer = SharedBufferPool::getGlobalBuffer();
	
	// Iterate over each stream and play it.
	for ( Index i = 0; i < instances.getSize(); i++ )
	{
		InstanceInfo& instanceInfo = instances[i];
		SoundInputStream* stream = instanceInfo.instance.stream;
		
		// Skip invalid or not-playing instances.
		if ( stream == NULL || !instanceInfo.playing )
			continue;
		
		// Make sure that the stream has the correct position (in case anyone else is reading from it).
		Int64 oldStreamPosition = stream->getPosition();
		
		if ( oldStreamPosition != instanceInfo.currentStreamPosition && stream->canSeek() )
		{
			// Seek to the current stream position.
			stream->seek( (Int64)instanceInfo.currentStreamPosition - oldStreamPosition );
		}
		
		// If the stream has no samples remaining, deactivate it if it is not looping.
		if ( !stream->hasSamplesRemaining() )
		{
			if ( instanceInfo.instance.loop )
			{
				// Seek to the begining of the stream (as we know it).
				Int64 seekAmount = stream->seek( -Int64(instanceInfo.currentStreamPosition) );
				instanceInfo.currentStreamPosition += seekAmount;
			}
			else
			{
				// Remove the stream.
				instanceInfo.playing = false;
				instanceInfo.instance.stream = NULL;
				numPlaying--;
				continue;
			}
		}
		
		//************************************************************************************
		
		SoundBuffer* streamBuffer = &streamOutputBuffer.getBuffer();
		SoundBuffer* alternateBuffer = &filterOutputBuffer.getBuffer();
		
		// Compute the number of samples that are needed for the new stream, based on the output and stream sample rates.
		Size numStreamSamples = Size(numSamples*(stream->getSampleRate() / outputSampleRate));
		
		// Get the output of the stream.
		numStreamSamples = stream->read( *streamBuffer, numStreamSamples );
		
		// Compute the current position within the stream.
		instanceInfo.currentStreamPosition += numStreamSamples;
		
		//************************************************************************************
		
		// Do sample rate conversion on the stream's audio to convert it to the output sample rate.
		if ( streamBuffer->getSampleRate() != outputSampleRate )
		{
			// Create a sample rate converter for the stream if it doesn't have one yet.
			if ( instanceInfo.resampler == NULL )
			{
				instanceInfo.resampler = om::util::construct<Resampler>( Resampler::BEST );
				instanceInfo.resampler->setIsSynchronized( false );
			}
			
			// Set the output sample rate.
			instanceInfo.resampler->setOutputSampleRate( outputSampleRate );
			
			// Convert the sample rate.
			numStreamSamples = instanceInfo.resampler->process( *streamBuffer, *alternateBuffer, numStreamSamples );
			
			// Swap the stream output buffers.
			SoundBuffer* temp = streamBuffer;
			streamBuffer = alternateBuffer;
			alternateBuffer = temp;
		}
		
		//************************************************************************************
		
		// Process the inserted filter for the stream if it has one.
		if ( instanceInfo.instance.insert != NULL )
		{
			numStreamSamples = instanceInfo.instance.insert->process( *streamBuffer,
															*alternateBuffer, numStreamSamples );
			
			// Indicate the final output buffer for the stream.
			streamBuffer = alternateBuffer;
		}
		
		// Make sure that we don't overflow the output buffer.
		numStreamSamples = math::min( numStreamSamples, numSamples );
		
		//************************************************************************************
		// Determine the target panning gain matrix for the stream's layout.
		
		outputChannelLayout.panLayout( streamBuffer->getLayout(), instanceInfo.instance.pan, targetChannelGains );
		
		// Make sure the current channel gain matrix has the correct size.
		if ( channelGains.getInputCount() != targetChannelGains.getInputCount() || 
			channelGains.getOutputCount() != targetChannelGains.getOutputCount() ||
			instanceInfo.currentStreamPosition == 0 )
		{
			channelGains = targetChannelGains;
		}
		
		// Get the total number of channels for the stream.
		const Size numInputChannels = streamBuffer->getChannelCount();
		
		//************************************************************************************
		// Render the stream to the output buffer.
		
		// Compute interpolation parameters for the stream's playback gain.
		const Gain streamGainChangerPerSample = Gain(0.5)*(instanceInfo.targetGain - instanceInfo.instance.gain) / numStreamSamples;
		Gain streamGain;
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			Sample32f* const outputStart = outputBuffer->getChannel(c);
			const Sample32f* const outputEnd = outputStart + numStreamSamples;
			
			for ( Index c2 = 0; c2 < numInputChannels; c2++ )
			{
				// Compute the target input gain for this channel pairing.
				Gain channelGain = channelGains.getGain( c2, c );
				Gain targetChannelGain = targetChannelGains.getGain( c2, c );
				Gain channelGainChangePerSample = Gain(0.5)*(targetChannelGain - channelGain) / numStreamSamples;
				streamGain = instanceInfo.instance.gain;
				
				// Skip this channel if the input gain is 0, it doesn't contribute to the output.
				if ( math::abs(channelGain) < math::epsilon<Gain>() &&
					math::abs(targetChannelGain) < math::epsilon<Gain>() )
					continue;
				
				Sample32f* output = outputStart;
				const Sample32f* input = streamBuffer->getChannel(c2);
				
				while ( output != outputEnd )
				{
					*output += streamGain*channelGain*(*input);
					input++;
					output++;
					streamGain += streamGainChangerPerSample;
					channelGain += channelGainChangePerSample;
				}
				
				// Update the channel I/O pair gain.
				channelGains.setGain( c2, c, channelGain );
			}
		}
		
		instanceInfo.instance.gain = streamGain;
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		SoundInputStream::readSamples() Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult SoundPlayer:: readSamples( SoundBuffer& outputBuffer, Size numSamples )
{
	return SoundFilter::read( outputBuffer, numSamples );
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
