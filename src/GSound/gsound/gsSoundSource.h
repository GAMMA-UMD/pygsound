/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundSource.h
 * Contents:    gsound::SoundSource class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SOURCE_H
#define INCLUDE_GSOUND_SOUND_SOURCE_H


#include "gsConfig.h"


#include "gsSoundDetector.h"
#include "gsSoundSourceFlags.h"
#include "gsSoundDirectivity.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class SoundSource;


/// The type to use for a sound source callback method.
/**
  * The callback should write a single channel of samples for the specified sound source to
  * the provided sample pointer. The callback should return the number of valid samples actually
  * written to the sample buffer. The callback can provide a pointer to user-defined data.
  */
typedef Size (*SourceCallback)( SoundSource* source, void* userData, Float* samples, Size numSamples, SampleRate sampleRate );




//********************************************************************************
/// A class that represents a sound source in a 3D environment.
/**
  * A sound source uses a monaural sound input stream as its source of sound to be auralized
  * through the propagation system. A sound source has a 3D position and orientation
  * and can be represented by a sphere with arbitrary non-zero radius, or can have a
  * triangle-mesh representation.
  *
  * The absolute loudness of a sound source in its environment is determined by
  * its sound power. This is the total radiated power of the source in watts.
  * The power of a source can also be conveniently specified as its power level in decibels
  * relative to the reference power, 10^-12 watts.
  *
  * Below is a listing of sound power levels for various real-world sources, from
  * loudest to quietest. 10dB indicates a change in power of 10x.
  * - 200 dBSWL: Saturn V rocket;
  * - 150 dBSWL: Jet plane at takeoff;
  * - 140 dBSWL: Propeller plane at takeoff;
  * - 130 dBSWL: Machine gun;
  * - 120 dBSWL: Small airplane engine; sonic boom; heavy thunder;
  * - 110 dBSWL: Motorcycle; loud concert; chain saw, 
  * - 100 dBSWL: Helicopter; air compressor; subway; outwoard motor, propeller plane.
  * - 90 dBSWL: Heavy traffic; large diesel vehicle; lawn mower; blender.
  * - 80 dBSWL: Voice, raised; alarm clock; dishwasher.
  * - 70 dBSWL: Voice, normal; toilet flushing; inside railroad car; inside car; clothes dryer.
  * - 60 dBSWL: Hair dryer; noisy home; ventilation fan.
  * - 50 dBSWL: window air conditioner; average home.
  * - 40 dBSWL: Voice, low; quiet home; refrigerator; bird singing;
  * - 30 dBSWL: Quiet conversation; broadcast studio.
  * - 20 dBSWL: Rustling leaves; empty auditorium; whisper; watch ticking.
  * - 10 dBSWL: Human breath.
  *
  * Data from: http://www.engineeringtoolbox.com/sound-power-level-d_58.html
  */
