/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundPropagator.cpp
 * Contents:    gsound::SoundPropagator class implementation
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


#include "gsSoundPropagator.h"


#include <algorithm>
#include "internal/gsSoundPathCache.h"
#include "internal/gsUTDFrequencyResponse.h"

#define EDGE_CLAMP 0


#define DIFFUSE_CACHE_ENABLED 1


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


using namespace internal;


//##########################################################################################
//##########################################################################################
//############		
//############		Listener Data Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: ListenerData
{
	public:
		
		GSOUND_INLINE ListenerData( const SoundListener* newListener,
									internal::PropagationData::ListenerData* newListenerData,
									SoundListenerIR* newOutputIR )
			:	listener( newListener ),
				listenerData( newListenerData ),
				soundPathCache( &newListenerData->soundPathCache ),
				outputIR( newOutputIR )
		{
		}
		
		/// A pointer to the listener for this listener data.
		const SoundListener* listener;
		
		/// A pointer to the internal data structure for this listener.
		internal::PropagationData::ListenerData* listenerData;
		
		/// A pointer to the sound path cache for the listener.
		internal::SoundPathCache* soundPathCache;
		
		/// A pointer to the output IR for this listener.
		SoundListenerIR* outputIR;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Source Data Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: SourceData
{
	public:
		
		GSOUND_INLINE SourceData( const SoundDetector* newDetector, internal::PropagationData::SourceData* newSourceData,
								SoundSourceIR* newOutputIR )
			:	detector( newDetector ),
				sourceData( newSourceData ),
				diffuseCache( &newSourceData->diffusePathCache ),
				irCache( &newSourceData->irCache ),
				visibilityCache( &newSourceData->visibilityCache ),
				directivity( NULL ),
				outputIR( newOutputIR ),
				numDiffuseRaysCast( 0 ),
				maxIRDistance( 0 )
		{
		}
		
		/// A pointer to the source detector for the source.
		const SoundDetector* detector;
		
		/// A pointer to the internal data structure for this source.
		internal::PropagationData::SourceData* sourceData;
		
		/// A pointer to the diffuse path cache for the source and current listener.
		internal::DiffusePathCache* diffuseCache;
		
		/// A pointer to the IR cache for the source and current listener.
		internal::IRCache* irCache;
		
		/// A pointer to the visibility cache for the source and current listener.
		internal::VisibilityCache* visibilityCache;
		
		/// A pointer to an object that describes this source's directivity.
		internal::SoundBandDirectivity* directivity;
		
		/// A pointer to the output IR for this source.
		SoundSourceIR* outputIR;
		
		/// The total number of diffuse rays cast from this source on the current frame.
		Size numDiffuseRaysCast;
		
		/// The maximum path length that should be sampled for this source.
		Float maxIRDistance;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Image Position Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: ImagePosition
{
	public:
		
		GSOUND_INLINE ImagePosition( const internal::WorldSpaceTriangle& newTriangle,
									const Vector3f& newImagePosition )
			:	triangle( newTriangle ),
				imagePosition( newImagePosition )
		{
		}
		
		
		internal::WorldSpaceTriangle triangle;
		Vector3f imagePosition;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		World-Space Edge Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: WorldSpaceEdge
{
	public:
		
		GSOUND_INLINE WorldSpaceEdge()
		{
		}
		
		GSOUND_INLINE WorldSpaceEdge( const Vector3f& newV1, const Vector3f& newV2, const Plane3f& newPlane1, const Plane3f& newPlane2,
									const Vector3f& edgeNormal, const internal::DiffractionEdge* newEdge )
			:	edge( newEdge ),
				v1( newV1 ),
				v2( newV2 ),
				plane1( newPlane1 ),
				plane2( newPlane2 ),
				normal( edgeNormal ),
				transform( NULL )
		{
			Real l;
			direction = (newV2 - newV1).normalize( l );
			length = l;
		}
		
		
		GSOUND_INLINE WorldSpaceEdge( const internal::DiffractionEdge* newEdge, const Transform3f& transform )
			:	edge( newEdge ),
				v1( transform.transformToWorld( *newEdge->v1 ) ),
				v2( transform.transformToWorld( *newEdge->v2 ) ),
				plane1( transform.transformToWorld( newEdge->plane1 ) ),
				plane2( transform.transformToWorld( newEdge->plane2 ) ),
				transform( &transform )
		{
			Real l;
			direction = (v2 - v1).normalize( l );
			length = l;
			normal = transform.transformToWorld( newEdge->getNormal() );
		}
		
		
		const internal::DiffractionEdge* edge;
		const Transform3f* transform;
		Vector3f v1;
		Vector3f v2;
		Vector3f direction;
		Real length;
		Plane3f plane1;
		Plane3f plane2;
		Vector3f normal;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Diffraction Point Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: DiffractionPoint
{
	public:
		
		GSOUND_INLINE DiffractionPoint( const Vector3f& newPoint )
			:	point( newPoint ),
				distance( 0 )
		{
		}
		
		/// The point on the edge where the diffraction occurs.
		Vector3f point;
		
		/// The distance so far along the path up to this point.
		Real distance;
		
		/// The plane of the edge that points towards the source image position.
		const Plane3f* sourcePlane;
		
		/// The plane of the edge that points towards the listener image position.
		const Plane3f* listenerPlane;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Diffraction Query Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: DiffractionQuery
{
	public:
		
		GSOUND_INLINE DiffractionQuery()
			:	source( NULL ),
				listener( NULL ),
				soundPathCache( NULL ),
				graph( NULL ),
				lastValidIndex( 0 ),
				listenerSpeed( 0 ),
				sourceIndex( 0 )
		{
		}
		
		/// The sound source for the query.
		const SoundDetector* source;
		
		
		/// The sound listener for the query.
		const SoundDetector* listener;
		
		
		/// The sound path cache for the listener.
		const internal::SoundPathCache* soundPathCache;
		
		
		/// The object upon which the query is being performed.
		const SoundObject* object;
		
		
		/// The diffraction graph upon which the query is being performed.
		const internal::DiffractionGraph* graph;
		
		
		/// The normalized vector from the listener to the source, the search direction.
		Vector3f listenerToSourceDirection;
		
		
		/// The normalized direction from the listener to the first edge diffraction point.
		Vector3f listenerPathDirection;
		
		
		/// The relative speed of the source and listener along the propagation path.
		Real listenerSpeed;
		
		
		/// A stack containing information for each diffraction point along the path.
		ArrayList<DiffractionPoint> points;
		
		
		/// A stack containing the frequency-dependent attenuation for each diffraction point along the path.
		ArrayList<FrequencyBandResponse> pointResponses;
		
		
		/// The last index in the image position stack which has been validated by visibility rays.
		Index lastValidIndex;
		
		
		/// The index of the source in the scene which is being queried.
		Index sourceIndex;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Specular Path Data Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: SpecularPathData
{
	public:
		
		GSOUND_INLINE SpecularPathData( SoundPathHash newHash, SoundPathFlags newFlags,
										const FrequencyBandResponse& newEnergy,
										const Vector3f& newDirection, const Vector3f& newSourceDirection,
										Real newDistance, Real newRelativeSpeed, Real newSpeedOfSound,
										Index newSourceIndex )
			:	energy( newEnergy ),
				direction( newDirection ),
				sourceDirection( newSourceDirection ),
				distance( newDistance ),
				relativeSpeed( newRelativeSpeed ),
				speedOfSound( newSpeedOfSound ),
				pathID(),
				pathHash( newHash ),
				pathFlags( newFlags ),
				sourceIndex( newSourceIndex )
		{
		}
		
		GSOUND_INLINE SpecularPathData( const internal::SoundPathID& newPathID, SoundPathFlags newFlags,
										const FrequencyBandResponse& newEnergy,
										const Vector3f& newDirection, const Vector3f& newSourceDirection,
										Real newDistance, Real newRelativeSpeed, Real newSpeedOfSound,
										Index newSourceIndex )
			:	energy( newEnergy ),
				direction( newDirection ),
				sourceDirection( newSourceDirection ),
				distance( newDistance ),
				relativeSpeed( newRelativeSpeed ),
				speedOfSound( newSpeedOfSound ),
				pathID( newPathID ),
				pathHash( newPathID.getHashCode() ),
				pathFlags( newFlags ),
				sourceIndex( newSourceIndex )
		{
		}
		
		
		/// The frequency-dependent energy of this specular path (after previous bounces).
		FrequencyBandResponse energy;
		
		/// The direction from the listener to the closest bounce point for this specular path.
		Vector3f direction;
		
		/// The direction from the source to the closest bounce point for this diffuse path.
		Vector3f sourceDirection;
		
		/// The total distance along this specular path (including previous bounces).
		Real distance;
		
		/// The relative speed along the specular path between the source and listener.
		Real relativeSpeed;
		
		/// The average speed of sound along the specular path between the source and listener.
		Real speedOfSound;
		
		/// The path ID for this specular path.
		internal::SoundPathID pathID;
		
		/// The hash code ID for this specular path.
		SoundPathHash pathHash;
		
		/// The path ID for this specular path.
		SoundPathFlags pathFlags;
		
		/// The sound source index.
		Index sourceIndex;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Diffuse Path Data Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: DiffusePathData
{
	public:
		
		GSOUND_INLINE DiffusePathData( SoundPathHash newPathHash, const FrequencyBandResponse& newEnergy,
										const Vector3f& newDirection, const Vector3f& newSourceDirection,
										Real newDistance, Real newRelativeSpeed, Index newSourceIndex )
			:	energy( newEnergy ),
				direction( newDirection ),
				sourceDirection( newSourceDirection ),
				distance( newDistance ),
				relativeSpeed( newRelativeSpeed ),
				pathHash( newPathHash ),
				sourceIndex( newSourceIndex )
		{
		}
		
		
		/// The frequency-dependent energy of this diffuse path (after previous bounces).
		FrequencyBandResponse energy;
		
		/// The direction from the listener to the closest bounce point for this diffuse path.
		Vector3f direction;
		
		/// The direction from the source to the closest bounce point for this diffuse path.
		Vector3f sourceDirection;
		
		/// The total distance along this diffuse path (including previous bounces).
		Real distance;
		
		/// The relative speed along the diffuse path between the source and listener.
		Real relativeSpeed;
		
		/// The hash code ID for this diffuse path.
		SoundPathHash pathHash;
		
		/// The sound source index.
		Index sourceIndex;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Data Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundPropagator:: ThreadData
{
	public:
		
		GSOUND_INLINE ThreadData( UInt32 randomSeed, SoundPropagator* newPropagator )
			:	randomVariable( randomSeed ),
				numDiffuseRaysCast( 0 ),
				numSpecularRaysCast( 0 ),
				totalRayDepth( 0 ),
				diffuseBufferIndex( 0 ),
				diffuseBufferFilled( 0 ),
				threadDone( 0 ),
				propagator( newPropagator )
		{
			diffusePaths[0].setCapacity( PATH_BUFFER_SIZE );
			diffusePaths[1].setCapacity( PATH_BUFFER_SIZE );
		}
		
		
		/// Add a diffuse path to the output buffer.
		GSOUND_INLINE void postPath( const DiffusePathData& newDiffusePath )
		{
			diffusePaths[diffuseBufferIndex].add( newDiffusePath );
			
			// If there are enough paths in the buffer, signal the main thread that they are available.
			if ( diffusePaths[diffuseBufferIndex].getSize() >= PATH_BUFFER_SIZE && !diffuseBufferFilled )
			{
				// Swap the buffers so that the main thread can consume the new paths.
				diffuseBufferIndex = 1 - diffuseBufferIndex;
				
				// Signal that there is a new buffer.
				diffuseBufferFilled++;
				propagator->mainThreadSignal.signal();
			}
		}
		
		
		
		
		SoundPropagator* propagator;
		
		
		/// A random variable which generates the initial directions for probe rays.
		math::Random<Real> randomVariable;
		
		
		/// A temporary object which is used to accumulate all points along a diffuse path.
		internal::SoundPathID diffusePathID;
		
		
		/// A temporary object which is used to accumulate all points along a specular path.
		internal::SoundPathID specularPathID;
		
		
		/// A list used to store the series of image sources for a path in world space.
		ArrayList<ImagePosition> imagePositions;
		
		
		/// A temporary array of validation rays used to compute specular validity.
		Array<Ray3f> validationRays;
		
		
		/// An object which stores information needed when doing a diffraction query.
		DiffractionQuery diffractionQuery;
		
		
		/// An output buffer of valid specular paths which should be added to the cache.
		ArrayList<SpecularPathData> specularPaths;
		
		
		/// A pair of output buffers of diffuse paths that hit the listener.
		ArrayList<DiffusePathData> diffusePaths[2];
		
		
		/// The index of the current buffer where the thread is putting its output diffuse paths.
		Index diffuseBufferIndex;
		
		
		/// An atomic boolean value that is 1 when the thread has filled a buffer of diffuse paths.
		Atomic<UInt32> diffuseBufferFilled;
		
		
		/// An atomic boolean value that is 1 when the thread is finished tracing all rays.
		Atomic<UInt32> threadDone;
		
		
		/// The total number of diffuse rays that were cast by this thread.
		Size numDiffuseRaysCast;
		
		
		/// The total number of specular rays that were cast by this thread.
		Size numSpecularRaysCast;
		
		
		/// The total number of ray bounces summed for all rays.
		Size totalRayDepth;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




SoundPropagator:: SoundPropagator()
	:	request(),
		scene( NULL ),
		statistics( NULL )
{
	threadPool.setPriority( ThreadPriority::LOW );
}




SoundPropagator:: SoundPropagator( const SoundPropagator& other )
	:	request(),
		scene( NULL ),
		statistics( NULL )
{
	threadPool.setPriority( ThreadPriority::LOW );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundPropagator:: ~SoundPropagator()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundPropagator& SoundPropagator:: operator = ( const SoundPropagator& other )
{
	if ( this != &other )
	{
		// Copy the other internal state of the SoundPropagator object.
		request = other.request;
		scene = other.scene;
		//threadPool = other.threadPool;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Sound Propagation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: propagateSound( const SoundScene& newScene, PropagationRequest& newRequest, SoundSceneIR& sceneIR )
{
	//***************************************************************************
	
	// Sanitize the sound propagation request and store a temporary pointer to it.
	request = &newRequest;
	request->numDirectRays = math::clamp( request->numDirectRays, Size(1), Size(1000000000) );;
	request->maxSpecularDepth = math::min( request->maxSpecularDepth, Size(1000) );
	request->numSpecularRays = math::min( request->numSpecularRays, Size(1000000000) );
	request->numSpecularSamples = math::clamp( request->numSpecularSamples, Size(1), Size(10000) );
	request->maxDiffuseDepth = math::min( request->maxDiffuseDepth, Size(1000) );
	request->numDiffuseRays = math::min( request->numDiffuseRays, Size(1000000000) );
	request->numDiffuseSamples = math::clamp( request->numDiffuseSamples, Size(1), Size(10000) );
	request->maxDiffractionDepth = math::min( request->maxDiffractionDepth, Size(1000) );
	request->maxDiffractionOrder = math::min( request->maxDiffractionOrder, Size(10) );
	request->responseTime = math::clamp( request->responseTime, Real(0.0), Real(100.0) );
	request->visibilityCacheTime = math::clamp( request->visibilityCacheTime, Real(0.0), Real(100.0) );
	request->rayOffset = math::clamp( request->rayOffset, Real(0.0), Real(1.0) );
	request->numThreads = math::clamp( request->numThreads, Size(1), 2*CPU::getCount() );
	request->minQuality = math::clamp( request->minQuality, Float(0), Float(1) );
	request->maxQuality = math::clamp( request->maxQuality, Float(0), Float(10) );
	request->quality = math::clamp( request->quality, request->minQuality, request->maxQuality );
	
	Timer totalTimer;
	
	//***************************************************************************
	// Initialize the thread pool if necessary.
	
	// Add the necessary number of threads to the thread pool.
	if ( threadPool.getThreadCount() != request->numThreads )
		threadPool.setThreadCount( request->numThreads );
	
	//***************************************************************************
	
	// Set an internal temporary pointer to the object which store analytic information.
	if ( request->flags.isSet( PropagationFlags::STATISTICS ) )
		statistics = request->statistics;
	else
		statistics = NULL;
	
	//***************************************************************************
	
	// Prepare the scene IR.
	prepareSceneData( newScene, sceneIR );
	
	//***************************************************************************
	// Do sound propagation for each listener in the scene.
	
	Float maxListenerIRLength = 0;
	Float averageIRLength = 0;
	Size numAverageIRSources = 0;
	const Size numListeners = listenerDataList.getSize();
	
	for ( Index l = 0; l < numListeners; l++ )
	{
		// Get the listener.
		ListenerData& listenerData = listenerDataList[l];
		const SoundListener& listener = *listenerData.listener;
		SoundListenerIR& listenerIR = *listenerData.outputIR;
		
		//***************************************************************************
		// Prepare data structures for propagation.
		
		// Make sure that the IR is initialized and empty for each sound source.
		prepareListenerSourceData( listener, listenerIR );
		
		//***************************************************************************
		// Find all direct/transmitted contribution paths.
		
		// Only do sound propagation if there are objects in the scene.
		if ( scene->getObjectCount() > 0 )
		{
			//***************************************************************************
			// Update the visibility caches for the sources and listener.
			
			if ( request->flags.isSet( PropagationFlags::VISIBILITY_CACHE ) )
				updateSourcesVisibility();
			
			//***************************************************************************
			// Check previously found cached paths to see if they are still valid.
			
			if ( request->flags.isSet( PropagationFlags::SPECULAR_CACHE ) )
				validateSpecularCache( listenerData, listenerIR );
			else
				listenerData.soundPathCache->clear();
			
			//***************************************************************************
			// Do listener sound propagation.
			
			doListenerPropagation( listenerData, listenerIR );
			
			//***************************************************************************
			// Do source sound propagation.
			
			if ( request->flags.isSet( PropagationFlags::SOURCE_DIFFUSE ) )
				doSourcesPropagation( listener, listenerIR );
		}
		
		addDirectPaths( listener, listenerIR, threadDataList[0] );
		
		//***************************************************************************
		// Post-process the IRs for the listener.
		
		// Convert the threshold in dB SPL to threshold in sound power.
		const FrequencyBandResponse thresholdPower = listener.getThresholdPower( request->frequencies );
		const Size numSources = listenerIR.getSourceCount();
		Float listenerIRLength = 0;
		numAverageIRSources += numSources;
		
		for ( Index s = 0; s < numSources; s++ )
		{
			SourceData& sourceData = sourceDataList[s];
			SoundSourceIR& sourceIR = listenerIR.getSourceIR(s);
			Float sourceIRLength = 0;
			
			// Trim the length of the IR based on the listener's threshold of hearing.
			if ( request->flags.isSet( PropagationFlags::IR_THRESHOLD ) )
				sourceIRLength = sourceIR.trim( thresholdPower );
			else
				sourceIRLength = sourceIR.getLength();
			
			// Determine the max IR length for the source on the next frame based on the current IR length.
			if ( request->flags.isSet( PropagationFlags::IR_THRESHOLD ) &&
				request->flags.isSet( PropagationFlags::ADAPTIVE_IR_LENGTH ) )
			{
				const Float baseGrowth = request->irGrowthRate*request->dt;
				Float previousMaxLength = sourceData.sourceData->maxIRLength;
				Float growth;
				
				if ( sourceIRLength + baseGrowth < previousMaxLength )
				{
					// Shrink the IR if the trimmed IR was significantly shorter than the prevous max length.
					growth = -math::min( baseGrowth, previousMaxLength - sourceIRLength );
				}
				else
				{
					// Grow the IR by at least the base growth.
					growth = math::max( baseGrowth, sourceIRLength - previousMaxLength );
				}
				
				Float maxIRLength = math::clamp( previousMaxLength + growth, request->minIRLength, request->maxIRLength );
				
				// Save the max IR length for later.
				sourceData.sourceData->maxIRLength = maxIRLength;
			}
			
			// Save the source IR length in the data for the source.
			sourceData.sourceData->irLength = sourceIRLength;
			sourceData.irCache->setLengthInSamples( sourceIR.getLengthInSamples() );
			
			averageIRLength += sourceIRLength;
			
			// Compute the maximum source IR length for the listener.
			listenerIRLength = math::max( listenerIRLength, sourceIRLength );
		}
		
		// Determine the max IR length for the listener on the next frame based on the current IR length.
		if ( request->flags.isSet( PropagationFlags::IR_THRESHOLD ) &&
			request->flags.isSet( PropagationFlags::ADAPTIVE_IR_LENGTH ) )
		{
			const Float baseGrowth = request->irGrowthRate*request->dt;
			Float previousMaxLength = listenerData.listenerData->maxIRLength;
			Float growth;
			
			if ( listenerIRLength + baseGrowth < previousMaxLength )
			{
				// Shrink the IR if the trimmed IR was significantly shorter than the prevous max length.
				growth = -math::min( baseGrowth, previousMaxLength - listenerIRLength );
			}
			else
			{
				// Grow the IR by at least the base growth.
				growth = math::max( baseGrowth, listenerIRLength - previousMaxLength );
			}
			
			Float maxIRLength = math::clamp( previousMaxLength + growth, request->minIRLength, request->maxIRLength );
			
			// Save the max IR length for later.
			listenerData.listenerData->maxIRLength = maxIRLength;
		}
		
		listenerData.listenerData->irLength = listenerIRLength;
		maxListenerIRLength = math::max( maxListenerIRLength, listenerIRLength );
	}
	
	// Remove old source and listener data.
	request->internalData.removeOldData();
	
	// Store the total time that was spent on this frame.
	Time totalTime = totalTimer.getElapsedTime();
	
	if ( statistics != NULL )
	{
		statistics->averageIRLength = averageIRLength / (numAverageIRSources*numListeners);
		statistics->maxIRLength = maxListenerIRLength;
		statistics->propagationTime = totalTime;
	}
	
	//***************************************************************************
	// Reset temporary pointers to NULL.
	
	scene = NULL;
	request = NULL;
	statistics = NULL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Listener Propagation Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: doListenerPropagation( const ListenerData& listenerData, SoundListenerIR& listenerIR )
{
	const Bool specularEnabled = request->flags.isSet( PropagationFlags::SPECULAR );
	const Bool diffuseEnabled = request->flags.isSet( PropagationFlags::DIFFUSE );
	const Bool diffractionEnabled = request->flags.isSet( PropagationFlags::DIFFRACTION );
	const Bool diffuseCacheEnabled = request->flags.isSet( PropagationFlags::DIFFUSE_CACHE );
	const Bool irCacheEnabled = request->flags.isSet( PropagationFlags::IR_CACHE ) && request->flags.isSet( PropagationFlags::SAMPLED_IR );
	const Size maxSpecularDepth = (Size)(request->maxSpecularDepth/**request->quality*/);
	const Size numSpecularRays = (Size)(request->numSpecularRays*request->quality);
	const Size maxDiffuseDepth = (Size)(request->maxDiffuseDepth/*request->quality*/);
	const Size numDiffuseRays = (Size)(request->numDiffuseRays*request->quality);
	const Size numThreads = request->numThreads;
	const Size numSources = sourceDataList.getSize();
	
	const SoundListener& listener = *listenerData.listener;
	SoundPathCache& soundPathCache = *listenerData.soundPathCache;
	
	// Determine what the maximum IR length for the listener should be.
	const Float maxIRLength = listenerData.listenerData->maxIRLength;
	
	//************************************************************************
	// Trace rays from the listener to find sound paths through the scene.
	
	Timer timer;
	
	if ( numThreads > 1 )
	{
		// Compute the total number of rays that should be traced for each thread.
		const Size specularRaysPerThread = (Size)math::ceiling( Real(numSpecularRays) / Real(numThreads) );
		const Size diffuseRaysPerThread = (Size)math::ceiling( Real(numDiffuseRays) / Real(numThreads) );
		Index specularRayStart = 0;
		Index diffuseRayStart = 0;
		
		// Queue the diffuse jobs for all threads.
		for ( Index i = 0; i < numThreads; i++ )
		{
			// Compute the number of rays that this thread should have.
			const Size numSpecularThreadRays = math::min( numSpecularRays - specularRayStart, specularRaysPerThread );
			const Size numDiffuseThreadRays = math::min( numDiffuseRays - diffuseRayStart, diffuseRaysPerThread );
			
			// Queue the job for this range of rays.
			threadPool.addJob( FunctionCall< void ( const SoundDetector&, const SoundPathCache&, Size, Size, Size, Size, Float, ThreadData& )>(
										bind( &SoundPropagator::propagateListenerRays, this ),
										listener, soundPathCache, maxSpecularDepth, numSpecularThreadRays,
										maxDiffuseDepth, numDiffuseThreadRays, maxIRLength, threadDataList[i] ) );
			
			specularRayStart += numSpecularThreadRays;
			diffuseRayStart += numDiffuseThreadRays;
		}
		
		//************************************************************************
		// Wait for the ray tracing jobs to finish and concurrently consume the diffuse paths generated.
		
		if ( diffuseEnabled )
		{
			Bool processing = true;
			
			while ( processing )
			{
				// Process any new path buffers.
				for ( Index i = 0; i < numThreads; i++ )
				{
					ThreadData& threadData = threadDataList[i];
					
					// Process new diffuse paths.
					if ( threadData.diffuseBufferFilled )
					{
						ArrayList<DiffusePathData>& newDiffusePaths = threadData.diffusePaths[!threadData.diffuseBufferIndex];
						
						// Update the caches with the new paths.
						if ( irCacheEnabled )
							updateIRCaches( newDiffusePaths );
						else if ( diffuseCacheEnabled )
							updateDiffuseCaches( newDiffusePaths );
						else
							outputDiffusePaths( newDiffusePaths, listenerIR );
						
						// Clear the list of paths for next time.
						newDiffusePaths.clear();
						
						// Signal to the thread that the path buffer has been consumed.
						threadData.diffuseBufferFilled--;
					}
				}
				
				// Check to see if all of the threads are done.
				processing = false;
				
				for ( Index i = 0; i < numThreads; i++ )
				{
					if ( !threadDataList[i].threadDone )
						processing = true;
				}
				
				// Give up the CPU since we have finished consuming all new paths.
				//Thread::sleep(0.0001);
				mainThreadSignal.wait( 0.0001 );
			}
		}
		
		// Wait for the ray tracing jobs to finish.
		threadPool.finishJobs();
	}
	else
	{
		// Do all diffuse propagation on the main thread to avoid switching contexts.
		propagateListenerRays( listener, soundPathCache, maxSpecularDepth, numSpecularRays,
								maxDiffuseDepth, numDiffuseRays, maxIRLength, threadDataList[0] );
	}
	
	//************************************************************************
	// Consume the final set of output paths.
	
	// A count of the number of specular and diffuse rays that were cast this frame.
	Size numDiffuseRaysCast = 0;
	Size numSpecularRaysCast = 0;
	Size totalRayDepth = 0;
	
	for ( Index i = 0; i < numThreads; i++ )
	{
		ThreadData& threadData = threadDataList[i];
		
		// Check both output buffers for paths.
		for ( Index bufferIndex = 0; bufferIndex < 2; bufferIndex++ )
		{
			ArrayList<DiffusePathData>& newDiffusePaths = threadData.diffusePaths[bufferIndex];
			
			if ( newDiffusePaths.getSize() > 0 )
			{
				// Update the caches with the new paths.
				if ( irCacheEnabled )
					updateIRCaches( newDiffusePaths );
				else if ( diffuseCacheEnabled )
					updateDiffuseCaches( newDiffusePaths );
				else
					outputDiffusePaths( newDiffusePaths, listenerIR );
				
				// Clear the list of paths for next time.
				newDiffusePaths.clear();
			}
		}
		
		// Make sure the atomic variables are in the correct state for next time.
		if  ( threadData.diffuseBufferFilled )
			threadData.diffuseBufferFilled--;
		
		if  ( threadData.threadDone )
			threadData.threadDone--;
		
		// Count the number of diffuse rays that were cast this frame.
		numDiffuseRaysCast += threadDataList[i].numDiffuseRaysCast;
		numSpecularRaysCast += threadDataList[i].numSpecularRaysCast;
		totalRayDepth += threadDataList[i].totalRayDepth;
	}
	
	timer.update();
	
	if ( statistics != NULL )
	{
		statistics->rayTracingTime = timer.getLastInterval();
		statistics->diffuseRayCount = numDiffuseRaysCast;
		statistics->specularRayCount = numSpecularRaysCast;
		statistics->diffuseRayDepth = Size(Float(totalRayDepth) / numDiffuseRaysCast);
	}
	
	//************************************************************************
	// Compute the output IR for each sound source in parallel based on the diffuse cache content.
	
	if ( diffuseEnabled )
	{
		if ( irCacheEnabled )
		{
			if ( numThreads > 1 )
			{
				for ( Index s = 0; s < numSources; s++ )
				{
					IRCache& irCache = *sourceDataList[s].irCache;
					SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
					
					threadPool.addJob( FunctionCall< void ( IRCache&, Size, SoundSourceIR& )>(
												bind( &SoundPropagator::outputIRCache, this ),
												irCache, numDiffuseRaysCast, sourceIR ) );
				}
				
				// Wait for the cache update jobs to finish.
				threadPool.finishJobs();
			}
			else
			{
				for ( Index s = 0; s < numSources; s++ )
				{
					IRCache& irCache = *sourceDataList[s].irCache;
					SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
					
					outputIRCache( irCache, numDiffuseRaysCast, sourceIR );
				}
			}
		}
		else if ( diffuseCacheEnabled )
		{
			if ( numThreads > 1 )
			{
				for ( Index s = 0; s < numSources; s++ )
				{
					DiffusePathCache& diffuseCache = *sourceDataList[s].diffuseCache;
					SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
					
					threadPool.addJob( FunctionCall< void ( DiffusePathCache&, Size, SoundSourceIR& )>(
												bind( &SoundPropagator::outputDiffuseCache, this ),
												diffuseCache, numDiffuseRaysCast, sourceIR ) );
				}
				
				// Wait for the cache update jobs to finish.
				threadPool.finishJobs();
			}
			else
			{
				for ( Index s = 0; s < numSources; s++ )
				{
					DiffusePathCache& diffuseCache = *sourceDataList[s].diffuseCache;
					SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
					
					outputDiffuseCache( diffuseCache, numDiffuseRaysCast, sourceIR );
				}
			}
		}
		else
		{
			// Normalize the paths based on the number of rays traced.
			const Float normalize = Float(1) / Float(numDiffuseRaysCast);
			
			if ( request->flags.isSet( PropagationFlags::SAMPLED_IR ) )
			{
				for ( Index s = 0; s < numSources; s++ )
				{
					SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
					SampledIR& sampledIR = sourceIR.getSampledIR();
					math::multiply( sampledIR.getIntensity() + sampledIR.getStartTimeInSamples()*SIMDBands::getWidth(), normalize,
									(sampledIR.getLengthInSamples() - sampledIR.getStartTimeInSamples())*SIMDBands::getWidth() );
				}
			}
			else
			{
				for ( Index s = 0; s < numSources; s++ )
				{
					SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
					
					const Size numPaths = sourceIR.getPathCount();
					
					for ( Index p = 0; p < numPaths; p++ )
					{
						SoundPath& path = sourceIR.getPath(p);
						
						if ( path.getFlags().isSet( SoundPathFlags::DIFFUSE ) )
							path.setIntensity( path.getIntensity()*normalize );
					}
				}
			}
		}
	}
	
	timer.update();
	
	if ( statistics != NULL )
		statistics->cacheUpdateTime = timer.getLastInterval();
	
	//************************************************************************
	// Update the specular cache.
	
	if ( specularEnabled || diffractionEnabled )
	{
		// Update the cache with the new paths found on this frame.
		for ( Index i = 0; i < numThreads; i++ )
		{
			ThreadData& threadData = threadDataList[i];
			ArrayList<SpecularPathData>& specularPaths = threadData.specularPaths;
			
			// Update the cache with the new paths found by this thread.
			updateSpecularCache( soundPathCache, specularPaths, listenerIR );
			
			// Clear the list of specular paths for next time.
			specularPaths.clear();
		}
	}
}




void SoundPropagator:: propagateListenerRays( const SoundDetector& listener, const internal::SoundPathCache& soundPathCache,
												Size maxSpecularDepth, Size numSpecularRays,
												Size maxDiffuseDepth, Size numDiffuseRays,
												Float maxIRLength, ThreadData& threadData )
{
	const Bool specularEnabled = request->flags.isSet( PropagationFlags::SPECULAR );
	const Bool diffuseEnabled = request->flags.isSet( PropagationFlags::DIFFUSE );
	const Bool diffractionEnabled = request->flags.isSet( PropagationFlags::DIFFRACTION );
	const Size maxDiffractionDepth = request->maxDiffractionDepth;
	
	//************************************************************************
	// Trace specular rays from the listener
	
	// An implementation-determined constant that counts the minimum cost (in ray casts)
	// for one emitted ray. This is used to account for the overhead associated with each emitted ray.
	const Size minRayCost = 6;
	const Size specularDepth = math::max( maxSpecularDepth, maxDiffractionDepth + 1 );
	
	if ( (specularEnabled || diffractionEnabled) && specularDepth > 0 )
	{
		// Cast as many rays as there is room in the ray budget.
		Size rayCastsRemaining = numSpecularRays*specularDepth;
		threadData.numSpecularRaysCast = 0;
		
		while ( rayCastsRemaining > Size(0) )
		{
			// Create the starting ray for this probe sequence.
			Ray3f ray( listener.getPosition(), getRandomDirection( threadData.randomVariable ) );
			
			Size raysCast = propagateListenerSpecularRay( listener, soundPathCache, ray,
													math::min( specularDepth, rayCastsRemaining ), maxIRLength, threadData );
			
			rayCastsRemaining -= math::min( math::min( math::max( raysCast, minRayCost ), specularDepth ), rayCastsRemaining );
			threadData.numSpecularRaysCast++;
		}
	}
	
	//************************************************************************
	// Trace diffuse rays from the listener if source diffuse is not enabled.
	
	threadData.totalRayDepth = 0;
	
	if ( diffuseEnabled && !request->flags.isSet( PropagationFlags::SOURCE_DIFFUSE ) )
	{
		// Cast as many rays as there is room in the ray budget.
		Size rayCastsRemaining = numDiffuseRays*maxDiffuseDepth;
		threadData.numDiffuseRaysCast = 0;
		
		while ( rayCastsRemaining > Size(0) )
		{
			// Create the starting ray for this probe sequence.
			Ray3f ray( listener.getPosition(), getRandomDirection( threadData.randomVariable ) );
			
			// Bias the ray's starting position by the radius in the ray's direction.
			ray.origin += ray.direction*listener.getRadius();
			
			// Propagate this ray and count the number of rays that were cast.
			Size raysCast = propagateListenerDiffuseRay( listener, ray, math::min( maxDiffuseDepth, rayCastsRemaining ),
													maxIRLength, ray.direction, threadData );
			
			threadData.totalRayDepth += raysCast;
			
			rayCastsRemaining -= math::min( math::min( math::max( raysCast, minRayCost ), maxDiffuseDepth ), rayCastsRemaining );
			threadData.numDiffuseRaysCast++;
		}
	}
	
	// Signal that we are done processing.
	threadData.threadDone++;
	mainThreadSignal.signal();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specular Propagation Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundPropagator:: propagateListenerSpecularRay( const SoundDetector& listener, const SoundPathCache& soundPathCache,
													Ray3f ray, Size numBounces, Float maxIRLength, ThreadData& threadData )
{
	const Bool specularEnabled = request->flags.isSet( PropagationFlags::SPECULAR );
	const Bool diffractionEnabled = request->flags.isSet( PropagationFlags::DIFFRACTION );
	const Bool visibilityCacheEnabled = request->flags.isSet( PropagationFlags::VISIBILITY_CACHE );
	const Bool specularCacheEnabled = request->flags.isSet( PropagationFlags::SPECULAR_CACHE );
	const Size maxDiffractionDepth = request->maxDiffractionDepth;
	const Size numSpecularSamples = request->numSpecularSamples;
	const Real rayOffset = request->rayOffset;
	const Size numSources = sourceDataList.getSize();
	const Real maxDistance = maxIRLength * scene->getMedium().getSpeed();
	
	//****************************************************************************************
	
	SoundPathID& specularPathID = threadData.specularPathID;
	specularPathID.setListener( &listener );
	
	ArrayList<ImagePosition>& imagePositions = threadData.imagePositions;
	Size numInitialImagePositions = imagePositions.getSize();
	Vector3f currentListenerImagePosition = numInitialImagePositions > 0 ? imagePositions.getLast().imagePosition : ray.origin;
	Vector3f directionFromListener;
	Vector3f directionToSource;
	FrequencyBandResponse specularAttenuation;
	Real totalDistance = 0;
	Real specularDistance;
	
	Real closestIntersection;
	ObjectSpaceTriangle closestTriangle;
	Index d;
	
	for ( d = 0; d < numBounces; d++ )
	{
		// Trace the ray through the scene.
		if ( scene->intersectRay( ray, math::max<Real>(), closestIntersection, closestTriangle ) )
		{
			// Transform the closest triangle into world space.
			const WorldSpaceTriangle worldSpaceTriangle( closestTriangle );
			Vector3f normal = worldSpaceTriangle.plane.normal;
			
			// Calculate the intersection point of the ray with the triangle in world space.
			Vector3f intersectionPoint = ray.origin + ray.direction*closestIntersection;
			
			// Compute the dot product of the triangle's normal with the incoming ray's direction.
			Real rayDotNormal = math::dot( ray.direction, normal );
			
			// Flip the normal if it points in the same direction as the ray.
			if ( rayDotNormal > Real(0) )
			{
				normal = -normal;
				rayDotNormal = -rayDotNormal;
			}
			
			// Bias the intersection point by a small amount in order to avoid floating point precision problems.
			intersectionPoint += normal*rayOffset;
			
			// Accumulate the total distance along the path.
			totalDistance += closestIntersection;
			
			// If the total distance is past the end of the IR, stop this ray.
			if ( totalDistance > maxDistance )
				break;
			
			//****************************************************************************************
			// Compute Diffraction Paths
			
			if ( diffractionEnabled && imagePositions.getSize() <= maxDiffractionDepth )
			{
				specularPathID.setSource( NULL );
				
				for ( Index s = 0; s < numSources; s++ )
				{
					const SoundDetector& source = *sourceDataList[s].detector;
					specularPathID.setSource( &source );
					
					const Vector3f& imagePosition = imagePositions.getSize() > 0 ? imagePositions.getLast().imagePosition : listener.getPosition();
					
					addDiffractionPaths( threadData, listener, &soundPathCache, source,
										imagePosition, worldSpaceTriangle, s );
				}
			}
			
			//****************************************************************************************
			// Reflect the ray and apply attenuation coefficients.
			
			// Get the material properties for the intersected triangle.
			const SoundMaterial* material = closestTriangle.triangle->getMaterial();
			
			// Compute the new reflected ray using the material's BRDF.
			ray = Ray3f( intersectionPoint, material->getSpecularReflection( ray.direction, normal ) );
			
			// Apply the attenuation due to this reflection.
			specularAttenuation *= material->getReflectivityBands()*(Real(1) - material->getScatteringBands());
			
			//****************************************************************************************
			// Compute Specular Paths
			
			// Reflect the current listener image position over the intersected triangle
			// and store the result for the next depth.
			currentListenerImagePosition = worldSpaceTriangle.plane.getReflection( currentListenerImagePosition );
			imagePositions.add( ImagePosition( worldSpaceTriangle, currentListenerImagePosition ) );
			
			specularPathID.addPoint( SoundPathPoint( SoundPathPoint::SPECULAR_REFLECTION, closestTriangle, 0 ) );
			
			if ( specularEnabled )
			{
				// If we have not already visited this path, check for any valid
				// reflection paths.
				for ( Index s = 0; s < numSources; s++ )
				{
					const SourceData& sourceData = sourceDataList[s];
					const SoundDetector& source = *sourceData.detector;
					
					// Skip sources that can't form a path because they are on the wrong side of the triangle.
					Vector3f sourceDirection = source.getPosition() - intersectionPoint;
					
					if ( math::dot( sourceDirection, normal ) < Real(0) )
						continue;
					
					// Skip sources that aren't visible to the triangle.
					if ( visibilityCacheEnabled && !sourceDataList[s].visibilityCache->containsTriangle( closestTriangle ) )
						continue;
					
					specularPathID.setSource( &source );
					
					// Check to see if this path is already in the cache. If so, skip it.
					if ( specularCacheEnabled && soundPathCache.containsPath( specularPathID ) )
						continue;
					
					Real visibility;
					
					if ( validateSpecularPath( Sphere3f( source.getPosition(), source.getRadius() ),
												listener.getPosition(), numSpecularSamples,
												specularDistance, directionFromListener, directionToSource, 
												visibility, threadData ) )
					{
						Real relativeSpeed = getRelativeSpeed( listener, directionFromListener, source, directionToSource );
						
						FrequencyBandResponse energy = visibility*getDistanceAttenuation(specularDistance)*specularAttenuation;
						
						if ( sourceData.directivity )
							energy *= sourceData.directivity->getResponse( (-directionToSource)*source.getOrientation() );
						
						threadData.specularPaths.add( SpecularPathData( specularPathID, SoundPathFlags::SPECULAR,
														energy, directionFromListener, -directionToSource,
														specularDistance, relativeSpeed, scene->getMedium().getSpeed(), s ) );
					}
				}
			}
		}
		else
		{
			// Ray didn't hit anything, so stop tracing for this ray.
			break;
		}
	}
	
	specularPathID.clearPoints();
	imagePositions.clear();
	
	return d;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Listener Diffuse Ray Propagation Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundPropagator:: propagateListenerDiffuseRay( const SoundDetector& listener, Ray3f ray, Size numBounces,
													Float maxIRLength,
													const Vector3f& listenerDirection, ThreadData& threadData )
{
	const Bool visibilityCacheEnabled = request->flags.isSet( PropagationFlags::VISIBILITY_CACHE );
	const Size numDiffuseSamples = request->numDiffuseSamples;
	const Real rayOffset = request->rayOffset;
	const Size numSources = sourceDataList.getSize();
	const Real maxDistance = maxIRLength * scene->getMedium().getSpeed();
	const Size maxSpecularDepth = request->flags.isSet( PropagationFlags::SPECULAR ) ? request->maxSpecularDepth : 0;
	
	//************************************************************************
	// Trace diffuse rays from the source
	
#if DIFFUSE_CACHE_ENABLED
	SoundPathID& diffusePathID = threadData.diffusePathID;
#endif // DIFFUSE_CACHE_ENABLED
	
	FrequencyBandResponse diffuseAttenuation;
	FrequencyBandResponse scatteringAttenuation;
	Real totalDistance = 0;
	
	Real intersectionDistance;
	ObjectSpaceTriangle closestTriangle;
	Index d = 0;
	
	for ( ; d < numBounces && totalDistance < maxDistance; d++ )
	{
		// Compute the maximum distance at which a ray intersection can occur, based on the max IR length.
		const Real remainingDistance = maxDistance - totalDistance;
		
		// Trace the ray through the scene.
		if ( scene->intersectRay( ray, remainingDistance, intersectionDistance, closestTriangle ) )
		{
			// Transform the closest triangle's normal into world space.
			Vector3f normal = closestTriangle.object->getTransform().transformToWorld( closestTriangle.triangle->getPlane() ).normal;
			
			// Flip the normal if it points in the same direction as the ray.
			if ( math::dot( ray.direction, normal ) > Real(0) )
				normal = -normal;
			
			// Calculate the intersection point of the ray with the triangle in world space.
			ray.origin += ray.direction*intersectionDistance;
			
			//****************************************************************************************
			
#if DIFFUSE_CACHE_ENABLED
			const WorldSpaceTriangle worldSpaceTriangle( closestTriangle );
			// Compute the barycentric coordinates of this intersection point with respect to the triangle.
			Vector3f barycentric = math::barycentric( worldSpaceTriangle.v1, worldSpaceTriangle.v2,
														worldSpaceTriangle.v3, ray.origin );
			
			// Compute the final barycentric cell index for the intersection point on the triangle.
			SoundPathPoint::IDType barycentricID = computeCellID( *closestTriangle.triangle, barycentric );
			
			// Add a new point to the diffuse path ID for this probe sequence.
			diffusePathID.addPoint( SoundPathPoint( SoundPathPoint::DIFFUSE_REFLECTION, closestTriangle, barycentricID ) );
#endif // DIFFUSE_CACHE_ENABLED
			
			// Bias the intersection point by a small amount in order to avoid floating point precision problems.
			ray.origin += normal*rayOffset;
			
			// Accumulate the total distance along the path so far.
			totalDistance += intersectionDistance;
			
			//****************************************************************************************
			// Reflect the ray and apply attenuation coefficients.
			
			// Get the material properties for the intersected triangle.
			const SoundMaterial* material = closestTriangle.triangle->getMaterial();
			
			// Update the amount of scattering attenuation.
			scatteringAttenuation *= (Real(1) - material->getScatteringBands());
			
			// Compute the attenuation due to non-scattered sound at this reflection depth.
			FrequencyBandResponse inverseScatteringAttenuation;
			
			// If the depth is over the max specular depth, just assume fully diffuse reflections.
			if ( d < maxSpecularDepth )
				inverseScatteringAttenuation = Real(1) - scatteringAttenuation;
			
			// Apply the attenuation due to this reflection.
			diffuseAttenuation *= material->getReflectivityBands();
			
			// Compute the new reflected ray using the material's BRDF.
			ray.direction = material->getReflection( ray.direction, normal, threadData.randomVariable );
			
			//****************************************************************************************
			// Compute Diffuse Paths
			
			// Determine if the reflected ray intersects any sound sources.
			for ( Index s = 0; s < numSources; s++ )
			{
				const SourceData& sourceData = sourceDataList[s];
				const SoundDetector& source = *sourceData.detector;
				
				// Don't sample this source if the path length is too long.
				if ( totalDistance >= sourceData.maxIRDistance )
					continue;
				
				Vector3f sourceDirection = source.getPosition() - ray.origin;
				
				// Skip sources that are on the other side of this triangle.
				if ( math::dot( sourceDirection, normal ) < Real(0) )
					continue;
				
				// If visibility caching is enabled, skip sources that are not very visible to the triangle.
				if ( visibilityCacheEnabled && !sourceDataList[s].visibilityCache->containsTriangle( closestTriangle ) )
					continue;
				
				// Get the visibility factor of the source based on occlusion (between 0 and 1).
				Real sourceVisibility = getDetectorVisibility( source, ray.origin, numDiffuseSamples,
																threadData );
				
				if ( sourceVisibility > Real(0) )
				{
					const Real radiusNormalize = Real(1) / math::square( source.getRadius() );
					const Real sourceDistance = sourceDirection.getMagnitude();
					
					if ( sourceDistance > math::epsilon<Real>() )
						sourceDirection /= sourceDistance;
					
					// Skip this path if it is past the max IR length.
					if ( totalDistance + sourceDistance >= maxDistance )
						continue;
					
					sourceVisibility *= getHemisphereSphereAttenuation( sourceDistance, source.getRadius() );
					sourceVisibility *= material->getDiffuseReflectionProbability( normal, sourceDirection );
					
					FrequencyBandResponse energy = (sourceVisibility*radiusNormalize)*(diffuseAttenuation*inverseScatteringAttenuation);
					
					if ( sourceData.directivity )
						energy *= sourceData.directivity->getResponse( (-sourceDirection)*source.getOrientation() );
					
#if DIFFUSE_CACHE_ENABLED
					threadData.postPath( DiffusePathData( diffusePathID.getHashCode(), energy, listenerDirection, -sourceDirection,
															totalDistance + sourceDistance, 0, s ) );
#else
					threadData.postPath( DiffusePathData( 0, energy, listenerDirection, -sourceDirection,
															totalDistance + sourceDistance, 0, s ) );
#endif
				}
			}
		}
		else
		{
			// Ray didn't hit anything, so stop tracing for this ray.
			break;
		}
	}
	
#if DIFFUSE_CACHE_ENABLED
	// Remove the path ID points that this ray added.
	diffusePathID.clearPoints();
#endif
	
	return d;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specular Cache Update Method
//############		
//##########################################################################################
//##########################################################################################




template < Bool sampledIREnabled, Bool dopplerSortingEnabled >
void SoundPropagator:: outputSpecularPath( const SpecularPathData& path, Float dopplerThreshold, SoundSourceIR& sourceIR )
{
	if ( sampledIREnabled )
	{
		if ( dopplerSortingEnabled )
		{
			// Compute the shifting amount in cents.
			const Float shift = Float(1) + (path.relativeSpeed / path.speedOfSound);
			const Float absShiftCents = math::abs( Float(1200)*math::log2( shift ) );
			
			// Add the cache entry as a path if the shift amount is significant.
			if ( absShiftCents >= dopplerThreshold ||
				path.pathFlags.isSet( SoundPathFlags::DIFFRACTION ) || 
				path.pathFlags.isSet( SoundPathFlags::SPECULAR ) )
			{
				sourceIR.addPath( SoundPath( path.pathHash, path.pathFlags, path.energy,
											path.direction, path.sourceDirection, path.distance,
											path.relativeSpeed, path.speedOfSound ) );
			}
			else
			{
				sourceIR.addImpulse( path.distance / path.speedOfSound, path.energy,
									path.direction, path.sourceDirection );
			}
		}
		else
		{
			sourceIR.addImpulse( path.distance / path.speedOfSound, path.energy,
								path.direction, path.sourceDirection );
		}
	}
	else
	{
		sourceIR.addPath( SoundPath( path.pathHash, path.pathFlags, path.energy,
									path.direction, path.sourceDirection, path.distance,
									path.relativeSpeed, path.speedOfSound ) );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specular Cache Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: updateSpecularCache( SoundPathCache& specularCache, const ArrayList<SpecularPathData>& newPaths, SoundListenerIR& listenerIR )
{
	const Bool sampledIREnabled = request->flags.isSet( PropagationFlags::SAMPLED_IR );
	const Bool dopplerSortingEnabled = request->flags.isSet( PropagationFlags::DOPPLER_SORTING );
	const Float dopplerThreshold = request->dopplerThreshold;
	const Index timeStamp = request->internalData.timeStamp;
	const Size numNewPaths = newPaths.getSize();
	
	for ( Index i = 0; i < numNewPaths; i++ )
	{
		const SpecularPathData& newPath = newPaths[i];
		
		if ( newPath.pathID.getPoint(0).getType() == SoundPathPoint::EDGE_DIFFRACTION )
		{
			SoundPathID pathID;
			pathID.setSource( newPath.pathID.getSource() );
			pathID.setListener( newPath.pathID.getListener() );
			pathID.addPoint( newPath.pathID.getPoint(0) );
			
			SoundPathCache::BucketType& bucket = specularCache.getBucket(pathID.getHashCode() % specularCache.getBucketCount());
			const Size bucketSize = bucket.getSize();
			Index j;
			
			for ( j = 0; j < bucketSize; j++ )
			{
				// If the entry is already in the cache, stop the search.
				if ( bucket[j].pathID == pathID )
					break;
			}
			
			// Add a new entry to the cache if the path was not found.
			if ( j == bucketSize )
			{
				bucket.add( SoundPathCache::Entry( pathID, timeStamp ) );
				
				// Add this new path to the output IR.
				SoundSourceIR& sourceIR = listenerIR.getSourceIR( newPath.sourceIndex );
				
				if ( sampledIREnabled )
				{
					if ( dopplerSortingEnabled )
						outputSpecularPath<true,true>( newPath, dopplerThreshold, sourceIR );
					else
						outputSpecularPath<true,false>( newPath, dopplerThreshold, sourceIR );
				}
				else
					outputSpecularPath<false,false>( newPath, dopplerThreshold, sourceIR );
			}
		}
		else
		{
			SoundPathCache::BucketType& bucket = specularCache.getBucket(newPath.pathHash % specularCache.getBucketCount());
			const Size bucketSize = bucket.getSize();
			Index j;
			
			for ( j = 0; j < bucketSize; j++ )
			{
				// If the entry is already in the cache, stop the search.
				if ( bucket[j].pathID == newPath.pathID )
					break;
			}
			
			// Add a new entry to the cache if the path was not found.
			if ( j == bucketSize )
			{
				bucket.add( SoundPathCache::Entry( newPath.pathID, timeStamp ) );
				
				// Add this new path to the output IR.
				SoundSourceIR& sourceIR = listenerIR.getSourceIR( newPath.sourceIndex );
				
				if ( sampledIREnabled )
				{
					if ( dopplerSortingEnabled )
						outputSpecularPath<true,true>( newPath, dopplerThreshold, sourceIR );
					else
						outputSpecularPath<true,false>( newPath, dopplerThreshold, sourceIR );
				}
				else
					outputSpecularPath<false,false>( newPath, dopplerThreshold, sourceIR );
			}
		}
	}
	
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specular Cache Validation Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: validateSpecularCache( const ListenerData& listenerData, SoundListenerIR& listenerIR )
{
	SoundPathCache& soundPathCache = *listenerData.soundPathCache;
	
	// Make sure the load factor for hash table is ok.
	soundPathCache.checkLoadFactor();
	
	//****************************************************************************************
	// Validate the previously cached paths in parallel.
	
	const Size numThreads = request->numThreads;
	const Size bucketCount = soundPathCache.getBucketCount();
	
	if ( numThreads > 1 )
	{
		// Compute the total number of buckets that should be updated for each thread.
		const Size bucketsPerThread = (Size)math::ceiling( Real(bucketCount) / Real(numThreads) );
		Index bucketStart = 0;
		
		// Update the cache in parallel for each range of buckets in the cache.
		for ( Index i = 0; i < numThreads; i++ )
		{
			ThreadData& threadData = threadDataList[i];
			
			// Compute the number of buckets that this thread should have.
			Size numThreadBuckets = math::min( bucketCount - bucketStart, bucketsPerThread );
			
			threadPool.addJob( FunctionCall< void ( SoundPathCache&, Index, Size, ThreadData& )>(
										bind( &SoundPropagator::validateSpecularCacheRange, this ),
										soundPathCache, bucketStart, numThreadBuckets, threadData ) );
			
			bucketStart += numThreadBuckets;
		}
		
		// Wait for the jobs to finish.
		threadPool.finishJobs();
	}
	else
	{
		// Validate paths on the main thread.
		validateSpecularCacheRange( soundPathCache, 0, bucketCount, threadDataList[0] );
	}
	
	//****************************************************************************************
	// Send the validated paths to the output IRs.
	
	const Bool sampledIREnabled = request->flags.isSet( PropagationFlags::SAMPLED_IR );
	const Bool dopplerSortingEnabled = request->flags.isSet( PropagationFlags::DOPPLER_SORTING );
	const Float dopplerThreshold = request->dopplerThreshold;
	
	for ( Index i = 0; i < numThreads; i++ )
	{
		ThreadData& threadData = threadDataList[i];
		ArrayList<SpecularPathData>& specularPaths = threadData.specularPaths;
		const Size numPaths = specularPaths.getSize();
		
		if ( sampledIREnabled )
		{
			if ( dopplerSortingEnabled )
			{
				for ( Index p = 0; p < numPaths; p++ )
				{
					const SpecularPathData& path = specularPaths[p];
					outputSpecularPath<true,true>( path, dopplerThreshold, listenerIR.getSourceIR( path.sourceIndex ) );
				}
			}
			else
			{
				for ( Index p = 0; p < numPaths; p++ )
				{
					const SpecularPathData& path = specularPaths[p];
					outputSpecularPath<true,false>( path, dopplerThreshold, listenerIR.getSourceIR( path.sourceIndex ) );
				}
			}
		}
		else
		{
			for ( Index p = 0; p < numPaths; p++ )
			{
				const SpecularPathData& path = specularPaths[p];
				outputSpecularPath<false,false>( path, dopplerThreshold, listenerIR.getSourceIR( path.sourceIndex ) );
			}
		}
		
		// Clear the list of paths for next time.
		specularPaths.clear();
	}
}




void SoundPropagator:: validateSpecularCacheRange( internal::SoundPathCache& specularCache, Index bucketStartIndex, Size numBuckets,
													ThreadData& threadData )
{
	//****************************************************************************************
	// Validate previous entries in the cache and output final paths for those entries.
	
	const Size maxPathAge = 0;
	const Index timeStamp = request->internalData.timeStamp;
	const Size numSources = sourceDataList.getSize();
	const Size numSpecularSamples = request->numSpecularSamples;
	const Bool specularEnabled = request->flags.isSet( PropagationFlags::SPECULAR );
	const Bool diffractionEnabled = request->flags.isSet( PropagationFlags::DIFFRACTION );
	
	Vector3f directionFromListener;
	Vector3f directionToSource;
	Real specularDistance;
	ArrayList<ImagePosition>& imagePositions = threadData.imagePositions;
	
	const Index lastBucketIndex = bucketStartIndex + numBuckets;
	
	for ( Index b = bucketStartIndex; b < lastBucketIndex; b++ )
	{
		SoundPathCache::BucketType& bucket = specularCache.getBucket(b);
		
		for ( Index i = 0; i < bucket.getSize(); )
		{
			SoundPathCache::Entry& entry = bucket[i];
			const SoundPathID& pathID = entry.pathID;
			const SoundDetector* source = pathID.getSource();
			const SoundDetector* listener = pathID.getListener();
			Index sourceIndex = math::max<Index>();
			
			// Determine the current index of the source in the output buffer.
			for ( Index s = 0; s < numSources; s++ )
			{
				if ( source == sourceDataList[s].detector )
				{
					sourceIndex = s;
					break;
				}
			}
			
			// Source no longer exists, remove this cache entry.
			if ( sourceIndex == math::max<Index>() )
			{
				bucket.removeAtIndexUnordered(i);
				continue;
			}
			
			// Handle diffraction as a special case.
			if ( pathID.getPoint(0).getType() == SoundPathPoint::EDGE_DIFFRACTION )
			{/*
				if ( validateDiffractionPath( pathID, sourceIndex, threadData ) )
				{
					// Update the time stamp for this entry.
					entry.timeStamp = timeStamp;
				}*/
				if ( diffractionEnabled && addDiffractionPaths( threadData, *listener,
											NULL, *source,
											listener->getPosition(),
											WorldSpaceTriangle( pathID.getPoint(0).getTriangle() ),
											sourceIndex ) )
				{
					// Update the time stamp for this entry.
					entry.timeStamp = timeStamp;
				}
				else
				{
					// Remove this path from the cache since it is no longer valid.
					bucket.removeAtIndexUnordered(i);
					continue;
				}
			}
			else if ( specularEnabled )
			{
				//****************************************************************************************
				// Generate a fake probe path so that we can call the path validation functions.
				
				imagePositions.clear();
				Vector3f listenerImagePosition = listener->getPosition();
				FrequencyBandResponse specularAttenuation;
				
				for ( Index j = 0; j < pathID.getPointCount(); j++ )
				{
					const SoundPathPoint& pathPoint = pathID.getPoint(j);
					
					// Get the reflecting triangle in world space and reflect the listener image position over it.
					WorldSpaceTriangle worldSpaceTriangle( pathPoint.getTriangle() );
					listenerImagePosition = worldSpaceTriangle.plane.getReflection( listenerImagePosition );
					imagePositions.add( ImagePosition( worldSpaceTriangle, listenerImagePosition ) );
					
					// Apply the material attenuation.
					const SoundMaterial* material = worldSpaceTriangle.objectSpaceTriangle.triangle->getMaterial();
					specularAttenuation *= material->getReflectivityBands()*(Real(1) - material->getScatteringBands());
				}
				
				//****************************************************************************************
				// Validate the path.
				
				Bool pathValid = false;
				Real visibility;
				
				if ( validateSpecularPath( Sphere3f( source->getPosition(), source->getRadius() ),
												listener->getPosition(), numSpecularSamples,
												specularDistance, directionFromListener, directionToSource, 
												visibility, threadData ) )
				{
					// A path was found for this sound source.
					// Update the time stamp for this entry.
					entry.timeStamp = timeStamp;
					
					Real relativeSpeed = getRelativeSpeed( *listener, directionFromListener, *source, directionToSource );
					FrequencyBandResponse energy = visibility*getDistanceAttenuation(specularDistance)*specularAttenuation;
					
					if ( sourceDataList[sourceIndex].directivity )
						energy *= sourceDataList[sourceIndex].directivity->getResponse( (-directionToSource)*source->getOrientation() );
					
					threadData.specularPaths.add(
									SpecularPathData( pathID.getHashCode(), SoundPathFlags::SPECULAR,
												energy, directionFromListener, -directionToSource, specularDistance,
												relativeSpeed, scene->getMedium().getSpeed(), sourceIndex ) );
					pathValid = true;
				}
				else
				{
					// Path no longer exists.
					// If the path is older than the threshold amount, remove it.
					if ( timeStamp - entry.timeStamp > maxPathAge )
					{
						bucket.removeAtIndexUnordered(i);
						continue;
					}
				}
			}
			else
			{
				// Path no longer exists.
				// If the path is older than the threshold amount, remove it.
				if ( timeStamp - entry.timeStamp > maxPathAge )
				{
					bucket.removeAtIndexUnordered(i);
					continue;
				}
			}
			
			i++;
		}
	}
	
	imagePositions.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specular Path Validation Method
//############		
//##########################################################################################
//##########################################################################################





Bool SoundPropagator:: validateSpecularPath( const Sphere3f& sourceSphere, const Vector3f& listenerPosition, Size numSamples,
											Real& totalDistance, Vector3f& directionFromListener, Vector3f& directionToSource,
											Real& visibility, ThreadData& threadData )
{
	if ( numSamples <= Size(1) || sourceSphere.radius < math::epsilon<Real>() )
	{
		if ( validatePointSpecularPath( sourceSphere, listenerPosition,
										totalDistance, directionFromListener,
										directionToSource, threadData ) )
		{
			visibility = 1;
			return true;
		}
		
		return false;
	}
	else
	{
		return sampleSpecularPath( sourceSphere, listenerPosition, numSamples,
									totalDistance, directionFromListener, directionToSource,
									visibility, threadData );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Point-Source Specular Path Validation Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPropagator:: validatePointSpecularPath( const Sphere3f& sourceSphere, const Vector3f& listenerPosition,
												Real& totalDistance, Vector3f& directionFromListener, Vector3f& directionToSource,
												ThreadData& threadData )
{
	const Real rayOffset = request->rayOffset;
	const ArrayList<ImagePosition>& imagePositions = threadData.imagePositions;
	
	totalDistance = Real(0);
	
	Vector3f virtualSourcePosition = sourceSphere.position;
	Real virtualSourceRadius = sourceSphere.radius;
	
	// Make sure that the vector from the listener image position to the source passes
	// through the given triangle at each depth.
	for ( Index i = imagePositions.getSize(); i > 0; i-- )
	{
		const WorldSpaceTriangle& triangle = imagePositions[i-1].triangle;
		const Vector3f& listenerImagePosition = imagePositions[i-1].imagePosition;
		
		// Make sure that the listener image position and virtual source position
		// are on opposite sides of the triangle.
		if ( triangle.plane.getSignedDistanceTo( listenerImagePosition )*
			triangle.plane.getSignedDistanceTo( virtualSourcePosition ) >= Real(0) )
			return false;
		
		// Compute the ray from the listener image position to the source.
		Ray3f testRay( virtualSourcePosition, listenerImagePosition - virtualSourcePosition );
		
		// Normalize the ray.
		Real sourceToListenerDistance = testRay.direction.getMagnitude();
		
		if ( sourceToListenerDistance > math::epsilon<Real>() )
			testRay.direction /= sourceToListenerDistance;
		
		// Make sure that the vector from the listener image position to the source passes
		// through the triangle at each depth.
		Real sourceToTriangleDistance;
		
		// If the virtual source is not valid, return.
		if ( !testRay.intersectsTriangle( triangle.v1, triangle.v2, triangle.v3, sourceToTriangleDistance ) )
			return false;
		
		// Trace a ray from this intersection point to the source to make sure that
		// the source is reachable from this location.
		if ( scene->intersectRay( testRay, sourceToTriangleDistance - 2*rayOffset ) )
			return false;
		
		// Calculate the intersection point of this ray with the triangle
		virtualSourcePosition = testRay.origin + testRay.direction*sourceToTriangleDistance;
		
		// Bias the intersection point to avoid precision errors
		// and update the new starting location of the occlusion test ray.
		if ( math::dot( testRay.direction, triangle.plane.normal ) > Real(0) )
			virtualSourcePosition -= rayOffset*triangle.plane.normal;
		else
			virtualSourcePosition += rayOffset*triangle.plane.normal;
		
		testRay.origin = virtualSourcePosition;
		
		// Accumulate the total distance along the path so far.
		totalDistance += sourceToTriangleDistance;
		
		if ( i == imagePositions.getSize() )
		{
			// After the first reflection, set the virtual source's radius to zero because virtual sources have no radius.
			virtualSourceRadius = 0;
			directionToSource = -testRay.direction;
		}
	}
	
	// Calculate the vector from the listener position to the virtual source.
	directionFromListener = virtualSourcePosition - listenerPosition;
	
	Real rayDistance = directionFromListener.getMagnitude();
	directionFromListener /= rayDistance;
	
	if ( scene->intersectRay( Ray3f( listenerPosition, directionFromListener ), rayDistance ) )
		return false;
	
	totalDistance += rayDistance;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Specular Path Sampling Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPropagator:: sampleSpecularPath( const Sphere3f& sourceSphere, const Vector3f& listenerPosition, Size numSamples,
											Real& totalDistance, Vector3f& directionFromListener, Vector3f& directionToSource,
											Real& visibility, ThreadData& threadData )
{
	const Real rayOffset = request->rayOffset;
	const Size numSpecularSamples = request->numSpecularSamples;
	const Size minNumValidRays = 1;
	const ArrayList<ImagePosition>& imagePositions = threadData.imagePositions;
	
	const Size numPoints = imagePositions.getSize();
	
	//*********************************************************************
	// Generate the validation rays from the source in the direction of the listener image.
	
	Array<Ray3f>& validationRays = threadData.validationRays;
	Size numValidRays = 0;
	
	// Make sure there are enough rays.
	if ( validationRays.getSize() < numSpecularSamples )
		validationRays.setSize( numSpecularSamples );
	
	const WorldSpaceTriangle& lastTriangle = imagePositions.getLast().triangle;
	const Vector3f& lastListenerImagePosition = imagePositions.getLast().imagePosition;
	Vector3f sourceDirection = sourceSphere.position - lastListenerImagePosition;
	const Real sourceDistance = sourceDirection.getMagnitude();
	
	if ( sourceDistance > math::epsilon<Real>() )
		sourceDirection /= sourceDistance;
	
	// Compute the angular size of the detector.
	const Real sourceHalfAngle = getSphereHalfAngleSize( sourceDistance, sourceSphere.radius );
	const Real cosHalfAngle = math::cos( sourceHalfAngle );
	
	// Compute the rotation matrix for the direction samples.
	Matrix3f sourceRotation = Matrix3f::planeBasis( sourceDirection );
	Real averageDistance = 0;
	
	// Generate the specular sampling rays from the source.
	for ( Index i = 0; i < numSpecularSamples; i++ )
	{
		// Generate a ray that samples the detectors's visibility.
		Ray3f ray( lastListenerImagePosition,
					(sourceRotation*getRandomDirectionInZCone( threadData.randomVariable, cosHalfAngle )).normalize() );
		
		// Make sure the ray intersects the last triangle reflector.
		Real triangleDistance;
		if ( !ray.intersectsTriangle( lastTriangle.v1, lastTriangle.v2, lastTriangle.v3, triangleDistance ) )
			continue;
		
		// Find the intersection with the source.
		Real sphereDistance;
		if ( !ray.intersectsSphere( sourceSphere, sphereDistance ) )
			continue;
		
		// Reverse the ray from the source.
		ray.origin += ray.direction*sphereDistance;
		ray.direction = -ray.direction;
		
		// Trace a ray through the scene to make sure there is no occluder.
		Real rayDistance = sphereDistance - triangleDistance;
		if ( scene->intersectRay( ray, rayDistance - 2*rayOffset ) )
			continue;
		
		// Compute the reflected ray.
		// Only update the origin because we can compute the direction later with better accuracy.
		ray.origin = ray.origin + ray.direction*rayDistance;
		
		if ( math::dot( ray.direction, lastTriangle.plane.normal ) > Real(0) )
			ray.origin -= rayOffset*lastTriangle.plane.normal;
		else
			ray.origin += rayOffset*lastTriangle.plane.normal;
		
		averageDistance += rayDistance;
		
		// Store the reflected ray in the list of validation rays.
		validationRays[numValidRays] = ray;
		numValidRays++;
	}
	
	// Make sure there are enough valid rays after the first sampling.
	if ( numValidRays < minNumValidRays )
		return false;
	
	totalDistance = averageDistance / Real(numValidRays);
	Vector3f sourceImagePosition = lastTriangle.plane.getReflection( sourceSphere.position );
	
	//*********************************************************************
	// Recursively sample the visibility back to the listener.
	
	for ( Index i = numPoints - 1; i > 0; i-- )
	{
		const ImagePosition& image = imagePositions[i-1];
		const WorldSpaceTriangle& triangle = image.triangle;
		const Vector3f& listenerImagePosition = image.imagePosition;
		
		// Make sure that the listener image position and virtual source position
		// are on opposite sides of the triangle. If not, this path is not valid.
		if ( triangle.plane.getSignedDistanceTo( listenerImagePosition )*
			triangle.plane.getSignedDistanceTo( sourceImagePosition ) > Real(0) )
			return false;
		
		// Compute the next source image position.
		sourceImagePosition = triangle.plane.getReflection( sourceImagePosition );
		
		Real averageDistance = 0;
		
		// For each validation ray, update its direction and check the visiblity of the next path segment.
		for ( Index j = 0; j < numValidRays; )
		{
			Ray3f& ray = validationRays[j];
			ray.direction = (listenerImagePosition - ray.origin).normalize();
			Real rayDistance;
			
			// Make sure the ray intersects the triangle at this depth.
			// Make sure the path along the ray to the triangle is clear.
			if ( !ray.intersectsTriangle( triangle.v1, triangle.v2, triangle.v3, rayDistance ) ||
				scene->intersectRay( ray, rayDistance - 2*rayOffset ) )
			{
				// Swap this ray with the last and reduce the number of valid rays.
				numValidRays--;
				
				// If no rays are valid, this path is not valid.
				if ( numValidRays < minNumValidRays )
					return false;
				
				ray = validationRays[numValidRays];
				continue;
			}
			
			// Compute the next ray origin.
			ray.origin += ray.direction*rayDistance;
			
			// Bias the intersection point to avoid precision errors
			// and update the new starting location of the occlusion test ray.
			if ( math::dot( ray.direction, triangle.plane.normal ) > Real(0) )
				ray.origin -= rayOffset*triangle.plane.normal;
			else
				ray.origin += rayOffset*triangle.plane.normal;
			
			averageDistance += rayDistance;
			j++;
		}
		
		// Accumulate the total average distance along the path.
		totalDistance += (averageDistance / Real(numValidRays));
	}
	
	//*********************************************************************
	// Compute the final visibility of the listener from the last reflecting triangle.
	
	const Size numFinalValidRays = numValidRays;
	averageDistance = 0;
	
	for ( Index i = 0; i < numFinalValidRays; i++ )
	{
		Ray3f& ray = validationRays[i];
		Real rayDistance;
		ray.direction = (listenerPosition - ray.origin).normalize( rayDistance );
		
		// Make sure the path along the ray to the listener is clear.
		if ( scene->intersectRay( ray, rayDistance - 2*rayOffset ) )
		{
			numValidRays--;
			
			// If no rays are valid, this path is not valid.
			if ( numValidRays < minNumValidRays )
				return false;
			
			continue;
		}
		
		averageDistance += rayDistance;
	}
	
	if ( numValidRays < minNumValidRays )
		return false;
	
	visibility = Real(numValidRays) / Real(numSpecularSamples);
	totalDistance += (averageDistance / Real(numValidRays));
	directionToSource = sourceDirection;
	directionFromListener = (sourceImagePosition - listenerPosition).normalize();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Diffuse Path Output Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: outputDiffusePaths( const ArrayList<DiffusePathData>& newPaths, SoundSourceIR& sourceIR )
{
	const Bool sampledIREnabled = request->flags.isSet( PropagationFlags::SAMPLED_IR );
	const Bool airAbsorption = request->flags.isSet( PropagationFlags::AIR_ABSORPTION );
	const Size numNewPaths = newPaths.getSize();
	const SoundMedium& medium = scene->getMedium();
	const Real speedOfSound = medium.getSpeed();
	
	if ( sampledIREnabled )
	{
		for ( Index i = 0; i < numNewPaths; i++ )
		{
			const DiffusePathData& pathData = newPaths[i];
			Real delay = pathData.distance / speedOfSound;
			
			sourceIR.addImpulse( delay, (airAbsorption ? medium.getAttenuation( pathData.distance )*pathData.energy : pathData.energy),
								pathData.direction, pathData.sourceDirection );
		}
	}
	else
	{
		for ( Index i = 0; i < numNewPaths; i++ )
		{
			const DiffusePathData& pathData = newPaths[i];
			
			sourceIR.addPath( SoundPath( pathData.pathHash, SoundPathFlags::DIFFUSE,
										(airAbsorption ? medium.getAttenuation( pathData.distance )*pathData.energy : pathData.energy),
										pathData.direction, pathData.sourceDirection, pathData.distance,
										0, speedOfSound ) );
		}
	}
}




void SoundPropagator:: outputDiffusePaths( const ArrayList<DiffusePathData>& newPaths, SoundListenerIR& listenerIR )
{
	const Bool sampledIREnabled = request->flags.isSet( PropagationFlags::SAMPLED_IR );
	const Bool airAbsorption = request->flags.isSet( PropagationFlags::AIR_ABSORPTION );
	const Size numNewPaths = newPaths.getSize();
	const SoundMedium& medium = scene->getMedium();
	const Real speedOfSound = medium.getSpeed();
	
	if ( sampledIREnabled )
	{
		for ( Index i = 0; i < numNewPaths; i++ )
		{
			const DiffusePathData& pathData = newPaths[i];
			SoundSourceIR& sourceIR = listenerIR.getSourceIR(pathData.sourceIndex);
			const Real delay = pathData.distance / speedOfSound;
			
			sourceIR.addImpulse( delay, (airAbsorption ? medium.getAttenuation( pathData.distance )*pathData.energy : pathData.energy),
								pathData.direction, pathData.sourceDirection );
		}
	}
	else
	{
		for ( Index i = 0; i < numNewPaths; i++ )
		{
			const DiffusePathData& pathData = newPaths[i];
			SoundSourceIR& sourceIR = listenerIR.getSourceIR(pathData.sourceIndex);
			
			sourceIR.addPath( SoundPath( pathData.pathHash, SoundPathFlags::DIFFUSE,
										(airAbsorption ? medium.getAttenuation( pathData.distance )*pathData.energy : pathData.energy),
										pathData.direction, pathData.sourceDirection, pathData.distance,
										0, speedOfSound ) );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Diffuse Cache Update Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: updateDiffuseCache( internal::DiffusePathCache& diffuseCache, const ArrayList<DiffusePathData>& newPaths )
{
	const Size numNewPaths = newPaths.getSize();
	const Index timeStamp = request->internalData.timeStamp;
	
	for ( Index i = 0; i < numNewPaths; i++ )
	{
		const DiffusePathData& path = newPaths[i];
		
		// Add the path's contribution to the cache.
		diffuseCache.addContribution( path.pathHash, path.energy,
									path.direction, path.sourceDirection, path.distance,
									path.relativeSpeed, timeStamp );
	}
}




void SoundPropagator:: updateDiffuseCaches( const ArrayList<DiffusePathData>& newPaths )
{
	const Size numNewPaths = newPaths.getSize();
	const Index timeStamp = request->internalData.timeStamp;
	
	for ( Index i = 0; i < numNewPaths; i++ )
	{
		const DiffusePathData& path = newPaths[i];
		DiffusePathCache& diffuseCache = *sourceDataList[path.sourceIndex].diffuseCache;
		
		// Add the path's contribution to the cache.
		diffuseCache.addContribution( path.pathHash, path.energy,
									path.direction, path.sourceDirection, path.distance,
									path.relativeSpeed, timeStamp );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Diffuse Cache Output Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: outputDiffuseCache( internal::DiffusePathCache& diffuseCache, Size numDiffuseRaysCast, SoundSourceIR& sourceIR )
{
	const Bool sampledIREnabled = request->flags.isSet( PropagationFlags::SAMPLED_IR );
	const Bool dopplerSortingEnabled = request->flags.isSet( PropagationFlags::DOPPLER_SORTING );
	const Index timeStamp = request->internalData.timeStamp;
	
	// Determine the maximum age of a path in frames.
	Size maxPathAge = 10;
	
	if ( request->dt > Real(0) )
		maxPathAge = (Size)math::ceiling( request->responseTime / request->dt );
	else
		maxPathAge = (Size)math::ceiling( request->responseTime / request->targetDt );
	
	const Size minPathRays = maxPathAge*numDiffuseRaysCast;
	
	// Get the propagation medium for the scene.
	const SoundMedium& medium = scene->getMedium();
	
	// Check the load factor for the diffuse cache.
	diffuseCache.checkLoadFactor();
	
	//****************************************************************************************
	// Iterate over the cache buckets and output an impulse or path for each cache entry.
	
	const Size numBuckets = diffuseCache.getBucketCount();
	
	if ( sampledIREnabled )
	{
		if ( dopplerSortingEnabled )
		{
			for ( Index b = 0; b < numBuckets; b++ )
			{
				DiffusePathCache::BucketType& bucket = diffuseCache.getBucket(b);
				
				for ( Index i = 0; i < bucket.getSize(); )
				{
					DiffusePathInfo& pathInfo = bucket[i];
					
					// Update the total number of rays that have been traced while this path was valid.
					pathInfo.setTotalRayCount( pathInfo.getTotalRayCount() + numDiffuseRaysCast );
					
					// Determine if this path should be removed based on its age.
					if ( timeStamp - pathInfo.getTimeStamp() > maxPathAge )
					{
						bucket.removeAtIndexUnordered(i);
						continue;
					}
					
					const Size totalRays = math::max( minPathRays, (Size)pathInfo.getTotalRayCount() );
					const Real inverseNumRays = Real(1) / Real(pathInfo.getRayCount());
					
					// Compute the average direction, distance and frequency band response for this path.
					const Real distance = pathInfo.getDistance() * inverseNumRays;
					const Real delay = distance / medium.getSpeed();
					const FrequencyBandResponse energy = medium.getAttenuation(distance) * pathInfo.getResponse() *
														(Real(1) / (Float(4)*math::pi<Float>()*Float(totalRays)));
					const Vector3f direction = pathInfo.getDirection().normalize();
					const Vector3f sourceDirection = pathInfo.getSourceDirection().normalize();
					const Real relativeSpeed = pathInfo.getRelativeSpeed() * inverseNumRays;
					
					//****************************************************************************************
					// Determine if this diffuse path should be shifted or not.
					
					// Compute the shifting amount.
					const Float shift = Float(1) + (relativeSpeed / medium.getSpeed());
					
					// Convert to cents.
					const Float absShiftCents = math::abs( Float(1200)*math::log2( shift ) );
					
					// Add the cache entry as a path if the shift amount is significant.
					if ( absShiftCents >= request->dopplerThreshold )
					{
						sourceIR.addPath( SoundPath( pathInfo.getHashCode(), SoundPathFlags::DIFFUSE,
													energy, direction, sourceDirection, distance,
													relativeSpeed, medium.getSpeed() ) );
					}
					else
						sourceIR.addImpulse( delay, energy, direction, sourceDirection );
					
					// Move to the next path in this bucket.
					i++;
				}
			}
		}
		else
		{
			for ( Index b = 0; b < numBuckets; b++ )
			{
				DiffusePathCache::BucketType& bucket = diffuseCache.getBucket(b);
				
				for ( Index i = 0; i < bucket.getSize(); )
				{
					DiffusePathInfo& pathInfo = bucket[i];
					
					// Update the total number of rays that have been traced while this path was valid.
					pathInfo.setTotalRayCount( pathInfo.getTotalRayCount() + numDiffuseRaysCast );
					
					// Determine if this path should be removed based on its age.
					if ( timeStamp - pathInfo.getTimeStamp() > maxPathAge )
					{
						bucket.removeAtIndexUnordered(i);
						continue;
					}
					
					const Size totalRays = math::max( minPathRays, (Size)pathInfo.getTotalRayCount() );
					const Real inverseNumRays = Real(1) / Real(pathInfo.getRayCount());
					
					// Compute the average direction, distance and frequency band response for this path.
					const Real distance = pathInfo.getDistance() * inverseNumRays;
					const Real delay = distance / medium.getSpeed();
					const FrequencyBandResponse energy = medium.getAttenuation(distance) * pathInfo.getResponse() *
														(Real(1) / (Float(4)*math::pi<Float>()*Float(totalRays)));
					const Vector3f direction = pathInfo.getDirection().normalize();
					const Vector3f sourceDirection = pathInfo.getSourceDirection().normalize();
					
					//****************************************************************************************
					
					// Add the path to the output buffer.
					sourceIR.addImpulse( delay, energy, direction, sourceDirection );
					
					// Move to the next path in this bucket.
					i++;
				}
			}
		}
	}
	else
	{
		// Output the diffuse cache as paths only.
		for ( Index b = 0; b < numBuckets; b++ )
		{
			DiffusePathCache::BucketType& bucket = diffuseCache.getBucket(b);
			
			for ( Index i = 0; i < bucket.getSize(); )
			{
				DiffusePathInfo& pathInfo = bucket[i];
				
				// Update the total number of rays that have been traced while this path was valid.
				pathInfo.setTotalRayCount( pathInfo.getTotalRayCount() + numDiffuseRaysCast );
				
				// Determine if this path should be removed based on its age.
				if ( timeStamp - pathInfo.getTimeStamp() > maxPathAge )
				{
					bucket.removeAtIndexUnordered(i);
					continue;
				}
				
				const Size totalRays = math::max( minPathRays, (Size)pathInfo.getTotalRayCount() );
				const Real inverseNumRays = Real(1) / Real(pathInfo.getRayCount());
				
				// Compute the average direction, distance and frequency band response for this path.
				const Real distance = pathInfo.getDistance() * inverseNumRays;
				const FrequencyBandResponse energy = medium.getAttenuation(distance) * pathInfo.getResponse() *
													(Real(1) / (Float(4)*math::pi<Float>()*Float(totalRays)));
				const Vector3f direction = pathInfo.getDirection().normalize();
				const Vector3f sourceDirection = pathInfo.getSourceDirection().normalize();
				const Real relativeSpeed = pathInfo.getRelativeSpeed() * inverseNumRays;
				
				//****************************************************************************************
				// Determine if this diffuse path should be shifted or not.
				
				sourceIR.addPath( SoundPath( pathInfo.getHashCode(), SoundPathFlags::DIFFUSE,
												energy, direction, sourceDirection, distance,
												relativeSpeed, medium.getSpeed() ) );
				
				// Move to the next path in this bucket.
				i++;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		IR Cache Update Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: updateIRCache( SoundSourceIR& sourceIR, const ArrayList<DiffusePathData>& newPaths )
{
	// Get the propagation medium for the scene.
	const SoundMedium& medium = scene->getMedium();
	const Real speedOfSound = medium.getSpeed();
	const Size numNewPaths = newPaths.getSize();
	const Bool airAbsorption = request->flags.isSet( PropagationFlags::AIR_ABSORPTION );
	
	for ( Index i = 0; i < numNewPaths; i++ )
	{
		const DiffusePathData& path = newPaths[i];
		
		// Add the path's contribution to the source IR.
		sourceIR.addImpulse( path.distance / speedOfSound,
							(airAbsorption ? medium.getAttenuation( path.distance )*path.energy : path.energy),
							path.direction, path.sourceDirection );
	}
}




void SoundPropagator:: updateIRCaches( const ArrayList<DiffusePathData>& newPaths )
{
	// Get the propagation medium for the scene.
	const SoundMedium& medium = scene->getMedium();
	const Real speedOfSound = medium.getSpeed();
	const Size numNewPaths = newPaths.getSize();
	const Bool airAbsorption = request->flags.isSet( PropagationFlags::AIR_ABSORPTION );
	
	for ( Index i = 0; i < numNewPaths; i++ )
	{
		const DiffusePathData& path = newPaths[i];
		SoundSourceIR& sourceIR = *sourceDataList[path.sourceIndex].outputIR;
		
		// Add the path's contribution to the source IR.
		sourceIR.addImpulse( path.distance / speedOfSound,
							(airAbsorption ? medium.getAttenuation( path.distance )*path.energy : path.energy),
							path.direction, path.sourceDirection );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		IR Cache Output Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: outputIRCache( internal::IRCache& irCache, Size numDiffuseRaysCast, SoundSourceIR& sourceIR )
{
	// The threshold where a path is considered no longer contributing.
	const Real threshold = Real(0.0001);
	
	// Determine the maximum age of a path in frames.
	Real maxPathAge = request->responseTime / (request->dt > Real(0) ? request->dt : request->targetDt);
	
	// Make sure we average for at least a few frames.
	maxPathAge = math::max( maxPathAge, Real(10) );
	
	// Compute the blend factor necessary to update the IR cache with the desired response time.
	const Float blendFactor = Real(1) - math::pow( threshold, Real(1) / Real(maxPathAge) );
	
	// Compute the gain factor to apply to the IR.
	const Float gainFactor = Float(1) / (Float(numDiffuseRaysCast));
	
	// Update the cache and get the output IR in the source IR.
	irCache.update( blendFactor, gainFactor, sourceIR.getSampledIR() );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Source Propagation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: doSourcesPropagation( const SoundDetector& listener, SoundListenerIR& listenerIR )
{
	const Bool diffuseEnabled = request->flags.isSet( PropagationFlags::DIFFUSE );
	const Size maxDiffuseDepth = request->maxDiffuseDepth;
	const Size numDiffuseRays = request->numDiffuseRays;
	const Size numThreads = request->numThreads;
	const Size numSources = sourceDataList.getSize();
	
	// Do sound propagation for each source.
	for ( Index s = 0; s < numSources; s++ )
		doSourcePropagation( listener, s, maxDiffuseDepth, numDiffuseRays );
	
	//************************************************************************
	// Compute the output IR for each sound source in parallel based on the diffuse cache content.
	
	if ( diffuseEnabled )
	{
		if ( numThreads > 1 )
		{
			for ( Index s = 0; s < numSources; s++ )
			{
				IRCache& irCache = *sourceDataList[s].irCache;
				SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
				Size numDiffuseRaysCast = sourceDataList[s].numDiffuseRaysCast;
				
				threadPool.addJob( FunctionCall< void ( IRCache&, Size, SoundSourceIR& )>(
											bind( &SoundPropagator::outputIRCache, this ),
											irCache, numDiffuseRaysCast, sourceIR ) );
			}
			
			// Wait for the cache update jobs to finish.
			threadPool.finishJobs();
		}
		else
		{
			for ( Index s = 0; s < numSources; s++ )
			{
				IRCache& irCache = *sourceDataList[s].irCache;
				SoundSourceIR& sourceIR = *sourceDataList[s].outputIR;
				Size numDiffuseRaysCast = sourceDataList[s].numDiffuseRaysCast;
				
				outputIRCache( irCache, numDiffuseRaysCast, sourceIR );
			}
		}
	}
}




void SoundPropagator:: doSourcePropagation( const SoundDetector& listener, Index sourceIndex,
											Size maxDiffuseDepth, Size numDiffuseRays )
{
	const Size numThreads = request->numThreads;
	
	SourceData& sourceData = sourceDataList[sourceIndex];
	const SoundDetector& source = *sourceData.detector;
	SoundSourceIR& sourceIR = *sourceData.outputIR;;
	
	//************************************************************************
	// Determine what the maximum IR length for the source should be.
	
	Float maxIRLength = request->maxIRLength;
	
	if ( request->flags.isSet( PropagationFlags::IR_THRESHOLD ) &&
		request->flags.isSet( PropagationFlags::ADAPTIVE_IR_LENGTH ) )
	{
		Float previousLength = sourceData.sourceData->maxIRLength;
		Float growth = request->irGrowthRate*request->dt;
		
		maxIRLength = math::clamp( previousLength + growth, request->minIRLength, request->maxIRLength );
	}
	
	//************************************************************************
	// Trace diffuse rays from the source
	
	if ( numThreads > Size(1) )
	{
		// Compute the total number of rays that should be traced for each thread.
		const Size raysPerThread = (Size)math::ceiling( Real(numDiffuseRays) / Real(numThreads) );
		Index rayStart = 0;
		
		// Queue the diffuse jobs for all threads.
		for ( Index i = 0; i < numThreads; i++ )
		{
			ThreadData& threadData = threadDataList[i];
			
			// Compute the number of rays that this thread should have.
			const Size numDiffuseThreadRays = math::min( numDiffuseRays - rayStart, raysPerThread );
			
			// Queue the job for this range of rays.
			threadPool.addJob( FunctionCall< void ( const SoundDetector&, const SoundDetector&, Size, Size, Float, ThreadData& )>(
										bind( &SoundPropagator::propagateSourceRays, this ),
										source, listener, maxDiffuseDepth, numDiffuseThreadRays, maxIRLength, threadData ) );
			
			rayStart += numDiffuseThreadRays;
		}
		
		Bool processing = true;
		
		while ( processing )
		{
			// Process any new path buffers.
			for ( Index i = 0; i < numThreads; i++ )
			{
				ThreadData& threadData = threadDataList[i];
				
				// Process new diffuse paths.
				if ( threadData.diffuseBufferFilled )
				{
					ArrayList<DiffusePathData>& newDiffusePaths = threadData.diffusePaths[!threadData.diffuseBufferIndex];
					
					// Update the caches with the new paths.
					updateIRCache( sourceIR, newDiffusePaths );
					
					// Clear the list of paths for next time.
					newDiffusePaths.clear();
					
					// Signal to the thread that the path buffer has been consumed.
					threadData.diffuseBufferFilled--;
				}
			}
			
			// Give up the CPU since we have finished consuming all new paths.
			Thread::yield();
			
			// Check to see if all of the threads are done.
			processing = false;
			
			for ( Index i = 0; i < numThreads; i++ )
			{
				if ( !threadDataList[i].threadDone )
					processing = true;
			}
		}
		
		// Wait for the ray tracing jobs to finish.
		threadPool.finishJobs();
	}
	else
	{
		// Do all diffuse propagation on the main thread to avoid switching contexts.
		propagateSourceRays( source, listener, maxDiffuseDepth, numDiffuseRays, maxIRLength, threadDataList[0] );
	}
	
	
	//************************************************************************
	// Consume the final set of output paths.
	
	// A count of the number of diffuse rays that were cast this frame.
	sourceData.numDiffuseRaysCast = 0;
	
	for ( Index i = 0; i < numThreads; i++ )
	{
		ThreadData& threadData = threadDataList[i];
		
		// Check both output buffers for paths.
		for ( Index bufferIndex = 0; bufferIndex < 2; bufferIndex++ )
		{
			ArrayList<DiffusePathData>& newDiffusePaths = threadData.diffusePaths[bufferIndex];
			
			if ( newDiffusePaths.getSize() > 0 )
			{
				// Update the caches with the new paths.
				updateIRCache( sourceIR, newDiffusePaths );
				
				// Clear the list of paths for next time.
				newDiffusePaths.clear();
			}
		}
		
		// Make sure the atomic variables are in the correct state for next time.
		if  ( threadData.diffuseBufferFilled )
			threadData.diffuseBufferFilled--;
		
		if  ( threadData.threadDone )
			threadData.threadDone--;
		
		// Count the number of diffuse rays that were cast this frame.
		sourceData.numDiffuseRaysCast += threadDataList[i].numDiffuseRaysCast;
	}
}




void SoundPropagator:: propagateSourceRays( const SoundDetector& source, const SoundDetector& listener,
										Size maxDiffuseDepth, Size numDiffuseRays, Float maxIRLength, ThreadData& threadData )
{
	//************************************************************************
	// Trace diffuse rays from the source
	
	Size rayCastsRemaining = numDiffuseRays*maxDiffuseDepth;
	threadData.numDiffuseRaysCast = 0;
	
	while ( rayCastsRemaining > Size(0) )
	{
		// Create the starting ray for this probe sequence.
		Ray3f ray( source.getPosition(), getRandomDirection( threadData.randomVariable ) );
		
		// Bias the ray's starting position by the source's radius.
		ray.origin += source.getRadius()*ray.direction;
		
		rayCastsRemaining -= propagateSourceDiffuseRay( listener, ray, math::min( maxDiffuseDepth, rayCastsRemaining ),
														maxIRLength, ray.direction, threadData );
		threadData.numDiffuseRaysCast++;
	}
	
	// Signal that we are done processing.
	threadData.threadDone++;
}




Size SoundPropagator:: propagateSourceDiffuseRay( const SoundDetector& detector, Ray3f ray, Size numBounces,
												Float maxIRLength,
												const Vector3f& sourceDirection, ThreadData& threadData )
{
	const Size numDiffuseSamples = request->numDiffuseSamples;
	const Real rayOffset = request->rayOffset;
	const Real radiusNormalize = Real(1) / math::square( detector.getRadius() );
	const Real maxDistance = request->maxIRLength * scene->getMedium().getSpeed();
	
	//************************************************************************
	// Trace diffuse rays from the source
	
	FrequencyBandResponse reflectionAttenuation;
	FrequencyBandResponse scatteringAttenuation;
	Real totalDistance = 0;
	Real instersectionDistance;
	ObjectSpaceTriangle closestTriangle;
	Index d;
	
	for ( d = 0; d < numBounces; d++ )
	{
		const Real remainingDistance = maxDistance - totalDistance;
		
		// Trace the ray through the scene.
		if ( scene->intersectRay( ray, remainingDistance, instersectionDistance, closestTriangle ) )
		{
			// Transform the closest triangle into world space.
			const WorldSpaceTriangle worldSpaceTriangle( closestTriangle );
			Vector3f normal = worldSpaceTriangle.plane.normal;
			
			// Calculate the intersection point of the ray with the triangle in world space.
			Vector3f intersectionPoint = ray.origin + ray.direction*instersectionDistance;
			
			// Compute the dot product of the triangle's normal with the incoming ray's direction.
			Real rayDotNormal = math::dot( ray.direction, normal );
			
			// Flip the normal if it points in the same direction as the ray.
			if ( rayDotNormal > Real(0) )
			{
				normal = -normal;
				rayDotNormal = -rayDotNormal;
			}
			
			// Bias the intersection point by a small amount in order to avoid floating point precision problems.
			intersectionPoint += normal*rayOffset;
			
			// Accumulate the total distance along the path so far.
			totalDistance += instersectionDistance;
			
			// If the total distance is past the end of the IR, stop this ray.
			if ( totalDistance > maxDistance )
				break;
			
			//****************************************************************************************
			
			// Get the material properties for the intersected triangle.
			const SoundMaterial* material = closestTriangle.triangle->getMaterial();
			
			// Apply the attenuation due to this reflection.
			reflectionAttenuation *= material->getReflectivityBands();
			
			// Update the amount of scattering attenuation.
			scatteringAttenuation *= (Real(1) - material->getScatteringBands());

			// This is necessary because of alignment problems with SIMD. FIX ME.
			FrequencyBandResponse inverseScatteringAttenuation;
			/*
			if ( d < request->maxSpecularDepth )
			{
				for ( Index b = 0; b < scatteringAttenuation.getBandCount(); b++ )
					inverseScatteringAttenuation[b] = Real(1) - scatteringAttenuation[b];
			}
			*/
			//****************************************************************************************
			
			// Compute the new reflected ray.
			ray = Ray3f( intersectionPoint, material->getReflection( ray.direction, normal, threadData.randomVariable ) );
			
			// Determine if the reflected ray intersects the listener.
			Vector3f listenerDirection = detector.getPosition() - intersectionPoint;
			
			// Skip listeners that are on the other side of this triangle.
			if ( math::dot( listenerDirection, normal ) < Real(0) )
				continue;
			
			// Get the visibility factor of the listener based on occlusion (between 0 and 1).
			Real listenerVisibility = getDetectorVisibility( detector, intersectionPoint, numDiffuseSamples,
															threadData );
			
			// If the listener is visible, output a diffuse rain sample.
			if ( listenerVisibility > Real(0) )
			{
				const Real listenerDistance = listenerDirection.getMagnitude();
				
				if ( listenerDistance > math::epsilon<Real>() )
					listenerDirection /= listenerDistance;
				
				// Skip this path if it is past the max IR length.
				if ( totalDistance + listenerDistance >= maxDistance )
					continue;
				
				listenerVisibility *= getHemisphereSphereAttenuation( listenerDistance, detector.getRadius() );
				listenerVisibility *= material->getDiffuseReflectionProbability( normal, listenerDirection );
				
				threadData.postPath(
								DiffusePathData( 0, (listenerVisibility*radiusNormalize)*reflectionAttenuation*inverseScatteringAttenuation,
												-listenerDirection, sourceDirection,
												totalDistance + listenerDistance, 0,
												0 ) );
			}
		}
		else
		{
			// Ray didn't hit anything, so stop tracing for this ray.
			break;
		}
	}
	
	return d;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Source Visibility Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: updateSourcesVisibility()
{
	Timer visibilityTimer;
	
	const Size numVisibilityRays = request->numVisibilityRays;
	const Size numSources = sourceDataList.getSize();
	
	for ( Index s = 0; s < numSources; s++ )
	{
		SourceData& sourceData = sourceDataList[s];
		const SoundDetector& source = *sourceData.detector;
		VisibilityCache* visibilityCache = sourceData.visibilityCache;
		
		if ( !visibilityCache )
			continue;
		
		threadPool.addJob( FunctionCall< void ( const Vector3f&, Real, Size, VisibilityCache& )>(
										bind( &SoundPropagator::updateVisibility, this ),
										source.getPosition(), source.getRadius(), numVisibilityRays, *visibilityCache ) );
	}
	
	// Wait until the visibility for all sources has been updated.
	threadPool.finishJobs();
	
	Time visibilityTime = visibilityTimer.getElapsedTime();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Source Visibility Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: updateVisibility( const Vector3f& position, Real radius, Size numVisibilityRays, VisibilityCache& visibilityCache )
{
	ThreadData& threadData = threadDataList[threadPool.getCurrentThreadIndex()];
	const Index timeStamp = request->internalData.timeStamp;
	
	Ray3f ray( position, Vector3f() );
	
	Real closestIntersection;
	ObjectSpaceTriangle closestTriangle;
	
	for ( Index i = 0; i < numVisibilityRays; i++ )
	{
		// Create the starting ray for this probe sequence.
		ray.direction = getRandomDirection( threadData.randomVariable );
		
		// Bias the ray's starting position by the source's radius.
		ray.origin = position + radius*ray.direction;
		
		// Trace the ray through the scene and get the intersected triangle.
		if ( scene->intersectRay( ray, math::max<Real>(), closestIntersection, closestTriangle ) )
		{
			// Add the triangle to the cache.
			visibilityCache.addTriangle( closestTriangle, timeStamp );
		}
	}
	
	// Check the load factor for the cache.
	visibilityCache.checkLoadFactor();
	
	//*********************************************************************
	// Remove old triangles from the visibility cache.
	
	// Determine the maximum age of a triangle in frames.
	Size maxTriangleAge = 1;
	
	if ( request->dt > Real(0) )
		maxTriangleAge = (Size)math::ceiling( request->visibilityCacheTime / request->dt );
	else
		maxTriangleAge = (Size)math::ceiling( request->visibilityCacheTime / request->targetDt );
	
	visibilityCache.removeOldTriangles( timeStamp, maxTriangleAge );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Direct Propagation Path Computation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: addDirectPaths( const SoundListener& listener, SoundListenerIR& listenerIR, ThreadData& threadData )
{
	const Bool directEnabled = request->flags.isSet( PropagationFlags::DIRECT );
	const Bool sampledIREnabled = request->flags.isSet( PropagationFlags::SAMPLED_IR );
	const Bool dopplerSortingEnabled = request->flags.isSet( PropagationFlags::DOPPLER_SORTING );
	//const Bool transmissionEnabled = request->flags.isSet( PropagationFlags::TRANSMISSION );
	
	const Vector3f& listenerPosition = listener.getPosition();
	Ray3f validationRay( listenerPosition, Vector3f() );
	const Size numSources = sourceDataList.getSize();
	SoundPathID& pathID = threadData.specularPathID;
	Vector3f averageDirection;
	
	// Shoot out direct rays from the listener to each source.
	for ( Index s = 0; s < numSources; s++ )
	{
		const SoundDetector& source = *sourceDataList[s].detector;
		const Vector3f& sourcePosition = source.getPosition();
		
		pathID.clearPoints();
		pathID.setSource( &source );
		pathID.setListener( &listener );
		
		Vector3f sourceDirection = sourcePosition - listenerPosition;
		Real sourceDistance = sourceDirection.getMagnitude();
		
		if ( directEnabled )
		{
			// Compute the angular size of the source.
			const Real distSquared = math::square(sourceDistance) - math::square(source.getRadius());
			const Real sourceHalfAngle = math::acos( (distSquared > Real(0) ? math::sqrt(distSquared) : Real(0)) / sourceDistance );
			
			// Compute the number of rays to trace for this source, based on the angular size of the source.
			const Size numDirectRays = (Size)math::max( request->numDirectRays*math::sqrt( math::sin( sourceHalfAngle ) ), Real(1) );
			
			Real sourceVisiblity = 0;
			
			if ( request->numDirectRays > 1 )
			{
				// Get the visibility factor of the source based on occlusion (between 0 and 1).
				sourceVisiblity = getDirectVisibility( source.getPosition(), source.getRadius(), listenerPosition, listener.getRadius(),
														averageDirection, numDirectRays, threadData );
			}
			else
			{
				if ( sourceDistance != Real(0) )
				{
					averageDirection = sourceDirection / sourceDistance;
					
					if ( !scene->intersectRay( Ray3f( listenerPosition, averageDirection ),
												math::max( sourceDistance - source.getRadius(), Real(0) ) ) )
						sourceVisiblity = 1;
				}
			}
			
			if ( sourceVisiblity > Real(0) )
			{
				sourceDirection /= sourceDistance;
				Real relativeSpeed = getRelativeSpeed( listener, sourceDirection, source, sourceDirection );
				
				SoundSourceIR& sourceIR = listenerIR.getSourceIR(s);
				FrequencyBandResponse energy = getDistanceAttenuation(sourceDistance) * sourceVisiblity;
				
				if ( sourceDataList[s].directivity )
					energy *= sourceDataList[s].directivity->getResponse( (-sourceDirection)*source.getOrientation() );
				
				if ( sampledIREnabled )
				{
					if ( dopplerSortingEnabled )
					{
						sourceIR.addPath( SoundPath( pathID.getHashCode(), SoundPathFlags::DIRECT,
									energy,
									averageDirection, -averageDirection, sourceDistance,
									relativeSpeed, scene->getMedium().getSpeed() ) );
					}
					else
					{
						sourceIR.addImpulse( sourceDistance / scene->getMedium().getSpeed(), energy,
											averageDirection, -averageDirection );
					}
				}
				else
				{
					sourceIR.addPath( SoundPath( pathID.getHashCode(), SoundPathFlags::DIRECT,
										energy,
										averageDirection, -averageDirection, sourceDistance,
										relativeSpeed, scene->getMedium().getSpeed() ) );
				}
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Diffraction Path Generation Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundPropagator:: addDiffractionPaths( ThreadData& threadData,
											const SoundDetector& listener,
											const SoundPathCache* soundPathCache,
											const SoundDetector& source,
											const Vector3f& listenerImagePosition,
											const WorldSpaceTriangle& worldTriangle,
											Index sourceIndex )
{
	//*********************************************************************
	
	const Real rayOffset = request->rayOffset;
	
	DiffractionQuery& query = threadData.diffractionQuery;
	query.source = &source;
	query.listener = &listener;
	query.soundPathCache = soundPathCache;
	query.object = worldTriangle.objectSpaceTriangle.object;
	query.graph = query.object->getMesh()->getDiffractionGraph();
	query.listenerToSourceDirection = (source.getPosition() - listenerImagePosition).normalize();
	query.sourceIndex = sourceIndex;
	const Size initialNumPaths = threadData.specularPaths.getSize();
	
	const Transform3f& objectTransform = query.object->getTransform();
	
	/// Check each edge of the triangle to see if it is a diffraction edge.
	for ( Index e = 0; e < 3; e++ )
	{
		const internal::DiffractionEdge* edge = worldTriangle.objectSpaceTriangle.triangle->getDiffractionEdge(e);
		
		// Skip edges that aren't diffraction edges.
		if ( edge == NULL )
			continue;
		
		// Transform the edge's neighboring planes to world space.
		Plane3f plane1 = objectTransform.transformToWorld( edge->plane1 );
		Plane3f plane2 = objectTransform.transformToWorld( edge->plane2 );
		
		// Make sure that the source image position is contained in the diffraction region of the edge.
		if ( !testEdgeOrientationVsPoint( plane1, plane2, listenerImagePosition, 0 ) )
			continue;
		
		// Get the vertices of the edge.
		const SoundVertex* v1;
		const SoundVertex* v2;
		worldTriangle.getEdgeVertices( e, v1, v2 );
		
		// Transform the edge to world space.
		WorldSpaceEdge worldSpaceEdge( *v1, *v2, plane1, plane2, objectTransform.transformToWorld( edge->getNormal() ), edge );
		
		// Compute the point of closet approach of the source-to-listener line and the edge's line.
		Real edgeT;
		
		computePointOfClosestApproach( worldSpaceEdge.v1, worldSpaceEdge.direction,
										query.source->getPosition(), query.listenerToSourceDirection, edgeT );
		
		// Make sure the closest point lies on the edge's extent. Skip invalid edges.
#if EDGE_CLAMP
		edgeT = math::clamp( edgeT, Real(0), worldSpaceEdge.length );
#else
		if ( edgeT < Real(0) || edgeT > worldSpaceEdge.length )
			continue;
#endif
		
		// Compute the closest point on the edge. This is the new source image position.
		Vector3f nextListenerImagePosition = worldSpaceEdge.v1 + worldSpaceEdge.direction*edgeT;
		nextListenerImagePosition += worldSpaceEdge.normal*rayOffset;
		
		// Add the image positions to the stack.
		query.points.add( DiffractionPoint( listenerImagePosition ) );
		query.points.add( DiffractionPoint( nextListenerImagePosition ) );
		query.listenerPathDirection = (nextListenerImagePosition - listenerImagePosition).normalize();
		query.listenerSpeed = math::dot( query.listenerPathDirection, listener.getVelocity() );
		query.lastValidIndex = 0;
		
		// Perform the diffraction query for this edge.
		recursiveDiffraction( threadData, worldSpaceEdge, 1 );
		
		// Reset the query data structures.
		query.points.clear();
		query.pointResponses.clear();
	}
	
	return threadData.specularPaths.getSize() > initialNumPaths;
}




void SoundPropagator:: recursiveDiffraction( ThreadData& threadData, const WorldSpaceEdge& edge, Index depth )
{
	DiffractionQuery& query = threadData.diffractionQuery;
	SoundPathID& pathID = threadData.specularPathID;
	const Transform3f& objectTransform = query.object->getTransform();
	const Vector3f& sourcePosition = query.source->getPosition();
	DiffractionPoint& lastPathPoint = query.points[query.points.getSize() - 2];
	DiffractionPoint& thisPathPoint = query.points[query.points.getSize() - 1];
	const Vector3f& lastListenerImagePosition = lastPathPoint.point;
	const Vector3f& listenerImagePosition = thisPathPoint.point;
	
	// Determine which side of the edge the listener image position is on.
	// true == faces plane 1, false == faces plane 2.
	Real plane1Distance = edge.plane1.getSignedDistanceTo( lastListenerImagePosition );
	Real plane2Distance = edge.plane2.getSignedDistanceTo( lastListenerImagePosition );
	Bool listenerOrientation = plane1Distance > plane2Distance && plane1Distance > Real(0);
	
	// Get the free triangle vertex on the same side of the edge as the image source position.
	Vector3f triangleFreeVertex = listenerOrientation ?
									objectTransform.transformToWorld( *edge.edge->getFreeVertex1() ) :
									objectTransform.transformToWorld( *edge.edge->getFreeVertex2() );
	
	// Compute the shadow region boundary plane for the edge and last source image position.
	Plane3f shadowBoundary( lastListenerImagePosition, edge.v1, edge.v2 );
	
	// Make sure that the shadow boundary plane's normal points in the right direction.
	if ( shadowBoundary.getSignedDistanceTo( triangleFreeVertex ) < Real(0) )
		shadowBoundary = -shadowBoundary;
	
	// Get the plane that faces the image source position.
	const Plane3f& listenerPlane = listenerOrientation ? edge.plane1 : edge.plane2;
	thisPathPoint.listenerPlane = &listenerPlane;
	
	// Get the plane which defines the triangle boundary on the opposite side of the edge.
	const Plane3f& oppositePlane = listenerOrientation ? edge.plane2 : edge.plane1;
	thisPathPoint.sourcePlane = &oppositePlane;
	
	SoundPathPoint pathIDPoint( SoundPathPoint::EDGE_DIFFRACTION,
								ObjectSpaceTriangle( listenerOrientation ? edge.edge->triangle1 : edge.edge->triangle2, query.object ),
								listenerOrientation ? edge.edge->edgeIndex1 : edge.edge->edgeIndex2 );
	
	// Add a new point to the path ID for this edge diffraction.
	pathID.addPoint( pathIDPoint );
	
	//*********************************************************************
	// Validate the path to the source, if possible.
	
	// Determine whether or not the source is in the shadow region by testing it against the boundary planes.
	Bool sourceInShadowRegion = shadowBoundary.getSignedDistanceTo( sourcePosition ) > Real(0) &&
								oppositePlane.getSignedDistanceTo( sourcePosition ) > Real(0);
	
	const Real diffractionEpsilon = 0.001f;
	
	if ( depth == 1 && query.soundPathCache && query.soundPathCache->containsPath( pathID ) )
	{
		pathID.removeLastPoint();
		return;
	}
	
	if ( sourceInShadowRegion )
	{
		Bool valid = true;
		Index pointIndex;
		
		const Index lastPointIndex = query.points.getSize() - 1;
		
		// Check the path so far to the listener that hasn't been validated to see if it is valid.
		for ( pointIndex = query.lastValidIndex; pointIndex < lastPointIndex; pointIndex++ )
		{
			DiffractionPoint& lastPoint = query.points[pointIndex];
			DiffractionPoint& thisPoint = query.points[pointIndex+1];
			
			// Compute the direction from the last image source position to this one.
			Vector3f direction = thisPoint.point - lastPoint.point;
			Real distance = direction.getMagnitude();
			
			if ( distance > diffractionEpsilon )
				direction /= distance;
			else
			{
				valid = false;
				break;
			}
			
			if ( scene->intersectRay( Ray3f( lastPoint.point + direction*diffractionEpsilon, direction ),
								distance - diffractionEpsilon*2 ) )
			{
				valid = false;
				break;
			}
			
			// Add the total distance along the path.
			thisPoint.distance = lastPoint.distance + distance;
			
			// Compute the path response for the last edge if there was one.
			if ( pointIndex > 0 )
			{
				const DiffractionPoint& lastLastPoint = query.points[pointIndex - 1];
				
				FrequencyBandResponse totalAttenuation = 
										computeUTDAttenuation( thisPoint.point, lastPoint.point, lastLastPoint.point,
																lastPoint.sourcePlane->normal, lastPoint.listenerPlane->normal,
																edge.direction, scene->getMedium().getSpeed(),
																request->frequencies );
				
				if ( pointIndex > 1 )
					totalAttenuation *= query.pointResponses[pointIndex-2];
				
				if ( query.pointResponses.getSize() <= pointIndex )
					query.pointResponses.add( totalAttenuation );
				else
					query.pointResponses[pointIndex] = totalAttenuation;
			}
		}
		
		query.lastValidIndex = pointIndex;
		
		if ( valid )
		{
			// Compute the vector from the current source image position to the listener.
			Vector3f sourceDirection = sourcePosition - listenerImagePosition;
			Real sourceDistance = sourceDirection.getMagnitude();
			
			if ( sourceDistance > diffractionEpsilon )
			{
				sourceDirection /= sourceDistance;
				
				// Check the path from the source's image position to the listener to make sure it is clear.
				Bool sourceVisible = !scene->intersectRay( Ray3f( listenerImagePosition + sourceDirection*diffractionEpsilon, sourceDirection ), 
														sourceDistance - diffractionEpsilon*2 );
				
				if ( sourceVisible )
				{
					// Compute the total frequency-dependent attenuation for the path to the listener.
					FrequencyBandResponse totalAttenuation =
									computeUTDAttenuation( sourcePosition, listenerImagePosition, lastListenerImagePosition,
															oppositePlane.normal, listenerPlane.normal,
															edge.direction, scene->getMedium().getSpeed(),
															request->frequencies );
					
					if ( depth > 1 )
						totalAttenuation *= query.pointResponses.getLast();
					
					Real totalDistance = query.points.getLast().distance + sourceDistance;
					Real sourceSpeed = math::dot( sourceDirection, query.source->getVelocity() );
					
					if ( sourceDataList[query.sourceIndex].directivity )
						totalAttenuation *= sourceDataList[query.sourceIndex].directivity->getResponse( (-sourceDirection)*query.source->getOrientation() );
					
					// Add the path to the output buffer.
					threadData.specularPaths.add(
						SpecularPathData( pathID, SoundPathFlags::DIFFRACTION,
									getDistanceAttenuation(totalDistance)*totalAttenuation,
									query.listenerPathDirection, -sourceDirection, totalDistance,
									sourceSpeed - query.listenerSpeed, scene->getMedium().getSpeed(),
									query.sourceIndex ) );
				}
			}
		}
	}
	
	// Return if the maximum depth has been reached.
	if ( depth >= request->maxDiffractionOrder || query.graph == NULL )
	{
		// Remove the last point from the path ID.
		pathID.removeLastPoint();
		return;
	}
	
	//*********************************************************************
	// Check the edge's neighbors for higher-order diffraction paths.
	
	const DiffractionGraph* graph = query.graph;
	const Size numNeighbors = edge.edge->numNeighbors;
	const Index neighborListStart = edge.edge->neighborListOffset;
	const Index neighborListEnd = neighborListStart + numNeighbors;
	
	for ( Index n = neighborListStart; n < neighborListEnd; n++ )
	{
		// Get the neighboring edge.
		const internal::DiffractionEdge* neighbor = &graph->getEdgeNeighbor(n);
		
		// Transform the edge into world space.
		const WorldSpaceEdge worldSpaceNeighbor( neighbor, objectTransform );
		
		// Compute the point of closet approach of the listener-to-source line and the edge's line.
		Real edgeT;
		
		computePointOfClosestApproach( worldSpaceNeighbor.v1, worldSpaceNeighbor.direction,
										query.listener->getPosition(), query.listenerToSourceDirection, edgeT );
		
		// Make sure the closest point lies on the edge's extent. Skip invalid edges.
#if EDGE_CLAMP
		edgeT = math::clamp( edgeT, Real(0), worldSpaceNeighbor.length );
#else
		if ( edgeT < Real(0) || edgeT > worldSpaceNeighbor.length )
			continue;
#endif
		
		// Compute the closest point on the edge. This is the new listener image position.
		Vector3f nextListenerImagePosition = worldSpaceNeighbor.v1 + worldSpaceNeighbor.direction*edgeT;
		nextListenerImagePosition += worldSpaceNeighbor.normal*diffractionEpsilon;
		
		// Determine if this new image position lies in the previous diffraction wedge.
		Bool neighborInWedge = shadowBoundary.getSignedDistanceTo( nextListenerImagePosition ) > Real(0) &&
								oppositePlane.getSignedDistanceTo( nextListenerImagePosition ) > Real(0);
		
		// Recursively find diffraction paths for valid edges.
		if ( neighborInWedge )
		{
			// Add the next diffraction point to the query stack.
			query.points.add( DiffractionPoint( nextListenerImagePosition ) );
			
			recursiveDiffraction( threadData, worldSpaceNeighbor, depth + 1 );
			
			// Remove the diffraction point from the query stack.
			query.points.removeLast();
			
			// Reset any validation that occurred.
			if ( query.lastValidIndex >= depth )
				query.lastValidIndex--;
		}
	}
	
	// Remove the last point from the path ID.
	pathID.removeLastPoint();
}




Bool SoundPropagator:: validateDiffractionPath( const SoundPathID& pathID, Index sourceIndex, ThreadData& threadData )
{
	const Real diffractionEpsilon = 0.001f;
	
	const SoundDetector& source = *pathID.getSource();
	const SoundDetector& listener = *pathID.getListener();
	
	Vector3f listenerToSourceDirection = (source.getPosition() - listener.getPosition()).normalize();
	Vector3f listenerDirection;
	Vector3f sourceDirection;
	Vector3f lastPoint;
	Vector3f currentPoint = listener.getPosition();
	Vector3f nextPoint;
	WorldSpaceEdge currentEdge;
	WorldSpaceEdge nextEdge;
	FrequencyBandResponse attenuation;
	Real totalDistance = 0;
	
	const Size numPoints = pathID.getPointCount();
	
	for ( Index i = 0; i <= numPoints; i++ )
	{
		if ( i == numPoints )
			nextPoint = source.getPosition();
		else
		{
			const SoundPathPoint& pathPoint = pathID.getPoint(i);
			const ObjectSpaceTriangle& triangle = pathPoint.getTriangle();
			nextEdge = WorldSpaceEdge( triangle.triangle->getDiffractionEdge(pathPoint.getID()), triangle.object->getTransform() );
			
			// Compute the point of closet approach of the listener-to-source line and the edge's line.
			Real edgeT;
			computePointOfClosestApproach( nextEdge.v1, nextEdge.direction,
											listener.getPosition(), listenerToSourceDirection, edgeT );
			
			// Make sure the closest point lies on the edge's extent. Skip invalid edges.
#if EDGE_CLAMP
			edgeT = math::clamp( edgeT, Real(0), nextEdge.length );
#else
			if ( edgeT < Real(0) || edgeT > nextEdge.length )
				return false;
#endif
			
			
			nextPoint = nextEdge.v1 + nextEdge.direction*edgeT;
			nextPoint += nextEdge.normal*diffractionEpsilon;
			
			if ( i == 0 )
				listenerDirection = (nextPoint - currentPoint).normalize();
		}
		
		//**********************************************************************************
		
		if ( i > 0 )
		{
			// Determine which side of the edge the listener image position is on.
			// true == faces plane 1, false == faces plane 2.
			Real plane1Distance = currentEdge.plane1.getSignedDistanceTo( lastPoint );
			Real plane2Distance = currentEdge.plane2.getSignedDistanceTo( lastPoint );
			Bool listenerOrientation = plane1Distance > plane2Distance && plane1Distance > Real(0);
			
			// Get the free triangle vertex on the same side of the edge as the image source position.
			Vector3f triangleFreeVertex = listenerOrientation ?
											currentEdge.transform->transformToWorld( *currentEdge.edge->getFreeVertex1() ) :
											currentEdge.transform->transformToWorld( *currentEdge.edge->getFreeVertex2() );
			
			// Compute the shadow region boundary plane for the edge and last source image position.
			Plane3f shadowBoundary( lastPoint, currentEdge.v1, currentEdge.v2 );
			
			// Make sure that the shadow boundary plane's normal points in the right direction.
			if ( shadowBoundary.getSignedDistanceTo( triangleFreeVertex ) < Real(0) )
				shadowBoundary = -shadowBoundary;
			
			// Get the plane that faces the image source position.
			const Plane3f listenerPlane = listenerOrientation ? currentEdge.plane1 : currentEdge.plane2;
			
			// Get the plane which defines the triangle boundary on the opposite side of the edge.
			const Plane3f oppositePlane = listenerOrientation ? currentEdge.plane2 : currentEdge.plane1;
			
			//*********************************************************************
			// Validate the path to the source, if possible.
			
			// Determine whether or not the next point is in the shadow region by testing it against the boundary planes.
			Bool nextInShadowRegion = shadowBoundary.getSignedDistanceTo( nextPoint ) > Real(0) &&
										oppositePlane.getSignedDistanceTo( nextPoint ) > Real(0);
			
			if ( !nextInShadowRegion )
				return false;
			
			// Compute the direction from the last image source position to this one.
			sourceDirection = nextPoint - currentPoint;
			Real distance = sourceDirection.getMagnitude();
			totalDistance += distance;
			
			if ( distance > math::epsilon<Real>() )
				sourceDirection /= distance;
			else
				return false;
			
			if ( scene->intersectRay( Ray3f( currentPoint + sourceDirection*diffractionEpsilon, sourceDirection ),
								distance - diffractionEpsilon*2 ) )
				return false;
			
			// Apply attenuation due to diffraction.
			if ( i > 0 )
			{
				attenuation *= computeUTDAttenuation( nextPoint, currentPoint, lastPoint,
													oppositePlane.normal, listenerPlane.normal,
													currentEdge.direction, scene->getMedium().getSpeed(),
													request->frequencies );
			}
		}
		else
		{
			// Compute the direction from the last image source position to this one.
			sourceDirection = nextPoint - currentPoint;
			Real distance = sourceDirection.getMagnitude();
			totalDistance += distance;
			
			if ( distance > math::epsilon<Real>() )
				sourceDirection /= distance;
			else
				return false;
			
			if ( scene->intersectRay( Ray3f( currentPoint + sourceDirection*diffractionEpsilon, sourceDirection ),
								distance - diffractionEpsilon*2 ) )
				return false;
		}
		
		if ( i != numPoints )
		{
			lastPoint = currentPoint;
			currentPoint = nextPoint;
			currentEdge = nextEdge;
		}
	}
	
	Real sourceSpeed = math::dot( sourceDirection, source.getVelocity() );
	Real listenerSpeed = math::dot( listenerDirection, listener.getVelocity() );
	
	if ( sourceDataList[sourceIndex].directivity )
		attenuation *= sourceDataList[sourceIndex].directivity->getResponse( (-sourceDirection)*source.getOrientation() );
	
	threadData.specularPaths.add( SpecularPathData( pathID.getHashCode(), SoundPathFlags::DIFFRACTION,
									getDistanceAttenuation(totalDistance)*attenuation,
									listenerDirection, -sourceDirection, totalDistance,
									sourceSpeed - listenerSpeed, scene->getMedium().getSpeed(),
									sourceIndex ) );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Scene Data Preparation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: prepareSceneData( const SoundScene& newScene, SoundSceneIR& sceneIR )
{
	// Store a temporary pointer to the scene.
	scene = &newScene;
	
	// Update the scene's BVH.
	scene->rebuildBVH();
	
	//*************************************************************************************
	
	// Get a pointer to the internal sound propagation data.
	PropagationData& propagationData = request->internalData;
	
	// Update the timestamp.
	propagationData.timeStamp++;
	propagationData.time += request->dt;
	
	//*************************************************************************************
	// Set the frequency bands that are being used for sound propagation for all meshes/materials in the scene.
	
	const Size numObjects = scene->getObjectCount();
	Size numTriangles = 0;
	Size numVertices = 0;
	Size numEdges = 0;
	Size totalSceneMemory = 0;
	
	for ( Index i = 0; i < numObjects; i++ )
	{
		const SoundMesh& mesh = *scene->getObject(i)->getMesh();
		const Size numMaterials = mesh.getMaterialCount();
		numTriangles += mesh.getTriangleCount();
		numVertices += mesh.getVertexCount();
		totalSceneMemory += mesh.getSizeInBytes();
		const internal::DiffractionGraph* diffractionGraph = mesh.getDiffractionGraph();
		
		if ( diffractionGraph != NULL )
			numEdges += diffractionGraph->getEdgeCount();
		
		for ( Index j = 0; j < numMaterials; j++ )
			mesh.getMaterial(j).setFrequencyBands( request->frequencies );
	}
	
	// Report the final object and mesh attribute count if statistics are enabled,
	if ( statistics != NULL )
	{
		statistics->objectCount = numObjects;
		statistics->triangleCount = numTriangles;
		statistics->vertexCount = numVertices;
		statistics->edgeCount = numEdges;
		statistics->sceneMemory = totalSceneMemory;
	}
	
	//*************************************************************************************
	// Determine the number of active listeners that need sound propagation.
	
	const Size numListeners = scene->getListenerCount();
	Size finalNumListeners = 0;
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		const SoundListener* listener = scene->getListener(i);
		
		if ( listener && listener->getIsEnabled() )
			finalNumListeners++;
	}
	
	// Report the final listener count if statistics are enabled,
	if ( statistics != NULL )
		statistics->listenerCount = finalNumListeners;
	
	//*************************************************************************************
	// Prepare the listener IRs.
	
	// Resize the scene IR so that it can hold all of the listeners.
	sceneIR.setListenerCount( finalNumListeners );
	sceneIR.clear();
	
	// Clear the list of listener data from the last frame.
	listenerDataList.clear();
	Index outputIndex = 0;
	
	// Prepare the IR for each valid listener.
	for ( Index i = 0; i < numListeners; i++ )
	{
		const SoundListener* listener = scene->getListener(i);
		
		// Skip disabled listeners.
		if ( !listener || !listener->getIsEnabled() )
			continue;
		
		SoundListenerIR& listenerIR = sceneIR.getListenerIR(outputIndex);
		
		// Make sure the IR has the right listener pointer and frequency bands.
		listenerIR.setListener( listener );
		listenerIR.setFrequencies( request->frequencies );
		
		// Get the listener data for this listener.
		Shared<PropagationData::ListenerData>* listenerData;
		
		if ( !propagationData.listeners.find( listener->getHashCode(), listener, listenerData ) )
		{
			listenerData = propagationData.listeners.add( listener->getHashCode(), listener, 
														Shared<PropagationData::ListenerData>::construct() );
			
			// Initialize the starting max IR length to be the maximum allowed length.
			(*listenerData)->irLength = request->maxIRLength;
			(*listenerData)->maxIRLength = request->maxIRLength;
		}
		
		// Update the listener data timestamp.
		(*listenerData)->timeStamp = propagationData.timeStamp;
		
		// Add a new listener data to the temporary list.
		listenerDataList.add( ListenerData( listener, *listenerData, &listenerIR ) );
		outputIndex++;
	}
	
	//*************************************************************************************
	// Prepare the per-thread IR data.
	
	// Compute the maximum number of concurrent threads for this scene.
	const Size numThreadData = threadPool.getThreadCount();
	
	// Add new thread data objects if necessary. Use a deterministic random seed.
	for ( Index i = threadDataList.getSize(); i < numThreadData; i++ )
		threadDataList.add( ThreadData( UInt32(42*(i + 1) + 27), this ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Listener Data Preparation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: prepareListenerSourceData( const SoundListener& listener, SoundListenerIR& listenerIR )
{
	// Get a pointer to the internal sound propagation data.
	PropagationData& propagationData = request->internalData;
	
	// Get the listener data for this listener.
	Shared<PropagationData::ListenerData>* listenerData;
	
	if ( !propagationData.listeners.find( listener.getHashCode(), &listener, listenerData ) )
		return;
	
	// Clear the previous source data.
	sourceDataList.clear();
	
	// If clustering is enabled, cluster the sources.
	if ( request->flags.isSet( PropagationFlags::SOURCE_CLUSTERING ) )
	{
		//*************************************************************************************
		// Update the source clustering for this listener.
		
		Timer time;
		
		scene->updateSourceClusters( listener, request->innerClusteringAngle, request->outerClusteringAngle );
		
		Time clusteringTime = time.getElapsedTime();
		
		if ( statistics != NULL )
			statistics->clusteringTime = clusteringTime;
		
		//*************************************************************************************
		// Count the number of propagating sources.
		
		Size finalNumSources = 0;
		const Size numSourceClusters = scene->getSourceClusterCount();
		
		for ( Index i = 0; i < numSourceClusters; i++ )
		{
			const SoundSourceCluster* cluster = scene->getSourceCluster(i);
			
			if ( cluster->getIsMerged() )
				finalNumSources++;
			else
				finalNumSources += cluster->getSourceCount();
		}
		
		// Make sure the listener IR has the correct number of sources.
		listenerIR.setSourceCount( finalNumSources );
		
		//*************************************************************************************
		
		// Report the final cluster count if statistics is enabled,
		if ( statistics != NULL )
		{
			const Size numSources = scene->getSourceCount();
			Size sourceCount = 0;
			
			// Count the number of valid sources (could be different than the number of clusters).
			for ( Index i = 0; i < numSources; i++ )
			{
				const SoundSource* source = scene->getSource(i);
				
				if ( source && source->getIsEnabled() )
					sourceCount++;
			}
			
			statistics->sourceCount = sourceCount;
			statistics->sourceClusterCount = finalNumSources;
		}
		
		//*************************************************************************************
		// For each source cluster in the scene, add a source data object to the source data list.
		
		Index outputSourceIndex = 0;
		
		for ( Index i = 0; i < numSourceClusters; i++ )
		{
			const SoundSourceCluster* cluster = scene->getSourceCluster(i);
			const Size numClusteredSources = cluster->getSourceCount();
			
			if ( cluster->getIsMerged() )
			{
				Shared<PropagationData::SourceData> clusterData;
				SoundSourceIR* sourceIR = &listenerIR.getSourceIR(outputSourceIndex);
				prepareSourceIR( *sourceIR );
				
				for ( Index s = 0; s < numClusteredSources; s++ )
				{
					SoundSource* source = cluster->getSource(s);
					
					// Add the source to this IR.
					sourceIR->addSource( source );
					
					// Get the previous source data for this source.
					Shared<PropagationData::SourceData>* sourceData;
					
					if ( !(*listenerData)->sources.find( source->getHashCode(), source, sourceData ) && clusterData.isNull() )
					{
						sourceData = (*listenerData)->sources.add( source->getHashCode(), source,
																	Shared<PropagationData::SourceData>::construct() );
						
						// Initialize the starting max IR length to be the maximum allowed length.
						(*sourceData)->irLength = request->maxIRLength;
						(*sourceData)->maxIRLength = request->maxIRLength;
					}
					
					if ( clusterData.isNull() )
						clusterData = *sourceData;
					else if ( clusterData != *sourceData )
					{
						// Merge the source data with the cluster data.
						//clusterData->diffusePathCache.addContributions( (*sourceData)->diffusePathCache );
						
						// Replace the source data with a pointer to the cluster data.
						*sourceData = clusterData;
					}
				}
				
				// Update the cluster data timestamp.
				clusterData->timeStamp = propagationData.timeStamp;
				
				// Add a temporary source to the internal list of propagating sources.
				sourceDataList.add( SourceData( cluster, clusterData, sourceIR ) );
				
				// Increment the output source index.
				outputSourceIndex++;
			}
			else
			{
				// Unmerged sources are used as if they weren't clustered at all.
				for ( Index s = 0; s < numClusteredSources; s++ )
				{
					SoundSource* source = cluster->getSource(s);
					
					// Prepare the source data and output IR for this source.
					prepareSourceData( source, listenerIR.getSourceIR(outputSourceIndex), **listenerData );
					
					// Move to the next output IR.
					outputSourceIndex++;
				}
			}
		}
	}
	else
	{
		// Clustering is disabled, just use the unclustered sources directly from the scene.
		const Size numSources = scene->getSourceCount();
		Size finalNumSources = 0;
		
		//*************************************************************************************
		
		// Count the number of propagating sources.
		for ( Index i = 0; i < numSources; i++ )
		{
			const SoundSource* source = scene->getSource(i);
			
			if ( source && source->getIsEnabled() )
				finalNumSources++;
		}
		
		// Report the final source count if statistics is enabled,
		if ( statistics != NULL )
		{
			statistics->sourceCount = finalNumSources;
			statistics->sourceClusterCount = finalNumSources;
			statistics->clusteringTime = 0;
		}
		
		//*************************************************************************************
		
		// Make sure the listener IR has the correct number of sources.
		listenerIR.setSourceCount( finalNumSources );
		Index outputSourceIndex = 0;
		
		// Prepare the output IR and source data list.
		for ( Index s = 0; s < numSources; s++ )
		{
			SoundSource* source = scene->getSource(s);
			
			// Skip disabled sources.
			if ( source && !source->getIsEnabled() )
				continue;
			
			// Prepare the source data and output IR for this source.
			prepareSourceData( source, listenerIR.getSourceIR(outputSourceIndex), **listenerData );
			
			// Move to the next output IR.
			outputSourceIndex++;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Prepare Source Data Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: prepareSourceData( SoundSource* source, SoundSourceIR& sourceIR,
											PropagationData::ListenerData& listenerData )
{
	// Prepare the IR.
	prepareSourceIR( sourceIR );
	
	// Add this source to the IR.
	sourceIR.addSource( source );
	
	// Get the previous source data for this source.
	Shared<PropagationData::SourceData>* sourceData;
	
	if ( !listenerData.sources.find( source->getHashCode(), source, sourceData ) )
	{
		sourceData = listenerData.sources.add( source->getHashCode(), source,
												Shared<PropagationData::SourceData>::construct() );
		
		// Initialize the starting max IR length to be the maximum allowed length.
		(*sourceData)->irLength = request->maxIRLength;
		(*sourceData)->maxIRLength = request->maxIRLength;
	}
	
	// Is the source data shared? (it shouldn't be if it is not part of a merged cluster)
	if ( sourceData->getReferenceCount() > 1 )
	{
		// Copy the source data so that it is unique.
		*sourceData = Shared<PropagationData::SourceData>::construct( **sourceData );
	}
	
	// Update the source data timestamp.
	(*sourceData)->timeStamp = listenerData.timeStamp;
	
	// Add a temporary source to the internal list of propagating sources.
	sourceDataList.add( SourceData( source, *sourceData, &sourceIR ) );
	SourceData& s = sourceDataList.getLast();
	
	// Compute the maximum allowed path length for this source.
	s.maxIRDistance = (*sourceData)->maxIRLength * scene->getMedium().getSpeed();
	
	if ( request->flags.isSet( PropagationFlags::SOURCE_DIRECTIVITY ) &&
		source->flagIsSet( SoundSourceFlags::DIRECTIVITY ) && 
		source->getDirectivity() != NULL )
	{
		const SoundDirectivity* directivity = source->getDirectivity();
		
		(*sourceData)->directivity.setDirectivity( *directivity, request->frequencies );
		s.directivity = &(*sourceData)->directivity;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Prepare Source IR Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: prepareSourceIR( SoundSourceIR& sourceIR )
{
	// Clear the previous sources from this IR.
	sourceIR.clearSources();
	
	// Make sure the source IR has the correct output sample rate.
	sourceIR.setSampleRate( request->sampleRate );
	
	// Enable or disable source direction output for sampled IRs.
	const Bool sourceDirectionsEnabled = request->flags.isSet( PropagationFlags::SAMPLED_IR_SOURCE_DIRECTIONS );
	sourceIR.getSampledIR().setSourceDirectionsEnabled( sourceDirectionsEnabled );
	
	// Set the reverb time for the IR.
	sourceIR.setReverbTime( scene->getReverbTime() );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cell ID Methods
//############		
//##########################################################################################
//##########################################################################################




SoundPathPoint::IDType SoundPropagator:: computeCellID( const internal::InternalSoundTriangle& triangle,
													const Vector3f& barycentric )
{
	Index k = triangle.getKeyVertex();
	Index e = (k + 1) % 3;
	Size numRows = triangle.getRowCount();
	Size numColumns = triangle.getColumnCount();
	
	Index row = (Index)math::floor( barycentric[k]*numRows );
	Size numRowColumns = (Size)math::ceiling( numColumns*Real(numRows - row)/Real(numRows) );
	Index column = (Index)math::floor( barycentric[e]*numRowColumns );
	
	// Compute the final 1D barycentric cell index for the intersection point on the triangle.
	return SoundPathPoint::IDType(numColumns*row + column);
}




Vector3f SoundPropagator:: computeCellCenter( const internal::ObjectSpaceTriangle& objectTriangle,
											SoundPathPoint::IDType cellID )
{
	const InternalSoundTriangle* triangle = objectTriangle.triangle;
	
	Size numRows = triangle->getRowCount();
	Size numColumns = triangle->getColumnCount();
	
	Index row = cellID / numColumns;
	Size numRowColumns = (Size)math::ceiling( numColumns*Real(numRows - row)/Real(numRows) );
	Index column = cellID % numColumns;
	
	Real baryK = (Real(row) + Real(0.5)) / Real(numRows);
	Real baryE = (Real(column) + Real(0.5)) / Real(numRowColumns);
	
	Index k = triangle->getKeyVertex();
	
	Vector3f bary;
	bary[k] = baryK;
	bary[(k + 1) % 3] = baryE;
	bary[(k + 2) % 3] = Real(1) - baryK - baryE;
	
	// Compute the object-space cell center.
	Vector3f cellPoint = (*triangle->getVertex(0))*bary[0] +
						(*triangle->getVertex(1))*bary[1] +
						(*triangle->getVertex(2))*bary[2];
	
	return objectTriangle.object->getTransform().transformToWorld( cellPoint );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Private Geometry Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundPropagator:: computePointOfClosestApproach( const Vector3f& p1, const Vector3f& v1,
														const Vector3f& p2, const Vector3f& v2,
														Real& v1t )
{
	Real v1DotV2 = math::dot( v1, v2 );
	Vector3f p1ToP2 = p2 - p1;
	
	// calculate the parameters of the axis lines of the points of closest approach.
	v1t = (math::dot( p1ToP2, v1 ) - math::dot( p1ToP2, v2 )*v1DotV2) / ( Real(1) - v1DotV2*v1DotV2 );
}




Bool SoundPropagator:: testEdgeOrientationVsPoint( const Plane3f& plane1, const Plane3f& plane2, const Vector3f& point, Real offset )
{
	Real d1 = plane1.getSignedDistanceTo( point );
	Real d2 = plane2.getSignedDistanceTo( point );
	
	Bool front1 = d1 > offset;
	Bool front2 = d2 > offset;
	Bool behind1 = d1 < -offset;
	Bool behind2 = d2 < -offset;
	
	return !((front1 & front2) || (behind1 & behind2));
}




Vector3f SoundPropagator:: getRandomDirection( math::Random<Real>& variable )
{
	Real u1 = variable.sample( Real(-1), Real(1) );
	Real u2 = variable.sample( Real(0), Real(1) );
	Real r = math::sqrt( Real(1) - u1*u1 );
	Real theta = Real(2)*math::pi<Real>()*u2;
	
	return Vector3f( r*math::cos( theta ), r*math::sin( theta ), u1 );
}




Vector3f SoundPropagator:: getRandomDirectionInZCone( math::Random<Real>& variable, Real cosHalfAngle )
{
	const Real u1 = variable.sample( cosHalfAngle, Real(1) );
	const Real u2 = variable.sample( Real(0), Real(1) );
	const Real r = math::sqrt( Real(1) - u1*u1 );
	const Real theta = Real(2)*math::pi<Real>()*u2;
	
	return Vector3f( r*math::cos( theta ), r*math::sin( theta ), u1 );
}




Vector3f SoundPropagator:: getRandomDirectionInHemisphere( math::Random<Real>& variable, const Vector3f& normal )
{
	Vector3f randomDirection = getRandomDirection( variable );
	
	if ( math::dot( randomDirection, normal ) < Real(0) )
		return -randomDirection;
	else
		return randomDirection;
}




Real SoundPropagator:: getDetectorVisibility( const SoundDetector& detector, const Vector3f& point,
											Size numSamples, ThreadData& threadData )
{
	Vector3f detectorDirection = detector.getPosition() - point;
	const Real detectorDistance = detectorDirection.getMagnitude();
	
	if ( detectorDistance < detector.getRadius() )
		return Real(1);
	
	detectorDirection /= detectorDistance;
	
	// Compute the rotation matrix for the direction samples.
	const Matrix3f detectorRotation = Matrix3f::planeBasis( detectorDirection );
	
	// Compute the angular size of the detector.
	const Real cosHalfAngle = getSphereCosHalfAngle( detectorDistance, detector.getRadius() );
	
	// Take samples to determine the detector's visibility.
	Size numVisible = 0;
	
	for ( Index i = 0; i < numSamples; i++ )
	{
		// Generate a ray that samples the detectors's visibility.
		Ray3f validationRay( point, 
							(detectorRotation*getRandomDirectionInZCone( threadData.randomVariable, cosHalfAngle )).normalize() );
		
		// Determine the distance along the ray where the sphere is intersected.
		Real rayDistance;
		
		if ( validationRay.intersectsSphere( detector.getBoundingSphere(), rayDistance ) )
		{
			// Trace a ray to see if that point is visible.
			if ( !scene->intersectRay( validationRay, rayDistance ) )
				numVisible++;
		}
	}
	
	// The visibility is the fraction of rays that hit the detector.
	return Real(numVisible) / Real(numSamples);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Detector-Detector Visibility Method
//############		
//##########################################################################################
//##########################################################################################




Real SoundPropagator:: getDirectVisibility( const Vector3f& sourcePosition, Real sourceRadius,
											const Vector3f& listenerPosition, Real listenerRadius,
											Vector3f& averageDirection, Size numSamples, ThreadData& threadData )
{
	Vector3f detectorDirection = sourcePosition - listenerPosition;
	Real detectorDistance = detectorDirection.getMagnitude();
	
	if ( detectorDistance < sourceRadius )
		return Real(1);
	
	detectorDirection /= detectorDistance;
	
	// Compute the angular size of the listener from the source.
	const Real cosHalfAngle = getSphereCosHalfAngle( detectorDistance, sourceRadius );
	
	// Compute the rotation matrix for the direction samples.
	Matrix3f detectorRotation = Matrix3f::planeBasis( detectorDirection );
	
	// Take samples to determine the detector's visibility.
	Size numVisible = 0;
	averageDirection = detectorDirection;
	
	for ( Index i = 0; i < numSamples; i++ )
	{
		// Generate a ray that samples the detectors's visibility.
		Ray3f validationRay( listenerPosition, 
							(detectorRotation*getRandomDirectionInZCone( threadData.randomVariable, cosHalfAngle )).normalize() );
		
		// Determine the distance along the ray where the sphere is intersected.
		Real rayDistance = math::max<Float>();
		
		if ( !validationRay.intersectsSphere( sourcePosition, sourceRadius, rayDistance ) )
			continue;
		
		rayDistance -= listenerRadius;
		validationRay.origin += validationRay.direction*listenerRadius;
		
		// Trace a ray to see if that point is visible.
		if ( !scene->intersectRay( validationRay, rayDistance ) )
		{
			numVisible++;
			averageDirection += validationRay.direction;
		}
	}
	
	Float averageDirectionLength2 = averageDirection.getMagnitudeSquared();
	
	if ( averageDirectionLength2 > math::epsilon<Float>() )
		averageDirection /= math::sqrt( averageDirectionLength2 );
	
	// The visibility is the fraction of rays that hit the detector.
	return Real(numVisible) / Real(numSamples);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Distance Attenuation Calculation Method
//############		
//##########################################################################################
//##########################################################################################




FrequencyBandResponse SoundPropagator:: getDistanceAttenuation( Real distance ) const
{
	// Adjust the attenuation based on spherical radiation.
	// Surface area = 4*pi*r^2
	// Since the intensity here is radius-independent, just divide by 4pi
	// to get the correct attenuation.
	Real broadbandAttenuation = Real(1)/((Real(4)*math::pi<Real>())*(Real(1) + distance*distance));
	
	if ( request->flags.isSet( PropagationFlags::AIR_ABSORPTION ) )
		return scene->getMedium().getAttenuation(distance)*broadbandAttenuation;
	else
		return broadbandAttenuation;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Relative Speed Calculation Method
//############		
//##########################################################################################
//##########################################################################################




Real SoundPropagator:: getRelativeSpeed( const SoundDetector& listener, const Vector3f& directionFromListener, 
										const SoundDetector& source, const Vector3f& directionToSource )
{
	Real listenerSpeed = math::dot( listener.getVelocity(), directionFromListener );
	Real sourceSpeed = math::dot( source.getVelocity(), directionToSource );
	
	return sourceSpeed - listenerSpeed;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
