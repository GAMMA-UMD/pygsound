/*
 * Project:     GSound
 * 
 * File:        gsound/gsRenderRequest.h
 * Contents:    gsound::RenderRequest class declaration
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


#ifndef INCLUDE_GSOUND_RENDER_REQUEST_H
#define INCLUDE_GSOUND_RENDER_REQUEST_H


#include "gsConfig.h"


#include "gsRenderFlags.h"
#include "gsFrequencyBands.h"
#include "gsSoundStatistics.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that describes how rendering should be performed.
/**
  * It contains parameters that determine how the rendering should be done
  * as well as parameters determining the quality
  * and performance characteristics of the rendering.
  */
class RenderRequest
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a render request object with the default initial parameters.
			RenderRequest();
			
			
		//********************************************************************************
		//******	Rendering Parameters
			
			
			/// An object which indicates the boolean configuration state for sound propagation rendering.
			RenderFlags flags;
			
			
			/// The channel layout that should be use for rendering this request.
			ChannelLayout channelLayout;
			
			
			/// An optional pointer to the head-related transfer function that should be used to render this request.
			/**
			  * An HRTF specifies how sound is filtered by the listener's head/outer ear geometry.
			  * This filtering can give the listener aural queues to the 3D direction of sound sources.
			  * For HRTF rendering to be enabled, this pointer must be non-NULL, represent a valid HRTF, and
			  * the RenderFlags::HRTF and RenderFlags::CONVOLUTION flags must be set.
			  * In addition, the number of channels in the channel layout must match the number of
			  * channels in the HRTF. Otherwise, the channel layout is used for spatial sound
			  * rendering instead of the HRTF.
			  */
			const HRTF* hrtf;
			
			
			/// The maximum spherical harmonic order that can be used to represent the HRTF.
			/**
			  * A spherical-harmonic representation is used to smoothly interpolate the
			  * HRTF measurement samples. The order (0, 1, 2,...) affects the quality:
			  * a higher-order HRTF will better match the original measured HRTF, but
			  * at the cost of extra storage required and extra computational cost. The
			  * overhead increases like O(N^2) for increasing order. Usually order 3 or 4 is
			  * sufficient for good-enough sound quality.
			  */
			Size maxHRTFOrder;
			
			
			/// The sample rate at which sampled impulse responses should be computed and audio rendering should be performed.
			/**
			  * If using sampled impulse responses, this value should match the sample rate of the
			  * PropagationRequest.
			  */
			SampleRate sampleRate;
			
			
			/// An object that describes the frequency bands for which to perform rendering.
			/**
			  * This object contains a fixed number of frequency bands that determine the propagation
			  * and rendering frequency bands.
			  */
			FrequencyBands frequencies;
			
			
			/// A pointer to an optional object which recieve runtime information about the propagation system and scene.
			/**
			  * If statistics are enabled and this pointer is not NULL, the propagation system sets
			  * data in the object that indicate the current performance of the system.
			  * This pointer may be NULL, indicating that analytic information is not required at all.
			  */
			SoundStatistics* statistics;
			
			
			/// The number of threads that should be used for audio rendering.
			/**
			  * This value determines how many threads are used for audio rendering
			  * tasks. Increasing this value on multiprocessor systems can increase
			  * the number of sources that can be rendered in real time.
			  */
			Size numThreads;
			
			
			/// The number of threads to use for updating rendered IRs.
			/**
			  * This value determines how many threads are used within the renderer
			  * to update the IR on each frame.
			  */
			Size numUpdateThreads;
			
			
			/// The maximum IR length that the system can render, in seconds.
			/**
			  * Any IR samples after this length are discarded for rendering. The memory required
			  * for rendering scales roughly linearly with this value, depending
			  * on the length of the input IRs, but the memory scales in terms of the FFT buffer
			  * size, so there may be large increases in the memory required for a small increase in the max
			  * IR length, especially at long IR lengths.
			  * 
			  * Rendering performance, as well as the number of threads required scales
			  * logarithmically with the length of the IR.
			  */
			Float maxIRLength;
			
			
			/// The maximum allowed processing latency in seconds for the sound propagation renderer.
			/**
			  * The renderer will attempt, if it is possible, to process its audio stream with a latency that is
			  * less than or equal to this latency. This value should be set as large as possible
			  * without the delay being perceptually noticeable in order to make the renderer
			  * resistant to processing stalls due to other threads temporarily using CPU time.
			  * The smaller the latency, the more work that must be done when performing block
			  * convolution, and the more threads that are necessary.
			  */
			Float maxLatency;
			
			
			/// The maximum allowed number of discrete paths that should be rendered per sound source.
			Size maxSourcePathCount;
			
			
			/// The maximum delay time in seconds that a discrete propagation path is allowed to have.
			/**
			  * This value determines how much of a sound source's input audio is buffered in time domain
			  * to allow discrete path rendering. Since most discrete paths (i.e. direct, diffraction, specular)
			  * are in the first 0.5s of most IRs, this value can generally be set much lower than
			  * maxIRLength. A smaller value will reduce memory requirements.
			  */
			Float maxPathDelay;
			
			
			/// The maximum rate (in seconds per second) that a discrete path's delay time is allowed to change.
			Float maxDelayRate;
			
			
			/// The minimum time in seconds that it takes to interpolate to a new impulse response.
			/**
			  * The actual fade time is lower bounded by the length of the FFT buffer, which depends on the
			  * delay time. IR partitions that are more delayed are interpolated more slowly to
			  * reduce artifacts and reduce the update rate for those partitions.
			  */
			Float irFadeTime;
			
			
			/// The time in seconds that it takes a discrete path to interpolate to a new volume.
			/**
			  * Setting this to a larger value results in smoother audio but can cause more
			  * paths to be rendered because paths are not removed until they are completely
			  * faded out. This can result in the maxSourcePathCount limit being exceeded
			  * for short periods of time while the previous paths are faded out.
			  */
			Float pathFadeTime;
			
			
			/// The minimum time in seconds that it takes to interpolate to a new HRTF.
			/**
			  * The actual fade time is lower bounded by the length of the FFT buffer.
			  */
			Float hrtfFadeTime;
			
			
			/// The time in seconds that it takes a source's audio to fade in or out when added to or removed from a cluster.
			Float sourceFadeTime;
			
			
			/// The time in seconds that it takes a cluster's audio to fade in when it is new.
			Float clusterFadeInTime;
			
			
			/// The time in seconds that it takes a cluster's audio to fade out (and then be removed) when it is out of date.
			Float clusterFadeOutTime;
			
			
			/// A constant linear gain factor that all rendered sound is scaled by.
			/**
			  * This value can be used to adjust the output volume of a renderer independent
			  * of the listener's sensitivity.
			  */
			Float volume;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_RENDER_REQUEST_H
