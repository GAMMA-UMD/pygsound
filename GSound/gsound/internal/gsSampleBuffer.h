/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSampleBuffer.h
 * Contents:    gsound::internal::SampleBuffer class declaration
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


#ifndef INCLUDE_GSOUND_SAMPLE_BUFFER_H
#define INCLUDE_GSOUND_SAMPLE_BUFFER_H


#include "gsInternalConfig.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




template < typename SampleType >
class SampleBuffer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sample buffer with 0 channels and 0 samples.
			GSOUND_INLINE SampleBuffer()
				:	samples( NULL ),
					numChannels( 0 ),
					numSamples( 0 ),
					totalNumSamples( 0 )
			{
			}
			
			
			/// Create a new sample buffer with the specified number of channels and samples.
			/**
			  * The sample buffer is not allocated.
			  */
			GSOUND_INLINE SampleBuffer( Size newNumChannels, Size newNumSamples )
				:	samples( NULL ),
					numChannels( newNumChannels ),
					numSamples( newNumSamples ),
					totalNumSamples( newNumChannels*newNumSamples )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sample buffer, releasing all internal data.
			GSOUND_INLINE ~SampleBuffer()
			{
				if ( samples )
					util::deallocateAligned( samples );
			}
			
			
		//********************************************************************************
		//******	Format Accessor Methods
			
			
			/// Return the number of channels in this sample buffer.
			GSOUND_FORCE_INLINE Size getChannelCount() const
			{
				return numChannels;
			}
			
			
			/// Return the number of samples in this sample buffer.
			GSOUND_FORCE_INLINE Size getSampleCount() const
			{
				return numSamples;
			}
			
			
			/// Set this sample buffer to have the specified number of frequency bands, channels, and samples.
			/**
			  * The contents of the resulting buffer are undefined.
			  */
			GSOUND_INLINE void setFormat( Size newNumChannels, Size newNumSamples )
			{
				// Compute the total number of samples needed for this IR.
				Size newTotalNumSamples = newNumChannels*newNumSamples;
				
				// Reallocate the array of samples if it was previously allocated.
				if ( samples != NULL && totalNumSamples < newTotalNumSamples )
				{
					util::deallocateAligned( samples );
					samples = util::allocateAligned<SampleType>( newTotalNumSamples, 16 );
				}
				
				totalNumSamples = newTotalNumSamples;
				numChannels = newNumChannels;
				numSamples = newNumSamples;
			}
			
			
			/// Set this sample buffer to have the specified number of frequency bands, channels, and samples.
			/**
			  * The old contents are copied and the rest of the memory is uninitialized.
			  */
			GSOUND_INLINE void setFormatCopy( Size newNumChannels, Size newNumSamples )
			{
				// Compute the total number of samples needed for this IR.
				Size newTotalNumSamples = newNumChannels*newNumSamples;
				
				// Reallocate the array of samples if it was previously allocated.
				if ( samples != NULL && totalNumSamples < newTotalNumSamples )
				{
					SampleType* oldSamples = samples;
					samples = util::allocateAligned<SampleType>( newTotalNumSamples, 16 );
					
					const Size numChannelsToCopy = math::min( numChannels, newNumChannels );
					const Size numSamplesToCopy = math::min( numSamples, newNumSamples );
					
					// Copy each channel.
					for ( Index c = 0; c < numChannelsToCopy; c++ )
						om::util::copyPOD( samples + c*newNumSamples, oldSamples + c*numSamples, numSamplesToCopy );
					
					util::deallocateAligned( oldSamples );
				}
				
				totalNumSamples = newTotalNumSamples;
				numChannels = newNumChannels;
				numSamples = newNumSamples;
			}
			
			
		//********************************************************************************
		//******	Sample Accessor Methods
			
			
			/// Return a pointer to the samples for the specified channel and sample offset.
			GSOUND_INLINE SampleType* getChannel( Index channelIndex, Index sampleOffset = 0 )
			{
				if ( samples == NULL )
					return NULL;
				
				/// Compute the final offset of the requested samples.
				Index offset = channelIndex*numSamples + sampleOffset;
				
				if ( offset < totalNumSamples )
					return samples + offset;
				else
					return NULL;
			}
			
			
			/// Return a const pointer to the samples for the specified channel and sample offset.
			GSOUND_INLINE const SampleType* getChannel( Index channelIndex, Index sampleOffset = 0 ) const
			{
				if ( samples == NULL )
					return NULL;
				
				/// Compute the final offset of the requested samples.
				Index offset = channelIndex*numSamples + sampleOffset;
				
				if ( offset < totalNumSamples )
					return samples + offset;
				else
					return NULL;
			}
			
			
		//********************************************************************************
		//******	Zero Methods
			
			
			/// Zero the contents of the sample buffer if it is allocated.
			GSOUND_INLINE void zero()
			{
				if ( samples )
					om::util::zeroPOD( samples, totalNumSamples );
			}
			
			
		//********************************************************************************
		//******	IR Release Method
			
			
			/// Return whether or not this sample buffer is currently allocated.
			GSOUND_INLINE Bool isAllocated() const
			{
				return samples != NULL;
			}
			
			
			/// Allocate the memory used by this sample buffer with its current format if the buffer is not allocated.
			GSOUND_INLINE void allocate()
			{
				if ( samples == NULL )
					samples = util::allocateAligned<SampleType>( totalNumSamples, 16 );
			}
			
			
			/// Release the memory used by this sample buffer, but keep its current format.
			GSOUND_INLINE void deallocate()
			{
				if ( samples != NULL )
				{
					util::deallocateAligned( samples );
					samples = NULL;
				}
			}
			
			
		//********************************************************************************
		//******	IR Size Accessor Methods
			
			
			/// Return the total size in bytes of this frequency domain IR's internal storage.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				Size totalSize = sizeof(SampleBuffer);
				
				if ( samples != NULL )
					totalSize += totalNumSamples*sizeof(SampleType);
				
				return totalSize;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of the samples that are in this frequency-domain IR.
			SampleType* samples;
			
			
			/// The number of channels per band in this frequency-domain IR.
			Size numChannels;
			
			
			/// The number of samples per channel in this frequency-domain IR.
			Size numSamples;
			
			
			/// The total number of samples that this IR stores.
			Size totalNumSamples;
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SAMPLE_BUFFER_H
