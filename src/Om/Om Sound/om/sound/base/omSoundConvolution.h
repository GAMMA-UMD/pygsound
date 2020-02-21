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

#ifndef INCLUDE_OM_SOUND_CONVOLUTION_H
#define INCLUDE_OM_SOUND_CONVOLUTION_H


#include "omSoundBaseConfig.h"


#include "omSoundBuffer.h"
#include "omSoundLerpState.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that implements multi-output dynamic streaming convolution.
class Convolution
{
	public:
		
		//********************************************************************************
		//******	Public Type Declarations
			
			
			/// A type that is used to represent the ID of a convolution instance.
			typedef Index ID;
			
			
			/// An invalid convolution instance ID.
			static const ID INVALID_ID = -1;
			
			
			/// A class that manages parameters for a convolution instance.
			class Request
			{
				public:
					
					/// Create a new convolution request with the default initial parameters.
					OM_INLINE Request()
						:	inputChannelCount( 1 ),
							outputChannelCount( 1 ),
							irChannelCount( 1 ),
							sampleRate( 44100.0 ),
							maxLatency( 0.01f ),
							maxUpdateLatency( 0.2f ),
							interpolationTime( 0.05f )
					{
					}
					
					
					/// The number of input channels there are for this request.
					/**
					  * This value should either be 1 or equal to the number of output channels.
					  * If 1, the input channel is used to feed all outputs.
					  * If the same number as the output, there is a 1-to-1 mapping of channels.
					  * The number of input channels must be at least 1.
					  * If the number of input channels is not 1 and not the same as the number of
					  * output channels, the unused input channels are fed with the last input channel.
					  */
					Size inputChannelCount;
					
					
					/// The number of output channels there are for this request.
					/**
					  * The number of output channels must be at least 1.
					  */
					Size outputChannelCount;
					
					
					/// The number of impulse response channels there are for this request.
					/**
					  * The number of IR channels should be either 1 or the number of output
					  * channels there are. If equal to 1, the same IR is used on all outputs.
					  * If the number of IR channels is not 1 and not the same as the number of
					  * output channels, the unused IR channels are fed with the last IR channel.
					  */
					Size irChannelCount;
					
					
					/// The sample rate that is used for this request.
					SampleRate sampleRate;
					
					
					/// The maximum allowed latency of the convolution audio processing stream in seconds.
					/**
					  * The system will attempt to meet this target by altering its processing.
					  */
					Float maxLatency;
					
					
					/// The maximum allowed latency for updates to the IR in seconds.
					/**
					  * Decreasing the latency limits the maximum FFT partition size (to less than half this value), but 
					  * also requires more computation for long IRs.
					  */
					Float maxUpdateLatency;
					
					
					/// The minimum crossfade interpolation time in seconds that is used when an IR is updated.
					Float interpolationTime;
					
			};
			
			
			/// A class that holds parameters for how an impulse response is updated.
			class IRRequest
			{
				public:
					
