/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundBandDirectivity.h
 * Contents:    gsound::internal::SoundBandDirectivity class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_BAND_DIRECTIVITY_H
#define INCLUDE_GSOUND_SOUND_BAND_DIRECTIVITY_H


#include "gsInternalConfig.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a directivity pattern for a directional sound source for discrete frequency bands.
class SoundBandDirectivity
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound band directivity object with an omnidirectional radiation pattern.
			SoundBandDirectivity();
			
			
		//********************************************************************************
		//******	Directivity Accessor Methods
			
			
			/// Return a reference to the spherical harmonic expansion for the direcitivity pattern.
			const SHExpansion<SIMDBands>& getDirectivity() const
			{
				return directivity;
			}
			
			
			/// Reset the band directivity to correspond to the specified directivity.
			/**
			  * The direcitivity with arbitrary frequency measurements is projected into the
			  * spherical harmonic basis for a fixed number of frequency bands. The
			  * spherical harmonic expansion uses the lowest order that satisfies the
			  * given absolute error threshold.
			  *
			  * The spherical harmonic order is limited to the specified max order.
			  * The expansion terminates early if the relative improvement in error is less
			  * than the specified convergence threshold (given as a fraction, 2% = 0.02).
			  */
			void setDirectivity( const SoundDirectivity& newDirectivity, const FrequencyBands& frequencies,
								Size numIntegrationSamples = Size(10000), Float maxError = Float(0.05),
								Float convergence = Float(0.02), Size maxOrder = Size(4) );
			
			
		//********************************************************************************
		//******	Response Accessor Methods
			
			
			/// Return the frequency band response of this directivity in the specified normalized 3D cartesian direction.
			FrequencyBandResponse getResponse( const Vector3f& direction ) const;
			
			
		//********************************************************************************
		//******	Size in Bytes Accessor Methods
			
			
			/// Return the approximate size in bytes of this sound band directivity object's allocated memory.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				return directivity.getCoefficientCount()*sizeof(FrequencyBandResponse) + 
						basis.getCoefficientCount()*sizeof(FrequencyBandResponse);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that stores a single frequency-dependent directivity sample for a 3D normalized direction.
			class GSOUND_ALIGN(16) Sample
			{
				public:
					
					/// Create a new directivity sample for the given 3D normalized direction.
					GSOUND_INLINE Sample( const Vector3f& newDirection, const SIMDBands& newResponse )
						:	direction( newDirection ),
							response( newResponse )
					{
					}
					
					
					/// The frequency response for this directivity sample.
					SIMDBands response;
					
					
					/// The normalized 3D cartesian direction for this directivity sample.
					Vector3f direction;
					
					
			};
			
			
		//********************************************************************************
		//******	Private Helper Functions
			
			
			/// Interpolate the samples in this directivity for the specified direction.
			SIMDBands interpolateSample( const Vector3f& direction, const Sample* samples, Size numSamples );
			
			
			/// Return a uniformly distributed random unit vector direction.
			GSOUND_FORCE_INLINE static Vector3f getRandomDirection( math::Random<Float>& variable )
			{
				const Float u1 = variable.sample( Float(-1), Float(1) );
				const Float u2 = variable.sample( Float(0), Float(1) );
				const Float r = math::sqrt( Float(1) - u1*u1 );
				const Float theta = Float(2)*math::pi<Float>()*u2;
				
				return Vector3f( r*math::cos( theta ), r*math::sin( theta ), u1 );
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A spherical harmonic expansion of this directivity pattern for frequency bands.
			SHExpansion<SIMDBands> directivity;
			
			
			/// A temporary projection of a 3D cartesian vector into the SH basis.
			mutable SHExpansion<SIMDBands> basis;
			
			
			/// A pointer to the last directivity information used by this band directivity.
			/**
			  * This pointer is only used to see if the directivity information source
			  * has changed since the last time the directivity was updated.
			  */
			const SoundDirectivity* lastDirectivity;
			
			
			/// The source directivity timestamp.
			/**
			  * This time stamp is used to see if the directivity information source
			  * has changed since the last time the directivity was updated.
			  */
			Index lastTimeStamp;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_BAND_DIRECTIVITY_H
