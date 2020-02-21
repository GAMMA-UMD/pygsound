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

#include "omSoundDirectionalIR.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const om::resources::ResourceType DirectionalIR:: RESOURCE_TYPE("DirectionalIR");


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




DirectionalIR:: DirectionalIR()
	:	channels(),
		length( 256 ),
		fftSize( 256 ),
		shOrder( 0 ),
		representation( SAMPLED ),
		domain( TIME_DOMAIN ),
		sampleRate( 44100 ),
		orientation( Matrix3f::IDENTITY ),
		fftReal( NULL ),
		fftComplex( NULL ),
		validHull( false )
{
}




DirectionalIR:: DirectionalIR( Size numChannels, Size newLength, SampleRate newSampleRate, Domain newDomain )
	:	channels( numChannels ),
		length( 0 ),
		fftSize( 0 ),
		shOrder( 0 ),
		representation( SAMPLED ),
		domain( newDomain ),
		sampleRate( newSampleRate ),
		orientation( Matrix3f::IDENTITY ),
		fftReal( NULL ),
		fftComplex( NULL ),
		validHull( false )
{
	this->setLength( newLength );
}




DirectionalIR:: DirectionalIR( const DirectionalIR& other )
	:	channels( other.channels ),
		length( other.length ),
		fftSize( other.fftSize ),
		shOrder( other.shOrder ),
		representation( other.representation ),
		domain( other.domain ),
		sampleRate( other.sampleRate ),
		orientation( other.orientation ),
		name( other.name ),
		description( other.description ),
		fftReal( NULL ),
		fftComplex( NULL ),
		validHull( false )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




