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

#ifndef INCLUDE_OM_SIMD_CROSSOVER_H
#define INCLUDE_OM_SIMD_CROSSOVER_H


#include "omSoundBaseConfig.h"


#include "omSoundFrequencyBands.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that uses time-domain IIR filtering to split an input audio stream into interleaved SIMD frequency bands.
template < typename T, Size frequencyCount >
class SIMDCrossover
{
	public:
		
		//********************************************************************************
		//******	Public Type Declarations
			
			
			/// The wide SIMD type used to process all frequency bands together.
			typedef math::SIMDArray<T,frequencyCount> SIMDType;
			
			
			/// A class that stores a few samples of history information for a crossover filter set.
			class OM_ALIGN(16) FilterHistory
			{
				public:
					
					OM_INLINE FilterHistory()
					{
						FilterHistory::reset();
					}
					
					/// Reset the filter history to zero.
					OM_INLINE void reset()
					{
						for ( Index i = 0; i < 4; i++ )
						{
							for ( Index j = 0; j < frequencyCount; j++ )
								input[i][j] = output[i][j] = T(0);
						}
					}
					
					/// The input and output histories for the cascaded 2nd-order filters.
					SIMDType input[4];
					SIMDType output[4];
					
			};
			
			
			/// A class that stores a few samples of history information for a crossover.
			class OM_ALIGN(16) History
			{
				public:
					
					/// Reset the crossover history to zero.
					OM_INLINE void reset()
					{
						for ( Index i = 0; i < (frequencyCount - 1); i++ )
							filters[i].reset();
					}
					
