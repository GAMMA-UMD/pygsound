/*
 * Project:     GSound
 * 
 * File:        gsound/gsIRMetrics.h
 * Contents:    gsound::IRMetrics class declaration
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


#ifndef INCLUDE_GSOUND_IR_METRICS_H
#define INCLUDE_GSOUND_IR_METRICS_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that contains various standard acoustic metrics.
class IRMetrics
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new IR metrics object with the default metric values.
			GSOUND_INLINE IRMetrics()
				:	t60( 0.0f ),
					edt( 0.0f ),
					c50( 0.0f ),
					c80( 0.0f ),
					d50( 0.0f ),
					g( 0.0f ),
					ts( 0.0f )
			{
			}
			
			
		//********************************************************************************
		//******	Public Methods
			
			
			GSOUND_INLINE static Bool writeTextFile( const char* irMetricsFilePath, const IRMetrics& metrics );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The reverberation time, the time in seconds that it takes for the sound energy to decay by 60dB after the first 5 dB of decay.
			FrequencyBandResponse t60;
			
			
			/// The early decay time, obtained from the first 10dB of the IR's decay from its peak level.
			FrequencyBandResponse edt;
			
			
			/// The difference in decibels between the energy arriving in the first 50ms to the late energy.
			FrequencyBandResponse c50;
			
			
			/// The difference in decibels between the energy arriving in the first 80ms to the late energy.
			FrequencyBandResponse c80;
			
			
			/// The ratio of sound energy from the first 50ms to total sound energy, measured as a fraction between 0 and 1.
			FrequencyBandResponse d50;
			
			
			/// The sound strength, G, measured in decibels.
			FrequencyBandResponse g;
			
			
			/// The center time, measured in seconds, the center of mass of the energy in the impulse response.
			FrequencyBandResponse ts;
			
			
			
};




Bool IRMetrics:: writeTextFile( const char* irMetricsFilePath, const IRMetrics& metrics )
{
	FILE* irMetricsFile = fopen( irMetricsFilePath, "w" );
	
	if ( irMetricsFile == NULL )
		return false;
	
	fprintf( irMetricsFile, "T60\n" );
	for ( Index b = 0; b < metrics.t60.getBandCount(); b++ )
	{
		fprintf( irMetricsFile, "%f", metrics.t60[b] );
		
		if ( b != metrics.t60.getBandCount()-1 )
			fprintf( irMetricsFile, "\t" );
	}
	
	fprintf( irMetricsFile, "\n" );
	fprintf( irMetricsFile, "\nEDT\n" );
	for ( Index b = 0; b < metrics.edt.getBandCount(); b++ )
	{
		fprintf( irMetricsFile, "%f", metrics.edt[b] );
		
		if ( b != metrics.edt.getBandCount()-1 )
			fprintf( irMetricsFile, "\t" );
	}
	
	fprintf( irMetricsFile, "\n" );
	fprintf( irMetricsFile, "\nC50\n" );
	for ( Index b = 0; b < metrics.c50.getBandCount(); b++ )
	{
		fprintf( irMetricsFile, "%f", metrics.c50[b] );
		
		if ( b != metrics.c50.getBandCount()-1 )
			fprintf( irMetricsFile, "\t" );
	}
	
	fprintf( irMetricsFile, "\n" );
	fprintf( irMetricsFile, "\nC80\n" );
	for ( Index b = 0; b < metrics.c80.getBandCount(); b++ )
	{
		fprintf( irMetricsFile, "%f", metrics.c80[b] );
		
		if ( b != metrics.c80.getBandCount()-1 )
			fprintf( irMetricsFile, "\t" );
	}
	
	fprintf( irMetricsFile, "\n" );
	fprintf( irMetricsFile, "\nD50\n" );
	for ( Index b = 0; b < metrics.d50.getBandCount(); b++ )
	{
		fprintf( irMetricsFile, "%f", metrics.d50[b]*100 );
		
		if ( b != metrics.d50.getBandCount()-1 )
			fprintf( irMetricsFile, "\t" );
	}
	
	fprintf( irMetricsFile, "\n" );
	fprintf( irMetricsFile, "\nG\n" );
	for ( Index b = 0; b < metrics.g.getBandCount(); b++ )
	{
		fprintf( irMetricsFile, "%f", metrics.g[b]*1000 );
		
		if ( b != metrics.g.getBandCount()-1 )
			fprintf( irMetricsFile, "\t" );
	}
	
	fprintf( irMetricsFile, "\n" );
	fprintf( irMetricsFile, "\nTS\n" );
	for ( Index b = 0; b < metrics.ts.getBandCount(); b++ )
	{
		fprintf( irMetricsFile, "%f", metrics.ts[b]*1000 );
		
		if ( b != metrics.ts.getBandCount()-1 )
			fprintf( irMetricsFile, "\t" );
	}
	
	fclose( irMetricsFile );
	
	return true;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_IR_METRICS_H
