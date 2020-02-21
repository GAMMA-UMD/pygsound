/*
 * Project:     GSound
 * 
 * File:        gsound/gsPropagationRequest.cpp
 * Contents:    gsound::PropagationRequest class implementation
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


#include "gsPropagationRequest.h"


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




PropagationRequest:: PropagationRequest()
	:	// Main parameters.
		flags( PropagationFlags::DEFAULT ),
		dt( 0.0f ),
		targetDt( 1.0f / 15.0f ),
		quality( 1 ),
		minQuality( 0.25f ),
		maxQuality( 1.5f ),
		minIRLength( 0.2f ),
		maxIRLength( 5.0f ),
		irGrowthRate( 2.0f ),
		numThreads( math::max( Size(CPU::getCount()*Float(0.5)), Size(1) ) ),
		debugFlags( DebugFlags::UNDEFINED ),
		debugCache( NULL ),
		statistics( NULL ),
		
		// Rendering parameters.
		sampleRate( 44100.0 ),
		dopplerThreshold( 10.0f ),
		
		// Ray-tracing parameters.
		numDirectRays( 200 ),
		maxDiffractionDepth( 0 ),
		maxDiffractionOrder( 3 ),
		maxSpecularDepth( 5 ),
		numSpecularRays( 1000 ),
		numSpecularSamples( 20 ),
		maxDiffuseDepth( 200 ),
		numDiffuseRays( 2000 ),
		numDiffuseSamples( 1 ),
		numVisibilityRays( 200 ),
		rayOffset( 0.0001f ),
		
		// Caching parameters.
		responseTime( 3.0f ),
		visibilityCacheTime( 5.0f ),
		
		// Clustering parameters.
		innerClusteringAngle( 20.0f ),
		outerClusteringAngle( 20.0f ),
		
		// Internal data.
		internalData()
{
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
