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

#ifndef INCLUDE_OM_HASH_MAP_H
#define INCLUDE_OM_HASH_MAP_H


#include "omUtilitiesConfig.h"


#include "../math/omPrimes.h"
#include "omAllocator.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A container class that uses a hash table to map key objects to value objects.
template < typename K, typename V, typename HashType = Hash, typename SizeType = Size >
class HashMap
{
	private:
		
		//********************************************************************************
		//******	Private Bucket Class
			
			
			class Entry
			{
				public:
					
					
					OM_INLINE Entry( HashType newKeyHash, const K& newKey, const V& newValue )
						:	next( NULL ),
							keyHash( newKeyHash ),
							key( newKey ),
							value( newValue )
					{
					}
					
					Entry( const Entry& other )
						:	keyHash( other.keyHash ),
							key( other.key ),
							value( other.value )
					{
						if ( other.next )
							next = util::construct<Entry>(*other.next);
						else
							next = NULL;
					}
					
					
					~Entry()
					{
						if ( next )
							util::destruct(next);
					}
					
					
					Entry* next;
					
					HashType keyHash;
					K key;
					V value;
					
			};
			
			
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a hash map with the default load factor and number of buckets.
			OM_INLINE HashMap()
				:	buckets( util::allocate<Entry*>(DEFAULT_NUMBER_OF_BUCKETS) ),
					numBuckets( DEFAULT_NUMBER_OF_BUCKETS ),
					numElements( 0 ),
					loadFactor( DEFAULT_LOAD_FACTOR ),
					loadThreshold( SizeType(DEFAULT_LOAD_FACTOR*DEFAULT_NUMBER_OF_BUCKETS) )
			{
				nullBuckets();
			}
			
			
			/// Create a hash map with the specified load factor and default number of buckets.
			OM_INLINE HashMap( Float newLoadFactor )
				:	buckets( util::allocate<Entry*>(DEFAULT_NUMBER_OF_BUCKETS) ),
					numBuckets( DEFAULT_NUMBER_OF_BUCKETS ),
					numElements( 0 ),
					loadFactor( math::clamp( newLoadFactor, 0.1f, 2.0f ) )
			{
				loadThreshold = SizeType(loadFactor*DEFAULT_NUMBER_OF_BUCKETS);
				nullBuckets();
			}
			
			
			/// Create a hash map with the default load factor and specified number of buckets.
			OM_INLINE HashMap( HashType newNumBuckets )
				:	numBuckets( math::nextPowerOf2Prime(newNumBuckets) ),
					numElements( 0 ),
					loadFactor( DEFAULT_LOAD_FACTOR )
			{
				buckets = util::allocate<Entry*>(numBuckets);
				loadThreshold = SizeType(DEFAULT_LOAD_FACTOR*numBuckets);
				nullBuckets();
			}
			
			
			/// Create a hash map with the specified load factor and number of buckets.
			OM_INLINE HashMap( HashType newNumBuckets, Float newLoadFactor )
				:	numBuckets( math::nextPowerOf2Prime(newNumBuckets) ),
					numElements( 0 ),
					loadFactor( math::clamp( newLoadFactor, 0.1f, 2.0f ) )
			{
				buckets = util::allocate<Entry*>(numBuckets);
				loadThreshold = SizeType(loadFactor*numBuckets);
				nullBuckets();
			}
			
			
		//********************************************************************************
		//******	Copy Constructor
			
			
			/// Create a hash map with the specified load factor and number of buckets.
			OM_INLINE HashMap( const HashMap& other )
				:	buckets( util::allocate<Entry*>(other.numBuckets) ),
					numBuckets( other.numBuckets ),
					numElements( other.numElements ),
					loadFactor( other.loadFactor ),
					loadThreshold( other.loadThreshold )
			{
				// Copy the hash table buckets
				const Entry* const * otherBucket = other.buckets;
				const Entry* const * const otherBucketsEnd = otherBucket + numBuckets;
				Entry** bucket = buckets;
				
				while ( otherBucket != otherBucketsEnd )
				{
					if ( *otherBucket )
						*bucket = util::construct<Entry>(**otherBucket);
					else
						*bucket = NULL;
					
					otherBucket++;
					bucket++;
				}
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the contents of one hash map into another.
			OM_INLINE HashMap& operator = ( const HashMap& other )
			{
				if ( this != &other )
				{
					deleteBuckets( buckets, numBuckets );
					
					// Copy the parameters from the other hash map.
					numBuckets = other.numBuckets;
					numElements = other.numElements;
					buckets = util::allocate<Entry*>( numBuckets );
					loadFactor = other.loadFactor;
					loadThreshold = other.loadThreshold;
					
					{
						// Copy the hash table buckets
						const Entry* const * otherBucket = other.buckets;
						const Entry* const * const otherBucketsEnd = otherBucket + numBuckets;
						Entry** bucket = buckets;
						
						while ( otherBucket != otherBucketsEnd )
						{
							if ( *otherBucket )
								*bucket = util::construct<Entry>(**otherBucket);
							else
								*bucket = NULL;
							
							otherBucket++;
							bucket++;
						}
					}
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a hash map and it's contents, deallocating all memory used.
			OM_INLINE ~HashMap()
			{
				deleteBuckets( buckets, numBuckets );
			}
			
			
		//********************************************************************************
		//******	Add Method
			
			
			/// Add a new mapping to the hash map, associating the given key with the given value.
			/**
			  * If the add operation was successful, it returns a pointer to the location
			  * where the mapping's value is stored. Otherwise, a NULL pointer is returned.
			  */
			OM_INLINE V* add( HashType keyHash, const K& key, const V& value )
			{
				// Check the load constraint, if necessary, increase the size of the table.
				if ( numElements > loadThreshold )
					resize( math::nextPowerOf2Prime( numBuckets + 1 ) );
				
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				numElements++;
				
				// Add the new element.
				if ( *bucket == NULL )
				{
					*bucket = util::construct<Entry>( keyHash, key, value );
					return &(*bucket)->value;
				}
				else
				{
					Entry* entry = *bucket;
					
					while ( entry->next )
						entry = entry->next;
					
					entry->next = util::construct<Entry>( keyHash, key, value );
					return &entry->next->value;
				}
			}
			
			
		//********************************************************************************
		//******	Set Method
			
			
			/// Set the mapping of a key to be the given value, regardless of it's previous state.
			/**
			  * The method returns TRUE if the key did not previously exist in the HashMap.
			  * Otherwise the method returns FALSE.
			  */
			OM_INLINE Bool set( HashType keyHash, const K& key, const V& value )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				
				if ( *bucket == NULL )
					*bucket = util::construct<Entry>( keyHash, key, value );
				else
				{
					Entry* entry = *bucket;
					
					if ( entry->keyHash == keyHash && entry->key == key )
					{
						entry->value = value;
						return false;
					}
					
					while ( entry->next )
					{
						entry = entry->next;
						
						if ( entry->keyHash == keyHash && entry->key == key )
						{
							entry->value = value;
							return false;
						}
					}
					
					entry->next = util::construct<Entry>( keyHash, key, value );
				}
				
				numElements++;
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Remove Methods
			
			
			/// Remove the first mapping of the given key.
			/**
			  * If the key does not exist in the hash map FALSE is returned,
			  * otherwise TRUE is returned.
			  */
			OM_INLINE Bool remove( HashType keyHash, const K& key )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				Entry** previousNext = bucket;
				Entry* entry = *bucket;
				
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key )
					{
						*previousNext = entry->next;
						
						entry->next = NULL;
						util::destruct(entry);
						numElements--;
						
						return true;
					}
					
					previousNext = &(*previousNext)->next;
					entry = entry->next;
				}
				
				return false;
			}
			
			
			/// Remove the all mappings for the given key.
			/**
			  * The method returns whether or not any mappings were removed
			  * from the hash map.
			  */
			OM_INLINE Bool removeAll( HashType keyHash, const K& key )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				Entry** previousNext = bucket;
				Entry* entry = *bucket;
				const SizeType oldNumElements = numElements;
				
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key )
					{
						*previousNext = entry->next;
						
						entry->next = NULL;
						util::destruct(entry);
						numElements--;
						
						entry = *previousNext;
						continue;
					}
					
					previousNext = &(*previousNext)->next;
					entry = entry->next;
				}
				
				return oldNumElements != numElements;
			}
			
			
			/// Remove a specific key-value mapping from the hash map.
			/**
			  * The method returns whether or not the key-value pair was successfully
			  * found and removed from the hash map.
			  */
			OM_INLINE Bool remove( HashType keyHash, const K& key, const V& value )
			{
				// Compute the bucket for the new element.
				Entry** bucket = buckets + keyHash % numBuckets;
				Entry** previousNext = bucket;
				Entry* entry = *bucket;
				
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key && entry->value == value )
					{
						*previousNext = entry->next;
						entry->next = NULL;
						
						util::destruct(entry);
						
						numElements--;
						
						return true;
					}
					
					previousNext = &(*previousNext)->next;
					entry = entry->next;
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Clear Method
			
			
			/// Clear all mappings from the hash map.
			OM_INLINE void clear()
			{
				// Delete all entries
				Entry** entry = buckets;
				const Entry* const * const entryEnd = entry + numBuckets;
				
				while ( entry != entryEnd )
				{
					if ( *entry )
					{
						util::destruct(*entry);
						*entry = NULL;
					}
					
					entry++;
				}
				
				numElements = 0;
			}
			
			
		//********************************************************************************
		//******	Contains Methods
			
			
			/// Query whether or not the specified key is contained in a hash map.
			OM_INLINE Bool find( HashType keyHash, const K& key, V*& value ) 
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key )
					{
						value = &entry->value;
						return true;
					}
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
		
			/// Query whether or not the specified key is contained in a hash map.
			OM_INLINE Bool find( HashType keyHash, const K& key, const V*& value ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key )
					{
						value = &entry->value;
						return true;
					}
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			
		
			/// Query whether or not the specified key is contained in a hash map.
			OM_INLINE Bool contains( HashType keyHash, const K& key ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key )
						return true;
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
			/// Query whether or not a particular mapping exists in the hash map.
			OM_INLINE Bool contains( HashType keyHash, const K& key, const V& value ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key && entry->value == value )
						return true;
					
					entry = entry->next;
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return the number of mappings in a hash map.
			OM_INLINE SizeType getSize() const
			{
				return numElements;
			}
			
			
			/// Return whether or not a hash map is empty.
			OM_INLINE Bool isEmpty() const
			{
				return numElements == SizeType(0);
			}
			
			
		//********************************************************************************
		//******	Get Methods
			
			
			/// Return a pointer to the value associated with the given key.
			/**
			  * If the key does not exist in the hash map, a NULL pointer is
			  * returned.
			  */
			OM_INLINE V* get( HashType keyHash, const K& key )
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key )
						return &entry->value;
					
					entry = entry->next;
				}
				
				return NULL;
			}
			
			
			/// Return a const pointer to the value associated with the given key.
			/**
			  * If the key does not exist in the hash map, a NULL pointer is
			  * returned.
			  */
			OM_INLINE const V* get( HashType keyHash, const K& key ) const
			{
				// Compute the bucket for the query.
				Entry* entry = *(buckets + keyHash % numBuckets);
				
				// Look for the key in the bucket.
				while ( entry )
				{
					if ( entry->keyHash == keyHash && entry->key == key )
						return &entry->value;
					
					entry = entry->next;
				}
				
				return NULL;
			}
			
			
		//********************************************************************************
		//******	Bucket Iterator Class
			
			
			/// A class that iterates over hash map elements.
			class BucketIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash map iterator for the specified hash map.
						OM_INLINE BucketIterator( HashMap& newHashMap, Index bucketIndex )
							:	bucket( newHashMap.buckets + bucketIndex ),
								hashMap( newHashMap )
						{
							currentEntry = *bucket;
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
						}
						
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash map
						  * has been iterated over.
						  */
						OM_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the key-value pair pointed to by the iterator.
						OM_INLINE V& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element value
						OM_INLINE V* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						
						/// Get the value of the key-value pair pointed to by the iterator.
						OM_INLINE V& getValue() const
						{
							return currentEntry->value;
						}
						
						
						
						/// Get the key of the key-value pair pointed to by the iterator.
						OM_INLINE K& getKey() const
						{
							return currentEntry->key;
						}
						
						
						
						/// Get the key hash of the key-value pair pointed to by the iterator.
						OM_INLINE HashType getKeyHash() const
						{
							return currentEntry->keyHash;
						}
						
						
						
						
						/// Remove the current element from the hash table.
						OM_INLINE void remove()
						{
							// Backup in the bucket so that we can remove the current element.
							// This is potentially inefficient, it would be best if the buckets
							// would use a doublely linked list, but this might add extra overhead
							// elsewhere.
							
							// Handle removing from the start of a bucket separately.
							if ( currentEntry == *bucket )
							{
								// Update the start node of the bucket.
								*bucket = currentEntry->next;
								
								// Destroy removed entry.
								currentEntry->next = NULL;
								util::destruct( currentEntry );
								
								// Update current entry.
								currentEntry = *bucket;
							}
							else
							{
								// Otherwise, iterate through the bucket until we find the element
								// before this one.
								Entry* previousEntry = *bucket;
								
								while ( previousEntry && previousEntry->next != currentEntry )
									previousEntry = previousEntry->next;
								
								previousEntry->next = currentEntry->next;
								Entry* temp = currentEntry;
								operator++();
								temp->next = NULL;
								util::destruct( temp );
							}
							
							hashMap.numElements--;
						}
						
						
						
						/// Reset the iterator to the beginning of the bucket.
						OM_INLINE void reset()
						{
							currentEntry = *bucket;
						}
						
						
						
						
				private:
					
					//********************************************
					//	Private Data Members
						
						
						/// The bucket in the HashMap that is being iterated over.
						Entry** bucket;
						
						
						/// The current entry in the hash map that the iterator is pointing to.
						Entry* currentEntry;
						
						
						/// The HashMap that is being iterated over.
						HashMap& hashMap;
						
						
			};
			
			
		//********************************************************************************
		//******	Bucket Iterator Class
			
			
			/// A class that iterates over hash map elements but cannot modify the hash map.
			class ConstBucketIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash map iterator for the specified hash map.
						OM_INLINE ConstBucketIterator( const HashMap& newHashMap, Index bucketIndex )
							:	bucket( newHashMap.buckets + bucketIndex ),
								hashMap( newHashMap )
						{
							currentEntry = *bucket;
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
						}
						
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash map
						  * has been iterated over.
						  */
						OM_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the key-value pair pointed to by the iterator.
						OM_INLINE const V& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element value
						OM_INLINE const V* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						
						/// Get the value of the key-value pair pointed to by the iterator.
						OM_INLINE const V& getValue() const
						{
							return currentEntry->value;
						}
						
						
						
						/// Get the key of the key-value pair pointed to by the iterator.
						OM_INLINE const K& getKey() const
						{
							return currentEntry->key;
						}
						
						
						
						/// Get the key hash of the key-value pair pointed to by the iterator.
						OM_INLINE HashType getKeyHash() const
						{
							return currentEntry->keyHash;
						}
						
						
						
						/// Reset the iterator to the beginning of the bucket.
						OM_INLINE void reset()
						{
							currentEntry = *bucket;
						}
						
						
						
						
				private:
					
					//********************************************
					//	Private Data Members
						
						
						/// The bucket in the HashMap that is being iterated over.
						Entry** bucket;
						
						
						/// The current entry in the hash map that the iterator is pointing to.
						Entry* currentEntry;
						
						
						/// The HashMap that is being iterated over.
						const HashMap& hashMap;
						
						
			};
			
			
		//********************************************************************************
		//******	Iterator Class
			
			
			/// A class that iterates over hash map elements.
			class Iterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash map iterator for the specified hash map.
						OM_INLINE Iterator( HashMap& newHashMap )
							:	currentBucket( newHashMap.buckets ),
								bucketsEnd( newHashMap.buckets + newHashMap.numBuckets ),
								hashMap( newHashMap )
						{
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
							
							if ( currentEntry == NULL )
							{
								currentBucket++;
								advanceToNextFullBucket();
							}
						}
						
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash map
						  * has been iterated over.
						  */
						OM_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the key-value pair pointed to by the iterator.
						OM_INLINE V& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element value
						OM_INLINE V* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						
						/// Get the value of the key-value pair pointed to by the iterator.
						OM_INLINE V& getValue() const
						{
							return currentEntry->value;
						}
						
						
						
						/// Get the key of the key-value pair pointed to by the iterator.
						OM_INLINE K& getKey() const
						{
							return currentEntry->key;
						}
						
						
						
						/// Get the key hash of the key-value pair pointed to by the iterator.
						OM_INLINE HashType getKeyHash() const
						{
							return currentEntry->keyHash;
						}
						
						
						
						
						/// Remove the current element from the hash table.
						OM_INLINE void remove()
						{
							// Backup in the bucket so that we can remove the current element.
							// This is potentially inefficient, it would be best if the buckets
							// would use a doublely linked list, but this might add extra overhead
							// elsewhere.
							
							// Handle removing from the start of a bucket separately.
							if ( currentEntry == *currentBucket )
							{
								*currentBucket = currentEntry->next;
								
								if ( *currentBucket != NULL )
								{
									currentEntry->next = NULL;
									util::destruct( currentEntry );
									currentEntry = *currentBucket;
								}
								else
								{
									util::destruct( currentEntry );
									currentBucket++;
									
									advanceToNextFullBucket();
								}
							}
							else
							{
								// Otherwise, iterate through the bucket until we find the element
								// before this one.
								Entry* previousEntry = *currentBucket;
								
								while ( previousEntry && previousEntry->next != currentEntry )
									previousEntry = previousEntry->next;
								
								previousEntry->next = currentEntry->next;
								Entry* temp = currentEntry;
								operator++();
								temp->next = NULL;
								util::destruct( temp );
							}
							
							hashMap.numElements--;
						}
						
						
						
						/// Reset the iterator to the beginning of the hash map.
						OM_INLINE void reset()
						{
							currentBucket = hashMap.buckets;
							
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
				private:
					
					//********************************************
					//	Private Methods
						
						
						
						
						/// Advance the iterator to the next non-empty bucket.
						OM_INLINE void advanceToNextFullBucket()
						{
							while ( *currentBucket == NULL && currentBucket != bucketsEnd )
								currentBucket++;
							
							if ( currentBucket == bucketsEnd )
								currentEntry = NULL;
							else
								currentEntry = *currentBucket;
						}
						
						
						
						
					//********************************************
					//	Private Data Members
						
						
						
						
						/// The current bucket in the HashMap.
						Entry** currentBucket;
						
						
						
						
						/// The last bucket in the HashMap.
						const Entry* const * const bucketsEnd;
						
						
						
						
						/// The current entry in the hash map that the iterator is pointing to.
						Entry* currentEntry;
						
						
						
						
						/// The HashMap that is being iterated over.
						HashMap& hashMap;
						
						
						
			};
			
			
		//********************************************************************************
		//******	ConstIterator Class
			
			
			/// A class that iterates over hash map elements without the ability to modify them.
			class ConstIterator
			{
				public:
					
					//********************************************
					//	Constructor
						
						
						
						/// Create a new hash map iterator for the specified hash map.
						OM_INLINE ConstIterator( const HashMap& newHashMap )
							:	currentBucket( newHashMap.buckets ),
								bucketsEnd( newHashMap.buckets + newHashMap.numBuckets ),
								hashMap( newHashMap )
						{
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
					//********************************************
					//	Public Methods
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ()
						{
							currentEntry = currentEntry->next;
							
							if ( currentEntry == NULL )
							{
								currentBucket++;
								
								advanceToNextFullBucket();
							}
						}
						
						
						
						
						/// Increment the location of a hash map iterator by one element.
						OM_INLINE void operator ++ ( int )
						{
							this->operator++();
						}
						
						
						
						
						/// Test whether or not the current element is valid.
						/**
						  * This will return FALSE when the last element of the hash map
						  * has been iterated over.
						  */
						OM_INLINE operator Bool () const
						{
							return currentEntry != NULL;
						}
						
						
						
						/// Return the value of the key-value pair pointed to by the iterator.
						OM_INLINE const V& operator * () const
						{
							return currentEntry->value;
						}
						
						
						
						
						/// Access the current iterator element value
						OM_INLINE const V* operator -> () const
						{
							return &currentEntry->value;
						}
						
						
						
						/// Get the value of the key-value pair pointed to by the iterator.
						OM_INLINE const V& getValue() const
						{
							return currentEntry->value;
						}
						
						
						
						/// Get the key of the key-value pair pointed to by the iterator.
						OM_INLINE const K& getKey() const
						{
							return currentEntry->key;
						}
						
						
						
						/// Get the key hash of the key-value pair pointed to by the iterator.
						OM_INLINE HashType getKeyHash() const
						{
							return currentEntry->keyHash;
						}
						
						
						
						/// Reset the iterator to the beginning of the hash map.
						OM_INLINE void reset()
						{
							currentBucket = hashMap.buckets;
							
							// advance until the first element
							advanceToNextFullBucket();
						}
						
						
						
						
				private:
					
					//********************************************
					//	Private Methods
						
						
						
						
						/// Advance the iterator to the next non-empty bucket.
						OM_INLINE void advanceToNextFullBucket()
						{
							while ( *currentBucket == NULL && currentBucket != bucketsEnd )
								currentBucket++;
							
							if ( currentBucket == bucketsEnd )
								currentEntry = NULL;
							else
								currentEntry = *currentBucket;
						}
						
						
						
						
					//********************************************
					//	Private Data Members
						
						
						
						
						/// The current bucket in the HashMap.
						const Entry* const * currentBucket;
						
						
						
						
						/// The last bucket in the HashMap.
						const Entry* const * bucketsEnd;
						
						
						
						
						/// The current entry in the hash map that the iterator is pointing to.
						const Entry* currentEntry;
						
						
						
						
						/// The HashMap that is being iterated over.
						const HashMap& hashMap;
						
						
						
			};
			
			
		//********************************************************************************
		//******	Iterator Methods=
			
			
			/// Return an iterator for the hash map that can modify the hash map.
			OM_INLINE Iterator getIterator()
			{
				return Iterator( *this );
			}
			
			
			/// Return a const-iterator for the hash map.
			OM_INLINE ConstIterator getIterator() const
			{
				return ConstIterator( *this );
			}
			
			
			/// Return an iterator for the bucket associated with the specified key hash code.
			OM_INLINE BucketIterator getBucketIterator( HashType keyHash )
			{
				return BucketIterator( *this, keyHash % numBuckets );
			}
			
			
			/// Return an iterator for the bucket associated with the specified key hash code.
			OM_INLINE ConstBucketIterator getBucketIterator( HashType keyHash ) const
			{
				return ConstBucketIterator( *this, keyHash % numBuckets );
			}
			
			
		//********************************************************************************
		//******	Load Factor Accessor Methods
			
			
			OM_INLINE void setLoadFactor( Float newLoadFactor )
			{
				loadFactor = math::clamp( newLoadFactor, 0.1f, 5.0f );
				loadThreshold = SizeType(loadFactor*numBuckets);
				
				// Check the load constraint, if necessary, increase the size of the table.
				if ( numElements > loadThreshold )
					resize( math::nextPowerOf2Prime( numBuckets + 1 ) );
			}
			
			
			OM_INLINE Float getLoadFactor() const
			{
				return loadFactor;
			}
			
			
	private:
			
		//********************************************************************************
		//******	Private Methods
			
			
			void resize( HashType newNumBuckets )
			{
				Entry** oldBuckets = buckets;
				HashType oldNumBuckets = numBuckets;
				
				// initialize all buckets and resize the array
				numBuckets = newNumBuckets;
				loadThreshold = SizeType(loadFactor*newNumBuckets);
				buckets = util::allocate<Entry*>( (Size)newNumBuckets );
				nullBuckets();
				
				// add old elements to the hash map.
				Entry** oldBucket = oldBuckets;
				const Entry* const * const oldBucketsEnd = oldBucket + oldNumBuckets;
				
				while ( oldBucket != oldBucketsEnd )
				{
					Entry* oldEntry = *oldBucket;
					
					while ( oldEntry )
					{
						Entry** bucket = buckets + oldEntry->keyHash % numBuckets;
						
						// Add the old element to the end of the bucket.
						if ( *bucket == NULL )
						{
							*bucket = oldEntry;
							oldEntry = oldEntry->next;
							(*bucket)->next = NULL;
						}
						else
						{
							Entry* entry = *bucket;
							
							while ( entry->next )
								entry = entry->next;
							
							entry->next = oldEntry;
							oldEntry = oldEntry->next;
							entry->next->next = NULL;
						}
					}
					
					oldBucket++;
				}
								
				// deallocate all memory currently used by the old buckets
				util::deallocate(oldBuckets);
			}
			
			
			OM_INLINE void nullBuckets()
			{
				Entry** bucket = buckets;
				const Entry* const * const bucketsEnd = bucket + numBuckets;
				
				while ( bucket != bucketsEnd )
				{
					*bucket = NULL;
					bucket++;
				}
			}
			
			
			OM_INLINE static void deleteBuckets( Entry** buckets, HashType numBuckets )
			{
				// Delete all entries
				Entry** entry = buckets;
				const Entry* const * const entryEnd = entry + numBuckets;
				
				while ( entry != entryEnd )
				{
					if ( *entry )
						util::destruct(*entry);
					
					entry++;
				}
				
				// Delete the bucket array.
				util::deallocate(buckets);
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to an array of pointers to the buckets for this hash map.
			Entry** buckets;
			
			
			/// The total number of buckets that are part of this hash map.
			HashType numBuckets;
			
			
			/// The total number of entries that are stored in this hash map.
			SizeType numElements;
			
			
			/// The maximum number of elements this hash map can have before it is resized.
			/**
			  * This value is computed as the load factor multiplied with the number of buckets.
			  */
			SizeType loadThreshold;
			
			
			/// The fraction of the number of buckets that the hash map can have filled before it resizes.
			Float loadFactor;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			static const HashType DEFAULT_NUMBER_OF_BUCKETS = 19;
			
			static const Float DEFAULT_LOAD_FACTOR;
			
			
			
};




template < typename K, typename V, typename HashType, typename SizeType >
const Float		HashMap<K,V,HashType,SizeType>:: DEFAULT_LOAD_FACTOR = 0.5f;




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

#endif // INCLUDE_OM_HASH_MAP_H
