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

#include "omSoundBuffer.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const om::resources::ResourceType SoundBuffer:: RESOURCE_TYPE("SoundBuffer");


//##########################################################################################
//##########################################################################################
//############		
//############		Sample Allocation Helper Methods
//############		
//##########################################################################################
//##########################################################################################




/// Allocate a sample buffer using the current alignment of the specified size.
OM_FORCE_INLINE static Sample32f* allocateSamples( Size numSamples )
{
	return om::util::allocateAligned<Sample32f>( numSamples, math::SIMDFloat4::getAlignment() );
}




/// Deallocate a sample buffer with the current alignment.
OM_FORCE_INLINE static void deallocateSamples( Sample32f* samples )
{
	om::util::deallocateAligned( samples );
}




OM_FORCE_INLINE static Size roundCapacity( Size capacity )
{
	const Size align = 4;
	Size remainder = capacity % align;
	
	if ( remainder == 0 )
		return capacity;
	else
		return capacity + align - remainder;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundBuffer:: SoundBuffer( Size newNumChannels )
	:	sampleRate( 44100 ),
		layout()
{
	initializeChannels( newNumChannels, 0 );
}




SoundBuffer:: SoundBuffer( Size newNumChannels, Size newNumSamples, SampleRate newSampleRate )
	:	sampleRate( newSampleRate ),
		layout( newNumChannels )
{
	initializeChannels( newNumChannels, newNumSamples );
}




SoundBuffer:: SoundBuffer( ChannelLayoutType newLayoutType )
	:	sampleRate( 44100 ),
		layout( newLayoutType )
{
	initializeChannels( layout.getChannelCount(), 0 );
}




SoundBuffer:: SoundBuffer( ChannelLayoutType newLayoutType, Size newNumSamples, SampleRate newSampleRate )
	:	sampleRate( 44100 ),
		layout( newLayoutType )
{
	initializeChannels( layout.getChannelCount(), newNumSamples );
}




SoundBuffer:: SoundBuffer( const ChannelLayout& newLayout )
	:	sampleRate( 44100 ),
		layout( newLayout )
{
	initializeChannels( layout.getChannelCount(), 0 );
}




SoundBuffer:: SoundBuffer( const ChannelLayout& newLayout, Size newNumSamples, SampleRate newSampleRate )
	:	sampleRate( 44100 ),
		layout( newLayout )
{
	initializeChannels( layout.getChannelCount(), newNumSamples );
}




SoundBuffer:: SoundBuffer( const SoundBuffer& other )
	:	sampleRate( other.sampleRate ),
		layout( other.layout )
{
	initializeCopyBuffer( other, other.numSamples );
}




SoundBuffer:: SoundBuffer( const SoundBuffer& other, Size newNumSamples )
	:	sampleRate( other.sampleRate ),
		layout( other.layout )
{
	initializeCopyBuffer( other, newNumSamples );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundBuffer:: ~SoundBuffer()
{
	if ( samples != NULL )
		deallocateSamples( samples );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundBuffer& SoundBuffer:: operator = ( const SoundBuffer& other )
{
	if ( this != &other )
	{
		// Copy the layout and sample rate of the buffer.
		sampleRate = other.sampleRate;
		layout = other.layout;
		
		// Make sure that this buffer has the right size and number of channels.
		if ( numSamples != other.numSamples )
			this->setSize( other.numSamples );
		
		if ( numChannels != other.numChannels )
			this->setChannelCount( other.numChannels );
		
		// Copy the data from the other buffer.
		other.copyTo( *this, other.numSamples );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Layout Modification Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: setLayout( const ChannelLayout& newLayout )
{
	// Make sure the buffer has the correct number of channels.
	if ( numChannels != newLayout.getChannelCount() )
		this->setChannelCount( newLayout.getChannelCount() );
	
	layout = newLayout;
}




void SoundBuffer:: setLayout( ChannelLayoutType newLayoutType )
{
	// Get a new layout for the predefined layout type.
	layout = ChannelLayout( newLayoutType );
	
	// Make sure the buffer has the correct number of channels.
	if ( numChannels != layout.getChannelCount() )
		this->setChannelCount( layout.getChannelCount() );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Count Modification Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: setChannelCount( Size newNumChannels )
{
	if ( newNumChannels > channelCapacity )
	{
		Size newTotalCapacity = sampleCapacity*newNumChannels;
		
		if ( newTotalCapacity > 0 )
		{
			if ( samples == NULL )
				samples = allocateSamples( newTotalCapacity );
			else
			{
				Sample32f* newSamples = allocateSamples( newTotalCapacity );
				
				// Copy the previous contents.
				for ( Index c = 0; c < numChannels; c++ )
					om::util::copy( newSamples + c*sampleCapacity, samples + c*sampleCapacity, numSamples );
				
				deallocateSamples( samples );
				samples = newSamples;
			}
		}
		
		channelCapacity = newNumChannels;
		totalCapacity = newTotalCapacity;
	}
	
	numChannels = newNumChannels;
	
	// Make sure that the layout has the correct number of channels.
	layout.setChannelCount( numChannels );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Size Modification Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: setSize( Size newNumSamples )
{
	// Reallocate the internal sample buffer if necessary.
	if ( newNumSamples > sampleCapacity )
	{
		Size newSampleCapacity = roundCapacity( newNumSamples );
		Size newTotalCapacity = newSampleCapacity*channelCapacity;
		
		if ( newTotalCapacity > 0 )
		{
			if ( samples == NULL )
				samples = allocateSamples( newTotalCapacity );
			else
			{
				Sample32f* newSamples = allocateSamples( newTotalCapacity );
				
				// Copy the previous contents.
				for ( Index c = 0; c < numChannels; c++ )
					om::util::copy( newSamples + c*newSampleCapacity, samples + c*sampleCapacity, numSamples );
				
				deallocateSamples( samples );
				samples = newSamples;
			}
		}
		
		sampleCapacity = newSampleCapacity;
		totalCapacity = newTotalCapacity;
	}
	
	numSamples = newNumSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Zero Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: zero( Index startIndex, Size numSamplesToZero )
{
	// Don't bother zeroing the buffer if it has no samples or no channels.
	if ( startIndex >= numSamples || numChannels == 0 )
		return;
	
	// Clamp the ending index to the end of the buffer.
	Index endIndex = math::min( startIndex + numSamplesToZero, numSamples );
	numSamplesToZero = endIndex - startIndex;
	
	// Zero each channel.
	for ( Index c = 0; c < numChannels; c++ )
		om::util::zero( (Float32*)(this->getChannel(c) + startIndex), numSamplesToZero );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Copy Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundBuffer:: copyToInterleavedBuffer( Sample32f* output, Size numSamplesToCopy ) const
{
	// Don't copy anything if the output buffer is NULL, or this buffer has no samples or no channels.
	if ( output == NULL || numSamples == 0 || numChannels == 0 )
		return 0;
	
	// Make sure that we aren't copying more samples than are available.
	numSamplesToCopy = math::min( numSamplesToCopy, numSamples );
	
	// Copy each channel in an interleaved fashion.
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Sample32f* source = this->getChannel(c);
		const Sample32f* const sourceEnd = source + numSamplesToCopy;
		Sample32f* destination = output;
		
		while ( source != sourceEnd )
		{
			*destination = *source;
			source++;
			destination += numChannels;
		}
		
		output++;
	}
	
	return numSamplesToCopy;
}




Size SoundBuffer:: copyTo( SoundBuffer& other, Size numSamplesToCopy ) const
{
	// Don't copy anything if the output buffer is the same, or this buffer has no samples or no channels.
	if ( this == &other || numSamples == 0 || numChannels == 0 )
		return 0;
	
	// Make sure that we aren't copying more samples than are available.
	numSamplesToCopy = math::min( numSamplesToCopy, numSamples );
	
	// Enlarge the other buffer if necessary.
	if ( other.getSize() < numSamplesToCopy )
		other.setSize( numSamplesToCopy );
	
	if ( other.getChannelCount() < numChannels )
		other.setChannelCount( numChannels );
	
	// Copy each channel.
	for ( Index c = 0; c < numChannels; c++ )
		om::util::copyPOD( other.getChannel(c), this->getChannel(c), numSamplesToCopy );
	
	return numSamplesToCopy;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Mixing Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundBuffer:: mixTo( SoundBuffer& other, Size numSamplesToMix ) const
{
	// Don't mix anything if the output buffer is the same, or this buffer has no samples or no channels.
	if ( this == &other || numSamples == 0 || numChannels == 0 )
		return 0;
	
	// Make sure that we aren't mixing more samples than are available.
	Size numMixSamples = math::min( numSamplesToMix, math::min(numSamples, other.numSamples) );
	const Size numMixChannels = math::min( numChannels, other.numChannels );
	
	// Mix each channel.
	for ( Index c = 0; c < numMixChannels; c++ )
		om::math::add( (Float32*)other.getChannel(c), (const Float32*)this->getChannel(c), numMixSamples );
	
	return numMixSamples;
}




Size SoundBuffer:: mixTo( SoundBuffer& other, Size numSamplesToMix, Gain gain ) const
{
	// Don't mix anything if the output buffer is the same, or this buffer has no samples or no channels.
	if ( this == &other || numSamples == 0 || numChannels == 0 )
		return 0;
	
	// Make sure that we aren't mixing more samples than are available.
	Size numMixSamples = math::min( numSamplesToMix, math::min(numSamples, other.numSamples) );
	const Size numMixChannels = math::min( numChannels, other.numChannels );
	
	// Mix each channel.
	for ( Index c = 0; c < numMixChannels; c++ )
		om::math::multiplyAdd( (Float32*)other.getChannel(c), (const Float32*)this->getChannel(c), gain, numMixSamples );
	
	return numMixSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Gain Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: applyGain( Gain gain, Size numSamplesToGain )
{
	// Don't do anything if this buffer has no samples or no channels.
	if ( numSamples == 0 || numChannels == 0 )
		return;
	
	// Make sure that we aren't applying gain to more samples than are available.
	Size numGainSamples = math::min( numSamplesToGain, numSamples );
	
	for ( Index c = 0; c < numChannels; c++ )
		om::math::multiply( (Float32*)this->getChannel(c), gain, numGainSamples );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Polarity Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: invertPolarity( Size numSamplesToInvert )
{
	// Don't do anything if this buffer has no samples or no channels.
	if ( numSamples == 0 || numChannels == 0 )
		return;
	
	// Make sure that we aren't applying gain to more samples than are available.
	numSamplesToInvert = math::min( numSamplesToInvert, numSamples );
	
	for ( Index c = 0; c < numChannels; c++ )
		om::math::multiply( (Float32*)this->getChannel(c), Float32(-1), numSamplesToInvert );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Format Copy Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: copyFormatTo( SoundBuffer& other, Size minimumNumberOfSamples ) const
{
	if ( other.getSize() < minimumNumberOfSamples )
		other.setSize( minimumNumberOfSamples );
	
	if ( other.getChannelCount() != numChannels )
		other.setChannelCount( numChannels );
	
	other.setSampleRate( sampleRate );
	other.layout = layout;
}




void SoundBuffer:: setFormat( Size newNumChannels, Size newNumSamples )
{
	if ( numSamples != newNumSamples )
		this->setSize( newNumSamples );
	
	if ( numChannels != newNumChannels )
		this->setChannelCount( newNumChannels );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Initialization Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundBuffer:: initializeChannels( Size newNumChannels, Size newNumSamples )
{
	// Set properties of the buffer.
	channelCapacity = numChannels = newNumChannels;
	numSamples = newNumSamples;
	sampleCapacity = roundCapacity( newNumSamples );
	totalCapacity = sampleCapacity*channelCapacity;
	
	if ( totalCapacity > 0 )
		samples = allocateSamples( totalCapacity );
	else
		samples = NULL;
}




void SoundBuffer:: initializeCopyBuffer( const SoundBuffer& buffer, Size newNumSamples )
{
	// Compute the total number of samples that can be copied from the other buffer.
	const Size finalNumChannels = buffer.getChannelCount();
	const Size finalNumSamples = math::min( buffer.getSize(), newNumSamples );
	
	// Initialize the channel array for the buffer's channels.
	this->initializeChannels( finalNumChannels, finalNumSamples );
	
	// Copy the data from the other buffer.
	for ( Index c = 0; c < numChannels; c++ )
		om::util::copy( (Float32*)this->getChannel(c), (const Float32*)buffer.getChannel(c), finalNumSamples );
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
