/*
 * Project:     GSound
 * 
 * File:        gsound/gsSampledIR.cpp
 * Contents:    gsound::SampledIR class implementation
 * 
 * Author(s):   Carl Schissler
 * Website:     http://gamma.cs.unc.edu/GSOUND/
 * 
 * License:
 * 
 *     Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
 *     All rights reserved.
 *     
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for educational, research, and non-profit purposes, without
 *     fee, and without a written agreement is hereby granted, provided that the
 *     above copyright notice, this paragraph, and the following four paragraphs
 *     appear in all copies.
 *     
 *     Permission to incorporate this software into commercial products may be
 *     obtained by contacting the University of North Carolina at Chapel Hill.
 *     
 *     This software program and documentation are copyrighted by Carl Schissler and
 *     the University of North Carolina at Chapel Hill. The software program and
 *     documentation are supplied "as is", without any accompanying services from
 *     the University of North Carolina at Chapel Hill or the authors. The University
 *     of North Carolina at Chapel Hill and the authors do not warrant that the
 *     operation of the program will be uninterrupted or error-free. The end-user
 *     understands that the program was developed for research purposes and is advised
 *     not to rely exclusively on the program for any reason.
 *     
 *     IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR ITS
 *     EMPLOYEES OR THE AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE
 *     UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *     THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 *     DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 *     STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS
 *     ON AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND
 *     THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 *     ENHANCEMENTS, OR MODIFICATIONS.
 */


#include "gsSampledIR.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const SampleRate SampledIR:: DEFAULT_SAMPLE_RATE = 44100.0;


//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




SampledIR:: SampledIR()
	:	intensity( NULL ),
		directions( NULL ),
		sourceDirections( NULL ),
		sourceDirectionsEnabled( false ),
		capacity( 0 ),
		startOffset( math::max<Index>() ),
		numSamples( 0 ),
		sampleRate( DEFAULT_SAMPLE_RATE )
{
}




SampledIR:: SampledIR( SampleRate newSampleRate )
	:	intensity( NULL ),
		directions( NULL ),
		sourceDirections( NULL ),
		sourceDirectionsEnabled( false ),
		capacity( 0 ),
		startOffset( math::max<Index>() ),
		numSamples( 0 ),
		sampleRate( math::max( newSampleRate, SampleRate(0) ) )
{
}




