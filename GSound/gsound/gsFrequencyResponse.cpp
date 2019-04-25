/*
 * Project:     GSound
 * 
 * File:        gsound/gsFrequencyResponse.cpp
 * Contents:    gsound::FrequencyResponse class implementation
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


#include "gsFrequencyResponse.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




FrequencyResponse:: FrequencyResponse()
{
}




FrequencyResponse:: FrequencyResponse( Real initialGain )
{
	// If the desired flat gain is not the default, add two points to the frequency response.
	if ( initialGain != Real(1) )
	{
		frequencies.add( Frequency( 20.0f, initialGain ) );
		frequencies.add( Frequency( 20000.0f, initialGain ) );
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		Frequency Accessor Methods
//############
//##########################################################################################
//##########################################################################################




Real FrequencyResponse:: getFrequencyGain( Real frequency ) const
{
	const Size numFrequencies = frequencies.getSize();
	
	// If there are no frequencies in this response, return unity gain.
	if ( numFrequencies == Size(0) )
		return Real(1);
	else if ( numFrequencies == Size(1) )
		return frequencies[0].gain;
	
	// Handle edge cases.
	if ( frequency <= frequencies[0].frequency )
		return frequencies[0].gain;
	else if ( frequency >= frequencies[numFrequencies - 1].frequency )
		return frequencies[numFrequencies - 1].gain;
	
	for ( Index i = 1; i < numFrequencies; i++ )
	{
		const Frequency& f1 = frequencies[i];
		
		if ( frequency < f1.frequency )
		{
			const Frequency& f0 = frequencies[i-1];
			return lerp( f0.frequency, f1.frequency, f0.gain, f1.gain, frequency );
		}
	}
	
	return frequencies[numFrequencies - 1].gain;
}




void FrequencyResponse:: setFrequency( Real frequency, Real gain )
{
	const Size numFrequencies = frequencies.getSize();
	
	// Handle the fast case where there are no frequencies or this frequency is after the last one.
	if ( numFrequencies == 0 || frequency > frequencies.getLast().frequency )
	{
		frequencies.add( Frequency( frequency, gain ) );
		return;
	}
	
	// Find the correct location within the list of frequencies.
	for ( Index i = 0; i < numFrequencies; i++ )
	{
		if ( frequency < frequencies[i].frequency )
		{
			frequencies.insert( i, Frequency( frequency, gain ) );
			return;
		}
		else if ( frequency == frequencies[i].frequency )
		{
			// Set the gain for the previously existing frequency point.
			frequencies[i].gain = gain;
			return;
		}
	}
}




Bool FrequencyResponse:: removeFrequency( Index frequencyIndex )
{
	if ( frequencyIndex < frequencies.getSize() )
		return frequencies.removeAtIndex( frequencyIndex );
	
	return false;
}




Bool FrequencyResponse:: removeFrequency( Real frequency )
{
	// Find the correct location within the list of frequencies.
	const Size numFrequencies = frequencies.getSize();
	
	for ( Index i = 0; i < numFrequencies; i++ )
	{
		if ( frequency == frequencies[i].frequency )
			return frequencies.removeAtIndex( i );
	}
	
	return false;
}




void FrequencyResponse:: reset( Real initialGain )
{
	frequencies.clear();
	
	// If the desired flat gain is not the default, add two points to the frequency response.
	if ( initialGain != Real(1) )
	{
		frequencies.add( Frequency( 20.0f, initialGain ) );
		frequencies.add( Frequency( 20000.0f, initialGain ) );
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		Frequency Band Gain Methods
//############
//##########################################################################################
//##########################################################################################




Real FrequencyResponse:: getBandGain( Real lowFrequency, Real highFrequency ) const
{
	const Size numFrequencies = frequencies.getSize();
	
	// If there are no frequencies in this response, return unity gain.
	if ( numFrequencies == Size(0) )
		return Real(1);
	else if ( numFrequencies == Size(1) )
		return frequencies[0].gain;
	
	//***********************************************************************
	// Swap the frequencies if they are out of order.
	
	if ( highFrequency < lowFrequency )
	{
		Real temp = highFrequency;
		highFrequency = lowFrequency;
		lowFrequency = temp;
	}
	else if ( lowFrequency == highFrequency )
		return this->getFrequencyGain( lowFrequency );
	
	//***********************************************************************
	// Find the low and high limits of the frequency band.
	
	Index lowBand;
	Index highBand;
	for ( lowBand = 0; frequencies[lowBand].frequency < lowFrequency && lowBand < numFrequencies; lowBand++ );
	for ( highBand = lowBand; frequencies[highBand].frequency < highFrequency && highBand < numFrequencies; highBand++ );
	
	Real totalGain = 0;
	
	//***********************************************************************
	// Handle the integration edge cases.
	
	if ( lowBand > 0 )
	{
		const Frequency& f0 = frequencies[lowBand-1];
		const Frequency& f1 = frequencies[lowBand];
		const Real lowGain = lerp( f0.frequency, f1.frequency, f0.gain, f1.gain, lowFrequency );
		totalGain += trapezoid( lowFrequency, f1.frequency, lowGain, f1.gain );
	}
	else
	{
		const Frequency& f1 = frequencies[lowBand];
		totalGain += trapezoid( lowFrequency, f1.frequency, f1.gain, f1.gain );
	}
	
	if ( highBand < numFrequencies )
	{
		const Frequency& f0 = frequencies[highBand-1];
		const Frequency& f1 = frequencies[highBand];
		const Real highGain = lerp( f0.frequency, f1.frequency, f0.gain, f1.gain, highFrequency );
		totalGain += trapezoid( f0.frequency, highFrequency, f0.gain, highGain );
	}
	else
	{
		const Frequency& f0 = frequencies[highBand-1];
		totalGain += trapezoid( f0.frequency, highFrequency, f0.gain, f0.gain );
	}
	
	//***********************************************************************
	// Integrate the middle of the band.
	
	for ( Index i = lowBand; i < highBand - 1; i++ )
	{
		const Frequency& f0 = frequencies[i];
		const Frequency& f1 = frequencies[i+1];
		totalGain += trapezoid( f0.frequency, f1.frequency, f0.gain, f1.gain );
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




Real FrequencyResponse:: getMax() const
{
	const Size numFrequencies = frequencies.getSize();
	
	// If there are no frequencies in this response, return unity gain.
	if ( numFrequencies == Size(0) )
		return Real(1);
	
	// Compute the maximum.
	Real maxGain = frequencies[0].gain;
	
	for ( Index i = 1; i < numFrequencies; i++ )
		maxGain = math::max( maxGain, frequencies[i].gain );
	
	return maxGain;
}




//##########################################################################################
//##########################################################################################
//############
//############		Average Gain Accessor Method
//############
//##########################################################################################
//##########################################################################################




Real FrequencyResponse:: getAverage() const
{
	const Size numFrequencies = frequencies.getSize();
	
	// If there are no frequencies in this response, return unity gain.
	if ( numFrequencies == Size(0) )
		return Real(1);
	else if ( numFrequencies == Size(1) )
		return frequencies[0].gain;
	
	// Integrate the gain over the entire response using the trapezoid rule.
	const Real minF = frequencies[0].frequency;
	const Real maxF = frequencies.getLast().frequency;
	Real totalGain = 0;
	
	for ( Index i = 1; i < numFrequencies; i++ )
	{
		const Frequency& f0 = frequencies[i-1];
		const Frequency& f1 = frequencies[i];
		totalGain += trapezoid( f0.frequency, f1.frequency, f0.gain, f1.gain );
	}
	
	// Divide by the total frequency range.
	return totalGain / (maxF - minF);
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
