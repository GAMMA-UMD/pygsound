/*
 * Project:     GSound

 * 
 * File:        gsound/gsSoundObject.h
 * Contents:    gsound::SoundObject class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_OBJECT_H
#define INCLUDE_GSOUND_SOUND_OBJECT_H


#include "gsConfig.h"


#include "gsSoundRay.h"
#include "gsSoundMesh.h"
#include "gsSoundObjectFlags.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
//********************************************************************************
//********************************************************************************
/// A class that is used to represent an instanced piece of scene geometry in a sound scene.
/**
  * A sound object has a rigid transform which is used to dynamically transform a SoundMesh
  * in world space. A sound object can have a mesh that can be shared among multiple
  * sound objects to allow instancing of geometry.
  */
class SoundObject
{
	public:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Constructors
			
			
			
			
			/// Create a sound object with the identity transform and no mesh.
			SoundObject();
			
			
			
			
			/// Create a sound object with the specified mesh and identity transform.
			SoundObject( SoundMesh* newMesh );
			
			
			
			
			/// Create a sound object with the specified mesh and transform.
			SoundObject( SoundMesh* newMesh, const Transform3f& newTransform );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Destructor
			
			
			
			
			/// Destroy this sound object, releasing its handle to the mesh.
			~SoundObject();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Mesh Accessor Method
			
			
			
			
			/// Return a pointer to the mesh that this sound object should use as its representation.
			/**
			  * The mesh is used during sound propagation as a representation of the
			  * object surfaces in the scene.
			  *
			  * A mesh can be shared among many objects. The user is responsible for
			  * destructing the mesh when it is not used by any objects, the object
			  * does not free the mesh when it is destroyed.
			  */
			GSOUND_INLINE SoundMesh* getMesh()
			{
				return mesh;
			}
			
			
			
			
			/// Return a pointer to the mesh that this sound object should use as its representation.
			/**
			  * The mesh is used during sound propagation as a representation of the
			  * object surfaces in the scene.
			  *
			  * A mesh can be shared among many objects. The user is responsible for
			  * destructing the mesh when it is not used by any objects, the object
			  * does not free the mesh when it is destroyed.
			  */
			GSOUND_INLINE const SoundMesh* getMesh() const
			{
				return mesh;
			}
			
			
			
			
			/// Set a pointer to the mesh that this sound object should use as its representation.
			/**
			  * The mesh is used during sound propagation as a representation of the
			  * object surfaces in the scene.
			  *
			  * A mesh can be shared among many objects. The user is responsible for
			  * destructing the mesh when it is not used by any objects, the object
			  * does not free the mesh when it is destroyed.
			  */
			void setMesh( SoundMesh* newMesh );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Transform Accessor Methods
			
			
			
			
			/// Get the rigid transform of this object.
			GSOUND_INLINE const Transform3f& getTransform() const
			{
				return transform;
			}
			
			
			
			
			/// Set the rigid transform of this object.
			void setTransform( const Transform3f& newTransform );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Position Accessor Methods
			
			
			
			
			/// Return the position of this object in world space.
			GSOUND_INLINE const Vector3f& getPosition() const
			{
				return transform.position;
			}
			
			
			
			
			/// Set the position of this object in world space.
			void setPosition( const Vector3f& newPosition );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Orientation Accessor Methods
			
			
			
			
			/// Return a 3x3 rotation matrix transforming from local to world coordinates for this object.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix in a right-handed coordinate system.
			  */
			GSOUND_INLINE const Matrix3f& getOrientation() const
			{
				return transform.orientation;
			}
			
			
			
			
			/// Set the orientation of this sound object in 3D space.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix using a right-handed coordinate system.
			  * The new orientation is automatically orthonormalized using Graham-Schmit
			  * orthonormalization. Use the setOrientationRaw() method to set the
			  * matrix directly and avoid the time spent in this operation if you
			  * are sure that your matrix will be orthonormal.
			  */
			void setOrientation( const Matrix3f& newOrientation );
			
			
			
			
			/// Set a 3x3 rotation matrix transforming from local to world coordinates for this mesh.
			/**
			  * The orientation is represented by a 3x3 orthonormal rotation
			  * matrix using a right-handed coordinate system. This method avoids
			  * the cost of the setOrientation() method by directly setting the matrix,
			  * but should be used only if you are sure that the new orientation matrix
			  * is orthonormal.
			  */
			GSOUND_INLINE void setOrientationRaw( const Matrix3f& newOrientation )
			{
				transform.orientation = newOrientation;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Scale Accessor Methods
			
			
			
			
			/// Return the scale of this object.
			GSOUND_INLINE Vector3f getScale() const
			{
				return transform.scale;
			}
			
			
			
			
			/// Set the scale of this object.
			void setScale( const Vector3f& newScale );
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Velocity Accessor Methods
			
			
			
			
			/// Return the velocity of this object in world space.
			GSOUND_INLINE const Vector3f& getVelocity() const
			{
				return velocity;
			}
			
			
			
			
			/// Set the velocity of this object in world space.
			GSOUND_INLINE void setVelocity( const Vector3f& newVelocity )
			{
				velocity = newVelocity;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Bounding Sphere Accessor Method
			
			
			
			
			/// Return a reference to the bounding sphere of this sound object in world space.
			GSOUND_INLINE const Sphere3f& getBoundingSphere() const
			{
				return worldSpaceBoundingSphere;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Flags Accessor Methods
			
			
			
			
			/// Return a reference to an object which contains boolean parameters of the sound object.
			GSOUND_INLINE SoundObjectFlags& getFlags()
			{
				return flags;
			}
			
			
			
			
			/// Return an object which contains boolean parameters of the sound object.
			GSOUND_INLINE const SoundObjectFlags& getFlags() const
			{
				return flags;
			}
			
			
			
			
			/// Set an object which contains boolean parameters of the sound object.
			GSOUND_INLINE void setFlags( const SoundObjectFlags& newFlags )
			{
				flags = newFlags;
			}
			
			
			
			
			/// Return whether or not the specified boolan flag is set for this sound object.
			GSOUND_INLINE Bool flagIsSet( SoundObjectFlags::Flag flag ) const
			{
				return flags.isSet( flag );
			}
			
			
			
			
			/// Set whether or not the specified boolan flag is set for this sound object.
			GSOUND_INLINE void setFlag( SoundObjectFlags::Flag flag, Bool newIsSet = true )
			{
				flags.set( flag, newIsSet );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Is Enabled Accessor Methods
			
			
			
			
			/// Return whether or not this object is enabled for sound propagation and rendering.
			/**
			  * Objects are enabled by default but can be disabled if no audio is being
			  * played for a object or if a object is not needed.
			  * This can increase the performance in scenes with large
			  * numbers of object that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE Bool getIsEnabled() const
			{
				return flags.isSet( SoundObjectFlags::ENABLED );
			}
			
			
			
			
			/// Set whether or not this object should be enabled for sound propagation and rendering.
			/**
			  * Objects are enabled by default but can be disabled if no audio is being
			  * played for a object or if a object is not needed.
			  * This can increase the performance in scenes with large
			  * numbers of object that might not all be active at any given time.
			  */
			GSOUND_FORCE_INLINE void setIsEnabled( Bool newIsEnabled )
			{
				flags.set( SoundObjectFlags::ENABLED, newIsEnabled );
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			
			
			/// Return an opaque pointer to user-defined data for this sound object.
			/**
			  * The object does not own the pointer to the user data. The user should
			  * manage the lifetime of the user data object.
			  */
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			
			
			/// Set an opaque pointer to user-defined data for this sound object.
			/**
			  * The object does not own the pointer to the user data. The user should
			  * manage the lifetime of the user data object.
			  */
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			
			
			/// Trace a ray through this object and compute the closest intersection.
			GSOUND_FORCE_INLINE void intersectRay( SoundRay& ray ) const
			{
				// Save the world-space origin and direction.
				om::math::SIMDFloat4 worldOrigin = ray.origin;
				om::math::SIMDFloat4 worldDirection = ray.direction;
				om::bvh::PrimitiveIndex worldPrimitive = ray.primitive;
				Float32 worldTMin = ray.tMin;
				Float32 worldTMax = ray.tMax;
				
				// Transform into object-local space.
				ray.origin = transform.transformToLocal( (Vector3f)ray.origin );
				ray.direction = transform.rotateToLocal( (Vector3f)ray.direction );
				ray.tMin = transform.transformToLocal( ray.tMin ).getMin();
				ray.tMax = transform.transformToLocal( ray.tMax ).getMax();
				ray.primitive = BVHGeometry::INVALID_PRIMITIVE;
				
				// Intersect the ray with the mesh.
				mesh->getBVH()->intersectRay( ray );
				
				if ( ray.hitValid() )
				{
					// Compute the intersection point in world space.
					om::math::SIMDFloat4 worldIntersection = transform.transformToWorld( (Vector3f)ray.getHitPoint() );
					
					// Compute the distance along the ray in the parent coordinate frame.
					Float32 worldDistance = math::dot( worldIntersection - worldOrigin, worldDirection )[0];
					
					if ( worldDistance < worldTMax )
					{
						// There was a valid intersection.
						ray.tMax = worldDistance;
						ray.normal = transform.rotateToWorld( (Vector3f)ray.normal );
						ray.object = (SoundObject*)this;
						ray.triangle = mesh->triangles->getPointer() + ray.primitive;
					}
					else
					{
						ray.tMax = worldTMax;
						ray.primitive = worldPrimitive;
					}
				}
				else
				{
					ray.tMax = worldTMax;
					ray.primitive = worldPrimitive;
				}
				
				// Restore the world-space ray data.
				ray.origin = worldOrigin;
				ray.direction = worldDirection;
				ray.tMin = worldTMin;
			}
			
			
			
			
	private:
		
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Helper Methods
			
			
			
			
			/// Update the world-space bounding sphere for this object.
			GSOUND_FORCE_INLINE void updateWorldSpaceBoundingSphere();
			
			
			
			
		//********************************************************************************
		//********************************************************************************
		//********************************************************************************
		//******	Private Data Members
			
			
			
			
			/// An object containing boolean configuration info for this sound object.
			SoundObjectFlags flags;
			
			
			
			
			/// The transform for this sound object from local to world space.
			Transform3f transform;
			
			
			
			
			/// The linear velocity of this sound object in world space.
			Vector3f velocity;
			
			
			
			
			/// The bounding sphere of this sound object in world space.
			Sphere3f worldSpaceBoundingSphere;
			
			
			
			
			/// A pointer to the mesh of this sound object.
			/**
			  * The mesh is used during sound propagation as a representation of the
			  * object surfaces in the scene.
			  *
			  * A mesh can be shared among many objects. The user is responsible for
			  * destructing the mesh when it is not used by any objects, the object
			  * does not free the mesh when it is destroyed.
			  */
			SoundMesh* mesh;
			
			
			
			
			/// An opaque pointer to user-defined data for this sound object.
			void* userData;
			
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_OBJECT_H
