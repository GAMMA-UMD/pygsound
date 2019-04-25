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

#include "omSoundPanner.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Panner:: NAME( "Panner" );
const UTF8String Panner:: MANUFACTURER( "Om Sound" );
const FilterVersion Panner:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Panner:: getName() const
{
	return NAME;
}




UTF8String Panner:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Panner:: getVersion() const
{
	return VERSION;
}




FilterCategory Panner:: getCategory() const
{
	return FilterCategory::IMAGING;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Panner:: processFrame( const SoundFrame& inputFrame,
										SoundFrame& outputFrame, Size numSamples )
{
	const SoundBuffer* inputBuffer;
	
	// If there is no input buffer or if it is NULL, return that an error occurred.
	if ( inputFrame.getBufferCount() == 0 || (inputBuffer = inputFrame.getBuffer(0)) == NULL )
		return SoundResult::ERROR;
	
	// If the input buffer doesn't have any channels, return that an error occurred.
	if ( inputBuffer->getChannelCount() == 0 )
		return SoundResult::ERROR;
	
	SoundBuffer* outputBuffer;
	
	// If there is no output buffer or if it is NULL, return that no samples were processed.
	if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
		return 0;
	
	// Make sure the output buffer is the right size.
	if ( outputBuffer->getSize() < numSamples )
		outputBuffer->setSize( numSamples );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************
	// Compute the mapping from the input layout to the output layout.
	
	const ChannelLayout& inputLayout = inputBuffer->getLayout();
	const ChannelLayout& outputLayout = outputBuffer->getLayout();
	
	outputLayout.panLayout( inputLayout, pan, targetChannelGains );
	
	// Reset parameter interpolation if this is the first processing frame or if the
	// input or output channel layout changed.
	if ( isFirstFrame() ||
		channelGains.getInputCount() != targetChannelGains.getInputCount() ||
		channelGains.getOutputCount() != targetChannelGains.getOutputCount() )
	{
		channelGains = targetChannelGains;
	}
	
	//*************************************************************************
	// Apply the channel mapping.
	
	// Zero the output buffer.
	outputBuffer->zero( 0, numSamples );
	
	// Mix each output channel.
	const Size numInputChannels = inputBuffer->getChannelCount();
	const Size numOutputChannels = outputBuffer->getChannelCount();
	
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		Sample32f* const outputStart = outputBuffer->getChannel(c);
		const Sample32f* const outputEnd = outputStart + numSamples;
		
		for ( Index i = 0; i < numInputChannels; i++ )
		{
			// Compute the target input gain for this channel pairing.
			Gain channelGain = channelGains.getGain( i, c );
			Gain targetChannelGain = targetChannelGains.getGain( i, c );
			Gain channelGainChangePerSample = Gain(0.5)*(targetChannelGain - channelGain) / numSamples;
			
			// Skip this channel if the input gain is 0, it doesn't contribute to the output.
			if ( math::abs(channelGain) < math::epsilon<Gain>() &&
				math::abs(targetChannelGain) < math::epsilon<Gain>() )
				continue;
			
			Sample32f* output = outputStart;
			const Sample32f* input = inputBuffer->getChannel(i);
			
			while ( output != outputEnd )
			{
				*output += channelGain*(*input);
				input++;
				output++;
				channelGain += channelGainChangePerSample;
			}
			
			// Update the channel pairing gain.
			channelGains.setGain( i, c, channelGain );
		}
	}
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
