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

#ifndef INCLUDE_OM_ALLOCATOR_H
#define INCLUDE_OM_ALLOCATOR_H


#include "omUtilitiesConfig.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Allocator Class Interface
//############		
//##########################################################################################
//##########################################################################################





//********************************************************************************
/// A class that represents the interface for objects that allocate memory.
class Allocator
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this allocator.
			virtual ~Allocator()
			{
			}
			
			
		//********************************************************************************
		//******	Allocation Methods
			
			
			/// Allocate the specified number of bytes and return a pointer to the uninitialized memory.
			virtual UByte* allocate( Size numBytes )
			{
				return (UByte*)OM_MALLOC( numBytes );
			}
			
			
			/// Free the previously allocated memory block pointed to by the specified pointer.
			virtual void deallocate( void* pointer )
			{
				OM_FREE( pointer );
			}
			
			
		//********************************************************************************
		//******	Static Allocation Methods
			
			
			/// Allocate unitialized memory for the specified number of objects of type T.
			template < typename T >
			OM_FORCE_INLINE static T* allocate( Size count = Size(1) )
			{
				// Call a macro defined in rimConfig.h to allocate the memory.
				T* memory = (T*)OM_MALLOC( count*sizeof(T) );
				
				// Make sure that the memory location returned is not null.
				OM_DEBUG_ASSERT( memory != NULL );
				
				return memory;
			}
			
			
			/// Free the previously allocated memory block pointed to by the specified pointer.
			template < typename T >
			OM_FORCE_INLINE static void deallocate( T* pointer )
			{
				// Call a macro defined in rimConfig.h to free the memory.
				OM_FREE( (void*)pointer );
			}
			
			
		//********************************************************************************
		//******	Static Object Construction Methods
			
			
			/// Call the default constructor of an object.
			template < typename T >
			OM_FORCE_INLINE static void construct( T* object )
			{
				new (object) T();
			}
			
			
			/// Call the constructor of an object with 1 constructor parameter.
			template < typename T, typename P1 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1 )
			{
				new (object) T( p1 );
			}
			
			
			/// Call the constructor of an object with 2 constructor parameters.
			template < typename T, typename P1, typename P2 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2 )
			{
				new (object) T( p1, p2 );
			}
			
			
			/// Call the constructor of an object with 3 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3 )
			{
				new (object) T( p1, p2, p3 );
			}
			
			
			/// Call the constructor of an object with 4 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3, typename P4 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
			{
				new (object) T( p1, p2, p3, p4 );
			}
			
			
			/// Call the constructors of an array of objects with 5 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
			{
				new (object) T( p1, p2, p3, p4, p5 );
			}
			
			
			/// Call the constructor of an object with 6 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
			{
				new (object) T( p1, p2, p3, p4, p5, p6 );
			}
			
			
			/// Call the constructor of an object with 7 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7 )
			{
				new (object) T( p1, p2, p3, p4, p5, p6, p7 );
			}
			
			
			/// Call the constructor of an object with 8 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8 )
			{
				new (object) T( p1, p2, p3, p4, p5, p6, p7, p8 );
			}
			
			
			/// Call the constructor of an object with 9 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9 )
			{
				new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
			}
			
			
			/// Call the constructor of an object with 10 constructor parameters.
			template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10 >
			OM_FORCE_INLINE static void construct( T* object, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10 )
			{
				new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
			}
			
			
		//********************************************************************************
		//******	Static Object Array Construction Methods
			
			
			/// Call the default constructors of an array of objects.
			template < typename T, typename SizeType >
			OM_INLINE static void constructArray( T* objects, SizeType number )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T();
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 1 constructor parameter.
			template < typename T, typename SizeType, typename P1 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 2 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 3 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 4 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3, typename P4 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3, p4 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 5 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3, typename P4, typename P5 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3, p4, p5 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 6 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3, p4, p5, p6 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 7 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3, p4, p5, p6, p7 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 8 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3, p4, p5, p6, p7, p8 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 9 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
					object++;
				}
			}
			
			
			/// Call the constructors of an array of objects with 10 constructor parameters.
			template < typename T, typename SizeType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10 >
			OM_INLINE static void constructArray( T* objects, SizeType number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10 )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
					object++;
				}
			}
			
			
		//********************************************************************************
		//******	Static Object Destruction Methods
			
			
			/// Call the destructor on an object of the templated type.
			template < typename T >
			OM_FORCE_INLINE static void destruct( T* object )
			{
				object->~T();
			}
			
			
			/// Call the destructors on an array of objects of the templated type.
			template < typename T, typename SizeType >
			OM_INLINE static void destructArray( T* objects, SizeType number )
			{
				T* object = objects;
				const T* const objectsEnd = objects + number;
				
				while ( object != objectsEnd )
				{
					object->~T();
					object++;
				}
			}
			
			
		//********************************************************************************
		//******	Static Object Copy Methods
			
			
			/// Copy an array of objects by calling the placement-new copy constructor for each object.
			template < typename T, typename SizeType >
			OM_INLINE static void copyArray( T* destination, const T* source, SizeType number )
			{
				const T* const sourceEnd = source + number;
				
				while ( source != sourceEnd )
				{
					new (destination) T(*source);
					destination++;
					source++;
				}
			}
			
			
			/// Copy an array of objects from the source to destination with placement new, calling destructors on the source.
			template < typename T, typename SizeType >
			OM_INLINE static void moveArray( T* destination, T* source, SizeType number )
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
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Allocator Class Interface
//############		
//##########################################################################################
//##########################################################################################





