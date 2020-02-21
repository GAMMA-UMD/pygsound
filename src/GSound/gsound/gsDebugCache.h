/*
 * Project:     GSound
 * 
 * File:        gsound/gsDebugCache.h
 * Contents:    gsound::DebugCache class declaration
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

#ifndef INCLUDE_GSOUND_DEBUG_CACHE_H
#define INCLUDE_GSOUND_DEBUG_CACHE_H


#include "gsConfig.h"


#include "gsSoundTriangle.h"
#include "gsSoundListener.h"
#include "gsSoundSource.h"
#include "gsSoundMaterial.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class used to cache debug information so that it can be drawn by an external renderer.
/**
  * This class consists of a list of elements to be drawn for each type of sound
  * propagation debug information. In practice, one passes this class to a 
  * SoundPropagator object when performing sound propagation. The SoundPropagator
  * object then adds debugging information to the DebugCache which can
  * then be drawn later by an external renderer.
  *
  * The types of debugging information that a DebugCache contains are:
  * - Direct Paths: These are unoccluded propagation paths from sound sources to the listener.
  * - Transmission Paths: These are direct paths that passed through an object in the scene.
  * - Reflection Paths: These are paths that consist of one or more specular reflections through the scene.
  * - Diffraction Paths: These are propagation paths where sound has diffracted over an edge.
  * - Object Meshes: These are world-space indexes mesh representations of all scene objects.
  * - Diffraction Edges: These are edges shared by two triangles that have been marked as diffracting.
  * - Sound Sources: These are the sound sources for which sound propagation has been performed.
  * - Sound Listeners: These are the sound listeners with which sound proapgation has been performed.
  * - Sound Detectors: These are the sound detectors used for sound propagation.
  *
  * Each of these types of debugging information can be enabled or disabled
  * if only certain types are needed. This can also provide a performance boost by
  * reducing the work that the SoundPropagator must do in order to produce the debugging
  * information.
  */
class DebugCache
{
	public:
		
		//********************************************************************************
		//******	Public Class Declarations
			
			
			/// A class that contains the position of the listener and source of a direct propagation path.
			class DirectPath
			{
				public:
					
					GSOUND_INLINE DirectPath( const Vector3f& newListenerPosition, const Vector3f& newSourcePosition )
						:	listenerPosition( newListenerPosition ),
							sourcePosition( newSourcePosition )
					{
					}
					
					
					
					Vector3f listenerPosition;
					Vector3f sourcePosition;
					
			};
			
			
			/// A class containing a list of points which represent interactions along an indirect propagation path.
			class IndirectPath
			{
				public:
					
					GSOUND_INLINE IndirectPath( const ArrayList<Vector3f>& newPoints )
						:	points( newPoints )
					{
					}
					
					
					/// A list of points along an indirect path (including the source and listener endpoints).
					ArrayList<Vector3f> points;
					
					
			};
			
			
			/// A class that contains the two endpoints of a diffraction edge.
			class DiffractionEdge
			{
				public:
					
					GSOUND_INLINE DiffractionEdge( const Vector3f& newV1, const Vector3f& newV2 )
						:	v1( newV1 ),
							v2( newV2 )
					{
					}
					
					
					/// The first endpoint of a diffraction edge.
					Vector3f v1;
					
					
					/// The second endpoint of a diffraction edge.
					Vector3f v2;
					
					
			};
			
			
			/// A class containing a list of the vertices and indexed triangles of a single object in a scene.
			class ObjectMesh
			{
				public:
					
