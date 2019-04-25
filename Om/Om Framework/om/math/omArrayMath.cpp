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

#include "omArrayMath.h"


#include "omSIMD.h"


#define UNROLL_COUNT 4


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op Unary Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_UNARY( functionName, T, OP1, OP2, unroll ) \
template <>\
void functionName( T* destination, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short arrays. */\
	if ( number < simdWidth )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 (OP2 *destination);\
			destination++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	/* Do the unaligned part at the beginning.*/\
	\
	while ( destination < alignedDest )\
	{\
		*destination OP1 (OP2 *destination);\
		destination++;\
	}\
	\
	/*************************************************************/\
	/* Do the middle SIMD part.*/\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 (OP2 ((SIMDT*)destination)[i]);\
		\
		destination += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 (OP2 *destination);\
		destination++;\
	}\
}\



ARRAY_OP_UNARY( negate, Int32, =, -, UNROLL_COUNT );
ARRAY_OP_UNARY( negate, Int64, =, -, UNROLL_COUNT );
ARRAY_OP_UNARY( negate, Float32, =, -, UNROLL_COUNT );
ARRAY_OP_UNARY( negate, Float64, =, -, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op-Scalar Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_OP_SCALAR( functionName, T, OP1, OP2, unroll ) \
template <>\
void functionName( T* destination, const T& b, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short arrays. */\
	if ( number < simdWidth )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 *destination OP2 b;\
			destination++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	/* Do the unaligned part at the beginning.*/\
	\
	while ( destination < alignedDest )\
	{\
		*destination OP1 *destination OP2 b;\
		destination++;\
	}\
	\
	/*************************************************************/\
	/* Do the middle SIMD part.*/\
	\
	const SIMDT simdB = b;\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 ((SIMDT*)destination)[i] OP2 simdB;\
		\
		destination += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 *destination OP2 b;\
		destination++;\
	}\
}\