//********************************************************************************
/// A class that allocates aligned memory from the global memory system.
template < Size defaultAlignment = Size(16) >
class AlignedAllocator : public Allocator
{
	public:
		
		//********************************************************************************
		//******	Allocation Methods
			
			
			/// Allocate the specified number of bytes and return a pointer to the uninitialized memory.
			virtual UByte* allocate( Size numBytes )
			{
				return (UByte*)OM_ALIGNED_MALLOC( numBytes, defaultAlignment );
			}
			
			
			/// Free the previously allocated memory block pointed to by the specified pointer.
			virtual void deallocate( void* pointer )
			{
				OM_ALIGNED_FREE( pointer );
			}
			
			
		//********************************************************************************
		//******	Allocation Methods




			/// Allocate unitialized memory for the specified number of objects of type T.
			template < typename T >
			OM_FORCE_INLINE static T* allocate( Size count = Size(1), Size alignment = defaultAlignment )
			{
				// Call a macro defined in rimConfig.h to allocate the memory.
				T* memory = (T*)OM_ALIGNED_MALLOC( count*sizeof(T), alignment );
				
				// Make sure that the memory location returned is not null.
				OM_DEBUG_ASSERT( memory != NULL );
				
				return memory;
			}





			/// Free the previously allocated memory block pointed to by the specified pointer.
			template < typename T >
			OM_FORCE_INLINE static void deallocate( T* pointer )
			{
				// Call a macro defined in rimConfig.h to free the memory.
				OM_ALIGNED_FREE( (void*)pointer );
			}



};




//##########################################################################################
//##########################################################################################
//############		
//############		Object Allocation Methods
//############		
//##########################################################################################
//##########################################################################################




/// Allocate memory for a single object of type T.
/**
  * This method returns a pointer to the start of the object in memory.
  * The constructor of the allocated object is not called. It is the
  * responsibility of the user to use placement new to initialize the
  * object. Alternatively, one can call the construct() method instead
  * which initializes the memory location by calling placement new after
  * allocating the memory.
  */
template < typename T >
OM_FORCE_INLINE T* allocate()
{
	return Allocator::allocate<T>();
}




/// Allocate memory for the specified number of objects of type T.
/**
  * This method returns a pointer to the start of the first object in memory.
  * No constructors are called on the allocated memory locations. One must use
  * placement new in order to initialize the objects.
  * 
  * @param count - the number of objects to allocate space for.
  */
template < typename T >
OM_FORCE_INLINE T* allocate( Size count )
{
	return Allocator::allocate<T>( count );
}




