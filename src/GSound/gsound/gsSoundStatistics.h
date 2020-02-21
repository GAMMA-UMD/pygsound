/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundStatistics.h
 * Contents:    gsound::SoundStatistics class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_STATISTICS_H
#define INCLUDE_GSOUND_SOUND_STATISTICS_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores statistics about the runtime performance of sound propagation and rendering.
class SoundStatistics
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sound statistics object.
			SoundStatistics();
			
			
		//********************************************************************************
		//******	Count Statistics
			
			
			/// Th enumber of enabled listeners in the scene on the last frame.
			Size listenerCount;
			
			
			/// The number of enabled sources in the scene on the last frame.
			Size sourceCount;
			
			
			/// The number of source clusters in the scene on the last frame.
			Size sourceClusterCount;
			
			
			/// The number of objects there were in the scene on the last frame.
			Size objectCount;
			
			
			/// The number of mesh triangles in the scene on the last frame.
			Size triangleCount;
			
			
			/// The number of mesh vertices in the scene on the last frame.
			Size vertexCount;
			
			
			/// The number of diffraction edges in the scene on the last frame.
			Size edgeCount;
			
			
			/// The total number of discrete sound paths that were computed and output on the last frame.
			Size pathCount;
			
			
			/// The total number of primary specular rays that were traced on the last frame.
			/**
			  * For enclosed scenes, this value will be roughly the same as the requested
			  * number of specular rays. For outdoor scenes, this value can be higher because
			  * many rays escape the scene after only a few bounces. The system automatically
			  * increases the number of primary specular rays if this occurs.
			  */
			Size specularRayCount;
			
			
			/// The total number of primary diffuse rays that were traced on the last frame.
			/**
			  * For enclosed scenes, this value will be roughly the same as the requested
			  * number of diffuse rays. For outdoor scenes, this value can be higher because
			  * many rays escape the scene after only a few bounces. The system automatically
			  * increases the number of primary diffuse rays if this occurs.
			  */
			Size diffuseRayCount;
			
			
			/// The average number of times that diffuse rays were reflected on the last frame.
			Size diffuseRayDepth;
			
			
			/// The total number of rays that were cast on the last frame.
			/**
			  * This includes every ray cast or visibility check that occurred on the
			  * last frame. This value is roughly proportional to the propagation cost.
			  */
			Size rayCastCount;
			
			
			/// The total number of discrete sound paths that are currently being rendered.
			Size renderedPathCount;
			
			
			/// The average length of all source IRs that were computed.
			Float averageIRLength;
			
			
			/// The length of the longest source IR that was computed.
			Float maxIRLength;
			
			
		//********************************************************************************
		//******	Runtime Performance Timing Statistics
			
			
			/// The time spent on source clustering on the last frame.
			Time clusteringTime;
			
			
			/// The time spent on rat tracing on the last frame.
			Time rayTracingTime;
			
			
			/// The time spent on cache updates on the last frame.
			Time cacheUpdateTime;
			
			
			/// The total time spent on sound propagation on the last frame.
			Time propagationTime;
			
			
			/// The time spent updating the IR for the sound renderer on the last frame.
			Time irUpdateTime;
			
			
			/// The current buffering load, from 0 to 1, indicating the fraction of real time spent buffering source audio.
			Float bufferingLoad;
			
			
			/// The current audio rendering load, from 0 to 1, indicating the fraction of real time spent rendering.
			Float renderingLoad;
			
			
			/// The latency in seconds of the rendering system.
			Float renderingLatency;
			
			
		//********************************************************************************
		//******	Preprocessing Timing Statistics
			
			
			/// The total time in seconds that it took to preprocess the last mesh.
			Time preprocessTime;
			
			
			/// The time in seconds that it took to voxelize and then triangulate the last mesh.
			Time remeshTime;
			
			
			/// The time in seconds that it took to weld vertices for the last mesh.
			Time weldTime;
			
			
			/// The time in seconds that it took to simplify the last mesh.
			Time simplifyTime;
			
			
			/// The time in seconds that it took to build the BVH for the last mesh.
			Time bvhTime;
			
			
			/// The time in seconds that it took to find the diffraction edges for the last mesh.
			Time edgeTime;
			
			
			/// The time in seconds that it took to determine the edge-edge visibility for the last mesh.
			Time edgeVisibilityTime;
			
			
		//********************************************************************************
		//******	Memory Usage Statistics
			
			
			/// The approximate total amount of memory in bytes used for the sound scene.
			/**
			  * This value includes objects, sources, listeners, meshes, and materials.
			  */
			Size sceneMemory;
			
			
			/// The approximate total amount of memory in bytes used for the propagation system.
			/**
			  * This value includes any caches or data structures that accelerate sound propagation.
			  */
			Size propagationMemory;
			
			
			/// The approximate total amount of memory in bytes used for IRs.
			Size irMemory;
			
			
			/// The approximate total amount of memory in bytes used for the rendering system.
			Size renderingMemory;
			
			
			/// The approximate total amount of memory in bytes used by the whole system.
			Size totalMemory;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_STATISTICS_H
