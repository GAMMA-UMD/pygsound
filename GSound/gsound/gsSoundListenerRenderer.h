/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundListenerRenderer.h
 * Contents:    gsound::SoundListenerRenderer class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_LISTENER_RENDERER_H
#define INCLUDE_GSOUND_SOUND_LISTENER_RENDERER_H


#include "gsConfig.h"


#include "gsSoundListenerIR.h"
#include "gsRenderRequest.h"
#include "gsSourceSoundBuffer.h"
#include "internal/gsPanLookupTable.h"
#include "internal/gsSIMDCrossover.h"
#include "internal/gsHRTFFilter.h"
#include "internal/gsSampleBuffer.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles rendering the audio for a single sound listener and many sources.
/**
  * This is the main sound propagation rendering class. It manages the convolution
  * and interpolation of the impulse responses from all of the sound sources
  * in a scene. The listener takes in the sound input streams from all of the
  * sources, and then convolves their input audio with the impulse responses
  * produced by propagation. The renderer also handles the rendering of sound source
  * clusters. All sound source audio is downmixed to mono and
  * converted to the output sample rate before convolution, and the output stream
  * for the renderer has the sample rate and channel layout provided by a RenderRequest.
  *
  * The impulse response for the listener renderer can be updated at any rate,
  * and the rate of interpolation is controlled by paramters in the render request
  * given to the renderer. Multithreading can be used to improve the speed at
  * which IRs can be updated.
  *
  * The listener renderer is heavyweight object, there should usually only be one
  * or two of these objects around at once. Each listener renderer creates
  * and manages multiple threads (as many as 12-16, depending on parameters).
  * The large number of threads is necessary to acheive performance and
  * latency requirements for the convolution of many sources.
  */
