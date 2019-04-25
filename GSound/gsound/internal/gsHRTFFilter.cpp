/*
 *  gsHRTFFilter.cpp
 *  GSound
 *
 *  Created by Carl Schissler on 3/3/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#include "gsHRTFFilter.h"


#include "fftw3.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Sample Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class HRTFFilter:: Sample
{
	public:
		
		/// Create a new HRTF sample for the given 3D normalized direction.
		GSOUND_INLINE Sample( const Vector3f& newDirection, const Filter& newFilter )
			:	direction( newDirection ),
				filter( newFilter )
		{
		}
		
		
		/// The normalized 3D cartesian direction for this HRTF sample.
		Vector3f direction;
		
		
		/// The filter for this HRTF sample.
		Filter filter;
		
		
};





//##########################################################################################
//##########################################################################################
//############		
//############		FFT Data Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class HRTFFilter:: FFTData
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			GSOUND_INLINE FFTData( Size newLength )
				:	length( newLength )
			{
				fftPlan = fftwf_plan_dft_r2c_1d( (int)newLength, NULL, NULL, FFTW_ESTIMATE | FFTW_DESTROY_INPUT );
				ifftPlan = fftwf_plan_dft_c2r_1d( (int)newLength, NULL, NULL, FFTW_ESTIMATE | FFTW_DESTROY_INPUT );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			GSOUND_INLINE ~FFTData()
			{
				if ( fftPlan )
					fftwf_destroy_plan( fftPlan );
				
				if ( ifftPlan )
					fftwf_destroy_plan( ifftPlan );
			}
			
			
		//********************************************************************************
		//******	FFT Methods
			
			
			GSOUND_FORCE_INLINE void fft( Float32* inOut ) const
			{
				fftwf_execute_dft_r2c( fftPlan, (float*)inOut, (fftwf_complex*)inOut );
			}
			
			
			GSOUND_FORCE_INLINE void ifft( Float32* inOut ) const
			{
				fftwf_execute_dft_c2r( ifftPlan, (fftwf_complex*)inOut, (float*)inOut );
			}
			
			
		//********************************************************************************
		//******	Data Members
			
			
			/// An FFT plan for this FFT data.
			fftwf_plan fftPlan;
			
			/// An inverse FFT plan for this FFT data.
			fftwf_plan ifftPlan;
			
			/// The length of the real FFT for this FFT data object.
			Size length;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




HRTFFilter:: HRTFFilter()
	:	channels(),
		order( 0 ),
		fftData( NULL ),
		sampleRate( 0 )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




HRTFFilter:: ~HRTFFilter()
{
	deinitializeFFTData();
}




//##########################################################################################
//##########################################################################################
//############		
//############		HRTF Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFFilter:: setHRTF( const HRTF& newHRTF, SampleRate newSampleRate, Size maxOrder, Float maxError,
							Float convergence, Size numIntegrationSamples )
{
	const Size maxIRLength = newHRTF.getFilterLength();
	const Size numChannels = newHRTF.getChannelCount();
	
	// If the max IR length is 0, then this is not a valid HRTF.
	if ( maxIRLength == 0 || numChannels == 0 || newSampleRate <= SampleRate(0) )
		return false;
	
	// Make sure this HRTF has the right number of channels.
	if ( channels.getSize() != numChannels )
		channels.setSize( numChannels );
	
	// Determine the padded filter length.
	sampleRate = newSampleRate;
	const Float resampleFactor = sampleRate == newHRTF.getSampleRate() ? Float(1) : Float(sampleRate / newHRTF.getSampleRate());
	const Size resampledLength = (Size)math::ceiling( resampleFactor*Float(maxIRLength) );
	length = math::isPowerOfTwo(resampledLength) ? resampledLength : math::nextPowerOfTwo(resampledLength);
	const Size paddedLength = length + 2;
	
	// Initialize the FFT data if it has not yet been initialized.
	if ( fftData == NULL || fftData->length != length )
		initializeFFTData( length );
	
	// A temporary array of the frequency-domain HRTF samples for this HRTF.
	om::ShortArray<ArrayList<Sample>,2> tempChannels( numChannels );
	
	// A temporary spherical harmonic basis used for HRTF projection.
	Array<Float> basis;
	
	// A random variable used for monte-carlo integration of HRTF.
	math::Random<Float> randomVariable;
	
	// A temporary filter.
	Filter tempFilter( paddedLength );
	
	//*******************************************************************************
	// Convert each sample in the new HRTF to frequency domain.
	
	const Matrix3f& orientation = newHRTF.getOrientation();
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Size numSamples = newHRTF.getSampleCount(c);
		ArrayList<Sample>& samples = tempChannels[c];
		samples.setCapacity( numSamples + numIntegrationSamples );
		
		for ( Index i = 0; i < numSamples; i++ )
		{
			Vector3f direction = orientation*newHRTF.getSampleDirection(c, i);
			const Float* ir = newHRTF.getSampleData(c, i);
			const Size irLength = maxIRLength;
			
			// Create a new reference-counted filter.
			Filter filter( paddedLength );
			
			if ( resampleFactor == Float(1) )
			{
				om::util::copy( filter.getScalars(), ir, irLength );
				om::util::zero( filter.getScalars() + irLength, paddedLength - irLength );
			}
			else
			{
				// Resample the IR.
				math::resample( ir, irLength, filter.getScalars(), resampleFactor, 256 );
				const Size resampledLength = (Size)math::ceiling( Float(irLength)*resampleFactor );
				om::util::zero( filter.getScalars() + resampledLength, paddedLength - resampledLength );
			}
			
			// Add the sample filter.
			samples.add( Sample( direction, filter ) );
			
			// Convert the sample to frequency domain.
			fftData->fft( samples.getLast().filter.getScalars() );
		}
	}
	
	//*******************************************************************************
	// Generate integration samples.
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		const Size numSamples = newHRTF.getSampleCount(c);
		ArrayList<Sample>& samples = tempChannels[c];
		
		for ( Index i = 0; i < numIntegrationSamples; i++ )
		{
			// Get a uniformly-distributed random direction.
			Vector3f direction = getRandomDirection( randomVariable );
			
			// Create a new reference-counted filter.
			Filter filter( paddedLength );
			
			// Interpolate the filter for the direction sample.
			interpolateSample( direction, samples.getPointer(), numSamples, filter.getScalars(), paddedLength );
			
			// Add the sample filter.
			samples.add( Sample( direction, filter ) );
		}
	}
	
	//*******************************************************************************
	// For increasing order, determine how well the SH approximation fits the data.
	
	Float lastError = math::max<Float>();
	Bool backtracked = false;
	Size lastCoefficientCount = 0;
	order = 0;
	
	while ( true )
	{
		// Zero-out the accumulator.
		const Size coefficientCount = SH::getCoefficientCount(order);
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			Array<Filter>& hrtf = channels[c].hrtf;
			
			// Increase the HRTF size if necessary.
			if ( hrtf.getSize() < coefficientCount )
			{
				hrtf.setSize( coefficientCount );
				basis.setSize( coefficientCount );
			}
			
			for ( Index i = lastCoefficientCount; i < coefficientCount; i++ )
			{
				if ( hrtf[i].getRowCount() != paddedLength )
				{
					hrtf[i].setSize( paddedLength, 1 );
					om::util::zero( hrtf[i].getScalars(), paddedLength );
				}
			}
		}
		
		//*******************************************************************************
		// Integrate the source HRTF over the SH basis.
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Size numSamples = newHRTF.getSampleCount(c);
			const Size totalNumSamples = numSamples + numIntegrationSamples;
			const Sample* samples = tempChannels[c].getPointer();
			Array<Filter>& hrtf = channels[c].hrtf;
			
			for ( Index i = numSamples; i < totalNumSamples; i++ )
			{
				const Vector3f& direction = samples[i].direction;
				const Filter& filter = samples[i].filter;
				
				SH::cartesian( order, direction, basis.getPointer() );
				
				for ( Index j = lastCoefficientCount; j < coefficientCount; j++ )
				{
					math::multiplyAdd( hrtf[j].getScalars(), filter.getScalars(),
										basis[j], paddedLength );
				}
			}
			
			// Normalize based on the number of samples and sphere surface area.
			const Float normalize = (Float(4)*math::pi<Float>()) / Float(numIntegrationSamples);
			
			for ( Index i = lastCoefficientCount; i < coefficientCount; i++ )
				math::multiply( (Float*)hrtf[i].getScalars(), normalize, paddedLength );
		}
		
		//*******************************************************************************
		// Determine the L2 error over all samples.
		
		Float error = 0.0f;
		Size errorSamples = 0;
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			const Size numSamples = newHRTF.getSampleCount(c);
			const Size totalNumSamples = numSamples + numIntegrationSamples;
			const Sample* samples = tempChannels[c].getPointer();
			Array<Filter>& hrtf = channels[c].hrtf;
			
			for ( Index i = numSamples; i < totalNumSamples; i++ )
			{
				const Vector3f& direction = samples[i].direction;
				const Filter& filter = samples[i].filter;
				
				// Project sample into SH basis.
				SH::cartesian( order, direction, basis.getPointer() );
				
				// Compute the interpolated sample filter.
				math::multiply( tempFilter.getScalars(), hrtf[0].getScalars(), basis[0], paddedLength );
				
				for ( Index j = 1; j < coefficientCount; j++ )
					math::multiplyAdd( tempFilter.getScalars(), hrtf[j].getScalars(), basis[j], paddedLength );
				
				// Accumulate the squared error for the interpolated filter.
				math::subtract( tempFilter.getScalars(), filter.getScalars(), paddedLength );
				error += math::dot( tempFilter.getScalars(), tempFilter.getScalars(), paddedLength );
			}
			
			errorSamples += numIntegrationSamples*paddedLength;
		}
		
		error = math::sqrt(error / Float(errorSamples));
		
		if ( error > lastError && !backtracked )
		{
			// Backtrack to the previous order since the error was less.
			order--;
			maxOrder = order;
			backtracked = true;
			continue;
		}
		
		// Check to see if the error is small enough or if there is convergence.
		if ( error < maxError || (lastError/error - Float(1)) < convergence || order == maxOrder )
			break;
		
		lastError = error;
		order++;
		lastCoefficientCount = coefficientCount;
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFFilter:: getFilter( Index channelIndex, const SHExpansion<Float>& basis, Float* filter ) const
{
	if ( !this->getFilter( channelIndex, basis, (Complex<Float>*)filter ) )
		return false;
	
	// Convert the filter to time domain.
	fftData->ifft( filter );
	
	math::multiply( filter, Float(1) / length, length );
	
	return true;
}




Bool HRTFFilter:: getFilter( Index channelIndex, const SHExpansion<Float>& basis, Complex<Float>* complexFilter ) const
{
	if ( complexFilter == NULL )
		return false;
	
	const Float* coefficients = basis.getCoefficients();
	
	if ( coefficients == NULL || channelIndex >= channels.getSize() )
		return false;
	
	const Filter* filters = channels[channelIndex].hrtf.getPointer();
	
	if ( filters == NULL )
		return false;
	
	// Determine how many filter coefficients to use.
	const Index minOrder = math::min( basis.getOrder(), order );
	const Size coefficientCount = SH::getCoefficientCount( minOrder );
	
	const Size filterSize = length + 2;
	Float* filter = (Float*)complexFilter;
	
	// Compute the dot product of the basis with the HRTF filter for the channel.
	math::multiply( filter, filters[0].getScalars(), coefficients[0], filterSize );
	
	for ( Index i = 1; i < coefficientCount; i++ )
		math::multiplyAdd( filter, filters[i].getScalars(), coefficients[i], filterSize );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Interpolation Method
//############		
//##########################################################################################
//##########################################################################################




Bool HRTFFilter:: interpolateSample( const Vector3f& direction, const Sample* samples, Size numSamples,
									Float* filter, Size filterLength )
{
	if ( numSamples == 0 )
		return false;
	else if ( numSamples == 1 )
	{
		om::util::copy( filter, samples[0].filter.getScalars(), filterLength );
		return true;
	}
	else if ( numSamples == 2 )
	{
		const Float cosTheta0 = math::max( math::dot( direction, samples[0].direction ), Float(0) );
		const Float cosTheta1 = math::max( math::dot( direction, samples[1].direction ), Float(0) );
		const Float cosThetaTotal = cosTheta0 + cosTheta1;
		
		if ( cosThetaTotal < math::epsilon<Float>() )
		{
			math::add( filter, samples[0].filter.getScalars(), samples[1].filter.getScalars(), filterLength );
			math::multiply( filter, Float(0.5), filterLength );
		}
		else
		{
			math::multiply( filter, samples[0].filter.getScalars(), cosTheta0, filterLength );
			math::multiplyAdd( filter, samples[1].filter.getScalars(), cosTheta1, filterLength );
			math::multiply( filter, Float(1)/cosThetaTotal, filterLength );
		}
		
		return true;
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
		
		if ( math::isNAN( bary.x ) || math::isNAN( bary.y ) || math::isNAN( bary.z ) )
		{
			// Interpolate the filter as the weighted sum of the closest samples.
			math::multiply( filter, samples[closest[0]].filter.getScalars(), Float(1)/Float(3), filterLength );
			math::multiplyAdd( filter, samples[closest[1]].filter.getScalars(), Float(1)/Float(3), filterLength );
			math::multiplyAdd( filter, samples[closest[2]].filter.getScalars(), Float(1)/Float(3), filterLength );
		}
		else
		{
			// Interpolate the filter as the weighted sum of the closest samples.
			math::multiply( filter, samples[closest[0]].filter.getScalars(), bary.x, filterLength );
			math::multiplyAdd( filter, samples[closest[1]].filter.getScalars(), bary.y, filterLength );
			math::multiplyAdd( filter, samples[closest[2]].filter.getScalars(), bary.z, filterLength );
		}
		
		return true;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		FFT Data Management Methods
//############		
//##########################################################################################
//##########################################################################################




void HRTFFilter:: initializeFFTData( Size newFilterLength )
{
	if ( fftData )
		util::destruct( fftData );
	
	fftData = util::construct<FFTData>( newFilterLength );
}




void HRTFFilter:: deinitializeFFTData()
{
	if ( fftData )
		util::destruct( fftData );
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
