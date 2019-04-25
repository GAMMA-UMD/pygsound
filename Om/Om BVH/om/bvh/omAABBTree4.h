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

#ifndef INCLUDE_OM_BVH_AABB_TREE_4_H
#define INCLUDE_OM_BVH_AABB_TREE_4_H


#include "omBVHConfig.h"


#include "omBVHBVH.h"


//##########################################################################################
//******************************  Start Om BVH Namespace  **********************************
OM_BVH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements a SIMD-accelerated 4-ary bounding volume hierarchy.
/**
  * For performance reasons, this implementation is limited to (2^31 - 1) billion primitives per-BVH, roughly 2.1 billion.
  */
class AABBTree4 : public BVH
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new quad AABB tree with no primitives.
			AABBTree4();
			
			
			/// Create a copy of the specified quad AABB tree, using the same primitives.
			AABBTree4( const AABBTree4& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this quad AABB tree.
			virtual ~AABBTree4();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign a copy of another quad AABB tree to this one, using the same primitives.
			AABBTree4& operator = ( const AABBTree4& other );
			
			
		//********************************************************************************
		//******	BVH Geometry Accessor Methods
			
			
			/// Return a pointer to the geometry used by this BVH.
			virtual BVHGeometry* getGeometry() const;
			
			
			/// Set a pointer to the geometry that this BVH should use.
			/**
			  * Calling this method invalidates the current BVH, requiring it
			  * to be rebuilt before it can be used.
			  */
			virtual Bool setGeometry( BVHGeometry* newGeometry );
			
			
		//********************************************************************************
		//******	BVH Building Methods
			
			
			/// Rebuild the BVH using the current set of primitives.
			virtual void rebuild();
			
			
			/// Do a quick update of the BVH by refitting the bounding volumes without changing the hierarchy.
			virtual void refit();
			
			
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			/// Trace the specified ray through this BVH and get the closest intersection.
			/**
			  * The ray is populated with information about the intersection.
			  */
			virtual void intersectRay( BVHRay& ray ) const;
			
			
			/// Test whether or not the specified ray hits anything in this BVH.
			/**
			  * The ray is populated with information about whether or not the ray was hit,
			  * but no intersection results are provided.
			  *
			  * This method can be faster than the intersectRay() method if only a
			  * boolean occlusion result is needed.
			  */
			virtual void testRay( BVHRay& ray ) const;
			
			
		//********************************************************************************
		//******	BVH Attribute Accessor Methods
			
			
			/// Return the maximum depth of this BVH's hierarchy.
			OM_INLINE Size getMaxDepth() const
			{
				return maxDepth;
			}
			
			
			/// Return whether or not this BVH is built, valid, and ready for use.
			virtual Bool isValid() const;
			
			
			/// Return the approximate total amount of memory in bytes allocated for this BVH.
			virtual Size getSizeInBytes() const;
			
			
		//********************************************************************************
		//******	Bounding Volume Accessor Methods
			
			
			/// Return an axis-aligned bounding box for this BVH's contents.
			virtual AABB3f getAABB() const;
			
			
			/// Return a bounding sphere for this BVH's contents.
			virtual Sphere3f getBoundingSphere() const;
			
			
		//********************************************************************************
		//******	Primitives Per Leaf Accessor Methods
			
			
			/// Return the maximum number of primitives that can be part of a leaf node in this BVH.
			OM_INLINE PrimitiveCount getPrimitivesPerLeaf() const
			{
				return maxNumPrimitivesPerLeaf;
			}
			
			
			/// Set the maximum number of primitives that can be part of a leaf node in this BVH.
			/**
			  * The change does not go into effect until the BVH is rebuilt.
			  */
			OM_INLINE void setPrimitivesPerLeaf( PrimitiveCount newPrimitivesPerLeaf )
			{
				maxNumPrimitivesPerLeaf = math::max( newPrimitivesPerLeaf, PrimitiveCount(1) );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that represents a single node in the quad AABB tree.
			class Node;
			
			
			/// A class that stores the AABB of a single primitive used during tree construction.
			class PrimitiveAABB;
			
			
			/// A class used to keep track of surface-area-heuristic paritioning data.
			class SplitBin;
			
			
			/// A class that represents an internally cached triangle that has an efficient storage layout.
			class CachedTriangle;
			
			
			/// A ray class with extra data used to speed up intersection tests.
			class TraversalRay;
			
			
			/// Define the type to use for offsets in the BVH.
			typedef UInt32 IndexType;
			
			
			/// A union type used to store either a pointer to a child node or leaf node info.
			typedef union Child
			{
				/// A pointer to the child node, if the low-order bit is not set.
				Node* node;
				
				/// A structure, 64-bits wide, that stores information for a leaf node.
				struct Leaf
				{
					/// The number of primitives in the leaf.
					UInt32 count;
					
					/// The offset of this leaf's primitives in the primitive array.
					UInt32 offset;
				} leaf;
			} Child;
			
			
		//********************************************************************************
		//******	Private Ray Tracing Methods
			
			
			OM_FORCE_INLINE static Bool traceRayVsNode( const TraversalRay& ray, const SIMDFloat4& tMin, const SIMDFloat4& tMax,
														Child& childNode, Child*& stack );
			
			
			/// Trace a ray through the BVH for generic-typed primitives.
			OM_FORCE_INLINE void traceRayVsGeneric( BVHRay& ray ) const;
			
			
			/// Trace a ray through the BVH for cached triangle primitives.
			OM_FORCE_INLINE void traceRayVsTriangles( BVHRay& ray ) const;
			
			
		//********************************************************************************
		//******	Private Ray-Primitive Intersection Methods
			
			
			/// Trace a ray through the BVH for cached triangle primitives.
			OM_FORCE_INLINE static void rayIntersectsTriangles( const TraversalRay& ray, BVHRay& rayData, const SIMDFloat4& tMin, SIMDFloat4& tMax,
																const CachedTriangle& triangle );
			
			
		//********************************************************************************
		//******	Private Tree Bulding Methods
			
			
			/// Build a tree starting at the specified node using the specified objects.
			/**
			  * This method returns the number of nodes in the tree created.
			  */
			static Size buildTreeRecursive( Node* node, const PrimitiveAABB* primitiveAABBs,
											PrimitiveIndex* primitiveIndices, PrimitiveIndex start, PrimitiveCount numPrimitives,
											SplitBin* splitBins, Size numSplitCandidates,
											Size maxNumObjectsPerLeaf, Size depth, Size& maxDepth );
			
			
			/// Partition the specified list of objects into two sets based on the given split plane.
			/**
			  * The objects are sorted so that the first N objects in the list are deemed "less" than
			  * the split plane along the split axis, and the next M objects are the remainder.
			  * The number of "lesser" objects is placed in the output variable.
			  */
			static void partitionPrimitivesSAH( const PrimitiveAABB* primitiveAABBs, PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives,
												SplitBin* splitBins, Size numSplitCandidates,
												Index& axis, PrimitiveCount& numLesserObjects,
												AABB3f& lesserVolume, AABB3f& greaterVolume );
			
			
			/// Partition the specified list of objects into two sets based on their median along the given axis.
			static void partitionPrimitivesMedian( const PrimitiveAABB* primitiveAABBs, PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives,
												Index splitAxis, PrimitiveCount& numLesserTriangles,
												AABB3f& lesserVolume, AABB3f& greaterVolume );
			
			
			/// Compute the axis-aligned bounding box for the specified list of objects.
			static AABB3f computeAABBForPrimitives( const PrimitiveAABB* primitiveAABBs,
													const PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives );
			
			
			/// Compute the axis-aligned bounding box for the specified list of objects' centroids.
			static AABB3f computeAABBForPrimitiveCentroids( const PrimitiveAABB* primitiveAABBs, 
															const PrimitiveIndex* primitiveIndices, PrimitiveCount numPrimitives );
			
			
			/// Get the surface area of a 3D axis-aligned bounding box specified by 2 SIMD min-max vectors.
			OM_FORCE_INLINE static float getAABBSurfaceArea( const math::SIMDFloat4& min,
															const math::SIMDFloat4& max );
			
			
		//********************************************************************************
		//******	Private Tree Refitting Methods
			
			
			/// Refit the bounding volume for the specified node and return the final bounding box.
			AABB3f refitTreeGeneric( const Child& node );
			
			
			/// Refit the bounding volume for the specified node and return the final bounding box.
			AABB3f refitTreeTriangles( const Child& node );
			
			
		//********************************************************************************
		//******	Primitive List Building Methods
			
			
			OM_FORCE_INLINE Size getTriangleArraySize() const;
			
			
			static Size getTriangleArraySize( const Child& node );
			
			
			Size fillTriangleArray( CachedTriangle* triangles, const BVHGeometry* geometry,
									Child& node, Size numFilled );
			
			
		//********************************************************************************
		//******	Other Helper Methods
			
			
			/// Return a deep copy of this tree's cached primitive data.
			UByte* copyPrimitiveData( Size& newCapacity ) const;
			
			
			/// Return the index of the smallest value in the specified SIMD float.
			OM_FORCE_INLINE static Int minIndex( const SIMDFloat4& x );
			
			
			/// Return the index of the smallest value in the specified SIMD float, placing the expanded min value in the output parameter.
			OM_FORCE_INLINE static Int minIndex( const SIMDFloat4& x, SIMDFloat4& wideMin );
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The maximum allowed depth of a tree.
			static const Size MAX_TREE_DEPTH = 32;
			
			
			/// The number of entries that a traversal stack should be able to hold.
			static const Size TRAVERSAL_STACK_SIZE = 4*MAX_TREE_DEPTH;
			
			
			/// The default intial number of splitting plane candidates that are considered when building the tree.
			static const Size DEFAULT_NUM_SPLIT_CANDIDATES = 32;
			
			
			/// The default maximum number of primitives that can be in a leaf node.
			static const PrimitiveCount DEFAULT_MAX_PRIMITIVES_PER_LEAF = 4;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to a flat array of nodes that make up this tree.
			Node* nodes;
			
			
			/// The number of nodes that are in this quad AABB tree.
			Size numNodes;
			
			
			/// The number of primitives that are part of this qaud AABB tree.
			IndexType numPrimitives;
			
			
			/// A packed array of client primitive indicies organized by node.
			/**
			  * This acts as a lookup table between the node primitive offset
			  * and the client's primitive ordering.
			  */
			PrimitiveIndex* primitiveIndices;
			
			
			/// The number of primitive indices that can be stored in the primitive index array.
			Size primitiveIndexCapacity;
			
			
			/// A packed list of primitive data that are organized by node.
			UByte* primitiveData;
			
			
			/// The capacity in bytes of the primitive data allocation.
			Size primitiveDataCapacity;
			
			
			/// An opaque interface to the geometry contained in this tree.
			BVHGeometry* geometry;
			
			
			/// An enum value that indicates the type of the cached primitives, or UNDEFINED if not cached.
			BVHGeometry::Type cachedPrimitiveType;
			
			
			/// The maximum depth of the hierarchy of this quad AABB tree.
			Size maxDepth;
			
			
			/// The number of Surface Area Heuristic split plane candidates to consider when building the tree.
			Size numSplitCandidates;
			
			
			/// The maximum number of primitives that this quad AABB tree can have per leaf node.
			PrimitiveCount maxNumPrimitivesPerLeaf;
			
			
};





//##########################################################################################
//******************************  End Om BVH Namespace  ************************************
OM_BVH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_BVH_AABB_TREE_4_H
