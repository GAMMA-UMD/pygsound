/*
 * Project:     GSound
 * 
 * File:        gsound/gsPropagationFlags.h
 * Contents:    gsound::PropagationFlags class declaration
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


#ifndef INCLUDE_GSOUND_PROPAGATION_FLAGS_H
#define INCLUDE_GSOUND_PROPAGATION_FLAGS_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies boolean configuration flags for sound propagation.
class PropagationFlags
{
	public:
		
		//********************************************************************************
		//******	Graphics Context Flags Enum Declaration
			
			
			/// An enum that specifies the different graphics context flags.
			enum Flag
			{
				/// A flag indicating whether or not direct sound propagation should be performed.
				/**
				  * Direct sound has a minimal performance cost, and so should generally be
				  * enabled unless for debugging purposes.
				  */
				DIRECT = (1 << 0),
				
				/// A flag indicating whether or not transmissive sound propagation should be performed.
				/**
				  * Transmission occurs when sound travels through geometry in the scene
				  * and emerges out the other side.
				  */
				TRANSMISSION = (1 << 1),
				
				/// A flag indicating whether or not specular reflection sound propagation should be performed.
				/**
				  * A specular reflection is when sound reflects at the same angle that it hits a surface.
				  * Specular reflections contribute primarily to early reflections.
				  */
				SPECULAR = (1 << 2),
				
				/// A flag indicating whether or not diffuse reflection sound propagation should be performed.
				/**
				  * A diffuse reflection is when sound is randomly scattered when it hits a surface.
				  * Diffuse reflections contribute significantly to late reverberation.
				  */
				DIFFUSE = (1 << 3),
				
				/// A flag indicating whether or not diffraction sound propagation should be performed.
				/**
				  * Diffraction enables sound to scatter around objects that would otherwise occlude the
				  * sound source.
				  */
				DIFFRACTION = (1 << 4),
				
				/// A flag indicating whether or not a cache for specular sound data should be used.
				/**
				  * This flag should always be enabled because it greatly improves both the
				  * performance and temporal coherence of specular paths with minimal performance
				  * impact and no loss of accuracy.
				  */
				SPECULAR_CACHE = (1 << 5),
				
				/// A flag indicating whether or not a cache for diffuse sound data should be used.
				/**
				  * Using the diffuse cache greatly improves the results of the diffuse propagation
				  * and allows many fewer diffuse rays to be traced (around 10x fewer). However,
				  * it may cause some small errors for sudden changes in the scene configuration
				  * if the cache averaging window is too large. These errors are not usually perceptible
				  * so this flag should usually be enabled.
				  */
				DIFFUSE_CACHE = (1 << 6),
				
				/// A flag indicating whether or not an IR cache should be used.
				/**
				  * The IR cache is an alternative to the diffuse path cache that uses less memory,
				  * is faster, and generally produces better results for a given number of rays.
				  * The IR cache is essentially a cached version of the IR that is slowly updated
				  * by the new IRs computed on each frame, i.e. there is a 1st order low-pass filter for each
				  * sample in the IR, to smooth the resulting IR over time for interactive applications.
				  *
				  * The IR cache can only be used if SAMPLED_IR is also enabled, since it does
				  * not track discrete paths from frame to frame.
				  */
				IR_CACHE = (1 << 7),
				
				/// A flag indicating whether or not visibility caches should be used for sources and listeners.
				/**
				  * If this flag is set, the system maintains a cache of the triangles that are visible to
				  * each source and listener that is used to accelerate visibility queries. If a triangle
				  * is known to not be visible to a source based on the cache, rays that hit that triangle
				  * are unlikley to hit the source, and so some ray visibility queries can be avoided.
				  */
				VISIBILITY_CACHE = (1 << 8),
				
				/// A flag indicating whether or not diffuse rays should be traced from sound sources instead of the listener.
				/**
				  * This is expensive if there are many sources but can produce more accurate results.
				  * This will cause the propagation cost to scale linearly with the number of sources,
				  * rather than sub-linearly (as it would with listener diffuse propagation).
				  */
				SOURCE_DIFFUSE = (1 << 10),
				
				/// A flag indicating whether or not directional sound sources are enabled.
				/**
				  * If this flag is enabled, any sources that have directivity information and that
				  * have the SoundSourceFlags::DIRECTIVITY flag enabled will have direction sound
				  * computed. Directional sound sources are more expensive to compute than omnidirectional
				  * sources.
				  */
				SOURCE_DIRECTIVITY = (1 << 19),
				
				/// A flag indicating whether or not source clustering should be enabled.
				/**
				  * Source clustering allows distant sources that are close to each other to be treated
				  * as a single source in order to increase performance when there are many sources.
				  */
				SOURCE_CLUSTERING = (1 << 11),
				
				/// A flag indicating whether or not air absorption should be computed for sound propagation paths.
				/**
				  * Air absorption attenuates sound in a frequency-dependent manner as it travels
				  * through the propagation medium. This flag should always be enabled for realistic output
				  * in large environments.
				  */
				AIR_ABSORPTION = (1 << 12),
				
				/// A flag indicating whether or not sampled IR output is enabled.
				/**
				  * If this flag is not set, all propagation output is always stored as discrete paths,
				  * rather than sampled IRs. If it is set, the propagation system may store output in
				  * the sampled IR for a sound source. This may be more efficient but may also require more
				  * memory for the IR buffer if number of paths is small.
				  * If computing very dense impulse responses, this flag should be
				  * enabled in order to avoid very long lists of overlapping paths that may use large amounts of memory.
				  */
				SAMPLED_IR = (1 << 13),
				
				/// A flag indicating whether or not source direction sampled IR output is enabled.
				/**
				  * If this flag is set and sampled IRs are enabled, the system stores the
				  * direction that the sound was emitted from the source for each sample in the IR,
				  * in addition to the direction towards the sound from the listener. This increases
				  * the memory required for an IR by a factor of ~42%, but also allows the rendering system
				  * to compute source directivity.
				  */
				SAMPLED_IR_SOURCE_DIRECTIONS = (1 << 14),
				
				/// A flag indicating whether or not impulse response should be trimmed based on perceptual thresholds.
				/**
				  * If this flag is set the system can trim the impulse response for each
				  * source so that only the audible parts (based on a threshold specified by
				  * the listener) are saved.
				  */
				IR_THRESHOLD = (1 << 15),
				
				/// A flag indicating whether or not the max allowed IR length can dynamically change based on scene parameters.
				/**
				  * If this flag is set along with IR_THRESHOLD, the system adaptively determines the
				  * max IR length for each source and listener and from this data chooses how far
				  * rays can travel in the scene before they are considered irrelevant. On each frame,
				  * the max allowed IR length for each source/listener is allowed to grow or shrink at the
				  * ir growth rate so that no inaudible rays are computed.
				  */
				ADAPTIVE_IR_LENGTH = (1 << 16),
				
				/// A flag indicating whether or not the simulation quality can be changed to meet performance requirements.
				/**
				  * If this flag is set, the SoundPropagationSystem can automatically scale certain
				  * propagation parameters based on the performance on previous frames.
				  * If the propagation time for the last frame was larger than the target
				  * delta time, the system reduces the quality factor by a proportional amount.
				  *
				  * If the system previously had its quality reduced and the delta time is now
				  * less than the target dt, the system returns the quality factor to 1 to regain
				  * full simulation quality.
				  */
				ADAPTIVE_QUALITY = (1 << 17),
				
				/// A flag indicating whether or not doppler path sorting is enabled.
				/**
				  * If this flag and sampled IR output is enabled, the system outputs paths as
				  * either discrete paths or as part of a sampled IR, depending on the amount of
				  * Doppler shifting that will occur for the path. The amount of shifting is compared
				  * to a perceptual threshold, and if it is greater than the threshold the path
				  * is output as a discrete path. Otherwise, the path is accumulated in the sampled IR.
				  */
				DOPPLER_SORTING = (1 << 18),
				
				/// A flag indicating whether or not statistical information about the propagation/rendering systems should be output.
				/**
				  * If this flag is set and a corresponding statistics object is set in the request,
				  * the sound propagation system provides statistics about propaation paths,
				  * geometry, etc. to the user.
				  */
				STATISTICS = (1 << 30),
				
				/// A flag indicating whether or not debug drawing information should be output.
				/**
				  * If this flag is set and a corresponding debug drawing cache is set in the request,
				  * the sound propagator provides debug information about propaation paths,
				  * geometry, etc. to the user.
				  */
				DEBUG = (1 << 31),
				
				/// The default flags to use for sound propagation.
				DEFAULT = DIRECT | DIFFRACTION | SPECULAR | SPECULAR_CACHE |
						DIFFUSE | IR_CACHE | VISIBILITY_CACHE |
						AIR_ABSORPTION | SAMPLED_IR | DOPPLER_SORTING | IR_THRESHOLD | ADAPTIVE_IR_LENGTH |
						ADAPTIVE_QUALITY | SOURCE_DIRECTIVITY,
				
				/// The flag value when all flags are not set.
				UNDEFINED = 0
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound propagation flags object with no flags set.
			GSOUND_INLINE PropagationFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new sound propagation flags object with the specified flag value initially set.
			GSOUND_INLINE PropagationFlags( Flag flag )
				:	flags( flag )
			{
			}
			
			
			/// Create a new sound propagation flags object with the specified initial combined flags value.
			GSOUND_INLINE PropagationFlags( UInt32 newFlags )
				:	flags( newFlags )
			{
			}
			
			
		//********************************************************************************
		//******	Integer Cast Operator
			
			
			/// Convert this flags object to an integer value.
			/**
			  * This operator is provided so that the object
			  * can be used as an integer value for bitwise logical operations.
			  */
			GSOUND_INLINE operator UInt32 () const
			{
				return flags;
			}
			
			
		//********************************************************************************
		//******	Name Value Status Accessor Methods
			
			
			/// Return whether or not the specified flag value is set for this flags object.
			GSOUND_INLINE Bool isSet( Flag flag ) const
			{
				return (flags & flag) != UNDEFINED;
			}
			
			
			/// Set whether or not the specified flag value is set for this flags object.
			GSOUND_INLINE void set( Flag flag, Bool newIsSet )
			{
				if ( newIsSet )
					flags |= flag;
				else
					flags &= ~flag;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A value indicating the flags that are set.
			UInt32 flags;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PROPAGATION_FLAGS_H
