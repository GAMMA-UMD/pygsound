/*
 * Project:     GSound
 * 
 * File:        gsound/gsFrequencyBandResponse.cpp
 * Contents:    gsound::FrequencyBandResponse class implementation
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


#include "gsFrequencyBandResponse.h"

 
//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




Real FrequencyBandResponse:: getFrequencyGain( Real frequency, const FrequencyBands& frequencies ) const
{
	// Handle edge cases.
	if ( frequency <= frequencies[0] )
		return response[0];
	else if ( frequency >= frequencies[numFrequencyBands - 1] )
		return response[numFrequencyBands - 1];
	
	// Find the band frequencies that bound the desired frequency.
	for ( Index i = 1; i < numFrequencyBands; i++ )
	{
		Real f1 = frequencies[i];
		
		if ( frequency < f1 )
		{
			// Interpolate the gain value at the desired frequency.
			Real f0 = frequencies[i-1];
			Real width = f1 - f0;
			Real a = (frequency - f0)/width;
			return response[i-1] + a*(response[i] - response[i-1]);
		}
	}
	
	return response[numFrequencyBands - 1];
}




Real FrequencyBandResponse:: getBandGain( Real lowFrequency, Real highFrequency, const FrequencyBands& frequencies ) const
{
	lowFrequency = math::clamp( lowFrequency, frequencies[0],
								frequencies[numFrequencyBands - 1] );
	
	highFrequency = math::clamp( highFrequency, frequencies[0],
								frequencies[numFrequencyBands - 1] );
	
	// Swap the frequencies if they are out of order.
	if ( highFrequency < lowFrequency )
	{
		Real temp = highFrequency;
		highFrequency = lowFrequency;
		lowFrequency = temp;
	}
	
	Index i = 0;
	Index lowBand;
	Index highBand;
	
	for ( ; frequencies[i] < lowFrequency; i++ );
	
	lowBand = i;
	
	for ( ; frequencies[i] < highFrequency; i++ );
	
	highBand = i;
	
	Real average = Real(0);
	
	for ( Index i = lowBand; i <= highBand; i++ )
		average += response[i];
	
	return average / Real(highBand - lowBand + 1);
}




FrequencyBandResponse FrequencyBandResponse:: convertTo( const FrequencyBands& frequenciesIn, const FrequencyBands& frequenciesOut ) const
{
	Real result[GSOUND_FREQUENCY_COUNT];
	
	for ( Index b = 0; b < frequenciesOut.getBandCount(); b++ )
		result[b] = getFrequencyGain( frequenciesOut[b], frequenciesIn );
	
	return FrequencyBandResponse(result);
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
