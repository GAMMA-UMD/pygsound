/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundPath.h
 * Contents:    gsound::SoundPath class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PATH_H
#define INCLUDE_GSOUND_SOUND_PATH_H


#include "gsConfig.h"


#include "gsFrequencyBandResponse.h"
#include "gsSoundPathFlags.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a single discrete sound path through a scene.
/**
  * It contains the direction from the listener in world space,
  * the total distance of the path, the relative speed along the path of the
  * source and listener, the total frequency-dependent attenuation along the path, 
  * and a unique integer hash code ID for the path.
  */
class SoundPath
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new sound path object by initializing all attributes.
			GSOUND_INLINE SoundPath( SoundPathHash newPathHash, SoundPathFlags newFlags,
									const FrequencyBandResponse& newIntensity,
									const Vector3f& newDirection, const Vector3f& newSourceDirection, Real newDistance,
									Real newRelativeSpeed, Real newSpeedOfSound )
				:	pathHash( newPathHash ),
					flags( newFlags ),
					intensity( newIntensity ),
					direction( newDirection ),
					sourceDirection( newSourceDirection ),
					distance( newDistance ),
					relativeSpeed( newRelativeSpeed ),
					speedOfSound( newSpeedOfSound )
			{
			}
			
			
		//********************************************************************************
		//******	Energy Accessor Methods
			
			
			/// Return the frequency-dependent intensity spectrum for this sound path.
			/**
			  * This value indicates the fraction of the source's total sound power that
			  * traveled along this sound path.
			  */
			GSOUND_FORCE_INLINE const FrequencyBandResponse& getIntensity() const
			{
				return intensity;
			}
			
			
			/// Set the frequency-dependent intensity spectrum for this sound path.
			/**
			  * This value indicates the fraction of the source's total sound power that
			  * traveled along this sound path.
			  */
			GSOUND_INLINE void setIntensity( const FrequencyBandResponse& newIntensity )
			{
				intensity = newIntensity;
			}
			
			
		//********************************************************************************
		//******	Direction Accessor Methods
			
			
			/// Return the unit direction from the listener to the virtual sound source.
			/**
			  * The vector returned is unit length and specified in world space.
			  */
			GSOUND_FORCE_INLINE const Vector3f& getDirection() const
			{
				return direction;
			}
			
			
			/// Set the unit direction from the listener to the virtual sound source.
			GSOUND_FORCE_INLINE void setDirection( const Vector3f& newDirection )
			{
				direction = newDirection;
			}
			
			
			/// Return the unit direction from the source towards the listener along the sound path.
			/**
			  * The vector returned is unit length and specified in world space.
			  */
			GSOUND_FORCE_INLINE const Vector3f& getSourceDirection() const
			{
				return sourceDirection;
			}
			
			
			/// Set the unit direction from the source towards the listener along the sound path.
			GSOUND_FORCE_INLINE void setSourceDirection( const Vector3f& newSourceDirection )
			{
				sourceDirection = newSourceDirection;
			}
			
			
		//********************************************************************************
		//******	Distance Accessor Methods
			
			
			/// Return the total distance in meters from the listener to the sound source along the path.
			GSOUND_FORCE_INLINE Real getDistance() const
			{
				return distance;
			}
			
			
			/// Set the total distance in meters from the listener to the sound source along the path.
			GSOUND_FORCE_INLINE void setDistance( Real newDistance )
			{
				distance = newDistance;
			}
			
			
		//********************************************************************************
		//******	Delay Accessor Methods
			
			
			/// Return the delay time in seconds from the source to the listener along the path.
			/**
			  * This time is computed by dividing the total path distance by the average
			  * speed of sound along the path.
			  */
			GSOUND_FORCE_INLINE Real getDelay() const
			{
				return distance / speedOfSound;
			}
			
			
		//********************************************************************************
		//******	Speed of Sound Accessor Methods
			
			
			/// Return the average speed of sound along the sound path in meters per second.
			GSOUND_FORCE_INLINE Real getSpeed() const
			{
				return speedOfSound;
			}
			
			
			/// Set the average speed of sound along the sound path in meters per second.
			GSOUND_FORCE_INLINE void setSpeed( Real newSpeedOfSound )
			{
				speedOfSound = newSpeedOfSound;
			}
			
			
		//********************************************************************************
		//******	Relative Speed Accessor Methods
			
			
			/// Return the relative speed of the source and listener along the sound path in meters per second.
			/**
			  * A negative speed means that the path distance is decreasing, while a positive
			  * speed indicates that the path is getting longer.
			  */
			GSOUND_FORCE_INLINE Real getRelativeSpeed() const
			{
				return relativeSpeed;
			}
			
			
			/// Set the relative speed of the source and listener along the sound path in meters per second.
			/**
			  * A negative speed means that the path distance is decreasing, while a positive
			  * speed indicates that the path is getting longer.
			  */
			GSOUND_FORCE_INLINE void getRelativeSpeed( Real newRelativeSpeed )
			{
				relativeSpeed = newRelativeSpeed;
			}
			
			
		//********************************************************************************
		//******	Sound Path Flags Accessor Methods
			
			
			/// Return an object describing boolean information about this path.
			GSOUND_FORCE_INLINE const SoundPathFlags& getFlags() const
			{
				return flags;
			}
			
			
			/// Return an object describing boolean information about this path.
			GSOUND_FORCE_INLINE void setFlags( const SoundPathFlags& newFlags )
			{
				flags = newFlags;
			}
			
			
		//********************************************************************************
		//******	Sound Path Hash Code Accessor Methods
			
			
			/// Return an integer hash code ID for this sound path.
			/**
			  * This hash code can be used to identify this path from one frame to the next.
			  * There is a very small probability of collisions, but the effects from these
			  * collisions are usually minimal.
			  */
			GSOUND_FORCE_INLINE SoundPathHash getHashCode() const
			{
				return pathHash;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A unique identifier for the sound path of this virtual source source.
			SoundPathHash pathHash;
			
			
			/// An object describing boolean information about this path.
			SoundPathFlags flags;
			
			
			/// The frequency-dependent intensity for the sound path.
			/**
			  * This value indicates the fraction of the source's total sound power that
			  * traveled along this sound path.
			  */
			FrequencyBandResponse intensity;
			
			
			/// The normalized world-space direction along the path from the listener to the sound source.
			Vector3f direction;
			
			
			/// The normalized world-space direction along the path from the sound source to the listener.
			Vector3f sourceDirection;
			
			
			/// The distance from the listener to the virtual sound source.
			Real distance;
			
			
			/// The relative speed of the source and listener along the sound path.
			Real relativeSpeed;
			
			
			/// The average speed of sound along the sound path.
			Real speedOfSound;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PATH_H
