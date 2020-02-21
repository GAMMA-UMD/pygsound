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

#ifndef INCLUDE_OM_FFT_REAL_H
#define INCLUDE_OM_FFT_REAL_H


#include "omMathConfig.h"


#include "omComplex.h"
#include "omFFTBase.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements a real fast fourier transform of a template type.
/**
  * The code is based on PFFFT by Julien Pommier, which is itself a translation
  * of FFTPackv4. This version allows different scalar types to be used, e.g. for
  * double precision, doing multiple FFTs at once with vector types.
  *
  *
  * Based on original fortran 77 code from FFTPACKv4 from NETLIB
  *  (http://www.netlib.org/fftpack), authored by Dr Paul Swarztrauber
  *  of NCAR, in 1985.
  *
  *  As confirmed by the NCAR fftpack software curators, the following
  *  FFTPACKv5 license applies to FFTPACKv4 sources. My changes are
  *  released under the same terms.
  *
  *  FFTPACK license:
  *
  * http://www.cisl.ucar.edu/css/software/fftpack5/ftpk.html
  *
  * Copyright (c) 2004 the University Corporation for Atmospheric
  * Research ("UCAR"). All rights reserved. Developed by NCAR's
  * Computational and Information Systems Laboratory, UCAR,
  * www.cisl.ucar.edu.
  *
  * Redistribution and use of the Software in source and binary forms,
  * with or without modification, is permitted provided that the
  * following conditions are met:
  *
  * - Neither the names of NCAR's Computational and Information Systems
  * Laboratory, the University Corporation for Atmospheric Research,
  * nor the names of its sponsors or contributors may be used to
  * endorse or promote products derived from this Software without
  * specific prior written permission.  
  *
  * - Redistributions of source code must retain the above copyright
  * notices, this list of conditions, and the disclaimer below.
  *
  * - Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions, and the disclaimer below in the
  * documentation and/or other materials provided with the
  * distribution.

  * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT
  * HOLDERS BE LIABLE FOR ANY CLAIM, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
  * SOFTWARE.
  */
template < typename Real >
class FFTReal
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new FFT with the specified size.
			OM_INLINE FFTReal( Size fftSize )
			{
				new_setup( (int)fftSize, &setup );
			}
			
			
			/// Create a copy of another FFT.
			OM_INLINE FFTReal( const FFTReal& other )
			{
				copySetup( &other.setup, &setup );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an FFT.
			OM_INLINE ~FFTReal()
			{
				destroy_setup( &setup );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another FFT object to this one.
			OM_INLINE FFTReal& operator = ( const FFTReal& other )
			{
				if ( this != &other )
				{
					destroy_setup( &setup );
					copySetup( &other.setup, &setup );
				}
				return *this;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Method
			
			
			/// Return the size of this FFT.
			OM_INLINE Size getSize() const
			{
				return setup.N;
			}
			
			
		//********************************************************************************
		//******	Real FFT Methods
			
			
			/// Convert N real float values to N/2+1 complex values in place.
			OM_INLINE void fft( Real* inOut, Real* workspace = NULL ) const
			{
				transform_internal<false>( &setup, (const Real*)inOut, (Real*)inOut, (v4sf*)workspace, 1 );
			}
			
			
			/// Convert N real float values to N/2+1 complex values.
			OM_INLINE void fft( const Real* input, Complex<Real>* output, Real* workspace ) const
			{
				transform_internal<false>( &setup, input, (Real*)output, (v4sf*)workspace, 1 );
			}
			
			
			/// Convert N/2+1 complex values to N real float values in place.
			OM_INLINE void ifft( Real* inOut, Real* workspace = NULL ) const
			{
				transform_internal<true>( &setup, inOut, inOut, (v4sf*)workspace, 1 );
			}
			
			
			/// Convert N/2+1 complex values to N real float values.
			OM_INLINE void ifft( const Complex<Real>* input, Real* output, Real* workspace ) const
			{
				transform_internal<true>( &setup, (const Real*)input, output, (v4sf*)workspace, 1 );
			}
			
			
		//********************************************************************************
		//******	Unordered Real FFT Methods
			
			
			/// Convert N real float values to N/2+1 complex values in place, with an implementation-defined order.
			OM_INLINE void fftUnordered( Real* inOut, Real* workspace = NULL ) const
			{
				transform_internal<false>( &setup, (const Real*)inOut, (Real*)inOut, (v4sf*)workspace, 0 );
			}
			
			
			/// Convert N real float values to N/2+1 complex values, with an implementation-defined order.
			OM_INLINE void fftUnordered( const Real* input, Complex<Real>* output, Real* workspace ) const
			{
				transform_internal<false>( &setup, input, (Real*)output, (v4sf*)workspace, 0 );
			}
			
			
			/// Convert N/2+1 complex values to N real float values in place, with an implementation-defined order.
			OM_INLINE void ifftUnordered( Real* inOut, Real* workspace = NULL ) const
			{
				transform_internal<true>( &setup, inOut, inOut, (v4sf*)workspace, 0 );
			}
			
			
			/// Convert N/2+1 complex values to N real float values, with an implementation-defined order.
			OM_INLINE void ifftUnordered( const Complex<Real>* input, Real* output, Real* workspace ) const
			{
				transform_internal<true>( &setup, (const Real*)input, output, (v4sf*)workspace, 0 );
			}
			
			
			/// Multiply add of N/2+1 unordered complex frequency-domain values.
			/**
			  * This function multiplys two array of complex values and adds the result
			  * to the destination array. The complex values are assumed to have the same ordering
			  * as the values returned by the fftUnordered method.
			  */
			OM_INLINE void multiplyAddUnordered( Complex<Float32>* destination, const Complex<Float32>* a, const Complex<Float32>* b ) const
			{
				zconvolve_accumulate( &setup, (const Real*)a, (const Real*)b, (Real*)destination );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Internal FFT Methods
			
			typedef FFTTypeInfo<Real> T;
			typedef typename T::Scalar Scalar;
			typedef typename T::v4sf v4sf;
			
			
			struct Setup
			{
				int N;
				int Ncvec; // nb of complex simd vectors (N/4 if PFFFT_COMPLEX, N/8 if PFFFT_REAL)
				int ifac[15];
				v4sf* data; // allocated room for twiddle coefs
				Scalar* e;    // points into 'data' , N/4*3 elements
				Scalar* twiddle; // points into 'data', N/4 elements
			};
			
			
			OM_FORCE_INLINE static void VCPLXMUL( v4sf& ar, v4sf& ai, const v4sf& br, const v4sf& bi )
			{
				v4sf tmp;
				tmp = T::VMUL(ar,bi);
				ar = T::VMUL(ar,br);
				ar = T::VSUB(ar,T::VMUL(ai,bi));
				ai = T::VMUL(ai,br);
				ai = T::VADD(ai,tmp);
			}
			
			
			OM_FORCE_INLINE static void VCPLXMULCONJ( v4sf& ar, v4sf& ai, const v4sf& br, const v4sf& bi )
			{
				v4sf tmp;
				tmp = T::VMUL(ar,bi);
				ar = T::VMUL(ar,br);
				ar = T::VADD(ar,T::VMUL(ai,bi));
				ai = T::VMUL(ai,br);
				ai = T::VSUB(ai,tmp);
			}
			
			
		//********************************************************************************
		//******	Real FFT Methods
			
			
			OM_NO_INLINE static void radf2_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, const Scalar* wa1 );
			OM_NO_INLINE static void radb2_ps( int ido, int l1, const v4sf* cc, v4sf* ch, const Scalar* wa1 );
			
			
			OM_NO_INLINE static void radf3_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
												const Scalar* wa1, const Scalar* wa2 );
			OM_NO_INLINE static void radb3_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
												const Scalar* wa1, const Scalar* wa2 );
			
			
			OM_NO_INLINE static void radf4_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
											   const Scalar* OM_RESTRICT wa1, const Scalar* OM_RESTRICT wa2, const Scalar* OM_RESTRICT wa3 );
			OM_NO_INLINE static void radb4_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
												const Scalar* OM_RESTRICT wa1, const Scalar* OM_RESTRICT wa2, const Scalar* OM_RESTRICT wa3 );
			
			
			OM_NO_INLINE static void radf5_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, 
												const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, const Scalar* wa4 );
			OM_NO_INLINE static void radb5_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, 
												const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, const Scalar* wa4 );
			
			
			OM_NO_INLINE static v4sf* rfftf1_ps( int n, const v4sf* input_readonly, v4sf* work1, v4sf* work2, 
												  const Scalar* wa, const int* ifac );
			OM_NO_INLINE static v4sf* rfftb1_ps( int n, const v4sf* input_readonly, v4sf* work1, v4sf* work2, 
													const Scalar* wa, const int* ifac );
			
			// Generic version does nothing because these are only needed for the SIMD code.
			OM_INLINE static void reversed_copy( int N, const v4sf* in, int in_stride, v4sf* out ) {}
			OM_INLINE static void unreversed_copy( int N, const v4sf* in, v4sf* out, int out_stride ) {}
			
			
			template < Bool inverse >
			OM_INLINE static void zreorder( const Setup* setup, const Real* in, Real* out );
			
			
			// Generic version does nothing because these are only needed for the SIMD code.
			OM_FORCE_INLINE static void real_finalize_4x4( const v4sf* in0, const v4sf* in1, const v4sf* in, const v4sf* e, v4sf* out ) {}
			OM_NO_INLINE static void real_finalize( int Ncvec, const v4sf *in, v4sf *out, const v4sf *e ) {}
			OM_FORCE_INLINE static void real_preprocess_4x4( const v4sf *in, const v4sf *e, v4sf *out, int first ) {}
			OM_NO_INLINE static void real_preprocess(int Ncvec, const v4sf *in, v4sf *out, const v4sf *e) {}
			
			
			template < Bool inverse >
			OM_INLINE static void transform_internal( const Setup* setup, const Real* finput, Real* foutput, v4sf* scratch, int ordered );
			
			
			OM_INLINE static void zconvolve_accumulate( const Setup* s, const Real* a, const Real* b, Real* ab );
			
			
			OM_INLINE static int decompose( int n, int* ifac, const int* ntryh );
			OM_INLINE static void rffti1_ps( int n, Scalar* wa, int* ifac );
			OM_INLINE static void new_setup( int N, Setup* s );
			OM_INLINE static void destroy_setup( Setup* s );
			OM_INLINE static void copy_setup( const Setup* s1, Setup* s2 );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			Setup setup;
			
			
};




