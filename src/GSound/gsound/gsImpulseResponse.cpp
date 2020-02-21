/*
 * Project:     GSound
 * 
 * File:        gsound/gsImpulseResponse.cpp
 * Contents:    gsound::ImpulseResponse class implementation
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


#include "gsImpulseResponse.h"


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




ImpulseResponse:: ImpulseResponse()
	:	buffer(),
		noiseRand( 42 )
{
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Update Method
//############
//##########################################################################################
//##########################################################################################




void ImpulseResponse:: setIR( const SoundSourceIR& sourceIR, const SoundListener& listener,
								const IRRequest& request )
{
	const Size numFrequencyBands = request.frequencies.getBandCount();
	const ChannelLayout& channelLayout = request.channelLayout;
	const SampleRate sampleRate = sourceIR.getSampleRate();
	const Size numChannels = channelLayout.getChannelCount();
	buffer.setLayout( channelLayout );
	buffer.setSampleRate( sampleRate );
	frequencies = request.frequencies;
	//****************************************************************************
	// Find the latest impulse to determine the response length.
	
	const Size filterBufferLength = 2048; // padding for crossover filters
	const Size irLengthInSamples = sourceIR.getLengthInSamples();
	const Size paddedIRLength = irLengthInSamples + filterBufferLength;
	const Size numPaths = sourceIR.getPathCount();
	const SampledIR& sampledIR = sourceIR.getSampledIR();
	const Size sampledIRStart = sampledIR.getStartTimeInSamples();
	const Size sampledIRLength = sampledIR.getLengthInSamples();
	
	//****************************************************************************
	// Bin the energy in the IR if requested.
	
	if ( request.binEnergy || request.metrics )
	{
		// Resize the bins to be the right size to hold the IR bins.
		const Size binSize = Size(math::ceiling(request.binTime*sampleRate));
		const Size binCount = Size(math::ceiling(Float(sourceIR.getLengthInSamples()) / Float(binSize)));
		
		if ( bins.getChannelCount() != Size(1) || bins.getSampleCount() != binCount )
			bins.setFormat( Size(1), binCount );
		
		bins.allocate();
		bins.zero();
		SIMDBands* binsStart = bins.getChannel(0);
		
		// Bin the sampledIR.
		for ( Index start = 0, b = 0; start < sampledIRLength; b++ )
		{
			const Size bSize = math::min( sampledIRLength - start, binSize );
			binsStart[b] = math::sum( (const SIMDBands*)sampledIR.getIntensity() + start, bSize );
			start += bSize;
		}
		
		// Add the path contributions.
		for ( Index i = 0; i < numPaths; i++ )
		{
			const SoundPath& path = sourceIR.getPath(i);
			Index binIndex = (Index)math::floor( path.getDelay() / request.binTime );
			binsStart[binIndex] += SIMDBands::loadUnaligned( (Float*)&path.getIntensity() );
		}
		
		// Compute acoustic metrics for the IR.
		if ( request.metrics )
		{
			for ( Index band = 0; band < numFrequencyBands; band++ )
				getMetrics( &bins.getChannel(0)[0][band], bins.getSampleCount(), numFrequencyBands, request.binTime, 60.0f, metrics, band );
		}
	}
	
	// Don't go further if the IR is not requested.
	if ( !request.ir )
		return;
	
	//****************************************************************************
	// Make sure the temporary storage is big enough.
	
	// Resize the sound buffer to be the right size to hold the last impulse.
	if ( buffer.getSize() != paddedIRLength )
		buffer.setSize( paddedIRLength );
	
	buffer.zero();
	
	const Size bandIRSize = paddedIRLength*frequencies.getBandCount();
	
	if ( bandIRs.getChannelCount() < numChannels || bandIRs.getSampleCount() < bandIRSize )
		bandIRs.setFormat( numChannels, bandIRSize );
	
	if ( pan.getChannelCount() < numChannels || pan.getSampleCount() < sampledIRLength )
		pan.setFormat( numChannels, sampledIRLength );
	
	bandIRs.allocate();
	pan.allocate();

	// Make sure the crossover has the correct frequency bands.
	crossover.setBands( request.frequencies, sampleRate );
	
	// Regenerate the noise buffer if it is too short.
	if ( noise.getSize() < paddedIRLength )
	{
		noise.allocate( paddedIRLength );
		
		for ( Index i = 0; i < paddedIRLength; i++ )
			noise[i] = noiseRand.sample( -1.0f, 1.0f );
		
		CrossoverType::History history;
		crossover.filterSIMD( history, (Float32*)noise.getPointer(), (Float32*)noise.getPointer(), paddedIRLength );
	}
	
	//****************************************************************************
	// Interleave the IRs for each band.
	
	// Pan the IR directions based on the channel layout.
	panDirections( sampledIR, channelLayout, listener.getOrientation(), pan );

	// Interleave the sampled IR bands for each channel.
	for ( Index c = 0; c < numChannels; c++ )
	{
		Float* channel = bandIRs.getChannel(c);
		
		// Zero the first part before the start of the IR.
		om::util::zero( channel, sampledIRStart*numFrequencyBands );
		
		// Interleave the frequency bands for the main part of the IR.
		interleaveBands( sampledIR, pan.getChannel(c), channel );
		
		// Zero the part past the end of the sampled IR.
		if ( paddedIRLength > sampledIRLength )
			om::util::zero( channel + sampledIRLength*numFrequencyBands, (paddedIRLength - sampledIRLength)*numFrequencyBands );
	}
	
	//****************************************************************************
	// Filter the interleaved IR and write the final IR output.
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		SIMDBands* irC = (SIMDBands*)bandIRs.getChannel(c);
		Float* outputC = buffer.getChannel(c);
		
		// Convert from energy to pressure magnitude.
		for ( Index i = 0; i < irLengthInSamples; i++ )
			irC[i] = math::sqrt( irC[i] );
		
		// Low-pass filter the energy histograms to remove high-frequency noise.
		CrossoverType::History crossoverHistory;
		crossover.filterSIMDLowPass( crossoverHistory, (Float32*)irC, (Float32*)irC, irLengthInSamples );
		
		for ( Index i = 0; i < irLengthInSamples; i++ )
			outputC[i] = math::sumScalar(noise[i] * irC[i]);
	}
	
	//****************************************************************************
	
	if ( request.hrtf != NULL && request.hrtf->getChannelCount() == numChannels )
	{
		const Size maxHRTFOrder = 4;
		
		// Generate the HRTF filter.
		if ( request.hrtf != hrtf )
		{
			hrtf = request.hrtf;
			hrtfFilter.setHRTF( *hrtf, sampleRate, maxHRTFOrder );

			const Size bufferLength = hrtfFilter.getFilterLength() + 2;
			
			if ( hrtfBuffer.getChannelCount() != 1 || hrtfBuffer.getSampleCount() < bufferLength )
				hrtfBuffer.setFormat( 1, bufferLength );
			
			hrtfBuffer.allocate();
			
			if ( bandIRs.getSampleCount() < hrtfFilter.getFilterLength()*numFrequencyBands )
				bandIRs.setFormat( numChannels, hrtfFilter.getFilterLength()*numFrequencyBands );
		}
		
		const Size hrtfLength = hrtfFilter.getFilterLength();
		
		for ( Index i = 0; i < numPaths; i++ )
		{
			const SoundPath& path = sourceIR.getPath(i);
			const Float delay = path.getDistance() / path.getSpeed();
			const Index sampleIndex = (Index)math::floor( delay*sampleRate );
			const FrequencyBandResponse& energy = path.getIntensity();
			const SIMDBands energyBands = math::sqrt( SIMDBands::loadUnaligned( (Float*)&energy ) );
			
			// Generate a SH basis for the path.
			SH::cartesian( maxHRTFOrder, path.getDirection()*listener.getOrientation(), shBasis );
			
			for ( Index c = 0; c < numChannels; c++ )
			{
				Float* const hrtfChannel = hrtfBuffer.getChannel(0);
				SIMDBands* const hrtfChannelBands = (SIMDBands*)bandIRs.getChannel(0);
				Float* const output = buffer.getChannel(c) + sampleIndex;
				
				// Get the time-domain filter for this path's direction and channel.
				hrtfFilter.getFilter( c, shBasis, hrtfChannel );
				
				// Filter the HRTF into the frequency bands.
				CrossoverType::History crossoverHistory;
				crossover.filterScalar( crossoverHistory, hrtfChannel, (Float*)hrtfChannelBands, hrtfLength );
				
				const Size length = math::min( paddedIRLength - sampleIndex, hrtfLength );
				
				// Add the HRTF to the IR and multiply by the path's pressure.
				for ( Index j = 0; j < length; j++ )
					output[j] += math::sumScalar( energyBands * hrtfChannelBands[j] );
			}
		}
	}
	else
	{
		if ( hrtfBuffer.getChannelCount() != 1 || hrtfBuffer.getSampleCount() < filterBufferLength )
			hrtfBuffer.setFormat( 1, filterBufferLength );
		
		if ( bandIRs.getSampleCount() < filterBufferLength*numFrequencyBands )
			bandIRs.setFormat( numChannels, filterBufferLength*numFrequencyBands );

		hrtfBuffer.allocate();
		hrtfBuffer.zero();
		hrtfBuffer.getChannel(0)[0] = 1;

		// Filter the HRTF into the frequency bands.
		CrossoverType::History crossoverHistory;
		crossover.filterScalar( crossoverHistory, hrtfBuffer.getChannel(0), (Float*)bandIRs.getChannel(0), filterBufferLength );
		

		for ( Index i = 0; i < numPaths; i++ )
		{
			const SoundPath& path = sourceIR.getPath(i);
			const Float delay = path.getDistance() / path.getSpeed();
			const Index sampleIndex = (Index)math::floor( delay*sampleRate );
			const FrequencyBandResponse& energy = path.getIntensity();
			const SIMDBands energyBands = math::sqrt( SIMDBands::loadUnaligned( (Float*)&energy ) );
			
			// Pan the impulse among the output channel layout. Skip the impulse if panning failed.
			if ( !channelLayout.panDirection( path.getDirection()*listener.getOrientation(), channelGains ) )
				continue;
			
			for ( Index c = 0; c < numChannels; c++ )
			{
				SIMDBands* const hrtfChannelBands = (SIMDBands*)bandIRs.getChannel(0);
				Float* const output = buffer.getChannel(c) + sampleIndex;
				
				const Size length = math::min( paddedIRLength - sampleIndex, filterBufferLength );
				
				// Add the HRTF to the IR and multiply by the path's pressure.
				for ( Index j = 0; j < length; j++ )
				{
					for ( Index b = 0; b < numFrequencyBands; b++ )
						output[j] += (energyBands[b] * hrtfChannelBands[j][b])*channelGains[c];
				}
			}
		}
	}
	
	//****************************************************************************
	// Scale the impulse response based on source/listener power.
	
	if ( request.normalize )
	{
		// Find the largest sample.
		Float maxSample = 0;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Float* channel = (const Float*)buffer.getChannel(c);
			maxSample = math::max( maxSample, math::max(
											math::abs( math::min( channel, paddedIRLength ) ),
											math::max( channel, paddedIRLength ) ) );
		}
		
		if ( maxSample != 0 )
			buffer.applyGain( Float(1)/maxSample );
	}
	else
	{
		// Compute the total power of the sources.
		const Size numSources = sourceIR.getSourceCount();
		Float totalPower = 0;
		
		for ( Index s = 0; s < numSources; s++ )
			totalPower += sourceIR.getSource(s)->getPower();
		
		// Compute the power sensitivity of the listener.
		const Real listenerPowerDB = listener.getSensitivity() + Real(10)*math::log10( Real(4)*gsound::math::pi<Real>() );
		const Real listenerGain = math::pow( Real(10), listenerPowerDB / Real(10) );
		
		// Apply the gain due to the source and listener.
		buffer.applyGain( (totalPower/(Real(4)*math::pi<Real>()))*listenerGain );
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		Band Interleaving Method
//############
//##########################################################################################
//##########################################################################################




void ImpulseResponse:: interleaveBands( const SampledIR& ir, const Float* pan, Float* output )
{
	// Zero the first bit of the interleaved IR.
	om::util::zeroPOD( (SIMDBands*)output, ir.getStartTimeInSamples() );
	
	// Do the panning for the rest of the IR.
	const SIMDBands* bands = (const SIMDBands*)ir.getIntensity() + ir.getStartTimeInSamples();
	SIMDBands* outputBands = (SIMDBands*)output + ir.getStartTimeInSamples();
	const Float* const panEnd = pan + ir.getLengthInSamples();
	pan += ir.getStartTimeInSamples();
	
	while ( pan != panEnd )
	{
		*outputBands = *bands*SIMDBands(*pan);
		
		pan++;
		bands++;
		outputBands++;
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		Direction Panning Method
//############
//##########################################################################################
//##########################################################################################




void ImpulseResponse:: panDirections( const SampledIR& ir, const ChannelLayout& channelLayout, const Matrix3f& orientation,
										internal::SampleBuffer<Float>& pan )
{
	const Size numChannels = channelLayout.getChannelCount();
	const Index irStart = ir.getStartTimeInSamples();
	const Size irLength = ir.getLengthInSamples();

	if (channelLayout.getType() == ChannelLayout::AMBISONIC_B)
	{
		// Ambisonic IR

		// Build the panning buffer.
		const Vector3f* directions = ir.getDirections() + irStart;
		for ( Index i = irStart; i < irLength; i++, directions++ )
		{
			Real directionMagnitude2 = (*directions).getMagnitudeSquared();
			if ( directionMagnitude2 > math::epsilon<Real>() )
			{
				// Compute the normalized panning direction for this sample.
				const Vector3f d = (*directions / math::sqrt(directionMagnitude2))*orientation;

				// Pan XYZ channels
				Float azimuth, elevation;
				azimuth = math::atan2( -d.z, d.x );
				elevation = math::asin( d.y );
				pan.getChannel(0)[i] = math::sqrt(2.0f) / 2;
				pan.getChannel(1)[i] = math::abs(math::cos(azimuth) * math::cos(elevation));
				pan.getChannel(2)[i] = math::abs(math::sin(azimuth) * math::cos(elevation));
				pan.getChannel(3)[i] = math::abs(math::sin(elevation));
			}
			else
			{
				pan.getChannel(0)[i] = math::sqrt(2.0f) / 2;
				for ( Index c = 1; c < numChannels; c++ )
					pan.getChannel(c)[i] = Float32(1);
			}
		}
	}
	else
	{
		switch ( numChannels )
		{
			// Mono IR.
			case 1:
			{
				om::util::set( pan.getChannel(0) + irStart, Float32(1), irLength - irStart );
			}
				break;

			default:
			{
				// Build the panning buffer.
				const Vector3f* directions = ir.getDirections() + irStart;

				for ( Index i = irStart; i < irLength; i++, directions++ )
				{
					Real directionMagnitude2 = (*directions).getMagnitudeSquared();

					if ( directionMagnitude2 > math::epsilon<Real>() )
					{
						// Compute the normalized panning direction for this sample.
						const Vector3f d = (*directions / math::sqrt(directionMagnitude2))*orientation;

						// Pan the impulse among the output channel layout. Skip the impulse if panning failed.
						if ( channelLayout.panDirection( d, channelGains ) )
						{
							for ( Index c = 0; c < numChannels; c++ )
								pan.getChannel(c)[i] = channelGains[c];
						}
					}
					else
					{
						for ( Index c = 0; c < numChannels; c++ )
							pan.getChannel(c)[i] = Float32(1);
					}
				}
			}
				break;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		Metrics Method
//############
//##########################################################################################
//##########################################################################################




void ImpulseResponse:: getMetrics( const SoundSourceIR& ir, const FrequencyBands& frequencies, Float snrDB, IRMetrics& metrics )
{
	//****************************************************************************
	// Build the energy time curve from the IR.
	
	const SampledIR& sampledIR = ir.getSampledIR();
	const Size irLength = ir.getLengthInSamples();
	const Size sampledIRStart = sampledIR.getStartTimeInSamples();
	const Size sampledIRLength = sampledIR.getLengthInSamples();
	const Float sampleRate = (Float)ir.getSampleRate();
	const SIMDBands* irSamples = (const SIMDBands*)sampledIR.getIntensity();
	
	om::PODArray<SIMDBands,1,Size,AlignedAllocator<16> > etc;
	etc.allocate( irLength );
	etc.zero();
	
	// Copy the sampled IR into the etc.
	om::util::copyPOD( etc.getPointer() + sampledIRStart, irSamples + sampledIRStart, sampledIRLength - sampledIRStart );
	
	// Add all paths to the etc.
	const Size numPaths = ir.getPathCount();
	
	for ( Index i = 0; i < numPaths; i++ )
	{
		const SoundPath& path = ir.getPath(i);
		const Float delay = path.getDistance() / path.getSpeed();
		const Index sampleIndex = (Index)math::floor( delay*sampleRate );
		
		etc[sampleIndex] += SIMDBands::loadUnaligned( (Float*)&path.getIntensity() );
	}
	
	//****************************************************************************
	
	const Float binTime = 0.01f;
	const Size binSize = Size(math::ceiling(binTime*ir.getSampleRate()));
	const Size binCount = Size(math::ceiling(Float(irLength) / Float(binSize)));
	om::PODArray<Float32,1,Size,AlignedAllocator<16> > bins;
	bins.allocate( binCount );
	
	// Get the metrics for each frequency band.
	for ( Index band = 0; band < GSOUND_FREQUENCY_COUNT; band++ )
	{
		// Bin the IR.
		bins.zero();
		for ( Index i = 0, b = 0; i < irLength; b++ )
		{
			const Size bSize = math::min( irLength - i, binSize );
			const Size bEnd = i + bSize;
			
			for ( Index j = i; j < bEnd; j++ )
				bins[b] += etc[j][band];
			
			i += bSize;
		}
		
		// Get the metrics for this frequency band.
		getMetrics( bins.getPointer(), binCount, 1, binTime, snrDB, metrics, band );
	}
}




void ImpulseResponse:: getMetrics( const SoundBuffer& ir, const FrequencyBands& frequencies, Float snrDB, IRMetrics& metrics )
{
	const Size irLength = ir.getSize();
	
	CrossoverType crossover;
	CrossoverType::History crossoverHistory;
	crossover.setBands( frequencies, ir.getSampleRate() );
	
	// Filter into frequency bands.
	om::PODArray<SIMDBands,1,Size,AlignedAllocator<16> > bands;
	bands.allocate( irLength );
	crossover.filterScalar( crossoverHistory, ir.getChannel(0), (Float32*)bands.getPointer(), irLength );
	
	SoundBuffer bandIR( 1, irLength );
	SoundBuffer bandETC( 1, irLength );
	
	const Float binTime = 0.01f;
	const Size binSize = Size(math::ceiling(binTime*ir.getSampleRate()));
	const Size binCount = Size(math::ceiling(Float(irLength) / Float(binSize)));
	om::PODArray<Float32,1,Size,AlignedAllocator<16> > bins;
	bins.allocate( binCount );
	
	// Compute energy time curves for each frequency band.
	for ( Index band = 0; band < GSOUND_FREQUENCY_COUNT; band++ )
	{
		// Copy the IR for the band.
		for ( Index i = 0; i < irLength; i++ )
			bandIR.getChannel(0)[i] = bands[i][band];
		
		// Compute the ETC for the band.
		energyTimeCurve( bandIR, bandETC );
		
		// Convert to intensity.
		om::math::multiply( bandETC.getChannel(0), bandETC.getChannel(0), irLength );
		
		// Bin the IR.
		bins.zero();
		for ( Index i = 0, b = 0; i < irLength; b++ )
		{
			const Size bSize = math::min( irLength - i, binSize );
			const Size bEnd = i + bSize;
			
			for ( Index j = i; j < bEnd; j++ )
				bins[b] += bandETC.getChannel(0)[j];
			
			i += bSize;
		}
		
		// Get the metrics for this frequency band.
		getMetrics( bins.getPointer(), binCount, 1, binTime, snrDB, metrics, band );
	}
}




template < typename T >
static Index advanceMax( const T* bins, Size numBins, Size stride, Index position )
{
	// Find the maximum bin after this point.
	Index maxBinIndex = position;
	Float maxBin = math::min<Float>();
	
	for ( Index i = position; i < numBins; i++ )
	{
		const Float bin = bins[i*stride];
		
		if ( bin > maxBin )
		{
			maxBin = bin;
			maxBinIndex = i;
		}
	}
	
	return maxBinIndex;
}




template < typename T >
void ImpulseResponse:: getMetrics( const T* intensity, Size irLength, Size stride, Float binSize, Float snrDB, IRMetrics& metrics, Index band )
{
	// Find the maximum intensity in the IR.
	T maxI = 0;
	
	for ( Index i = 0; i < irLength; i++ )
		maxI = math::max( intensity[i*stride], maxI );
	
	const Float snr = math::pow( 10.0f, -snrDB/10.0f );
	const Float noiseFloor = maxI * snr;
	
	
	/*
	// The number of bins to use from the end of the IR to determine the noise floor.
	const Float noiseFloorTime = 0.4;
	const Size noiseFloorBins = om::math::min( (Size)om::math::ceiling( noiseFloorTime / binSize ), irLength );
	T noiseMax = om::math::negativeInfinity<Float>();
	
	for ( Index i = irLength; i > (irLength - noiseFloorBins); i-- )
		noiseMax = om::math::max( noiseMax, intensity[(i-1)*stride] );
	
	const Float snrSlop = 40;
	const T noiseFloor = noiseMax*snrSlop;
	*/
	//****************************************************************************
	// Find the start of the impulse response decay.
	
	Index irStart = 0;
	Index decayStart = 0;
	
	{
		// Find the first non-zero bin in the IR.
		for ( ; decayStart < irLength; decayStart++ )
		{
			if ( intensity[decayStart*stride] > noiseFloor )
				break;
		}
		
		irStart = decayStart;
		
		// Skip the first non-zero bin to ignore the direct sound.
		decayStart++;
		
		// Find the maximum bin after this point.
		decayStart = advanceMax( intensity, irLength, stride, decayStart );
		
		// Check to see if there is a local minima between this peak and the previous.
		const Float minimaTresholdDB = -10.0f;
		const Float minimaThreshold = math::pow( 10.0f, minimaTresholdDB/10.0f );
		
		while ( decayStart + 1 < irLength && intensity[(decayStart + 1)*stride] > noiseFloor )
		{
			// Advance to the next maximum.
			const Index nextMax = advanceMax( intensity, irLength, stride, decayStart + 1 );
			
			// Check to see if there is a local minima between this peak and the previous.
			const Float minimaEnergy = intensity[nextMax*stride]*minimaThreshold;
			Bool foundMinima = false;
			
			for ( Index i = decayStart + 1; i < nextMax; i++ )
			{
				if ( intensity[i*stride] < minimaEnergy )
				{
					foundMinima = true;
					break;
				}
			}
			
			// If there was a local minima, advance to the next max.
			if ( foundMinima )
				decayStart = nextMax;
			else
				break;
		}
		
		// Skip the maximum.
		decayStart++;
	}
	
	//****************************************************************************
	// Find the end of the impulse response decay.
	
	Index irEnd = irLength;
	
	{
		while ( irEnd > irStart && intensity[(irEnd-1)*stride] < noiseFloor )
			irEnd--;
	}
	
	Array<T> reverseTime(irLength);
	T reverseTimeTotal = 0;
	
	for ( Index i = irLength; i > 0; i-- )
	{
		reverseTimeTotal += intensity[(i-1)*stride];
		reverseTime[i-1] = reverseTimeTotal;
	}
	
	const Float decayStartEnergy = reverseTime[decayStart];
	
	//****************************************************************************
	// Compute the T60.
	
	{
		// Find the first bin that is -5dB down from the decay start energy.
		Index t60Start = decayStart;
		const Float db5Threshold = decayStartEnergy*math::pow( Float(10), Float(-5)/Float(10) );
		
		for ( ; t60Start < irEnd; t60Start++ )
		{
			Float e = reverseTime[t60Start];
			
			if ( e <= db5Threshold )
				break;
		}
		
		// Find the first bin that is -35dB down from the decay start energy.
		Index t60End = t60Start;
		const Float db65Threshold = decayStartEnergy*math::pow( Float(10), Float(-35)/Float(10) );
		
		for ( ; t60End < irEnd; t60End++ )
		{
			Float e = reverseTime[t60End];
			
			if ( e <= db65Threshold && e > noiseFloor )
			{
				t60End++;
				break;
			}
		}
		
		// Do a linear least-squares fit of the decay curve.
		const Float invN = Float(1) / Float(t60End - t60Start);
		
		// Compute the mean along the X direction.
		T sumX = 0;
		Float time = Float(0.5)*binSize + t60Start*binSize;
		
		for ( Index i = t60Start; i < t60End; i++ )
		{
			sumX += time;
			time += binSize;
		}
		
		const Float meanX = sumX * invN;
		
		// Compute the sum of squared residuals.
		time = Float(0.5)*binSize + t60Start*binSize;
		T sumRX2 = 0;
		T sumRXY = 0.0f;
		//SIMDBands sumY = 0.0f;
		
		for ( Index i = t60Start; i < t60End; i++ )
		{
			Float rX = (time - meanX);
			T dB = Float(10)*math::log10( reverseTime[i] );
			sumRX2 += rX*rX;
			sumRXY += rX*dB;
			//sumY += dB;
			time += binSize;
		}
		
		// Compute the slope (as decay rate in dB/second) and intercept.
		T slope = sumRXY / sumRX2;
		//T intercept = (sumY - sumX*slope)*invN;
		
		// Compute the T60 as the time to decay by 60db.
		T t60 = T(-60.0f) / slope;
		
		metrics.t60[band] = t60;
	}
	
	//****************************************************************************
	// Compute the EDT.
	
	{
		// Find the first bin that is -10dB down from the decay start energy.
		Index edtStart = decayStart;
		Index edtEnd = edtStart;
		const Float db10Threshold = decayStartEnergy*math::pow( Float(10), Float(-10)/Float(10) );
		
		for ( ; edtEnd < irEnd; edtEnd++ )
		{
			Float e = reverseTime[edtEnd];
			
			if ( e <= db10Threshold && e > noiseFloor )
			{
				edtEnd++;
				break;
			}
		}
		
		// Do a linear least-squares fit of the decay curve.
		const Float invN = Float(1) / Float(edtEnd - edtStart);
		
		// Compute the mean along the X direction.
		Float sumX = 0;
		Float time = Float(0.5)*binSize + edtStart*binSize;
		
		for ( Index i = edtStart; i < edtEnd; i++ )
		{
			sumX += time;
			time += binSize;
		}
		
		const Float meanX = sumX * invN;
		
		// Compute the sum of squared residuals.
		time = Float(0.5)*binSize + edtStart*binSize;
		Float sumRX2 = 0;
		T sumRXY = 0.0f;
		
		for ( Index i = edtStart; i < edtEnd; i++ )
		{
			Float rX = (time - meanX);
			T dB = T(10)*math::log10( reverseTime[i] );
			sumRX2 += rX*rX;
			sumRXY += rX*dB;
			time += binSize;
		}
		
		// Compute the slope (as decay rate in dB/second) and intercept.
		T slope = sumRXY / sumRX2;
		
		// Compute the EDT as the time to decay by 60db.
		T edt = T(-60.0f) / slope;
		
		metrics.edt[band] = edt;
	}
	
	//****************************************************************************
	// Compute the C50, C80, and D50 metrics.
	
	{
		const T* nonZeroBins = intensity + irStart*stride;
		const Size numNonZeroBins = irEnd - irStart;
		const Size numBins50ms = math::min( (Size)math::round( Float(0.05) / binSize ), numNonZeroBins );
		const Size numBins80ms = math::min( (Size)math::round( Float(0.08) / binSize ), numNonZeroBins );
		
		// Sum the energy in the first 50ms.
		T energy50ms = 0;
		for ( Index i = 0; i < numBins50ms; i++ )
			energy50ms += nonZeroBins[i*stride];
		
		// Sum the energy in the first 80ms.
		T energy80ms = 0;
		for ( Index i = 0; i < numBins80ms; i++ )
			energy80ms += nonZeroBins[i*stride];
		
		// Sum the energy after the first 50ms.
		T energyAfter50ms = 0;
		for ( Index i = numBins50ms; i < numNonZeroBins; i++ )
			energyAfter50ms += nonZeroBins[i*stride];
		
		// Sum the energy after the first 80ms.
		T energyAfter80ms = 0;
		for ( Index i = numBins80ms; i < numNonZeroBins; i++ )
			energyAfter80ms += nonZeroBins[i*stride];
		
		// Sum the total energy.
		T energyTotal = 0;
		for ( Index i = 0; i < numNonZeroBins; i++ )
			energyTotal += nonZeroBins[i*stride];
		
		T c50 = T(10) * math::log10( energy50ms / energyAfter50ms );
		T c80 = T(10) * math::log10( energy80ms / energyAfter80ms );
		T d50 = energy50ms / energyTotal;
		
		metrics.c50[band] = c50;
		metrics.c80[band] = c80;
		metrics.d50[band] = d50;
	}
	
	//****************************************************************************
	// Compute the G metric.
	
	{
		// Sum the total energy.
		T energyTotal = 0;
		for ( Index i = irStart; i < irEnd; i++ )
			energyTotal += intensity[i*stride];
		
		T anechoic10m = Float(1) / (Float(4)*math::pi<Float>()*(Float(1) + math::square(Float(10))));
		T g = Float(10) * math::log10( energyTotal / anechoic10m );
		
		metrics.g[band] = g;
	}
	
	//****************************************************************************
	// Compute the TS metric.
	
	{
		T center = T(0);
		T total = T(0);
		T time = T(0.5)*binSize;
		
		for ( Index i = irStart; i < irEnd; i++ )
		{
			center += intensity[i*stride]*time;
			total += intensity[i*stride];
			time += binSize;
		}
		
		T ts = center / total;
		
		metrics.ts[band] = ts;
	}
}




