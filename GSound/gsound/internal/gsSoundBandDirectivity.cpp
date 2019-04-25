/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundBandDirectivity.cpp
 * Contents:    gsound::internal::SoundBandDirectivity class implementation
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


#include "gsSoundBandDirectivity.h"


#include "../gsSoundDirectivity.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




SoundBandDirectivity:: SoundBandDirectivity()
	:	lastDirectivity( NULL ),
		lastTimeStamp( 0 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Directivity Accessor Method
//############		
//##########################################################################################
//##########################################################################################




void SoundBandDirectivity:: setDirectivity( const SoundDirectivity& newDirectivity, const FrequencyBands& frequencies,
											Size numIntegrationSamples, Float maxError, Float convergence, Size maxOrder )
{
	// Don't update the directivity if it hasn't changed.
	if ( lastDirectivity == &newDirectivity && lastTimeStamp == newDirectivity.timeStamp )
		return;
	
	// Save a pointer to the directivity and timestamp for later identification.
	lastDirectivity = &newDirectivity;
	lastTimeStamp = newDirectivity.timeStamp;
	
	// An array of the directivity samples for this directivity pattern.
	ArrayList<Sample> samples;
	
	// A random variable used for monte-carlo integration of directivity patterns.
	math::Random<Float> randomVariable;
	
	//*******************************************************************************
	// Convert each sample in the new directivity pattern to the frequency bands.
	
	const Matrix3f& orientation = newDirectivity.getOrientation();
	const Size numInputSamples = newDirectivity.getSampleCount();
	
	// Compute the total number of samples to use for integeration.
	const Size numSamples = numInputSamples + numIntegrationSamples;
	
	SIMDBands totalResponse = Float(0);
	
	for ( Index i = 0; i < numInputSamples; i++ )
	{
		const Vector3f& direction = newDirectivity.getSampleDirection(i);
		const FrequencyResponse& response = newDirectivity.getSample(i);
		
		SIMDBands bandResponse;
		
		// Square to convert to intensity.
		for ( Index b = 0; b < frequencies.getBandCount(); b++ )
			bandResponse[b] = math::square( response.getFrequencyGain(frequencies[b]) );
		
		totalResponse += bandResponse;
		
		samples.add( Sample( (orientation*direction).normalize(), bandResponse ) );
	}
	
	// Use a set number of interpolated monte-carlo integration samples to improve the results.
	for ( Index j = 0; j < numIntegrationSamples; j++ )
	{
		Vector3f direction = getRandomDirection( randomVariable );
		SIMDBands response = interpolateSample( direction, samples.getPointer(), numInputSamples );
		totalResponse += response;
		
		samples.add( Sample( direction, response ) );
	}
	
	// Normalize the samples so that the total sound power is preserved.
	Float normalizeFactor = Float(1) / (math::sumScalar( totalResponse ) / frequencies.getBandCount() / numSamples);
	
	for ( Index i = 0; i < numSamples; i++ )
	{
		Sample& sample = samples[i];
		sample.response *= normalizeFactor;
	}
	
	//*******************************************************************************
	// For increasing order, determine how well the SH approximation fits the data.
	
	Float lastError = math::max<Float>();
	Bool backtracked = false;
	
	for ( Index order = 0; order <= maxOrder; )
	{
		// Zero-out the accumulator.
		const Size coefficientCount = (order+1)*(order+1);
		const Size simdCoefficientCount = coefficientCount*SIMDBands::getWidth();
		
		if ( directivity.getCoefficientCount() < coefficientCount || basis.getCoefficientCount() < coefficientCount )
		{
			directivity.setOrderUninitialized( order );
			basis.setOrderUninitialized( order );
		}
		
		om::util::zero( (Float*)directivity.getCoefficients(), simdCoefficientCount );
		
		//*******************************************************************************
		// Integrate the source directivity over the SH basis.
		
		// Always use the input samples as integration points.
		for ( Index i = 0; i < numSamples; i++ )
		{
			const Sample& sample = samples[i];
			SH::cartesian( order, sample.direction, basis );
			math::multiplyAdd( directivity.getCoefficients(), basis.getCoefficients(), sample.response, coefficientCount );
		}
		
		// Normalize based on the number of samples and sphere surface area.
		const Float normalize = (Float(4)*math::pi<Float>()) / Float(numSamples);
		math::multiply( (Float*)directivity.getCoefficients(), normalize, simdCoefficientCount );
		
		//*******************************************************************************
		// Determine the L2 error over all samples.
		
		SIMDBands simdError = Float(0);
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			const Sample& sample = samples[i];
			
			SH::cartesian( order, sample.direction, basis );
			SIMDBands response = math::dot( directivity.getCoefficients(), basis.getCoefficients(), coefficientCount );
			
			// Accumulate the squared error.
			SIMDBands responseError = response - sample.response;
			simdError += responseError*responseError;
		}
		
		simdError = math::sqrt(simdError / Float(numSamples));
		
		// Average the error across bands.
		const Float error = math::sumScalar( simdError ) / frequencies.getBandCount();
		
		if ( error > lastError && !backtracked )
		{
			// Backtrack to the previous order since the error was less.
			order--;
			maxOrder = order;
			backtracked = true;
			continue;
		}
		
		// Check to see if the error is small enough or if there is convergence.
		if ( error < maxError || (lastError/error - Float(1)) < convergence )
			break;
		
		lastError = error;
		order++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Response Accessor Method
//############		
//##########################################################################################
//##########################################################################################




FrequencyBandResponse SoundBandDirectivity:: getResponse( const Vector3f& direction ) const
{
	// Project direction into spherical harmonic basis.
	SH::cartesian( directivity.getOrder(), direction, basis );
	
	// Compute the dot product of the direction and the directivity coefficients.
	SIMDBands response = math::dot( directivity.getCoefficients(), basis.getCoefficients(), directivity.getCoefficientCount() );
	response = math::max( response, SIMDBands(0.0f) );
	
	return response.toArray();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Interpolation Method
//############		
//##########################################################################################
//##########################################################################################




SIMDBands SoundBandDirectivity:: interpolateSample( const Vector3f& direction, const Sample* samples, Size numSamples )
{
	if ( numSamples == 0 )
		return SIMDBands(1.0f);
	else if ( numSamples == 1 )
		return samples[0].response;
	else if ( numSamples == 2 )
	{
		const Float cosTheta0 = math::max( math::dot( direction, samples[0].direction ), Float(0) );
		const Float cosTheta1 = math::max( math::dot( direction, samples[1].direction ), Float(0) );
		const Float cosThetaTotal = cosTheta0 + cosTheta1;
		
		if ( cosThetaTotal < math::epsilon<Float>() )
			return Float(0.5)*(samples[0].response + samples[1].response);
		else
			return (cosTheta0*samples[0].response + cosTheta1*samples[1].response)/(cosTheta0 + cosTheta1);
	}
	else
	{
		// Find the 3 samples that are closest to the input direction.
		Index closest[3] = { 0, 1, 2 };
		Float cosTheta[3] = { -1.0f, -1.0f, -1.0f };
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			Float cosTheta2 = math::dot( direction, samples[i].direction );
			
			if ( cosTheta2 > cosTheta[0] )
			{
				cosTheta[2] = cosTheta[1];	closest[2] = closest[1];
				cosTheta[1] = cosTheta[0];	closest[1] = closest[0];
				cosTheta[0] = cosTheta2;	closest[0] = i;
			}
			else if ( cosTheta2 > cosTheta[1] )
			{
				cosTheta[2] = cosTheta[1];	closest[2] = closest[1];
				cosTheta[1] = cosTheta2;	closest[1] = i;
			}
			else if ( cosTheta2 > cosTheta[2] )
			{
				cosTheta[2] = cosTheta2;	closest[2] = i;
			}
		}
		
		// Compute the barycentric coordinates of the direction and interpolate based on those.
		Vector3f bary = math::barycentric( samples[closest[0]].direction, samples[closest[1]].direction,
											samples[closest[2]].direction, direction );
		
		return bary.x*samples[closest[0]].response + bary.y*samples[closest[1]].response + bary.z*samples[closest[2]].response;
	}
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
