/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundPropagationSystem.h
 * Contents:    gsound::SoundPropagationSystem class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_PROPAGATION_SYSTEM_H
#define INCLUDE_GSOUND_SOUND_PROPAGATION_SYSTEM_H


#include "gsConfig.h"


#include "gsSoundScene.h"
#include "gsSoundSceneIR.h"
#include "gsSoundPropagator.h"
#include "gsSourceSoundBuffer.h"
#include "gsSoundListenerRenderer.h"
#include "gsSoundMeshPreprocessor.h"


#include "gsImpulseResponse.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that manages the concurrent propagation and rendering of an acoustic simulation.
/**
  * This class handles computing sound propagation and rendering concurrently
  * in the background for a scene. The user calls the update() method to trigger the computation
  * of a new frame of sound propagation. This propagation can either happen synchronously,
  * where the calling thread does not return until the frame is over, or can happen on a background
  * thread (the default). When the sound propagation is computed,
  * its output is sent to the renderer for each listener.
  *
  * For most uses in interactive simulations, this class should be the main access
  * point for a sound simulation. There should be no need to explicitly handle propagation
  * or rendering, only to create a scene, give it to the system, and then set the
  * rendering and propagation parameters.
  *
  * The class manages a set of listeners for which sound is being rendered.
  * In order to access the rendered output for a listener, call getListenerRenderer() to
  * get the renderer for a listener, then call renderer.read() to invoke the rendering for
  * a frame of audio samples. Alternatively, the renderListener() method can be used to 
  * render the audio for a given listener.
  */
