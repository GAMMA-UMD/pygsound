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

#ifndef INCLUDE_OM_FFT_H
#define INCLUDE_OM_FFT_H


#include "omMathConfig.h"


#include "omFFTReal.h"
#include "omFFTComplex.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		1D Fourier Transform Methods
//############		
//##########################################################################################
//##########################################################################################




/// Convert N complex values to N complex values in-place using the forward transform.
void fft( Complex<Float32>* inOut, Size size );


/// Convert N complex values to N complex values using the reverse transform.
void ifft( Complex<Float32>* inOut, Size size );




/// Convert N real float values to N/2+1 complex values in place.
void fft( Float32* inOut, Size size );


/// Convert N real float values to N/2+1 complex values.
void fft( const Float32* input, Complex<Float32>* output, Size size );




/// Convert N/2+1 complex values to N real float values in place.
void ifft( Float32* inOut, Size size );


/// Convert N/2+1 complex values to N real float values.
void ifft( const Complex<Float32>* input, Float32* output, Size size );




/// Shift the zero-frequency components of the specified array to the center of the array.
/**
  * This can be useful when visualizing the output of an FFT.
  *
  * The method return whether or not the operation was successful. It may fail if the
  * data pointer is NULL or if the size is not divisible by 2.
  */
void fftShift( Complex<Float32>* data, Size size );




//##########################################################################################
//##########################################################################################
//############		
//############		2D Fourier Transform Methods
//############		
//##########################################################################################
//##########################################################################################




/// Compute the forward 2D Fourier transform in-place on the specified array of complex numbers.
/**
  * The input data should be stored in a row-major format, with each row representing a
  * contiguous block of complex numbers. The results of the transform are stored in the input data array.
  *
  * The method returns whether or not the FFT was successfully performed on the input data.
  * The method may fail if the data pointer is NULL or if the requested FFT width or height is not
  * a power of two.
  */
template < typename T >
Bool fft2D( Complex<T>* data, Size width, Size height );




/// Compute the inverse 2D Fourier transform in-place on the specified array of complex numbers.
/**
  * The input data should be stored in a row-major format, with each row representing a
  * contiguous block of complex numbers. The results of the transform are stored in the input data array.
  *
  * The method returns whether or not the FFT was successfully performed on the input data.
  * The method may fail if the data pointer is NULL or if the requested FFT width or height is not
  * a power of two.
  */
template < typename T >
Bool ifft2D( Complex<T>* data, Size width, Size height );




/// Shift the zero-frequency components of the specified 2D matrix to the center of the matrix.
/**
  * This can be useful when visualizing the output of an FFT.
  *
  * The method return whether or not the operation was successful. It may fail if the
  * data pointer is NULL or if the width or height are not divisible by 2.
  */
template < typename T >
Bool fftShift2D( Complex<T>* data, Size width, Size height );




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_FFT_H
