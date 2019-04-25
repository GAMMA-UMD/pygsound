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

#ifndef INCLUDE_OM_INTERPOLATION_TYPE_H
#define INCLUDE_OM_INTERPOLATION_TYPE_H


#include "omMathConfig.h"
#include "../data/omString.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that enumerates the different types of sampled function interpolation.
class Interpolation
{
	public:
		
		//********************************************************************************
		//******	Public Enum Declaration
			
			
			/// An enum type which represents the different interpolation types.
			enum Enum
			{
				/// An undefined or unknown interpolation type.
				UNDEFINED = 0,
				
				/// An interpolation type where no interpolation is performed.
				/**
				  * The nearest sample value is used to determine the final interpolated value.
				  */
				NONE = 1,
				
				/// An interpolation type where the nearest sample value is used to determine the final interpolated value.
				NEAREST = NONE,
				
				/// An interpolation type where basic linear interpolation is used between end points.
				LINEAR = 2,
				
				/// An interpolation type where cubic interpolation is used to interpolate between end points + control points.
				/**
				  * This type of interpolation requires an additional control point for each end point
				  * that determines the shape of the curve.
				  */
				BEZIER = 3,
				
				/// An interpolation type where cubic interpolation is used to interpolate between end points + tangents.
				/**
				  * This type of interpolation requires an additional tangent value for each end point
				  * that determines the shape of the curve.
				  */
				HERMITE = 4,
				
				/// An interpolation type which only guarantees to go through the start and end points, but not necessarily the middle ones.
				B_SPLINE = 5,
				
				/// An interpolation type which uses a sinc low-pass filter to interpolate control points.
				SINC = 6,
				
				/// An interpolation type which uses spherical linear interpolation for correct rotation interpolation.
				/**
				  * This animation type is only valid for quaternion attribute types (4-component vectors).
				  */
				SLERP = 7
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new interpolation type with the UNDEFINED enum value.
			OM_INLINE Interpolation()
				:	type( UNDEFINED )
			{
			}
			
			
			/// Create a new interpolation type with the specified interpolation type enum value.
			OM_INLINE Interpolation( Enum newType )
				:	type( newType )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this interpolation type to an enum value.
			OM_INLINE operator Enum () const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the interpolation type.
			data::String toString() const;
			
			
			/// Convert this interpolation type into a string representation.
			OM_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum representing the interpolation type.
			Enum type;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Interpolation Functions
//############		
//##########################################################################################
//##########################################################################################




/// Compute and return the linearly-interpolated value at y(x), where x is in the range [x0,x1], y(x0) = y0, y(x1) = y1.
template < typename T, typename U >
OM_FORCE_INLINE T lerp( T x, T x0, T x1, U y0, U y1 )
{
	return y0 + (y1 - y0)*((x - x0)/(x1 - x0));
}




/// Compute and return the area of a trapezoidal integration slice.
template < typename T, typename U >
OM_FORCE_INLINE T trapezoid( T x0, T x1, U y0, U y1 )
{
	return T(0.5)*(y0 + y1)*(x0 - x1);
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_INTERPOLATION_TYPE_H