#if defined(OM_COMPILER_GCC)
#  define VLA_ARRAY_ON_STACK(type__, varname__, size__) type__ varname__[size__];
#elif defined(OM_COMPILER_MSVC)
#  define VLA_ARRAY_ON_STACK(type__, varname__, size__) type__ *varname__ = (type__*)_alloca(size__ * sizeof(type__))
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT 2
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTReal<Real>:: radf2_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, const Scalar* wa1 )
{
	static const Scalar minus_one = -1.f;
	int i, k, l1ido = l1*ido;
	for (k=0; k < l1ido; k += ido) {
		v4sf a = cc[k], b = cc[k + l1ido];
		ch[2*k] = T::VADD(a, b);
		ch[2*(k+ido)-1] = T::VSUB(a, b);
	}
	if (ido < 2) return;
	if (ido != 2) {
		for (k=0; k < l1ido; k += ido) {
			for (i=2; i<ido; i+=2) {
				v4sf tr2 = cc[i - 1 + k + l1ido], ti2 = cc[i + k + l1ido];
				v4sf br = cc[i - 1 + k], bi = cc[i + k];
				VCPLXMULCONJ(tr2, ti2, T::LD_PS1(wa1[i - 2]), T::LD_PS1(wa1[i - 1])); 
				ch[i + 2*k] = T::VADD(bi, ti2);
				ch[2*(k+ido) - i] = T::VSUB(ti2, bi);
				ch[i - 1 + 2*k] = T::VADD(br, tr2);
				ch[2*(k+ido) - i -1] = T::VSUB(br, tr2);
			}
		}
		if (ido % 2 == 1) return;
	}
	for (k=0; k < l1ido; k += ido) {
		ch[2*k + ido] = T::SVMUL(minus_one, cc[ido-1 + k + l1ido]);
		ch[2*k + ido-1] = cc[k + ido-1];
	}
} /* radf2 */




template < typename Real >
void FFTReal<Real>:: radb2_ps( int ido, int l1, const v4sf *cc, v4sf *ch, const Scalar* wa1 )
{
	static const Scalar minus_two=-2;
	int i, k, l1ido = l1*ido;
	v4sf a,b,c,d, tr2, ti2;
	for (k=0; k < l1ido; k += ido) {
		a = cc[2*k]; b = cc[2*(k+ido) - 1];
		ch[k] = T::VADD(a, b);
		ch[k + l1ido] = T::VSUB(a, b);
	}
	if (ido < 2) return;
	if (ido != 2) {
		for (k = 0; k < l1ido; k += ido) {
			for (i = 2; i < ido; i += 2) {
				a = cc[i-1 + 2*k]; b = cc[2*(k + ido) - i - 1];
				c = cc[i+0 + 2*k]; d = cc[2*(k + ido) - i + 0];
				ch[i-1 + k] = T::VADD(a, b);
				tr2 = T::VSUB(a, b);
				ch[i+0 + k] = T::VSUB(c, d);
				ti2 = T::VADD(c, d);
				VCPLXMUL(tr2, ti2, T::LD_PS1(wa1[i - 2]), T::LD_PS1(wa1[i - 1]));
				ch[i-1 + k + l1ido] = tr2;
				ch[i+0 + k + l1ido] = ti2;
			}
		}
		if (ido % 2 == 1) return;
	}
	for (k = 0; k < l1ido; k += ido) {
		a = cc[2*k + ido-1]; b = cc[2*k + ido];
		ch[k + ido-1] = T::VADD(a,a);
		ch[k + ido-1 + l1ido] = T::SVMUL(minus_two, b);
	}
} /* radb2 */




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT 3
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTReal<Real>:: radf3_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
								const Scalar* wa1, const Scalar* wa2 )
{
	static const Scalar taur = Scalar(-0.5);
	static const Scalar taui = Scalar(0.866025403784439);
	int i, k, ic;
	v4sf ci2, di2, di3, cr2, dr2, dr3, ti2, ti3, tr2, tr3, wr1, wi1, wr2, wi2;
	for (k=0; k<l1; k++) {
		cr2 = T::VADD(cc[(k + l1)*ido], cc[(k + 2*l1)*ido]);
		ch[3*k*ido] = T::VADD(cc[k*ido], cr2);
		ch[(3*k+2)*ido] = T::SVMUL(taui, T::VSUB(cc[(k + l1*2)*ido], cc[(k + l1)*ido]));
		ch[ido-1 + (3*k + 1)*ido] = T::VADD(cc[k*ido], T::SVMUL(taur, cr2));
	}
	if (ido == 1) return;
	for (k=0; k<l1; k++) {
		for (i=2; i<ido; i+=2) {
			ic = ido - i;
			wr1 = T::LD_PS1(wa1[i - 2]); wi1 = T::LD_PS1(wa1[i - 1]);
			dr2 = cc[i - 1 + (k + l1)*ido]; di2 = cc[i + (k + l1)*ido];
			VCPLXMULCONJ(dr2, di2, wr1, wi1);
			
			wr2 = T::LD_PS1(wa2[i - 2]); wi2 = T::LD_PS1(wa2[i - 1]);
			dr3 = cc[i - 1 + (k + l1*2)*ido]; di3 = cc[i + (k + l1*2)*ido];
			VCPLXMULCONJ(dr3, di3, wr2, wi2);
			
			cr2 = T::VADD(dr2, dr3);
			ci2 = T::VADD(di2, di3);
			ch[i - 1 + 3*k*ido] = T::VADD(cc[i - 1 + k*ido], cr2);
			ch[i + 3*k*ido] = T::VADD(cc[i + k*ido], ci2);
			tr2 = T::VADD(cc[i - 1 + k*ido], T::SVMUL(taur, cr2));
			ti2 = T::VADD(cc[i + k*ido], T::SVMUL(taur, ci2));
			tr3 = T::SVMUL(taui, T::VSUB(di2, di3));
			ti3 = T::SVMUL(taui, T::VSUB(dr3, dr2));
			ch[i - 1 + (3*k + 2)*ido] = T::VADD(tr2, tr3);
			ch[ic - 1 + (3*k + 1)*ido] = T::VSUB(tr2, tr3);
			ch[i + (3*k + 2)*ido] = T::VADD(ti2, ti3);
			ch[ic + (3*k + 1)*ido] = T::VSUB(ti3, ti2);
		}
	}
} /* radf3 */




