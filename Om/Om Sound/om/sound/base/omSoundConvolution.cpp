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

#include "omSoundConvolution.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Instance FDL Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class Convolution:: InstanceFDL
{
	public:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// Create a new instance FDL for the specified processing block size.
			OM_INLINE InstanceFDL( Size newBlockSize )
				:	blockSize( newBlockSize ),
					fft( NULL ),
					fftSize( 2*newBlockSize ),
					fftSizeComplex( newBlockSize + 1 ),
					partitionStorage( math::nextMultiple( newBlockSize + 1, Size(2) ) ),
					irOffset( 0 ),
					bufferedSamples( 0 ),
					previousFrames( 0 ),
					nextFDLFactor( 1 ),
					deadline( 0 ),
					inputPartition( 0 ),
					partitionCount( 0 ),
					interpolation(),
					mainIRIndex( 0 ),
					inputIRIndex( 1 ),
					inputIR( false ),
					zeroOutput( true ),
					firstFrame( true )
			{
			}
			
			
		//********************************************************************************
		//******	IR Accessor Methods
			
			
			/// Return the current IR for this partition.
			OM_INLINE BufferComplex32f& getMainIR()
			{
				return irs[mainIRIndex];
			}
			
			
			/// Return the current target IR for this partition.
			OM_INLINE BufferComplex32f& getTargetIR()
			{
				return irs[(inputIRIndex + 2) % 3];
			}
			
			
			/// Return the current input IR for this partition.
			OM_INLINE BufferComplex32f& getInputIR()
			{
				return irs[inputIRIndex];
			}
			
			
			/// Move to the next input IR.
			OM_INLINE void nextInputIR()
			{
				inputIRIndex = (inputIRIndex + 1) % 3;
			}
			
			
			/// Move to the next mainIR.
			OM_INLINE void nextMainIR()
			{
				mainIRIndex = (mainIRIndex + 1) % 3;
			}
			
			
		//********************************************************************************
		//******	Accumulator Accessor Methods
			
			
			/// Return the current main output accumulator for this partition.
			OM_INLINE Buffer32f& getMainOutput()
			{
				return output[0];
			}
			
			
			/// Return the current target output accumulator for this partition.
			OM_INLINE Buffer32f& getTargetOutput()
			{
				return output[1];
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to an object that handles processing FFTs for this FDL.
			FFTReal<Sample32f>* fft;
			
			
			/// The block size of input sound this FDL corresponds to.
			Size blockSize;
			
			
			/// The size of real FFT this FDL corresponds to, twice the block size.
			Size fftSize;
			
			
			/// The size of complex FFT this FDL corresponds to, twice the block size plus one.
			Size fftSizeComplex;
			
			
			/// The number of complex samples stored for each partition for alignment.
			Size partitionStorage;
			
			
			/// The sample offset within the IR where the first partition of this FDL is located.
			Index irOffset;
			
			
			/// The number of input samples that are buffered for this FDL.
			Size bufferedSamples;
			
			
			/// The number of previous frames of this FDL that were processed since the next largest FDL started.
			Size previousFrames;
			
			
			/// The size factor for the next largest FDL.
			Size nextFDLFactor;
			
			
			/// The index of the next deadline that this FDL is targeting.
			Index deadline;
			
			
			/// The index of the least recently used input partition.
			Index inputPartition;
			
			
			/// The number of partitions in this FDL.
			Size partitionCount;
			
			
			/// An array of IRs containing the padded partitions for the FDL impulse response in frequency domain.
			/**
			  * These 3 IRs are rotated between in order to update the IR without any waiting.
			  */
			BufferComplex32f irs[3];
			
			
			/// A history of the frequency-domain input data for all of the partitions with the FDL size.
			BufferComplex32f inputAccumulator;
			
			
			/// The output buffers for the main and target IRs that are interpolated upon read to get the final FDL output.
			Buffer32f output[2];
			
			
			/// An object that keeps track of the state of interpolation to the target IR.
			LerpState<Float32> interpolation;
			
			
			/// A boolean value used to signal when there is a new IR ready to interpolate to.
			Atomic<Index> inputIR;
			
			
			/// A boolean value indicating whether or not the FDL instance has zero output.
			Bool zeroOutput;
			
			
			/// A boolean value that is TRUE when the FDL has not yet processed its first frame.
			Bool firstFrame;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			/// The index of the current main IR for this FDL in the array of IRs.
			Index mainIRIndex;
			
			
			/// The index of the current input IR for this FDL in the array of IRs.
			Index inputIRIndex;
			
			
};





//##########################################################################################
//##########################################################################################
//############		
//############		Instance Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class Convolution:: Instance
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new convolution instance with the default initial state.
			OM_INLINE Instance()
				:	id( INVALID_ID ),
					inputQueuePosition( 0 ),
					outputQueuePosition( 0 ),
					queueSize( 0 ),
					maxIRLength( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The user ID for this instance (its index).
			ConvolutionID id;
			
			
			/// An object that stores the user parameters for this convolution instance.
			Request request;
			
			
			/// A list of the FDLs that are a part of this convolution instance, ordered from smallest to largest.
			ArrayList<InstanceFDL*> fdls;
			
			
			/// A circular queue of input samples that buffers input sound so that power-of-two-sized FFT passes can be made.
			Buffer32f inputQueue;
			
			
			/// A circular queue of output samples that buffers output sound so that power-of-two-sized FFT passes can be made.
			Buffer32f outputQueue;
			
			
			/// The current write position within the input convolution queues.
			Index inputQueuePosition;
			
			
			/// The current write position within the output convolution queues.
			Index outputQueuePosition;
			
			
			/// The size of the convolution input and output queues.
			Index queueSize;
			
			
			/// The current maximum supported IR length in samples for this instance.
			Size maxIRLength;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Convolution:: Convolution()
	:	threadCount( 0 )
{
}




Convolution:: Convolution( const Convolution& other )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Convolution:: ~Convolution()
{
	// Stop processing and clear all convolution instances.
	this->clearInstances();
	
	// Delete the FFT objects.
	for ( Index i = 0; i < ffts.getSize(); i++ )
		util::destruct( ffts[i] );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assingment Operator
//############		
//##########################################################################################
//##########################################################################################




Convolution& Convolution:: operator = ( const Convolution& other )
{
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Thread Count Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: setThreadCount( Size newThreadCount )
{
	threadCount = newThreadCount;
	threadPool.setThreadCount( newThreadCount );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instance Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Convolution::ID Convolution:: newInstance( const Request& request )
{
	// Make a new convolution instance.
	const Index instanceIndex = instances.addNew();
	Instance& instance = instances[instanceIndex];
	instance.id = (ConvolutionID)instanceIndex;
	instance.request = request;
	
	// Determine how many input/output/IR channels there should be.
	instance.request.outputChannelCount = math::max( instance.request.outputChannelCount, Size(1) );
	instance.request.inputChannelCount = (instance.request.inputChannelCount == instance.request.outputChannelCount) ?
											instance.request.outputChannelCount : Size(1);
	instance.request.irChannelCount = (instance.request.irChannelCount == instance.request.outputChannelCount) ?
										instance.request.outputChannelCount : Size(1);
	
	instance.inputQueue.setSize( instance.request.inputChannelCount, 0 );
	instance.outputQueue.setSize( instance.request.outputChannelCount, 0 );
	
	return instanceIndex;
}




Bool Convolution:: deleteInstance( ID instanceID )
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	Instance& instance = instances[instanceID];
	
	// Wait for the instance FDLs to finish processing.
	if ( threadCount != 0 )
	{
		const Size numFDLs = instance.fdls.getSize();
		
		for ( Index i = 0; i < numFDLs; i++ )
		{
			threadPool.finishJob( getFDLJobID( instanceID, instance.fdls[i]->deadline ) );
			util::destruct( instance.fdls[i] );
		}
	}
	
	// Remove the instance.
	instances.remove( instanceID );
	
	return true;
}




void Convolution:: clearInstances()
{
	for ( ObjectPool<Instance>::Iterator i = instances.getIterator(); i; i++ )
	{
		Instance& instance = *i;
		
		// Wait for the instance FDLs to finish processing.
		if ( threadCount != 0 )
		{
			const Size numFDLs = instance.fdls.getSize();
			
			for ( Index i = 0; i < numFDLs; i++ )
			{
				threadPool.finishJob( getFDLJobID( instance.id, instance.fdls[i]->deadline ) );
				util::destruct( instance.fdls[i] );
			}
		}
	}
	
	instances.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instance Request Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Convolution::Request Convolution:: getRequest( ID instanceID ) const
{
	if ( instances.isUnused( instanceID ) )
		return Convolution::Request();
	
	return instances[instanceID].request;
}




Bool Convolution:: getRequest( ID instanceID, Request& request ) const
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	request = instances[instanceID].request;
	
	return true;
}




Bool Convolution:: setRequest( ID instanceID, const Request& request )
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	Instance& instance = instances[instanceID];
	
	// Update the parameters that don't require stopping the convolution pipeline.
	instance.request.sampleRate = math::max( request.sampleRate, SampleRate(0) );
	instance.request.interpolationTime = math::max( request.interpolationTime, Float(0) );
	
	// Check to see if anything changed that would require restarting the pipeline.
	if ( instance.request.inputChannelCount != request.inputChannelCount ||
		instance.request.outputChannelCount != request.outputChannelCount || 
		instance.request.irChannelCount != request.irChannelCount ||
		instance.request.maxLatency != request.maxLatency ||
		instance.request.maxUpdateLatency != request.maxUpdateLatency )
	{
		
		
		
		
		
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instance IR Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Convolution:: getIR( ID instanceID, SoundBuffer& ir ) const
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	
	return false;
}




Bool Convolution:: prepareIR( ID instanceID, const IRRequest& request )
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	prepareIR( instances[instanceID], request );
	
	return true;
}




Bool Convolution:: prepareIR( ID instanceID, const SoundBuffer& ir, const IRRequest& request )
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	prepareIR( instances[instanceID], ir, request );
	
	return true;
}




Bool Convolution:: setIR( ID instanceID, const SoundBuffer& ir, const IRRequest& request )
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	updateIR( instances[instanceID], ir, request );
	
	return true;
}




Bool Convolution:: setIR( ID instanceID, const IRDelegate& delegate, const IRRequest& request )
{
	if ( instances.isUnused( instanceID ) || delegate.readIR.isNull() )
		return false;
	
	updateIR( instances[instanceID], delegate, request );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		IR Prepare Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: prepareIR( Instance& instance, const IRRequest& request )
{
	const Size requestLength = request.length;
	const Index bufferStartOffset = request.start;
	const Index safeIRLength = math::min( math::max( request.start + requestLength, bufferStartOffset ), requestLength ) - bufferStartOffset;
	const Size newIREnd = request.delay + safeIRLength;
	
	//******************************************************************************
	// Enlarge the IR if necessary.
	
	if ( newIREnd > instance.maxIRLength )
		setIRLength( instance, newIREnd );
}




void Convolution:: prepareIR( Instance& instance, const SoundBuffer& ir, const IRRequest& request )
{
	const Size requestLength = request.length == 0 ? ir.getSize() : request.length;
	const Index bufferStartOffset = math::min( request.start, ir.getSize() );
	const Index safeIRLength = math::min( math::max( request.start + requestLength, bufferStartOffset ), requestLength ) - bufferStartOffset;
	const Size newIREnd = request.delay + safeIRLength;
	
	//******************************************************************************
	// Enlarge the IR if necessary.
	
	if ( newIREnd > instance.maxIRLength )
		setIRLength( instance, newIREnd );
}




//##########################################################################################
//##########################################################################################
//############		
//############		IR Update Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: updateIR( Instance& instance, const SoundBuffer& ir, const IRRequest& request )
{
	const Size requestLength = request.length == 0 ? ir.getSize() : math::min( request.length, ir.getSize() );
	const Index bufferStartOffset = math::min( request.start, ir.getSize() );
	const Index safeIRLength = math::min( math::max( request.start + requestLength, bufferStartOffset ), requestLength ) - bufferStartOffset;
	const Size newIRStart = request.delay;
	const Size newIREnd = request.delay + safeIRLength;
	const Size numIRChannels = math::min( instance.request.irChannelCount, ir.getChannelCount() );
	
	//******************************************************************************
	// Enlarge the IR if necessary.
	
	if ( newIREnd > instance.maxIRLength )
		setIRLength( instance, newIREnd );
	
	//******************************************************************************
	// Update the input IR for the FDLs.
	
	const Size maxNumFDLs = instance.fdls.getSize();
	
	for ( Index fdlIndex = 0; fdlIndex < maxNumFDLs; fdlIndex++ )
	{
		InstanceFDL& fdl = *instance.fdls[fdlIndex];
		
		// Skip this FDL if the rendering thread has not consumed the last updated IR.
		if ( fdl.inputIR )
			continue;
		
		// Get the input IR for this FDL.
		BufferComplex32f& inputIR = fdl.getInputIR();
		
		// Get the offset within the original IR of the start of this FDL (not padded).
		const Index fdlOffset = fdl.irOffset;
		const Index fdlEnd = fdlOffset + fdl.partitionCount*fdl.blockSize;
		
		// Compute the FFT magnitude normalization factor.
		const Float fftNormalize = request.gain / Float(fdl.fftSize);
		
		// If the FDL overlaps the IR, then update the IR, otherwise set the FDL to zero.
		if ( fdlEnd > newIRStart && fdlOffset < newIREnd )
		{
			// Make sure the input IR for this FDL is allocated.
			if ( !inputIR.isAllocated() )
				inputIR.allocate();
			
			// Copy the new IR to the input IR, one partition at a time.
			for ( Index j = 0; j < fdl.partitionCount; j++ )
			{
				// Compute the size of the IR data for this partition.
				const Index paddedPartitionOffset = j*fdl.partitionStorage;
				const Index partitionOffset = fdlOffset + j*fdl.blockSize;
				const Index partitionEnd = partitionOffset + fdl.blockSize;
				
				// Update this partition in the IR.
				if ( partitionEnd > newIRStart && partitionOffset < newIREnd )
				{
					const Index partitionStartOffset = partitionOffset < newIRStart ? (newIRStart - partitionOffset) : 0;
					const Index partitionStart = partitionOffset + partitionStartOffset;
					const Size remainingBlockSize = fdl.blockSize - partitionStartOffset;
					const Size partitionLength = math::min( newIREnd - partitionStart, remainingBlockSize );
					
					// Copy the new IR samples to the partition IR.
					for ( Index c = 0; c < numIRChannels; c++ )
					{
						Float32* const irSamples = (Float32*)(inputIR[c] + paddedPartitionOffset);
						Float32* const irSamplesStart = irSamples + partitionStartOffset;
						const Float32* newIRSamples = ir.getChannel(c) + (bufferStartOffset + (partitionStart - request.delay));
						
						// Zero the first of the partition before the IR starts.
						if ( partitionStartOffset != 0 )
							util::zeroPOD( irSamples, partitionStartOffset );
						
						// Multiply by the FFT normalization factor and write into the partition.
						math::multiply( irSamplesStart, newIRSamples, fftNormalize, partitionLength );
						
						// Zero the last half of the FFT padding
						util::zeroPOD( irSamplesStart + partitionLength, fdl.fftSize - (partitionLength + partitionStartOffset) );
						
						// Convert the IR partition to frequency domain.
						fdl.fft->fftUnordered( irSamples );
					}
				}
				else
				{
					// Zero the IR if this partition is past the end of the sampled IR.
					for ( Index c = 0; c < numIRChannels; c++ )
					{
						Complex32f* const irSamples = inputIR[c] + paddedPartitionOffset;
						util::zeroPOD( irSamples, fdl.fftSizeComplex );
					}
				}
			}
		}
		else
		{
			// Deallocate this input IR since it is all zeros.
			if ( inputIR.isAllocated() )
				inputIR.deallocate();
		}
		
		// Atomically increment the number of input IR, signaling to the FDL thread that there is a new IR.
		fdl.inputIR++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		IR Delegate Update Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: updateIR( Instance& instance, const IRDelegate& delegate, const IRRequest& request )
{
	const Size requestLength = request.length;
	const Index bufferStartOffset = math::min( request.start, request.length );
	const Index safeIRLength = math::min( math::max( request.start + requestLength, bufferStartOffset ), requestLength ) - bufferStartOffset;
	const Size newIRStart = request.delay;
	const Size newIREnd = request.delay + safeIRLength;
	const Size numIRChannels = instance.request.irChannelCount;
	
	//******************************************************************************
	// Enlarge the IR if necessary.
	
	if ( newIREnd > instance.maxIRLength )
		setIRLength( instance, newIREnd );
	
	//******************************************************************************
	// Update the input IR for the FDLs.
	
	const Size maxNumFDLs = instance.fdls.getSize();
	
	for ( Index fdlIndex = 0; fdlIndex < maxNumFDLs; fdlIndex++ )
	{
		InstanceFDL& fdl = *instance.fdls[fdlIndex];
		
		// Skip this FDL if the rendering thread has not consumed the last updated IR.
		if ( fdl.inputIR )
			continue;
		
		// Get the input IR for this FDL.
		BufferComplex32f& inputIR = fdl.getInputIR();
		
		// Get the offset within the original IR of the start of this FDL (not padded).
		const Index fdlOffset = fdl.irOffset;
		const Index fdlEnd = fdlOffset + fdl.partitionCount*fdl.blockSize;
		
		// Compute the FFT magnitude normalization factor.
		const Float fftNormalize = request.gain / Float(fdl.fftSize);
		
		// If the FDL overlaps the IR, then update the IR, otherwise set the FDL to zero.
		if ( fdlEnd > newIRStart && fdlOffset < newIREnd )
		{
			// Make sure the input IR for this FDL is allocated.
			if ( !inputIR.isAllocated() )
				inputIR.allocate();
			
			// Copy the new IR to the input IR, one partition at a time.
			for ( Index j = 0; j < fdl.partitionCount; j++ )
			{
				// Compute the size of the IR data for this partition.
				const Index paddedPartitionOffset = j*fdl.partitionStorage;
				const Index partitionOffset = fdlOffset + j*fdl.blockSize;
				const Index partitionEnd = partitionOffset + fdl.blockSize;
				
				// Update this partition in the IR.
				if ( partitionEnd > newIRStart && partitionOffset < newIREnd )
				{
					const Index partitionStartOffset = partitionOffset < newIRStart ? (newIRStart - partitionOffset) : 0;
					const Index partitionStart = partitionOffset + partitionStartOffset;
					const Size remainingBlockSize = fdl.blockSize - partitionStartOffset;
					const Size partitionLength = math::min( newIREnd - partitionStart, remainingBlockSize );
					const Index offset = bufferStartOffset + (partitionStart - request.delay);
					
					// Copy the new IR samples to the partition IR.
					for ( Index c = 0; c < numIRChannels; c++ )
					{
						Float32* const irSamples = (Float32*)(inputIR[c] + paddedPartitionOffset);
						Float32* const irSamplesStart = irSamples + partitionStartOffset;
						
						// Zero the first of the partition before the IR starts.
						if ( partitionStartOffset != 0 )
							util::zeroPOD( irSamples, partitionStartOffset );
						
						// Read the IR samples into the partition storage.
						delegate.readIR( c, offset, partitionLength, irSamplesStart );
						
						// Multiply by the FFT normalization factor.
						math::multiply( irSamplesStart, fftNormalize, partitionLength );
						
						// Zero the last half of the FFT padding
						util::zeroPOD( irSamplesStart + partitionLength, fdl.fftSize - (partitionLength + partitionStartOffset) );
						
						// Convert the IR partition to frequency domain.
						fdl.fft->fftUnordered( irSamples );
					}
				}
				else
				{
					// Zero the IR if this partition is past the end of the sampled IR.
					for ( Index c = 0; c < numIRChannels; c++ )
					{
						Complex32f* const irSamples = inputIR[c] + paddedPartitionOffset;
						util::zeroPOD( irSamples, fdl.fftSizeComplex );
					}
				}
			}
		}
		else
		{
			// Deallocate this input IR since it is all zeros.
			if ( inputIR.isAllocated() )
				inputIR.deallocate();
		}
		
		// Atomically increment the number of input IR, signaling to the FDL thread that there is a new IR.
		fdl.inputIR++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Rendering Methods
//############		
//##########################################################################################
//##########################################################################################





Bool Convolution:: convolve( ID instanceID, const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples )
{
	if ( instances.isUnused( instanceID ) )
		return false;
	
	convolve( instances[instanceID], inputBuffer, outputBuffer, numSamples );
	
	return true;
}




void Convolution:: convolve( Instance& instance, const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples )
{
	// Make sure the buffer is big enough.
	if ( outputBuffer.getChannelCount() < instance.request.outputChannelCount || outputBuffer.getSize() < numSamples )
		outputBuffer.setFormat( instance.request.outputChannelCount, numSamples );
	
	outputBuffer.setSampleRate( instance.request.sampleRate );
	
	// If there is no IR, just zero the output.
	if ( instance.maxIRLength == 0 )
	{
		outputBuffer.zero( 0, numSamples );
		return;
	}
	
	//******************************************************************************
	
	Size samplesProcessed = 0;
	
	while ( samplesProcessed < numSamples )
	{
		// Determine the number of samples to process in this iteration.
		// Make sure to not go past the next deadline.
		const Size numFrameSamples = math::min( numSamples - samplesProcessed,
												instance.fdls[0]->blockSize - instance.fdls[0]->bufferedSamples );
		
		// Write to the convolution input queue for the instance.
		writeInput( instance, inputBuffer, samplesProcessed, numFrameSamples );
		
		//******************************************************************************
		// Determine how many deadlines are due on this frame (possibly none).
		
		Size numDeadlines = 0;
		
		for ( Index i = 0; i < instance.fdls.getSize(); i++ )
		{
			InstanceFDL& fdl = *instance.fdls[i];
			
			// Update the number of buffered samples for this FDL.
			fdl.bufferedSamples += numFrameSamples;
			
			if ( fdl.bufferedSamples  >= fdl.blockSize )
				numDeadlines++;
		}
		
		// Process this FFT frame.
		if ( numDeadlines > 0 )
			processFFTFrame( instance, numDeadlines );
		
		//******************************************************************************
		
		// Read the convolution output queue and write to the output buffer.
		readOutput( instance, outputBuffer, samplesProcessed, numFrameSamples );
		
		// Update how many samples we have processed.
		samplesProcessed += numFrameSamples;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Input and Output Methods
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: writeInput( Instance& instance, const SoundBuffer& inputBuffer,
								Index startOffset, Size numSamples )
{
	const Size numInputChannels = instance.request.inputChannelCount;
	Index inputWrite = instance.inputQueuePosition;
	
	// Copy the input to the internal input queue, possibly in multiple passes.
	for ( Size samplesRemaining = numSamples; samplesRemaining > 0; )
	{
		// Write until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( samplesRemaining, instance.queueSize - inputWrite );
		
		for ( Index c = 0; c < numInputChannels; c++ )
		{
			const Float32* source = inputBuffer.getChannel(c) + startOffset;
			Float32* destination = instance.inputQueue[c] + inputWrite;
			
			// Copy the input for the instance to the input queue.
			util::copyPOD( destination, source, blockSize );
		}
		
		// Advance the input write position.
		inputWrite = (inputWrite + blockSize) % instance.queueSize;
		samplesRemaining -= blockSize;
	}
	
	instance.inputQueuePosition = inputWrite;
}




void Convolution:: readOutput( Instance& instance, SoundBuffer& outputBuffer,
								Index startOffset, Size numSamples )
{
	const Size numOutputChannels = instance.request.outputChannelCount;
	Index outputRead = instance.outputQueuePosition;
	
	// Copy the output from the internal output queue, possibly in multiple passes.
	for ( Size samplesRemaining = numSamples; samplesRemaining > 0; )
	{
		// Read until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( samplesRemaining, instance.queueSize - outputRead );
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			Float32* queue = instance.outputQueue[c] + outputRead;
			Float32* output = outputBuffer.getChannel(c) + startOffset;
			
			// Copy the block from the output queue to the instance output buffer.
			util::copyPOD( output, queue, blockSize );
			
			// Zero the output queue for the next time.
			util::zeroPOD( queue, blockSize );
		}
		
		outputRead = (outputRead + blockSize) % instance.queueSize;
		samplesRemaining -= blockSize;
	}
	
	instance.outputQueuePosition = outputRead;
}




//##########################################################################################
//##########################################################################################
//############		
//############		FFT Processing Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: processFFTFrame( Instance& instance, Size numDeadlines )
{
	// Wait for all of the needed FDLs to finish processing.
	if ( threadCount > 0 )
		threadPool.finishJob( getFDLJobID( instance.id, numDeadlines - 1 ) );
	
	//******************************************************************************
	
	// Read the FDL outputs and accumulate them in the output queue.
	for ( Index i = 0; i < numDeadlines; i++ )
		readFDLOutput( instance, i );
	
	// Copy the next set of input buffers to the FDLs.
	for ( Index i = 0; i < numDeadlines; i++ )
		writeFDLInput( instance, i );
	
	//******************************************************************************
	// Update the rendering state for all deadlines that are due and start processing the new input data.
	
	// Queue the rendering jobs in the order that they should execute. (shortest FDL first)
	for ( Index i = 0; i < numDeadlines; i++ )
	{
		InstanceFDL& fdl = *instance.fdls[i];
		
		// Update the number of previously processed frames in this FDL's long interval.
		fdl.previousFrames = (fdl.previousFrames + 1) % fdl.nextFDLFactor;
		
		// Set the deadline index when this FDL is next due.
		fdl.deadline = getNextDeadlineForFDL( instance, i );
		
		// Update the number of buffered input samples.
		fdl.bufferedSamples -= fdl.blockSize;
		
		// Give smaller FDLs a higher priority since they are due sooner.
		const Float32 priority = 1.0f / Float32(i+1);
		
		if ( threadCount == 0 )
			renderFDL( instance, fdl );
		else
		{
			threadPool.addJob( FunctionCall<void(Instance&, InstanceFDL&)>(
								Convolution::renderFDL, instance, fdl ),
								getFDLJobID( instance.id, fdl.deadline ), priority );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Input Write Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: writeFDLInput( Instance& instance, Index fdlIndex )
{
	InstanceFDL& fdl = *instance.fdls[fdlIndex];
	
	// Update the new input write position.
	fdl.inputPartition = (fdl.inputPartition + 1) % fdl.partitionCount;
	const Index inputPartitionOffset = fdl.inputPartition*fdl.partitionStorage;
	
	// Copy the buffered input to the FDL input buffer.
	const Size numInputChannels = instance.request.inputChannelCount;
	const Size inputLength = fdl.fftSize;
	const Index inputReadPosition = ((instance.inputQueuePosition + instance.queueSize) - 2*fdl.blockSize -
									(fdlIndex != 0 ? instance.fdls[0]->blockSize : 0)) % instance.queueSize;
	
	for ( Index inputIndex = 0, inputRead = inputReadPosition; inputIndex < inputLength; )
	{
		// Write until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( inputLength - inputIndex, instance.queueSize - inputRead );
		
		for ( Index c = 0; c < numInputChannels; c++ )
		{
			Float32* const inputStart = (Float32*)(fdl.inputAccumulator[c] + inputPartitionOffset);
			const Float32* source = instance.inputQueue[c] + inputRead;
			
			util::copyPOD( inputStart + inputIndex, source, blockSize );
		}
		
		inputRead = (inputRead + blockSize) % instance.queueSize;
		inputIndex += blockSize;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Input Write Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: readFDLOutput( Instance& instance, Index fdlIndex )
{
	InstanceFDL& fdl = *instance.fdls[fdlIndex];
	
	if ( fdl.zeroOutput )
		return;
	
	const Size numOutputChannels = instance.request.outputChannelCount;
	const Buffer32f& fdlOutput = fdl.getMainOutput();
	const Size outputLength = fdl.blockSize;
	const Index outputWritePosition = (instance.inputQueuePosition + instance.fdls[0]->blockSize - fdl.blockSize + 
										(fdlIndex != 0 ? fdl.irOffset - fdl.blockSize : 0)) % instance.queueSize;
	
	// Accumulate the FDL output in the output queue, possibly in several steps.
	for ( Index outputIndex = 0, outputWrite = outputWritePosition; outputIndex < outputLength; )
	{
		// Write until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( outputLength - outputIndex, instance.queueSize - outputWrite );
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			// Add to the output queue.
			math::add( instance.outputQueue[c] + outputWrite,
						fdlOutput[c] + outputIndex, blockSize );
		}
		
		outputWrite = (outputWrite + blockSize) % instance.queueSize;
		outputIndex += blockSize;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: renderFDL( Instance& instance, InstanceFDL& fdl )
{
	const Size numInputChannels = instance.request.inputChannelCount;
	const SampleRate sampleRate = instance.request.sampleRate;
	const Float32 frameLength = Float32(fdl.blockSize/sampleRate);
	const Float32 interpolationLength = math::max( Float32(fdl.blockSize/sampleRate), instance.request.interpolationTime );
	
	//******************************************************************************
	// Swap to the next IR if there is an input IR.
	
	// Determine whether or not the IR was just updated.
	if ( fdl.inputIR && fdl.interpolation.isFinished() )
	{
		// Setup the interpolation state. Fade in if there is a new non-zero IR. Otherwise, fade out.
		if ( fdl.getInputIR().isAllocated() )
		{
			fdl.interpolation = LerpState<Float32>( fdl.firstFrame ? 1.0f : 0.0f, 1.0f, interpolationLength );
			fdl.firstFrame = false;
		}
		else
			fdl.interpolation = LerpState<Float32>( 1.0f, 0.0f, interpolationLength );
		
		// Move to the next IR and signal atomically to the update thread that we have consumed the last input IR.
		fdl.nextInputIR();
		fdl.inputIR--;
	}
	
	//******************************************************************************
	// Convert the input audio to frequency domain.
	
	BufferComplex32f& fdlInput = fdl.inputAccumulator;
	const Index inputPartitionOffset = fdl.inputPartition*fdl.partitionStorage;
	
	for ( Index c = 0; c < numInputChannels; c++ )
		fdl.fft->fftUnordered( (Float32*)(fdlInput[c] + inputPartitionOffset) );
	
	//******************************************************************************
	// Convolve the audio for each partition with the IR partition for that partition's delay.
	// Accumulate the results to the FDL output buffer.
	
	BufferComplex32f& mainIR = fdl.getMainIR();
	BufferComplex32f& targetIR = fdl.getTargetIR();
	Buffer32f& mainOutput = fdl.getMainOutput();
	Buffer32f& targetOutput = fdl.getTargetOutput();
	const Bool needsInterpolation = !fdl.interpolation.isFinished();
	const Bool hasMainOutput = mainIR.isAllocated();
	const Bool hasTargetOutput = needsInterpolation && targetIR.isAllocated();
	
	// Make sure the output accumulators are allocated, or deallocate them if the IR is zero.
	if ( mainIR.isAllocated() || (needsInterpolation && targetIR.isAllocated()) )
	{
		targetOutput.allocate();
		mainOutput.allocate();
	}
	else
	{
		targetOutput.deallocate();
		mainOutput.deallocate();
	}
	
	// If the current main IR for this FDL is nonzero, render the convolution.
	if ( mainIR.isAllocated() )
		renderFDLOutput( fdl, fdlInput, mainIR, mainOutput );
	
	//******************************************************************************
	// Interpolate to the target IR if there is one.
	
	if ( needsInterpolation )
	{
		// There is a new IR for this frame, compute the output with both the old and new IRs and interpolate.
		if ( targetIR.isAllocated() )
		{
			// Render the target IR output to a temporary buffer.
			renderFDLOutput( fdl, fdlInput, targetIR, targetOutput );
			
			if ( hasMainOutput )
			{
				// Interpolate between the outputs for the main and target IRs.
				interpolateFDLOutput( fdl, mainOutput, targetOutput, sampleRate );
			}
			else
			{
				// Fade in the target IR's output.
				fadeFDLOutput( fdl, mainOutput, targetOutput, sampleRate );
			}
		}
		else
		{
			// Fade the FDL out since the target IR is zero. Don't do anything if the previous IR is zero.
			if ( hasMainOutput )
				fadeFDLOutput( fdl, mainOutput, mainOutput, sampleRate );
		}
		
		// If the interpolation is finishing on this frame, move to the next main IR.
		if ( fdl.interpolation.time <= frameLength )
			fdl.nextMainIR();
		
		// Update the interpolation time.
		fdl.interpolation.time = math::max( fdl.interpolation.time - frameLength, 0.0f );
	}
	else if ( hasMainOutput )
	{
		// Copy the accumulated output directly to the output buffer with no interpolation.
		copyFDLOutput( fdl, mainOutput );
	}
	
	// Determine whether or not the output of this FDL is zero.
	// This is only when both the main and target outputs are zero.
	fdl.zeroOutput = !hasMainOutput && !hasTargetOutput;
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Output Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: renderFDLOutput( const InstanceFDL& fdl, const BufferComplex32f& input,
									const BufferComplex32f& ir, Buffer32f& output )
{
	const Size partitionCount = fdl.partitionCount;
	const Size numInputChannels = input.getSize(0);
	const Size numOutputChannels = output.getSize(0);
	const Size numIRChannels = ir.getSize(0);
	
	// Zero the output.
	for ( Index c = 0; c < numOutputChannels; c++ )
		util::zeroPOD( (Complex32f*)output[c], fdl.fftSizeComplex );
	
	// Convolve the partitions and accumulate to the output.
	for ( Index p = 0, inputIndex = fdl.inputPartition; p < partitionCount; p++ )
	{
		Index paddedPartitionOffset = p*fdl.partitionStorage;
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			const Complex32f* const fdlStart = c < numInputChannels ? input[c] : input[numInputChannels-1];
			const Complex32f* const inputStart = fdlStart + inputIndex*fdl.partitionStorage;
			const Complex32f* const irStart = c < numIRChannels ? ir[c] : ir[numIRChannels-1];
			const Complex32f* const channelIR = irStart + paddedPartitionOffset;
			Complex32f* const channelOutput = (Complex32f*)output[c];
			/*
			if ( p == 0 )
				math::multiply( channelOutput, inputStart, channelIR, fdl.fftSizeComplex );
			else
				math::multiplyAdd( channelOutput, inputStart, channelIR, fdl.fftSizeComplex );
			*/
			fdl.fft->multiplyAddUnordered( channelOutput, inputStart, channelIR );
		}
		
		// Go to the next input partition for the next ir partition.
		inputIndex = ((inputIndex + partitionCount) - 1) % partitionCount;
	}
	
	//******************************************************************************
	
	// Convert the output to time domain in place.
	for ( Index c = 0; c < numOutputChannels; c++ )
		fdl.fft->ifftUnordered( output[c] );
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Output Interpolation Methods
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: interpolateFDLOutput( InstanceFDL& fdl, Buffer32f& mainOutput, Buffer32f& targetOutput,
										SampleRate sampleRate )
{
	const Size numOutputChannels = mainOutput.getSize(0);
	const Size outputLength = fdl.blockSize;
	LerpState<Float32>& interpolation = fdl.interpolation;
	const Float32 interpolationTime = Float32(outputLength/sampleRate);
	const Float32 interpolationFraction = math::min( interpolationTime / interpolation.time, 1.0f );
	const Float32 stepSize = interpolationFraction*(interpolation.target - interpolation.current) / Float32(outputLength);
	
	Float32 a = interpolation.current;
	
	// Interpolate each channel for the block size.
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		Float32* outputChannel = mainOutput[c];
		const Float32* const outputChannelEnd = outputChannel + outputLength;
		Float32* mainChannel = outputChannel + fdl.blockSize;
		Float32* targetChannel = targetOutput[c] + fdl.blockSize;
		
		a = interpolation.current;
		
		// Interpolate the output accumulators.
		while ( outputChannel != outputChannelEnd )
		{
			*outputChannel = ((*targetChannel) - (*mainChannel))*a + (*mainChannel);
			outputChannel++;
			targetChannel++;
			mainChannel++;
			a += stepSize;
		}
	}
	
	interpolation.current = a;
}




void Convolution:: fadeFDLOutput( InstanceFDL& fdl, Buffer32f& mainOutput, Buffer32f& targetOutput,
								SampleRate sampleRate )
{
	const Size numOutputChannels = mainOutput.getSize(0);
	const Size outputLength = fdl.blockSize;
	LerpState<Float32>& interpolation = fdl.interpolation;
	const Float32 interpolationTime = Float32(outputLength/sampleRate);
	const Float32 interpolationFraction = math::min( interpolationTime / interpolation.time, 1.0f );
	const Float32 stepSize = interpolationFraction*(interpolation.target - interpolation.current) / Float32(outputLength);
	
	Float32 a = interpolation.current;
	
	// Interpolate each channel for the block size.
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		Float32* outputChannel = mainOutput[c];
		const Float32* const outputChannelEnd = outputChannel + outputLength;
		Float32* targetChannel = targetOutput[c] + fdl.blockSize;
		
		a = interpolation.current;
		
		// Interpolate the output accumulators.
		while ( outputChannel != outputChannelEnd )
		{
			*outputChannel = (*targetChannel)*a;
			outputChannel++;
			targetChannel++;
			a += stepSize;
		}
	}
	
	interpolation.current = a;
}




void Convolution:: copyFDLOutput( InstanceFDL& fdl, Buffer32f& output )
{
	const Size numOutputChannels = output.getSize(0);
	
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		Float32* outputChannel = output[c];
		util::copyPOD( outputChannel, outputChannel + fdl.blockSize, fdl.blockSize );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Next Deadline Method
//############		
//##########################################################################################
//##########################################################################################




Index Convolution:: getNextDeadlineForFDL( Instance& instance, Index fdlIndex )
{
	Index maxDeadlineIndex = fdlIndex;
	const Size numFDLsMinusOne = instance.fdls.getSize() - 1;
	
	// Determine the deadline index for when this FDL is next due.
	while ( maxDeadlineIndex < numFDLsMinusOne )
	{
		InstanceFDL& fdl = *instance.fdls[maxDeadlineIndex];
		
		// If the next FDL frame will complete the frame of the next largest FDL, increase the counter.
		if ( fdl.previousFrames + 1 >= fdl.nextFDLFactor )
			maxDeadlineIndex++;
		else
			break;
	}
	
	return maxDeadlineIndex;
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Initialization Method
//############		
//##########################################################################################
//##########################################################################################




void Convolution:: setIRLength( Instance& instance, Size maxIRLength )
{
	const Size numInputChannels = instance.request.inputChannelCount;
	const Size numIRChannels = instance.request.irChannelCount;
	const Size numOutputChannels = instance.request.outputChannelCount;
	
	const Size maxFirstFDLSize = Size((1.0f/2.0f)*instance.request.maxLatency*instance.request.sampleRate);
	const Size firstFDLSize = math::max( DEFAULT_MIN_FDL_SIZE, (math::isPowerOfTwo(maxFirstFDLSize) ?
								maxFirstFDLSize : math::nextPowerOfTwo(maxFirstFDLSize)) / 2 );
	const Size maxFDLSize = Size((1.0f/2.0f)*instance.request.maxUpdateLatency*instance.request.sampleRate);
	
	//******************************************************************************
	// Determine the IR offset and block size of the first new FDL.
	
	Index irOffset;
	Size blockSize;
	
	if ( instance.fdls.getSize() == 0 )
	{
		blockSize = firstFDLSize;
		irOffset = 0;
	}
	else
	{
		const InstanceFDL& lastFDL = *instance.fdls.getLast();
		blockSize = lastFDL.blockSize*DEFAULT_FDL_FACTOR <= maxFDLSize ? lastFDL.blockSize*DEFAULT_FDL_FACTOR : lastFDL.blockSize;
		irOffset = lastFDL.irOffset + lastFDL.partitionCount*lastFDL.blockSize;
	}
	
	//******************************************************************************
	// Create the new FDLs.
	
	
	Index blockSize2 = firstFDLSize;
	Index irOffset2 = 0;
	for ( Index fdlIndex = 0; ; fdlIndex++ )
	{
		// Move to the next FDL.
		irOffset2 += DEFAULT_PARTITIONS_PER_FDL*blockSize2;
		
		if ( irOffset2 >= maxIRLength )
			break;
		
		blockSize2 *= DEFAULT_FDL_FACTOR;
	}
	
	for ( Index fdlIndex = instance.fdls.getSize(); ; fdlIndex++ )
	{
		// Create a new FDL and add it to the instance.
		InstanceFDL* fdl = util::construct<InstanceFDL>( blockSize );
		instance.fdls.add( fdl );
		
		// Initialize the FDL state.
		fdl->irOffset = irOffset;
		fdl->partitionCount = DEFAULT_PARTITIONS_PER_FDL;
		fdl->nextFDLFactor = blockSize*DEFAULT_FDL_FACTOR <= maxFDLSize ? DEFAULT_FDL_FACTOR : Size(1);
		const Size fdlLength = fdl->partitionCount*fdl->partitionStorage;

		// Initialize the IRs for the FDL, but don't allocate them.
		for ( Index i = 0; i < 3; i++ )
			fdl->irs[i].setSize( numIRChannels, fdlLength );
		
		// Initialize the input accumulator.
		fdl->inputAccumulator.allocate( numInputChannels, fdlLength );
		fdl->inputAccumulator.zero();
		
		// Initialize the output buffer to hold a complex frame of output.
		for ( Index i = 0; i < 2; i++ )
			fdl->output[i].allocate( numOutputChannels, 2*fdl->partitionStorage );
		
		// Initialize the FFT for the FDL size if necessary.
		if ( fdlIndex == ffts.getSize() )
			ffts.add( util::construct< FFTReal<Float32> >( fdl->fftSize ) );
		
		fdl->fft = ffts[fdlIndex];
		
		// Move to the next FDL.
		irOffset += fdl->partitionCount*blockSize;
		
		// Stop if the end of the require IR lenth has been reached.
		if ( irOffset >= maxIRLength )
			break;
		
		blockSize *= fdl->nextFDLFactor;
	}
	
	//******************************************************************************
	/// Resize the input and output queues to match the largest FDL size.
	
	const Size newQueueSize = 3*blockSize2;
	//const Size newQueueSize = 3*blockSize;
	
	if ( newQueueSize != instance.queueSize )
	{
		instance.inputQueuePosition = resizeQueue( instance.inputQueue, instance.inputQueuePosition, instance.queueSize, newQueueSize );
		instance.outputQueuePosition = resizeQueue( instance.outputQueue, instance.outputQueuePosition, instance.queueSize, newQueueSize );
		instance.queueSize = newQueueSize;
	}
	
	// Update the maximum supported IR length for the instance.
	instance.maxIRLength = irOffset;
}




Index Convolution:: resizeQueue( Buffer32f& queue, Index position, Size currentSize, Size newSize )
{
	// Unwrap the old queue contents to the new queue buffer size.
	if ( queue.isAllocated() )
		queue.setSizeCopyUnwrap( queue.getSize(0), newSize, 0, position );
	else
		queue.allocate( queue.getSize(0), newSize );
	
	// Zero the uninitialized end of the queue.
	queue.zero( 0, queue.getSize(0), currentSize, newSize - currentSize );
	
	return 0;
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
