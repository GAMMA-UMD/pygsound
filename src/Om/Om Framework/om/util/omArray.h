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

#ifndef INCLUDE_OM_ARRAY_H
#define INCLUDE_OM_ARRAY_H


#include "omUtilitiesConfig.h"


#include "omAllocator.h"
#include "omCopy.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents a dynamically-sized sequence of objects stored contiguously in memory.
template < class T, typename SizeType = Size, typename AllocatorType = Allocator >
class Array
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create an empty array. This constructor does not allocate any memory.
			OM_INLINE Array()
				:	pointer( NULL ),
					size( 0 )
			{
			}
			
			
			/// Create an array of the specified size with default-constructed elements.
			OM_INLINE explicit Array( SizeType arraySize )
				:	pointer( AllocatorType::template allocate<T>(arraySize) ),
					size( arraySize )
			{
				Allocator::constructArray( pointer, arraySize );
			}
			
			
			/// Create an array of the specified size with elements created from the specified prototype.
			OM_INLINE explicit Array( SizeType arraySize, const T& prototype )
				:	pointer( AllocatorType::template allocate<T>(arraySize) ),
					size( arraySize )
			{
				Allocator::constructArray( pointer, arraySize, prototype );
			}
			
			
			/// Create an array which uses the specified pointer to a block of memory.
			/**
			  * After calling this constructor, the Array now owns the pointer and will
			  * automatically delete it when the array is destructed.
			  */
			OM_INLINE explicit Array( T* newArray, SizeType arraySize )
				:	pointer( newArray ),
					size( newArray == NULL ? 0 : arraySize )
			{
			}
			
			
			/// Create an array with elements from the specified pointer, copying the data.
			OM_INLINE explicit Array( const T* newArray, SizeType arraySize )
			{
				if ( newArray != NULL )
				{
					pointer = AllocatorType::template allocate<T>( arraySize );
					Allocator::copyArray( pointer, newArray, arraySize );
					size = arraySize;
				}
				else
				{
					pointer = NULL;
					size = 0;
				}
			}
			
			
		//********************************************************************************
		//******	Copy Constructors
			
			
			/// Create a deep copy of the specified array object.
			OM_INLINE Array( const Array& other )
				:	pointer( AllocatorType::template allocate<T>( other.size ) ),
					size( other.size )
			{
				Allocator::copyArray( pointer, other.pointer, other.size );
			}
			
			
			/// Create a deep copy of the specified array object, using only the specified number of elements.
			OM_INLINE Array( const Array& other, SizeType number )
				:	size( number < other.size ? number : other.size )
			{
				pointer = AllocatorType::template allocate<T>( size );
				Allocator::copyArray( pointer, other.pointer, size );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Copy the contents from another array into this array, replacing the current contents.
			OM_INLINE Array& operator = ( const Array& other )
			{
				if ( this != &other )
				{
					// Clean up the previous array if necessary.
					if ( pointer != NULL )
					{
						Allocator::destructArray( pointer, size );
						AllocatorType::deallocate( pointer );
					}
					
					// Allocate a new array and copy the other array's objects.
					pointer = AllocatorType::template allocate<T>( other.size );
					Allocator::copyArray( pointer, other.pointer, other.size );
					size = other.size;
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			

			
			/// Destroy an array object and deallocate its internal array.
			OM_INLINE ~Array()
			{
				if ( pointer != NULL )
				{
					Allocator::destructArray( pointer, size );
					AllocatorType::deallocate( pointer );
				}
			}
			
			
		//********************************************************************************
		//******	Array Pointer Accessor Methods
			
			
			/// Convert this array to a pointer and return the result.
			OM_INLINE T* getPointer()
			{
				return pointer;
			}
			
			
			/// Convert this array to a pointer and return the result, const version.
			OM_INLINE const T* getPointer() const
			{
				return pointer;
			}
			
			
			/// Convert this array to a pointer.
			OM_INLINE operator T* ()
			{
				return pointer;
			}
			
			
			/// Convert this array to a pointer, const version.
			OM_INLINE operator const T* () const
			{
				return pointer;
			}
			
			
		//********************************************************************************
		//******	Equality Operators
			
			
			/// Compare this array to another array for equality.
			/**
			  * If this array has the same size as the other array and
			  * has the same elements, the arrays are equal and TRUE is
			  * returned. Otherwise, the arrays are not equal and FALSE is returned.
			  */
			OM_INLINE Bool operator == ( const Array& array ) const
			{
				if ( size != array.size )
					return false;
				
				const T* compare1 = pointer;
				const T* compare1End = pointer + size;
				const T* compare2 = array.pointer;
				
				while ( compare1 != compare1End )
				{
					if ( *compare1 != *compare2 )
						return false;
					
					compare1++;
					compare2++;
				}
				
				return true;
			}
			
			
			/// Compare this array to another array for inequality.
			/**
			  * If this array has a different size than the other array or
			  * has different elements, the arrays are not equal and TRUE is
			  * returned. Otherwise, the arrays are equal and FALSE is returned.
			  */
			OM_INLINE Bool operator != ( const Array& array ) const
			{
				return !(*this == array);
			}
			
			
		//********************************************************************************
		//******	Array Concatenation Operators
			
			
			/// Concatenate the contents of this array with another array and return the resulting new array.
			OM_NO_INLINE Array operator + ( const Array& other ) const
			{
				SizeType newArraySize = size + other.size;
				T* newArray = AllocatorType::template allocate<T>( newArraySize );
				T* destination = newArray;
				const T* source1 = pointer;
				const T* const source1End = source1 + size;
				const T* source2 = other.pointer;
				const T* const source2End = source2 + other.size;
				
				while ( source1 != source1End )
				{
					new ( destination ) T( *source1 );
					destination++;
					source1++;
				}
				
				while ( source2 != source2End )
				{
					new ( destination ) T( *source2 );
					destination++;
					source2++;
				}
				
				return Array( newArray, newArraySize );
			}
			
			
		//********************************************************************************
		//******	Array SizeType Accessor Methods
			
			
			/// Get the size of this array.
			OM_INLINE SizeType getSize() const
			{
				return size;
			}
			
			
			/// Resize this array, copying as many elements from the old array to the new array as possible.
			/**
			  * If there are new elements created at the end of the array, they are
			  * default constructed.
			  */
			OM_INLINE void setSize( SizeType newSize )
			{
				this->setSize( newSize, T() );
			}
			
			
			/// Resize this array, copying as many elements from the old array to the new array as possible.
			/**
			  * If there are new elements created at the end of the array, they are
			  * initialize to the specified default value.
			  */
			OM_NO_INLINE void setSize( SizeType newSize, const T& prototype );
			
			
		//********************************************************************************
		//******	Array Element Accessor Methods
			
			
			/// Set all of the values in this array to the specified value.
			OM_INLINE void setAll( const T& prototype )
			{
				util::set( pointer, prototype, size );
			}
			
			
		//********************************************************************************
		//******	Array Trait Accessor Methods
			
			
			/// Return whether or not this array object's internal array is NULL.
			OM_INLINE Bool isNull() const
			{
				return pointer == NULL;
			}
			
			
			/// Return whether or not this array object's internal array is not NULL.
			OM_INLINE Bool isSet() const
			{
				return pointer != NULL;
			}
			
			

			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the array data.
			T* pointer;
			
			
			/// The size of the array.
			SizeType size;
			
			
		//********************************************************************************
		//******	Private Friend Class
			
			
			template < typename U, typename SizeType2, typename AllocatorType2 >
			friend class Array;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Array SizeType Set Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T, typename SizeType, typename AllocatorType >
void Array<T,SizeType,AllocatorType>:: setSize( SizeType newSize, const T& prototype )
{
	if ( size == newSize )
		return;
	
	if ( pointer != NULL )
	{
		// Resize the array.
		T* oldPointer = pointer;
		pointer = AllocatorType::template allocate<T>( newSize );
		
		// Copy the valid array elements.
		const SizeType copyNumber = (size < newSize ? size : newSize);
		Allocator::copyArray( pointer, oldPointer, copyNumber );
		
		// Construct the new elements.
		Allocator::constructArray( pointer + copyNumber, newSize - copyNumber, prototype );
		
		// Destroy the old array.
		Allocator::destructArray( oldPointer, size );
		AllocatorType::deallocate( oldPointer );
	}
	else
	{
		// Construct a new array.
		pointer = AllocatorType::template allocate<T>( newSize );
		Allocator::constructArray( pointer, newSize, prototype );
	}
	
	size = newSize;
}




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_ARRAY_H
