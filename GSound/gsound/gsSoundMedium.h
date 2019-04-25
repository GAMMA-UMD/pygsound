/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMedium.h
 * Contents:    gsound::SoundMedium class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MEDIUM_H
#define INCLUDE_GSOUND_SOUND_MEDIUM_H


#include "gsConfig.h"


#include "gsFrequencyBandResponse.h"
#include "gsFrequencyBands.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies how sound propagates within a medium.
/**
  * The attributes stored by this class include the speed of sound in the
  * propagation medium (meters per second), as well as the frequency-dependent
  * air absorption in the medium (dB per meter).
  */
class SoundMedium
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a default medium with no air absorption and the default speed of sound of 343 meters/second.
			SoundMedium();
			
			
			/// Create a new medium with the specified speed of sound and frequency-dependent air absorption.
			SoundMedium( Real newSpeedOfSound, const FrequencyBandResponse& newAirAbsorption );
			
			
		//********************************************************************************
		//******	Speed of Sound Accessor Methods
			
			
			/// Return the speed of sound in the medium in meters/second.
			GSOUND_INLINE Real getSpeed() const
			{
				return speedOfSound;
			}
			
			
			/// Set the speed of sound in the medium in meters/second.
			/**
			  * The speed of sound is clamped to be greater than or equal to 0.
			  */
			GSOUND_INLINE void setSpeed( Real newSpeedOfSound )
			{
				speedOfSound = math::max( newSpeedOfSound, Real(0) );
			}
			
			
		//********************************************************************************
		//******	Air Absorption Accessor Methods
			
			
			/// Return the absorption in dB/meter of this medium.
			GSOUND_INLINE FrequencyBandResponse& getAbsorption()
			{
				return absorption;
			}
			
			
			/// Return the absorption in dB/meter of this medium.
			GSOUND_INLINE const FrequencyBandResponse& getAbsorption() const
			{
				return absorption;
			}
			
			
			/// Set the absorption in dB/meter of this medium.
			GSOUND_INLINE void setAbsorption( const FrequencyBandResponse& newAirAbsorption )
			{
				absorption = newAirAbsorption;
			}
			
			
			/// Return the absorption in dB/meter of this medium at the specified frequency band index.
			GSOUND_INLINE Real getAbsorption( Index frequencyBandIndex ) const
			{
				return absorption[frequencyBandIndex];
			}
			
			
			/// Set the absorption in dB/meter of this medium at the specified frequency band index.
			GSOUND_INLINE void setAbsorption( Index frequencyBandIndex, Real newDBPerMeter )
			{
				absorption[frequencyBandIndex] = math::max( newDBPerMeter, Real(0) );
			}
			
			
		//********************************************************************************
		//******	Attenuation Accessor Methods
			
			
			/// Return the linear attenuation of sound due to traveling the specified distance through this medium.
			/**
			  * This function computes the attenuation due to air absorption, but not
			  * due to sound spreading loss.
			  */
			GSOUND_INLINE FrequencyBandResponse getAttenuation( Real distance ) const
			{
				// Compute total attenuation in dB/meter.
				FrequencyBandResponse result = absorption*distance;
				
				// Convert to linear.
				for ( Index i = 0; i < result.getBandCount(); i++ )
					result[i] = math::dbToLinear( -result[i] );
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Air Attenuation Computation
			
			
			/// Return a sound medium with the correct attributes for the given air properties.
			/**
			  * This method uses models for the air absorption and speed of sound to compute
			  * these attributes for any given reasonable value of temperature in degrees celsius,
			  * pressure in kPa, and relative humidity (percentage, [0 to 100]).
			  */
			static SoundMedium getAirMedium( Real tempC, Real pressurekPa, Real relativeHumidity,
											const FrequencyBands& frequencies = FrequencyBands() );
			
			
		//********************************************************************************
		//******	Static Medium Objects
			
			
			/// The default standard medium of air at sea level, 20 degrees C and RH of 50%.
			/**
			  * This medium is constructed using the default frequency bands.
			  */
			static const SoundMedium AIR;
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			
			/// Compute the speed of sound in air with the specified attributes.
			static Real getAirSpeedOfSound( Real tempC, Real pressurekPa, Real relativeHumidity );
			
			
			/// Compute the absorption of air with the specified attributes.
			static FrequencyBandResponse getAbsorption( Real tempC, Real pressurekPa, Real relativeHumidity,
														const FrequencyBands& frequencies );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The air absorption coefficient in dB per meter for each frequency band.
			FrequencyBandResponse absorption;
			
			
			/// The speed of sound in this medium.
			Real speedOfSound;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MEDIUM_H
