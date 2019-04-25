/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsVisibilityCache.cpp
 * Contents:    gsound::internal::VisibilityCache class implementation
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


#include "gsVisibilityCache.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




const Float VisibilityCache:: DEFAULT_LOAD_FACTOR = 1.0f;




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




VisibilityCache:: VisibilityCache()
	:	numBuckets( DEFAULT_INITIAL_BUCKET_COUNT ),
		loadFactor( DEFAULT_LOAD_FACTOR ),
		buckets( util::constructArray<BucketType>( DEFAULT_INITIAL_BUCKET_COUNT ) )
{
}




VisibilityCache:: VisibilityCache( Hash newNumBuckets, Float newLoadFactor )
	:	numBuckets( math::nextPowerOf2Prime( newNumBuckets ) ),
		loadFactor( math::max( newLoadFactor, Float(0.1) ) )
{
	buckets = util::constructArray<BucketType>( numBuckets );
}




VisibilityCache:: VisibilityCache( const VisibilityCache& other )
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




VisibilityCache:: ~VisibilityCache()
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




VisibilityCache& VisibilityCache:: operator = ( const VisibilityCache& other )
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




Size VisibilityCache:: getTriangleCount() const
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




Bool VisibilityCache:: addTriangle( const ObjectSpaceTriangle& newTriangle, Index timeStamp )
{
	BucketType& bucket = buckets[newTriangle.getHashCode() % numBuckets];
	const Size bucketSize = bucket.getSize();
	
	for ( Index i = 0; i < bucketSize; i++ )
	{
		Entry& entry = bucket[i];
		
		if ( entry.triangle == newTriangle )
		{
			entry.timeStamp = timeStamp;
			return false;
		}
	}
	
	bucket.add( Entry( newTriangle, timeStamp ) );
	
	return true;
}




Bool VisibilityCache:: containsTriangle( const ObjectSpaceTriangle& triangle ) const
{
	BucketType& bucket = buckets[triangle.getHashCode() % numBuckets];
	const Size bucketSize = bucket.getSize();
	
	for ( Index i = 0; i < bucketSize; i++ )
	{
		Entry& entry = bucket[i];
		
		if ( entry.triangle == triangle )
			return true;
	}
	
	return false;
}




void VisibilityCache:: clear()
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
//############		Cache Update Method
//############		
//##########################################################################################
//##########################################################################################




void VisibilityCache:: removeOldTriangles( Index timeStamp, Size maxAge )
{
	register BucketType* bucket = buckets;
	register const BucketType* const bucketsEnd = buckets + numBuckets;
	
	while ( bucket != bucketsEnd )
	{
		for ( Index i = 0; i < bucket->getSize(); i++ )
		{
			if ( timeStamp - (*bucket)[i].timeStamp > maxAge )
			{
				bucket->removeAtIndexUnordered( i );
				continue;
			}
			i++;
		}
		
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




void VisibilityCache:: checkLoadFactor()
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
			register const Entry* entry = oldBucket->getPointer();
			register const Entry* const entrysEnd = entry + oldBucket->getSize();
			
			while ( entry != entrysEnd )
			{
				BucketType* bucket = buckets + (entry->triangle.getHashCode() % numBuckets);
				bucket->add( *entry );
				
				entry++;
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




Size VisibilityCache:: getSizeInBytes() const
{
	Size totalSize = sizeof(VisibilityCache) + numBuckets*sizeof(BucketType);
	
	for ( Index i = 0; i < numBuckets; i++ )
		totalSize += buckets[i].getCapacity()*sizeof(Entry);
	
	return totalSize;
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
