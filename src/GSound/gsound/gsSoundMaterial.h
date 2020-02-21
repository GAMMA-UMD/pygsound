/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMaterial.h
 * Contents:    gsound::SoundMaterial class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MATERIAL_H
#define INCLUDE_GSOUND_SOUND_MATERIAL_H


#include "gsConfig.h"


#include "gsFrequencyResponse.h"
#include "gsFrequencyBandResponse.h"
#include "gsSoundMedium.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies the acoustic properties of a surface in the scene.
/**
  * These properties determine how sound interacts with the material: how much
  * sound is reflected, scattered, and transmitted through the material across
  * the frequency range. The parameters here are given for sound intensity,
  * not sound pressure, and so adjustments may have to be made to standard
  * material measurements to convert to intensity ratios instead of pressure ratios.
  *
  * A material's reflectivity specifies how much of each frequency is reflected
  * (between 0 and 1) when sound hits a surface. This parameter affects both specular
  * and diffuse reflected sound.
  *
  * A material's scattering specifies how much of each frequency's reflected
  * sound is scattered (between 0 and 1) when sound hits a surface. A value of 0
  * indicates no diffuse reflection while 1 indicates a perfectly diffuse surface.
  *
  * A material's transmission specifies how much of the sound that is not
  * reflected that will be transmitted through the material when sound
  * hits a surface. The rest of the energy is absorbed by the material and
  * converted into heat, etc.
  */
