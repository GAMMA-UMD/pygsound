/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundDirectivity.h
 * Contents:    gsound::SoundDirectivity class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_DIRECTIVITY_H
#define INCLUDE_GSOUND_SOUND_DIRECTIVITY_H


#include "gsConfig.h"


#include "gsFrequencyResponse.h"
#include "internal/gsSoundBandDirectivity.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a frequency-dependent directivity pattern for a directional sound source.
/**
  * The directivity determines how sound is radiated from a source in all
  * directions for each frequency. The directivity is specified by providing
  * frequency response measurements for various directions. Internally, a
  * spherical harmonic representation is used to smoothly interpolate the
  * frequency responses for all directions. The directivity is sampled for
  * each sound path that is generated.
  *
  * The directivity is projected into the spherical harmonic basis whenever
  * it has been modified and is needed for a sound source. This operation is
  * slightly expensive (milliseconds), so only modify directivity when absolutely necessary.
  */
class SoundDirectivity
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound directivity object with an omnidirectional radiation pattern.
			SoundDirectivity();
			
			
			/// Create a new sound directivity object with an omnidirectional radiation pattern.
			SoundDirectivity( const UTF8String& newName );
			
			
		//********************************************************************************
		//******	Sample Accessor Methods
			
			
			/// Return the number of directivity samples there are in this directivity object.
			GSOUND_INLINE Size getSampleCount() const
			{
				return samples.getSize();
			}
			
			
			/// Return a const reference to the frequency response for the directivity sample with the given index.
			GSOUND_INLINE const FrequencyResponse& getSample( Index sampleIndex ) const
			{
				return samples[sampleIndex].response;
			}
			
			
			/// Set the frequency response for the directivity sample with the given index.
			/**
			  * The previous response for that sample index is replaced with the new one.
			  */
			void setSample( Index sampleIndex, const FrequencyResponse& newResponse );
			
			
			/// Add a new directivity sample with the given frequency response for the specified 3D direction.
			void addSample( const Vector3f& direction, const FrequencyResponse& response );
			
			
		//********************************************************************************
		//******	Sample Direction Accessor Methods
			
			
			/// Return a const reference to the 3D normalized direction for the directivity sample with the given index.
			GSOUND_INLINE const Vector3f& getSampleDirection( Index sampleIndex ) const
			{
				return samples[sampleIndex].direction;
			}
			
			
			/// Set the 3D normalized direction for the directivity sample with the given index.
			/**
			  * The previous direction for that sample index is replaced with the new one.
			  */
			GSOUND_INLINE void setSampleDirection( Index sampleIndex, const Vector3f& newDirection )
			{
				if ( sampleIndex < samples.getSize() )
				{
					samples[sampleIndex].direction = newDirection.normalize();
					timeStamp++;
				}
			}
			
			
		//********************************************************************************
		//******	Reset Method
			
			
			/// Reset this directivity pattern to an omnidirectional pattern.
			void reset();
			
			
		//********************************************************************************
		//******	Orientation Accessor Methods
			
			
			/// Return a 3x3 orthonormal matrix that rotates this sound directivity pattern.
			/**
			  * This rotation is applied to the directivity to transform it into the local coordinates
			  * of a sound source during preprocessing, or whenever the directivity is modified.
			  * At runtime, the source's orientation is applied to the result of this rotation
			  * to transform it into world space.
			  */
			GSOUND_FORCE_INLINE const Matrix3f& getOrientation() const
			{
				return orientation;
			}
			
			
			/// Set a 3x3 orthonormal matrix that rotates this sound directivity pattern.
			/**
			  * This rotation is applied to the directivity to transform it into the local coordinates
			  * of a sound source during preprocessing, or whenever the directivity is modified.
			  * At runtime, the source's orientation is applied to the result of this rotation
			  * to transform it into world space.
			  */
			void setOrientation( const Matrix3f& newOrientation );
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return the name of this sound directivity.
			GSOUND_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set the name of this sound directivity.
			GSOUND_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Predefined Directivity Objects
			
			
			/// A directivity pattern corresponding to a human voice (singing).
			static const SoundDirectivity HUMAN_VOICE;
			
			
			/// A directivity pattern corresponding to a trumpet.
			static const SoundDirectivity TRUMPET;
			
			
			/// A directivity pattern corresponding to a trombone.
			static const SoundDirectivity TROMBONE;
			
			
			/// A directivity pattern corresponding to a JBL LSR4328P speaker.
			static const SoundDirectivity JBL_LSR_4328P;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A class that stores a single frequency-dependent directivity sample for a 3D normalized direction.
			class Sample
			{
				public:
					
					/// Create a new directivity sample for the given 3D normalized direction.
					GSOUND_INLINE Sample( const Vector3f& newDirection, const FrequencyResponse& newResponse )
						:	direction( newDirection ),
							response( newResponse )
					{
					}
					
					
					/// The normalized 3D cartesian direction for this directivity sample.
					Vector3f direction;
					
					
					/// The frequency response for this directivity sample.
					FrequencyResponse response;
					
					
			};
			
			
			friend class internal::SoundBandDirectivity;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of the directivity samples for this directivity pattern.
			ArrayList<Sample> samples;
			
			
			/// A 3x3 orthonormal rotation matrix indicating a rotation to apply to the directivity samples.
			Matrix3f orientation;
			
			
			/// The name of this sound directivity.
			UTF8String name;
			
			
			/// An integer time stamp that is incremented each time the directivity is changed.
			Index timeStamp;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_DIRECTIVITY_H