template < typename Real >
void FFTReal<Real>:: radb3_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
								const Scalar* wa1, const Scalar* wa2 )
{
	static const Scalar taur = Scalar(-0.5);
	static const Scalar taui = Scalar(0.866025403784439);
	static const Scalar taui_2 = Scalar(0.866025403784439*2);
	int i, k, ic;
	v4sf ci2, ci3, di2, di3, cr2, cr3, dr2, dr3, ti2, tr2;
	for (k=0; k<l1; k++) {
		tr2 = cc[ido-1 + (3*k + 1)*ido]; tr2 = T::VADD(tr2,tr2);
		cr2 = T::VMADD(T::LD_PS1(taur), tr2, cc[3*k*ido]);
		ch[k*ido] = T::VADD(cc[3*k*ido], tr2);
		ci3 = T::SVMUL(taui_2, cc[(3*k + 2)*ido]);
		ch[(k + l1)*ido] = T::VSUB(cr2, ci3);
		ch[(k + 2*l1)*ido] = T::VADD(cr2, ci3);
	}
	if (ido == 1) return;
	for (k=0; k<l1; k++) {
		for (i=2; i<ido; i+=2) {
			ic = ido - i;
			tr2 = T::VADD(cc[i - 1 + (3*k + 2)*ido], cc[ic - 1 + (3*k + 1)*ido]);
			cr2 = T::VMADD(T::LD_PS1(taur), tr2, cc[i - 1 + 3*k*ido]);
			ch[i - 1 + k*ido] = T::VADD(cc[i - 1 + 3*k*ido], tr2);
			ti2 = T::VSUB(cc[i + (3*k + 2)*ido], cc[ic + (3*k + 1)*ido]);
			ci2 = T::VMADD(T::LD_PS1(taur), ti2, cc[i + 3*k*ido]);
			ch[i + k*ido] = T::VADD(cc[i + 3*k*ido], ti2);
			cr3 = T::SVMUL(taui, T::VSUB(cc[i - 1 + (3*k + 2)*ido], cc[ic - 1 + (3*k + 1)*ido]));
			ci3 = T::SVMUL(taui, T::VADD(cc[i + (3*k + 2)*ido], cc[ic + (3*k + 1)*ido]));
			dr2 = T::VSUB(cr2, ci3);
			dr3 = T::VADD(cr2, ci3);
			di2 = T::VADD(ci2, cr3);
			di3 = T::VSUB(ci2, cr3);
			VCPLXMUL(dr2, di2, T::LD_PS1(wa1[i-2]), T::LD_PS1(wa1[i-1]));
			ch[i - 1 + (k + l1)*ido] = dr2;
			ch[i + (k + l1)*ido] = di2;
			VCPLXMUL(dr3, di3, T::LD_PS1(wa2[i-2]), T::LD_PS1(wa2[i-1]));
			ch[i - 1 + (k + 2*l1)*ido] = dr3;
			ch[i + (k + 2*l1)*ido] = di3;
		}
	}
} /* radb3 */




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT 4
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTReal<Real>:: radf4_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
                                   const Scalar* OM_RESTRICT wa1, const Scalar* OM_RESTRICT wa2, const Scalar* OM_RESTRICT wa3 )
{
	static const Scalar minus_hsqt2 = Scalar(-0.7071067811865475);
	int i, k, l1ido = l1*ido;
	{
		const v4sf* OM_RESTRICT cc_ = cc, *OM_RESTRICT cc_end = cc + l1ido; 
		v4sf* OM_RESTRICT ch_ = ch;
		while (cc < cc_end) {
			// this loop represents between 25% and 40% of total radf4_ps cost !
			v4sf a0 = cc[0], a1 = cc[l1ido];
			v4sf a2 = cc[2*l1ido], a3 = cc[3*l1ido];
			v4sf tr1 = T::VADD(a1, a3);
			v4sf tr2 = T::VADD(a0, a2);
			ch[2*ido-1] = T::VSUB(a0, a2);
			ch[2*ido  ] = T::VSUB(a3, a1);
			ch[0      ] = T::VADD(tr1, tr2);
			ch[4*ido-1] = T::VSUB(tr2, tr1);
			cc += ido; ch += 4*ido;
		}
		cc = cc_; ch = ch_;
	}
	if (ido < 2) return;
	if (ido != 2) {
		for (k = 0; k < l1ido; k += ido) {
			const v4sf* OM_RESTRICT pc = (v4sf*)(cc + 1 + k);
			for (i=2; i<ido; i += 2, pc += 2) {
				int ic = ido - i;
				v4sf wr, wi, cr2, ci2, cr3, ci3, cr4, ci4;
				v4sf tr1, ti1, tr2, ti2, tr3, ti3, tr4, ti4;
				
				cr2 = pc[1*l1ido+0];
				ci2 = pc[1*l1ido+1];
				wr=T::LD_PS1(wa1[i - 2]);
				wi=T::LD_PS1(wa1[i - 1]);
				VCPLXMULCONJ(cr2,ci2,wr,wi);
				
				cr3 = pc[2*l1ido+0];
				ci3 = pc[2*l1ido+1];
				wr = T::LD_PS1(wa2[i-2]); 
				wi = T::LD_PS1(wa2[i-1]);
				VCPLXMULCONJ(cr3, ci3, wr, wi);
				
				cr4 = pc[3*l1ido];
				ci4 = pc[3*l1ido+1];
				wr = T::LD_PS1(wa3[i-2]); 
				wi = T::LD_PS1(wa3[i-1]);
				VCPLXMULCONJ(cr4, ci4, wr, wi);
				
				/* at this point, on SSE, five of "cr2 cr3 cr4 ci2 ci3 ci4" should be loaded in registers */
				
				tr1 = T::VADD(cr2,cr4);
				tr4 = T::VSUB(cr4,cr2); 
				tr2 = T::VADD(pc[0],cr3);
				tr3 = T::VSUB(pc[0],cr3);
				ch[i - 1 + 4*k] = T::VADD(tr1,tr2);
				ch[ic - 1 + 4*k + 3*ido] = T::VSUB(tr2,tr1); // at this point tr1 and tr2 can be disposed
				ti1 = T::VADD(ci2,ci4);
				ti4 = T::VSUB(ci2,ci4);
				ch[i - 1 + 4*k + 2*ido] = T::VADD(ti4,tr3);
				ch[ic - 1 + 4*k + 1*ido] = T::VSUB(tr3,ti4); // dispose tr3, ti4
				ti2 = T::VADD(pc[1],ci3);
				ti3 = T::VSUB(pc[1],ci3);
				ch[i + 4*k] = T::VADD(ti1, ti2);
				ch[ic + 4*k + 3*ido] = T::VSUB(ti1, ti2);
				ch[i + 4*k + 2*ido] = T::VADD(tr4, ti3);
				ch[ic + 4*k + 1*ido] = T::VSUB(tr4, ti3);
			}
		}
		if (ido % 2 == 1) return;
	}
	for (k=0; k<l1ido; k += ido) {
		v4sf a = cc[ido-1 + k + l1ido], b = cc[ido-1 + k + 3*l1ido];
		v4sf c = cc[ido-1 + k], d = cc[ido-1 + k + 2*l1ido];
		v4sf ti1 = T::SVMUL(minus_hsqt2, T::VADD(a, b));
		v4sf tr1 = T::SVMUL(minus_hsqt2, T::VSUB(b, a));
		ch[ido-1 + 4*k] = T::VADD(tr1, c);
		ch[ido-1 + 4*k + 2*ido] = T::VSUB(c, tr1);
		ch[4*k + 1*ido] = T::VSUB(ti1, d); 
		ch[4*k + 3*ido] = T::VADD(ti1, d); 
	}
} /* radf4 */




