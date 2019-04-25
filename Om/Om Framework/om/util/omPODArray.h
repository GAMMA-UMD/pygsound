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

#ifndef INCLUDE_OM_POD_ARRAY_H
#define INCLUDE_OM_POD_ARRAY_H


#include "omUtilitiesConfig.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// The template prototype for the PODArray class.
template < typename T, Size dimension = Size(1), typename SizeType = Size, typename AllocatorType = Allocator >
class PODArray;




//********************************************************************************
/// A specialization of the PODArray class with 1 dimension.
template < typename T, typename SizeType, typename AllocatorType >
class PODArray<T,1,SizeType,AllocatorType>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty POD array without any memory allocated.
			OM_INLINE PODArray()
				:	array( NULL ),
					size( 0 ),
					capacity( 0 )
			{
			}
			
			
			/// Create a new POD array of the given size without any memory allocated.
			OM_INLINE PODArray( SizeType newSize )
				:	array( NULL ),
					size( newSize ),
					capacity( 0 )
			{
			}
			
			
			/// Create a new empty POD array that copies the state of another array.
			OM_INLINE PODArray( const PODArray& other )
				:	size( other.size ),
					capacity( other.size )
			{
				if ( other.array )
				{
					array = AllocatorType::template allocate<T>( other.size );
					util::copyPOD( array, other.array, other.size );
				}
				else
					array = NULL;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a POD array, releasing any allocated memory.
			OM_INLINE ~PODArray()
			{
				if ( array )
					AllocatorType::deallocate( array );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another PODArray to this one, copying its elements.
			OM_INLINE PODArray& operator = ( const PODArray& other )
			{
				if ( this != &other )
				{
					this->setSize( other.size );
					
					if ( other.array )
						util::copyPOD( array, other.array, other.size );
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return the current capacity of the array's allocated storage.
			OM_INLINE SizeType getCapacity() const
			{
				return capacity;
			}
			
			
			/// Return the current valid size of the array.
			OM_INLINE SizeType getSize() const
			{
				return size;
			}
			
			
			/// Return the current number of bytes that are internally allocated, the memory usage.
			OM_INLINE SizeType getSizeInBytes() const
			{
				return capacity*sizeof(T);
			}
			
			
			/// Set the current valid size of the array.
			/**
			  * The contents of the resulting buffer are undefined. If the array
			  * was not previously allocated the method does not allocate any memory.
			  */
			OM_INLINE void setSize( SizeType newSize )
			{
				if ( array != NULL && newSize > capacity )
				{
					AllocatorType::deallocate( array );
					array = AllocatorType::template allocate<T>( newSize );
					capacity = newSize;
				}
				
				size = newSize;
			}
			
			
			/// Set the current valid size of the array.
			/**
			  * The old contents are copied and the rest of the memory is uninitialized.
			  * If the array was not previously allocated the method does not allocate any memory.
			  */
			OM_INLINE void setSizeCopy( SizeType newSize )
			{
				if ( array != NULL && newSize > capacity )
				{
					T* oldArray = array;
					array = AllocatorType::template allocate<T>( newSize );
					
					util::copyPOD( array, oldArray, math::min( newSize, size ) );
					
					AllocatorType::deallocate( oldArray );
					capacity = newSize;
				}
				
				size = newSize;
			}
			
			
		//********************************************************************************
		//******	Allocation Methods
			
			
			/// Return whether or not this PODArray's internal memory buffer is currently allocated.
			OM_INLINE Bool isAllocated() const
			{
				return array != NULL;
			}
			
			
			/// Make sure that the internal memory is allocated and has at least the given size.
			/**
			  * If the memory is already allocated with at least the given size,
			  * the method has no effect. If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void allocate( SizeType newSize )
			{
				if ( newSize > capacity )
				{
					if ( array )
						AllocatorType::deallocate( array );
					
					array = AllocatorType::template allocate<T>( newSize );
					capacity = newSize;
				}
				
				size = newSize;
			}
			
			
			/// Make sure that the internal memory is allocated and has at least the given size.
			/**
			  * If the memory is already allocated with at least the given size,
			  * the method has no effect. If new memory is allocated, the contents
			  * from the old array are copied to the new one and the rest of the memory is uninitialized.
			  */
			OM_INLINE void allocateCopy( SizeType newSize )
			{
				if ( newSize > capacity )
				{
					T* oldArray = array;
					array = AllocatorType::template allocate<T>( newSize );
					
					if ( array )
					{
						util::copyPOD( array, oldArray, math::min( newSize, size ) );
						AllocatorType::deallocate( oldArray );
					}
					
					capacity = newSize;
				}
				
				size = newSize;
			}
			
			
			/// Make sure that the internal memory is allocated.
			/**
			  * If the memory is already allocated, the method has no effect.
			  * If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void allocate()
			{
				if ( array == NULL )
				{
					array = AllocatorType::template allocate<T>( size );
					capacity = size;
				}
			}
			
			
			/// Free the memory used by the array but keep its current size.
			/**
			  * If the memory is already allocated, the method has no effect.
			  * If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void deallocate()
			{
				if ( array != NULL )
				{
					AllocatorType::deallocate( array );
					capacity = 0;
					array = NULL;
				}
			}
			
			
		//********************************************************************************
		//******	Accessor Operators
			
			
			/// Return a pointer to the first element in this array.
			OM_INLINE T* getPointer() const
			{
				return array;
			}
			
			
			/// Return a reference to the array element at the specified index.
			OM_INLINE T& operator [] ( SizeType i )
			{
				return array[i];
			}
			
			
			/// Return a const reference to the array element at the specified index.
			OM_INLINE const T& operator [] ( SizeType i ) const
			{
				return array[i];
			}
			
			
			/// Return a reference to the array element at the specified index.
			OM_INLINE T& operator () ( SizeType i )
			{
				return array[i];
			}
			
			
			/// Return a const reference to the array element at the specified index.
			OM_INLINE const T& operator () ( SizeType i ) const
			{
				return array[i];
			}
			
			
		//********************************************************************************
		//******	Zero Method
			
			
			/// Zero the entire contents of the array if it is allocated.
			OM_INLINE void zero()
			{
				if ( array )
					util::zeroPOD( array, size );
			}
			
			
			/// Zero a range of the array if it is allocated.
			OM_INLINE void zero( SizeType startIndex, SizeType number )
			{
				if ( array )
					util::zeroPOD( array + startIndex, size );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the data for this POD array.
			T* array;
			
			
			/// The current size of the array along each dimension.
			SizeType size;
			
			
			/// The capacity of the array.
			SizeType capacity;
			
			
			
};




//********************************************************************************
/// A specialization of the PODArray class with 2 dimensions.
template < typename T, typename SizeType, typename AllocatorType >
class PODArray<T,2,SizeType,AllocatorType>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty POD array without any memory allocated.
			OM_INLINE PODArray()
				:	array( NULL ),
					capacity( 0 )
			{
				size[0] = 0;
				size[1] = 0;
			}
			
			
			/// Create a new POD array of the given sizes without any memory allocated.
			OM_INLINE PODArray( SizeType newSize0, SizeType newSize1 )
				:	array( NULL ),
					capacity( 0 )
			{
				size[0] = newSize0;
				size[1] = newSize1;
			}
			
			
			/// Create a new empty POD array that copies the state of another array.
			OM_INLINE PODArray( const PODArray& other )
				:	capacity( other.capacity )
			{
				size[0] = other.size[0];
				size[1] = other.size[1];
				
				if ( other.array )
				{
					const SizeType numToCopy = other.size[0]*other.size[1];
					array = AllocatorType::template allocate<T>( numToCopy );
					util::copyPOD( array, other.array, numToCopy );
				}
				else
					array = NULL;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a POD array, releasing any allocated memory.
			OM_INLINE ~PODArray()
			{
				if ( array )
					AllocatorType::deallocate( array );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another PODArray to this one, copying its elements.
			OM_INLINE PODArray& operator = ( const PODArray& other )
			{
				if ( this != &other )
				{
					this->setSize( other.size[0], other.size[1] );
					
					if ( other.array )
					{
						this->allocate();
						const SizeType numToCopy = other.size[0]*other.size[1];
						util::copyPOD( array, other.array, numToCopy );
					}
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return the current capacity of the array's allocated storage.
			OM_INLINE SizeType getCapacity() const
			{
				return capacity;
			}
			
			
			/// Return the current size of the array along the given dimension index.
			OM_INLINE SizeType getSize( SizeType dimension ) const
			{
				return size[dimension];
			}
			
			
			/// Return the current number of bytes that are internally allocated, the memory usage.
			OM_INLINE SizeType getSizeInBytes() const
			{
				return capacity*sizeof(T);
			}
			
			
			/// Set the current valid size of the array.
			/**
			  * The contents of the resulting buffer are undefined. If the array
			  * was not previously allocated the method does not allocate any memory.
			  */
			OM_INLINE void setSize( SizeType newSize0, SizeType newSize1 )
			{
				const SizeType requiredCapacity = newSize0*newSize1;
				
				if ( array != NULL && requiredCapacity > capacity )
				{
					AllocatorType::deallocate( array );
					array = AllocatorType::template allocate<T>( requiredCapacity );
					capacity = requiredCapacity;
				}
				
				size[0] = newSize0;
				size[1] = newSize1;
			}
			
			
			/// Set the current valid size of the array.
			/**
			  * The old contents are copied and the rest of the memory is uninitialized.
			  * If the array was not previously allocated the method does not allocate any memory.
			  */
			OM_INLINE void setSizeCopy( SizeType newSize0, SizeType newSize1 )
			{
				const SizeType requiredCapacity = newSize0*newSize1;
				
				if ( array != NULL && requiredCapacity > capacity )
				{
					T* oldArray = array;
					array = AllocatorType::template allocate<T>( requiredCapacity );
					
					const SizeType size0Copy = math::min( size[0], newSize0 );
					const SizeType size1Copy = math::min( size[1], newSize1 );
					
					for ( SizeType i = 0; i < size0Copy; i++ )
						util::copyPOD( array + i*newSize1, oldArray + i*size[1], size1Copy );
					
					AllocatorType::deallocate( oldArray );
					capacity = requiredCapacity;
				}
				
				size[0] = newSize0;
				size[1] = newSize1;
			}
			
			
			/// Set the current valid size of the array, copying and unwrwapping the previous contents around a split point.
			/**
			  * This method is intended for efficient resizing of circular buffers.
			  * The old contents are copied after being unwrapped around the splitting point for each
			  * dimension. The split points indicate the new starting indices of the array. The
			  * array regions before the split points are copied after the end of the previous array data in
			  * the new array.
			  * The rest of the memory is uninitialized.
			  * If the array was not previously allocated the method does not allocate any memory.
			  */
			OM_INLINE void setSizeCopyUnwrap( SizeType newSize0, SizeType newSize1, SizeType split0 = 0, SizeType split1 = 0 )
			{
				const SizeType requiredCapacity = newSize0*newSize1;
				
				if ( array != NULL )
				{
					T* oldArray = array;
					array = AllocatorType::template allocate<T>( requiredCapacity );
					
					const SizeType size0Copy = math::min( size[0], newSize0 );
					const SizeType size1Copy = math::min( size[1], newSize1 );
					split0 = math::min( split0, size0Copy );
					split1 = math::min( split1, size1Copy );
					
					for ( SizeType i = split0; i < size0Copy; i++ )
					{
						SizeType afterSplit0 = i - split0;
						SizeType afterSplit1 = size1Copy - split1;
						util::copyPOD( array + afterSplit0*newSize1, oldArray + i*size[1] + split1, afterSplit1 );
						util::copyPOD( array + afterSplit0*newSize1 + afterSplit1, oldArray + i*size[1], split1 );
					}
					
					for ( SizeType i = 0; i < split0; i++ )
					{
						SizeType afterSplit0 = i + (size0Copy - split0);
						SizeType afterSplit1 = size1Copy - split1;
						util::copyPOD( array + afterSplit0*newSize1, oldArray + i*size[1] + split1, afterSplit1 );
						util::copyPOD( array + afterSplit0*newSize1 + afterSplit1, oldArray + i*size[1], split1 );
					}
					
					AllocatorType::deallocate( oldArray );
					capacity = requiredCapacity;
				}
				
				size[0] = newSize0;
				size[1] = newSize1;
			}
			
			
		//********************************************************************************
		//******	Allocation Methods
			
			
			/// Return whether or not this PODArray's internal memory buffer is currently allocated.
			OM_INLINE Bool isAllocated() const
			{
				return array != NULL;
			}
			
			
			/// Make sure that the internal memory is allocated and has at least the given size.
			/**
			  * If the memory is already allocated with at least the given size,
			  * the method has no effect. If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void allocate( SizeType newSize0, SizeType newSize1 )
			{
				const SizeType requiredSize = newSize0*newSize1;
				
				if ( requiredSize > capacity )
				{
					if ( array )
						AllocatorType::deallocate( array );
					
					array = AllocatorType::template allocate<T>( requiredSize );
					capacity = requiredSize;
				}
				
				size[0] = newSize0;
				size[1] = newSize1;
			}
			
			
			/// Make sure that the internal memory is allocated.
			/**
			  * If the memory is already allocated, the method has no effect.
			  * If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void allocate()
			{
				if ( array == NULL )
				{
					capacity = size[0]*size[1];
					array = AllocatorType::template allocate<T>( capacity );
				}
			}
			
			
			/// Free the memory used by the array but keep its current size.
			/**
			  * If the memory is already allocated, the method has no effect.
			  * If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void deallocate()
			{
				if ( array != NULL )
				{
					AllocatorType::deallocate( array );
					capacity = 0;
					array = NULL;
				}
			}
			
			
		//********************************************************************************
		//******	Accessor Operators
			
			
			/// Return a pointer to the first element in this array.
			OM_INLINE T* getPointer() const
			{
				return array;
			}
			
			
			/// Return a reference to the row at the specified index.
			OM_INLINE T* operator [] ( SizeType i )
			{
				return array + i*size[1];
			}
			
			
			/// Return a const reference to the row at the specified index.
			OM_INLINE const T* operator [] ( SizeType i ) const
			{
				return array + i*size[1];
			}
			
			
			/// Return a reference to the row at the specified index.
			OM_INLINE T* operator () ( SizeType i )
			{
				return array + i*size[1];
			}
			
			
			/// Return a const reference to the row at the specified index.
			OM_INLINE const T* operator () ( SizeType i ) const
			{
				return array + i*size[1];
			}
			
			
			/// Return a reference to the array element at the specified 2D index.
			OM_INLINE T& operator () ( SizeType i, SizeType j )
			{
				return array[i*size[1] + j];
			}
			
			
			/// Return a const reference to the array element at the specified 2D index.
			OM_INLINE const T& operator () ( SizeType i, SizeType j ) const
			{
				return array[i*size[1] + j];
			}
			
			
		//********************************************************************************
		//******	Zero Method
			
			
			/// Zero the entire contents of the array if it is allocated.
			OM_INLINE void zero()
			{
				if ( array )
					util::zeroPOD( array, size[0]*size[1] );
			}
			
			
			/// Zero a range of the array for each dimension if it is allocated.
			OM_INLINE void zero( SizeType start0, SizeType number0, SizeType start1, SizeType number1 )
			{
				if ( array )
				{
					// Determine the range of rows to zero.
					T* row = array + start0*size[1];
					T* const rowsEnd = row + number0*size[1];
					
					// Zero the range for each row.
					while ( row != rowsEnd )
					{
						util::zeroPOD( row + start1, number1 );
						row += size[1];
					}
				}
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the data for this POD array.
			T* array;
			
			
			/// The current size of the array along each dimension.
			SizeType size[2];
			
			
			/// The total allocated capacity of the array.
			SizeType capacity;
			
			
			
};




//********************************************************************************
/// A specialization of the PODArray class with 3 dimensions.
template < typename T, typename SizeType, typename AllocatorType >
class PODArray<T,3,SizeType,AllocatorType>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty POD array without any memory allocated.
			OM_INLINE PODArray()
				:	array( NULL ),
					capacity( 0 )
			{
				size[0] = 0;
				size[1] = 0;
				size[2] = 0;
			}
			
			
			/// Create a new POD array of the given sizes without any memory allocated.
			OM_INLINE PODArray( SizeType newSize0, SizeType newSize1, SizeType newSize2 )
				:	array( NULL ),
					capacity( 0 )
			{
				size[0] = newSize0;
				size[1] = newSize1;
				size[2] = newSize2;
			}
			
			
			/// Create a new empty POD array that copies the state of another array.
			OM_INLINE PODArray( const PODArray& other )
				:	capacity( other.capacity )
			{
				size[0] = other.size[0];
				size[1] = other.size[1];
				size[2] = other.size[2];
				
				if ( other.array )
				{
					const SizeType numToCopy = other.size[0]*other.size[1]*other.size[2];
					array = AllocatorType::template allocate<T>( numToCopy );
					util::copyPOD( array, other.array, numToCopy );
				}
				else
					array = NULL;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a POD array, releasing any allocated memory.
			OM_INLINE ~PODArray()
			{
				if ( array )
					AllocatorType::deallocate( array );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another PODArray to this one, copying its elements.
			OM_INLINE PODArray& operator = ( const PODArray& other )
			{
				if ( this != &other )
				{
					this->setSize( other.size[0], other.size[1], other.size[2] );
					
					if ( other.array )
					{
						this->allocate();
						const SizeType numToCopy = other.size[0]*other.size[1]*other.size[2];
						util::copyPOD( array, other.array, numToCopy );
					}
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return the current capacity of the array's allocated storage.
			OM_INLINE SizeType getCapacity() const
			{
				return capacity;
			}
			
			
			/// Return the current size of the array along the given dimension index.
			OM_INLINE SizeType getSize( SizeType dimension ) const
			{
				return size[dimension];
			}
			
			
			/// Return the current number of bytes that are internally allocated, the memory usage.
			OM_INLINE SizeType getSizeInBytes() const
			{
				return capacity*sizeof(T);
			}
			
			
			/// Set the current valid size of the array.
			/**
			  * The contents of the resulting buffer are undefined. If the array
			  * was not previously allocated the method does not allocate any memory.
			  */
			OM_INLINE void setSize( SizeType newSize0, SizeType newSize1, SizeType newSize2 )
			{
				const SizeType requiredCapacity = newSize0*newSize1*newSize2;
				
				if ( array != NULL && requiredCapacity > capacity )
				{
					AllocatorType::deallocate( array );
					array = AllocatorType::template allocate<T>( requiredCapacity );
					capacity = requiredCapacity;
				}
				
				size[0] = newSize0;
				size[1] = newSize1;
				size[2] = newSize2;
			}
			
			
			/// Set the current valid size of the array.
			/**
			  * The old contents are copied and the rest of the memory is uninitialized.
			  * If the array was not previously allocated the method does not allocate any memory.
			  */
			OM_INLINE void setSizeCopy( SizeType newSize0, SizeType newSize1, SizeType newSize2 )
			{
				const SizeType requiredCapacity = newSize0*newSize1*newSize2;
				
				if ( array != NULL && requiredCapacity > capacity )
				{
					T* oldArray = array;
					array = AllocatorType::template allocate<T>( requiredCapacity );
					
					const SizeType size0Copy = math::min( size[0], newSize0 );
					const SizeType size1Copy = math::min( size[1], newSize1 );
					const SizeType size2Copy = math::min( size[2], newSize2 );
					const SizeType newPlaneSize = newSize1*newSize2;
					const SizeType oldPlaneSize = size[1]*size[2];
					
					for ( SizeType i = 0; i < size0Copy; i++ )
					{
						for ( SizeType j = 0; j < size1Copy; j++ )
						{
							util::copyPOD( array + i*newPlaneSize + j*newSize2,
											oldArray + i*oldPlaneSize + j*size[2],
											size2Copy );
						}
					}
					
					AllocatorType::deallocate( oldArray );
					capacity = requiredCapacity;
				}
				
				size[0] = newSize0;
				size[1] = newSize1;
				size[2] = newSize2;
			}
			
			
		//********************************************************************************
		//******	Allocation Methods
			
			
			/// Return whether or not this PODArray's internal memory buffer is currently allocated.
			OM_INLINE Bool isAllocated() const
			{
				return array != NULL;
			}
			
			
			/// Make sure that the internal memory is allocated and has at least the given size.
			/**
			  * If the memory is already allocated with at least the given size,
			  * the method has no effect. If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void allocate( SizeType newSize0, SizeType newSize1, SizeType newSize2 )
			{
				const SizeType requiredSize = newSize0*newSize1*newSize2;
				
				if ( requiredSize > capacity )
				{
					if ( array )
						AllocatorType::deallocate( array );
					
					array = AllocatorType::template allocate<T>( requiredSize );
					capacity = requiredSize;
				}
				
				size[0] = newSize0;
				size[1] = newSize1;
				size[2] = newSize2;
			}
			
			
			/// Make sure that the internal memory is allocated.
			/**
			  * If the memory is already allocated, the method has no effect.
			  * If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void allocate()
			{
				if ( array == NULL )
				{
					capacity = size[0]*size[1]*size[2];
					array = AllocatorType::template allocate<T>( capacity );
				}
			}
			
			
			/// Free the memory used by the array but keep its current size.
			/**
			  * If the memory is already allocated, the method has no effect.
			  * If new memory is allocated, its contents are undefined.
			  */
			OM_INLINE void deallocate()
			{
				if ( array != NULL )
				{
					AllocatorType::deallocate( array );
					capacity = 0;
					array = NULL;
				}
			}
			
			
		//********************************************************************************
		//******	Accessor Operators
			
			
			/// Return a pointer to the first element in this array.
			OM_INLINE T* getPointer() const
			{
				return array;
			}
			
			
			/// Return a reference to the 2D array at the specified index.
			OM_INLINE T* operator [] ( SizeType i )
			{
				return array + i*size[1]*size[2];
			}
			
			
			/// Return a const reference to the 2D array at the specified index.
			OM_INLINE const T* operator [] ( SizeType i ) const
			{
				return array + i*size[1]*size[2];
			}
			
			
			/// Return a reference to the 2D array at the specified index.
			OM_INLINE T* operator () ( SizeType i )
			{
				return array + i*size[1]*size[2];
			}
			
			
			/// Return a const reference to the 2D array at the specified index.
			OM_INLINE const T* operator () ( SizeType i ) const
			{
				return array + i*size[1]*size[2];
			}
			
			
			/// Return a reference to the row at the specified 2D index.
			OM_INLINE T* operator () ( SizeType i, SizeType j )
			{
				return array + i*size[1]*size[2] + j*size[2];
			}
			
			
			/// Return a const reference to the row at the specified 2D index.
			OM_INLINE const T* operator () ( SizeType i, SizeType j ) const
			{
				return array + i*size[1]*size[2] + j*size[2];
			}
			
			
			/// Return a reference to the array element at the specified 3D index.
			OM_INLINE T& operator () ( SizeType i, SizeType j, SizeType k )
			{
				return array[i*size[1]*size[2] + j*size[2] + k];
			}
			
			
			/// Return a const reference to the array element at the specified 3D index.
			OM_INLINE const T& operator () ( SizeType i, SizeType j, SizeType k ) const
			{
				return array[i*size[1]*size[2] + j*size[2] + k];
			}
			
			
		//********************************************************************************
		//******	Zero Method
			
			
			/// Zero the entire contents of the array if it is allocated.
			OM_INLINE void zero()
			{
				if ( array )
					util::zeroPOD( array, size[0]*size[1]*size[2] );
			}
			
			
			/// Zero a range of the array for each dimension if it is allocated.
			OM_INLINE void zero( SizeType start0, SizeType number0, SizeType start1, SizeType number1, SizeType start2, SizeType number2 )
			{
				if ( array )
				{
					const SizeType planeSize = size[1]*size[2];
					T* plane = array + start0*planeSize;
					T* const planesEnd = plane + number0*planeSize;
					
					while ( plane != planesEnd )
					{
						T* row = plane + start1*size[2];
						T* const rowsEnd = row + number1*size[2];
						
						while ( row != rowsEnd )
						{
							util::zeroPOD( row + start2, number2 );
							row += size[2];
						}
						
						plane += planeSize;
					}
				}
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the data for this POD array.
			T* array;
			
			
			/// The current size of the array along each dimension.
			SizeType size[3];
			
			
			/// The total allocated capacity of the array.
			SizeType capacity;
			
			
			
};




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_POD_ARRAY_H