					/// Create a new IR request with the default parameters.
					OM_INLINE IRRequest()
						:	start( 0 ),
							length( 0 ),
							delay( 0 ),
							gain( 1 ),
							asynchronous( false )
					{
					}
					
					
					/// The sample index in the input IR where the IR used for convolution should start.
					/**
					  * This value effectively trims off the beginning samples of the IR.
					  */
					Index start;
					
					
					/// If non-zero, indicates the number of samples from the IR to use for convolution.
					Size length;
					
					
					/// A broadband delay in samples that is applied to the IR.
					Index delay;
					
					
					/// A linear gain factor that is applied to the IR before convolution.
					Float gain;
					
					
					/// A boolean value that indicates whether or not the IR update is asynchronous.
					/**
					  * If equal to TRUE, the IR is only updated if the convolution system has processed
					  * previous pending updated IRs. This can be used to save processing and
					  * to allow for more efficient IR updates without any waiting.
					  *
					  * If equal to FALSE (default), the IR is always updated, but the calling thread
					  * must wait until all previous pending updated IRs are processed before submitting
					  * the new one.
					  */
					Bool asynchronous;
					
					
			};
			
			
			/// A class that opaquely provides an IR from an external source.
			/**
			  * The delegate should provide as many IR samples as
			  * specified in the IRRequest's length.
			  */
			class IRDelegate
			{
				public:
					
					
					/// Write a range of IR samples for the specified channel to the given pointer.
					/**
					  * The delegate should generate the requested number of IR samples starting
					  * at the given offset index from the start of the IR and write
					  * them to the output buffer.
					  *
					  * The convolution system will always request parts of the IR in sequential order
					  * and will always read each channel in sequential order for a given range of IR samples.
					  * However, not every part of the IR may be requested if it is not possible to update those
					  * regions due to pending interpolation for a previous update operation.
					  * Thus, computation can be saved since not every part of the IR needs to be computed.
					  * This functionality is most useful in applications where the IR is changing constantly,
					  * such as in sound propagation systems.
					  */
					Function<void ( Index channel, Index offset, Size numSamples, Sample32f* samples )> readIR;
					
					
					/// An object that stores an opaque pointer to delegate data.
					UserData data;
					
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new convolution system with the default initial state.
			Convolution();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a convolution system and release all internal resources.
			~Convolution();
			
			
		//********************************************************************************
		//******	Thread Count Accessor Methods
			
			
			/// Return the number of threads that this convolution system is using to process convolution.
			OM_INLINE Size getThreadCount() const
			{
				return threadCount;
			}
			
			
			/// Set the number of threads that this convolution system is using to process convolution.
			void setThreadCount( Size newThreadCount );
			
			
		//********************************************************************************
		//******	Instance Accessor Methods
			
			
			/// Return the number of active convolution instances there are in this convolution system.
			OM_INLINE Size getInstanceCount() const
			{
				return instances.getSize();
			}
			
			
			/// Create a new convolution instance that uses the specified convolution request parameters.
			/**
			  * If the method succeeds, an opaque integer ID for the convolution instance
			  * is returned. Otherwise, INVALID_ID is returned and no instance is created.
			  */
			ID newInstance( const Request& request );
			
			
			/// Delete the convolution instance with the specified ID.
			/**
			  * The method waits for background thread processing for the instance to finish, then
			  * stops the convolution for the instance and frees all resources.
			  *
			  * The method returns whether or not the instance was successfully deleted.
			  */
			Bool deleteInstance( ID instanceID );
			
			
			/// Delete all convolution instances and stop all convolution processing.
			/**
			  * The method waits for background thread processing to finish, then
			  * stops the convolution for all instances and frees all resources.
			  */
			void clearInstances();
			
			
		//********************************************************************************
		//******	Instance Request Accessor Methods
			
			
			/// Get the convolution request for the specified instance.
			/**
			  * The method returns the current convolution request for the instance.
			  */
			Request getRequest( ID instanceID ) const;
			
			
			/// Get the convolution request for the specified instance.
			/**
			  * The method writes the current convolution request for the specified instance
			  * to the output parameter.
			  *
			  * The method returns whether or not the instance request was able to be
			  * accessed.
			  */
			Bool getRequest( ID instanceID, Request& request ) const;
			
			
			/// Modify the convolution request for the specified instance.
			/**
			  * The method potentially halts background processing for the specified instance
			  * and then restarts convolution using the new parameters.
			  *
			  * The method returns whether or not the instance request was able to be
			  * changed.
			  */
			Bool setRequest( ID instanceID, const Request& request );
			
			
		//********************************************************************************
		//******	Instance IR Accessor Methods
			
			
			/// Get the impulse response for the specified convolution instance.
			/**
			  * If the method succeeds, it writes the current impulse response for the
			  * given convolution instance to the output sound buffer.
			  *
			  * The method returns whether or not the IR was able to be accessed.
			  */
			Bool getIR( ID instanceID, SoundBuffer& ir ) const;
			
			
			/// Prepare the impulse response for the specified convolution instance with custom parameters.
			/**
			  * The method allocates the necessary space for the IR, but does not copy the data
			  * or perform other IR preprocessing. This method CANNOT be called concurrently with
			  * the conolve() method.
			  *
			  * The method returns whether or not the IR was able to be updated.
			  */
			Bool prepareIR( ID instanceID, const IRRequest& request );
			
			
			/// Prepare the impulse response for the specified convolution instance with custom parameters.
			/**
			  * The method allocates the necessary space for the IR, but does not copy the data
			  * or perform other IR preprocessing. This method CANNOT be called concurrently with
			  * the conolve() method.
			  *
			  * The method returns whether or not the IR was able to be updated.
			  */
			Bool prepareIR( ID instanceID, const SoundBuffer& ir, const IRRequest& request );
			
			
			/// Update the impulse response for the specified convolution instance.
			/**
			  * The default IRRequest is used for the update.
			  * The method returns whether or not the IR was able to be updated.
			  */
			OM_INLINE Bool setIR( ID instanceID, const SoundBuffer& ir )
			{
				return this->setIR( instanceID, ir, IRRequest() );
			}
			
			
			/// Update the impulse response for the specified convolution instance with custom parameters.
			/**
			  * The provided IRRequest is used to determine how the update is handled.
			  *
			  * This method CANNOT be called concurrently with the convolve() method,
			  * HOWEVER, if the method prepareIR() is called before setIR()
			  * (and synchronized via external mutex), then this method can be called without
			  * synchronization. If prepareIR() is not called beforehand, the method must
			  * be synchronized with convolve().
			  * 
			  * The method returns whether or not the IR was able to be updated.
			  */
			Bool setIR( ID instanceID, const SoundBuffer& ir, const IRRequest& request );
			
			
			/// Update the impulse response for the specified convolution instance with custom parameters.
			/**
			  * The IR is requested by the convolution system from the IR delegate.
			  * The provided IRRequest indicates the length of the opaque IR and 
			  * is used to determine how the update is handled.
			  * Since not every part of the IR will be updated if the update is asynchronous,
			  * using a delegate allows computation to be saved when it is expensive to compute the
			  * new IR.
			  *
			  * This method CANNOT be called concurrently with the convolve() method,
			  * HOWEVER, if the method prepareIR() is called before setIR()
			  * (and synchronized via external mutex), then this method can be called without
			  * synchronization. If prepareIR() is not called beforehand, the method must
			  * be synchronized with convolve().
			  * 
			  * The method returns whether or not the IR was able to be updated.
			  */
			Bool setIR( ID instanceID, const IRDelegate& delegate, const IRRequest& request );
			
			
		//********************************************************************************
		//******	Rendering Methods
			
			
			/// Render the convolution for a single instance using the specified input and output buffers.
			/**
			  * If the method succeeds, it uses the specified number of samples from the input buffer
			  * as input for the convolution and writes the same number of samples to the output buffer.
			  *
			  * The method returns whether or not the operation was successful.
			  */
			Bool convolve( ID instanceID, const SoundBuffer& input, SoundBuffer& output, Size numSamples );
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that stores information about a single convolution instance.
			class Instance;
			
			
			/// A class that manages a single FFT partition size for a convolution instance.
			class InstanceFDL;
			
			
			/// The type to use for a 32-bit floating point complex number.
			typedef math::Complex<Float32> Complex32f;
			
			
			/// The type to use for a 2D buffer of 32-bit floating point numbers.
			typedef PODArray< Float32, 2, Size, AlignedAllocator<16> > Buffer32f;
			
			
			/// The type to use for a 2D buffer of 32-bit floating point complex numbers.
			typedef PODArray< Complex32f, 2, Size, AlignedAllocator<16> > BufferComplex32f;
			
			
			typedef ObjectPool<Instance>::Iterator InstanceIterator;
			
			
		//********************************************************************************
		//******	Private Copy Operations
			
			
			/// Declared private to prevent copying. TODO: fix me.
			Convolution( const Convolution& other );
			
			
			/// Declared private to prevent copying. TODO: fix me.
			Convolution& operator = ( const Convolution& other );
			
			
		//********************************************************************************
		//******	Private IR Update Methods
			
			
			/// Prepare an instance so that it is ready to recieve a new IR.
			void prepareIR( Instance& instance, const IRRequest& request );
			
			
			/// Prepare an instance so that it is ready to recieve a new IR.
			void prepareIR( Instance& instance, const SoundBuffer& ir, const IRRequest& request );
			
			
			/// Update an instance so that it can support an IR of the specified length.
			void setIRLength( Instance& instance, Size maxIRLength );
			
			
			/// Update the IR for the specified convolution instance.
			void updateIR( Instance& instance, const SoundBuffer& ir, const IRRequest& request );
			
			
			/// Update the IR for the specified convolution instance using an opaque external delegate.
			void updateIR( Instance& instance, const IRDelegate& delegate, const IRRequest& request );
			
			
		//********************************************************************************
		//******	Private Convolution Methods
			
			
			void convolve( Instance& instance, const SoundBuffer& inputBuffer,
							SoundBuffer& outputBuffer, Size numSamples );
			
			
			/// Read from the specified input buffer and write the samples to the instance's input queue.
			OM_FORCE_INLINE static void writeInput( Instance& instance, const SoundBuffer& inputBuffer,
													Index startOffset, Size numSamples );
			
			
			/// Read output from the instance's output queue and write it to the specified output buffer.
			OM_FORCE_INLINE static void readOutput( Instance& instance, SoundBuffer& outputBuffer,
													Index startOffset, Size numSamples );
			
			
			/// Process a convolution frame after the specified number of FDL deadlines have finished.
			void processFFTFrame( Instance& instance, Size numDeadlines );
			
			
			/// Copy from the instance's input queue to the input partition for the specified FDL index.
			OM_FORCE_INLINE static void writeFDLInput( Instance& instance, Index fdlIndex );
			
			
			/// Copy from the output buffer of the FDL with the given index and write to the instance's output queue.
			OM_FORCE_INLINE static void readFDLOutput( Instance& instance, Index fdlIndex );
			
			
		//********************************************************************************
		//******	Private FDL Rendering Methods
			
			
			/// The main FDL rendering method.
			static void renderFDL( Instance& instance, InstanceFDL& fdl );
			
			
			/// A function that performs the convolution and inverse FFT for the given ir and output buffers.
			OM_FORCE_INLINE static void renderFDLOutput( const InstanceFDL& fdl, const BufferComplex32f& input,
														const BufferComplex32f& ir, Buffer32f& output );
			
			
			/// Interpolate the last half of both the main and target output buffers and write to first half of the main output buffer.
			OM_FORCE_INLINE static void interpolateFDLOutput( InstanceFDL& fdl, Buffer32f& mainOutput, Buffer32f& targetOutput,
																SampleRate sampleRate );
			
			
			/// Fade the last half of the target buffer and write it to first half of the main output buffer.
			OM_FORCE_INLINE static void fadeFDLOutput( InstanceFDL& fdl, Buffer32f& mainOutput, Buffer32f& targetOutput,
														SampleRate sampleRate );
			
			
			/// Copy the last half of the output buffer to the first half.
			OM_FORCE_INLINE static void copyFDLOutput( InstanceFDL& fdl, Buffer32f& output );
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// A function that returns the next deadline index for the given instance and FDL.
			OM_FORCE_INLINE static Index getNextDeadlineForFDL( Instance& instance, Index fdlIndex );
			
			
			/// Return an ID for the specified instance and deadline index.
			OM_FORCE_INLINE static Index getFDLJobID( Index instanceIndex, Index deadlineIndex )
			{
				return ((Index)(instanceIndex+1) << 8) | ((Index)deadlineIndex);
			}
			
			
			/// Resize the specified circular convolution queue and return the new position.
			static Index resizeQueue( Buffer32f& queue, Index position, Size currentSize, Size newSize );
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default minimum FDL size that a convolution system can have.
			const static Size DEFAULT_MIN_FDL_SIZE = 16;
			
			
			/// The default number of partitions there are for each FDL size.
			const static Size DEFAULT_PARTITIONS_PER_FDL = 4;
			
			
			/// The default factor that the FDL size grows by for each larger FDL.
			const static Size DEFAULT_FDL_FACTOR = 2;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pool of the active convolution instances.
			ObjectPool<Instance> instances;
			
			
			/// A list of the FFTs that are part of this convolution system, one for each block size.
			ArrayList< FFTReal<Sample32f>*> ffts;
			
			
			/// A pool of threads that process convolution in the background to avoid audio glitches.
			ThreadPool threadPool;
			
			
			/// The number of threads that this convolution system is using to render convolution.
			Size threadCount;
			
			
			
};




/// A type that is used to represent the ID of a convolution instance.
typedef Convolution::ID ConvolutionID;




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CONVOLUTION_H
