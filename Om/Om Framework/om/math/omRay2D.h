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

#ifndef INCLUDE_OM_RAY_2D_H
#define INCLUDE_OM_RAY_2D_H


#include "omMathConfig.h"


#include "omVector2D.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a ray in 2D space.
/**
  * This class contains two data members: origin and direction. Origin represents
  * the starting position of the ray and direction represents the positively
  * parameterized direction along the ray.
  */
template < typename T >
class Ray2D
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a ray starting at the origin pointing in the positive Y direction.
			OM_FORCE_INLINE Ray2D()
				:	origin( 0, 0 ),
					direction( 0, 1 )
			{
			}
			
			
			/// Create a ray with the specified origin and direction.
			OM_FORCE_INLINE Ray2D( const VectorND<T,2>& newOrigin, const VectorND<T,2>& newDirection )
				:	origin( newOrigin ),
					direction( newDirection )
			{
			}
			
			
		//********************************************************************************
		//******	Ray Methods
			
			
			/// Get the position along the ray at the specified parameter value.
			/**
			  * This position is calculated using the equation:
			  * position = origin + parameter*direction.
			  */
			OM_FORCE_INLINE VectorND<T,2> getPositionAt( T parameter ) const
			{
				return origin + parameter*direction;
			}
			
			
			/// Return a new ray with a unit-length direction vector.
			OM_FORCE_INLINE VectorND<T,2> normalize() const
			{
				return VectorND<T,2>( origin, direction.normalize() );
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The origin of the ray in 2D space;
			VectorND<T,2> origin;
			
			
			/// The direction of the ray in 2D space.
			VectorND<T,2> direction;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RAY_3D_H
