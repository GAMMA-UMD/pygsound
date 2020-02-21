/*
 * Project:     GSound
 * 
 * File:        gsound/gsRenderFlags.h
 * Contents:    gsound::RenderFlags class declaration
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


#ifndef INCLUDE_GSOUND_RENDERING_FLAGS_H
#define INCLUDE_GSOUND_RENDERING_FLAGS_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies boolean configuration flags for sound propagation rendering.
class RenderFlags
{
	public:
		
		//********************************************************************************
		//******	Rendering Flags Enum Declaration
			
			
			/// An enum that specifies the different graphics context flags.
			enum Flag
			{
				/// A flag indicating whether or not convolution for sampled impulse responses should be performed.
				/**
				  * If this flag is set, the sound renderer enables frequency-domain block
				  * convolution of sampled impulse responses. This is the preferred mode of rendering.
				  *
				  * This flag generally should not be disabled except for debug purposes.
				  */
				CONVOLUTION = (1 << 0),
				
				/// A flag indicating whether or not discrete path rendering should be performed.
				/**
				  * If this flag is set, the sound renderer uses time-domain delay interpolation to render
				  * discrete sound paths that are part of the input impulse responses. This rendering
				  * mode is used for significant paths like direct sound, diffracted sound, and early
				  * reflections. The number of paths that can be rendered for each source
				  * can be limited by setting the RenderRequest::maxSourcePathCount.
				  * Paths that don't fit within this budget are added to the sampled IR and
				  * rendered using block convolution.
				  *
				  * This flag generally should not be disabled except for debug purposes.
				  */
				DISCRETE_PATHS = (1 << 1),
				
				/// A flag indicating whether or not head-related transfer function filtering is enabled.
				/**
				  * If this flag is set and the render request has a valid HRTF pointer, the
				  * renderer applies HRTF filtering to the listener's sound.
				  */
				HRTF = (1 << 2),
				
				/// A flag indicating whether or not artificial reverb is enabeld.
				/**
				  * If this flag is set, artificial reverb is rendered instead of the impulse response.
				  */
				REVERB = (1 << 3),
				
				/// A flag indicating whether or not analytical information about the rendering system should be output.
				/**
				  * If this flag is set and a corresponding statistics object is set in the request,
				  * the renderer provides debug information about the current renderer state to the user.
				  */
				STATISTICS = (1 << 30),
				
				/// The default flags to use for sound rendering.
				DEFAULT = CONVOLUTION | DISCRETE_PATHS,
				
				/// The flag value when all flags are not set.
				UNDEFINED = 0
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new rendering flags object with no flags set.
			GSOUND_INLINE RenderFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new rendering flags object with the specified flag value initially set.
			GSOUND_INLINE RenderFlags( Flag flag )
				:	flags( flag )
			{
			}
			
			
			/// Create a new rendering flags object with the specified initial combined flags value.
			GSOUND_INLINE RenderFlags( UInt32 newFlags )
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


#endif // INCLUDE_GSOUND_RENDERING_FLAGS_H
