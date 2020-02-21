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

#ifndef INCLUDE_OM_SIMD_AABB_3D_H
#define INCLUDE_OM_SIMD_AABB_3D_H


#include "omMathConfig.h"


#include "omAABB3D.h"
#include "omSIMDVector3D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




template < typename T, Size width >
class SIMDAABB3D;




//********************************************************************************
/// A class that represents a set of 3D vectors stored in a SIMD-compatible format.
/**
  * This class is used to store and operate on a set of axis-aligned bounding boxes
  * in a SIMD fashion. The bounding boxes are stored in a structure-of-arrays format
  * that accelerates SIMD operations. Each bounding box is specified by a minimum
  * and maximum vertex coordinate.
  */
template < typename T >
class OM_ALIGN(16) SIMDAABB3D<T,4>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a SIMD axis-aligned bounding box that has all components initialized to zero.
			OM_FORCE_INLINE SIMDAABB3D()
			{
			}
			
			
			/// Create a SIMD axis-aligned bounding box from a single bounding box.
			OM_FORCE_INLINE SIMDAABB3D( const AABB3D<T>& aabb )
				:	min( aabb.min ),
					max( aabb.max )
			{
			}
			
			
			/// Create a SIMD axis-aligned bounding box from the four specified bounding boxes.
			OM_FORCE_INLINE SIMDAABB3D( const AABB3D<T>& aabb1, const AABB3D<T>& aabb2,
										const AABB3D<T>& aabb3, const AABB3D<T>& aabb4 )
				:	min( aabb1.min, aabb2.min, aabb3.min, aabb4.min ),
					max( aabb1.max, aabb2.max, aabb3.max, aabb4.max )
			{
			}
			
			
		//********************************************************************************
		//******	Accessor Methods
			
			
			/// Get either the minimal or maximal vertex of this AABB.
			/**
			  * If the index parameter is 0, the minimal vertex is returned, if the
			  * index parameter is 1, the maximal vertex is returned. Otherwise the
			  * result is undefined.
			  */
			OM_FORCE_INLINE const SIMDVector3D<T,4>& getMinMax( Index i ) const
			{
				return (&min)[i];
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
			
			
			/// The minimum coordinate vectors for this SIMD axis-aligned bounding box.
			SIMDVector3D<T,4> min;
			
			
			/// The maximum coordinate vectors for this SIMD axis-aligned bounding box.
			SIMDVector3D<T,4> max;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_AABB_3D_H
