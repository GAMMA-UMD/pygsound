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

#ifndef INCLUDE_OM_SIMD_SCALAR_H
#define INCLUDE_OM_SIMD_SCALAR_H


#include "omMathConfig.h"


#include "omScalarMath.h"
#include "omSIMDConfig.h"
#include "omSIMDTypes.h"
#include "omSIMDFlags.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// The prototype for the SIMDScalar class.
/**
  * Since only certain types and widths are supported on SIMD hardware, this class,
  * forward-declared here is specialized for different types and widths that are
  * hardware compliant. Using another type or width will produce an error because
  * a class with those invalid arguments will not be defined.
  */
template < typename T, Size width >
class SIMDScalar;




//##########################################################################################
//##########################################################################################
//############		
//############		Associative SIMD Scalar Operators
//############		
//##########################################################################################
//##########################################################################################




/// Add a scalar value to each component of this scalar and return the resulting scalar.
template < typename T, Size width >
OM_FORCE_INLINE SIMDScalar<T,width> operator + ( const T value, const SIMDScalar<T,width>& scalar )
{
	return SIMDScalar<T,width>(value) + scalar;
}




/// Subtract a scalar value from each component of this scalar and return the resulting scalar.
template < typename T, Size width >
OM_FORCE_INLINE SIMDScalar<T,width> operator - ( const Float32 value, const SIMDScalar<T,width>& scalar )
{
	return SIMDScalar<T,width>(value) - scalar;
}




/// Multiply a scalar value by each component of this scalar and return the resulting scalar.
template < typename T, Size width >
OM_FORCE_INLINE SIMDScalar<T,width> operator * ( const Float32 value, const SIMDScalar<T,width>& scalar )
{
	return SIMDScalar<T,width>(value) * scalar;
}




/// Divide each component of this scalar by a scalar value and return the resulting scalar.
template < typename T, Size width >
OM_FORCE_INLINE SIMDScalar<T,width> operator / ( const Float32 value, const SIMDScalar<T,width>& scalar )
{
	return SIMDScalar<T,width>(value) / scalar;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_SCALAR_H
