/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com/om
 * Author(s):   Carl Schissler
 * 
 * Copyright (c) 2016, Carl Schissler
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright
 * 	   notice, this list of conditions and the following disclaimer.
 * 	2. Redistributions in binary form must reproduce the above copyright
 * 	   notice, this list of conditions and the following disclaimer in the
 * 	   documentation and/or other materials provided with the distribution.
 * 	3. Neither the name of the copyright holder nor the
 * 	   names of its contributors may be used to endorse or promote products
 * 	   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "omAABBTree4.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Fat SIMD Ray Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class OM_ALIGN(16) AABBTree4:: TraversalRay
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			OM_INLINE TraversalRay( const BVHRay& ray )
				:	origin( ray.origin ),
					direction( ray.direction ),
					inverseDirection( math::reciprocal( ray.direction ) )
			{
				signMin[0] = sizeof(SIMDFloat4)*(ray.direction[0] < Float32(0) ? 1 : 0);
				signMin[1] = sizeof(SIMDFloat4)*(ray.direction[1] < Float32(0) ? 3 : 2);
				signMin[2] = sizeof(SIMDFloat4)*(ray.direction[2] < Float32(0) ? 5 : 4);
				signMax[0] = sizeof(SIMDFloat4) ^ signMin[0];
				signMax[1] = sizeof(SIMDFloat4) ^ signMin[1];
				signMax[2] = sizeof(SIMDFloat4) ^ signMin[2];
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			SIMDVector3f origin;
			
			
			/// The direction vector of this SIMD Ray.
			SIMDVector3f direction;
			
			
			/// The inverse of the direction vector of this SIMD Ray.
			SIMDVector3f inverseDirection;
			
			
			/// Byte offsets into the bounding box node's bounding box array for each axis, as determined by the ray direction signs.
			Index signMin[3];
			Index signMax[3];
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Node Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class OM_ALIGN(128) AABBTree4:: Node
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an uninitialized node.
			OM_FORCE_INLINE Node()
			{
			}
			
			
			/// Create a new leaf node for the specified primitive offset and primitive count.
			OM_FORCE_INLINE Node( const Node& other )
			{
				bounds[0] = other.bounds[0];
				bounds[1] = other.bounds[1];
				bounds[2] = other.bounds[2];
				bounds[3] = other.bounds[3];
				bounds[4] = other.bounds[4];
				bounds[5] = other.bounds[5];
				
				for ( Index i = 0; i < 4; i++ )
				{
					if ( isLeaf( other.child[i] ) )
						child[i] = other.child[i];
					else
						child[i].node = this + (other.child[i].node - &other);
				}
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			OM_FORCE_INLINE Node& operator = ( const Node& other )
			{
				bounds[0] = other.bounds[0];
				bounds[1] = other.bounds[1];
				bounds[2] = other.bounds[2];
				bounds[3] = other.bounds[3];
				bounds[4] = other.bounds[4];
				bounds[5] = other.bounds[5];
				
				for ( Index i = 0; i < 4; i++ )
				{
					if ( isLeaf( other.child[i] ) )
						child[i] = other.child[i];
					else
						child[i].node = this + (other.child[i].node - &other);
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Public Static Methods
			
			
			/// Return whether or not the specified node pointer actually refers to a leaf node.
			OM_FORCE_INLINE static Bool isLeaf( const Child& child )
			{
				return (reinterpret_cast<PointerInt>(child.node) & 0x1) != 0;
			}
			
			
			/// Return the number of primitives that are part of the child's leaf node.
			OM_FORCE_INLINE static UInt32 getLeafCount( const Child& child )
			{
#if defined(OM_BIG_ENDIAN) && defined(OM_PLATFORM_64_BIT)
				// Return count unchanged since the flag is in the offset.
				return child.leaf.count;
#else
				// Shift the count to erase the leaf flag bit.
				return child.leaf.count >> 1;
#endif
			}
			
			
			/// Return the primitive array offset for the child's leaf node.
			OM_FORCE_INLINE static UInt32 getLeafOffset( const Child& child )
			{
#if defined(OM_BIG_ENDIAN) && defined(OM_PLATFORM_64_BIT)
				return child.leaf.offset >> 1;
#else
				// Return offset unchanged since the flag is in the count.
				return child.leaf.offset;
#endif
			}
			
			
		//********************************************************************************
		//******	Child Accessor Methods
			
			
			/// Return the child value for this node at the given index.
			OM_FORCE_INLINE Child& getChild( Index i )
			{
				return child[i];
			}
			
			
			/// Return the child value for this node at the given index.
			OM_FORCE_INLINE const Child& getChild( Index i ) const
			{
				return child[i];
			}
			
			
			/// Set the relative offset of the child at the given index from this node.
			OM_FORCE_INLINE void setChild( Index i, Index offset )
			{
				child[i].node = this + offset;
			}
			
			
			/// Set the relative offset of the child at the given index from this node.
			OM_FORCE_INLINE void setLeaf( Index i, Index count, Index offset )
			{
				// Set the count for the leaf, setting the leaf flag if necessary.
#if defined(OM_BIG_ENDIAN) && defined(OM_PLATFORM_64_BIT)
				child[i].leaf.count = (UInt32)count;
#else
				child[i].leaf.count = (UInt32)((count << 1) | 0x1);
#endif
				
				// Set the offset for the leaf, setting the leaf flag if necessary.
#if defined(OM_BIG_ENDIAN) && defined(OM_PLATFORM_64_BIT)
				child[i].leaf.offset = (UInt32)((offset << 1) | 0x1);
#else
				child[i].leaf.offset = (UInt32)offset;
#endif
			}
			
			
			/// Set the relative offset of the child at the given index from this node.
			OM_FORCE_INLINE static void setLeaf( Child& child, Index count, Index offset )
			{
				// Set the count for the leaf, setting the leaf flag if necessary.
#if defined(OM_BIG_ENDIAN) && defined(OM_PLATFORM_64_BIT)
				child.leaf.count = (UInt32)count;
#else
				child.leaf.count = (UInt32)((count << 1) | 0x1);
#endif
				
				// Set the offset for the leaf, setting the leaf flag if necessary.
#if defined(OM_BIG_ENDIAN) && defined(OM_PLATFORM_64_BIT)
				child.leaf.offset = (UInt32)((offset << 1) | 0x1);
#else
				child.leaf.offset = (UInt32)offset;
#endif
			}
			
			
			OM_FORCE_INLINE void setChildAABB( Index i, const AABB3f& newAABB )
			{
				bounds[0][i] = newAABB.min.x;
				bounds[1][i] = newAABB.max.x;
				bounds[2][i] = newAABB.min.y;
				bounds[3][i] = newAABB.max.y;
				bounds[4][i] = newAABB.min.z;
				bounds[5][i] = newAABB.max.z;
			}
			
			
			/// Compute and return the bounding box of this node's children.
			OM_FORCE_INLINE AABB3f getAABB() const
			{
				AABB3f result( bounds[0][0], bounds[1][0], bounds[2][0], bounds[3][0], bounds[4][0], bounds[5][0] );
				
				for ( Index i = 1; i < 4; i++ )
					result |= AABB3f( bounds[0][i], bounds[1][i], bounds[2][i], bounds[3][i], bounds[4][i], bounds[5][i] );
				
				return result;
			}
			
			
		//********************************************************************************
		//******	Ray Intersection Methods
			
			
			OM_FORCE_INLINE SIMDInt4 intersectRay( const TraversalRay& ray, const SIMDFloat4& tMin, const SIMDFloat4& tMax, SIMDFloat4& near ) const
			{
				SIMDFloat4 txmin = (SIMDFloat4::load((const Float32*)((const UByte*)bounds + ray.signMin[0])) - ray.origin.x) * ray.inverseDirection.x;
				SIMDFloat4 txmax = (SIMDFloat4::load((const Float32*)((const UByte*)bounds + ray.signMax[0])) - ray.origin.x) * ray.inverseDirection.x;
				SIMDFloat4 tymin = (SIMDFloat4::load((const Float32*)((const UByte*)bounds + ray.signMin[1])) - ray.origin.y) * ray.inverseDirection.y;
				SIMDFloat4 tymax = (SIMDFloat4::load((const Float32*)((const UByte*)bounds + ray.signMax[1])) - ray.origin.y) * ray.inverseDirection.y;
				SIMDFloat4 tzmin = (SIMDFloat4::load((const Float32*)((const UByte*)bounds + ray.signMin[2])) - ray.origin.z) * ray.inverseDirection.z;
				SIMDFloat4 tzmax = (SIMDFloat4::load((const Float32*)((const UByte*)bounds + ray.signMax[2])) - ray.origin.z) * ray.inverseDirection.z;
				
				near = math::max( math::max( txmin, tymin ), math::max( tzmin, tMin ) );
				SIMDFloat4 far = math::min( math::min( math::min( txmax, tymax ), tzmax ), tMax );
				
				return near <= far;
			}
			
			
		//********************************************************************************
		//******	Tree Refitting Methods
			
			
			/// Refit the bounding volume for the specified node and return the final bounding box.
			static AABB3f refitTreeGeneric( const Child& node );
			
			
			/// Refit the bounding volume for the specified node and return the final bounding box.
			static AABB3f refitTreeTriangles( const Child& node );
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A set of 4 SIMD axis-aligned bounding boxes for this quad node.
			/**
			  * The bounding boxes are stored in the following format:
			  *	- 0: xMin
			  * - 1: xMax
			  * - 2: yMin
			  * - 3: yMax
			  * - 4: zMin
			  * - 5: zMax
			  */
			SIMDFloat4 bounds[6];
			
			
			/// A array of unions for each 64-bit child node pointer/leaf node.
			Child child[4];
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Primitive AABB Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class OM_ALIGN(16) AABBTree4:: PrimitiveAABB
{
	public:
		
		OM_FORCE_INLINE PrimitiveAABB( const AABB3f& aabb )
			:	min( aabb.min ),
				max( aabb.max )
		{
			centroid = (min + max)*Float(0.5);
		}
		
		
		/// The minimum coordinate of the primitive's axis-aligned bounding box.
		SIMDFloat4 min;
		
		
		/// The maximum coordinate of the primitive's axis-aligned bounding box.
		SIMDFloat4 max;
		
		
		/// The centroid of the primitive's axis-aligned bounding box.
		SIMDFloat4 centroid;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Split Bin Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class OM_ALIGN(16) AABBTree4:: SplitBin
{
	public:
		
		OM_INLINE SplitBin()
			:	min( math::max<Float32>() ),
				max( math::min<Float32>() ),
				numPrimitives( 0 )
		{
		}
		
		/// The minimum of this split bin's bounding box.
		SIMDFloat4 min;
		
		/// The maximum of this split bin's bounding box.
		SIMDFloat4 max;
		
		/// The number of primitives that were assigned to this split bin.
		PrimitiveCount numPrimitives;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Cached Triangle Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class OM_ALIGN(16) AABBTree4:: CachedTriangle
{
	public:
		
		OM_INLINE CachedTriangle( const SIMDVector3f& newV0,
									const SIMDVector3f& newE1,
									const SIMDVector3f& newE2,
									const StaticArray<IndexType,4>& newIndices  )
			:	v0( newV0 ),
				e1( newE1 ),
				e2( newE2 )
		{
			indices[0] = newIndices[0];
			indices[1] = newIndices[1];
			indices[2] = newIndices[2];
			indices[3] = newIndices[3];
		}
		
		/// The vertex of this triangle with index 0.
		SIMDVector3f v0;
		
		/// The edge vector between vertex 0 and vertex 1.
		SIMDVector3f e1;
		
		/// The edge vector between vertex 0 and vertex 2.
		SIMDVector3f e2;
		
		/// The indices of the 4 packed triangles.
		PrimitiveIndex indices[4];
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




AABBTree4:: AABBTree4()
	:	nodes( NULL ),
		numNodes( 0 ),
		numPrimitives( 0 ),
		primitiveIndices( NULL ),
		primitiveIndexCapacity( 0 ),
		primitiveData( NULL ),
		primitiveDataCapacity( 0 ),
		geometry(),
		cachedPrimitiveType( BVHGeometry::UNDEFINED ),
		maxDepth( 0 ),
		maxNumPrimitivesPerLeaf( DEFAULT_MAX_PRIMITIVES_PER_LEAF ),
		numSplitCandidates( DEFAULT_NUM_SPLIT_CANDIDATES )
{
}




AABBTree4:: AABBTree4( const AABBTree4& other )
	:	nodes( NULL ),
		numNodes( other.numNodes ),
		numPrimitives( other.numPrimitives ),
		primitiveIndices( NULL ),
		primitiveIndexCapacity( 0 ),
		primitiveData( NULL ),
		primitiveDataCapacity( 0 ),
		geometry( other.geometry ),
		cachedPrimitiveType( other.cachedPrimitiveType ),
		maxDepth( other.maxDepth ),
		maxNumPrimitivesPerLeaf( other.maxNumPrimitivesPerLeaf ),
		numSplitCandidates( other.numSplitCandidates )
{
	if ( numNodes > 0 )
		nodes = util::copyArrayAligned( other.nodes, other.numNodes, sizeof(Node) );
	
	if ( numPrimitives > 0 )
	{
		primitiveData = other.copyPrimitiveData( primitiveDataCapacity );
		primitiveIndices = util::allocate<IndexType>( numPrimitives );
		util::copy( primitiveIndices, other.primitiveIndices, numPrimitives );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




AABBTree4:: ~AABBTree4()
{
	if ( nodes )
		util::deallocateAligned( nodes );
	
	if ( primitiveData )
		util::deallocateAligned( primitiveData );
	
	if ( primitiveIndices )
		util::deallocate( primitiveIndices );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




AABBTree4& AABBTree4:: operator = ( const AABBTree4& other )
{
	if ( this != &other )
	{
		if ( numNodes < other.numNodes )
		{
			if ( nodes )
				util::deallocateAligned( nodes );
			
			nodes = util::copyArrayAligned( other.nodes, other.numNodes, sizeof(Node) );
		}
		else if ( other.numNodes > 0 )
			util::copy( nodes, other.nodes, other.numNodes );
		
		if ( primitiveData )
			util::deallocateAligned( primitiveData );
		
		if ( primitiveIndices )
			util::deallocate( primitiveIndices );
		
		if ( other.numPrimitives > 0 )
		{
			primitiveData = other.copyPrimitiveData( primitiveDataCapacity );
			
			if ( other.numPrimitives > primitiveIndexCapacity )
			{
				util::deallocate( primitiveIndices );
				primitiveIndices = util::allocate<IndexType>( numPrimitives );
			}
			
			util::copy( primitiveIndices, other.primitiveIndices, numPrimitives );
		}
		
		geometry = other.geometry;
		numPrimitives = other.numPrimitives;
		numNodes = other.numNodes;
		maxDepth = other.maxDepth;
		maxNumPrimitivesPerLeaf = other.maxNumPrimitivesPerLeaf;
		numSplitCandidates = other.numSplitCandidates;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Geometry Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




BVHGeometry* AABBTree4:: getGeometry() const
{
	return geometry;
}




Bool AABBTree4:: setGeometry( BVHGeometry* newGeometry )
{
	geometry = newGeometry;
	
	// Set the number of nodes and primitives to 0 to signal that the BVH needs to be rebuilt.
	numNodes = 0;
	numPrimitives = 0;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		BVH Building Methods
//############		
//##########################################################################################
//##########################################################################################




void AABBTree4:: rebuild()
{
	maxDepth = 0;
	
	if ( geometry == NULL )
		return;
	
	// Update the primitive set.
	geometry->update();
	
	// Get the number of primitives that are now in the BVH.
	const PrimitiveCount newNumPrimitives = geometry->getPrimitiveCount();
	
	// Don't build the tree if there are no primitives.
	if ( newNumPrimitives == 0 )
		return;
	
	//**************************************************************************************
	
	// Make sure the array of client primitive indices is big enough.
	if ( newNumPrimitives >= primitiveIndexCapacity )
	{
		if ( primitiveIndices != NULL )
			util::deallocate( primitiveIndices );
		
		primitiveIndices = util::allocate<PrimitiveIndex>( newNumPrimitives );
		primitiveIndexCapacity = newNumPrimitives;
	}
	
	// Initialize the primitives indices to start with client indices.
	for ( PrimitiveIndex i = 0; i < newNumPrimitives; i++ )
		primitiveIndices[i] = i;
	
	// Allocate a temporary array to hold the list of PrimitiveAABB objects.
	PrimitiveAABB* primitiveAABBs = util::allocateAligned<PrimitiveAABB>( newNumPrimitives, 16 );
	
	// Initialize all PrimitiveAABB objects with the primitives for this tree.
	for ( PrimitiveIndex i = 0; i < newNumPrimitives; i++ )
		new (primitiveAABBs + i) PrimitiveAABB( geometry->getPrimitiveAABB(i) );
	
	//**************************************************************************************
	
	const Size numSplitBins = numSplitCandidates + 1;
	
	// Allocate a temporary array to hold the split bins.
	SplitBin* splitBins = util::allocateAligned<SplitBin>( numSplitBins, 16 );
	
	//**************************************************************************************
	
	// Compute the maximum number of nodes needed for this tree (2*n - 1).
	const Size newNumNodes = math::max( Size(2)*newNumPrimitives - 1, Size(5) );
	
	// Allocate space for the nodes in this tree.
	if ( newNumNodes > numNodes )
	{
		if ( nodes )
			util::deallocateAligned( nodes );
		
		nodes = util::allocateAligned<Node>( newNumNodes, sizeof(Node) );
		numNodes = newNumNodes;
	}
	
	// Build the tree, starting with the root node, returning the actual number of nodes needed.
	const Size finalNumNodes = buildTreeRecursive( nodes, primitiveAABBs, primitiveIndices, 0, newNumPrimitives,
													splitBins, numSplitBins, maxNumPrimitivesPerLeaf, 2, maxDepth );
	
	// Reallocate the node memory to a smaller buffer to save memory.
	if ( finalNumNodes < numNodes )
	{
		Node* oldNodes = nodes;
		nodes = util::allocateAligned<Node>( finalNumNodes, sizeof(Node) );
		
		util::copy( nodes, oldNodes, finalNumNodes );
		
		util::deallocateAligned( oldNodes );
		numNodes = finalNumNodes;
	}
	
	//**************************************************************************************
	
	// Determine if the BVH should cache the primitives based on their type.
	numPrimitives = newNumPrimitives;
	Size newPrimitiveDataSize = 0;
	
	switch ( geometry->getPrimitiveType() )
	{
		case BVHGeometry::TRIANGLES:
			newPrimitiveDataSize = getTriangleArraySize()*sizeof(CachedTriangle);
			break;
	}
	
	// Allocate an array to hold the primitive data.
	if ( newPrimitiveDataSize > primitiveDataCapacity )
	{
		if ( primitiveData )
			util::deallocateAligned( primitiveData );
		
		primitiveData = util::allocateAligned<UByte>( newPrimitiveDataSize, 16 );
		primitiveDataCapacity = newPrimitiveDataSize;
	}
	
	// Copy the current order of the TriangleAABB list into the tree's list of primitive pointers.
	switch ( geometry->getPrimitiveType() )
	{
		case BVHGeometry::TRIANGLES:
		{
			Child root;
			root.node = nodes;
			fillTriangleArray( (CachedTriangle*)primitiveData, geometry, root, 0 );
			cachedPrimitiveType = BVHGeometry::TRIANGLES;
			break;
		}
			
		default:
			cachedPrimitiveType = BVHGeometry::UNDEFINED;
			break;
	}
	
	//**************************************************************************************
	// Clean up the temporary arrays of TriangleAABB primitives and split bins.
	
	util::deallocateAligned( primitiveAABBs );
	util::deallocateAligned( splitBins );
}




void AABBTree4:: refit()
{
	if ( numNodes == 0 )
		return;
	
	// If the number or type of primitives has changed, rebuild the tree instead.
	if ( numPrimitives != geometry->getPrimitiveCount() || cachedPrimitiveType != geometry->getPrimitiveType() )
	{
		this->rebuild();
		return;
	}
	
	// Refit the tree for different kinds of primitives.
	Child root;
	root.node = nodes;
	
	switch ( cachedPrimitiveType )
	{
		case BVHGeometry::TRIANGLES:
			this->refitTreeTriangles( root );
			break;
		default:
			this->refitTreeGeneric( root );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray Tracing Methods
//############		
//##########################################################################################
//##########################################################################################




void AABBTree4:: intersectRay( BVHRay& ray ) const
{
	if ( numNodes == 0 )
		return;
	
	if ( cachedPrimitiveType == BVHGeometry::TRIANGLES )
		traceRayVsTriangles( ray );
	else
		traceRayVsGeneric( ray );
}




void AABBTree4:: testRay( BVHRay& ray ) const
{
	return intersectRay( ray );
}




//##########################################################################################
//##########################################################################################
//############		
//############		BVH Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################





Bool AABBTree4:: isValid() const
{
	return numNodes > 0;
}




Size AABBTree4:: getSizeInBytes() const
{
	Size totalSize = sizeof(AABBTree4);
	
	totalSize += numNodes*sizeof(Node);
	totalSize += primitiveDataCapacity;
	totalSize += primitiveIndexCapacity*sizeof(Index);
	
	return totalSize;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Bounding Volume Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




AABB3f AABBTree4:: getAABB() const
{
	if ( numNodes == 0 )
		return AABB3f( math::infinity<Float>(), math::negativeInfinity<Float>() );
	else
		return nodes->getAABB();
}




Sphere3f AABBTree4:: getBoundingSphere() const
{
	if ( numNodes == 0 )
		return Sphere3f( Vector3f(), math::infinity<Float>() );
	else
	{
		AABB3f bbox = nodes->getAABB();
		return Sphere3f( bbox.getCenter(), Float(0.5)*bbox.getDiagonal().getMagnitude() );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Generic Ray Tracing Method
//############		
//##########################################################################################
//##########################################################################################




void AABBTree4:: traceRayVsGeneric( BVHRay& rayData ) const
{
	Child traversalStack[TRAVERSAL_STACK_SIZE];
	Child* const stackBase = traversalStack;
	Child* stack = stackBase + 1;
	Child node;
	node.node = nodes;
	*stack = node;
	
	const BVHGeometry* const geo = geometry;
	const PrimitiveIndex* const indices = primitiveIndices;
	TraversalRay ray( rayData );
	const SIMDFloat4 tMin = rayData.tMin;
	SIMDFloat4 tMax = rayData.tMax;
	
	while ( true )
	{
		nextNode:
		
		if ( Node::isLeaf( node ) )
		{
			geo->intersectRay( indices + Node::getLeafOffset( node ),
								Node::getLeafCount( node ), rayData );
			tMax = rayData.tMax;
		}
		else
		{
			if ( traceRayVsNode( ray, tMin, tMax, node, stack ) )
				goto nextNode;
		}
		
		node = *stack;
		stack--;
		
		if ( stack == stackBase )
			break;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle Ray Tracing Method
//############		
//##########################################################################################
//##########################################################################################




void AABBTree4:: traceRayVsTriangles( BVHRay& rayData ) const
{
	Child traversalStack[TRAVERSAL_STACK_SIZE];
	Child* const stackBase = traversalStack;
	Child* stack = stackBase + 1;
	Child node;
	node.node = nodes;
	*stack = node;
	
	const CachedTriangle* const triangles = (const CachedTriangle*)primitiveData;
	TraversalRay ray( rayData );
	const Float tMaxInput = rayData.tMax;
	const SIMDFloat4 tMin = rayData.tMin;
	SIMDFloat4 tMax = rayData.tMax;
	SIMDFloat4 triangleDistance;
	
	while ( true )
	{
		nextNode:
		
		if ( Node::isLeaf( node ) )
		{
			const UInt32 numNodePrimitives = Node::getLeafCount( node );
			
			if ( numNodePrimitives == 1 )
			{
				// Fast case for a single quad triangle.
				const CachedTriangle* triangle = triangles + Node::getLeafOffset( node );
				
				// Find the intersections and update the ray data.
				rayIntersectsTriangles( ray, rayData, tMin, tMax, *triangle );
			}
			else
			{
				// General case for many triangles.
				const CachedTriangle* triangle = triangles + Node::getLeafOffset( node );
				const CachedTriangle* const trianglesEnd = triangle + numNodePrimitives;
				
				while ( triangle != trianglesEnd )
				{
					// Find the intersections and update the ray data.
					rayIntersectsTriangles( ray, rayData, tMin, tMax, *triangle );
					
					triangle++;
				}
			}
		}
		else
		{
			if ( traceRayVsNode( ray, tMin, tMax, node, stack ) )
				goto nextNode;
		}
		
		node = *stack;
		stack--;
		
		if ( stack == stackBase )
			break;
	}
	
	// If the ray hit something closer than the input t-max, set the hit geometry.
	if ( rayData.tMax < tMaxInput )
		rayData.geometry = geometry;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Trace Ray vs Inner Node Method
//############		
//##########################################################################################
//##########################################################################################




OM_FORCE_INLINE static int firstSetBit( int mask )
{
#if defined(OM_COMPILER_GCC)
	return __builtin_ctz( *reinterpret_cast<unsigned int*>( &mask ) );
#elif defined(OM_COMPILER_MSVC)
	unsigned long index;
	_BitScanForward( &index, mask );
	return index;
#else
	#error
#endif
}




OM_FORCE_INLINE static int clearFirstSetBit( int& mask )
{
	int index = firstSetBit( mask );
	mask &= mask - 1;
	return index;
}




Bool AABBTree4:: traceRayVsNode( const TraversalRay& ray, const SIMDFloat4& tMin, const SIMDFloat4& tMax,
								Child& childNode, Child*& stack )
{
	const Node* const node = childNode.node;
	
	// Intersect the ray with the node's children.
	SIMDFloat4 near;
	SIMDInt4 intersectionResult = node->intersectRay( ray, tMin, tMax, near );
	Int mask = intersectionResult.getMask();
	
	//************************************************************************
	
	switch ( mask )
	{
		// No hits. Backtrack on the stack.
		case 0:
			return false;
		
		// 1 Hit. Replace the current node with the hit child.
		case 1 << 0:	childNode = node->getChild(0);	return true;
		case 1 << 1:	childNode = node->getChild(1);	return true;
		case 1 << 2:	childNode = node->getChild(2);	return true;
		case 1 << 3:	childNode = node->getChild(3);	return true;
		
		// 2 Hits.
		case 3: // 0011
		{
			stack++;
			
			if ( near[1] < near[0] )
			{
				*stack = node->getChild(0);
				childNode = node->getChild(1);
			}
			else
			{
				*stack = node->getChild(1);
				childNode = node->getChild(0);
			}
			return true;
		}
		
		case 5: // 0101
		{
			stack++;
			
			if ( near[2] < near[0] )
			{
				*stack = node->getChild(0);
				childNode = node->getChild(2);
			}
			else
			{
				*stack = node->getChild(2);
				childNode = node->getChild(0);
			}
			return true;
		}
		
		case 6: // 0110
		{
			stack++;
			
			if ( near[2] < near[1] )
			{
				*stack = node->getChild(1);
				childNode = node->getChild(2);
			}
			else
			{
				*stack = node->getChild(2);
				childNode = node->getChild(1);
			}
			return true;
		}
		
		case 9: // 1001
		{
			stack++;
			
			if ( near[3] < near[0] )
			{
				*stack = node->getChild(0);
				childNode = node->getChild(3);
			}
			else
			{
				*stack = node->getChild(3);
				childNode = node->getChild(0);
			}
			return true;
		}
		
		case 10: // 1010
		{
			stack++;
			
			if ( near[3] < near[1] )
			{
				*stack = node->getChild(1);
				childNode = node->getChild(3);
			}
			else
			{
				*stack = node->getChild(3);
				childNode = node->getChild(1);
			}
			return true;
		}
		
		case 12: // 1100
		{
			stack++;
			
			if ( near[3] < near[2] )
			{
				*stack = node->getChild(2);
				childNode = node->getChild(3);
			}
			else
			{
				*stack = node->getChild(3);
				childNode = node->getChild(2);
			}
			return true;
		}
		
		
		default:
		{
			// There are more than 2 hit children.
			// Determine the index of the closest hit child.
			Int closestChildIndex = minIndex( math::select( intersectionResult, near, SIMDFloat4(math::infinity<Float>()) ) );
			
			// Clear the bit of the closest hit child.
			mask &= ~(1 << closestChildIndex);
			
			//****************************************************
			// Second hit.
			
			// Get the index of the next hit and clear it from the mask.
			Int i = clearFirstSetBit( mask );
			
			// Put the child onto the stack.
			stack++;
			*stack = node->getChild(i);
			
			//****************************************************
			// Third hit.
			
			// Get the index of the next hit and clear it from the mask.
			i = clearFirstSetBit( mask );
			
			// Put the child onto the stack.
			stack++;
			*stack = node->getChild(i);
			
			//****************************************************
			// Fourth hit, if necessary.
			
			if ( mask )
			{
				// Get the index of the next hit and clear it from the mask.
				i = clearFirstSetBit( mask );
				
				// Put the child onto the stack.
				stack++;
				*stack = node->getChild(i);
			}
			
			// Determine the next node to traverse.
			childNode = node->getChild(closestChildIndex);
			return true;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray Vs. Triangle Intersection Method
//############		
//##########################################################################################
//##########################################################################################




void AABBTree4:: rayIntersectsTriangles( const TraversalRay& ray, BVHRay& rayData, const SIMDFloat4& tMin, SIMDFloat4& tMax,
										const CachedTriangle& triangle )
{
	// the vector perpendicular to edge 2 and the ray's direction
	SIMDVector3f pvec = math::cross( ray.direction, triangle.e2 );
	SIMDFloat4 det = math::dot( triangle.e1, pvec );
	
	// Do the first rejection test for the triangles, test to see if the ray is in the same plane as the triangle.
	SIMDInt4 result = math::abs(det) >= math::epsilon<Float>();
	
	//************************************************************************************
	
	SIMDFloat4 inverseDet = Float(1) / det;
	SIMDVector3f v0ToSource = ray.origin - triangle.v0;
	SIMDFloat4 u = math::dot( v0ToSource, pvec ) * inverseDet;
	
	// Do the second rejection test for the triangles. See if the UV coordinate is within the valid range.
	result &= (u >= Float(0)) & (u <= Float(1));
	
	//************************************************************************************
	
	SIMDVector3f qvec = math::cross( v0ToSource, triangle.e1 );
	SIMDFloat4 v = math::dot( ray.direction, qvec ) * inverseDet;
	
	// Do the third rejection test for the triangles. See if the UV coordinate is within the valid range.
	result &= (v >= Float(0)) & (u + v <= Float(1));
	
	//************************************************************************************
	
	SIMDFloat4 distance = math::dot( triangle.e2, qvec ) * inverseDet;
	
	// Make sure that the triangles are hit by the forward side of the ray.
	result &= (distance > tMin) & (distance < tMax);
	
	//************************************************************************************
	
	// Find the closest intersection index if there was an intersection.
	if ( result.getMask() )
	{
		// Find the closest valid intersection.
		distance = math::select( result, distance, SIMDFloat4(math::infinity<Float>()) );
		Int minTIndex = minIndex( distance, tMax );
		
		// Update the ray data.
		rayData.tMax = tMax[0];
		rayData.bary0 = u[minTIndex];
		rayData.bary1 = v[minTIndex];
		rayData.primitive = triangle.indices[minTIndex];
		rayData.normal = math::cross( Vector3f( triangle.e1.x[minTIndex], triangle.e1.y[minTIndex], triangle.e1.z[minTIndex] ),
										Vector3f( triangle.e2.x[minTIndex], triangle.e2.y[minTIndex], triangle.e2.z[minTIndex] ) );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Recursive Tree Construction Method
//############		
//##########################################################################################
//##########################################################################################




Size AABBTree4:: buildTreeRecursive( Node* node, const PrimitiveAABB* primitiveAABBs,
									PrimitiveIndex* primitiveIndices, PrimitiveIndex start, PrimitiveCount numPrimitives,
									SplitBin* splitBins, Size numSplitBins, 
									Size maxNumPrimitivesPerLeaf, Size depth, Size& maxDepth )
{
	// The split axis used for each split (0 = X, 1 = Y, 2 = Z).
	StaticArray<Index,3> splitAxis;
	
	// The number of primitives in a child node (leaf or not).
	StaticArray<PrimitiveCount,4> numChildPrimitives;
	
	// The 4 volumes of the child nodes.
	StaticArray<AABB3f,4> volumes;
	
	//***************************************************************************
	// Partition the set of primitives into two sets.
	
	PrimitiveIndex* const primitiveIndicesStart = primitiveIndices + start;
	PrimitiveCount numLesserPrimitives = 0;
	
	partitionPrimitivesSAH( primitiveAABBs, primitiveIndicesStart, numPrimitives,
							splitBins, numSplitBins, splitAxis[0], numLesserPrimitives,
							volumes[0], volumes[2] );
	
	// Compute the number of primitives greater than the split plane along the split axis.
	PrimitiveCount numGreaterPrimitives = numPrimitives - numLesserPrimitives;
	
	//***************************************************************************
	// Partition the primitive subsets into four sets based on the next two splitting planes.
	
	// If the number of primitives on this side of the first partition is less than or equal to the max number of
	// primitives per leaf, put all the primitives in the first child.
	if ( numLesserPrimitives <= maxNumPrimitivesPerLeaf )
	{
		numChildPrimitives[0] = numLesserPrimitives;
		numChildPrimitives[1] = 0;
		volumes[0] = computeAABBForPrimitives( primitiveAABBs, primitiveIndicesStart, numLesserPrimitives );
	}
	else
	{
		partitionPrimitivesSAH( primitiveAABBs, primitiveIndicesStart, numLesserPrimitives,
							splitBins, numSplitBins, splitAxis[1],
							numChildPrimitives[0], volumes[0], volumes[1] );
	}
	
	// If the number of primitives on this side of the first partition is less than or equal to the max number of
	// primitives per leaf, put all the primitives in the first child.
	if ( numGreaterPrimitives <= maxNumPrimitivesPerLeaf )
	{
		numChildPrimitives[2] = numGreaterPrimitives;
		numChildPrimitives[3] = 0;
		volumes[2] = computeAABBForPrimitives( primitiveAABBs, primitiveIndicesStart + numLesserPrimitives, numGreaterPrimitives );
	}
	else
	{
		partitionPrimitivesSAH( primitiveAABBs, primitiveIndicesStart + numLesserPrimitives, numGreaterPrimitives,
							splitBins, numSplitBins, splitAxis[2],
							numChildPrimitives[2], volumes[2], volumes[3] );
	}
	
	// Compute the number of primitives greater than the split plane along the split axis.
	numChildPrimitives[1] = numLesserPrimitives - numChildPrimitives[0];
	numChildPrimitives[3] = numGreaterPrimitives - numChildPrimitives[2];
	
	//***************************************************************************
	// Determine for each child whether to create a leaf node or an inner node.
	
	// Create the node.
	new (node) Node();
	
	// Keep track of the total number of nodes in the subtree.
	Size numTreeNodes = 1;
	PrimitiveIndex primitiveStartIndex = start;
	
	for ( Index i = 0; i < 4; i++ )
	{
		// Set the child bounding box.
		node->setChildAABB( i, volumes[i] );
		
		if ( numChildPrimitives[i] <= maxNumPrimitivesPerLeaf || depth >= MAX_TREE_DEPTH )
		{
			// This child is a leaf node.
			node->setLeaf( i, numChildPrimitives[i], primitiveStartIndex );
		}
		else
		{
			// This is an inner node. Set the relative index of this child from the parent node.
			node->setChild( i, numTreeNodes );
			
			// Construct the tree recursively.
			Size numChildNodes = buildTreeRecursive( node + numTreeNodes, primitiveAABBs, primitiveIndices,
													primitiveStartIndex, numChildPrimitives[i],
													splitBins, numSplitBins, maxNumPrimitivesPerLeaf,
													depth + 1, maxDepth );
			
			// Add the number of nodes created in the child subtree.
			numTreeNodes += numChildNodes;
		}
		
		// Adjust the primitive start index by the number of primitives in the subtree.
		primitiveStartIndex += numChildPrimitives[i];
	}
	
	//***************************************************************************
	
	// Update the maximum tree depth.
	if ( depth > maxDepth )
		maxDepth = depth;
	
	// Return the number of nodes in this subtree.
	return numTreeNodes;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Surface Area Heuristic Object Partition Method
//############		
//##########################################################################################
//##########################################################################################




void AABBTree4:: partitionPrimitivesSAH( const PrimitiveAABB* primitiveAABBs, PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives,
										SplitBin* splitBins, Size numSplitBins,
										Index& splitAxis, PrimitiveCount& numLesserPrimitives,
										AABB3f& lesserVolume, AABB3f& greaterVolume )
{
	// If there are no primitives to partition, return immediately.
	if ( numPrimitives < 2 )
	{
		splitAxis = 0;
		numLesserPrimitives = numPrimitives;
		lesserVolume = computeAABBForPrimitives( primitiveAABBs, primitiveIndices, numPrimitives );
		return;
	}
	
	//**************************************************************************************
	// Compute the AABB of the primitive centroids.
	
	// We use the centroids as the 'keys' in splitting primitives.
	const AABB3f centroidAABB = computeAABBForPrimitiveCentroids( primitiveAABBs, primitiveIndices, numPrimitives );
	const Vector3f centroidAABBSize = centroidAABB.max - centroidAABB.min;
	
	//**************************************************************************************
	// Initialize the split bins.
	
	// Determine the number of candidate locations to examine for the split planes.
	const Size numSplitBinsUsed = math::max( math::min( numSplitBins, Size(2)*numPrimitives ),
											math::min( Size(8), numSplitBins ) );
	const Size numSplitCandidates = numSplitBinsUsed - 1;
	
	const Float binningConstant1 = Float(numSplitBinsUsed)*(Float(1) - Float(0.00001));
	Float minSplitCost = math::max<Float>();
	Float minSplitPlane = 0;
	SIMDFloat4 lesserMin;
	SIMDFloat4 lesserMax;
	SIMDFloat4 greaterMin;
	SIMDFloat4 greaterMax;
	numLesserPrimitives = 0;
	splitAxis = 0;
	
	for ( Index axis = 0; axis < 3; axis++ )
	{
		// Compute some constants that are valid for all bins/primitives.
		const Float binningConstant = binningConstant1 / centroidAABBSize[axis];
		const Float binWidth = centroidAABBSize[axis] / Float(numSplitBinsUsed);
		const Float binsStart = centroidAABB.min[axis];
		
		// Initialize the split bins to their starting values.
		for ( Index i = 0; i < numSplitBinsUsed; i++ )
			new (splitBins + i) SplitBin();
		
		//**************************************************************************************
		// For each primitive, determine which bin it overlaps and increase that bin's counter.
		
		for ( PrimitiveIndex i = 0; i < numPrimitives; i++ )
		{
			const PrimitiveAABB& t = primitiveAABBs[primitiveIndices[i]];
			
			Index binIndex = (Index)(binningConstant*(t.centroid[axis] - binsStart));
			SplitBin& bin = splitBins[binIndex];
			
			// Update the number of primitives that this bin contains, as well as the AABB for those primitives.
			bin.numPrimitives++;
			bin.min = math::min( bin.min, t.min );
			bin.max = math::max( bin.max, t.max );
		}
		
		//**************************************************************************************
		// Find the split plane with the smallest SAH cost.
		
		PrimitiveCount numLeftPrimitives = 0;
		SIMDFloat4 leftMin( math::max<float>() );
		SIMDFloat4 leftMax( math::min<float>() );
		
		for ( Index i = 0; i < numSplitCandidates; i++ )
		{
			// Since the left candidate is only growing, we can incrementally construct the AABB for this side.
			// Incrementally enlarge the bounding box for this side, and compute the number of primitives
			// on this side of the split.
			{
				SplitBin& bin = splitBins[i];
				numLeftPrimitives += bin.numPrimitives;
				leftMin = math::min( leftMin, bin.min );
				leftMax = math::max( leftMax, bin.max );
			}
			
			PrimitiveCount numRightPrimitives = 0;
			SIMDFloat4 rightMin( math::max<float>() );
			SIMDFloat4 rightMax( math::min<float>() );
			
			// Compute the bounding box for this side, and compute the number of primitives
			// on this side of the split.
			for ( Index j = i + 1; j < numSplitBinsUsed; j++ )
			{
				SplitBin& bin = splitBins[j];
				numRightPrimitives += bin.numPrimitives;
				rightMin = math::min( rightMin, bin.min );
				rightMax = math::max( rightMax, bin.max );
			}
			
			// Compute the cost for this split candidate.
			Float splitCost = Float(numLeftPrimitives)*getAABBSurfaceArea( leftMin, leftMax ) + 
							Float(numRightPrimitives)*getAABBSurfaceArea( rightMin, rightMax );
			
			// If the split cost is the lowest so far, use it as the new minimum split.
			if ( splitCost <= minSplitCost )
			{
				minSplitCost = splitCost;
				minSplitPlane = binsStart + binWidth*Float(i + 1);
				
				// Save the bounding boxes for this split candidate.
				lesserMin = leftMin;
				lesserMax = leftMax;
				greaterMin = rightMin;
				greaterMax = rightMax;
				
				// Save the number of primitives to the left of the split.
				numLesserPrimitives = numLeftPrimitives;
				
				// Save the axis of the minimum cost split candidate.
				splitAxis = axis;
			}
		}
	}
	
	//**************************************************************************************
	
	// If the split was unsuccessful, try a median split that is guaranteed to split the primitives.
	if ( numLesserPrimitives == 0 || numLesserPrimitives == numPrimitives )
	{
		// Choose to split along the axis with the largest extent.
		Size splitAxis = centroidAABBSize[0] > centroidAABBSize[1] ? 
						centroidAABBSize[0] > centroidAABBSize[2] ? 0 : 2 :
						centroidAABBSize[1] > centroidAABBSize[2] ? 1 : 2;
		
		// Use a median-based partition to split the primitives.
		partitionPrimitivesMedian( primitiveAABBs, primitiveIndices, numPrimitives,
									splitAxis, numLesserPrimitives, lesserVolume, greaterVolume );
		
		return;
	}
	
	//**************************************************************************************
	// Partition the primitives into two sets based on the minimal cost split plane.
	
	PrimitiveIndex left = 0;
	PrimitiveIndex right = numPrimitives - 1;
	
	while ( left < right )
	{
		PrimitiveIndex leftIndex = primitiveIndices[left];
		
		// Move right while primitive < split plane.
		while ( primitiveAABBs[leftIndex].centroid[splitAxis] <= minSplitPlane && left < right )
		{
			left++;
			leftIndex = primitiveIndices[left];
		}
		
		PrimitiveIndex rightIndex = primitiveIndices[right];
		
		// Move left while primitive > split plane.
		while ( primitiveAABBs[rightIndex].centroid[splitAxis] > minSplitPlane && left < right )
		{
			right--;
			rightIndex = primitiveIndices[right];
		}
		
		if ( left < right )
		{
			// Swap the primitives because they are out of order.
			primitiveIndices[left] = rightIndex;
			primitiveIndices[right] = leftIndex;
		}
	}
	
	// Set the number of primitives that are to the left of the split plane.
	lesserVolume = AABB3f( lesserMin[0], lesserMax[0], lesserMin[1], lesserMax[1], lesserMin[2], lesserMax[2] );
	greaterVolume = AABB3f( greaterMin[0], greaterMax[0], greaterMin[1], greaterMax[1], greaterMin[2], greaterMax[2] );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Median Object Partition Method
//############		
//##########################################################################################
//##########################################################################################




void AABBTree4:: partitionPrimitivesMedian( const PrimitiveAABB* primitiveAABBs, PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives,
											Index splitAxis, PrimitiveCount& numLesserPrimitives,
											AABB3f& lesserVolume, AABB3f& greaterVolume )
{
	if ( numPrimitives == 2 )
	{
		numLesserPrimitives = 1;
		lesserVolume = computeAABBForPrimitives( primitiveAABBs, primitiveIndices, 1 );
		greaterVolume = computeAABBForPrimitives( primitiveAABBs, primitiveIndices + 1, 1 );
		return;
	}
	
	PrimitiveIndex first = 0;
	PrimitiveIndex last = numPrimitives - 1;
	PrimitiveIndex middle = (first + last)/2;
	
	while ( true )
	{
		PrimitiveIndex mid = first;
		const Float key = primitiveAABBs[primitiveIndices[mid]].centroid[splitAxis];
		
		for ( PrimitiveIndex j = first + 1; j <= last; j ++)
		{
			PrimitiveIndex clientIndex = primitiveIndices[j];
			
			if ( primitiveAABBs[clientIndex].centroid[splitAxis] > key )
			{
				mid++;
				
				// Interchange indices.
				const PrimitiveIndex temp = primitiveIndices[mid];
				primitiveIndices[mid] = clientIndex;
				primitiveIndices[j] = temp;
			}
		}
		
		// Interchange the first and mid value.
		const PrimitiveIndex temp = primitiveIndices[mid];
		primitiveIndices[mid] = primitiveIndices[first];
		primitiveIndices[first] = temp;
		
		if ( mid + 1 == middle )
			break;
		
		if ( mid + 1 > middle )
			last = mid - 1;
		else
			first = mid + 1;
	}
	
	numLesserPrimitives = numPrimitives / 2;
	lesserVolume = computeAABBForPrimitives( primitiveAABBs, primitiveIndices, numLesserPrimitives );
	greaterVolume = computeAABBForPrimitives( primitiveAABBs, primitiveIndices + numLesserPrimitives, numPrimitives - numLesserPrimitives );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Generic Tree Refit Method
//############		
//##########################################################################################
//##########################################################################################




AABB3f AABBTree4:: refitTreeGeneric( const Child& node )
{
	if ( Node::isLeaf(node) )
	{
		// Compute the bounding box of this leaf's primitives.
		const PrimitiveIndex* primitive = primitiveIndices + Node::getLeafOffset( node );
		const PrimitiveIndex primitiveCount = Node::getLeafCount( node );
		
		AABB3f result = geometry->getPrimitiveAABB( primitive[0] );
		
		for ( PrimitiveIndex i = 1; i < primitiveCount; i++ )
			result.enlargeFor( geometry->getPrimitiveAABB( primitive[i] ) );
		
		return result;
	}
	else
	{
		AABB3f result( math::max<Float>(), math::min<Float>() );
		
		// Resursively find the new bounding box for the children of this node.
		for ( Index i = 0; i < 4; i++ )
		{
			Child child = node.node->getChild(i);
			
			// Skip empty leaves.
			if ( Node::isLeaf(child) && child.leaf.count == 0 )
				continue;
			
			AABB3f childAABB = refitTreeGeneric( child );
			
			// Store the bounding box for the child in this node.
			node.node->setChildAABB( i, childAABB );
			
			// Find the bounding box containing all children.
			result.enlargeFor( childAABB );
		}
		
		return result;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle Tree Refit Method
//############		
//##########################################################################################
//##########################################################################################




AABB3f AABBTree4:: refitTreeTriangles( const Child& node )
{
	if ( Node::isLeaf(node) )
	{
		// Compute the bounding box of this leaf's primitives.
		CachedTriangle* triangle = (CachedTriangle*)primitiveData + Node::getLeafOffset( node );
		const PrimitiveCount primitiveCount = (PrimitiveCount)Node::getLeafCount( node );
		
		AABB3f result( math::max<Float>(), math::min<Float>() );
		Vector3f v0, v1, v2;
		
		for ( PrimitiveIndex i = 0; i < primitiveCount; i++ )
		{
			CachedTriangle& t = triangle[i];
			
			for ( Index j = 0; j < 4; j++ )
			{
				PrimitiveIndex clientIndex = t.indices[j];
				
				// Get the triangle vertices.
				geometry->getTriangle( clientIndex, v0, v1, v2 );
				
				// Enlarge the leaf's bounding box.
				result.enlargeFor( v0 );
				result.enlargeFor( v1 );
				result.enlargeFor( v2 );
				
				// Update cached triangles.
				Vector3f e1 = v1 - v0;
				Vector3f e2 = v2 - v0;
				t.v0.x[j] = v0.x;	t.v0.y[j] = v0.y;	t.v0.z[j] = v0.z;
				t.e1.x[j] = e1.x;	t.e1.y[j] = e1.y;	t.e1.z[j] = e1.z;
				t.e2.x[j] = e2.x;	t.e2.y[j] = e2.y;	t.e2.z[j] = e2.z;
			}
		}
		
		return result;
	}
	else
	{
		AABB3f result( math::max<Float>(), math::min<Float>() );
		
		// Resursively find the new bounding box for the children of this node.
		for ( Index i = 0; i < 4; i++ )
		{
			Child child = node.node->getChild(i);
			
			// Skip empty leaves.
			if ( Node::isLeaf(child) && child.leaf.count == 0 )
				continue;
			
			AABB3f childAABB = refitTreeTriangles( child );
			
			// Store the bounding box for the child in this node.
			node.node->setChildAABB( i, childAABB );
			
			// Find the bounding box containing all children.
			result.enlargeFor( childAABB );
		}
		
		return result;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Axis-Aligned Bound Box Calculation Methods
//############		
//##########################################################################################
//##########################################################################################




AABB3f AABBTree4:: computeAABBForPrimitives( const PrimitiveAABB* primitiveAABBs,
											const PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives )
{
	/// Create a bounding box with the minimum at the max float value and visce versa.
	SIMDFloat4 min( math::max<float>() );
	SIMDFloat4 max( math::min<float>() );
	
	const PrimitiveIndex* const primitiveIndicesEnd = primitiveIndices + numPrimitives;
	
	while ( primitiveIndices != primitiveIndicesEnd )
	{
		const PrimitiveAABB& aabb = primitiveAABBs[*primitiveIndices];
		min = math::min( min, aabb.min );
		max = math::max( max, aabb.max );
		
		primitiveIndices++;
	}
	
	return AABB3f( min[0], max[0], min[1], max[1], min[2], max[2] );
}




AABB3f AABBTree4:: computeAABBForPrimitiveCentroids( const PrimitiveAABB* primitiveAABBs,
													const PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives )
{
	/// Create a bounding box with the minimum at the max float value and visce versa.
	SIMDFloat4 min( math::max<float>() );
	SIMDFloat4 max( math::min<float>() );
	
	const PrimitiveIndex* const primitiveIndicesEnd = primitiveIndices + numPrimitives;
	
	while ( primitiveIndices != primitiveIndicesEnd )
	{
		const PrimitiveAABB& aabb = primitiveAABBs[*primitiveIndices];
		min = math::min( min, aabb.centroid );
		max = math::max( max, aabb.centroid );
		
		primitiveIndices++;
	}
	
	return AABB3f( min[0], max[0], min[1], max[1], min[2], max[2] );
}




float AABBTree4:: getAABBSurfaceArea( const SIMDFloat4& min, const SIMDFloat4& max )
{
	const SIMDFloat4 aabbDimension = max - min;
	
	return float(2)*(aabbDimension[0]*aabbDimension[1] +
					aabbDimension[0]*aabbDimension[2] +
					aabbDimension[1]*aabbDimension[2]);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle List Building Methods
//############		
//##########################################################################################
//##########################################################################################




Size AABBTree4:: getTriangleArraySize() const
{
	Child root;
	root.node = nodes;
	return getTriangleArraySize( root );
}




Size AABBTree4:: getTriangleArraySize( const Child& node )
{
	if ( Node::isLeaf(node) )
		return math::nextMultiple( Node::getLeafCount( node ), PrimitiveIndex(4) ) >> 2;
	else
	{
		Size result = 0;
		
		for ( Index i = 0; i < 4; i++ )
			result += getTriangleArraySize( node.node->getChild(i) );
		
		return result;
	}
}




Size AABBTree4:: fillTriangleArray( CachedTriangle* triangles, const BVHGeometry* geometry,
									Child& node, Size numFilled )
{
	Size currentOutputIndex = numFilled;
	Vector3f v0, v1, v2;
	
	if ( Node::isLeaf(node) )
	{
		Size numLeafTriangles = (Size)Node::getLeafCount( node );
		Size numTruncatedTriangles = ((numLeafTriangles >> 2) << 2);
		Size numPaddedTriangles = numTruncatedTriangles == numLeafTriangles ? 
									numTruncatedTriangles : numTruncatedTriangles + 4;
		
		// Update the per-node primitive count to reflect that 4 regular triangles = 1 cached triangle.
		Index currentOffset = (Index)Node::getLeafOffset( node );
		const Size numIterations = numPaddedTriangles >> 2;
		Node::setLeaf( node, numIterations, currentOutputIndex );
		
		for ( Index k = 0; k < numIterations; k++ )
		{
			// Determine the number of triangles to go into this cached triangle, 4 or less.
			Size numRemainingTriangles = math::min( numLeafTriangles - k*4, Size(4) );
			CachedTriangle& tri = triangles[currentOutputIndex];
			
			// Get the triangle from the primitive set.
			for ( Index t = 0; t < 4; t++ )
			{
				// If there are no more remaining triangles, use the last valid one.
				PrimitiveIndex clientIndex;
				
				if ( t < numRemainingTriangles )
					clientIndex = primitiveIndices[currentOffset + t];
				else
					clientIndex = primitiveIndices[currentOffset + numRemainingTriangles - 1];
				
				geometry->getTriangle( clientIndex, v0, v1, v2 );
				Vector3f e1 = v1 - v0;
				Vector3f e2 = v2 - v0;
				
				// Convert to SIMD layout and store the triangle.
				tri.v0.x[t] = v0.x;	tri.v0.y[t] = v0.y;	tri.v0.z[t] = v0.z;
				tri.e1.x[t] = e1.x; tri.e1.y[t] = e1.y; tri.e1.z[t] = e1.z;
				tri.e2.x[t] = e2.x; tri.e2.y[t] = e2.y; tri.e2.z[t] = e2.z;
				tri.indices[t] = clientIndex;
			}
			
			currentOffset += 4;
			currentOutputIndex++;
		}
	}
	else
	{
		for ( Index i = 0; i < 4; i++ )
			currentOutputIndex += fillTriangleArray( triangles, geometry, node.node->getChild(i), currentOutputIndex );
	}
	
	return currentOutputIndex - numFilled;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Primitive Data Copy Method
//############		
//##########################################################################################
//##########################################################################################




UByte* AABBTree4:: copyPrimitiveData( Size& newCapacity ) const
{
	switch ( cachedPrimitiveType )
	{
		case BVHGeometry::TRIANGLES:
		{
			newCapacity = getTriangleArraySize();
			return (UByte*)util::copyArrayAligned( (const CachedTriangle*)primitiveData, newCapacity, 16 );
		}
		
		default:
			return NULL;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Ray Tracing Helper Methods
//############		
//##########################################################################################
//##########################################################################################




Int AABBTree4:: minIndex( const SIMDFloat4& x )
{
	const SIMDInt4 indices1( 0, 1, 2, 3 );
	const SIMDInt4 indices2( 2, 3, 0, 1 );
	
	// Shuffle the value once to find the minimum of 0 & 2, 1 & 3.
	SIMDFloat4 x2 = math::shuffle<2,3,0,1>( x );
	
	// Determine the indices of the values which are the minimum of 0 & 2, 1 & 3.
	SIMDInt4 indices3 = math::select( x < x2, indices1, indices2 );
	
	// Find the minimum of 0 & 2, 1 & 3.
	x2 = math::min( x, x2 );
	
	// Shuffle the values again to determine the minimum value.
	SIMDFloat4 x3 = math::shuffle<1,0,3,2>( x2 );
	
	// Compute the index of the closest intersection.
	SIMDInt4 minimumIndex = math::select( x2 < x3, indices3, math::shuffle<1,0,3,2>( indices3 ) );
	
	return minimumIndex[0];
}




Int AABBTree4:: minIndex( const SIMDFloat4& x, SIMDFloat4& wideMin )
{
	const SIMDInt4 indices1( 0, 1, 2, 3 );
	const SIMDInt4 indices2( 2, 3, 0, 1 );
	
	// Shuffle the value once to find the minimum of 0 & 2, 1 & 3.
	SIMDFloat4 x2 = math::shuffle<2,3,0,1>( x );
	
	// Determine the indices of the values which are the minimum of 0 & 2, 1 & 3.
	SIMDInt4 indices3 = math::select( x < x2, indices1, indices2 );
	
	// Find the minimum of 0 & 2, 1 & 3.
	x2 = math::min( x, x2 );
	
	// Shuffle the values again to determine the minimum value.
	SIMDFloat4 x3 = math::shuffle<1,0,3,2>( x2 );
	
	// Compute the index of the closest intersection.
	SIMDInt4 minimumIndex = math::select( x2 < x3, indices3, math::shuffle<1,0,3,2>( indices3 ) );
	
	// Compute a 4-wide vector of the minimum value.
	wideMin = math::min( x2, x3 );
	
	return minimumIndex[0];
}




//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