ARRAY_OP_OP_SCALAR( add, Int32, =, +, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( add, Int64, =, +, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( add, Float32, =, +, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( add, Float64, =, +, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( subtract, Int32, =, -, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( subtract, Int64, =, -, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( subtract, Float32, =, -, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( subtract, Float64, =, -, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiply, Int32, =, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiply, Int64, =, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiply, Float32, =, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiply, Float64, =, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplyAdd, Int32, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplyAdd, Int64, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplyAdd, Float32, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplyAdd, Float64, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplySubtract, Int32, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplySubtract, Int64, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplySubtract, Float32, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( multiplySubtract, Float64, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divide, Int32, =, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divide, Int64, =, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divide, Float32, =, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divide, Float64, =, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideAdd, Int32, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideAdd, Int64, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideAdd, Float32, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideAdd, Float64, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideSubtract, Int32, -=, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideSubtract, Int64, -=, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideSubtract, Float32, -=, /, UNROLL_COUNT );
ARRAY_OP_OP_SCALAR( divideSubtract, Float64, -=, /, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op-Array Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_ARRAY_OP_SCALAR( functionName, T, OP1, OP2, unroll ) \
template <>\
void functionName( T* destination, const T* a, const T& b, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( a != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth || destAlignment != aAlignment )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 *a OP2 b;\
			destination++;\
			a++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( destination < alignedDest )\
	{\
		*destination OP1 *a OP2 b;\
		destination++;\
		a++;\
	}\
	\
	/*************************************************************/\
	\
	const SIMDT simdB = b;\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 ((SIMDT*)a)[i] OP2 simdB;\
		\
		destination += simdWidth;\
		a += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 *a OP2 b;\
		destination++;\
		a++;\
	}\
}\




ARRAY_OP_ARRAY_OP_SCALAR( add, Int32, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( add, Int64, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( add, Float32, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( add, Float64, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( subtract, Int32, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( subtract, Int64, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( subtract, Float32, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( subtract, Float64, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiply, Int32, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiply, Int64, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiply, Float32, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiply, Float64, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplyAdd, Int32, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplyAdd, Int64, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplyAdd, Float32, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplyAdd, Float64, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplySubtract, Int32, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplySubtract, Int64, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplySubtract, Float32, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( multiplySubtract, Float64, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divide, Int32, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divide, Int64, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divide, Float32, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divide, Float64, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideAdd, Int32, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideAdd, Int64, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideAdd, Float32, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideAdd, Float64, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideSubtract, Int32, -=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideSubtract, Int64, -=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideSubtract, Float32, -=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_SCALAR( divideSubtract, Float64, -=, /, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op-Array Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_OP_ARRAY( functionName, T, OP1, OP2, unroll ) \
template <>\
void functionName( T* destination, const T* b, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( b != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)b) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const Size bAlignment = ((PointerInt)b) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth || destAlignment != bAlignment )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 *destination OP2 *b;\
			destination++;\
			b++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( destination < alignedDest )\
	{\
		*destination OP1 *destination OP2 *b;\
		destination++;\
		b++;\
	}\
	\
	/*************************************************************/\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 ((SIMDT*)destination)[i] OP2 ((SIMDT*)b)[i];\
		\
		destination += simdWidth;\
		b += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 *destination OP2 *b;\
		destination++;\
		b++;\
	}\
}\




ARRAY_OP_OP_ARRAY( add, Int32, =, +, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( add, Int64, =, +, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( add, Float32, =, +, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( add, Float64, =, +, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( subtract, Int32, =, -, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( subtract, Int64, =, -, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( subtract, Float32, =, -, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( subtract, Float64, =, -, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiply, Int32, =, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiply, Int64, =, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiply, Float32, =, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiply, Float64, =, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplyAdd, Int32, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplyAdd, Int64, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplyAdd, Float32, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplyAdd, Float64, +=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplySubtract, Int32, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplySubtract, Int64, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplySubtract, Float32, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( multiplySubtract, Float64, -=, *, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divide, Int32, =, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divide, Int64, =, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divide, Float32, =, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divide, Float64, =, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideAdd, Int32, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideAdd, Int64, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideAdd, Float32, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideAdd, Float64, +=, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideSubtract, Int32, -=, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideSubtract, Int64, -=, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideSubtract, Float32, -=, /, UNROLL_COUNT );
ARRAY_OP_OP_ARRAY( divideSubtract, Float64, -=, /, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op-Array Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_ARRAY_OP_ARRAY( functionName, T, OP1, OP2, unroll ) \
template <>\
void functionName( T* destination, const T* a, const T* b, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( a != NULL );\
	OM_DEBUG_ASSERT( b != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)b) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();\
	const Size bAlignment = ((PointerInt)b) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth || destAlignment != aAlignment || destAlignment != bAlignment )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 *a OP2 *b;\
			destination++;\
			a++;\
			b++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( destination < alignedDest )\
	{\
		*destination OP1 *a OP2 *b;\
		destination++;\
		a++;\
		b++;\
	}\
	\
	/*************************************************************/\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 ((SIMDT*)a)[i] OP2 ((SIMDT*)b)[i];\
		\
		destination += simdWidth;\
		a += simdWidth;\
		b += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 *a OP2 *b;\
		destination++;\
		a++;\
		b++;\
	}\
}\




ARRAY_OP_ARRAY_OP_ARRAY( add, Int32, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( add, Int64, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( add, Float32, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( add, Float64, =, +, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( subtract, Int32, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( subtract, Int64, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( subtract, Float32, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( subtract, Float64, =, -, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiply, Int32, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiply, Int64, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiply, Float32, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiply, Float64, =, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplyAdd, Int32, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplyAdd, Int64, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplyAdd, Float32, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplyAdd, Float64, +=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplySubtract, Int32, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplySubtract, Int64, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplySubtract, Float32, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( multiplySubtract, Float64, -=, *, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divide, Int32, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divide, Int64, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divide, Float32, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divide, Float64, =, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideAdd, Int32, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideAdd, Int64, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideAdd, Float32, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideAdd, Float64, +=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideSubtract, Int32, -=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideSubtract, Int64, -=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideSubtract, Float32, -=, /, UNROLL_COUNT );
ARRAY_OP_ARRAY_OP_ARRAY( divideSubtract, Float64, -=, /, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op-Function Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_FUNCTION( functionName, T, OP1, F, unroll ) \
template <>\
void functionName( T* destination, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short arrays. */\
	if ( number < simdWidth )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 F( *destination );\
			destination++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	/* Do the unaligned part at the beginning.*/\
	\
	while ( destination < alignedDest )\
	{\
		*destination OP1 F( *destination );\
		destination++;\
	}\
	\
	/*************************************************************/\
	/* Do the middle SIMD part.*/\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 F( ((SIMDT*)destination)[i] );\
		\
		destination += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 F( *destination );\
		destination++;\
	}\
}\




ARRAY_OP_FUNCTION( abs, Int32, =, math::abs, UNROLL_COUNT );
ARRAY_OP_FUNCTION( abs, Float32, =, math::abs, UNROLL_COUNT );
ARRAY_OP_FUNCTION( abs, Float64, =, math::abs, UNROLL_COUNT );
ARRAY_OP_FUNCTION( sqrt, Float32, =, math::sqrt, UNROLL_COUNT );
ARRAY_OP_FUNCTION( sqrt, Float64, =, math::sqrt, UNROLL_COUNT );
ARRAY_OP_FUNCTION( ceiling, Float32, =, math::ceiling, UNROLL_COUNT );
ARRAY_OP_FUNCTION( ceiling, Float64, =, math::ceiling, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op-Function(Array) Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_FUNCTION_ARRAY( functionName, T, OP1, F, unroll ) \
template <>\
void functionName( T* destination, const T* b, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( b != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)b) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const Size bAlignment = ((PointerInt)b) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth || destAlignment != bAlignment )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 F( *b );\
			destination++;\
			b++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( destination < alignedDest )\
	{\
		*destination OP1 F( *b );\
		destination++;\
		b++;\
	}\
	\
	/*************************************************************/\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 F( ((SIMDT*)b)[i] );\
		\
		destination += simdWidth;\
		b += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 F( *b );\
		destination++;\
		b++;\
	}\
}\




ARRAY_OP_FUNCTION_ARRAY( abs, Float32, =, math::abs, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY( abs, Float64, =, math::abs, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY( sqrt, Float32, =, math::sqrt, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY( sqrt, Float64, =, math::sqrt, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY( ceiling, Float32, =, math::ceiling, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY( ceiling, Float64, =, math::ceiling, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array-Op-Function(Array,Array) Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_OP_FUNCTION_ARRAY_ARRAY( functionName, T, OP1, F, unroll ) \
template <>\
void functionName( T* destination, const T* a, const T* b, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( destination != NULL );\
	OM_DEBUG_ASSERT( a != NULL );\
	OM_DEBUG_ASSERT( b != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)b) % sizeof(T) == 0 );\
	\
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();\
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();\
	const Size bAlignment = ((PointerInt)b) % SIMDT::getAlignment();\
	const T* const destinationEnd = destination + number;\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth || destAlignment != aAlignment || destAlignment != bAlignment )\
	{\
		while ( destination < destinationEnd )\
		{\
			*destination OP1 F( *a, *b );\
			destination++;\
			a++;\
			b++;\
		}\
		return;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedDest = destination + numPreScalar;\
	const T* const simdDestEnd = alignedDest + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( destination < alignedDest )\
	{\
		*destination OP1 F( *a, *b );\
		destination++;\
		a++;\
		b++;\
	}\
	\
	/*************************************************************/\
	\
	while ( destination < simdDestEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			((SIMDT*)destination)[i] OP1 F( ((SIMDT*)a)[i], ((SIMDT*)b)[i] );\
		\
		destination += simdWidth;\
		a += simdWidth;\
		b += simdWidth;\
	}\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( destination < destinationEnd )\
	{\
		*destination OP1 F( *a, *b );\
		destination++;\
		a++;\
		b++;\
	}\
}\




ARRAY_OP_FUNCTION_ARRAY_ARRAY( min, Int32, =, math::min, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY_ARRAY( min, Float32, =, math::min, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY_ARRAY( min, Float64, =, math::min, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY_ARRAY( max, Int32, =, math::max, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY_ARRAY( max, Float32, =, math::max, UNROLL_COUNT );
ARRAY_OP_FUNCTION_ARRAY_ARRAY( max, Float64, =, math::max, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Complex Multiply Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
void multiply( Complex<Float32>* destination, const Complex<Float32>* a, const Complex<Float32>* b, Size number )
{
	typedef Complex<Float32> T;
	typedef SIMDScalar<Float32,4> SIMDT;
	const Size simdWidth = UNROLL_COUNT*(SIMDT::getWidth() / 2);
	
	OM_DEBUG_ASSERT( destination != NULL );
	OM_DEBUG_ASSERT( a != NULL );
	OM_DEBUG_ASSERT( b != NULL );
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );
	OM_DEBUG_ASSERT( ((PointerInt)b) % sizeof(T) == 0 );
	
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();
	const Size bAlignment = ((PointerInt)b) % SIMDT::getAlignment();
	const T* const destinationEnd = destination + number;
	
	/* Use the simple scalar approach for short or mutually unaligned arrays. */
	if ( number < simdWidth || destAlignment != aAlignment || destAlignment != bAlignment )
	{
		while ( destination < destinationEnd )
		{
			*destination = *a * *b;
			destination++;
			a++;
			b++;
		}
		return;
	}
	
	/*************************************************************/
	/* Determine how many unaligned elements there are before the aligned portion.*/
	
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;
	const Size numSIMD = numSIMDIterations*simdWidth;
	
	const T* const alignedDest = destination + numPreScalar;
	const T* const simdDestEnd = alignedDest + numSIMD;
	
	/*************************************************************/
	
	/* Do the unaligned part at the beginning.*/
	while ( destination < alignedDest )
	{
		*destination = *a * *b;
		destination++;
		a++;
		b++;
	}
	
	/*************************************************************/
	
	while ( destination < simdDestEnd )
	{
		for ( Index i = 0; i < UNROLL_COUNT; i++ )
		{
			// inputSIMD = (a0 b0 a1 b1)
			// lows = (a0 a0 a1 a1)
			SIMDT lows = math::lows( ((SIMDT*)a)[i] );
			SIMDT temp1 = lows*(((SIMDT*)b)[i]);
			
			// highs = (b0 b0 b1 b1)
			SIMDT highs = math::highs( ((SIMDT*)a)[i] );
			SIMDT temp2 = highs*math::shuffle<1,0,3,2>(((SIMDT*)b)[i]);
			
			((SIMDT*)destination)[i] = math::subAdd( temp1, temp2 );
		}
		
		destination += simdWidth;
		a += simdWidth;
		b += simdWidth;
	}
	
	/*************************************************************/
	/* Do the scalar part at the end.*/
	
	while ( destination < destinationEnd )
	{
		*destination = *a * *b;
		destination++;
		a++;
		b++;
	}
}




template <>
void multiplyAdd( Complex<Float32>* destination, const Complex<Float32>* a, const Complex<Float32>* b, Size number )
{
	typedef Complex<Float32> T;
	typedef SIMDScalar<Float32,4> SIMDT;
	const Size simdWidth = UNROLL_COUNT*(SIMDT::getWidth() / 2);
	
	OM_DEBUG_ASSERT( destination != NULL );
	OM_DEBUG_ASSERT( a != NULL );
	OM_DEBUG_ASSERT( b != NULL );
	OM_DEBUG_ASSERT( ((PointerInt)destination) % sizeof(T) == 0 );
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );
	OM_DEBUG_ASSERT( ((PointerInt)b) % sizeof(T) == 0 );
	
	const Size destAlignment = ((PointerInt)destination) % SIMDT::getAlignment();
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();
	const Size bAlignment = ((PointerInt)b) % SIMDT::getAlignment();
	const T* const destinationEnd = destination + number;
	
	/* Use the simple scalar approach for short or mutually unaligned arrays. */
	if ( number < simdWidth || destAlignment != aAlignment || destAlignment != bAlignment )
	{
		while ( destination < destinationEnd )
		{
			*destination += *a * *b;
			destination++;
			a++;
			b++;
		}
		return;
	}
	
	/*************************************************************/
	/* Determine how many unaligned elements there are before the aligned portion.*/
	
	const Size numPreScalar = SIMDT::getWidth() - destAlignment / sizeof(T);
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;
	const Size numSIMD = numSIMDIterations*simdWidth;
	
	const T* const alignedDest = destination + numPreScalar;
	const T* const simdDestEnd = alignedDest + numSIMD;
	
	/*************************************************************/
	
	/* Do the unaligned part at the beginning.*/
	while ( destination < alignedDest )
	{
		*destination += *a * *b;
		destination++;
		a++;
		b++;
	}
	
	/*************************************************************/
	
	while ( destination < simdDestEnd )
	{
		for ( Index i = 0; i < UNROLL_COUNT; i++ )
		{
			// inputSIMD = (a0 b0 a1 b1)
			// lows = (a0 a0 a1 a1)
			SIMDT lows = math::lows( ((SIMDT*)a)[i] );
			SIMDT temp1 = lows*(((SIMDT*)b)[i]);
			
			// highs = (b0 b0 b1 b1)
			SIMDT highs = math::highs( ((SIMDT*)a)[i] );
			SIMDT temp2 = highs*math::shuffle<1,0,3,2>(((SIMDT*)b)[i]);
			
			((SIMDT*)destination)[i] += math::subAdd( temp1, temp2 );
		}
		
		destination += simdWidth;
		a += simdWidth;
		b += simdWidth;
	}
	
	/*************************************************************/
	/* Do the scalar part at the end.*/
	
	while ( destination < destinationEnd )
	{
		*destination += *a * *b;
		destination++;
		a++;
		b++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Dot Product Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_NORM( functionName, T, unroll ) \
template <>\
T functionName( const T* a, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( a != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );\
	\
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();\
	const T* const aEnd = a + number;\
	T result( 0 );\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth )\
	{\
		while ( a < aEnd )\
		{\
			result += *a * *a;\
			a++;\
		}\
		return math::sqrt( result );\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - aAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedA = a + numPreScalar;\
	const T* const simdAEnd = alignedA + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( a < alignedA )\
	{\
		result += *a * *a;\
		a++;\
	}\
	\
	/*************************************************************/\
	\
	SIMDT simdResult( T(0) );\
	\
	while ( a < simdAEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			simdResult += ((SIMDT*)a)[i] * ((SIMDT*)a)[i];\
		\
		a += simdWidth;\
	}\
	\
	result += math::sumScalar( simdResult );\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( a < aEnd )\
	{\
		result += *a * *a;\
		a++;\
	}\
	\
	return math::sqrt( result );\
}\




ARRAY_NORM( norm, Float32, UNROLL_COUNT );
ARRAY_NORM( norm, Float64, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Dot Product Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_DOT_ARRAY( functionName, T, unroll ) \
template <>\
T functionName( const T* a, const T* b, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( a != NULL );\
	OM_DEBUG_ASSERT( b != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );\
	OM_DEBUG_ASSERT( ((PointerInt)b) % sizeof(T) == 0 );\
	\
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();\
	const Size bAlignment = ((PointerInt)b) % SIMDT::getAlignment();\
	const T* const aEnd = a + number;\
	T result( 0 );\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth || aAlignment != bAlignment )\
	{\
		while ( a < aEnd )\
		{\
			result += *a * *b;\
			a++;\
			b++;\
		}\
		return result;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - aAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedA = a + numPreScalar;\
	const T* const simdAEnd = alignedA + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( a < alignedA )\
	{\
		result += *a * *b;\
		a++;\
		b++;\
	}\
	\
	/*************************************************************/\
	\
	SIMDT simdResult( T(0) );\
	\
	while ( a < simdAEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			simdResult += ((SIMDT*)a)[i] * ((SIMDT*)b)[i];\
		\
		a += simdWidth;\
		b += simdWidth;\
	}\
	\
	result += math::sumScalar( simdResult );\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( a < aEnd )\
	{\
		result += *a * *b;\
		a++;\
		b++;\
	}\
	\
	return result;\
}\




ARRAY_DOT_ARRAY( dot, Float32, UNROLL_COUNT );
ARRAY_DOT_ARRAY( dot, Float64, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array Sum Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_SUM( functionName, T, unroll ) \
template <>\
T functionName( const T* array, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( array != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)array) % sizeof(T) == 0 );\
	\
	const Size arrayAlignment = ((PointerInt)array) % SIMDT::getAlignment();\
	const T* const arrayEnd = array + number;\
	T result = T(0);\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth )\
	{\
		while ( array < arrayEnd )\
		{\
			result += *array;\
			array++;\
		}\
		return result;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - arrayAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedArray = array + numPreScalar;\
	const T* const simdArrayEnd = alignedArray + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( array < alignedArray )\
	{\
		result += *array;\
		array++;\
	}\
	\
	/*************************************************************/\
	\
	SIMDT simdResult( T(0) );\
	\
	while ( array < simdArrayEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			simdResult += ((SIMDT*)array)[i];\
		\
		array += simdWidth;\
	}\
	\
	result += math::sumScalar( simdResult );\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( array < arrayEnd )\
	{\
		result += *array;\
		array++;\
	}\
	\
	return result;\
}\




ARRAY_SUM( sum, Float32, UNROLL_COUNT );
ARRAY_SUM( sum, Float64, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Array Product Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_PRODUCT( functionName, T, unroll ) \
template <>\
T functionName( const T* array, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( array != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)array) % sizeof(T) == 0 );\
	\
	const Size arrayAlignment = ((PointerInt)array) % SIMDT::getAlignment();\
	const T* const arrayEnd = array + number;\
	T result = T(1);\
	\
	/* Use the simple scalar approach for short or mutually unaligned arrays. */\
	if ( number < simdWidth )\
	{\
		while ( array < arrayEnd )\
		{\
			result *= *array;\
			array++;\
		}\
		return result;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - arrayAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedArray = array + numPreScalar;\
	const T* const simdArrayEnd = alignedArray + numSIMD;\
	\
	/*************************************************************/\
	\
	/* Do the unaligned part at the beginning.*/\
	while ( array < alignedArray )\
	{\
		result *= *array;\
		array++;\
	}\
	\
	/*************************************************************/\
	\
	SIMDT simdResult( T(1) );\
	\
	while ( array < simdArrayEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			simdResult *= ((SIMDT*)array)[i];\
		\
		array += simdWidth;\
	}\
	\
	for ( Index i = 0; i < SIMDT::getWidth(); i++ )\
		result *= simdResult[i];\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( array < arrayEnd )\
	{\
		result *= *array;\
		array++;\
	}\
	\
	return result;\
}\




ARRAY_PRODUCT( product, Float32, UNROLL_COUNT );
ARRAY_PRODUCT( product, Float64, UNROLL_COUNT );




//##########################################################################################
//##########################################################################################
//############		
//############		Min/Max Method
//############		
//##########################################################################################
//##########################################################################################




#define ARRAY_MIN_MAX_FUNCTION( functionName, T, F, unroll ) \
template <>\
T functionName( const T* a, Size number )\
{\
	typedef SIMDScalar<T,SIMDType<T>::WIDTH> SIMDT;\
	const Size simdWidth = unroll*SIMDT::getWidth();\
	\
	OM_DEBUG_ASSERT( a != NULL );\
	OM_DEBUG_ASSERT( ((PointerInt)a) % sizeof(T) == 0 );\
	\
	const Size aAlignment = ((PointerInt)a) % SIMDT::getAlignment();\
	const T* const aEnd = a + number;\
	T result = *a;\
	\
	/* Use the simple scalar approach for short arrays. */\
	if ( number < simdWidth )\
	{\
		a++;\
		while ( a < aEnd )\
		{\
			result = F( result, *a );\
			a++;\
		}\
		return result;\
	}\
	\
	/*************************************************************/\
	/* Determine how many unaligned elements there are before the aligned portion.*/\
	\
	const Size numPreScalar = SIMDT::getWidth() - aAlignment / sizeof(T);\
	const Size numSIMDIterations = (number - numPreScalar) / simdWidth;\
	const Size numSIMD = numSIMDIterations*simdWidth;\
	\
	const T* const alignedA = a + numPreScalar;\
	const T* const simdAEnd = alignedA + numSIMD;\
	a++;\
	\
	/*************************************************************/\
	/* Do the unaligned part at the beginning.*/\
	\
	while ( a < alignedA )\
	{\
		result = F( result, *a );\
		a++;\
	}\
	\
	/*************************************************************/\
	/* Do the middle SIMD part.*/\
	\
	SIMDT simdResult(result);\
	\
	while ( a < simdAEnd )\
	{\
		for ( Index i = 0; i < unroll; i++ )\
			simdResult = F( simdResult, ((SIMDT*)a)[i] );\
		\
		a += simdWidth;\
	}\
	\
	result = F( simdResult )[0];\
	\
	/*************************************************************/\
	/* Do the scalar part at the end.*/\
	\
	while ( a < aEnd )\
	{\
		result = F( result, *a );\
		a++;\
	}\
	\
	return result;\
}\




ARRAY_MIN_MAX_FUNCTION( min, Float32, math::min, UNROLL_COUNT );
ARRAY_MIN_MAX_FUNCTION( min, Float64, math::min, UNROLL_COUNT );
ARRAY_MIN_MAX_FUNCTION( max, Float32, math::max, UNROLL_COUNT );
ARRAY_MIN_MAX_FUNCTION( max, Float64, math::max, UNROLL_COUNT );




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
