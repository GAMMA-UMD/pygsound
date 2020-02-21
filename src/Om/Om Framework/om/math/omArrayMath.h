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

#ifndef INCLUDE_OM_ARRAY_MATH_H
#define INCLUDE_OM_ARRAY_MATH_H


#include "omMathConfig.h"


#include "omScalarMath.h"
#include "omComplex.h"
#include "omVector2D.h"
#include "omVector3D.h"
#include "omVector4D.h"
#include "omMatrix2D.h"
#include "omMatrix3D.h"
#include "omMatrix4D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Array Add Methods
//############		
//##########################################################################################
//##########################################################################################




/// Compute and return the sum of all of the values in the specified array.
template < typename T >
OM_INLINE T sum( const T* array, Size number )
{
	if ( number == 0 )
		return T();
	
	const T* arrayEnd = array + number;
	T result = *array;
	array++;
	
	while ( array != arrayEnd )
	{
		result += (*array);
		array++;
	}
	
	return result;
}

template <> Float32 sum( const Float32* array, Size number );
template <> Float64 sum( const Float64* array, Size number );




/// Add a scalar value to each entry in the destination array, storing the result.
template < typename T >
OM_INLINE void add( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (scalar);
		destination++;
	}
}

template <> void add( Int32* destination, const Int32& scalar, Size number );
template <> void add( Int64* destination, const Int64& scalar, Size number );
template <> void add( Float32* destination, const Float32& scalar, Size number );
template <> void add( Float64* destination, const Float64& scalar, Size number );

template < typename T >
OM_INLINE void add( Complex<T>* destination, T scalar, Size number )
{
	return add( (T*)destination, scalar, number*2 );
}

template < typename T, Size size >
OM_INLINE void add( VectorND<T,size>* destination, T scalar, Size number )
{
	return add( (T*)destination, scalar, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void add( MatrixND<T,size0,size1>* destination, T scalar, Size number )
{
	return add( (T*)destination, scalar, number*(size0*size1) );
}




/// Add each entry of the second array to each entry in the destination array, storing the result.
template < typename T >
OM_INLINE void add( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*b);
		b++;
		destination++;
	}
}

template <> void add( Int32* destination, const Int32* b, Size number );
template <> void add( Int64* destination, const Int64* b, Size number );
template <> void add( Float32* destination, const Float32* b, Size number );
template <> void add( Float64* destination, const Float64* b, Size number );

template < typename T >
OM_INLINE void add( Complex<T>* destination, const Complex<T>* b, Size number )
{
	return add( (T*)destination, (const T*)b, number*2 );
}

template < typename T, Size size >
OM_INLINE void add( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return add( (T*)destination, (const T*)b, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void add( MatrixND<T,size0,size1>* destination, const MatrixND<T,size0,size1>* b, Size number )
{
	return add( (T*)destination, (const T*)b, number*(size0*size1) );
}




/// Add the entries of two arrays and store the result in the destination array.
template < typename T >
OM_INLINE void add( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a) + (*b);
		a++;
		b++;
		destination++;
	}
}

