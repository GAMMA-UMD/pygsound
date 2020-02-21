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

#ifndef INCLUDE_OM_FFT_COMPLEX_H
#define INCLUDE_OM_FFT_COMPLEX_H


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
class FFTComplex
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new FFT with the specified size.
			OM_INLINE FFTComplex( Size fftSize )
			{
				new_setup( (int)fftSize, &setup );
			}
			
			
			/// Create a copy of another FFT.
			OM_INLINE FFTComplex( const FFTComplex& other )
			{
				copySetup( &other.setup, &setup );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an FFT.
			OM_INLINE ~FFTComplex()
			{
				destroy_setup( &setup );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another FFT object to this one.
			OM_INLINE FFTComplex& operator = ( const FFTComplex& other )
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
		//******	Complex FFT Methods
			
			
			/// Convert N complex values to N complex values using the forward transform.
			OM_INLINE void fft( Complex<Real>* inOut, Complex<Real>* workspace = NULL ) const
			{
				transform_internal<false>( &setup, (const Real*)inOut, (Real*)inOut, (v4sf*)workspace, 1 );
			}
			
			
			/// Convert N complex values to N complex values using the forward transform.
			OM_INLINE void fft( const Complex<Real>* input, Complex<Real>* output, Complex<Real>* workspace ) const
			{
				transform_internal<false>( &setup, (const Real*)input, (Real*)output, (v4sf*)workspace, 1 );
			}
			
			
			/// Convert N complex values to N complex values using the reverse transform.
			OM_INLINE void ifft( Complex<Real>* inOut, Complex<Real>* workspace = NULL ) const
			{
				transform_internal<true>( &setup, (const Real*)inOut, (Real*)inOut, (v4sf*)workspace, 1 );
			}
			
			
			/// Convert N complex values to N complex values using the reverse transform.
			OM_INLINE void ifft( const Complex<Real>* input, Complex<Real>* output, Complex<Real>* workspace ) const
			{
				transform_internal<true>( &setup, (const Real*)input, (Real*)output, (v4sf*)workspace, 1 );
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
			
			
			OM_NO_INLINE static void passf2_ps( int ido, int l1, const v4sf* cc, v4sf* ch, const Scalar* wa1, Scalar fsign );
			OM_NO_INLINE static void passf3_ps( int ido, int l1, const v4sf* cc, v4sf* ch,
												const Scalar* wa1, const Scalar* wa2, Scalar fsign );
			OM_NO_INLINE static void passf4_ps( int ido, int l1, const v4sf* cc, v4sf* ch,
												const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, Scalar fsign );
			OM_NO_INLINE static void passf5_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, 
												const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, const Scalar* wa4, Scalar fsign );
			
			OM_INLINE static v4sf* cfftf1_ps( int n, const v4sf* input_readonly, v4sf* work1, v4sf* work2, 
												const Scalar* wa, const int* ifac, Scalar fsign );
			
			
			// Generic version does nothing because these are only needed for the SIMD code.
			OM_INLINE static void reversed_copy( int N, const v4sf* in, int in_stride, v4sf* out ) {}
			OM_INLINE static void unreversed_copy( int N, const v4sf* in, v4sf* out, int out_stride ) {}
			
			
			template < Bool inverse >
			OM_INLINE static void zreorder( const Setup* setup, const Real* in, Real* out );
			
			// Generic version does nothing because these are only needed for the SIMD code.
			OM_FORCE_INLINE static void real_finalize_4x4( const v4sf* in0, const v4sf* in1, const v4sf* in, const v4sf* e, v4sf* out ) {}
			OM_NO_INLINE static void cplx_finalize( int Ncvec, const v4sf *in, v4sf *out, const v4sf *e ) {}
			OM_FORCE_INLINE static void real_preprocess_4x4( const v4sf *in, const v4sf *e, v4sf *out, int first ) {}
			OM_NO_INLINE static void cplx_preprocess(int Ncvec, const v4sf *in, v4sf *out, const v4sf *e) {}
			
			template < Bool inverse >
			OM_INLINE static void transform_internal( const Setup* setup, const Real* finput, Real* foutput, v4sf* scratch, int ordered );
			
			
			OM_INLINE static int decompose( int n, int* ifac, const int* ntryh );
			OM_INLINE static void cffti1_ps( int n, Scalar* wa, int* ifac );
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
//############		Complex FFT 2
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTComplex<Real>:: passf2_ps( int ido, int l1, const v4sf* cc, v4sf* ch, const Scalar* wa1, Scalar fsign )
{
	int k, i;
	int l1ido = l1*ido;
	if (ido <= 2) {
		for (k=0; k < l1ido; k += ido, ch += ido, cc+= 2*ido) {
			ch[0]         = T::VADD(cc[0], cc[ido+0]);
			ch[l1ido]     = T::VSUB(cc[0], cc[ido+0]);
			ch[1]         = T::VADD(cc[1], cc[ido+1]);
			ch[l1ido + 1] = T::VSUB(cc[1], cc[ido+1]);
		}
	} else {
		for (k=0; k < l1ido; k += ido, ch += ido, cc += 2*ido) {
			for (i=0; i<ido-1; i+=2) {
				v4sf tr2 = T::VSUB(cc[i+0], cc[i+ido+0]);
				v4sf ti2 = T::VSUB(cc[i+1], cc[i+ido+1]);
				v4sf wr = T::LD_PS1(wa1[i]), wi = T::VMUL(T::LD_PS1(fsign), T::LD_PS1(wa1[i+1]));
				ch[i]   = T::VADD(cc[i+0], cc[i+ido+0]);
				ch[i+1] = T::VADD(cc[i+1], cc[i+ido+1]);
				VCPLXMUL(tr2, ti2, wr, wi);
				ch[i+l1ido]   = tr2;
				ch[i+l1ido+1] = ti2;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Complex FFT 3
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTComplex<Real>:: passf3_ps( int ido, int l1, const v4sf* cc, v4sf* ch,
									const Scalar* wa1, const Scalar* wa2, Scalar fsign )
{
	static const Scalar taur = Scalar(-0.5f);
	const Scalar taui = Scalar(0.866025403784439)*fsign;
	int i, k;
	v4sf tr2, ti2, cr2, ci2, cr3, ci3, dr2, di2, dr3, di3;
	int l1ido = l1*ido;
	Scalar wr1, wi1, wr2, wi2;
	OM_DEBUG_ASSERT(ido > 2);
	for (k=0; k< l1ido; k += ido, cc+= 3*ido, ch +=ido) {
		for (i=0; i<ido-1; i+=2) {
			tr2 = T::VADD(cc[i+ido], cc[i+2*ido]);
			cr2 = T::VADD(cc[i], T::SVMUL(taur,tr2));
			ch[i]    = T::VADD(cc[i], tr2);
			ti2 = T::VADD(cc[i+ido+1], cc[i+2*ido+1]);
			ci2 = T::VADD(cc[i    +1], T::SVMUL(taur,ti2));
			ch[i+1]  = T::VADD(cc[i+1], ti2);
			cr3 = T::SVMUL(taui, T::VSUB(cc[i+ido], cc[i+2*ido]));
			ci3 = T::SVMUL(taui, T::VSUB(cc[i+ido+1], cc[i+2*ido+1]));
			dr2 = T::VSUB(cr2, ci3);
			dr3 = T::VADD(cr2, ci3);
			di2 = T::VADD(ci2, cr3);
			di3 = T::VSUB(ci2, cr3);
			wr1=wa1[i], wi1=fsign*wa1[i+1], wr2=wa2[i], wi2=fsign*wa2[i+1]; 
			VCPLXMUL(dr2, di2, T::LD_PS1(wr1), T::LD_PS1(wi1));
			ch[i+l1ido] = dr2; 
			ch[i+l1ido + 1] = di2;
			VCPLXMUL(dr3, di3, T::LD_PS1(wr2), T::LD_PS1(wi2));
			ch[i+2*l1ido] = dr3;
			ch[i+2*l1ido+1] = di3;
		}
	}
} /* radf3 */




//##########################################################################################
//##########################################################################################
//############		
//############		Complex FFT 4
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTComplex<Real>:: passf4_ps( int ido, int l1, const v4sf* cc, v4sf* ch,
                                   const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, Scalar fsign )
{
	/* isign == -1 for forward transform and +1 for backward transform */
	int i, k;
	v4sf ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, tr2, tr3, tr4;
	int l1ido = l1*ido;
	if (ido == 2) {
		for (k=0; k < l1ido; k += ido, ch += ido, cc += 4*ido) {
			tr1 = T::VSUB(cc[0], cc[2*ido + 0]);
			tr2 = T::VADD(cc[0], cc[2*ido + 0]);
			ti1 = T::VSUB(cc[1], cc[2*ido + 1]);
			ti2 = T::VADD(cc[1], cc[2*ido + 1]);
			ti4 = T::VMUL(T::VSUB(cc[1*ido + 0], cc[3*ido + 0]), T::LD_PS1(fsign));
			tr4 = T::VMUL(T::VSUB(cc[3*ido + 1], cc[1*ido + 1]), T::LD_PS1(fsign));
			tr3 = T::VADD(cc[ido + 0], cc[3*ido + 0]);
			ti3 = T::VADD(cc[ido + 1], cc[3*ido + 1]);
			
			ch[0*l1ido + 0] = T::VADD(tr2, tr3);
			ch[0*l1ido + 1] = T::VADD(ti2, ti3);
			ch[1*l1ido + 0] = T::VADD(tr1, tr4);
			ch[1*l1ido + 1] = T::VADD(ti1, ti4);
			ch[2*l1ido + 0] = T::VSUB(tr2, tr3);
			ch[2*l1ido + 1] = T::VSUB(ti2, ti3);        
			ch[3*l1ido + 0] = T::VSUB(tr1, tr4);
			ch[3*l1ido + 1] = T::VSUB(ti1, ti4);
		}
	} else {
		for (k=0; k < l1ido; k += ido, ch+=ido, cc += 4*ido) {
			for (i=0; i<ido-1; i+=2) {
				Scalar wr1, wi1, wr2, wi2, wr3, wi3;
				tr1 = T::VSUB(cc[i + 0], cc[i + 2*ido + 0]);
				tr2 = T::VADD(cc[i + 0], cc[i + 2*ido + 0]);
				ti1 = T::VSUB(cc[i + 1], cc[i + 2*ido + 1]);
				ti2 = T::VADD(cc[i + 1], cc[i + 2*ido + 1]);
				tr4 = T::VMUL(T::VSUB(cc[i + 3*ido + 1], cc[i + 1*ido + 1]), T::LD_PS1(fsign));
				ti4 = T::VMUL(T::VSUB(cc[i + 1*ido + 0], cc[i + 3*ido + 0]), T::LD_PS1(fsign));
				tr3 = T::VADD(cc[i + ido + 0], cc[i + 3*ido + 0]);
				ti3 = T::VADD(cc[i + ido + 1], cc[i + 3*ido + 1]);
				
				ch[i] = T::VADD(tr2, tr3);
				cr3    = T::VSUB(tr2, tr3);
				ch[i + 1] = T::VADD(ti2, ti3);
				ci3 = T::VSUB(ti2, ti3);
				
				cr2 = T::VADD(tr1, tr4);
				cr4 = T::VSUB(tr1, tr4);
				ci2 = T::VADD(ti1, ti4);
				ci4 = T::VSUB(ti1, ti4);
				wr1=wa1[i], wi1=fsign*wa1[i+1];
				VCPLXMUL(cr2, ci2, T::LD_PS1(wr1), T::LD_PS1(wi1));
				wr2=wa2[i], wi2=fsign*wa2[i+1]; 
				ch[i + l1ido] = cr2;
				ch[i + l1ido + 1] = ci2;
				
				VCPLXMUL(cr3, ci3, T::LD_PS1(wr2), T::LD_PS1(wi2));
				wr3=wa3[i], wi3=fsign*wa3[i+1]; 
				ch[i + 2*l1ido] = cr3;
				ch[i + 2*l1ido + 1] = ci3;
				
				VCPLXMUL(cr4, ci4, T::LD_PS1(wr3), T::LD_PS1(wi3));
				ch[i + 3*l1ido] = cr4;
				ch[i + 3*l1ido + 1] = ci4;
			}
		}
	}
} /* passf4 */




//##########################################################################################
//##########################################################################################
//############		
//############		Complex FFT 5
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
void FFTComplex<Real>:: passf5_ps( int ido, int l1, const v4sf* OM_RESTRICT cc, v4sf* OM_RESTRICT ch, 
								const Scalar* wa1, const Scalar* wa2, const Scalar* wa3, const Scalar* wa4, Scalar fsign )
{
	static const Scalar tr11 = Scalar(0.309016994374947);
	const Scalar ti11 = Scalar(0.951056516295154)*fsign;
	static const Scalar tr12 = Scalar(-0.809016994374947);
	const Scalar ti12 = Scalar(0.587785252292473)*fsign;
	
	/* Local variables */
	int i, k;
	v4sf ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, cr4, ti2, ti3,
		ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;
	Scalar wr1, wi1, wr2, wi2, wr3, wi3, wr4, wi4;
	
#define cc_ref(a_1,a_2) cc[(a_2-1)*ido + a_1 + 1]
#define ch_ref(a_1,a_3) ch[(a_3-1)*l1*ido + a_1 + 1]
	
	OM_DEBUG_ASSERT(ido > 2);
	for (k = 0; k < l1; ++k, cc += 5*ido, ch += ido) {
		for (i = 0; i < ido-1; i += 2) {
			ti5 = T::VSUB(cc_ref(i  , 2), cc_ref(i  , 5));
			ti2 = T::VADD(cc_ref(i  , 2), cc_ref(i  , 5));
			ti4 = T::VSUB(cc_ref(i  , 3), cc_ref(i  , 4));
			ti3 = T::VADD(cc_ref(i  , 3), cc_ref(i  , 4));
			tr5 = T::VSUB(cc_ref(i-1, 2), cc_ref(i-1, 5));
			tr2 = T::VADD(cc_ref(i-1, 2), cc_ref(i-1, 5));
			tr4 = T::VSUB(cc_ref(i-1, 3), cc_ref(i-1, 4));
			tr3 = T::VADD(cc_ref(i-1, 3), cc_ref(i-1, 4));
			ch_ref(i-1, 1) = T::VADD(cc_ref(i-1, 1), T::VADD(tr2, tr3));
			ch_ref(i  , 1) = T::VADD(cc_ref(i  , 1), T::VADD(ti2, ti3));
			cr2 = T::VADD(cc_ref(i-1, 1), T::VADD(T::SVMUL(tr11, tr2),T::SVMUL(tr12, tr3)));
			ci2 = T::VADD(cc_ref(i  , 1), T::VADD(T::SVMUL(tr11, ti2),T::SVMUL(tr12, ti3)));
			cr3 = T::VADD(cc_ref(i-1, 1), T::VADD(T::SVMUL(tr12, tr2),T::SVMUL(tr11, tr3)));
			ci3 = T::VADD(cc_ref(i  , 1), T::VADD(T::SVMUL(tr12, ti2),T::SVMUL(tr11, ti3)));
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
			wr1=wa1[i], wi1=fsign*wa1[i+1], wr2=wa2[i], wi2=fsign*wa2[i+1]; 
			wr3=wa3[i], wi3=fsign*wa3[i+1], wr4=wa4[i], wi4=fsign*wa4[i+1]; 
			VCPLXMUL(dr2, di2, T::LD_PS1(wr1), T::LD_PS1(wi1));
			ch_ref(i - 1, 2) = dr2;
			ch_ref(i, 2)     = di2;
			VCPLXMUL(dr3, di3, T::LD_PS1(wr2), T::LD_PS1(wi2));
			ch_ref(i - 1, 3) = dr3;
			ch_ref(i, 3)     = di3;
			VCPLXMUL(dr4, di4, T::LD_PS1(wr3), T::LD_PS1(wi3));
			ch_ref(i - 1, 4) = dr4;
			ch_ref(i, 4)     = di4;
			VCPLXMUL(dr5, di5, T::LD_PS1(wr4), T::LD_PS1(wi4));
			ch_ref(i - 1, 5) = dr5;
			ch_ref(i, 5)     = di5;
		}
	}
#undef ch_ref
#undef cc_ref
}




//##########################################################################################
//##########################################################################################
//############		
//############		Complex FFT
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
typename FFTComplex<Real>::v4sf* FFTComplex<Real>:: cfftf1_ps( int n, const v4sf* input_readonly, v4sf* work1, v4sf* work2, 
														const Scalar* wa, const int* ifac, Scalar fsign )
{  
	v4sf *in  = (v4sf*)input_readonly;
	v4sf *out = (in == work2 ? work1 : work2); 
	int nf = ifac[1], k1;
	int l1 = 1;
	int iw = 0;
	OM_DEBUG_ASSERT(in != out && work1 != work2);
	for (k1=2; k1<=nf+1; k1++) {
		int ip = ifac[k1];
		int l2 = ip*l1;
		int ido = n / l2;
		int idot = ido + ido;
		switch (ip) {
			case 5: {
				int ix2 = iw + idot;
				int ix3 = ix2 + idot;
				int ix4 = ix3 + idot;
				passf5_ps(idot, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4], fsign);
			} break;
			case 4: {
				int ix2 = iw + idot;
				int ix3 = ix2 + idot;
				passf4_ps(idot, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], fsign);
			} break;
			case 2: {
				passf2_ps(idot, l1, in, out, &wa[iw], fsign);
			} break;
			case 3: {
				int ix2 = iw + idot;
				passf3_ps(idot, l1, in, out, &wa[iw], &wa[ix2], fsign);
			} break;
			default:
				OM_DEBUG_ASSERT(0);
		}
		l1 = l2;
		iw += (ip - 1)*idot;
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
OM_INLINE void FFTComplex<Float32>:: reversed_copy( int N, const v4sf* in, int in_stride, v4sf* out )
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
OM_INLINE void FFTComplex<Float32>:: unreversed_copy( int N, const v4sf *in, v4sf *out, int out_stride )
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
void FFTComplex<Float32>:: zreorder( const Setup* setup, const Float32* in, Float32* out )
{
	int k, Ncvec = setup->Ncvec;
	const v4sf *vin = (const v4sf*)in;
	v4sf *vout = (v4sf*)out;
	OM_DEBUG_ASSERT(in != out);
	
	if ( inverse ) {
		for (k=0; k < Ncvec; ++k) { 
			int kk = (k/4) + (k%4)*(Ncvec/4);
			T::UNINTERLEAVE2(vin[kk*2], vin[kk*2+1], vout[k*2], vout[k*2+1]);
		}
	} else {
		for (k=0; k < Ncvec; ++k) { 
			int kk = (k/4) + (k%4)*(Ncvec/4);
			T::INTERLEAVE2(vin[k*2], vin[k*2+1], vout[kk*2], vout[kk*2+1]);
		}
	}
}




// Standard generic template scalar implementation.
template < typename Real > template < Bool inverse >
void FFTComplex<Real>:: zreorder( const Setup *setup, const Real* in, Real* out )
{
	int k, N = setup->N;
	for (k=0; k < 2*N; ++k) out[k] = in[k];
}




//##########################################################################################
//##########################################################################################
//############		
//############		Complex FFT Finalize
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_INLINE void FFTComplex<Float32>:: cplx_finalize( int Ncvec, const v4sf* in, v4sf* out, const v4sf* e )
{
	int k, dk = Ncvec/T::SIMD_SZ; // number of 4x4 matrix blocks
	v4sf r0, i0, r1, i1, r2, i2, r3, i3;
	v4sf sr0, dr0, sr1, dr1, si0, di0, si1, di1;
	OM_DEBUG_ASSERT(in != out);
	for (k=0; k < dk; ++k) {    
		r0 = in[8*k+0]; i0 = in[8*k+1];
		r1 = in[8*k+2]; i1 = in[8*k+3];
		r2 = in[8*k+4]; i2 = in[8*k+5];
		r3 = in[8*k+6]; i3 = in[8*k+7];
		T::VTRANSPOSE4(r0,r1,r2,r3);
		T::VTRANSPOSE4(i0,i1,i2,i3);
		VCPLXMUL(r1,i1,e[k*6+0],e[k*6+1]);
		VCPLXMUL(r2,i2,e[k*6+2],e[k*6+3]);
		VCPLXMUL(r3,i3,e[k*6+4],e[k*6+5]);
		
		sr0 = T::VADD(r0,r2); dr0 = T::VSUB(r0, r2);
		sr1 = T::VADD(r1,r3); dr1 = T::VSUB(r1, r3);
		si0 = T::VADD(i0,i2); di0 = T::VSUB(i0, i2);
		si1 = T::VADD(i1,i3); di1 = T::VSUB(i1, i3);
		
		/*
		 transformation for each column is:
		 
		 [1   1   1   1   0   0   0   0]   [r0]
		 [1   0  -1   0   0  -1   0   1]   [r1]
		 [1  -1   1  -1   0   0   0   0]   [r2]
		 [1   0  -1   0   0   1   0  -1]   [r3]
		 [0   0   0   0   1   1   1   1] * [i0]
		 [0   1   0  -1   1   0  -1   0]   [i1]
		 [0   0   0   0   1  -1   1  -1]   [i2]
		 [0  -1   0   1   1   0  -1   0]   [i3]    
		 */
		
		r0 = T::VADD(sr0, sr1); i0 = T::VADD(si0, si1);
		r1 = T::VADD(dr0, di1); i1 = T::VSUB(di0, dr1);
		r2 = T::VSUB(sr0, sr1); i2 = T::VSUB(si0, si1);
		r3 = T::VSUB(dr0, di1); i3 = T::VADD(di0, dr1);
		
		*out++ = r0; *out++ = i0; *out++ = r1; *out++ = i1;
		*out++ = r2; *out++ = i2; *out++ = r3; *out++ = i3;
	}
}




template <>
OM_INLINE void FFTComplex<Float32>:: cplx_preprocess( int Ncvec, const v4sf *in, v4sf *out, const v4sf *e )
{
	int k, dk = Ncvec/T::SIMD_SZ; // number of 4x4 matrix blocks
	v4sf r0, i0, r1, i1, r2, i2, r3, i3;
	v4sf sr0, dr0, sr1, dr1, si0, di0, si1, di1;
	OM_DEBUG_ASSERT(in != out);
	for (k=0; k < dk; ++k) {    
		r0 = in[8*k+0]; i0 = in[8*k+1];
		r1 = in[8*k+2]; i1 = in[8*k+3];
		r2 = in[8*k+4]; i2 = in[8*k+5];
		r3 = in[8*k+6]; i3 = in[8*k+7];
		
		sr0 = T::VADD(r0,r2); dr0 = T::VSUB(r0, r2);
		sr1 = T::VADD(r1,r3); dr1 = T::VSUB(r1, r3);
		si0 = T::VADD(i0,i2); di0 = T::VSUB(i0, i2);
		si1 = T::VADD(i1,i3); di1 = T::VSUB(i1, i3);
		
		r0 = T::VADD(sr0, sr1); i0 = T::VADD(si0, si1);
		r1 = T::VSUB(dr0, di1); i1 = T::VADD(di0, dr1);
		r2 = T::VSUB(sr0, sr1); i2 = T::VSUB(si0, si1);
		r3 = T::VADD(dr0, di1); i3 = T::VSUB(di0, dr1);
		
		VCPLXMULCONJ(r1,i1,e[k*6+0],e[k*6+1]);
		VCPLXMULCONJ(r2,i2,e[k*6+2],e[k*6+3]);
		VCPLXMULCONJ(r3,i3,e[k*6+4],e[k*6+5]);
		
		T::VTRANSPOSE4(r0,r1,r2,r3);
		T::VTRANSPOSE4(i0,i1,i2,i3);
		
		*out++ = r0; *out++ = i0; *out++ = r1; *out++ = i1;
		*out++ = r2; *out++ = i2; *out++ = r3; *out++ = i3;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Complex FFT
//############		
//##########################################################################################
//##########################################################################################




// Specialization for 32-bit floats using SIMD.
template <> template < Bool inverse >
void FFTComplex<Float32>:: transform_internal( const Setup *setup, const Float32* finput, Float32* foutput, v4sf* scratch, int ordered )
{
	int k, Ncvec   = setup->Ncvec;
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
		
		cplx_preprocess(Ncvec, vinput, buff[ib], (v4sf*)setup->e);
		ib = (cfftf1_ps(Ncvec, buff[ib], buff[0], buff[1], 
						setup->twiddle, &setup->ifac[0], +1) == buff[0] ? 0 : 1);
		for (k=0; k < Ncvec; ++k) {
			T::INTERLEAVE2(buff[ib][k*2], buff[ib][k*2+1], buff[ib][k*2], buff[ib][k*2+1]);
		}
	}
	else
	{
		ib = !ib;
		v4sf *tmp = buff[ib];
		for (k=0; k < Ncvec; ++k) {
			T::UNINTERLEAVE2(vinput[k*2], vinput[k*2+1], tmp[k*2], tmp[k*2+1]);
		}
		ib = (cfftf1_ps(Ncvec, buff[ib], buff[!ib], buff[ib], 
						setup->twiddle, &setup->ifac[0], -1) == buff[0] ? 0 : 1);
		cplx_finalize(Ncvec, buff[ib], buff[!ib], (v4sf*)setup->e);
		
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
	OM_DEBUG_ASSERT(buff[ib] == voutput);
}




// Standard generic template scalar implementation.
template < typename Real > template < Bool inverse >
void FFTComplex<Real>:: transform_internal( const Setup* setup, const Real* input, Real* output, v4sf* scratch, int ordered )
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
	ordered = 0; // it is always ordered.
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
		
		ib = (cfftf1_ps(Ncvec, input, buff[ib], buff[!ib], 
						setup->twiddle, &setup->ifac[0], +1) == buff[0] ? 0 : 1);
	}
	else
	{
		ib = (cfftf1_ps(Ncvec, input, buff[ib], buff[!ib], 
						setup->twiddle, &setup->ifac[0], -1) == buff[0] ? 0 : 1);
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
	OM_DEBUG_ASSERT(buff[ib] == output);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Real FFT Setup
//############		
//##########################################################################################
//##########################################################################################




template < typename Real >
int FFTComplex<Real>:: decompose( int n, int* ifac, const int* ntryh )
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
void FFTComplex<Real>:: cffti1_ps( int n, Scalar* wa, int* ifac )
{
	static const int ntryh[] = { 5,3,4,2,0 };
	int k1, j, ii;
	
	int nf = decompose(n,ifac,ntryh);
	Scalar argh = (2*math::pi<Scalar>())/(Scalar)n;
	int i = 1;
	int l1 = 1;
	for (k1=1; k1<=nf; k1++) {
		int ip = ifac[k1+1];
		int ld = 0;
		int l2 = l1*ip;
		int ido = n / l2;
		int idot = ido + ido + 2;
		int ipm = ip - 1;
		for (j=1; j<=ipm; j++) {
			Scalar argld;
			int i1 = i, fi = 0;
			wa[i-1] = 1;
			wa[i] = 0;
			ld += l1;
			argld = ld*argh;
			for (ii = 4; ii <= idot; ii += 2) {
				i += 2;
				fi += 1;
				wa[i-1] = math::cos(fi*argld);
				wa[i] = math::sin(fi*argld);
			}
			if (ip > 5) {
				wa[i1-1] = wa[i-1];
				wa[i1] = wa[i];
			}
		}
		l1 = l2;
	}
} /* cffti1 */




template < typename Real >
void FFTComplex<Real>:: new_setup( int N, Setup* s )
{
	int k, m;
	/* unfortunately, the fft size must be a multiple of 16 for complex FFTs 
     and 32 for real FFTs -- a lot of stuff would need to be rewritten to
     handle other cases (or maybe just switch to a scalar fft, I don't know..) */
	OM_DEBUG_ASSERT((N%(T::SIMD_SZ*T::SIMD_SZ))==0 && N>0);
	//OM_DEBUG_ASSERT((N % 32) == 0);
	s->N = N;
	/* nb of complex simd vectors */
	s->Ncvec = (N)/T::SIMD_SZ;
	s->data = util::allocateAligned<v4sf>( 2*s->Ncvec, 16 );
	s->e = (Scalar*)s->data;
	s->twiddle = (Scalar*)(s->data + (2*s->Ncvec*(T::SIMD_SZ-1))/T::SIMD_SZ);  
	
	for (k=0; k < s->Ncvec; ++k) {
		int i = k/T::SIMD_SZ;
		int j = k%T::SIMD_SZ;
		for (m=0; m < T::SIMD_SZ-1; ++m) {
			Scalar A = -2*math::pi<Scalar>()*(m+1)*k / N;
			s->e[(2*(i*3 + m) + 0)*T::SIMD_SZ + j] = math::cos(A);
			s->e[(2*(i*3 + m) + 1)*T::SIMD_SZ + j] = math::sin(A);
		}
    }
    cffti1_ps( N/T::SIMD_SZ, s->twiddle, s->ifac );
	
	/* check that N is decomposable with allowed prime factors */
	for (k=0, m=1; k < s->ifac[1]; ++k) { m *= s->ifac[2+k]; }
	if (m != N/T::SIMD_SZ)
	{
		//destroy_setup(s); s = 0;
	}
}




template < typename Real >
void FFTComplex<Real>:: destroy_setup( Setup* s )
{
	util::deallocateAligned( s->data );
}




template < typename Real >
void FFTComplex<Real>:: copy_setup( const Setup* s1, Setup* s2 )
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


#endif // INCLUDE_OM_FFT_COMPLEX_H
