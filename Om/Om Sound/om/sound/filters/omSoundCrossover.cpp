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

#include "omSoundCrossover.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String Crossover:: NAME( "Crossover" );
const UTF8String Crossover:: MANUFACTURER( "Om Sound" );
const FilterVersion Crossover:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Crossover:: Crossover()
	:	SoundFilter( 1, 1 ),
		multibandInput( false )
{
	// Add the main frequency band.
	frequencyBands.add( FrequencyBand() );
}




Crossover:: Crossover( const Crossover& other )
	:	SoundFilter( 1, other.getOutputCount() ),
		points( other.points.getSize() ),
		frequencyBands( other.frequencyBands.getSize() ),
		multibandInput( other.multibandInput )
{
	// Copy the crossover filters.
	for ( Index i = 0; i < other.points.getSize(); i++ )
		points.add( other.points[i] );
	
	// Update the frequency bands for the new configuration.
	updateFrequencyBands();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################





Crossover:: ~Crossover()
{
	// Deallocate all of the crossover filters.
	for ( Index i = 0; i < frequencyBands.getSize(); i++ )
	{
		FrequencyBand& band = frequencyBands[i];
		
		for ( Index i = 0; i < band.filters.getSize(); i++ )
			om::util::destruct( band.filters[i] );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




Crossover& Crossover:: operator = ( const Crossover& other )
{
	if ( this != &other )
	{
		lockMutex();
		
		multibandInput = other.multibandInput;
		
		// Clear this crossover's filters.
		points.clear();
		
		// Copy the new crossover filters.
		for ( Index i = 0; i < other.points.getSize(); i++ )
			points.add( other.points[i] );
		
		// Update the frequency bands for the new configuration.
		updateFrequencyBands();
		
		unlockMutex();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Crossover Filter Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Crossover:: addFilter( Float newFrequency, Size newFilterOrder )
{
	// Make sure that the new filter frequency is valid. If not, return failure.
	if ( newFrequency <= Float(0) )
		return false;
	
	// Sanitize the new filter order.
	newFilterOrder = math::clamp( newFilterOrder, Size(2), Size(8) );
	
	//*************************************************************************
	
	lockMutex();
	
	// Increase the number of outputs this filter has by 1.
	this->setOutputCount( this->getOutputCount() + 1 );
	const Size numCrossoverPoints = points.getSize();
	
	// If this is the first filter for this crossover, add it to the end of the list of crossover points.
	if ( numCrossoverPoints == 0 )
		points.add( CrossoverPoint( newFrequency, newFilterOrder ) );
	else
	{
		// Otherwise, find the location within the list of crossover points for the new filter.
		Index i = 0;
		
		for ( ; i < numCrossoverPoints; i++ )
		{
			// If the new frequency is less than the current frequency, insert it before the current filter.
			if ( newFrequency < points[i].frequency )
			{
				// Insert the new filter.
				points.insert( i, CrossoverPoint( newFrequency, newFilterOrder ) );
				break;
			}
		}
		
		// Add the filter to the end of the list if it wasn't able to be added.
		if ( i == numCrossoverPoints )
			points.add( CrossoverPoint( newFrequency, newFilterOrder ) );
	}
	
	// Update the frequency band filters based on the new configuration.
	updateFrequencyBands();
	
	unlockMutex();
	
	return true;
}




void Crossover:: removeFilter( Index filterIndex )
{
	if ( filterIndex >= points.getSize() )
		return;
	
	lockMutex();
	
	// Decrease the number of outputs this filter has by 1.
	this->setOutputCount( this->getOutputCount() - 1 );
	
	// Remove the old filter.
	points.removeAtIndex( filterIndex );
	
	// Update the frequency band filters based on the new configuration.
	updateFrequencyBands();
	
	unlockMutex();
}




void Crossover:: clearFilters()
{
	lockMutex();
	
	// Deallocate all of the crossover filters.
	for ( Index i = 0; i < frequencyBands.getSize(); i++ )
	{
		FrequencyBand& band = frequencyBands[i];
		
		for ( Index i = 0; i < band.filters.getSize(); i++ )
			om::util::destruct( band.filters[i] );
	}
	
	// Clear the frequency bands and add a new one.
	points.clear();
	frequencyBands.clear();
	frequencyBands.add( FrequencyBand() );
	
	unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Crossover Filter Frequency Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Crossover:: setFilterFrequency( Index filterIndex, Float newFrequency )
{
	if ( filterIndex >= points.getSize() || newFrequency <= Float(0) )
		return false;
	
	lockMutex();
	
	// Set the new frequency.
	points[filterIndex].frequency = newFrequency;
	
	// Update the filters for the current configuration.
	updateFrequencyBands();
	
	unlockMutex();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Crossover Filter Order Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Crossover:: setFilterOrder( Index filterIndex, Size newOrder )
{
	if ( filterIndex >= points.getSize() )
		return false;
	
	// Sanitize the new filter order.
	newOrder = math::clamp( newOrder, Size(2), Size(8) );
	
	lockMutex();
	
	// Set the new order.
	points[filterIndex].order = newOrder;
	
	// Update the filters for the current configuration.
	updateFrequencyBands();
	
	unlockMutex();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Input Type Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Crossover:: setIsMultiInput( Bool newIsMultiInput )
{
	lockMutex();
	
	multibandInput = newIsMultiInput;
	
	if ( multibandInput )
		this->setInputCount( this->getOutputCount() );
	else
		this->setInputCount( 1 );
	
	unlockMutex();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Input and Output Name Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Crossover:: getOutputName( Index outputIndex ) const
{
	if ( outputIndex < points.getSize() + 1 )
		return UTF8String("Output ") + outputIndex;
	else
		return UTF8String();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String Crossover:: getName() const
{
	return NAME;
}




UTF8String Crossover:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion Crossover:: getVersion() const
{
	return VERSION;
}




FilterCategory Crossover:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Method
//############		
//##########################################################################################
//##########################################################################################




void Crossover:: resetStream()
{
	// Reset the crossover filters.
	const Size numFrequencyBands = points.getSize() + 1;
	
	for ( Index i = 0; i < numFrequencyBands; i++ )
	{
		FrequencyBand& band = frequencyBands[i];
		const Size numFilters = band.filters.getSize();
		
		for ( Index j = 0; j < numFilters; j++ )
			band.filters[j]->reset();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult Crossover:: processFrame( const SoundFrame& inputFrame,
											SoundFrame& outputFrame, Size numSamples )
{
	const SoundBuffer* inputBuffer;
	
	// If there is no input buffer or if it is NULL, return that an error occurred.
	if ( inputFrame.getBufferCount() == 0 || (inputBuffer = inputFrame.getBuffer(0)) == NULL )
		return SoundResult::ERROR;
	
	const SampleRate inputSampleRate = inputBuffer->getSampleRate();
	
	// If the input sample rate is zero, return that an error occurred.
	if ( inputSampleRate <= SampleRate(0) )
		return SoundResult::ERROR;
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************
	// Check the number of crossover outputs.
	
	// Determine how many frequency bands the crossover should process output for.
	const Size numCrossoverPoints = points.getSize();
	const Size numOutputsToProcess = math::min( numCrossoverPoints + 1, outputFrame.getBufferCount() );
	
	// If there are no crossover filters, pass through the input to the output.
	if ( numOutputsToProcess <= Size(1) )
	{
		SoundBuffer* outputBuffer;
		
		for ( Index i = 0; i < numOutputsToProcess; i++ )
		{
			const SoundBuffer* input = inputBuffer;
			
			if ( multibandInput )
			{
				// Skip this band if the output buffer is NULL.
				if ( (input = inputFrame.getBuffer(i)) == NULL )
				{
					if ( (outputBuffer = outputFrame.getBuffer(i)) != NULL )
						outputBuffer->zero( 0, numSamples );
					
					continue;
				}
			}
			
			// Make sure that the output buffer exists.
			if ( (outputBuffer = outputFrame.getBuffer(i)) != NULL )
			{
				// Copy the input straight to the output.
				input->copyFormatTo( *outputBuffer, numSamples );
				input->copyTo( *outputBuffer, numSamples );
			}
		}
		
		return numSamples;
	}
	
	//*************************************************************************
	// Process the output for each frequency band.
	
	for ( Index i = 0; i < numOutputsToProcess; i++ )
	{
		// Get the output buffer for this frequency band.
		SoundBuffer* outputBuffer;
		
		// Skip this band if the output buffer is NULL.
		if ( (outputBuffer = outputFrame.getBuffer(i)) == NULL )
			continue;
		
		const SoundBuffer* input = inputBuffer;
		
		if ( multibandInput )
		{
			// Skip this band if the output buffer is NULL.
			if ( (input = inputFrame.getBuffer(i)) == NULL )
			{
				outputBuffer->zero( 0, numSamples );
				continue;
			}
		}
		
		// Get the band for this output.
		FrequencyBand& band = frequencyBands[i];
		
		// Process the first filter, placing the output into the output buffer.
		band.filters[0]->process( *input, *outputBuffer, numSamples );
		
		// Determine the number of times the filters inverted the signal so that we can reverse it later.
		Index numInverts = 0;
		
		// Invert the polarity if necessary.
		if ( filterNeedsInversion( *band.filters[0] ) )
			numInverts++;
		
		// Process each remaining filter for this output in series.
		for ( Index j = 1; j < numCrossoverPoints; j++ )
		{
			band.filters[j]->process( *outputBuffer, *outputBuffer, numSamples );
			
			// Invert the polarity if necessary.
			if ( filterNeedsInversion( *band.filters[j] ) )
				numInverts++;
		}
		
		// If the number of inversions is odd, we need to reverse the polarity for this band's output.
		if ( numInverts % 2 == 1 )
			outputBuffer->invertPolarity();
	}
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Frequency Band Update Method
//############		
//##########################################################################################
//##########################################################################################




void Crossover:: updateFrequencyBands()
{
	const Size numCrossoverPoints = points.getSize();
	const Size numFrequencyBands = numCrossoverPoints + 1;
	
	// If there need to be more frequency bands, add new ones.
	while ( frequencyBands.getSize() < numFrequencyBands )
		frequencyBands.add( FrequencyBand() );
	
	// Update the filters for each frequency band.
	for ( Index i = 0; i < numFrequencyBands; i++ )
	{
		FrequencyBand& band = frequencyBands[i];
		
		// Add the filters for this frequency band.
		for ( Index j = 0; j < numCrossoverPoints; j++ )
		{
			CrossoverPoint& point = points[j];
			
			// Compute the new direction for the filter.
			CutoffFilter::Direction newDirection = (j >= i) ? CutoffFilter::LOW_PASS : CutoffFilter::HIGH_PASS;
			
			if ( j >= band.filters.getSize() )
			{
				// We need to add a new filter for this band.
				CutoffFilter* newFilter = om::util::construct<CutoffFilter>( CutoffFilter::LINKWITZ_RILEY, newDirection,
																			point.order, point.frequency );
				
				// Make sure the filter is not synchronized.
				newFilter->setIsSynchronized( false );
				
				// Add the new filter to the list of filters.
				band.filters.add( newFilter );
			}
			else
			{
				// We can reuse the existing filter.
				CutoffFilter* filter = band.filters[j];
				
				// Update the frequency and order for the filter.
				if ( filter->getFrequency() != point.frequency )
					filter->setFrequency( point.frequency );
				
				if ( filter->getOrder() != point.order )
					filter->setOrder( point.order );
				
				// Update the direction for the filter.
				if ( filter->getDirection() != newDirection )
					filter->setDirection( newDirection );
			}
		}
	}
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
