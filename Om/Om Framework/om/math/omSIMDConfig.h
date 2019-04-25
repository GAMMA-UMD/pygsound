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

#ifndef INCLUDE_OM_SIMD_CONFIG_H
#define INCLUDE_OM_SIMD_CONFIG_H


#include "omMathConfig.h"


#include "omScalarMath.h"


//##########################################################################################
//##########################################################################################
//############		
//############		SIMD Configuration Macros
//############		
//##########################################################################################
//##########################################################################################




/// Determine whether or not SIMD code should be used.
/**
  * If set to 1, many operations will be parallelized using SIMD vector operations.
  * This will generally increase performance but may not work on all hardware. If set to
  * 0, no SIMD operations will be used. If enabled but the hardware doesn't support SIMD
  * instructions, a serial fallback implementation will be used.
  */
#ifndef OM_USE_SIMD
	#define OM_USE_SIMD 1
#endif




/// Define the newest major version of SSE that can be used by GSound.
/**
  * This value can be used to limit the complexity of the SSE operations
  * performed when compiling for hardware that doesn't support newer SSE versions.
  * Only SSE versions up to this version (specified as an integer number)
  * can be used.
  */
#ifndef OM_SSE_MAX_MAJOR_VERSION
	#define OM_SSE_MAX_MAJOR_VERSION 4
#endif




/// Define the newest minor version of SSE that can be used by GSound.
/**
  * This value can be used to limit the complexity of the SSE operations
  * performed when compiling for hardware that doesn't support newer SSE versions.
  * Only SSE versions up to this version (specified as an integer number)
  * can be used.
  */
#ifndef OM_SSE_MAX_MINOR_VERSION
	#define OM_SSE_MAX_MINOR_VERSION 2
#endif




// Handle Altivec.
#if OM_USE_SIMD && defined(OM_CPU_POWER_PC) && defined(__ALTIVEC__)
	// Define that Altivec instructions are available.
	#define OM_SIMD_ALTIVEC 1
	
	// Redfine the keyword associated with an Altivec vector to avoid name collisions.
	#ifdef OM_COMPILER_GCC
		#undef vector
		#define OM_ALTIVEC_VECTOR __vector
	#endif
#endif




// Handle Intel SIMD.
#if OM_USE_SIMD && defined(OM_CPU_INTEL)
	// Define that SSE instructions are available.
	#define OM_SIMD_SSE 1
	
	// Define a macro which determines whether a specified (major,minor) version of SSE is allowed by the user.
	#define OM_SSE_VERSION_IS_ALLOWED( majorVersion, minorVersion ) \
				(OM_SSE_MAX_MAJOR_VERSION > majorVersion || \
				(OM_SSE_MAX_MAJOR_VERSION == majorVersion && OM_SSE_MAX_MINOR_VERSION >= minorVersion))
	
	// Determine the newest available version of SSE and include its header.
	#if defined(OM_COMPILER_GCC)
		#if defined (__AVX512F__)
			#define OM_SSE_MAJOR_VERSION 5
			#define OM_SSE_MINOR_VERSION 2
		#elif defined(__AVX2__)
			#define OM_SSE_MAJOR_VERSION 5
			#define OM_SSE_MINOR_VERSION 1
		#elif defined(__AVX__)
			#define OM_SSE_MAJOR_VERSION 5
			#define OM_SSE_MINOR_VERSION 0
		#elif defined(__SSE4_2__)
			#define OM_SSE_MAJOR_VERSION 4
			#define OM_SSE_MINOR_VERSION 2
		#elif defined(__SSE4_1__)
			#define OM_SSE_MAJOR_VERSION 4
			#define OM_SSE_MINOR_VERSION 1
		#elif defined(__SSSE3__)
			#define OM_SSE_MAJOR_VERSION 3
			#define OM_SSE_MINOR_VERSION 1
		#elif defined(__SSE3__)
			#define OM_SSE_MAJOR_VERSION 3
			#define OM_SSE_MINOR_VERSION 0
		#elif defined(__SSE2__)
			#define OM_SSE_MAJOR_VERSION 2
			#define OM_SSE_MINOR_VERSION 0
		#elif defined(__SSE__)
			#define OM_SSE_MAJOR_VERSION 1
			#define OM_SSE_MINOR_VERSION 0
		#else
			#define OM_SSE_MAJOR_VERSION 0
			#define OM_SSE_MINOR_VERSION 0
		#endif
	#elif defined(OM_COMPILER_MSVC)
		#if defined (__AVX512F__)
			#define OM_SSE_MAJOR_VERSION 5
			#define OM_SSE_MINOR_VERSION 2
		#elif defined(__AVX2__)
			#define OM_SSE_MAJOR_VERSION 5
			#define OM_SSE_MINOR_VERSION 1
		#elif defined(__AVX__)
			#define OM_SSE_MAJOR_VERSION 5
			#define OM_SSE_MINOR_VERSION 0
		#else
			// Assume the newest version because MSVC doesn't define the SSE macros.
			#define OM_SSE_MAJOR_VERSION 4
			#define OM_SSE_MINOR_VERSION 2
		#endif
	#endif
	
	
	/// A macro which produces a boolean value indicating whether the specified (major,minor) version of SSE is supported.
	#define OM_SSE_VERSION_IS_SUPPORTED( majorVersion, minorVersion ) \
				(OM_SSE_VERSION_IS_ALLOWED( majorVersion, minorVersion ) && \
				((majorVersion < OM_SSE_MAJOR_VERSION) || \
				(majorVersion == OM_SSE_MAJOR_VERSION && minorVersion <= OM_SSE_MINOR_VERSION)))
#else // !OM_SIMD_SSE
	// A macro which produces a boolean value indicating whether the specified (major,minor) version of SSE is supported.
	#define OM_SSE_VERSION_IS_SUPPORTED( majorVersion, minorVersion ) 0
#endif




// Handle ARM SIMD
#if defined(OM_CPU_ARM) && defined(__ARM_NEON__)
	#include <arm_neon.h>
	
	// Define that ARM Neon instructions are available.
	#define OM_SIMD_ARM_NEON 1
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		SIMD Header Includes
//############		
//##########################################################################################
//##########################################################################################




#if OM_SSE_VERSION_IS_SUPPORTED(1,0)
	#include <mmintrin.h> // Include for MMX intrinsics
	#include <xmmintrin.h> // Include for SSE intrinsics
#endif

#if OM_SSE_VERSION_IS_SUPPORTED(2,0)
	#include <emmintrin.h> // Include for SSE2 intrinsics
#endif

#if OM_SSE_VERSION_IS_SUPPORTED(3,0)
	#include <pmmintrin.h> // Include for SSE3 intrinsics
#endif

#if OM_SSE_VERSION_IS_SUPPORTED(3,1)
	#include <tmmintrin.h> // Include for SSSE3 intrinsics
#endif

#if OM_SSE_VERSION_IS_SUPPORTED(4,1)
	#include <smmintrin.h> // Include for SSE4.1 intrinsics
#endif

#if OM_SSE_VERSION_IS_SUPPORTED(4,2)
	#include <nmmintrin.h> // Include for SSE4.2 intrinsics
#endif

#if OM_SSE_VERSION_IS_SUPPORTED(5,0) || OM_SSE_VERSION_IS_SUPPORTED(5,1)
	#include <immintrin.h> // Include for AVX / AVX2 intrinsics
#endif

#if OM_SSE_VERSION_IS_SUPPORTED(5,2)
	#include <zmmintrin.h> // Include for AVX-512 intrinsics
#endif




//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_CONFIG_H
