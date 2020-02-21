/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMeshPreprocessor.h
 * Contents:    gsound::SoundMeshPreprocessor class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MESH_PREPROCESSOR_H
#define INCLUDE_GSOUND_SOUND_MESH_PREPROCESSOR_H


#include "gsConfig.h"


#include "gsMeshRequest.h"
#include "gsSoundMesh.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that does preprocessing on a triangle mesh before it is used for sound propagation.
/**
  * This can include mesh simplication and diffraction edge analysis. The class
  * outputs a simplified mesh with adjaceny and other information used
  * in sound propagation. A MeshRequest object is used to determine how
  * the mesh is processed.
  */
class SoundMeshPreprocessor
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a mesh preprocessor.
			SoundMeshPreprocessor();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound mesh preprocessor, releasing all internal data.
			~SoundMeshPreprocessor();
			
			
		//********************************************************************************
		//******	Mesh Processing Method
			
			
			/// Preprocess the specified mesh using the requested parameters.
			/**
			  * The method processes the input mesh, placing the result in the
			  * output mesh parameter. The previous contents of the mesh parameter
			  * are replaced with the preprocessed geometry.
			  */
			Bool processMesh( const SoundVertex* vertices, Size numVertices,
								const SoundTriangle* triangles, Size numTriangles,
								const SoundMaterial* materials, Size numMaterials,
								const MeshRequest& request, SoundMesh& mesh );
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Classes
			
			
			/// A class that represents a node in a voxel tree.
			class Voxel;
			
			
			/// A class that represents a hiearchical tree of voxels.
			class VoxelTree;
			
			
			/// A class used to store a bounding box and a parent voxel in a stack of parents.
			class VoxelParent;
			
			
			/// A class used to store the voxel values and positions for a set of 8 neighboring voxels.
			class VoxelOctet;
			
			
			/// A class used to store temporary information about a vertex that is being welded.
			class WeldingVertex;
			
			
			/// A class that stores extra adjacency information for a mesh vertex.
			class FatVertex;
			
			
			/// A class that stores extra adjacency information for a mesh triangle.
			class FatTriangle;
			
			
			/// A class that stores information about a single mesh edge collapse operation.
			class EdgeCollapse;
			
			
			/// A class that is used for efficiently sorting edge collapses.
			class EdgeCollapseReference;
			
			
			/// A class that efficiently manages a dynamic min-heap of edge collapses.
			class EdgeCollapseQueue;
			
			
			/// A class that stores quadric-error-metric information for a vertex.
			class QEMVertex;
			
			
			/// A class that stores temporary information about a merged diffraction edge.
			class TempDiffractionEdge;
			
			
			/// A class that stores thread-local mesh preprocessing data.
			class ThreadData
			{
				public:
					
					/// A list of temporary edge neighbors.
					ArrayList<UInt32> edgeNeighbors;
					
			};
			
			
			
		
		//********************************************************************************
		//******	Private Voxelization Methods
			
			
			static void remeshRegion( const ArrayList<SoundVertex>& vertices,
									const ArrayList<SoundTriangle>& triangles,
									AABB3f region, AABB3f triangulationRegion, Size maxDepth, const MeshRequest& request,
									ArrayList<FatVertex>* outputVertices,
									ArrayList<FatTriangle>* outputTriangles, Mutex* outputMutex );
			
			
			static Bool voxelizeMesh( const ArrayList<SoundVertex>& vertices,
										const ArrayList<SoundTriangle>& triangles,
										const AABB3f& region, Size maxDepth, VoxelTree& result );
			
			
			static Voxel* voxelizeMeshRecursive( const ArrayList<SoundVertex>& vertices,
												const ArrayList<SoundTriangle>& triangles,
												ArrayList<ArrayList<Index> >& triangleIndices,
												const Vector3f& center, Real voxelSize, Index depth, Size maxDepth );
			
			
			GSOUND_FORCE_INLINE static Vector3f getClosestPointOnTriangle( const Vector3f& v1, const Vector3f& v2,
																		const Vector3f& v3, const Vector3f& p );
			
			
			GSOUND_FORCE_INLINE static Real getTriangleDistance( const Vector3f& v1, const Vector3f& v2,
																const Vector3f& v3, const Vector3f& p );
			
			
			
		
		//********************************************************************************
		//******	Private Marching Cubes Methods
			
			
			static Bool triangulateVoxelTree( const VoxelTree& voxelTree, Size depth, const AABB3f& region, Real threshold,
												ArrayList<SoundVertex>& outputVertices,
												ArrayList<SoundTriangle>& outputTriangles );
			
			
			static void triangulateVoxelRecursive( const Voxel* voxel, const Vector3f& center, Real voxelSize,
													Real threshold, Index depth, Size maxDepth, const AABB3f& region,
													ArrayList<VoxelParent>& parentStack, 
													ArrayList<SoundVertex>& outputVertices,
													ArrayList<SoundTriangle>& outputTriangles );
			
			
			/// Triangulate the specified octect of voxels.
			/**
			  * Given a grid cell and an isolevel, calculate the triangular
			  * facets required to represent the isosurface through the cell.
			  * Return the number of triangular facets, the array "triangles"
			  * will be loaded up with the vertices at most 5 triangular facets.
			  * 0 will be returned if the grid cell is either totally above
			  * or totally below the isolevel.
			  */
			static void triangulateOctet( const VoxelOctet& octect, Real threshold,
										ArrayList<SoundVertex>& outputVertices,
										ArrayList<SoundTriangle>& outputTriangles );
			
			
			// Linearly interpolate the position where an isosurface cuts an edge between two vertices.
			static Vector3f interpolateVoxels( const Vector3f& p1, const Vector3f& p2,
												Real value1, Real value2, Real threshold,
												Index materialIndex1, Index materialIndex2, Index& materialIndex );
			
			
			/// Return the value of the voxel that neighbors this one with the given center position.
			/**
			  * This method looks backwards up the parent voxel stack until it finds a parent
			  * that contains the neighbor position, then recurses down the tree until it finds
			  * the voxel (if it exists) and returns the voxel's fill value. A value of 0 means
			  * that the neighboring voxel is empty.
			  */
			static Real getNeighborVoxelFill( const ArrayList<VoxelParent>& parentStack,
											const Vector3f& center, Index& materialIndex );
			
			
			static Real getVoxelFill( const Voxel* parent, const Vector3f& center, Real voxelSize,
									const Vector3f& query, Index depth, Size queryDepth, Index& materialIndex );
			
			
			
			/// Pick the material index which is best represented in the specified material indices.
			static Index getBestMaterial( Index m0, Index m1, Index m2 )
			{
				if ( m0 == m1 || m0 == m2 )
					return m0;
				else if ( m1 == m0 || m1 == m2 )
					return m1;
				else
					return m2;
			}
			
			
			
		
		//********************************************************************************
		//******	Private Vertex Welding Methods
			
			
			static Bool weldVertices( const ArrayList<SoundVertex>& inputVertices,
									const ArrayList<SoundTriangle>& inputTriangles, Real tolerance,
									ArrayList<SoundVertex>& outputVertices, ArrayList<SoundTriangle>& outputTriangles );
			
			
			
			GSOUND_FORCE_INLINE static Bool weldVertex( const Vector3f& vertex, const ArrayList<Vector3f>& vertices,
												Array<ShortArrayList<Index,8> >& gridHashTable,
												const math::AABB3i& gridNeighborhood, Real tolerance, Index& weldedIndex );
			
			
			/// Return a hash code for a grid cell in a 3D unbounded spatial hash grid.
			GSOUND_FORCE_INLINE static Hash getGridCellHash( Int x, Int y, Int z )
			{
				return om::HashCode( *((Hash*)&x), *((Hash*)&y), *((Hash*)&z) );
			}
			
			
			/// Compute connectivity information for the specified mesh.
			static Bool fattenMesh( const ArrayList<SoundVertex>& inputVertices,
									const ArrayList<SoundTriangle>& inputTriangles,
									ArrayList<FatVertex>& outputVertices, ArrayList<FatTriangle>& outputTriangles );
			
			
			
		
		//********************************************************************************
		//******	Private Edge Collapse Methods
			
			
			static Bool collapseEdges( ArrayList<FatVertex>& vertices, ArrayList<FatTriangle>& triangles, Real maxCost );
			
			
			static Matrix4f computeQ( const FatVertex& vertex, const ArrayList<FatTriangle>& triangles );
			
			
			GSOUND_FORCE_INLINE static Real computeQError( const Matrix4f& Q, const Vector3f& v );
			
			
			GSOUND_FORCE_INLINE static Vector3f computeCollapseVertex( const Matrix4f& Q12, const Vector3f& v1, const Vector3f& v2 );
			
			
			static Bool vertexIsBorder( const FatVertex& vertex, const ArrayList<FatTriangle>& triangles );
			
			
			
		
		//********************************************************************************
		//******	Sound Mesh Construction Method
			
			
			/// Flatten any collapsed vertices and triangles.
			static Bool flattenMesh( ArrayList<FatVertex>& inputVertices, ArrayList<FatTriangle>& inputTriangles,
									ArrayList<FatVertex>& outputVertices, ArrayList<FatTriangle>& outputTriangles );
			
			
			/// Flatten any collapsed vertices and triangles.
			static Bool dietMesh( ArrayList<FatVertex>& inputVertices, ArrayList<FatTriangle>& inputTriangles,
									ArrayList<SoundVertex>& outputVertices, ArrayList<SoundTriangle>& outputTriangles );
			
			
			Bool buildSoundMesh( const ArrayList<FatVertex>& vertices, const ArrayList<FatTriangle>& triangles,
								const Shared<ArrayList<SoundMaterial> >& materials, const MeshRequest& request,
								SoundMesh& mesh );
			
			
			
		
		//********************************************************************************
		//******	Private Diffraction Preprocessing Methods
			
			
			Shared<internal::DiffractionGraph> buildEdgeGraph( const ArrayList<FatVertex>& fatVertices, const ArrayList<FatTriangle>& fatTriangles,
																const ArrayList<SoundVertex>& vertices, ArrayList<internal::InternalSoundTriangle>& triangles,
																const BVH& bvh, const MeshRequest& request );
			
			
			void testEdgeVisibility( ArrayList<internal::DiffractionEdge>& edges, Index startIndex, Size numEdges,
									const BVH& bvh, const MeshRequest& request, ThreadData& threadData );
			
			
			/// Return whether or not the specified diffraction edges can mutually diffract based on their orientation.
			GSOUND_FORCE_INLINE static Bool testEdgeOrientation( const internal::DiffractionEdge& edge1,
																const internal::DiffractionEdge& edge2 );
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Edge table for marching cubes.
			static const UInt16 edgeTable[256];
			
			
			/// Triangle table for marching cubes.
			static const Int8 triangleTable[256][16];
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pool of worker threads which the mesh preprocessor delegates tasks to.
			ThreadPool threadPool;
			
			
			/// A list of data structures for each thread containing thread-local data.
			ArrayList<ThreadData> threadDataList;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MESH_PREPROCESSOR_H
