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

#ifndef INCLUDE_OM_SIMD_TYPES_H
#define INCLUDE_OM_SIMD_TYPES_H


#include "omSIMDConfig.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that contains a way to access various global properties of SIMD processing.
template < typename T, Size width >
class SIMDTypeN
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = width;
		
		/// The required alignment of a SIMD vector of the SIMD type.
		static const Size ALIGNMENT = sizeof(T);
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef T Scalar;
		
		/// The base type to use for vectors of the SIMD type.
		struct Vector
		{
			T x[WIDTH];
		};
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Int8 Specializations
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that contains attributes for Int8-typed SIMD processing.
template <>
class SIMDTypeN<Int8,16>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 16;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 1;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Int8 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		typedef OM_ALTIVEC_VECTOR char Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		typedef __m128i Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		typedef int8x16_t Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//********************************************************************************
/// A class that contains attributes for Int8-typed SIMD processing.
template <>
class SIMDTypeN<Int8,8>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 8;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 1;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Int8 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		typedef int8x8_t Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Int16 Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that contains attributes for Int16-typed SIMD processing.
template <>
class SIMDTypeN<Int16,8>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 8;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 2;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Int16 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		typedef OM_ALTIVEC_VECTOR short Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		typedef __m128i Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		typedef int16x8_t Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Int32 Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that contains attributes for Int32-typed SIMD processing.
template <>
class SIMDTypeN<Int32,4>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 4;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 4;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Int32 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		typedef OM_ALTIVEC_VECTOR int Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		typedef __m128i Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		typedef int32x4_t Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//********************************************************************************
/// A class that contains attributes for Int32-typed SIMD processing.
template <>
class SIMDTypeN<Int32,8>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 8;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,1)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 4;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Int32 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,1)
		typedef __m256i Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Int64 Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that contains attributes for Int64-typed SIMD processing.
template <>
class SIMDTypeN<Int64,2>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 2;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 8;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Int64 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		typedef __m128i Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//********************************************************************************
/// A class that contains attributes for Int64-typed SIMD processing.
template <>
class SIMDTypeN<Int64,4>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 4;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,1)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 8;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Int64 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,1)
		typedef __m256i Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Float32 Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that contains attributes for Float32-typed SIMD processing.
template <>
class SIMDTypeN<Float32,4>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 4;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
		static const Size ALIGNMENT = 16;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 4;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Float32 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_ALTIVEC)
		typedef OM_ALTIVEC_VECTOR float Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(1,0)
		typedef __m128 Vector;
#elif OM_USE_SIMD && defined(OM_SIMD_ARM_NEON)
		typedef float32x4_t Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//********************************************************************************
/// A class that contains attributes for Float32-typed SIMD processing.
template <>
class SIMDTypeN<Float32,8>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 8;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,0)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 4;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Float32 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,0)
		typedef __m256 Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Float64 Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that contains attributes for Float64-typed SIMD processing.
template <>
class SIMDTypeN<Float64,2>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 2;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 8;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Float64 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(2,0)
		typedef __m128d Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//********************************************************************************
/// A class that contains attributes for Float64-typed SIMD processing.
template <>
class SIMDTypeN<Float64,4>
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 4;
		
		/// The required alignment of a SIMD vector of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,0)
		static const Size ALIGNMENT = 16;
#else
		static const Size ALIGNMENT = 8;
#endif
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef Float64 Scalar;
		
		/// The base type to use for vectors of the SIMD type.
#if OM_USE_SIMD && defined(OM_SIMD_SSE) && OM_SSE_VERSION_IS_SUPPORTED(5,0)
		typedef __m256d Vector;
#else
		struct Vector
		{
			Scalar x[WIDTH];
		};
#endif
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Default Type Specialization
//############		
//##########################################################################################
//##########################################################################################




//********************************************************************************
/// A class that contains default attributes for SIMD processing of a template type.
template < typename T >
class SIMDType
{
	public:
		
		/// The width of a SIMD vector of the SIMD type.
		static const Size WIDTH = 1;
		
		/// The required alignment of a SIMD vector of the SIMD type.
		static const Size ALIGNMENT = sizeof(T);
		
		/// The base type to use for scalar elements of the SIMD type.
		typedef T Scalar;
		
		/// The base type to use for vectors of the SIMD type.
		struct Vector
		{
			T x[WIDTH];
		};
		
};




//********************************************************************************
/// A class that contains attributes for Int8-typed SIMD processing.
template <>
class SIMDType<Int8> : public SIMDTypeN<Int8,16>
{
};




//********************************************************************************
/// A class that contains attributes for Int16-typed SIMD processing.
template <>
class SIMDType<Int16> : public SIMDTypeN<Int16,8>
{
};




//********************************************************************************
/// A class that contains attributes for Int32-typed SIMD processing.
template <>
class SIMDType<Int32> : public SIMDTypeN<Int32,4>
{
};




//********************************************************************************
/// A class that contains attributes for Int64-typed SIMD processing.
template <>
class SIMDType<Int64> : public SIMDTypeN<Int64,2>
{
};




//********************************************************************************
/// A class that contains attributes for Float32-typed SIMD processing.
template <>
class SIMDType<Float32> : public SIMDTypeN<Float32,4>
{
};




//********************************************************************************
/// A class that contains attributes for Float32-typed SIMD processing.
template <>
class SIMDType<Float64> : public SIMDTypeN<Float64,2>
{
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_TYPES_H
