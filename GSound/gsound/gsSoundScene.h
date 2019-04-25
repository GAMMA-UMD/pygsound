/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundScene.h
 * Contents:    gsound::SoundScene class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SCENE_H
#define INCLUDE_GSOUND_SOUND_SCENE_H


#include "gsConfig.h"


#include "gsSoundSource.h"
#include "gsSoundListener.h"
#include "gsSoundObject.h"
#include "gsSoundMedium.h"
#include "gsSoundRay.h"
#include "internal/gsObjectSpaceTriangle.h"
#include "internal/gsSoundSourceClusterer.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a scene where sound propagation is performed.
/**
  * A SoundScene object contains a list of pointers to SoundSource objects
  * that describe locations in space that produce sound. A SoundScene also
  * contains a list of pointers to SoundObject objects that represent
  * geometry in the scene which can obstruct the propagation of sound. The listeners
  * in a scene determine the locations where sound is heard in a scene.
  *
  * In addition, a SoundScene allows one to specify the propagation medium for the
  * scene, in order to allow different types of air propagation.
  */
class SoundScene
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empty sound scene with no objects, sources, or listeners.
			SoundScene();
			
			
			/// Create a copy of another scene with the same contents.
			SoundScene( const SoundScene& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound scene, releasing all internal resources.
			~SoundScene();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another scene to this one.
			SoundScene& operator = ( const SoundScene& other );
			
			
		//********************************************************************************
		//******	Sound Source Accessor Methods
			
			
			/// Return the number of sources in this sound scene.
			GSOUND_FORCE_INLINE Size getSourceCount() const
			{
				return sources.getSize();
			}
			
			
			/// Return a pointer to the source with the specified index.
			/**
			  * If the given index is greater than or equal to the number of
			  * sources in the scene, a NULL pointer is returned. Otherwise,
			  * a pointer to the source at the specified index in an internal
			  * array is returned.
			  * 
			  * @param sourceIndex - the index of the source to be queried.
			  * @return a pointer to the source at the given index.
			  */
			GSOUND_INLINE SoundSource* getSource( Index sourceIndex ) const
			{
				return sourceIndex < sources.getSize() ? sources[sourceIndex] : NULL;
			}
			
			
			/// Add a new source to this sound scene.
			/**
			  * If the new source is NULL, the method has no effect and returns FALSE.
			  * Otherwise, the new source is added and TRUE is returned.
			  * 
			  * @param newSource - a source to be added to the sound scene.
			  * @return whether or not the source was successfully added.
			  */
			Bool addSource( SoundSource* newSource );
			
			
			/// Remove the specified source from this sound scene.
			/**
			  * This method performs a linear search for the specified source
			  * in the scene and removes it if it is found. A value of TRUE is
			  * returned if the source was found and removed. Otherwise, FALSE
			  * is returned.
			  * 
			  * @param source - a source to remove from the sound scene.
			  * @return whether or not the source was successfully removed.
			  */
			Bool removeSource( SoundSource* source );
			
			
			/// Remove all sources from this sound scene.
			void clearSources();
			
			
		//********************************************************************************
		//******	Sound Listener Accessor Methods
			
			
			/// Return the number of listeners in this sound scene.
			GSOUND_FORCE_INLINE Size getListenerCount() const
			{
				return listeners.getSize();
			}
			
			
			/// Return a pointer to the listener with the specified index.
			/**
			  * If the given index is greater than or equal to the number of
			  * listeners in the scene, a NULL pointer is returned. Otherwise,
			  * a pointer to the listener at the specified index in an internal
			  * array is returned.
			  * 
			  * @param listenerIndex - the index of the listener to be queried.
			  * @return a pointer to the listener at the given index.
			  */
			GSOUND_INLINE SoundListener* getListener( Index listenerIndex ) const
			{
				return listenerIndex < listeners.getSize() ? listeners[listenerIndex] : NULL;
			}
			
			
			/// Add a new listener to this sound scene.
			/**
			  * If the new listener is NULL, the method has no effect and returns FALSE.
			  * Otherwise, the new listener is added and TRUE is returned.
			  * 
			  * @param newListener - a listener to be added to the sound scene.
			  * @return whether or not the listener was successfully added.
			  */
			Bool addListener( SoundListener* newListener );
			
			
			/// Remove the specified listener from this sound scene.
			/**
			  * This method performs a linear search for the specified listener
			  * in the scene and removes it if it is found. A value of TRUE is
			  * returned if the listener was found and removed. Otherwise, FALSE
			  * is returned.
			  * 
			  * @param listener - a listener to remove from the sound scene.
			  * @return whether or not the listener was successfully removed.
			  */
			Bool removeListener( SoundListener* listener );
			
			
			/// Remove all listeners from this sound scene.
			void clearListeners();
			
			
		//********************************************************************************
		//******	Sound Source Cluster Accessor Methods
			
			
			/// Return a pointer to the source cluster with the specified index in this scene.
			GSOUND_INLINE const internal::SoundSourceCluster* getSourceCluster( Index sourceClusterIndex ) const
			{
				return sourceClusterIndex < sourceClusterer.getClusterCount() ?
							sourceClusterer.getCluster(sourceClusterIndex) : NULL;
			}
			
			
			/// Return the number of source clusters in this sound scene.
			GSOUND_FORCE_INLINE Size getSourceClusterCount() const
			{
				return sourceClusterer.getClusterCount();
			}
			
			
			/// Update the sound source clusters in this scene for the specified listener and clustering parameters.
			GSOUND_INLINE void updateSourceClusters( const SoundListener& listener, Real innerClusteringAngle, Real outerClusteringAngle ) const
			{
				sourceClusterer.updateClusters( listener, *this, innerClusteringAngle, outerClusteringAngle );
			}
			
			
		//********************************************************************************
		//******	Sound Object Accessor Methods
			
			
			/// Return the number of objects in this sound scene.
			GSOUND_FORCE_INLINE Size getObjectCount() const
			{
				return objects.getSize();
			}
			
			
			/// Return a pointer to the object with the specified index.
			/**
			  * If the given index is greater than or equal to the number of
			  * objects in the scene, a NULL pointer is returned. Otherwise,
			  * a pointer to the object at the specified index in an internal
			  * array is returned.
			  * 
			  * @param objectIndex - the index of the object to be queried.
			  * @return a pointer to the object at the given index.
			  */
			GSOUND_INLINE SoundObject* getObject( Index objectIndex ) const
			{
				return objectIndex < objects.getSize() ? objects[objectIndex] : NULL;
			}
			
			
			/// Add a new object to this sound scene.
			/**
			  * If the new object is NULL, the method has no effect. The
			  * object bounding volume heirarchy for this scene is then rebuilt
			  * with the new object, incurring the cost of this operation.
			  * 
			  * @param newObject - an object to be added to the sound scene.
			  */
			Bool addObject( SoundObject* newObject );
			
			
			
			/// Remove the specified object from this sound scene.
			/**
			  * This method performs a linear search for the specified object
			  * in the scene and removes it if it is found. A value of TRUE is
			  * returned if the object was found and removed. Otherwise, FALSE
			  * is returned. If the object was removed successfully, the object
			  * bounding volume hierarchy is rebuilt, incurring the cost of this
			  * operation.
			  * 
			  * @param object - an object to remove from the sound scene.
			  * @return whether or not the object was successfully removed.
			  */
			Bool removeObject( SoundObject* object );
			
			
			/// Remove all objects from this sound scene.
			/**
			  * This method removes all objects in this scene in constant time
			  * and then deallocates any bounding volume hierarchy for the objects
			  * that previously existed.
			  */
			void clearObjects();
			
			
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			/// Trace a ray through the scene and return the first intersected triangle.
			/**
			  * The method returns whether or not the ray hit anything.
			  */
			GSOUND_FORCE_INLINE Bool intersectRay( const Ray3f& ray, Float maxDistance,
											Float& intersection, internal::ObjectSpaceTriangle& triangle ) const
			{
				SoundRay soundRay( ray, 0.0f, maxDistance );
				
				this->intersectRay( soundRay );
				
				if ( soundRay.hitValid() )
				{
					intersection = soundRay.tMax;
					triangle = internal::ObjectSpaceTriangle( soundRay.triangle, soundRay.object );
					
					return true;
				}
				
				return false;
			}
			
			
			/// Trace a ray through the scene and return the first intersected triangle.
			/**
			  * The method returns whether or not the ray hit anything.
			  */
			GSOUND_FORCE_INLINE Bool intersectRay( const Ray3f& ray, Float maxDistance ) const
			{
				SoundRay soundRay( ray, 0.0f, maxDistance );
				
				this->intersectRay( soundRay );
				
				return soundRay.hitValid();
			}
			
			
			/// Trace a ray through the scene and return the first intersected triangle.
			/**
			  * The method returns whether or not the ray hit anything.
			  */
			GSOUND_FORCE_INLINE Bool intersectRay( SoundRay& ray ) const;
			
			
			/// Trace a ray through the scene to the specified maximum distance, returning TRUE if it hits anything.
			GSOUND_FORCE_INLINE Bool testRay( SoundRay& ray ) const
			{
				return this->intersectRay( ray );
			}
			
			
		//********************************************************************************
		//******	Sound Medium Accessor Methods
			
			
			/// Return a reference to the sound propagation medium which is used for this scene.
			GSOUND_FORCE_INLINE SoundMedium& getMedium()
			{
				return medium;
			}
			
			
			/// Return a const reference to the sound propagation medium which is used for this scene.
			GSOUND_FORCE_INLINE const SoundMedium& getMedium() const
			{
				return medium;
			}
			
			
			/// Set the sound propagation medium which is used for this scene.
			GSOUND_INLINE void setMedium( const SoundMedium& newMedium )
			{
				medium = newMedium;
			}
			
			
		//********************************************************************************
		//******	Reverb Time Accessor Method
			
			
			/// Return the global reverb time for this scene.
			GSOUND_INLINE Real getReverbTime() const
			{
				return reverbTime;
			}
			
			
			/// Set the global reverb time for this scene.
			GSOUND_INLINE void setReverbTime( Real newReverbTime )
			{
				reverbTime = newReverbTime;
			}
			
			
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			/// Return an opaque pointer to user-defined data for this SoundScene.
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			/// Set an opaque pointer to user-defined data for this SoundScene.
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
		//********************************************************************************
		//******	BVH Methods
			
			
			/// Rebuild the bounding volume hierarchy of objects in the scene.
			void rebuildBVH() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that provides scene geometry to a BVH.
			class SceneBVH;
			
			
		//********************************************************************************
		//******	Private Friend Classes
			
			
			/// Make the sound propagator class a friend so that it can update the scene's internal data.
			friend class SoundPropagator;
			
			
			/// The number of objects at which the scene will use a BVH for ray tracing among objects.
			static const Size OBJECT_COUNT_THRESHOLD = 8;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of pointers to all sources in this scene.
			ArrayList<SoundSource*> sources;
			
			
			/// A list of pointers to all listeners in this scene.
			ArrayList<SoundListener*> listeners;
			
			
			/// A list of pointers to the objects in this scene.
			ArrayList<SoundObject*> objects;
			
			
			/// A pointer to an object that manages the BVH for this scene.
			mutable SceneBVH* bvh;
			
			
			/// An object which maintains a hierarchy of the sources in the scene.
			mutable internal::SoundSourceClusterer sourceClusterer;
			
			
			/// The sound propagation medium which is used for this scene.
			SoundMedium medium;
			
			
			/// The global reverb time (-60dB) in seconds for the scene.
			Real reverbTime;
			
			
			/// An opaque pointer to user-defined data for this SoundScene.
			void* userData;
			
			
			
};


