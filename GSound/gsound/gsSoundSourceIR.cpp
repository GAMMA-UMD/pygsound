/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundSourceIR.cpp
 * Contents:    gsound::SoundSourceIR class implementation
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


#include "gsSoundSourceIR.h"


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




SoundSourceIR:: SoundSourceIR()
	:	startTime( math::max<Float>() ),
		length( 0 ),
		reverbTime( 0 )
{
}




SoundSourceIR:: SoundSourceIR( const SoundSource* newSource )
	:	startTime( math::max<Float>() ),
		length( 0 ),
		reverbTime( 0 )
{
	sources.add( newSource );
}




//##########################################################################################
//##########################################################################################
//############
//############		Destructor
//############
//##########################################################################################
//##########################################################################################




SoundSourceIR:: ~SoundSourceIR()
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Path Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void SoundSourceIR:: addPaths( const SoundSourceIR& newPaths )
{
	paths.addAll( newPaths.paths );
	startTime = math::min( startTime, newPaths.getStartTime() );
	length = math::max( length, newPaths.getLength() );
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Reset Method
//############
//##########################################################################################
//##########################################################################################




void SoundSourceIR:: reset()
{
	paths.reset();
	sampledIR.reset();
	startTime = math::max<Float>();
	length = 0;
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Trim Method
//############
//##########################################################################################
//##########################################################################################




Float SoundSourceIR:: trim( const FrequencyBandResponse& thresholdPower )
{
	// Find the total intensity of the sources in this IR.
	const Size numSources = sources.getSize();
	Real totalPower = 0;
	
	for ( Index i = 0; i < numSources; i++ )
		totalPower += sources[i]->getPower();
	
	// Convert the threshold in sound power to threshold in relative intensity.
	FrequencyBandResponse threshold = thresholdPower / totalPower;
	
	Float sampledIRLength = sampledIR.trim( threshold );
	
	return math::max( sampledIRLength, length );
}




//##########################################################################################
//##########################################################################################
//############
//############		Total Intensity Computation Method
//############
//##########################################################################################
//##########################################################################################




FrequencyBandResponse SoundSourceIR:: getTotalIntensity() const
{
	FrequencyBandResponse totalIntensity( Real(0) );
	const Size numPaths = paths.getSize();
	
	for ( Index i = 0; i < numPaths; i++ )
	{
		const SoundPath& path = paths[i];
		totalIntensity += path.getIntensity();
	}
	
	return totalIntensity + sampledIR.getTotalIntensity();
}




FrequencyBandResponse SoundSourceIR:: getPressure() const
{
	FrequencyBandResponse pressureDB = this->getPressureLevel();
	FrequencyBandResponse pressure;
	
	// Convert from linear intensity to SPL in dB.
	for ( Index i = 0; i < pressureDB.getBandCount(); i++ )
		pressure[i] = Real(2e-5)*math::pow( Real(10), pressureDB[i] / Real(20) );
	
	return pressure;
}




FrequencyBandResponse SoundSourceIR:: getPressureLevel() const
{
	FrequencyBandResponse totalIntensity = this->getTotalIntensity();
	
	// Compute the total power of all the sound sources.
	const Size numSources = sources.getSize();
	Real totalPower = 0;
	
	for ( Index i = 0; i < numSources; i++ )
		totalPower += sources[i]->getPower();
	
	// Convert from linear intensity to SPL in dB.
	FrequencyBandResponse totalSPL;
	
	for ( Index i = 0; i < totalIntensity.getBandCount(); i++ )
		totalSPL[i] = Real(10)*math::log10( (totalPower*totalIntensity[i]) / Real(1e-12) );
	
	return totalSPL;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
