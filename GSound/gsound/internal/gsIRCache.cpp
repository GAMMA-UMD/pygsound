/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsIRCache.cpp
 * Contents:    gsound::internal::IRCache class implementation
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


#include "gsIRCache.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




IRCache:: IRCache()
	:	firstFrame( true )
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Cache Update Method
//############
//##########################################################################################
//##########################################################################################




void IRCache:: update( Float blendFactor, Float energyScaleFactor, SampledIR& outputIR )
{
	blendFactor = math::clamp( blendFactor, Float(0), Float(1) );
	
	// If this is the first frame, initialize the cache with the raw IR.
	if ( firstFrame )
	{
		blendFactor = Float(1);
		firstFrame = false;
	}
	
	const Size cachedLength = cachedIR.getLengthInSamples();
	const Size inputLength = outputIR.getLengthInSamples();
	const Size outputLength = math::max( cachedLength, inputLength );
	
	// Make the IRs longer if they are too short.
	if ( cachedLength < outputLength )
		cachedIR.setLengthInSamples( outputLength );
	
	if ( inputLength < outputLength )
		outputIR.setLengthInSamples( cachedLength, true );
	
	if ( cachedIR.getStartTimeInSamples() > outputIR.getStartTimeInSamples() )
		cachedIR.setStartTimeInSamples( outputIR.getStartTimeInSamples() );
	
	if ( outputIR.getStartTimeInSamples() > cachedIR.getStartTimeInSamples() )
		outputIR.setStartTimeInSamples( cachedIR.getStartTimeInSamples() );
	
	cachedIR.setSourceDirectionsEnabled( outputIR.getSourceDirectionsEnabled() );
	
	//*************************************************************************************
	// Update the directions.
	
	updateDirections( cachedIR.getDirections(), outputIR.getDirections(),
						blendFactor, cachedIR.getLengthInSamples() );
	
	// Update the source directions if necessary.
	if ( outputIR.getSourceDirectionsEnabled() )
	{
		updateDirections( cachedIR.getSourceDirections(), outputIR.getSourceDirections(),
						blendFactor, outputLength );
	}
	
	//*************************************************************************************
	// Update each frequency band.
	
	updateIntensity( cachedIR.getIntensity(), outputIR.getIntensity(),
					blendFactor, energyScaleFactor, outputLength );
}




//##########################################################################################
//##########################################################################################
//############
//############		Cache Clear Methods
//############
//##########################################################################################
//##########################################################################################




void IRCache:: clear()
{
	cachedIR.clear();
	firstFrame = true;
}




void IRCache:: reset()
{
	cachedIR.reset();
	firstFrame = true;
}




//##########################################################################################
//##########################################################################################
//############
//############		Cache Update Methods
//############
//##########################################################################################
//##########################################################################################




void IRCache:: updateDirections( Vector3f* cache, Vector3f* output, Float blend, Size length )
{
	const Float inverseBlend = Float(1) - blend;
	
	linearCombine( (Float32*)cache, inverseBlend, (Float32*)output, blend, length*3 );
}




GSOUND_FORCE_INLINE static void linearCombine2( Float* a, Float aWeight, Float* b, Float bWeight, Float bScale, Size length )
{
#if defined(GSOUND_USE_SIMD)
	const Size unroll = 1;
	const Size simdWidth = unroll*SIMDFloat::getWidth();
	const Size numSIMDIterations = length / simdWidth;
	const Size numSIMD = numSIMDIterations*simdWidth;
	const Float* const simdAEnd = a + numSIMD;
	const Float* const aEnd = a + length;
	const SIMDFloat simdAWeight = aWeight;
	const SIMDFloat simdBWeight = bWeight;
	
	while ( a < simdAEnd )
	{
		for ( Index i = 0; i < unroll; i++ )
		{
			SIMDFloat c = math::square( math::sqrt(((SIMDFloat*)a)[i])*simdAWeight +
							math::sqrt(((SIMDFloat*)b)[i]*bScale)*simdBWeight );
			
			((SIMDFloat*)a)[i] = c;
			((SIMDFloat*)b)[i] = c;
		}
		
		a += simdWidth;
		b += simdWidth;
	}
	
	// Do the scalar part at the end.
	while ( a < aEnd )
	{
		Float c = math::square( math::sqrt(*a)*aWeight + math::sqrt(*b)*bWeight ) * bScale;
		*a = c;
		*b = c;
		a++;
		b++;
	}
	
#else
	const Float* const aEnd = a + length;
	
	// Apply the new IR to the cached IR.
	while ( a < aEnd )
	{
		Float c = (*a)*aWeight + (*b)*bWeight;
		*a = c;
		*b = c;
		a++;
		b++;
		i++;
	}
#endif
}




void IRCache:: updateIntensity( Float* cache, Float* output, Float blend, Float energyScale, Size length )
{
	const Float inverseBlend = Float(1) - blend;
	
	//linearCombine2( cache, inverseBlend, output, blend, energyScale, length*GSOUND_FREQUENCY_COUNT );
	linearCombine( cache, inverseBlend, output, blend*energyScale, length*GSOUND_FREQUENCY_COUNT );
}




void IRCache:: linearCombine( Float* a, Float aWeight, Float* b, Float bWeight, Size length )
{
#if defined(GSOUND_USE_SIMD)
	const Size unroll = 2;
	const Size simdWidth = unroll*SIMDFloat::getWidth();
	const Size numSIMDIterations = length / simdWidth;
	const Size numSIMD = numSIMDIterations*simdWidth;
	const Float* const simdAEnd = a + numSIMD;
	const Float* const aEnd = a + length;
	const SIMDFloat simdAWeight = aWeight;
	const SIMDFloat simdBWeight = bWeight;
	
	while ( a < simdAEnd )
	{
		for ( Index i = 0; i < unroll; i++ )
		{
			SIMDFloat c = ((SIMDFloat*)a)[i]*simdAWeight +
							((SIMDFloat*)b)[i]*simdBWeight;
			((SIMDFloat*)a)[i] = c;
			((SIMDFloat*)b)[i] = c;
		}
		
		a += simdWidth;
		b += simdWidth;
	}
	
	// Do the scalar part at the end.
	while ( a < aEnd )
	{
		Float c = (*a)*aWeight + (*b)*bWeight;
		*a = c;
		*b = c;
		a++;
		b++;
	}
	
#else
	const Float* const aEnd = a + length;
	
	// Apply the new IR to the cached IR.
	while ( a < aEnd )
	{
		Float c = (*a)*aWeight + (*b)*bWeight;
		*a = c;
		*b = c;
		a++;
		b++;
		i++;
	}
#endif
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
