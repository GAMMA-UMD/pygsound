/*
 * Project:     GSound
 * 
 * File:        gsound/gsSampledIR.h
 * Contents:    gsound::SampledIR class declaration
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


#ifndef INCLUDE_GSOUND_SAMPLED_IR_H
#define INCLUDE_GSOUND_SAMPLED_IR_H


#include "gsConfig.h"


#include "gsFrequencyBandResponse.h"
#include "gsSoundPath.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a discretely sampled spatial impulse response.
/**
  * A sampled IR contains an impulse response that is sampled at a certain sample
  * rate (e.g. 44100Hz). It contains the IRs for GSOUND_FREQUENCY_COUNT frequency bands,
  * as well as direction IRs that contain 3D vectors for each sample.
  *
  * This is the most commonly used IR representation because it does not require
  * storing each discrete path through the scene (there may be millions). Paths that
  * have the same delay time have their directionality and energy added.
  */
class SampledIR
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty sampled IR of length 0 with the default sample rate of 44.1 kHz.
			SampledIR();
			
			
			/// Create a new empty sampled IR of length 0 with the specified sample rate.
			SampledIR( SampleRate newSampleRate );
			
			
			/// Create a new sampled IR that is an exact copy of another IR.
			SampledIR( const SampledIR& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sampled IR, releasing its internal storage.
			~SampledIR();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another IR to this one.
			/**
			  * This IR's internal storage is reallocated if necessary to hold the other
			  * IR.
			  */
			SampledIR& operator = ( const SampledIR& other );
			
			
		//********************************************************************************
		//******	IR Start Time Accessor Methods
			
			
			/// Return the delay time in samples of the first non-zero sample in this IR.
			GSOUND_INLINE Size getStartTimeInSamples() const
			{
				return math::min( startOffset, numSamples );
			}
			
			
			/// Set the number of samples there are in this sampled IR.
			/**
			  * If the specified length is shorter than the current length, the
			  * IR is truncated. Otherwise, the IR is extended with zeros.
			  */
			GSOUND_INLINE void setStartTimeInSamples( Size newStartTimeInSamples )
			{
				startOffset = math::min( newStartTimeInSamples, numSamples );
			}
			
			
			/// Return the delay time in seconds of the first non-zero sample in this IR.
			GSOUND_INLINE Float getStartTime() const
			{
				if ( sampleRate != SampleRate(0) )
					return Float(math::min( startOffset, numSamples ) / sampleRate);
				else
					return Float(0);
			}
			
			
		//********************************************************************************
		//******	IR Length Accessor Methods
			
			
			/// Return the length in seconds of this sampled IR.
			GSOUND_INLINE Float getLength() const
			{
				if ( sampleRate != SampleRate(0) )
					return Float(numSamples / sampleRate);
				else
					return Float(0);
			}
			
			
			/// Return the number of samples there are in this sampled IR.
			GSOUND_INLINE Size getLengthInSamples() const
			{
				return numSamples;
			}
			
			
			/// Set the number of samples there are in this sampled IR.
			/**
			  * If the specified length is shorter than the current length, the
			  * IR is truncated but memory is not reallocated.
			  * Otherwise, the IR is extended with zeros and reallocated if necessary.
			  */
			GSOUND_INLINE void setLengthInSamples( Size newLengthInSamples, Bool zeroPadding = true )
			{
				if ( newLengthInSamples > capacity )
					reallocate( newLengthInSamples );
				
				if ( newLengthInSamples > numSamples && zeroPadding )
					zero( numSamples, newLengthInSamples - numSamples );
				
				numSamples = newLengthInSamples;
				startOffset = math::min( startOffset, newLengthInSamples > 0 ? newLengthInSamples - 1 : 0 );
			}
			
			
		//********************************************************************************
		//******	IR Trimming Methods
			
			
			/// Trim the source IR's length based on the specified threshold of hearing.
			/**
			  * The threshold is specified as a fraction of the source's total power.
			  * The method returns the resulting length of the IR in seconds.
			  */
			Float trim( const FrequencyBandResponse& threshold );
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the sample rate of this IR in samples per second.
			GSOUND_INLINE SampleRate getSampleRate() const
			{
				return sampleRate;
			}
			
			
			/// Set the sample rate of this IR in samples per second.
			/**
			  * The new sample rate is clamped to be greater than or equal to 0.
			  * This method does not change the currently stored IR, just the sample rate
			  * at which it is interpreted.
			  */
			GSOUND_INLINE void setSampleRate( SampleRate newSampleRate )
			{
				sampleRate = math::max( newSampleRate, SampleRate(0) );
			}
			
			
		//********************************************************************************
		//******	Impulse Accessor Methods
			
			
			/// Add a new impulse to this IR at the specified delay time, with the given energy and normalized direction.
			GSOUND_INLINE void addImpulse( Float delay, const FrequencyBandResponse& newIntensity,
										const Vector3f& direction, const Vector3f& sourceDirection )
			{
				Index sampleIndex = (Index)math::floor( math::max( delay*sampleRate, SampleRate(0) ) );
				Size newNumSamples = sampleIndex + 1;
				
				// Reallocate or zero the IR if necessary.
				if ( newNumSamples > capacity )
					reallocate( newNumSamples );

//                if ( sampleIndex > numSamples )  // originally, Carl didn't account for the equal case
                if ( sampleIndex >= numSamples )
					zero( numSamples, newNumSamples - numSamples );

				// Accumulate the IR in each band.
				*(FrequencyBandResponse*)(intensity + sampleIndex*numFrequencyBands) += newIntensity;
				
				// Accumulate the directions.
				directions[sampleIndex] += direction;
				
				if ( sourceDirectionsEnabled )
					sourceDirections[sampleIndex] += sourceDirection;
				
				numSamples = math::max( numSamples, newNumSamples );
				startOffset = math::min( startOffset, sampleIndex );
			}
			
			
			/// Add a new impulse to this IR that corresponds to the specified sound path.
			GSOUND_FORCE_INLINE void addImpulse( const SoundPath& path )
			{
				this->addImpulse( path.getDelay(), path.getIntensity(), path.getDirection(), path.getSourceDirection() );
			}
			
			
			/// Accumulate another IR in this one, adding all of the other IR's energy and directions.
			/**
			  * If the two IRs do not have the same sample rate, the method fails and FALSE is returned.
			  * Otherwise, the method succeeds and returns TRUE.
			  */
			Bool addIR( const SampledIR& other );
			
			
			/// Reset the IR to be of length 0 with no impulses.
			/**
			  * This method keeps the IR storage to avoid many reallocations.
			  */
			GSOUND_INLINE void clear()
			{
				startOffset = math::max<Index>();
				numSamples = 0;
			}
			
			
			/// Reset the IR to be of length 0 with no impulses.
			/**
			  * This method deallocates the IR storage.
			  */
			void reset();
			
			
		//********************************************************************************
		//******	IR Frequency Band Accessor Methods
			
			
			/// Return the number of frequency bands that this sampled IR has.
			GSOUND_INLINE Size getBandCount() const
			{
				return numFrequencyBands;
			}
			
			
			/// Return a pointer to the intensity impulse response with interleaved frequency bands.
			GSOUND_INLINE Float* getIntensity()
			{
				return intensity;
			}
			
			
			/// Return a pointer to the intensity impulse response with interleaved frequency bands.
			GSOUND_INLINE const Float* getIntensity() const
			{
				return intensity;
			}
			
			
		//********************************************************************************
		//******	Direction Sample Accessor Methods
			
			
			/// Return a pointer to the impulse response directions.
			GSOUND_INLINE Vector3f* getDirections()
			{
				return directions;
			}
			
			
			/// Return a pointer to the impulse response directions.
			GSOUND_INLINE const Vector3f* getDirections() const
			{
				return directions;
			}
			
			
		//********************************************************************************
		//******	Source Direction Sample Accessor Methods
			
			
			/// Return a pointer to the impulse response source directions.
			/**
			  * If source directions are not enabled for this IR, the method returns NULL.
			  */
			GSOUND_INLINE Vector3f* getSourceDirections()
			{
				return sourceDirections;
			}
			
			
			/// Return a pointer to the impulse response source directions.
			/**
			  * If source directions are not enabled for this IR, the method returns NULL.
			  */
			GSOUND_INLINE const Vector3f* getSourceDirections() const
			{
				return sourceDirections;
			}
			
			
			/// Return whether or not this sampled IR is storing source directions.
			GSOUND_INLINE Bool getSourceDirectionsEnabled() const
			{
				return sourceDirectionsEnabled;
			}
			
			
			/// Set whether or not this sampled IR is storing source directions.
			void setSourceDirectionsEnabled( Bool newSourceDirectionsEnabled );
			
			
		//********************************************************************************
		//******	Total Energy Accessor Method
			
			
			/// Compute and return the total fraction of the source's power contained in the impulse response for each frequency band.
			FrequencyBandResponse getTotalIntensity() const;
			
			
		//********************************************************************************
		//******	IR Size Accessor Methods
			
			
			/// Return the approximate size of the memory used by this sampled IR.
			Size getSizeInBytes() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Reallocate the capacity of this IR so that it is at least the specified size.
			void reallocate( Size minimumSize );
			
			
			/// Zero the specified range of samples in this IR.
			void zero( Index startIndex, Size number );
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The global number of frequency bands in an IR.
			static const Size numFrequencyBands = GSOUND_FREQUENCY_COUNT;
			
			
			/// The default sample rate that is used for a sampled IR, 44.1 kHz.
			static const SampleRate DEFAULT_SAMPLE_RATE;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The intensity for each sample and frequency band in this sampled IR.
			/**
			  * The samples for each frequency band are interleaved.
			  */
			Float* intensity;
			
			
			/// The directions for each sample in this sampled IR.
			Vector3f* directions;
			
			
			/// The source directions for each sample in this sampled IR.
			Vector3f* sourceDirections;
			
			
			/// The index of the first non-zero sample in the IR.
			Index startOffset;
			
			
			/// The number of valid samples in this IR.
			Size numSamples;
			
			
			/// The allocated capacity of this IR in samples.
			Size capacity;
			
			
			/// The sample rate of this impulse repsonse.
			SampleRate sampleRate;
			
			
			/// A boolean value that indicates whether or not this sampled IR stores source directions.
			Bool sourceDirectionsEnabled;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SAMPLED_IR_H
