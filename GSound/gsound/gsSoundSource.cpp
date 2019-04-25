/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundSource.cpp
 * Contents:    gsound::SoundSource class implementation
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


#include "gsSoundSource.h"


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




SoundSource:: SoundSource()
	:	SoundDetector(),
		flags( SoundSourceFlags::DEFAULT ),
		power( 1 ),
		priority( 0 ),
		directivity( NULL ),
		sampler( ChannelLayout::MONO, 44100.0 ),
		filter( NULL )
{
	setPowerLevel( 70 );
}




SoundSource:: SoundSource( const Vector3f& newPosition, Real newRadius )
	:	SoundDetector( newPosition, newRadius ),
		flags( SoundSourceFlags::DEFAULT ),
		power( 1 ),
		priority( 0 ),
		directivity( NULL ),
		sampler( ChannelLayout::MONO, 44100.0 ),
		filter( NULL )
{
	setPowerLevel( 70 );
}




//##########################################################################################
//##########################################################################################
//############
//############		Sound Playing Methods
//############
//##########################################################################################
//##########################################################################################




Index SoundSource:: playSound( SoundInputStream* stream, Float gain, Bool loop, SoundFilter* insert )
{
	if ( stream == NULL )
		return 0;
	
	om::sound::SoundPlayer::Instance instance( stream );
	instance.gain = gain;
	instance.loop = loop;
	instance.insert = insert;
	
	return sampler.play( instance );
}




Bool SoundSource:: pauseSound( Index soundID )
{
	sampler.pause( soundID );
	return true;
}




Bool SoundSource:: resumeSound( Index soundID )
{
	sampler.resume( soundID );
	return true;
}




Bool SoundSource:: stopSound( Index soundID )
{
	return sampler.stop( soundID );
}




void SoundSource:: stopSounds()
{
	sampler.stopAll();
}




//##########################################################################################
//##########################################################################################
//############
//############		Sound Read Method
//############
//##########################################################################################
//##########################################################################################




Size SoundSource:: readSamples( SoundBuffer& outputBuffer, const Time& outputLength )
{
	// Determine how many samples to read from the source's stream.
	const SampleRate outputSampleRate = sampler.getOutputSampleRate();
	Size numSamples = (Size)math::ceiling( outputSampleRate*outputLength );
	
	if ( filter == NULL )
		return sampler.SoundInputStream::read( outputBuffer, numSamples );
	else if ( filter->allowsInPlaceProcessing() )
	{
		numSamples = sampler.SoundInputStream::read( outputBuffer, numSamples );
		
		// Process the output filter.
		return filter->process( outputBuffer, outputBuffer, numSamples );
	}
	else
	{
		SharedSoundBuffer sharedBuffer = SharedBufferPool::getGlobalBuffer( sampler.getChannelCount(), numSamples, outputSampleRate );
		
		numSamples = sampler.SoundInputStream::read( sharedBuffer.getBuffer(), numSamples );
		
		// Process the output filter.
		return filter->process( sharedBuffer.getBuffer(), outputBuffer, numSamples );
	}
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
