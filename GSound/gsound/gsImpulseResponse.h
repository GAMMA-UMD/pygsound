/*
 * Project:     GSound
 * 
 * File:        gsound/gsImpulseResponse.h
 * Contents:    gsound::ImpulseResponse class declaration
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


#ifndef INCLUDE_GSOUND_IMPULSE_RESPONSE_H
#define INCLUDE_GSOUND_IMPULSE_RESPONSE_H


#include "gsConfig.h"


#include "gsSoundListenerIR.h"
#include "gsIRRequest.h"
#include "gsIRMetrics.h"
#include "internal/gsSIMDCrossover.h"
#include "internal/gsSampleBuffer.h"
#include "internal/gsHRTFFilter.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that creates a multichannel impulse response for offline usage.
/**
  * This class can be used to generate high-quality frequency-dependent IRs for
  * purposes other than sound rendering. It should not be used for performance-intensive
  * applications because the implementation is not very optimized.
  */
class ImpulseResponse
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new impulse response of length 0 with the specified format.
			ImpulseResponse();
			
			
		//********************************************************************************
		//******	Response Update Methods
			
			
			/// Update this impulse response for the specified source IR, listener, and frequency bands.
			/**
			  * The source IR is panned among the impulse response's channel layout after being
			  * transformed into the listener's coordinate system. The given frequency bands
			  * are used to compute a frequency-dependent filtered IR.
			  *
			  * If the normalize flag is TRUE, the resulting IR is normalized so that the
			  * maximal or minimal sample value is 1 or -1. Otherwise, the IR
			  * is scaled based on the source power and listener sensitivity.
			  */
			void setIR( const SoundSourceIR& sourceIR, const SoundListener& listener,
						const IRRequest& request );
			
			
		//********************************************************************************
		//******	IR Length Accessor Methods
			
			
			/// Return the length in seconds of this IR.
			GSOUND_INLINE Float getLength() const
			{
				return Float(buffer.getSize() / buffer.getSampleRate());
			}
			
			
			/// Return the length in samples of this IR.
			GSOUND_INLINE Size getLengthInSamples() const
			{
				return buffer.getSize();
			}
			
			
		//********************************************************************************
		//******	Channel Layout Accessor Methods
			
			
			/// Return the number of channels there are in this impulse response.
			GSOUND_INLINE Size getChannelCount() const
			{
				return buffer.getChannelCount();
			}
			
			
			/// Return a pointer to the sample data for the specified channel in this impulse response.
			GSOUND_INLINE const Float* getChannel( Index channelIndex ) const
			{
				return (const Float*)buffer.getChannel( channelIndex );
			}
			
			
			/// Return the current channel layout used to pan sound paths.
			GSOUND_INLINE const ChannelLayout& getChannelLayout() const
			{
				return buffer.getLayout();
			}
			
			
			/// Set the channel layout used to pan sound paths.
			/**
			  * Calling this method resets the impulse response to be of length 0.
			  */
			GSOUND_INLINE void setChannelLayout( const ChannelLayout& newChannelLayout )
			{
				buffer.setLayout( newChannelLayout );
				buffer.setSize( 0 );
			}
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the current sample rate used for this impulse response.
			GSOUND_INLINE SampleRate getSampleRate() const
			{
				return buffer.getSampleRate();
			}
			
			
		//********************************************************************************
		//******	Frequencies Accessor Methods
			
			
			/// Return the frequency bands that this impulse response was computed for.
			GSOUND_INLINE const FrequencyBands& getFrequencies() const
			{
				return frequencies;
			}
			
			
		//********************************************************************************
		//******	IR Buffer Accessor Method
			
			
			/// Return a reference to the sound buffer that contains the sample data for this impulse response.
			GSOUND_INLINE const SoundBuffer& getBuffer() const
			{
				return buffer;
			}
			
			
		//********************************************************************************
		//******	Binned IR Accessor Methods
			
			
			/// Return the number of enery bins there are in the impulse response.
			GSOUND_INLINE Size getBinCount() const
			{
				return bins.getSampleCount();
			}
			
			
			/// Return the energy for each band in this IR for the given bin index.
			GSOUND_INLINE const FrequencyBandResponse& getBin( Index binIndex ) const
			{
				return *reinterpret_cast<const FrequencyBandResponse*>( &(bins.getChannel(0)[binIndex]) );
			}
			
			
		//********************************************************************************
		//******	Acoustic Metrics Accessor Method
			
			
			/// Return a reference to the sound buffer that contains the sample data for this impulse response.
			GSOUND_INLINE const IRMetrics& getMetrics() const
			{
				return metrics;
			}
			
			
			/// Get the metrics for a simulated IR.
			static void getMetrics( const SoundSourceIR& ir, const FrequencyBands& frequencies, Float snrDB, IRMetrics& metrics );
			
			
			/// Get the metrics for a pressure IR.
			static void getMetrics( const SoundBuffer& ir, const FrequencyBands& frequencies, Float snrDB, IRMetrics& metrics );
			
			
	private:
		
		//********************************************************************************
		//******	Private Type Declarations
			
			
			/// Define the type of SIMD crossover to use for frequency band filtering.
			typedef internal::SIMDCrossover<Float32,GSOUND_FREQUENCY_COUNT> CrossoverType;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			static void interleaveBands( const SampledIR& ir, const Float* pan, Float* output );
			
			
			void panDirections( const SampledIR& ir, const ChannelLayout& channelLayout, const Matrix3f& orientation,
								internal::SampleBuffer<Float>& pan );
			
			
			template < typename T >
			static void getMetrics( const T* intensity, Size numBins, Size stride, Float binSize, Float snr,
									IRMetrics& metrics, Index band );
			
			
			static void energyTimeCurve( const SoundBuffer& input, SoundBuffer& result );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A sound buffer containing the impulse response samples for all channels.
			SoundBuffer buffer;
			
			
			/// A buffer containing the bins for each frequency band.
			internal::SampleBuffer<SIMDBands> bins;
			
			
			/// An object that contains various acoustic metrics for the impulse response.
			IRMetrics metrics;
			
			
		//********************************************************************************
		//******	Temporary Private Data Members
			
			
			/// An object that stores the frequency bands for each IR.
			FrequencyBands frequencies;
			
			
			/// An object that is used to filter the impulse response into different frequency bands.
			CrossoverType crossover;
			
			
			/// A temporary buffer of pan values for each channel (stored packed one after another).
			internal::SampleBuffer<Float> pan;
			
			
			/// A temporary buffer of interleaved broad-band IRs for each frequency band, before being combined into one IR.
			internal::SampleBuffer<Float> bandIRs;
			
			
			/// A temporary array of gain coefficients used for panning sound paths.
			Array<Gain> channelGains;
			
			
			/// A spherical harmonic basis used for HRTF interpolation.
			SHExpansion<Float> shBasis;
			
			
			/// A buffer of filtered noise that is used to reconstruct the phase of the pressure IR.
			om::PODArray<SIMDBands,1,Size,AlignedAllocator<16> > noise;
			
			
			/// A random number generator used to generate the IR's filtered noise.
			om::math::Random<Float> noiseRand;
			
			
			/// A temporary buffer that contains a broadband interpolated HRTF filter.
			internal::SampleBuffer<Float> hrtfBuffer;
			
			
			/// An object that maintains data for an HRTF so that it can be used to filter audio.
			internal::HRTFFilter hrtfFilter;
			
			
			/// A pointer to the HRTF that this impulse response is using.
			const HRTF* hrtf;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_IMPULSE_RESPONSE_H
