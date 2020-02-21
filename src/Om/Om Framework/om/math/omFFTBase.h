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

#ifndef INCLUDE_OM_FFT_BASE_H
#define INCLUDE_OM_FFT_BASE_H


#include "omMathConfig.h"


#include "omSIMDScalar.h"
#include "omSIMDScalarFloat32_4.h"
#include "omSIMDArray.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




template < typename T >
class FFTTypeInfo
{
	public:
		typedef T Scalar;
		typedef T v4sf;
		
		static const int SIMD_SZ = 1;
		
		OM_FORCE_INLINE static v4sf VZERO() { return T(0.0f); }
		OM_FORCE_INLINE static v4sf VMUL( v4sf a, v4sf b ) { return a*b; }
		OM_FORCE_INLINE static v4sf SVMUL( Scalar scalar, v4sf v ) { return v*scalar; }
		OM_FORCE_INLINE static v4sf VADD( v4sf a, v4sf b ) { return a + b; }
		OM_FORCE_INLINE static v4sf VMADD( v4sf a, v4sf b, v4sf c ) { return a*b + c; }
		OM_FORCE_INLINE static v4sf VSUB( v4sf a, v4sf b ) { return a - b; }
		OM_FORCE_INLINE static v4sf LD_PS1( Scalar scalar ) { return v4sf(scalar); }
};




template < typename U, Size width >
class FFTTypeInfo< SIMDScalar<U,width> >
{
	public:
		typedef U Scalar;
		typedef SIMDScalar<U,width> v4sf;
		
		static const int SIMD_SZ = 1;
		
		OM_FORCE_INLINE static v4sf VZERO() { return v4sf(U(0)); }
		OM_FORCE_INLINE static v4sf VMUL( v4sf a, v4sf b ) { return a*b; }
		OM_FORCE_INLINE static v4sf SVMUL( Scalar scalar, v4sf v ) { return v*scalar; }
		OM_FORCE_INLINE static v4sf VADD( v4sf a, v4sf b ) { return a + b; }
		OM_FORCE_INLINE static v4sf VMADD( v4sf a, v4sf b, v4sf c ) { return a*b + c; }
		OM_FORCE_INLINE static v4sf VSUB( v4sf a, v4sf b ) { return a - b; }
		OM_FORCE_INLINE static v4sf LD_PS1( Scalar scalar ) { return v4sf(scalar); }
};




template < typename U, Size width >
class FFTTypeInfo< SIMDArray<U,width> >
{
	public:
		typedef U Scalar;
		typedef SIMDArray<U,width> v4sf;
		
		static const int SIMD_SZ = 1;
		
		OM_FORCE_INLINE static v4sf VZERO() { return v4sf(U(0)); }
		OM_FORCE_INLINE static v4sf VMUL( v4sf a, v4sf b ) { return a*b; }
		OM_FORCE_INLINE static v4sf SVMUL( Scalar scalar, v4sf v ) { return v*scalar; }
		OM_FORCE_INLINE static v4sf VADD( v4sf a, v4sf b ) { return a + b; }
		OM_FORCE_INLINE static v4sf VMADD( v4sf a, v4sf b, v4sf c ) { return a*b + c; }
		OM_FORCE_INLINE static v4sf VSUB( v4sf a, v4sf b ) { return a - b; }
		OM_FORCE_INLINE static v4sf LD_PS1( Scalar scalar ) { return v4sf(scalar); }
};




template <>
class FFTTypeInfo<Float32>
{
	public:
		
		typedef Float32 Scalar;
		typedef SIMDScalar<Float32,4> v4sf;
		
		static const int SIMD_SZ = 4;
		
		OM_FORCE_INLINE static v4sf VZERO() { return v4sf(0.0f); }
		OM_FORCE_INLINE static v4sf VMUL( const v4sf& a, const v4sf& b ) { return a*b; }
		OM_FORCE_INLINE static v4sf SVMUL( Scalar scalar, const v4sf& v ) { return v*scalar; }
		OM_FORCE_INLINE static v4sf VADD( const v4sf& a, const v4sf& b ) { return a + b; }
		OM_FORCE_INLINE static v4sf VMADD( const v4sf& a, const v4sf& b, const v4sf& c ) { return a*b + c; }
		OM_FORCE_INLINE static v4sf VSUB( const v4sf& a, const v4sf& b ) { return a - b; }
		OM_FORCE_INLINE static v4sf LD_PS1( Scalar scalar ) { return v4sf(scalar); }
		OM_FORCE_INLINE static void INTERLEAVE2( const v4sf& in1, const v4sf& in2, v4sf& out1, v4sf& out2 )
		{
			v4sf temp = math::interleaveLows( in1, in2 );
			out2 = math::interleaveHighs( in1, in2 );
			out1 = temp;
		}
		OM_FORCE_INLINE static void UNINTERLEAVE2( const v4sf& in1, const v4sf& in2, v4sf& out1, v4sf& out2 )
		{
			v4sf temp = math::shuffle<0,2,0,2>( in1, in2 );
			out2 = math::shuffle<1,3,1,3>( in1, in2 );
			out1 = temp;
		}
		OM_FORCE_INLINE static v4sf VSWAPHL( const v4sf& a, const v4sf& b )
		{
			return math::shuffle<0,1,2,3>( b, a );
		}
		OM_FORCE_INLINE static void VTRANSPOSE4( v4sf& r0, v4sf& r1, v4sf& r2, v4sf& r3 )
		{
			math::transpose4x4( r0, r1, r2, r3, r0, r1, r2, r3 );
		}
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FFT_BASE_H
