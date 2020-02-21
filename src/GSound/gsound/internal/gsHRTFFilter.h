/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsHRTFFilter.h
 * Contents:    gsound::internal::HRTFFilter class declaration
 * 
 * Author(s):   Carl Schissler
 * Website:     http://gamma.cs.unc.edu/GSOUND/
 * 
 * License:
 * 
 *     Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
 *     All rights reserved.
 *     
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for educational, research, and non-profit purposes, without
 *     fee, and without a written agreement is hereby granted, provided that the
 *     above copyright notice, this paragraph, and the following four paragraphs
 *     appear in all copies.
 *     
 *     Permission to incorporate this software into commercial products may be
 *     obtained by contacting the University of North Carolina at Chapel Hill.
 *     
 *     This software program and documentation are copyrighted by Carl Schissler and
 *     the University of North Carolina at Chapel Hill. The software program and
 *     documentation are supplied "as is", without any accompanying services from
 *     the University of North Carolina at Chapel Hill or the authors. The University
 *     of North Carolina at Chapel Hill and the authors do not warrant that the
 *     operation of the program will be uninterrupted or error-free. The end-user
 *     understands that the program was developed for research purposes and is advised
 *     not to rely exclusively on the program for any reason.
 *     
 *     IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR ITS
 *     EMPLOYEES OR THE AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE
 *     UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *     THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 *     DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 *     STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS
 *     ON AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND
 *     THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 *     ENHANCEMENTS, OR MODIFICATIONS.
 */


#ifndef INCLUDE_GSOUND_HRTF_FILTER_H
#define INCLUDE_GSOUND_HRTF_FILTER_H


#include "gsInternalConfig.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles interpolation and filter generation from arbitrary head-related transfer functions.
/**
  * A spherical harmonic representation is used to approximate an input multichannel HRTF,
  * then this representation is queried to generate smoothly interpolating 
  * filters for an input spherical harmonic basis that describes the directionality
  * of the incoming sound. The resulting filter can then be used to update a convolution-based
  * renderer.
  */
class HRTFFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new HRTF filter object without any HRTF.
			HRTFFilter();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy an HRTF filter, releasing internal resources.
			~HRTFFilter();
			
			
		//********************************************************************************
		//******	HRTF Accessor Methods
			
			
			/// Return the number of channels there are in this HRTF.
			GSOUND_INLINE Size getChannelCount() const
			{
				return channels.getSize();
			}
			
			
			/// Reset the HRTF filter to correspond to the specified HRTF.
			/**
			  * The HRTF is projected into the spherical harmonic basis. The
			  * spherical harmonic expansion uses the lowest order that satisfies the
			  * given absolute error threshold.
			  *
			  * The spherical harmonic order is limited to the specified max order.
			  * The expansion terminates early if the relative improvement in error is less
			  * than the specified convergence threshold (given as a fraction, 2% = 0.02).
			  */
			Bool setHRTF( const HRTF& newHRTF, SampleRate sampleRate, Size maxOrder = Size(9), Float maxError = Float(0.05),
							Float convergence = Float(0.00), Size numIntegrationSamples = Size(2000) );
			
			
		//********************************************************************************
		//******	Filter Accessor Methods
			
			
			/// Return the order of the spherical harmonic expansion of this HRTF.
			GSOUND_INLINE Size getSHOrder() const
			{
				return order;
			}
			
			
			/// Return the length of the power-of-two-padded filter for the HRTF in time domain.
			GSOUND_INLINE Size getFilterLength() const
			{
				return length;
			}
			
			
			/// Get the time-domain HRTF filter for the given channel index and SH expansion in the output filter array.
			/**
			  * The filter array must be at least (N + 2) real values long, where N is the power-of-two
			  * time-domain length of the HRTF filter from getFilterLength(). The extra space is
			  * used for the FFT from frequency domain (the internal format) to time domain.
			  */
			Bool getFilter( Index channelIndex, const SHExpansion<Float>& basis, Float* filter ) const;
			
			
			/// Get the frequency-domain HRTF filter for the given channel index and SH expansion in the output filter array.
			/**
			  * The filter array must be at least (N/2 + 1) complex values long,
			  * or (N + 2) floating-point values, where N is the power-of-two
			  * time-domain length of the HRTF filter from getFilterLength().
			  */
			Bool getFilter( Index channelIndex, const SHExpansion<Float>& basis, Complex<Float>* filter ) const;
			
			
		//********************************************************************************
		//******	Size in Bytes Accessor Methods
			
			
			/// Return the approximate size in bytes of this sound band HRTF object's allocated memory.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				return SH::getCoefficientCount(order)*length*sizeof(Float);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// The type to use for an HRTF filter sample.
			typedef om::math::Matrix<Float> Filter;
			
			
			/// An opaque object that hides the FFT implementation.
			class FFTData;
			
			
			/// A class that stores a single HRTF sample for a 3D normalized direction.
			class Sample;
			
			
			/// A class that stores information about an HRTF channel.
			class Channel
			{
				public:
					
					
					/// A spherical harmonic expansion of this HRTF channel.
					Array<Filter> hrtf;
					
					
			};
			
			
		//********************************************************************************
		//******	Private Helper Functions
			
			
			/// Interpolate the samples in this HRTF for the specified direction.
			static Bool interpolateSample( const Vector3f& direction, const Sample* samples, Size numSamples,
											Float* filter, Size filterLength );
			
			
			/// Return a uniformly distributed random unit vector direction.
			GSOUND_FORCE_INLINE static Vector3f getRandomDirection( math::Random<Float>& variable )
			{
				const Float u1 = variable.sample( Float(-1), Float(1) );
				const Float u2 = variable.sample( Float(0), Float(1) );
				const Float r = math::sqrt( Float(1) - u1*u1 );
				const Float theta = Float(2)*math::pi<Float>()*u2;
				
				return Vector3f( r*math::cos( theta ), r*math::sin( theta ), u1 );
			}
			
			
			/// Initialize the FFT data for this HRTF filter.
			void initializeFFTData( Size newFilterLength );
			
			
			/// Clean up and destroy the FFT data for this HRTF filter.
			void  deinitializeFFTData();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of the channels in this HRTF.
			om::ShortArray<Channel,2> channels;
			
			
			/// The order of the spherical harmonic expansion of this HRTF.
			Size order;
			
			
			/// The power-of-two-padded length of the HRTF filter in time domain.
			Size length;
			
			
			/// A pointer to intermediate data used to do FFTs.
			FFTData* fftData;
			
			
			/// The sample rate that this HRTF filter is currently stored in.
			SampleRate sampleRate;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_HRTF_FILTER_H
