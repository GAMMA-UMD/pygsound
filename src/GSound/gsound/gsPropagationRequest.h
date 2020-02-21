/*
 * Project:     GSound
 * 
 * File:        gsound/gsPropagationRequest.h
 * Contents:    gsound::PropagationRequest class declaration
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


#ifndef INCLUDE_GSOUND_PROPAGATION_REQUEST_H
#define INCLUDE_GSOUND_PROPAGATION_REQUEST_H


#include "gsConfig.h"


#include "gsPropagationFlags.h"
#include "gsDebugFlags.h"
#include "gsDebugCache.h"
#include "gsFrequencyBands.h"
#include "gsSoundStatistics.h"
#include "internal/gsPropagationData.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that describes how sound propagation should be performed.
/**
  * It contains parameters that determine what kinds of sound propagation should be done
  * (i.e. direct, diffuse, specular, etc), as well as parameters determining the quality
  * and performance characteristics of the sound propagation.
  */
class PropagationRequest
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a sound propagation request object with the default initial parameters.
			PropagationRequest();
			
			
		//********************************************************************************
		//******	Main Parameters
			
			
			/// An object which indicates the boolean configuration state for sound propagation.
			PropagationFlags flags;
			
			
			/// The simulation time interval for this propagation request in seconds.
			/**
			  * The user should set this value on each frame that sound propagation is performed,
			  * so that the proapagtion system knows the time that is passing during the current
			  * frame. This allows mechanisms like caching to operate based on the wall-clock-time.
			  *
			  * If the user leaves this value as the default (0), the system uses the target delta time as
			  * the delta time for the current frame.
			  */
			Float dt;
			
			
			/// The target simulation time interval for the propagation system in seconds per frame.
			/**
			  * This parameter determines the target number of frames per second
			  * that a propagation system should update the simulation. If a propagation
			  * system is asked to update and the interval since the last update is smaller
			  * than the target update rate, the system can choose to skip the simulation frame
			  * to reduce CPU usage.
			  *
			  * Generally, it is not useful to update a simulation more than 15 to 20 times per second
			  * because any more often will not be perceptible.
			  */
			Float targetDt;
			
			
			/// The minimum IR length that the propagation system should compute, in seconds.
			/**
			  * This value sets a lower bound on the length of the impulse responses that the
			  * propagation system tries to compute. If the ADAPTIVE_IR_LENGTH flag is set, the
			  * system ensures that the IR length is always at least this long.
			  */
			Float minIRLength;
			
			
			/// The maximum IR length that the propagation system can compute, in seconds.
			/**
			  * Any IR samples after this length are discarded during propagation. The memory required
			  * for rendering and propagation scales roughly linearly with this value, depending
			  * on the scene and material properties. Rendering performance scales
			  * logarithmically with the length of the IR.
			  */
			Float maxIRLength;
			
			
			/// The maximum rate (in seconds per second) that the length of the IR is allowed to change.
			/**
			  * If IR_THRESHOLD and ADAPTIVE_IR_LENGTH are enabled, the sound propagation system
			  * clamps the amount that the length of the IR can change to at most this value.
			  */
			Float irGrowthRate;
			
			
			/// A global quality multiplier that is used to scale the other propagation parameters.
			/**
			  * This value, initially 1, can be used to reduce the simulation quality in order
			  * to meet performance requirements. If the flag ADAPTIVE_QUALITY is set, the
			  * sound propagation system may reduce this value in order to meet the target
			  * simulation time interval, or increase the quality if the propagation is
			  * faster than the alotted time.
			  */
			Float quality;
			
			
			/// The minimum quality multiplier to use when the flag ADAPTIVE_QUALITY is set.
			/**
			  * This value is the lower bound on the simulation quality.
			  */
			Float minQuality;
			
			
			/// The maximum quality multiplier to use when the flag ADAPTIVE_QUALITY is set.
			/**
			  * This value is the upper bound on the simulation quality.
			  */
			Float maxQuality;
			
			
			/// The number of threads to use for sound propagation.
			/**
			  * Set this value to the number of available hardware threads of your CPU to maximize
			  * sound propagation performance. However, if too many threads are used, it
			  * can impact rendering performance and cause audio glitches.
			  */
			Size numThreads;
			
			
			/// An object that describes what debug information should be returned in the debug cache.
			DebugFlags debugFlags;
			
			
			/// A pointer to a cache which buffers debug information produced during sound propagation.
			/**
			  * This pointer may be NULL, indicating that debug information is not required at all.
			  */
			DebugCache* debugCache;
			
			
			/// A pointer to an optional object which recieve runtime information about the propagation system and scene.
			/**
			  * If statistics are enabled and this pointer is not NULL, the propagation system sets
			  * data in the object that indicate the current performance of the system.
			  * This pointer may be NULL, indicating that analytic information is not required at all.
			  */
			SoundStatistics* statistics;
			
			
		//********************************************************************************
		//******	Rendering Parameters
			
			
			/// An object that describes the frequency bands for which to perform sound propagation and rendering.
			/**
			  * This object contains a fixed number of frequency bands that determine the propagation
			  * and rendering frequency bands.
			  */
			FrequencyBands frequencies;
			
			
			/// The sample rate at which sampled impulse responses should be computed and audio rendering should be performed.
			SampleRate sampleRate;
			
			
			/// The doppler shifting magnitude in cents above which a path will be output as a discrete path.
			/**
			  * This parameter only has effect if the flags SAMPLED_IR and DOPPLER_SORTING are set in the
			  * propagation flags. If so, each propagation path is inspected upon output and stored as either
			  * a discrete propagation path or accumulated in a sampled IR based on the amount of doppler shifting
			  * for the path.
			  */
			Float dopplerThreshold;
			
			
		//********************************************************************************
		//******	Ray Tracing Parameters
			
			
			/// The maximum number of direct sound visibility rays traced to determine how visible each source is.
			/**
			  * This number of rays randomly distributed in the cone bounding the source
			  * are cast, and the number of rays that pass the occlusion test determine
			  * how strong the direct contribution is.
			  *
			  * The system may take fewer samples if the angular size of the source is small,
			  * in order to reduce the number of direct rays in simulations with many distant sources.
			  */
			Size numDirectRays;
			
			
			/// The maximum number of reflections that can occur before a diffraction path.
			/**
			  * The complexity of the diffraction simulation increases linearly with this parameter.
			  */
			Size maxDiffractionDepth;
			
			
			/// The maximum allowed number of recursive diffractions that may be detected.
			/**
			  * The complexity of the diffraction computation increases exponentially with the
			  * maximum diffraction order, values above 5 or so may produce very slow simulations.
			  * The complexity is very scene and viewpoint-dependent.
			  */
			Size maxDiffractionOrder;
			
			
			/// The maximum depth to which specular rays should be propagated.
			/**
			  * Usually this parameter does not need to be more than 5 to 10
			  * in order to capture the most important specular reflections.
			  * The cost for specular sound propagation scales linearly with this parameter.
			  */
			Size maxSpecularDepth;
			
			
			/// The number of rays to emit to find specular propagation paths.
			Size numSpecularRays;
			
			
			/// The number of rays to use to determine the visibility of a specularly reflected source.
			Size numSpecularSamples;
			
			
			/// The maximum depth to which diffuse rays should be propagated.
			/**
			  * For full late reverberation, this value should be at least 30 to 100,
			  * depending on the size of the scene. The cost for sound propagation scales
			  * linearly with this parameter.
			  */
			Size maxDiffuseDepth;
			
			
			/// The number of diffuse rays to emit from each sound source or listener.
			Size numDiffuseRays;
			
			
			/// The number of ray occlusion query samples that are taken when estimating a source's visibility for diffuse rain.
			/**
			  * A value of 1 causes a single visibility ray to be traced from a reflection point
			  * to each sound source's center to determine if the source is visible. This can be inaccurate
			  * if the source is partially occluded. For better (but slower) results, this value can be increased
			  * to trace more visibility rays and better estimate how visible the source is. Additional rays
			  * randomly sample the cone that contains the source with the vertex at the reflection point.
			  */
			Size numDiffuseSamples;
			
			
			/// The number of visibility rays that are used to determine which triangles are visible to sources and listeners.
			/**
			  * The resulting triangles intersected by these rays are stored in a visibility cache
			  * that persists over many frames in order to accelerate source visibility queries for
			  * diffuse sound propagation. These rays are only traced if the VISIBILITY_CACHE flag is enabled.
			  */
			Size numVisibilityRays;
			
			
			/// A small value used to bias ray-triangle intersection points away from the triangle.
			/**
			  * This is done to reduce the prevalence of precision problems in ray tracing. A good
			  * rule of thumb is to set this value to be an order of magnitude smaller than the smallest
			  * features in a scene.
			  */
			Real rayOffset;
			
			
		//********************************************************************************
		//******	Caching Parameters
			
			
			/// The response time (in seconds) that is used to improve the propagation results.
			/**
			  * A longer cache time improves the quality of the diffuse sound, but at the expense
			  * of longer response times to changes in the scene configuration. For instance, if
			  * a sound source moves and suddenly becomes inaudible to the listener, it may take
			  * at most this time for the diffuse sound field to react to the changes. A reasonable
			  * value is in the 0.2 to 0.7 second range where the error is usually not perceptible.
			  */
			Real responseTime;
			
			
			/// The minimum time in seconds that a triangle stays in the visibility cache for source or listener.
			/**
			  * A longer cache time improves the quality of the visibility results and allows fewer visibility rays
			  * to be traced on each frame. Since the visibility cache is only an acceleration structure and does not
			  * directly affect the sound quality or introduce error, a value of several seconds can be used in order to maximize
			  * the set of cached visible triangles (accuracy always increases with a longer cache time).
			  * If the cache time is too long and the source is moving, there may be extra non-visible triangles stored in the cache
			  * that may make things slower than they could be (but won't introduce error).
			  */
			Real visibilityCacheTime;
			
			
		//********************************************************************************
		//******	Clustering Parameters
			
			
			/// The angular threshold in degrees at which a cluster of sound sources will be treated as a single source.
			Real innerClusteringAngle;
			
			
			/// The angular threshold in degrees at which sound sources will be placed in a cluster.
			Real outerClusteringAngle;
			
			
		//********************************************************************************
		//******	Internal Data
			
			
			/// An object that contains internal implementation-specific sound propagation data (i.e. caches).
			/**
			  * This object is automatically managed by the propagation system.
			  * The user can call internalData.reset() to reset the caches that are stored.
			  * Reseting the caches might be necessary if there is a large change in the scene
			  * configuration that occurs.
			  */
			internal::PropagationData internalData;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROPAGATION_REQUEST_H
