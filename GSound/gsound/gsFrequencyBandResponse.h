/*
 * Project:     GSound
 * 
 * File:        gsound/gsFrequencyBandResponse.h
 * Contents:    gsound::FrequencyBandResponse class declaration
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


#ifndef INCLUDE_GSOUND_FREQUENCY_BAND_RESPONSE_H
#define INCLUDE_GSOUND_FREQUENCY_BAND_RESPONSE_H


#include "gsConfig.h"


#include "gsFrequencyBands.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a frequency band response over a fixed number of frequencies.
/**
  * The number of frequencies is determined by the value of GSOUND_FREQUENCY_COUNT
  * in gsConfig.h. The frequency band response is used to store gain coefficients
  * used in the sound propagation and rendering runtime. While materials can be
  * stored using arbitrary frequency measurements via FrequencyResponse, these
  * arbitrary frequencies must be mapped to a common set of bands for the entire
  * simulation. The frequencies are specified with a FrequencyBands object which
  * is passed to the simulation in a PropagationRequest or RenderRequest object.
  */
class GSOUND_ALIGN(16) FrequencyBandResponse
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default frequency band response with the value of 1 across all frequency bands.
			GSOUND_INLINE FrequencyBandResponse()
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] = Real(1);
			}
			
			
			/// Create a frequency band response with the specified gain across all frequency bands.
			/**
			  * @param initialGain - the initial gain value used for all frequency bands.
			  */
			GSOUND_INLINE FrequencyBandResponse( Real initialGain )
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] = initialGain;
			}
			
			
			/// Create a frequency band response with gain coefficients from the specified array.
			/**
			  * The array must not be NULL and must have at least the correct number of elements.
			  * The constructor reads GSOUND_FREQUENCY_COUNT values from the input array
			  * and uses them to initialize the frequency band response.
			  *
			  * @param array - the array of initial gain values used for each frequency band.
			  */
			GSOUND_INLINE FrequencyBandResponse( const Real array[GSOUND_FREQUENCY_COUNT] )
			{
				for ( Index i = 0; i < numFrequencyBands; i++ )
					response[i] = array[i];
			}
			
			
		//********************************************************************************
		//******	Frequency Band Accessor Methods
			
			
			/// Return the response at the specified frequency band index.
			/**
			  * If the specified band index is greater than or equal to the number
			  * of frequency bands in the FrequencyBandResponse, an assertion is raised.
			  * Otherwise, this operator returns the gain coefficient of the frequency band
			  * at the specified index.
			  * 
			  * @param bandIndex - the index of the frequency band gain coefficient to query.
			  * @return a reference to the gain at the specified frequency band.
			  */
			GSOUND_FORCE_INLINE Real& operator [] ( Index bandIndex )
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return response[bandIndex];
			}
			
			
			/// Return the response at the specified frequency band index.
			/**
			  * If the specified band index is greater than or equal to the number
			  * of frequency bands in the FrequencyBandResponse, an assertion is raised.
			  * Otherwise, this operator returns the gain coefficient of the frequency band
			  * at the specified index.
			  * 
			  * @param bandIndex - the index of the frequency band gain coefficient to query.
			  * @return the gain at the specified frequency band.
			  */
			GSOUND_FORCE_INLINE Real operator [] ( Index bandIndex ) const
			{
				GSOUND_DEBUG_ASSERT( bandIndex < numFrequencyBands );
				
				return response[bandIndex];
			}
			
			
			/// Return the number of bands in this frequency band response.
			/**
			  * The value returned by this method is GSOUND_FREQUENCY_COUNT and is fixed at compilation time
			  * in order to allow simple multiplication of FrequencyBandResponse objects
			  * and to reduce the number of allocations performed.
			  * 
			  * @return the number of bands in this FrequencyBandResponse.
			  */
			GSOUND_INLINE Size getBandCount() const
			{
				return numFrequencyBands;
			}
			
			
		//********************************************************************************
		//******	Frequency Band Average Gain Methods
			
			
			/// Return the interpolated gain for the specified frequency.
			/**
			  * This method interpolates between the two nearest frequencies in the given
			  * frequency bands at the specified frequency. It returns the linearly interpolated
			  * gain value of this response at that frequency.
			  */
			Real getFrequencyGain( Real frequency, const FrequencyBands& frequencies ) const;
			
			
			/// Return the average gain over the specified frequency range.
			/**
			  * This method is useful when mapping a higher-resolution frequency band response
			  * down to a lower-resolution frequency band response, especially when doing DSP
			  * operations on audio. The requested band boundaries are clamped to the 
			  * minimum and maximum frequencies in this frequency band response and will be swaped
			  * if they are not specified in ascending order.
			  *
			  * The method returns the average gain over the specified frequency band
			  * range.
			  * 
			  * @param lowFrequency - the low frequency of the range to query for average gain.
			  * @param highFrequency - the high frequency of the range to query for average gain.
			  * @return the average gain over the specified frequency band.
			  */
			Real getBandGain( Real lowFrequency, Real highFrequency, const FrequencyBands& frequencies ) const;
			
			
			/// Return the average gain over the specified frequency range.
			GSOUND_INLINE Real getBandGain( const AABB1f& bandRange, const FrequencyBands& frequencies ) const
			{
				return this->getBandGain( bandRange.min, bandRange.max, frequencies );
			}
			
			
			/// Return the average gain of this FrequencyBandResponse over its entire range.
			/**
			  * This method computes and returns the average of the gain coefficients of all
			  * frequency bands in this FrequencyBandResponse.
			  * 
			  * @return the average gain of all frequency bands in this FrequencyBandResponse.
			  */
			GSOUND_FORCE_INLINE Real getAverage() const
			{
				return math::sumScalar( SIMDBands::loadUnaligned( response ) ) / Real(numFrequencyBands);
			}
			
			
			/// Convert this response from the specified frequency bands to the output frequency bands.
			/**
			  * This method performs frequency interpolation in order to smoothly convert the frequency
			  * resposnse to the destination frequency bands. This is a lossy conversion if the
			  * frequency bands are not equal.
			  */
			FrequencyBandResponse convertTo( const FrequencyBands& frequenciesIn, const FrequencyBands& frequenciesOut ) const;
			
			
		//********************************************************************************
		//******	Max Gain Method
			
			
			/// Return the maximum value over all frequencies.
			GSOUND_FORCE_INLINE Real getMax() const
			{
				Real maxGain = response[0];
				
				for ( Index i = 1; i < numFrequencyBands; i++ )
					maxGain = math::max( maxGain, response[i] );
				
				return maxGain;
			}
			
			
		//********************************************************************************
		//******	Frequency Response Addition Operators
			
			
			/// Add this FrequencyBandResponse to another and return the result.
			/**
			  * The gains for each band in the frequency band responses are added together.
			  * 
			  * @param other - the FrequencyBandResponse to add to this FrequencyBandResponse object.
			  * @return the result of the addition operation.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse operator + ( const FrequencyBandResponse& other ) const
			{
				return FrequencyBandResponse( SIMDBands::loadUnaligned( response ) + SIMDBands::loadUnaligned( other.response ) );
			}
			
			
			/// Add a FrequencyBandResponse to this one and modify this response.
			/**
			  * The gains for each band in the frequency band responses are added together.
			  * 
			  * @param other - the FrequencyBandResponse to add to this FrequencyBandResponse object.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse& operator += ( const FrequencyBandResponse& other )
			{
				(SIMDBands::loadUnaligned( response ) + SIMDBands::loadUnaligned( other.response )).storeUnaligned( response );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Frequency Response Subtraction Operators
			
			
			/// Subtract a FrequencyBandResponse from this one and return the result.
			/**
			  * The gains for each band in the frequency band responses are subtracted.
			  * 
			  * @param other - the FrequencyBandResponse to subtract from this FrequencyBandResponse object.
			  * @return the result of the subtraction operation.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse operator - ( const FrequencyBandResponse& other ) const
			{
				return FrequencyBandResponse( SIMDBands::loadUnaligned( response ) - SIMDBands::loadUnaligned( other.response ) );
			}
			
			
			/// Subtract a FrequencyBandResponse from this one and modify this response.
			/**
			  * The gains for each band in the frequency band responses are subtracted.
			  * 
			  * @param other - the FrequencyBandResponse to subtract from this FrequencyBandResponse object.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse& operator -= ( const FrequencyBandResponse& other )
			{
				(SIMDBands::loadUnaligned( response ) - SIMDBands::loadUnaligned( other.response )).storeUnaligned( response );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Frequency Response Multiplication Operators
			
			
			/// Multiply this frequency band response by another and return the result.
			/**
			  * The gains for each band in the frequency band responses are multiplied together
			  * with the resulting frequency band response representing the response if the two
			  * responses were applied in series to incoming audio.
			  * 
			  * @param other - the FrequencyBandResponse to multiply with this FrequencyBandResponse object.
			  * @return the result of the multiplication operation.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse operator * ( const FrequencyBandResponse& other ) const
			{
				return FrequencyBandResponse( SIMDBands::loadUnaligned( response ) * SIMDBands::loadUnaligned( other.response ) );
			}
			
			
			/// Multiply this frequency band response by another and modify this response.
			/**
			  * The gains for each band in the frequency band responses are multiplied together
			  * with the resulting frequency band response representing the response if the two
			  * responses were applied in series to incoming audio.
			  * 
			  * @param other - the FrequencyBandResponse to multiply with this FrequencyBandResponse object.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse& operator *= ( const FrequencyBandResponse& other )
			{
				(SIMDBands::loadUnaligned( response ) * SIMDBands::loadUnaligned( other.response )).storeUnaligned( response );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Frequency Response Division Operators
			
			
			/// Multiply this frequency band response by another and return the result.
			/**
			  * The gains for each band in the frequency band responses are divided.
			  * 
			  * @param other - the FrequencyBandResponse to multiply with this FrequencyBandResponse object.
			  * @return the result of the division operation.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse operator / ( const FrequencyBandResponse& other ) const
			{
				return FrequencyBandResponse( SIMDBands::loadUnaligned( response ) / SIMDBands::loadUnaligned( other.response ) );
			}
			
			
			/// Divide this frequency band response by another and modify this response.
			/**
			  * The gains for each band in the frequency band responses are divided.
			  * 
			  * @param other - the FrequencyBandResponse to divide this FrequencyBandResponse object by.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse& operator /= ( const FrequencyBandResponse& other )
			{
				(SIMDBands::loadUnaligned( response ) / SIMDBands::loadUnaligned( other.response )).storeUnaligned( response );
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Frequency Response/Scalar Multiplication Operators
			
			
			/// Multiply this frequency band response by a frequency-independent gain factor and return the result.
			/**
			  * The gain for each band in this frequency band response is multiplied by the specified
			  * constant gain factor.
			  * 
			  * @param other - the gain to multiply with this FrequencyBandResponse object.
			  * @return the result of the multiplication operation.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse operator * ( const Real& gain ) const
			{
				return FrequencyBandResponse( SIMDBands::loadUnaligned( response ) * SIMDBands(gain) );
			}
			
			
			/// Multiply this frequency band response by a frequency-independent gain factor and modify this response.
			/**
			  * The gain for each band in this frequency band response is multiplied by the specified
			  * constant gain factor.
			  * 
			  * @param other - the gain to multiply with this FrequencyBandResponse object.
			  * @return a reference to this FrequencyReponse in order to allow operator chaining.
			  */
			GSOUND_FORCE_INLINE FrequencyBandResponse& operator *= ( const Real& gain )
			{
				(SIMDBands::loadUnaligned( response ) * SIMDBands(gain)).storeUnaligned( response );
				
				return *this;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The number of frequency bands in this frequency band response.
			static const Size numFrequencyBands = GSOUND_FREQUENCY_COUNT;
			
			
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Create a new frequency band response with the values from the specified SIMD array.
			GSOUND_FORCE_INLINE FrequencyBandResponse( const SIMDBands& simd )
			{
				simd.storeUnaligned( response );
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The gain response across the range of this frequency band response.
			Real response[numFrequencyBands];
			
			
		//********************************************************************************
		//******	Friend Functions
			
			
			friend FrequencyBandResponse operator - ( Real, const FrequencyBandResponse& );
			friend FrequencyBandResponse operator + ( Real, const FrequencyBandResponse& );
			friend FrequencyBandResponse operator * ( Real, const FrequencyBandResponse& );
			friend FrequencyBandResponse operator / ( Real, const FrequencyBandResponse& );
			
			
};




//##########################################################################################
//##########################################################################################
//############
//############		Primitive Type Operators.
//############
//##########################################################################################
//##########################################################################################




GSOUND_INLINE FrequencyBandResponse operator + ( Real value, const FrequencyBandResponse& response )
{
	return FrequencyBandResponse( SIMDBands( value ) + SIMDBands::loadUnaligned( response.response ) );
}




GSOUND_INLINE FrequencyBandResponse operator - ( Real value, const FrequencyBandResponse& response )
{
	return FrequencyBandResponse( SIMDBands( value ) - SIMDBands::loadUnaligned( response.response ) );
}




GSOUND_INLINE FrequencyBandResponse operator * ( Real value, const FrequencyBandResponse& response )
{
	return FrequencyBandResponse( SIMDBands( value ) * SIMDBands::loadUnaligned( response.response ) );
}




GSOUND_INLINE FrequencyBandResponse operator / ( Real value, const FrequencyBandResponse& response )
{
	return FrequencyBandResponse( SIMDBands( value ) / SIMDBands::loadUnaligned( response.response ) );
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_FREQUENCY_BAND_RESPONSE_H
