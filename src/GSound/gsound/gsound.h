/*
 * Project:     GSound
 * 
 * File:        gsound/gsound.h
 * Contents:    All includes for GSound library, imported into gsound:: namespace.
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


#ifndef INCLUDE_GSOUND_H
#define INCLUDE_GSOUND_H


//##########################################################################################
//##########################################################################################
//############		
//############		Library Documentation Main Page
//############		
//##########################################################################################
//##########################################################################################



/** \mainpage
  * 
  * \section main_intro Introduction
  * 
  * GSound is a C/C++ library that simulates interactive sound propagation in a 3D virtual
  * environment. The library also contains functionality for auralizing the results of
  * the sound propagation and applying the propagation to streams of input sound source audio.
  *
  * There are two interfaces to the system that can be used that each have their own benefits
  * and drawbacks.
  * - The main C++ interface can be used by including the file \ref gsound.h.
  *   The C++ interface is contained within the \ref gsound namespace. This interface allows
  *   greater flexibility and customization, but also requires managing all of the required headers
  *   for library dependencies. This interface is more likely to change in future updates.
  * - The C interface can be used by including the file \ref gsoundC.h. This file contains
  *   an opaque C-style interface to the core library functionality. This is the preferred interface
  *   if using the library in a DLL or if calling the library from an external scripting language.
  *   This interface is less likely to change due to future updates and so is the recommended interface
  *   unless more control is needed by the user.
  *
  * \subsection object_types Object Types
  *
  * The most important high-level objects that the user must interact with are summarized below:
  * - gsound::SoundScene: A scene where sound is propagated. Scenes contain a set of objects,
  *   sources, and listeners that define the sound propagation inputs. The scene also
  *   manages the propagation medium(s) for the environment.
  * - gsound::SoundSource: A sound source that is located within a 3D virtual scene with an
  *   associated position and orientation. Sources may be spherical, or can be described
  *   by arbitrary closed triangle meshes. Sources can also be directional. Each source
  *   manages playback of its sound data that is requested from SoundInputStream objects
  *   (i.e. sound files).
  * - gsound::SoundListener: A sound listener in a 3D virtual scene that has an associated position
  *   and orientation. Listeners may be spherical, or can be described
  *   by arbitrary closed triangle meshes. Sound propagation is computed for each listener
  *   in the scene, and each listener's audio is rendered separately.
  * - gsound::SoundObject: A sound object represents an instance of a sound mesh with an associated
  *   transformation. Multiple objects can share the same mesh. Objects can dynamically
  *   move, scale, and rotate in the scene.
  * - gsound::SoundMesh: A static triangle mesh that has been preprocessed for sound propagation.
  *   A mesh contains vertices, triangles, and diffraction edge information, as
  *   well as a BVH for the mesh. Meshes can be instanced in the scene using sound objects.
  *   A mesh also contains a set of materials that can be dynamically modified and specified
  *   per-triangle. Meshes are not usually created directly, but instead by preprocessing input triangle
  *   data using a propagation system or mesh preprocessor.
  * - gsound::SoundMaterial: An object that describes how sound interacts with a mesh surface in the
  *   scene. A material specifies how sound is scattered, reflected, and transmitted by triangles
  *   that have the material.
  * - gsound::SoundMedium: An object that describes how sound travels through a propagation medium in
  *   the scene. The medium determines properties like the speed of sound and air attenuation.
  * - gsound::SoundPropagationSystem: The main interface for interactive sound propagation. A system
  *   contains all of the functionality necessary to propagate and render sound for an
  *   interactive system with multiple listeners. It supports asynchronous propagation
  *   and rendering, and handles the data transfer between the propagation and rendering systems.
  *   The propagation system buffers input source audio so that it can be used for multiple
  *   listeners simultaneously.
  * - gsound::PropagationRequest: An object that contains all of the parameters for how sound propagation
  *   should be performed, including ray-tracing parameters.
  * - gsound::RenderRequest: An object that describes how rendering should be done for a listener.
  *   Each listener that is being rendered by a propagation system can have an associated shared
  *   render request that determines the parameters for rendering, such as sample rate and
  *   channel layout.
  * - gsound::MeshRequest: An object that describes how an input mesh should be preprocessed into
  *   a sound mesh that can be used by the propagation system.
  *
  * \subsection dependencies Dependencies
  *
  * To compile and use the library, the following dependencies are required:
  * - FFTW: The FFTW library is used to perform FFTs during sound propagation rendering. The
  *   FFTW headers are not exposed to the public library interface, and so are not required
  *   after building the GSound lib.
  * - Om Framework: A set of utility and math classes, headers public.
  * - Om BVH: A library that creates bounding volume hierarchies and does ray tracing, headers public.
  * - Om Sound: A library for sound processing and I/O, headers public.
  * - Om Resources: A library for resource loading and I/O, headers public.
  * - Om Compression: A library for compressed data I/O, headers public.
  *
  * If the C-based interface in gsoundC.h is used, none of the dependency headers are exposed.
  */


//##########################################################################################
//##########################################################################################
//############		
//############		Library Include Files
//############		
//##########################################################################################
//##########################################################################################




// Library configuration.
#include "gsConfig.h"


// Mesh and Material Classes.
#include "gsFrequencyBands.h"
#include "gsFrequencyBandResponse.h"
#include "gsSoundTriangle.h"
#include "gsSoundMaterial.h"
#include "gsSoundMesh.h"


// Scene Object Classes.
#include "gsSoundObject.h"
#include "gsSoundDetector.h"
#include "gsSoundSource.h"
#include "gsSoundListener.h"
#include "gsSoundScene.h"


// Mesh Processing Classes.
#include "gsMeshFlags.h"
#include "gsMeshRequest.h"
#include "gsSoundMeshPreprocessor.h"


// Debug Classes.
#include "gsDebugFlags.h"
#include "gsDebugCache.h"


// Propagation Classes.
#include "gsPropagationFlags.h"
#include "gsPropagationRequest.h"
#include "gsSoundPropagator.h"


// Impulse Response Classes.
#include "gsSampledIR.h"
#include "gsSoundSourceIR.h"
#include "gsSoundListenerIR.h"
#include "gsSoundSceneIR.h"
#include "gsImpulseResponse.h"


// Rendering Classes.
#include "gsRenderFlags.h"
#include "gsRenderRequest.h"
#include "gsSoundListenerRenderer.h"


// System Classes.
#include "gsSoundPropagationSystem.h"



//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_H