class SoundPropagationSystem
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new default sound propagation system with no scene to simulate.
			SoundPropagationSystem();
			
			
			/// Create a copy of this sound propagation system and its internal state.
			SoundPropagationSystem( const SoundPropagationSystem& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sound propagation system, releasing all internal resources.
			virtual ~SoundPropagationSystem();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the internal state of one sound propagation system to another.
			SoundPropagationSystem& operator = ( const SoundPropagationSystem& other );
			
			
		//********************************************************************************
		//******	Update Method
			
			
			/// Update this propagation system for the specified time interval in seconds.
			/**
			  * This method starts the computation of a frame of sound propagation on
			  * another thread. If a previous frame update is still being processed,
			  * the method saves the delta time and returns without starting any propagation,
			  * so that it does not block the calling thread for any significant time.
			  *
			  * The user should call this method at least as often as the target frame rate
			  * of the simulation. It is better to call the method at a higher rate (e.g. 60Hz),
			  * the extra updates are ignored until the target delta time has elapsed, and the
			  * true update rate will better match the target delta time.
			  * 
			  * If the synchronous flag is TRUE, the method may block the calling thread until
			  * previous update operations finish. It will then start a new propagation update
			  * and wait until that is complete to return. This flag forces a new frame to
			  * be computed.
			  */
			virtual void update( Float dt, Bool synchronous = false );
			
			
		//********************************************************************************
		//******	Scene Accessor Methods
			
			
			/// Return a pointer to the current scene where sound propagation is being performed.
			GSOUND_INLINE const SoundScene* getScene() const
			{
				return scene;
			}
			
			
			/// Set the scene where sound propagation should be performed.
			/**
			  * If the specified scene is NULL, it replaces the current scene and
			  * no further sound propagation is done until a valid scene is supplied.
			  * Calling this method also resets the internal propagation data structures for
			  * the system's propagation request.
			  */
			void setScene( const SoundScene* newScene );
			
			
		//********************************************************************************
		//******	Propagation Request Accessor Methods
			
			
			/// Return a pointer to an object that contains parameters for how sound propagation is performed.
			GSOUND_INLINE PropagationRequest* getRequest()
			{
				return propagationRequest;
			}
			
			
			/// Return a pointer to an object that contains parameters for how sound propagation is performed.
			GSOUND_INLINE const PropagationRequest* getRequest() const
			{
				return propagationRequest;
			}
			
			
			/// Set a pointer to an object that contains parameters for how sound propagation is performed.
			/**
			  * If the specified pointer is NULL, it removes the current request, and on the
			  * next update, the system will create a new default request to replace it.
			  *
			  * The system does not own the request. The user is responsible for managing the request's memory.
			  */
			GSOUND_INLINE void setRequest( PropagationRequest* newRequest )
			{
				propagationRequest = newRequest;
			}
			
			
		//********************************************************************************
		//******	Listener Accessor Methods
			
			
			/// Return the current number of listeners that are being rendered by this sound propagation system.
			GSOUND_INLINE Size getListenerCount() const
			{
				return listenerRenderers.getSize();
			}
			
			
			/// Return a pointer to the listener that is being rendered at the specified listener index.
			/**
			  * The method returns NULL if there is no listener with that index.
			  */
			const SoundListener* getListener( Index listenerIndex ) const;
			
			
			/// Return a pointer to render request for the specified listener pointer.
			/**
			  * The method returns NULL if there is no listener being rendered with that pointer.
			  */
			const RenderRequest* getListenerRequest( const SoundListener* listener ) const;
			
			
			/// Return a pointer to renderer for the specified listener pointer.
			/**
			  * The method returns NULL if there is no listener being rendered with that pointer.
			  */
			SoundListenerRenderer* getListenerRenderer( const SoundListener* listener );
			
			
			/// Return a const pointer to renderer for the specified listener pointer.
			/**
			  * The method returns NULL if there is no listener being rendered with that pointer.
			  */
			const SoundListenerRenderer* getListenerRenderer( const SoundListener* listener ) const;
			
			
			/// Return a pointer to renderer output stream for the specified listener pointer.
			/**
			  * The method returns NULL if there is no listener being rendered with that pointer.
			  */
			SoundInputStream* getListenerRendererStream( const SoundListener* listener );
			
			
			/// Return a const pointer to renderer output stream for the specified listener pointer.
			/**
			  * The method returns NULL if there is no listener being rendered with that pointer.
			  */
			const SoundInputStream* getListenerRendererStream( const SoundListener* listener ) const;
			
			
			/// Return a pointer to renderer output filter for the specified listener pointer.
			/**
			  * The method returns NULL if there is no listener being rendered with that pointer.
			  */
			SoundFilter* getListenerRendererFilter( const SoundListener* listener );
			
			
			/// Return a const pointer to renderer output filter for the specified listener pointer.
			/**
			  * The method returns NULL if there is no listener being rendered with that pointer.
			  */
			const SoundFilter* getListenerRendererFilter( const SoundListener* listener ) const;
			
			
			/// Add a new listener that should be rendered to this propagation system.
			/**
			  * If the listener is in the current sound scene that is being
			  * simulated, the listener's renderer is updated with the new propagation data.
			  * Otherwise, the listener's renderer will not be updated with new IRs.
			  *
			  * The method fails if the listener or request pointer is NULL. It is not an error to
			  * add the same listener more than once - multiple audio output streams (of possibly
			  * different formats) can be computed. The listener's audio stream will be rendered
			  * using the specified render request object's parameters. The request parameters
			  * can be changed by the user and the rendering mode will be updated.
			  *
			  * The system does not own the request. The user is responsible for managing the request's memory.
			  */
			Bool addListener( const SoundListener* listener, const RenderRequest* request );
			
			
			/// Remove all renderers for the specified sound listener.
			/**
			  * If there are any listener renderers that are auralizing the specified listener,
			  * they are removed from this propagation system.
			  *
			  * The method returns whether or not any listener renderers were removed.
			  */
			Bool removeListener( const SoundListener* listener );
			
			
			/// Remove all listeners that are being auralized from this sound propagation system.
			void clearListeners();
			
			
		//********************************************************************************
		//******	Listener Rendering Method
			
			
			/// Render the sound propagation output for the specified listener into the given output buffer.
			/**
			  * If there is a listener renderer for the specified listener, the renderer places
			  * the specified number of samples in the output buffer and returns the number of samples
			  * written. If there is no renderer for that listener, 0 is returned and the buffer is unmodified.
			  */
			Size renderListener( const SoundListener* listener, SoundBuffer& outputBuffer, Size numSamples );
			
			
		//********************************************************************************
		//******	IR Accessor Methods
			 
			 
			 
			 
			 /// Copy the last computed IR for the scene into the specified output parameter.
			 /**
			   * This method allows the current impulse response for the scene to be accessed asynchronously.
			   */
			 void getSceneIR( SoundSceneIR& ir ) const;
			
			
		//********************************************************************************
		//******	Mesh Processing Methods
			
			
			/// Preprocess the specified mesh using the requested parameters.
			/**
			  * The method returns the preprocessed mesh or a NULL pointer if
			  * preprocessing failed.
			  */
			virtual Bool processMesh( const SoundVertex* newVertices, Size numVertices,
										const SoundTriangle* newTriangles, Size numTriangles,
										const SoundMaterial* newMaterials, Size numMaterials,
										const MeshRequest& request, SoundMesh& mesh );
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			/// A class that stores the relationship between a listener and the renderer that is computing the listener audio.
			class ListenerRenderer;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Compute one frame of sound propagation and then spawn IR update job(s).
			void doSoundPropagation( Float dt );
			
			
			/// Update a listener renderer with the specified listener IR and render request.
			void updateListenerIR( SoundListenerRenderer& renderer, const SoundListenerIR& ir,
									const RenderRequest& request );
			
			
			/// Buffer the input sound for all sources up to the given offset from this system's stream start.
			/**
			  * The method returns whether or not the requested stream position 
			  */
			Bool bufferSourceSound( const Time& newStreamTime );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the object that handles sound propagation for this system.
			SoundPropagator propagator;
			
			
			/// A pointer to the object which preprocesses meshes for this sound propagation system.
			SoundMeshPreprocessor preprocessor;
			
			
			/// An array of two scene IR objects that are used to double-buffer the propagation IR data.
			SoundSceneIR sceneIR[2];
			
			
			/// The index of the current scene IR that is being used for propagation output.
			Index currentIR;
			
			
			/// A list of objects that describe the listeners that should be auralized by this propagation system.
			ArrayList<ListenerRenderer*> listenerRenderers;
			



			/// A buffer that is used to store the current frame of input for all listener renderers.
			SourceSoundBuffer sourceSoundBuffer;
			
			
			/// The time that is buffered in the sound source buffer.
			Time bufferedTime;
			
			
			/// The time since the start of the system's stream for the buffered sound.
			Time streamTime;
			
			
			/// A pointer to the sound scene where propagation should be performed.
			const SoundScene* scene;
			
			
			/// An object that describes how sound propagation should be performed by this system.
			PropagationRequest* propagationRequest;
			
			
			/// A thread pool that handles the computation of frame update jobs.
			ThreadPool updateThreadPool;
			
			
			/// A mutex that is used to synchronize the system.
			mutable Mutex mutex;
			
			
			/// The number of threads that are in the update thread pool.
			Size numUpdateThreads;
			
			
			/// An atomically modified flag that indicates whether or not sound propagation is being computed currently.
			Atomic<Size> isPropagating;
			
			
			/// The time in seconds that has passed while propagation has been performed.
			Float missingTime;
			
			
			/// The time that was spent doing sound propagation on the last frame.
			Time propagationTime;
			
			
			/// The time that was spent updating the rendering IRs on the last frame.
			Time irUpdateTime;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_PROPAGATION_SYSTEM_H