class SoundMaterial
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a default material with reflectivity = 0.95, scattering = 0.5, transmission = 0.
			SoundMaterial();
			
			
			/// Create a sound material with the specified reflection, scattering, and transmission.
			SoundMaterial( const FrequencyResponse& newReflectivity,
							const FrequencyResponse& newScattering,
							const FrequencyResponse& newTransmission );
			
			
			/// Create a sound material with the specified reflection, scattering, and transmission.
			SoundMaterial( const FrequencyResponse& newReflectivity,
							const FrequencyResponse& newScattering,
							const FrequencyResponse& newTransmission, const UTF8String& newName );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound material, releasing all internal resources.
			~SoundMaterial();
			
			
		//********************************************************************************
		//******	Reflection Attenuation Accessor Methods
			
			
			/// Return a reference to the reflectivity of this material.
			GSOUND_INLINE FrequencyResponse& getReflectivity()
			{
				return reflectivity;
			}
			
			
			/// Return a const reference to the reflectivity of this material.
			GSOUND_INLINE const FrequencyResponse& getReflectivity() const
			{
				return reflectivity;
			}
			
			
			/// Set the reflectivity of this material.
			GSOUND_INLINE void setReflectivity( const FrequencyResponse& newReflectivity )
			{
				reflectivity = newReflectivity;
			}
			
			
			/// Set the pressure absorption coefficients of this material, converting it to intensity reflectivity.
			/**
			  * This method sets the reflectivity of the material based on the specified
			  * pressure absorption coefficients. The absorption coefficients
			  * are converted by the transformation sqrt(1 - a) to get the reflectivity
			  * coefficient needed by the sound propagation system.
			  *
			  * Since most standard material measurments are pressure absorption, this
			  * method is provided to easily import absorption coefficients into the
			  * propagation system.
			  */
			void setAbsorption( const FrequencyResponse& newAbsorption );
			
			
		//********************************************************************************
		//******	Scattering Accessor Methods
			
			
			/// Return a reference to the scattering coefficients of this material.
			GSOUND_INLINE FrequencyResponse& getScattering()
			{
				return scattering;
			}
			
			
			/// Return a const reference to the scattering coefficients of this material.
			GSOUND_INLINE const FrequencyResponse& getScattering() const
			{
				return scattering;
			}
			
			
			/// Set the scattering coefficients of this material.
			GSOUND_INLINE void setScattering( const FrequencyResponse& newScattering )
			{
				scattering = newScattering;
				averageScattering = scattering.getAverage();
			}
			
			
		//********************************************************************************
		//******	Absorption Attenuation Accessor Methods
			
			
			/// Return a reference to the transmission attenuation of this material.
			GSOUND_INLINE FrequencyResponse& getTransmission()
			{
				return transmission;
			}
			
			
			/// Return a const reference to the transmission attenuation of this material.
			GSOUND_INLINE const FrequencyResponse& getTransmission() const
			{
				return transmission;
			}
			
			
			/// Set the transmission attenuation of this material.
			GSOUND_INLINE void setTransmission( const FrequencyResponse& newTransmission )
			{
				transmission = newTransmission;
			}
			
			
		//********************************************************************************
		//******	Sound Medium Accessor Methods
			
			
			/// Return a reference to the sound propagation medium which is used for this material.
			GSOUND_FORCE_INLINE SoundMedium& getMedium()
			{
				return medium;
			}
			
			
			/// Return a const reference to the sound propagation medium which is used for this material.
			GSOUND_FORCE_INLINE const SoundMedium& getMedium() const
			{
				return medium;
			}
			
			
			/// Set the sound propagation medium which is used for this material.
			GSOUND_INLINE void setMedium( const SoundMedium& newMedium )
			{
				medium = newMedium;
			}
			
			
		//********************************************************************************
		//******	Color Accessor Method
			
			
			/// Return an RGBA color to use for this material.
			GSOUND_INLINE const Vector4f& getColor() const
			{
				return color;
			}
			
			
			/// Set an RGBA color to use for this material.
			GSOUND_INLINE void setColor( const Vector4f& newColor )
			{
				color = newColor;
			}
			
			
		//********************************************************************************
		//******	Name Accessor Methods
			
			
			/// Return the name of this sound material.
			GSOUND_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set the name of this sound material.
			GSOUND_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Ray Reflection Methods
			
			
			/// Specularly reflect the specified incoming ray direction with this material.
			GSOUND_FORCE_INLINE Vector3f getSpecularReflection( const Vector3f& direction, const Vector3f& normal ) const
			{
				return direction - Real(2)*normal*math::dot( direction, normal );
			}
			
			
			/// Diffusely reflect the specified incoming ray direction with this material.
			/**
			  * The material uses the diffuse part of the BRDF to generate a randomly reflected ray
			  * for the given direction and normal.
			  */
			GSOUND_FORCE_INLINE Vector3f getDiffuseReflection( const Vector3f& normal, math::Random<Real>& random ) const
			{
				return getLambertianDirectionInHemisphere( random, normal );
			}
			
			
			/// Return the probability that this material scatters a direction in the given reflection direction.
			GSOUND_FORCE_INLINE Real getDiffuseReflectionProbability( const Vector3f& normal, const Vector3f& reflection ) const
			{
				// Lambertian reflectance is proportional to the cosine of the angle between
				// the reflected vector and the normal.
				// PDF = cos(theta) / pi
				//return math::max( Real(2)*math::dot( normal, reflection ), Real(0) );
				return math::max( (Real(1) / math::pi<Real>())*math::dot( normal, reflection ), Real(0) );
			}
			
			
			/// Reflect the specified incoming ray direction with this material.
			/**
			  * The material uses its BRDF to generate a reflected ray
			  * for the given direction and normal.
			  */
			GSOUND_FORCE_INLINE Vector3f getReflection( const Vector3f& direction, const Vector3f& normal,
														math::Random<Real>& random ) const
			{
				if ( random.sample( Real(0), Real(1) ) < averageScattering )
					return getLambertianDirectionInHemisphere( random, normal );
				else
					return direction - Real(2)*normal*math::dot( direction, normal );
				/*
				Vector3f specularDirection = direction - Real(2)*normal*math::dot( direction, normal );
				Vector3f diffuseDirection = getLambertianDirectionInHemisphere( random, normal );
				
				return ((Real(1) - averageScattering)*specularDirection +
						averageScattering*diffuseDirection).normalize();*/
			}
			
			
			/// Set the frequency bands to use for the band responses for this material.
			void setFrequencyBands( const FrequencyBands& newBands ) const;
			
			
			/// Return a const reference to the reflectivity bands of this material.
			GSOUND_INLINE const FrequencyBandResponse& getReflectivityBands() const
			{
				return reflectivityBands;
			}
			
			
			/// Return a const reference to the scattering coefficients bands of this material.
			GSOUND_INLINE const FrequencyBandResponse& getScatteringBands() const
			{
				return scatteringBands;
			}
			
			
			/// Return the frequency-averaged broadband scattering coefficient of this material.
			GSOUND_INLINE Real getAverageScattering() const
			{
				return averageScattering;
			}
			
			
			/// Return a const reference to the transmission attenuation bands of this material.
			GSOUND_INLINE const FrequencyBandResponse& getTransmissionBands() const
			{
				return transmissionBands;
			}
			
			
		//********************************************************************************
		//******	Predefined Material Objects
			
			
			/// The default frequency-dependent reflectivity for a sound material.
			static const FrequencyResponse DEFAULT_REFLECTIVITY;
			
			
			/// The default frequency-dependent scattering for a sound material.
			static const FrequencyResponse DEFAULT_SCATTERING;
			
			
			/// The default frequency-dependent transmission for a sound material.
			static const FrequencyResponse DEFAULT_TRANSMISSION;
			
			
		//********************************************************************************
		//******	Predefined Material Objects
			
			
			/// A sound material for unpainted bricks.
			static const SoundMaterial BRICK;
			
			
			/// A sound material for painted bricks.
			static const SoundMaterial BRICK_PAINTED;
			
			
			/// A sound material for light carpet on a solid backing.
			static const SoundMaterial CARPET;
			
			
			/// A sound material for heavy carpet on a solid backing.
			static const SoundMaterial CARPET_HEAVY;
			
			
			/// A sound material for heavy carpet on a foam rubber padding.
			static const SoundMaterial CARPET_HEAVY_PADDED;
			
			
			/// A sound material for smooth concrete.
			static const SoundMaterial CONCRETE;
			
			
			/// A sound material for rough concrete.
			static const SoundMaterial CONCRETE_ROUGH;
			
			
			/// A sound material for unpainted concrete block.
			static const SoundMaterial CONCRETE_BLOCK;
			
			
			/// A sound material for painted concrete block.
			static const SoundMaterial CONCRETE_BLOCK_PAINTED;
			
			
			/// A sound material for ordinary glass windows.
			static const SoundMaterial GLASS;
			
			
			/// A sound material for heavy glass windows.
			static const SoundMaterial GLASS_HEAVY;
			
			
			/// A sound material for grass, 2" high.
			static const SoundMaterial GRASS;
			
			
			/// A sound material for gravel.
			static const SoundMaterial GRAVEL;
			
			
			/// A sound material for 1/2" gypsum board, nailed to 2x4s with air cavities.
			static const SoundMaterial GYPSUM_BOARD;
			
			
			/// A sound material for ceramic tiles.
			static const SoundMaterial CERAMIC_TILE;
			
			
			/// A sound material for plaster on bricks.
			static const SoundMaterial PLASTER_ON_BRICK;
			
			
			/// A sound material for plaster on concrete blocks.
			static const SoundMaterial PLASTER_ON_CONCRETE_BLOCK;
			
			
			/// A sound material for snow, fresh, 4" thick.
			static const SoundMaterial SNOW;
			
			
			/// A sound material for smooth steel.
			static const SoundMaterial STEEL;
			
			
			/// A sound material for a water surface.
			static const SoundMaterial WATER;
			
			
			/// A sound material for thin (1/4") wood panneling with an air space behind.
			static const SoundMaterial WOOD_THIN;
			
			
			/// A sound material for thick (1") wood panneling with an air space behind.
			static const SoundMaterial WOOD_THICK;
			
			
			/// A sound material for a wood floor.
			static const SoundMaterial WOOD_FLOOR;
			
			
			/// A sound material for wood on concrete.
			static const SoundMaterial WOOD_ON_CONRETE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Reflection Helper Methods
			
			
			/// Return a uniformly distributed random unit vector direction.
			GSOUND_FORCE_INLINE static Vector3f getRandomDirection( math::Random<Real>& variable )
			{
				const Real u1 = variable.sample( Real(-1), Real(1) );
				const Real u2 = variable.sample( Real(0), Real(1) );
				const Real r = math::sqrt( Real(1) - u1*u1 );
				const Real theta = Real(2)*math::pi<Real>()*u2;
				
				return Vector3f( r*math::cos( theta ), r*math::sin( theta ), u1 );
			}
			
			
			/// Return a uniformly distributed random unit vector direction which are a hemisphere defined by a plane normal.
			GSOUND_FORCE_INLINE static Vector3f getRandomDirectionInHemisphere( math::Random<Real>& variable,
																		const Vector3f& normal )
			{
				Vector3f randomDirection = getRandomDirection( variable );
				
				if ( math::dot( randomDirection, normal ) < Real(0) )
					return -randomDirection;
				else
					return randomDirection;
			}
			
			
			/// Return a lambertian distributed random unit vector direction which are a hemisphere defined by a plane normal.
			GSOUND_FORCE_INLINE static Vector3f getLambertianDirectionInHemisphere( math::Random<Real>& variable,
																				const Vector3f& normal )
			{
				const Real u1 = variable.sample( Real(0), Real(1) );
				const Real u2 = variable.sample( Real(0), Real(1) );
				const Real r = math::sqrt(u1);
				const Real theta = Real(2)*math::pi<Real>()*u2;

				const Real x = r * math::cos( theta );
				const Real y = r * math::sin( theta );
				
				Vector3f randomDirection( x, y, math::sqrt( math::max( Real(1) - u1, Real(0) )) );
				
				// Transform the ray distribution into the tangent space.
				return Matrix3f::planeBasis( normal )*randomDirection;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The attenuation coefficients per-band for sound reflected off of a surface with this material.
			mutable FrequencyBandResponse reflectivityBands;
			
			
			/// The scattering coefficients per-band for this material at each frequency.
			mutable FrequencyBandResponse scatteringBands;
			
			
			/// The attenuation coefficients per-band for the sound that is transmitted across a material boundary.
			mutable FrequencyBandResponse transmissionBands;
			
			
			/// The attenuation coefficients for sound reflected off of a surface with this material.
			/**
			  * When a sound bounces off of a surface, the final sound will be attenuated
			  * by this amount.
			  */
			FrequencyResponse reflectivity;
			
			
			/// The scattering coefficients for this material at each frequency.
			/**
			  * Each component of the response is a value in the range [0,1] which indicates
			  * the fraction of the incident sound that is scattered away from the specular
			  * direction for that frequency band.
			  */
			FrequencyResponse scattering;
			
			
			/// The attenuation coefficients for the sound that is transmitted across a material boundary.
			/**
			  * When sound hits a surface, the portion that is not reflected is either absorbed
			  * (disappated as heat) or transmitted through the material. Multiplying that sound
			  * by these coefficients gives the sound that is transmitted through the material.
			  */
			FrequencyResponse transmission;
			
			
			/// The propagation medium corresponding to sound traveling through this material.
			SoundMedium medium;
			
			
			/// The name of this sound material.
			UTF8String name;
			
			
			/// An RGBA color to use for this material.
			Vector4f color;
			
			
			/// The frequency-averaged scattering coefficient for this material, used to scatter rays.
			mutable Real averageScattering;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MATERIAL_H
