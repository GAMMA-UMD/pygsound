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

#include "omSIMDFlags.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




#if defined(OM_SIMD_SSE) && defined(OM_COMPILER_GCC)
#if OM_COMPILER_VERSION > OM_GCC_VERSION(4,4)
static inline UInt64 _xgetbv(unsigned int index)
{
	unsigned int eax, edx;
	
	__asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
	
	return ((unsigned long long)edx << 32) | eax;
}
#else
static inline UInt64 _xgetbv(unsigned int index)
{
	return 0;
}
#endif
#endif




SIMDFlags SIMDFlags:: get()
{
	// Start with no flags set.
	UInt32 flags = UNDEFINED;
	
	//**********************************************************************
	// If this is PowerPC, assume Altivec is supported.
	
#if defined(OM_SIMD_ALTIVEC)
	flags |= ALTIVEC;
#endif
	
	//**********************************************************************
	// If this is ARM, assume Neon is supported.
	
#if defined(OM_SIMD_ARM_NEON)
	flags |= ARM_NEON;
#endif
	
	//**********************************************************************
	// Get info about the Intel CPU
	
#if defined(OM_SIMD_SSE)
#if defined(OM_COMPILER_MSVC) && _MSC_FULL_VER >= 160040219
	// Must be VS2010 SP1 or later.
	int cpuInfo[4];
    __cpuid( cpuInfo, 0x00000001 );
	int eax = cpuInfo[0];
	int ebx = cpuInfo[1];
	int ecx = cpuInfo[2];
	int edx = cpuInfo[3];
	
#elif defined(OM_COMPILER_GCC)
	
	#define cpuid(func,ax,bx,cx,dx)\
	__asm__ __volatile__ ("cpuid":\
	"=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));
	
	int eax = 0;
	int ebx = 0;
	int ecx = 0;
	int edx = 0;
	
	cpuid( 0x00000001, eax, ebx, ecx, edx );
	
#else
	#error "Unsupported Compiler"
#endif
	
	//**********************************************************************
	// Interpret the results of the CPUID.
	
	Bool osUsesXSAVE_XRSTORE = (ecx & (1 << 27)) != 0;
	Bool sse = (edx & (1 << 25)) != 0;
	Bool sse2 = (edx & (1 << 26)) != 0;
	Bool sse3 = (ecx & (1 << 0)) != 0;
	Bool ssse3 = (ecx & (1 << 9)) != 0;
	Bool sse41 = (ecx & (1 << 19)) != 0;
	Bool sse42 = (ecx & (1 << 20)) != 0;
	Bool avx = (ecx & (1 << 28)) != 0;
	Bool avx2 = (ebx & (1 << 5)) != 0;
	
	if ( sse ) flags |= SSE;
	if ( sse2 ) flags |= SSE_2;
	if ( sse3 ) flags |= SSE_3;
	if ( ssse3 ) flags |= SSSE_3;
	if ( sse41 ) flags |= SSE_4_1;
	if ( sse42 ) flags |= SSE_4_2;
	
    if ( osUsesXSAVE_XRSTORE && (avx || avx2) )
    {
		// Check if the OS will save the YMM registers
		UInt64 xcrFeatureMask = _xgetbv( 0x00000001 );
		
		if ( xcrFeatureMask & 0x6 )
		{
			if ( avx ) flags |= AVX;
			if ( avx2 ) flags |= AVX_2;
		}
    }
	
#endif // OM_SIMD_SSE
	
	//**********************************************************************
	
	return flags;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
