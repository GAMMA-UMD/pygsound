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

#ifndef INCLUDE_OM_COPY_H
#define INCLUDE_OM_COPY_H


#include "omUtilitiesConfig.h"


//##########################################################################################
//****************************  Start Om Utilities Namespace  ******************************
OM_UTILITIES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Swap Method
//############		
//##########################################################################################
//##########################################################################################




/// Swap the locations of two values of the templated type.
template < typename T >
OM_INLINE void swap( T& a, T& b )
{
	T temp = a;
	a = b;
	b = temp;
}




/// Swap the contents of two arrays of the templated type.
template < typename T, typename SizeType >
OM_INLINE void swap( T* a, T* b, SizeType number )
{
	const T* const aEnd = a + number;
	
	while ( a != aEnd )
	{
		swap( *a, *b );
		a++;
		b++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Copy Method For Complex Types
//############		
//##########################################################################################
//##########################################################################################




/// Copy the specified number of objects from the source pointer to the destination pointer.
/**
  * This method does not check the validity of the arguments.
  */
template < typename T, typename SizeType >
OM_INLINE void copy( T* destination, const T* source, SizeType number )
{
	const T* const sourceEnd = source + number;
	
	while ( source != sourceEnd )
	{
		*destination = *source;
		
		destination++;
		source++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Copy Method For Complex Types - POD Type Overloads
//############		
//##########################################################################################
//##########################################################################################




template < typename SizeType >
OM_FORCE_INLINE void copy( unsigned char* destination, const unsigned char* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(unsigned char) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( char* destination, const char* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(char) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( unsigned short* destination, const unsigned short* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(unsigned short) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( short* destination, const short* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(short) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( unsigned int* destination, const unsigned int* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(unsigned int) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( int* destination, const int* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(int) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( unsigned long* destination, const unsigned long* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(unsigned long) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( long* destination, const long* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(long) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( unsigned long long* destination, const unsigned long long* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(unsigned long long) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( long long* destination, const long long* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(long long) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( float* destination, const float* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(float) );
}




template < typename SizeType >
OM_FORCE_INLINE void copy( double* destination, const double* source, SizeType number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(double) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Copy Method For POD Types
//############		
//##########################################################################################
//##########################################################################################




/// Copy the specified number of POD-type (plain-old-data) objects from the source pointer to the destination pointer.
/**
  * The objects of type T should have either an implicitly-declared or trivial
  * copy constructor/assignment operator. The byes from the source pointer are
  * copied directly to the destination.
  */
template < typename T >
OM_INLINE void copyPOD( T* destination, const T* source, Size number )
{
	std::memcpy( (void*)destination, (void*)source, number*sizeof(T) );
}





//##########################################################################################
//##########################################################################################
//############		
//############		Primary Convert Method For Complex Types
//############		
//##########################################################################################
//##########################################################################################




/// Copy the specified number of objects from the source pointer to the destination pointer, converting object type.
/**
  * The method uses the assignment operator to assign the objects from the
  * source to destination.
  *
  * This method does not check the validity of the arguments in order to not
  * impose unnecessary overhead on this low-level method.
  */
template < typename T, typename U >
OM_INLINE void convert( T* destination, const U* source, Size number )
{
	const U* const sourceEnd = source + number;
	
	while ( source != sourceEnd )
	{
		*destination = *source;
		
		destination++;
		source++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Memory Set Methods
//############		
//##########################################################################################
//##########################################################################################




/// Set the specified number of objects at the destination pointer to the given value.
/**
  * The method uses the assignment operator to assign the value to the destination.
  *
  * This method does not check the validity of the arguments in order to not
  * impose unnecessary overhead on this low-level method.
  */
template < typename T >
OM_INLINE void set( T* destination, T value, Size number )
{
	const T* const destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		*destination = value;
		destination++;
	}
}




template <>
OM_INLINE void set( unsigned char* destination, unsigned char value, Size number )
{
	std::memset( destination, value, number );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Memory Zero Methods
//############		
//##########################################################################################
//##########################################################################################




/// Set the specified number of objects at the destination pointer to zero.
/**
  * The default value for the type is assigned to the destination array.
  *
  * This method does not check the validity of the arguments in order to not
  * impose unnecessary overhead on this low-level method.
  */
template < typename T >
OM_INLINE void zero( T* destination, Size number )
{
	const T* const destinationEnd = destination + number;
	
	while ( destination < destinationEnd )
	{
		*destination = T();
		destination++;
	}
}




template <>
OM_FORCE_INLINE void zero( unsigned char* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(unsigned char) );
}




template <>
OM_FORCE_INLINE void zero( char* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(char) );
}




template <>
OM_FORCE_INLINE void zero( unsigned short* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(unsigned short) );
}




template <>
OM_FORCE_INLINE void zero( short* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(short) );
}




template <>
OM_FORCE_INLINE void zero( unsigned int* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(unsigned int) );
}




template <>
OM_FORCE_INLINE void zero( int* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(int) );
}




template <>
OM_FORCE_INLINE void zero( unsigned long* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(unsigned long) );
}




template <>
OM_FORCE_INLINE void zero( long* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(long) );
}




template <>
OM_FORCE_INLINE void zero( unsigned long long* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(unsigned long long) );
}




template <>
OM_FORCE_INLINE void zero( long long* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(long long) );
}




template <>
OM_FORCE_INLINE void zero( float* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(float) );
}




template <>
OM_FORCE_INLINE void zero( double* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(double) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Copy Method For POD Types
//############		
//##########################################################################################
//##########################################################################################




/// Set the specified number of POD-type (plain-old-data) objects from the source pointer to all zeros.
/**
  * The objects of type T should have either an implicitly-declared or trivial
  * copy constructor/assignment operator. The bytes in the destination pointer are
  * initialized to zero.
  */
template < typename T >
OM_INLINE void zeroPOD( T* destination, Size number )
{
	std::memset( (unsigned char*)destination, 0, number*sizeof(T) );
}




//##########################################################################################
//****************************  End Om Utilities Namespace  ********************************
OM_UTILITIES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_COPY_H