template < typename Real >
void FFTReal<Real>:: radb4_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch,
								const Scalar* OM_RESTRICT wa1, const Scalar* OM_RESTRICT wa2, const Scalar* OM_RESTRICT wa3 )
{
	static const Scalar minus_sqrt2 = Scalar(-1.414213562373095);
	static const Scalar two = Scalar(2.0);
	int i, k, l1ido = l1*ido;
	v4sf ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, tr2, tr3, tr4;
	{
		const v4sf* OM_RESTRICT cc_ = cc, * OM_RESTRICT ch_end = ch + l1ido; 
		v4sf *ch_ = ch;
		while (ch < ch_end) {
			v4sf a = cc[0], b = cc[4*ido-1];
			v4sf c = cc[2*ido], d = cc[2*ido-1];
			tr3 = T::SVMUL(two,d);
			tr2 = T::VADD(a,b);
			tr1 = T::VSUB(a,b);
			tr4 = T::SVMUL(two,c);
			ch[0*l1ido] = T::VADD(tr2, tr3);
			ch[2*l1ido] = T::VSUB(tr2, tr3);
			ch[1*l1ido] = T::VSUB(tr1, tr4);
			ch[3*l1ido] = T::VADD(tr1, tr4);
			
			cc += 4*ido; ch += ido;
		}
		cc = cc_; ch = ch_;
	}
	if (ido < 2) return;
	if (ido != 2) {
		for (k = 0; k < l1ido; k += ido) {
			const v4sf* OM_RESTRICT pc = (v4sf*)(cc - 1 + 4*k);
			v4sf* OM_RESTRICT ph = (v4sf*)(ch + k + 1);
			for (i = 2; i < ido; i += 2) {
				
				tr1 = T::VSUB(pc[i], pc[4*ido - i]);
				tr2 = T::VADD(pc[i], pc[4*ido - i]);
				ti4 = T::VSUB(pc[2*ido + i], pc[2*ido - i]);
				tr3 = T::VADD(pc[2*ido + i], pc[2*ido - i]);
				ph[0] = T::VADD(tr2, tr3);
				cr3 = T::VSUB(tr2, tr3);
				
				ti3 = T::VSUB(pc[2*ido + i + 1], pc[2*ido - i + 1]);
				tr4 = T::VADD(pc[2*ido + i + 1], pc[2*ido - i + 1]);
				cr2 = T::VSUB(tr1, tr4);
				cr4 = T::VADD(tr1, tr4);
				
				ti1 = T::VADD(pc[i + 1], pc[4*ido - i + 1]);
				ti2 = T::VSUB(pc[i + 1], pc[4*ido - i + 1]);
				
				ph[1] = T::VADD(ti2, ti3); ph += l1ido;
				ci3 = T::VSUB(ti2, ti3);
				ci2 = T::VADD(ti1, ti4);
				ci4 = T::VSUB(ti1, ti4);
				VCPLXMUL(cr2, ci2, T::LD_PS1(wa1[i-2]), T::LD_PS1(wa1[i-1]));
				ph[0] = cr2;
				ph[1] = ci2; ph += l1ido;
				VCPLXMUL(cr3, ci3, T::LD_PS1(wa2[i-2]), T::LD_PS1(wa2[i-1]));
				ph[0] = cr3;
				ph[1] = ci3; ph += l1ido;
				VCPLXMUL(cr4, ci4, T::LD_PS1(wa3[i-2]), T::LD_PS1(wa3[i-1]));
				ph[0] = cr4;
				ph[1] = ci4; ph = ph - 3*l1ido + 2;
			}
		}
		if (ido % 2 == 1) return;
	}
	for (k=0; k < l1ido; k+=ido) {
		int i0 = 4*k + ido;
		v4sf c = cc[i0-1], d = cc[i0 + 2*ido-1];
		v4sf a = cc[i0+0], b = cc[i0 + 2*ido+0];
		tr1 = T::VSUB(c,d);
		tr2 = T::VADD(c,d);
		ti1 = T::VADD(b,a);
		ti2 = T::VSUB(b,a);
		ch[ido-1 + k + 0*l1ido] = T::VADD(tr2,tr2);
		ch[ido-1 + k + 1*l1ido] = T::SVMUL(minus_sqrt2, T::VSUB(ti1, tr1));
		ch[ido-1 + k + 2*l1ido] = T::VADD(ti2, ti2);
		ch[ido-1 + k + 3*l1ido] = T::SVMUL(minus_sqrt2, T::VADD(ti1, tr1));
	}
} /* radb4 */




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT 5
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTReal<Real>:: radf5_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, 
								const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, const Scalar* wa4 )
{
	static const Scalar tr11 = Scalar(0.309016994374947);
	static const Scalar ti11 = Scalar(0.951056516295154);
	static const Scalar tr12 = Scalar(-0.809016994374947);
	static const Scalar ti12 = Scalar(0.587785252292473);
	
	/* System generated locals */
	int cc_offset, ch_offset;
	
	/* Local variables */
	int i, k, ic;
	v4sf ci2, di2, ci4, ci5, di3, di4, di5, ci3, cr2, cr3, dr2, dr3, dr4, dr5,
    cr5, cr4, ti2, ti3, ti5, ti4, tr2, tr3, tr4, tr5;
	int idp2;
	
	
#define cc_ref(a_1,a_2,a_3) cc[((a_3)*l1 + (a_2))*ido + a_1]
#define ch_ref(a_1,a_2,a_3) ch[((a_3)*5 + (a_2))*ido + a_1]
	
	/* Parameter adjustments */
	ch_offset = 1 + ido * 6;
	ch -= ch_offset;
	cc_offset = 1 + ido * (1 + l1);
	cc -= cc_offset;
	
	/* Function Body */
	for (k = 1; k <= l1; ++k) {
		cr2 = T::VADD(cc_ref(1, k, 5), cc_ref(1, k, 2));
		ci5 = T::VSUB(cc_ref(1, k, 5), cc_ref(1, k, 2));
		cr3 = T::VADD(cc_ref(1, k, 4), cc_ref(1, k, 3));
		ci4 = T::VSUB(cc_ref(1, k, 4), cc_ref(1, k, 3));
		ch_ref(1, 1, k) = T::VADD(cc_ref(1, k, 1), T::VADD(cr2, cr3));
		ch_ref(ido, 2, k) = T::VADD(cc_ref(1, k, 1), T::VADD(T::SVMUL(tr11, cr2), T::SVMUL(tr12, cr3)));
		ch_ref(1, 3, k) = T::VADD(T::SVMUL(ti11, ci5), T::SVMUL(ti12, ci4));
		ch_ref(ido, 4, k) = T::VADD(cc_ref(1, k, 1), T::VADD(T::SVMUL(tr12, cr2), T::SVMUL(tr11, cr3)));
		ch_ref(1, 5, k) = T::VSUB(T::SVMUL(ti12, ci5), T::SVMUL(ti11, ci4));
	}
	if (ido == 1) {
		return;
	}
	idp2 = ido + 2;
	for (k = 1; k <= l1; ++k) {
		for (i = 3; i <= ido; i += 2) {
			ic = idp2 - i;
			dr2 = T::LD_PS1(wa1[i-3]); di2 = T::LD_PS1(wa1[i-2]);
			dr3 = T::LD_PS1(wa2[i-3]); di3 = T::LD_PS1(wa2[i-2]);
			dr4 = T::LD_PS1(wa3[i-3]); di4 = T::LD_PS1(wa3[i-2]);
			dr5 = T::LD_PS1(wa4[i-3]); di5 = T::LD_PS1(wa4[i-2]);
			VCPLXMULCONJ(dr2, di2, cc_ref(i-1, k, 2), cc_ref(i, k, 2));
			VCPLXMULCONJ(dr3, di3, cc_ref(i-1, k, 3), cc_ref(i, k, 3));
			VCPLXMULCONJ(dr4, di4, cc_ref(i-1, k, 4), cc_ref(i, k, 4));
			VCPLXMULCONJ(dr5, di5, cc_ref(i-1, k, 5), cc_ref(i, k, 5));
			cr2 = T::VADD(dr2, dr5);
			ci5 = T::VSUB(dr5, dr2);
			cr5 = T::VSUB(di2, di5);
			ci2 = T::VADD(di2, di5);
			cr3 = T::VADD(dr3, dr4);
			ci4 = T::VSUB(dr4, dr3);
			cr4 = T::VSUB(di3, di4);
			ci3 = T::VADD(di3, di4);
			ch_ref(i - 1, 1, k) = T::VADD(cc_ref(i - 1, k, 1), T::VADD(cr2, cr3));
			ch_ref(i, 1, k) = T::VSUB(cc_ref(i, k, 1), T::VADD(ci2, ci3));//
			tr2 = T::VADD(cc_ref(i - 1, k, 1), T::VADD(T::SVMUL(tr11, cr2), T::SVMUL(tr12, cr3)));
			ti2 = T::VSUB(cc_ref(i, k, 1), T::VADD(T::SVMUL(tr11, ci2), T::SVMUL(tr12, ci3)));//
			tr3 = T::VADD(cc_ref(i - 1, k, 1), T::VADD(T::SVMUL(tr12, cr2), T::SVMUL(tr11, cr3)));
			ti3 = T::VSUB(cc_ref(i, k, 1), T::VADD(T::SVMUL(tr12, ci2), T::SVMUL(tr11, ci3)));//
			tr5 = T::VADD(T::SVMUL(ti11, cr5), T::SVMUL(ti12, cr4));
			ti5 = T::VADD(T::SVMUL(ti11, ci5), T::SVMUL(ti12, ci4));
			tr4 = T::VSUB(T::SVMUL(ti12, cr5), T::SVMUL(ti11, cr4));
			ti4 = T::VSUB(T::SVMUL(ti12, ci5), T::SVMUL(ti11, ci4));
			ch_ref(i - 1, 3, k) = T::VSUB(tr2, tr5);
			ch_ref(ic - 1, 2, k) = T::VADD(tr2, tr5);
			ch_ref(i, 3, k) = T::VADD(ti2, ti5);
			ch_ref(ic, 2, k) = T::VSUB(ti5, ti2);
			ch_ref(i - 1, 5, k) = T::VSUB(tr3, tr4);
			ch_ref(ic - 1, 4, k) = T::VADD(tr3, tr4);
			ch_ref(i, 5, k) = T::VADD(ti3, ti4);
			ch_ref(ic, 4, k) = T::VSUB(ti4, ti3);
		}
	}
#undef cc_ref
#undef ch_ref
} /* radf5 */




