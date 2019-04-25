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

#ifndef INCLUDE_OM_SOUND_FREQUENCY_DATA_H
#define INCLUDE_OM_SOUND_FREQUENCY_DATA_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a sequence of frequency-dependent values in sorted order.
/**
  * The typical use of this class is to store a frequency response that is sampled
  * at irregular intervals.
  */
class FrequencyData
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a frequency data response with no data points.
			FrequencyData();
			
			
			/// Create a frequency data response with the specified value across all frequencies.
			/**
			  * This constructor creates a response with two frequency response points: 20 Hz
			  * and 20 kHz with the given value.
			  *
			  * @param data - the initial data value used for all frequencies.
			  */
			FrequencyData( Float data );
			
			
		//********************************************************************************
		//******	Data Point Accessor Methods
			
			
			/// Return the number of frequency data points there are in this response.
			OM_INLINE Size getPointCount() const
			{
				return points.getSize();
			}
			
			
			/// Return the frequency for the specified point index within this response.
			/**
			  * The frequency index must be less than the total number of frequencies in the response.
			  */
			OM_INLINE Float getFrequency( Index pointIndex ) const
			{
				return points[pointIndex].frequency;
			}
			
			
			/// Return the data value for the specified point index within this response.
			/**
			  * The frequency index must be less than the total number of frequencies in the response.
			  */
			OM_INLINE Float getData( Index pointIndex ) const
			{
				return points[pointIndex].data;
			}
			
			
			/// Set the data value for the specified point index within this response.
			/**
			  * The frequency index must be less than the total number of frequencies in the response.
			  */
			OM_INLINE void setData( Index pointIndex, Float data )
			{
				points[pointIndex].data = data;
			}
			
			
			/// Add the specified frequency data point to this response.
			/**
			  * The new frequency is inserted in the internal list of frequencies at the
			  * correct sorted position in the list. If there is a previously-existing
			  * frequency data point for that frequency, the data replaces the previous
			  * data without adding a new point.
			  *
			  * It is most efficient to add data points in sorted order.
			  */
			void addPoint( Float frequency, Float data );
			
			
			/// Remove the frequency data point at the specified index in this response.
			/**
			  * The method returns whether or not the frequency at that index was able
			  * to be removed.
			  */
			Bool removePoint( Index pointIndex );
			
			
			/// Remove the data point with the specified frequency in this response.
			/**
			  * If there is no data point with that frequency, FALSE is returned
			  * and the response is not modified. Otherwise, the method succeeds and TRUE
			  * is returned.
			  */
			Bool removeFrequency( Float frequency );
			
			
			/// Remove all frequency data points from this frequency data response.
			void clear();
			
			
		//********************************************************************************
		//******	Data Interpolation Method
			
			
			/// Return the interpolated gain for the specified frequency.
			/**
			  * This method interpolates between the two nearest frequencies to the specified frequency.
			  * It returns the linearly interpolated gain value of this response at that frequency.
			  *
			  * If there are no data points, a value of 0 is returned.
			  */
			Float interpolate( Float frequency ) const;
			
			
		//********************************************************************************
		//******	Average Methods
			
			
			/// Return the maximum value over all frequencies.
			Float getMax() const;
			
			
			/// Return the average gain of this FrequencyData over its entire range.
			/**
			  * This method integrates the interpolated gain coefficients across the
			  * frequency response and returns the average coefficient.
			  * 
			  * @return the average data value over all frequencies.
			  */
			Float getAverage() const;
			
			
			/// Return the average data value over the specified frequency range.
			/**
			  * This method is useful when mapping higher-resolution frequency data
			  * down to lower-resolution frequency data. The band boundaries are clamped
			  * to the minimum and maximum frequencies in this frequency data response and
			  * will be swaped if they are not specified in ascending order.
			  *
			  * The method returns the average value over the specified frequency band
			  * range via trapezoidal integration.
			  */
			Float getBandAverage( Float minFrequency, Float maxFrequency ) const;
			
			
			/// Return the average data value over the specified frequency range.
			OM_INLINE Float getBandAverage( const AABB1f& bandRange ) const
			{
				return this->getBandAverage( bandRange.min, bandRange.max );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that stores a data point for a specific frequency.
			class Point
			{
				public:
					
					/// Create a new frequency data point with the specified frequency and data.
					OM_INLINE Point( Float newFrequency, Float newData )
						:	frequency( newFrequency ),
							data( newData )
					{
					}
					
					
					/// The frequency for this frequency response point.
					Float frequency;
					
					
					/// The linear gain coefficient for this frequency response point.
					Float data;
					
			};
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Linearly interpolate between the gain coefficients at two different frequencies.
			OM_FORCE_INLINE static Float lerp( Float f0, Float f1, Float g0, Float g1, Float f )
			{
				return g0 + (g1 - g0)*((f - f0)/(f1 - f0));
			}
			
			
			/// Compute the area of the trapezoid for the specified frequency data points.
			OM_FORCE_INLINE static Float trapezoid( Float f0, Float f1, Float g0, Float g1 )
			{
				return Float(0.5)*(g0 + g1)*(f1 - f0);
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the frequency data points sorted in increasing order by frequency.
			ArrayList<Point> points;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FREQUENCY_DATA_H
