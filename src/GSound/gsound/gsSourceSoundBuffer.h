/*
 * Project:     GSound
 * 
 * File:        gsound/gsSourceSoundBuffer.h
 * Contents:    gsound::SourceSoundBuffer class declaration
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


#ifndef INCLUDE_GSOUND_SOURCE_SOUND_BUFFER_H
#define INCLUDE_GSOUND_SOURCE_SOUND_BUFFER_H


#include "gsConfig.h"


#include "gsSoundSource.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that contains buffers of input sound for all sources in a scene.
/**
  * A source sound buffer is used to pass the input audio for sound sources to
  * the listener renderer(s) that need it. This allows multiple renderers to be used
  * without synchronization problems. The input for each source is buffered here
  * by the sound propagation system and then passed to the renderers each time a buffer
  * of output sound is needed.
  */
class SourceSoundBuffer
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new source sound buffer with no sources.
			SourceSoundBuffer();
			
			
			/// Create a new source sound buffer that is a copy of another buffer.
			SourceSoundBuffer( const SourceSoundBuffer& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a source sound buffer, releasing all internal buffers.
			~SourceSoundBuffer();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another source sound buffer to this one.
			SourceSoundBuffer& operator = ( const SourceSoundBuffer& other );
			
			
		//********************************************************************************
		//******	Source Buffer Accessor Methods
			
			
			/// Return the number of sound sources that are stored in this buffer.
			GSOUND_FORCE_INLINE Size getSourceCount() const
			{
				return sources.getSize();
			}
			
			
			/// Return a pointer to the source with the given index.
			GSOUND_INLINE const SoundSource* getSource( Index sourceIndex ) const
			{
				return sources[sourceIndex];
			}
			
			
			/// Return a pointer to the sound buffer for the source with the given index.
			GSOUND_INLINE SoundBuffer* getSourceBuffer( Index sourceIndex )
			{
				return buffers[sourceIndex];
			}
			
			
			/// Return a const pointer to the sound buffer for the source with the given index.
			GSOUND_INLINE const SoundBuffer* getSourceBuffer( Index sourceIndex ) const
			{
				return buffers[sourceIndex];
			}
			
			
			/// Add a new sound source to this source sound buffer, creating a new buffer for the source if necessary.
			/**
			  * A pointer to the buffer for the source is returned, or NULL if there was an error.
			  */
			SoundBuffer* addSource( const SoundSource* source );
			
			
			/// Remove all sources and buffers from this source sound buffer.
			GSOUND_INLINE void clearSources()
			{
				sources.clear();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of pointers to sound sources, for which there is a 1-to-1 correspondence with buffers.
			ArrayList<const SoundSource*> sources;
			
			
			/// A list of pointers to sound buffers that correspond to sound sources.
			ArrayList<SoundBuffer*> buffers;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOURCE_SOUND_BUFFER_H
