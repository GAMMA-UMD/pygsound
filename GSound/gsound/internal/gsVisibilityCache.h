/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsVisibilityCache.h
 * Contents:    gsound::internal::VisibilityCache class declaration
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


#ifndef INCLUDE_GSOUND_VISIBILITY_CACHE_H
#define INCLUDE_GSOUND_VISIBILITY_CACHE_H


#include "gsInternalConfig.h"


#include "gsObjectSpaceTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that caches the triangles visible to a sound detector.
class VisibilityCache
{
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// A class that stores an entry for a visibility cache.
			class Entry
			{
				public:
					
					GSOUND_INLINE Entry( const ObjectSpaceTriangle& newTriangle, Index newTimeStamp )
						:	triangle( newTriangle ),
							timeStamp( newTimeStamp )
					{
					}
					
					/// The triangle associated with this cache entry.
					ObjectSpaceTriangle triangle;
					
					/// The time stamp of the last frame this entry was valid.
					Index timeStamp;
					
			};
			
			
			/// Define the type to use for the hash table buckets of this cache.
			typedef ShortArrayList<Entry,1ul> BucketType;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empy visibility cache with the default number of hash table buckets.
			VisibilityCache();
			
			
			/// Create an empy visibility cache with the specified number of hash table buckets and loadFactor.
			VisibilityCache( Hash newNumBuckets, Float newLoadFactor );
			
			
			/// Create a copy of another visibility cache.
			VisibilityCache( const VisibilityCache& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a visibility cache object and deallocate all resources that it has allocated.
			~VisibilityCache();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the contents of one visibility cache to ths cache, replacing this cache's state.
			VisibilityCache& operator = ( const VisibilityCache& other );
			
			
		//********************************************************************************
		//******	Cache Accessor Methods
			
			
			/// Return the number of entries that are in this visibility cache.
			Size getTriangleCount() const;
			
			
			/// If a triangle doesn't currently exists in the cache, add it to the cache.
			/**
			  * If the triangle was already in the cache, return FALSE and leave the
			  * cache unmodified. Otherwise, return TRUE and add the sound path to the
			  * cache.
			  * 
			  * @param newSoundPathID - the trianlge to try to add to this visibility cache.
			  * @return whether or not the trianlge was added to the cache.
			  */
			Bool addTriangle( const ObjectSpaceTriangle& newTriangle, Index timeStamp );
			
			
			/// Return whether or not this visibility cache currently contains the specified triangle.
			/**
			  * If the cache currently contains this triangle, the method returns TRUE.
			  * Otherwise, FALSE is returned.
			  * 
			  * @param soundPath - the sound path to be tested to see if it exists in the cache.
			  * @return whether or not the cache currently contains the specified triangle.
			  */
			Bool containsTriangle( const ObjectSpaceTriangle& triangle ) const;
			
			
			/// Remove all triangles from this cache.
			void clear();
			
			
		//********************************************************************************
		//******	Cache Update Method
			
			
			/// Remove all triangles from this cache that are older than the specified max age.
			void removeOldTriangles( Index timeStamp, Size maxAge );
			
			
		//********************************************************************************
		//******	Bucket Accessor Methods
			
			
			/// Return the total number of hash-table buckets are part of this visibility cache.
			GSOUND_INLINE Size getBucketCount() const
			{
				return numBuckets;
			}
			
			
			/// Return a reference to the bucket at the specified index in this visibility cache.
			GSOUND_INLINE VisibilityCache::BucketType& getBucket( Index bucketIndex )
			{
				return buckets[bucketIndex];
			}
			
			
			/// Return a const reference to the bucket at the specified index in this visibility cache.
			GSOUND_INLINE const VisibilityCache::BucketType& getBucket( Index bucketIndex ) const
			{
				return buckets[bucketIndex];
			}
			
			
		//********************************************************************************
		//******	Load Factor Constraint Methods
			
			
			/// Get the load factor used by this visibility cache to avoid bucket clustering.
			GSOUND_INLINE Float getLoadFactor() const
			{
				return loadFactor;
			}
			
			
			/// Set the load factor used by this visibility cache to avoid bucket clustering.
			/**
			  * The input value is clamped to the range [0.1,+infinity].
			  */
			GSOUND_INLINE void setLoadFactor( Float newLoadFactor )
			{
				loadFactor = math::max( newLoadFactor, Float(0.1) );
			}
			
			
			/// Check to see if the hash table should be resized, and if necessary, resize it.
			/**
			  * If the number of entries in the cache is greater than the load factor multiplied
			  * by the number of buckets in the cache's hash table, the cache is resized
			  * to a prime number of buckets at least twice the current size and great enough
			  * to satisfy the load factor constraint. This is a potentially expensive operation
			  * and should only performed once per sound propagation frame to avoid lots of
			  * reallocation.
			  */
			void checkLoadFactor();
			
			
		//********************************************************************************
		//******	Cache Size Accessor Methods
			
			
			/// Return the approximate size in bytes of the memory used for this visibility cache.
			Size getSizeInBytes() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Define the default number of hash table buckets that this visibility cache should start with.
			static const Size DEFAULT_INITIAL_BUCKET_COUNT = 193;
			
			
			/// Define the default load factor for this cache's hash table.
			static const Float DEFAULT_LOAD_FACTOR;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of the buckets in this visibility cache.
			BucketType* buckets;
			
			
			/// The number of buckets that are part of this visibility cache.
			Size numBuckets;
			
			
			/// The maximum entry fraction of the number of buckets that should be allowed.
			Float loadFactor;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_VISIBILITY_CACHE_H