SampledIR:: SampledIR( const SampledIR& other )
	:	intensity( NULL ),
		directions( NULL ),
		sourceDirections( NULL ),
		sourceDirectionsEnabled( other.sourceDirectionsEnabled ),
		capacity( other.capacity ),
		startOffset( other.startOffset ),
		numSamples( other.numSamples ),
		sampleRate( other.sampleRate )
{
	if ( capacity > 0 )
	{
		// Copy the intensity.
		intensity = util::allocateAligned<Float>( capacity*numFrequencyBands, SIMDFloat::getAlignment() );
		om::util::copy( intensity, other.intensity, numSamples*numFrequencyBands );
		
		// Copy the directions.
		directions = util::allocateAligned<Vector3f>( capacity, SIMDFloat::getAlignment() );
		om::util::copy( directions, other.directions, numSamples );
		
		if ( sourceDirectionsEnabled )
		{
			sourceDirections = util::allocateAligned<Vector3f>( capacity, SIMDFloat::getAlignment() );
			om::util::copy( sourceDirections, other.sourceDirections, numSamples );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		Destructor
//############
//##########################################################################################
//##########################################################################################




SampledIR:: ~SampledIR()
{
	if ( intensity )
		util::deallocateAligned( intensity );
	
	if ( directions )
		util::deallocateAligned( directions );
	
	if ( sourceDirections )
		util::deallocateAligned( sourceDirections );
}




//##########################################################################################
//##########################################################################################
//############
//############		Assignment Operator
//############
//##########################################################################################
//##########################################################################################




SampledIR& SampledIR:: operator = ( const SampledIR& other )
{
	if ( this != &other )
	{
		// Reallocate this IR if necessary.
		if ( other.numSamples > capacity )
			reallocate( other.numSamples );
		
		// Copy attributes.
		this->setSourceDirectionsEnabled( other.sourceDirectionsEnabled );
		startOffset = other.startOffset;
		numSamples = other.numSamples;
		sampleRate = other.sampleRate;
		
		// Copy the other IR.
		if ( numSamples > Size(0) )
		{
			om::util::copy( directions + startOffset, other.directions + startOffset, numSamples - startOffset );
			
			if ( sourceDirectionsEnabled )
				om::util::copy( sourceDirections + startOffset, other.sourceDirections + startOffset, numSamples - startOffset );
			
			om::util::copy( intensity + startOffset*numFrequencyBands,
							other.intensity + startOffset*numFrequencyBands,
							(numSamples - startOffset)*numFrequencyBands );
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Add Method
//############
//##########################################################################################
//##########################################################################################




Bool SampledIR:: addIR( const SampledIR& other )
{
	if ( sampleRate != other.sampleRate )
		return false;
	
	// Reallocate and zero the IR if necessary.
	if ( other.numSamples >= capacity )
		reallocate( other.numSamples);
	if ( other.numSamples > numSamples )
		zero( numSamples, other.numSamples - numSamples );
	
	// Update the IR length and start offset.
	startOffset = math::min( startOffset, other.startOffset );
	numSamples = math::max( numSamples, other.numSamples );
	
	// Add the other IR.
	math::add( (Real*)(directions + other.startOffset),
				(Real*)(other.directions + other.startOffset),
				3*(other.numSamples - other.startOffset) );
	
	if ( sourceDirectionsEnabled && other.sourceDirectionsEnabled )
	{
		math::add( (Real*)(sourceDirections + other.startOffset),
					(Real*)(other.sourceDirections + other.startOffset),
					3*(other.numSamples - other.startOffset) );
	}
	
	math::add( intensity + other.startOffset*numFrequencyBands,
				other.intensity + other.startOffset*numFrequencyBands,
				(other.numSamples - other.startOffset)*numFrequencyBands );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Trim Method
//############
//##########################################################################################
//##########################################################################################




Float SampledIR:: trim( const FrequencyBandResponse& threshold )
{
	if ( numSamples > 0 )
	{
		// Trim the IR separately for each band, use the longest length.
		const SIMDBands* const bandIntensities = (const SIMDBands*)intensity;
		const SIMDBands* bandIntensitiesEnd = bandIntensities + numSamples - 1;
		SIMDBands bandThreshold;
		
		for ( Index b = 0; b < numFrequencyBands; b++ )
			bandThreshold[b] = threshold[b];
		
		while ( bandIntensitiesEnd >= bandIntensities )
		{
			// If any band is over the threshold for that band, stop trimming.
			SIMDIntBands over = (*bandIntensitiesEnd) > bandThreshold;
			
			if ( over )
				break;
			
			bandIntensitiesEnd--;
		}
		
		numSamples = Size(bandIntensitiesEnd - bandIntensities) + 1;
	}
	
	return this->getLength();
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Reset Method
//############
//##########################################################################################
//##########################################################################################




void SampledIR:: reset()
{
	if ( intensity )
	{
		intensity = NULL;
		util::deallocateAligned( intensity );
	}
	
	if ( directions )
	{
		directions = NULL;
		util::deallocateAligned( directions );
	}
	
	if ( sourceDirections )
	{
		sourceDirections = NULL;
		util::deallocateAligned( sourceDirections );
	}
	
	startOffset = math::max<Index>();
	numSamples = 0;
	capacity = 0;
}




//##########################################################################################
//##########################################################################################
//############
//############		Source Direction Accessor Method
//############
//##########################################################################################
//##########################################################################################




void SampledIR:: setSourceDirectionsEnabled( Bool newSourceDirectionsEnabled )
{
	if ( newSourceDirectionsEnabled && !sourceDirectionsEnabled )
	{
		// Allocate a buffer of the current capacity for the source directions.
		sourceDirections = util::allocateAligned<Vector3f>( capacity, SIMDFloat::getAlignment() );
		
		// Zero the directions for the valid range of samples.
		om::util::zero( (Float*)(sourceDirections), numSamples*3 );
	}
	else if ( !newSourceDirectionsEnabled && sourceDirectionsEnabled )
	{
		// Deallocate the source direction array to save memory.
		util::deallocateAligned( sourceDirections );
		sourceDirections = NULL;
	}
	
	sourceDirectionsEnabled = newSourceDirectionsEnabled;
}




//##########################################################################################
//##########################################################################################
//############
//############		Total Intensity Computation Method
//############
//##########################################################################################
//##########################################################################################




FrequencyBandResponse SampledIR:: getTotalIntensity() const
{
	FrequencyBandResponse total( Float(0) );
	const FrequencyBandResponse* bandIntensities = (const FrequencyBandResponse*)intensity;
	const FrequencyBandResponse* const bandIntensitiesEnd = bandIntensities + numSamples;
	
	while ( bandIntensities != bandIntensitiesEnd )
	{
		total += *bandIntensities;
		bandIntensities++;
	}
	
	return total;
}




//##########################################################################################
//##########################################################################################
//############
//############		Size in Bytes Accessor Method
//############
//##########################################################################################
//##########################################################################################




Size SampledIR:: getSizeInBytes() const
{
	return sizeof(SampledIR) + sizeof(Float)*capacity*numFrequencyBands +
			sizeof(Vector3f)*capacity + (sourceDirectionsEnabled ? sizeof(Vector3f)*capacity : 0);
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Reallocation Method
//############
//##########################################################################################
//##########################################################################################




void SampledIR:: reallocate( Size minimumSize )
{
	// Pad the new size to be a multiple of the SIMD width.
	Size padding = minimumSize % SIMDFloat::getWidth();
	minimumSize += (SIMDFloat::getWidth() - padding);
	
	if ( minimumSize > capacity )
	{
		Vector3f* newDirections = util::allocateAligned<Vector3f>( minimumSize, SIMDFloat::getAlignment() );
		Float* newIntensity = util::allocateAligned<Float>( minimumSize*numFrequencyBands, SIMDFloat::getAlignment() );
		
		// Copy, then deallocate the old directions.
		if ( directions )
		{
			om::util::copy( newDirections, directions, numSamples );
			util::deallocateAligned( directions );
		}
		
		// Copy, then deallocate the old frequency bands.
		if ( intensity )
		{
			om::util::copy( newIntensity, intensity, numSamples*numFrequencyBands );
			util::deallocateAligned( intensity );
		}
		
		// Reallocate the source directions if they are enabled.
		if ( sourceDirectionsEnabled )
		{
			Vector3f* newSourceDirections = util::allocateAligned<Vector3f>( minimumSize, SIMDFloat::getAlignment() );
			
			if ( sourceDirections )
			{
				om::util::copy( newSourceDirections, sourceDirections, numSamples );
				util::deallocateAligned( sourceDirections );
			}
			
			sourceDirections = newSourceDirections;
		}
		
		directions = newDirections;
		intensity = newIntensity;
		capacity = minimumSize;
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Zero Method
//############
//##########################################################################################
//##########################################################################################




void SampledIR:: zero( Index startIndex, Size number )
{
	// Zero the directions.
	om::util::zeroPOD( directions + startIndex, number );
	
	if ( sourceDirectionsEnabled )
		om::util::zeroPOD( sourceDirections + startIndex, number );
	
	// Zero the intensity for all bands.
	om::util::zeroPOD( intensity + startIndex*numFrequencyBands, number*numFrequencyBands );
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