DirectionalIR:: ~DirectionalIR()
{
	if ( fftReal != NULL )
		util::destruct( fftReal );
	
	if ( fftComplex != NULL )
		util::destruct( fftComplex );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




DirectionalIR& DirectionalIR:: operator = ( const DirectionalIR& other )
{
	if ( this != &other )
	{
		channels = other.channels;
		length = other.length;
		fftSize = other.fftSize;
		shOrder = other.shOrder;
		representation = other.representation;
		domain = other.domain;
		sampleRate = other.sampleRate;
		orientation = other.orientation;
		name = other.name;
		description = other.description;
		validHull = false;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Domain Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool DirectionalIR:: setDomain( Domain newDomain )
{
	const Size filterDataLength = this->getFilterDataLength();
	const Size numChannels = channels.getSize();
	
	// Convert the internal data for each channel.
	for ( Index c = 0; c < numChannels; c++ )
	{
		Channel& channel = channels[c];
		const Size numSamples = channel.samples.getSize();
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			Sample& sample = channel.samples[i];
			
			// Make sure the sample data is large enough and allocated.
			sample.data.setSizeCopy( filterDataLength );
			
			// Convert the sample data.
			convertSampleDomain( domain, sample.data.getPointer(), newDomain, sample.data.getPointer(), MIN_PHASE );
		}
	}
	
	domain = newDomain;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Length Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void DirectionalIR:: setLength( Size newLength )
{
	const Size oldLength = length;
	const Size oldFFTSize = fftSize;
	length = newLength;
	fftSize = math::isPowerOfTwo(newLength) ? newLength : math::nextPowerOfTwo(newLength);
	const Size filterDataSize = this->getFilterDataLength();
	
	// Make sure the FFT has the correct size.
	if ( fftReal && fftReal->getSize() != fftSize )
	{
		util::destruct( fftReal );
		fftReal = util::construct< FFTReal<Float32> >( fftSize );
	}
	
	// Convert the internal data for each channel.
	const Size numChannels = channels.getSize();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		Channel& channel = channels[c];
		const Size numSamples = channel.samples.getSize();
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			Sample& sample = channel.samples[i];
			
			// Resample the data for the sample to the new length.
			switch ( domain )
			{
				case TIME_DOMAIN:
				{
					// Make sure the sample data has the correct size.
					sample.data.setSizeCopy( filterDataSize );
					
					// Zero the new samples if the length increased.
					if ( length > oldLength )
						util::zeroPOD( sample.data.getPointer() + oldLength, length - oldLength );
				}
				break;
				
				case FREQUENCY_DOMAIN:
				case FREQUENCY_DOMAIN_MAGNITUDE:
				case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
				{
					if ( fftSize != oldFFTSize )
					{
						// Convert the data to complex frequency domain.
						if ( domain == FREQUENCY_DOMAIN_MAGNITUDE_PHASE )
							frequencyMagnitudePhaseToComplex( sample.data.getPointer(), sample.data.getPointer(), oldFFTSize );
						else if ( domain == FREQUENCY_DOMAIN_MAGNITUDE )
							frequencyMagnitudeToComplex( sample.data.getPointer(), sample.data.getPointer(), oldFFTSize, MIN_PHASE );
						
						// Make sure the temporary buffer is big enough.
						const Size bufferLength = math::max( oldFFTSize, fftSize ) + 2;
						tempBuffer.allocate( bufferLength );
						
						// Convert the old data to time domain.
						math::ifft( (const Complex32f*)sample.data.getPointer(), tempBuffer.getPointer(), oldFFTSize );
						math::multiply( tempBuffer.getPointer(), Float(1)/Float(oldFFTSize), oldFFTSize );
						
						// Zero the new samples if the length increased.
						if ( bufferLength > oldLength )
							util::zeroPOD( tempBuffer.getPointer() + oldLength, bufferLength - oldLength );
						
						// Make sure the sample data has the correct size.
						sample.data.allocate( filterDataSize );
						
						// Convert the time domain data to frequency domain.
						math::fft( tempBuffer.getPointer(), (Complex32f*)sample.data.getPointer(), fftSize );
						
						// Convert the data from complex frequency domain to the response's domain.
						if ( domain == FREQUENCY_DOMAIN_MAGNITUDE_PHASE )
							frequencyComplexToMagnitudePhase( sample.data.getPointer(), sample.data.getPointer(), fftSize );
						else if ( domain == FREQUENCY_DOMAIN_MAGNITUDE )
							frequencyComplexToMagnitude( sample.data.getPointer(), sample.data.getPointer(), fftSize );
					}
				}
				break;
			}
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void DirectionalIR:: setChannelCount( Size newChannelCount )
{
	channels.setSize( newChannelCount );
}




void DirectionalIR:: clearChannels()
{
	channels.setSize( 0 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool DirectionalIR:: addSample( Index channelIndex, const Vector3f& direction, Float delay, const FrequencyData& magnitude )
{
	if ( channelIndex >= channels.getSize() )
		return false;
	
	// Add a new sample to the channel.
	Channel& channel = channels[channelIndex];
	const Float directionMag2 = direction.getMagnitudeSquared();
	channel.samples.addNew( directionMag2 < math::minPositive<Float>() ? Vector3f() : direction / math::sqrt(directionMag2),
							math::max( delay, Float(0) ) );
	Sample& sample = channel.samples.getLast();
	
	// Make sure the sample's data storage is allocated.
	const Size sampleLength = this->getFilterLength();
	sample.data.allocate( sampleLength );
	
	// Convert the frequency magnitude data into the response's domain.
	switch ( domain )
	{
		case TIME_DOMAIN:
		{
			// Make sure the temporary buffer is big enough.
			tempBuffer.allocate( fftSize + 2 );
			
			// Convert to frequency domain complex via min-phase reconstruction, then to time domain.
			frequencyMagnitudeToComplex( magnitude, tempBuffer.getPointer(), fftSize, MIN_PHASE );
			frequencyComplexToTime( tempBuffer.getPointer(), sample.data.getPointer() );
		}
		break;
		
		case FREQUENCY_DOMAIN:
			frequencyMagnitudeToComplex( magnitude, sample.data.getPointer(), fftSize, MIN_PHASE );
			break;
		
		case FREQUENCY_DOMAIN_MAGNITUDE:
			frequencyMagnitudeToMagnitude( magnitude, sample.data.getPointer(), fftSize );
			break;
		
		case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
			frequencyMagnitudeToComplex( magnitude, sample.data.getPointer(), fftSize, MIN_PHASE );
			frequencyComplexToMagnitudePhase( sample.data.getPointer(), sample.data.getPointer(), fftSize );
			break;
	}
	
	// Remember that we need to regenerate the convex hull to do interpolation.
	validHull = false;
	
	return true;
}




Bool DirectionalIR:: addSample( Index channelIndex, const Vector3f& direction, Float delay, const Float32* sampleData )
{
	if ( channelIndex >= channels.getSize() || sampleData == NULL )
		return false;
	
	// Add a new sample to the channel.
	Channel& channel = channels[channelIndex];
	const Float directionMag2 = direction.getMagnitudeSquared();
	channel.samples.addNew( directionMag2 < math::minPositive<Float>() ? Vector3f() : direction / math::sqrt(directionMag2),
							math::max( delay, Float(0) ) );
	Sample& sample = channel.samples.getLast();
	
	// Make sure the sample's data storage is allocated.
	const Size sampleLength = this->getFilterLength();
	const Size filterDataLength = this->getFilterDataLength();
	sample.data.allocate( filterDataLength );
	
	// Copy the sample data.
	util::copy( sample.data.getPointer(), sampleData, sampleLength );
	util::zeroPOD( sample.data.getPointer() + sampleLength, filterDataLength - sampleLength );
	
	// Remember that we need to regenerate the convex hull to do interpolation.
	validHull = false;
	
	return true;
}




void DirectionalIR:: clearSamples()
{
	const Size numChannels = channels.getSize();
	
	for ( Index c = 0; c < numChannels; c++ )
		channels[c].samples.clear();
	
	// Remember that we need to regenerate the convex hull to do interpolation.
	validHull = false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Interpolation Methods
//############		
//##########################################################################################
//##########################################################################################




Size DirectionalIR:: getFilterLength( Domain outputDomain ) const
{
	switch ( outputDomain )
	{
		case TIME_DOMAIN:
			return length;
		case FREQUENCY_DOMAIN:
			return fftSize + 2;
		case FREQUENCY_DOMAIN_MAGNITUDE:
			return fftSize / 2 + 1;
		case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
			return fftSize + 2;
	}
	
	return 0;
}




Size DirectionalIR:: getFilter( Index channelIndex, Domain outputDomain, Phase phase,
									math::Interpolation interpolation, const Vector3f& xyz, Float* filterData ) const
{
	if ( channelIndex >= channels.getSize() || filterData == NULL )
		return 0;
	
	// Handle spherical harmonic representations.
	if ( representation == SPHERICAL_HARMONIC )
	{
		// Determine the coefficient count for the SH order.
		const Size numSHCoefficients = math::SH::getCoefficientCount( shOrder );
		
		// Make sure the temporary coefficient buffer is allocated.
		tempSHBasis.allocate( numSHCoefficients );
		
		// Compute the SH basis in the filter direction.
		math::SH::cartesian( shOrder, xyz*orientation, tempSHBasis.getPointer() );
		
		// Get the filter for that basis.
		return this->getFilterSH( channelIndex, outputDomain, phase, shOrder, tempSHBasis.getPointer(), filterData );
	}
	
	//****************************************************************************
	// Find the convex hull triangle that this vector intersects.
	
	Index triangleIndex;
	Vector3f bary;
	
	if ( !findTriangle( channelIndex, xyz*orientation, triangleIndex, bary ) )
		return 0;
	
	// Get the samples that need to be interpolated.
	const Channel& channel = channels[channelIndex];
	const Triangle& triangle = triangles[triangleIndex];
	const Sample& sample1 = channel.samples[triangle.v1];
	const Sample& sample2 = channel.samples[triangle.v2];
	const Sample& sample3 = channel.samples[triangle.v3];
	const Size filterLength = this->getFilterLength();
	
	//****************************************************************************
	// Interpolate the samples.
	
	switch ( interpolation )
	{
		// Nearest-neighbor interpolation.
		case math::Interpolation::NONE:
		{
			if ( bary.x >= bary.y && bary.x >= bary.z )
				util::copyPOD( filterData, sample1.data.getPointer(), filterLength );
			else if ( bary.y >= bary.x && bary.y >= bary.z )
				util::copyPOD( filterData, sample2.data.getPointer(), filterLength );
			else if ( bary.z >= bary.x && bary.z >= bary.y )
				util::copyPOD( filterData, sample3.data.getPointer(), filterLength );
		}
		break;
		
		default:
		{
			// Interpolate the filter using linear barycentric interpolation.
			math::multiply( filterData, sample1.data.getPointer(), bary.x, filterLength );
			math::multiplyAdd( filterData, sample2.data.getPointer(), bary.y, filterLength );
			math::multiplyAdd( filterData, sample3.data.getPointer(), bary.z, filterLength );
		}
		break;
	}
	
	// Convert the interpolated filter to the output domain.
	convertSampleDomain( domain, filterData, outputDomain, filterData, phase );
	
	return this->getFilterLength( outputDomain );
}




Size DirectionalIR:: getFilterSH( Index channelIndex, Domain outputDomain, Phase phase,
									Size outputSHOrder, const Float* shBasis, Float* filterData ) const
{
	if ( channelIndex >= channels.getSize() || filterData == NULL || shBasis == NULL || representation != SPHERICAL_HARMONIC )
		return 0;
	
	const Channel& channel = channels[channelIndex];
	const Size filterLength = this->getFilterLength();
	const Size outputFilterLength = this->getFilterLength( outputDomain );
	
	// Determine the output spherical harmonic order to use.
	outputSHOrder = math::min( outputSHOrder, shOrder );
	const Size outputCoefficients = math::min( channel.samples.getSize(),
												math::SH::getCoefficientCount( outputSHOrder ) );
	
	//****************************************************************************
	// Accumulate the interpolated filter.
	
	if ( outputCoefficients > 0 )
	{
		math::multiply( filterData, channel.samples[0].data.getPointer(), shBasis[0], filterLength );
		
		for ( Index i = 1; i < outputCoefficients; i++ )
		{
			const Sample& sample = channel.samples[i];
			math::multiplyAdd( filterData, sample.data.getPointer(), shBasis[i], filterLength );
		}
	}
	else
		util::zeroPOD( filterData, outputFilterLength );
	
	// Convert the interpolated filter to the output domain.
	convertSampleDomain( domain, filterData, outputDomain, filterData, phase );
	
	return outputFilterLength;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Spherical Harmonic Conversion Method
//############		
//##########################################################################################
//##########################################################################################




Bool DirectionalIR:: convertToSH( const math::SHProjection& projection, DirectionalIR& result ) const
{
	if ( this == &result )
		return false;
	
	// Clear the samples in the output response.
	result.clearSamples();
	
	// Make sure the output response has the correct parameters.
	const Size numChannels = channels.getSize();
	const Size filterLength = this->getFilterLength();
	result.representation = SPHERICAL_HARMONIC;
	result.domain = domain;
	result.shOrder = 0;
	result.sampleRate = sampleRate;
	result.length = length;
	result.fftSize = fftSize;
	result.orientation = orientation;
	result.name = name;
	result.description = description;
	result.channels.setSize( numChannels );
	
	// Make sure the temporary buffers are allocated.
	result.tempSHBasis.allocate( math::SH::getCoefficientCount( projection.maxOrder ) );
	result.tempBuffer.allocate( filterLength );
	tempBuffer.allocate( filterLength );
	
	//*******************************************************************************
	// Generate integration samples.
	
	// Determine how many integration samples to use.
	const Size numIntegrationSamples = math::max( projection.sampleCount, Size(100) );
	
	// A random variable used for monte-carlo integration of HRTF.
	math::Random<Float> randomVariable;
	math::Random<Float>::State randomState = randomVariable.getState();
	
	//*******************************************************************************
	// For increasing order, determine how well the SH approximation fits the data.
	
	Float lastError = math::max<Float>();
	Size lastCoefficientCount = 0;
	Index order = 0;
	
	while ( true )
	{
		//*******************************************************************************
		// Zero-out the coefficient accumulators for the new SH order coefficients.
		
		const Size coefficientCount = math::SH::getCoefficientCount(order);
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			Channel& resultChannel = result.channels[c];
			
			// Zero the new SH order coefficients (the others are already computed and valid).
			for ( Index i = lastCoefficientCount; i < coefficientCount; i++ )
			{
				resultChannel.samples.addNew( Vector3f(), 0.0f );
				Sample& resultSample = resultChannel.samples.getLast();
				
				// Allocate space for the result filter SH coefficients.
				resultSample.data.allocate( filterLength );
				resultSample.data.zero( 0, filterLength );
			}
		}
		
		//*******************************************************************************
		// Integrate the source HRTF over the SH basis.
		
		// Reset the random seed.
		randomVariable.setState( randomState );
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			Channel& resultChannel = result.channels[c];
			
			for ( Index i = 0; i < numIntegrationSamples; i++ )
			{
				// Get a uniformly-distributed random direction for the integration sample.
				Vector3f direction = getRandomDirection( randomVariable );
				
				// Evaluate the spherical harmonic basis functions for the sample direction.
				math::SH::cartesian( order, direction, result.tempSHBasis.getPointer() );
				
				// Interpolate the original filter for the direction.
				this->getFilter( c, domain, ORIGINAL_PHASE, math::Interpolation::LINEAR, direction, result.tempBuffer.getPointer() );
				
				// Accumulate the new filter coefficients for this SH order.
				for ( Index j = lastCoefficientCount; j < coefficientCount; j++ )
				{
					Sample& resultSample = resultChannel.samples[j];
					math::multiplyAdd( resultSample.data.getPointer(), result.tempBuffer.getPointer(),
										result.tempSHBasis[j], filterLength );
				}
			}
			
			// Normalize based on the number of samples and sphere surface area.
			const Float normalize = (Float(4)*math::pi<Float>()) / Float(numIntegrationSamples);
			
			for ( Index i = lastCoefficientCount; i < coefficientCount; i++ )
			{
				Sample& resultSample = resultChannel.samples[i];
				math::multiply( resultSample.data.getPointer(), normalize, filterLength );
			}
		}
		
		//*******************************************************************************
		// Determine the L2 error over all samples.
		
		// Reset the random seed.
		randomVariable.setState( randomState );
		
		Float error = 0.0f;
		Size errorSamples = 0;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Channel& resultChannel = result.channels[c];
			
			for ( Index i = 0; i < numIntegrationSamples; i++ )
			{
				// Get a uniformly-distributed random direction for the integration sample.
				Vector3f direction = getRandomDirection( randomVariable );
				
				// Evaluate the spherical harmonic basis functions for the sample direction.
				math::SH::cartesian( order, direction, result.tempSHBasis.getPointer() );
				
				// Interpolate the original filter for the direction.
				this->getFilter( c, domain, ORIGINAL_PHASE, math::Interpolation::LINEAR, direction, result.tempBuffer.getPointer() );
				
				// Reconstruct the current SH filter.
				math::multiply( tempBuffer.getPointer(),
								resultChannel.samples[0].data.getPointer(),
								result.tempSHBasis[0], filterLength );
				
				for ( Index j = 1; j < coefficientCount; j++ )
				{
					math::multiplyAdd( tempBuffer.getPointer(),
										resultChannel.samples[j].data.getPointer(),
										result.tempSHBasis[j], filterLength );
				}
				
				// Compute and accumulate the squared error for the interpolated filter.
				math::subtract( tempBuffer.getPointer(), result.tempBuffer.getPointer(), filterLength );
				error += math::dot( tempBuffer.getPointer(), tempBuffer.getPointer(), filterLength );
			}
			
			errorSamples += numIntegrationSamples*filterLength;
		}
		
		error = math::sqrt(error / Float(errorSamples));
		
		// Check to see if the error is small enough or if there is convergence.
		if ( error < projection.maxError ||
			(lastError/error - Float(1)) < projection.convergence ||
			order == projection.maxOrder )
			break;
		
		lastError = error;
		order++;
		lastCoefficientCount = coefficientCount;
	}
	
	result.shOrder = order;
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Triangle Find Method
//############		
//##########################################################################################
//##########################################################################################




Bool DirectionalIR:: findTriangle( Index channelIndex, const Vector3f& direction, Index& triangle, Vector3f& bary ) const
{
	const Channel& channel = channels[channelIndex];
	
	if ( !validHull )
	{
		// Build the convex hull
		buildHull( channel.samples.getPointer(), channel.samples.getSize(), triangles );
		
		validHull = true;
	}
	
	math::Ray3f ray( Vector3f(), direction );
	
	for ( Index i = 0; i < triangles.getSize(); i++ )
	{
		const Triangle& tri = triangles[i];
		const Vector3f& v1 = channel.samples[tri.v1].direction;
		const Vector3f& v2 = channel.samples[tri.v2].direction;
		const Vector3f& v3 = channel.samples[tri.v3].direction;
		
		// Check to see if the ray intersects the triangle.
		Float distance;
		
		if ( ray.intersectsTriangle( v1, v2, v3, distance, bary.y, bary.z ) )
		{
			// Compute the final barycentric coordinate.
			bary.x = Float(1) - bary.y - bary.z;
			
			// Set the output triangle index.
			triangle = i;
			
			return true;
		}
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Domain Conversion Method
//############		
//##########################################################################################
//##########################################################################################




Bool DirectionalIR:: convertDomain( Domain newDomain, Phase phase, DirectionalIR& result ) const
{
	if ( this == &result || representation == SPHERICAL_HARMONIC )
		return false;
	else if ( domain == newDomain )
		result = *this;
	
	// Clear the samples in the output response.
	result.clearSamples();
	
	// Make sure the output response has the correct parameters.
	const Size numChannels = channels.getSize();
	result.representation = representation;
	result.domain = newDomain;
	result.orientation = orientation;
	result.shOrder = 0;
	result.sampleRate = sampleRate;
	result.length = length;
	result.fftSize = fftSize;
	result.name = name;
	result.description = description;
	result.channels.setSize( numChannels );
	
	const Size filterDataLength = this->getFilterDataLength();
	
	// Convert each channel's data.
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Channel& channel = channels[c];
		Channel& resultChannel = result.channels[c];
		const Size numSamples = channel.samples.getSize();
		
		// Allocate enough space for all of the samples that are in this channel.
		if ( resultChannel.samples.getCapacity() < numSamples )
			resultChannel.samples.setCapacity( numSamples );
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			const Sample& sample = channel.samples[i];
			
			// Add a new sample to the output response.
			resultChannel.samples.addNew( sample.direction, sample.delay );
			Sample& resultSample = resultChannel.samples.getLast();
			
			// Make sure the sample data is large enough and allocated.
			resultSample.data.allocate( filterDataLength );
			
			// Convert the sample data.
			convertSampleDomain( domain, sample.data.getPointer(), newDomain, resultSample.data.getPointer(), phase );
		}
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Domain Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




void DirectionalIR:: convertSampleDomain( Domain inputDomain, const Float32* dataIn, Domain outputDomain, Float32* dataOut, Phase phase ) const
{
	switch ( inputDomain )
	{
		case TIME_DOMAIN:
		{
			switch ( outputDomain )
			{
				case TIME_DOMAIN:
					if ( dataIn != dataOut )
						util::copyPOD( dataOut, dataIn, length );
					break;
				case FREQUENCY_DOMAIN:
					timeToFrequencyComplex( dataIn, dataOut );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE:
					timeToFrequencyComplex( dataIn, dataOut );
					frequencyComplexToMagnitude( dataOut, dataOut, fftSize );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
					timeToFrequencyComplex( dataIn, dataOut );
					frequencyComplexToMagnitudePhase( dataOut, dataOut, fftSize );
					break;
			}
		}
		break;
		
		case FREQUENCY_DOMAIN:
		{
			switch ( outputDomain )
			{
				case TIME_DOMAIN:
					frequencyComplexToTime( dataIn, dataOut );
					break;
				case FREQUENCY_DOMAIN:
					if ( dataIn != dataOut )
						util::copyPOD( dataOut, dataIn, fftSize + 2 );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE:
					frequencyComplexToMagnitude( dataIn, dataOut, fftSize );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
					frequencyComplexToMagnitudePhase( dataIn, dataOut, fftSize );
					break;
			}
		}
		break;
		
		case FREQUENCY_DOMAIN_MAGNITUDE:
		{
			switch ( outputDomain )
			{
				case TIME_DOMAIN:
					frequencyMagnitudeToComplex( dataIn, dataOut, fftSize, phase );
					frequencyComplexToTime( dataOut, dataOut );
					break;
				case FREQUENCY_DOMAIN:
					frequencyMagnitudeToComplex( dataIn, dataOut, fftSize, phase );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE:
					if ( dataIn != dataOut )
						util::copyPOD( dataOut, dataIn, fftSize/2 + 1 );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
					frequencyMagnitudeToComplex( dataIn, dataOut, fftSize, phase );
					frequencyComplexToMagnitudePhase( dataIn, dataOut, fftSize );
					break;
			}
		}
		break;
		
		case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
		{
			switch ( outputDomain )
			{
				case TIME_DOMAIN:
					frequencyMagnitudePhaseToComplex( dataIn, dataOut, fftSize );
					frequencyComplexToTime( dataOut, dataOut );
					break;
				case FREQUENCY_DOMAIN:
					frequencyMagnitudePhaseToComplex( dataIn, dataOut, fftSize );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE:
					frequencyMagnitudePhaseToMagnitude( dataIn, dataOut, fftSize );
					break;
				case FREQUENCY_DOMAIN_MAGNITUDE_PHASE:
					if ( dataIn != dataOut )
						util::copyPOD( dataOut, dataIn, fftSize + 2 );
					break;
			}
		}
		break;
	}
}




void DirectionalIR:: timeToFrequencyComplex( const Float32* dataIn, Float32* dataOut ) const
{
	if ( fftReal == NULL )
		fftReal = util::construct< FFTReal<Float32> >( fftSize );
	else if ( fftReal->getSize() != fftSize )
	{
		util::destruct( fftReal );
		fftReal = util::construct< FFTReal<Float32> >( fftSize );
	}
	
	// Convert the data to frequency domain.
	if ( dataIn == dataOut )
		fftReal->fft( dataOut );
	else
		fftReal->fft( dataIn, (Complex32f*)dataOut, NULL );
}




void DirectionalIR:: frequencyComplexToTime( const Float32* dataIn, Float32* dataOut ) const
{
	if ( fftReal == NULL )
		fftReal = util::construct< FFTReal<Float32> >( fftSize );
	else if ( fftReal->getSize() != fftSize )
	{
		util::destruct( fftReal );
		fftReal = util::construct< FFTReal<Float32> >( fftSize );
	}
	
	// Convert the data to time domain.
	if ( dataIn == dataOut )
		fftReal->ifft( dataOut );
	else
		fftReal->ifft( (const Complex32f*)dataIn, dataOut, NULL );
	
	// Correct the inverse FFT scaling.
	math::multiply( dataOut, Float(1)/Float(fftSize), length );
	util::zeroPOD( dataOut + length, fftSize - length );
}




void DirectionalIR:: frequencyComplexToMagnitude( const Float32* dataIn, Float32* dataOut, Size fftSize )
{
	const Size magnitudeSize = fftSize / 2 + 1;
	
	for ( Index i = 0, j = 0; i < magnitudeSize; i++, j += 2 )
	{
		Float32 real = dataIn[j];
		Float32 imag = dataIn[j+1];
		dataOut[i] = math::sqrt( real*real + imag*imag );
	}
}




void DirectionalIR:: frequencyComplexToMagnitudePhase( const Float32* dataIn, Float32* dataOut, Size fftSize )
{
	const Size complexSize = fftSize + 2;
	
	for ( Index i = 0; i < complexSize; i += 2 )
	{
		Float32 real = dataIn[i];
		Float32 imag = dataIn[i+1];
		dataOut[i] = math::sqrt( real*real + imag*imag );
		dataOut[i+1] = math::atan2( imag, real );
	}
}




void DirectionalIR:: frequencyMagnitudeToMagnitude( const FrequencyData& dataIn, Float32* dataOut, Size fftSize ) const
{
	// Resample the frequency magnitude data to the Fourier domain, maginitude only.
	const Float frequencyStep = Float(sampleRate) / Float(fftSize);
	const Size magnitudeSize = fftSize / 2 + 1;
	
	for ( Index f = 0; f < magnitudeSize; f++ )
	{
		// Compute the frequency for this FFT bin.
		Float frequency = f*frequencyStep;
		
		// Interpolate the frequency data to get the magnitude.
		dataOut[f] = dataIn.interpolate( frequency );
	}
}




void DirectionalIR:: frequencyMagnitudeToComplex( const FrequencyData& dataIn, Float32* dataOut, Size fftSize, Phase phase ) const
{
	const Size magnitudeSize = fftSize / 2 + 1;
	
	// Make sure the temporary buffer is big enough.
	tempSHBasis.allocate( magnitudeSize );
	
	// Resample the frequency magnitude data to the Fourier domain, maginitude only.
	frequencyMagnitudeToMagnitude( dataIn, tempSHBasis.getPointer(), fftSize );
	
	// Convert the magnitude-only data to complex as a min-phase filter.
	frequencyMagnitudeToComplex( tempSHBasis.getPointer(), dataOut, fftSize, phase );
}




void DirectionalIR:: frequencyMagnitudeToComplex( const Float32* dataIn, Float32* dataOut, Size fftSize, Phase phase ) const
{
	if ( fftReal == NULL )
		fftReal = util::construct< FFTReal<Float32> >( fftSize );
	else if ( fftReal->getSize() != fftSize )
	{
		util::destruct( fftReal );
		fftReal = util::construct< FFTReal<Float32> >( fftSize );
	}
	
	if ( fftComplex == NULL )
		fftComplex = util::construct< FFTComplex<Float32> >( fftSize );
	else if ( fftComplex->getSize() != fftSize )
	{
		util::destruct( fftComplex );
		fftComplex = util::construct< FFTComplex<Float32> >( fftSize );
	}
	
	const Float32 fftNormalize = Float32(1) / Float32(fftSize);
	const Size magnitudeSize = fftSize / 2 + 1;
	
	// Make sure the temporary buffer is big enough.
	tempBuffer.allocate( 2*(fftSize+2) );
	Complex32f* const tempComplex = (Complex32f*)tempBuffer.getPointer();
	
	if ( phase == LINEAR_PHASE )
	{
		// Copy the magnitude data to the complex array, interleave imaginary parts = 0.
		util::convert( tempComplex, dataIn, magnitudeSize );
		
		// Mirror the positive frequencies onto the negative frequencies.
		for ( Index f = 1; f < magnitudeSize; f++ )
			tempComplex[fftSize - f] = tempComplex[f];
		
		// Convert the data to time domain.
		fftComplex->ifft( tempComplex );
		
		// Copy to output, swap the first and last half.
		Size halfLength = fftSize / 2;
		
		for ( Index f = 0; f < halfLength; f++ )
			dataOut[f+halfLength] = tempComplex[f].r*fftNormalize;
		
		for ( Index f = halfLength; f < fftSize; f++ )
			dataOut[f-halfLength] = tempComplex[f].r*fftNormalize;
		
		// Convert the data back to frequency domain.
		fftReal->fft( dataOut );
	}
	else
	{
		// Copy the magnitude data to the complex array, interleave imaginary parts = 0.
		// Take the log for each frequency data and insert zero for imaginary parts.
		for ( Index f = 0; f < magnitudeSize; f++ )
		{
			const Float32 mag = std::abs(dataIn[f]);
			const Float32 logMag = (mag > 0.00001f ? math::ln( mag ) : 0.0f);
			tempComplex[f] = logMag;
		}
		
		// Mirror the positive frequencies onto the negative frequencies.
		for ( Index f = 1; f < magnitudeSize; f++ )
			tempComplex[fftSize - f] = tempComplex[f];
		
		//************************************************************************
		// Compute the hilbert transform to reconstruct the phase.
		
		// Backward transform of frequency data into time domain.
		fftComplex->ifft( tempComplex );
		
		// Add the conjugate of the negative frequencies onto the positive ones.
		for ( Index f = 1; f < magnitudeSize; f++ )
			tempComplex[f] += tempComplex[fftSize - f].conjugate();
		
		// Zero negative frequencies.
		util::zeroPOD( tempComplex + magnitudeSize, fftSize - magnitudeSize );
		
		// Forward transform to frequency domain.
		fftComplex->fft( tempComplex );
		
		Complex32f* const dataOutComplex = (Complex32f*)dataOut;
		
		// Convert from magnitude/phase to complex domain.
		for ( Index f = 0; f < magnitudeSize; f++ )
		{
			const Float32 magnitude = dataIn[f];
			const Float32 phase = tempComplex[f].i*fftNormalize;
			tempComplex[f] = Complex32f::polar( magnitude, phase );
		}
		
		for ( Index f = magnitudeSize; f < fftSize; f++ )
		{
			//const float magnitude = math::exp( tempComplex[f].r*fftNormalize );// (also works)
			const Float32 magnitude = dataIn[fftSize - f];
			const Float32 phase = tempComplex[f].i*fftNormalize;
			tempComplex[f] = Complex32f::polar( magnitude, phase );
		}
		
		// Zero phase for DC frequency.
		dataOutComplex[0].i = 0;
		
		//************************************************************************
		
		// Convert the data to time domain.
		fftComplex->ifft( tempComplex );
		
		// Copy the result to the output.
		for ( Index f = 0; f < fftSize; f++ )
			dataOut[f] = tempComplex[f].r*fftNormalize;
		
		// Convert the data back to frequency domain.
		fftReal->fft( dataOut );
	}
}




void DirectionalIR:: frequencyMagnitudePhaseToComplex( const Float32* dataIn, Float32* dataOut, Size fftSize )
{
	const Size complexSize = fftSize + 2;
	
	for ( Index i = 0; i < complexSize; i += 2 )
	{
		Float32 mag = dataIn[i];
		Float32 phase = dataIn[i+1];
		dataOut[i] = mag*math::cos( phase );
		dataOut[i+1] = mag*math::sin( phase );
	}
}




void DirectionalIR:: frequencyMagnitudePhaseToMagnitude( const Float32* dataIn, Float32* dataOut, Size fftSize )
{
	const Size magnitudeSize = fftSize / 2 + 1;
	
	for ( Index i = 0, j = 0; i < magnitudeSize; i++, j += 2 )
	{
		Float32 mag = dataIn[j];
		dataOut[i] = mag;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convex Hull Building Method
//############		
//##########################################################################################
//##########################################################################################




void DirectionalIR:: buildHull( const Sample* vertices, Size numVertices, ArrayList<Triangle>& triangles )
{
	// Return failure if there are less than 4 input vertices.
	if ( numVertices < 4 )
		return;
	
	//****************************************************************************
	// First, find the most extreme points along the 3 principle axes.
	
	Index extremeVertices[6] = { 0, 0, 0, 0, 0, 0 };
	
	for ( Index i = 1; i < numVertices; i++ )
	{
		const Vector3f& v = vertices[i].direction;
		
		if ( v.x < vertices[extremeVertices[0]].direction.x )
			extremeVertices[0] = i;
		
		if ( v.x > vertices[extremeVertices[1]].direction.x )
			extremeVertices[1] = i;
		
		if ( v.y < vertices[extremeVertices[2]].direction.y )
			extremeVertices[2] = i;
		
		if ( v.y > vertices[extremeVertices[3]].direction.y )
			extremeVertices[3] = i;
		
		if ( v.z < vertices[extremeVertices[4]].direction.z )
			extremeVertices[4] = i;
		
		if ( v.z > vertices[extremeVertices[5]].direction.z )
			extremeVertices[5] = i;
	}
	
	//****************************************************************************
	// Second, find the first 3 unique vertices of these 6 vertex indices.
	// Use these 3 vertices to form a double-sided triangle which is then recursively
	// subdivided until the hull encloses all input vertices. If there are only
	// 2 unique vertices, then pick a vertex at random from the input vertices that
	// is not one of the 2 already chosen. Use this vertex to complete the starting
	// double-sided triangle.
	
	Size numInitialVertices = 0;
	Index initialVertices[3];
	
	for ( Index i = 0; i < 6 && numInitialVertices < 3; i++ )
	{
		Index newVertexIndex = extremeVertices[i];
		Bool foundVertex = false;
		
		for ( Index j = 0; j < numInitialVertices; j++ )
			foundVertex |= (initialVertices[j] == newVertexIndex);
		
		if ( !foundVertex )
		{
			initialVertices[numInitialVertices] = newVertexIndex;
			numInitialVertices++;
		}
	}
	
	
	// Pick a vertex at random from the input vertices that is not one of the 
	// already chosen. Use this vertex to complete the starting double-sided triangle.
	if ( numInitialVertices == 2 )
	{
		for ( Index testVertexIndex = 0; testVertexIndex < numVertices; testVertexIndex++ )
		{
			if ( testVertexIndex != initialVertices[0] && testVertexIndex != initialVertices[1] )
			{
				initialVertices[2] = testVertexIndex;
				numInitialVertices = 3;
				break;
			}
		}
	}
	
	// Input vertices are probably coplanar, return that there was a failure.
	if ( numInitialVertices != 3 )
		return;
	
	//****************************************************************************
	// Setup the initial state of the algorithm.
	
	// Make sure that the output arrays of triangles and vertices are empty.
	triangles.clear();
	
	// Create a temporary array for the vertex data.
	Array<HullClass> vertexData( numVertices );
	vertexData.setAll( UNKNOWN_VERTEX );
	
	// Add the first two triangles to the convex hull. These triangles
	// have the same vertices but face in opposite directions.
	
	// Mark the vertices that are part of the starting hull triangles as part of the hull.
	vertexData[initialVertices[0]] = HULL_VERTEX;
	vertexData[initialVertices[1]] = HULL_VERTEX;
	vertexData[initialVertices[2]] = HULL_VERTEX;
	
	// Determine the plane of the starting triangle.
	math::Plane3f plane( vertices[initialVertices[0]].direction,
						vertices[initialVertices[1]].direction,
						vertices[initialVertices[2]].direction );
	
	// Add each 'side' of the starting triangle to the convex hull.
	triangles.add( Triangle( initialVertices[0], initialVertices[1], initialVertices[2], plane ) );
	triangles.add( Triangle( initialVertices[2], initialVertices[1], initialVertices[0], -plane ) );
	
	//****************************************************************************
	// Now, we recursively subdivide the triangle given by these three vertices
	// until the triangles contain all other vertices.
	
	// declare some arrays that are used to hold intermediate data while
	// we are computing the convex hull. This avoids multiple constructions/
	// destructions of these heap-allocated objects which only need to be
	// allocated once.
	ShortArrayList<Edge,32> edgesToFill;
	Bool foundExteriorVertices;
	
	const Float maxError = 0.0f;
	
	do
	{
		// Set that we haven't found any exterior vertices yet.
		// If this value is still false after searching for exterior vertices,
		// the algorithm is concluded.
		foundExteriorVertices = false;
		
		for ( Index t = 0; t < triangles.getSize(); t++ )
		{
			const Triangle& triangle = triangles[t];
			
			//**************************************************************************
			// Loop through all vertices in order to find the vertex (if it exists)
			// which is the farthest from the front face of the triangle.
			
			Index splittingVertexIndex;
			Float maximumVertexDistance = 0.0f;
			
			for ( Index v = 0; v < numVertices; v++ )
			{
				if ( vertexData[v] == UNKNOWN_VERTEX )
				{
					Float vertexDistance = triangle.plane.getSignedDistanceTo( vertices[v].direction );
					
					if ( vertexDistance > maximumVertexDistance )
					{
						maximumVertexDistance = vertexDistance;
						splittingVertexIndex = v;
					}
				}
			}
			
			//**************************************************************************
			// If there was a vertex which was beyond the plane of the triangle 
			// (in the direction of its normal), then we subdivide the triangle.
			
			if ( maximumVertexDistance > maxError )
			{
				// We have found an exterior vertex, mark a flag so that we perform the algorithm another iteration.
				foundExteriorVertices = true;
				
				edgesToFill.clear();
				
				// Find all of the triangles that need to be deleted because they face the splitting point.
				for ( Index t2 = 0; t2 < triangles.getSize(); t2++ )
				{
					const Triangle& deadTriangle = triangles[t2];
					
					// Compute the distance from the triangle's plane to the splitting vertex.
					Float vertexDistance = deadTriangle.plane.getSignedDistanceTo( vertices[splittingVertexIndex].direction );
					
					// If the vertex is in front of this triangle, remove the triangle.
					if ( vertexDistance > math::epsilon<Float>() )
					{
						// Check to make sure that the three edges of this triangle
						// are not in the set of edges already added. If so, mark
						// them so that they will not be added to the set of edges to fill.
						Bool foundEdge[3] = { false, false, false };
						
						for ( Index j = 0; j < edgesToFill.getSize(); )
						{
							const Edge& edge = edgesToFill[j];
							
							if ( edge.v1 == deadTriangle.v1 )
							{
								if ( edge.v2 == deadTriangle.v2 )
								{
									FoundEdgeV1V2:
									
									foundEdge[0] = true;
									edgesToFill.removeAtIndexUnordered(j);
									continue;
								}
								else if ( edge.v2 == deadTriangle.v3 )
								{
									FoundEdgeV1V3:
									
									foundEdge[2] = true;
									edgesToFill.removeAtIndexUnordered(j);
									continue;
								}
							}
							else if ( edge.v1 == deadTriangle.v2 )
							{
								if ( edge.v2 == deadTriangle.v1 )
									goto FoundEdgeV1V2;
								else if ( edge.v2 == deadTriangle.v3 )
								{
									FoundEdgeV2V3:
									
									foundEdge[1] = true;
									edgesToFill.removeAtIndexUnordered(j);
									continue;
								}
							}
							else if ( edge.v1 == deadTriangle.v3 )
							{
								if ( edge.v2 == deadTriangle.v1 )
									goto FoundEdgeV1V3;
								else if ( edge.v2 == deadTriangle.v2 )
									goto FoundEdgeV2V3;
							}
							
							j++;
						}
						
						//**************************************************************************
						// Add all three of the triangle's edges to the edge list
						// which indicates the edges that need to be attatched to the new vertex.
						
						if ( !foundEdge[0] )
							edgesToFill.addNew( deadTriangle.v1, deadTriangle.v2, 2 );
						
						if ( !foundEdge[1] )
							edgesToFill.addNew( deadTriangle.v2, deadTriangle.v3, 0 );
						
						if ( !foundEdge[2] )
							edgesToFill.addNew( deadTriangle.v1, deadTriangle.v3, 1 );
						
						// Remove this triangle from the convex hull.
						triangles.removeAtIndexUnordered( t2 );
						t2--;
						
						// Make sure that we don't skip any triangles in the outer triangle loop.
						if ( t2 <= t )
							t--;
					}
				}
				
				//**************************************************************************
				// Loop over each edge that needs to be completed into a triangle
				// and create and add the new triangle to the list of triangles in the convex hull.
				
				for ( Index i = 0; i < edgesToFill.getSize(); i++ )
				{
					const Edge& edge = edgesToFill[i];
					Index v1, v2, v3;
					
					switch ( edge.winding )
					{
						case 0:
						{
							v1 = splittingVertexIndex;
							v2 = edge.v1;
							v3 = edge.v2;
						}
						break;
						
						case 1:
						{
							v1 = edge.v1;
							v2 = splittingVertexIndex;
							v3 = edge.v2;
						}
						break;
						
						default:
						{
							v1 = edge.v1;
							v2 = edge.v2;
							v3 = splittingVertexIndex;
						}
						break;
					}
					
					math::Plane3f plane( vertices[v1].direction, vertices[v2].direction, vertices[v3].direction );
					
					// Ignore degenerate triangles.
					if ( math::isNAN( plane.offset ) )
						continue;
					
					
					// Find a vertex behind this triangle.
					Index backVertex = 0;
					
					for ( ; backVertex < numVertices; backVertex++ )
					{
						if ( vertexData[backVertex] == HULL_VERTEX &&
							backVertex != edge.v1 && backVertex != edge.v2 && backVertex != splittingVertexIndex )
							break;
					}
					
					// Reverse the plane so that it points outside the hull.
					if ( plane.getSignedDistanceTo( vertices[backVertex].direction ) > math::epsilon<Float>() )
						plane = -plane;
					
					triangles.addNew( v1, v2, v3, plane );
				}
				
				// Make this splitting vertex part of the convex hull.
				vertexData[splittingVertexIndex] = HULL_VERTEX;
			}
		}
	}
	while ( foundExteriorVertices );
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