template <> void add( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void add( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void add( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void add( Float64* destination, const Float64* a, const Float64* b, Size number );

template < typename T >
OM_INLINE void add( Complex<T>* destination, const Complex<T>* a, const Complex<T>* b, Size number )
{
	return add( (T*)destination, (const T*)a, (const T*)b, number*2 );
}

template < typename T, Size size >
OM_INLINE void add( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return add( (T*)destination, (const T*)a, (const T*)b, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void add( MatrixND<T,size0,size1>* destination, const MatrixND<T,size0,size1>* a, const MatrixND<T,size0,size1>* b, Size number )
{
	return add( (T*)destination, (const T*)a, (const T*)b, number*(size0*size1) );
}




/// Add a scalar value to each entry in the destination array, storing the result.
template < typename T >
OM_INLINE void add( T* destination, const T* a, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a) + (scalar);
		a++;
		destination++;
	}
}

template <> void add( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void add( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void add( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void add( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T >
OM_INLINE void add( Complex<T>* destination, const Complex<T>* a, T scalar, Size number )
{
	return add( (T*)destination, (const T*)a, scalar, number*2 );
}

template < typename T, Size size >
OM_INLINE void add( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return add( (T*)destination, (const T*)a, scalar, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void add( MatrixND<T,size0,size1>* destination, const MatrixND<T,size0,size1>* a, T scalar, Size number )
{
	return add( (T*)destination, (const T*)a, scalar, number*(size0*size1) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Subtract Methods
//############		
//##########################################################################################
//##########################################################################################




/// Negate each element in the specified array, storing the result in the same array.
template < typename T >
void negate( T* destination, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = -(*destination);
		destination++;
	}
}

template <> void negate( Int32* destination, Size number );
template <> void negate( Int64* destination, Size number );
template <> void negate( Float32* destination, Size number );
template <> void negate( Float64* destination, Size number );

template < typename T >
OM_INLINE void negate( Complex<T>* destination, Size number )
{
	return negate( (T*)destination, number*2 );
}

template < typename T, Size size >
OM_INLINE void negate( VectorND<T,size>* destination, Size number )
{
	return negate( (T*)destination, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void negate( MatrixND<T,size0,size1>* destination, Size number )
{
	return negate( (T*)destination, number*(size0*size1) );
}




/// Negate each element in an array, storing the result in a destination array.
template < typename T >
void negate( T* destination, const T* a, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = -(*a);
		a++;
		destination++;
	}
}

template <> void negate( Int32* destination, const Int32* b, Size number );
template <> void negate( Int64* destination, const Int64* b, Size number );
template <> void negate( Float32* destination, const Float32* b, Size number );
template <> void negate( Float64* destination, const Float64* b, Size number );

template < typename T >
OM_INLINE void negate( Complex<T>* destination, const Complex<T>* b, Size number )
{
	return negate( (T*)destination, (const T*)b, number*2 );
}

template < typename T, Size size >
OM_INLINE void negate( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return negate( (T*)destination, (const T*)b, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void negate( MatrixND<T,size0,size1>* destination, const MatrixND<T,size0,size1>* b, Size number )
{
	return negate( (T*)destination, (const T*)b, number*(size0*size1) );
}




/// Subtract a scalar from each element of the destination array, storing the result in the destination.
template < typename T >
OM_INLINE void subtract( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (scalar);
		destination++;
	}
}

template <> void subtract( Int32* destination, const Int32& scalar, Size number );
template <> void subtract( Int64* destination, const Int64& scalar, Size number );
template <> void subtract( Float32* destination, const Float32& scalar, Size number );
template <> void subtract( Float64* destination, const Float64& scalar, Size number );

template < typename T >
OM_INLINE void subtract( Complex<T>* destination, T scalar, Size number )
{
	return subtract( (T*)destination, scalar, number*2 );
}

template < typename T, Size size >
OM_INLINE void subtract( VectorND<T,size>* destination, T scalar, Size number )
{
	return subtract( (T*)destination, scalar, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void subtract( MatrixND<T,size0,size1>* destination, T scalar, Size number )
{
	return subtract( (T*)destination, scalar, number*(size0*size1) );
}




/// Subtract each entry of the second array from the destination array and store the result in the destination.
template < typename T >
OM_INLINE void subtract( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*b);
		b++;
		destination++;
	}
}

template <> void subtract( Int32* destination, const Int32* b, Size number );
template <> void subtract( Int64* destination, const Int64* b, Size number );
template <> void subtract( Float32* destination, const Float32* b, Size number );
template <> void subtract( Float64* destination, const Float64* b, Size number );

template < typename T >
OM_INLINE void subtract( Complex<T>* destination, const Complex<T>* b, Size number )
{
	return subtract( (T*)destination, (const T*)b, number*2 );
}

template < typename T, Size size >
OM_INLINE void subtract( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return subtract( (T*)destination, (const T*)b, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void subtract( MatrixND<T,size0,size1>* destination, const MatrixND<T,size0,size1>* b, Size number )
{
	return subtract( (T*)destination, (const T*)b, number*(size0*size1) );
}




/// Subtract an array from another and store the result in the destination array.
template < typename T >
OM_INLINE void subtract( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a) - (*b);
		a++;
		b++;
		destination++;
	}
}

template <> void subtract( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void subtract( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void subtract( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void subtract( Float64* destination, const Float64* a, const Float64* b, Size number );

template < typename T >
OM_INLINE void subtract( Complex<T>* destination, const Complex<T>* a, const Complex<T>* b, Size number )
{
	return subtract( (T*)destination, (const T*)a, (const T*)b, number*2 );
}

template < typename T, Size size >
OM_INLINE void subtract( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return subtract( (T*)destination, (const T*)a, (const T*)b, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void subtract( MatrixND<T,size0,size1>* destination, const MatrixND<T,size0,size1>* a, const MatrixND<T,size0,size1>* b, Size number )
{
	return subtract( (T*)destination, (const T*)a, (const T*)b, number*(size0*size1) );
}




/// Subtract a scalar from an array and store the result in the destination array.
template < typename T >
OM_INLINE void subtract( T* destination, const T* a, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a) - (scalar);
		a++;
		destination++;
	}
}

template <> void subtract( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void subtract( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void subtract( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void subtract( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T >
OM_INLINE void subtract( Complex<T>* destination, const Complex<T>* a, T scalar, Size number )
{
	return subtract( (T*)destination, (const T*)a, scalar, number*2 );
}

template < typename T, Size size >
OM_INLINE void subtract( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return subtract( (T*)destination, (const T*)a, scalar, number*size );
}

template < typename T, Size size0, Size size1 >
OM_INLINE void subtract( MatrixND<T,size0,size1>* destination, const MatrixND<T,size0,size1>* a, T scalar, Size number )
{
	return subtract( (T*)destination, (const T*)a, scalar, number*(size0*size1) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Multiply Methods
//############		
//##########################################################################################
//##########################################################################################




/// Compute and return the product of all of the values in the specified array.
template < typename T >
OM_INLINE T product( const T* array, Size number )
{
	const T* arrayEnd = array + number;
	T result = T(1);
	
	while ( array != arrayEnd )
	{
		result *= (*array);
		array++;
	}
	
	return result;
}

template <> Float32 product( const Float32* array, Size number );
template <> Float64 product( const Float64* array, Size number );




/// Compute and return the euclidean norm (length) of the specified vector.
template < typename T >
OM_INLINE T norm( const T* array, Size number )
{
	if ( number == 0 )
		return T();
	
	const T* arrayEnd = array + number;
	T result = (*array)*(*array);
	array++;
	
	while ( array != arrayEnd )
	{
		result += (*array)*(*array);
		array++;
	}
	
	return math::sqrt( result );
}

template <> Float32 norm( const Float32* array, Size number );
template <> Float64 norm( const Float64* array, Size number );




/// Compute and return the dot product of the values in the two given arrays.
template < typename T >
OM_INLINE T dot( const T* a, const T* b, Size number )
{
	if ( number == 0 )
		return T();
	
	const T* aEnd = a + number;
	T result = (*a)*(*b);
	a++;
	b++;
	
	while ( a != aEnd )
	{
		result += (*a)*(*b);
		a++;
		b++;
	}
	
	return result;
}

template <> Float32 dot( const Float32* a, const Float32* b, Size number );
template <> Float64 dot( const Float64* a, const Float64* b, Size number );




/// Multiply each entry in the destination array by a scalar and store the result in the destination.
template < typename T >
OM_INLINE void multiply( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) *= (scalar);
		destination++;
	}
}

template <> void multiply( Int32* destination, const Int32& scalar, Size number );
template <> void multiply( Int64* destination, const Int64& scalar, Size number );
template <> void multiply( Float32* destination, const Float32& scalar, Size number );
template <> void multiply( Float64* destination, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void multiply( VectorND<T,size>* destination, T scalar, Size number )
{
	return multiply( (T*)destination, scalar, number*size );
}




/// Multiply each entry in the destination array by another array entry and store the result in the destination.
template < typename T >
OM_INLINE void multiply( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) *= (*b);
		b++;
		destination++;
	}
}

template <> void multiply( Int32* destination, const Int32* b, Size number );
template <> void multiply( Int64* destination, const Int64* b, Size number );
template <> void multiply( Float32* destination, const Float32* b, Size number );
template <> void multiply( Float64* destination, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void multiply( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return multiply( (T*)destination, (const T*)b, number*size );
}




/// Multiply the elements in two arrays and store the result of each multiplication in the destination.
template < typename T >
OM_INLINE void multiply( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a)*(*b);
		a++;
		b++;
		destination++;
	}
}

template <> void multiply( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void multiply( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void multiply( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void multiply( Float64* destination, const Float64* a, const Float64* b, Size number );
template <> void multiply( Complex<Float32>* destination, const Complex<Float32>* a, const Complex<Float32>* b, Size number );

template < typename T, Size size >
OM_INLINE void multiply( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return multiply( (T*)destination, (const T*)a, (const T*)b, number*size );
}




/// Multiply the elements in an array by a scalar and store the result in the destination array.
template < typename T >
OM_INLINE void multiply( T* destination, const T* a, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a)*(scalar);
		a++;
		destination++;
	}
}

template <> void multiply( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void multiply( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void multiply( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void multiply( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void multiply( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return multiply( (T*)destination, (const T*)a, scalar, number*size );
}



/// Multiply the contents of an input array by a linearly-interpolated scalar and write to the result array.
template < typename T >
OM_INLINE void multiplyLerp( T* destination, const T* a, T& x, T dx, Size number )
{
	const T* const destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		*destination = (*a)*x;
		x += dx;
		a++;
		destination++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Multiply-Add Methods
//############		
//##########################################################################################
//##########################################################################################




/// Multiply each entry in the destination by a scalar, then add the result to the destination array entry.
template < typename T >
OM_INLINE void multiplyAdd( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*destination)*(scalar);
		destination++;
	}
}

template <> void multiplyAdd( Int32* destination, const Int32& scalar, Size number );
template <> void multiplyAdd( Int64* destination, const Int64& scalar, Size number );
template <> void multiplyAdd( Float32* destination, const Float32& scalar, Size number );
template <> void multiplyAdd( Float64* destination, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void multiplyAdd( VectorND<T,size>* destination, T scalar, Size number )
{
	return multiplyAdd( (T*)destination, scalar, number*size );
}




/// Multiply each entry in the destination by another array, then add the result to the destination entry.
template < typename T >
OM_INLINE void multiplyAdd( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*destination)*(*b);
		b++;
		destination++;
	}
}

template <> void multiplyAdd( Int32* destination, const Int32* b, Size number );
template <> void multiplyAdd( Int64* destination, const Int64* b, Size number );
template <> void multiplyAdd( Float32* destination, const Float32* b, Size number );
template <> void multiplyAdd( Float64* destination, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void multiplyAdd( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return multiplyAdd( (T*)destination, (const T*)b, number*size );
}




/// Multiply the enties of two arrays and add the results to the destination array entries.
template < typename T >
OM_INLINE void multiplyAdd( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*a)*(*b);
		a++;
		b++;
		destination++;
	}
}

template <> void multiplyAdd( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void multiplyAdd( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void multiplyAdd( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void multiplyAdd( Float64* destination, const Float64* a, const Float64* b, Size number );
template <> void multiplyAdd( Complex<Float32>* destination, const Complex<Float32>* a, const Complex<Float32>* b, Size number );

template < typename T, Size size >
OM_INLINE void multiplyAdd( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return multiplyAdd( (T*)destination, (const T*)a, (const T*)b, number*size );
}




/// Multiply the entries of an array by a scalar, then add the result to a destination array.
template < typename T >
OM_INLINE void multiplyAdd( T* destination, const T* a, const T& scalar, Size number )
{
	const T* const destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*a)*(scalar);
		a++;
		destination++;
	}
}

template <> void multiplyAdd( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void multiplyAdd( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void multiplyAdd( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void multiplyAdd( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void multiplyAdd( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return multiplyAdd( (T*)destination, (const T*)a, scalar, number*size );
}



/// Multiply the contents of an input array by a linearly-interpolated scalar and add to the result array.
template < typename T >
OM_INLINE void multiplyAddLerp( T* destination, const T* a, T& x, T dx, Size number )
{
	const T* const destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		*destination += (*a)*x;
		x += dx;
		a++;
		destination++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Multiply-Subtract Methods
//############		
//##########################################################################################
//##########################################################################################




/// Multiply each entry in the destination by a scalar, then subtract the result from the destination array entry.
template < typename T >
OM_INLINE void multiplySubtract( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*destination)*(scalar);
		destination++;
	}
}

template <> void multiplySubtract( Int32* destination, const Int32& scalar, Size number );
template <> void multiplySubtract( Int64* destination, const Int64& scalar, Size number );
template <> void multiplySubtract( Float32* destination, const Float32& scalar, Size number );
template <> void multiplySubtract( Float64* destination, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void multiplySubtract( VectorND<T,size>* destination, T scalar, Size number )
{
	return multiplySubtract( (T*)destination, scalar, number*size );
}




/// Multiply each entry in the destination by another array, then subtract the result from the destination entry.
template < typename T >
OM_INLINE void multiplySubtract( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*destination)*(*b);
		b++;
		destination++;
	}
}

template <> void multiplySubtract( Int32* destination, const Int32* b, Size number );
template <> void multiplySubtract( Int64* destination, const Int64* b, Size number );
template <> void multiplySubtract( Float32* destination, const Float32* b, Size number );
template <> void multiplySubtract( Float64* destination, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void multiplySubtract( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return multiplySubtract( (T*)destination, (const T*)b, number*size );
}




/// Multiply the enties of two arrays and subtract the results from the destination array entries.
template < typename T >
OM_INLINE void multiplySubtract( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*a)*(*b);
		a++;
		b++;
		destination++;
	}
}

template <> void multiplySubtract( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void multiplySubtract( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void multiplySubtract( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void multiplySubtract( Float64* destination, const Float64* a, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void multiplySubtract( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return multiplySubtract( (T*)destination, (const T*)a, (const T*)b, number*size );
}




/// Multiply the entries of an array by a scalar, then subtract the result from a destination array.
template < typename T >
OM_INLINE void multiplySubtract( T* destination, const T* a, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*a)*(scalar);
		a++;
		destination++;
	}
}

template <> void multiplySubtract( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void multiplySubtract( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void multiplySubtract( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void multiplySubtract( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void multiplySubtract( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return multiplySubtract( (T*)destination, (const T*)a, scalar, number*size );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Divide Methods
//############		
//##########################################################################################
//##########################################################################################




/// Divide each entry in the destination array by a scalar and store the result in the destination.
template < typename T >
OM_INLINE void divide( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) /= (scalar);
		destination++;
	}
}

template <> void divide( Int32* destination, const Int32& scalar, Size number );
template <> void divide( Int64* destination, const Int64& scalar, Size number );
template <> void divide( Float32* destination, const Float32& scalar, Size number );
template <> void divide( Float64* destination, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void divide( VectorND<T,size>* destination, T scalar, Size number )
{
	return divide( (T*)destination, scalar, number*size );
}




/// Divide each entry in the destination array by another array entry and store the result in the destination.
template < typename T >
OM_INLINE void divide( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) /= (*b);
		b++;
		destination++;
	}
}

template <> void divide( Int32* destination, const Int32* b, Size number );
template <> void divide( Int64* destination, const Int64* b, Size number );
template <> void divide( Float32* destination, const Float32* b, Size number );
template <> void divide( Float64* destination, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void divide( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return divide( (T*)destination, (const T*)b, number*size );
}




/// Divide the elements of one array by those in another and store the result in the destination.
template < typename T >
OM_INLINE void divide( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a)/(*b);
		a++;
		b++;
		destination++;
	}
}

template <> void divide( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void divide( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void divide( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void divide( Float64* destination, const Float64* a, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void divide( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return divide( (T*)destination, (const T*)a, (const T*)b, number*size );
}




/// Divide the elements in an array by a scalar and store the result in the destination array.
template < typename T >
OM_INLINE void divide( T* destination, const T* a, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = (*a)/(scalar);
		a++;
		destination++;
	}
}

template <> void divide( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void divide( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void divide( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void divide( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void divide( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return divide( (T*)destination, (const T*)a, scalar, number*size );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Divide-Add Methods
//############		
//##########################################################################################
//##########################################################################################




/// Divide each entry in the destination by a scalar, then add the result to the destination array entry.
template < typename T >
OM_INLINE void divideAdd( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*destination)/(scalar);
		destination++;
	}
}

template <> void divideAdd( Int32* destination, const Int32& scalar, Size number );
template <> void divideAdd( Int64* destination, const Int64& scalar, Size number );
template <> void divideAdd( Float32* destination, const Float32& scalar, Size number );
template <> void divideAdd( Float64* destination, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void divideAdd( VectorND<T,size>* destination, T scalar, Size number )
{
	return divideAdd( (T*)destination, scalar, number*size );
}




/// Divide each entry in the destination by another array, then add the result to the destination entry.
template < typename T >
OM_INLINE void divideAdd( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*destination)/(*b);
		b++;
		destination++;
	}
}

template <> void divideAdd( Int32* destination, const Int32* b, Size number );
template <> void divideAdd( Int64* destination, const Int64* b, Size number );
template <> void divideAdd( Float32* destination, const Float32* b, Size number );
template <> void divideAdd( Float64* destination, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void divideAdd( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return divideAdd( (T*)destination, (const T*)b, number*size );
}




/// Divide the enties of one array by another and add the results to the destination array entries.
template < typename T >
OM_INLINE void divideAdd( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*a)/(*b);
		a++;
		b++;
		destination++;
	}
}

template <> void divideAdd( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void divideAdd( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void divideAdd( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void divideAdd( Float64* destination, const Float64* a, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void divideAdd( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return divideAdd( (T*)destination, (const T*)a, (const T*)b, number*size );
}




/// Divide the entries of an array by a scalar, then add the result to a destination array.
template < typename T >
OM_INLINE void divideAdd( T* destination, const T* a, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) += (*a)/(scalar);
		a++;
		destination++;
	}
}

template <> void divideAdd( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void divideAdd( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void divideAdd( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void divideAdd( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void divideAdd( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return divideAdd( (T*)destination, (const T*)a, scalar, number*size );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Divide-Subtract Methods
//############		
//##########################################################################################
//##########################################################################################




/// Divide each entry in the destination by a scalar, then subtract the result from the destination array entry.
template < typename T >
OM_INLINE void divideSubtract( T* destination, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*destination)/(scalar);
		destination++;
	}
}

template <> void divideSubtract( Int32* destination, const Int32& scalar, Size number );
template <> void divideSubtract( Int64* destination, const Int64& scalar, Size number );
template <> void divideSubtract( Float32* destination, const Float32& scalar, Size number );
template <> void divideSubtract( Float64* destination, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void divideSubtract( VectorND<T,size>* destination, T scalar, Size number )
{
	return divideSubtract( (T*)destination, scalar, number*size );
}




/// Divide each entry in the destination by another array, then subtract the result from the destination entry.
template < typename T >
OM_INLINE void divideSubtract( T* destination, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*destination)/(*b);
		b++;
		destination++;
	}
}

template <> void divideSubtract( Int32* destination, const Int32* b, Size number );
template <> void divideSubtract( Int64* destination, const Int64* b, Size number );
template <> void divideSubtract( Float32* destination, const Float32* b, Size number );
template <> void divideSubtract( Float64* destination, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void divideSubtract( VectorND<T,size>* destination, const VectorND<T,size>* b, Size number )
{
	return divideSubtract( (T*)destination, (const T*)b, number*size );
}




/// Divide the enties of one array by another and subtract the results from the destination array entries.
template < typename T >
OM_INLINE void divideSubtract( T* destination, const T* a, const T* b, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*a)/(*b);
		a++;
		b++;
		destination++;
	}
}

template <> void divideSubtract( Int32* destination, const Int32* a, const Int32* b, Size number );
template <> void divideSubtract( Int64* destination, const Int64* a, const Int64* b, Size number );
template <> void divideSubtract( Float32* destination, const Float32* a, const Float32* b, Size number );
template <> void divideSubtract( Float64* destination, const Float64* a, const Float64* b, Size number );

template < typename T, Size size >
OM_INLINE void divideSubtract( VectorND<T,size>* destination, const VectorND<T,size>* a, const VectorND<T,size>* b, Size number )
{
	return divideSubtract( (T*)destination, (const T*)a, (const T*)b, number*size );
}




/// Divide the entries of an array by a scalar, then subtract the result from a destination array.
template < typename T >
OM_INLINE void divideSubtract( T* destination, const T* a, const T& scalar, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) -= (*a)/(scalar);
		a++;
		destination++;
	}
}

template <> void divideSubtract( Int32* destination, const Int32* a, const Int32& scalar, Size number );
template <> void divideSubtract( Int64* destination, const Int64* a, const Int64& scalar, Size number );
template <> void divideSubtract( Float32* destination, const Float32* a, const Float32& scalar, Size number );
template <> void divideSubtract( Float64* destination, const Float64* a, const Float64& scalar, Size number );

template < typename T, Size size >
OM_INLINE void divideSubtract( VectorND<T,size>* destination, const VectorND<T,size>* a, T scalar, Size number )
{
	return divideSubtract( (T*)destination, (const T*)a, scalar, number*size );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Array Function Methods
//############		
//##########################################################################################
//##########################################################################################




/// Compute the absolute value of each entry in an array and store the result in the same array.
template < typename T >
OM_INLINE void abs( T* destination, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::abs(*destination);
		destination++;
	}
}

template <> void abs( Int32* destination, Size number );
template <> void abs( Float32* destination, Size number );
template <> void abs( Float64* destination, Size number );




/// Compute the absolute value of each entry in an array and store the result in the destination array.
template < typename T >
OM_INLINE void abs( T* destination, const T* a, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::abs(*a);
		a++;
		destination++;
	}
}

template <> void abs( Int32* destination, const Int32* a, Size number );
template <> void abs( Float32* destination, const Float32* a, Size number );
template <> void abs( Float64* destination, const Float64* a, Size number );




/// Compute the square root of each entry in an array and store the result in the same array.
template < typename T >
OM_INLINE void sqrt( T* destination, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::sqrt(*destination);
		destination++;
	}
}

template <> void sqrt( Int32* destination, Size number );
template <> void sqrt( Float32* destination, Size number );
template <> void sqrt( Float64* destination, Size number );




/// Compute the square root of each entry in an array and store the result in the destination array.
template < typename T >
OM_INLINE void sqrt( T* destination, const T* a, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::sqrt(*a);
		a++;
		destination++;
	}
}

template <> void sqrt( Int32* destination, const Int32* a, Size number );
template <> void sqrt( Float32* destination, const Float32* a, Size number );
template <> void sqrt( Float64* destination, const Float64* a, Size number );




//##########################################################################################
//##########################################################################################
//############		
//############		Rounding Methods
//############		
//##########################################################################################
//##########################################################################################




/// Compute the floor of each entry in an array and store the result in the same array.
template < typename T >
OM_INLINE void floor( T* destination, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::floor(*destination);
		destination++;
	}
}

template <> void floor( Float32* destination, Size number );
template <> void floor( Float64* destination, Size number );




/// Compute the floor of each entry in an array and store the result in the destination array.
template < typename T >
OM_INLINE void floor( T* destination, const T* a, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::floor(*a);
		a++;
		destination++;
	}
}

template <> void floor( Float32* destination, const Float32* a, Size number );
template <> void floor( Float64* destination, const Float64* a, Size number );




/// Compute the ceiling of each entry in an array and store the result in the same array.
template < typename T >
OM_INLINE void ceiling( T* destination, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::floor(*destination);
		destination++;
	}
}

template <> void ceiling( Float32* destination, Size number );
template <> void ceiling( Float64* destination, Size number );




/// Compute the ceiling of each entry in an array and store the result in the destination array.
template < typename T >
OM_INLINE void ceiling( T* destination, const T* a, Size number )
{
	const T* destinationEnd = destination + number;
	
	while ( destination != destinationEnd )
	{
		(*destination) = math::floor(*a);
		a++;
		destination++;
	}
}

template <> void ceiling( Float32* destination, const Float32* a, Size number );
template <> void ceiling( Float64* destination, const Float64* a, Size number );




//##########################################################################################
//##########################################################################################
//############		
//############		Min/Max Methods
//############		
//##########################################################################################
//##########################################################################################




/// Find the smaller of each pair of entries in two arrays and store the result in the destination.
template < typename T >
void min( T* destination, const T* a, const T* b, Size number );


/// Compute and return the minimum value in the specified array.
template < typename T >
T min( const T* array, Size number );




/// Find the larger of each pair of entries in two arrays and store the result in the destination.
template < typename T >
void max( T* destination, const T* a, const T* b, Size number );


/// Compute and return the maximum value in the specified array.
template < typename T >
T max( const T* array, Size number );




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_ARRAY_MATH_H
