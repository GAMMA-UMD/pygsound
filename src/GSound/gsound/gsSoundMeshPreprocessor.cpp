/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMeshPreprocessor.cpp
 * Contents:    gsound::SoundMeshPreprocessor class implementation
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


#include "gsSoundMeshPreprocessor.h"


#include <algorithm>


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


using namespace gsound::internal;


//##########################################################################################
//##########################################################################################
//############		
//############		Voxel Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: Voxel
{
	public:
		
		GSOUND_INLINE Voxel( Real newFill, Index newMaterialIndex )
			:	fill( newFill ),
				materialIndex( newMaterialIndex )
		{
			for ( Index i = 0; i < 8; i++ )
				children[i] = NULL;
		}
		
		GSOUND_INLINE ~Voxel()
		{
			for ( Index i = 0; i < 8; i++ )
			{
				if ( children[i] )
					util::destruct( children[i] );
			}
		}
		
		/// The measure of how much this voxel is occupied by geometry.
		Real fill;
		
		/// The index of the material to be used for this voxel.
		Index materialIndex;
		
		/// Pointers to the eight children of this voxel or NULL if the child is empty.
		Voxel* children[8];
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Voxel Tree Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: VoxelTree
{
	public:
		
		GSOUND_INLINE VoxelTree()
			:	root( NULL ),
				center(),
				size( 0 )
		{
		}
		
		
		GSOUND_INLINE ~VoxelTree()
		{
			if ( root )
				util::destruct( root );
		}
		
		GSOUND_INLINE void release()
		{
			if ( root )
				util::destruct( root );
			
			root = NULL;
		}
		
		
		/// A pointer to the root node of this voxel tree.
		Voxel* root;
		
		/// The center of the root voxel for this tree.
		Vector3f center;
		
		/// The width, height, and depth of the root voxel.
		Real size;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Voxel Parent Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: VoxelParent
{
	public:
		
		GSOUND_INLINE VoxelParent( const Voxel* newVoxel, const AABB3f& newBounds )
			:	voxel( newVoxel ),
				bounds( newBounds )
		{
		}
		
		/// A pointer to the voxel for this voxel parent.
		const Voxel* voxel;
		
		/// The bounding box for this voxel parent.
		AABB3f bounds;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Voxel Octect Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: VoxelOctet
{
	public:
		
		/// An array of the centers for a group of 8 neighboring voxels.
		Vector3f center[8];
		
		
		/// An array of the fill values for a group of 8 neighboring voxels.
		Real fill[8];
		
		
		/// An array of the material indices for a group of 8 neighboring voxels.
		Index materialIndex[8];
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Welding Vertex Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: WeldingVertex
{
	public:
		
		GSOUND_INLINE WeldingVertex()
			:	weldedIndex( math::max<Size>() ),
				finalIndex( 0 )
		{
		}
		
		/// Reset this welding vertex to its initial state.
		GSOUND_INLINE void reset()
		{
			weldedIndex = math::max<Size>();
			finalIndex = 0;
		}
		
		/// The index of the vertex that this vertex was welded to, or math::max<Size>() if not welded.
		Index weldedIndex;
		
		/// The final index of this vertex in the output list.
		Index finalIndex;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Fat Vertex Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: FatVertex
{
	public:
		
		GSOUND_INLINE FatVertex( const Vector3f& newPosition )
			:	position( newPosition ),
				finalIndex( 0 ),
				collapsed( false ),
				checked( false )
		{
		}
		
		Vector3f position;
		
		/// A list of the indices of the vertices that share an edge with vertex.
		ShortArrayList<Index,4> vertexNeighbors;
		
		/// A list of the indices of the triangles that use this vertex.
		ShortArrayList<Index,6> triangleNeighbors;
		
		/// The final index of this vertex in the output list.
		Index finalIndex;
		
		/// A boolean value indicating whether or not this vertex has been collapsed.
		Bool collapsed;
		
		/// A boolean value indicating whether or not this vertex has been collapsed.
		Bool checked;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Fat Triangle Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: FatTriangle
{
	public:
		
		/// Create a new fat triangle with the specified vertex indices, material, and plane.
		GSOUND_INLINE FatTriangle( Index newV0, Index newV1, Index newV2, Index newMaterialIndex, const Plane3f& newPlane )
			:	materialIndex( newMaterialIndex ),
				plane( newPlane ),
				finalIndex( 0 ),
				collapsed( false )
		{
			v[0] = newV0;
			v[1] = newV1;
			v[2] = newV2;
		}
		
		
		/// Return whether or not this triangle has a vertex with the specified index in the mesh.
		GSOUND_INLINE Bool hasVertex( const Index vertexIndex ) const
		{
			return v[0] == vertexIndex || v[1] == vertexIndex || v[2] == vertexIndex;
		}
		
		
		/// Get the index within the triangle of the vertex with the given index within the mesh.
		GSOUND_INLINE Bool getVertexIndex( const Index vertexIndex, Index& index ) const
		{
			for ( Index i = 0; i < 3; i++ )
			{
				if ( v[i] == vertexIndex )
				{
					index = i;
					return true;
				}
			}
			return false;
		}
		
		
		/// Replace the vertex with the specified index in the mesh with a new vertex index.
		GSOUND_INLINE Bool replaceVertex( Index replaceIndex, Index newIndex )
		{
			if ( v[0] == replaceIndex )
				v[0] = newIndex;
			else if ( v[1] == replaceIndex )
				v[1] = newIndex;
			else if ( v[2] == replaceIndex )
				v[2] = newIndex;
			else
				return false;
			
			return true;
		}
		
		
		/// The indices of the vertices of this triangle.
		Index v[3];
		
		/// The plane equation for this triangle.
		Plane3f plane;
		
		/// The index of this triangle's material.
		Index materialIndex;
		
		/// The final index of this triangle in the output list.
		Index finalIndex;
		
		/// A boolean value indicating whether or not this triangle has been collapsed.
		Bool collapsed;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Collapse Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: EdgeCollapse
{
	public:
		
		/// Create a new edge collapse operation with the specified starting vertex indices, target position, and collapse cost.
		GSOUND_INLINE EdgeCollapse( Index newV1, Index newV2, const Vector3f& newTarget, Real newCost )
			:	v1( newV1 ),
				v2( newV2 ),
				target( newTarget ),
				cost( newCost ),
				queueIndex( math::max<Index>() )
		{
		}
		
		
		/// Return whether or not this edge collapse is the same as another.
		GSOUND_INLINE Bool operator == ( const EdgeCollapse& other ) const
		{
			return (v1 == other.v1 && v2 == other.v2) || (v1 == other.v2 && v2 == other.v1);
		}
		
		
		/// The index of the first vertex of this edge collapse operation.
		Index v1;
		
		/// The index of the second vertex of this edge collapse operation.
		Index v2;
		
		/// The target position of this edge collapse operation.
		Vector3f target;
		
		/// The cost to perform this edge collapse.
		Real cost;
		
		/// The index position of this edge collapse in the edge collapse queue.
		Index queueIndex;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Collapse Reference Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: EdgeCollapseReference
{
	public:
		
		/// Create a new edge collapse reference for the specified edge collapse operation.
		GSOUND_INLINE EdgeCollapseReference( EdgeCollapse* newCollapse )
			:	collapse( newCollapse )
		{
		}
		
		
		/// Return whether or not this edge collapse reference is the same as another.
		GSOUND_INLINE Bool operator == ( const EdgeCollapseReference& other ) const
		{
			return collapse == other.collapse;
		}
		
		
		/// Return whether or not this edge collapse reference has a lower cost than another.
		GSOUND_INLINE Bool operator < ( const EdgeCollapseReference& other ) const
		{
			return collapse->cost > other.collapse->cost;
		}
		
		
		/// Return the cost for performing the edge collapse referenced by this object.
		GSOUND_INLINE Real getCost() const
		{
			return collapse->cost;
		}
		
		
		/// A pointer to the edge collapse this object is referencing.
		EdgeCollapse* collapse;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Collapse Queue Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: EdgeCollapseQueue
{
	public:
		
		/// Create a new edge collapse queue that has the specified initial capacity.
		GSOUND_INLINE EdgeCollapseQueue( Size newCapacity )
			:	array( util::allocate<EdgeCollapseReference>( newCapacity ) ),
				capacity( newCapacity ),
				numCollapses( 0 )
		{
		}
		
		
		/// Destroy an edge collapse queue and release all internal memory.
		GSOUND_INLINE ~EdgeCollapseQueue()
		{
			this->clear();
			util::deallocate( array );
		}
		
		
		/// Add a new edge collapse to the queue, resorting the queue to maintain collapse ordering.
		GSOUND_INLINE void add( const EdgeCollapseReference& newCollapse )
		{
			if ( numCollapses == capacity )
				doubleCapacity();
			
			Index i = numCollapses;
			Index parent = getParentIndex(i);
			
			new (array + numCollapses) EdgeCollapseReference( newCollapse );
			array[i].collapse->queueIndex = i;
			numCollapses++;
			
			// reorder the queue's heap so that the new element is in the correct place.
			while ( array[parent] < array[i] )
			{
				swap( parent, i );
				
				i = parent;
				parent = getParentIndex(i);
			}
		}
		
		
		/// Remove the first collapse from the queue, resorting the queue to maintain collapse ordering.
		GSOUND_INLINE void remove()
		{
			// Decrement the number of elements in the queue.
			numCollapses--;
			
			// Copy the last element in the queue's array into the largest element's location.
			array[0] = array[numCollapses];
			array[0].collapse->queueIndex = 0;
			
			// Call the destructor for the old last element.
			array[numCollapses].~EdgeCollapseReference();
			
			// Convert the new array into a heap, starting at the first element.
			this->heapify( 0 );
		}
		
		
		/// Ensure that the heap is propery ordered after the specified element was changed.
		GSOUND_INLINE void update( Index i )
		{
			if ( i > 0 )
			{
				Index parent = getParentIndex(i);
				
				// Did the entry increase its value?
				if ( array[parent] < array[i] )
				{
					do
					{
						swap( parent, i );
						
						i = parent;
						parent = getParentIndex(i);
					}
					while ( i > 0 && array[parent] < array[i] );
					
					return;
				}
			}
			
			this->heapify( i );
		}
		
		
		/// Return a reference to the first edge collapse in the queue, the one with the lowest cost.
		GSOUND_INLINE const EdgeCollapseReference& getFirst() const
		{
			return *array;
		}
		
		
		/// Return the total number of edge collapses that are stored in this queue.
		GSOUND_INLINE Size getSize() const
		{
			return numCollapses;
		}
		
		
		/// Clear all edge collapses from this queue.
		GSOUND_INLINE void clear()
		{
			if ( array != NULL )
				callDestructors( array, numCollapses );
			
			numCollapses = Size(0);
		}
		
		
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Return the index of a child elements's parent element given the child element's index.
			/**
			  * If the child index is zero, the method returns 0 because this child element is
			  * at the top of the heap and has no parent by definition.
			  */
			GSOUND_INLINE static Index getParentIndex( Index child )
			{
				if ( child == Index(0) )
					return Index(0);
				
				return (child - Index(1))/Index(2);
			}
			
			
			/// Return the index of the left child element given the parent element's index.
			GSOUND_INLINE static Index getChildIndex1( Index parent )
			{
				return (parent << 1) + Index(1);
			}
			
			
			/// Return the index of the right child element given the parent element's index.
			GSOUND_INLINE static Index getChildIndex2( Index parent )
			{
				return (parent << 1) + Index(2);
			}
			
			
			/// Convert the specified sub-heap, with root at index i, into a heap.
			GSOUND_INLINE void heapify( Index i )
			{
				while ( i < numCollapses )
				{
					Index childIndex1 = getChildIndex1(i);
					Index childIndex2 = getChildIndex2(i);
					Index max;
					
					if ( childIndex1 < numCollapses && array[i] < array[childIndex1] )
						max = childIndex1;
					else
						max = i;
					
					if ( childIndex2 < numCollapses && array[max] < array[childIndex2] )
						max = childIndex2;
					
					if ( max == i )
						break;
					
					swap( max, i );
					i = max;
				}
			}
			
			
			// Swap two elements in the heap.
			GSOUND_FORCE_INLINE void swap( Index index1, Index index2 )
			{
				EdgeCollapseReference temp = array[index1];
				array[index1] = array[index2];
				array[index2] = temp;
				
				// Update the indices for the swapped elements.
				array[index1].collapse->queueIndex = index1;
				array[index2].collapse->queueIndex = index2;
			}
			
			
			/// Double the capacity of the internal array.
			GSOUND_INLINE void doubleCapacity()
			{
				// check to see if the array has not yet been allocated.
				if ( capacity == 0 )
				{
					// allocate the array to hold elements.
					capacity = 8;
					array = util::allocate<EdgeCollapseReference>( capacity );
				}
				else
					resize( capacity << 1 );
			}
			
			
			/// Double the size of the element array to increase the capacity of the priority queue.
			/**
			  * This method deallocates the previously used array, and then allocates
			  * a new block of memory with a size equal to double the previous size.
			  * It then copies over all of the previous elements into the new array.
			  */
			void resize( Size newCapacity )
			{
				// Update the capacity and allocate a new array.
				capacity = newCapacity;
				EdgeCollapseReference* oldArray = array;
				array = util::allocate<EdgeCollapseReference>( capacity );
				
				// copy the elements from the old array to the new array.
				moveObjects( array, oldArray, numCollapses );
				
				// deallocate the old array.
				util::deallocate( oldArray );
			}
			
			
			GSOUND_INLINE static void moveObjects( EdgeCollapseReference* destination,
													const EdgeCollapseReference* source, Size number )
			{
				const EdgeCollapseReference* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					// copy the object from the source to destination
					new (destination) EdgeCollapseReference(*source);
					
					// call the destructors on the source
					source->~EdgeCollapseReference();
					
					destination++;
					source++;
				}
			}
			
			
			GSOUND_INLINE static void callDestructors( EdgeCollapseReference* array, Size number )
			{
				const EdgeCollapseReference* const arrayEnd = array + number;
				
				while ( array != arrayEnd )
				{
					array->~EdgeCollapseReference();
					array++;
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the storage for this queue's array-based heap.
			EdgeCollapseReference* array;
			
			
			/// The capacity of this queue's internal storage array.
			Size capacity;
			
			
			/// The number of edge collapses that are stored in this queue.
			Size numCollapses;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Collapse Queue Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: QEMVertex
{
	public:
		
		/// Create a new QEM vertex with the specified error metric matrix.
		GSOUND_INLINE QEMVertex( const Matrix4f& newQ )
			:	Q( newQ )
		{
		}
		
		
		/// The quadric error metric matrix Q for this vertex.
		Matrix4f Q;
		
		
		/// A list of the edge collapses that include this vertex.
		ShortArrayList<EdgeCollapseReference,4> collapses;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Temp Diffraction Edge Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMeshPreprocessor:: TempDiffractionEdge
{
	public:
		
		GSOUND_INLINE TempDiffractionEdge( const internal::InternalSoundTriangle* newTriangle1,
											Index newEdgeIndex1,
											const internal::InternalSoundTriangle* newTriangle2,
											Index newEdgeIndex2 )
			:	triangle1( newTriangle1 ),
				triangle2( newTriangle2 ),
				edgeIndex1( newEdgeIndex1 ),
				edgeIndex2( newEdgeIndex2 ),
				plane1( newTriangle1->getPlane() ),
				plane2( newTriangle2->getPlane() ),
				merged( false ),
				mergedIndex( 0 )
		{
			v[0] = v[1] = 0;
			
			// Make sure that the planes point towards the outside of the diffraction edge.
			const SoundVertex* freeVertex1 = triangle1->getEdgeFreeVertex( edgeIndex1 );
			const SoundVertex* freeVertex2 = triangle2->getEdgeFreeVertex( edgeIndex2 );
			
			if ( plane1.getSignedDistanceTo( *freeVertex2 ) > Real(0) )
				plane1 = -plane1;
			
			if ( plane2.getSignedDistanceTo( *freeVertex1 ) > Real(0) )
				plane2 = -plane2;
		}
		
		const internal::InternalSoundTriangle* triangle1;
		const internal::InternalSoundTriangle* triangle2;
		Index edgeIndex1;
		Index edgeIndex2;
		Plane3f plane1;
		Plane3f plane2;
		Index v[2];
		Bool merged;
		Index mergedIndex;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundMeshPreprocessor:: SoundMeshPreprocessor()
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




SoundMeshPreprocessor:: ~SoundMeshPreprocessor()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mesh Preprocessing Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: processMesh( const SoundVertex* vertices, const Size numVertices,
											const SoundTriangle* triangles, const Size numTriangles,
											const SoundMaterial* materials, const Size numMaterials,
											const MeshRequest& request, SoundMesh& mesh )
{
	if ( vertices == NULL || numVertices < Size(3) || triangles == NULL || numTriangles < Size(1) )
		return false;
	
	//***********************************************************************
	// Initialize the thread pool.
	
	const Size numThreads = request.numThreads;
	
	// Add the necessary number of threads to the thread pool.
	if ( threadPool.getThreadCount() != numThreads )
		threadPool.setThreadCount( numThreads );
	
	for ( Index i = threadDataList.getSize(); i < numThreads; i++ )
		threadDataList.add( ThreadData() );
	
	//***********************************************************************
	// Initialize timing.
	
	// Create a timer that is used to time the processing stages.
	Timer timer;
	Timer totalTimer;
	
	// Reset all times.
	if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
	{
		SoundStatistics& statistics = *request.statistics;
		statistics.preprocessTime = statistics.remeshTime = statistics.weldTime = 0;
		statistics.simplifyTime = statistics.bvhTime = statistics.edgeTime = statistics.edgeVisibilityTime = 0;
	}
	
	//***********************************************************************
	// Sanitize the mesh input.
	
	ArrayList<SoundVertex> sanitizedVertices( numVertices );
	ArrayList<SoundTriangle> sanitizedTriangles( numTriangles );
	
	// Transform the vertices.
	for ( Index i = 0; i < numVertices; i++ )
	{
		SoundVertex vertex = vertices[i];
		
		// Zero-out invalid vertex components that might break the mesh.
		if ( math::isNAN( vertex.x ) || math::isInfinite( vertex.x ) || math::abs(vertex.x) < math::minPositive<Real>() )
			vertex.x = Real(0);
		
		if ( math::isNAN( vertex.y ) || math::isInfinite( vertex.y ) || math::abs(vertex.y) < math::minPositive<Real>() )
			vertex.y = Real(0);
		
		if ( math::isNAN( vertex.z ) || math::isInfinite( vertex.z ) || math::abs(vertex.z) < math::minPositive<Real>() )
			vertex.z = Real(0);
		
		sanitizedVertices.add( request.transform.transformToWorld( vertex ) );
	}
	
	const Size numSanitizedVertices = sanitizedVertices.getSize();
	
	// Sanitize the triangle indices.
	for ( Index i = 0; i < numTriangles; i++ )
	{
		const SoundTriangle& t = triangles[i];
		
		// Skip triangles with invalid vertex indices.
		if ( t.v[0] >= numSanitizedVertices || t.v[1] >= numSanitizedVertices || t.v[2] >= numSanitizedVertices )
			continue;
		
		sanitizedTriangles.add( SoundTriangle( t.v[0], t.v[1], t.v[2],
												t.materialIndex >= numMaterials ? 0 : t.materialIndex ) );
	}
	
	// Make sure there are enough vertices and triangles.
	if ( sanitizedVertices.getSize() < Size(3) || sanitizedTriangles.getSize() < Size(1) )
		return false;
	
	// Copy the materials.
	Shared< ArrayList<SoundMaterial> > sanitizedMaterials = Shared<ArrayList<SoundMaterial> >::construct();
	
	// If there are no materials provided, use a default.
	if ( materials != NULL && numMaterials > 0 )
		sanitizedMaterials->addAll( materials, numMaterials );
	else
		sanitizedMaterials->add( SoundMaterial() );
	
	//***********************************************************************
	// Process the mesh.
	
	ArrayList<FatVertex> flatVertices;
	ArrayList<FatTriangle> flatTriangles;
	
	const Bool voxelize = request.flags.isSet( MeshFlags::VOXELIZE );
	const Bool simplify = request.flags.isSet( MeshFlags::SIMPLIFY );
	const Bool weld = request.flags.isSet( MeshFlags::WELD );
	
	if ( voxelize && request.voxelSize > Real(0) )
	{
		const Real voxelResolution = request.voxelSize;
		
		//***********************************************************************
		// Determine the size of the mesh.
		
		AABB3f bounds( sanitizedVertices[0], sanitizedVertices[0] );
		const Size numSanitizedVertices = sanitizedVertices.getSize();
		
		for ( Index v = 1; v < numSanitizedVertices; v++ )
			bounds.enlargeFor( sanitizedVertices[v] );
		
		//***********************************************************************
		// Determine the number of regions to voxelize.
		
		Size maxGridDepth = 7;
		Size maxGridSize = 1 << maxGridDepth;
		const Size regionOverlap = 1;
		Real overlapSize = regionOverlap*voxelResolution;
		
		Vector3f fittedGridSize = bounds.getSize() / voxelResolution + 2*regionOverlap;
		
		// Determine the subdivision depth necessary to meet the required resolution.
		Size3D maxDepth( Size(math::log2( fittedGridSize.x )) + 1,
								Size(math::log2( fittedGridSize.y )) + 1,
								Size(math::log2( fittedGridSize.z )) + 1 );
		
		Size3D gridSize( 1 << maxDepth.x, 1 << maxDepth.y, 1 << maxDepth.z );
		Size3D numRegions = math::ceiling( Vector3f(gridSize) / Vector3f((Real)maxGridSize) );
		
		maxGridDepth = math::min( maxGridDepth, math::max( maxDepth.x, math::max( maxDepth.y, maxDepth.z ) ) );
		maxGridSize = Size(1) << maxGridDepth;
		
		const Real regionSize = maxGridSize*voxelResolution;
		const Real regionStride = (maxGridSize - 1*regionOverlap)*voxelResolution;
		
		Vector3f gridMin = bounds.getCenter() - Real(0.5)*Vector3f(numRegions*maxGridSize - 2*regionOverlap)*voxelResolution;
		
		//***********************************************************************
		// Spawn jobs for each of the regions that need to be remeshed.
		
		Mutex outputMutex;
		
		for ( Index x = 0; x < numRegions.x; x++ )
		{
			Vector3f rowMin = gridMin;
			
			for ( Index y = 0; y < numRegions.y; y++ )
			{
				Vector3f columnMin = rowMin;
				
				for ( Index z = 0; z < numRegions.z; z++ )
				{
					AABB3f region( columnMin, columnMin + regionSize );
					AABB3f triangulationRegion( region.min, region.max - overlapSize );
					
					threadPool.addJob( FunctionCall< void ( const ArrayList<SoundVertex>&, const ArrayList<SoundTriangle>&,
															AABB3f, AABB3f, Size, const MeshRequest&,
															ArrayList<FatVertex>*, ArrayList<FatTriangle>*, Mutex* )>(
												bind( SoundMeshPreprocessor::remeshRegion ),
												sanitizedVertices, sanitizedTriangles, region, triangulationRegion, maxGridDepth, request,
												&flatVertices, &flatTriangles, &outputMutex ) );
					
					columnMin.z += regionStride;
				}
				
				rowMin.y += regionStride;
			}
			
			gridMin.x += regionStride;
		}
		
		threadPool.finishJobs();
		
		// Update the time that it took to remesh the mesh.
		timer.update();
		
		if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
			request.statistics->remeshTime += timer.getLastInterval();
		
		//***********************************************************************
		// Post Process the mesh to get rid of seams.
		
		ArrayList<SoundVertex> slimVertices;
		ArrayList<SoundTriangle> slimTriangles;
		
		dietMesh( flatVertices, flatTriangles, slimVertices, slimTriangles );
		
		// Update the timer.
		timer.update();
		
		//***********************************************************************
		// Weld vertices.
		
		ArrayList<SoundVertex> weldedVertices;
		ArrayList<SoundTriangle> weldedTriangles;
		
		if ( !weldVertices( slimVertices, slimTriangles, request.weldTolerance, weldedVertices, weldedTriangles ) )
			return false;
		
		ArrayList<FatVertex> fatVertices;
		ArrayList<FatTriangle> fatTriangles;
		
		if ( !fattenMesh( weldedVertices, weldedTriangles, fatVertices, fatTriangles ) )
			return false;
		
		// Update the timer and total vertex welding time.
		timer.update();
		
		if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
			request.statistics->weldTime += timer.getLastInterval();
		
		//***********************************************************************
		// Simplify the mesh.
		
		if ( simplify )
		{
			if ( !collapseEdges( fatVertices, fatTriangles, request.simplifyTolerance ) )
				return false;
			
			if ( !collapseEdges( fatVertices, fatTriangles, request.simplifyTolerance ) )
				return false;
		}
		
		flatVertices.clear();
		flatTriangles.clear();
		
		if ( !flattenMesh( fatVertices, fatTriangles, flatVertices, flatTriangles ) )
			return false;
		
		// Update the timer and total simplification time.
		timer.update();
		
		if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
			request.statistics->simplifyTime += timer.getLastInterval();
		
		//***********************************************************************
		// Build the final sound mesh.
		
		Bool result = buildSoundMesh( flatVertices, flatTriangles, sanitizedMaterials, request, mesh );
		
		// Output the total time for preprocessing.
		totalTimer.update();
		
		if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
			request.statistics->preprocessTime += totalTimer.getLastInterval();
		
		return result;
	}
	else
	{
		//***********************************************************************
		// Convert the mesh into the proper format.
		
		ArrayList<FatVertex> fatVertices;
		ArrayList<FatTriangle> fatTriangles;
		
		timer.update();
		
		//***********************************************************************
		// Weld vertices.
		
		if ( weld )
		{
			ArrayList<SoundVertex> weldedVertices;
			ArrayList<SoundTriangle> weldedTriangles;
			
			if ( !weldVertices( sanitizedVertices, sanitizedTriangles, request.weldTolerance, weldedVertices, weldedTriangles ) )
				return false;
			
			if ( !fattenMesh( weldedVertices, weldedTriangles, fatVertices, fatTriangles ) )
				return false;
			
			// Update the timer and total vertex welding time.
			timer.update();
			
			if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
				request.statistics->weldTime += timer.getLastInterval();
			
			// Simplify the mesh if necessary.
			if ( simplify )
			{
				if ( !collapseEdges( fatVertices, fatTriangles, request.simplifyTolerance ) )
					return false;
			}
		}
		else
		{
			if ( !fattenMesh( sanitizedVertices, sanitizedTriangles, fatVertices, fatTriangles ) )
				return false;
			
			// Simplify the mesh if necessary.
			if ( simplify )
			{
				if ( !collapseEdges( fatVertices, fatTriangles, request.simplifyTolerance ) )
					return false;
			}
		}
		
		if ( !flattenMesh( fatVertices, fatTriangles, flatVertices, flatTriangles ) )
			return false;
		
		// Update the timer and total simplification time.
		timer.update();
		
		if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
			request.statistics->simplifyTime += timer.getLastInterval();
		
		//***********************************************************************
		// Build the final sound mesh.
		
		Bool result = buildSoundMesh( flatVertices, flatTriangles, sanitizedMaterials, request, mesh );
		
		// Output the total time for preprocessing.
		totalTimer.update();
		
		if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
			request.statistics->preprocessTime += totalTimer.getLastInterval();
		
		return result;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mesh Remeshing Method
//############		
//##########################################################################################
//##########################################################################################




void SoundMeshPreprocessor:: remeshRegion( const ArrayList<SoundVertex>& vertices,
										const ArrayList<SoundTriangle>& triangles,
										AABB3f region, AABB3f triangulationRegion, Size maxDepth, const MeshRequest& request,
										ArrayList<FatVertex>* outputVertices,
										ArrayList<FatTriangle>* outputTriangles, Mutex* outputMutex )
{
	VoxelTree voxelTree;
	
	if ( !voxelizeMesh( vertices, triangles, region, maxDepth, voxelTree ) )
		return;
	
	ArrayList<SoundVertex> triangulationVertices;
	ArrayList<SoundTriangle> triangulation;
	
	triangulateVoxelTree( voxelTree, maxDepth, triangulationRegion, 0.5, triangulationVertices, triangulation );
	
	if ( triangulation.getSize() == 0 )
		return;
	
	// Clean up the voxel memory.
	voxelTree.release();
	ArrayList<FatVertex> fatVertices;
	ArrayList<FatTriangle> fatTriangles;
	
	const Bool simplify = request.flags.isSet( MeshFlags::SIMPLIFY );
	const Bool weld = request.flags.isSet( MeshFlags::WELD );
	
	if ( weld )
	{
		ArrayList<SoundVertex> weldedVertices;
		ArrayList<SoundTriangle> weldedTriangles;
		
		if ( !weldVertices( triangulationVertices, triangulation, request.weldTolerance, weldedVertices, weldedTriangles ) )
			return;
		
		// Clean up the triangulation memory.
		triangulationVertices.reset();
		triangulation.reset();
		
		if ( !fattenMesh( weldedVertices, weldedTriangles, fatVertices, fatTriangles ) )
			return;
		
		// Clean up the welded vertices memory.
		weldedVertices.reset();
		weldedTriangles.reset();
		
		if ( simplify )
		{
			if ( !collapseEdges( fatVertices, fatTriangles, request.simplifyTolerance ) )
				return;
		}
	}
	else
	{
		if ( !fattenMesh( triangulationVertices, triangulation, fatVertices, fatTriangles ) )
			return;
		
		// Clean up the triangulation memory.
		triangulationVertices.reset();
		triangulation.reset();
		
		if ( simplify )
		{
			if ( !collapseEdges( fatVertices, fatTriangles, request.simplifyTolerance ) )
				return;
		}
	}
	
	// Lock the output mutex while we are adding geometry to the output lists.
	outputMutex->lock();
	
	flattenMesh( fatVertices, fatTriangles, *outputVertices, *outputTriangles );
	
	outputMutex->unlock();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mesh Voxelization Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: voxelizeMesh( const ArrayList<SoundVertex>& vertices,
											const ArrayList<SoundTriangle>& triangles,
											const AABB3f& region, Size maxDepth, VoxelTree& result )
{
	// Fail if there is no geometry.
	if ( vertices.getSize() < Size(3) || triangles.getSize() < Size(1) )
		return false;
	
	//***********************************************************************
	
	// Compute the center of the bounding cube.
	result.center = region.getCenter();
	
	// Compute the size of the bounding cube.
	result.size = math::max( region.getWidth(), math::max( region.getHeight(), region.getDepth() ) );
	
	// Increase the size by 1 voxel in each direction so that the geometry isn't
	// adjacent to the voxelization boundary.
	//result.size += 1*resolution;
	//result.center += 1*resolution;
	
	//***********************************************************************
	// Prepare the list of triangle indices.
	
	ArrayList< ArrayList<Index> > triangleIndices( maxDepth );
	
	const Size numTriangles = triangles.getSize();
	const Real maxChildDistance = Real(1)*math::sqrt(Real(3.0))*result.size;
	
	triangleIndices.add( ArrayList<Index>( numTriangles ) );
	
	for ( Index t = 0; t < numTriangles; t++ )
	{
		const SoundTriangle& triangle = triangles[t];
		const Real distance = getTriangleDistance( vertices[triangle.v[0]], vertices[triangle.v[1]],
													vertices[triangle.v[2]], result.center );
		
		if ( distance < maxChildDistance )
			triangleIndices[0].add( t );
	}
	
	for ( Index i = 1; i <= maxDepth; i++ )
		triangleIndices.add( ArrayList<Index>() );
	
	//***********************************************************************
	// Recursively refine the voxelization
	
	// Voxelize the mesh recursively
	result.root = voxelizeMeshRecursive( vertices, triangles, triangleIndices,
										result.center, result.size, 0, maxDepth );
	
	if ( result.root == NULL )
		return false;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Recursive Mesh Voxelization Method
//############		
//##########################################################################################
//##########################################################################################




SoundMeshPreprocessor::Voxel* SoundMeshPreprocessor:: voxelizeMeshRecursive( const ArrayList<SoundVertex>& vertices,
													const ArrayList<SoundTriangle>& triangles,
													ArrayList<ArrayList<Index> >& triangleIndices,
													const Vector3f& center, Real voxelSize, Index depth, Size maxDepth )
{
	//***********************************************************************
	// Find all triangles that intersect this voxel.
	
	const ArrayList<Index>& currentTriangles = triangleIndices[depth];
	ArrayList<Index>* childTriangles = NULL;
	
	if ( depth < maxDepth )
	{
		childTriangles = &triangleIndices[depth + 1];
		childTriangles->clear();
	}
	
	// Compute the maximum distance that a triangle can be from this voxel. (half voxel diagonal)
	const Real maxChildDistance = Real(1)*math::sqrt(Real(3.0))*voxelSize;
	const Real maxDistance = voxelSize*math::sqrt(Real(3.0));
	Real filled = 0;
	const Size numTriangles = currentTriangles.getSize();
	Index materialIndex = 0;
	
	for ( Index i = 0; i < numTriangles; i++ )
	{
		Index t = currentTriangles[i];
		const SoundTriangle& triangle = triangles[t];
		
		// Get the distance to this voxel's center.
		Real distance = getTriangleDistance( vertices[triangle.v[0]], vertices[triangle.v[1]],
											vertices[triangle.v[2]], center );
		
		if ( distance < maxChildDistance )
		{
			if ( distance < maxDistance )
			{
				Real contribution = Real(1) - (distance / maxDistance);
				
				if ( contribution > filled )
				{
					filled = contribution;
					materialIndex = triangle.materialIndex;
				}
			}
			
			// Send the triangle to the child voxels.
			if ( childTriangles )
				childTriangles->add( t );
		}
	}
	
	//***********************************************************************
	
	// If no triangles overlapped this voxel, return that it is empty.
	if ( filled == Real(0) )
		return util::construct<Voxel>( Real(0), materialIndex );
	
	// Create this voxel.
	Voxel* voxel = util::construct<Voxel>( filled, materialIndex );
	
	//***********************************************************************
	// Recurse to the child voxels if the maximum depth has not been reached.
	
	if ( depth < maxDepth )
	{
		Real childVoxelSize = voxelSize*Real(0.5);
		Real quarterVoxelSize = childVoxelSize*Real(0.5);
		Size nextDepth = depth + 1;
		
		Vector3f minMax[2] = { center - quarterVoxelSize, center + quarterVoxelSize };
		Index childIndex = 0;
		
		for ( Index x = 0; x <= 1; x++ )
		{
			for ( Index y = 0; y <= 1; y++ )
			{
				for ( Index z = 0; z <= 1; z++, childIndex++ )
				{
					Vector3f childCenter( minMax[x].x, minMax[y].y, minMax[z].z );
					
					voxel->children[childIndex] = voxelizeMeshRecursive( vertices, triangles, triangleIndices,
																		childCenter, childVoxelSize, nextDepth, maxDepth );
				}
			}
		}
	}
	
	return voxel;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle Distance Method
//############		
//##########################################################################################
//##########################################################################################




Vector3f SoundMeshPreprocessor:: getClosestPointOnTriangle( const Vector3f& a, const Vector3f& b,
															const Vector3f& c, const Vector3f& p )
{
	// Check if P in vertex region outside A
    Vector3f ab = b - a;
    Vector3f ac = c - a;
    Vector3f ap = p - a;
    float d1 = math::dot(ab, ap);
    float d2 = math::dot(ac, ap);
	
	// barycentric coordinates (1,0,0)
    if ( d1 <= 0.0f && d2 <= 0.0f )
		return a;

    // Check if P in vertex region outside B
    Vector3f bp = p - b;
    float d3 = math::dot(ab, bp);
    float d4 = math::dot(ac, bp);
	
	// barycentric coordinates (0,1,0)
    if (d3 >= 0.0f && d4 <= d3)
		return b;

    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1*d4 - d3*d2;
    if ( vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f )
	{
        float v = d1 / (d1 - d3);
        return a + v * ab; // barycentric coordinates (1-v,v,0)
    }

    // Check if P in vertex region outside C
    Vector3f cp = p - c;
    float d5 = math::dot(ab, cp);
    float d6 = math::dot(ac, cp);
	
	// barycentric coordinates (0,0,1)
    if ( d6 >= 0.0f && d5 <= d6 )
		return c;

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5*d2 - d1*d6;
	
    if ( vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f )
	{
        float w = d2 / (d2 - d6);
		
		// barycentric coordinates (1-w,0,w)
        return a + w * ac;
    }

    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3*d6 - d5*d4;
	
    if ( va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f )
	{
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		
		// barycentric coordinates (0,1-w,w)
        return b + w * (c - b);
    }

    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
	
	// = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
    return a + ab * v + ac * w;
}




Real SoundMeshPreprocessor:: getTriangleDistance( const Vector3f& a, const Vector3f& b,
												const Vector3f& c, const Vector3f& p )
{
	return getClosestPointOnTriangle( a, b, c, p ).getDistanceTo( p );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Voxel Triangulation Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: triangulateVoxelTree( const VoxelTree& voxelTree, Size depth, const AABB3f& region, Real threshold,
												ArrayList<SoundVertex>& outputVertices,
												ArrayList<SoundTriangle>& outputTriangles )
{
	if ( voxelTree.root == NULL )
		return false;
	
	ArrayList<VoxelParent> parentStack;
	
	// Recursively triangulate the voxel tree at the maximum depth.
	triangulateVoxelRecursive( voxelTree.root, voxelTree.center, voxelTree.size,
								threshold, 0, depth, region, parentStack, outputVertices, outputTriangles );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Recursive Voxel Triangulation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundMeshPreprocessor:: triangulateVoxelRecursive( const Voxel* voxel, const Vector3f& center, Real voxelSize,
														Real threshold, Index depth, Size maxDepth, const AABB3f& region,
														ArrayList<VoxelParent>& parentStack, 
														ArrayList<SoundVertex>& outputVertices,
														ArrayList<SoundTriangle>& outputTriangles )
{
	if ( depth < maxDepth )
	{
		// Recurse to child voxels if they are non-empty.
		Real childVoxelSize = voxelSize*Real(0.5);
		Real quarterVoxelSize = childVoxelSize*Real(0.5);
		Vector3f minMax[2] = { center - quarterVoxelSize, center + quarterVoxelSize };
		Size nextDepth = depth + 1;
		Index childIndex = 0;
		
		// Push the voxel parent object onto the stack.
		AABB3f voxelBounds( center - childVoxelSize, center + childVoxelSize );
		parentStack.add( VoxelParent( voxel, voxelBounds ) );
		
		for ( Index x = 0; x <= 1; x++ )
		{
			for ( Index y = 0; y <= 1; y++ )
			{
				for ( Index z = 0; z <= 1; z++, childIndex++ )
				{
					if ( voxel->children[childIndex] != NULL )
					{
						Vector3f childCenter( minMax[x].x, minMax[y].y, minMax[z].z );
						
						// Skip children that are outside of the requested region.
						if ( !region.contains( childCenter ) )
							continue;
						
						triangulateVoxelRecursive( voxel->children[childIndex], childCenter, childVoxelSize,
													threshold, nextDepth, maxDepth, region, parentStack, outputVertices, outputTriangles );
					}
				}
			}
		}
		
		// Pop the voxel parent object.
		parentStack.removeLast();
	}
	else
	{
		// Triangulate this voxel and its neighbors.
		
		// Compute the most-positive neighboring voxel position.
		Vector3f corner = center + voxelSize;
		
		UByte octetData[sizeof(VoxelOctet)/sizeof(UByte)];
		VoxelOctet* octet = (VoxelOctet*)octetData;
		
		// Fill in the neighboring voxel positions.
		octet->center[0] = center;
		octet->center[1] = Vector3f( corner.x, center.y, center.z );
		octet->center[2] = Vector3f( corner.x, corner.y, center.z );
		octet->center[3] = Vector3f( center.x, corner.y, center.z );
		octet->center[4] = Vector3f( center.x, center.y, corner.z );
		octet->center[5] = Vector3f( corner.x, center.y, corner.z );
		octet->center[6] = corner;
		octet->center[7] = Vector3f( center.x, corner.y, corner.z );
		
		// Fill in the neighboring voxel fill values.
		for ( Index i = 0; i < 8; i++ )
			octet->fill[i] = getNeighborVoxelFill( parentStack, octet->center[i], octet->materialIndex[i] );
		
		// Triangulate the octet.
		triangulateOctet( *octet, threshold, outputVertices, outputTriangles );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Octet Triangulation Method
//############		
//##########################################################################################
//##########################################################################################




void SoundMeshPreprocessor:: triangulateOctet( const VoxelOctet& octet, Real threshold,
												ArrayList<SoundVertex>& outputVertices,
												ArrayList<SoundTriangle>& outputTriangles )
{
	Index outputVertexIndices[12];
	Index outputMaterialIndices[12];
	
	// Determine the index into the edge table which tells us which vertices are inside of the surface
	Index cubeIndex = 0;
	
	if ( octet.fill[0] < threshold ) cubeIndex |= 1;
	if ( octet.fill[1] < threshold ) cubeIndex |= 2;
	if ( octet.fill[2] < threshold ) cubeIndex |= 4;
	if ( octet.fill[3] < threshold ) cubeIndex |= 8;
	if ( octet.fill[4] < threshold ) cubeIndex |= 16;
	if ( octet.fill[5] < threshold ) cubeIndex |= 32;
	if ( octet.fill[6] < threshold ) cubeIndex |= 64;
	if ( octet.fill[7] < threshold ) cubeIndex |= 128;
	
	// Cube is entirely in/out of the surface.
	if ( edgeTable[cubeIndex] == 0 )
		return;
	
	// Find the vertices where the surface intersects the cube.
	if ( edgeTable[cubeIndex] & 1 )
	{
		outputVertexIndices[0] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[0], octet.center[1],
												octet.fill[0], octet.fill[1], threshold,
												octet.materialIndex[0], octet.materialIndex[1], outputMaterialIndices[0] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 2 )
	{
		outputVertexIndices[1] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[1], octet.center[2],
												octet.fill[1], octet.fill[2], threshold,
												octet.materialIndex[1], octet.materialIndex[2], outputMaterialIndices[1] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 4 )
	{
		outputVertexIndices[2] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[2], octet.center[3],
												octet.fill[2], octet.fill[3], threshold,
												octet.materialIndex[2], octet.materialIndex[3], outputMaterialIndices[2] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 8 )
	{
		outputVertexIndices[3] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[3], octet.center[0],
												octet.fill[3], octet.fill[0], threshold,
												octet.materialIndex[3], octet.materialIndex[0], outputMaterialIndices[3] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 16 )
	{
		outputVertexIndices[4] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[4], octet.center[5],
												octet.fill[4], octet.fill[5], threshold,
												octet.materialIndex[4], octet.materialIndex[5], outputMaterialIndices[4] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 32 )
	{
		outputVertexIndices[5] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[5], octet.center[6],
												octet.fill[5], octet.fill[6], threshold,
												octet.materialIndex[5], octet.materialIndex[6], outputMaterialIndices[5] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 64 )
	{
		outputVertexIndices[6] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[6], octet.center[7],
												octet.fill[6], octet.fill[7], threshold,
												octet.materialIndex[6], octet.materialIndex[7], outputMaterialIndices[6] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 128 )
	{
		outputVertexIndices[7] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[7], octet.center[4],
												octet.fill[7], octet.fill[4], threshold,
												octet.materialIndex[7], octet.materialIndex[4], outputMaterialIndices[7] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 256 )
	{
		outputVertexIndices[8] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[0], octet.center[4],
												octet.fill[0], octet.fill[4], threshold,
												octet.materialIndex[0], octet.materialIndex[4], outputMaterialIndices[8] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 512 )
	{
		outputVertexIndices[9] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[1], octet.center[5],
												octet.fill[1], octet.fill[5], threshold,
												octet.materialIndex[1], octet.materialIndex[5], outputMaterialIndices[9] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 1024 )
	{
		outputVertexIndices[10] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[2], octet.center[6],
												octet.fill[2], octet.fill[6], threshold,
												octet.materialIndex[2], octet.materialIndex[6], outputMaterialIndices[10] ) );
	}
	  
	if ( edgeTable[cubeIndex] & 2048 )
	{
		outputVertexIndices[11] = outputVertices.getSize();
		outputVertices.add( interpolateVoxels( octet.center[3], octet.center[7],
												octet.fill[3], octet.fill[7], threshold,
												octet.materialIndex[3], octet.materialIndex[7], outputMaterialIndices[11] ) );
	}
	
	const Int8* tableRow = triangleTable[cubeIndex];
	
	for ( Index i = 0; tableRow[i] != -1; i+= 3 )
	{
		Index v0 = outputVertexIndices[tableRow[i]];
		Index v1 = outputVertexIndices[tableRow[i+1]];
		Index v2 = outputVertexIndices[tableRow[i+2]];
		
		// Compute the best material for this trangle.
		Index m0 = outputMaterialIndices[tableRow[i]];
		Index m1 = outputMaterialIndices[tableRow[i+1]];
		Index m2 = outputMaterialIndices[tableRow[i+2]];
		
		outputTriangles.add( SoundTriangle( v0, v1, v2, getBestMaterial( m0, m1, m2 ) ) );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Voxel Interpolation Method
//############		
//##########################################################################################
//##########################################################################################




Vector3f SoundMeshPreprocessor:: interpolateVoxels( const Vector3f& p1, const Vector3f& p2, Real value1, Real value2, Real threshold,
												Index materialIndex1, Index materialIndex2, Index& materialIndex )
{
	if ( math::abs(threshold - value1) < math::epsilon<Real>() )
	{
		materialIndex = materialIndex1;
		return p1;
	}
	
	if ( math::abs(threshold - value2) < math::epsilon<Real>() )
	{
		materialIndex = materialIndex2;
		return p2;
	}
	
	if ( math::abs(value2 - value1) < math::epsilon<Real>() )
	{
		materialIndex = materialIndex1;
		return p1;
	}
	
	Real mu = (threshold - value1) / (value2 - value1);
	
	// Pick the closer material.
	materialIndex = mu > Real(0.5) ? materialIndex2 : materialIndex1;
	
	return p1 + mu*(p2 - p1);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Neighbor Voxel Fill Methods
//############		
//##########################################################################################
//##########################################################################################




Real SoundMeshPreprocessor:: getNeighborVoxelFill( const ArrayList<VoxelParent>& parentStack,
													const Vector3f& center, Index& materialIndex )
{
	Index parentIndex = parentStack.getSize();
	
	for ( ; parentIndex > 0; parentIndex-- )
	{
		const VoxelParent& parent = parentStack[parentIndex - 1];
		
		if ( parent.bounds.contains( center ) )
		{
			// Found a parent of the neighoring voxel.
			// Traverse downwards through the tree to get the voxel value.
			return getVoxelFill( parent.voxel, parent.bounds.getCenter(), parent.bounds.getWidth(), center,
								0, parentStack.getSize() - parentIndex + 1, materialIndex );
		}
	}
	
	// Didn't find a parent. This means that the voxel is outside of the tree bounds.
	return 0;
}




Real SoundMeshPreprocessor:: getVoxelFill( const SoundMeshPreprocessor::Voxel* parent, const Vector3f& center,
											Real voxelSize, const Vector3f& query, Index depth, Size queryDepth,
											Index& materialIndex )
{
	if ( depth == queryDepth )
	{
		materialIndex = parent->materialIndex;
		return parent->fill;
	}
	
	// Compute the index of the child voxel for the query point.
	UInt xDirection = query.x > center.x;
	UInt yDirection = query.y > center.y;
	UInt zDirection = query.z > center.z;
	UInt childIndex = (xDirection << 2) | (yDirection << 1) | zDirection;
	
	const SoundMeshPreprocessor::Voxel* child = parent->children[childIndex];
	
	// If the child is empty, then return 0 fill.
	if ( child == NULL )
	{
		materialIndex = 0;
		return 0;
	}
	
	Real childVoxelSize = voxelSize*Real(0.5);
	Real quarterVoxelSize = childVoxelSize*Real(0.5);
	Vector3f childCenter( xDirection ? center.x + quarterVoxelSize : center.x - quarterVoxelSize,
						yDirection ? center.y + quarterVoxelSize : center.y - quarterVoxelSize,
						zDirection ? center.z + quarterVoxelSize : center.z - quarterVoxelSize );
	
	return getVoxelFill( child, childCenter, childVoxelSize, query, depth + 1, queryDepth, materialIndex );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Vertex Welding Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: weldVertices( const ArrayList<SoundVertex>& inputVertices,
										const ArrayList<SoundTriangle>& inputTriangles, Real tolerance,
										ArrayList<SoundVertex>& outputVertices,
										ArrayList<SoundTriangle>& outputTriangles )
{
	//****************************************************************
	// Create the welding vertex data structure.
	
	const Size numInputVertices = inputVertices.getSize();
	
	// Create an array of welding vertices.
	Array<WeldingVertex> weldingVertices( numInputVertices );
	
	// Initialize each welding vertex index.
	for ( Index i = 0; i < numInputVertices; i++ )
		weldingVertices[i].weldedIndex = i;
	
	// Create the vertex welding grid hash table.
	Array<ShortArrayList<Index,8> > gridHashTable( numInputVertices );
	
	//***************************************************************************
	// Weld all of the vertices using a spatial hash structure
	
	const Real gridSize = Real(2)*tolerance;
	
	for ( Index i = 0; i < numInputVertices; i++ )
	{
		const Vector3f& vertex = inputVertices[i];
		
		// Compute the cells that this vertex's neighborhood overlaps on the grid.
		Vector3f cellMin = (vertex - tolerance)/gridSize;
		Vector3f cellMax = (vertex + tolerance)/gridSize;
		
		math::AABB3i cellBounds( cellMin, cellMax );
		
		// Try to weld the vertex.
		if ( !weldVertex( vertex, inputVertices, gridHashTable, cellBounds, tolerance, weldingVertices[i].weldedIndex ) )
		{
			// Couldn't weld the vertex.
			// Insert the vertex into the cells that its neighborhood overlaps.
			for ( Int x = cellBounds.min.x; x <= cellBounds.max.x; x++ )
			{
				for ( Int y = cellBounds.min.y; y <= cellBounds.max.y; y++ )
				{
					for ( Int z = cellBounds.min.z; z <= cellBounds.max.z; z++ )
					{
						Hash bucketIndex = getGridCellHash( x, y, z ) % gridHashTable.getSize();
						
						ShortArrayList<Index,8>& bucket = gridHashTable[bucketIndex];
						
						// Add the vertex to the bucket if no welding candidates were found.
						bucket.add( i );
					}
				}
			}
		}
	}
	
	
	//***************************************************************************
	// Prepare the output list of vertices.
	
	for ( Index i = 0; i < numInputVertices; i++ )
	{
		if ( weldingVertices[i].weldedIndex == i )
		{
			// Vertex was not welded.
			weldingVertices[i].finalIndex = outputVertices.getSize();
			outputVertices.add( inputVertices[i] );
		}
	}
	
	
	//***************************************************************************
	// Prepare the output list of triangles.
	
	const Size numInputTriangles = inputTriangles.getSize();
	
	for ( Index i = 0; i < numInputTriangles; i++ )
	{
		const SoundTriangle& triangle = inputTriangles[i];
		
		// Determine the final indices of this triangle's vertices after welding.
		Index v0 = weldingVertices[weldingVertices[triangle.v[0]].weldedIndex].finalIndex;
		Index v1 = weldingVertices[weldingVertices[triangle.v[1]].weldedIndex].finalIndex;
		Index v2 = weldingVertices[weldingVertices[triangle.v[2]].weldedIndex].finalIndex;
		
		// Determine if the triangle is degenerate. If so, skip it.
		if ( v0 == v1 || v0 == v2 || v1 == v2 )
			continue;
		
		// Create the new triangle.
		outputTriangles.add( SoundTriangle( v0, v1, v2, triangle.materialIndex ) );
	}
	
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Single Vertex Welding Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: weldVertex( const Vector3f& vertex, const ArrayList<Vector3f>& vertices, 
										Array<ShortArrayList<Index,8> >& gridHashTable,
										const math::AABB3i& gridNeighborhood, Real tolerance, Index& weldedIndex )
{
	Real toleranceSquared = tolerance*tolerance;
	
	for ( Int x = gridNeighborhood.min.x; x <= gridNeighborhood.max.x; x++ )
	{
		for ( Int y = gridNeighborhood.min.y; y <= gridNeighborhood.max.y; y++ )
		{
			for ( Int z = gridNeighborhood.min.z; z <= gridNeighborhood.max.z; z++ )
			{
				Hash bucketIndex = getGridCellHash( x, y, z ) % gridHashTable.getSize();
				
				ShortArrayList<Index,8>& bucket = gridHashTable[bucketIndex];
				const Size bucketSize = bucket.getSize();
				
				// Look for vertices within the welding tolerance for this bucket.
				for ( Index j = 0; j < bucketSize; j++ )
				{
					const Index v = bucket[j];
					
					if ( vertices[v].getDistanceToSquared( vertex ) <= toleranceSquared )
					{
						weldedIndex = v;
						return true;
					}
				}
			}
		}
	}
	
	// Didn't find a vertex to weld this one to.
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Collapse Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: fattenMesh( const ArrayList<SoundVertex>& inputVertices,
										const ArrayList<SoundTriangle>& inputTriangles,
										ArrayList<FatVertex>& outputVertices,
										ArrayList<FatTriangle>& outputTriangles )
{
	//***************************************************************************
	// Prepare the output list of vertices.
	
	const Size numInputVertices = inputVertices.getSize();
	outputVertices.setCapacity( numInputVertices );
	
	for ( Index i = 0; i < numInputVertices; i++ )
		outputVertices.add( FatVertex( inputVertices[i] ) );
	
	//***************************************************************************
	// Prepare the output list of triangles.
	
	const Size numInputTriangles = inputTriangles.getSize();
	
	for ( Index i = 0; i < numInputTriangles; i++ )
	{
		const SoundTriangle& triangle = inputTriangles[i];
		
		// Determine the final indices of this triangle's vertices after welding.
		Index v0 = triangle.v[0];
		Index v1 = triangle.v[1];
		Index v2 = triangle.v[2];
		
		// Determine if the triangle is degenerate. If so, skip it.
		if ( v0 == v1 || v0 == v2 || v1 == v2 )
			continue;
		
		// Compute the final normal for the triangle.
		const Vector3f& p0 = outputVertices[v0].position;
		const Vector3f& p1 = outputVertices[v1].position;
		const Vector3f& p2 = outputVertices[v2].position;
		
		Vector3f normal = math::cross( p1 - p0, p2 - p0 );
		Float nMag = normal.getMagnitude();
		
		// Skip degenerate (e.g. linear) triangles.
		if ( nMag < math::epsilon<Float>() )
			continue;
		
		// Construct a plane for the triangle.
		Plane3f plane( normal / nMag, p0 );
		
		// Get the output index for this triangle.
		Index triangleIndex = outputTriangles.getSize();
		
		// Create the new triangle.
		outputTriangles.add( FatTriangle( v0, v1, v2, triangle.materialIndex, plane ) );
		
		// Tell the vertices that they are part of this triangle.
		outputVertices[v0].triangleNeighbors.add( triangleIndex );
		outputVertices[v1].triangleNeighbors.add( triangleIndex );
		outputVertices[v2].triangleNeighbors.add( triangleIndex );
	}
	
	//***************************************************************************
	// Determine the vertex neighbors.
	
	const Size numOutputVertices = outputVertices.getSize();
	
	for ( Index i = 0; i < numOutputVertices; i++ )
	{
		FatVertex& vertex = outputVertices[i];
		const Size numTriangleNeighbors = vertex.triangleNeighbors.getSize();
		
		for ( Index j = 0; j < numTriangleNeighbors; j++ )
		{
			const FatTriangle& triangle = outputTriangles[vertex.triangleNeighbors[j]];
			
			for ( Index k = 0; k < 3; k++ )
			{
				Index neighborIndex = triangle.v[k];
				
				// Skip this vertex;
				if ( neighborIndex == i )
					continue;
				
				// See if this neighbor index is already part of the list of neighbors for this vertex.
				const Size numVertexNeighbors = vertex.vertexNeighbors.getSize();
				
				for ( Index l = 0; l < numVertexNeighbors; l++ )
				{
					// See if we found this vertex neighbor already. If so, break from the loop.
					if ( vertex.vertexNeighbors[l] == neighborIndex )
						goto duplicateVertexNeighbor;
				}
				
				// We didn't find this neighbor yet, so add it to the list of neighbors.
				vertex.vertexNeighbors.add( neighborIndex );
				
				duplicateVertexNeighbor:;
			}
		}
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Collapse Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: collapseEdges( ArrayList<FatVertex>& vertices, ArrayList<FatTriangle>& triangles, Real maxCost )
{
	//***************************************************************************
	// Compute the error matrix for each vertex in the mesh.
	
	const Size numVertices = vertices.getSize();
	ArrayList<QEMVertex> qemVertices( numVertices );
	
	for ( Index i = 0; i < numVertices; i++ )
	{
		FatVertex& vertex = vertices[i];
		vertex.checked = false;
		qemVertices.add( computeQ( vertex, triangles ) );
	}
	
	//***************************************************************************
	// Compute the target vertices and initial costs for all edges in the mesh.
	
	ArrayList<EdgeCollapse> edgeCollapses;
	
	for ( Index i = 0; i < numVertices; i++ )
	{
		FatVertex& vertex = vertices[i];
		QEMVertex& qemVertex = qemVertices[i];
		const Size numNeighbors = vertex.vertexNeighbors.getSize();
		
		// Skip previously collapsed vertices.
		if ( vertex.collapsed )
			continue;
		
		for ( Index n = 0; n < numNeighbors; n++ )
		{
			const Index neighborIndex = vertex.vertexNeighbors[n];
			FatVertex& vertex2 = vertices[neighborIndex];
			
			// Skip neighbors that have already been checked.
			if ( vertex2.checked || vertex2.collapsed )
				continue;
			
			const QEMVertex& qemVertex2 = qemVertices[neighborIndex];
			
			// Compute the combined error matrix for these vertices.
			Matrix4f Q12 = qemVertex.Q + qemVertex2.Q;
	
			// Compute the optimal collapse vertex and the cost for this edge collapse.
			Vector3f target = computeCollapseVertex( Q12, vertex.position, vertex2.position );
			
			// Compute the error for this target vertex.
			Real cost = computeQError( Q12, target );
			
			// Add this edge collapse to the edge collapse list.
			edgeCollapses.add( EdgeCollapse( i, neighborIndex, target, cost ) );
		}
		
		vertex.checked = true;
	}
	
	EdgeCollapseQueue edgeCollapseQueue( edgeCollapses.getSize() );
	
	for ( Index i = 0; i < edgeCollapses.getSize(); i++ )
	{
		EdgeCollapse& collapse = edgeCollapses[i];
		QEMVertex& qemV1 = qemVertices[collapse.v1];
		QEMVertex& qemV2 = qemVertices[collapse.v2];
		
		qemV1.collapses.add( &collapse );
		qemV2.collapses.add( &collapse );
		edgeCollapseQueue.add( EdgeCollapseReference( &collapse ) );
	}
	
	//***************************************************************************
	// Collapse edges until the maximum cost is reached.
	
	nextEdgeCollapse:
	
	while ( edgeCollapseQueue.getSize() > 0 )
	{
		// Get the next edge collapse, then remove it from the queue.
		EdgeCollapseReference collapseReference = edgeCollapseQueue.getFirst();
		edgeCollapseQueue.remove();
		
		// Check to see if all further collapses exceed the maximum cost parameter.
		// If so, we are done collapsing edges.
		if ( collapseReference.getCost() > maxCost )
			break;
		
		EdgeCollapse& collapse = *collapseReference.collapse;
		
		// Skip degenerate edge collapses.
		if ( collapse.v1 == collapse.v2 )
			continue;
		
		const Index fromIndex = collapse.v1;
		FatVertex& fromVertex = vertices[fromIndex];
		
		const Index toIndex = collapse.v2;
		FatVertex& toVertex = vertices[toIndex];
		
		// Skip 'from' or 'to' vertices that have already been collapsed.
		if ( fromVertex.collapsed || toVertex.collapsed )
			continue;
		
		if ( vertexIsBorder( fromVertex, triangles ) || vertexIsBorder( toVertex, triangles ) )
			continue;
		
		//***************************************************************************
		// Check to make sure this edge collapse won't invert any triangles.
		
		for ( Index t = 0; t < fromVertex.triangleNeighbors.getSize(); t++ )
		{
			const Index triangleIndex = fromVertex.triangleNeighbors[t];
			const FatTriangle& triangle = triangles[triangleIndex];
			
			// Check only triangles that aren't removed.
			if ( !triangle.hasVertex( toIndex ) )
			{
				// Recompute the triangle plane equation.
				Plane3f newPlane( triangle.v[0] == fromIndex ? collapse.target : vertices[triangle.v[0]].position,
								triangle.v[1] == fromIndex ? collapse.target : vertices[triangle.v[1]].position,
								triangle.v[2] == fromIndex ? collapse.target : vertices[triangle.v[2]].position );
				
				// If the normals don't point in the same direction, then the triangle
				// would be reversed by this collapse and the mesh would fold over.
				// Don't do this edge collapse because it makes the model degenerate.
				if ( math::dot( triangle.plane.normal, newPlane.normal ) < Real(0.0) )
					goto nextEdgeCollapse;
			}
		}
		
		for ( Index t = 0; t < toVertex.triangleNeighbors.getSize(); t++ )
		{
			const Index triangleIndex = toVertex.triangleNeighbors[t];
			const FatTriangle& triangle = triangles[triangleIndex];
			
			// Check only triangles that aren't removed.
			if ( !triangle.hasVertex( fromIndex ) )
			{
				// Recompute the triangle plane equation.
				Plane3f newPlane( triangle.v[0] == toIndex ? collapse.target : vertices[triangle.v[0]].position,
								triangle.v[1] == toIndex ? collapse.target : vertices[triangle.v[1]].position,
								triangle.v[2] == toIndex ? collapse.target : vertices[triangle.v[2]].position );
				
				// If the normals don't point in the same direction, then the triangle
				// would be reversed by this collapse and the mesh would fold over.
				// Don't do this edge collapse because it makes the model degenerate.
				if ( math::dot( triangle.plane.normal, newPlane.normal ) < Real(0.0) )
					goto nextEdgeCollapse;
			}
		}
		
		//***************************************************************************
		
		// Mark the 'from' vertex as collapsed.
		fromVertex.collapsed = true;
		
		// Update the 'to' vertex to have the optimal collapsed position.
		toVertex.position = collapse.target;
		
		
		// Update the triangles that shared the from vertex.
		for ( Index t = 0; t < fromVertex.triangleNeighbors.getSize(); t++ )
		{
			const Index triangleIndex = fromVertex.triangleNeighbors[t];
			FatTriangle& triangle = triangles[triangleIndex];
			
			if ( triangle.hasVertex( toIndex ) )
			{
				// This triangle becomes degenerate after the edge collapse.
				// Mark it as collapsed.
				triangle.collapsed = true;
				
				// Remove this triangle from the list of neighbors for its vertices.
				if ( triangle.v[0] != fromIndex )
					vertices[triangle.v[0]].triangleNeighbors.remove( triangleIndex );
				
				if ( triangle.v[1] != fromIndex )
					vertices[triangle.v[1]].triangleNeighbors.remove( triangleIndex );
				
				if ( triangle.v[2] != fromIndex )
					vertices[triangle.v[2]].triangleNeighbors.remove( triangleIndex );
			}
			else
			{
				// This triangle needs to be updated with the new vertex index.
				triangle.replaceVertex( fromIndex, toIndex );
				
				// Recompute the triangle plane equation.
				triangle.plane = Plane3f( vertices[triangle.v[0]].position,
										vertices[triangle.v[1]].position,
										vertices[triangle.v[2]].position );
				
				// Make sure the to vertex has this triangle as its neighbor.
				toVertex.triangleNeighbors.add( triangleIndex );
			}
		}
		
		// Clear the triangle neighbor list for the 'from' vertex since it is no longer used.
		fromVertex.triangleNeighbors.clear();
		
		toVertex.vertexNeighbors.removeUnordered( fromIndex );
		
		for ( Index i = 0; i < fromVertex.vertexNeighbors.getSize(); i++ )
		{
			const Index neighborIndex = fromVertex.vertexNeighbors[i];
			
			if ( neighborIndex == toIndex )
				continue;
			
			// Add this neighbor of the 'from' vertex to the list of neighbors for the 'to' vertex.
			if ( !toVertex.vertexNeighbors.contains( neighborIndex ) )
			{
				// Update the 'to' vertex.
				toVertex.vertexNeighbors.add( neighborIndex );
				
				// Update the neighbor vertex.
				FatVertex& neighbor = vertices[neighborIndex];
				neighbor.vertexNeighbors.removeUnordered( fromIndex );
				neighbor.vertexNeighbors.add( toIndex );
			}
		}
		
		fromVertex.vertexNeighbors.clear();
		
		QEMVertex& fromQEMVertex = qemVertices[fromIndex];
		QEMVertex& toQEMVertex = qemVertices[toIndex];
		
		// Compute the error matrix for the collapsed vertex.
		toQEMVertex.Q += fromQEMVertex.Q;
		
		// Remove the current collapse from the 'to' vertex.
		toQEMVertex.collapses.remove( collapseReference );
		
		// Merge the collapses associated with the 'from' vertex with those from the 'to' vertex.
		for ( Index i = 0; i < fromQEMVertex.collapses.getSize(); i++ )
		{
			const EdgeCollapseReference& ref = fromQEMVertex.collapses[i];
			
			// Replace the 'from' vertex in the collapse structures.
			if ( ref.collapse->v1 == fromIndex )
				ref.collapse->v1 = toIndex;
			else if ( ref.collapse->v2 == fromIndex )
				ref.collapse->v2 = toIndex;
			
			if ( ref.collapse->v1 == ref.collapse->v2 )
				continue;
			
			// Add the collapse to the collapses for the 'to' vertex if it is not a duplicate.
			if ( !toQEMVertex.collapses.contains( ref ) )
				toQEMVertex.collapses.add( ref );
		}
		
		// Clear the collapses from the 'from' vertex.
		fromQEMVertex.collapses.clear();
		
		// Recompute the cost for all edge collapses that involve the 'to' vertex.
		for ( Index i = 0; i < toQEMVertex.collapses.getSize(); )
		{
			const EdgeCollapseReference& ref = toQEMVertex.collapses[i];
			EdgeCollapse& newCollapse = *ref.collapse;
			const Index v1Index = newCollapse.v1;
			const Index v2Index = newCollapse.v2;
			
			// Compute the combined error matrix for these vertices.
			Matrix4f Q12 = qemVertices[v1Index].Q + qemVertices[v2Index].Q;
			
			// Compute the new optimal collapse vertex and the cost for this edge collapse.
			newCollapse.target = computeCollapseVertex( Q12, vertices[v1Index].position, vertices[v2Index].position );
			
			// Compute the error for this target vertex.
			newCollapse.cost = computeQError( Q12, newCollapse.target );
			
			// Update the recomputed edge collapse in the heap.
			edgeCollapseQueue.update( newCollapse.queueIndex );
			
			i++;
		}
	}
	
	//***************************************************************************
	
	return true;
}




Matrix4f SoundMeshPreprocessor:: computeQ( const FatVertex& vertex, const ArrayList<FatTriangle>& triangles )
{
	const Size numTriangleNeighbors = vertex.triangleNeighbors.getSize();
	Matrix4f Q;
	
	for ( Index i = 0; i < numTriangleNeighbors; i++ )
	{
		const Index triangleIndex = vertex.triangleNeighbors[i];
		const FatTriangle& triangle = triangles[triangleIndex];
		
		// Compute the error matrix for this triangle's plane.
		Vector4f p( triangle.plane.normal, triangle.plane.offset );
		Matrix4f Kp( p.x*p.x, p.y*p.x, p.z*p.x, p.w*p.x,
					p.x*p.y, p.y*p.y, p.z*p.y, p.w*p.y,
					p.x*p.z, p.y*p.z, p.z*p.z, p.w*p.z,
					p.x*p.w, p.y*p.w, p.z*p.w, p.w*p.w );
		
		// Accumulate the error matrix.
		Q += Kp;
	}
	
	return Q;
}





Real SoundMeshPreprocessor:: computeQError( const Matrix4f& Q, const Vector3f& v )
{
	Vector4f v4( v, 1 );
	
	return math::abs( math::dot( v4, Q*v4 ) );
}




Vector3f SoundMeshPreprocessor:: computeCollapseVertex( const Matrix4f& Q12, const Vector3f& v1, const Vector3f& v2 )
{/*
	// Compute the matrix to solve for the optimal collapse location.
	Matrix4f q = Q12;
	q.x.w = q.y.w = q.z.w = 0;
	q.w.w = 1;
	
	// Try inverting the matrix Q to compute the minimum-cost collapsed vertex.
	Matrix4f qInverse;
	
	Vector3f midpoint = math::midpoint( v1, v2 );
	Real midpointCost = computeQError( Q12, midpoint );
	
	if ( q.invert( qInverse, 0.1 ) && computeQError( Q12, qInverse.w.getXYZ() ) < midpointCost )
	{
		return qInverse.w.getXYZ();
	}
	else*/
	{
		// Inversion failed so pick the vertex or midpoint with the lowest cost.
		Vector3f midpoint = math::midpoint( v1, v2 );
		Real midpointCost = computeQError( Q12, midpoint );
		Real v1Cost = computeQError( Q12, v1 );
		Real v2Cost = computeQError( Q12, v2 );
		
		// Return the vertex or midpoint with the least cost.
		if ( v1Cost < v2Cost )
		{
			if ( v1Cost < midpointCost )
				return v1;
			else
				return midpoint;
		}
		else
		{
			if ( v2Cost < midpointCost )
				return v2;
			else
				return midpoint;
		}
	}
}




Bool SoundMeshPreprocessor:: vertexIsBorder( const FatVertex& vertex, const ArrayList<FatTriangle>& triangles )
{
	const Size numVertexNeighbors = vertex.vertexNeighbors.getSize();
	const Size numTriangleNeighbors = vertex.triangleNeighbors.getSize();
	Index materialIndex = math::max<Index>();
	
	for ( Index v = 0; v < numVertexNeighbors; v++ )
	{
		const Index neighborIndex = vertex.vertexNeighbors[v];
		Size numNeighborTriangles = 0;
		
		for ( Index t = 0; t < numTriangleNeighbors; t++ )
		{
			const FatTriangle& triangle = triangles[vertex.triangleNeighbors[t]];
			
			// If there is a disagreement on the material used by the triangles that shared this vertex, it is a border.
			if ( materialIndex == math::max<Index>() )
				materialIndex = triangle.materialIndex;
			else if ( materialIndex != triangle.materialIndex )
				return true;
			
			if ( triangle.hasVertex( neighborIndex ) )
				numNeighborTriangles++;
		}
		
		if ( numNeighborTriangles == Size(1) )
			return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mesh Flattening Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: flattenMesh( ArrayList<FatVertex>& inputVertices, ArrayList<FatTriangle>& inputTriangles,
											ArrayList<FatVertex>& outputVertices, ArrayList<FatTriangle>& outputTriangles )
{
	//***************************************************************************
	// Build the final output list of vertices.
	
	const Size numInputVertices = inputVertices.getSize();
	
	for ( Index i = 0; i < numInputVertices; i++ )
	{
		FatVertex& inputVertex = inputVertices[i];
		
		if ( inputVertex.collapsed )
			continue;
		
		inputVertex.finalIndex = outputVertices.getSize();
		
		outputVertices.add( FatVertex( inputVertex.position ) );
	}
	
	//***************************************************************************
	// Build the list of vertex neighbors.
	
	for ( Index i = 0; i < numInputVertices; i++ )
	{
		const FatVertex& inputVertex = inputVertices[i];
		
		if ( inputVertex.collapsed )
			continue;
		
		FatVertex& outputVertex = outputVertices[inputVertex.finalIndex];
		
		for ( Index j = 0; j < inputVertex.vertexNeighbors.getSize(); j++ )
		{
			const FatVertex& neighborVertex = inputVertices[inputVertex.vertexNeighbors[j]];
			
			if ( neighborVertex.collapsed )
				continue;
			
			outputVertex.vertexNeighbors.add( neighborVertex.finalIndex );
		}
	}
	
	//***************************************************************************
	// Build the final output list of triangles.
	
	const Size numInputTriangles = inputTriangles.getSize();
	
	for ( Index i = 0; i < numInputTriangles; i++ )
	{
		FatTriangle& inputTriangle = inputTriangles[i];
		
		if ( inputTriangle.collapsed )
			continue;
		
		inputTriangle.finalIndex = outputTriangles.getSize();
		
		// Get the final vertex indices.
		Index v0 = inputVertices[inputTriangle.v[0]].finalIndex;
		Index v1 = inputVertices[inputTriangle.v[1]].finalIndex;
		Index v2 = inputVertices[inputTriangle.v[2]].finalIndex;
		
		// Ignore triangles with 0 area.
		Real area = Real(0.5)*math::cross( (outputVertices[v2].position - outputVertices[v0].position),
											(outputVertices[v2].position - outputVertices[v1].position) ).getMagnitude();
		
		if ( area < math::epsilon<Real>() )
			continue;
		
		outputTriangles.add( FatTriangle( v0, v1, v2, inputTriangle.materialIndex, inputTriangle.plane ) );
	}
	
	//***************************************************************************
	// Build the list of triangle neighbors.
	
	for ( Index i = 0; i < numInputVertices; i++ )
	{
		const FatVertex& inputVertex = inputVertices[i];
		
		if ( inputVertex.collapsed )
			continue;
		
		FatVertex& outputVertex = outputVertices[inputVertex.finalIndex];
		
		for ( Index j = 0; j < inputVertex.triangleNeighbors.getSize(); j++ )
		{
			const FatTriangle& neighborTriangle = inputTriangles[inputVertex.triangleNeighbors[j]];
			
			if ( neighborTriangle.collapsed )
				continue;
			
			outputVertex.triangleNeighbors.add( neighborTriangle.finalIndex );
		}
	}
	
	return true;
}




Bool SoundMeshPreprocessor:: dietMesh( ArrayList<FatVertex>& inputVertices, ArrayList<FatTriangle>& inputTriangles,
									ArrayList<SoundVertex>& outputVertices, ArrayList<SoundTriangle>& outputTriangles )
{
	const Size numInputVertices = inputVertices.getSize();
	const Size numInputTriangles = inputTriangles.getSize();
	outputVertices.setCapacity( numInputVertices );
	outputTriangles.setCapacity( numInputTriangles );
	
	//***************************************************************************
	// Build the final output list of vertices.
	
	for ( Index i = 0; i < numInputVertices; i++ )
	{
		FatVertex& inputVertex = inputVertices[i];
		
		if ( inputVertex.collapsed )
			continue;
		
		inputVertex.finalIndex = outputVertices.getSize();
		
		outputVertices.add( inputVertex.position );
	}
	
	//***************************************************************************
	// Build the final output list of triangles.
	
	for ( Index i = 0; i < numInputTriangles; i++ )
	{
		FatTriangle& inputTriangle = inputTriangles[i];
		
		if ( inputTriangle.collapsed )
			continue;
		
		inputTriangle.finalIndex = outputTriangles.getSize();
		
		// Get the final vertex indices.
		Index v0 = inputVertices[inputTriangle.v[0]].finalIndex;
		Index v1 = inputVertices[inputTriangle.v[1]].finalIndex;
		Index v2 = inputVertices[inputTriangle.v[2]].finalIndex;
		
		outputTriangles.add( SoundTriangle( v0, v1, v2, inputTriangle.materialIndex ) );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Mesh Construction Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: buildSoundMesh( const ArrayList<FatVertex>& inputVertices, const ArrayList<FatTriangle>& inputTriangles,
											const Shared<ArrayList<SoundMaterial> >& materials, const MeshRequest& request,
											SoundMesh& mesh )
{
	Timer timer;
	
	//***************************************************************************
	// Build the final output list of vertices.
	
	const Size numInputVertices = inputVertices.getSize();
	Shared<ArrayList<SoundVertex> > vertices = Shared<ArrayList<SoundVertex> >::construct();
	
	for ( Index i = 0; i < numInputVertices; i++ )
		vertices->add( inputVertices[i].position );
	
	//***************************************************************************
	// Build the final output list of triangles.
	
	const Size numInputTriangles = inputTriangles.getSize();
	Shared<ArrayList<InternalSoundTriangle> > triangles = Shared<ArrayList<InternalSoundTriangle> >::construct();
	
	for ( Index i = 0; i < numInputTriangles; i++ )
	{
		const FatTriangle& triangle = inputTriangles[i];
		
		triangles->add( InternalSoundTriangle( &(*vertices)[triangle.v[0]],
												&(*vertices)[triangle.v[1]],
												&(*vertices)[triangle.v[2]], &(*materials)[triangle.materialIndex] ) );
		InternalSoundTriangle& newTriangle = triangles->getLast();
		
		// Determine the key vertex and number of rows and columns for this triangle.
		Index maxHeightEdge = 0;
		Real maxHeight = 0;
		Real maxHeightEdgeLength = 0;
		
		for ( Index e = 0; e < 3; e++ )
		{
			const SoundVertex* e1, *e2;
			newTriangle.getEdgeVertices( e, e1, e2 );
			
			Real edgeLength = e1->getDistanceTo( *e2 );
			Real height = newTriangle.getArea()*Real(2) / edgeLength;
			
			if ( height > maxHeight )
			{
				maxHeightEdge = e;
				maxHeight = height;
				maxHeightEdgeLength = edgeLength;
			}
		}
		
		Index keyIndex = InternalSoundTriangle::getEdgeFreeVertexIndex( maxHeightEdge );
		Size numRows = (Size)math::ceiling( maxHeight / request.diffuseResolution );
		Size numColumns = (Size)math::ceiling( maxHeightEdgeLength / request.diffuseResolution );
		
		newTriangle.setKeyVertex( keyIndex );
		newTriangle.setRowCount( numRows );
		newTriangle.setColumnCount( numColumns );
	}
	
	// Update the timer.
	timer.update();
	
	//***************************************************************************
	// Construct the BVH for this mesh.
	
	// Construct the BVH object in a temporary mesh object.
	SoundMesh mesh2;
	mesh2.setData( vertices, triangles, materials, Shared<DiffractionGraph>() );
	
	// Update the timer and the BVH timing information.
	timer.update();
	
	if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
		request.statistics->bvhTime += timer.getLastInterval();
	
	//***************************************************************************
	// Build the diffraction graph if necessary.
	
	Shared<DiffractionGraph> diffractionGraph;
	
	if ( request.flags.isSet( MeshFlags::DIFFRACTION_EDGES ) )
	{
		diffractionGraph = buildEdgeGraph( inputVertices, inputTriangles,
											*vertices, *triangles, *mesh2.getBVH(), request );
	}
	
	//***************************************************************************
	// Construct and return the final mesh.
	
	// Set the mesh attributes.
	mesh.setData( vertices, triangles, materials, diffractionGraph );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Diffraction Edge Graph Building Method
//############		
//##########################################################################################
//##########################################################################################




Shared<DiffractionGraph> SoundMeshPreprocessor:: buildEdgeGraph( const ArrayList<FatVertex>& fatVertices, const ArrayList<FatTriangle>& fatTriangles,
																	const ArrayList<SoundVertex>& vertices, ArrayList<InternalSoundTriangle>& triangles,
																	const BVH& bvh, const MeshRequest& request )
{
	ArrayList<TempDiffractionEdge> tempEdges;
	
	Timer timer;
	
	//**************************************************************************
	// Determine which edges should be diffraction edges.
	
	Real diffractionThreshold = math::abs( math::cos( math::degreesToRadians( math::mod( request.minDiffractionEdgeAngle, Real(90) ) ) ) );
	
	for ( Index i = 0; i < fatTriangles.getSize(); i++ )
	{
		const FatTriangle& fatTriangle = fatTriangles[i];
		InternalSoundTriangle& triangle = triangles[i];
		
		for ( Index v = 0; v < 3; v++ )
		{
			const Index vertexIndex = fatTriangle.v[v];
			const FatVertex& fatVertex = fatVertices[vertexIndex];
			
			const Size numTriangleNeighbors = fatVertex.triangleNeighbors.getSize();
			
			for ( Index j = 0; j < numTriangleNeighbors; j++ )
			{
				Index neighborIndex = fatVertex.triangleNeighbors[j];
				
				if ( neighborIndex == i )
					continue;
				
				const FatTriangle& fatTriangle2 = fatTriangles[neighborIndex];
				InternalSoundTriangle& triangle2 = triangles[neighborIndex];
				
				for ( Index l = v + 1; l < 3; l++ )
				{
					Index searchIndex = fatTriangle.v[l];
					
					for ( Index k = 0; k < 3; k++ )
					{
						if ( fatTriangle2.v[k] == searchIndex )
						{
							// The index of the edge on triangle 1.
							Index edgeIndex1 = InternalSoundTriangle::getEdgeIndex( v, l );
							Index edgeIndex2 = math::max<Index>();
							
							// Compute the edge index on triangle 2.
							for ( Index m = 0; m < 3; m++ )
							{
								if ( m != k && fatTriangle2.v[m] == vertexIndex )
								{
									edgeIndex2 = InternalSoundTriangle::getEdgeIndex( k, m );
									break;
								}
							}
							
							// Skip this edge if it is not shared.
							if ( edgeIndex2 == math::max<Index>() )
								continue;
							
							// Check to see if this edge has already been found. If so, ignore it.
							if ( triangle.getDiffractionEdge(edgeIndex1) != NULL ||
								triangle2.getDiffractionEdge(edgeIndex2) != NULL )
								continue;
							
							const SoundVertex* v1;
							const SoundVertex* v2;
							triangle.getEdgeVertices( edgeIndex1, v1, v2 );
							
							// Skip edges that are too short.
							if ( v1->getDistanceTo(*v2) < request.minDiffractionEdgeLength )
								continue;
							
							const SoundVertex* free1 = triangle.getEdgeFreeVertex( edgeIndex1 );
							const SoundVertex* free2 = triangle2.getEdgeFreeVertex( edgeIndex2 );
							
							Vector3f normal1 = triangle.getNormal();
							Vector3f normal2 = triangle2.getNormal();
							
							if ( triangle.getPlane().getSignedDistanceTo( *free2 ) > Real(0) )
								normal1 = -normal1;
							
							if ( triangle2.getPlane().getSignedDistanceTo( *free1 ) > Real(0) )
								normal2 = -normal2;
							
							Real nDotN = math::dot( triangle.getNormal(), triangle2.getNormal() );
							
							// Check the angle between the triangle normals to see if this should be a diffraction edge.
							if ( nDotN < diffractionThreshold )
							{
								Index diffractionEdgeIndex = tempEdges.getSize() + 1;
								
								// Store the index of the edge in the pointer for now since the edge pointer may be reallocated.
								triangle.setDiffractionEdge( edgeIndex1, (const DiffractionEdge*)diffractionEdgeIndex );
								triangle2.setDiffractionEdge( edgeIndex2, (const DiffractionEdge*)diffractionEdgeIndex );
								
								// Add the new diffraction edge to the list of edges.
								tempEdges.add( TempDiffractionEdge( &triangle, edgeIndex1, &triangle2, edgeIndex2 ) );
								
								TempDiffractionEdge& tempEdge = tempEdges.getLast();
								tempEdge.v[0] = vertexIndex;
								tempEdge.v[1] = searchIndex;
								tempEdge.mergedIndex = diffractionEdgeIndex - 1;
							}
						}
					}
				}
			}
		}
	}
	
	//**************************************************************************
	// Build the final list of diffraction edges.
	
	// Create the output lists.
	Shared< ArrayList<DiffractionEdge> > diffractionEdges = Shared<ArrayList<DiffractionEdge> >::construct();
	
	for ( Index i = 0; i < tempEdges.getSize(); i++ )
	{
		TempDiffractionEdge& tempEdge = tempEdges[i];
		
		if ( tempEdge.merged )
			continue;
		
		tempEdge.mergedIndex = diffractionEdges->getSize();
		
		diffractionEdges->add( DiffractionEdge( tempEdge.triangle1, tempEdge.edgeIndex1,
												tempEdge.triangle2, tempEdge.edgeIndex2,
												&vertices[tempEdge.v[0]], &vertices[tempEdge.v[1]],
												tempEdge.plane1, tempEdge.plane2 ) );
	}
	
	//**************************************************************************
	// Finalize the diffraction edge pointers.
	
	for ( Index i = 0; i < triangles.getSize(); i++ )
	{
		InternalSoundTriangle& triangle = triangles[i];
		
		for ( Index e = 0; e < 3; e++ )
		{
			// Set the final diffraction edge pointer.
			const DiffractionEdge* edge = triangle.getDiffractionEdge(e);
			
			if ( edge != NULL )
			{
				Index edgeIndex = tempEdges[(Index)edge - 1].mergedIndex;
				
				triangle.setDiffractionEdge( e, &((*diffractionEdges)[edgeIndex]) );
			}
		}
	}
	
	// Update the timer and the amount of time it took to find the diffraction edges.
	timer.update();
	
	if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
		request.statistics->edgeTime += timer.getLastInterval();
	
	//**************************************************************************
	// Find the visible edge neighbors for each diffraction edge.
	
	if ( request.flags.isSet( MeshFlags::DIFFRACTION_GRAPH ) )
	{
		// Check the visibility for each edge.
		const Size numDiffractionEdges = diffractionEdges->getSize();
		
		Size edgesPerThread = (Size)math::ceiling( Float(numDiffractionEdges) / Float(request.numThreads) );
		Index startIndex = 0;
		
		if ( request.numThreads > 1 )
		{
			for ( Index i = 0; i < request.numThreads && startIndex < numDiffractionEdges; i++ )
			{
				const Size numThreadEdges = math::min( edgesPerThread, numDiffractionEdges - startIndex );
				
				threadPool.addJob( FunctionCall< void ( ArrayList<DiffractionEdge>&, Index, Size, const BVH&, const MeshRequest&, ThreadData& )>(
											bind( &SoundMeshPreprocessor::testEdgeVisibility, this ),
											*diffractionEdges, startIndex, numThreadEdges, bvh, request, threadDataList[i] ) );
				
				startIndex += edgesPerThread;
			}
			
			threadPool.finishJobs();
		}
		else
			testEdgeVisibility( *diffractionEdges, 0, numDiffractionEdges, bvh, request, threadDataList[0] );
		
		//**************************************************************************
		// Determine the total size of the edge neighbor list.
		
		Size numEdgeNeighbors = 0;
		
		for ( Index i = 0; i < request.numThreads; i++ )
			numEdgeNeighbors += threadDataList[i].edgeNeighbors.getSize();
		
		ArrayList<UInt32> edgeNeighbors( numEdgeNeighbors );
		
		//**************************************************************************
		// Combine the output edge neighbor lists for each thread.
		
		startIndex = 0;
		Index neighborOffset = 0;
		
		for ( Index i = 0; i < request.numThreads && startIndex < numDiffractionEdges; i++ )
		{
			const Size numThreadEdges = math::min( edgesPerThread, numDiffractionEdges - startIndex );
			const Index endIndex = startIndex + numThreadEdges;
			ThreadData& threadData = threadDataList[i];
			ArrayList<UInt32>& threadEdgeNeighbors = threadData.edgeNeighbors;
			
			for ( Index e = startIndex; e < endIndex; e++ )
			{
				DiffractionEdge& edge = (*diffractionEdges)[e];
				const Index neighborsStart = edge.neighborListOffset;
				edge.neighborListOffset = (UInt32)edgeNeighbors.getSize();
				
				edgeNeighbors.addAll( &threadEdgeNeighbors[neighborsStart], edge.numNeighbors );
			}
			
			neighborOffset += threadEdgeNeighbors.getSize();
			startIndex += edgesPerThread;
		}
		
		//**************************************************************************
		
		timer.update();
		
		if ( request.statistics && request.flags.isSet( MeshFlags::STATISTICS ) )
			request.statistics->edgeVisibilityTime += timer.getLastInterval();
		
		return Shared<internal::DiffractionGraph>::construct( diffractionEdges, edgeNeighbors );
	}
	
	return Shared<internal::DiffractionGraph>::construct( diffractionEdges );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Visibility Method
//############		
//##########################################################################################
//##########################################################################################




void SoundMeshPreprocessor:: testEdgeVisibility( ArrayList<DiffractionEdge>& edges, Index startIndex, Size numEdges,
												const BVH& bvh, const MeshRequest& request, ThreadData& threadData )
{
	const Real edgeResolution = request.edgeResolution;
	const Size minRaysPerEdge = request.minRaysPerEdge;
	const Size maxRaysPerEdge = request.maxRaysPerEdge;
	const Real edgeOffset = request.edgeOffset;
	const Real rayDirectionThreshold = Real(0.001);
	const Size endIndex = startIndex + numEdges;
	const Size totalNumEdges = edges.getSize();
	ArrayList<UInt32>& edgeNeighbors = threadData.edgeNeighbors;
	
	for ( Index e = startIndex; e < endIndex; e++ )
	{
		DiffractionEdge& edge1 = edges[e];
		
		// Compute the number of rays to trace for the first edge, based on its length.
		const Size edge1Rays = math::clamp( Size(math::ceiling(edge1.getLength() / edgeResolution)),
											minRaysPerEdge, maxRaysPerEdge );
		
		// Compute the bias vector which determines the offset from the edge when determining visibility.
		// This helps deal with numerical issues.
		Vector3f edge1Offset = edge1.getNormal()*edgeOffset;
		Vector3f edge1Extent = edge1.getExtent();
		
		// Set the start index of this edge's adjacency list.
		edge1.neighborListOffset = (UInt32)edgeNeighbors.getSize();
		
		for ( Index e2 = 0; e2 < totalNumEdges; e2++ )
		{
			if ( e2 == e )
				continue;
			
			const DiffractionEdge& edge2 = edges[e2];
			
			// Make sure that the edges lie in each other's shadow regions.
			if ( !testEdgeOrientation( edge1, edge2 ) )
				continue;
			
			// Compute the number of rays to trace for the second edge, based on its length.
			const Size edge2Rays = math::clamp( Size(math::ceiling(edge2.getLength() / edgeResolution)),
												minRaysPerEdge, maxRaysPerEdge );
			
			// Compute the bias vector which determines the offset from the edge when determining visibility.
			// This helps deal with numerical issues.
			Vector3f edge2Offset = edge2.getNormal()*edgeOffset;
			Vector3f edge2Extent = edge2.getExtent();
			
			// Trace at most NxM rays to validate the visiblity of the edge pair.
			// If a ray is found to have visibility, skip all further rays.
			// Only do all NxM rays if there is no visibility between the edges.
			Bool visible = false;
			
			for ( Index i = 0; i < edge1Rays; i++ )
			{
				// Compute the point on the first edge.
				Vector3f p1 = edge1.getStart() + edge1Extent*(Real(i + 1) / (edge1Rays + 1));
				p1 += edge1Offset;
				
				// If the point is not in the diffraction region of the second edge, skip it.
				if ( !edge2.testOrientation( p1, Real(0.001) ) )
					continue;
				
				for ( Index j = 0; j < edge2Rays; j++ )
				{
					// Compute the point on the second edge.
					Vector3f p2 = edge2.getStart() + edge2Extent*(Real(j + 1) / (edge2Rays + 1));
					p2 += edge2Offset;
					
					// If the point is not in the diffraction region of the first edge, skip it.
					if ( !edge1.testOrientation( p2, edgeOffset ) )
						continue;
					
					// Compute the ray and distance along the ray between the points.
					Real distance = (p2 - p1).getMagnitude();
					
					// Skip this ray if the points are coincident.
					if ( distance < math::epsilon<Real>() )
						continue;
					
					Ray3f ray( p1, (p2 - p1) / distance );
					
					// Skip a ray if it is in the opposite direction of both triangle normals.
					// This means that the candidate edge point is outside of the first edge's diffraction region.
					if ( math::dot( ray.direction, edge1.plane1.normal ) < -rayDirectionThreshold &&
						math::dot( ray.direction, edge1.plane2.normal ) < -rayDirectionThreshold )
						continue;
					
					// Trace the ray. If it doesn't hit anything, the edges are mutually visible.
					BVHRay bvhRay( ray, 0.0f, distance - 2*edgeOffset );
					bvh.intersectRay( bvhRay );
					
					if ( !bvhRay.hitValid() )
					{
						visible = true;
						break;
					}
				}
			}
			
			// If the edge is visible, add it to the adjaceny list.
			if ( visible )
				edgeNeighbors.add( (UInt32)e2 );
		}
		
		// Set the number of neighbors that were found for this edge.
		edge1.numNeighbors = UInt32(edgeNeighbors.getSize() - edge1.neighborListOffset);
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Edge Orientation Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMeshPreprocessor:: testEdgeOrientation( const internal::DiffractionEdge& edge1, const internal::DiffractionEdge& edge2 )
{
	Float offset = 0.0001f;
	
	// Test edge 1's endpoints versus edge 2's diffraction regions.
	Real e11Distance1 = edge2.plane1.getSignedDistanceTo( *edge1.v1 );
	Real e11Distance2 = edge2.plane2.getSignedDistanceTo( *edge1.v1 );
	Real e12Distance1 = edge2.plane1.getSignedDistanceTo( *edge1.v2 );
	Real e12Distance2 = edge2.plane2.getSignedDistanceTo( *edge1.v2 );
	
	Bool e11Front1 = e11Distance1 > offset;
	Bool e11Front2 = e11Distance2 > offset;
	Bool e12Front1 = e12Distance1 > offset;
	Bool e12Front2 = e12Distance2 > offset;
	
	Bool e11Behind1 = e11Distance1 < -offset;
	Bool e11Behind2 = e11Distance2 < -offset;
	Bool e12Behind1 = e12Distance1 < -offset;
	Bool e12Behind2 = e12Distance2 < -offset;
	
	/// Edge is outside of diffraction regions if it has both endpoints in front of both planes
	// or both endpoints behind both planes.
	Bool e1Outside = (e11Front1 & e11Front2 & e12Front1 & e12Front2) ||
					(e11Behind1 & e11Behind2 & e12Behind1 & e12Behind2);
	
	if ( e1Outside )
		return false;
	
	Real e21Distance1 = edge1.plane1.getSignedDistanceTo( *edge2.v1 );
	Real e21Distance2 = edge1.plane2.getSignedDistanceTo( *edge2.v1 );
	Real e22Distance1 = edge1.plane1.getSignedDistanceTo( *edge2.v2 );
	Real e22Distance2 = edge1.plane2.getSignedDistanceTo( *edge2.v2 );
	
	Bool e21Front1 = e21Distance1 > offset;
	Bool e21Front2 = e21Distance2 > offset;
	Bool e22Front1 = e22Distance1 > offset;
	Bool e22Front2 = e22Distance2 > offset;
	
	Bool e21Behind1 = e21Distance1 < -offset;
	Bool e21Behind2 = e21Distance2 < -offset;
	Bool e22Behind1 = e22Distance1 < -offset;
	Bool e22Behind2 = e22Distance2 < -offset;
	
	/// Edge is outside of diffraction regions if it has both endpoints in front of both planes
	// or both endpoints behind both planes.
	Bool e2Outside = (e21Front1 & e21Front2 & e22Front1 & e22Front2) ||
					(e21Behind1 & e21Behind2 & e22Behind1 & e22Behind2);
	
	if ( e2Outside )
		return false;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Marching Cubes Edge Table
//############		
//##########################################################################################
//##########################################################################################




const UInt16 SoundMeshPreprocessor:: edgeTable[256] =
{
	0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   
};




//##########################################################################################
//##########################################################################################
//############		
//############		Marching Cubes Triangle Table
//############		
//##########################################################################################
//##########################################################################################




const Int8 SoundMeshPreprocessor:: triangleTable[256][16] =
{
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
	{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
	{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
	{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
	{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
	{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
	{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
	{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
	{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
	{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
	{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
	{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
	{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
	{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
	{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
	{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
	{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
	{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
	{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
	{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
	{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
	{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
	{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
	{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
	{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
	{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
	{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
	{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
	{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
	{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
	{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
	{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
	{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
	{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
	{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
	{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

