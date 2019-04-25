/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsDiffusePathCache.h
 * Contents:    gsound::internal::DiffusePathCache class declaration
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


#ifndef INCLUDE_GSOUND_DIFFUSE_PATH_CACHE_H
#define INCLUDE_GSOUND_DIFFUSE_PATH_CACHE_H


#include "gsInternalConfig.h"


#include "gsDiffusePathInfo.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a persistent cache that maintains a moving average of the contributions of many sound paths.
class DiffusePathCache
{
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// Define the type to use for the hash table buckets of this cache.
			typedef ShortArrayList<DiffusePathInfo,1> BucketType;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty diffuse path cache.
			DiffusePathCache();
			
			
			/// Create a new empty diffuse path cache with the specified number of hash table buckets and load factor.
			DiffusePathCache( Size newNumBuckets, Float newLoadFactor );
			
			
			/// Create a deep copy of the specified diffuse path cache.
			DiffusePathCache( const DiffusePathCache& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this diffuse path cache, releasing all internal resources.
			~DiffusePathCache();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another diffuse path cache to this one.
			DiffusePathCache& operator = ( const DiffusePathCache& other );
			
			
		//********************************************************************************
		//******	Cache Accessor Methods
			
			
			/// Return the number of entries that are in this sound path cache.
			Size getPathCount() const;
			
			
			/// Update the path with the specified hash code for a ray with the given attributes.
			void addContribution( SoundPathHash pathHash, const FrequencyBandResponse& response,
								const Vector3f& direction, const Vector3f& sourceDirection,
								Real distance, Real relativeSpeed, Index timeStamp );
			
			
			/// Merge the specified diffuse path cache with this one, adding all paths that didn't exist previously.
			void addContributions( const DiffusePathCache& otherCache );
			
			
			/// Remove all previously cached diffuse sound data from this cache.
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
		//******	Load Factor Accessor Methods
			
			
			/// Return the load factor used by this diffuse path cache to avoid bucket clustering.
			GSOUND_INLINE Float getLoadFactor() const
			{
				return loadFactor;
			}
			
			
			/// Set the load factor used by this diffuse path cache to avoid bucket clustering.
			/**
			  * The input value is clamped to the range [0.1,+infinity].
			  */
			GSOUND_INLINE void setLoadFactor( Float newLoadFactor )
			{
				loadFactor = math::max( newLoadFactor, Float(0.1) );
			}
			
			
			/// Check to see if the number of buckets in the hash table cache should be increased, and if necessary, increase it.
			void checkLoadFactor();
			
			
		//********************************************************************************
		//******	Cache Size in Bytes Accessor Method
			
			
			/// Return the approximate storage allocated by this cache.
			Size getSizeInBytes() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Define the default number of hash table buckets that this diffuse path cache should start with.
			static const Size DEFAULT_INITIAL_BUCKET_COUNT = 193;
			
			
			/// Define the default load factor for this cache's hash table.
			static const Float DEFAULT_LOAD_FACTOR;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of the buckets in this diffuse path cache.
			BucketType* buckets;
			
			
			/// The number of buckets that are part of this diffuse path cache.
			Size numBuckets;
			
			
			/// The maximum entry fraction of the number of buckets that should be allowed.
			Float loadFactor;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_DIFFUSE_PATH_CACHE_H
