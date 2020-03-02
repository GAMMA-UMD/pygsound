/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundPropagationSystem.cpp
 * Contents:    gsound::SoundPropagationSystem class implementation
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


#include "gsSoundPropagationSystem.h"


#define UPDATE_JOB_ID 0x1000
#define PROPAGATION_JOB_ID 0x1001


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




class SoundPropagationSystem:: ListenerRenderer : public SoundFilter, public SoundInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new listener render state object associated with the specified listener.
			GSOUND_INLINE ListenerRenderer( const SoundListener* newListener,
											const RenderRequest* newRequest,
											SoundPropagationSystem* newSystem )
				:	listener( newListener ),
					request( newRequest ),
					renderer( *newRequest ),
					system( newSystem ),
					streamTime( newSystem->streamTime ),
					streamPosition( 0 )/*,
					updateTimestamp( 0 )*/
			{
			}
			
			
		//********************************************************************************
		//******	Sound Input Stream Methods
			
			
			/// Return the number of samples remaining in the sound input stream.
			virtual SoundSize getSamplesRemaining() const
			{
				return math::max<SoundSize>();
			}
			
			
			/// Return the current position of the stream in samples relative to the start of the stream.
			virtual SampleIndex getPosition() const
			{
				return streamPosition;
			}
			
			
			/// Return the number of channels that are in the sound input stream.
			virtual Size getChannelCount() const
			{
				return request->channelLayout.getChannelCount();
			}
			
			
			/// Return the sample rate of the sound input stream's source audio data.
			virtual SampleRate getSampleRate() const
			{
				return request->sampleRate;
			}
			
			
			/// Return the actual sample type used in the stream.
			virtual SampleType getNativeSampleType() const
			{
				return SampleType::SAMPLE_32F;
			}
			
			
			/// Return whether or not the stream has a valid source of sound data.
			virtual Bool isValid() const
			{
				return true;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to the listener that is being rendered by this renderer.
			const SoundListener* listener;
			
			
			/// An object that describes how the listener's audio should be rendered.
			const RenderRequest* request;
			
			
			/// A pointer to the sound propagation system that created this listener renderer.
			SoundPropagationSystem* system;
			
			
			/// A listener renderer that is being used to render the listener's audio.
			SoundListenerRenderer renderer;
			
			
			/// The number of samples since the start of the system's stream.
			SampleIndex streamPosition;
			
			
			/// The time since the start of the system's stream for the buffered sound.
			Time streamTime;
			
			
	private:
		
		//********************************************************************************
		//******	Private Rendering Methods
			
			
			/// Read the specified number of samples from the input stream into the output buffer.
			virtual SoundResult readSamples( SoundBuffer& outputBuffer, Size numSamples )
			{
				return renderSamples( outputBuffer, numSamples );
			}
			
			
			/// Read the specified number of samples from the input stream into the output buffer.
			virtual SoundResult processFrame( const SoundFrame& inputFrame, SoundFrame& outputFrame, Size numSamples )
			{
				SoundBuffer* outputBuffer;
				
				// If there is no output buffer or if it is NULL, return that no samples were processed.
				if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
					return 0;
				
				return renderSamples( *outputBuffer, numSamples );
			}
			
			
			/// Render the given number of samples for this listener renderer into the output buffer.
			GSOUND_INLINE Size renderSamples( SoundBuffer& outputBuffer, Size numSamples )
			{
				// Create a timer to time how long it takes to buffer the sound data.
				Timer bufferTimer;
				
				// Compute the length of the output frame in seconds.
				Time frameTime = Time( Double(numSamples) / outputBuffer.getSampleRate() );
				
				// Determine the number of output samples.
				Size numOutputSamples = (Size)math::floor( frameTime*request->sampleRate );
				
				// Update the frame time, rounded to the nearest sample.
				//frameTime = Time( Double(numOutputSamples) / request->sampleRate );
				
				// Advance the stream time by the frame time.
				streamTime += frameTime;
				streamPosition += numOutputSamples;
				
				// Buffer the input sound source data for this frame.
				if ( !system->bufferSourceSound( streamTime ) )
				{
					// The stream time is out of date, reset it to match the global position.
					streamTime = system->streamTime;
					
					// Buffer the sound for the up to date position.
					system->bufferSourceSound( streamTime );
				}
				
				// Report the buffering load if statistics are enabled.
				if ( request->statistics && request->flags.isSet( RenderFlags::STATISTICS ) )
					request->statistics->bufferingLoad = Float(bufferTimer.getElapsedTime() / frameTime);
				
				// Render the sound for this listener.
				return renderer.render( system->sourceSoundBuffer, outputBuffer, frameTime );
				
				//**********************************************************************
				/*
				outputBuffer.zero();
				updateMutex.lock();
				
				SoundBuffer tempBuffer( outputBuffer.getChannelCount(), numSamples );
				HashMap<SoundSource*,SoundSourceRenderer>::Iterator s = sourceRenderers.getIterator();
				
				for ( ; s; s++ )
				{
					SoundBuffer* sourceInput = NULL;
					
					for ( Index i = 0; i < system->sourceSoundBuffer.getSourceCount(); i++ )
					{
						if ( system->sourceSoundBuffer.getSource(i) == s.getKey() )
						{
							sourceInput = system->sourceSoundBuffer.getSourceBuffer(i);
							break;
						}
					}
					
					if ( sourceInput == NULL )
						continue;
					
					convolution.convolve( s->convolutionID, *sourceInput, tempBuffer, numSamples );
					tempBuffer.mixTo( outputBuffer, numSamples );
				}
				
				updateMutex.unlock();
				return numSamples;*/
			}
			
			
};




