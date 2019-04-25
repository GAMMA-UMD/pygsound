/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundPathCache.h
 * Contents:    gsound::internal::SoundPathCache class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PATH_CACHE_H
#define INCLUDE_GSOUND_SOUND_PATH_CACHE_H


#include "gsInternalConfig.h"


#include "gsSoundPathID.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




class SoundPathCache
{
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			class Entry
			{
				public:
					
					GSOUND_INLINE Entry( const SoundPathID& newPathID, Index newTimeStamp )
						:	pathID( newPathID ),
							timeStamp( newTimeStamp )
					{
					}
					
					
					SoundPathID pathID;
					
					Index timeStamp;
					
					
			};
			
			
			/// Define the type to use for the hash table buckets of this cache.
			typedef Entry EntryType;
			typedef ShortArrayList<Entry,1> BucketType;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an empy propagation path cache with the default number of hash table buckets.
			SoundPathCache();
			
			
			/// Create an empy propagation path cache with the specified number of hash table buckets and loadFactor.
			SoundPathCache( Hash newNumBuckets, Float newLoadFactor );
			
			
			/// Create a copy of another sound path cache.
			SoundPathCache( const SoundPathCache& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound path cache object and deallocate all resources that it has allocated.
			~SoundPathCache();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the contents of one sound path cache to ths cache, replacing this cache's state.
			SoundPathCache& operator = ( const SoundPathCache& other );
			
			
		//********************************************************************************
		//******	Cache Accessor Methods
			
			
			/// Return the number of entries that are in this sound path cache.
			Size getPathCount() const;
			
			
			/// If a sound path doesn't currently exists in the cache, add it to the cache.
			/**
			  * If the sound path was already in the cache, return FALSE and leave the
			  * cache unmodified. Otherwise, return TRUE and add the sound path to the
			  * cache.
			  * 
			  * @param newSoundPathID - the sound path to try to add to this sound path cache.
			  * @return whether or not the path was added to the cache.
			  */
			Bool addPath( const SoundPathID& newSoundPathID, Index timeStamp );
			
			
			/// Merge the specified sound path cache with this one, adding all paths that didn't exist previously.
			void addPaths( const SoundPathCache& otherCache );
			
			
			/// Return whether or not this sound path cache currently contains the specified sound path.
			/**
			  * If the cache currently contains this sound path, the method returns TRUE.
			  * Otherwise, FALSE is returned.
			  * 
			  * @param soundPath - the sound path to be tested to see if it exists in the cache.
			  * @return whether or not the cache currently contains the specified sound path.
			  */
			Bool containsPath( const SoundPathID& soundPath ) const;
			
			
			/// Remove all sound paths from this cache.
			void clear();
			
			
		//********************************************************************************
		//******	Bucket Accessor Methods
			
			
			/// Return the total number of hash-table buckets are part of this diffuse path cache.
			GSOUND_INLINE Size getBucketCount() const
			{
				return numBuckets;
			}
			
			
			/// Return a reference to the bucket at the specified index in this diffuse path cache.
			GSOUND_INLINE BucketType& getBucket( Index bucketIndex )
			{
				return buckets[bucketIndex];
			}
			
			
			/// Return a const reference to the bucket at the specified index in this diffuse path cache.
			GSOUND_INLINE const BucketType& getBucket( Index bucketIndex ) const
			{
				return buckets[bucketIndex];
			}
			
			
		//********************************************************************************
		//******	Load Factor Constraint Methods
			
			
			/// Get the load factor used by this sound path cache to avoid bucket clustering.
			GSOUND_INLINE Float getLoadFactor() const
			{
				return loadFactor;
			}
			
			
			/// Set the load factor used by this sound path cache to avoid bucket clustering.
			/**
			  * The input value is clamped to the range [0.1,+infinity].
			  */
			GSOUND_INLINE void setLoadFactor( Float newLoadFactor )
			{
				loadFactor = math::max( newLoadFactor, Float(0.1) );
			}
			
			
			/// Check to see if the hash table should be resized, and if necessary, resize it.
			/**
			  * If the number of paths in the cache is greater than the load factor multiplied
			  * by the number of buckets in the cache's hash table, the cache is resized
			  * to a prime number of buckets at least twice the current size and great enough
			  * to satisfy the load factor constraint. This is a potentially expensive operation
			  * and should only performed once per sound propagation frame to avoid massive
			  * reallocation.
			  */
			void checkLoadFactor();
			
			
		//********************************************************************************
		//******	Cache Size in Bytes Accessor Method
			
			
			/// Return the approximate storage allocated by this cache.
			Size getSizeInBytes() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Define the default number of hash table buckets that this sound path cache should start with.
			static const Size DEFAULT_INITIAL_BUCKET_COUNT = 193;
			
			
			/// Define the default load factor for this cache's hash table.
			static const Float DEFAULT_LOAD_FACTOR;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of the buckets in this sound path cache.
			BucketType* buckets;
			
			
			/// The number of buckets that are part of this sound path cache.
			Size numBuckets;
			
			
			/// The maximum entry fraction of the number of buckets that should be allowed.
			Float loadFactor;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PATH_CACHE_H
