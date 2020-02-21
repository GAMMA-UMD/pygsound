/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundPathFlags.h
 * Contents:    gsound::SoundPathFlags class declaration
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

#ifndef INCLUDE_GSOUND_SOUND_PATH_FLAGS_H
#define INCLUDE_GSOUND_SOUND_PATH_FLAGS_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies boolean information about a SoundPath.
/**
  * These flags allow the user to determine what kind of sound paths are
  * returned from the propagation system. For instance, the user could treat
  * direct paths differently than other paths by inspecting the path flags.
  */
class SoundPathFlags
{
	public:
		
		//********************************************************************************
		//******	Sound Path Flags Enum Declaration
			
			
			/// An enum that specifies the different sound path flags.
			enum Flag
			{
				/// A flag indicating that the path represents direct sound.
				DIRECT = (1 << 0),
				
				/// A flag indicating that the path represents transmitted sound.
				TRANSMISSION = (1 << 1),
				
				/// A flag indicating that the path represents specularly reflected sound.
				SPECULAR = (1 << 2),
				
				/// A flag indicating that the path represents diffusely reflected sound.
				DIFFUSE = (1 << 3),
				
				/// A flag indicating that the path represents diffracted sound.
				DIFFRACTION = (1 << 4),
				
				/// The flag value when no flags are set for the path.
				UNDEFINED = 0
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound path flags object with no flags set.
			GSOUND_INLINE SoundPathFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new sound path flags object with the specified flag value initially set.
			GSOUND_INLINE SoundPathFlags( Flag flag )
				:	flags( flag )
			{
			}
			
			
			/// Create a new sound path flags object with the specified initial combined flags value.
			GSOUND_INLINE SoundPathFlags( UInt32 newFlags )
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
		//******	Flag Is Set Accessor Methods
			
			
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


#endif // INCLUDE_GSOUND_SOUND_PATH_FLAGS_H
