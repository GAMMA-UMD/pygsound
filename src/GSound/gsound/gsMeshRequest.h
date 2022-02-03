/*
 * Project:     GSound
 * 
 * File:        gsound/gsMeshRequest.h
 * Contents:    gsound::MeshRequest class declaration
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


#ifndef INCLUDE_GSOUND_PREPROCESSOR_REQUEST_H
#define INCLUDE_GSOUND_PREPROCESSOR_REQUEST_H


#include "gsConfig.h"


#include "gsMeshFlags.h"
#include "gsSoundStatistics.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies the parameters for a mesh preprocessing operation.
class MeshRequest
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a mesh request object with the default initial settings.
			MeshRequest();
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// An object which contains boolean configuration information for this mesh request.
			MeshFlags flags;
			
			
			/// A transformation to apply to the mesh before simplificiation begins.
			Transform3f transform;
			
			
			/// The voxel size to use when remeshing the input mesh.
			/**
			  * The voxel size determines the 'resolution' of the resulting mesh.
			  * A larger voxel size will reduce the time to voxelize and remesh the
			  * mesh, but will also introduce more error in the output. A mesh
			  * is always inflated by 1/2 the voxel size if remeshing is enabled.
			  */
			Real voxelSize;
			
			
			/// The welding tolerance that the mesh preprocessor should use to weld vertices.
			/**
			  * If any two vertices in the mesh are closer to each other than this distance,
			  * they will be merged into one vertex and all triangles that share them
			  * will now reference the welded vertex.
			  */
			Real weldTolerance;
			
			
			/// The maximum allowed mesh simplification error.
			/**
			  * When the mesh's surface is being simplified, no simplification that deviates more than
			  * this distance from the original mesh will be allowed.
			  */
			Real simplifyTolerance;
			
			
			/// The diffraction angle threshold used by the mesh preprocessor to find diffraction edges.
			/**
			  * If the angle between normals of two neighboring triangles
			  * is less than this value, then the edge that they share is not diffracting.
			  * Thus, a larger diffraction angle will result in less diffracting edges while
			  * lower threshold will result in more edges.
			  */
			Real minDiffractionEdgeAngle;
			
			
			/// The minimum allowed length for a diffraction edge.
			/**
			  * All edges that are shorter than this length are not marked as diffraction edges.
			  */
			Real minDiffractionEdgeLength;
			
			
			/// The distance in meters between edge visibility samples along an edge when determining edge-edge visibility.
			Real edgeResolution;
			
			
			/// The minimum number of rays per edge that should be used to test an edge-edge visibility.
			Size minRaysPerEdge;
			
			
			/// The maximum number of rays per edge that should be used to test an edge-edge visibility.
			Size maxRaysPerEdge;
			
			
			/// The edge offset in meters that is used to bias away from diffraction edges when testing visibility.
			Real edgeOffset;
			
			
			/// The maximum allowed size for the diffuse subdivision patches for the mesh.
			Real diffuseResolution;
			
			
			/// The number of threads to use to compute a mesh preprocessing request.
			Size numThreads;
			
			
			/// A pointer to an optional object which recieve runtime information about the preprocessing system.
			/**
			  * If statistics are enabled and this pointer is not NULL, the preprocessing system sets
			  * data in the object that indicate information about the last mesh processed.
			  * This pointer may be NULL, indicating that analytic information is not required at all.
			  */
			SoundStatistics* statistics;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PREPROCESSOR_REQUEST_H