					GSOUND_INLINE ObjectMesh( const ArrayList<SoundVertex>& newVertices,
												const ArrayList<SoundTriangle>& newTriangles,
												const ArrayList<SoundMaterial>& newMaterials )
						:	vertices( newVertices ),
							triangles( newTriangles ),
							materials( newMaterials )
					{
					}
					
					
					/// A list of the vertices for this mesh in world space.
					ArrayList<SoundVertex> vertices;
					
					
					/// A list of indexed triangles for the vertices.
					ArrayList<SoundTriangle> triangles;
					
					
					/// A list of indexed triangles for the vertices.
					ArrayList<SoundMaterial> materials;
					
					
			};
			
			
		//********************************************************************************
		//******	Constructor
			
			
			/// Create an empty debug cache.
			DebugCache();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			~DebugCache();
			
			
		//********************************************************************************
		//******	Sound Listener Accessor Methods
			
			
			/// Add a sound listener to this debug cache.
			void addListener( const SoundListener* listener );
			
			
			/// Return a pointer to the internal array of sound sources.
			GSOUND_INLINE const SoundListener* const* getListeners() const
			{
				return listeners.getPointer();
			}
			
			
			/// Return the number of cached listeners.
			GSOUND_INLINE Size getListenerCount() const
			{
				return listeners.getSize();
			}
			
			
		//********************************************************************************
		//******	Sound Source Accessor Methods
			
			
			/// Add a sound source to this debug cache.
			void addSource( const SoundSource* source );
			
			
			/// Return a pointer to the internal array of sound sources.
			GSOUND_INLINE const SoundSource* const* getSources() const
			{
				return sources.getPointer();
			}
			
			
			/// Return the number of cached sources.
			GSOUND_INLINE Size getSourceCount() const
			{
				return sources.getSize();
			}
			
			
		//********************************************************************************
		//******	Sound Detector Accessor Methods
			
			
			/// Add a sound source to this debug cache.
			void addDetector( const SoundDetector* detector );
			
			
			/// Return a pointer to the internal array of sound detectors.
			GSOUND_INLINE const SoundDetector* const* getDetectors() const
			{
				return detectors.getPointer();
			}
			
			
			/// Return the number of cached detectors.
			GSOUND_INLINE Size getDetectorCount() const
			{
				return detectors.getSize();
			}
			
			
		//********************************************************************************
		//******	Object Mesh Accessor Methods
			
			
			/// Add an object mesh with the specified vertices and triangles to this debug cache.
			void addObjectMesh( const ArrayList<SoundVertex>& vertices,
								const ArrayList<SoundTriangle>& triangles,
								const ArrayList<SoundMaterial>& materials );
			
			
			/// Return a pointer to the internal array of object meshes.
			GSOUND_INLINE const ObjectMesh* getObjectMeshes() const
			{
				return objectMeshes.getPointer();
			}
			
			
			/// Return the number of cached object meshes.
			GSOUND_INLINE Size getObjectMeshCount() const
			{
				return objectMeshes.getSize();
			}
			
			
		//********************************************************************************
		//******	Diffraction Edges Accessor Methods
			
			
			/// Add a diffraction edge with the specified edge endpoints to this debug cache.
			void addDiffractionEdge( const Vector3f& v1, const Vector3f& v2 );
			
			
			/// Return a pointer to the internal array of diffraction edges.
			GSOUND_INLINE const DiffractionEdge* getDiffractionEdges() const
			{
				return diffractionEdges.getPointer();
			}
			
			
			/// Return the number of cached diffraction edges.
			GSOUND_INLINE Size getDiffractionEdgeCount() const
			{
				return diffractionEdges.getSize();
			}
			
			
		//********************************************************************************
		//******	Direct Propagation Path Accessor Methods
			
			
			/// Add a direct propagation path to this cache with the specified listener and source positions.
			void addDirectPath( const Vector3f& listenerPosition, const Vector3f& sourcePosition );
			
			
			/// Return a pointer to the internal array of direct propagation paths.
			GSOUND_INLINE const DirectPath* getDirectPaths() const
			{
				return directPaths.getPointer();
			}
			
			
			/// Return the number of cached direct sound paths.
			GSOUND_INLINE Size getDirectPathCount() const
			{
				return directPaths.getSize();
			}
			
			
		//********************************************************************************
		//******	Diffraction Propagation Path Accessor Methods
			
			
			/// Add a diffraction propagation path with the specified path points to this debug cache.
			void addDiffractionPath( const ArrayList<Vector3f>& points );
			
			
			/// Return a pointer to the internal array of diffraction propagation paths.
			GSOUND_INLINE const IndirectPath* getDiffractionPaths() const
			{
				return diffractionPaths.getPointer();
			}
			
			
			/// Return the number of cached diffraction paths.
			GSOUND_INLINE Size getDiffractionPathCount() const
			{
				return diffractionPaths.getSize();
			}
			
			
		//********************************************************************************
		//******	Specular Propagation Path Accessor Methods
			
			
			/// Add a specular propagation path with the specified path points to this debug cache.
			void addSpecularPath( const ArrayList<Vector3f>& points );
			
			
			/// Return a pointer to the internal array of specular propagation paths.
			GSOUND_INLINE const IndirectPath* getSpecularPaths() const
			{
				return specularPaths.getPointer();
			}
			
			
			/// Return the number of cached specular paths.
			GSOUND_INLINE Size getSpecularPathCount() const
			{
				return specularPaths.getSize();
			}
			
			
		//********************************************************************************
		//******	Diffuse Propagation Path Accessor Methods
			
			
			/// Add a diffuse propagation path with the specified path points to this debug cache.
			void addDiffusePath( const ArrayList<Vector3f>& points );
			
			
			/// Return a pointer to the internal array of diffuse propagation paths.
			GSOUND_INLINE const IndirectPath* getDiffusePaths() const
			{
				return diffusePaths.getPointer();
			}
			
			
			/// Return the number of cached diffuse paths.
			GSOUND_INLINE Size getDiffusePathCount() const
			{
				return diffusePaths.getSize();
			}
			
			
		//********************************************************************************
		//******	Transmission Propagation Path Accessor Methods
			
			
			/// Add a transmission sound path to this cache with the specified listener and source positions.
			void addTransmissionPath( const Vector3f& listenerPosition, const Vector3f& sourcePosition );
			
			
			/// Return a pointer to the internal array of transmission paths.
			GSOUND_INLINE const DirectPath* getTransmissionPaths() const
			{
				return transmissionPaths.getPointer();
			}
			
			
			/// Return the number of cached transmission paths.
			GSOUND_INLINE Size getTransmissionPathCount() const
			{
				return transmissionPaths.getSize();
			}
			
			
		//********************************************************************************
		//******	Cache Clear Method
			
			
			/// Clear all of the internal arrays of debug drawing information.
			void clear();
			
			
			/// Reset all of the internal arrays of debug drawing information, deallocating their storage.
			void reset();
			
			
		//********************************************************************************
		//******	Synchronization Method
			
			
			/// Lock this debug cache's mutex so that the cache can be safely accessed by the user.
			/**
			  * Failure to pair each call to lock() with a call to unlock() may cause a deadlock.
			  */
			GSOUND_INLINE void lock()
			{
				mutex.lock();
			}
			
			
			/// Unlock this debug cache's mutex so that the cache can be safely accessed by the user.
			/**
			  * Failure to pair each call to lock() with a call to unlock() may cause a deadlock.
			  */
			GSOUND_INLINE void unlock()
			{
				mutex.unlock();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of pointers to sound listeners in this debug cache.
			ArrayList<const SoundListener*> listeners;
			
			
			/// A list of pointers to sound sources in this debug cache.
			ArrayList<const SoundSource*> sources;
			
			
			/// A list of pointers to sound sources in this debug cache.
			ArrayList<const SoundDetector*> detectors;
			
			
			/// A list of object meshes in the scene.
			ArrayList<ObjectMesh> objectMeshes;
			
			
			/// A list of the diffraction edges in the scene.
			ArrayList<DiffractionEdge> diffractionEdges;
			
			
			/// A list of the current set of direct propagation paths.
			ArrayList<DirectPath> directPaths;
			
			
			/// A list of the current set of diffraction propagation paths.
			ArrayList<IndirectPath> diffractionPaths;
			
			
			/// A list of the current set of specular propagation paths.
			ArrayList<IndirectPath> specularPaths;
			
			
			/// A list of the current set of diffuse propagation paths.
			ArrayList<IndirectPath> diffusePaths;
			
			
			/// A list of the current set of transmission propagation paths.
			ArrayList<DirectPath> transmissionPaths;
			
			
			/// A mutex that synchronizes access to the debug information for multithreaded access.
			Mutex mutex;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_DEBUG_CACHE_H
