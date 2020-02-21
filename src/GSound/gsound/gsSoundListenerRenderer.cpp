/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundListenerRenderer.cpp
 * Contents:    gsound::SoundListenerRenderer class implementation
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


#include "gsSoundListenerRenderer.h"


#include <algorithm>
#include "fftw3.h"


/// A bias applied to input source audio in order to avoid denormal floating point numbers.
/**
  * The inverse of this bias is applied on output to keep the same output level overall.
  */
#define POWER_BIAS 1e6f


using namespace gsound::internal;


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		FDL Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: FDL
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			GSOUND_INLINE FDL( Size numChannels, Size paddedFFTSize )
				:	output( numChannels, paddedFFTSize/2 ),
					outputAccumulatorIndex( 0 ),
					currentAccumulatorPosition( 0 ),
					interpolation( 0, 0, 0 ),
					mainIRIndex( 0 ),
					inputIRIndex( 1 ),
					numInputIRs( 0 ),
					zeroOutput( true )
			{
			}
			
			
		//********************************************************************************
		//******	IR Accessor Methods
			
			
			/// Return the current target IR for this partition.
			GSOUND_INLINE SampleBuffer<ComplexSample>& getMainIR()
			{
				return irs[mainIRIndex];
			}
			
			
			/// Return the current target IR for this partition.
			GSOUND_INLINE SampleBuffer<ComplexSample>& getTargetIR()
			{
				return irs[(inputIRIndex + 2) % 3];
			}
			
			
			/// Return the current input IR for this partition.
			GSOUND_INLINE SampleBuffer<ComplexSample>& getInputIR()
			{
				return irs[inputIRIndex];
			}
			
			
			/// Move to the next input IR.
			GSOUND_INLINE void nextInputIR()
			{
				inputIRIndex = (inputIRIndex + 1) % 3;
			}
			
			
			/// Move to the next mainIR.
			GSOUND_INLINE void nextMainIR()
			{
				mainIRIndex = (mainIRIndex + 1) % 3;
			}
			
			
		//********************************************************************************
		//******	IR Accessor Methods
			
			
			/// Return the current main output accumulator for this partition.
			GSOUND_INLINE SampleBuffer<Float32>& getMainAccumulator()
			{
				return outputAccumulator[outputAccumulatorIndex];
			}
			
			
			/// Return the current target output accumulator for this partition.
			GSOUND_INLINE SampleBuffer<Float32>& getTargetAccumulator()
			{
				return outputAccumulator[(outputAccumulatorIndex + 1) % 2];
			}
			
			
			/// Swap the main and target accumulator so that the target becomes the main.
			GSOUND_INLINE void nextAccumulator()
			{
				outputAccumulatorIndex = (outputAccumulatorIndex + 1) % 2;
			}
			
			
		//********************************************************************************
		//******	Size Accessor Method
			
			
			/// Return the total size in bytes of this FDL's internal storage.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				Size totalSize = sizeof(FDL);
				totalSize += output.getSizeInBytes();
				
				for ( Index i = 0; i < 3; i++ )
					totalSize += irs[i].getSizeInBytes();
				
				for ( Index i = 0; i < 2; i++ )
					totalSize += outputAccumulator[i].getSizeInBytes();
				
				return totalSize;
			}
			
			
		//********************************************************************************
		//******	Deallocate Method
			
			
			/// Release the memory that is used by this FDL.
			GSOUND_INLINE void deallocate()
			{
				output.deallocate();
				
				for ( Index i = 0; i < 2; i++ )
					outputAccumulator[i].deallocate();
				
				for ( Index i = 0; i < 3; i++ )
					irs[i].deallocate();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// An array of IRs containing the padded partitions for the FDL impulse response in frequency domain.
			/**
			  * These 3 IRs are rotated between in order to update the IR without any waiting.
			  */
			SampleBuffer<ComplexSample> irs[3];
			
			/// A temporary buffer used to hold an output frame for the FDL, prior to output accumulation.
			SampleBuffer<Float32> output;
			
			/// The output accumulators for the main and target IRs that are interpolated upon read to get the final FDL output.
			SampleBuffer<Float32> outputAccumulator[2];
			
			/// The current position of this FDL within the output accumulator buffers.
			Index currentAccumulatorPosition;
			
			/// An object that keeps track of the state of interpolation to the target IR.
			LerpState<Float32> interpolation;
			
			/// An array of crossover histories for each channel in this FDL for the last sample in the FDL.
			/**
			  * This corresponds to the history for the last time the FDL partitions were updated.
			  */
			Array<CrossoverType::History,Size,AlignedAllocator<16> > crossoverHistory;
			
			/// The number of input IRs that are now available for this FDL.
			Atomic<Size> numInputIRs;
			
			/// A boolean value that is TRUE if the current output is all zeros (no output).
			/**
			  * This value is set by the FDL rendering thread so that it can signal to
			  * the main thread whether or not its output is all zero.
			  */
			Bool zeroOutput;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The index of the current output accumulator that corresponds to the main filter output.
			Index outputAccumulatorIndex;
			
			/// The index of the current main IR for this FDL in the array of IRs.
			Index mainIRIndex;
			
			/// The index of the current input IR for this FDL in the array of IRs.
			Index inputIRIndex;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Renderer State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: ConvolutionState
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new convolution renderer state.
			GSOUND_INLINE ConvolutionState()
				:	fdls( DEFAULT_MAX_FDL_COUNT ),
					maxIRLengthInSamples( 0 ),
					interpolationTime( 0 ),
					sampleRate( 0 )
			{
			}
			
			
			/// Return the total size in bytes of this convolution state's internal storage.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				Size totalSize = sizeof(ConvolutionState);
				
				for ( Index i = 0; i < fdls.getSize(); i++ )
					totalSize += fdls[i]->getSizeInBytes();
				
				totalSize += inputAccumulator.getSizeInBytes();
				totalSize += inputQueue.getSizeInBytes();
				totalSize += outputQueue.getSizeInBytes();
				
				return totalSize;
			}
			
			
		//********************************************************************************
		//******	Deallocate Method
			
			
			/// Release the memory that is used by this cluster render state.
			GSOUND_INLINE void deallocate()
			{
				inputAccumulator.deallocate();
				inputQueue.deallocate();
				outputQueue.deallocate();
				
				for ( Index i = 0; i < fdls.getSize(); i++ )
					fdls[i]->deallocate();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// An array storing frequency-domain delay lines for the different IR partition sizes.
			ArrayList<FDL*> fdls;
			
			
			/// An array containing the padded buffered input of the frequency-domain delay lines for the whole IR's length.
			SampleBuffer<ComplexSample> inputAccumulator;
			
			
			/// An object that buffers input audio for the convolution filter so that power-of-two-sized FFT passes can be made.
			SampleBuffer<Float32> inputQueue;
			
			
			/// An object that buffers output audio for the convolution filter so that power-of-two-sized FFT passes can be made.
			SampleBuffer<Float32> outputQueue;
			
			
			/// The max allowed IR length in samples.
			Size maxIRLengthInSamples;
			
			
			/// The time in seconds that this convolution state should take to interpolate to the next IR.
			Float interpolationTime;
			
			
			/// The sample rate at which this convolution state is currently processing.
			SampleRate sampleRate;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		FDL State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: FDLState
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			GSOUND_INLINE FDLState()
				:	fftSize( 0 ),
					paddedFFTSize( 0 ),
					paddedFFTStorage( 0 ),
					paddedOffset( 0 ),
					offset( 0 ),
					maxNumPartitions( 0 ),
					numBufferedSamples( 0 ),
					numPreviousFrames( 0 ),
					nextFDLMultiplier( 1 ),
					deadlineIndex( 0 ),
					inputPartitionIndex( 0 ),
					outputAccumulatorSize( 0 ),
					inputReadPosition( 0 ),
					outputWritePosition( 0 ),
					fftPlan( NULL ),
					ifftPlan( NULL )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			GSOUND_INLINE ~FDLState()
			{
				if ( fftPlan )
					fftwf_destroy_plan( fftPlan );
				
				if ( ifftPlan )
					fftwf_destroy_plan( ifftPlan );
			}
			
			
		//********************************************************************************
		//******	FFT Methods
			
			
			GSOUND_FORCE_INLINE void fft( Float32* inout ) const
			{
				fftwf_execute_dft_r2c( fftPlan, (float*)inout, (fftwf_complex*)inout );
			}
			
			
			GSOUND_FORCE_INLINE void ifft( Float32* inout ) const
			{
				fftwf_execute_dft_c2r( ifftPlan, (fftwf_complex*)inout, (float*)inout );
			}
			
			
		//********************************************************************************
		//******	Public Data Members'
			
			
			/// Return the total size in bytes of this FDL state's internal storage.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				return sizeof(FDLState);
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The number of samples in this FDL.
			Size fftSize;
			
			/// The number of padded FFT samples in this FDL.
			Size paddedFFTSize;
			
			/// The number of padded complex FFT samples in this FDL's FFT output.
			Size complexFFTSize;
			
			/// The number of padded complex FFT samples in this FDL's FFT output, needed for alignment.
			Size paddedFFTStorage;
			
			/// The padded offset index within the IR of the first partition of this FDL.
			Index paddedOffset;
			
			/// The offset index within the IR of the first partition of this FDL.
			Index offset;
			
			/// The maximum number of valid partitions in this FDL.
			Size maxNumPartitions;
			
			/// The number of input samples for this FDL.
			Size numBufferedSamples;
			
			/// The number of frames of this FDL that have been processed since the last frame of the next larger FDL.
			Size numPreviousFrames;
			
			/// The size multiplier for the next largest FDL.
			Size nextFDLMultiplier;
			
			/// The index of the next deadline for this FDL.
			Index deadlineIndex;
			
			/// The index of the least recently used partition.
			Index inputPartitionIndex;
			
			/// The number of samples that are in each circular output accumulator.
			Size outputAccumulatorSize;
			
			/// The current read position within the input queue for the input samples.
			Index inputReadPosition;
			
			/// The current write position within the output queue for the output samples.
			Index outputWritePosition;
			
			/// An FFT plan for this FDL size.
			fftwf_plan fftPlan;
			
			/// An inverse FFT plan for this FDL size.
			fftwf_plan ifftPlan;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Path Sort ID Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: PathSortID
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default interpolation state with the specified target gain.
			GSOUND_INLINE PathSortID( const SoundPath* newPath, Float newSortValue )
				:	path( newPath ),
					sortValue( newSortValue )
			{
			}
			
			
			// Return whether or not the sort value for this path is less than another path's sort value.
			GSOUND_FORCE_INLINE Bool operator < ( const PathSortID& other ) const
			{
				return sortValue > other.sortValue;
			}
			
			
			/// A pointer to the path that this path sort ID refers to.
			const SoundPath* path;
			
			/// The value by which this path should be sorted.
			Float sortValue;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Interpolation State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class GSOUND_ALIGN(16) SoundListenerRenderer:: SIMDBandLerpState
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default interpolation state with the specified target gain.
			GSOUND_INLINE SIMDBandLerpState( const SIMDBands& newTargetGain )
				:	currentGain( Float32(0) ),
					targetGain( newTargetGain )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The current gain of the delay tap represented by this interpolation state.
			SIMDBands currentGain;
			
			/// The target gain for the delay tap represented by this interpolation state.
			SIMDBands targetGain;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Path State Class Definition
//############		
//##########################################################################################
//##########################################################################################





class SoundListenerRenderer:: PathState
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			GSOUND_INLINE PathState( Index newIndex )
				:	currentDelayTime( 0 ),
					targetDelayTime( 0 ),
					delayChangePerSecond( 0 ),
					lerpTime( 0 ),
					timeStamp( 0 ),
					index( newIndex )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The current delay time for this propagation path.
			Float currentDelayTime;
			
			/// The target delay time for this propagation path.
			Float targetDelayTime;
			
			/// The current change in delay time for this propagation path, in units of seconds per second.
			Float delayChangePerSecond;
			
			/// The time remaining for the path interpolation.
			Float lerpTime;
			
			/// The index of the last simulation frame when this path was updated.
			Index timeStamp;
			
			/// The index of this path in its path renderer's path state storage.
			Index index;
			
			/// An object that stores boolean flags for this path.
			SoundPathFlags flags;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Path Renderer State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: PathRenderState
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new path renderer state.
			GSOUND_INLINE PathRenderState()
				:	numValidPaths( 0 ),
					lerpState(),
					hasNewPaths( 0 ),
					crossoverHistory( new ( om::util::allocateAligned<CrossoverType::History>( 1, 16 ) )
											CrossoverType::History() ),
					delayBufferSize( 0 ),
					paddedDelayBufferSize( 0 ),
					currentDelayWriteIndex( 0 ),
					timeStamp( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this path state, releasing all internal resources.
			GSOUND_INLINE ~PathRenderState()
			{
				om::util::deallocateAligned( crossoverHistory );
			}
			
			
		//********************************************************************************
		//******	Path Accessor Methods
			
			
			/// Return the number of paths that are currently being rendered.
			GSOUND_INLINE Size getPathCount() const
			{
				return numValidPaths - freePathList.getSize();
			}
			
			
			/// Find the path with the specified hash in the renderer, returning a pointer to it.
			/**
			  * The method allocates a new path state for the path hash if it did not
			  * previously exist in the renderer.
			  */
			GSOUND_INLINE PathState* findPath( SoundPathHash pathHash )
			{
				PathState* pathState;
				
				if ( pathMap.find( pathHash, pathHash, pathState ) )
					return pathState;
				else
					return NULL;
			}
			
			
			/// Create a new path in the renderer for the specified hash.
			GSOUND_INLINE PathState* newPath( SoundPathHash pathHash )
			{
				// Determine the index of the path state.
				Index pathIndex;
				
				// Use a path index from the free list if there is one.
				if ( freePathList.getSize() > 0 )
				{
					pathIndex = freePathList.getLast();
					freePathList.removeLast();
				}
				else
				{
					// Otherwise, use the index past the current last path.
					pathIndex = numValidPaths;
					numValidPaths++;
					
					// Make sure the lerp state is big enough.
					if ( numValidPaths > lerpState.getSampleCount() )
						lerpState.setFormatCopy( lerpState.getChannelCount(), numValidPaths*2 );
				}
				
				// Add a new path state to the renderer.
				return pathMap.add( pathHash, pathHash, PathState( pathIndex ) );
			}
			
			
			/// Remove the path at the specified index in this renderer.
			GSOUND_INLINE void removePath( Index pathIndex )
			{
				if ( pathIndex == numValidPaths - 1 )
					numValidPaths = pathIndex;
				else
				{
					// Add the path to the free list.
					freePathList.add( pathIndex );
				}
			}
			
			
		//********************************************************************************
		//******	Size in Bytes Accessor Method
			
			
			/// Return the approximate size in bytes of the memory allocated by this path state.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				Size totalSize = sizeof(PathRenderState);
				totalSize += pathMap.getSize()*sizeof(PathState);
				totalSize += lerpState.getSizeInBytes();
				totalSize += freePathList.getCapacity()*sizeof(Index);
				totalSize += newPaths.getCapacity()*sizeof(SoundPath);
				totalSize += sizeof(CrossoverType::History);
				totalSize += delayBuffer.getSizeInBytes();
				
				return totalSize;
			}
			
			
		//********************************************************************************
		//******	Deallocate Method
			
			
			/// Release the memory that is used by this path state.
			GSOUND_INLINE void deallocate()
			{
				delayBuffer.deallocate();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A map from sound path hash codes to path indices within this path state.
			HashMap<SoundPathHash,PathState,SoundPathHash> pathMap;
			
			/// A buffer containing the interpolation states for all of the paths in this path renderer.
			SampleBuffer<SIMDBandLerpState> lerpState;
			
			/// A list of the new updated propagation paths that should be rendered using this path render state.
			ArrayList<SoundPath> newPaths;
			
			/// A copy of the listener's orientation matrix so that we can pan the paths without a reference to the listener.
			Matrix3f listenerOrientation;
			
			/// An atomically modified value that is 1 if there are new paths and 0 otherwise.
			Atomic<Size> hasNewPaths;
			
			/// An object that contains a history for this path state's crossover filters.
			CrossoverType::History* crossoverHistory;
			
			/// A buffer used to store the previous band-interleaved audio from the source so that it can be delayed.
			SampleBuffer<Float32> delayBuffer;
			
			/// The size in of the delay buffer in regular (non-interleaved) samples.
			Size delayBufferSize;
			
			/// The size in of the delay buffer in band-interleaved samples.
			Size paddedDelayBufferSize;
			
			/// The current sample index being written to in the delay buffer.
			Index currentDelayWriteIndex;
			
			/// The time stamp when this path renderer state was last updated.
			Index timeStamp;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the path indices within this path state that are not used.
			ArrayList<Index> freePathList;
			
			/// The number of valid paths that are stored in the renderer.
			Size numValidPaths;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster Render State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: ClusteredSourceState
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new cluster source state for the given source and cluster pair.
			GSOUND_INLINE ClusteredSourceState( SourceState* newSource, ClusterState* newCluster )
				:	source( newSource ),
					cluster( newCluster ),
					gain( 0, 0, 0 ),
					timeStamp( 0 )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to the source render state for this clustered source.
			SourceState* source;
			
			/// A pointer to the cluster render state for this clustered source.
			ClusterState* cluster;
			
			/// An object that stores the interpolation state for this clustered source's gain.
			LerpState<Float32> gain;
			
			/// The time stamp when this clustered source state was last updated.
			Index timeStamp;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster Render State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: ClusterState
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new cluster render state.
			GSOUND_INLINE ClusterState()
				:	sourceIR( NULL ),
					gain( 1, 1, 0 ),
					timeStamp( 0 ),
					convolutionStateIndex( 0 )
			{
				reverb.setDryGain( 0 );
				reverb.setWetGainDB( -25 );
				reverb.setHighPassFrequency( 20 );
				reverb.setHighPassOrder( 1 );
				reverb.setLowPassFrequency( 5000 );
				reverb.setLowPassOrder( 1 );
				reverb.setDensity( 0.5 );
			}
			
			
		//********************************************************************************
		//******	Deallocate Method
			
			
			/// Deallocate the big allocated memory buffers used by this cluster render state.
			GSOUND_INLINE void deallocate()
			{
				sources.clear();
				pathRenderer.deallocate();
			}
			
			
		//********************************************************************************
		//******	Size Accessor Method
			
			
			/// Return the approximate number of bytes allocated by this cluster render state.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				return sizeof(ClusterState) + sources.getCapacity()*sizeof(ClusteredSourceState*) +
						pathRenderer.getSizeInBytes() + inputBuffer.getSizeInBytes() + outputBuffer.getSizeInBytes();
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A list of the sound sources which are being rendered using this render state.
			ShortArrayList< Shared<ClusteredSourceState>, 2 > sources;
			
			
			/// An object that stores the information needed for discrete path interpolation rendering.
			PathRenderState pathRenderer;
			
			
			/// The index of the convolution state that renders the main IR of this cluster.
			Index convolutionStateIndex;
			
			
			/// The index of the convolution state that renders the HRTF for this cluster.
			Index hrtfConvolutionIndex;
			
			
			/// A pointer to the sound source IR that is being used as input to this cluster on the current frame.
			const SoundSourceIR* sourceIR;
			
			
			/// A buffer containing the main input audio for this cluster render state.
			SoundBuffer inputBuffer;
			
			
			/// A buffer containing HRTF input audio for this cluster render state.
			SoundBuffer hrtfInputBuffer;
			
			
			/// A buffer containing output audio for this cluster render state.
			SoundBuffer outputBuffer;
			
			
			/// An object used to compute synthetic reverb for the cluster.
			om::sound::ReverbFilter reverb;
			
			
			/// An object that stores the interpolation state for this cluster's gain.
			LerpState<Float32> gain;
			
			
			/// The frame index when the path data for this cluster was last updated.
			Index timeStamp;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Source Render State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: SourceState
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new cluster render state.
			GSOUND_INLINE SourceState()
				:	source( NULL ),
					resampler( NULL ),
					updateTimeStamp( 0 )
			{
			}
			
			
			/// Create a copy of the specified cluster render state object.
			GSOUND_INLINE SourceState( const SourceState& other )
				:	source( other.source ),
					resampler( NULL ),
					updateTimeStamp( other.updateTimeStamp )
			{
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Create a new cluster render state.
			GSOUND_INLINE ~SourceState()
			{
				if ( resampler != NULL )
					util::destruct( resampler );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another cluster render state to this one.
			GSOUND_INLINE SourceState& operator = ( const SourceState& other )
			{
				if ( this != &other )
				{
					source = other.source;
					clusters = other.clusters;
					updateTimeStamp = other.updateTimeStamp;
					
					if ( resampler != NULL )
						resampler->reset();
				}
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Public Helper Methods
			
			
			/// Update the information for this source's previous membership to the given cluster.
			/**
			  * The method returns whether or not the source was previously a member of
			  * that cluster.
			  */
			GSOUND_INLINE Bool updateCluster( const ClusterState* clusterState, Index newTimeStamp )
			{
				const Size numClusters = clusters.getSize();
				
				for ( Index i = 0; i < numClusters; i++ )
				{
					if ( clusters[i]->cluster == clusterState )
					{
						clusters[i]->timeStamp = newTimeStamp;
						return true;
					}
				}
				
				return false;
			}
			
			
			/// Remove this source's previous membership to the given cluster.
			/**
			  * The method returns whether or not the source was previously a member of
			  * that cluster.
			  */
			GSOUND_INLINE Bool removeCluster( const ClusterState* clusterState )
			{
				const Size numClusters = clusters.getSize();
				
				for ( Index i = 0; i < numClusters; i++ )
				{
					if ( clusters[i]->cluster == clusterState )
					{
						clusters.removeAtIndexUnordered( i );
						return true;
					}
				}
				
				return false;
			}
			
			
		//********************************************************************************
		//******	Size in Bytes Accessor Method
			
			
			/// Return the approximate number of bytes allocated by this source render state.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				Size totalSize = sizeof(SourceState);
				totalSize += inputBuffer.getSizeInBytes();
				
				if ( resampler )
					totalSize += sizeof(Resampler);
				
				return totalSize;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The source associated with this source render state.
			const SoundSource* source;
			
			
			/// An object which handles sample rate conversion for this sound source's input audio.
			Resampler* resampler;
			
			
			/// A list of the clusters which use this sound source.
			ShortArrayList< Shared<ClusteredSourceState>, 2 > clusters;
			
			
			/// A buffer containing the final input audio for this source.
			SoundBuffer inputBuffer;
			
			
			/// The frame index when the path data for this source was last updated.
			Index updateTimeStamp;
			
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Update Thread State Class Definition
//############		
//##########################################################################################
//##########################################################################################




class SoundListenerRenderer:: UpdateThreadState
{
	public:
		
		/// Return the approximate size in bytes of the memory allocated by this update thread state.
		GSOUND_INLINE Size getSizeInBytes() const
		{
			Size totalSize = panBuffer.getSizeInBytes();
			totalSize += noiseBuffer.getSizeInBytes();
			totalSize += interleavedPartition.getSizeInBytes();
			totalSize += pathSortIDs.getCapacity()*sizeof(PathSortID);
			totalSize += channelGains.getSize()*sizeof(Gain);
			totalSize += shBasis.getCoefficientCount()*sizeof(Float32);
			
			return totalSize;
		}
		
		
		/// A list of objects that are used to sort propagation paths.
		ArrayList<PathSortID> pathSortIDs;
		
		/// A temporary array of channel gain coefficients used in impulse panning.
		Array<Gain> channelGains;
		
		/// A temporary impulse response partition that stores noise used in generating the IR.
		SampleBuffer<Float32> noiseBuffer;
		
		/// A temporary impulse response partition that stores the panning of impulses across the output channels.
		SampleBuffer<Float32> panBuffer;
		
		/// A temporary impulse response partition that is used when building a band-interleaved IR.
		SampleBuffer<Float32> interleavedPartition;
		
		/// A temporary spherical harmonic basis for a single 3D direction vector.
		SHExpansion<Float32> shBasis;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundListenerRenderer:: SoundListenerRenderer()
	:	request(),
		timeStamp( 0 ),
		processingLoad( 0 ),
		maxFDLCount( DEFAULT_MAX_FDL_COUNT ),
		minFDLSize( DEFAULT_MIN_FDL_SIZE ),
		maxFDLSize( DEFAULT_MAX_FDL_SIZE ),
		partitionsPerFDL( DEFAULT_PARTITIONS_PER_FDL ),
		convolutionQueuePosition( 0 ),
		convolutionQueueSize( 0 )
{
	// Initialize the crossover.
	crossover.setBands( request.frequencies, request.sampleRate );
	
	initializeFDLs( Size(request.maxIRLength*request.sampleRate) );
	
	updateThreadPool.setPriority( ThreadPriority::LOW );
	//renderThreadPool.setPriority( ThreadPriority::HIGH );
}




SoundListenerRenderer:: SoundListenerRenderer( const RenderRequest& newRequest )
	:	request(),
		timeStamp( 0 ),
		processingLoad( 0 ),
		maxFDLCount( DEFAULT_MAX_FDL_COUNT ),
		minFDLSize( DEFAULT_MIN_FDL_SIZE ),
		maxFDLSize( DEFAULT_MAX_FDL_SIZE ),
		partitionsPerFDL( DEFAULT_PARTITIONS_PER_FDL ),
		convolutionQueuePosition( 0 ),
		convolutionQueueSize( 0 )
{
	updateRequest( newRequest );
	
	initializeFDLs( Size(request.maxIRLength*request.sampleRate) );
	
	updateThreadPool.setPriority( ThreadPriority::LOW );
	//renderThreadPool.setPriority( ThreadPriority::HIGH );
}



/*
SoundListenerRenderer:: SoundListenerRenderer( const SoundListenerRenderer& other )
	:	request( other.request ),
		crossover( other.crossover ),
		sourceStates( other.sourceStates.getSize() ),
		clusterStates( other.clusterStates.getSize() ),
		timeStamp( other.timeStamp ),
		listenerGain( other.listenerGain ),
		numUnusedClusterStates( other.numUnusedClusterStates ),
		processingLoad( other.processingLoad ),
		maxIRLengthInSamples( other.maxIRLengthInSamples ),
		maxFDLCount( other.maxFDLCount ),
		minFDLSize( other.minFDLSize ),
		maxFDLSize( other.maxFDLSize ),
		partitionsPerFDL( other.partitionsPerFDL ),
		convolutionQueueSize( other.convolutionQueueSize ),
		convolutionInputWritePosition( other.convolutionInputWritePosition ),
		convolutionOutputReadPosition( other.convolutionOutputReadPosition )
{
	// Rebuild the renderer data.
	for ( Index i = 0; i < other.sourceStates.getSize(); i++ )
	{
		const SoundSource* source = other.sourceStates[i]->source;
		
		if ( source == NULL )
			continue;
		
		// Copy the renderers;
		sourceStates.add( util::construct<SourceState>( *other.sourceStates[i] ) );
		sourceStateMap.add( source->getHashCode(), source, sourceStates.getLast() );
	}
	
	// Rebuild the renderer data.
	for ( Index i = 0; i < other.clusterStates.getSize(); i++ )
	{
		// Copy the renderers;
		clusterStates.add( util::construct<ClusterState>( *other.clusterStates[i] ) );
	}
	
	// Initialize the FDLs
	initializeFDLs( Size(request.maxIRLength*request.sampleRate), maxIRLengthInSamples );
	request.maxIRLength = Float( maxIRLengthInSamples / request.sampleRate );
	
	updateThreadPool.setPriority( ThreadPriority::LOW );
}
*/



//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundListenerRenderer:: ~SoundListenerRenderer()
{
	renderingMutex.lock();

	// Wait on the update threads to finish processing.
	updateThreadPool.finishJobs();

	// Wait on the render threads to finish processing.
	renderThreadPool.finishJobs();

	// Wait for the threads to finish and destroy the FDLs.
	for ( Index i = 0; i < fdls.getSize(); i++ )
		util::destruct( fdls[i] );
	
	fdls.clear();
	sourceStateMap.clear();
	sourceStates.clear();
	clusterStates.clear();
	convolutionStates.clear();
	renderStates.clear();
	updateStates.clear();

	renderingMutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################



/*
SoundListenerRenderer& SoundListenerRenderer:: operator = ( const SoundListenerRenderer& other )
{
	if ( this != &other )
	{
		sourceStateMap.clear();
		sourceStates.clear();
		clusterStates.clear();
		
		request = other.request;
		crossover = other.crossover;
		timeStamp = other.timeStamp;
		listenerGain = other.listenerGain;
		numUnusedSourceStates = other.numUnusedSourceStates;
		numUnusedClusterStates = other.numUnusedClusterStates;
		processingLoad = other.processingLoad;
		maxIRLengthInSamples = other.maxIRLengthInSamples;
		maxFDLCount = other.maxFDLCount;
		minFDLSize = other.minFDLSize;
		maxFDLSize = other.maxFDLSize;
		partitionsPerFDL = other.partitionsPerFDL;
		
		// Rebuild the renderer data.
		for ( Index i = 0; i < other.sourceStates.getSize(); i++ )
		{
			const SoundSource* source = other.sourceStates[i]->source;
			
			if ( source == NULL )
				continue;
			
			// Copy the renderers;
			sourceStates.add( util::construct<SourceState>( *other.sourceStates[i] ) );
			sourceStateMap.add( source->getHashCode(), source, sourceStates.getLast() );
		}
		
		// Rebuild the renderer data.
		for ( Index i = 0; i < other.clusterStates.getSize(); i++ )
		{
			// Copy the renderers;
			clusterStates.add( util::construct<ClusterState>( *other.clusterStates[i] ) );
		}
	}
	
	return *this;
}*/




//##########################################################################################
//##########################################################################################
//############		
//############		Size in Bytes Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundListenerRenderer:: getSizeInBytes() const
{
	renderingMutex.lock();
	
	Size totalSize = this->getSizeInBytesInternal();
	
	renderingMutex.unlock();
	
	return totalSize;
}




Size SoundListenerRenderer:: getSizeInBytesInternal() const
{
	Size totalSize = sizeof(SoundListenerRenderer);
	totalSize += fdls.getCapacity()*sizeof(FDLState*);
	totalSize += sourceStates.getSizeInBytes();
	totalSize += clusterStates.getSizeInBytes();
	totalSize += convolutionStates.getSizeInBytes();
	const Size numFDLs = fdls.getSize();
	const Size numSourceStates = sourceStates.getSize();
	const Size numClusterStates = clusterStates.getSize();
	const Size numConvolutionStates = convolutionStates.getSize();
	const Size numUpdateStates = updateStates.getSize();
	
	for ( Index i = 0; i < numFDLs; i++ )
		totalSize += fdls[i]->getSizeInBytes();
	
	for ( Index i = 0; i < numSourceStates; i++ )
		totalSize += sourceStates[i]->getSizeInBytes();
	
	for ( Index i = 0; i < numClusterStates; i++ )
		totalSize += clusterStates[i]->getSizeInBytes();
	
	for ( Index i = 0; i < numConvolutionStates; i++ )
		totalSize += convolutionStates[i]->getSizeInBytes();
	
	for ( Index i = 0; i < numUpdateStates; i++ )
		totalSize += updateStates[i].getSizeInBytes();
	
	return totalSize;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Request Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: setRequest( const RenderRequest& newRequest )
{
	// Lock a mutex to exclude the rendering thread from access.
	renderingMutex.lock();
	
	updateRequest( newRequest );
	
	renderingMutex.unlock();
}




void SoundListenerRenderer:: updateRequest( const RenderRequest& newRequest )
{
	request.flags = newRequest.flags;
	request.numThreads = math::max( newRequest.numThreads, Size(1) );
	request.numUpdateThreads = math::max( newRequest.numUpdateThreads, Size(1) );
	request.maxSourcePathCount = newRequest.maxSourcePathCount;
	request.maxPathDelay = math::clamp( newRequest.maxPathDelay, Float(0), newRequest.maxIRLength );
	request.maxDelayRate = math::max( newRequest.maxDelayRate, Float(0) );
	request.irFadeTime = math::max( newRequest.irFadeTime, Float(0) );
	request.pathFadeTime = math::max( newRequest.pathFadeTime, Float(0) );
	request.sourceFadeTime = math::max( newRequest.sourceFadeTime, Float(0) );
	request.clusterFadeInTime = math::max( newRequest.clusterFadeInTime, Float(0) );
	request.clusterFadeOutTime = math::max( newRequest.clusterFadeOutTime, Float(0) );
	request.volume = math::max( newRequest.volume, Float(0) );
	request.maxHRTFOrder = math::clamp( newRequest.maxHRTFOrder, Size(0), Size(9) );
	
	//******************************************************************************
	// Make sure the rendering thread pool has the correct number of threads.
	
	if ( renderThreadPool.getThreadCount() != request.numThreads )
		renderThreadPool.setThreadCount( request.numThreads );
	
	for ( Index i = renderStates.getSize(); i < request.numThreads; i++ )
		renderStates.add( RenderThreadState() );
	
	//***********************************************************************
	
	// Update the rendering format if necessary.
	if ( timeStamp == 0 )
	if ( (request.channelLayout != newRequest.channelLayout) ||
		(request.sampleRate != newRequest.sampleRate) ||
		(request.maxLatency != newRequest.maxLatency) )
	{
		request.channelLayout = newRequest.channelLayout;
		request.sampleRate = newRequest.sampleRate > SampleRate(0) ? newRequest.sampleRate : SampleRate(44100);
		request.maxLatency = newRequest.maxLatency;
	}
	
	//***********************************************************************
	// Update the crossover frequencies.
	
	request.frequencies = newRequest.frequencies;
	
	// Check to see if the frequency bands have changed.
	if ( request.frequencies != crossover.getBands() || timeStamp == 0 )
	{
		// Update the crossover's bands.
		crossover.setBands( request.frequencies, request.sampleRate );
	}
	
	//***********************************************************************
	// Update the HRTF.
	
	if ( request.flags.isSet( RenderFlags::HRTF ) && request.hrtf != newRequest.hrtf )
	{
		request.hrtf = newRequest.hrtf;
		
		if ( newRequest.hrtf )
			hrtf.setHRTF( *newRequest.hrtf, request.sampleRate, request.maxHRTFOrder );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		IR Update Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundListenerRenderer:: updateIR( const SoundListenerIR& listenerIR, const RenderRequest& newRequest )
{
	// Lock a mutex to exclude the rendering thread from access while we update the source clusters.
	renderingMutex.lock();
	
	// Update the global timestamp for this listener renderer.
	timeStamp++;
	
	//***********************************************************************
	
	// Update the rendering parameters from the request's data.
	updateRequest( newRequest );
	
	// Update the listener sensitivity.
	const SoundListener& listener = *listenerIR.getListener();
	const Real listenerPowerDB = listener.getSensitivity() + Real(10)*math::log10( Real(4)*gsound::math::pi<Real>() );
	const Real targetListenerGain = request.volume * math::pow( Real(10), listenerPowerDB / Real(10) ) / POWER_BIAS;
	
	if ( targetListenerGain != listenerGain.target )
	{
		listenerGain.target = targetListenerGain;
		listenerGain.time = request.pathFadeTime;
	}
	
	// Make sure the update thread pool is initialized and has the correct number of threads.
	if ( updateThreadPool.getThreadCount() != request.numUpdateThreads )
		updateThreadPool.setThreadCount( request.numUpdateThreads );
	
	// Make sure there are enough thread states for all of the update threads.
	for ( Index i = updateStates.getSize(); i < request.numUpdateThreads; i++ )
		updateStates.add( UpdateThreadState() );
	
	//***********************************************************************
	// Update the cluster and source rendering states.
	
	const Size numSourceIRs = listenerIR.getSourceCount();
	Size totalRenderedPathCount = 0;
	
	for ( Index i = 0; i < numSourceIRs; i++ )
	{
		const SoundSourceIR& sourceIR = listenerIR.getSourceIR(i);
		const Size numIRSources = sourceIR.getSourceCount();
		
		// Ignore any IRs that don't correspond to any sound sources or paths.
		if ( numIRSources == 0 )
			continue;
		
		//***********************************************************************
		// Determine the cluster rendering state corresponding to this IR.
		
		ClusterState* clusterState = NULL;
		
		for ( Index s = 0; s < numIRSources; s++ )
		{
			const SoundSource* source = sourceIR.getSource(s);
			
			if ( source == NULL )
				continue;
			
			const Index* sourceStateIndex;
			
			if ( sourceStateMap.find( source->getHashCode(), source, sourceStateIndex ) )
			{
				SourceState* sourceState = sourceStates[*sourceStateIndex];
				const Size numClusters = sourceState->clusters.getSize();
				
				if ( numClusters > Size(0) )
				{
					// Found a cluster state for this IR, finish and break from the search loop.
					clusterState = sourceState->clusters[0]->cluster;
					break;
				}
			}
		}
		
		// If there was no previously existing cluster state for this IR, create a new one.
		if ( clusterState == NULL )
			clusterState = createCluster();
		else if ( clusterState->timeStamp == timeStamp )
			continue;
		
		// Update the timestamp for the cluster state.
		clusterState->timeStamp = timeStamp;
		clusterState->sourceIR = &sourceIR;
		
		totalRenderedPathCount += clusterState->pathRenderer.getPathCount();
		
		//***********************************************************************
		// Update the source rendering states for all sources for this IR.
		
		for ( Index s = 0; s < numIRSources; s++ )
		{
			const SoundSource* source = sourceIR.getSource(s);
			
			if ( source == NULL )
				continue;
			
			// Find or create a source render state for this source.
			Index* sourceStateIndex;
			SourceState* sourceState;
			
			if ( sourceStateMap.find( source->getHashCode(), source, sourceStateIndex ) )
				sourceState = sourceStates[*sourceStateIndex];
			else
			{
				Index newSourceIndex = sourceStates.construct();
				sourceState = sourceStates[newSourceIndex];
				
				// Update the source state with the new information.
				sourceState->source = source;
				
				// Add this new state to the map of sources.
				sourceStateMap.add( source->getHashCode(), source, newSourceIndex );
			}
			
			// Make sure the source and cluster know they are linked.
			if ( !sourceState->updateCluster( clusterState, timeStamp ) )
			{
				// Source was not previously a member of this cluster.
				// Create a new clustered source to track that membership.
				Shared<ClusteredSourceState> clusteredSource = Shared<ClusteredSourceState>::construct( sourceState, clusterState );
				
				// Set the fade in parameters.
				clusteredSource->gain.current = Gain(0);
				clusteredSource->gain.target = Gain(1);
				clusteredSource->gain.time = request.sourceFadeTime;
				clusteredSource->timeStamp = timeStamp;
				
				// Add the clustered source to the cluster and source states.
				sourceState->clusters.add( clusteredSource );
				clusterState->sources.add( clusteredSource );
			}
			
			// Update the time stamp for the source state.
			sourceState->updateTimeStamp = timeStamp;
		}
	}
	
	//***********************************************************************
	// Report rendering analytic information.
	
	if ( newRequest.statistics != NULL && newRequest.flags.isSet( RenderFlags::STATISTICS ) )
	{
		newRequest.statistics->renderingLoad = processingLoad;
		newRequest.statistics->renderingLatency = fdls.getSize() > 0 ? Float(3*minFDLSize / request.sampleRate) : 0.0f;
		newRequest.statistics->renderedPathCount = totalRenderedPathCount;
		
		// Compute the size in bytes of this renderer.
		newRequest.statistics->renderingMemory += this->getSizeInBytesInternal();
	}
	
	//***********************************************************************
	
	renderingMutex.unlock();
	
	//***********************************************************************
	// Update the cluster and source IRs asynchronously in parallel.
	
	const Size numClusterStates = clusterStates.getSize();
	
	for ( Index i = 0; i < numClusterStates; i++ )
	{
		ClusterState& clusterState = *clusterStates[i];
		
		if ( clusterStates.isUnused(i) || clusterState.sourceIR == NULL )
			continue;
		
		// Add the update job for this cluster to the thread pool.
		updateThreadPool.addJob( FunctionCall<void(ClusterState&, const SoundSourceIR&, const SoundListener&, const FrequencyBands&)>(
											bind( &SoundListenerRenderer::updateClusterIR, this ),
											clusterState, *clusterState.sourceIR, listener, listenerIR.getFrequencies() ) );
	}
	
	updateThreadPool.finishJobs();
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster IR Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: updateClusterIR( ClusterState& clusterState, const SoundSourceIR& ir,
											const SoundListener& listener, const FrequencyBands& frequencies )
{
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	
	//***********************************************************************
	// Get the update thread data for this thread.
	
	UpdateThreadState* threadState = &updateStates[updateThreadPool.getCurrentThreadIndex()];
	
	// Clear the list of doppler shifted paths.
	threadState->pathSortIDs.clear();
	
	const Size biggestFDLSize = fdls.getLast()->fftSize;
	const Size interleavedPartitionSize = biggestFDLSize*request.frequencies.getBandCount();
	
	// Make sure the interleaved IR partition has the correct size.
	if ( threadState->interleavedPartition.getChannelCount() != numOutputChannels ||
		threadState->interleavedPartition.getSampleCount() != interleavedPartitionSize )
	{
		threadState->interleavedPartition.setFormat( numOutputChannels, interleavedPartitionSize );
	}
	
	// Make sure the interleaved IR partition is allocated.
	if ( !threadState->interleavedPartition.isAllocated() )
		threadState->interleavedPartition.allocate();
	
	// Make sure the pan buffer has the correct size.
	if ( threadState->panBuffer.getChannelCount() != numOutputChannels ||
		threadState->panBuffer.getSampleCount() != biggestFDLSize )
	{
		threadState->panBuffer.setFormat( numOutputChannels, biggestFDLSize );
	}
	
	// Make sure the pan buffer is allocated.
	if ( !threadState->panBuffer.isAllocated() )
		threadState->panBuffer.allocate();
	
	// Make sure the noise buffer has the correct size.
	Bool formatChanged = false;
	const Size maxIRLengthInSamples = fdls.getLast()->offset + fdls.getLast()->maxNumPartitions*fdls.getLast()->fftSize;
	const Size maxIRLengthInBandSamples = request.frequencies.getBandCount() * maxIRLengthInSamples;
	
	if ( threadState->noiseBuffer.getChannelCount() != 1 ||
		threadState->noiseBuffer.getSampleCount() < maxIRLengthInBandSamples )
	{
		threadState->noiseBuffer.setFormat( 1, maxIRLengthInBandSamples );
		formatChanged = true;
	}
	
	// Make sure the pan buffer is allocated.
	if ( !threadState->noiseBuffer.isAllocated() || formatChanged )
	{
		threadState->noiseBuffer.allocate();
		SIMDBands* noise = (SIMDBands*)threadState->noiseBuffer.getChannel(0);
		
		// Generate the noise.
		for ( Index i = 0; i < maxIRLengthInSamples; i++ )
			noise[i] = /*2.0f*(Float)math::random( 0, 1 ) - 1.0f;*/math::random( -1.0f, 1.0f );
		
		// Filter the noise.
		CrossoverType::History crossoverHistory;
		crossover.filterSIMD( crossoverHistory, (Float32*)noise, (Float32*)noise, maxIRLengthInSamples );
	}
	
	//***********************************************************************
	// Update the reverb information.
	
	clusterState.reverb.setDecayTime( ir.getReverbTime() );
	
	//***********************************************************************
	// Update the path renderer with the new paths.
	
	PathRenderState& pathRenderer = clusterState.pathRenderer;
	
	const Bool pathRenderingEnabled = request.flags.isSet( RenderFlags::DISCRETE_PATHS );
	const Size maxNumPaths = pathRenderingEnabled ? request.maxSourcePathCount : 0;
	const Size numPaths = ir.getPathCount();
	
	// Sort the paths by decreasing intensity if there are too many.
	if ( numPaths > maxNumPaths )
	{
		// Add the paths to a list to be sorted by decreasing intensity.
		for ( Index i = 0; i < numPaths; i++ )
		{
			const SoundPath& path = ir.getPath(i);
			Float pathSortValue = path.getIntensity().getMax();
			
			// Give paths with long delay times a small sort value.
			if ( path.getDelay() > request.maxPathDelay )
				pathSortValue = 0;
			
			threadState->pathSortIDs.add( PathSortID( &path, pathSortValue ) );
		}
		
		// Update the path renderer asychronously if the rendering thread has consumed all previous paths.
		if ( !pathRenderer.hasNewPaths )
		{
			// Sort the paths by decreasing intensity.
			sortPathsDecreasing( threadState->pathSortIDs );
			
			// Determine the number of new paths.
			Size numNewPaths = math::min( threadState->pathSortIDs.getSize(), maxNumPaths );
			
			// Add the maximum number of new paths to the render state.
			for ( Index i = 0; i < numNewPaths; i++ )
				pathRenderer.newPaths.add( *threadState->pathSortIDs[i].path );
			
			// Store the listener's orientation for later.
			pathRenderer.listenerOrientation = listener.getOrientation();
			
			// Atomically increment the new path indicator, signaling that there are new paths to the renderer.
			pathRenderer.hasNewPaths++;
		}
	}
	else
	{
		// Update the path renderer asychronously if the rendering thread has consumed all previous paths.
		if ( !pathRenderer.hasNewPaths )
		{
			// Add the paths directly to the list of new paths for the path render state.
			pathRenderer.newPaths.addAll( ir.getPathPointer(), numPaths );
			
			// Store the listener's orientation for later.
			pathRenderer.listenerOrientation = listener.getOrientation();
			
			// Atomically increment the new path indicator, signaling that there are new paths to the renderer.
			pathRenderer.hasNewPaths++;
		}
	}
	
	//***********************************************************************
	// Update the HRTF for the cluster if enabled.
	
	if ( request.flags.isSet( RenderFlags::HRTF ) )
	{
		ConvolutionState& hrtfState = *convolutionStates[clusterState.hrtfConvolutionIndex];
		
		updateHRTF( hrtfState, ir, listener, *threadState );
	}
	
	//***********************************************************************
	// Update the convolution renderer with the new IR.
	
	ConvolutionState& convolutionState = *convolutionStates[clusterState.convolutionStateIndex];
	
	updateConvolutionIR( convolutionState, ir, listener, frequencies, *threadState );
}




//##########################################################################################
//##########################################################################################
//############		
//############		HRTF IR Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: updateHRTF( ConvolutionState& convolutionState, const SoundSourceIR& sourceIR,
										const SoundListener& listener, UpdateThreadState& threadState )
{
	const Size numOutputChannels = hrtf.getChannelCount();
	
	//******************************************************************************
	// Compute an orthonormal spherical harmonic basis of the direct sound that is arriving.
	
	const Size numPaths = sourceIR.getPathCount();
	const Index hrtfOrder = hrtf.getSHOrder();
	Size numDirectionSamples = 0;
	
	// Zero the spherical harmonic basis.
	threadState.shBasis.zero();
	
	for ( Index i = 0; i < numPaths; i++ )
	{
		const SoundPath& path = sourceIR.getPath(i);
		
		if ( path.getFlags().isSet( SoundPathFlags::DIRECT ) )
		{
			SH::cartesian( hrtfOrder, path.getDirection()*listener.getOrientation(), threadState.shBasis );
			numDirectionSamples++;
			break;
		}
	}
	
	//******************************************************************************
	// Determine the HRTF filter based on the spherical harmonic basis.
	
	Size hrtfLength = 0;
	
	if ( numDirectionSamples > 0 )
	{
		hrtfLength = hrtf.getFilterLength();
		
		// Compute the filter for each channel.
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			Float* hrtfFilter = threadState.interleavedPartition.getChannel(c);
			
			hrtf.getFilter( c, threadState.shBasis, hrtfFilter );
		}
	}
	
	//******************************************************************************
	// Update the input IR for the FDLs.
	
	const Size maxNumFDLs = convolutionState.fdls.getSize();
	
	for ( Index fdlIndex = 0; fdlIndex < maxNumFDLs; fdlIndex++ )
	{
		FDL& fdl = *convolutionState.fdls[fdlIndex];
		const FDLState& fdlState = *fdls[fdlIndex];
		
		// Skip this FDL if the rendering thread has not consumed the last updated IR.
		if ( fdl.numInputIRs > 0 )
			continue;
		
		// Get the input IR for this FDL.
		SampleBuffer<ComplexSample>& inputIR = fdl.getInputIR();
		
		// Compute the offset within the original IR of the start of this FDL (not padded).
		const Index fdlOffset = fdlState.offset;
		
		// If the FDL overlaps the IR, then update the IR, otherwise set the FDL to zero.
		if ( fdlOffset < hrtfLength )
		{
			// Make sure the input IR for this FDL is allocated.
			if ( !inputIR.isAllocated() )
				inputIR.allocate();
			
			// Copy the new IR to the input IR, one partition at a time.
			for ( Index j = 0; j < fdlState.maxNumPartitions; j++ )
			{
				// Compute the size of the IR data for this partition.
				const Index paddedPartitionOffset = j*fdlState.paddedFFTStorage;
				const Index partitionOffset = fdlOffset + j*fdlState.fftSize;
				
				// Update this partition in the IR.
				if ( partitionOffset < hrtfLength )
				{
					const Size partitionLength = hrtfLength < partitionOffset ? 0 : math::min( hrtfLength - partitionOffset, fdlState.fftSize );
					
					// Copy the HRTF IR samples to the partition IR.
					for ( Index c = 0; c < numOutputChannels; c++ )
					{
						Float* const irSamples = (Float*)inputIR.getChannel( c, paddedPartitionOffset );
						const Float* hrtfSamples = threadState.interleavedPartition.getChannel(c) + partitionOffset;
						
						// Copy the HRTF into the partition, zeroing the last half of the FFT padding.
						om::util::copyPOD( irSamples, hrtfSamples, partitionLength );
						om::util::zeroPOD( irSamples + partitionLength, fdlState.paddedFFTSize - partitionLength );
						
						// Convert the HRTF partition to frequency domain.
						fdlState.fft( irSamples );
					}
				}
				else
				{
					// Zero the IR if this partition is past the end of the sampled IR.
					for ( Index c = 0; c < numOutputChannels; c++ )
					{
						ComplexSample* const irSamples = inputIR.getChannel( c, paddedPartitionOffset );
						om::util::zeroPOD( irSamples, fdlState.complexFFTSize );
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
		fdl.numInputIRs++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Renderer IR Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: updateConvolutionIR( ConvolutionState& convolutionState, const SoundSourceIR& sourceIR,
												const SoundListener& listener, const FrequencyBands& frequencies,
												UpdateThreadState& threadState )
{
	const SampledIR& ir = sourceIR.getSampledIR();
	const Size sampledIRLength = math::min( ir.getLengthInSamples(), convolutionState.maxIRLengthInSamples );
	const Index irStart = sourceIR.getStartTimeInSamples();
	const Size irLength = math::min( sourceIR.getLengthInSamples(), convolutionState.maxIRLengthInSamples );
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	
	const Size maxPathDelay = sourceIR.getMaxPathDelayInSamples();
	const Bool pathRenderingEnabled = request.flags.isSet( RenderFlags::DISCRETE_PATHS );
	const Size maxNumPaths = pathRenderingEnabled ? request.maxSourcePathCount : 0;
	const PathSortID* extraPaths = threadState.pathSortIDs.getPointer() + maxNumPaths;
	const Size numExtraPaths = threadState.pathSortIDs.getSize() > maxNumPaths ? 
								threadState.pathSortIDs.getSize() - maxNumPaths : 0;
	
	//******************************************************************************
	// Update the input IR for the FDLs.
	
	const Size maxNumFDLs = convolutionState.fdls.getSize();
	
	for ( Index fdlIndex = 0; fdlIndex < maxNumFDLs; fdlIndex++ )
	{
		FDL& fdl = *convolutionState.fdls[fdlIndex];
		const FDLState& fdlState = *fdls[fdlIndex];
		
		// Skip this FDL if the rendering thread has not consumed the last updated IR.
		if ( fdl.numInputIRs > 0 )
			continue;
		
		// Make sure the FDL's crossover history is allocated and has enough channels.
		if ( fdl.crossoverHistory.getSize() < numOutputChannels )
			fdl.crossoverHistory.setSize( numOutputChannels );
		
		CrossoverType::History* histories = fdl.crossoverHistory.getPointer();
		
		if ( fdlIndex == 0 )
		{
			// Reset the crossover history.
			for ( Index c = 0; c < numOutputChannels; c++ )
				histories[c].reset();
		}
		else
		{
			// Initialize the history with that of the partition before this.
			om::util::copyPOD( histories, convolutionState.fdls[fdlIndex-1]->crossoverHistory.getPointer(), numOutputChannels );
		}
		
		// Get the input IR for this FDL.
		SampleBuffer<ComplexSample>& inputIR = fdl.getInputIR();
		
		// Compute the offset within the original IR of the start of this FDL (not padded).
		const Index fdlOffset = fdlState.offset;
		const Size fdlLength = fdlState.maxNumPartitions*fdlState.fftSize;
		const Size fdlEnd = fdlOffset + fdlLength;
		
		// If the FDL overlaps the IR, then update the IR, otherwise set the FDL to zero.
		if ( fdlOffset < irLength && fdlEnd > irStart )
		{
			// Make sure the input IR for this FDL is allocated.
			if ( !inputIR.isAllocated() )
				inputIR.allocate();
			
			// Copy the new IR to the input IR, one partition at a time.
			for ( Index j = 0; j < fdlState.maxNumPartitions; j++ )
			{
				// Compute the size of the IR data for this partition.
				const Index paddedPartitionOffset = j*fdlState.paddedFFTStorage;
				const Index partitionOffset = fdlOffset + j*fdlState.fftSize;
				
				// Update this partition in the IR.
				if ( partitionOffset < sampledIRLength || partitionOffset < maxPathDelay )
				{
					const Size partitionLength = partitionOffset >= sampledIRLength ? 0 :
												math::min( sampledIRLength - partitionOffset, fdlState.fftSize );
					
					updatePartitionIR( listener, ir, extraPaths, partitionOffset < maxPathDelay ? numExtraPaths : 0,
										paddedPartitionOffset, partitionOffset, partitionLength,
										fdlState, inputIR, histories, threadState );
				}
				else
				{
					// Zero the IR if this partition is past the end of the sampled IR.
					for ( Index c = 0; c < numOutputChannels; c++ )
					{
						ComplexSample* const irSamples = inputIR.getChannel( c, paddedPartitionOffset );
						om::util::zeroPOD( irSamples, fdlState.complexFFTSize );
					}
					
					// Reset the crossover history since the last partition is zeros.
					for ( Index c = 0; c < numOutputChannels; c++ )
						histories[c].reset();
				}
			}
		}
		else
		{
			// Deallocate this input IR since it is all zeros.
			if ( inputIR.isAllocated() )
				inputIR.deallocate();
			
			// Reset the crossover history.
			for ( Index c = 0; c < numOutputChannels; c++ )
				histories[c].reset();
		}
		
		// Atomically increment the number of input IR, signaling to the FDL thread that there is a new IR.
		fdl.numInputIRs++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Partition IR Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: updatePartitionIR( const SoundListener& listener, const SampledIR& ir,
												const PathSortID* extraPaths, Size numExtraPaths,
												Index paddedPartitionOffset, Index partitionOffset, Size partitionLength,
												const FDLState& fdlState, SampleBuffer<ComplexSample>& fdlIR,
												CrossoverType::History* histories, UpdateThreadState& threadState )
{
	const FrequencyBands& frequencies = request.frequencies;
	const Size numFrequencyBands = frequencies.getBandCount();
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	const Index partitionEnd = partitionOffset + fdlState.fftSize;
	
	//******************************************************************************
	//******************************************************************************
	// Handle each channel layout differently.
	
	// Initialize each band and channel of the partition with the new IR.
	switch ( numOutputChannels )
	{
		// Mono IR.
		case 1:
		{
			Float32* panBuffer = threadState.panBuffer.getChannel(0);
			
			om::util::set( panBuffer, Float32(1), partitionLength );
		}
		break;
		
		// Stereo IR.
		case 2:
		{
			// Build the panning buffer.
			const Vector3f* inputDirections = ir.getDirections() + partitionOffset;
			const Vector3f* const inputDirectionsEnd = inputDirections + partitionLength;
			Float32* panBufferL = threadState.panBuffer.getChannel(0);
			Float32* panBufferR = threadState.panBuffer.getChannel(1);
			
			while ( inputDirections < inputDirectionsEnd )
			{
				Real directionMagnitude2 = (*inputDirections).getMagnitudeSquared();
				
				if ( directionMagnitude2 > math::epsilon<Real>() )
				{
					// Compute the normalized panning direction for this sample.
					const Vector3f d = (*inputDirections / math::sqrt(directionMagnitude2))*listener.getOrientation();
					
					Float left, right;
					panLookupTable.pan( d, left, right );
					
					// Pan left to right.
					*panBufferL = left;
					*panBufferR = right;
				}
				else
				{
					*panBufferL = Float32(1);
					*panBufferR = Float32(1);
				}
				
				inputDirections++;
				panBufferL++;
				panBufferR++;
			}
		}
		break;
		
		default:
		{
			// Build the panning buffer.
			const Vector3f* inputDirections = ir.getDirections() + partitionOffset;
			
			for ( Index i = 0; i < partitionLength; i++, inputDirections++ )
			{
				Real directionMagnitude2 = (*inputDirections).getMagnitudeSquared();
				
				if ( directionMagnitude2 > math::epsilon<Real>() )
				{
					// Compute the normalized panning direction for this sample.
					const Vector3f d = (*inputDirections / math::sqrt(directionMagnitude2))*listener.getOrientation();
					
					// Pan the impulse among the output channel layout. Skip the impulse if panning failed.
					if ( request.channelLayout.panDirection( d, threadState.channelGains ) )
					{
						for ( Index c = 0; c < numOutputChannels; c++ )
							threadState.panBuffer.getChannel(c)[i] = threadState.channelGains[c];
					}
				}
				else
				{
					for ( Index c = 0; c < numOutputChannels; c++ )
						threadState.panBuffer.getChannel(c)[i] = Float32(1);
				}
			}
		}
		break;
	}
	
	//******************************************************************************
	//******************************************************************************
	// Build the IR for each band.
	
	// Fill the interleaved IR with samples from each band of the input IR.
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		const Float* pan = threadState.panBuffer.getChannel(c);
		Float* interleaved = threadState.interleavedPartition.getChannel(c);
		
		// Interleave the frequency bands in the partition.
		interleaveBands( ir, partitionOffset, partitionLength, pan, interleaved );
	}
	
	// Zero the end of the interleaved IR if the sampled IR partition length doesn't fill the whole FFT size.
	if ( partitionLength < fdlState.fftSize )
	{
		const Index interleavedOffset = partitionLength*GSOUND_FREQUENCY_COUNT;
		const Size interleavedNumber = (fdlState.fftSize - partitionLength)*GSOUND_FREQUENCY_COUNT;
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			Float* interleaved = threadState.interleavedPartition.getChannel(c) + interleavedOffset;
			om::util::zeroPOD( interleaved, interleavedNumber );
		}
	}
	
	AABB1f bandRanges[GSOUND_FREQUENCY_COUNT];
	
	for ( Index b = 0; b < GSOUND_FREQUENCY_COUNT; b++ )
		bandRanges[b] = frequencies.getBandRange(b);
	
	// Add the contributions from the extra paths.
	for ( Index i = 0; i < numExtraPaths; i++ )
	{
		const SoundPath& path = *extraPaths[i].path;
		Index delayIndex = (Index)math::floor( path.getDelay()*request.sampleRate );
		
		// Skip paths that are not within the bounds of this FDL.
		if ( delayIndex >= partitionOffset && delayIndex < partitionEnd )
		{
			// Compute the normalized panning direction for this sample.
			const Vector3f d = path.getDirection()*listener.getOrientation();
			
			// Pan the impulse among the output channel layout. Skip the impulse if panning failed.
			if ( !request.channelLayout.panDirection( d, threadState.channelGains ) )
				continue;
			
			// Compute the destination padded index of the path in the complex sampled IR.
			const Index delayFDLOffset = (delayIndex - partitionOffset)*GSOUND_FREQUENCY_COUNT;
			const FrequencyBandResponse& intensity = path.getIntensity();
			
			// Add the path to the IR.
			for ( Index b = 0; b < numFrequencyBands; b++ )
			{
				const Gain bandGain = math::sqrt(intensity.getBandGain( bandRanges[b], frequencies ));
				
				for ( Index c = 0; c < numOutputChannels; c++ )
				{
					Float* interleaved = threadState.interleavedPartition.getChannel(c);
					interleaved[delayFDLOffset + b] += bandGain*threadState.channelGains[c];
				}
			}
		}
	}
	
	// Filter the bands in each channel and write to the final IR in time domain.
	for ( Index c = 0; c < numOutputChannels; c++ )
	{
		SIMDBands* interleaved = (SIMDBands*)threadState.interleavedPartition.getChannel(c);
		const SIMDBands* noise = (SIMDBands*)threadState.noiseBuffer.getChannel(0) + partitionOffset;
		const SIMDBands* noiseEnd = noise + fdlState.fftSize;
		Float32* const irSamples = (Float32*)fdlIR.getChannel( c, paddedPartitionOffset );
		Float32* ir = irSamples;
		
		crossover.filterSIMDLowPass( histories[c], (Float32*)interleaved, (Float32*)interleaved, fdlState.fftSize );
		
		while ( noise != noiseEnd )
		{
			*ir = math::sumScalar( (*interleaved)*(*noise) );
			interleaved++;
			noise++;
			ir++;
		}
		
		// Zero the FFT padding.
		om::util::zeroPOD( irSamples + fdlState.fftSize, fdlState.fftSize );
		
		// Convert the partition channel to frequency domain in place.
		fdlState.fft( irSamples );
	}
}




void SoundListenerRenderer:: interleaveBands( const SampledIR& ir, Index partitionOffset, Size partitionLength,
												const Float* pan, Float* partition )
{
	const SIMDBands* bands = (const SIMDBands*)ir.getIntensity() + partitionOffset;
	SIMDBands* partitionBands = (SIMDBands*)partition;
	const Float* const panEnd = pan + partitionLength;
	
	while ( pan != panEnd )
	{
		*partitionBands = math::sqrt(*bands)*SIMDBands(*pan);
		
		pan++;
		bands++;
		partitionBands++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Clear Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: clearIR()
{
	// Lock the mutex which handles rendering synchronization.
	renderingMutex.lock();
	
	
	
	
	// Unlock the rendering synchronization mutex.
	renderingMutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Rendering Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundListenerRenderer:: render( const SourceSoundBuffer& sourceInputBuffers, SoundBuffer& outputBuffer, const Time& outputLength )
{
	// Create a timer for this audio frame.
	Timer frameTimer;
	
	//******************************************************************************
	// Prepare the output buffer.
	
	renderingMutex.lock();
	
	// Make sure the output buffer has the right channel layout and size.
	outputBuffer.setLayout( request.channelLayout );
	outputBuffer.setSampleRate( request.sampleRate );
	
	// Determine how many samples should be rendered at the renderer's output sample rate.
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	const Size numSamples = (Size)math::round( outputLength*request.sampleRate );
	
	if ( outputBuffer.getSize() < numSamples )
		outputBuffer.setSize( numSamples );
	
	// Zero the output buffer.
	outputBuffer.zero( 0, numSamples );
	
	//******************************************************************************
	
	// Prepare the input audio for each sound source.
	bufferSourceInput( sourceInputBuffers, numSamples );
	
	// Mix the input audio for each source cluster.
	mixClusterInput( numSamples );
	
	//******************************************************************************
	
	// Zero the output buffers for the clusters.
	const Size numClusterStates = clusterStates.getSize();
	const Bool hrtfEnabled = request.flags.isSet( RenderFlags::HRTF );
	
	for ( Index i = 0; i < numClusterStates; i++ )
	{
		if ( clusterStates.isUnused(i) )
			continue;
		
		ClusterState& clusterState = *clusterStates[i];
		
		//******************************************************************************
		// Make sure the cluster output buffer is big enough.
		
		SoundBuffer& clusterOutputBuffer = clusterState.outputBuffer;
		
		if ( clusterOutputBuffer.getSize() < numSamples )
			clusterOutputBuffer.setSize( numSamples );
		
		if ( clusterOutputBuffer.getChannelCount() < numOutputChannels )
			clusterOutputBuffer.setChannelCount( numOutputChannels );
		
		clusterOutputBuffer.zero( 0, numSamples );
		
		if ( hrtfEnabled )
		{
			SoundBuffer& hrtfInputBuffer = clusterState.hrtfInputBuffer;
			
			if ( hrtfInputBuffer.getSize() < numSamples )
				hrtfInputBuffer.setSize( numSamples );
			
			if ( hrtfInputBuffer.getChannelCount() < Size(1) )
				hrtfInputBuffer.setChannelCount( Size(1) );
			
			hrtfInputBuffer.zero( 0, numSamples );
		}
	}
	
	//******************************************************************************
	// Render the output audio for all source clusters in parallel, mixing to the cluster output buffers.
	
	// Render discrete paths.
	if ( request.flags.isSet( RenderFlags::DISCRETE_PATHS ) )
		renderPaths( numSamples );
	
	// Render convolution for sampled IRs.
	if ( request.flags.isSet( RenderFlags::CONVOLUTION ) )
		renderConvolution( numSamples );
	
	if ( request.flags.isSet( RenderFlags::REVERB ) )
		renderReverb( numSamples );
	
	//******************************************************************************
	// Accumulate the cluster output audio to the main output buffer.
	
	mixClusterOutput( outputBuffer, numSamples );
	
	//******************************************************************************
	
	// Compute the fraction of the time spent rendering the sound.
	processingLoad = Float(frameTimer.getElapsedTime() / outputLength);
	
	renderingMutex.unlock();
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Source Input Buffering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: bufferSourceInput( const SourceSoundBuffer& sourceInput, Size numSamples )
{
	// Prepare the input audio for each sound source.
	const Size numInputSources = sourceInput.getSourceCount();
	
	for ( Index s = 0; s < numInputSources; s++ )
	{
		const SoundSource* const source = sourceInput.getSource(s);
		const Index* sourceStateIndex;
		
		// Find the source render state for this source if there is one.
		if ( sourceStateMap.find( source->getHashCode(), source, sourceStateIndex ) )
		{
			SourceState* const sourceState = sourceStates[*sourceStateIndex];
			SoundBuffer& sourceStateBuffer = sourceState->inputBuffer;
			
			// Make sure the source state input buffer is big enough.
			if ( sourceStateBuffer.getChannelCount() < Size(1) )
				sourceStateBuffer.setChannelCount( Size(1) );
			
			if ( sourceStateBuffer.getSize() < numSamples )
				sourceStateBuffer.setSize( numSamples );
			
			// Get the input buffer of source audio.
			const SoundBuffer* sourceInputBuffer = sourceInput.getSourceBuffer(s);
			
			if ( sourceInputBuffer )
			{
				// Copy the input buffer samples.
				Size numInputSamples = math::min( numSamples, sourceInputBuffer->getSize() );
				const Size numInputChannels = sourceInputBuffer->getChannelCount();
				const SampleRate inputSampleRate = sourceInputBuffer->getSampleRate();
				
				// Do sample rate conversion if necessary.
				if ( inputSampleRate != request.sampleRate )
				{
					// Create a new sample rate converter for the source if necessary.
					if ( sourceState->resampler == NULL )
						sourceState->resampler = util::construct<Resampler>();
					
					Resampler* sourceRateConverter = sourceState->resampler;
					
					// Update the output sample rate for the converter if it differs from the output rate.
					if ( sourceRateConverter->getOutputSampleRate() != request.sampleRate )
						sourceRateConverter->setOutputSampleRate( request.sampleRate );
					
					// Convert the sample rate of the input buffer.
					numInputSamples = sourceRateConverter->process( *sourceInputBuffer, sourceStateBuffer, numInputSamples );
					
					if ( numInputChannels > Size(1) )
					{
						// Add the remaining channels to the first one.
						for ( Index c = 1; c < numInputChannels; c++ )
						{
							om::math::add( (Float32*)sourceStateBuffer.getChannel(0),
											(Float32*)sourceStateBuffer.getChannel(c), numInputSamples );
						}
						
						// Divide by the number of channels.
						om::math::multiply( (Float32*)sourceStateBuffer.getChannel(0), Float(1) / Float(numInputChannels), numInputSamples );
					}
				}
				else
				{
					// Copy the input sound directly to the source state buffer, mixing to mono if necessary.
					om::util::copyPOD( sourceStateBuffer.getChannel(0), sourceInputBuffer->getChannel(0), numInputSamples );
					
					if ( numInputChannels > Size(1) )
					{
						// Add the remaining channels.
						for ( Index c = 1; c < numInputChannels; c++ )
						{
							om::math::add( (Float32*)sourceStateBuffer.getChannel(0),
											(Float32*)sourceInputBuffer->getChannel(c), numInputSamples );
						}
						
						// Divide by the number of channels.
						om::math::multiply( (Float32*)sourceStateBuffer.getChannel(0), Float(1) / Float(numInputChannels), numInputSamples );
					}
				}
				
				// Zero the end of the input buffer if necessary.
				if ( numInputSamples < numSamples )
					sourceStateBuffer.zero( numInputSamples, numSamples - numInputSamples );
			}
			else
			{
				// Zero the source input buffer because there is no valid buffer.
				sourceStateBuffer.zero( 0, numSamples );
			}
		}
	}
	
	//***********************************************************************
	// Remove old sound source states from the renderer.
	
	HashMap<const SoundSource*,Index>::Iterator sourceStateIterator = sourceStateMap.getIterator();
	
	while ( sourceStateIterator )
	{
		SourceState* sourceState = sourceStates[sourceStateIterator.getValue()];
		
		// Check to see if the source state should be removed.
		if ( sourceState->updateTimeStamp != timeStamp && sourceState->clusters.getSize() == Size(0) )
		{
			// Source is no longer used for any clusters, remove it and move to the next one.
			sourceState->source = NULL;
			sourceStateIterator.remove();
			continue;
		}
		
		sourceStateIterator++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster Input Mixing Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: mixClusterInput( Size numSamples )
{
	// The length of the output buffer in seconds.
	const Float outputLength = Float(numSamples) / Float(request.sampleRate);
	const Size numClusterStates = clusterStates.getSize();
	
	for ( Index i = 0; i < numClusterStates; i++ )
	{
		ClusterState& clusterState = *clusterStates[i];
		
		// Skip unused cluster states.
		if ( clusterStates.isUnused(i) )
			continue;
		
		LerpState<Float32>& clusterGain = clusterState.gain;
		
		// Check to see if this cluster should be faded out if it hasn't been updated.
		if ( clusterState.timeStamp != timeStamp )
		{
			// Start to fade this cluster out.
			if ( clusterGain.target != Gain(0) )
				clusterGain.time = request.clusterFadeOutTime;
			
			clusterGain.target = Gain(0);
		}
		
		//******************************************************************************
		// Make sure the cluster input buffers are big enough.
		
		SoundBuffer& clusterInputBuffer = clusterState.inputBuffer;
		
		if ( clusterInputBuffer.getSize() < numSamples )
			clusterInputBuffer.setSize( numSamples );
		
		if ( clusterInputBuffer.getChannelCount() != Size(1) )
			clusterInputBuffer.setChannelCount( Size(1) );
		
		SoundBuffer& hrtfInputBuffer = clusterState.hrtfInputBuffer;
		
		if ( hrtfInputBuffer.getSize() < numSamples )
			hrtfInputBuffer.setSize( numSamples );
		
		if ( hrtfInputBuffer.getChannelCount() != Size(1) )
			hrtfInputBuffer.setChannelCount( Size(1) );
		
		//******************************************************************************
		// Render each source's audio into the cluster's input buffer.
		
		Sample32f* const clusterInputStart = clusterInputBuffer.getChannel(0);
		
		for ( Index s = 0; s < clusterState.sources.getSize(); )
		{
			ClusteredSourceState& clusteredSource = *clusterState.sources[s];
			LerpState<Float32>& sourceGain = clusteredSource.gain;
			
			// If the clustered source is old, start to fade it out.
			if ( clusteredSource.timeStamp != timeStamp )
			{
				if ( sourceGain.target != Gain(0) )
					sourceGain.time = request.sourceFadeTime;
				
				sourceGain.target = Gain(0);
			}
			
			const SoundBuffer& sourceInputBuffer = clusteredSource.source->inputBuffer;
			const SoundSource* source = clusteredSource.source->source;
			
			if ( !source )
			{
				clusteredSource.source->removeCluster( clusteredSource.cluster );
				clusterState.sources.removeAtIndexUnordered(s);
				continue;
			}
			
			Sample32f* clusterInput = clusterInputStart;
			const Sample32f* sourceInput = sourceInputBuffer.getChannel(0);
			const Sample32f* const sourceInputEnd = sourceInput + numSamples;
			
			Gain currentSourceGain = sourceGain.current;
			const Gain targetSourceGain = sourceGain.target*source->getPower()*POWER_BIAS;
			
			if ( currentSourceGain == targetSourceGain )
			{
				if ( s == 0 )
					math::multiply( (Float32*)clusterInput, (const Float32*)sourceInput, currentSourceGain, sourceInputEnd - sourceInput );
				else
					math::multiplyAdd( (Float32*)clusterInput, (const Float32*)sourceInput, currentSourceGain, sourceInputEnd - sourceInput );
				
				// Ensure there is no interpolation.
				sourceGain.time = 0;
			}
			else
			{
				// If the fade time is not set, determine a default value.
				if ( sourceGain.time == Float(0) )
					sourceGain.time = request.sourceFadeTime;
				
				// Interpolate the source gain change.
				const Gain gainChangePerSample = (targetSourceGain - sourceGain.current)
																/ (sourceGain.time*Gain(request.sampleRate));
				
				if ( sourceGain.time <= outputLength )
				{
					const Sample32f* const interpolationEnd = sourceInput + Size(numSamples*(sourceGain.time / outputLength));
					
					if ( s == 0 )
						interpolateGainSet( sourceInput, interpolationEnd, clusterInput, currentSourceGain, gainChangePerSample );
					else
						interpolateGainAdd( sourceInput, interpolationEnd, clusterInput, currentSourceGain, gainChangePerSample );
					
					// Interpolation is finished.
					sourceGain.time = 0;
					sourceGain.current = targetSourceGain;
					
					// Check to see if this clustered source is supposed to removed.
					if ( clusteredSource.timeStamp != timeStamp && sourceGain.current == 0 )
					{
						// The fade-out interpolation is finished, so now this clustered source
						// can be destroyed and removed from its source and cluster render states.
						
						// Remove the clustered source from the source render state.
						clusteredSource.source->removeCluster( &clusterState );
						
						// Remove the clustered source from this cluster.
						clusterState.sources.removeAtIndexUnordered( s );
						
						// Continue to the next clustered source.
						continue;
					}
					
					// Finish the output with constant gain.
					if ( s == 0 )
						math::multiply( (Float32*)clusterInput, (const Float32*)sourceInput, currentSourceGain, sourceInputEnd - sourceInput );
					else
						math::multiplyAdd( (Float32*)clusterInput, (const Float32*)sourceInput, currentSourceGain, sourceInputEnd - sourceInput );
				}
				else
				{
					if ( s == 0 )
						interpolateGainSet( sourceInput, sourceInputEnd, clusterInput, currentSourceGain, gainChangePerSample );
					else
						interpolateGainAdd( sourceInput, sourceInputEnd, clusterInput, currentSourceGain, gainChangePerSample );
					
					sourceGain.current += gainChangePerSample*numSamples;
					sourceGain.time -= outputLength;
				}
			}
			
			// Move to the next source for this cluster.
			s++;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster Output Mixing Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: mixClusterOutput( SoundBuffer& outputBuffer, Size numSamples )
{
	const Float outputLength = Float(numSamples) / Float(request.sampleRate);
	const Size numClusterStates = clusterStates.getSize();
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	
	// Mix the output audio for each cluster to the output buffer.
	for ( Index clusterIndex = 0; clusterIndex < numClusterStates; clusterIndex++ )
	{
		ClusterState& clusterState = *clusterStates[clusterIndex];
		
		// Skip unused cluster states.
		if ( clusterStates.isUnused(clusterIndex) )
			continue;
		
		SoundBuffer& clusterOutputBuffer = clusterState.outputBuffer;
		Size numClusterSamples = clusterOutputBuffer.getSize();
		
		//******************************************************************************
		// Mix the cluster audio to the output.
		
		LerpState<Float32>& clusterGain = clusterState.gain;
		
		if ( clusterGain.current == clusterGain.target )
		{
			// No output level interpolation.
			if ( clusterGain.current == Gain(1) )
			{
				// Fast version with no output gain, mix directly to the output buffer.
				clusterOutputBuffer.mixTo( outputBuffer, numClusterSamples );
			}
			else
			{
				clusterOutputBuffer.applyGain( clusterGain.current, numClusterSamples );
				clusterOutputBuffer.mixTo( outputBuffer, numClusterSamples );
			}
			
			// Make sure there is no interpolation.
			clusterGain.time = Float(0);
		}
		else
		{
			// Interpolate the output gain for this cluster.
			
			// If the fade time is not set, determine a default value.
			if ( clusterGain.time == Float(0) )
			{
				if ( clusterGain.target > clusterGain.current )
					clusterGain.time = request.clusterFadeInTime;
				else
					clusterGain.time = request.clusterFadeOutTime;
			}
			
			// Determine how much to interpolate this output buffer.
			const Gain gainChangePerSample = (clusterGain.target - clusterGain.current)
											/ (clusterGain.time*Gain(request.sampleRate));
			
			if ( clusterGain.time <= outputLength )
			{
				Size numInterpolatedSamples = Size(numClusterSamples*(clusterGain.time / outputLength));
				
				for ( Index c = 0; c < numOutputChannels; c++ )
				{
					const Sample32f* clusterOutput = clusterOutputBuffer.getChannel(c);
					const Sample32f* const clusterInterpolationEnd = clusterOutput + numInterpolatedSamples;
					Sample32f* output = outputBuffer.getChannel(c);
					Gain tempClusterGain = clusterGain.current;
					
					interpolateGainAdd( clusterOutput, clusterInterpolationEnd, output, tempClusterGain, gainChangePerSample );
					
					tempClusterGain = clusterGain.target;
					
					math::multiplyAdd( (Float32*)output, (const Float32*)clusterOutput, tempClusterGain,
										numClusterSamples - numInterpolatedSamples );
				}
				
				// Interpolation is finished.
				clusterGain.current = clusterGain.target;
				clusterGain.time = 0;
				
				// Check to see if this cluster is supposed to removed.
				if ( clusterState.timeStamp != timeStamp && clusterGain.current == Gain(0) )
				{
					// The fade-out interpolation is finished, so now this cluster
					// can be destroyed and its rendering finished.
					
					destroyCluster( clusterIndex );
					
					// Continue to the next clustered source.
					continue;
				}
			}
			else
			{
				for ( Index c = 0; c < numOutputChannels; c++ )
				{
					const Sample32f* clusterOutput = clusterOutputBuffer.getChannel(c);
					const Sample32f* const clusterOutputEnd = clusterOutput + numClusterSamples;
					Sample32f* output = outputBuffer.getChannel(c);
					
					interpolateGainAdd( clusterOutput, clusterOutputEnd, output, clusterGain.current, gainChangePerSample );
				}
				
				clusterGain.current += numClusterSamples*gainChangePerSample;
				clusterGain.time -= outputLength;
			}
		}
	}
	
	//******************************************************************************
	// Apply the gain due to listener sensitivity.
	
	if ( listenerGain.current == listenerGain.target )
	{
		// No output level interpolation.
		outputBuffer.applyGain( listenerGain.current, numSamples );
		
		// Make sure there is no interpolation.
		listenerGain.time = Float(0);
	}
	else
	{
		// Interpolate the output gain for this cluster.
		
		// If the fade time is not set, determine a default value.
		if ( listenerGain.time == Float(0) )
			listenerGain.time = request.pathFadeTime;
		
		// Determine how much to interpolate this output buffer.
		const Gain gainChangePerSample = (listenerGain.target - listenerGain.current)
										/ (listenerGain.time*Gain(request.sampleRate));
		
		if ( listenerGain.time <= outputLength )
		{
			Size numInterpolatedSamples = Size(numSamples*(listenerGain.time / outputLength));
			
			for ( Index c = 0; c < numOutputChannels; c++ )
			{
				Sample32f* output = outputBuffer.getChannel(c);
				const Sample32f* const outputInterpolationEnd = output + numInterpolatedSamples;
				const Sample32f* const outputEnd = output + numSamples;
				Gain clusterGain = listenerGain.current;
				
				while ( output != outputInterpolationEnd )
				{
					*output = clusterGain*(*output);
					clusterGain += gainChangePerSample;
					output++;
				}
				
				clusterGain = listenerGain.target;
				
				while ( output != outputEnd )
				{
					*output = clusterGain*(*output);
					output++;
				}
			}
			
			// Interpolation is finished.
			listenerGain.current = listenerGain.target;
			listenerGain.time = 0;
		}
		else
		{
			for ( Index c = 0; c < numOutputChannels; c++ )
			{
				Sample32f* output = outputBuffer.getChannel(c);
				const Sample32f* const outputEnd = output + numSamples;
				Gain clusterGain = listenerGain.current;
				
				while ( output != outputEnd )
				{
					*output = clusterGain*(*output);
					clusterGain += gainChangePerSample;
					output++;
				}
			}
			
			listenerGain.current += numSamples*gainChangePerSample;
			listenerGain.time -= outputLength;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: renderReverb( Size numSamples )
{
	const Size numClusterStates = clusterStates.getSize();
	SharedSoundBuffer sharedBuffer = SharedBufferPool::getGlobalBuffer( request.channelLayout.getChannelCount(), numSamples, request.sampleRate );
	
	// Render the paths for each cluster state.
	for ( Index i = 0; i < numClusterStates; i++ )
	{
		ClusterState& clusterState = *clusterStates[i];
		
		// Skip unused cluster states.
		if ( clusterStates.isUnused(i) )
			continue;
		
		//******************************************************************************
		// Render the audio for this cluster.
		/*
		// Widen the input buffer to the number of output channels.
		Size oldChannelCount = clusterState.hrtfInputBuffer.getChannelCount();
		Size outputChannelCount = clusterState.outputBuffer.getChannelCount();
		clusterState.hrtfInputBuffer.setChannelCount( outputChannelCount );
		
		for ( Index c = oldChannelCount; c < outputChannelCount; c++ )
		{
			om::util::copyPOD( clusterState.hrtfInputBuffer.getChannel(c),
								clusterState.hrtfInputBuffer.getChannel(oldChannelCount-1),
								numSamples );
		}
		*/
		
		// Widen the input buffer to the number of output channels.
		Size oldChannelCount = clusterState.inputBuffer.getChannelCount();
		Size outputChannelCount = clusterState.outputBuffer.getChannelCount();
		clusterState.inputBuffer.setChannelCount( outputChannelCount );
		
		for ( Index c = oldChannelCount; c < outputChannelCount; c++ )
		{
			om::util::copyPOD( clusterState.inputBuffer.getChannel(c),
								clusterState.inputBuffer.getChannel(oldChannelCount-1),
								numSamples );
		}
		
		UInt flushMode = _MM_GET_FLUSH_ZERO_MODE();
		_MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
		
		clusterState.reverb.process( clusterState.inputBuffer, sharedBuffer.getBuffer(), numSamples );
		
		if ( request.flags.isSet( RenderFlags::REVERB ) )
			sharedBuffer.getBuffer().mixTo( clusterState.outputBuffer, numSamples );
		
		_MM_SET_FLUSH_ZERO_MODE( flushMode );
		
		clusterState.hrtfInputBuffer.setChannelCount( oldChannelCount );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: renderPaths( Size numSamples )
{
	const Size numClusterStates = clusterStates.getSize();
	
	// Render the paths for each cluster state.
	for ( Index i = 0; i < numClusterStates; i++ )
	{
		ClusterState& clusterState = *clusterStates[i];
		
		// Skip unused cluster states.
		if ( clusterStates.isUnused(i) )
			continue;
		
		//******************************************************************************
		// Render the audio for this cluster.
		
		const SoundBuffer& clusterInputBuffer = clusterState.inputBuffer;
		SoundBuffer& clusterOutputBuffer = clusterState.outputBuffer;
		SoundBuffer& clusterHRTFBuffer = clusterState.hrtfInputBuffer;
		
		renderPathState( clusterState.pathRenderer, clusterInputBuffer, clusterOutputBuffer, clusterHRTFBuffer, numSamples );
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path State Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: renderPathState( PathRenderState& renderer, const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer,
												SoundBuffer& hrtfBuffer, Size numSamples )
{
	//***********************************************************************
	// Get the render thread data for this thread.
	
	RenderThreadState* threadState = &sharedRenderState;
	
	//***********************************************************************
	
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	const Size numFrequencyBands = request.frequencies.getBandCount();
	
	//******************************************************************************
	// Do initial setup for path rendering
	
	// Compute some values needed for path interpolation.
	const Float outputLengthInSeconds = Float(numSamples)/Float(request.sampleRate);
	const Float halfSampleLength = Float(0.5)/Float(request.sampleRate);
	const Float inverseNumSamples = Float(1) / Float(numSamples);
	
	//****************************************************************************
	// Write the crossover's output to the delay buffer in frequency-band-interleaved format.
	
	// Save the index of the sample with 0 delay so that we can calculate delay offsets later.
	Index currentDelayReadIndex = renderer.currentDelayWriteIndex;
	Float32* const delayBufferStart = renderer.delayBuffer.getChannel(0);
	const Float32* inputStart = (const Float32*)inputBuffer.getChannel(0);
	
	for ( Size numSamplesProcessed = 0; numSamplesProcessed < numSamples; )
	{
		// Write until the end of the delay buffer is reached.
		Size numSamplesToWrite = math::min( numSamples - numSamplesProcessed,
											renderer.delayBufferSize - renderer.currentDelayWriteIndex );
		
		// Compute the current offset in the input and delay buffers.
		const Float32* input = inputStart + numSamplesProcessed;
		Float32* const delay = delayBufferStart + renderer.currentDelayWriteIndex*numFrequencyBands;
		
		// Process the crossover filter, writing the band-interleaved output to the delay buffer.
		crossover.filterScalar( *renderer.crossoverHistory, input, delay, numSamplesToWrite );
		
		// Update the delay write index.
		renderer.currentDelayWriteIndex = (renderer.currentDelayWriteIndex + numSamplesToWrite) % renderer.delayBufferSize;
		
		// Update the number of samples remaining to be written.
		numSamplesProcessed += numSamplesToWrite;
	}
	
	//****************************************************************************
	// Update the paths if there are new paths.
	
	if ( renderer.hasNewPaths )
	{
		updatePathIR( renderer, *threadState );
		
		// Atomically signal to the update thread that we are ready for new paths.
		renderer.hasNewPaths--;
	}
	
	//****************************************************************************
	//****************************************************************************
	// Render every propagation path for the source.
	
	const Bool hrtfEnabled = request.flags.isSet( RenderFlags::HRTF );
	const Float currentDelayReadIndexFloat = (Float)currentDelayReadIndex;
	const Float sampleRateFloat = (Float)request.sampleRate;
	const Float delayBufferSizeFloat = (Float)renderer.delayBufferSize;
	
	SampleBuffer<SIMDBandLerpState>& lerpState = renderer.lerpState;
	HashMap<SoundPathHash,PathState,SoundPathHash>::Iterator pathStateIterator = renderer.pathMap.getIterator();
	
	while ( pathStateIterator )
	{
		PathState& pathState = *pathStateIterator;
		const Index pathIndex = pathState.index;
		const Bool hrtfPath = hrtfEnabled && pathState.flags.isSet( SoundPathFlags::DIRECT );
		const Size numPathOutputChannels = hrtfPath ? Size(1) : numOutputChannels;
		
		//****************************************************************************
		// Determine if the path should fade out or be removed.
		
		if ( pathState.timeStamp == renderer.timeStamp - 1 )
		{
			// Start to fade out the path if the path doesn't exist on this frame.
			pathState.lerpTime = request.pathFadeTime;
			
			// Set all channel gains to zero.
			for ( Index c = 0; c < numOutputChannels; c++ )
			{
				SIMDBandLerpState& channelLerp = *lerpState.getChannel( c, pathIndex );
				channelLerp.targetGain = Float32(0);
			}
		}
		
		const Float lerpFraction = pathState.lerpTime > 0 ? (outputLengthInSeconds / pathState.lerpTime) : 0;
		Float lerpRate = lerpFraction*inverseNumSamples;
		
		//****************************************************************************
		// Determine how the delay of the propagation path should change.
		
		Float delayChange = 0;
		
		// If the current and target delay times are closer than half a sample, set them to be the same.
		if ( math::fuzzyEquals( pathState.currentDelayTime, pathState.targetDelayTime, halfSampleLength ) )
			pathState.currentDelayTime = pathState.targetDelayTime;
		else
		{
			// Determine how much the delay time should change.
			const Float dopplerDelayRate = pathState.delayChangePerSecond;
			const Float absDopplerDelayRate = math::abs(dopplerDelayRate);
			const Float midpointDelayRate = (math::average( pathState.currentDelayTime, pathState.targetDelayTime ) - 
													pathState.currentDelayTime ) / outputLengthInSeconds;
			const Float absMidpointDelayRate = math::abs(midpointDelayRate);
			
			// Start with the midpoint delay rate.
			Float delayRate = midpointDelayRate;
			
			// Use the doppler delay change if it is set to a significant value.
			if ( absDopplerDelayRate > halfSampleLength*outputLengthInSeconds )
			{
				// If the doppler delay rate seems to be the more important, us it directly.
				if ( absDopplerDelayRate > absMidpointDelayRate || dopplerDelayRate*midpointDelayRate < 0 )
					delayRate = dopplerDelayRate;
				else
				{
					// Otherwise, clamp the magnitude of the midpoint delay rate to be less than the doppler amount for smoother interpolation.
					delayRate = math::sign(midpointDelayRate)*math::min( absMidpointDelayRate, absDopplerDelayRate );
				}
			}
			
			// Make sure the rate of change is less than the max.
			delayRate = math::sign(delayRate)*math::min( math::abs(delayRate), request.maxDelayRate );
			
			// Average the last delay change with the current calculated one to determine the next estimate of the rate of change.
			pathState.delayChangePerSecond = math::average( pathState.delayChangePerSecond, delayRate );
			delayChange = delayRate*outputLengthInSeconds;
		}
		
		const Float nextDelay = pathState.currentDelayTime + delayChange;
		
		if ( nextDelay >= request.maxPathDelay )
			delayChange = math::max( request.maxPathDelay - pathState.currentDelayTime, Float(0) );
		else if ( nextDelay < Float(0) )
			delayChange = math::max( -pathState.currentDelayTime, Float(0) );
		
		// Compute the change in delay offset per sample (1 == no change).
		const Float delayChangePerSample = Float(1) - delayChange*sampleRateFloat*inverseNumSamples;
		
		// Compute the floating-point offset within the delay buffer of the current read position.
		Float delayStartFloat = currentDelayReadIndexFloat - pathState.currentDelayTime*sampleRateFloat;
		
		if ( delayStartFloat < Float(0) )
			delayStartFloat += delayBufferSizeFloat;
		
		//****************************************************************************
		// Render the path for each channel and frequency band.
		
		for ( Index c = 0; c < numPathOutputChannels; c++ )
		{
			Float32* const outputStart = hrtfPath ? (Float32*)hrtfBuffer.getChannel(c) : (Float32*)outputBuffer.getChannel(c);
			SIMDBandLerpState& channelLerp = *lerpState.getChannel( c, pathIndex );
			
			// Determine the target gain and how much to interpolate.
			SIMDBands targetGain = channelLerp.targetGain;
			SIMDBands gainChangePerSample = (channelLerp.targetGain - channelLerp.currentGain)*lerpRate;
			
			// Interpolate the delay.
			Float currentDelay = delayStartFloat;
			
			// Read from the delay buffer until the output buffer is full.
			Size numSamplesProcessed = 0;
			
			while ( numSamplesProcessed < numSamples )
			{
				// Make sure the current delay is not past the end of the buffer.
				if ( currentDelay >= delayBufferSizeFloat )
					currentDelay -= delayBufferSizeFloat;
				
				// Determine how many samples to process.
				Float samplesBeforeBufferEnd = (delayBufferSizeFloat - currentDelay)/delayChangePerSample;
				const Size numSamplesToProcess = math::min( numSamples - numSamplesProcessed, (Size)samplesBeforeBufferEnd );
				
				// Compute the output range.
				Float32* const output = outputStart + numSamplesProcessed;
				const Float32* const outputEnd = output + numSamplesToProcess;
				
				if ( samplesBeforeBufferEnd <= Float(1) )
				{
					// Reached the end of the delay buffer. Interpolate the first and last samples.
					const Float32* lastDelay = delayBufferStart + Index(currentDelay)*SIMDBands::getWidth();
					const Float32* nextDelay = delayBufferStart;
					Float32 aSIMD = currentDelay - math::floor( currentDelay );
					
					SIMDBands last = SIMDBands::load(lastDelay);
					*output += math::sumScalar( ((SIMDBands::load(nextDelay) - last)*aSIMD + last)*channelLerp.currentGain );
					
					channelLerp.currentGain += gainChangePerSample;
					currentDelay += delayChangePerSample;
					numSamplesProcessed++;
				}
				else
				{
					// Render the path for this iteration.
					if ( delayChange == Float(0) )
					{
						// There is no change in the delay time, so use a more efficient rendering method.
						interpolateDelayEqual( output, outputEnd, delayBufferStart,
											currentDelay, channelLerp.currentGain, gainChangePerSample );
					}
					else
					{
						// The delay time changes, so use the expensive rendering method.
						interpolateDelayChanges( output, outputEnd, delayBufferStart,
												currentDelay, delayChangePerSample,
												channelLerp.currentGain, gainChangePerSample );
					}
					
					currentDelay += numSamplesToProcess*delayChangePerSample;
					numSamplesProcessed += numSamplesToProcess;
				}
			}
		}
		
		// Update the delay time for this path.
		pathState.currentDelayTime += delayChange;
		
		if ( pathState.lerpTime <= outputLengthInSeconds )
		{
			pathState.lerpTime = 0;
			
			if ( pathState.timeStamp < renderer.timeStamp )
			{
				// This path has been faded out, remove it.
				pathStateIterator.remove();
				renderer.removePath( pathIndex );
				continue;
			}
		}
		else if ( pathState.lerpTime > 0 )
			pathState.lerpTime -= outputLengthInSeconds;
		
		// Advance to the next path.
		pathStateIterator++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Renderer IR Update Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: updatePathIR( PathRenderState& renderer, RenderThreadState& threadState )
{
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	const FrequencyBands& frequencies = request.frequencies;
	const Size numFrequencyBands = frequencies.getBandCount();
	const Bool hrtfEnabled = request.flags.isSet( RenderFlags::HRTF );
	
	// Update the renderer timestamp.
	renderer.timeStamp = timeStamp;
	
	// Compute the constant delay in seconds that the frequency-domain convolution incurrs.
	// The paths are delayed by this amount to match the convolution output.
	const Float convolutionLatency = Float(3*minFDLSize / request.sampleRate);
	
	// Get the new paths.
	const SoundPath* newPaths = renderer.newPaths.getPointer();
	const Size numNewPaths = renderer.newPaths.getSize();
	
	//***********************************************************************
	// Make sure the path renderer buffer has the correct size.
	
	SampleBuffer<SIMDBandLerpState>& lerpState = renderer.lerpState;
	
	if ( lerpState.getChannelCount() != numOutputChannels || lerpState.getSampleCount() < numNewPaths )
		lerpState.setFormatCopy( numOutputChannels, math::max( lerpState.getSampleCount(), numNewPaths ) );
	
	// Make sure the storage is allocated.
	if ( !lerpState.isAllocated() )
		lerpState.allocate();
	
	//***********************************************************************
	// Update the paths.
	
	for ( Index i = 0; i < numNewPaths; i++ )
	{
		const SoundPath& path = newPaths[i];
		const SoundPathHash pathHash = path.getHashCode();
		const FrequencyBandResponse& intensity = path.getIntensity();
		const Bool hrtfPath = hrtfEnabled && path.getFlags().isSet( SoundPathFlags::DIRECT );
		
		// Compute the delay time and delay change for this path.
		const Float extraLatency = hrtfPath ? 0.0f : convolutionLatency;
		const Float delayTime = math::min( extraLatency + path.getDelay(), request.maxPathDelay );
		const Float delayChangePerSecond = path.getRelativeSpeed() / path.getSpeed();
		
		// Find the render state for this path in the previous rendering state,
		// or create one if it does not exist yet.
		PathState* pathState = renderer.findPath( pathHash );
		Bool newPath = false;
		
		if ( !pathState )
		{
			pathState = renderer.newPath( pathHash );
			newPath = true;
		}
		
		// Update the target delay time and doppler delay change for the path.
		pathState->targetDelayTime = delayTime;
		pathState->delayChangePerSecond = delayChangePerSecond;
		pathState->lerpTime = request.pathFadeTime;
		pathState->timeStamp = renderer.timeStamp;
		pathState->flags = path.getFlags();
		
		// Get the index of this path in the path renderer's storage.
		const Index pathIndex = pathState->index;
		
		if ( hrtfPath )
		{
			// Set the gain coefficients for each output channel and frequency band.
			for ( Index band = 0; band < numFrequencyBands; band++ )
			{
				// Get the gain for this frequency band.
				Gain bandGain = intensity.getBandGain( frequencies.getBandRange( band ), frequencies );
				
				// Initialize the state for each channel of this frequency band.
				for ( Index c = 0; c < numOutputChannels; c++ )
					lerpState.getChannel( c, pathIndex )->targetGain[band] = math::sqrt(bandGain);
			}
		}
		else
		{
			// Pan the impulse among the output channel layout.
			if ( !request.channelLayout.panDirection( path.getDirection()*renderer.listenerOrientation, threadState.channelGains ) )
				threadState.channelGains.setAll( 1.0f );
			
			// Set the gain coefficients for each output channel and frequency band.
			for ( Index band = 0; band < numFrequencyBands; band++ )
			{
				// Get the gain for this frequency band.
				Gain bandGain = intensity.getBandGain( frequencies.getBandRange( band ), frequencies );
				
				// Initialize the state for each channel of this frequency band.
				for ( Index c = 0; c < numOutputChannels; c++ )
					lerpState.getChannel( c, pathIndex )->targetGain[band] = math::sqrt(bandGain)*threadState.channelGains[c];
			}
		}
		
		// Set the current gain and delay to be the target since this path is new.
		if ( newPath )
		{
			pathState->currentDelayTime = pathState->targetDelayTime;
			
			for ( Index c = 0; c < numOutputChannels; c++ )
			{
				SIMDBandLerpState& channelState = *lerpState.getChannel( c, pathIndex );
				channelState.currentGain = Float32(0);
				channelState.targetGain = channelState.targetGain;
			}
		}
	}
	
	// Clear the list of new paths for next time.
	renderer.newPaths.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: renderConvolution( Size numSamples )
{
	const Size numClusterStates = clusterStates.getSize();
	const Bool hrtfEnabled = request.flags.isSet( RenderFlags::HRTF );
	
	//******************************************************************************
	
	Size numSamplesProcessed = 0;
	
	while ( numSamplesProcessed < numSamples )
	{
		// Determine the number of samples to process in this iteration.
		// Make sure to not go past the next deadline.
		const Size numFrameSamples = math::min( numSamples - numSamplesProcessed,
												minFDLSize - fdls[0]->numBufferedSamples );
		
		//******************************************************************************
		// Write to the convolution input queue for each cluster.
		
		for ( Index clusterIndex = 0; clusterIndex < numClusterStates; clusterIndex++ )
		{
			ClusterState& clusterState = *clusterStates[clusterIndex];
			
			// Skip unused clusters.
			if ( clusterStates.isUnused(clusterIndex) )
				continue;
			
			// Copy the main convolution input for the cluster.
			ConvolutionState& convolutionState = *convolutionStates[clusterState.convolutionStateIndex];
			writeConvolutionInput( convolutionState, clusterState.inputBuffer, numSamplesProcessed, numFrameSamples );
			
			// Copy the HRTF convolution input for the cluster.
			ConvolutionState& hrtfState = *convolutionStates[clusterState.hrtfConvolutionIndex];

			if ( hrtfEnabled )
				writeConvolutionInput( hrtfState, clusterState.hrtfInputBuffer, numSamplesProcessed, numFrameSamples );
			else
				advanceConvolutionInput( hrtfState, numSamplesProcessed, numFrameSamples );
		}
		
		//******************************************************************************
		// Determine which deadlines are due on this frame (may be none).
		
		Size numDeadlines = 0;
		
		for ( Index i = 0; i < fdls.getSize(); i++ )
		{
			FDLState& fdl = *fdls[i];
			
			// Update the number of buffered samples for this FDL.
			fdl.numBufferedSamples += numFrameSamples;
			
			if ( fdl.numBufferedSamples >= fdl.fftSize )
				numDeadlines++;
		}
		
		//******************************************************************************
		// Process this FFT frame.
		
		if ( numDeadlines > 0 )
			processFFTFrame( numDeadlines );
		
		//******************************************************************************
		// Read from the convolution output queue for each cluster.
		
		for ( Index clusterIndex = 0; clusterIndex < numClusterStates; clusterIndex++ )
		{
			ClusterState& clusterState = *clusterStates[clusterIndex];
			
			// Skip unused clusters.
			if ( clusterStates.isUnused(clusterIndex) )
				continue;
			
			// Mix the main convolution output to the output buffer for the cluster.
			ConvolutionState& convolutionState = *convolutionStates[clusterState.convolutionStateIndex];
			SoundBuffer buffer = clusterState.outputBuffer;

			if ( request.flags.isSet( RenderFlags::REVERB ) )
				advanceConvolutionOutput( convolutionState, numSamplesProcessed, numFrameSamples );
			else
				mixConvolutionOutput( convolutionState, clusterState.outputBuffer, numSamplesProcessed, numFrameSamples );
			
			// Mix the HRTF output to the output buffer for the cluster.
			ConvolutionState& hrtfState = *convolutionStates[clusterState.hrtfConvolutionIndex];

			if ( hrtfEnabled )
				mixConvolutionOutput( hrtfState, clusterState.outputBuffer, numSamplesProcessed, numFrameSamples );
			else
				advanceConvolutionOutput( hrtfState, numSamplesProcessed, numFrameSamples );
		}
		
		//******************************************************************************
		
		// Increment the current position in the convolution queue.
		convolutionQueuePosition = (convolutionQueuePosition + numFrameSamples) % convolutionQueueSize;
		numSamplesProcessed += numFrameSamples;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Input Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: writeConvolutionInput( ConvolutionState& convolutionState, const SoundBuffer& inputBuffer,
													Index startOffset, Size numSamples )
{
	Index inputWrite = convolutionQueuePosition;
	
	// Copy the input to the internal input queue, possibly in multiple passes.
	for ( Size samplesRemaining = numSamples; samplesRemaining > 0; )
	{
		// Write until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( samplesRemaining, convolutionQueueSize - inputWrite );
		
		const Float32* source = (const Float32*)inputBuffer.getChannel(0) + startOffset;
		Float32* destination = convolutionState.inputQueue.getChannel( 0, inputWrite );
		
		// Copy the input for the cluster to the input queue.
		om::util::copyPOD( destination, source, blockSize );
		
		// Advance the input write position.
		inputWrite = (inputWrite + blockSize) % convolutionQueueSize;
		samplesRemaining -= blockSize;
	}
}




void SoundListenerRenderer:: advanceConvolutionInput( ConvolutionState& convolutionState, Index startOffset, Size numSamples )
{
	Index inputWrite = convolutionQueuePosition;
	
	// Copy the input to the internal input queue, possibly in multiple passes.
	for ( Size samplesRemaining = numSamples; samplesRemaining > 0; )
	{
		// Write until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( samplesRemaining, convolutionQueueSize - inputWrite );
		Float32* destination = convolutionState.inputQueue.getChannel( 0, inputWrite );
		
		// Copy the input for the cluster to the input queue.
		om::util::zeroPOD( destination, blockSize );
		
		// Advance the input write position.
		inputWrite = (inputWrite + blockSize) % convolutionQueueSize;
		samplesRemaining -= blockSize;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Output Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: mixConvolutionOutput( ConvolutionState& convolutionState, SoundBuffer& outputBuffer,
													Index startOffset, Size numSamples )
{
	const Size numOutputChannels = convolutionState.outputQueue.getChannelCount();
	Index outputRead = convolutionQueuePosition;
	
	// Copy the output from the internal output queue, possibly in multiple passes.
	for ( Size samplesRemaining = numSamples; samplesRemaining > 0; )
	{
		// Read until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( samplesRemaining, convolutionQueueSize - outputRead );
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			const Float32* queue = convolutionState.outputQueue.getChannel( c, outputRead );
			Float32* output = (Float32*)outputBuffer.getChannel(c) + startOffset;
			
			// Add the block from the output queue to the cluster output buffer.
			math::add( output, queue, blockSize );
			
			// Zero the output queue for the next time.
			om::util::zeroPOD( queue, blockSize );
		}
		
		outputRead = (outputRead + blockSize) % convolutionQueueSize;
		samplesRemaining -= blockSize;
	}
}




void SoundListenerRenderer:: advanceConvolutionOutput( ConvolutionState& convolutionState, Index startOffset, Size numSamples )
{
	const Size numOutputChannels = convolutionState.outputQueue.getChannelCount();
	Index outputRead = convolutionQueuePosition;
	
	// Copy the output from the internal output queue, possibly in multiple passes.
	for ( Size samplesRemaining = numSamples; samplesRemaining > 0; )
	{
		// Read until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( samplesRemaining, convolutionQueueSize - outputRead );
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			const Float32* queue = convolutionState.outputQueue.getChannel( c, outputRead );
			
			// Zero the output queue for the next time.
			om::util::zeroPOD( queue, blockSize );
		}
		
		outputRead = (outputRead + blockSize) % convolutionQueueSize;
		samplesRemaining -= blockSize;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		FFT Frame Processing Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: processFFTFrame( Size numDeadlines )
{
	//******************************************************************************
	// Wait for all of the needed FDLs to finish processing.
	
	renderThreadPool.finishJob( numDeadlines - 1 );
	
	//******************************************************************************
	// Write the output FDL data to the output queue.
	
	const Size numConvolutionStates = convolutionStates.getSize();
	
	for ( Index convolutionIndex = 0; convolutionIndex < numConvolutionStates; convolutionIndex++ )
	{
		// Skip unused convolution states.
		if ( convolutionStates.isUnused(convolutionIndex) )
			continue;
		
		ConvolutionState& convolutionState = *convolutionStates[convolutionIndex];
		const Size numStateDeadlines = math::min( numDeadlines, convolutionState.fdls.getSize() );
		const Size numOutputChannels = convolutionState.outputQueue.getChannelCount();
		
		for ( Index i = 0; i < numStateDeadlines; i++ )
		{
			FDL& fdl = *convolutionState.fdls[i];
			const FDLState& fdlState = *fdls[i];
			
			// Compute the FFT magnitude normalization factor.
			const Float normalize = Float(1)/Float(fdlState.paddedFFTSize);
			const Size outputLength = fdlState.fftSize;
			
			if ( !fdl.zeroOutput )
			{
				Index outputWrite = fdlState.outputWritePosition;
				
				// Accumulate the FDL output in the output queue, possibly in several steps.
				for ( Index outputIndex = 0; outputIndex < outputLength; )
				{
					// Write until the end of the buffer or until we run out of samples.
					Size blockSize = math::min( outputLength - outputIndex, convolutionQueueSize - outputWrite );
					
					for ( Index c = 0; c < numOutputChannels; c++ )
					{
						const Float* fdlOutput = fdl.output.getChannel( c, outputIndex );
						Float* queue = convolutionState.outputQueue.getChannel( c, outputWrite );
						
						// Multilply by the normalization factor, then add to the output queue.
						math::multiplyAdd( queue, fdlOutput, normalize, blockSize );
					}
					
					outputWrite = (outputWrite + blockSize) % convolutionQueueSize;
					outputIndex += blockSize;
				}
			}
		}
	}
	
	//******************************************************************************
	// Update the input partition indices for the FDL deadlines that finished.
	
	for ( Index i = 0; i < numDeadlines; i++ )
	{
		FDLState& fdlState = *fdls[i];
		
		// Update the new output write position.
		fdlState.outputWritePosition = (fdlState.outputWritePosition + fdlState.fftSize) % convolutionQueueSize;
		fdlState.inputPartitionIndex = (fdlState.inputPartitionIndex + 1) % fdlState.maxNumPartitions;
	}
	
	//******************************************************************************
	// Copy the next set of input buffers to the FDLs.
	
	for ( Index convolutionIndex = 0; convolutionIndex < numConvolutionStates; convolutionIndex++ )
	{
		// Skip unused convolution states.
		if ( convolutionStates.isUnused(convolutionIndex) )
			continue;
		
		ConvolutionState& convolutionState = *convolutionStates[convolutionIndex];
		const Size numStateDeadlines = math::min( numDeadlines, convolutionState.fdls.getSize() );
		
		for ( Index i = 0; i < numStateDeadlines; i++ )
		{
			const FDLState& fdlState = *fdls[i];
			ComplexSample* const fdlInputStart = convolutionState.inputAccumulator.getChannel( 0, fdlState.paddedOffset );
			Float32* const inputStart = (Float32*)(fdlInputStart + fdlState.inputPartitionIndex*fdlState.paddedFFTStorage);
			const Size inputLength = fdlState.fftSize;
			Index inputRead = fdlState.inputReadPosition;
			
			// Copy the buffered input to the FDL input buffer.
			for ( Index inputIndex = 0; inputIndex < inputLength; )
			{
				// Write until the end of the buffer or until we run out of samples.
				Size blockSize = math::min( inputLength - inputIndex, convolutionQueueSize - inputRead );
				
				const Float32* source = convolutionState.inputQueue.getChannel( 0, inputRead );
				Float32* destination = inputStart + inputIndex;
				
				om::util::copyPOD( destination, source, blockSize );
				
				inputRead = (inputRead + blockSize) % convolutionQueueSize;
				inputIndex += blockSize;
			}
			
			// Set the rest of the buffer padding to zero.
			om::util::zeroPOD( inputStart + fdlState.fftSize, fdlState.fftSize );
		}
	}
	
	//******************************************************************************
	// Update the rendering state for all deadlines that are due.
	
	for ( Index i = 0; i < numDeadlines; i++ )
	{
		FDLState& fdlState = *fdls[i];
		
		// Update the number of previously processed frames in this FDL's long interval.
		fdlState.numPreviousFrames = (fdlState.numPreviousFrames + 1) % fdlState.nextFDLMultiplier;

		// Set the deadline index when this FDL is next due.
		fdlState.deadlineIndex = getNextDeadlineForFDL( i );
		
		// Update the number of buffered input samples.
		fdlState.numBufferedSamples -= fdlState.fftSize;
		
		// Update the new output write position.
		fdlState.inputReadPosition = (fdlState.inputReadPosition + fdlState.fftSize) % convolutionQueueSize;
	}
	
	//******************************************************************************
	// Inform the worker threads to start processing the new input data.
	
	// Queue the rendering jobs in the order that they should execute. (shortest FDL first)
	for ( Index i = 0; i < numDeadlines; i++ )
	{
		const FDLState& fdlState = *fdls[i];
		
		// Give smaller FDLs a higher priority since they are due sooner.
		// This is so that FDLs due sooner can be computed before previously queued larger FDLs.
		const Float priority = Float(1) / Float(i+1);
		
		for ( Index convolutionIndex = 0; convolutionIndex < numConvolutionStates; convolutionIndex++ )
		{
			if ( convolutionStates.isUnused(convolutionIndex) )
				continue;
			
			ConvolutionState& convolutionState = *convolutionStates[convolutionIndex];
			
			if ( convolutionState.fdls.getSize() <= i )
				continue;
			
			renderThreadPool.addJob( FunctionCall<void(const FDLState&, ConvolutionState&, FDL&)>(
												om::bind( &SoundListenerRenderer::renderFDL, this ),
												fdlState, convolutionState, *convolutionState.fdls[i] ),
									fdlState.deadlineIndex, priority );
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Rendering Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: renderFDL( const FDLState& fdlState, ConvolutionState& convolutionState, FDL& fdl )
{
	RenderThreadState& threadState = renderStates[renderThreadPool.getCurrentThreadIndex()];
	SampleBuffer<ComplexSample>& fftBuffer = threadState.fftBuffer;
	
	const Size numOutputChannels = convolutionState.outputQueue.getChannelCount();
	const SampleRate sampleRate = convolutionState.sampleRate;
	const Float frameLength = Float(fdlState.fftSize/sampleRate);
	const Size partitionCount = fdlState.maxNumPartitions;
	const Float interpolationLength = math::max( Float((fdlState.fftSize+1)/sampleRate), convolutionState.interpolationTime );
	
	// Make sure the FFT buffer is big enough.
	if ( fftBuffer.getChannelCount() < numOutputChannels || fftBuffer.getSampleCount() < fdlState.paddedFFTStorage )
		fftBuffer.setFormat( numOutputChannels, fdlState.paddedFFTStorage );
	
	fftBuffer.allocate();
	
	//******************************************************************************
	// Swap to the next IR if there is an input IR.
	
	// Keep track of whether or not the IR was just updated.
	Bool updatedIR = false;
	
	// Is there a new IR for this FDL? Only update if the interpolation is finished.
	if ( fdl.numInputIRs > 0 && fdl.interpolation.isFinished() )
	{
		// Remember that the IR was updated.
		updatedIR = true;
		
		// Setup the interpolation state.
		fdl.interpolation.time = interpolationLength;
		
		if ( fdl.getInputIR().isAllocated() )
		{
			// Fade in the new IR.
			fdl.interpolation.current = Float(0);
			fdl.interpolation.target = Float(1);
		}
		else
		{
			// Fade out the previous IR.
			fdl.interpolation.current = Float(1);
			fdl.interpolation.target = Float(0);
		}
		
		// Move to the next IR.
		fdl.nextInputIR();
		
		// Signal atomically to the update thread that we have consumed the last input IR.
		fdl.numInputIRs--;
	}
	
	//******************************************************************************
	// Convert the input audio to frequency domain.
	
	const Index inputPartitionIndex = fdlState.inputPartitionIndex;
	ComplexSample* const fdlInputStart = convolutionState.inputAccumulator.getChannel( 0, fdlState.paddedOffset );
	ComplexSample* const currentInputStart = fdlInputStart + inputPartitionIndex*fdlState.paddedFFTStorage;
	
	fdlState.fft( (Float32*)currentInputStart );
	
	//******************************************************************************
	// Convolve the audio for each partition with the IR partition for that partition's delay.
	// Accumulate the results to the FDL output buffer.
	
	SampleBuffer<ComplexSample>& mainIR = fdl.getMainIR();
	SampleBuffer<ComplexSample>& targetIR = fdl.getTargetIR();
	SampleBuffer<Float32>& fdlOutput = fdl.output;
	SampleBuffer<Float32>& mainAccumulator = fdl.getMainAccumulator();
	SampleBuffer<Float32>& targetAccumulator = fdl.getTargetAccumulator();
	const Bool needsInterpolation = updatedIR || !fdl.interpolation.isFinished();
	Bool mainOutput = false;
	Bool targetOutput = false;
	
	// Make sure the output accumulators are allocated.
	if ( mainIR.isAllocated() || needsInterpolation && targetIR.isAllocated() )
	{
		targetAccumulator.allocate();
		mainAccumulator.allocate();
		fdlOutput.allocate();
	}
	else
	{
		// Deallocate the accumulators if the IR output is zero.
		mainAccumulator.deallocate();
		targetAccumulator.deallocate();
		fdlOutput.deallocate();
	}
	
	// If the current main IR for this FDL is nonzero, render the convolution.
	if ( mainIR.isAllocated() )
	{
		// Render the main IR output to a temporary buffer.
		renderFDLOutput( fdlState, fdlInputStart, mainIR, fftBuffer,
						numOutputChannels, inputPartitionIndex, partitionCount );
		
		// Convert that output to time domain and accumulate in the output accumulator for this FDL.
		accumulateFDLOutput( fdlState, fftBuffer, mainAccumulator,
							fdl.currentAccumulatorPosition, numOutputChannels );
		mainOutput = true;
	}
	
	//******************************************************************************
	// Interpolate to the target IR if there is one.
	
	if ( needsInterpolation )
	{
		// There is a new IR for this frame, compute the output with both the old and new IRs and interpolate.
		if ( targetIR.isAllocated() )
		{
			// Zero the output accumulator for the new target IR if the IR was just updated.
			if ( updatedIR )
				targetAccumulator.zero();
			
			// Render the target IR output to a temporary buffer.
			renderFDLOutput( fdlState, fdlInputStart, targetIR, fftBuffer,
							numOutputChannels, inputPartitionIndex, partitionCount );
			
			// Convert that output to time domain and accumulate in the output accumulator for this FDL.
			accumulateFDLOutput( fdlState, fftBuffer, targetAccumulator,
								fdl.currentAccumulatorPosition, numOutputChannels );
			
			if ( updatedIR )
			{
				// Since the IR was just updated, we can't output anything from the target IR yet.
				// Just output the last IR for now.
				copyFDLOutput( fdlState, fdlOutput, mainAccumulator, targetAccumulator,
								fdl.currentAccumulatorPosition, numOutputChannels );
			}
			else
			{
				if ( mainOutput )
				{
					// Interpolate between the outputs for the main and target IRs.
					interpolateFDLOutput( fdlState, fdlOutput, mainAccumulator, targetAccumulator,
										fdl.currentAccumulatorPosition, fdl.interpolation, sampleRate, numOutputChannels );
				}
				else
				{
					// Fade in the target IR's output.
					fadeFDLOutput( fdlState, fdlOutput, targetAccumulator, mainAccumulator,
									fdl.currentAccumulatorPosition, fdl.interpolation, sampleRate, numOutputChannels );
				}
				
				// Remember that there was target output on this frame.
				targetOutput = true;
			}
		}
		else
		{
			// Fade the FDL out since the target IR is zero. Don't do anything if the previous IR is zero.
			if ( mainOutput )
			{
				fadeFDLOutput( fdlState, fdlOutput, mainAccumulator, targetAccumulator,
								fdl.currentAccumulatorPosition, fdl.interpolation, sampleRate, numOutputChannels );
			}
		}
		
		// If the interpolation is finishing on this frame, move to the next main IR.
		if ( fdl.interpolation.time <= frameLength )
		{
			fdl.nextMainIR();
			fdl.nextAccumulator();
		}
		
		// Update the interpolation time.
		if ( !updatedIR )
			fdl.interpolation.time = math::max( fdl.interpolation.time - frameLength, Float(0) );
	}
	else
	{
		if ( mainOutput )
		{
			// Copy the accumulated output directly to the output buffer with no interpolation.
			copyFDLOutput( fdlState, fdlOutput, mainAccumulator, targetAccumulator,
							fdl.currentAccumulatorPosition, numOutputChannels );
		}
	}
	
	// Determine whether or not the output of this FDL is zero.
	// This is only when both the main and target outputs are zero.
	fdl.zeroOutput = !mainOutput && !targetOutput;
	
	// Advance the output accumulator position.
	fdl.currentAccumulatorPosition = (fdl.currentAccumulatorPosition + fdlState.fftSize) % fdlState.outputAccumulatorSize;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Render FDL Output Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: renderFDLOutput( const FDLState& fdlState, const ComplexSample* const fdlInputStart,
											const SampleBuffer<ComplexSample>& ir, SampleBuffer<ComplexSample>& output,
											Size numOutputChannels, Index inputPartitionIndex, Size partitionCount )
{
	for ( Index p = 0, inputIndex = inputPartitionIndex; p < partitionCount; p++ )
	{
		Index paddedPartitionOffset = p*fdlState.paddedFFTStorage;
		const ComplexSample* const inputStart = fdlInputStart + inputIndex*fdlState.paddedFFTStorage;
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			const ComplexSample* const channelIR = ir.getChannel( c, paddedPartitionOffset );
			ComplexSample* const channelOutput = output.getChannel(c);
			
			if ( p == 0 )
				math::multiply( channelOutput, inputStart, channelIR, fdlState.complexFFTSize );
			else
				math::multiplyAdd( channelOutput, inputStart, channelIR, fdlState.complexFFTSize );
		}
		
		// Go to the next input partition for the next ir partition.
		inputIndex = ((inputIndex + partitionCount) - 1) % partitionCount;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Accumulate FDL Output Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: accumulateFDLOutput( const FDLState& fdlState, SampleBuffer<ComplexSample>& output,
												SampleBuffer<Float32>& accumulator, Index currentPosition,
												Size numOutputChannels )
{
	// Convert the output to time domain in place.
	for ( Index c = 0; c < numOutputChannels; c++ )
		fdlState.ifft( (Float32*)output.getChannel(c) );
	
	//*****************************************************************************
	// Write the output to the accumulator.
	
	const Size outputLength = fdlState.paddedFFTSize;
	
	for ( Index outputIndex = 0, outputWrite = currentPosition; outputIndex < outputLength; )
	{
		// Write until the end of the buffer or until we run out of samples.
		Size blockSize = math::min( outputLength - outputIndex, fdlState.outputAccumulatorSize - outputWrite );
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			const Float32* fdlOutput = (Float32*)output.getChannel(c) + outputIndex;
			Float32* queue = accumulator.getChannel( c, outputWrite );
			const Float32* const queueEnd = queue + blockSize;
			
			while ( queue != queueEnd )
			{
				*queue += *fdlOutput;
				queue++;
				fdlOutput++;
			}
		}
		
		outputWrite = (outputWrite + blockSize) % fdlState.outputAccumulatorSize;
		outputIndex += blockSize;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		FDL Output Interpolation Methods
//############		
//##########################################################################################
//##########################################################################################





void SoundListenerRenderer:: interpolateFDLOutput( const FDLState& fdlState, SampleBuffer<Float32>& output,
													SampleBuffer<Float32>& mainAccumulator, SampleBuffer<Float32>& targetAccumulator,
													Index currentAccumulatorPosition, LerpState<Float32>& interpolation,
													SampleRate sampleRate, Size numOutputChannels )
{
	const Size outputLength = fdlState.fftSize;
	const Float interpolationTime = Float(outputLength/sampleRate);
	const Float interpolationFraction = math::min( interpolationTime / interpolation.time, Float(1) );
	const Float stepSize = interpolationFraction*(interpolation.target - interpolation.current) / Float(outputLength);
	
	for ( Index readIndex = currentAccumulatorPosition, outputIndex = 0; outputIndex < outputLength; )
	{
		// Read until the end of the output, or until the end of the accumulator.
		const Size blockSize = math::min( outputLength - outputIndex, fdlState.outputAccumulatorSize - readIndex );
		Float a;
		
		// Interpolate each channel for the block size.
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			Float* outputChannel = output.getChannel( c, outputIndex );
			const Float* const outputChannelEnd = outputChannel + blockSize;
			Float* mainChannel = mainAccumulator.getChannel( c, readIndex );
			Float* targetChannel = targetAccumulator.getChannel( c, readIndex );
			
			a = interpolation.current;
			
			// Interpolate the output accumulators.
			while ( outputChannel != outputChannelEnd )
			{
				*outputChannel = ((*targetChannel) - (*mainChannel))*a + (*mainChannel);
				*targetChannel = 0;
				*mainChannel = 0;
				outputChannel++;
				targetChannel++;
				mainChannel++;
				a += stepSize;
			}
		}
		
		interpolation.current = a;
		
		readIndex = (readIndex + blockSize) % fdlState.outputAccumulatorSize;
		outputIndex += blockSize;
	}
}




void SoundListenerRenderer:: fadeFDLOutput( const FDLState& fdlState, SampleBuffer<Float32>& output,
											SampleBuffer<Float32>& mainAccumulator, SampleBuffer<Float32>& targetAccumulator,
											Index currentAccumulatorPosition, LerpState<Float32>& interpolation,
											SampleRate sampleRate, Size numOutputChannels )
{
	const Size outputLength = fdlState.fftSize;
	const Float interpolationTime = Float(outputLength/sampleRate);
	const Float interpolationFraction = math::min( interpolationTime / interpolation.time, Float(1) );
	const Float stepSize = interpolationFraction*(interpolation.target - interpolation.current) / Float(outputLength);
	
	for ( Index readIndex = currentAccumulatorPosition, outputIndex = 0; outputIndex < outputLength; )
	{
		// Read until the end of the output, or until the end of the accumulator.
		const Size blockSize = math::min( outputLength - outputIndex, fdlState.outputAccumulatorSize - readIndex );
		Float a;
		
		// Interpolate each channel for the block size.
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			Float* outputChannel = (Float*)output.getChannel( c, outputIndex );
			const Float* const outputChannelEnd = outputChannel + blockSize;
			Float* mainChannel = (Float*)mainAccumulator.getChannel( c, readIndex );
			Float* targetChannel = (Float*)targetAccumulator.getChannel( c, readIndex );
			
			a = interpolation.current;
			
			// Interpolate the output accumulators.
			while ( outputChannel != outputChannelEnd )
			{
				*outputChannel = (*mainChannel)*a;
				*targetChannel = 0;
				*mainChannel = 0;
				outputChannel++;
				targetChannel++;
				mainChannel++;
				a += stepSize;
			}
		}
		
		interpolation.current = a;
		
		readIndex = (readIndex + blockSize) % fdlState.outputAccumulatorSize;
		outputIndex += blockSize;
	}
}




void SoundListenerRenderer:: copyFDLOutput( const FDLState& fdlState, SampleBuffer<Float32>& output,
									SampleBuffer<Float32>& mainAccumulator, SampleBuffer<Float32>& targetAccumulator,
									Index currentAccumulatorPosition, Size numOutputChannels )
{
	const Size outputLength = fdlState.fftSize;
	
	for ( Index readIndex = currentAccumulatorPosition, outputIndex = 0; outputIndex < outputLength; )
	{
		// Read until the end of the output, or until the end of the accumulator.
		const Size blockSize = math::min( outputLength - outputIndex, fdlState.outputAccumulatorSize - readIndex );
		
		// Interpolate each channel for the block size.
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			Float* outputChannel = (Float*)output.getChannel( c, outputIndex );
			Float* mainChannel = (Float*)mainAccumulator.getChannel( c, readIndex );
			Float* targetChannel = (Float*)targetAccumulator.getChannel( c, readIndex );
			
			om::util::copyPOD( outputChannel, mainChannel, blockSize );
			
			// Zero the accumulators for the next pass.
			om::util::zeroPOD( mainChannel, blockSize );
			om::util::zeroPOD( targetChannel, blockSize );
		}
		
		readIndex = (readIndex + blockSize) % fdlState.outputAccumulatorSize;
		outputIndex += blockSize;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Next FDL Deadline Method
//############		
//##########################################################################################
//##########################################################################################




Index SoundListenerRenderer:: getNextDeadlineForFDL( Index fdlIndex )
{
	Index maxDeadlineIndex = fdlIndex;
	const Size numFDLsMinusOne = fdls.getSize() - 1;
	
	// Determine the deadline index for when this FDL is next due.
	while ( maxDeadlineIndex < numFDLsMinusOne )
	{
		FDLState& fdl = *fdls[maxDeadlineIndex];
		
		// If the next FDL frame will complete the frame of the next largest FDL, increase the counter.
		if ( fdl.numPreviousFrames + 1 >= fdl.nextFDLMultiplier )
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




Size SoundListenerRenderer:: initializeFDLs( Size maxIRLength )
{
	// Determine the length of the first FDL based on the required latency.
	const Size maxFirstFDLLength = Size((1.0f/3.0f)*request.maxLatency*request.sampleRate);
	Size fftSize = minFDLSize = (math::isPowerOfTwo(maxFirstFDLLength) ? maxFirstFDLLength : math::nextPowerOfTwo(maxFirstFDLLength)) / 2;
	
	// Determine how large the I/O queues should be.
	convolutionQueueSize = 3*maxFDLSize;
	
	// Reset the current read/write pointer for the input and output queues.
	convolutionQueuePosition = 0;
	
	// Initialize the FDLs.
	Index offset = 0;
	Index paddedOffset = 0;
	Index fdlIndex = 0;
	
	while ( offset < maxIRLength && fdls.getSize() < maxFDLCount && fftSize <= maxFDLSize )
	{
		FDLState* fdlState;
		
		// Add a new FDL if this IR is longer than any before.
		if ( fdlIndex < fdls.getSize() )
			fdlState = fdls[fdlIndex];
		else
		{
			// Add a new FDL state.
			fdlState = om::util::construct<FDLState>();
			fdls.add( fdlState );
			
			// Set the FDL parameters.
			fdlState->fftSize = fftSize;
			fdlState->paddedFFTSize = Size(2)*fftSize;
			fdlState->complexFFTSize = fftSize + 1;
			fdlState->paddedFFTStorage = math::nextMultiple( fdlState->complexFFTSize, Size(2) );
			fdlState->offset = offset;
			fdlState->paddedOffset = paddedOffset;
			fdlState->inputPartitionIndex = 0;
			fdlState->maxNumPartitions = partitionsPerFDL;
			fdlState->nextFDLMultiplier = DEFAULT_FDL_MULTIPLIER;
			fdlState->outputAccumulatorSize = 2*fdlState->paddedFFTSize;
			
			// Compute the offset for the FDL in the input and output queues.
			Size startOffset = (offset + 2*minFDLSize) % fftSize;
			fdlState->inputReadPosition = ((startOffset + minFDLSize + convolutionQueueSize) - fftSize) % convolutionQueueSize;
			
			if ( fdlIndex == 0 )
				fdlState->outputWritePosition = 2*minFDLSize;
			else
				fdlState->outputWritePosition = 2*minFDLSize + offset - fftSize;
			
			// Initialize the FFT plans.
			fdlState->fftPlan = fftwf_plan_dft_r2c_1d( (int)fdlState->paddedFFTSize, NULL, NULL, FFTW_ESTIMATE | FFTW_DESTROY_INPUT );
			fdlState->ifftPlan = fftwf_plan_dft_c2r_1d( (int)fdlState->paddedFFTSize, NULL, NULL, FFTW_ESTIMATE | FFTW_DESTROY_INPUT );
		}
		
		// Move to the next FDL.
		offset += fdlState->maxNumPartitions*fftSize;
		paddedOffset += fdlState->maxNumPartitions*fdlState->paddedFFTStorage;
		fdlIndex++;
		fftSize *= DEFAULT_FDL_MULTIPLIER;
	}
	
	return fdlIndex;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster Creation Method
//############		
//##########################################################################################
//##########################################################################################




SoundListenerRenderer::ClusterState* SoundListenerRenderer:: createCluster()
{
	Index clusterIndex = clusterStates.construct();
	ClusterState* clusterState = clusterStates[clusterIndex];
	
	// Set the gain interpolation parameters.
	clusterState->gain.current = Gain(1);
	clusterState->gain.target = Gain(1);
	clusterState->gain.time = 0;
	
	// Allocate a convolution state for the cluster's main IR and one for the HRTF.
	clusterState->convolutionStateIndex = createConvolution( request.maxIRLength, request.sampleRate, request.irFadeTime );
	clusterState->hrtfConvolutionIndex = createConvolution( request.maxIRLength, request.sampleRate, request.hrtfFadeTime );
	
	//******************************************************************************
	// Initialize the delay buffer for path rendering.
	
	// Get the path renderer.
	PathRenderState& pathRenderer = clusterState->pathRenderer;
	
	// Compute the actual delay buffer size where each sample represents a single interleaved frequency band.
	pathRenderer.delayBufferSize = Size(2)*Size(request.maxPathDelay*request.sampleRate);
	pathRenderer.paddedDelayBufferSize = pathRenderer.delayBufferSize*SIMDBands::getWidth();
	
	if ( pathRenderer.delayBuffer.getChannelCount() != Size(1) ||
		pathRenderer.delayBuffer.getSampleCount() != pathRenderer.paddedDelayBufferSize )
	{
		pathRenderer.delayBuffer.setFormat( Size(1), pathRenderer.paddedDelayBufferSize );
	}
	
	// Make sure the delay buffer is allocated.
	pathRenderer.delayBuffer.allocate();
	
	// Zero the path rendering delay buffer.
	pathRenderer.delayBuffer.zero();
	
	// Make sure the crossover history is zero.
	pathRenderer.crossoverHistory->reset();
	
	
	//******************************************************************************
	
	return clusterState;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Cluster Destruction Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: destroyCluster( Index clusterIndex )
{
	ClusterState& clusterState = *clusterStates[clusterIndex];
	
	// Deallocate the memory used by the cluster state.
	clusterState.deallocate();
	
	// Destroy the convolution states for the cluster.
	destroyConvolution( clusterState.convolutionStateIndex );
	destroyConvolution( clusterState.hrtfConvolutionIndex );
	
	// Release the cluster state object.
	clusterStates.release( clusterIndex );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Creation Method
//############		
//##########################################################################################
//##########################################################################################




Index SoundListenerRenderer:: createConvolution( Float maxIRLength, SampleRate sampleRate, Float interpolationTime )
{
	const Index convolutionStateIndex = convolutionStates.construct();
	ConvolutionState& convolutionState = *convolutionStates[convolutionStateIndex];
	
	const Size numOutputChannels = request.channelLayout.getChannelCount();
	
	// Initialize the FDLs based on the max IR length.
	const Size numFDLs = fdls.getSize();//initializeFDLs( maxIRLength*sampleRate );
	const FDLState& lastFDL = *fdls[numFDLs-1];
	
	convolutionState.maxIRLengthInSamples = lastFDL.offset + lastFDL.maxNumPartitions*lastFDL.fftSize;
	convolutionState.sampleRate = sampleRate;
	convolutionState.interpolationTime = interpolationTime;
	
	//******************************************************************************
	// Initialize buffers.
	
	// Make sure the input accumulator can handle the max IR length.
	const Size accumulatorLength = lastFDL.paddedOffset + lastFDL.maxNumPartitions*lastFDL.paddedFFTStorage;
	
	// Make sure that the input and output queues are big enough to hold an FFT frame's
	// worth of samples and have the right format.
	convolutionState.inputQueue.setFormat( 1, convolutionQueueSize );
	convolutionState.inputQueue.allocate();
	convolutionState.inputQueue.zero();
	
	convolutionState.outputQueue.setFormat( numOutputChannels, convolutionQueueSize );
	convolutionState.outputQueue.allocate();
	convolutionState.outputQueue.zero();
	
	convolutionState.inputAccumulator.setFormat( 1, accumulatorLength );
	convolutionState.inputAccumulator.allocate();
	convolutionState.inputAccumulator.zero();
	
	//******************************************************************************
	// Initialize the FDLs for the convolution state
	
	for ( Index fdlIndex = 0; fdlIndex < numFDLs; fdlIndex++ )
	{
		const FDLState& fdlState = *fdls[fdlIndex];
		FDL* fdl;
		
		// Create a new FDL for this convolution state if necessary.
		if ( convolutionState.fdls.getSize() <= fdlIndex )
		{
			fdl = om::util::construct<FDL>( numOutputChannels, fdlState.paddedFFTSize );
			convolutionState.fdls.add( fdl );
		}
		else
			fdl = convolutionState.fdls[fdlIndex];
		
		// Initialize the FDL.
		const Size fdlLength = fdlState.maxNumPartitions*fdlState.paddedFFTStorage;
		
		// Initialize the IRs for the FDL.
		for ( Index i = 0; i < 3; i++ )
			fdl->irs[i].setFormat( numOutputChannels, fdlLength );
		
		// Initialize the output accumulators.
		fdl->currentAccumulatorPosition = 0;
		
		for ( Index i = 0; i < 2; i++ )
			fdl->outputAccumulator[i].setFormat( numOutputChannels, fdlState.outputAccumulatorSize );
		
		// Initialize the output buffer.
		fdl->output.setFormat( numOutputChannels, fdlState.fftSize );
		fdl->output.allocate();
		fdl->output.zero();
	}
	
	//******************************************************************************
	
	return convolutionStateIndex;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Convolution Destruction Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: destroyConvolution( Index convolutionIndex )
{
	ConvolutionState& convolutionState = *convolutionStates[convolutionIndex];
	
	// Deallocate the memory used by the convolution state.
	convolutionState.deallocate();
	
	// Deallocate the convolution states for this cluster.
	convolutionStates.release( convolutionIndex );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Path Sorting Method
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: sortPathsDecreasing( ArrayList<PathSortID>& paths )
{
	std::sort( paths.getPointer(), paths.getPointer() + paths.getSize() );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Delay Interpolation Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: interpolateDelayEqual( Float32* output, const Float32* const outputEnd,
												const Float32* delayBufferStart,
												Float currentDelay,
												SIMDBands& currentGain, const SIMDBands& gainChangePerSample )
{
	Index delayOffset = Index(currentDelay)*SIMDBands::getWidth();
	const Float32* lastDelay = delayBufferStart + delayOffset;
	const Float32* nextDelay = delayBufferStart + delayOffset + SIMDBands::getWidth();
	const SIMDBands currentDelaySIMD = currentDelay;
	const SIMDBands aSIMD = currentDelaySIMD - math::floor( currentDelaySIMD );
	
	while ( output != outputEnd )
	{
		SIMDBands last = SIMDBands::load(lastDelay);
		*output += math::sumScalar( ((SIMDBands::load(nextDelay) - last)*aSIMD + last)*currentGain );
		
		output++;
		lastDelay += SIMDBands::getWidth();
		nextDelay += SIMDBands::getWidth();
		currentGain += gainChangePerSample;
	}
}




void SoundListenerRenderer:: interpolateDelayChanges( Float32* output, const Float32* const outputEnd,
												const Float32* const delayBufferStart,
												Float currentDelay, const Float delayChangePerSample,
												SIMDBands& currentGain, const SIMDBands& gainChangePerSample )
{
	const Float32* delay = delayBufferStart + Index(currentDelay)*SIMDBands::getWidth();
	const Float32* lastDelay = delay;
	delay += SIMDBands::getWidth();
	
	SIMDBands simdDelay = currentDelay;
	SIMDBands aSIMD = simdDelay - math::floor(simdDelay);
	const Float d = delayChangePerSample;
	const SIMDBands dSIMD = d;
	
	SIMDBands currentDelaySIMD = currentDelay;
	
	while ( output != outputEnd )
	{
		SIMDBands last = SIMDBands::load(lastDelay);
		*output += math::sumScalar( ((SIMDBands::load(delay) - last)*aSIMD + last)*currentGain );
		
		output++;
		aSIMD += dSIMD;
		currentGain += gainChangePerSample;
		
		while ( aSIMD[0] > 1.0f )
		{
			aSIMD -= 1.0f;
			lastDelay = delay;
			delay += SIMDBands::getWidth();
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Gain Interpolation Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundListenerRenderer:: interpolateGainSet( const Sample32f*& input, const Sample32f* const inputEnd, Sample32f*& output,
											Gain& gain, Gain gainChangePerSample )
{
	while ( input != inputEnd )
	{
		*output = (*input)*gain;
		gain += gainChangePerSample;
		input++;
		output++;
	}
}




void SoundListenerRenderer:: interpolateGainAdd( const Sample32f*& input, const Sample32f* const inputEnd, Sample32f*& output,
											Gain& gain, Gain gainChangePerSample )
{
	while ( input != inputEnd )
	{
		*output += (*input)*gain;
		gain += gainChangePerSample;
		input++;
		output++;
	}
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