//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




SoundPropagationSystem:: SoundPropagationSystem()
	:	scene( NULL ),
		streamTime( 0 ),
		bufferedTime( 0 ),
		propagationRequest( NULL ),
		currentIR( 0 ),
		numUpdateThreads( 2 ),
		isPropagating( 0 ),
		missingTime( 0 )
{
	updateThreadPool.setPriority( ThreadPriority::LOW );
}




SoundPropagationSystem:: SoundPropagationSystem( const SoundPropagationSystem& other )
	:	isPropagating( 0 ),
		streamTime( 0 ),
		bufferedTime( 0 )
{
	other.mutex.lock();
	
	// Get the data from the other system.
	scene = other.scene;
	propagationRequest = other.propagationRequest;
	currentIR = other.currentIR;
	numUpdateThreads = other.numUpdateThreads;
	isPropagating = other.isPropagating;
	missingTime = other.missingTime;
	propagationTime = other.propagationTime;
	irUpdateTime = other.irUpdateTime;
	updateThreadPool.setPriority( ThreadPriority::LOW );
	
	// Copy the listener renderers in the other system.
	for ( Index i = 0; i < other.listenerRenderers.getSize(); i++ )
	{
		const ListenerRenderer& listenerState = *other.listenerRenderers[i];
		listenerRenderers.add( util::construct<ListenerRenderer>( listenerState.listener, listenerState.request, this ) );
	}
	
	other.mutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############
//############		Destructor
//############
//##########################################################################################
//##########################################################################################




SoundPropagationSystem:: ~SoundPropagationSystem()
{
	mutex.lock();

	// Finish updates.
	updateThreadPool.finishJobs();
	
	// Destroy the listener renderers.
	const Size numListenerRenderers = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListenerRenderers; i++ )
		util::destruct( listenerRenderers[i] );
	
	listenerRenderers.clear();
	
	mutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############
//############		Assignment Operator
//############
//##########################################################################################
//##########################################################################################




SoundPropagationSystem& SoundPropagationSystem:: operator = ( const SoundPropagationSystem& other )
{
	if ( this != &other )
	{
		mutex.lock();
		other.mutex.lock();
		
		// Finish updates.
		updateThreadPool.finishJobs();
		
		// Clear the previous listeners from this system.
		for ( Index i = 0; i < listenerRenderers.getSize(); i++ )
			util::destruct( listenerRenderers[i] );
		
		listenerRenderers.clear();
		
		// Get the data from the other system.
		bufferedTime = 0;
		streamTime = 0;
		scene = other.scene;
		propagationRequest = other.propagationRequest;
		currentIR = other.currentIR;
		numUpdateThreads = other.numUpdateThreads;
		isPropagating = other.isPropagating;
		missingTime = other.missingTime;
		propagationTime = other.propagationTime;
		irUpdateTime = other.irUpdateTime;
		
		// Copy the listener renderers in the other system.
		for ( Index i = 0; i < other.listenerRenderers.getSize(); i++ )
		{
			const ListenerRenderer& listenerState = *other.listenerRenderers[i];
			listenerRenderers.add( util::construct<ListenerRenderer>( listenerState.listener, listenerState.request, this ) );
		}
		
		other.mutex.unlock();
		mutex.unlock();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############
//############		System Update Method
//############
//##########################################################################################
//##########################################################################################




void SoundPropagationSystem:: update( Float dt, Bool synchronous )
{
	mutex.lock();
	
	if ( propagationRequest == NULL )
	{
		mutex.unlock();
		return;
	}
	
	//********************************************************************************
	// Make sure the update thread pool is initialized.
	
	// Add or remove the necessary number of threads to the thread pool.
	if ( updateThreadPool.getThreadCount() != numUpdateThreads )
		updateThreadPool.setThreadCount( numUpdateThreads );
	
	//********************************************************************************
	// Perform sound propagation in the current scene.
	
	if ( scene )
	{
		// Accumulate the delta time for this update.
		missingTime += dt;
		
		// Only spawn a new propagation job if the last job is finished.
		if ( missingTime >= propagationRequest->targetDt )
		{
			// Add a new propagation job to the thread pool and start processing this frame.
			if ( synchronous || !isPropagating )
			{
				updateThreadPool.addJob( om::bindCall( &SoundPropagationSystem::doSoundPropagation, this,
														propagationRequest->targetDt ),
										PROPAGATION_JOB_ID );
			}
			
			missingTime = 0;
		}
	}
	else
	{
		// Clear the IRs for all listeners.
		const Size numListeners = listenerRenderers.getSize();
		
		for ( Index i = 0; i < numListeners; i++ )
		{
			ListenerRenderer& listenerState = *listenerRenderers[i];
			listenerState.renderer.clearIR();
		}
	}
	
	// If the user wants synchronous operation, wait until the propagation and update are finished.
	if ( synchronous )
		updateThreadPool.finishJobs();
	
	mutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############
//############		Scene Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void SoundPropagationSystem:: setScene( const SoundScene* newScene )
{
	mutex.lock();
	
	scene = newScene;
	
	// Reset the internal propagation data.
	if ( propagationRequest )
		propagationRequest->internalData.reset();
	
	mutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############
//############		Listener Accessor Methods
//############
//##########################################################################################
//##########################################################################################




const SoundListener* SoundPropagationSystem:: getListener( Index listenerIndex ) const
{
	if ( listenerIndex < listenerRenderers.getSize() )
		return listenerRenderers[listenerIndex]->listener;
	else
		return NULL;
}




const RenderRequest* SoundPropagationSystem:: getListenerRequest( const SoundListener* listener ) const
{
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
			return listenerRenderers[i]->request;
	}
	
	return NULL;
}




SoundListenerRenderer* SoundPropagationSystem:: getListenerRenderer( const SoundListener* listener )
{
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
			return &listenerRenderers[i]->renderer;
	}
	
	return NULL;
}




const SoundListenerRenderer* SoundPropagationSystem:: getListenerRenderer( const SoundListener* listener ) const
{
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
			return &listenerRenderers[i]->renderer;
	}
	
	return NULL;
}




SoundInputStream* SoundPropagationSystem:: getListenerRendererStream( const SoundListener* listener )
{
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
			return listenerRenderers[i];
	}
	
	return NULL;
}




const SoundInputStream* SoundPropagationSystem:: getListenerRendererStream( const SoundListener* listener ) const
{
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
			return listenerRenderers[i];
	}
	
	return NULL;
}




