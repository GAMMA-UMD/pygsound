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

#include "omSoundFrequencyData.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




FrequencyData:: FrequencyData()
{
}




FrequencyData:: FrequencyData( Float initialGain )
{
	points.add( Point( 20.0f, initialGain ) );
	points.add( Point( 20000.0f, initialGain ) );
}




//##########################################################################################
//##########################################################################################
//############
//############		Frequency Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void FrequencyData:: addPoint( Float frequency, Float gain )
{
	const Size numPoints = points.getSize();
	
	// Handle the fast case where there are no frequencies or this frequency is after the last one.
	if ( numPoints == 0 || frequency > points.getLast().frequency )
	{
		points.add( Point( frequency, gain ) );
		return;
	}
	
	// Find the correct location within the list of points.
	for ( Index i = 0; i < numPoints; i++ )
	{
		if ( frequency < points[i].frequency )
		{
			points.insert( i, Point( frequency, gain ) );
			return;
		}
		else if ( frequency == points[i].frequency )
		{
			// Set the gain for the previously existing frequency point.
			points[i].data = gain;
			return;
		}
	}
}




Bool FrequencyData:: removePoint( Index frequencyIndex )
{
	if ( frequencyIndex < points.getSize() )
		return points.removeAtIndex( frequencyIndex );
	
	return false;
}




Bool FrequencyData:: removeFrequency( Float frequency )
{
	// Find the correct location within the list of points.
	const Size numPoints = points.getSize();
	
	for ( Index i = 0; i < numPoints; i++ )
	{
		if ( frequency == points[i].frequency )
			return points.removeAtIndex( i );
	}
	
	return false;
}




void FrequencyData:: clear()
{
	points.clear();
}




//##########################################################################################
//##########################################################################################
//############
//############		Data Interpolation Method
//############
//##########################################################################################
//##########################################################################################




Float FrequencyData:: interpolate( Float frequency ) const
{
	const Size numPoints = points.getSize();
	
	// If there are no frequencies in this response, return zero.
	if ( numPoints == Size(0) )
		return Float(0);
	else if ( numPoints == Size(1) )
		return points[0].data;
	
	// Handle edge cases.
	if ( frequency <= points[0].frequency )
		return points[0].data;
	else if ( frequency >= points[numPoints - 1].frequency )
		return points[numPoints - 1].data;
	
	for ( Index i = 1; i < numPoints; i++ )
	{
		const Point& f1 = points[i];
		
		if ( frequency < f1.frequency )
		{
			const Point& f0 = points[i-1];
			return lerp( f0.frequency, f1.frequency, f0.data, f1.data, frequency );
		}
	}
	
	return points[numPoints - 1].data;
}




//##########################################################################################
//##########################################################################################
//############
//############		Frequency Band Gain Methods
//############
//##########################################################################################
//##########################################################################################




