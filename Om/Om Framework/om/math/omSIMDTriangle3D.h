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

#ifndef INCLUDE_OM_SIMD_TRIANGLE_3D_H
#define INCLUDE_OM_SIMD_TRIANGLE_3D_H


#include "omMathConfig.h"


#include "omVector3D.h"
#include "omSIMDVector3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A template prototype declaration for the SIMDAABB3D class.
/**
  * This class is used to store and operate on a set of N 3D triangles
  * in a SIMD fashion. The triangles are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each triangle is specified by 3 vertex
  * coordinates that indicate the vertices of the triangle.
  */
template < typename T, Size dimension >
class SIMDTriangle3D;




//********************************************************************************
/// A specialization for the SIMDTriangle3D class that has a SIMD width of 4.
/**
  * This class is used to store and operate on a set of 4 3D triangles
  * in a SIMD fashion. The triangles are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each triangle is specified by 3 vertex
  * coordinates that indicate the vertices of the triangle.
  */
template < typename T >
class OM_ALIGN(16) SIMDTriangle3D<T,4>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a SIMD triangle with N copies of the specified triangle for a SIMD width of N.
			OM_FORCE_INLINE SIMDTriangle3D( const VectorND<T,3>& newV0, const VectorND<T,3>& newV1,
												const VectorND<T,3>& newV2 )
				:	v0( newV0 ),
					v1( newV1 ),
					v2( newV2 )
			{
			}
			
			
			/// Create a SIMD ray with the 4 rays it contains equal to the specified rays.
			OM_FORCE_INLINE SIMDTriangle3D( const SIMDVector3D<T,4>& newV0, const SIMDVector3D<T,4>& newV1,
												const SIMDVector3D<T,4>& newV2 )
				:	v0( newV0 ),
					v1( newV1 ),
					v2( newV2 )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The first vertex of the SIMD triangle.
			SIMDVector3D<T,4> v0;
			
			
			/// The second vertex of the SIMD triangle.
			SIMDVector3D<T,4> v1;
			
			
			/// The second vertex of the SIMD triangle.
			SIMDVector3D<T,4> v2;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_TRIANGLE_3D_H