SoundFilter* SoundPropagationSystem:: getListenerRendererFilter( const SoundListener* listener )
{
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
			return listenerRenderers[i];
	}
	
	return NULL;
}




const SoundFilter* SoundPropagationSystem:: getListenerRendererFilter( const SoundListener* listener ) const
{
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
			return listenerRenderers[i];
	}
	
	return NULL;
}




Bool SoundPropagationSystem:: addListener( const SoundListener* listener, const RenderRequest* request )
{
	// Fail if the listener pointer is NULL.
	if ( listener == NULL || request == NULL )
		return false;
	
	mutex.lock();
	
	listenerRenderers.add( util::construct<ListenerRenderer>( listener, request, this ) );
	
	mutex.unlock();

	return true;
}




Bool SoundPropagationSystem:: removeListener( const SoundListener* listener )
{
	mutex.lock();
	
	Bool success = false;
	const Size numListenerRenderers = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListenerRenderers; )
	{
		if ( listenerRenderers[i]->listener == listener )
		{
			util::destruct( listenerRenderers[i] );
			listenerRenderers.removeAtIndexUnordered( i );
			success = true;
			continue;
		}
		
		i++;
	}
	
	mutex.unlock();

	return success;
}




void SoundPropagationSystem:: clearListeners()
{
	mutex.lock();
	
	const Size numListenerRenderers = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListenerRenderers; i++ )
		util::destruct( listenerRenderers[i] );
	
	listenerRenderers.clear();

	mutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############
