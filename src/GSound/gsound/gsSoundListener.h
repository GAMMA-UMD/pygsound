/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundListener.h
 * Contents:    gsound::SoundListener class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_LISTENER_H
#define INCLUDE_GSOUND_SOUND_LISTENER_H


#include "gsConfig.h"


#include "gsSoundDetector.h"
#include "gsSoundListenerFlags.h"
#include "gsFrequencyResponse.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a sound reciever in a 3D sound propagation system.
/**
  * Each listener corresponds to an audio 'view' of the scene that it inhabits,
  * the analogue for graphics is a camera.
  *
  * A listener has a position and is usually represented by a sphere.
  */
class SoundListener : public SoundDetector
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default listener centered at the origin.
			SoundListener();
			
			
			/// Create a listener with the specified position and radius.
			SoundListener( const Vector3f& newPosition, Real newRadius );
			
			
		//********************************************************************************
		//******	Sensitivity Accessor Methods
			
			
			/// Return the hearing sensitivity of the listener in dB SPL.
			/**
			  * The listener's hearing sensitivity is used to determine the dynamic range
			  * of the listener's output audio. Increasing the sensitivity makes quiet sounds
			  * louder. The sensitivity is a scale factor that converts from high-dynamic-range
			  * to the dynamic range of the output audio (i.e. samples between -1 and 1).
			  */
			GSOUND_FORCE_INLINE Float getSensitivity() const
			{
				return sensitivity;
			}
			
			
			/// Set the hearing sensitivity of the listener in dB SPL.
			/**
			  * The listener's hearing sensitivity is used to determine the dynamic range
			  * of the listener's output audio. Increasing the sensitivity makes quiet sounds
			  * louder. The sensitivity is a scale factor that converts from high-dynamic-range
			  * to the dynamic range of the output audio (i.e. samples between -1 and 1).
			  *
			  * The sensitivity is independent of the listener's hearing threshold, so the
			  * threshold may need to also be adjusted to keep the hearing threshold perceptually
			  * valid.
			  */
			GSOUND_FORCE_INLINE void setSensitivity( Float newSensitivity )
			{
				sensitivity = newSensitivity;
			}
			
			
		//********************************************************************************
		//******	Hearing Threshold Accessor Methods
			
			
			/// Return the frequency-dependent threshold in dB SPL below which sound is considered inaudible for this listener.
			/**
			  * The hearing threshold is an absolute threshold in dB SPL that determines
			  * the cutoff of what is considered inaudible by the listener. The threshold
			  * is used to determine which sound sources are audible to the listener, as
			  * well as the audible parts of the source impulse responses.
			  */
			GSOUND_FORCE_INLINE FrequencyResponse& getThreshold()
			{
				return threshold;
			}
			
			
			/// Return the frequency-dependent threshold in dB SPL below which sound is considered inaudible for this listener.
			/**
			  * The hearing threshold is an absolute threshold in dB SPL that determines
			  * the cutoff of what is considered inaudible by the listener. The threshold
			  * is used to determine which sound sources are audible to the listener, as
			  * well as the audible parts of the source impulse responses.
			  */
			GSOUND_FORCE_INLINE const FrequencyResponse& getThreshold() const
			{
				return threshold;
			}
			
			
			/// Set the frequency-dependent threshold in dB SPL below which sound is considered inaudible for this listener.
			/**
			  * The hearing threshold is an absolute threshold in dB SPL that determines
			  * the cutoff of what is considered inaudible by the listener. The threshold
			  * is used to determine which sound sources are audible to the listener, as
			  * well as the audible parts of the source impulse responses.
			  */
			GSOUND_FORCE_INLINE void setThreshold( const FrequencyResponse& newThresholdDBSPL )
			{
				threshold = newThresholdDBSPL;
			}
			
			
		//********************************************************************************
		//******	Hearing Threshold Bias Accessor Methods
			
			
			/// Return the constant bias in dB applied to this listener's frequency-dependent threshold of hearing.
			/**
			  * The threshold bias can be used to adjust the threshold of hearing
			  * without having to manually adjust the threshold frequencies.
			  * This can be useful if the user is in a noisy listening environment
			  * or is hearing-impaired, and can also be used to adjust the threshold
			  * based on the user's actual physical listening level (with calibration).
			  *
			  * The default bias is 5 dB. A positive bias means that the hearing threshold is raised.
			  */
			GSOUND_FORCE_INLINE Float getThresholdBias() const
			{
				return thresholdBias;
			}
			
			
			/// Set the constant bias in dB applied to this listener's frequency-dependent threshold of hearing.
			/**
			  * The threshold bias can be used to adjust the threshold of hearing
			  * without having to manually adjust the threshold frequencies.
			  * This can be useful if the user is in a noisy listening environment
			  * or is hearing-impaired, and can also be used to adjust the threshold
			  * based on the user's actual physical listening level (with calibration).
			  *
			  * The default bias is 5 dB. A positive bias means that the hearing threshold is raised.
			  */
			GSOUND_FORCE_INLINE void setThresholdBias( Float newThresholdBias )
			{
				thresholdBias = newThresholdBias;
			}
			
			
		//********************************************************************************
		//******	Hearing Threshold Power Accessor Methods
			
			
			/// Return the frequency-dependent threshold in sound power below which sound is considered inaudible for this listener.
			/**
			  * This method computes a frequency band response that contains the hearing threshold
			  * in sound power (watts) for each of the given frequency bands.
			  */
			FrequencyBandResponse getThresholdPower( const FrequencyBands& frequencies ) const;
			
			
		//********************************************************************************
		//******	Flags Accessor Methods
			
			
			/// Return a reference to an object which contains boolean parameters of the sound listener.
			GSOUND_INLINE SoundListenerFlags& getFlags()
			{
				return flags;
			}
			
			
			/// Return an object which contains boolean parameters of the sound listener.
			GSOUND_INLINE const SoundListenerFlags& getFlags() const
			{
				return flags;
			}
			
			
			/// Set an object which contains boolean parameters of the sound listener.
			GSOUND_INLINE void setFlags( const SoundListenerFlags& newFlags )
			{
				flags = newFlags;
			}
			
			
			/// Return whether or not the specified boolan flag is set for this sound listener.
			GSOUND_INLINE Bool flagIsSet( SoundListenerFlags::Flag flag ) const
			{
				return flags.isSet( flag );
			}
			
			
			/// Set whether or not the specified boolan flag is set for this sound listener.
			GSOUND_INLINE void setFlag( SoundListenerFlags::Flag flag, Bool newIsSet = true )
			{
				flags.set( flag, newIsSet );
			}
			
			
		//********************************************************************************
		//******	Is Enabled Accessor Methods
			
			
			/// Return whether or not this listener is enabled for sound propagation and rendering.
			/**
			  * Listeners are enabled by default but can be disabled if no audio is being
			  * played for a listener or if a listener is not needed.
			  * This can increase the performance in scenes with large
			  * numbers of listeners that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE Bool getIsEnabled() const
			{
				return flags.isSet( SoundListenerFlags::ENABLED );
			}
			
			
			/// Set whether or not this listener should be enabled for sound propagation and rendering.
			/**
			  * Listeners are enabled by default but can be disabled if no audio is being
			  * played for a listener or if a listener is not needed.
			  * This can increase the performance in scenes with large
			  * numbers of listeners that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE void setIsEnabled( Bool newIsEnabled )
			{
				flags.set( SoundListenerFlags::ENABLED, newIsEnabled );
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// The default sensitivity in dB SPL that is used for sound listeners.
			static const Float DEFAULT_SENSITIVITY;
			
			
			/// The default hearing threshold in dB SPL over all frequencies that is used for sound listeners.
			/**
			  * This response represents the measured hearing threshold of an average person
			  * across the frequency range.
			  */
			static const FrequencyResponse DEFAULT_THRESHOLD;
			
			
			/// The default constant hearing threshold bias in dB that is used for sound listeners.
			static const Float DEFAULT_THRESHOLD_BIAS;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// Return the approximate human threshold of hearing in dB SPL for the specified frequency.
			/**
			  * Equation from "Calculating Virtual Pitch" by E. Terhardt, 1979.
			  */
			GSOUND_INLINE static Real getHearingThreshold( Real frequency );
			
			
			/// Compute an approximation of the frequency response for the default listener hearing threshold.
			/**
			  * The computed response has the specified number of bands, with frequencies logarithmically
			  * spaced from 20 to 20 kHz.
			  */
			static void getHearingThresholdResponse( Real minFreq, Real maxFreq, Size numBands, FrequencyResponse& response );
			
			
			/// Compute an approximation of the frequency response for the default listener hearing threshold.
			/**
			  * The computed response has the specified number of bands, with frequencies logarithmically
			  * spaced from 20 to 20 kHz.
			  */
			static FrequencyResponse getHearingThresholdResponse( Real minFreq, Real maxFreq, Size numBands );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object containing boolean configuration info for this sound listener.
			SoundListenerFlags flags;
			
			
			/// The hearing sensitivity of the listener in dB SPL.
			/**
			  * The listener's hearing sensitivity is used to determine the dynamic range
			  * of the listener's output audio. Increasing the sensitivity makes quiet sounds
			  * louder. The sensitivity is a scale factor that converts from high-dynamic-range
			  * to the dynamic range of the output audio (i.e. samples between -1 and 1).
			  */
			Real sensitivity;
			
			
			/// The hearing threshold bias of the listener in dB.
			/**
			  * The listener's hearing sensitivity is used to determine the dynamic range
			  * of the listener's output audio. Increasing the sensitivity makes quiet sounds
			  * louder. The sensitivity is a scale factor that converts from high-dynamic-range
			  * to the dynamic range of the output audio (i.e. samples between -1 and 1).
			  */
			Float thresholdBias;
			
			
			/// The hearing threshold of the listener in dB SPL over all frequencies.
			/**
			  * The hearing threshold is an absolute threshold in dB SPL that determines
			  * the cutoff of what is considered inaudible by the listener. The threshold
			  * is used to determine which sound sources are audible to the listener, as
			  * well as the audible parts of the source impulse responses.
			  */
			FrequencyResponse threshold;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_LISTENER_H
