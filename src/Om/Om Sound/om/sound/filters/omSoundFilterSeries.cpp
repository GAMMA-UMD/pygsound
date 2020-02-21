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

#include "omSoundFilterSeries.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String FilterSeries:: NAME( "Filter Series" );
const UTF8String FilterSeries:: MANUFACTURER( "Om Sound" );
const FilterVersion FilterSeries:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




FilterSeries:: FilterSeries()
	:	SoundFilter( 0, 0 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundFilter* FilterSeries:: getFilter( Index filterIndex )
{
	lockMutex();
	
	if ( filterIndex < filters.getSize() )
	{
		SoundFilter* filter = filters[filterIndex];
		unlockMutex();
		
		return filter;
	}
	
	unlockMutex();
	return NULL;
}




Bool FilterSeries:: setFilter( Index filterIndex, SoundFilter* newFilter )
{
	if ( newFilter == NULL )
		return false;
	
	lockMutex();
	
	if ( filterIndex < filters.getSize() )
	{
		// Update the output count if necessary.
		if ( filterIndex == filters.getSize() - 1 )
			this->setOutputCount( newFilter->getOutputCount() );
		
		filters[filterIndex] = newFilter;
		unlockMutex();
		return true;
	}
	
	unlockMutex();
	return false;
}




Bool FilterSeries:: addFilter( SoundFilter* newFilter )
{
	if ( newFilter == NULL )
		return false;
	
	lockMutex();
	
	// Update the number input or outputs that this filter series has if necessary.
	if ( filters.getSize() == 0 )
		this->setInputCount( newFilter->getInputCount() );
	else
		this->setOutputCount( newFilter->getOutputCount() );
	
	filters.add( newFilter );
	
	unlockMutex();
	
	return true;
}




Bool FilterSeries:: insertFilter( Index filterIndex, SoundFilter* newFilter )
{
	if ( newFilter == NULL )
		return false;
	
	lockMutex();
	
	// Update the input or output count if necessary.
	if ( filterIndex == 0 )
		this->setInputCount( newFilter->getInputCount() );
	else if ( filterIndex >= filters.getSize() )
		this->setOutputCount( newFilter->getOutputCount() );
	
	// Insert the new filter.
	filters.insert( filterIndex, newFilter );
	
	unlockMutex();
	
	return true;
}




Bool FilterSeries:: removeFilter( Index filterIndex )
{
	lockMutex();
	
	if ( filterIndex < filters.getSize() )
	{
		filters.removeAtIndex( filterIndex );
		
		// Update the input or output count if necessary.
		if ( filterIndex == 0 )
		{
			if ( filters.getSize() > 0 )
				this->setInputCount( filters.getFirst()->getInputCount() );
			else
				this->setInputCount( 0 );
		}
		else if ( filterIndex == filters.getSize() )
		{
			if ( filters.getSize() > 0 )
				this->setOutputCount( filters.getLast()->getOutputCount() );
			else
				this->setOutputCount( 0 );
		}
		
		unlockMutex();
		return true;
	}
	
	unlockMutex();
	
	return false;
}




void FilterSeries:: clearFilters()
{
	lockMutex();
	filters.clear();
	
	// Reset the number of inputs and outputs.
	this->setInputCount( 0 );
	this->setOutputCount( 0 );
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Input and Output Name Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String FilterSeries:: getInputName( Index inputIndex ) const
{
	lockMutex();
	
	if ( filters.getSize() > 0 )
	{
		UTF8String name = filters[0]->getInputName( inputIndex );
		unlockMutex();
		
		return name;
	}
	else
	{
		unlockMutex();
		return UTF8String();
	}
}




UTF8String FilterSeries:: getOutputName( Index outputIndex ) const
{
	lockMutex();
	
	if ( filters.getSize() > 0 )
	{
		UTF8String name = filters.getLast()->getOutputName( outputIndex );
		unlockMutex();
		
		return name;
	}
	else
	{
		unlockMutex();
		return UTF8String();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String FilterSeries:: getName() const
{
	return NAME;
}




UTF8String FilterSeries:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion FilterSeries:: getVersion() const
{
	return VERSION;
}




FilterCategory FilterSeries:: getCategory() const
{
	return FilterCategory::ROUTING;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Method
//############		
//##########################################################################################
//##########################################################################################




void FilterSeries:: resetStream()
{
	// Reset all of the filters in the series.
	for ( Index i = 0; i < filters.getSize(); i++ )
		filters[i]->reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult FilterSeries:: processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples )
{
	const Size numFilters = filters.getSize();
	
	if ( numFilters == 0 )
	{
		// Determine how many buffers the input and output frames share.
		Size numBuffersToCopy = math::min( inputFrame.getBufferCount(), outputFrame.getBufferCount() );
		
		// Copy the contents of each input buffer to the output buffer.
		for ( Index i = 0; i < numBuffersToCopy; i++ )
		{
			const SoundBuffer* inputBuffer = inputFrame.getBuffer(i);
			SoundBuffer* outputBuffer = outputFrame.getBuffer(i);
			
			// Skip this pair of buffers if the output buffer is NULL.
			if ( outputBuffer == NULL )
				continue;
			
			// If the input buffer is NULL, write zeros to the output buffer.
			if ( inputBuffer == NULL )
				outputBuffer->zero( 0, numSamples );
			else
			{
				// Copy the input buffer's format to this output buffer.
				inputBuffer->copyFormatTo( *outputBuffer, numSamples );
				
				// Copy the input buffer to this output buffer.
				inputBuffer->copyTo( *outputBuffer, numSamples );
			}
		}
		
		return numSamples;
	}
	else if ( numFilters == 1 )
	{
		// There is only one filter, process it by itself.
		return filters[0]->process( inputFrame, outputFrame, numSamples );
	}
	else
	{
		//***************************************************************************
		// Prepare the first temporary buffer.
		
		{
			// Determine the number of outputs that the first filter has.
			Size numFilterOutputs = filters[0]->getOutputCount();
			
			for ( Index i = 0; i < numFilterOutputs; i++ )
			{
				const SoundBuffer* inputBuffer;
				const SoundBuffer* outputBuffer;
				
				// Request a buffer the same format as the input buffer or output buffer for the first frame.
				if ( i < inputFrame.getBufferCount() && (inputBuffer = inputFrame.getBuffer(i)) != NULL )
				{
					tempFrame1.addBuffer( getTempBuffer( inputBuffer->getChannelCount(),
														numSamples, inputBuffer->getSampleRate() ) );
				}
				else if ( i < outputFrame.getBufferCount() && (outputBuffer = outputFrame.getBuffer(i)) != NULL )
				{
					tempFrame1.addBuffer( getTempBuffer( outputBuffer->getChannelCount(),
														numSamples, outputBuffer->getSampleRate() ) );
				}
				else
				{
					// If there's no corresponding input or output buffer, just create a 1-channel buffer at 44.1kHz.
					// This information will likely be changed by the first filter, so it doesn't matter what
					// we pick.
					tempFrame1.addBuffer( getTempBuffer( 1, numSamples, 44100 ) );
				}
			}
		}
		
		//***************************************************************************
		// Process the first filter.
		
		Size currentNumSamples = filters[0]->process( inputFrame, tempFrame1, numSamples );
		
		//***************************************************************************
		// Process each successive filter, except for the last one.
		
		Index lastFilterIndex = numFilters - 1;
		
		for ( Index i = 1; i < lastFilterIndex; i++ )
		{
			SoundFilter& filter = *filters[i];
			Size numFilterOutputs = filter.getOutputCount();
			const SoundFrame* sourceFrame;
			SoundFrame* destinationFrame;
			
			// Figure out which temporary processing frames to use.
			if ( i % 2 == 0 )
			{
				// Filter index is even, processing is from tempFrame2 to tempFrame1.
				sourceFrame = &tempFrame2;
				destinationFrame = &tempFrame1;
			}
			else
			{
				// Filter index is odd, processing is from tempFrame1 to tempFrame2.
				sourceFrame = &tempFrame1;
				destinationFrame = &tempFrame2;
			}
			
			// Make sure that there are enough buffers in the output frame to hold the filter's outputs.
			while ( destinationFrame->getBufferCount() < numFilterOutputs )
			{
				// Use the input buffer's format if possible.
				Index newBufferIndex = destinationFrame->getBufferCount();
				
				if ( newBufferIndex < sourceFrame->getBufferCount() )
				{
					const SoundBuffer* inputBuffer = sourceFrame->getBuffer(newBufferIndex);
					
					destinationFrame->addBuffer( getTempBuffer( inputBuffer->getChannelCount(),
																currentNumSamples, inputBuffer->getSampleRate() ) );
				}
				else
					destinationFrame->addBuffer( getTempBuffer( 1, currentNumSamples, 44100 ) );
			}
			
			// Process the filter.
			currentNumSamples = filter.process( *sourceFrame, *destinationFrame, currentNumSamples );
		}
		
		//***************************************************************************
		// Process the last filter.
		
		if ( numFilters % 2 == 0 )
		{
			// Even number of filters, so last filter processes from tempFrame1 to outputFrame.
			currentNumSamples = filters[lastFilterIndex]->process( tempFrame1, outputFrame, currentNumSamples );
		}
		else
		{
			// Odd number of filters, so last filter processes from tempFrame2 to outputFrame.
			currentNumSamples = filters[lastFilterIndex]->process( tempFrame2, outputFrame, currentNumSamples );
		}
		
		// Release all of the shared buffers which were used.
		tempBuffers.clear();
		
		// Clear the buffers from the temporary frames.
		tempFrame1.clearBuffers();
		tempFrame2.clearBuffers();
		
		return currentNumSamples;
	}
	
	return SoundResult::ERROR;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