//############		Listener Rendering Method
//############
//##########################################################################################
//##########################################################################################




Size SoundPropagationSystem:: renderListener( const SoundListener* listener, SoundBuffer& outputBuffer, Size numSamples )
{
	ScopedMutex scopedMutex( mutex );
	
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listenerRenderers[i]->listener == listener )
		{
			SoundInputStream* stream = listenerRenderers[i];
			numSamples = stream->read( outputBuffer, numSamples );
			
			return numSamples;
		}
	}
	
	return 0;
}




//##########################################################################################
//##########################################################################################
//############
//############		IR Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void SoundPropagationSystem:: getSceneIR( SoundSceneIR& ir ) const
{
	mutex.lock();
	
	ir = sceneIR[(currentIR + 1) % 2];
	
	mutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############
//############		Mesh Preprocessing Methods
//############
//##########################################################################################
//##########################################################################################




Bool SoundPropagationSystem:: processMesh( const SoundVertex* newVertices, Size numVertices,
											const SoundTriangle* newTriangles, Size numTriangles,
											const SoundMaterial* newMaterials, Size numMaterials,
											const MeshRequest& request, SoundMesh& mesh )
{
	return preprocessor.processMesh( newVertices, numVertices, newTriangles, numTriangles, newMaterials, numMaterials, request, mesh );
}




//##########################################################################################
//##########################################################################################
//############
//############		Sound Propagation Update Method
//############
//##########################################################################################
//##########################################################################################




void SoundPropagationSystem:: doSoundPropagation( Float dt )
{
	// Atomically signal to the main thread that sound propagation is currently being performed.
	isPropagating++;
	
	//********************************************************************************
	// Determine the simulation quality based on the last frame time.
	
	// Determine the quality factor to use for the simulation based on the last frame time.
	if ( propagationRequest->flags.isSet( PropagationFlags::ADAPTIVE_QUALITY ) )
	{
		Float lastFrameTime = (Float)propagationTime;
		Float targetDt = propagationRequest->targetDt;
		
		// Compute the desired quality factor based on the ratio needed to correct
		// for the last frame's overage.
		//Float lastRatio = (targetDt / lastFrameTime);
		Float response = 0.25;
		Float lastRatio = targetDt / (targetDt*(1 - response) + lastFrameTime*response);
		
		propagationRequest->quality = math::clamp( propagationRequest->quality*lastRatio,
													propagationRequest->minQuality, propagationRequest->maxQuality );
	}
	else
		propagationRequest->quality = Float(1);
	
	//********************************************************************************
	// Do sound propagation.
	
	// Create a timer that times how long sound propagation takes.
	Timer propagationTimer;
	
	// Get the current output IR.
	SoundSceneIR& outputIR = sceneIR[currentIR];
	
	// Update the request with the current delta time.
	propagationRequest->dt = dt;
	
	// Do the sound propagation, storing the output in the current IR.
	propagator.propagateSound( *scene, *propagationRequest, outputIR );
	
	// Update the total time that was taken on this frame.
	propagationTime = propagationTimer.getElapsedTime();
	
	//********************************************************************************
	// Report the global statistics from the last frame.
	
	if ( propagationRequest->statistics && propagationRequest->flags.isSet( PropagationFlags::STATISTICS ) )
	{
		SoundStatistics* statistics = propagationRequest->statistics;
		
		statistics->irUpdateTime = irUpdateTime;
		statistics->pathCount = outputIR.getPathCount();
		statistics->irMemory = outputIR.getSizeInBytes() + sceneIR[(currentIR + 1) % 2].getSizeInBytes();
		statistics->propagationMemory = propagationRequest->internalData.getSizeInBytes();
		statistics->totalMemory = statistics->sceneMemory + statistics->propagationMemory +
										statistics->renderingMemory + statistics->irMemory;
		statistics->renderingMemory = 0;
	}
	
	//********************************************************************************
	// Spawn IR update jobs.
	
	// Wait for previous update jobs to finish before swapping the IRs.
	updateThreadPool.finishJob( UPDATE_JOB_ID );
	
	irUpdateTime = 0;
	
	// Swap the IRs.
	currentIR = (currentIR + 1) % 2;
	
	// For each valid listener renderer, update the IR concurrently using the update thread pool.
	const Size numListeners = listenerRenderers.getSize();
	
	for ( Index i = 0; i < numListeners; i++ )
	{
		ListenerRenderer* listenerRenderer = listenerRenderers[i];
		const SoundListener* listener = listenerRenderer->listener;
		const SoundListenerIR* listenerIR = outputIR.findListenerIR( listener );
		
		if ( listenerIR )
		{
			// Update the renderer with the current IR on another thread.
			updateThreadPool.addJob( FunctionCall<void (SoundListenerRenderer&, const SoundListenerIR&, const RenderRequest&)>( 
											om::bind( &SoundPropagationSystem::updateListenerIR, this ),
											listenerRenderer->renderer, *listenerIR, *listenerRenderer->request ), UPDATE_JOB_ID );
		}
		else
		{
			// Clear this renderer's state because there is no IR update for it.
			listenerRenderer->renderer.clearIR();
		}
		
		/*
		if ( !listenerIR )
			continue;
		
		const Size numSources = listenerIR->getSourceCount();
		
		for ( Index s = 0; s < numSources; s++ )
		{
			const SoundSourceIR& sourceIR = listenerIR->getSourceIR(s);
			SoundSource* source = (SoundSource*)sourceIR.getSource(0);
			listenerRenderer->updateSourceIR( source, sourceIR );
		}*/
	}
	
	//********************************************************************************
	
	// Atomically signal to the main thread that sound propagation is no longer being performed.
	isPropagating--;
}




//##########################################################################################
//##########################################################################################
//############
//############		Listener IR Update Method
//############
//##########################################################################################
//##########################################################################################




void SoundPropagationSystem:: updateListenerIR( SoundListenerRenderer& renderer, const SoundListenerIR& ir,
												const RenderRequest& request )
{
	Timer updateTimer;
	
	renderer.updateIR( ir, request );
	
	irUpdateTime = math::max( irUpdateTime, updateTimer.getElapsedTime() );
}




//##########################################################################################
//##########################################################################################
//############
//############		Source Sound Buffering Method
//############
//##########################################################################################
//##########################################################################################




Bool SoundPropagationSystem:: bufferSourceSound( const Time& newStreamTime )
{
	// Don't buffer anything if the stream is up to date.
	if ( newStreamTime == streamTime )
		return true;
	else if ( newStreamTime < streamTime && (streamTime - newStreamTime) > bufferedTime )
		return false;
	
	sourceSoundBuffer.clearSources();
	
	if ( scene )
	{
		// Get the input sound for all of the sources in the scene.
		const Size numSources = scene->getSourceCount();
		bufferedTime = newStreamTime - streamTime;
		
		for ( Index s = 0; s < numSources; s++ )
		{
			SoundSource* source = scene->getSource(s);
			SoundBuffer* sourceBuffer = sourceSoundBuffer.addSource( source );
			
			if ( sourceBuffer )
				source->readSamples( *sourceBuffer, bufferedTime );
		}
	}
	
	streamTime = newStreamTime;
	
	return true;
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
