/*
 * Project:     GSound
 * 
 * File:        gsound/gsSourceSoundBuffer.cpp
 * Contents:    gsound::SourceSoundBuffer class implementation
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


#include "gsSourceSoundBuffer.h"


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




SourceSoundBuffer:: SourceSoundBuffer()
{
}




SourceSoundBuffer:: SourceSoundBuffer( const SourceSoundBuffer& other )
	:	sources( other.sources.getSize() ),
		buffers( other.sources.getSize() )
{
	const Size numOtherSources = other.sources.getSize();
	
	for ( Index i = 0; i < numOtherSources; i++ )
	{
		sources.add( other.sources[i] );
		buffers.add( util::construct<SoundBuffer>( *other.buffers[i] ) );
	}
}




//##########################################################################################
//##########################################################################################
//############
//############		Destructor
//############
//##########################################################################################
//##########################################################################################




SourceSoundBuffer:: ~SourceSoundBuffer()
{
	for ( Index i = 0; i < buffers.getSize(); i++ )
		util::destruct( buffers[i] );
}




//##########################################################################################
//##########################################################################################
//############
//############		Assignment Operator
//############
//##########################################################################################
//##########################################################################################




SourceSoundBuffer& SourceSoundBuffer:: operator = ( const SourceSoundBuffer& other )
{
	if ( this != &other )
	{
		// Clear the previous sources.
		sources.clear();
		
		// Copy the other sources and buffers.
		const Size numOtherSources = other.sources.getSize();
		
		for ( Index i = 0; i < numOtherSources; i++ )
		{
			sources.add( other.sources[i] );
			
			if ( i >= buffers.getSize() )
				buffers.add( util::construct<SoundBuffer>( *other.buffers[i] ) );
			else
				*buffers[i] = *other.buffers[i];
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############
//############		Source Buffer Accessor Methods
//############
//##########################################################################################
//##########################################################################################




SoundBuffer* SourceSoundBuffer:: addSource( const SoundSource* source )
{
	sources.add( source );
	
	if ( sources.getSize() > buffers.getSize() )
		buffers.add( util::construct<SoundBuffer>() );
	
	return buffers[sources.getSize()-1];
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