class SoundSource : public SoundDetector
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sound source positioned at the origin.
			SoundSource();
			
			
			/// Create a sound source with the specified position and radius.
			SoundSource( const Vector3f& newPosition, Real newRadius );
			
			
		//********************************************************************************
		//******	Power Accessor Methods
			
			
			/// Return the total power of the sound source in watts.
			/**
			  * This value is a linear measure of the total radiated power emitted by the sound source.
			  */
			GSOUND_FORCE_INLINE Real getPower() const
			{
				return power;
			}
			
			
			/// Return the total power of the sound source in dB SWL (sound power level).
			/**
			  * This value is a unitless logarithmic measure of the total radiated power
			  * emitted by the sound source, compared to a reference value of 10e-12 watts.
			  */
			GSOUND_FORCE_INLINE Real getPowerLevel() const
			{
				const Real referencePower = Real(1e-12); // watts
				return Real(10)*math::log10( power / referencePower );
			}
			
			
			/// Set the total power of the sound source in watts.
			/**
			  * This value is a linear measure of the total radiated power emitted by the sound source.
			  * The input power is clamped to be larger than 0.
			  */
			GSOUND_FORCE_INLINE void setPower( Real newPower )
			{
				power = math::max( newPower, Real(0) );
			}
			
			
			/// Set the total power of the sound source in dB SWL (sound power level).
			/**
			  * This value is a unitless logarithmic measure of the total radiated power
			  * emitted by the sound source, compared to a reference value of 10e-12 watts.
			  */
			GSOUND_FORCE_INLINE void setPowerLevel( Real newPowerDB )
			{
				const Real referencePower = Real(1e-12); // watts
				power = referencePower*math::pow( Real(10), newPowerDB / Real(10) );
			}
			
			
		//********************************************************************************
		//******	Priority Accessor Methods
			
			
			/// Return a value indicating the priority for this sound source.
			/**
			  * The range of valid priorities is user defined. The sound propagation system
			  * can apply source level of detail approaches to the sources in a scene with the
			  * lowest priorities. The default priority is 0.
			  */
			GSOUND_FORCE_INLINE Float getPriority() const
			{
				return priority;
			}
			
			
			/// Set a value indicating the priority for this sound source.
			/**
			  * The range of valid priorities is user defined. The sound propagation system
			  * can apply source level of detail approaches to the sources in a scene with the
			  * lowest priorities. The default priority is 0.
			  */
			GSOUND_FORCE_INLINE void setPriority( Float newPriority )
			{
				priority = newPriority;
			}
			
			
		//********************************************************************************
		//******	Directivity Accessor Methods
			
			
			/// Return a pointer to an object that determines how this source radiates sound in different directions.
			/**
			  * The directivity pattern is sampled for each sound path that is detected,
			  * producing directional sound sources like people, speakers, etc.
			  * If the directivity pointer is NULL, the source is assumed to have an omnidirectional
			  * radiation pattern.
			  *
			  * The source does not own or free the directivity object, the user is
			  * responsible for managing the memory.
			  */
			GSOUND_INLINE SoundDirectivity* getDirectivity()
			{
				return directivity;
			}
			
			
			/// Return a const pointer to an object that determines how this source radiates sound in different directions.
			/**
			  * The directivity pattern is sampled for each sound path that is detected,
			  * producing directional sound sources like people, speakers, etc.
			  * If the directivity pointer is NULL, the source is assumed to have an omnidirectional
			  * radiation pattern.
			  *
			  * The source does not own or free the directivity object, the user is
			  * responsible for managing the memory.
			  */
			GSOUND_INLINE const SoundDirectivity* getDirectivity() const
			{
				return directivity;
			}
			
			
			/// Set a pointer to an object that determines how this source radiates sound in different directions.
			/**
			  * The directivity pattern is sampled for each sound path that is detected,
			  * producing directional sound sources like people, speakers, etc.
			  * If the directivity pointer is NULL, the source is assumed to have an omnidirectional
			  * radiation pattern.
			  *
			  * The source does not own or free the directivity object, the user is
			  * responsible for managing the memory.
			  */
			GSOUND_INLINE void setDirectivity( SoundDirectivity* newDirectivity )
			{
				directivity = newDirectivity;
			}
			
			
		//********************************************************************************
		//******	Flags Accessor Methods
			
			
			/// Return a reference to an object which contains boolean parameters of the sound source.
			GSOUND_INLINE SoundSourceFlags& getFlags()
			{
				return flags;
			}
			
			
			/// Return an object which contains boolean parameters of the sound source.
			GSOUND_INLINE const SoundSourceFlags& getFlags() const
			{
				return flags;
			}
			
			
			/// Set an object which contains boolean parameters of the sound source.
			GSOUND_INLINE void setFlags( const SoundSourceFlags& newFlags )
			{
				flags = newFlags;
			}
			
			
			/// Return whether or not the specified boolan flag is set for this sound source.
			GSOUND_INLINE Bool flagIsSet( SoundSourceFlags::Flag flag ) const
			{
				return flags.isSet( flag );
			}
			
			
			/// Set whether or not the specified boolan flag is set for this sound source.
			GSOUND_INLINE void setFlag( SoundSourceFlags::Flag flag, Bool newIsSet = true )
			{
				flags.set( flag, newIsSet );
			}
			
			
		//********************************************************************************
		//******	Is Enabled Accessor Methods
			
			
			/// Return whether or not this source is enabled for sound propagation and rendering.
			/**
			  * Sources are enabled by default but can be disabled if no audio is being
			  * played for a source or if a source is not needed.
			  * This can increase the performance in scenes with large
			  * numbers of source that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE Bool getIsEnabled() const
			{
				return flags.isSet( SoundSourceFlags::ENABLED );
			}
			
			
			/// Set whether or not this source should be enabled for sound propagation and rendering.
			/**
			  * Sources are enabled by default but can be disabled if no audio is being
			  * played for a source or if a source is not needed.
			  * This can increase the performance in scenes with large
			  * numbers of source that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE void setIsEnabled( Bool newIsEnabled )
			{
				flags.set( SoundSourceFlags::ENABLED, newIsEnabled );
			}
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the sample rate that this sound source's output audio is computed at.
			/**
			  * Sounds that are played by this sound source are automatically converted
			  * to this sample rate if the rate doesn't match.
			  *
			  * This sample rate should match the output sample rate of the renderer(s) used
			  * for sound propagation rendering. Otherwise, sample rate conversion is done
			  * to convert the source's audio to the output sample rate. This conversion
			  * can produce artifacts, so it is advisable to use the same sample rate for
			  * everything.
			  */
			GSOUND_INLINE SampleRate getSampleRate() const
			{
				return sampler.getOutputSampleRate();
			}
			
			
			/// Set the sample rate that this sound source's output audio is computed at.
			/**
			  * Sounds that are played by this sound source are automatically converted
			  * to this sample rate if the rate doesn't match.
			  *
			  * This sample rate should match the output sample rate of the renderer(s) used
			  * for sound propagation rendering. Otherwise, sample rate conversion is done
			  * to convert the source's audio to the output sample rate. This conversion
			  * can produce artifacts, so it is advisable to use the same sample rate for
			  * everything.
			  *
			  * The sample rate should be a positive (non-zero) number.
			  */
			GSOUND_INLINE void setSampleRate( SampleRate newSampleRate )
			{
				sampler.setOutputSampleRate( newSampleRate );
			}
			
			
		//********************************************************************************
		//******	Sound Playback Methods
			
			
			/// Start playing the specified sound stream with the given parameters.
			/**
			  * If the sound is able to be played by the source, the method starts to play
			  * the sound and returns an ID for the sound. If the method fails, the method returns
			  * 0 indicating that no sound was played.
			  *
			  * @param stream - a pointer to the stream (i.e. file) to play.
			  * @param gain - a linear multiplier applied to the stream when it is played.
			  * @param loop - a boolean value indicating whether or not this sound should loop when it is finished.
			  * @param fromStart - a boolean value indicating if the stream should be rewound to play from its beginning.
			  * @param insert - an optional SoundFilter that should be applied to the stream's audio.
			  */
			Index playSound( SoundInputStream* stream, Float gain = Float(1), Bool loop = false,
								SoundFilter* insert = NULL );
			
			
			/// Pause playback of the currently playing sound with the given ID.
			/**
			  * The method returns whether or not a sound with that ID was able to be paused.
			  */
			Bool pauseSound( Index soundID );
			
			
			/// Resume playback of the currently paused sound with the given ID.
			/**
			  * The method returns whether or not a sound with that ID was able to be resumed.
			  */
			Bool resumeSound( Index soundID );
			
			
			/// Stop playing the sound with the specified ID in this source and remove it from the source.
			/**
			  * The method returns whether or not a sound with that ID was able to be stoped.
			  */
			Bool stopSound( Index soundID );
			
			
			/// Stop all sounds from playing in this source and remove the streams.
			void stopSounds();
			
			
		//********************************************************************************
		//******	Filter Accessor Methods
			
			
			/// Return a pointer to the output filter for this sound source.
			/**
			  * This filter is inserted on the output of this source before convolution with impulse responses.
			  * This allows the user to apply effects directly to the source audio stream
			  * such as dynamic range limiting or equalization.
			  *
			  * If the filter is NULL, then no filter is applied to the source's audio.
			  */
			GSOUND_INLINE SoundFilter* getFilter()
			{
				return filter;
			}
			
			
			/// Return a pointer to the output filter for this sound source.
			/**
			  * This filter is inserted on the output of this source before convolution with impulse responses.
			  * This allows the user to apply effects directly to the source audio stream
			  * such as dynamic range limiting or equalization.
			  *
			  * If the filter is NULL, then no filter is applied to the source's audio.
			  */
			GSOUND_INLINE const SoundFilter* getFilter() const
			{
				return filter;
			}
			
			
			/// Return a pointer to the output filter for this sound source.
			/**
			  * This filter is inserted on the output of this source before convolution with impulse responses.
			  * This allows the user to apply effects directly to the source audio stream
			  * such as dynamic range limiting or equalization.
			  *
			  * If the filter is NULL, then no filter is applied to the source's audio.
			  */
			GSOUND_INLINE void setFilter( SoundFilter* newFilter )
			{
				filter = newFilter;
			}
			
			
		//********************************************************************************
		//******	Sound Read Method
			
			
			/// Read the given length of time from this source's input audio stream and write it to the output buffer.
			/**
			  * This function mixes any currently playing source streams into the output buffer
			  * and advances their positions by the given output length. If a source
			  * needs to be used with multiple listeners, the source audio should be captured
			  * only once and shared among all listeners (not for each listener).
			  *
			  * The method returns the number of samples written to the output buffer.
			  *
			  * This method is normally called by the sound propagation system internally to buffer
			  * the source sound data for multiple listeners. This source sound is then
			  * convolved with the impulse response for this source separately for each listener.
			  */
			Size readSamples( SoundBuffer& outputBuffer, const Time& outputLength );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object containing boolean configuration info for this sound source.
			SoundSourceFlags flags;
			
			
			/// The total power of the sound source in watts.
			/**
			  * This value is a linear measure of the total radiated power emitted by the sound source.
			  */
			Real power;
			
			
			/// An integer value indicating the priority for this sound source.
			/**
			  * The range of valid priorities is user defined. The sound propagation system
			  * applies source level of detail approaches to the sources in a scene with the
			  * lowest priorities. The default priority is 0.
			  */
			Float priority;
			
			
			/// A pointer to an object that determines how this source radiates sound in different directions.
			SoundDirectivity* directivity;
			
			
			/// An object that plays and mixes sound streams for this source.
			om::sound::SoundPlayer sampler;
			
			
			/// A sound filter that is inserted on the output of this source before convolution with impulse responses.
			SoundFilter* filter;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SOURCE_H
