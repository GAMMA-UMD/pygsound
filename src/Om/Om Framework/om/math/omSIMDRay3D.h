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

#ifndef INCLUDE_OM_SIMD_RAY_3D_H
#define INCLUDE_OM_SIMD_RAY_3D_H


#include "omMathConfig.h"


#include "omRay3D.h"
#include "omSIMDVector3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




template < typename T, Size width >
class SIMDRay3D;




//********************************************************************************
/// A class that represents a set of 3D rays stored in a SIMD-compatible format.
/**
  * This class is used to store and operate on a set of 3D rays
  * in a SIMD fashion. The rays are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each ray is specified by an origin
  * point and a direction vector.
  */
template < typename T >
class OM_ALIGN(16) SIMDRay3D<T,4>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a SIMD ray with N copies of the specified ray for a SIMD width of N.
			OM_FORCE_INLINE SIMDRay3D( const VectorND<T,3>& newOrigin, const VectorND<T,3>& newDirection )
				:	origin( newOrigin ),
					direction( newDirection )
			{
			}
			
			
			/// Create a SIMD ray with N copies of the specified ray for a SIMD width of N.
			OM_FORCE_INLINE SIMDRay3D( const Ray3D<T>& ray )
				:	origin( ray.origin ),
					direction( ray.direction )
			{
			}
			
			
			/// Create a SIMD ray with the 4 rays it contains equal to the specified rays.
			OM_FORCE_INLINE SIMDRay3D( const Ray3D<T>& ray1, const Ray3D<T>& ray2,
										const Ray3D<T>& ray3, const Ray3D<T>& ray4 )
				:	origin( ray1.origin, ray2.origin, ray3.origin, ray4.origin ),
					direction( ray1.direction, ray2.direction, ray3.direction, ray4.direction )
			{
			}
			
			
		//********************************************************************************
		//******	Required Alignment Accessor Methods
			
			
			/// Return the alignment required for objects of this type.
			/**
			  * For most SIMD types this value will be 16 bytes. If there is
			  * no alignment required, 0 is returned.
			  */
			OM_FORCE_INLINE static Size getAlignment()
			{
				return 16;
			}
			
			
			/// Get the width of this vector (number of 3D vectors it has).
			OM_FORCE_INLINE static Size getWidth()
			{
				return 4;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A SIMD 3D vector indicating the origin of the ray(s).
			SIMDVector3D<T,4> origin;
			
			
			/// A SIMD 3D vector indicating the direction of the ray(s).
			SIMDVector3D<T,4> direction;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_RAY_3D_H
