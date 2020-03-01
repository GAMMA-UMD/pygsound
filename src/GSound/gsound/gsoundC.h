/*
 * Project:     GSound
 * 
 * File:        gsound/gsoundC.h
 * Contents:    C interface for the GSound library
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


#ifndef INCLUDE_GSOUND_C_H
#define INCLUDE_GSOUND_C_H


/* Inform C++ code to use C calling conventions. */
#ifdef __cplusplus
extern "C"
{
#endif


/* Define DLL export symbol. */
#ifndef GSOUND_EXPORT
	#if defined( __GNUC__ )
		#define GSOUND_EXPORT __attribute__((visibility("default")))
	#elif defined( _MSC_VER )
		#define GSOUND_EXPORT __declspec(dllexport)
	#else
		#error Unsupported compiler.
	#endif
#endif




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Public Type Definitions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief The type used to store a boolean value. */
typedef unsigned int gsBool;

/**
  * \brief The type used to store a count of a number of items.
  *
  * This type is 64-bits wide and unsigned on all systems.
  */
typedef unsigned long long gsSize;

/** \brief The type used to store an index. This type is an alias for gsSize. */
typedef gsSize gsIndex;

/** \brief The type used to store a real number (floating-point). This type is 32-bits wide. */
typedef float gsFloat;

/** \brief The type used to store a sample rate. This type is 64-bits wide (floating-point). */
typedef double gsSampleRate;

/**
  * \brief The type used to store the integer identifier for an internal object.
  *
  * An ID of 0 indicates an error or invalid object.
  */
typedef unsigned int gsID;

/** \brief The type use to store the ID for a sound propagation system object. */
typedef gsID gsSystemID;

/** \brief The type use to store the ID for a sound device object. */
typedef gsID gsDeviceID;

/** \brief The type use to store the ID for a sound propagation request object. */
typedef gsID gsRequestID;

/** \brief The type use to store the ID for a render request object. */
typedef gsID gsRenderRequestID;

/** \brief The type use to store the ID for a mesh request object. */
typedef gsID gsMeshRequestID;

/** \brief The type use to store the ID for a sound scene object. */
typedef gsID gsSceneID;

/** \brief The type use to store the ID for a sound source object. */
typedef gsID gsSourceID;

/** \brief The type use to store the ID for a sound listener object. */
typedef gsID gsListenerID;

/** \brief The type use to store the ID for a sound object. */
typedef gsID gsObjectID;

/** \brief The type use to store the ID for a sound mesh. */
typedef gsID gsMeshID;

/** \brief The type use to store the ID for a sound material. */
typedef gsID gsMaterialID;

/** \brief The type use to store the ID for a sound. */
typedef gsID gsSoundID;

/** \brief The type use to store the ID for an HRTF. */
typedef gsID gsHRTFID;

/** \brief The value of an invalid internal object ID. */
const gsID GS_INVALID_ID = 0;

/**
  * \brief The type to use for a sound source audio input callback.
  *
  * This callback is called from the audio rendering thread whenever a new buffer
  * of source audio is needed. The user should fill the sample buffer with
  * channel-interleaved sample data of the specified length and channel count.
  */
typedef void (*gsSourceCallback)( gsSourceID sourceID, gsFloat* samples, gsSize numSamples, gsSize numChannels, gsSampleRate sampleRate );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Flag Parameter Enum														*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/**
  * \brief An enum describing the different boolean flags for sound requests.
  *
  * Use the functions gsRequestGetFlag() and gsRequestSetFlag() to retrieve or set
  * the value of a given flag for a propagation request object.
  *
  * Use the functions gsRenderRequestGetFlag() and gsRenderRequestSetFlag() to retrieve or set
  * the value of a given flag for a render request object.
  *
  * Use the functions gsMeshRequestGetFlag() and gsMeshRequestSetFlag() to retrieve or set
  * the value of a given flag for a mesh preprocessing request object.
  */
typedef enum
{
	/**********************************************************************************/
	/* Main Simulation Flags */
	
	/**
	  * \brief A flag indicating whether or not direct sound should be propagated.
	  *
	  * Direct sound has a minimal performance cost, and so should generally be
	  * enabled unless for debugging purposes.
	  */
	GS_DIRECT = 1,
	
	/**
	  * \brief A flag indicating whether or not transmitted sound should be propagated.
	  *
	  * Transmission occurs when sound travels through geometry in the scene
	  * and emerges out the other side.
	  */
	GS_TRANSMISSION = 2,
	
	/**
	  * \brief A flag indicating whether or not specular reflection sound propagation should be performed.
	  *
	  * A specular reflection is when sound reflects at the same angle that it hits a surface.
	  * Specular reflections contribute primarily to early reflections.
	  */
	GS_SPECULAR = 3,
	
	/**
	  * \brief A flag indicating whether or not diffuse reflection sound propagation should be performed.
	  *
	  * A diffuse reflection is when sound is randomly scattered when it hits a surface.
	  * Diffuse reflections contribute significantly to late reverberation.
	  */
	GS_DIFFUSE = 4,
	
	/**
	  * \brief A flag indicating whether or not diffraction sound propagation should be performed.
	  *
	  * Diffraction enables sound to scatter around objects that would otherwise occlude the
	  * sound source.
	  */
	GS_DIFFRACTION = 5,
	
	/**
	  * \brief A flag indicating whether or not a cache for specular sound data should be used.
	  *
	  * This flag should always be enabled because it greatly improves both the
	  * performance and temporal coherence of specular paths with minimal performance
	  * impact and no loss of accuracy.
	  */
	GS_SPECULAR_CACHE = 6,
	
	/** A flag indicating whether or not a cache for diffuse sound data should be used. */
	GS_DIFFUSE_CACHE = 7,
	
	/**
	  * \brief A flag indicating whether or not an IR cache should be used.
	  *
	  * The IR cache is an alternative to the diffuse path cache that uses less memory,
	  * is faster, and generally produces better results for a given number of rays.
	  * The IR cache is essentially a cached version of the IR that is slowly updated
	  * by the new IRs computed on each frame, i.e. there is a 1st order low-pass filter for each
	  * sample in the IR, to smooth the resulting IR over time for interactive applications.
	  *
	  * The IR cache can only be used if SAMPLED_IR is also enabled, since it does
	  * not track discrete paths from frame to frame.
	  */
	GS_IR_CACHE = 8,
	
	/**
	  * \brief A flag indicating whether or not a visibility cache should be used.
	  *
	  * If this flag is set, the system maintains a cache of the triangles that are visible to
	  * each source and listener that is used to accelerate visibility queries. If a triangle
	  * is known to not be visible to a source based on the cache, rays that hit that triangle
	  * are unlikley to hit the source, and so some ray visibility queries can be avoided.
	  */
	GS_VISIBILITY_CACHE = 9,
	
	/**
	  * \brief A flag indicating whether or not the "diffuse rain" sampling approach should be used.
	  *
	  * This technique was described in Schröder's
	  * Physically Based Real-Time Auralization of Interactive Virtual Environments.
	  * It is used to substantially increase the number of diffuse paths found for
	  * a given number of rays by taking samples for every source at each ray-scene
	  * intersection point. This technique has an additional runtime cost,
	  * but tends to produce much better results for a given number of diffuse rays.
	  */
	GS_DIFFUSE_SAMPLES = 10,
	
	/**
	  * \brief A flag indicating whether or not diffuse rays should be traced from sound sources instead of the listener.
	  *
	  * This is expensive if there are many sources but can produce more accurate results.
	  * This will cause the propagation cost to scale linearly with the number of sources,
	  * rather than sub-linearly (as it would with listener diffuse propagation).
	  */
	GS_SOURCE_DIFFUSE = 11,
	
	/**
	  * \brief A flag indicating whether or not directional sound sources are enabled.
	  *
	  * If this flag is enabled, any sources that have directivity information and that
	  * have the SoundSourceFlags::DIRECTIVITY flag enabled will have direction sound
	  * computed. Directional sound sources are more expensive to compute than omnidirectional
	  * sources.
	  */
	GS_SOURCE_DIRECTIVITY = 12,
	
	/**
	  * \brief A flag indicating whether or not source clustering should be enabled.
	  *
	  * Source clustering allows distant sources that are close to each other to be treated
	  * as a single source in order to increase performance when there are many sources.
	  */
	GS_SOURCE_CLUSTERING = 13,
	
	/**
	  * \brief A flag indicating whether or not air absorption should be computed for sound propagation paths.
	  *
	  * Air absorption attenuates sound in a frequency-dependent manner as it travels
	  * through the propagation medium. This flag should always be enabled for realistic output
	  * in large environments.
	  */
	GS_AIR_ABSORPTION = 14,
	
	/**
	  * \brief A flag indicating whether or not sampled IR output is enabled.
	  *
	  * If this flag is not set, all propagation output is always stored as discrete paths,
	  * rather than sampled IRs. If it is set, the propagation system may store output in
	  * the sampled IR for a sound source. This may be more efficient but may also require more
	  * memory for the IR buffer if number of paths is small.
	  * If computing very dense impulse responses, this flag should be
	  * enabled in order to avoid very long lists of overlapping paths that may use large amounts of memory.
	  */
	GS_SAMPLED_IR = 15,
	
	/**
	  * \brief A flag indicating whether or not source direction sampled IR output is enabled.
	  *
	  * If this flag is set and sampled IRs are enabled, the system stores the
	  * direction that the sound was emitted from the source for each sample in the IR,
	  * in addition to the direction towards the sound from the listener. This increases
	  * the memory required for an IR by a factor of ~42%, but also allows the rendering system
	  * to compute source directivity.
	  */
	GS_SAMPLED_IR_SOURCE_DIRECTIONS = 16,
	
	/**
	  * \brief A flag indicating whether or not impulse response should be trimmed based on perceptual thresholds.
	  *
	  * If this flag is set the system can trim the impulse response for each
	  * source so that only the audible parts (based on a threshold specified by
	  * the listener) are saved.
	  */
	GS_IR_THRESHOLD = 17,
	
	/**
	  * \brief A flag indicating whether or not the max allowed IR length can dynamically change based on scene parameters.
	  *
	  * If this flag is set along with IR_THRESHOLD, the system adaptively determines the
	  * max IR length for each source and listener and from this data chooses how far
	  * rays can travel in the scene before they are considered irrelevant. On each frame,
	  * the max allowed IR length for each source/listener is allowed to grow or shrink at the
	  * ir growth rate so that no inaudible rays are computed.
	  */
	GS_ADAPTIVE_IR_LENGTH = 18,
	
	/**
	  * \brief A flag indicating whether or not the simulation quality can be changed to meet performance requirements.
	  *
	  * If this flag is set, the SoundPropagationSystem can automatically scale certain
	  * propagation parameters based on the performance on previous frames.
	  * If the propagation time for the last frame was larger than the target
	  * delta time, the system reduces the quality factor by a proportional amount.
	  *
	  * If the system previously had its quality reduced and the delta time is now
	  * less than the target dt, the system returns the quality factor to 1 to regain
	  * full simulation quality.
	  */
	GS_ADAPTIVE_QUALITY = 19,
	
	/**********************************************************************************/
	/* Rendering Flags */
	
	/** \brief A flag indicating whether or not doppler path sorting is enabled. */
	GS_DOPPLER_SORTING = 20,
	
	/**
	  * \brief A flag indicating whether or not convolution for sampled impulse responses should be performed.
	  *
	  * If this flag is set, the sound renderer enables frequency-domain block
	  * convolution of sampled impulse responses. This is the preferred mode of rendering.
	  *
	  * This flag generally should not be disabled except for debug purposes.
	  */
	GS_CONVOLUTION = 21,
	
	/**
	  * \brief A flag indicating whether or not discrete path rendering should be performed.
	  *
	  * If this flag is set, the sound renderer uses time-domain delay interpolation to render
	  * discrete sound paths that are part of the input impulse responses. This rendering
	  * mode is used for significant paths like direct sound, diffracted sound, and early
	  * reflections. The number of paths that can be rendered for each source
	  * can be limited by setting the RenderRequest::maxSourcePathCount.
	  * Paths that don't fit within this budget are added to the sampled IR and
	  * rendered using block convolution.
	  *
	  * This flag generally should not be disabled except for debug purposes.
	  */
	GS_DISCRETE_PATHS = 22,
	
	/**
	  * \brief A flag indicating whether or not discrete path rendering should be performed.
	  *
	  * If this flag is set, the sound renderer uses time-domain delay interpolation to render
	  * discrete sound paths that are part of the input impulse responses. This rendering
	  * mode is used for significant paths like direct sound, diffracted sound, and early
	  * reflections. The number of paths that can be rendered for each source
	  * can be limited by setting the RenderRequest::maxSourcePathCount.
	  * Paths that don't fit within this budget are added to the sampled IR and
	  * rendered using block convolution.
	  *
	  * This flag generally should not be disabled except for debug purposes.
	  */
	GS_HRTF = 23,
	
	/**********************************************************************************/
	/* Mesh Preprocessing Flags */
	
	/**
	  * \brief A flag which indicates that diffraction edges should be found for the preprocessed mesh.
	  *
	  * Setting this flag enables 1st order diffraction for a mesh, but high-order diffraction
	  * may not be available with just this option. Diffraction edges are found by looking
	  * at the curvature of the mesh at each edge. Edges where there is high curvature are
	  * marked as diffraction edges.
	  */
	GS_DIFFRACTION_EDGES = 24,
	
	/**
	  * \brief A flag which indicates that diffraction edge visibility should be determined for the preprocessed mesh.
	  *
	  * Setting this flag enables fast high-order diffraction computation by using a static
	  * per-object visiblity graph. Computing this graph may be time consuming due to the
	  * large number of edge pairs that need to be considered and it may take up considerable storage.
	  * For complex models, it may be good to simplify the mesh to reduce the number of edges that
	  * need to be considered.
	  */
	GS_DIFFRACTION_GRAPH = 25,
	
	/**
	  * \brief A flag which indicates whether or not voxel-based mesh simplification should be done.
	  *
	  * If enabled, the mesh is voxelized to the specified resolution,
	  * then retriangulated using the marching cubes algorithm.
	  */
	GS_VOXELIZE = 26,
	
	/**
	  * \brief A flag which indicates whether or not the mesh preprocessor should weld vertices.
	  *
	  * If enabled, the multiple vertices may be combined into one if they are less than
	  * the welding distance away from each other.
	  */
	GS_WELD = 27,
	
	/**
	  * \brief A flag which indicates whether or not mesh surface simplification should be done.
	  *
	  * If enabled, the mesh's surface is simplified based on the simplification tolerance parameter.
	  */
	GS_SIMPLIFIY = 28
	
} gsFlag;




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Request Parameter Enum													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/**
  * \brief An enum describing the different parameters for sound requests of various types.
  *
  * Use the functions gsRequestGetParam* and gsRequestSetParam* to retrieve or set
  * the value of a given parameter for a propagation request object.
  *
  * Use the functions gsRenderRequestGetParam* and gsRenderRequestSetParam* to retrieve or set
  * the value of a given parameter for a render request object.
  *
  * Use the functions gsMeshRequestGetParam* and gsMeshRequestSetParam* to retrieve or set
  * the value of a given parameter for a mesh request object.
  */
typedef enum
{
	/**********************************************************************************/
	/* Main Simulation Parameters */
	
	/**
	  * \brief The target frame time for the propagation system in seconds per frame.
	  *
	  * This parameter determines the target number of frames per second
	  * that a propagation system should update the simulation. If a propagation
	  * system is asked to update and the interval since the last update is smaller
	  * than the target update rate, the system can choose to skip the simulation frame
	  * to reduce CPU usage.
	  *
	  * Generally, it is not useful to update a simulation more than 15 to 20 times per second
	  * because any more often will not be perceptible.
	  */
	GS_TARGET_DT = 1,
	
	/**
	  * \brief The minimum IR length that the propagation system should compute, in seconds
	  *
	  * This value sets a lower bound on the length of the impulse responses that the
	  * propagation system tries to compute. If the GS_ADAPTIVE_IR_LENGTH flag is set, the
	  * system ensures that the IR length is always at least this long.
	  */
	GS_IR_MIN_LENGTH = 2,
	
	/**
	  * \brief The maximum IR length that the propagation system can compute, in seconds.
	  *
	  * Any IR samples after this length are discarded during propagation. The memory required
	  * for rendering and propagation scales roughly linearly with this value, depending
	  * on the scene and material properties. Rendering performance scales
	  * logarithmically with the length of the IR.
	  */
	GS_IR_MAX_LENGTH = 3,
	
	/**
	  * \brief The maximum rate (in seconds per second) that the length of the IR is allowed to change.
	  *
	  * If GS_IR_THRESHOLD and GS_ADAPTIVE_IR_LENGTH are enabled, the sound propagation system
	  * clamps the amount that the length of the IR can change to at most this value.
	  */
	GS_IR_GROWTH_RATE = 4,
	
	/**
	  * \brief A global quality multiplier that is used to scale the other propagation parameters.
	  *
	  * This value, initially 1, can be used to reduce the simulation quality in order
	  * to meet performance requirements. If the flag GS_ADAPTIVE_QUALITY is set, the
	  * sound propagation system may reduce this value in order to meet the target
	  * simulation time interval, or increase the quality if the propagation is
	  * faster than the alotted time.
	  */
	GS_QUALITY = 5,
	
	/**
	  * \brief The minimum quality multiplier to use when the flag ADAPTIVE_QUALITY is set.
	  *
	  * This value is the lower bound on the simulation quality.
	  */
	GS_MIN_QUALITY = 6,
	
	/**
	  * \brief The maximum quality multiplier to use when the flag ADAPTIVE_QUALITY is set.
	  *
	  * This value is the upper bound on the simulation quality.
	  */
	GS_MAX_QUALITY = 7,
	
	/**
	  * \brief The number of threads to use for sound propagation.
	  *
	  * Set this value to the number of available hardware threads of your CPU to maximize
	  * sound propagation performance. However, if too many threads are used, it
	  * can impact rendering performance and cause audio glitches.
	  */
	GS_PROPAGATION_THREAD_COUNT = 8,
	
	
	/**********************************************************************************/
	/* Rendering Parameters */
	
	/** \brief The sample rate at which sampled impulse responses should be computed and audio rendering should be performed. */
	GS_SAMPLE_RATE = 9,
	
	/** \brief The channel layout type that should be used when rendering. */
	GS_CHANNEL_LAYOUT = 10,
	
	/**
	  * \brief The doppler shifting magnitude in cents above which a path will be output as a discrete path.
	  *
	  * This parameter only has effect if the flags GS_SAMPLED_IR and GS_DOPPLER_SORTING are set in the
	  * propagation flags. If so, each propagation path is inspected upon output and stored as either
	  * a discrete propagation path or accumulated in a sampled IR based on the amount of doppler shifting
	  * for the path.
	  */
	GS_DOPPLER_THRESHOLD = 11,
	
	/**
	  * \brief The number of threads to use for updating rendered IRs.
	  *
	  * This value determines how many threads are used within the renderer
	  * to update the IR on each frame.
	  */
	GS_UPDATE_THREAD_COUNT = 12,
	
	/**
	  * \brief The maximum allowed processing latency in seconds for the sound propagation renderer.
	  *
	  * The renderer will attempt, if it is possible, to process its audio stream with a latency that is
	  * less than or equal to this latency. This value should be set as large as possible
	  * without the delay being perceptually noticeable in order to make the renderer
	  * resistant to processing stalls due to other threads temporarily using CPU time.
	  * The smaller the latency, the more work that must be done when performing block
	  * convolution, and the more threads that are necessary.
	  */
	GS_MAX_LATENCY = 13,
	
	/** \brief The maximum allowed number of discrete paths that should be rendered per sound source. */
	GS_MAX_SOURCE_PATH_COUNT = 14,
	
	/**
	  * \brief The maximum delay time in seconds that a discrete propagation path is allowed to have.
	  *
	  * This value determines how much of a sound source's input audio is buffered in time domain
	  * to allow discrete path rendering. Since most discrete paths (i.e. direct, diffraction, specular)
	  * are in the first 0.5s of most IRs, this value can generally be set much lower than
	  * maxIRLength. A smaller value will reduce memory requirements.
	  */
	GS_MAX_PATH_DELAY = 15,
	
	/** \brief The maximum rate (in seconds per second) that a discrete path's delay time is allowed to change. */
	GS_MAX_DELAY_RATE = 16,
	
	/**
	  * \brief The minimum time in seconds that it takes to interpolate to a new impulse response.
	  *
	  * The actual fade time is lower bounded by the length of the FFT buffer, which depends on the
	  * delay time. IR partitions that are more delayed are interpolated more slowly to
	  * reduce artifacts and reduce the update rate for those partitions.
	  */
	GS_IR_FADE_TIME = 17,
	
	/**
	  * \brief The time in seconds that it takes a discrete path to interpolate to a new volume.
	  *
	  * Setting this to a larger value results in smoother audio but can cause more
	  * paths to be rendered because paths are not removed until they are completely
	  * faded out. This can result in the maxSourcePathCount limit being exceeded
	  * for short periods of time while the previous paths are faded out.
	  */
	GS_PATH_FADE_TIME = 18,
	
	/** \brief The time in seconds that it takes a source's audio to fade in or out when added to or removed from a cluster. */
	GS_SOURCE_FADE_TIME = 19,
	
	/** \brief The time in seconds that it takes a cluster's audio to fade in when it is new. */
	GS_CLUSTER_FADE_IN_TIME = 20,
	
	/** \brief The time in seconds that it takes a cluster's audio to fade out when it is new. */
	GS_CLUSTER_FADE_OUT_TIME = 21,
	
	/**
	  * \brief A constant linear gain factor that all rendered sound is scaled by.
	  *
	  * This value can be used to adjust the output volume of a renderer independent
	  * of the listener's sensitivity.
	  */
	GS_VOLUME = 22,
	
	
	/**********************************************************************************/
	/* Ray Tracing Parameters */
	
	/**
	  * \brief The maximum number of direct sound visibility rays traced to determine how visible each source is.
	  *
	  * This number of rays randomly distributed in the cone bounding the source
	  * are cast, and the number of rays that pass the occlusion test determine
	  * how strong the direct contribution is.
	  *
	  * The system may take fewer samples if the angular size of the source is small,
	  * in order to reduce the number of direct rays in simulations with many distant sources.
	  */
	GS_DIRECT_RAY_COUNT = 23,
	
	/**
	  * \brief The maximum number of reflections that can occur before a diffraction path.
	  *
	  * The complexity of the diffraction simulation increases linearly with this parameter.
	  */
	GS_DIFFRACTION_MAX_DEPTH = 24,
	
	/**
	  * \brief The maximum allowed number of recursive diffractions that may be detected.
	  *
	  * The complexity of the diffraction computation increases exponentially with the
	  * maximum diffraction order, values above 5 or so may produce very slow simulations.
	  * The complexity is very scene and viewpoint-dependent.
	  */
	GS_DIFFRACTION_MAX_ORDER = 25,
	
	/**
	  * \brief The maximum depth to which specular rays should be propagated.
	  *
	  * Usually this parameter does not need to be more than 5 to 10
	  * in order to capture the most important specular reflections.
	  * The cost for specular sound propagation scales linearly with this parameter.
	  */
	GS_SPECULAR_MAX_DEPTH = 26,
	
	/** \brief The number of rays to emit to find specular propagation paths. */
	GS_SPECULAR_RAY_COUNT = 27,
	
	/** \brief The number of rays to use to determine the visibility of a specularly reflected source. */
	GS_SPECULAR_SAMPLE_COUNT = 28,
	
	/**
	  * \brief The maximum depth to which diffuse rays should be propagated.
	  *
	  * For full late reverberation, this value should be at least 30 to 100,
	  * depending on the size of the scene. The cost for sound propagation scales
	  * linearly with this parameter.
	  */
	GS_DIFFUSE_MAX_DEPTH = 29,
	
	/** \brief The number of diffuse rays to emit from each sound source or listener. */
	GS_DIFFUSE_RAY_COUNT = 30,
	
	/**
	  * \brief The number of ray occlusion query samples that are taken when estimating a source's visibility for diffuse rain.
	  *
	  * A value of 1 causes a single visibility ray to be traced from a reflection point
	  * to each sound source's center to determine if the source is visible. This can be inaccurate
	  * if the source is partially occluded. For better (but slower) results, this value can be increased
	  * to trace more visibility rays and better estimate how visible the source is. Additional rays
	  * randomly sample the cone that contains the source with the vertex at the reflection point.
	  */
	GS_DIFFUSE_SAMPLE_COUNT = 31,
	
	/**
	  * \brief The number of visibility rays that are used to determine which triangles are visible to sources and listeners.
	  *
	  * The resulting triangles intersected by these rays are stored in a visibility cache
	  * that persists over many frames in order to accelerate source visibility queries for
	  * diffuse sound propagation. These rays are only traced if the flag GS_VISIBILITY_CACHE is enabled.
	  */
	GS_VISIBILITY_RAY_COUNT = 33,
	
	/** \brief
	  * A small value used to bias ray-triangle intersection points away from the triangle.
	  *
	  * This is done to reduce the prevalence of precision problems in ray tracing. A good
	  * rule of thumb is to set this value to be an order of magnitude smaller than the smallest
	  * features in a scene.
	  */
	GS_RAY_OFFSET = 34,
	
	
	/**********************************************************************************/
	/* Caching Parameters */
	
	/**
	  * \brief The averaging window size (in seconds) that is used to improve the results.
	  *
	  * A longer cache time improves the quality of the diffuse sound, but at the expense
	  * of longer response times to changes in the scene configuration. For instance, if
	  * a sound source moves and suddenly becomes inaudible to the listener, it may take
	  * at most this time for the diffuse sound field to react to the changes. A reasonable
	  * value is in the 0.2 to 0.7 second range where the error is usually not perceptible.
	  */
	GS_RESPONSE_TIME = 35,
	
	/**
	  * \brief The minimum time in seconds that a triangle stays in the visibility cache for source or listener.
	  *
	  * A longer cache time improves the quality of the visibility results and allows fewer visibility rays
	  * to be traced on each frame. Since the visibility cache is only an acceleration structure and does not
	  * directly affect the sound quality or introduce error, a value of several seconds can be used in order to maximize
	  * the set of cached visible triangles (accuracy always increases with a longer cache time).
	  * If the cache time is too long and the source is moving, there may be extra non-visible triangles stored in the cache
	  * that may make things slower than they could be (but won't introduce error).
	  */
	GS_VISIBILITY_CACHE_TIME = 36,
	
	
	/**********************************************************************************/
	/* Mesh Preprocessing Parameters */
	
	/**
	  * \brief The voxel size to use when remeshing the input mesh.
	  *
	  * The voxel size determines the 'resolution' of the resulting mesh.
	  * A larger voxel size will reduce the time to voxelize and remesh the
	  * mesh, but will also introduce more error in the output. A mesh
	  * is always inflated by 1/2 the voxel size if remeshing is enabled.
	  */
	GS_VOXEL_SIZE = 37,
	
	/**
	  * \brief The welding tolerance that the mesh preprocessor should use to weld vertices.
	  *
	  * If any two vertices in the mesh are closer to each other than this distance,
	  * they will be merged into one vertex and all triangles that share them
	  * will now reference the welded vertex.
	  */
	GS_WELD_TOLERANCE = 38,
	
	/**
	  * \brief The maximum allowed mesh simplification error.
	  *
	  * When the mesh's surface is being simplified, no simplification that deviates more than
	  * this distance from the original mesh will be allowed.
	  */
	GS_SIMPLIFY_TOLERANCE = 39,
	
	/**
	  * \brief The diffraction angle threshold used by the mesh preprocessor to find diffraction edges.
	  *
	  * If the angle between normals of two neighboring triangles
	  * is less than this value, then the edge that they share is not diffracting.
	  * Thus, a larger diffraction angle will result in less diffracting edges while
	  * lower thresold will result in more edges.
	  */
	GS_MIN_DIFFRACTION_EDGE_ANGLE = 40,
	
	/**
	  * \brief The minimum allowed length for a diffraction edge.
	  *
	  * All edges that are shorter than this length are not marked as diffraction edges.
	  */
	GS_MIN_DIFFRACTION_EDGE_LENGTH = 41,
	
	/** \brief The minimum number of rays per edge that should be used to test an edge-edge visibility. */
	GS_EDGE_RAY_COUNT_MIN = 42,
	
	/** \brief The maximum number of rays per edge that should be used to test an edge-edge visibility. */
	GS_EDGE_RAY_COUNT_MAX = 43,
	
	/** \brief The edge offset in meters that is used to bias away from diffraction edges when testing visibility. */
	GS_EDGE_OFFSET = 44,
	
	/** \brief The maximum allowed size for the diffuse subdivision patches for the mesh. */
	GS_DIFFUSE_RESOLUTION = 45,
	
	/** \brief The number of threads to use to compute a mesh preprocessing request. */
	GS_PREPROCESS_THREAD_COUNT = 46
	
} gsParameter;




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Channel Layout Enum														*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/**
  * \brief An enum describing the possible output channel layouts for a sound propagation renderer.
  *
  * In general, the cost of sound rendering scales linearly with the number
  * of output channels that must be rendered.
  */
typedef enum
{
	/** \brief An undefined channel layout, usually indicative of an error or invalid layout. */
	GS_CHANNEL_LAYOUT_UNDEFINED = 0,
	
	/**
	  * \brief A mono channel layout with 1 channel.
	  *
	  * This format does not perform any spatial audio processing.
	  */
	GS_CHANNEL_LAYOUT_MONO = 1,
	
	/**
	  * \brief A stereo channel layout with 2 channels (left and right).
	  *
	  * This format uses basic cosine panning to spatialize the sound.
	  */
	GS_CHANNEL_LAYOUT_STEREO = 2,
	
	/**
	  * \brief A channel layout with 2 channels that uses binaural spatial sound filtering.
	  * 
	  * An HRTF is used to spatialize the sound.
	  */
	GS_CHANNEL_LAYOUT_BINAURAL = 3,
	
	/**
	  * \brief A channel layout with 4 channels that corresponds to a quadrophonic speaker setup.
	  *
	  * Front left and right speakers are placed 30 degrees off of front center. Back left
	  * and right speakers are placed 95 degrees off of front center.
	  */
	GS_CHANNEL_LAYOUT_QUAD = 4,
	
	/**
	  * \brief A channel layout with 5 channels that corresponds to a 5.1 surround speaker setup.
	  *
	  * Front left and right speakers are placed 30 degrees off of front center. Back left
	  * and right speakers are placed 110 degrees off of front center. The low frequency
	  * channel is not used.
	  */
	GS_CHANNEL_LAYOUT_5_1 = 5,
	
	/**
	  * \brief A channel layout with 7 channels that corresponds to a 7.1 surround speaker setup.
	  *
	  * Front left and right speakers are placed 30 degrees off of front center. Back left
	  * and right speakers are placed 135 degrees off of front center. Side left and right
	  * speakers are placed 90 degrees off of front center. The low frequency
	  * channel is not used.
	  */
	GS_CHANNEL_LAYOUT_7_1 = 7,
	
} gsChannelLayout;




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Response Type Enum														*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/**
  * \brief An enum describing the different frequency responses for a sound material.
  */
typedef enum
{
	/** \brief A frequency response indicating the linear gain factor to apply when there is a reflection. */
	GS_REFLECTIVITY = 1,
	
	/** \brief A frequency response indicating the scattering coefficient for diffuse reflections. */
	GS_SCATTERING = 2,
	
	/** \brief A frequency response indicating the fraction of non-reflected sound that is transmitted through an object. */
	/*GS_TRANSMISSION = 3*/
	
} gsResponseType;




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Library Functions														*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/



/** \brief Initialize the library so that it can be used.
  * 
  * This function can be called more than once.
  */
void GSOUND_EXPORT gsInit();

/** \brief Completely reset and clean up the library, releasing all devices, systems, and objects. */
void GSOUND_EXPORT gsReset();


/** \brief Get the major, minor, and revision version numbers of the library in the output parameters. */
void GSOUND_EXPORT gsGetVersion( gsSize* major, gsSize* minor, gsSize* revision );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Device Functions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Initialize a new sound device that uses the default input and output. */
gsDeviceID GSOUND_EXPORT gsNewDefaultDevice();

/** \brief Destroy a sound device with the specified ID. */
void GSOUND_EXPORT gsDeleteDevice( gsDeviceID deviceID );

/** \brief Set the system and listener that should be auralized by the given device. */
gsBool GSOUND_EXPORT gsDeviceSetSystem( gsDeviceID deviceID, gsSystemID systemID, gsListenerID listenerID );

/** \brief Start audio output for this device. */
gsBool GSOUND_EXPORT gsDeviceStart( gsDeviceID deviceID );

/** \brief Stop audio output for this device. */
gsBool GSOUND_EXPORT gsDeviceStop( gsDeviceID deviceID );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound System Functions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Initialize a new sound system. */
gsSystemID GSOUND_EXPORT gsNewSystem();

/** \brief Destroy a sound system with the specified ID. */
void GSOUND_EXPORT gsDeleteSystem( gsSystemID systemID );

/** \brief Update the simulation with the specified ID. */
gsBool GSOUND_EXPORT gsSystemUpdate( gsSystemID systemID, gsFloat dt, gsBool synchronous );

/** \brief Set the scene that the specified sound system should simulate. */
gsBool GSOUND_EXPORT gsSystemSetScene( gsSystemID systemID, gsSceneID sceneID );

/** \brief Get the request that the specified sound system is using to simulate sound. */
gsBool GSOUND_EXPORT gsSystemGetRequest( gsSystemID systemID, gsRequestID* requestID );

/** \brief Set the request that the specified sound system is using to simulate sound. */
gsBool GSOUND_EXPORT gsSystemSetRequest( gsSystemID systemID, gsRequestID requestID );

/** \brief Add a listener that the specified sound system should render with the given request. */
gsBool GSOUND_EXPORT gsSystemAddListener( gsSystemID systemID, gsListenerID listenerID, gsRenderRequestID renderingRequestID );

/** \brief Remove a listener from a system that was previously being rendered. */
gsBool GSOUND_EXPORT gsSystemRemoveListener( gsSystemID systemID, gsListenerID listenerID );

/** \brief Read a buffer of interleaved audio samples for the specified listener. */
gsBool GSOUND_EXPORT gsSystemReadSamples( gsSystemID systemID, gsListenerID listenerID,
											gsFloat* samples, gsSize numSamples, gsSize numChannels );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Propagation Request Functions											*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Initialize a new propagation request that has the default parameters. */
gsRequestID GSOUND_EXPORT gsNewRequest();

/** \brief Destroy a propagation request with the specified ID. */
void GSOUND_EXPORT gsDeleteRequest( gsRequestID requestID );

/**
  * \brief Get the value of a boolean flag for the specified propagation request.
  *
  * The function responds to the following flags:
  * GS_DIRECT, GS_TRANSMISSION, GS_SPECULAR, GS_DIFFUSE, GS_DIFFRACTION,
  * GS_SPECULAR_CACHE, GS_DIFFUSE_CACHE, GS_IR_CACHE, GS_VISIBILITY_CACHE,
  * GS_DIFFUSE_SAMPLES, GS_SOURCE_DIFFUSE, GS_SOURCE_DIRECTIVITY, GS_SOURCE_CLUSTERING,
  * GS_AIR_ABSORPTION, GS_SAMPLED_IR, GS_SAMPLED_IR_SOURCE_DIRECTIONS,
  * GS_IR_THRESHOLD, GS_ADAPTIVE_IR_LENGTH, GS_ADAPTIVE_QUALITY, GS_DOPPLER_SORTING.
  */
gsBool GSOUND_EXPORT gsRequestGetFlag( gsRequestID requestID, gsFlag flag, gsBool* value );

/**
  * \brief Set the value of a boolean flag for the specified propagation request.
  *
  * The function responds to the following flags:
  * GS_DIRECT, GS_TRANSMISSION, GS_SPECULAR, GS_DIFFUSE, GS_DIFFRACTION,
  * GS_SPECULAR_CACHE, GS_DIFFUSE_CACHE, GS_IR_CACHE, GS_VISIBILITY_CACHE,
  * GS_DIFFUSE_SAMPLES, GS_SOURCE_DIFFUSE, GS_SOURCE_DIRECTIVITY, GS_SOURCE_CLUSTERING,
  * GS_AIR_ABSORPTION, GS_SAMPLED_IR, GS_SAMPLED_IR_SOURCE_DIRECTIONS,
  * GS_IR_THRESHOLD, GS_ADAPTIVE_IR_LENGTH, GS_ADAPTIVE_QUALITY, GS_DOPPLER_SORTING.
  */
gsBool GSOUND_EXPORT gsRequestSetFlag( gsRequestID requestID, gsFlag flag, gsBool value );

/**
  * \brief Get the value of a floating-point parameter for the specified propagation request.
  *
  * The function responds to the following parameters:
  * GS_TARGET_DT, GS_IR_MIN_LENGTH, GS_IR_MAX_LENGTH, GS_IR_GROWTH_RATE,
  * GS_QUALITY, GS_MIN_QUALITY, GS_MAX_QUALITY, GS_SAMPLE_RATE,
  * GS_DOPPLER_THRESHOLD, GS_DIFFUSE_SAMPLE_PROBABILITY, GS_RAY_OFFSET,
  * GS_DIFFUSE_CACHE_TIME, GS_VISIBILITY_CACHE_TIME.
  */
gsBool GSOUND_EXPORT gsRequestGetParamF( gsRequestID requestID, gsParameter parameter, gsFloat* value );

/**
  * \brief Set the value of a floating-point parameter for the specified propagation request.
  *
  * The function responds to the following parameters:
  * GS_TARGET_DT, GS_IR_MIN_LENGTH, GS_IR_MAX_LENGTH, GS_IR_GROWTH_RATE,
  * GS_QUALITY, GS_MIN_QUALITY, GS_MAX_QUALITY, GS_SAMPLE_RATE,
  * GS_DOPPLER_THRESHOLD, GS_DIFFUSE_SAMPLE_PROBABILITY, GS_RAY_OFFSET,
  * GS_DIFFUSE_CACHE_TIME, GS_VISIBILITY_CACHE_TIME.
  */
gsBool GSOUND_EXPORT gsRequestSetParamF( gsRequestID requestID, gsParameter parameter, gsFloat value );

/** \brief Get the value of an integer parameter for the specified propagation request.
  *
  * The function responds to the following parameters:
  * GS_PROPAGATION_THREAD_COUNT, GS_DIRECT_RAY_COUNT, GS_DIFFRACTION_MAX_DEPTH,
  * GS_DIFFRACTION_MAX_ORDER, GS_SPECULAR_MAX_DEPTH, GS_SPECULAR_RAY_COUNT,
  * GS_SPECULAR_SAMPLE_COUNT, GS_DIFFUSE_MAX_DEPTH, GS_DIFFUSE_RAY_COUNT,
  * GS_DIFFUSE_SAMPLE_COUNT, GS_VISIBILITY_RAY_COUNT.
  */
gsBool GSOUND_EXPORT gsRequestGetParamI( gsRequestID requestID, gsParameter parameter, gsSize* value );

/** \brief Set the value of an integer parameter for the specified propagation request.
  *
  * The function responds to the following parameters:
  * GS_PROPAGATION_THREAD_COUNT, GS_DIRECT_RAY_COUNT, GS_DIFFRACTION_MAX_DEPTH,
  * GS_DIFFRACTION_MAX_ORDER, GS_SPECULAR_MAX_DEPTH, GS_SPECULAR_RAY_COUNT,
  * GS_SPECULAR_SAMPLE_COUNT, GS_DIFFUSE_MAX_DEPTH, GS_DIFFUSE_RAY_COUNT,
  * GS_DIFFUSE_SAMPLE_COUNT, GS_VISIBILITY_RAY_COUNT.
  */
gsBool GSOUND_EXPORT gsRequestSetParamI( gsRequestID requestID, gsParameter parameter, gsSize value );

/** \brief Reset the internal cached propagation data for the specified propagation request. */
gsBool GSOUND_EXPORT gsRequestReset( gsRequestID requestID );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Render Request Functions												*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Initialize a new render request that has the default parameters. */
gsRenderRequestID GSOUND_EXPORT gsNewRenderRequest();

/** \brief Destroy a render request with the specified ID. */
void GSOUND_EXPORT gsDeleteRenderRequest( gsRenderRequestID requestID );

/**
  * \brief Get the value of a boolean flag for the specified render request.
  *
  * The function responds to the following flags:
  * GS_CONVOLUTION, GS_DISCRETE_PATHS.
  */
gsBool GSOUND_EXPORT gsRenderRequestGetFlag( gsRenderRequestID requestID, gsFlag flag, gsBool* value );

/**
  * \brief Set the value of a boolean flag for the specified render request.
  *
  * The function responds to the following flags:
  * GS_CONVOLUTION, GS_DISCRETE_PATHS.
  */
gsBool GSOUND_EXPORT gsRenderRequestSetFlag( gsRenderRequestID requestID, gsFlag flag, gsBool value );

/**
  * \brief Get the value of a floating-point parameter for the specified render request.
  *
  * The function responds to the following parameters:
  * GS_SAMPLE_RATE, GS_IR_MAX_LENGTH, GS_MAX_LATENCY, GS_MAX_PATH_DELAY, GS_IR_FADE_TIME,
  * GS_PATH_FADE_TIME, GS_SOURCE_FADE_TIME, GS_CLUSTER_FADE_IN_TIME, GS_CLUSTER_FADE_OUT_TIME,
  * GS_VOLUME.
  */
gsBool GSOUND_EXPORT gsRenderRequestGetParamF( gsRenderRequestID requestID, gsParameter parameter, gsFloat* value );

/**
  * \brief Set the value of a floating-point parameter for the specified render request.
  *
  * The function responds to the following parameters:
  * GS_SAMPLE_RATE, GS_IR_MAX_LENGTH, GS_MAX_LATENCY, GS_MAX_PATH_DELAY, GS_IR_FADE_TIME,
  * GS_PATH_FADE_TIME, GS_SOURCE_FADE_TIME, GS_CLUSTER_FADE_IN_TIME, GS_CLUSTER_FADE_OUT_TIME,
  * GS_VOLUME.
  */
gsBool GSOUND_EXPORT gsRenderRequestSetParamF( gsRenderRequestID requestID, gsParameter parameter, gsFloat value );

/**
  * \brief Get the value of an integer parameter for the specified render request.
  *
  * The function responds to the following parameters:
  * GS_CHANNEL_LAYOUT, GS_UPDATE_THREAD_COUNT, GS_MAX_SOURCE_PATH_COUNT.
  */
gsBool GSOUND_EXPORT gsRenderRequestGetParamI( gsRenderRequestID requestID, gsParameter parameter, gsSize* value );

/**
  * \brief Set the value of an integer parameter for the specified render request.
  *
  * The function responds to the following parameters:
  * GS_CHANNEL_LAYOUT, GS_UPDATE_THREAD_COUNT, GS_MAX_SOURCE_PATH_COUNT.
  */
gsBool GSOUND_EXPORT gsRenderRequestSetParamI( gsRenderRequestID requestID, gsParameter parameter, gsSize value );

/**
  * \brief Set the ID of the HRTF that the specified render request is using.
  *
  * If the HRTF and request are valid, the function returns TRUE to indicate success.
  */
gsBool GSOUND_EXPORT gsRenderRequestSetHRTF( gsRenderRequestID requestID, gsHRTFID hrtfID );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Mesh Request Functions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Initialize a new mesh request that has the default parameters. */
gsMeshRequestID GSOUND_EXPORT gsNewMeshRequest();

/** \brief Destroy a mesh request with the specified ID. */
void GSOUND_EXPORT gsDeleteMeshRequest( gsMeshRequestID requestID );

/**
  * \brief Get the value of a boolean flag for the specified mesh request.
  *
  * The function responds to the following flags:
  * GS_DIFFRACTION_EDGES, GS_DIFFRACTION_GRAPH, GS_VOXELIZE, GS_WELD, GS_SIMPLIFIY.
  */
gsBool GSOUND_EXPORT gsMeshRequestGetFlag( gsMeshRequestID requestID, gsFlag flag, gsBool* value );

/**
  * \brief Set the value of a boolean flag for the specified mesh request.
  *
  * The function responds to the following flags:
  * GS_DIFFRACTION_EDGES, GS_DIFFRACTION_GRAPH, GS_VOXELIZE, GS_WELD, GS_SIMPLIFIY.
  */
gsBool GSOUND_EXPORT gsMeshRequestSetFlag( gsMeshRequestID requestID, gsFlag flag, gsBool value );

/**
  * \brief Get the value of a floating-point parameter for the specified mesh request.
  *
  * The function responds to the following parameters:
  * GS_VOXEL_SIZE, GS_WELD_TOLERANCE, GS_SIMPLIFY_TOLERANCE, GS_MIN_DIFFRACTION_EDGE_ANGLE,
  * GS_MIN_DIFFRACTION_EDGE_LENGTH, GS_EDGE_OFFSET, GS_DIFFUSE_RESOLUTION.
  */
gsBool GSOUND_EXPORT gsMeshRequestGetParamF( gsMeshRequestID requestID, gsParameter parameter, gsFloat* value );

/**
  * \brief Set the value of a floating-point parameter for the specified mesh request.
  *
  * The function responds to the following parameters:
  * GS_VOXEL_SIZE, GS_WELD_TOLERANCE, GS_SIMPLIFY_TOLERANCE, GS_MIN_DIFFRACTION_EDGE_ANGLE,
  * GS_MIN_DIFFRACTION_EDGE_LENGTH, GS_EDGE_OFFSET, GS_DIFFUSE_RESOLUTION.
  */
gsBool GSOUND_EXPORT gsMeshRequestSetParamF( gsMeshRequestID requestID, gsParameter parameter, gsFloat value );

/**
  * \brief Get the value of an integer parameter for the specified mesh request.
  *
  * The function responds to the following parameters:
  * GS_EDGE_RAY_COUNT_MIN, GS_EDGE_RAY_COUNT_MAX, GS_PREPROCESS_THREAD_COUNT.
  */
gsBool GSOUND_EXPORT gsMeshRequestGetParamI( gsMeshRequestID requestID, gsParameter parameter, gsSize* value );

/**
  * \brief Set the value of an integer parameter for the specified mesh request.
  *
  * The function responds to the following parameters:
  * GS_EDGE_RAY_COUNT_MIN, GS_EDGE_RAY_COUNT_MAX, GS_PREPROCESS_THREAD_COUNT.
  */
gsBool GSOUND_EXPORT gsMeshRequestSetParamI( gsMeshRequestID requestID, gsParameter parameter, gsSize value );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Scene Functions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new empty sound scene. */
gsSceneID GSOUND_EXPORT gsNewScene();

/** \brief Destroy the sound scene with the specified ID. */
void GSOUND_EXPORT gsDeleteScene( gsSceneID sceneID );



/** \brief Get the number of objects that are in a scene. */
gsBool GSOUND_EXPORT gsSceneGetObjectCount( gsSceneID sceneID, gsSize* objectCount );

/** \brief Add the specified object to a scene. */
gsBool GSOUND_EXPORT gsSceneAddObject( gsSceneID sceneID, gsObjectID objectID );

/** \brief Remove the specified object from a scene. */
gsBool GSOUND_EXPORT gsSceneRemoveObject( gsSceneID sceneID, gsObjectID objectID );

/** \brief Remove all objects from a scene. */
gsBool GSOUND_EXPORT gsSceneClearObjects( gsSceneID sceneID );



/** \brief Get the number of sound sources that are in a scene. */
gsBool GSOUND_EXPORT gsSceneGetSourceCount( gsSceneID sceneID, gsSize* sourceCount );

/** \brief Add the specified sound source to a scene. */
gsBool GSOUND_EXPORT gsSceneAddSource( gsSceneID sceneID, gsSourceID sourceID );

/** \brief Remove the specified sound source from a scene. */
gsBool GSOUND_EXPORT gsSceneRemoveSource( gsSceneID sceneID, gsSourceID sourceID );

/** \brief Remove all sound sources from a scene. */
gsBool GSOUND_EXPORT gsSceneClearSources( gsSceneID sceneID );



/** \brief Get the number of sound listeners that are in a scene. */
gsBool GSOUND_EXPORT gsSceneGetListenerCount( gsSceneID sceneID, gsSize* listenerCount );

/** \brief Add the specified sound listener to a scene. */
gsBool GSOUND_EXPORT gsSceneAddListener( gsSceneID sceneID, gsSourceID listenerID );

/** \brief Remove the specified sound listener from a scene. */
gsBool GSOUND_EXPORT gsSceneRemoveListener( gsSceneID sceneID, gsSourceID listenerID );

/** \brief Remove all sound listeners from a scene. */
gsBool GSOUND_EXPORT gsSceneClearListeners( gsSceneID sceneID );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Source Functions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new sound source. */
gsSourceID GSOUND_EXPORT gsNewSource();

/** \brief Destroy the sound source with the specified ID. */
void GSOUND_EXPORT gsDeleteSource( gsSourceID sourceID );

/** \brief Get the position of the center of the specified sound source. */
gsBool GSOUND_EXPORT gsSourceGetPosition( gsSourceID sourceID, gsFloat* x, gsFloat* y, gsFloat* z );

/** \brief Set the position of the center of the specified sound source. */
gsBool GSOUND_EXPORT gsSourceSetPosition( gsSourceID sourceID, gsFloat x, gsFloat y, gsFloat z );

/** \brief Get the velocity of the specified sound source. */
gsBool GSOUND_EXPORT gsSourceGetVelocity( gsSourceID sourceID, gsFloat* vx, gsFloat* vy, gsFloat* vz );

/** \brief Set the velocity of the specified sound source. */
gsBool GSOUND_EXPORT gsSourceSetVelocity( gsSourceID sourceID, gsFloat vx, gsFloat vy, gsFloat vz );

/** \brief Get the radius of the specified sound source's spherical detector. */
gsBool GSOUND_EXPORT gsSourceGetRadius( gsSourceID sourceID, gsFloat* radius );

/** \brief Set the radius of the specified sound source's spherical detector. */
gsBool GSOUND_EXPORT gsSourceSetRadius( gsSourceID sourceID, gsFloat radius );

/** \brief Get the orientation of the specified sound source. */
gsBool GSOUND_EXPORT gsSourceGetOrientation( gsSourceID sourceID, gsFloat* xx, gsFloat* xy, gsFloat* xz,
																	gsFloat* yx, gsFloat* yy, gsFloat* yz );

/** \brief Set the orientation of the specified sound source. */
gsBool GSOUND_EXPORT gsSourceSetOrientation( gsSourceID sourceID, gsFloat xx, gsFloat xy, gsFloat xz,
																	gsFloat yx, gsFloat yy, gsFloat yz );

/** \brief Get the power of the specified sound source in watts. */
gsBool GSOUND_EXPORT gsSourceGetPower( gsSourceID sourceID, gsFloat* power );

/** \brief Set the power of the specified sound source in watts. */
gsBool GSOUND_EXPORT gsSourceSetPower( gsSourceID sourceID, gsFloat power );

/** \brief Get the power of the specified sound source in dB SWL, 10^-12 W reference. */
gsBool GSOUND_EXPORT gsSourceGetPowerLevel( gsSourceID sourceID, gsFloat* powerDBSWL );

/** \brief Set the power of the specified sound source in dB SWL, 10^-12 W reference. */
gsBool GSOUND_EXPORT gsSourceSetPowerLevel( gsSourceID sourceID, gsFloat powerDBSWL );

/**
  * \brief Get the sample rate of the sound source.
  *
  * This sample rate should match the sample rate of the rendering and propagation
  * systems. If there is a mismatch, sample rate conversion is performed but
  * is slow and can introduce artifacts.
  */
gsBool GSOUND_EXPORT gsSourceGetSampleRate( gsSourceID sourceID, gsSampleRate* sampleRate );

/**
  * \brief Set the sample rate of the sound source.
  *
  * This sample rate should match the sample rate of the rendering and propagation
  * systems. If there is a mismatch, sample rate conversion is performed but
  * is slow and can introduce artifacts.
  */
gsBool GSOUND_EXPORT gsSourceSetSampleRate( gsSourceID sourceID, gsSampleRate sampleRate );

/** \brief Play the specified sound through the given source. */
gsBool GSOUND_EXPORT gsSourcePlaySound( gsSourceID sourceID, gsSoundID soundID, gsFloat volume, gsBool loop );

/** \brief Pause playing the specified sound through the given source. */
gsBool GSOUND_EXPORT gsSourcePauseSound( gsSourceID sourceID, gsSoundID soundID );

/** \brief Resume playing the specified sound through the given source. */
gsBool GSOUND_EXPORT gsSourceResumeSound( gsSourceID sourceID, gsSoundID soundID );

/** \brief Stop playing the specified sound through the given source. */
gsBool GSOUND_EXPORT gsSourceStopSound( gsSourceID sourceID, gsSoundID soundID );

/** \brief Stop playing the all sounds through the given source. */
gsBool GSOUND_EXPORT gsSourceStopSounds( gsSourceID sourceID );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Listener Functions												*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new sound listener. */
gsListenerID GSOUND_EXPORT gsNewListener();

/** \brief Destroy the sound listener with the specified ID. */
void GSOUND_EXPORT gsDeleteListener( gsListenerID listenerID );

/** \brief Get the position of the center of the specified sound listener. */
gsBool GSOUND_EXPORT gsListenerGetPosition( gsListenerID listenerID, gsFloat* x, gsFloat* y, gsFloat* z );

/** \brief Set the position of the center of the specified sound listener. */
gsBool GSOUND_EXPORT gsListenerSetPosition( gsListenerID listenerID, gsFloat x, gsFloat y, gsFloat z );

/** \brief Get the velocity of the specified sound listener. */
gsBool GSOUND_EXPORT gsListenerGetVelocity( gsListenerID listenerID, gsFloat* vx, gsFloat* vy, gsFloat* vz );

/** \brief Set the velocity of the specified sound listener. */
gsBool GSOUND_EXPORT gsListenerSetVelocity( gsListenerID listenerID, gsFloat vx, gsFloat vy, gsFloat vz );

/** \brief Get the radius of the specified listener source's spherical detector. */
gsBool GSOUND_EXPORT gsListenerGetRadius( gsListenerID listenerID, gsFloat* radius );

/** \brief Set the radius of the specified listener source's spherical detector. */
gsBool GSOUND_EXPORT gsListenerSetRadius( gsListenerID listenerID, gsFloat radius );

/** \brief Get the sensitivity in dB SPL of the specified listener's rendered audio. */
gsBool GSOUND_EXPORT gsListenerGetSensitivity( gsListenerID listenerID, gsFloat* sensitivity );

/** \brief Set the sensitivity in dB SPL of the specified listener's rendered audio. */
gsBool GSOUND_EXPORT gsListenerSetSensitivity( gsListenerID listenerID, gsFloat sensitivity );

/** \brief Get the orientation of the specified sound listener. */
gsBool GSOUND_EXPORT gsListenerGetOrientation( gsListenerID listenerID, gsFloat* xx, gsFloat* xy, gsFloat* xz,
																		gsFloat* yx, gsFloat* yy, gsFloat* yz );

/** \brief Set the orientation of the specified sound listener. */
gsBool GSOUND_EXPORT gsListenerSetOrientation( gsListenerID listenerID, gsFloat xx, gsFloat xy, gsFloat xz,
																		gsFloat yx, gsFloat yy, gsFloat yz );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Object Functions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new sound object with no mesh. */
gsObjectID GSOUND_EXPORT gsNewObject();

/** \brief Destroy the sound object with the specified ID. */
void GSOUND_EXPORT gsDeleteObject( gsObjectID objectID );

/** \brief Get the position of the center of the specified sound object. */
gsBool GSOUND_EXPORT gsObjectGetPosition( gsObjectID objectID, gsFloat* x, gsFloat* y, gsFloat* z );

/** \brief Set the position of the center of the specified sound object. */
gsBool GSOUND_EXPORT gsObjectSetPosition( gsObjectID objectID, gsFloat x, gsFloat y, gsFloat z );

/** \brief Get the orientation of the specified sound object. */
gsBool GSOUND_EXPORT gsObjectGetOrientation( gsObjectID objectID, gsFloat* xx, gsFloat* xy, gsFloat* xz,
																gsFloat* yx, gsFloat* yy, gsFloat* yz );

/** \brief Set the orientation of the specified sound object. */
gsBool GSOUND_EXPORT gsObjectSetOrientation( gsObjectID objectID, gsFloat xx, gsFloat xy, gsFloat xz,
																gsFloat yx, gsFloat yy, gsFloat yz );

/** \brief Get the scale of the specified sound object. */
gsBool GSOUND_EXPORT gsObjectGetScale( gsObjectID objectID, gsFloat* scaleX, gsFloat* scaleY, gsFloat* scaleZ );

/** \brief Set the scale of the specified sound object. */
gsBool GSOUND_EXPORT gsObjectSetScale( gsObjectID objectID, gsFloat scaleX, gsFloat scaleY, gsFloat scaleZ );

/** \brief Get the mesh ID for the object with the specified ID. */
gsBool GSOUND_EXPORT gsObjectGetMesh( gsObjectID objectID, gsMeshID* meshID );

/** \brief Set the mesh for the object with the specified ID. */
gsBool GSOUND_EXPORT gsObjectSetMesh( gsObjectID objectID, gsMeshID meshID );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Mesh Functions													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new mesh with no mesh data. */
gsMeshID GSOUND_EXPORT gsNewMesh();

/** \brief Destroy the sound mesh with the specified ID. */
void GSOUND_EXPORT gsDeleteMesh( gsMeshID mesh );

/**
  * \brief Replace the mesh with the specified vertices, triangles, and materials.
  *
  * The current contents of the mesh are replaced with a new mesh from the given data.
  * The mesh is preprocessed using the specified system and mesh request.
  * This method may potentially take several seconds to execute, depending on settings.
  */
gsBool GSOUND_EXPORT gsMeshSetData( gsMeshID mesh, gsSystemID systemID, gsMeshRequestID meshRequestID,
									const gsFloat* vertices, gsSize numVertices,
									const gsIndex* triangles, gsSize numTriangles,
									const gsMaterialID* materialIDs, gsSize numMaterials );

/** \brief Replace the material at the given index in a mesh with a new material. */
gsBool GSOUND_EXPORT gsMeshSetMaterial( gsMeshID meshID, gsIndex materialIndex, gsMaterialID materialID );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Material Functions												*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new default material. */
gsMaterialID GSOUND_EXPORT gsNewMaterial();

/** \brief Destroy the material with the specified ID. */
void GSOUND_EXPORT gsDeleteMaterial( gsMaterialID materialID );

/** \brief Get the interpolated value for a frequency in the specified response type that is part of a material. */
gsBool GSOUND_EXPORT gsMaterialGet( gsMaterialID materialID, gsResponseType responseType, gsFloat frequency, gsFloat* value );

/** \brief Add a frequency/value pair the specified response type that is part of a material. */
gsBool GSOUND_EXPORT gsMaterialSet( gsMaterialID materialID, gsResponseType responseType, gsFloat frequency, gsFloat value );

/** \brief Reset the given response in a material to a flat response with the specified value. */
gsBool GSOUND_EXPORT gsMaterialReset( gsMaterialID materialID, gsResponseType responseType, gsFloat value );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		Sound Functions															*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new sound with no sound data. */
gsSoundID GSOUND_EXPORT gsNewSound();

/** \brief Destroy the sound with the specified ID. */
void GSOUND_EXPORT gsDeleteSound( gsSoundID soundID );


/**
  * \brief Replace a sound with data that is located at the specified sound file path, given by a NULL-terminated UTF-8 string.
  *
  * If the stream parameter is set to TRUE, the sound is streamed from the file
  * (slower but saves memory). Otherwise, the sound is loaded into memory and
  * played from there.
  * 
  * The following file formats are supported:
  * WAVE, AIFF
  */
gsBool GSOUND_EXPORT gsSoundSetFile( gsSoundID soundID, const char* filePath, gsBool streaming );


/**
  * \brief Replace a sound with data from the specified channel-interleaved sample pointer.
  *
  * Interleaved sample data is read from the given sample pointer and
  * used to replace the sound's previous data. There must be at least
  * numChannels*numSamples values in the sample array.
  */
gsBool GSOUND_EXPORT gsSoundSetData( gsSoundID soundID, const gsFloat* samples, gsSize numChannels,
									gsSize numSamples, gsSampleRate sampleRate );




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		HRTF Functions															*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/** \brief Create a new HRTF with no data. */
gsHRTFID GSOUND_EXPORT gsNewHRTF();

/** \brief Destroy the HRTF with the specified ID. */
void GSOUND_EXPORT gsDeleteHRTF( gsHRTFID hrtfID );


/**
  * \brief Replace an HRTF with data that is located at the specified file path, given by a NULL-terminated UTF-8 string.
  */
gsBool GSOUND_EXPORT gsHRTFSetFile( gsHRTFID hrtfID, const char* filePath );




#ifdef __cplusplus
} /* End of C interface. */
#endif /* __cplusplus */