/// Allocate memory for the specified number of objects of type T.
/**
  * This method returns a pointer to the start of the first object in memory.
  * No constructors are called on the allocated memory locations. One must use
  * placement new in order to initialize the objects.
  * 
  * @param count - the number of objects to allocate space for.
  * @param alignment - the required alignment for the memory in bytes.
  */
template < typename T >
OM_FORCE_INLINE T* allocateAligned( Size count, Size alignment = Size(16) )
{
	return AlignedAllocator<>::allocate<T>( count, alignment );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Pointer Deallocation Method
//############		
//##########################################################################################
//##########################################################################################




/// Free the memory block pointed to by the specified pointer.
/**
  * If the pointer is NULL, an assertion is raised.
  * 
  * @param pointer - a pointer to the memory to be deallocated.
  */
template < typename T >
OM_FORCE_INLINE void deallocate( T* pointer )
{
	return Allocator::deallocate( pointer );
}




/// Free the aligned memory block pointed to by the specified pointer.
/**
  * If the pointer is NULL, an assertion is raised. This method should
  * only be called on memory aligned using the allocateAligned() family of
  * functions.
  * 
  * @param pointer - a pointer to the memory to be deallocated.
  */
template < typename T >
OM_FORCE_INLINE void deallocateAligned( T* pointer )
{
	return AlignedAllocator<>::deallocate( pointer );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Object Construction Methods
//############		
//##########################################################################################
//##########################################################################################




/// Construct and allocate an object of the templated type T, calling the default constructor.
template < typename T >
OM_INLINE T* construct()
{
	T* object = allocate<T>();
	new (object) T();
	return object;
}




/// Construct and allocate an object of the templated type T with one constructor parameter.
template < typename T, typename P1 >
OM_INLINE T* construct( const P1& p1 )
{
	T* object = allocate<T>();
	new (object) T( p1 );
	return object;
}




/// Construct and allocate an object of the templated type T with 2 constructor parameters.
template < typename T, typename P1, typename P2 >
OM_INLINE T* construct( const P1& p1, const P2& p2 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2 );
	return object;
}




/// Construct and allocate an object of the templated type T with 3 constructor parameters.
template < typename T, typename P1, typename P2, typename P3 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3 );
	return object;
}




/// Construct and allocate an object of the templated type T with 4 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3, p4 );
	return object;
}




/// Construct and allocate an object of the templated type T with 5 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3, p4, p5 );
	return object;
}




/// Construct and allocate an object of the templated type T with 6 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3, p4, p5, p6 );
	return object;
}




/// Construct and allocate an object of the templated type T with 7 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3, p4, p5, p6, p7 );
	return object;
}




/// Construct and allocate an object of the templated type T with 8 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3, p4, p5, p6, p7, p8 );
	return object;
}




/// Construct and allocate an object of the templated type T with 9 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	return object;
}




