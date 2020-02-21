/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundDetector.h
 * Contents:    gsound::SoundDetector class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_DETECTOR_H
#define INCLUDE_GSOUND_SOUND_DETECTOR_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a sound detector in a 3D sound propagation system.
/**
  * A sound detector can represent either a sound source or sound listener,
  * and contains common attributes for both. This allows sources and listeners
  * to be treated identically, simplifying the implementation of propagation algorithms.
  *
  * A detector is spherical and its radius determines the 'size' of a source or listener
  * in the environment. A larger radius means that a source radiates the sound over a larger
  * area. The radius should be non-zero, and it is best to choose a radius
  * that corresponds to the size of the source or listener's visual representation.
  * With the usual ray sampling strategy, the sound energy is normalized so that
  * the radius does not affect the resulting loudness of the sound source or listener.
  */
class SoundDetector
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default detector of radius 1.0 centered at the origin.
			SoundDetector();
			
			
			/// Create a detector with the specified position and radius.
			SoundDetector( const Vector3f& newPosition, Real newRadius );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this detector, releasing all internal resources.
			virtual ~SoundDetector();
			
			
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			/// Return the position of this detector in 3D world space.
			GSOUND_FORCE_INLINE const Vector3f& getPosition() const
			{
				return transform.position;
			}
			
			
			/// Set the position of this detector in 3D world space.
			GSOUND_FORCE_INLINE void setPosition( const Vector3f& newPosition )
			{
				transform.position = newPosition;
			}
			
			
		//********************************************************************************
		//******	Orientation Accessor Methods
			
			
			/// Return a 3x3 rotation matrix transforming from local to world coordinates for this detector.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix in a right-handed coordinate system.
			  */
			GSOUND_FORCE_INLINE const Matrix3f& getOrientation() const
			{
				return transform.orientation;
			}
			
			
			/// Set a 3x3 rotation matrix transforming from local to world coordinates for this detector.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix using a right-handed coordinate system.
			  * The new orientation is automatically orthonormalized using Graham-Schmit
			  * orthonormalization. Use the setOrientationRaw() method to set the
			  * matrix directly and avoid the time spent in this operation if you
			  * are sure that your matrix will be orthonormal.
			  */
			void setOrientation( const Matrix3f& newOrientation );
			
			
			/// Set a 3x3 rotation matrix transforming from local to world coordinates for this detector.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix using a right-handed coordinate system. This method stores the given
			  * matrix directly as the detector's orientation and should be used only if
			  * the new orientation matrix is guaranteed to be orthonormal.
			  */
			GSOUND_INLINE void setOrientationRaw( const Matrix3f& newOrientation )
			{
				transform.orientation = newOrientation;
			}
			
			
			/// Return the view direction of the detector.
			GSOUND_INLINE Vector3f getViewDirection() const
			{
				return -transform.orientation.z;
			}
			
			
			/// Return the up direction of the detector.
			GSOUND_FORCE_INLINE const Vector3f& getUpDirection() const
			{
				return transform.orientation.y;
			}
			
			
			/// Return the left direction of the detector.
			GSOUND_INLINE Vector3f getLeftDirection() const
			{
				return -transform.orientation.x;
			}
			
			
			/// Return the right direction of the detector.
			GSOUND_FORCE_INLINE const Vector3f& getRightDirection() const
			{
				return transform.orientation.x;
			}
			
			
		//********************************************************************************
		//******	Transform Accessor Methods
			
			
			/// Return a reference to the local-to-world transform for this detector.
			GSOUND_FORCE_INLINE const Transform3f& getTransform() const
			{
				return transform;
			}
			
			
			/// Set the local-to-world transform for this detector, ignoring any scaling.
			void setTransform( const Transform3f& newTransform );
			
			
		//********************************************************************************
		//******	Velocity Accessor Methods
			
			
			/// Return the linear velocity of this detector in world space.
			GSOUND_FORCE_INLINE const Vector3f& getVelocity() const
			{
				return velocity;
			}
			
			
			/// Set the linear velocity of this detector in world space.
			GSOUND_FORCE_INLINE void setVelocity( const Vector3f& newVelocity )
			{
				velocity = newVelocity;
			}
			
			
		//********************************************************************************
		//******	Radius Accessor Methods
			
			
			/// Return the bounding sphere radius for this detector.
			GSOUND_FORCE_INLINE Real getRadius() const
			{
				return radius;
			}
			
			
			/// Set the bounding sphere radius for this detector.
			/**
			  * The new radius is clamped to be in the range [0,+inf].
			  */
			GSOUND_FORCE_INLINE void setRadius( Real newRadius )
			{
				radius = math::max( newRadius, Real(0) );
			}
			
			
		//********************************************************************************
		//******	Bounding Sphere Accessor Methods
			
			
			/// Return an object representing the bounding sphere of this detector in world space.
			GSOUND_FORCE_INLINE Sphere3f getBoundingSphere() const
			{
				return Sphere3f( transform.position, radius );
			}
			
			
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			/// Return an opaque pointer to user-defined data for this detector.
			/**
			  * The detector does not own the pointer to the user data. The user should
			  * manage the lifetime of the user data object.
			  */
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			/// Set an opaque pointer to user-defined data for this detector.
			/**
			  * The detector does not own the pointer to the user data. The user should
			  * manage the lifetime of the user data object.
			  */
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			/// Return an integer identifier for the sound detector.
			/**
			  * This identifier is not guaranteed to be unique among sound detectors
			  * but will be unique in most cases.
			  */
			GSOUND_FORCE_INLINE Hash getHashCode() const
			{
				return hashCode;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The local-to-world transform for this detector.
			Transform3f transform;
			
			
			/// The linear velocity of the detector in world spacce.
			Vector3f velocity;
			
			
			/// The radius of this detector's bounding sphere.
			Real radius;
			
			
			/// A pseudo-unique identifier for the sound detector generated randomly at construction.
			Hash hashCode;
			
			
			/// An opaque pointer to user-defined data for this detector.
			void* userData;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_DETECTOR_H