					/// History information for each of the filter sets in this crossover history.
					FilterHistory filters[frequencyCount - 1];
					
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new uninitialized SIMD crossover.
			OM_INLINE SIMDCrossover()
				:	filters( NULL )
			{
			}
			
			
			/// Create a new SIMD crossover for the specified frequency bands.
			OM_INLINE SIMDCrossover( const FrequencyBands& newBands )
				:	filters( NULL )
			{
				this->setBands( newBands );
			}
			
			
			/// Create a new SIMD crossover as a copy of another.
			OM_INLINE SIMDCrossover( const SIMDCrossover& other )
				:	filters( NULL )
			{
				if ( other.filters )
					filters = new ( om::util::allocateAligned<CrossoverFilters>( 1, 16 ) ) CrossoverFilters( *other.filters );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			OM_INLINE ~SIMDCrossover()
			{
				if ( filters )
					om::util::destructAligned( filters );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			OM_INLINE SIMDCrossover& operator = ( const SIMDCrossover& other )
			{
				if ( this != &other )
				{
					if ( !filters && other.filters )
						filters = om::util::allocateAligned<CrossoverFilters>( 1, 16 );
					
					if ( other.filters )
						filters = new ( filters ) CrossoverFilters( *other.filters );
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Crossover Filter Processing Methods
			
			
			/// Apply this crossover filter to the specified input buffer, writing the band-separated SIMD output.
			OM_FORCE_INLINE void filterScalar( History& history, const T* input,
												T* simdOutput, Size numSamples )
			{
				if ( !filters )
					return;
				
				const T* const inputEnd = input + numSamples;
				const Size advance = SIMDType::getWidth();
				CrossoverFilters& crossover = *filters;
				
				UInt flushMode = _MM_GET_FLUSH_ZERO_MODE();
				_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
				
				// Copy the history to the stack so that there is no round-trip to memory.
				History localHistory = history;
				
				while ( input != inputEnd )
				{
					// Expand the input to the SIMD width.
					SIMDType simdInput(*input);
					
					// Apply each filter set (loop will be unrolled).
					for ( Index i = 0; i < numFilterSets; i++ )
						crossover.filters[i].apply( simdInput, localHistory.filters[i] );
					
					// Write the output.
					simdInput.store( simdOutput );
					
					input++;
					simdOutput += advance;
				}
				
				// Sanitize the history to avoid denormalized floating-point numbers.
				for ( Index i = 0; i < numFilterSets; i++ )
				{
					FilterHistory& hist = localHistory.filters[i];
					
					for ( Index j = 0; j < 4; j++ )
					{
						hist.input[j] = math::select( math::abs(hist.input[j]) < SIMDType(math::epsilon<T>()),
													SIMDType(T(0)), hist.input[j] );
						hist.output[j] = math::select( math::abs(hist.output[j]) < SIMDType(math::epsilon<T>()),
													SIMDType(T(0)), hist.output[j] );
					}
				}
				
				_MM_SET_FLUSH_ZERO_MODE( flushMode );
				
				// Store the history.
				history = localHistory;
			}
			
			
			/// Apply this crossover filter to the specified SIMD input buffer, writing the filtered output.
			OM_FORCE_INLINE void filterSIMD( History& history, const T* simdInput, T* simdOutput, Size numSamples )
			{
				if ( !filters )
					return;
				
				UInt flushMode = _MM_GET_FLUSH_ZERO_MODE();
				_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
				
				const Size advance = SIMDType::getWidth();
				const T* const outputEnd = simdOutput + advance*numSamples;
				CrossoverFilters& crossover = *filters;
				
				// Copy the history to the stack so that there is no round-trip to memory.
				History localHistory = history;
				
				while ( simdOutput != outputEnd )
				{
					// Expand the input to the SIMD width.
					SIMDType in = SIMDType::load(simdInput);
					
					// Apply each filter set (loop will be unrolled).
					for ( Index i = 0; i < numFilterSets; i++ )
						crossover.filters[i].apply( in, localHistory.filters[i] );
					
					// Write the output.
					in.store( simdOutput );
					
					simdInput += advance;
					simdOutput += advance;
				}
				
				// Store the history.
				history = localHistory;
				
				_MM_SET_FLUSH_ZERO_MODE( flushMode );
			}
			
			
			/// Apply this crossover filter to the specified SIMD input buffer, writing the filtered output.
			OM_FORCE_INLINE void filterSIMDLowPass( History& history, const T* simdInput, T* simdOutput, Size numSamples )
			{
				if ( !filters )
					return;
				
				UInt flushMode = _MM_GET_FLUSH_ZERO_MODE();
				_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
				
				const Size advance = SIMDType::getWidth();
				const T* const outputEnd = simdOutput + advance*numSamples;
				CrossoverFilters& crossover = *filters;
				
				// Copy the history to the stack so that there is no round-trip to memory.
				History localHistory = history;
				
				while ( simdOutput != outputEnd )
				{
					// Expand the input to the SIMD width.
					SIMDType in = SIMDType::load(simdInput);
					
					// Apply each filter set (loop will be unrolled).
					for ( Index i = 0; i < numFilterSets; i++ )
						crossover.filtersLP[i].apply( in, localHistory.filters[i] );
					
					// Write the output.
					in.store( simdOutput );
					
					simdInput += advance;
					simdOutput += advance;
				}
				
				// Store the history.
				history = localHistory;
				
				_MM_SET_FLUSH_ZERO_MODE( flushMode );
			}
			
			
			/// Apply this crossover filter to the specified SIMD input buffer, writing the filtered output.
			OM_FORCE_INLINE void filterSIMDLowPassSingle( History& history, const SIMDType& simdInput, SIMDType& simdOutput )
			{
				simdOutput = simdInput;
				
				// Apply each filter set (loop will be unrolled).
				for ( Index i = 0; i < numFilterSets; i++ )
					filters->filtersLP[i].apply( simdOutput, history.filters[i] );
			}
			
			
			/// Enable flushing subnormal floats to zero for performance.
			OM_FORCE_INLINE UInt enableFlushToZero()
			{
				UInt flushMode = _MM_GET_FLUSH_ZERO_MODE();
				_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
				return flushMode;
			}
			
			
			/// Disable flushing subnormal floats to zero.
			OM_FORCE_INLINE void disableFlushToZero( UInt flushMode )
			{
				_MM_SET_FLUSH_ZERO_MODE( flushMode );
			}
			
			
		//********************************************************************************
		//******	Impulse Response Methods
			
			
			/// Compute the interleaved time-domain IRs of this crossover for all frequency bands.
			OM_INLINE void getSIMDIR( T* ir, Size numSamples ) const
			{
				if ( !filters )
					return;
				
				UInt flushMode = _MM_GET_FLUSH_ZERO_MODE();
				_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
				
				const Size advance = SIMDType::getWidth();
				const T* const outputEnd = ir + advance*numSamples;
				CrossoverFilters& crossover = *filters;
				History history;
				
				// The initial impulse.
				SIMDType impulse( T(1) );
				
				// The first sample.
				if ( ir != outputEnd )
				{
					// Apply each filter set (loop will be unrolled).
					for ( Index i = 0; i < numFilterSets; i++ )
						crossover.filters[i].apply( impulse, history.filters[i] );
					
					// Write the output.
					impulse.store( ir );
					ir += advance;
				}
				
				// The rest of the input.
				impulse = SIMDType( T(0) );
				
				// The rest of the IR.
				while ( ir != outputEnd )
				{
					SIMDType in = impulse;
					
					// Apply each filter set (loop will be unrolled).
					for ( Index i = 0; i < numFilterSets; i++ )
						crossover.filters[i].apply( in, history.filters[i] );
					
					// Write the output.
					in.store( ir );
					ir += advance;
				}
				
				_MM_SET_FLUSH_ZERO_MODE( flushMode );
			}
			
			
			
		//********************************************************************************
		//******	Crossover Band Accessor Methods
			
			
			/// Reset the crossover for the specified frequency bands.
			OM_INLINE void setBands( const FrequencyBands& bands, SampleRate sampleRate )
			{
				if ( filters == NULL )
					filters = om::util::allocateAligned<CrossoverFilters>( 1, 16 );
				
				CrossoverFilters& crossover = *filters;
				
				for ( Index i = 0; i < numFilterSets; i++ )
				{
					FilterSet& filterSet = crossover.filters[i];
					FilterSet& filterSetLP = crossover.filtersLP[i];
					const T crossover = bands.getCrossover(i);
					const T frequencyRatio = math::clamp( T(crossover / sampleRate), T(0), T(0.499) );
					const T w0HighPass = math::tan( math::pi<T>()*frequencyRatio );
					const T w0LowPass = T(1) / w0HighPass;
					
					// Determine the filter for each band for this filter set.
					for ( Index j = 0; j < frequencyCount; j++ )
					{
						if ( i >= j )
						{
							// Low-pass 4th-order Linkwitz-Riley filter, implemented as 2 cascaded 2nd-order Butterworth filters.
							getButterworth2LowPass( w0LowPass, filterSet.a[0][j], filterSet.a[1][j], filterSet.a[2][j],
													filterSet.b[0][j], filterSet.b[1][j] );
							getButterworth2LowPass( w0LowPass, filterSet.a[3][j], filterSet.a[4][j], filterSet.a[5][j],
													filterSet.b[2][j], filterSet.b[3][j] );
							
							getButterworth2LowPass( w0LowPass, filterSetLP.a[0][j], filterSetLP.a[1][j], filterSetLP.a[2][j],
													filterSetLP.b[0][j], filterSetLP.b[1][j] );
							getButterworth2LowPass( w0LowPass, filterSetLP.a[3][j], filterSetLP.a[4][j], filterSetLP.a[5][j],
													filterSetLP.b[2][j], filterSetLP.b[3][j] );
						}
						else
						{
							// High-pass 4th-order Linkwitz-Riley filter, implemented as 2 cascaded 2nd-order Butterworth filters.
							getButterworth2HighPass( w0HighPass, filterSet.a[0][j], filterSet.a[1][j], filterSet.a[2][j],
													filterSet.b[0][j], filterSet.b[1][j] );
							getButterworth2HighPass( w0HighPass, filterSet.a[3][j], filterSet.a[4][j], filterSet.a[5][j],
													filterSet.b[2][j], filterSet.b[3][j] );
							
							// Unity IR for the low-pass filter coefficients.
							filterSetLP.a[0][j] = T(1);
							filterSetLP.a[1][j] = filterSetLP.a[2][j] = filterSetLP.b[0][j] = filterSetLP.b[1][j] = T(0);
							filterSetLP.a[3][j] = T(1);
							filterSetLP.a[4][j] = filterSetLP.a[5][j] = filterSetLP.b[2][j] = filterSetLP.b[3][j] = T(0);
						}
					}
				}
			}
			
			
		//********************************************************************************
		//******	Status Accessor Method
			
			
			/// Return whether or not this SIMD crossover has had its filters initialized and is ready for use.
			OM_INLINE Bool isInitialized() const
			{
				return filters != NULL;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The number of filter sets that this SIMD crossover uses.
			static const Size numFilterSets = frequencyCount - 1;
			
			
		//********************************************************************************
		//******	Private Type Declarations
			
			
			/// A class that implements a SIMD-wide set of crossover filters.
			class FilterSet
			{
				public:
					
					/// Apply the filter set to the specified value using the given history.
					OM_FORCE_INLINE void apply( SIMDType& inputOutput, FilterHistory& history )
					{
						// Apply first 2nd-order filter.
						SIMDType in = a[0]*inputOutput;
						SIMDType in2 = (in - b[0]*history.output[0]) + (a[1]*history.input[0] - b[1]*history.output[1]) + a[2]*history.input[1];
						
						// Update the history information.
						history.input[1] = history.input[0];	history.input[0] = in;
						history.output[1] = history.output[0];	history.output[0] = in2;
						
						// Apply second 2nd-order filter to the result of the first.
						in = a[3]*in2;
						inputOutput = (in - b[2]*history.output[2]) + (a[4]*history.input[2] - b[3]*history.output[3]) + a[5]*history.input[3];
						
						// Update the history information.
						history.input[3] = history.input[2];	history.input[2] = in;
						history.output[3] = history.output[2];	history.output[2] = inputOutput;
					}
					
					/// The coefficients for two cascaded 2nd-order filters.
					SIMDType a[6];
					SIMDType b[4];
					
			};
			
			
			/// A class that store the aligned filter coefficients for a crossover.
			class CrossoverFilters
			{
				public:
					
					
					/// The coefficients for two cascaded 2nd-order filters.
					FilterSet filters[numFilterSets];
					
					/// The coefficients for two cascaded 2nd-order filters.
					FilterSet filtersLP[numFilterSets];
					
			};
			
			
		//********************************************************************************
		//******	Filter Computation Methods
			
			
			/// Get the coefficients of a 1st-order butterworth filter with the given w0.
			OM_INLINE static void getButterworth1LowPass( T w0, T& a0, T& a1, T& b0 )
			{
				T A = 1 + w0;
				
				a0 = 1 / A;
				a1 = 1;
				b0 = (1 - w0)*a0;
			}
			
			
			/// Get the coefficients of a 1st-order butterworth high-pass filter with the given w0.
			OM_INLINE static void getButterworth1HighPass( T w0, T& a0, T& a1, T& a2, T& b0, T& b1 )
			{
				getButterworth1LowPass( w0, a0, a1, b0 );
				a1 = -a1;
				b0 = -b0;
			}
			
			
			/// Get the coefficients of a 2nd-order butterworth low-pass filter with the given w0.
			OM_INLINE static void getButterworth2LowPass( T w0, T& a0, T& a1, T& a2, T& b0, T& b1 )
			{
				T B1 = -2.0f * math::cos( math::pi<T>()*T(3)/T(4) );
				T w0squared = w0*w0;
				T A = 1 + B1*w0 + w0squared;
				
				a0 = 1 / A;
				a1 = 2;
				a2 = 1;
				b0 = 2*(1 - w0squared)*a0;
				b1 = (1 - B1*w0 + w0squared)*a0;
			}
			
			
			/// Get the coefficients of a 2nd-order butterworth high-pass filter with the given w0.
			OM_INLINE static void getButterworth2HighPass( T w0, T& a0, T& a1, T& a2, T& b0, T& b1 )
			{
				getButterworth2LowPass( w0, a0, a1, a2, b0, b1 );
				a1 = -a1;
				b0 = -b0;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The coefficients for two cascaded 2nd-order filters.
			CrossoverFilters* filters;
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_CROSSOVER_H