template < typename Real >
void FFTReal<Real>:: radb5_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, 
						const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, const Scalar* wa4 )
{
	static const Scalar tr11 = Scalar(0.309016994374947);
	static const Scalar ti11 = Scalar(0.951056516295154);
	static const Scalar tr12 = Scalar(-0.809016994374947);
	static const Scalar ti12 = Scalar(0.587785252292473);
	
	int cc_offset, ch_offset;
	
	/* Local variables */
	int i, k, ic;
	v4sf ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, cr4, ti2, ti3,
    ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;
	int idp2;
	
#define cc_ref(a_1,a_2,a_3) cc[((a_3)*5 + (a_2))*ido + a_1]
#define ch_ref(a_1,a_2,a_3) ch[((a_3)*l1 + (a_2))*ido + a_1]
	
	/* Parameter adjustments */
	ch_offset = 1 + ido * (1 + l1);
	ch -= ch_offset;
	cc_offset = 1 + ido * 6;
	cc -= cc_offset;
	
	/* Function Body */
	for (k = 1; k <= l1; ++k) {
		ti5 = T::VADD(cc_ref(1, 3, k), cc_ref(1, 3, k));
		ti4 = T::VADD(cc_ref(1, 5, k), cc_ref(1, 5, k));
		tr2 = T::VADD(cc_ref(ido, 2, k), cc_ref(ido, 2, k));
		tr3 = T::VADD(cc_ref(ido, 4, k), cc_ref(ido, 4, k));
		ch_ref(1, k, 1) = T::VADD(cc_ref(1, 1, k), T::VADD(tr2, tr3));
		cr2 = T::VADD(cc_ref(1, 1, k), T::VADD(T::SVMUL(tr11, tr2), T::SVMUL(tr12, tr3)));
		cr3 = T::VADD(cc_ref(1, 1, k), T::VADD(T::SVMUL(tr12, tr2), T::SVMUL(tr11, tr3)));
		ci5 = T::VADD(T::SVMUL(ti11, ti5), T::SVMUL(ti12, ti4));
		ci4 = T::VSUB(T::SVMUL(ti12, ti5), T::SVMUL(ti11, ti4));
		ch_ref(1, k, 2) = T::VSUB(cr2, ci5);
		ch_ref(1, k, 3) = T::VSUB(cr3, ci4);
		ch_ref(1, k, 4) = T::VADD(cr3, ci4);
		ch_ref(1, k, 5) = T::VADD(cr2, ci5);
	}
	if (ido == 1) {
		return;
	}
	idp2 = ido + 2;
	for (k = 1; k <= l1; ++k) {
		for (i = 3; i <= ido; i += 2) {
			ic = idp2 - i;
			ti5 = T::VADD(cc_ref(i  , 3, k), cc_ref(ic  , 2, k));
			ti2 = T::VSUB(cc_ref(i  , 3, k), cc_ref(ic  , 2, k));
			ti4 = T::VADD(cc_ref(i  , 5, k), cc_ref(ic  , 4, k));
			ti3 = T::VSUB(cc_ref(i  , 5, k), cc_ref(ic  , 4, k));
			tr5 = T::VSUB(cc_ref(i-1, 3, k), cc_ref(ic-1, 2, k));
			tr2 = T::VADD(cc_ref(i-1, 3, k), cc_ref(ic-1, 2, k));
			tr4 = T::VSUB(cc_ref(i-1, 5, k), cc_ref(ic-1, 4, k));
			tr3 = T::VADD(cc_ref(i-1, 5, k), cc_ref(ic-1, 4, k));
			ch_ref(i - 1, k, 1) = T::VADD(cc_ref(i-1, 1, k), T::VADD(tr2, tr3));
			ch_ref(i, k, 1) = T::VADD(cc_ref(i, 1, k), T::VADD(ti2, ti3));
			cr2 = T::VADD(cc_ref(i-1, 1, k), T::VADD(T::SVMUL(tr11, tr2), T::SVMUL(tr12, tr3)));
			ci2 = T::VADD(cc_ref(i  , 1, k), T::VADD(T::SVMUL(tr11, ti2), T::SVMUL(tr12, ti3)));
			cr3 = T::VADD(cc_ref(i-1, 1, k), T::VADD(T::SVMUL(tr12, tr2), T::SVMUL(tr11, tr3)));
			ci3 = T::VADD(cc_ref(i  , 1, k), T::VADD(T::SVMUL(tr12, ti2), T::SVMUL(tr11, ti3)));
			cr5 = T::VADD(T::SVMUL(ti11, tr5), T::SVMUL(ti12, tr4));
			ci5 = T::VADD(T::SVMUL(ti11, ti5), T::SVMUL(ti12, ti4));
			cr4 = T::VSUB(T::SVMUL(ti12, tr5), T::SVMUL(ti11, tr4));
			ci4 = T::VSUB(T::SVMUL(ti12, ti5), T::SVMUL(ti11, ti4));
			dr3 = T::VSUB(cr3, ci4);
			dr4 = T::VADD(cr3, ci4);
			di3 = T::VADD(ci3, cr4);
			di4 = T::VSUB(ci3, cr4);
			dr5 = T::VADD(cr2, ci5);
			dr2 = T::VSUB(cr2, ci5);
			di5 = T::VSUB(ci2, cr5);
			di2 = T::VADD(ci2, cr5);
			VCPLXMUL(dr2, di2, T::LD_PS1(wa1[i-3]), T::LD_PS1(wa1[i-2]));
			VCPLXMUL(dr3, di3, T::LD_PS1(wa2[i-3]), T::LD_PS1(wa2[i-2]));
			VCPLXMUL(dr4, di4, T::LD_PS1(wa3[i-3]), T::LD_PS1(wa3[i-2]));
			VCPLXMUL(dr5, di5, T::LD_PS1(wa4[i-3]), T::LD_PS1(wa4[i-2]));
			
			ch_ref(i-1, k, 2) = dr2; ch_ref(i, k, 2) = di2;
			ch_ref(i-1, k, 3) = dr3; ch_ref(i, k, 3) = di3;
			ch_ref(i-1, k, 4) = dr4; ch_ref(i, k, 4) = di4;
			ch_ref(i-1, k, 5) = dr5; ch_ref(i, k, 5) = di5;
		}
	}
#undef cc_ref
#undef ch_ref
} /* radb5 */




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
typename FFTReal<Real>::v4sf* FFTReal<Real>:: rfftf1_ps( int n, const v4sf* input_readonly, v4sf* work1, v4sf* work2, 
														const Scalar* wa, const int* ifac )
{  
	v4sf *in  = (v4sf*)input_readonly;
	v4sf *out = (in == work2 ? work1 : work2);
	int nf = ifac[1], k1;
	int l2 = n;
	int iw = n-1;
	OM_DEBUG_ASSERT(in != out && work1 != work2);
	for (k1 = 1; k1 <= nf; ++k1) {
		int kh = nf - k1;
		int ip = ifac[kh + 2];
		int l1 = l2 / ip;
		int ido = n / l2;
		iw -= (ip - 1)*ido;
		switch (ip) {
			case 5: {
				int ix2 = iw + ido;
				int ix3 = ix2 + ido;
				int ix4 = ix3 + ido;
				radf5_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4]);
			} break;
			case 4: {
				int ix2 = iw + ido;
				int ix3 = ix2 + ido;
				radf4_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3]);
			} break;
			case 3: {
				int ix2 = iw + ido;
				radf3_ps(ido, l1, in, out, &wa[iw], &wa[ix2]);
			} break;
			case 2:
				radf2_ps(ido, l1, in, out, &wa[iw]);
				break;
			default:
				OM_DEBUG_ASSERT(0);
				break;
		}
		l2 = l1;
		if (out == work2) {
			out = work1; in = work2;
		} else {
			out = work2; in = work1;
		}
	}
	return in; /* this is in fact the output .. */
} /* rfftf1 */




template < typename Real >
typename FFTReal<Real>::v4sf* FFTReal<Real>:: rfftb1_ps( int n, const v4sf* input_readonly, v4sf* work1, v4sf* work2, 
														const Scalar* wa, const int* ifac )
{  
	v4sf *in  = (v4sf*)input_readonly;
	v4sf *out = (in == work2 ? work1 : work2);
	int nf = ifac[1], k1;
	int l1 = 1;
	int iw = 0;
	OM_DEBUG_ASSERT(in != out);
	for (k1=1; k1<=nf; k1++) {
		int ip = ifac[k1 + 1];
		int l2 = ip*l1;
		int ido = n / l2;
		switch (ip) {
			case 5: {
				int ix2 = iw + ido;
				int ix3 = ix2 + ido;
				int ix4 = ix3 + ido;
				radb5_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4]);
			} break;
			case 4: {
				int ix2 = iw + ido;
				int ix3 = ix2 + ido;
				radb4_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3]);
			} break;
			case 3: {
				int ix2 = iw + ido;
				radb3_ps(ido, l1, in, out, &wa[iw], &wa[ix2]);
			} break;
			case 2:
				radb2_ps(ido, l1, in, out, &wa[iw]);
				break;
			default:
				OM_DEBUG_ASSERT(0);
				break;
		}
		l1 = l2;
		iw += (ip - 1)*ido;
		
		if (out == work2) {
			out = work1; in = work2;
		} else {
			out = work2; in = work1;
		}
	}
	return in; /* this is in fact the output .. */
}