void ImpulseResponse:: energyTimeCurve( const SoundBuffer& input, SoundBuffer& result )
{
	const Size irLength = input.getSize();
	const Size fftSize = om::math::nextPowerOfTwo( irLength );
	const Size magnitudeSize = fftSize / 2 + 1;
	const Float32 fftNormalize = Float32(1) / Float32(fftSize);
	
	result.setFormat( 1, irLength );
	Sample32f* etc = result.getChannel(0);
	const Sample32f* ir = input.getChannel(0);
	
	typedef om::math::Complex<Float> Complex32f;
	math::FFTComplex<Float32> fft( fftSize );
	om::PODArray<Complex32f,1,Size,AlignedAllocator<16> > tempBuffer;
	om::PODArray<Complex32f,1,Size,AlignedAllocator<16> > tempBuffer2;
	tempBuffer.allocate( fftSize );
	tempBuffer2.allocate( fftSize );
	Complex32f* temp = tempBuffer.getPointer();
	Complex32f* temp2 = tempBuffer2.getPointer();
	
	// Copy the IR to the temp buffer.
	om::util::convert( temp, ir, irLength );
	om::util::zero( temp + irLength, fftSize - irLength );
	
	//***********************************************************************
	// Compute the hilbert transform.
	
	// Transform into frequency domain.
	fft.fft( temp, temp2 );
	
	// Set DC frequency to zero.
	temp[0] = 0;
	
	// Multiply positive frequencies by -i
	for ( Index f = 1; f < magnitudeSize; f++ )
		temp[f] = temp[f]*Complex32f(0,-1);
	
	// Multiply negative frequencies by i
	for ( Index f = magnitudeSize; f < fftSize; f++ )
		temp[f] = temp[f]*Complex32f(0,1);
	
	// Transform back to time domain.
	fft.ifft( temp, temp2 );
	
	// Add the result to the original IR to get the envelope.
	for ( Index i = 0; i < irLength; i++ )
		etc[i] = om::math::sqrt( ir[i]*ir[i] + om::math::square( temp[i].r*fftNormalize ) );
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