//##########################################################################################
//##########################################################################################
//############		
//############		Scene BVH Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundScene:: SceneBVH : public BVHGeometry
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new scene geometry with no child BVH's.
			GSOUND_INLINE SceneBVH( const SoundScene* newScene )
				:	scene( newScene )
			{
				bvh.setGeometry( this );
			}
			
			
		//********************************************************************************
		//******	Primitive-BVH Interface Methods
			
			
			/// Return the number of BVH's contained in this scene geometry.
			virtual om::bvh::PrimitiveIndex getPrimitiveCount() const
			{
				return (om::bvh::PrimitiveCount)scene->objects.getSize();
			}
			
			
			/// Return an axis-aligned bounding box for the BVH with the specified index.
			virtual AABB3f getPrimitiveAABB( om::bvh::PrimitiveIndex primitiveIndex ) const
			{
				SoundObject* object = scene->objects[primitiveIndex];
				
				return object->getTransform().transformToWorld( object->getMesh()->getBoundingBox() );
			}
			
			
			/// Return an axis-aligned bounding box for the BVH with the specified index.
			virtual Sphere3f getPrimitiveBoundingSphere( om::bvh::PrimitiveIndex primitiveIndex ) const
			{
				return scene->objects[primitiveIndex]->getBoundingSphere();
			}
			
			
			/// Return whether or not the primitive with the specified index is intersected by the specified ray.
			virtual void intersectRay( om::bvh::PrimitiveIndex primitiveIndex, BVHRay& ray ) const
			{
				SoundObject* object = scene->objects[primitiveIndex];
				object->intersectRay( *((SoundRay*)&ray) );
			}
			
			
			/// Return whether or not the primitives with the specified indices are intersected by the specified ray.
			virtual void intersectRay( const om::bvh::PrimitiveIndex* primitiveIndices,
										om::bvh::PrimitiveCount numPrimitives, BVHRay& ray ) const
			{
				for ( om::bvh::PrimitiveCount i = 0; i < numPrimitives; i++ )
				{
					SoundObject* object = scene->objects[primitiveIndices[i]];
					object->intersectRay( *((SoundRay*)&ray) );
				}
			}
				
				
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The BVH that holds the scene geometry.
			om::bvh::AABBTree4 bvh;
			
			
			/// A pointer to the scene that this geometry is in.
			const SoundScene* scene;
			
			
};




Bool SoundScene:: intersectRay( SoundRay& ray ) const
{
	// Do simple intersection with each object and pick the closest one.
	// This will be faster than a BVH for the usual number of objects...
	const Size numObjects = objects.getSize();
	
	if ( numObjects < OBJECT_COUNT_THRESHOLD )
	{
		for ( Index i = 0; i < numObjects; i++ )
		{
			SoundObject* object = objects[i];
			
			if ( Ray3f( ray.origin, ray.direction ).intersectsSphere( object->getBoundingSphere() ) )
				object->intersectRay( ray );
		}
		
		return ray.hitValid();
	}
	else
	{
		bvh->bvh.intersectRay( ray );
		
		return ray.hitValid();
	}
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SCENE_H