/*##########################################################################################*/
/*##########################################################################################*/
/*############																				*/
/*############		C++ Interface Extension													*/
/*############																				*/
/*##########################################################################################*/
/*##########################################################################################*/




/**
  * Define GSOUND_C_BRIDGE to enable the C/C++ interface bridge.
  * This allows the user to access the C++ objects directly, but
  * requires publicly including a large number of dependencies,
  * so it is disabled by default to keep the header clean.
  */
#if defined(GSOUND_C_BRIDGE) && defined(__cplusplus)

#include "gsound.h"

/** \brief Return a pointer to the sound propagation request with the specified ID. */
gsound::PropagationRequest* gsGetRequest( gsRequestID requestID );

/** \brief Return a pointer to the render request with the specified ID. */
gsound::RenderRequest* gsGetRenderRequest( gsRenderRequestID requestID );

/** \brief Return a pointer to the mesh request with the specified ID. */
gsound::MeshRequest* gsGetMeshRequest( gsMeshRequestID requestID );

/** \brief Return a pointer to the sound system with the specified ID. */
gsound::SoundPropagationSystem* gsGetSystem( gsSystemID systemID );

/** \brief Return a pointer to the sound scene with the specified ID. */
gsound::SoundScene* gsGetScene( gsSceneID sceneID );

/** \brief Return a pointer to the sound source with the specified ID. */
gsound::SoundSource* gsGetSource( gsSourceID sourceID );

/** \brief Return a pointer to the sound listener with the specified ID. */
gsound::SoundListener* gsGetListener( gsListenerID listenerID );

/** \brief Return a pointer to the sound object with the specified ID. */
gsound::SoundObject* gsGetObject( gsObjectID objectID );

/** \brief Return a pointer to the sound mesh with the specified ID. */
gsound::SoundMesh* gsGetMesh( gsMeshID meshID );

/** \brief Return a pointer to the sound material with the specified ID. */
gsound::SoundMaterial* gsGetMesh( gsMaterialID materialID );

#endif /* __cplusplus */





#endif // INCLUDE_GSOUND_C_H