/// Construct and allocate an object of the templated type T with 10 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10 >
OM_INLINE T* construct( const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10 )
{
	T* object = allocate<T>();
	new (object) T( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
	return object;
}





//##########################################################################################
//##########################################################################################
//############		
//############		Object Destruction Methods
//############		
//##########################################################################################
//##########################################################################################




/// Destruct and free an object of the templated type T at the specified location.
/**
  * This method calls the destructor of the pointed-to object and then deallocates
  * the memory used by that object. If the object pointer is NULL, an assertion is raised.
  * 
  * @param object - a pointer to the object to be destroyed.
  */
template < typename T >
OM_FORCE_INLINE void destruct( T* object )
{
	// Call the object's destructor.
	object->~T();
	
	// Deallocate the memory use by the object.
	deallocate( object );
}




/// Destruct and free an object of the templated type T at the specified location.
/**
  * This method calls the destructor of the pointed-to object and then deallocates
  * the memory used by that object. If the object pointer is NULL, an assertion is raised.
  * 
  * @param object - a pointer to the object to be destroyed.
  */
template < typename T >
OM_FORCE_INLINE void destructAligned( T* object )
{
	// Call the object's destructor.
	object->~T();
	
	// Deallocate the memory use by the object.
	deallocateAligned( object );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Object Array Construction Methods
//############		
//##########################################################################################
//##########################################################################################




/// Construct and allocate an array of objects of the templated type T, calling the default constructor.
template < typename T >
OM_INLINE T* constructArray( Size number )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with one constructor parameter.
template < typename T, typename P1 >
OM_INLINE T* constructArray( Size number, const P1& p1 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 2 constructor parameters.
template < typename T, typename P1, typename P2 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 3 constructor parameters.
template < typename T, typename P1, typename P2, typename P3 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 4 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3, p4 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 5 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 6 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 7 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 8 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7, p8 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 9 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 10 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9, typename P10 >
OM_INLINE void* constructArray( Size number, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10 )
{
	T* objects = allocate<T>( number );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
	return objects;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Aligned Object Array Construction Methods
//############		
//##########################################################################################
//##########################################################################################




/// Construct and allocate an array of objects of the templated type T, calling the default constructor.
template < typename T >
OM_INLINE T* constructArrayAligned( Size number, Size alignment )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with one constructor parameter.
template < typename T, typename P1 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 2 constructor parameters.
template < typename T, typename P1, typename P2 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 3 constructor parameters.
template < typename T, typename P1, typename P2, typename P3 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 4 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3, p4 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 5 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 6 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 7 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 8 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7, p8 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 9 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	return objects;
}




/// Construct and allocate an array of objects of the templated type T with 10 constructor parameters.
template < typename T, typename P1, typename P2, typename P3, typename P4, typename P5,
						typename P6, typename P7, typename P8, typename P9, typename P10 >
OM_INLINE T* constructArrayAligned( Size number, Size alignment, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9, const P10& p10 )
{
	T* objects = allocateAligned<T>( number, alignment );
	Allocator::constructArray( objects, number, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
	return objects;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Copy Method
//############		
//##########################################################################################
//##########################################################################################




/// Allocate a new array and copy the specified number of elements from an old array.
template < typename T >
OM_INLINE T* copyArray( const T* oldArray, Size number )
{
	T* newArray = allocate<T>( number );
	
	const T* oldObject = oldArray;
	const T* const oldArrayEnd = oldArray + number;
	T* newObject = newArray;
	
	while ( oldObject != oldArrayEnd )
	{
		new (newObject) T(*oldObject);
		oldObject++;
		newObject++;
	}
	
	return newArray;
}




/// Allocate a new aligned array and copy the specified number of elements from an old array.
template < typename T >
OM_INLINE T* copyArrayAligned( const T* oldArray, Size number, Size alignment )
{
	T* newArray = allocateAligned<T>( number, alignment );
	
	const T* oldObject = oldArray;
	const T* const oldArrayEnd = oldArray + number;
	T* newObject = newArray;
	
	while ( oldObject != oldArrayEnd )
	{
		new (newObject) T(*oldObject);
		oldObject++;
		newObject++;
	}
	
	return newArray;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Object Array Destruction Method
//############		
//##########################################################################################
//##########################################################################################




/// Destruct and free an array of objects of the templated type T at the specified location.
template < typename T >
OM_INLINE void destructArray( T* objects, Size number )
{
	OM_DEBUG_ASSERT( objects != NULL );
	
	// Call the objects' destructors.
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		object->~T();
		object++;
	}
	
	// Deallocate the memory use by the objects.
	deallocate( objects );
}




/// Destruct and free an array of objects of the templated type T at the specified location.
/**
  * This method should only be used to deallocate memory allocated with the allocateAligned()
  * family of functions.
  */
template < typename T >
OM_INLINE void destructArrayAligned( T* objects, Size number )
{
	OM_DEBUG_ASSERT( objects != NULL );
	
	// Call the objects' destructors.
	T* object = objects;
	const T* const objectsEnd = objects + number;
	
	while ( object != objectsEnd )
	{
		object->~T();
		object++;
	}
	
	// Deallocate the memory use by the objects.
	deallocateAligned( objects );
}




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_ALLOCATOR_H
