/*
 * Project:     GSound
 * 
 * File:        gsound/gsFrequencyResponse.h
 * Contents:    gsound::FrequencyResponse class declaration
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


#ifndef INCLUDE_GSOUND_FREQUENCY_RESPONSE_H
#define INCLUDE_GSOUND_FREQUENCY_RESPONSE_H


#include "gsConfig.h"


#include "gsFrequencyBands.h"
#include "gsFrequencyBandResponse.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a frequency response over an arbitrary number of frequencies.
/**
  * This class is used to store generic band-independent material properties for
  * sound meshes. A frequency response is a list of frequency/amplitude pairs,
  * sorted by increasing frequency. During sound propagation, a frequency response
  * is approximated by a fixed number (i.e. 4 or 8) frequency bands that average
  * the amplitude over each frequency range.
  */
class FrequencyResponse
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default frequency band response with unity gain (=1) across all frequencies.
			FrequencyResponse();
			
			
			/// Create a frequency band response with the specified gain across all frequencies.
			/**
			  * This constructor creates a response with two frequency response points: 20 Hz
			  * and 20 kHz with the given gain value.
			  *
			  * @param initialGain - the initial gain value used for all frequency bands.
			  */
			FrequencyResponse( Real initialGain );
			
			
		//********************************************************************************
		//******	Frequency Accessor Methods
			
			
			/// Return the number of frequency response data points there are in this response.
			GSOUND_INLINE Size getFrequencyCount() const
			{
				return frequencies.getSize();
			}
			
			
			/// Return the frequency for the specified frequency index within this response.
			/**
			  * The frequency index must be less than the total number of frequencies in the response.
			  */
			GSOUND_INLINE Real getFrequency( Index frequencyIndex ) const
			{
				return frequencies[frequencyIndex].frequency;
			}
			
			
			/// Return the gain coefficient for the specified frequency index within this response.
			/**
			  * The frequency index must be less than the total number of frequencies in the response.
			  */
			GSOUND_INLINE Real getFrequencyGain( Index frequencyIndex ) const
			{
				return frequencies[frequencyIndex].gain;
			}
			
			
			/// Set the gain coefficient for the specified frequency index within this response.
			/**
			  * The frequency index must be less than the total number of frequencies in the response.
			  */
			GSOUND_INLINE void setFrequencyGain( Index frequencyIndex, Real gain )
			{
				frequencies[frequencyIndex].gain = gain;
			}
			
			
			/// Return the interpolated gain for the specified frequency.
			/**
			  * This method interpolates between the two nearest frequencies to the specified frequency.
			  * It returns the linearly interpolated gain value of this response at that frequency.
			  */
			Real getFrequencyGain( Real frequency ) const;
			
			
			/// Add the specified frequency response point to this response.
			/**
			  * The new frequency is inserted in the internal list of frequencies at the
			  * correct sorted position in the list. If there is a previously-existing
			  * frequency data point for that frequency, the gain replaces the previous
			  * gain without adding a new point.
			  */
			void setFrequency( Real frequency, Real gain );
			
			
			/// Remove the frequency data point at the specified index in this response.
			/**
			  * The method returns whether or not the frequency at that index was able
			  * to be removed.
			  */
			Bool removeFrequency( Index frequencyIndex );
			
			
			/// Remove the frequency data point with the specified frequency in this response.
			/**
			  * If there is no frequency data point with that frequency, FALSE is returned
			  * and the response is not modified. Otherwise, the method succeeds and TRUE
			  * is returned.
			  */
			Bool removeFrequency( Real frequency );
			
			
			/// Remove all frequency data points from this response, resulting in a response with the given gain.
			void reset( Real initialGain = Real(1) );
			
			
		//********************************************************************************
		//******	Frequency Band Gain Methods
			
			
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
			Real getBandGain( Real lowFrequency, Real highFrequency ) const;
			
			
			/// Return the average gain over the specified frequency range.
			GSOUND_INLINE Real getBandGain( const AABB1f& bandRange ) const
			{
				return this->getBandGain( bandRange.min, bandRange.max );
			}
			
			
			/// Convert this generic response to a response for a set of frequency bands.
			/**
			  * This method averages the gain over each output frequency band in this response
			  * to get the resulting output frequency band response.
			  */
			GSOUND_INLINE FrequencyBandResponse getBandResponse( const FrequencyBands& frequencies ) const
			{
				Real result[GSOUND_FREQUENCY_COUNT];
				
				for ( Index b = 0; b < frequencies.getBandCount(); b++ )
					result[b] = this->getFrequencyGain( frequencies[b] );
				
				return FrequencyBandResponse(result);
			}
			
			
		//********************************************************************************
		//******	Max Gain Method
			
			
			/// Return the maximum value over all frequencies.
			Real getMax() const;
			
			
			/// Return the average gain of this FrequencyResponse over its entire range.
			/**
			  * This method integrates the interpolated gain coefficients across the
			  * frequency response and returns the average coefficient.
			  * 
			  * @return the average gain of all frequencies in this FrequencyResponse.
			  */
			Real getAverage() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that respresents a single point in a generic frequency response.
			class Frequency
			{
				public:
					
					/// Create a new frequency response point with the specified frequency and response.
					GSOUND_INLINE Frequency( Real newFrequency, Real newGain )
						:	frequency( newFrequency ),
							gain( newGain )
					{
					}
					
					
					/// The frequency for this frequency response point.
					Real frequency;
					
					
					/// The linear gain coefficient for this frequency response point.
					Real gain;
					
			};
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Linearly interpolate between the gain coefficients at two different frequencies.
			GSOUND_FORCE_INLINE static Real lerp( Real f0, Real f1, Real g0, Real g1, Real f )
			{
				return g0 + (g1 - g0)*((f - f0)/(f1 - f0));
			}
			
			
			/// Compute the area of the trapezoid for the specified frequency data points.
			GSOUND_FORCE_INLINE static Real trapezoid( Real f0, Real f1, Real g0, Real g1 )
			{
				return Real(0.5)*(g0 + g1)*(f1 - f0);
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the frequencies in this frequency response and their associated gain coefficients.
			ArrayList<Frequency> frequencies;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_FREQUENCY_RESPONSE_H
