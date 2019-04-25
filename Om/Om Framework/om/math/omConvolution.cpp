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

#include "omConvolution.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




template < typename T >
OM_FORCE_INLINE static Bool convolveFilter( const T* input, Size inputSize, const T* filter, Size filterSize, T* output )
{
	if ( input == NULL || filter == NULL || output == NULL || filterSize == Size(0) )
		return false;
	
	Size outputSize = inputSize + filterSize - 1;
	
	for ( Index n = 0; n < outputSize; n++)
	{
		output[n] = 0;
		
		Index kmin = (n >= filterSize - 1) ? n - (filterSize - 1) : 0;
		Index kmax = (n < inputSize - 1) ? n : inputSize - 1;
		
		for ( Index k = kmin; k <= kmax; k++ )
			output[n] += input[k] * filter[n - k];
	}
	
	return true;
}




template <>
Bool convolve( const Float32* input, Size inputSize,
				const Float32* filter, Size filterSize, Float* output )
{
	return convolveFilter( input, inputSize, filter, filterSize, output );
}




template <>
Bool convolve( const Float64* input, Size inputSize,
				const Float64* filter, Size filterSize, Float64* output )
{
	return convolveFilter( input, inputSize, filter, filterSize, output );
}




template <>
Bool convolve( const Complex<Float32>* input, Size inputSize,
				const Complex<Float32>* filter, Size filterSize, Complex<Float32>* output )
{
	return convolveFilter( input, inputSize, filter, filterSize, output );
}




template <>
Bool convolve( const Complex<Float64>* input, Size inputSize,
				const Complex<Float64>* filter, Size filterSize, Complex<Float64>* output )
{
	return convolveFilter( input, inputSize, filter, filterSize, output );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
