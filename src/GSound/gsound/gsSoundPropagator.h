/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundPropagator.h
 * Contents:    gsound::SoundPropagator class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PROPAGATOR_H
#define INCLUDE_GSOUND_SOUND_PROPAGATOR_H


#include "gsConfig.h"


#include "internal/gsWorldSpaceTriangle.h"
#include "internal/gsSoundPathID.h"
#include "internal/gsDiffusePathCache.h"
#include "gsPropagationRequest.h"
#include "gsSoundScene.h"
#include "gsSoundSceneIR.h"
#include "gsDebugCache.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that computes sound propagation in virtual scenes.
/**
  * A sound propagator takes as input a virtual scene and computes a set of
  * impulse responses or discrete sound paths that can then be used to render
  * audio for each source and listener.
  */
class SoundPropagator
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a sound propagator object with the default initial state.
			SoundPropagator();
			
			
			/// Create a copy of a sound propagator object, copying all internal state of that object.
			SoundPropagator( const SoundPropagator& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound propagator object, releasing all internal state and memory.
			virtual ~SoundPropagator();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another sound propagator object to this object.
			SoundPropagator& operator = ( const SoundPropagator& other );
			
			
		//********************************************************************************
		//******	Sound Propagation Method
			
			
			/// Propagate sound in the specified scene with the given listener.
			/**
			  * This is the primary method by which sound propagation is performed. When
			  * this method is called, the scene is used to describe the sources, listeners,
			  * and objects involved in sound propagation.
			  *
			  * The output of the sound propagation is placed in the scene IR parameter.
			  * The scene IR is automatically updated and replaced with contents that store
			  * the results of the propagation.
			  *
			  * The specified request may be modified if some parameters are not in the valid
			  * range or use unsupported features. The request is also updated with internal data
			  * that is used to cache propagation data from frame to frame.
			  *
			  * @param scene - the scene where sound propagation should be performed.
			  * @param request - an object indicating the parameters for sound propagation.
			  * @param sceneIR - an object where the output propagation data is stored.
			  */
			virtual void propagateSound( const SoundScene& scene, PropagationRequest& request, SoundSceneIR& sceneIR );
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that keeps track of the intersected triangle and listener image position at each probe depth.
			class ImagePosition;
			
			
			/// A class containing information about an edge that has been transformed into world space.
			class WorldSpaceEdge;
			
			
			/// A class that contains information about a single diffraction point on an edge.
			class DiffractionPoint;
			
			
			/// A class that contains information needed when searching a diffraction graph for paths.
			class DiffractionQuery;
			
			
			/// A class that stores temporary data about a specular path through the scene.
			class SpecularPathData;
			
			
			/// A class that stores temporary data about a diffuse path through the scene.
			class DiffusePathData;
			
			
			/// A class that stores propagation data for an enabled listener in the current scene.
			class ListenerData;
			
			
			/// A class that stores propagation data for an enabled source in the current scene.
			class SourceData;
			
			
			/// A class that stores thread-local information for threads that compute sound propagation.
			class ThreadData;
			
			
		//********************************************************************************
		//******	Listener Sound Propagation Methods
			
			
			void doListenerPropagation( const ListenerData& listenerData, SoundListenerIR& listenerIR );
			
			
			void propagateListenerRays( const SoundDetector& listener, const internal::SoundPathCache& soundPathCache,
										Size maxSpecularDepth, Size numSpecularRays, Size maxDiffuseDepth, Size numDiffuseRays,
										Float maxIRLength, ThreadData& threadData );
			
			
			Size propagateListenerSpecularRay( const SoundDetector& listener, const internal::SoundPathCache& soundPathCache,
												Ray3f ray, Size numBounces, Float maxIRLength, ThreadData& threadData );
			
			
			Size propagateListenerDiffuseRay( const SoundDetector& listener,
												Ray3f ray, Size numBounces, Float maxIRLength,
												const Vector3f& listenerDirection, ThreadData& threadData );
			
			
		//********************************************************************************
		//******	Source Sound Propagation Methods
			
			
			/// Do sound propagation from each sources's perspective, adding all paths to the listener IR.
			void doSourcesPropagation( const SoundDetector& listener, SoundListenerIR& listenerIR );
			
			
			/// Do sound propagation from each sources's perspective, adding all paths to the listener IR.
			void doSourcePropagation( const SoundDetector& listener, Index sourceIndex,
										Size maxDiffuseDepth, Size numDiffuseRays );
			
			
			/// Do diffuse sound propagation for the specified sound source and parameters.
			void propagateSourceRays( const SoundDetector& source, const SoundDetector& listener,
									Size maxDiffuseDepth, Size numDiffuseRays, Float maxIRLength, ThreadData& threadData );
			
			
			Size propagateSourceDiffuseRay( const SoundDetector& listener, Ray3f ray, Size numBounces,
											Float maxIRLength, const Vector3f& sourceDirection, ThreadData& threadData );
			
			
			/// Update the visibility caches for all sources in the scene.
			void updateSourcesVisibility();
			
			
			void updateVisibility( const Vector3f& position, Real radius, Size numVisibilityRays,
									internal::VisibilityCache& visibilityCache );
			
			
		//********************************************************************************
		//******	Diffuse Propagation Methods
			
			
			/// Output the specified diffuse paths to the output source IR.
			GSOUND_FORCE_INLINE void outputDiffusePaths( const ArrayList<DiffusePathData>& newPaths, SoundSourceIR& sourceIR );
			
			
			/// Output the specified diffuse paths to the output listener IR for all sound sources.
			GSOUND_FORCE_INLINE void outputDiffusePaths( const ArrayList<DiffusePathData>& newPaths, SoundListenerIR& listenerIR );
			
			
			/// Update a diffuse cache based on the specified list of newly detected paths.
			GSOUND_FORCE_INLINE void updateDiffuseCache( internal::DiffusePathCache& diffuseCache, const ArrayList<DiffusePathData>& newPaths );
			
			
			/// Update the diffuse caches for all sources in the scene based on the specified list of newly detected paths.
			GSOUND_FORCE_INLINE void updateDiffuseCaches( const ArrayList<DiffusePathData>& newPaths );
			
			
			/// Compute the output for the specified diffuse cache for a given number of rays cast.
			void outputDiffuseCache( internal::DiffusePathCache& diffuseCache, Size numDiffuseRaysCast, SoundSourceIR& sourceIR );
			
			
			/// Update an IR cache based on the specified list of newly detected paths.
			GSOUND_FORCE_INLINE void updateIRCache( SoundSourceIR& sourceIR, const ArrayList<DiffusePathData>& newPaths );
			
			
			/// Update the IR caches for all sources in the scene based on the specified list of newly detected paths.
			GSOUND_FORCE_INLINE void updateIRCaches( const ArrayList<DiffusePathData>& newPaths );
			
			
			/// Compute the output for the specified IR cache for a given number of rays cast.
			GSOUND_FORCE_INLINE void outputIRCache( internal::IRCache& irCache, Size numDiffuseRaysCast, SoundSourceIR& sourceIR );
			
			
		//********************************************************************************
		//******	Specular Propagation Methods
			
			
			/// Update the specified specular path cache with a new set of paths that were found.
			void updateSpecularCache( internal::SoundPathCache& specularCache, const ArrayList<SpecularPathData>& newPaths, SoundListenerIR& listenerIR );
			
			
			/// Check all cached probe paths for validity. If they are valid, add them to the output path buffer.
			void validateSpecularCache( const ListenerData& listenerData, SoundListenerIR& listenerIR );
			
			
			void validateSpecularCacheRange( internal::SoundPathCache& specularCache, Index bucketStartIndex, Size numBuckets,
											ThreadData& threadData );
			
			
			GSOUND_NO_INLINE Bool validateSpecularPath( const Sphere3f& sourceSphere, const Vector3f& listenerPosition, Size numSamples,
														Real& totalDistance, Vector3f& directionFromListener, Vector3f& directionToSource,
														Real& visibility, ThreadData& threadData );
			
			
			GSOUND_FORCE_INLINE Bool validatePointSpecularPath( const Sphere3f& sourceSphere, const Vector3f& listenerPosition,
																Real& totalDistance, Vector3f& directionFromListener, Vector3f& directionToSource,
																ThreadData& threadData );
			
			
			GSOUND_FORCE_INLINE Bool sampleSpecularPath( const Sphere3f& sourceSphere, const Vector3f& listenerPosition, Size numSamples,
														Real& totalDistance, Vector3f& directionFromListener, Vector3f& directionToSource,
														Real& visibility, ThreadData& threadData );
			
			
			
			template < Bool sampledIREnabled, Bool dopplerSortingEnabled >
			GSOUND_FORCE_INLINE void outputSpecularPath( const SpecularPathData& path, Float dopplerThreshold,
														SoundSourceIR& sourceIR );
			
			
		//********************************************************************************
		//******	Direct Sound Propagation Method
			
			
			/// Add all direct/transmitted propagation paths to the propagation path buffer.
			void addDirectPaths( const SoundListener& listener, SoundListenerIR& pathBuffer, ThreadData& threadData );
			
			
		//********************************************************************************
		//******	Diffraction Sound Propagation Methods
			
			
			/// Add any valid diffraction propagation paths for the specified triangle to the propagation path buffer.
			Bool addDiffractionPaths( ThreadData& threadData,
									const SoundDetector& listener,
									const internal::SoundPathCache* soundPathCache,
									const SoundDetector& source,
									const Vector3f& listenerImagePosition,
									const internal::WorldSpaceTriangle& worldTriangle,
									Index sourceIndex );
			
			
			/// Recursively determine high-order diffraction paths for the specified query and edge.
			void recursiveDiffraction( ThreadData& threadData, const WorldSpaceEdge& edge, Index depth );
			
			
			
			Bool validateDiffractionPath( const internal::SoundPathID& pathID, Index sourceIndex, ThreadData& threadData );
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Prepare the internal scene data and scene IR for sound propagation for multiple listeners.
			void prepareSceneData( const SoundScene& newScene, SoundSceneIR& sceneIR );
			
			
			/// Prepare the internal source data and listener IR for sound propagation for the specified listener.
			void prepareListenerSourceData( const SoundListener& listener, SoundListenerIR& listenerIR );
			
			
			/// Prepare the source IR and internal data for an unclustered sound source and listener.
			void prepareSourceData( SoundSource* source, SoundSourceIR& sourceIR,
									internal::PropagationData::ListenerData& listenerData );
			
			
			/// Prepare the specified source IR for output based on the current output parameters.
			GSOUND_FORCE_INLINE void prepareSourceIR( SoundSourceIR& sourceIR );
			
			
			/// Get the relative speed of the specified listener and source along the specified path.
			GSOUND_INLINE Real getRelativeSpeed( const SoundDetector& listener, const Vector3f& directionFromListener, 
												const SoundDetector& source, const Vector3f& directionToSource );
			
			
		//********************************************************************************
		//******	Private Geometry Helper Methods
			
			
			/// Return the cell ID for the specified triangle and barycentric coordinate.
			GSOUND_FORCE_INLINE static internal::SoundPathPoint::IDType computeCellID( const internal::InternalSoundTriangle& triangle,
																		const Vector3f& barycentric );
			
			
			/// Return the world-space center for the triangle patch with the specified ID.
			GSOUND_FORCE_INLINE static Vector3f computeCellCenter( const internal::ObjectSpaceTriangle& triangle,
																internal::SoundPathPoint::IDType cellID );
			
			
			/// Find the points of closest approach on two lines, only computing the point on the first line.
			GSOUND_FORCE_INLINE static void computePointOfClosestApproach( const Vector3f& p1, const Vector3f& v1,
																			const Vector3f& p2, const Vector3f& v2,
																			Real& v1t );
			
			
			/// Return whether or not the specified point is in front of both planes by at least the given offset.
			GSOUND_FORCE_INLINE static Bool testEdgeOrientationVsPoint( const Plane3f& plane1, const Plane3f& plane2,
																		const Vector3f& point, Real offset );
			
			
			/// Return a uniformly distributed random unit vector direction.
			GSOUND_FORCE_INLINE static Vector3f getRandomDirection( math::Random<Real>& variable );
			
			
			/// Return a uniformly distributed random vector in the cone aligned with the Z+ axis with the given cos(half-angle).
			GSOUND_FORCE_INLINE static Vector3f getRandomDirectionInZCone( math::Random<Real>& variable, Real cosTheta );
			
			
			/// Return a uniformly distributed random unit vector direction which are a hemisphere defined by a plane normal.
			GSOUND_FORCE_INLINE static Vector3f getRandomDirectionInHemisphere( math::Random<Real>& variable, const Vector3f& normal );
			
			
			/// Return the form factor (fraction visible) of the specified detector from the given point.
			/**
			  * The given number of random rays are used to sample the visibility of the detector.
			  */
			GSOUND_FORCE_INLINE Real getDetectorVisibility( const SoundDetector& detector, const Vector3f& point,
															Size numSamples, ThreadData& threadData );
			
			
			/// Return the form factor (fraction visible) of the specified source from the listener.
			/**
			  * The given number of random rays are used to sample the visibility of the detector.
			  */
			GSOUND_FORCE_INLINE Real getDirectVisibility( const Vector3f& sourcePosition, Real sourceRadius,
														const Vector3f& listenerPosition, Real listenerRadius,
														Vector3f& averageDirection, Size numSamples, ThreadData& threadData );
			
			
			/// Return half of the angular size in radians of a sphere for an observer at a given distance.
			GSOUND_FORCE_INLINE static Real getSphereHalfAngleSize( Real observerDistance, Real sphereRadius )
			{
				const Real sideSquared = math::square(observerDistance) - math::square(sphereRadius);
				const Real sphereHalfAngle = sideSquared > 0 ? math::acos( math::sqrt( sideSquared ) / observerDistance ) : Real(1);
				
				return sphereHalfAngle;
			}
			
			
			/// Return cosine of half of the angular size in radians of a sphere for an observer at a given distance.
			GSOUND_FORCE_INLINE static Real getSphereCosHalfAngle( Real observerDistance, Real sphereRadius )
			{
				const Real sideSquared = math::square(observerDistance) - math::square(sphereRadius);
				return math::sqrt( sideSquared ) / observerDistance;
			}
			
			
			/// Return the fraction of the hemisphere area that a given sphere projects onto.
			GSOUND_FORCE_INLINE static Real getHemisphereSphereAttenuation( Real observerDistance, Real sphereRadius )
			{
				Real halfAngle = getSphereHalfAngleSize( observerDistance, sphereRadius );
				
				// These should be multiplied by pi, but it cancels out, so removed.
				const Real projectedArea = math::square( observerDistance*math::tan(halfAngle) );
				const Real hemisphereArea = Real(2)*math::square( observerDistance );
				
				return hemisphereArea > projectedArea ? projectedArea / hemisphereArea : Real(1);
			}
			
			
			/// Return the frequency-dependent attenuation factor for sound that travels the specified distance.
			GSOUND_FORCE_INLINE FrequencyBandResponse getDistanceAttenuation( Real distance ) const;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The number of paths that can be placed in a thread's output path buffer before they are sent to the main thread.
			static const Size PATH_BUFFER_SIZE = 128;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the current sound sources in the scene.
			ArrayList<SourceData> sourceDataList;
			
			
			/// A list of the current sound listeners in the scene.
			ArrayList<ListenerData> listenerDataList;
			
			
			/// A list of data structures for each source in the scene containing thread-local data.
			ArrayList<ThreadData> threadDataList;
			
			
			/// A pool of worker threads which the sound propagator delegates tasks to.
			ThreadPool threadPool;
			
			
			/// A signal that the main thread waits on when it has no paths to process.
			Signal mainThreadSignal;
			
			
			/// A pointer to the current sound propagation request.
			PropagationRequest* request;
			
			
			/// A temporary pointer to the current scene where sound propagation is being performed.
			const SoundScene* scene;
			
			
			/// A temporary pointer to an object which stores analytic information about propagation.
			SoundStatistics* statistics;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PROPAGATOR_H
