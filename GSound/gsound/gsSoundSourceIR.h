/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundSourceIR.h
 * Contents:    gsound::SoundSourceIR class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SOURCE_IR_H
#define INCLUDE_GSOUND_SOUND_SOURCE_IR_H


#include "gsConfig.h"


#include "gsSoundSource.h"
#include "gsSoundPath.h"
#include "gsSampledIR.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encapsulates the acoustic response for a sound source.
/**
  * A SoundSourceIR contains a pointer to the sound source(s) which
  * it is associated with, as well as a buffer of objects describing sound paths
  * through a scene to a single SoundListener. A source IR can also contain
  * a SampledIR that stores a discretely sampled version of the IR, depending on
  * the propagation parameters. The total IR for the source(s) is the sum of the
  * sampled IR and the discrete paths.
  *
  * In practice, one doesn't need to directly interact with any SoundSourceIR
  * objects. The manipulation of the data structure happens automatically behind the scenes.
  * However, the interface for querying sound paths is left public in case one wishes
  * to examine the output of the sound propagation system rather than render it.
  */
class SoundSourceIR
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty SoundSourceIR not associated with any sound source.
			SoundSourceIR();
			
			
			/// Create an empty SoundSourceIR which holds paths for the specified sound source.
			SoundSourceIR( const SoundSource* newSource );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sound source IR, releasing all internal resources.
			~SoundSourceIR();
			
			
		//********************************************************************************
		//******	Sound Source Accessor Methods
			
			
			/// Return the number of sources that share this impulse response.
			GSOUND_INLINE Size getSourceCount() const
			{
				return sources.getSize();
			}
			
			
			/// Return a pointer to the sound source associated with this SoundSourceIR at the specified index.
			GSOUND_INLINE const SoundSource* getSource( Index sourceIndex ) const
			{
				GSOUND_DEBUG_ASSERT( sourceIndex < sources.getSize() );
				
				return sources[sourceIndex];
			}
			
			
			/// Set the sound source that this SoundSourceIR is associated with at the specified index.
			GSOUND_INLINE void setSource( Index sourceIndex, const SoundSource* newSource )
			{
				GSOUND_DEBUG_ASSERT( sourceIndex < sources.getSize() );
				
				sources[sourceIndex] = newSource;
			}
			
			
			/// Add a new source for this impulse response.
			/**
			  * The new source is ignored if the source pointer is NULL.
			  */
			GSOUND_INLINE void addSource( const SoundSource* newSource )
			{
				if ( newSource )
					sources.add( newSource );
			}
			
			
			/// Remove all sound sources from this SoundSourceIR.
			/**
			  * The IR itself is unchanged by this operation.
			  */
			GSOUND_INLINE void clearSources()
			{
				sources.clear();
			}
			
			
		//********************************************************************************
		//******	Path Accessor Methods
			
			
			/// Return the number of sound paths that this impulse response contains.
			GSOUND_INLINE Size getPathCount() const
			{
				return paths.getSize();
			}
			
			
			/// Return a reference to the SoundPath at the specified index in the buffer.
			GSOUND_INLINE SoundPath& getPath( Index pathIndex )
			{
				GSOUND_DEBUG_ASSERT( pathIndex < paths.getSize() );
				return paths[pathIndex];
			}
			
			
			/// Return a reference to the SoundPath at the specified index in the buffer.
			GSOUND_INLINE const SoundPath& getPath( Index pathIndex ) const
			{
				GSOUND_DEBUG_ASSERT( pathIndex < paths.getSize() );
				return paths[pathIndex];
			}
			
			
			/// Return a pointer to the contiguous array of paths that this IR stores.
			GSOUND_INLINE const SoundPath* getPathPointer() const
			{
				return paths.getPointer();
			}
			
			
			/// Add a new SoundPath to the SoundSourceIR.
			GSOUND_INLINE void addPath( const SoundPath& newSoundPath )
			{
				paths.add( newSoundPath );
				
				const Float pathDelay = newSoundPath.getDelay();
				startTime = math::min( startTime, pathDelay );
				length = math::max( length, pathDelay );
			}
			
			
			/// Add all entries from one source IR to this one.
			void addPaths( const SoundSourceIR& newPaths );
			
			
		//********************************************************************************
		//******	Sampled IR Accessor Methods
			
			
			/// Return a reference to the sampled IR for this sound source IR.
			GSOUND_INLINE SampledIR& getSampledIR()
			{
				return sampledIR;
			}
			
			
			/// Return a reference to the sampled IR for this sound source IR.
			GSOUND_INLINE const SampledIR& getSampledIR() const
			{
				return sampledIR;
			}
			
			
			/// Add the specified impulse to this sound source IR's sampled impulse response.
			GSOUND_INLINE void addImpulse( Float delay, const FrequencyBandResponse& newEnergy,
											const Vector3f& direction, const Vector3f& sourceDirection )
			{
				sampledIR.addImpulse( delay, newEnergy, direction, sourceDirection );
			}
			
			
			/// Add a new impulse to the sampled IR that corresponds to the specified sound path.
			GSOUND_FORCE_INLINE void addImpulse( const SoundPath& path )
			{
				sampledIR.addImpulse( path );
			}
			
			
		//********************************************************************************
		//******	IR Clear Methods
			
			
			/// Clear all impulses and paths from this sound source IR.
			/**
			  * This method keeps the IR storage to avoid many reallocations.
			  */
			GSOUND_INLINE void clear()
			{
				paths.clear();
				sampledIR.clear();
				startTime = math::max<Float>();
				length = 0;
			}
			
			
			/// Clear all impulses and paths from this sound source IR.
			/**
			  * This method deallocates the IR storage.
			  */
			void reset();
			
			
		//********************************************************************************
		//******	IR Length Accessor Methods
			
			
			/// Return the delay time in seconds of the first sound arrival at the listener.
			/**
			  * Before this delay time, the IR can be assumed to be all zeros (empty).
			  */
			GSOUND_INLINE Float getStartTime() const
			{
				return math::min( math::min( startTime, sampledIR.getStartTime() ), this->getLength() );
			}
			
			
			/// Return the delay time in samples of the first sound arrival at the listener.
			/**
			  * Before this delay time, the IR can be assumed to be all zeros (empty).
			  */
			GSOUND_INLINE Index getStartTimeInSamples() const
			{
				SampleRate sampleRate = sampledIR.getSampleRate();
				
				return (Index)math::floor(this->getStartTime()*sampleRate);
			}
			
			
			/// Return the length in seconds of this IR.
			GSOUND_INLINE Float getLength() const
			{
				return math::max( length, sampledIR.getLength() );
			}
			
			
			/// Return the length in samples of this IR.
			GSOUND_INLINE Size getLengthInSamples() const
			{
				SampleRate sampleRate = sampledIR.getSampleRate();
				Size pathLength = (Size)math::ceiling(length*sampleRate);
				return math::max( pathLength, sampledIR.getLengthInSamples() );
			}
			
			
			/// Return the smallest delay time in seconds of the discrete paths in this IR.
			GSOUND_INLINE Float getMinPathDelay() const
			{
				return startTime;
			}
			
			
			/// Return the smallest delay time in samples of the discrete paths in this IR.
			GSOUND_INLINE Size getMinPathDelayInSamples() const
			{
				return (Size)math::floor( startTime*sampledIR.getSampleRate() );
			}
			
			
			
			/// Return the longest delay time in seconds of the discrete paths in this IR.
			GSOUND_INLINE Float getMaxPathDelay() const
			{
				return length;
			}
			
			
			
			/// Return the longest delay time in samples of the discrete paths in this IR.
			GSOUND_INLINE Size getMaxPathDelayInSamples() const
			{
				return (Size)math::ceiling( length*sampledIR.getSampleRate() );
			}
			
			
		//********************************************************************************
		//******	IR Windowing Methods
			
			
			/// Trim the source IR's length based on the specified threshold of hearing in units of sound power (watts).
			/**
			  * The method returns the resulting length of the IR in seconds.
			  */
			Float trim( const FrequencyBandResponse& thresholdPower );
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the sample rate of this IR in samples per second.
			GSOUND_INLINE SampleRate getSampleRate() const
			{
				return sampledIR.getSampleRate();
			}
			
			
			/// Set the sample rate of this IR in samples per second.
			/**
			  * The new sample rate is clamped to be greater than or equal to 0.
			  * This method does not change the currently stored IR, just the sample rate
			  * at which it is interpreted.
			  */
			GSOUND_INLINE void setSampleRate( SampleRate newSampleRate )
			{
				sampledIR.setSampleRate( newSampleRate );
			}
			
			
		//********************************************************************************
		//******	Reverb Time Accessor Method
			
			
			/// Return the reverb time for this IR.
			GSOUND_INLINE Real getReverbTime() const
			{
				return reverbTime;
			}
			
			
			/// Set the reverb time for this IR.
			GSOUND_INLINE void setReverbTime( Real newReverbTime )
			{
				reverbTime = newReverbTime;
			}
			
			
		//********************************************************************************
		//******	Total Energy Accessor Method
			
			
			/// Compute and return the total fraction of the source's energy contained in the impulse response.
			FrequencyBandResponse getTotalIntensity() const;
			
			
			/// Compute and return the sound pressure in pascals of the source.
			FrequencyBandResponse getPressure() const;
			
			
			/// Compute and return the sound pressure level (in dB SPL, per frequency band) of the source.
			FrequencyBandResponse getPressureLevel() const;
			
			
		//********************************************************************************
		//******	Storage Size Accessor Methods
			
			
			/// Return the approximate size in bytes of the memory used for this IR.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				return sizeof(SoundSourceIR) + paths.getCapacity()*sizeof(SoundPath) + 
						sources.getCapacity()*sizeof(SoundSource*) + sampledIR.getSizeInBytes();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of all discrete sound paths for the source(s) associated with this sound impulse response.
			ArrayList<SoundPath> paths;
			
			
			/// An object that contains a sampled IR for this sound source.
			SampledIR sampledIR;
			
			
			/// A list of to the sound sources that this sound impulse response contains paths for.
			ShortArrayList<const SoundSource*,4> sources;
			
			
			/// The delay time in seconds of the first impulse or path in this IR.
			Float startTime;
			
			
			/// The length in seconds of this IR.
			Float length;
			
			
			/// The reverb time (-60dB) in seconds for the IR.
			Real reverbTime;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SOURCE_IR_H