//##########################################################################################
//##########################################################################################
//############		
//############		Real Reordering
//############		
//##########################################################################################
//##########################################################################################




// Specialization for 32-bit floats using SIMD.
/* [0 0 1 2 3 4 5 6 7 8] -> [0 8 7 6 5 4 3 2 1] */
template <>
OM_INLINE void FFTReal<Float32>:: reversed_copy( int N, const v4sf* in, int in_stride, v4sf* out )
{
	v4sf g0, g1;
	int k;
	T::INTERLEAVE2(in[0], in[1], g0, g1); in += in_stride;
	
	*--out = T::VSWAPHL(g0, g1); // [g0l, g0h], [g1l g1h] -> [g1l, g0h]
	for (k=1; k < N; ++k) {
		v4sf h0, h1;
		T::INTERLEAVE2(in[0], in[1], h0, h1); in += in_stride;
		*--out = T::VSWAPHL(g1, h0);
		*--out = T::VSWAPHL(h0, h1);
		g1 = h1;
	}
	*--out = T::VSWAPHL(g1, g0);
}



// Specialization for 32-bit floats using SIMD.
template <>
OM_INLINE void FFTReal<Float32>:: unreversed_copy( int N, const v4sf *in, v4sf *out, int out_stride )
{
	v4sf g0, g1, h0, h1;
	int k;
	g0 = g1 = in[0]; ++in;
	for (k=1; k < N; ++k) {
		h0 = *in++; h1 = *in++;
		g1 = T::VSWAPHL(g1, h0);
		h0 = T::VSWAPHL(h0, h1);
		T::UNINTERLEAVE2(h0, g1, out[0], out[1]); out += out_stride;
		g1 = h1;
	}
	h0 = *in++; h1 = g0;
	g1 = T::VSWAPHL(g1, h0);
	h0 = T::VSWAPHL(h0, h1);
	T::UNINTERLEAVE2(h0, g1, out[0], out[1]);
}




// Specialization for 32-bit floats using SIMD.
template <> template < Bool inverse >
void FFTReal<Float32>:: zreorder( const Setup* setup, const Float32* in, Float32* out )
{
	int k, N = setup->N;
	const v4sf *vin = (const v4sf*)in;
	v4sf *vout = (v4sf*)out;
	OM_DEBUG_ASSERT(in != out);
	
	int dk = N/32;
	
	if ( inverse )
	{
		Float x_N = in[N];
		for (k=0; k < dk; ++k) {
			T::UNINTERLEAVE2(vin[2*(0*dk + k) + 0], vin[2*(0*dk + k) + 1], vout[k*8 + 0], vout[k*8 + 1]);
			T::UNINTERLEAVE2(vin[2*(2*dk + k) + 0], vin[2*(2*dk + k) + 1], vout[k*8 + 4], vout[k*8 + 5]);
		}
		unreversed_copy(dk, (v4sf*)(in + N/4), (v4sf*)(out + N - 6*T::SIMD_SZ), -8);
		unreversed_copy(dk, (v4sf*)(in + 3*N/4), (v4sf*)(out + N - 2*T::SIMD_SZ), -8);
		out[T::SIMD_SZ] = x_N;
	} else {
		for (k=0; k < dk; ++k) {
			T::INTERLEAVE2(vin[k*8 + 0], vin[k*8 + 1], vout[2*(0*dk + k) + 0], vout[2*(0*dk + k) + 1]);
			T::INTERLEAVE2(vin[k*8 + 4], vin[k*8 + 5], vout[2*(2*dk + k) + 0], vout[2*(2*dk + k) + 1]);
		}
		reversed_copy(dk, vin+2, 8, (v4sf*)(out + N/2));
		reversed_copy(dk, vin+6, 8, (v4sf*)(out + N));
	}
}




