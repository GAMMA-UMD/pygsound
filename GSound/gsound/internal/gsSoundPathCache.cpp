/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundPathCache.cpp
 * Contents:    gsound::internal::SoundPathCache class implementation
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


#include "gsSoundPathCache.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const Float SoundPathCache:: DEFAULT_LOAD_FACTOR = 1.0f;


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundPathCache:: SoundPathCache()
	:	numBuckets( DEFAULT_INITIAL_BUCKET_COUNT ),
		loadFactor( DEFAULT_LOAD_FACTOR ),
		buckets( util::constructArray<BucketType>( DEFAULT_INITIAL_BUCKET_COUNT ) )
{
}




SoundPathCache:: SoundPathCache( Hash newNumBuckets, Float newLoadFactor )
	:	numBuckets( math::nextPowerOf2Prime( newNumBuckets ) ),
		loadFactor( math::max( newLoadFactor, Float(0.1) ) )
{
	buckets = util::constructArray<BucketType>( numBuckets );
}




SoundPathCache:: SoundPathCache( const SoundPathCache& other )
	:	numBuckets( other.numBuckets ),
		loadFactor( other.loadFactor ),
		buckets( util::allocate<BucketType>( other.numBuckets ) )
{
	register BucketType* bucket = buckets;
	register const BucketType* otherBucket = other.buckets;
	register const BucketType* const otherBucketsEnd = other.buckets + other.numBuckets;
	
	while ( otherBucket != otherBucketsEnd )
	{
		// Copy each bucket.
		new (bucket) BucketType( *otherBucket );
		
		otherBucket++;
		bucket++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundPathCache:: ~SoundPathCache()
{
	util::destructArray( buckets, numBuckets );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundPathCache& SoundPathCache:: operator = ( const SoundPathCache& other )
{
	if ( this != &other )
	{
		// Destroy the old bucket array.
		util::destructArray( buckets, numBuckets );
		
		// Initialize the state for this cache from the other bucket.
		numBuckets = other.numBuckets;
		loadFactor = other.loadFactor;
		buckets = util::allocate<BucketType>( other.numBuckets );
		
		// Copy the buckets from the other cache.
		register BucketType* bucket = buckets;
		register const BucketType* otherBucket = other.buckets;
		register const BucketType* const otherBucketsEnd = other.buckets + other.numBuckets;
		
		while ( otherBucket != otherBucketsEnd )
		{
			// Copy each bucket.
			new (bucket) BucketType( *otherBucket );
			
			otherBucket++;
			bucket++;
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cache Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundPathCache:: getPathCount() const
{
	Size numEntries = 0;
	
	BucketType* bucket = buckets;
	const BucketType* const bucketsEnd = bucket + numBuckets;
	
	while ( bucket != bucketsEnd )
	{
		numEntries += bucket->getSize();
		bucket++;
	}
	
	return numEntries;
}




Bool SoundPathCache:: addPath( const SoundPathID& pathID, Index timeStamp )
{
	SoundPathHash pathHash = pathID.getHashCode();
	BucketType& bucket = buckets[pathHash % numBuckets];
	const Size bucketSize = bucket.getSize();
	
	for ( Index i = 0; i < bucketSize; i++ )
	{
		EntryType& entry = bucket[i];
		
		if ( entry.pathID == pathID )
		{
			entry.timeStamp = timeStamp;
			return false;
		}
	}
	
	bucket.add( Entry( pathID, timeStamp ) );
	
	return true;
}




Bool SoundPathCache:: containsPath( const SoundPathID& pathID ) const
{
	BucketType& bucket = buckets[pathID.getHashCode() % numBuckets];
	const Size bucketSize = bucket.getSize();
	
	for ( Index i = 0; i < bucketSize; i++ )
	{
		EntryType& entry = bucket[i];
		
		if ( entry.pathID == pathID )
			return true;
	}
	
	return false;
}




void SoundPathCache:: clear()
{
	register BucketType* bucket = buckets;
	register const BucketType* const bucketsEnd = buckets + numBuckets;
	
	while ( bucket != bucketsEnd )
	{
		bucket->clear();
		bucket++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Load Factor Check Method
//############		
//##########################################################################################
//##########################################################################################




void SoundPathCache:: checkLoadFactor()
{
	//********************************************************************************
	// Count how many entries are in the cache.
	
	Size numEntries = 0;
	
	BucketType* bucket = buckets;
	const BucketType* const bucketsEnd = bucket + numBuckets;
	
	while ( bucket != bucketsEnd )
	{
		numEntries += bucket->getSize();
		bucket++;
	}
	
	//********************************************************************************
	
	Size maxEntryCount = Size(numBuckets * loadFactor);
	
	if ( numEntries > maxEntryCount )
	{
		// Save the current state of the cache.
		BucketType* oldBuckets = buckets;
		Size oldNumBuckets = numBuckets;
		
		// Compute the new number of buckets.
		numBuckets = math::nextPowerOf2Prime( Size(numEntries / loadFactor) );
		
		// Allocate a new bucket array.
		buckets = util::constructArray<BucketType>( numBuckets );
		
		// Reinsert all paths into the new buckets.
		register const BucketType* oldBucket = oldBuckets;
		register const BucketType* const oldBucketsEnd = oldBuckets + oldNumBuckets;
		
		while ( oldBucket != oldBucketsEnd )
		{
			// For each path in the old bucket, add it to the new buckets.
			register const EntryType* path = oldBucket->getPointer();
			register const EntryType* const pathsEnd = path + oldBucket->getSize();
			
			while ( path != pathsEnd )
			{
				BucketType* bucket = buckets + (path->pathID.getHashCode() % numBuckets);
				bucket->add( *path );
				
				path++;
			}
			
			oldBucket++;
		}
		
		// Deallocate the old bucket array.
		util::destructArray( oldBuckets, oldNumBuckets );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cache Size in Bytes Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundPathCache:: getSizeInBytes() const
{
	Size totalSize = 0;
	
	// Add the size allocated by each bucket.
	for ( Index i = 0; i < numBuckets; i++ )
	{
		const BucketType& bucket = buckets[i];
		
		for ( Index j = 0; j < bucket.getSize(); j++ )
			totalSize += bucket[j].pathID.getSizeInBytes();
		
		totalSize += sizeof(BucketType) + bucket.getCapacity()*sizeof(Entry);
	}
	
	return totalSize + sizeof(SoundPathCache);
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
