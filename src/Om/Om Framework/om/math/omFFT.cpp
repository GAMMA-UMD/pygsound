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

#include "omFFT.h"


#include "../util/omCopy.h"
#include "../threads/omMutex.h"
#include "../lang/omShared.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


/// A static list of global FFT objects.
static util::ArrayList< lang::Shared< FFTReal<Float32> > > fftsRealFloat32;
static util::ArrayList< lang::Shared< FFTComplex<Float32> > > fftsComplexFloat32;


/// A mutex that protects the global FFT access from multiple threads.
static lang::Shared<threads::Mutex> fftMutex = lang::Shared<threads::Mutex>::construct();


//##########################################################################################
//##########################################################################################
//############		
//############		Global FFT Methods
//############		
//##########################################################################################
//##########################################################################################




#define GLOBAL_FFT_ACCESSOR( ComplexReal, Type, array )\
OM_FORCE_INLINE FFT##ComplexReal <Type>* getGlobalFFT##ComplexReal##Type( Size fftSize )\
{\
	Index fftIndex = math::lastSetBit( fftSize );\
	\
	fftMutex->lock();\
	\
	/* Create a new FFT object. */\
	while ( fftIndex >= array.getSize() )\
	{\
		/* Create a new FFT object. */\
		if ( fftIndex == array.getSize() )\
			array.add( lang::Shared< FFT##ComplexReal<Type> >::construct( fftSize ) );\
		else\
			array.addNew();\
	}\
	\
	FFT##ComplexReal<Type>* fft = array[fftIndex];\
	\
	/* Create a new FFT object if it was not yet created.*/\
	if ( fft == NULL )\
		fft = array[fftIndex] = lang::Shared< FFT##ComplexReal<Type> >::construct( fftSize );\
	\
	fftMutex->unlock();\
	\
	return fft;\
}


GLOBAL_FFT_ACCESSOR( Real, Float32, fftsRealFloat32 );
GLOBAL_FFT_ACCESSOR( Complex, Float32, fftsComplexFloat32 );


//##########################################################################################
//##########################################################################################
//############		
//############		1D Global FFT Methods
//############		
//##########################################################################################
//##########################################################################################




void fft( Complex<Float32>* inOut, Size size )
{
	if ( inOut == NULL || size == 0 )
		return;
	
	FFTComplex<Float32>* fft = getGlobalFFTComplexFloat32( size );
	fft->fft( inOut );
}




void ifft( Complex<Float32>* inOut, Size size )
{
	if ( inOut == NULL || size == 0 )
		return;
	
	FFTComplex<Float32>* fft = getGlobalFFTComplexFloat32( size );
	fft->ifft( inOut );
}




void fft( Float32* inOut, Size size )
{
	if ( inOut == NULL || size == 0 )
		return;
	
	FFTReal<Float32>* fft = getGlobalFFTRealFloat32( size );
	fft->fft( inOut );
}




void fft( const Float32* input, Complex<Float32>* output, Size size )
{
	if ( input == NULL || output == NULL || size == 0 )
		return;
	
	FFTReal<Float32>* fft = getGlobalFFTRealFloat32( size );
	fft->fft( input, output, NULL );
}




void ifft( Float32* inOut, Size size )
{
	if ( inOut == NULL || size == 0 )
		return;
	
	FFTReal<Float32>* fft = getGlobalFFTRealFloat32( size );
	fft->ifft( inOut );
}




void ifft( const Complex<Float32>* input, Float32* output, Size size )
{
	if ( input == NULL || output == NULL || size == 0 )
		return;
	
	FFTReal<Float32>* fft = getGlobalFFTRealFloat32( size );
	fft->ifft( input, output, NULL );
}




void fftShift( Complex<Float32>* data, Size size )
{
	// Check the validity of the arguments
	if ( data == NULL || size & 0x1 )
		return;
	
	// Compute the half size.
	Size halfSize = size / 2;
	const Complex<Float32>* const dataHalfway = data + halfSize;
	
	// Swap the first and second halves of the array.
	for ( Complex<Float32>* first = data, *second = data + halfSize; first < dataHalfway; first++, second++ )
	{
		Complex<Float32> temp = *first;
		*first = *second;
		*second = temp;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		2D Global FFT Methods
//############		
//##########################################################################################
//##########################################################################################































































template < typename T >
struct CommonWeight
{
	T w1r;
	T w1i;
	T w2r;
	T w2i;
	T w3r;
	T w3i;
};




static inline int ilog2( unsigned int n )
{
	int c;
	
	for ( c = 0; n >>= 1; ++c );
	
	return c;
}




static UInt32 reverseWord( UInt32 k )
{
	static const UInt8 b[256] = {
		0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240,
		8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248,
		4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244,
		12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252,
		2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
		10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250,
		6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246,
		14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254,
		1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241,
		9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
		5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245,
		13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253,
		3, 131, 67, 195, 35, 163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243,
		11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251,
		7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
		15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255 };
	
	UInt8 b0 = b[k & 0xFF];
	UInt8 b1 = b[(k >> 8) & 0xFF];
	UInt8 b2 = b[(k >> 16) & 0xFF];
	UInt8 b3 = b[(k >> 24) & 0xFF];
	
	return UInt32(b0) << 24 | UInt32(b1) << 16 | UInt32(b2) << 8 | UInt32(b3);
}




template < typename T >
static T rotateWord( unsigned int k )
{
	return T(1.0) / T(4294967296.0) * reverseWord(k);
}





template < typename T >
static void generateCommonWeights( CommonWeight<T>*& weights, Size oldLength, Size newLength )
{
	Size weightFraction = 4;
	
	// Compute the required weight array size.
	Size oldNumWeights = oldLength / weightFraction;
	Size numWeights = newLength / weightFraction;
	
	CommonWeight<T>* newWeights = util::allocate< CommonWeight<T> >( numWeights );
	
	// Copy the old weights.
	if ( weights )
	{
		om::util::copy( newWeights, weights, oldNumWeights );
		util::deallocate( weights );
	}
	
	weights = newWeights;
	
	const T twoPi = 2*math::pi<T>();
	
	for ( Index k0 = 0; k0 < numWeights; ++k0 )
	{
		const T x = twoPi * rotateWord<T>(4*k0);
		
		weights[k0].w1r = math::cos(x);
		weights[k0].w1i = math::tan(x);
		weights[k0].w2r = math::cos( T(2)*x );
		weights[k0].w2i = math::tan( T(2)*x );
		weights[k0].w3r = T(2)*weights[k0].w2r - T(1);
		weights[k0].w3i = math::tan( T(3)*x );
	}
	
}





/*
	FFT8_0Weights implements FFT_Butterflies with m=3 and k0=0. 
*/
template < typename T, Bool inverse >
static void FFT8_0Weights( Complex<T>* data,
							int c0 // Coefficient for k0.
							)
{
	// Prepare a constant, sqrt(2)/2.
	const T sqrt2d2 = math::sqrt(T(2)) / T(2);
	
	// Coefficient for k1 is coefficient for k0 divided by 1<<m.
	const int c1 = c0 >> 3;
	
	T a0r, a0i, a1r, a1i, a2r, a2i, a3r, a3i,
		a4r, a4i, a5r, a5i, a6r, a6i, a7r, a7i,
		b0r, b0i, b1r, b1i, b2r, b2i, b3r, b3i,
		b4r, b4i, b5r, b5i, b6r, b6i, b7r, b7i,
		c0r, c0i, c1r, c1i, c2r, c2i, c3r, c3i,
		c4r, c4i, c5r, c5i, c6r, c6i, c7r, c7i,
		d0r, d0i, d1r, d1i, d2r, d2i, d3r, d3i,
		d4r, d4i, d5r, d5i, d6r, d6i, d7r, d7i,
		t5r, t5i, t7r, t7i;
	
	for ( int k2 = 0; k2 < c1; ++k2 )
	{
		if ( inverse )
		{
			a0r = data[c1*0 + k2].i;
			a0i = data[c1*0 + k2].r;
			a1r = data[c1*1 + k2].i;
			a1i = data[c1*1 + k2].r;
			a2r = data[c1*2 + k2].i;
			a2i = data[c1*2 + k2].r;
			a3r = data[c1*3 + k2].i;
			a3i = data[c1*3 + k2].r;
			a4r = data[c1*4 + k2].i;
			a4i = data[c1*4 + k2].r;
			a5r = data[c1*5 + k2].i;
			a5i = data[c1*5 + k2].r;
			a6r = data[c1*6 + k2].i;
			a6i = data[c1*6 + k2].r;
			a7r = data[c1*7 + k2].i;
			a7i = data[c1*7 + k2].r;
		}
		else
		{
			a0r = data[c1*0 + k2].r;
			a0i = data[c1*0 + k2].i;
			a1r = data[c1*1 + k2].r;
			a1i = data[c1*1 + k2].i;
			a2r = data[c1*2 + k2].r;
			a2i = data[c1*2 + k2].i;
			a3r = data[c1*3 + k2].r;
			a3i = data[c1*3 + k2].i;
			a4r = data[c1*4 + k2].r;
			a4i = data[c1*4 + k2].i;
			a5r = data[c1*5 + k2].r;
			a5i = data[c1*5 + k2].i;
			a6r = data[c1*6 + k2].r;
			a6i = data[c1*6 + k2].i;
			a7r = data[c1*7 + k2].r;
			a7i = data[c1*7 + k2].i;
		}
		
		// w == 1
		b0r = a0r + a4r;
		b0i = a0i + a4i;
		b1r = a1r + a5r;
		b1i = a1i + a5i;
		b2r = a2r + a6r;
		b2i = a2i + a6i;
		b3r = a3r + a7r;
		b3i = a3i + a7i;
		b4r = a0r - a4r;
		b4i = a0i - a4i;
		b5r = a1r - a5r;
		b5i = a1i - a5i;
		b6r = a2r - a6r;
		b6i = a2i - a6i;
		b7r = a3r - a7r;
		b7i = a3i - a7i;
		
		// w == 1
		c0r = b0r + b2r;
		c0i = b0i + b2i;
		c1r = b1r + b3r;
		c1i = b1i + b3i;
		c2r = b0r - b2r;
		c2i = b0i - b2i;
		c3r = b1r - b3r;
		c3i = b1i - b3i;
		
		// w == i
		c4r = b4r - b6i;
		c4i = b4i + b6r;
		c5r = b5r - b7i;
		c5i = b5i + b7r;
		c6r = b4r + b6i;
		c6i = b4i - b6r;
		c7r = b5r + b7i;
		c7i = b5i - b7r;
		t5r = c5r - c5i;
		t5i = c5r + c5i;
		t7r = c7r + c7i;
		t7i = c7r - c7i;
		
		// w == 1
		d0r = c0r + c1r;
		d0i = c0i + c1i;
		d1r = c0r - c1r;
		d1i = c0i - c1i;
		
		// w == i
		d2r = c2r - c3i;
		d2i = c2i + c3r;
		d3r = c2r + c3i;
		d3i = c2i - c3r;
		
		// w = sqrt(2)/2 * (+1+i).
		d4r = + t5r * sqrt2d2 + c4r;
		d4i = + t5i * sqrt2d2 + c4i;
		d5r = - t5r * sqrt2d2 + c4r;
		d5i = - t5i * sqrt2d2 + c4i;
		
		// w = sqrt(2)/2 * (-1+i).
		d6r = - t7r * sqrt2d2 + c6r;
		d6i = + t7i * sqrt2d2 + c6i;
		d7r = + t7r * sqrt2d2 + c6r;
		d7i = - t7i * sqrt2d2 + c6i;
		
		data[c1*0 + k2].r = d0r;
		data[c1*0 + k2].i = d0i;
		data[c1*1 + k2].r = d1r;
		data[c1*1 + k2].i = d1i;
		data[c1*2 + k2].r = d2r;
		data[c1*2 + k2].i = d2i;
		data[c1*3 + k2].r = d3r;
		data[c1*3 + k2].i = d3i;
		data[c1*4 + k2].r = d4r;
		data[c1*4 + k2].i = d4i;
		data[c1*5 + k2].r = d5r;
		data[c1*5 + k2].i = d5i;
		data[c1*6 + k2].r = d6r;
		data[c1*6 + k2].i = d6i;
		data[c1*7 + k2].r = d7r;
		data[c1*7 + k2].i = d7i;
	}
}




/*
	FFT4_0Weights implements FFT_Butterflies with m=2 and k0=0.
	The calculations for a weightless radix-4 butterfly are
	straightforward and can be derived from FFT4_1WeightPerCall
	by replacing w1r, w2r, and w3r with 1 and w1i, w2i, and w3i
	with 0 and simplifying the resulting code:
*/
template < typename T, Bool inverse >
static void FFT4_0Weights( Complex<T>* data,
							int c0 // Coefficient for k0.
							)
{
	// Coefficient for k1 is coefficient for k0 divided by 1<<m.
	const int c1 = c0 >> 2;
	
	T a0r, a0i, a1r, a1i, a2r, a2i, a3r, a3i,
		c0r, c0i, c1r, c1i, c2r, c2i, c3r, c3i,
		d0r, d0i, d1r, d1i, d2r, d2i, d3r, d3i;
	
	for ( int k2 = 0; k2 < c1; ++k2 )
	{
		if ( inverse )
		{
			a0r = data[c1*0 + k2].i;
			a0i = data[c1*0 + k2].r;
			a1r = data[c1*1 + k2].i;
			a1i = data[c1*1 + k2].r;
			a2r = data[c1*2 + k2].i;
			a2i = data[c1*2 + k2].r;
			a3r = data[c1*3 + k2].i;
			a3i = data[c1*3 + k2].r;
		}
		else
		{
			a0r = data[c1*0 + k2].r;
			a0i = data[c1*0 + k2].i;
			a1r = data[c1*1 + k2].r;
			a1i = data[c1*1 + k2].i;
			a2r = data[c1*2 + k2].r;
			a2i = data[c1*2 + k2].i;
			a3r = data[c1*3 + k2].r;
			a3i = data[c1*3 + k2].i;
		}
		
		c0r = + a2r + a0r;
		c0i = + a2i + a0i;
		c2r = - a2r + a0r;
		c2i = - a2i + a0i;
		c1r = + a3r + a1r;
		c1i = + a3i + a1i;
		c3r = - a3r + a1r;
		c3i = - a3i + a1i;
		d0r = + c1r + c0r;
		d0i = + c1i + c0i;
		d1r = - c1r + c0r;
		d1i = - c1i + c0i;
		d2r = - c3i + c2r;
		d2i = + c3r + c2i;
		d3r = + c3i + c2r;
		d3i = - c3r + c2i;
		data[c1*0 + k2].r = d0r;
		data[c1*0 + k2].i = d0i;
		data[c1*1 + k2].r = d1r;
		data[c1*1 + k2].i = d1i;
		data[c1*2 + k2].r = d2r;
		data[c1*2 + k2].i = d2i;
		data[c1*3 + k2].r = d3r;
		data[c1*3 + k2].i = d3i;
	}
}






template < typename T >
static void FFT4_1WeightPerIteration( Complex<T>* data, // Address of output vector.
									int u0, // Upper bound on k0.
									const CommonWeight<T>* weights // Array of weight values.
									)
{
	T a0r, a0i, a1r, a1i, a2r, a2i, a3r, a3i,
					b1r, b1i, b2r, b2i, b3r, b3i,
		c0r, c0i, c1r, c1i, c2r, c2i, c3r, c3i,
		d0r, d0i, d1r, d1i, d2r, d2i, d3r, d3i;
	
	for ( int k0 = 0; k0 < u0; ++k0 )
	{
		// Load values for current weight.
		CommonWeight<T> weight = weights[k0];
		
		for ( int k2 = 0; k2 < 4; ++k2 )
		{
			a0r = data[16*k0 + 4*0 + k2].r;
			a0i = data[16*k0 + 4*0 + k2].i;
			a1r = data[16*k0 + 4*1 + k2].r;
			a1i = data[16*k0 + 4*1 + k2].i;
			a2r = data[16*k0 + 4*2 + k2].r;
			a2i = data[16*k0 + 4*2 + k2].i;
			a3r = data[16*k0 + 4*3 + k2].r;
			a3i = data[16*k0 + 4*3 + k2].i;
			b1r = - a1i * weight.w1i + a1r;
			b1i = + a1r * weight.w1i + a1i;
			b2r = - a2i * weight.w2i + a2r;
			b2i = + a2r * weight.w2i + a2i;
			b3r = - a3i * weight.w3i + a3r;
			b3i = + a3r * weight.w3i + a3i;
			c0r = + b2r * weight.w2r + a0r;
			c0i = + b2i * weight.w2r + a0i;
			c2r = - b2r * weight.w2r + a0r;
			c2i = - b2i * weight.w2r + a0i;
			c1r = + b3r * weight.w3r + b1r;
			c1i = + b3i * weight.w3r + b1i;
			c3r = - b3r * weight.w3r + b1r;
			c3i = - b3i * weight.w3r + b1i;
			d0r = + c1r * weight.w1r + c0r;
			d0i = + c1i * weight.w1r + c0i;
			d1r = - c1r * weight.w1r + c0r;
			d1i = - c1i * weight.w1r + c0i;
			d2r = - c3i * weight.w1r + c2r;
			d2i = + c3r * weight.w1r + c2i;
			d3r = + c3i * weight.w1r + c2r;
			d3i = - c3r * weight.w1r + c2i;
			data[16*k0 + 4*0 + k2].r = d0r;
			data[16*k0 + 4*0 + k2].i = d0i;
			data[16*k0 + 4*1 + k2].r = d1r;
			data[16*k0 + 4*1 + k2].i = d1i;
			data[16*k0 + 4*2 + k2].r = d2r;
			data[16*k0 + 4*2 + k2].i = d2i;
			data[16*k0 + 4*3 + k2].r = d3r;
			data[16*k0 + 4*3 + k2].i = d3i;
		}
	}
}





/*
	FFT4_1WeightPerCall implements FFT_Butterflies with m=2 and vIn=VOut
	and with weight values provided so that it need not calculate them.
*/
template < typename T >
static void FFT4_1WeightPerCall( Complex<T>* data, // Address of output vector.
								int k0, // k0 from equation.
								int c0, // Coefficient for k0.
								CommonWeight<T> weight // Values for weight calculations.
								)
{
	// Coefficient for k1 is coefficient for k0 divided by 1<<m.
	const int c1 = c0 >> 2;
	int k2;
	T a0r, a0i, a1r, a1i, a2r, a2i, a3r, a3i,
					b1r, b1i, b2r, b2i, b3r, b3i,
		c0r, c0i, c1r, c1i, c2r, c2i, c3r, c3i,
		d0r, d0i, d1r, d1i, d2r, d2i, d3r, d3i;
	
	for ( k2 = 0; k2 < c1; ++k2 )
	{
		a0r = data[c0*k0 + c1*0 + k2].r;
		a0i = data[c0*k0 + c1*0 + k2].i;
		a1r = data[c0*k0 + c1*1 + k2].r;
		a1i = data[c0*k0 + c1*1 + k2].i;
		a2r = data[c0*k0 + c1*2 + k2].r;
		a2i = data[c0*k0 + c1*2 + k2].i;
		a3r = data[c0*k0 + c1*3 + k2].r;
		a3i = data[c0*k0 + c1*3 + k2].i;
		b1r = - a1i * weight.w1i + a1r;
		b1i = + a1r * weight.w1i + a1i;
		b2r = - a2i * weight.w2i + a2r;
		b2i = + a2r * weight.w2i + a2i;
		b3r = - a3i * weight.w3i + a3r;
		b3i = + a3r * weight.w3i + a3i;
		c0r = + b2r * weight.w2r + a0r;
		c0i = + b2i * weight.w2r + a0i;
		c2r = - b2r * weight.w2r + a0r;
		c2i = - b2i * weight.w2r + a0i;
		c1r = + b3r * weight.w3r + b1r;
		c1i = + b3i * weight.w3r + b1i;
		c3r = - b3r * weight.w3r + b1r;
		c3i = - b3i * weight.w3r + b1i;
		d0r = + c1r * weight.w1r + c0r;
		d0i = + c1i * weight.w1r + c0i;
		d1r = - c1r * weight.w1r + c0r;
		d1i = - c1i * weight.w1r + c0i;
		d2r = - c3i * weight.w1r + c2r;
		d2i = + c3r * weight.w1r + c2i;
		d3r = + c3i * weight.w1r + c2r;
		d3i = - c3r * weight.w1r + c2i;
		data[c0*k0 + c1*0 + k2].r = d0r;
		data[c0*k0 + c1*0 + k2].i = d0i;
		data[c0*k0 + c1*1 + k2].r = d1r;
		data[c0*k0 + c1*1 + k2].i = d1i;
		data[c0*k0 + c1*2 + k2].r = d2r;
		data[c0*k0 + c1*2 + k2].i = d2i;
		data[c0*k0 + c1*3 + k2].r = d3r;
		data[c0*k0 + c1*3 + k2].i = d3i;
	}
}




template < typename T, Bool inverse >
static void FFT4_Final( Complex<T>* data, // Address of output vector.
						int u0, // Upper bound on k0.
						const CommonWeight<T>* weights // Array of weight values.
						)
{
	T a0r, a0i, a1r, a1i, a2r, a2i, a3r, a3i,
					b1r, b1i, b2r, b2i, b3r, b3i,
		c0r, c0i, c1r, c1i, c2r, c2i, c3r, c3i,
		d0r, d0i, d1r, d1i, d2r, d2i, d3r, d3i;
	
	for ( int k0 = 0; k0 < u0; ++k0 )
	{
		// Load values for current weight.
		CommonWeight<T> weight = weights[k0];
		
		a0r = data[4*k0 + 0].r;
		a0i = data[4*k0 + 0].i;
		a1r = data[4*k0 + 1].r;
		a1i = data[4*k0 + 1].i;
		a2r = data[4*k0 + 2].r;
		a2i = data[4*k0 + 2].i;
		a3r = data[4*k0 + 3].r;
		a3i = data[4*k0 + 3].i;
		
		b1r = - a1i * weight.w1i + a1r;
		b1i = + a1r * weight.w1i + a1i;
		b2r = - a2i * weight.w2i + a2r;
		b2i = + a2r * weight.w2i + a2i;
		b3r = - a3i * weight.w3i + a3r;
		b3i = + a3r * weight.w3i + a3i;
		c0r = + b2r * weight.w2r + a0r;
		c0i = + b2i * weight.w2r + a0i;
		c2r = - b2r * weight.w2r + a0r;
		c2i = - b2i * weight.w2r + a0i;
		c1r = + b3r * weight.w3r + b1r;
		c1i = + b3i * weight.w3r + b1i;
		c3r = - b3r * weight.w3r + b1r;
		c3i = - b3i * weight.w3r + b1i;
		d0r = + c1r * weight.w1r + c0r;
		d0i = + c1i * weight.w1r + c0i;
		d1r = - c1r * weight.w1r + c0r;
		d1i = - c1i * weight.w1r + c0i;
		d2r = - c3i * weight.w1r + c2r;
		d2i = + c3r * weight.w1r + c2i;
		d3r = + c3i * weight.w1r + c2r;
		d3i = - c3r * weight.w1r + c2i;
		
		if ( inverse )
		{
			data[4*k0 + 0].i = d0r;
			data[4*k0 + 0].r = d0i;
			data[4*k0 + 1].i = d1r;
			data[4*k0 + 1].r = d1i;
			data[4*k0 + 2].i = d2r;
			data[4*k0 + 2].r = d2i;
			data[4*k0 + 3].i = d3r;
			data[4*k0 + 3].r = d3i;
		}
		else
		{
			data[4*k0 + 0].r = d0r;
			data[4*k0 + 0].i = d0i;
			data[4*k0 + 1].r = d1r;
			data[4*k0 + 1].i = d1i;
			data[4*k0 + 2].r = d2r;
			data[4*k0 + 2].i = d2i;
			data[4*k0 + 3].r = d3r;
			data[4*k0 + 3].i = d3i;
		}
	}
}




template < typename T >
OM_FORCE_INLINE void BitReversalPermute( Complex<T>* data, int n )
{
	unsigned int i, j, shift;
	
	// Figure out how much to shift to get high log2(n) bits to the right.
	shift = 32 - ilog2(n);
	
	for ( i = 0; (int)i < n; ++i )
	{
		// Take i, reverse the whole word, and get just the high bits.
		j = reverseWord(i) >> shift;
		
		// We don't want to swap each pair twice, so swap only when i < j.
		if ( i < j )
		{
			// Swap elements i and j.
			Complex<T> temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
	}
}




static CommonWeight<float>* floatWeights = NULL;
static Size floatWeightsLength = 0;
static CommonWeight<double>* doubleWeights = NULL;
static Size doubleWeightsLength = 0;
static threads::Mutex* weightMutex = util::construct<threads::Mutex>();



template < typename T >
OM_INLINE static const CommonWeight<T>* getWeights( Size length );


template <>
OM_INLINE const CommonWeight<double>* getWeights<double>( Size length )
{
	if ( doubleWeightsLength < length )
	{
		weightMutex->lock();
		
		if ( doubleWeightsLength < length )
		{
			generateCommonWeights( doubleWeights, doubleWeightsLength, length );
			doubleWeightsLength = length;
		}
		
		weightMutex->unlock();
	}
	
	return doubleWeights;
}


template <>
OM_INLINE const CommonWeight<float>* getWeights<float>( Size length )
{
	if ( floatWeightsLength < length )
	{
		weightMutex->lock();
		
		if ( floatWeightsLength < length )
		{
			generateCommonWeights( floatWeights, floatWeightsLength, length );
			floatWeightsLength = length;
		}

		weightMutex->unlock();
	}
	
	return floatWeights;
}




template < typename T, Bool inverse >
OM_FORCE_INLINE static Bool fft( Complex<T>* data, Size length )
{
	if ( data == NULL || !math::isPowerOfTwo(length) )
		return false;
	
	if ( length < 4 )
		return false;
	
	int N = ilog2((unsigned int)length);
	
	//*******************************************************************
	// Generate the weights for the FFT.
	
	const CommonWeight<T>* weights = getWeights<T>( length );
	
	//*******************************************************************
	// Perform the FFT.
	
	if ( N & 1 )
		FFT8_0Weights<T,inverse>( data, (int)length );
	else
		FFT4_0Weights<T,inverse>( data, (int)length );
	
	int nLower = N & 1 ? 3 : 2;
	int n;
	
	for ( n = nLower; n < N - 4; n += 2 )
		FFT4_0Weights<T,false>( data, 1 << (N - n) );
	
	for ( int k0 = 1; nLower < (N - 4); nLower += 2 )
	for ( ; k0 < (1 << nLower); ++k0 )
	for ( n = nLower; n < (N - 4); n += 2 )
		FFT4_1WeightPerCall( data, k0, 1 << (N - n), weights[k0] );
	
	if ( n < N - 2 )
		FFT4_1WeightPerIteration( data, 1 << (N - 4), weights );
	
	if ( n < N - 1 )
		FFT4_Final<T,inverse>( data, 1 << (N - 2), weights );
	else if ( inverse )
	{
		// Reverse the real and imaginary components if we are performing the inverse DFT
		// and can't do the work in the final FFT method.
		for ( Index i = 0; i < length; i++ )
		{
			T temp = data[i].r;
			data[i].r = data[i].i;
			data[i].i = temp;
		}
	}
	
	BitReversalPermute( data, (int)length );
	
	return true;
}




template < typename T >
OM_FORCE_INLINE static Bool fftShiftInternal( Complex<T>* data, Size size )
{
	// Check the validity of the arguments
	if ( data == NULL || size & 0x1 )
		return false;
	
	// Compute the half size.
	Size halfSize = size / 2;
	const Complex<T>* const dataHalfway = data + halfSize;
	
	// Swap the first and second halves of the array.
	for ( Complex<T>* first = data, *second = data + halfSize; first < dataHalfway; first++, second++ )
	{
		Complex<T> temp = *first;
		*first = *second;
		*second = temp;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		2D FFT Implementation
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
OM_FORCE_INLINE static Bool fftTranspose( Complex<T>* data, Size width, Size height )
{
	if ( data == NULL )
		return false;
	
	// Use a simple algorithm for square matrices.
	if ( width == height )
	{
		for ( Index y = 0; y < height - 1; y++ )
		{
			for ( Index x = y + 1; x < width; x++ )
			{
				Complex<T> temp = data[y*width + x];
				data[y*width + x] = data[x*width + y];
				data[x*width + y] = temp;
			}
		}
	}
	else
		return false;
	
	return true;
}




template < typename T, Bool inverse >
OM_FORCE_INLINE static Bool fft2D( Complex<T>* data, Size width, Size height )
{
	// Check the validity of the arguments
	if ( data == NULL )
		return false;
	
	if ( !math::isPowerOfTwo( width ) || !math::isPowerOfTwo(height) )
		return false;
	
	const Complex<T>* const dataEnd = data + width*height;
	
	// Do the FFT for each of the rows of the 2D matrix.
	for ( Complex<T>* row = data; row < dataEnd; row += width )
		fft<T,inverse>( row, width );
	
	// Transpose the matrix so that the columns are located along rows instead.
	// This allows us to use the simple 1D FFT for columns too.
	if ( !fftTranspose( data, width, height ) )
		return false;
	
	// Do the FFT for each of the rows of the 2D matrix.
	for ( Complex<T>* column = data; column < dataEnd; column += height )
		fft<T,inverse>( column, height );
	
	// Transpose the matrix back to its original form.
	if ( !fftTranspose( data, height, width ) )
		return false;
	
	return true;
}




template < typename T >
OM_FORCE_INLINE static Bool fftShift2DInternal( Complex<T>* data, Size width, Size height )
{
	// Check the validity of the arguments
	if ( data == NULL || width & 0x1 || height & 0x1 )
		return false;
	
	// Compute the half dimensions.
	Size halfWidth = width / 2;
	Size halfHeight = height / 2;
	
	// Swap bottom left and top right quadrants.
	for ( Index y = 0; y < halfHeight; y++ )
	{
		for ( Index x = 0; x < halfWidth; x++ )
		{
			// Compute the absolute index of the element in the bottom left quadrant.
			Index index1 = y*width + x;
			
			// Compute the absolute index of the element in the top right quadrant.
			Index index2 = (y + halfHeight)*width + (x + halfWidth);
			
			// Swap the elements at those locations.
			Complex<T> temp = data[index1];
			data[index1] = data[index2];
			data[index2] = temp;
		}
	}
	
	// Swap top left and bottom right quadrants.
	for ( Index y = 0; y < halfHeight; y++ )
	{
		for ( Index x = 0; x < halfWidth; x++ )
		{
			// Compute the absolute index of the element in the top left quadrant.
			Index index1 = (y + halfHeight)*width + x;
			
			// Compute the absolute index of the element in the bottom right quadrant.
			Index index2 = y*width + (x + halfWidth);
			
			// Swap the elements at those locations.
			Complex<T> temp = data[index1];
			data[index1] = data[index2];
			data[index2] = temp;
		}
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		2D Fourier Transform Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
Bool fft2D( Complex<Float>* data, Size width, Size height )
{
	return fft2D<Float,true>( data, width, height );
}




template <>
Bool fft2D( Complex<Double>* data, Size width, Size height )
{
	return fft2D<Double,true>( data, width, height );
}




template <>
Bool ifft2D( Complex<Float>* data, Size width, Size height )
{
	return fft2D<Float,false>( data, width, height );
}




template <>
Bool ifft2D( Complex<Double>* data, Size width, Size height )
{
	return fft2D<Double,false>( data, width, height );
}




template <>
Bool fftShift2D( Complex<Float>* data, Size width, Size height )
{
	return fftShift2DInternal( data, width, height );
}




template <>
Bool fftShift2D( Complex<Double>* data, Size width, Size height )
{
	return fftShift2DInternal( data, width, height );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