// Standard generic template scalar implementation.
template < typename Real > template < Bool inverse >
void FFTReal<Real>:: zreorder( const Setup *setup, const Real* in, Real* out )
{
	int k, N = setup->N;
	if ( inverse ) {
		Real x_N = in[N];
		for (k=1; k < N-1; ++k) out[k] = in[k+1]; 
		out[0] = in[0];
		out[N-1] = x_N;
	} else {
		Real x_N = in[N-1];
		for (k=N-1; k > 1; --k) out[k] = in[k-1]; 
		out[0] = in[0];
		out[1] = x_N;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT Finalize
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_INLINE void FFTReal<Float32>:: real_finalize_4x4( const v4sf* in0, const v4sf* in1, const v4sf* in, const v4sf* e, v4sf* out )
{
	v4sf r0, i0, r1, i1, r2, i2, r3, i3;
	v4sf sr0, dr0, sr1, dr1, si0, di0, si1, di1;
	r0 = *in0; i0 = *in1;
	r1 = *in++; i1 = *in++; r2 = *in++; i2 = *in++; r3 = *in++; i3 = *in++;
	T::VTRANSPOSE4(r0,r1,r2,r3);
	T::VTRANSPOSE4(i0,i1,i2,i3);
	
	/*
	 transformation for each column is:
	 
	 [1   1   1   1   0   0   0   0]   [r0]
	 [1   0  -1   0   0  -1   0   1]   [r1]
	 [1   0  -1   0   0   1   0  -1]   [r2]
	 [1  -1   1  -1   0   0   0   0]   [r3]
	 [0   0   0   0   1   1   1   1] * [i0]
	 [0  -1   0   1  -1   0   1   0]   [i1]
	 [0  -1   0   1   1   0  -1   0]   [i2]
	 [0   0   0   0  -1   1  -1   1]   [i3]    
	 */
	VCPLXMUL(r1,i1,e[0],e[1]);
	VCPLXMUL(r2,i2,e[2],e[3]);
	VCPLXMUL(r3,i3,e[4],e[5]);
	
	sr0 = T::VADD(r0,r2); dr0 = T::VSUB(r0,r2); 
	sr1 = T::VADD(r1,r3); dr1 = T::VSUB(r3,r1);
	si0 = T::VADD(i0,i2); di0 = T::VSUB(i0,i2); 
	si1 = T::VADD(i1,i3); di1 = T::VSUB(i3,i1);
	
	r0 = T::VADD(sr0, sr1);
	r3 = T::VSUB(sr0, sr1);
	i0 = T::VADD(si0, si1);
	i3 = T::VSUB(si1, si0);
	r1 = T::VADD(dr0, di1);
	r2 = T::VSUB(dr0, di1);
	i1 = T::VSUB(dr1, di0);
	i2 = T::VADD(dr1, di0);
	
	*out++ = r0;
	*out++ = i0;
	*out++ = r1;
	*out++ = i1;
	*out++ = r2;
	*out++ = i2;
	*out++ = r3;
	*out++ = i3;
}




template <>
OM_INLINE void FFTReal<Float32>:: real_finalize( int Ncvec, const v4sf *in, v4sf *out, const v4sf *e )
{
	int k, dk = Ncvec/T::SIMD_SZ; // number of 4x4 matrix blocks
	/* fftpack order is f0r f1r f1i f2r f2i ... f(n-1)r f(n-1)i f(n)r */
	
	v4sf cr, ci, *uout = (v4sf*)out;
	v4sf save = in[7], zero = T::VZERO();
	Float32 xr0, xi0, xr1, xi1, xr2, xi2, xr3, xi3;
	static const Float32 s = Float32(math::sqrt(2.0f)/2);
	
	cr = in[0]; ci = in[Ncvec*2-1];
	OM_DEBUG_ASSERT(in != out);
	real_finalize_4x4(&zero, &zero, in+1, e, out);
	
	/*
	 [cr0 cr1 cr2 cr3 ci0 ci1 ci2 ci3]
	 
	 [Xr(1)]  ] [1   1   1   1   0   0   0   0]
	 [Xr(N/4) ] [0   0   0   0   1   s   0  -s]
	 [Xr(N/2) ] [1   0  -1   0   0   0   0   0]
	 [Xr(3N/4)] [0   0   0   0   1  -s   0   s]
	 [Xi(1)   ] [1  -1   1  -1   0   0   0   0]
	 [Xi(N/4) ] [0   0   0   0   0  -s  -1  -s]
	 [Xi(N/2) ] [0  -1   0   1   0   0   0   0]
	 [Xi(3N/4)] [0   0   0   0   0  -s   1  -s]
	 */
	
	xr0=(cr[0]+cr[2]) + (cr[1]+cr[3]); uout[0][0] = xr0;
	xi0=(cr[0]+cr[2]) - (cr[1]+cr[3]); uout[1][0] = xi0;
	xr2=(cr[0]-cr[2]);                     uout[4][0] = xr2;
	xi2=(cr[3]-cr[1]);                     uout[5][0] = xi2;
	xr1= ci[0] + s*(ci[1]-ci[3]);        uout[2][0] = xr1;
	xi1=-ci[2] - s*(ci[1]+ci[3]);        uout[3][0] = xi1;
	xr3= ci[0] - s*(ci[1]-ci[3]);        uout[6][0] = xr3;
	xi3= ci[2] - s*(ci[1]+ci[3]);        uout[7][0] = xi3; 
	
	for (k=1; k < dk; ++k) {
		v4sf save_next = in[8*k+7];
		real_finalize_4x4(&save, &in[8*k+0], in + 8*k+1,
								e + k*6, out + k*8);
		save = save_next;
	}
	
}




template <>
OM_INLINE void FFTReal<Float32>:: real_preprocess_4x4( const v4sf *in, const v4sf *e, v4sf* out, int first )
{
	v4sf r0=in[0], i0=in[1], r1=in[2], i1=in[3], r2=in[4], i2=in[5], r3=in[6], i3=in[7];
	/*
	 transformation for each column is:
	 
	 [1   1   1   1   0   0   0   0]   [r0]
	 [1   0   0  -1   0  -1  -1   0]   [r1]
	 [1  -1  -1   1   0   0   0   0]   [r2]
	 [1   0   0  -1   0   1   1   0]   [r3]
	 [0   0   0   0   1  -1   1  -1] * [i0]
	 [0  -1   1   0   1   0   0   1]   [i1]
	 [0   0   0   0   1   1  -1  -1]   [i2]
	 [0   1  -1   0   1   0   0   1]   [i3]    
	 */
	v4sf sr0 = T::VADD(r0,r3), dr0 = T::VSUB(r0,r3); 
	v4sf sr1 = T::VADD(r1,r2), dr1 = T::VSUB(r1,r2);
	v4sf si0 = T::VADD(i0,i3), di0 = T::VSUB(i0,i3); 
	v4sf si1 = T::VADD(i1,i2), di1 = T::VSUB(i1,i2);
	
	r0 = T::VADD(sr0, sr1);
	r2 = T::VSUB(sr0, sr1);
	r1 = T::VSUB(dr0, si1);
	r3 = T::VADD(dr0, si1);
	i0 = T::VSUB(di0, di1);
	i2 = T::VADD(di0, di1);
	i1 = T::VSUB(si0, dr1);
	i3 = T::VADD(si0, dr1);
	
	VCPLXMULCONJ(r1,i1,e[0],e[1]);
	VCPLXMULCONJ(r2,i2,e[2],e[3]);
	VCPLXMULCONJ(r3,i3,e[4],e[5]);
	
	T::VTRANSPOSE4(r0,r1,r2,r3);
	T::VTRANSPOSE4(i0,i1,i2,i3);
	
	if (!first) {
		*out++ = r0;
		*out++ = i0;
	}
	*out++ = r1;
	*out++ = i1;
	*out++ = r2;
	*out++ = i2;
	*out++ = r3;
	*out++ = i3;
}




template <>
OM_INLINE void FFTReal<Float32>:: real_preprocess(int Ncvec, const v4sf *in, v4sf *out, const v4sf *e)
{
	int k, dk = Ncvec/T::SIMD_SZ; // number of 4x4 matrix blocks
	/* fftpack order is f0r f1r f1i f2r f2i ... f(n-1)r f(n-1)i f(n)r */
	
	v4sf Xr, Xi, *uout = (v4sf*)out;
	Float32 cr0, ci0, cr1, ci1, cr2, ci2, cr3, ci3;
	static const Float32 s = Float32(math::sqrt(2.0f));
	OM_DEBUG_ASSERT(in != out);
	for (k=0; k < 4; ++k) {
		Xr[k] = ((Float32*)in)[8*k];
		Xi[k] = ((Float32*)in)[8*k+4];
	}
	
	real_preprocess_4x4(in, e, out+1, 1); // will write only 6 values
	
	/*
	 [Xr0 Xr1 Xr2 Xr3 Xi0 Xi1 Xi2 Xi3]
	 
	 [cr0] [1   0   2   0   1   0   0   0]
	 [cr1] [1   0   0   0  -1   0  -2   0]
	 [cr2] [1   0  -2   0   1   0   0   0]
	 [cr3] [1   0   0   0  -1   0   2   0]
	 [ci0] [0   2   0   2   0   0   0   0]
	 [ci1] [0   s   0  -s   0  -s   0  -s]
	 [ci2] [0   0   0   0   0  -2   0   2]
	 [ci3] [0  -s   0   s   0  -s   0  -s]
	 */
	for (k=1; k < dk; ++k) {    
		real_preprocess_4x4(in+8*k, e + k*6, out-1+k*8, 0);
	}
	
	cr0=(Xr[0]+Xi[0]) + 2*Xr[2]; uout[0][0] = cr0;
	cr1=(Xr[0]-Xi[0]) - 2*Xi[2]; uout[0][1] = cr1;
	cr2=(Xr[0]+Xi[0]) - 2*Xr[2]; uout[0][2] = cr2;
	cr3=(Xr[0]-Xi[0]) + 2*Xi[2]; uout[0][3] = cr3;
	ci0= 2*(Xr[1]+Xr[3]);                       uout[2*Ncvec-1][0] = ci0;
	ci1= s*(Xr[1]-Xr[3]) - s*(Xi[1]+Xi[3]); uout[2*Ncvec-1][1] = ci1;
	ci2= 2*(Xi[3]-Xi[1]);                       uout[2*Ncvec-1][2] = ci2;
	ci3=-s*(Xr[1]-Xr[3]) - s*(Xi[1]+Xi[3]); uout[2*Ncvec-1][3] = ci3;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT
//############		
//##########################################################################################
//##########################################################################################




// Specialization for 32-bit floats using SIMD.
template <> template < Bool inverse >
void FFTReal<Float32>:: transform_internal( const Setup *setup, const Float32* finput, Float32* foutput, v4sf* scratch, int ordered )
{
	int k, Ncvec   = setup->Ncvec, N = setup->N;
	int nf_odd = (setup->ifac[1] & 1);
	
	// temporary buffer is allocated on the stack if the scratch pointer is NULL
	int stack_allocate = (scratch == 0 ? Ncvec*2 : 1);
	VLA_ARRAY_ON_STACK(v4sf, scratch_on_stack, stack_allocate);
	
	const v4sf *vinput = (const v4sf*)finput;
	v4sf *voutput      = (v4sf*)foutput;
	v4sf *buff[2]      = { voutput, scratch ? scratch : scratch_on_stack };
	int ib = (nf_odd ^ ordered ? 1 : 0);
	
	//OM_DEBUG_ASSERT(finput != foutput);
	if ( inverse )
	{
		if (vinput == buff[ib]) { 
			ib = !ib; // may happen when finput == foutput
		}
		if (ordered) {
			zreorder<inverse>(setup, (Float32*)vinput, (Float32*)buff[ib] ); 
			vinput = buff[ib]; ib = !ib;
		}
		
		real_preprocess(Ncvec, vinput, buff[ib], (v4sf*)setup->e);
		ib = (rfftb1_ps(Ncvec*2, buff[ib], buff[0], buff[1], 
						setup->twiddle, &setup->ifac[0]) == buff[0] ? 0 : 1);
	}
	else
	{
		ib = !ib;
		ib = (rfftf1_ps(Ncvec*2, vinput, buff[ib], buff[!ib],
						setup->twiddle, &setup->ifac[0]) == buff[0] ? 0 : 1);      
		real_finalize(Ncvec, buff[ib], buff[!ib], (v4sf*)setup->e);
		
		if (ordered) {
			zreorder<inverse>(setup, (Float32*)buff[!ib], (Float32*)buff[ib] );       
		} else ib = !ib;
	}
	
	if (buff[ib] != voutput) {
		/* extra copy required -- this situation should only happen when finput == foutput */
		OM_DEBUG_ASSERT(finput==foutput);
		for (k=0; k < Ncvec; ++k) {
			v4sf a = buff[ib][2*k], b = buff[ib][2*k+1];
			voutput[2*k] = a; voutput[2*k+1] = b;
		}
		ib = !ib;
	}
	
	if ( !inverse && ordered )
	{
		foutput[N] = foutput[1];
		foutput[N+1] = 0.0f;
		foutput[1] = 0.0f;
	}
	
	OM_DEBUG_ASSERT(buff[ib] == voutput);
}




// Standard generic template scalar implementation.
template < typename Real > template < Bool inverse >
void FFTReal<Real>:: transform_internal( const Setup* setup, const Real* input, Real* output, v4sf* scratch, int ordered )
{
	int Ncvec   = setup->Ncvec;
	int nf_odd = (setup->ifac[1] & 1);
	
	// temporary buffer is allocated on the stack if the scratch pointer is NULL
	int stack_allocate = (scratch == 0 ? Ncvec*2 : 1);
	VLA_ARRAY_ON_STACK(v4sf, scratch_on_stack, stack_allocate);
	Real* buff[2];
	int ib;
	if (scratch == 0) scratch = scratch_on_stack;
	buff[0] = output; buff[1] = scratch;
	ib = (nf_odd ^ ordered ? 1 : 0);
	
	if ( inverse )
	{
		if (input == buff[ib]) {
			ib = !ib; // may happen when finput == foutput
		}
		if (ordered) {
			zreorder<inverse>(setup, input, buff[!ib] ); 
			input = buff[!ib];
		}
		
		ib = (rfftb1_ps(Ncvec*2, input, buff[ib], buff[!ib], 
							setup->twiddle, &setup->ifac[0]) == buff[0] ? 0 : 1);
	}
	else
	{
		ib = (rfftf1_ps(Ncvec*2, input, buff[ib], buff[!ib],
						setup->twiddle, &setup->ifac[0]) == buff[0] ? 0 : 1);
		if (ordered) {
			zreorder<inverse>(setup, buff[ib], buff[!ib] ); ib = !ib;
		}
	}
	
	if (buff[ib] != output) {
		int k;
		// extra copy required -- this situation should happens only when finput == foutput
		OM_DEBUG_ASSERT(input==output);
		for (k=0; k < Ncvec; ++k) {
			Real a = buff[ib][2*k], b = buff[ib][2*k+1];
			output[2*k] = a; output[2*k+1] = b;
		}
		ib = !ib;
	}
	
	if ( !inverse && ordered )
	{
		output[2*Ncvec] = output[1];
		output[2*Ncvec+1] = Real(Scalar(0));
		output[1] = Real(Scalar(0));
	}
	
	OM_DEBUG_ASSERT(buff[ib] == output);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Unordered Multiply-Add
//############		
//##########################################################################################
//##########################################################################################




// Specialization for 32-bit floats using SIMD.
template <>
OM_INLINE void FFTReal<Float32>:: zconvolve_accumulate( const Setup* s, const Float32* a, const Float32* b, Float32* ab )
{
	int Ncvec = s->Ncvec;
	const v4sf* OM_RESTRICT va = (const v4sf*)a;
	const v4sf* OM_RESTRICT vb = (const v4sf*)b;
	v4sf* OM_RESTRICT vab = (v4sf*)ab;
	
#ifdef __arm__
	__builtin_prefetch(va);
	__builtin_prefetch(vb);
	__builtin_prefetch(vab);
	__builtin_prefetch(va+2);
	__builtin_prefetch(vb+2);
	__builtin_prefetch(vab+2);
	__builtin_prefetch(va+4);
	__builtin_prefetch(vb+4);
	__builtin_prefetch(vab+4);
	__builtin_prefetch(va+6);
	__builtin_prefetch(vb+6);
	__builtin_prefetch(vab+6);
#endif
	
	Float32 ar, ai, br, bi, abr, abi;
	int i;
	
	//OM_DEBUG_ASSERT(VALIGNED(a) && VALIGNED(b) && VALIGNED(ab));
	ar = ((v4sf*)va)[0][0];
	ai = ((v4sf*)va)[1][0];
	br = ((v4sf*)vb)[0][0];
	bi = ((v4sf*)vb)[1][0];
	abr = ((v4sf*)vab)[0][0];
	abi = ((v4sf*)vab)[1][0];
	
	for (i=0; i < Ncvec; i += 2) {
		v4sf ar, ai, br, bi;
		ar = va[2*i+0]; ai = va[2*i+1];
		br = vb[2*i+0]; bi = vb[2*i+1];
		VCPLXMUL(ar, ai, br, bi);
		vab[2*i+0] = T::VADD(ar, vab[2*i+0]);
		vab[2*i+1] = T::VADD(ai, vab[2*i+1]);
		ar = va[2*i+2]; ai = va[2*i+3];
		br = vb[2*i+2]; bi = vb[2*i+3];
		VCPLXMUL(ar, ai, br, bi);
		vab[2*i+2] = T::VADD(ar, vab[2*i+2]);
		vab[2*i+3] = T::VADD(ai, vab[2*i+3]);
	}
	
	((v4sf*)vab)[0][0] = abr + ar*br;
	((v4sf*)vab)[1][0] = abi + ai*bi;
}




// Standard generic template scalar implementation.
template < typename Real >
void FFTReal<Real>:: zconvolve_accumulate( const Setup* s, const Real* a, const Real* b, Real* ab )
{
	int i, Ncvec = s->Ncvec;
	
	// take care of the fftpack ordering
	ab[0] += a[0]*b[0];
	ab[2*Ncvec-1] += a[2*Ncvec-1]*b[2*Ncvec-1];
	++ab; ++a; ++b; --Ncvec;
	
	for (i=0; i < Ncvec; ++i) {
		Real ar, ai, br, bi;
		ar = a[2*i+0]; ai = a[2*i+1];
		br = b[2*i+0]; bi = b[2*i+1];
		VCPLXMUL(ar, ai, br, bi);
		ab[2*i+0] += ar;
		ab[2*i+1] += ai;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT Setup
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
int FFTReal<Real>:: decompose( int n, int* ifac, const int* ntryh )
{
	int nl = n, nf = 0, i, j = 0;
	for (j=0; ntryh[j]; ++j) {
		int ntry = ntryh[j];
		while (nl != 1) {
			int nq = nl / ntry;
			int nr = nl - ntry * nq;
			if (nr == 0) {
				ifac[2+nf++] = ntry;
				nl = nq;
				if (ntry == 2 && nf != 1) {
					for (i = 2; i <= nf; ++i) {
						int ib = nf - i + 2;
						ifac[ib + 1] = ifac[ib];
					}
					ifac[2] = 2;
				}
			} else break;
		}
	}
	ifac[0] = n;
	ifac[1] = nf;  
	return nf;
}




template < typename Real >
void FFTReal<Real>:: rffti1_ps( int n, Scalar* wa, int* ifac )
{
	static const int ntryh[] = { 4,2,3,5,0 };
	int k1, j, ii;
	
	int nf = decompose(n,ifac,ntryh);
	Scalar argh = (2*math::pi<Scalar>()) / n;
	int is = 0;
	int nfm1 = nf - 1;
	int l1 = 1;
	for (k1 = 1; k1 <= nfm1; k1++) {
		int ip = ifac[k1 + 1];
		int ld = 0;
		int l2 = l1*ip;
		int ido = n / l2;
		int ipm = ip - 1;
		for (j = 1; j <= ipm; ++j) {
			Scalar argld;
			int i = is, fi=0;
			ld += l1;
			argld = ld*argh;
			for (ii = 3; ii <= ido; ii += 2) {
				i += 2;
				fi += 1;
				wa[i - 2] = math::cos(fi*argld);
				wa[i - 1] = math::sin(fi*argld);
			}
			is += ido;
		}
		l1 = l2;
	}
} /* rffti1 */




template < typename Real >
void FFTReal<Real>:: new_setup( int N, Setup* s )
{
	int k, m;
	/* unfortunately, the fft size must be a multiple of 16 for complex FFTs 
     and 32 for real FFTs -- a lot of stuff would need to be rewritten to
     handle other cases (or maybe just switch to a scalar fft, I don't know..) */
	OM_DEBUG_ASSERT((N%(2*T::SIMD_SZ*T::SIMD_SZ))==0 && N>0);
	//OM_DEBUG_ASSERT((N % 32) == 0);
	s->N = N;
	/* nb of complex simd vectors */
	s->Ncvec = (N/2)/T::SIMD_SZ;
	s->data = util::allocateAligned<v4sf>( 2*s->Ncvec, 16 );
	s->e = (Scalar*)s->data;
	s->twiddle = (Scalar*)(s->data + (2*s->Ncvec*(T::SIMD_SZ-1))/T::SIMD_SZ);  
	
	for (k=0; k < s->Ncvec; ++k) {
		int i = k/T::SIMD_SZ;
		int j = k%T::SIMD_SZ;
		for (m=0; m < T::SIMD_SZ-1; ++m) {
			Scalar A = -2*math::pi<Scalar>()*(m+1)*k / N;
			s->e[(2*(i*3 + m) + 0) * T::SIMD_SZ + j] = math::cos(A);
			s->e[(2*(i*3 + m) + 1) * T::SIMD_SZ + j] = math::sin(A);
		}
	}
	rffti1_ps(N/T::SIMD_SZ, s->twiddle, s->ifac);
	
	/* check that N is decomposable with allowed prime factors */
	for (k=0, m=1; k < s->ifac[1]; ++k) { m *= s->ifac[2+k]; }
	if (m != N/T::SIMD_SZ)
	{
		//destroy_setup(s); s = 0;
	}
}




template < typename Real >
void FFTReal<Real>:: destroy_setup( Setup* s )
{
	util::deallocateAligned( s->data );
}




template < typename Real >
void FFTReal<Real>:: copy_setup( const Setup* s1, Setup* s2 )
{
	*s2 = *s1;
	s2->data = util::allocateAligned<v4sf>( s1->Ncvec*2, 16 );
	util::copyPOD( s2->data, s1->data, s1->Ncvec*2 );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FFT_REAL_H
