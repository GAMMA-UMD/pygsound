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

#ifndef INCLUDE_OM_RESAMPLE_H
#define INCLUDE_OM_RESAMPLE_H


#include "omMathConfig.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		1D Convolution Methods
//############		
//##########################################################################################
//##########################################################################################




/// Resample the input data by the given factor and put the result in the output.
/**
  * The output array must have enough space to hold ceiling(inputSize*factor) elements.
  *
  * This is a slow implementation that should not be used for performance-intensive
  * applications or where a stream of data must be resampled.
  */
template < typename T, typename U >
Bool resample( const T* input, Size inputSize, T* output, U factor, Size filterWidth, U filterFactor = U(0.499) )
{
	if ( input == NULL || inputSize == 0 || output == NULL )
		return false;
	
	const Size outputSize = (Size)math::ceiling(U(inputSize)*factor);
	const T* const outputEnd = output + outputSize;
	const U xStep = U(1)/factor;
	const U fMax = filterFactor;
	const U gain = U(2)*fMax*xStep;
	const U filterHalfWidth = U(0.5)*U(filterWidth);
	const U inverseFilterWidth = U(1)/U(filterWidth);
	const U filterStep = U(1);
	const U inputMax = U(inputSize - 1);
	U x = U(0.0);
	
	while ( output != outputEnd )
	{
		U xi = x - filterHalfWidth;
		*output = T(0);
		
		for ( Index h = 0; h < filterWidth; h++ )
		{
			U xiFloor = math::floor( xi );
			U j = xiFloor - x;
			
			U window = U(0.5) - U(0.5)*math::cos( U(2)*math::pi<U>()*(U(0.5) + j*inverseFilterWidth) );
			U a = U(2)*math::pi<U>()*j*fMax;
			U s = a == U(0) ? U(1) : math::sin(a) / a;
			
			Index i = Index(math::clamp(xiFloor, U(0), inputMax));
			
			*output += gain*window*s*input[i];
			xi += filterStep;
		}
		
		output++;
		x += xStep;
	}
	
	return true;
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_RESAMPLE_H
