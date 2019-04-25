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

#ifndef INCLUDE_OM_SIMD_PLANE_3D_H
#define INCLUDE_OM_SIMD_PLANE_3D_H


#include "omMathConfig.h"


#include "omPlane3D.h"
#include "omSIMDVector3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A template prototype declaration for the SIMDPlane3D class.
/**
  * This class is used to store and operate on a set of N 3D planes
  * in a SIMD fashion. The planes are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each plane is specified by a normal and
  * origin offset.
  */
template < typename T, Size dimension >
class SIMDPlane3D;




//********************************************************************************
/// A specialization for the SIMDPlane3D class that has a SIMD width of 4.
/**
  * This class is used to store and operate on a set of 4 3D planes
  * in a SIMD fashion. The planes are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each plane is specified by a normal and
  * origin offset.
  */
template < typename T >
class OM_ALIGN(16) SIMDPlane3D<T,4>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a SIMD plane with N copies of the specified plane for a SIMD width of N.
			OM_FORCE_INLINE SIMDPlane3D( const Plane3D<T>& plane )
				:	normal( plane.normal ),
					offset( plane.offset )
			{
			}
			
			
			/// Create a SIMD plane with the 4 planes it contains equal to the specified planes.
			OM_FORCE_INLINE SIMDPlane3D( const Plane3D<T>& plane1, const Plane3D<T>& plane2,
											const Plane3D<T>& plane3, const Plane3D<T>& plane4 )
				:	normal( plane1.normal, plane2.normal, plane3.normal, plane4.normal ),
					offset( plane1.offset, plane2.offset, plane3.offset, plane4.offset )
			{
			}
			
			
		//********************************************************************************
		//******	Point Distance Methods
			
			
			/// Get the perpendicular distance from the specified point to the plane.
			OM_FORCE_INLINE SIMDScalar<T,4> getDistanceTo( const SIMDVector3D<T,4>& point ) const
			{
				return math::abs( math::dot( normal, point ) + offset );
			}
			
			
			/// Get the perpendicular distance from the specified point to the plane.
			OM_FORCE_INLINE SIMDScalar<T,4> getSignedDistanceTo( const SIMDVector3D<T,4>& point ) const
			{
				return math::dot( normal, point ) + offset;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A SIMD 3D vector indicating the normals of the planes.
			SIMDVector3D<T,4> normal;
			
			
			/// A SIMD scalar indicating the distance that the planes are offset from the origin.
			SIMDScalar<T,4> offset;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_PLANE_3D_H
