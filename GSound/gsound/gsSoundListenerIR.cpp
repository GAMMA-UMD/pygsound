/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundListenerIR.cpp
 * Contents:    gsound::SoundListenerIR class implementation
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


#include "gsSoundListenerIR.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############
//############		Constructor
//############
//##########################################################################################
//##########################################################################################




SoundListenerIR:: SoundListenerIR()
	:	numSources( 0 ),
		listener( NULL )
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Destructor
//############
//##########################################################################################
//##########################################################################################




SoundListenerIR:: ~SoundListenerIR()
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Source Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void SoundListenerIR:: setSourceCount( Size newNumSources )
{
	if ( sourceIRs.getSize() < newNumSources )
		sourceIRs.setSize( newNumSources );
	
	numSources = newNumSources;
}




//##########################################################################################
//##########################################################################################
//############
//############		Clear Methods
//############
//##########################################################################################
//##########################################################################################




void SoundListenerIR:: clear()
{
	for ( Index i = 0; i < sourceIRs.getSize(); i++ )
	{
		SoundSourceIR& sourceIR = sourceIRs[i];
		
		sourceIR.clear();
		sourceIR.clearSources();
	}
}




void SoundListenerIR:: reset()
{
	for ( Index i = 0; i < sourceIRs.getSize(); i++ )
	{
		SoundSourceIR& sourceIR = sourceIRs[i];
		
		sourceIR.reset();
		sourceIR.clearSources();
	}
	
	numSources = 0;
}




//##########################################################################################
//##########################################################################################
//############
//############		Path Count Accessor Methods
//############
//##########################################################################################
//##########################################################################################




Size SoundListenerIR:: getPathCount() const
{
	Size total = 0;
	
	for ( Index i = 0; i < sourceIRs.getSize(); i++ )
		total += sourceIRs[i].getPathCount();
	
	return total;
}



 
//##########################################################################################
//##########################################################################################
//############
//############		IR Trim Method
//############
//##########################################################################################
//##########################################################################################




void SoundListenerIR:: trim()
{
	// Convert the threshold in dB SPL to threshold in sound power.
	const FrequencyBandResponse thresholdPower = listener->getThresholdPower( frequencies );
	
	// Trim each source IR based on this threshold.
	const Size numSourceIRs = sourceIRs.getSize();
	
	for ( Index i = 0; i < numSourceIRs; i++ )
		sourceIRs[i].trim( thresholdPower );
}




//##########################################################################################
//##########################################################################################
//############
//############		Pressure Accessor Methods
//############
//##########################################################################################
//##########################################################################################




FrequencyBandResponse SoundListenerIR:: getPressure() const
{
	// Compute the total pressure for all sound sources.
	FrequencyBandResponse pressure = Real(0);
	
	for ( Index i = 0; i < sourceIRs.getSize(); i++ )
		pressure += sourceIRs[i].getPressure();
	
	return pressure;
}




FrequencyBandResponse SoundListenerIR:: getPressureLevel() const
{
	FrequencyBandResponse pressure = this->getPressure();
	FrequencyBandResponse pressureDB;
	
	// Convert from linear pressure to SPL in dB.
	for ( Index i = 0; i < pressure.getBandCount(); i++ )
	{
		if ( pressure[i] > Real(2e-5) )
			pressureDB[i] = Real(20)*math::log10( pressure[i] / Real(2e-5) );
		else
			pressureDB[i] = Real(-1000);
	}
	
	return pressureDB;
}




//##########################################################################################
//##########################################################################################
//############
//############		Storage Size Accessor Methods
//############
//##########################################################################################
//##########################################################################################




Size SoundListenerIR:: getSizeInBytes() const
{
	Size totalSize = 0;
	
	for ( Index i = 0; i < sourceIRs.getSize(); i++ )
		totalSize += sourceIRs[i].getSizeInBytes();
	
	return totalSize + sizeof(SoundListenerIR);
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
