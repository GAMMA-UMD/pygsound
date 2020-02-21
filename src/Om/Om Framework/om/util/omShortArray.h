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

#ifndef INCLUDE_OM_SHORT_ARRAY_H
#define INCLUDE_OM_SHORT_ARRAY_H


#include "omUtilitiesConfig.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a dynamically-sized sequence of objects stored contiguously in memory.
/**
  * This class functions identically to the Array class, except that it has local
  * storage for a small number of objects that are stored as part of the array object,
  * eliminating an array allocation if the number of objects is small.
  */
template < typename T, Size localCapacity = Size(4), typename SizeType = Size >
class ShortArray
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create an empty array. This constructor does not allocate any memory.
			OM_INLINE ShortArray()
				:	pointer( (T*)localStorage ),
					size( 0 )
			{
			}
			
			
			/// Create an array of the specified size with default-constructed elements.
			OM_INLINE explicit ShortArray( SizeType arraySize )
				:	size( arraySize )
			{
				if ( arraySize <= localCapacity )
					pointer = (T*)localStorage;
				else
					pointer = util::allocate<T>( arraySize );
				
				initializeObjects( pointer, arraySize );
			}
			
			
			/// Create an array of the specified size with elements created from the specified prototype.
			OM_INLINE explicit ShortArray( SizeType arraySize, const T& prototype )
				:	size( arraySize )
			{
				if ( arraySize <= localCapacity )
					pointer = (T*)localStorage;
				else
					pointer = util::allocate<T>( arraySize );
				
				initializeObjects( pointer, arraySize, prototype );
			}
			
			
			/// Create an array which uses the specified pointer to a block of memory.
			/**
			  * After calling this constructor, the array now owns the pointer and will
			  * automatically delete it when the array is destructed.
			  */
			OM_INLINE explicit ShortArray( T* newArray, SizeType arraySize )
			{
				if ( newArray != NULL )
				{
					pointer = newArray;
					size = arraySize;
				}
				else
				{
					pointer = (T*)localStorage;
					size = 0;
				}
			}
			
			
			/// Create an array with elements from the specified pointer, copying the data.
			OM_INLINE explicit ShortArray( const T* newArray, SizeType arraySize )
			{
				if ( newArray != NULL )
				{
					if ( arraySize <= localCapacity )
						pointer = (T*)localStorage;
					else
						pointer = util::allocate<T>( arraySize );
					
					copyObjects( pointer, newArray, arraySize );
					size = arraySize;
				}
				else
				{
					pointer = (T*)localStorage;
					size = 0;
				}
			}
			
			
		//********************************************************************************
		//******	Copy Constructors
			
			
			/// Create a deep copy of the specified array object.
			OM_INLINE ShortArray( const ShortArray& other )
				:	size( other.size )
			{
				if ( size <= localCapacity )
					pointer = (T*)localStorage;
				else
					pointer = util::allocate<T>( size );
				
				copyObjects( pointer, other.pointer, size );
			}
			
			
			/// Create a deep copy of the specified array object.
			template < Size localCapacity2 >
			OM_INLINE ShortArray( const ShortArray<T,localCapacity2,SizeType>& other )
				:	size( other.size )
			{
				if ( size <= localCapacity )
					pointer = (T*)localStorage;
				else
					pointer = util::allocate<T>( size );
				
				copyObjects( pointer, other.pointer, size );
			}
			
			
			/// Create a deep copy of the specified array object, using only the specified number of elements.
			template < Size localCapacity2 >
			OM_INLINE ShortArray( const ShortArray<T,localCapacity2,SizeType>& other, SizeType number )
				:	size( number < other.size ? number : other.size )
			{
				if ( size <= localCapacity )
					pointer = (T*)localStorage;
				else
					pointer = util::allocate<T>( size );
				
				copyObjects( pointer, other.pointer, size );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			/// Destroy an array object and deallocate its internal array.
			OM_INLINE ~ShortArray()
			{
				callDestructors( pointer, size );
				
				if ( pointer != (T*)localStorage )
					util::deallocate( pointer );
			}
			
			
		//********************************************************************************
		//******	Assignment Operators
			
			
			/// Copy the contents from another array into this array, replacing the current contents.
			OM_INLINE ShortArray& operator = ( const ShortArray& other )
			{
				if ( this != &other )
				{
					callDestructors( pointer, size );
					
					if ( other.size > size )
					{
						if ( pointer != (T*)localStorage )
							util::deallocate( pointer );
						
						if ( other.size > localCapacity )
							pointer = util::allocate<T>( other.size );
						else
							pointer = (T*)localStorage;
					}
					
					copyObjects( pointer, other.pointer, other.size );
					size = other.size;
				}
				
				return *this;
			}
			
			
			/// Copy the contents from another array into this array, replacing the current contents.
			template < Size localCapacity2 >
			OM_INLINE ShortArray<T,localCapacity,SizeType>& operator = ( const ShortArray<T,localCapacity2,SizeType>& other )
			{
				if ( this != &other )
				{
					callDestructors( pointer, size );
					
					if ( other.size > size )
					{
						if ( pointer != (T*)localStorage )
							util::deallocate( pointer );
						
						if ( other.size > localCapacity )
							pointer = util::allocate<T>( other.size );
						else
							pointer = (T*)localStorage;
					}
					
					copyObjects( pointer, other.pointer, other.size );
					size = other.size;
				}
				
				return *this;
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
			template < Size localCapacity2 >
			OM_INLINE Bool operator == ( const ShortArray<T,localCapacity2,SizeType>& array ) const
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
			template < Size localCapacity2 >
			OM_INLINE Bool operator != ( const ShortArray<T,localCapacity2,SizeType>& array ) const
			{
				return !(*this == array);
			}
			
			
		//********************************************************************************
		//******	Array Concatenation Operators
			
			
			/// Concatenate the contents of this array with another array and return the resulting new array.
			template < Size localCapacity2 >
			OM_NO_INLINE ShortArray<T,localCapacity,SizeType> operator + ( const ShortArray<T,localCapacity2,SizeType>& other ) const
			{
				Size newArraySize = size + other.size;
				T* newArray = util::allocate<T>( newArraySize );
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
				
				return ShortArray<T,localCapacity,SizeType>( newArray, newArraySize );
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
				T* element = pointer;
				const T* const elementsEnd = element + size;
				
				while ( element != elementsEnd )
				{
					*element = prototype;
					element++;
				}
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Methods
			
			
			/// Call the destructors for teh specified number of objects in the given array pointer.
			static void callDestructors( T* array, SizeType number )
			{
				const T* const arrayEnd = array + number;
				
				while ( array != arrayEnd )
				{
					array->~T();
					array++;
				}
			}
			
			
			/// Copy the specified number of objects from the source to the destination pointer.
			static void copyObjects( T* destination, const T* source, SizeType number )
			{
				const T* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					new (destination) T(*source);
					destination++;
					source++;
				}
			}
			
			
			/// Move the specified number of objects from the source to the destination pointer, calling the previous object destructors.
			static void moveObjects( T* destination, const T* source, SizeType number )
			{
				const T* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					new (destination) T(*source);
					source->~T();
					
					destination++;
					source++;
				}
			}
			
			
			/// Initialize the specified number of objects stored at the specified pointer with their default constructor.
			static void initializeObjects( T* destination, SizeType number )
			{
				T* const destinationEnd = destination + number;
				
				while ( destination != destinationEnd )
				{
					new (destination) T();
					destination++;
				}
			}
			
			
			/// Initialize the specified number of objects stored at the specified pointer as copies of the specified prototype object.
			static void initializeObjects( T* destination, SizeType number, const T& prototype )
			{
				T* const destinationEnd = destination + number;
				
				while ( destination != destinationEnd )
				{
					new (destination) T(prototype);
					destination++;
				}
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the array data.
			T* pointer;
			
			
			/// The size of the array.
			SizeType size;
			
			
			/// An array of bytes with static size that hold the locally-stored elements in this short array.
			UByte localStorage[sizeof(T)*localCapacity];
			
			
		//********************************************************************************
		//******	Private Friend Class
			
			
			template < typename U, Size localSize2, typename SizeType2 >
			friend class ShortArray;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Array SizeType Set Method
//############		
//##########################################################################################
//##########################################################################################




template < typename T, Size localCapacity, typename SizeType >
void ShortArray<T,localCapacity,SizeType>:: setSize( SizeType newSize, const T& prototype )
{
	if ( size == newSize )
		return;
	
	if ( newSize < size )
		callDestructors( pointer + newSize, size - newSize );
	else
	{
		T* newPointer = (newSize <= localCapacity) ? (T*)localStorage : util::allocate<T>( newSize );
		
		moveObjects( newPointer, pointer, size );
		initializeObjects( newPointer + size, newSize - size, prototype );
		
		if ( pointer != (T*)localStorage )
			util::deallocate( pointer );
	}
	
	size = newSize;
}




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SHORT_ARRAY_H