Float FrequencyData:: getBandAverage( Float lowFrequency, Float highFrequency ) const
{
	const Size numPoints = points.getSize();
	
	// If there are no frequencies in this response, return unity gain.
	if ( numPoints == Size(0) )
		return Float(1);
	else if ( numPoints == Size(1) )
		return points[0].data;
	
	//***********************************************************************
	// Swap the frequencies if they are out of order.
	
	if ( highFrequency < lowFrequency )
	{
		Float temp = highFrequency;
		highFrequency = lowFrequency;
		lowFrequency = temp;
	}
	else if ( lowFrequency == highFrequency )
		return this->interpolate( lowFrequency );
	
	//***********************************************************************
	// Find the low and high limits of the frequency band.
	
	Index lowBand;
	Index highBand;
	for ( lowBand = 0; points[lowBand].frequency < lowFrequency && lowBand < numPoints; lowBand++ );
	for ( highBand = lowBand; highBand < numPoints && points[highBand].frequency < highFrequency; highBand++ );
	
	//***********************************************************************
	// Integrate over the frequency band range.
	
	Float totalGain = 0;
	
	if ( lowBand == highBand )
	{
		if ( lowBand == 0 )
		{
			const Point& f0 = points[lowBand];
			totalGain += trapezoid( lowFrequency, highFrequency, f0.data, f0.data );
		}
		else if ( highBand == numPoints )
		{
			const Point& f0 = points[highBand-1];
			totalGain += trapezoid( lowFrequency, highFrequency, f0.data, f0.data );
		}
		else
		{
			const Point& f0 = points[lowBand-1];
			const Point& f1 = points[highBand];
			const Float lowGain = lerp( f0.frequency, f1.frequency, f0.data, f1.data, lowFrequency );
			const Float highGain = lerp( f0.frequency, f1.frequency, f0.data, f1.data, highFrequency );
			totalGain += trapezoid( lowFrequency, highFrequency, lowGain, highGain );
		}
	}
	else
	{
		// Integrate the low part.
		if ( lowBand == 0 )
		{
			const Point& f1 = points[lowBand];
			totalGain += trapezoid( lowFrequency, f1.frequency, f1.data, f1.data );
		}
		else
		{
			const Point& f0 = points[lowBand-1];
			const Point& f1 = points[lowBand];
			const Float lowGain = lerp( f0.frequency, f1.frequency, f0.data, f1.data, lowFrequency );
			totalGain += trapezoid( lowFrequency, f1.frequency, lowGain, f1.data );
		}
		
		// Integrate the high part.
		if ( highBand == numPoints )
		{
			const Point& f0 = points[highBand-1];
			totalGain += trapezoid( f0.frequency, highFrequency, f0.data, f0.data );
		}
		else
		{
			const Point& f0 = points[highBand-1];
			const Point& f1 = points[highBand];
			const Float highGain = lerp( f0.frequency, f1.frequency, f0.data, f1.data, highFrequency );
			totalGain += trapezoid( f0.frequency, highFrequency, f0.data, highGain );
		}
		
		// Integrate the middle part.
		for ( Index i = lowBand; i < highBand - 1; i++ )
		{
			const Point& f0 = points[i];
			const Point& f1 = points[i+1];
			totalGain += trapezoid( f0.frequency, f1.frequency, f0.data, f1.data );
		}
	}
	
	// Divide by the total frequency range.
	return totalGain / (highFrequency - lowFrequency);
}




//##########################################################################################
//##########################################################################################
//############
//############		Max Gain Accessor Method
//############
//##########################################################################################
//##########################################################################################




Float FrequencyData:: getMax() const
{
	const Size numPoints = points.getSize();
	
	// If there are no frequencies in this response, return unity gain.
	if ( numPoints == Size(0) )
		return Float(1);
	
	// Compute the maximum.
	Float maxGain = points[0].data;
	
	for ( Index i = 1; i < numPoints; i++ )
		maxGain = math::max( maxGain, points[i].data );
	
	return maxGain;
}




//##########################################################################################
//##########################################################################################
//############
//############		Average Gain Accessor Method
//############
//##########################################################################################
//##########################################################################################




Float FrequencyData:: getAverage() const
{
	const Size numPoints = points.getSize();
	
	// If there are no frequencies in this response, return unity gain.
	if ( numPoints == Size(0) )
		return Float(1);
	else if ( numPoints == Size(1) )
		return points[0].data;
	
	// Integrate the gain over the entire response using the trapezoid rule.
	const Float minF = points[0].frequency;
	const Float maxF = points.getLast().frequency;
	Float totalGain = 0;
	
	for ( Index i = 1; i < numPoints; i++ )
	{
		const Point& f0 = points[i-1];
		const Point& f1 = points[i];
		totalGain += trapezoid( f0.frequency, f1.frequency, f0.data, f1.data );
	}
	
	// Divide by the total frequency range.
	return totalGain / (maxF - minF);
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
