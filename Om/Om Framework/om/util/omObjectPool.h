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

#ifndef INCLUDE_OM_OBJECT_POOL_H
#define INCLUDE_OM_OBJECT_POOL_H


#include "omUtilitiesConfig.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that manages a pool of objects that are stored at persistent locations in memory.
/**
  * The object pool allocates blocks of objects of a fixed power-of-two size contiguously
  * in memory for better cache performance than if each object were allocated separately.
  * A single contiguous array of objects cannot be used because it would cause objects
  * to be reallocated if the array grows in size.
  */
template < typename T, Size blockSizeLog2 = Size(3), typename SizeType = Size, typename AllocatorType = Allocator >
class ObjectPool
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty object pool without any memory allocated.
			OM_INLINE ObjectPool()
				:	size( 0 )
			{
			}
			
			
			/// Create a new empty object pool that copies the state of another pool.
			OM_INLINE ObjectPool( const ObjectPool& other )
				:	blocks( other.blocks.getSize() ),
					freeList( other.freeList ),
					size( other.size )
			{
				// Copy the blocks from the other pool.
				const SizeType numBlocks = blocks.getSize();
				
				for ( SizeType i = 0; i < numBlocks; i++ )
				{
					Block* block = AllocatorType::template allocate<Block>();
					new ( block ) Block( *other.blocks[i] );
					blocks.add( block );
				}
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an object pool, releasing any allocated memory and destructing all objects.
			OM_INLINE ~ObjectPool()
			{
				// Free all of the blocks.
				const SizeType numBlocks = blocks.getSize();
				
				for ( SizeType i = 0; i < numBlocks; i++ )
				{
					blocks[i]->~Block();
					AllocatorType::deallocate( blocks[i] );
				}
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another ObjectPool to this one, copying its elements.
			OM_INLINE ObjectPool& operator = ( const ObjectPool& other )
			{
				if ( this != &other )
				{
					// Free all of the previous blocks.
					const SizeType numOldBlocks = blocks.getSize();
					
					for ( SizeType i = 0; i < numOldBlocks; i++ )
					{
						blocks[i]->~Block();
						AllocatorType::deallocate( blocks[i] );
					}
					
					blocks.clear();
					freeList.clear();
					
					// Copy the free list.
					freeList.addAll( other.freeList );
					size = other.size;
					
					// Copy the blocks from the other pool.
					const SizeType numBlocks = blocks.getSize();
					
					for ( SizeType i = 0; i < numBlocks; i++ )
					{
						Block* block = AllocatorType::template allocate<Block>();
						new ( block ) Block( *other.blocks[i] );
						blocks.add( block );
					}
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Method
			
			
			/// Return the current number of valid objects there are in the pool.
			OM_INLINE SizeType getSize() const
			{
				return size;
			}
			
			
			/// Return the current number of objects that the pool can store without allocating more memory.
			OM_INLINE SizeType getCapacity() const
			{
				return blocks.getSize()*BLOCK_SIZE;
			}
			
			
		//********************************************************************************
		//******	Accessor Operators
			
			
			/// Return a reference to the array element at the specified index.
			OM_INLINE T& operator [] ( SizeType objectIndex )
			{
				return *getObject(objectIndex);
			}
			
			
			/// Return a const reference to the array element at the specified index.
			OM_INLINE const T& operator [] ( SizeType objectIndex ) const
			{
				return *getObject(objectIndex);
			}
			
			
			/// Return whether or not the object with the given index is currently valid.
			OM_INLINE Bool isUsed( SizeType objectIndex ) const
			{
				if ( objectIndex >= blocks.getSize()*BLOCK_SIZE )
					return false;
				
				// Get the block from the free object index.
				const Block* block = blocks[getBlockIndex(objectIndex)];
				const SizeType localIndex = getLocalIndex(objectIndex);
				
				return block->valid[localIndex] != 0;
			}
			
			
			/// Return whether or not the object with the given index is currently invalid.
			OM_INLINE Bool isUnused( SizeType objectIndex ) const
			{
				if ( objectIndex >= blocks.getSize()*BLOCK_SIZE )
					return true;
				
				// Get the block from the free object index.
				const Block* block = blocks[getBlockIndex(objectIndex)];
				const SizeType localIndex = getLocalIndex(objectIndex);
				
				return block->valid[localIndex] == 0;
			}
			
			
		//********************************************************************************
		//******	Object Add Methods
			
			
			/// Copy an object and add the copy to the object pool, returning the object's index in the pool.
			OM_INLINE SizeType add( const T& prototype )
			{
				SizeType objectIndex;
				T* object = newObject( objectIndex );
				
				new ( object ) T( prototype );
				return objectIndex;
			}
			
			
			/// Construct an object in-place using the given parameters, returning the object's index in the pool.
			OM_INLINE SizeType addNew()
			{
				SizeType objectIndex;
				
				T* object = newObject( objectIndex );
				
				new ( object ) T();
				return objectIndex;
			}
			
			
			/// Construct an object in-place using the given parameters, returning the object's index in the pool.
			template < typename P1 >
			OM_INLINE SizeType addNew( const P1& p1 )
			{
				SizeType objectIndex;
				T* object = newObject( objectIndex );
				
				new ( object ) T( p1 );
				return objectIndex;
			}
			
			
			/// Construct an object in-place using the given parameters, returning the object's index in the pool.
			template < typename P1, typename P2 >
			OM_INLINE SizeType addNew( const P1& p1, const P2& p2 )
			{
				SizeType objectIndex;
				T* object = newObject( objectIndex );
				
				new ( object ) T( p1, p2 );
				return objectIndex;
			}
			
			
			/// Construct an object in-place using the given parameters, returning the object's index in the pool.
			template < typename P1, typename P2, typename P3 >
			OM_INLINE SizeType addNew( const P1& p1, const P2& p2, const P3& p3 )
			{
				SizeType objectIndex;
				T* object = newObject( objectIndex );
				
				new ( object ) T( p1, p2, p3 );
				return objectIndex;
			}
			
			
			/// Construct an object in-place using the given parameters, returning the object's index in the pool.
			template < typename P1, typename P2, typename P3, typename P4 >
			OM_INLINE SizeType addNew( const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
			{
				SizeType objectIndex;
				T* object = newObject( objectIndex );
				
				new ( object ) T( p1, p2, p3, p4 );
				return objectIndex;
			}
			
			
			/// Construct an object in-place using the given parameters, returning the object's index in the pool.
			template < typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_INLINE SizeType addNew( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
			{
				SizeType objectIndex;
				T* object = newObject( objectIndex );
				
				new ( object ) T( p1, p2, p3, p4, p5 );
				return objectIndex;
			}
			
			
		//********************************************************************************
		//******	Object Remove Methods
			
			
			/// Remove and destroy the object with the specified index in this pool.
			OM_INLINE void remove( SizeType objectIndex )
			{
				// Get the block from the free object index.
				Block* block = blocks[getBlockIndex(objectIndex)];
				SizeType localIndex = getLocalIndex(objectIndex);
				
				// Destruct the object.
				((T*)block->objects + localIndex)->~T();
				
				// Mark the object as invalid.
				block->valid[localIndex] = false;
				
				// Add the index to the free list.
				freeList.add( objectIndex );
				size--;
			}
			
			
			/// Remove all objects from the pool.
			OM_INLINE void clear()
			{
				// Free all of the blocks.
				const SizeType numBlocks = blocks.getSize();
				
				for ( SizeType i = 0; i < numBlocks; i++ )
				{
					blocks[i]->~Block();
					AllocatorType::deallocate( blocks[i] );
				}
				
				blocks.clear();
				freeList.clear();
				size = 0;
			}
			
			
		//********************************************************************************
		//******	Iterator Class
			
			
			/// A class that iterates over the valid objects in an object pool.
			class Iterator
			{
				public:
					
					/// Create a new hash map iterator for the specified hash map.
					OM_INLINE Iterator( ObjectPool& newPool )
						:	pool( newPool ),
							blockIndex( 0 )
					{
						if ( pool.blocks.getSize() == 0 )
							blockObject = NULL;
						else
						{
							updateBlock();
							findNextValidObject();
						}
					}
					
					
					/// Increment the location of an object pool iterator by one element.
					OM_INLINE void operator ++ ()
					{
						blockValid++;
						blockObject++;
						findNextValidObject();
					}
					
					
					/// Increment the location of an object pool iterator by one element.
					OM_INLINE void operator ++ ( int )
					{
						this->operator++();
					}
					
					
					/// Test whether or not the current element is valid.
					/**
					  * This will return FALSE when the last element of the object pool
					  * has been iterated over.
					  */
					OM_INLINE operator Bool () const
					{
						return blockObject != NULL;
					}
					
					
					/// Return a reference to the current object that the iterater points to.
					OM_INLINE T& operator * () const
					{
						return *blockObject;
					}
					
					
					/// Return a pointer to the current object that the iterater points to.
					OM_INLINE T* operator -> () const
					{
						return blockObject;
					}
					
					
					/// Remove the current element from the object pool.
					OM_INLINE void remove()
					{
						// Destruct the object.
						blockObject->~T();
						
						// Mark the object as invalid.
						*blockValid = false;
						
						// Add the index to the free list.
						pool.freeList.add( this->getIndex() );
						pool.size--;
						
						// Move to the next object.
						blockValid++;
						blockObject++;
						findNextValidObject();
					}
					
					
					/// Reset the iterator to the beginning of the object pool.
					OM_INLINE void reset()
					{
						blockIndex = 0;
						
						if ( pool.blocks.getSize() == 0 )
							blockObject = NULL;
						else
						{
							updateBlock();
							findNextValidObject();
						}
					}
					
					
					/// Return the index of the current object that the iterator points to.
					OM_INLINE SizeType getIndex() const
					{
						return (blockIndex << SizeType(blockSizeLog2)) + (BLOCK_SIZE - SizeType(blockValidEnd - blockValid));
					}
					
					
				private:
					
					/// Update the iteration variables to start at the beginning of the current block.
					OM_INLINE void updateBlock()
					{
						Block* block = pool.blocks[blockIndex];
						blockValid = block->valid;
						blockValidEnd = block->valid + BLOCK_SIZE;
						blockObject = (T*)block->objects;
					}
					
					
					/// Advance the iterator until the next valid object in the object pool.
					OM_FORCE_INLINE void findNextValidObject()
					{
						newBlock:
						
						// Find the next valid object in the current block.
						while ( blockValid != blockValidEnd )
						{
							// If we found a valid object, return.
							if ( *blockValid != 0 )
								return;
							
							blockValid++;
							blockObject++;
						}
						
						// Go to the next block.
						blockIndex++;
						
						// If there are no more blocks, indicate that the end is reached and return.
						if ( blockIndex == pool.blocks.getSize() )
						{
							blockObject = NULL;
							return;
						}
						
						// Update block info.
						updateBlock();
						
						// Try the next block.
						goto newBlock;
					}
					
					
					/// A pointer to the next object in the current block.
					T* blockObject;
					
					
					/// A pointer to the next object's valid status in the current block.
					UByte* blockValid;
					
					
					/// A pointer to the end of the current block's valid status array.
					const UByte* blockValidEnd;
					
					
					/// The index of the current block that the iterator is iterating over.
					SizeType blockIndex;
					
					
					/// The object pool that is being iterated over.
					ObjectPool& pool;
					
			};
			
			
		//********************************************************************************
		//******	Iterator Accessor Methods
			
			
			/// Return an object that iterates over the valid objects in this object pool.
			OM_INLINE Iterator getIterator()
			{
				return Iterator(*this);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The size of a contiguous block of objects in this object pool.
			static const SizeType BLOCK_SIZE = SizeType(1) << SizeType(blockSizeLog2);
			
			
			/// A mask that masks out the low-order bits of an object index (its block-local index).
			static const SizeType LOCAL_MASK = BLOCK_SIZE - SizeType(1);
			
			
			/// A mask that masks out the high-order bits of an object index (its block index).
			static const SizeType BLOCK_MASK = ~LOCAL_MASK;
			
			
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that stores contiguously allocated objects in a fixed-size block.
			class Block
			{
				public:
					
					/// Create a new block with no valid objects.
					OM_INLINE Block()
					{
						// Set all validity indicators to the unused state.
						util::zeroPOD( valid, BLOCK_SIZE );
					}
					
					/// Construct a copy of another block, copying its valid objects.
					OM_INLINE Block( const Block& other )
					{
						for ( SizeType i = 0; i < BLOCK_SIZE; i++ )
						{
							valid[i] = other.valid[i];
							
							if ( other.valid[i] )
								new ( (T*)objects + i ) T( ((T*)other.objects)[i] );
						}
					}
					
					/// Destroy a block, destructing all valid objects.
					OM_INLINE ~Block()
					{
						for ( SizeType i = 0; i < BLOCK_SIZE; i++ )
						{
							if ( valid[i] )
								((T*)objects + i)->~T();
						}
					}
					
					
					/// Contiguous memory storage for a block's worth of objects.
					UByte objects[BLOCK_SIZE*sizeof(T)];
					
					/// A pointer to an array of bytes of the block length that indicate the valid objects.
					UByte valid[BLOCK_SIZE];
					
					
			};
			
			
		//********************************************************************************
		//******	Private Methods
			
			
			/// Return a pointer to an unused memory location where a new object can be constructed.
			OM_INLINE T* newObject( SizeType& objectIndex )
			{
				// If there are any objects in the free list, use them first.
				if ( freeList.getSize() > 0 )
				{
					objectIndex = freeList.getLast();
					freeList.removeLast();
					
					// Get the block from the free object index.
					Block* block = blocks[getBlockIndex(objectIndex)];
					SizeType localIndex = getLocalIndex(objectIndex);
					
					// Mark the object as valid and return a pointer to the memory.
					block->valid[localIndex] = true;
					size++;
					return ((T*)block->objects) + localIndex;
				}
				else
				{
					// All previous objects are valid.
					SizeType localIndex = size & LOCAL_MASK;
					
					// Determine the final index of the object, the current size.
					objectIndex = size;
					size++;
					
					if ( localIndex == 0 )
					{
						// The last block is full, add a new one.
						Block* newBlock = AllocatorType::template allocate<Block>();
						new ( newBlock ) Block();
						blocks.add( newBlock );
						
						// Mark the object as valid and return a pointer to the memory.
						newBlock->valid[0] = true;
						return (T*)newBlock->objects;
					}
					else
					{
						// Get the last block.
						Block* block = blocks.getLast();
						
						// Mark the object as valid and return a pointer to the memory.
						block->valid[localIndex] = true;
						return (T*)block->objects + localIndex;
					}
				}
			}
			
			
			/// Return a pointer to the object at the specified index in the pool.
			OM_INLINE T* getObject( SizeType objectIndex ) const
			{
				return (T*)(blocks[getBlockIndex(objectIndex)]->objects) + getLocalIndex(objectIndex);
			}
			
			
			/// Return the block index for the specified object index.
			OM_INLINE static SizeType getBlockIndex( SizeType index )
			{
				return (index & BLOCK_MASK) >> blockSizeLog2;
			}
			
			
			/// Return the block-local index for the specified object index.
			OM_INLINE static SizeType getLocalIndex( SizeType index )
			{
				return (index & LOCAL_MASK);
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of pointers to contiguous blocks of objects.
			ArrayList<Block*,SizeType> blocks;
			
			
			/// A list of the unused object indices in this pool.
			ArrayList<SizeType,SizeType> freeList;
			
			
			/// The number of valid objects in this object pool.
			SizeType size;
			
			
			
};




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_OBJECT_POOL_H