class SoundListenerRenderer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default sound listener renderer that has no sources to render.
			/**
			  * This renderer uses the default rendering parameters.
			  */
			SoundListenerRenderer();
			
			
			/// Create a new default sound listener renderer that has no sources to render.
			/**
			  * This renderer uses the specified rendering parameters to initialize
			  * the rendering.
			  */
			SoundListenerRenderer( const RenderRequest& request );
			
			
			/// Create a new sound listener renderer which has the same state as another.
			private:SoundListenerRenderer( const SoundListenerRenderer& other );
			public:
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this listener renderer and release all of its internal state.
			virtual ~SoundListenerRenderer();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the entire state of another listener renderer to this one.
			private:SoundListenerRenderer& operator = ( const SoundListenerRenderer& other );
			public:
			
			
		//********************************************************************************
		//******	Request Accessor Methods
			
			
			/// Return a reference to the current render request for this listener.
			GSOUND_INLINE const RenderRequest& getRequest() const
			{
				return request;
			}
			
			
			/// Set the render request that this listener renderer should use to render audio.
			/**
			  * The request contains all of the parameters for the renderer. This
			  * is a potentially expensive operation.
			  */
			void setRequest( const RenderRequest& newRequest );
			
			
		//********************************************************************************
		//******	IR Accessor Methods
			
			
			/// Update the impulse responses for all sources in the specified listener IR.
			/**
			  * The method returns whether or not the IRs for the sources in the listener IR
			  * were able to be updated. The method can fail if the sample rates in the IR and
			  * request don't match.
			  *
			  * If the rendering format in the request differs from the current
			  * rendering output format (sample rate, channel layout), the rendering stream
			  * is restarted with the new format.
			  *
			  * This method uses as many update threads as specified in the request
			  * to update the IRs for the sources in the listener IR.
			  */
			Bool updateIR( const SoundListenerIR& listenerIR, const RenderRequest& request );
			
			
			/// Clear the impulse responses for all sources and remove them from the renderer.
			void clearIR();
			
			
		//********************************************************************************
		//******	Rendering Method
			
			
			/// Render the sound from the given source input buffers for this listener into the output buffer.
			/**
			  * The specified output length of sound is rendered for this listener using
			  * the given source input sound data as input. The number of samples written to the
			  * output buffer is returned.
			  */
			Size render( const SourceSoundBuffer& sourceInputBuffers, SoundBuffer& outputBuffer, const Time& outputLength );
			
			
		//********************************************************************************
		//******	Output Channel Layout Accessor Methods
			
			
			/// Return the current number of output channels for this renderer.
			GSOUND_INLINE Size getChannelCount() const
			{
				return request.channelLayout.getChannelCount();
			}
			
			
			/// Return a reference to an object which describes the current output channel format for this renderer.
			GSOUND_INLINE const ChannelLayout& getChannelLayout() const
			{
				return request.channelLayout;
			}
			
			
		//********************************************************************************
		//******	Output Sample Rate Accessor Methods
			
			
			/// Return the sample rate at which this renderer is rendering.
			/**
			  * Sound source input audio is automatically converted to this sample rate
			  * if it does not match the output sample rate.
			  */
			GSOUND_INLINE SampleRate getSampleRate() const
			{
				return request.sampleRate;
			}
			
			
		//********************************************************************************
		//******	Size in Bytes Accessor Method
			
			
			/// Return the approximate total size in bytes of memory occupied by this listener renderer.
			/**
			  * This function requires acquiring a mutex, and waiting may cause artifacts
			  * in the output audio stream in some cases when the system is under load.
			  * It is advisable to call this function infrequently during a simulation.
			  *
			  * A better way to get this info is to use a SoundStatistics object to hold
			  * the info, passed into updateIR() with the request object.
			  */
			Size getSizeInBytes() const;
			
			
	private:
		
		//********************************************************************************
		//******	Private Type Declarations
			
			
			/// Define the type of SIMD crossover to use for frequency band filtering.
			typedef internal::SIMDCrossover<Float32,GSOUND_FREQUENCY_COUNT> CrossoverType;
			
			
			/// A class that stores data needed for rendering convolution for a single sound source.
			class ConvolutionState;
			
			
			/// A class representing a Frequency-domain Delay Line for a particular FFT window size.
			class FDL;
			
			
			/// A class holding the master state for an FDL.
			class FDLState;
			
			
			/// An object that stores a pointer to a propagation path and a value to sort it by.
			class PathSortID;
			
			
			/// A class that stores the interpolation state for frequency bands.
			class SIMDBandLerpState;
			
			
			/// A class that stores data needed for rendering a single interpolated path for a sound source.
			class PathState;
			
			
			/// A class that stores data needed for rendering interpolated paths for a single sound source.
			class PathRenderState;
			
			
			/// A class that stores the relationship between a source and cluster rendering state.
			class ClusteredSourceState;
			
			
			/// A class that contains information about the current rendering state of a single propagation path.
			class SourceState;
			
			
			/// A class that contains information about the current rendering state of a single propagation path.
			class ClusterState;
			
			
			/// An object that holds local working data for an update worker thread.
			class UpdateThreadState;
			
			
			/// An object that holds local working data for a rendering worker thread.
			class RenderThreadState
			{
				public:
					
					/// A temporary array of channel gain coefficients used in impulse panning.
					Array<Gain> channelGains;
					
					/// A temporary buffer used to hold an output frame for the FDL, prior to output accumulation.
					internal::SampleBuffer<ComplexSample> fftBuffer;
					
			};
			
			
			/// A class that manages a pool of objects of the templated type.
			template < typename T >
			class ObjectPool
			{
				public:
					
					GSOUND_INLINE ObjectPool()
						:	objects( 100 )
					{
					}
					
					/// Destroy an object pool, releasing all internally allocated objects.
					GSOUND_INLINE ~ObjectPool()
					{
						const Size numObjects = objects.getSize();
						
						for ( Index i = 0; i < numObjects; i++ )
							util::destruct( objects[i].object );
					}
					
					
					/// Return the size in bytes of the memory used by this object pool, not including the objects themselves.
					GSOUND_INLINE Size getSizeInBytes() const
					{
						Size totalSize = sizeof(ObjectPool<T>);
						totalSize += objects.getCapacity()*sizeof(Entry);
						totalSize += freeList.getCapacity()*sizeof(Index);
						
						return totalSize;
					}
					
					
					/// Return the total size of this object pool, including unused objects.
					GSOUND_INLINE Size getSize() const
					{
						return objects.getSize();
					}
					
					
					/// Return whether or not the object with the given index is currently not used.
					GSOUND_INLINE Bool isUnused( Index objectIndex ) const
					{
						return objects[objectIndex].referenceCount == 0;
					}
					
					
					/// Return a pointer to the object in the pool at the given index.
					GSOUND_INLINE T* operator [] ( Index objectIndex )
					{
						return objects[objectIndex].object;
					}
					
					
					/// Return a const pointer to the object in the pool at the given index.
					GSOUND_INLINE const T* operator [] ( Index objectIndex ) const
					{
						return objects[objectIndex].object;
					}
					
					
					/// Create a new object in the pool (or use a previously released object). The object index is returned.
					GSOUND_INLINE Index construct( const T& prototype )
					{
						if ( freeList.getSize() > 0 )
						{
							Index freeIndex = freeList.getLast();
							freeList.removeLast();
							
							objects[freeIndex].referenceCount = 1;
							return freeIndex;
						}
						else
						{
							Index objectIndex = objects.getSize();
							objects.add( Entry( util::construct<T>( prototype ) ) );
							return objectIndex;
						}
					}
					
					
					/// Create a new object in the pool (or use a previously released object). The object index is returned.
					GSOUND_INLINE Index construct()
					{
						if ( freeList.getSize() > 0 )
						{
							Index freeIndex = freeList.getLast();
							freeList.removeLast();
							
							objects[freeIndex].referenceCount = 1;
							return freeIndex;
						}
						else
						{
							Index objectIndex = objects.getSize();
							objects.add( Entry( util::construct<T>() ) );
							return objectIndex;
						}
					}
					
					
					
					GSOUND_INLINE void release( Index objectIndex )
					{
						Entry& entry = objects[objectIndex];
						
						// Add this unused object's index to the free list if it is no longer referenced.
						if ( --entry.referenceCount == 0 )
							freeList.add( objectIndex );
					}
					
					
					GSOUND_INLINE void clear()
					{
						const Size numObjects = objects.getSize();
						
						for ( Index i = 0; i < numObjects; i++ )
							util::destruct( objects[i].object );
						
						objects.clear();
						freeList.clear();
					}
					
					
				private:
					
					/// A class that stores an entry for an object pool.
					class Entry
					{
						public:
							
							GSOUND_INLINE Entry( T* newObject )
								:	object( newObject ),
									referenceCount( 1 )
							{
							}
							
							
							T* object;
							
							Atomic<Size> referenceCount;
							
					};
					
					
					/// A list of the objects in the pool.
					ArrayList<Entry> objects;
					
					
					/// A list of the objects in the pool that are currently unused (but constructed).
					ArrayList<Index> freeList;
					
			};
			
			
		//********************************************************************************
		//******	Main Rendering Methods
			
			
			// Prepare the input audio for each sound source and store it in a buffer.
			void bufferSourceInput( const SourceSoundBuffer& sourceInput, Size numSamples );
			
			
			void mixClusterInput( Size numSamples );
			
			
			void mixClusterOutput( SoundBuffer& outputBuffer, Size numSamples );
			
			
			void renderReverb( Size numSamples );
			
			
			/// Return the total size in bytes of the memory allocated by this listener renderer.
			GSOUND_FORCE_INLINE Size getSizeInBytesInternal() const;
			
			
		//********************************************************************************
		//******	Convolution Rendering Methods
			
			
			/// Copy the given number of samples from the input buffer to the input of the convolution state.
			GSOUND_FORCE_INLINE void advanceConvolutionInput( ConvolutionState& convolutionState,
															Index startOffset, Size numSamples );
			
			
			/// Copy the given number of samples from the input buffer to the input of the convolution state.
			GSOUND_FORCE_INLINE void writeConvolutionInput( ConvolutionState& convolutionState,
															const SoundBuffer& inputBuffer,
															Index startOffset, Size numSamples );
			
			
			/// Mix output samples from the given convolution state into an output buffer.
			GSOUND_FORCE_INLINE void mixConvolutionOutput( ConvolutionState& convolutionState,
															SoundBuffer& outputBuffer,
															Index startOffset, Size numSamples );




			GSOUND_FORCE_INLINE void advanceConvolutionOutput( ConvolutionState& convolutionState, Index startOffset, Size numSamples );
			
			
			void renderConvolution( Size numSamples );
			
			
			void processFFTFrame( Size numDeadlines );
			
			
			
			/// Render an FDL for the given convolution state.
			void renderFDL( const FDLState& fdlState, ConvolutionState& convolutionState, FDL& fdl );
			
			
			
			GSOUND_FORCE_INLINE static void renderFDLOutput( const FDLState& fdlState, const ComplexSample* const fdlInputStart,
															const internal::SampleBuffer<ComplexSample>& ir, internal::SampleBuffer<ComplexSample>& output,
															Size numOutputChannels, Index inputPartitionIndex, Size partitionCount );
			
			
			GSOUND_FORCE_INLINE static void accumulateFDLOutput( const FDLState& fdlState, internal::SampleBuffer<ComplexSample>& output,
																internal::SampleBuffer<Float32>& accumulator, Index currentAccumulatorPosition,
																Size numOutputChannels );
			
			
			GSOUND_FORCE_INLINE static void interpolateFDLOutput( const FDLState& fdlState, internal::SampleBuffer<Float32>& output,
															internal::SampleBuffer<Float32>& mainAccumulator, internal::SampleBuffer<Float32>& targetAccumulator,
															Index currentAccumulatorPosition, LerpState<Float32>& interpolation,
															SampleRate sampleRate, Size numOutputChannels );
			
			
			GSOUND_FORCE_INLINE static void fadeFDLOutput( const FDLState& fdlState, internal::SampleBuffer<Float32>& output,
													internal::SampleBuffer<Float32>& mainAccumulator, internal::SampleBuffer<Float32>& targetAccumulator,
													Index currentAccumulatorPosition, LerpState<Float32>& interpolation,
													SampleRate sampleRate, Size numOutputChannels );
			
			
			GSOUND_FORCE_INLINE static void copyFDLOutput( const FDLState& fdlState, internal::SampleBuffer<Float32>& output,
													internal::SampleBuffer<Float32>& mainAccumulator, internal::SampleBuffer<Float32>& targetAccumulator,
													Index currentAccumulatorPosition, Size numOutputChannels );
			
			
			Index getNextDeadlineForFDL( Index fdlIndex );
			
			
		//********************************************************************************
		//******	Path Rendering Methods
			
			
			/// Render the paths for all clusters in this listener renderer for the specified number of samples.
			void renderPaths( Size numSamples );
			
			
			/// Render the paths for the given path renderer using the given input and output buffers and number of samples.
			void renderPathState( PathRenderState& renderer, const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer,
								SoundBuffer& hrtfBuffer, Size numSamples );
			
			
		//********************************************************************************
		//******	IR Update Methods
			
			
			/// Update the rendering request that this listener renderer is using.
			void updateRequest( const RenderRequest& newRequest );
			
			
			/// Initialize the FDLs and FDL threads for this renderer for the specified max IR length.
			Size initializeFDLs( Size maxIRLength );
			
			
			/// Allocate a new cluster and return a pointer to the cluster.
			ClusterState* createCluster();
			
			
			/// Deallocate the resources used by the cluster with the given index.
			void destroyCluster( Index clusterIndex );
			
			
			/// Create a new convolution instance and return its index.
			Index createConvolution( Float maxIRLength, SampleRate sampleRate, Float interpolationTime );
			
			
			/// Destroy the convolution state with the given index.
			void destroyConvolution( Index convolutionIndex );
			
			
			
			void updateClusterIR( ClusterState& clusterState, const SoundSourceIR& ir,
								const SoundListener& listener, const FrequencyBands& frequencies );
			
			
			void updatePathIR( PathRenderState& renderer, RenderThreadState& threadState );
			
			
			void updateHRTF( ConvolutionState& convolutionState, const SoundSourceIR& sourceIR,
							const SoundListener& listener, UpdateThreadState& threadState );
			
			
			void updateConvolutionIR( ConvolutionState& convolutionState, const SoundSourceIR& sourceIR,
										const SoundListener& listener, const FrequencyBands& frequencies,
										UpdateThreadState& threadState );
			
			
			void updatePartitionIR( const SoundListener& listener, const SampledIR& ir, const PathSortID* extraPaths, Size numExtraPaths,
									Index paddedPartitionOffset, Index partitionOffset, Size partitionLength,
									const FDLState& fdlState, internal::SampleBuffer<ComplexSample>& fdlIR,
									CrossoverType::History* histories, UpdateThreadState& threadState );
			
			
			GSOUND_FORCE_INLINE static void interleaveBands( const SampledIR& ir, Index partitionOffset, Size partitionLength,
															const Float* pan, Float* partition );
			
			
			/// Sort the specified list of paths in decreasing order.
			GSOUND_FORCE_INLINE static void sortPathsDecreasing( ArrayList<PathSortID>& paths );
			
			
		//********************************************************************************
		//******	Rendering Helper Methods
			
			
			GSOUND_FORCE_INLINE static void interpolateDelayEqual( Float32* output, const Float32* const outputEnd,
										const Float32* delayBufferStart,
										Float currentDelay,
										SIMDBands& currentGain, const SIMDBands& gainChangePerSample );
			

			
			
			GSOUND_FORCE_INLINE static void interpolateDelayChanges( Float32* output, const Float32* const outputEnd,
										const Float32* delayBufferStart,
										Float currentDelay, const Float delayChangePerSample,
										SIMDBands& currentGain, const SIMDBands& gainChangePerSample );
			
			
			GSOUND_FORCE_INLINE static void interpolateGainSet( const Sample32f*& input, const Sample32f* const inputEnd, Sample32f*& output,
																Gain& gain, Gain gainChangePerSample );
			
			
			GSOUND_FORCE_INLINE static void interpolateGainAdd( const Sample32f*& input, const Sample32f* const inputEnd, Sample32f*& output,
																Gain& gain, Gain gainChangePerSample );
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default maximum number of doppler shifted paths that can be rendered per sound source.
			static const Size DEFAULT_MAX_NUM_DOPPLER_PATHS_PER_SOURCE = 10;
			
			
		//********************************************************************************
		//******	Private Static Convolution Paramter Data Members
			
			
			/// The default number of partitions to use for each FDL.
			static const Size DEFAULT_PARTITIONS_PER_FDL = 4;
			
			
			/// The factor by which the FDL size grows for each additional FDL.
			static const Size DEFAULT_FDL_MULTIPLIER = 2;
			
			
			/// The default number of partitions to use for each FDL.
			static const Size DEFAULT_MAX_FDL_COUNT = 16;
			
			
			/// The default number of samples for the first FDL partition.
			static const Size DEFAULT_MIN_FDL_SIZE = 256;
			
			
			/// The default maximum number of samples for an FDL partition.
			static const Size DEFAULT_MAX_FDL_SIZE = 32768;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An array of the master FDL state objects for this renderer.
			ArrayList<FDLState*> fdls;
			
			
			/// A map from sound sources to their rendering states.
			HashMap<const SoundSource*,Index> sourceStateMap;
			
			
			/// A pool of the source render states for this listener.
			ObjectPool<SourceState> sourceStates;
			
			
			/// A list of the cluster render states for this listener.
			ObjectPool<ClusterState> clusterStates;
			
			
			/// A pool of the convolution render states for this renderer.
			ObjectPool<ConvolutionState> convolutionStates;
			
			
			/// A list of objects that store thread-local state for each rendering thread.
			ArrayList<RenderThreadState> renderStates;
			
			
			RenderThreadState sharedRenderState;
			
			
			/// A list of objects that store thread-local state for each update thread.
			ArrayList<UpdateThreadState> updateStates;
			
			
			/// A thread pool that handles updating IRs for sound sources.
			ThreadPool updateThreadPool;
			
			
			/// A thread pool that handles sound rendering jobs for this listener renderer.
			ThreadPool renderThreadPool;
			
			
			/// A SIMD crossover that is being used to filter input source audio.
			CrossoverType crossover;
			
			
			/// The update timestamp for this listener renderer.
			/**
			  * This value is increased by 1 each time the IR for the renderer is updated.
			  * It is used to know when paths, sources, and clusters have not been
			  * updated and should be removed from the renderer.
			  */
			Index timeStamp;
			
			
			/// An object that stores the interpolation state for this listener renderer's gain.
			LerpState<Float32> listenerGain;
			
			
			/// A lookup table to use to do fast panning of IRs.
			internal::PanLookupTable<128> panLookupTable;
			
			
			/// An object that handles HRTF filter generation for this listener renderer.
			internal::HRTFFilter hrtf;
			
			
			/// The current processing load for the main rendering thread.
			Float processingLoad;
			
			
			/// A mutex that is locked whenever rendering is being done on the main rendering thread.
			mutable Mutex renderingMutex;
			
			
		//********************************************************************************
		//******	Private Parameter Data Members
			
			
			/// The current render request that the listener renderer is executing.
			RenderRequest request;
			
			
			/// The maximum number of FDLs that are allowed for this convolution renderer.
			Size maxFDLCount;
			
			
			/// The minimum number of samples allowed for an FDL partition.
			Size minFDLSize;
			
			
			/// The maximum number of samples allowed for an FDL partition.
			Size maxFDLSize;
			
			
			/// The number of partitions to use for each FDL.
			Size partitionsPerFDL;
			
			
			/// The current write position within the input and output covolution queues.
			Index convolutionQueuePosition;
			
			
			/// The size of the convolution input and output queues.
			Index convolutionQueueSize;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_LISTENER_RENDERER_H
