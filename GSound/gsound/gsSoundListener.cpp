/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundListener.cpp
 * Contents:    gsound::SoundListener class implementation
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


#include "gsSoundListener.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const Float SoundListener:: DEFAULT_SENSITIVITY = Float(42);
const Float SoundListener:: DEFAULT_THRESHOLD_BIAS = Float(-10);
const FrequencyResponse SoundListener:: DEFAULT_THRESHOLD = getHearingThresholdResponse( Real(20), Real(20000), 31 );


//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




SoundListener:: SoundListener()
	:	SoundDetector(),
		flags( SoundListenerFlags::DEFAULT ),
		sensitivity( DEFAULT_SENSITIVITY ),
		thresholdBias( DEFAULT_THRESHOLD_BIAS )
{
	getHearingThresholdResponse( Real(20), Real(20000), 31, threshold );
}




SoundListener:: SoundListener( const Vector3f& newPosition, Real newRadius )
	:	SoundDetector( newPosition, newRadius ),
		flags( SoundListenerFlags::DEFAULT ),
		sensitivity( DEFAULT_SENSITIVITY ),
		thresholdBias( DEFAULT_THRESHOLD_BIAS )
{
	getHearingThresholdResponse( Real(20), Real(20000), 31, threshold );
}




//##########################################################################################
//##########################################################################################
//############
//############		Threshold Power Accessor Method
//############
//##########################################################################################
//##########################################################################################




FrequencyBandResponse SoundListener:: getThresholdPower( const FrequencyBands& frequencies ) const
{
	FrequencyResponse thresholdPower = threshold;
	
	for ( Index i = 0; i < thresholdPower.getFrequencyCount(); i++ )
	{
		const Real thresholdDBSPL = thresholdPower.getFrequencyGain(i) + thresholdBias;
		//const Real thresholdPowerDB = thresholdDBSPL + Real(10)*math::log10( Real(4)*gsound::math::pi<Real>() );
		const Real thresholdI = Real(1e-12)*math::pow( Real(10), thresholdDBSPL / Real(10) );
		
		thresholdPower.setFrequencyGain( i, thresholdI );
	}
	
	return thresholdPower.getBandResponse( frequencies );
}




//##########################################################################################
//##########################################################################################
//############
//############		Hearing Threshold Response Methods
//############
//##########################################################################################
//##########################################################################################




Real SoundListener:: getHearingThreshold( Real frequency )
{
	Real f1000 = frequency / Real(1000);
	return Real(3.64)*math::pow( f1000, Real(-0.8) ) - 
			Real(6.5)*math::exp( Real(-0.6)*math::square(f1000 - Real(3.3)) ) +
			Real(1e-3)*math::square(f1000)*math::square(f1000);
}




void SoundListener:: getHearingThresholdResponse( Real minFreq, Real maxFreq, Size numBands, FrequencyResponse& response )
{
	response.reset();
	
	const Real lnMinF = math::ln( minFreq );
	const Real lnMaxF = math::ln( maxFreq );
	
	for ( Index i = 0; i < numBands; i++ )
	{
		const Real a = (Real(i) + Real(0.5)) / Real(numBands);
		const Real lnF = lnMinF + a*(lnMaxF - lnMinF);
		const Real f = math::exp( lnF );
		const Real fThreshold = getHearingThreshold( f );
		
		response.setFrequency( f, fThreshold );
	}
}




FrequencyResponse SoundListener:: getHearingThresholdResponse( Real minFreq, Real maxFreq, Size numBands )
{
	FrequencyResponse response;
	
	getHearingThresholdResponse( minFreq, maxFreq, numBands, response );
	
	return response;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
