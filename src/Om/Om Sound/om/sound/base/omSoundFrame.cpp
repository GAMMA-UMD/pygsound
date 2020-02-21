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

#include "omSoundFrame.h"


#include "omSoundBuffer.h"
#include "omSoundMIDIBuffer.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Copy Constructor
//############		
//##########################################################################################
//##########################################################################################




SoundFrame:: SoundFrame( const SoundFrame& other )
	:	numBuffers( other.numBuffers ),
		bufferCapacity( other.bufferCapacity ),
		numMIDIBuffers( other.numMIDIBuffers ),
		midiBufferCapacity( other.midiBufferCapacity ),
		time( other.time )
{
	// Copy the sound buffers.
	if ( bufferCapacity > FIXED_BUFFER_ARRAY_SIZE )
		buffers = om::util::allocate<SoundBuffer*>( bufferCapacity );
	else
		buffers = bufferArray;
	
	for ( Index i = 0; i < numBuffers; i++ )
		buffers[i] = other.buffers[i];
	
	// Copy the MIDI buffers.
	if ( midiBufferCapacity > FIXED_MIDI_BUFFER_ARRAY_SIZE )
		midiBuffers = om::util::allocate<MIDIBuffer*>( midiBufferCapacity );
	else
		midiBuffers = midiBufferArray;
	
	for ( Index i = 0; i < numMIDIBuffers; i++ )
		midiBuffers[i] = other.midiBuffers[i];
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




SoundFrame& SoundFrame:: operator = ( const SoundFrame& other )
{
	if ( this != &other )
	{
		// Copy the timestamp for the frame.
		time = other.time;
		
		//**********************************************************************
		// Copy sound buffers.
		
		if ( other.numBuffers > bufferCapacity )
		{
			// Clean up the old buffer array if necessary.
			if ( buffers != bufferArray )
				om::util::deallocate( buffers );
			
			// Use the other frame's buffer capacity.
			bufferCapacity = other.bufferCapacity;
			buffers = om::util::allocate<SoundBuffer*>( bufferCapacity );
		}
		
		numBuffers = other.numBuffers;
		
		// Copy the other buffer pointers.
		for ( Index i = 0; i < numBuffers; i++ )
			buffers[i] = other.buffers[i];
		
		//**********************************************************************
		// Copy MIDI buffers.
		
		if ( other.numMIDIBuffers > midiBufferCapacity )
		{
			// Clean up the old buffer array if necessary.
			if ( midiBuffers != midiBufferArray )
				om::util::deallocate( midiBuffers );
			
			// Use the other frame's buffer capacity.
			midiBufferCapacity = other.midiBufferCapacity;
			midiBuffers = om::util::allocate<MIDIBuffer*>( midiBufferCapacity );
		}
		
		numMIDIBuffers = other.numMIDIBuffers;
		
		// Copy the other buffer pointers.
		for ( Index i = 0; i < numMIDIBuffers; i++ )
			midiBuffers[i] = other.midiBuffers[i];
		
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Copy Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundFrame:: copyTo( SoundFrame& other ) const
{
	if ( this == &other )
		return;
	
	Size buffersToCopy = math::min( numBuffers, other.numBuffers );
	
	for ( Index i = 0; i < buffersToCopy; i++ )
	{
		const SoundBuffer* source = buffers[i];
		SoundBuffer* destination = other.buffers[i];
		
		if ( source )
		{
			if ( destination )
				source->copyTo( *destination );
		}
		else if ( destination )
			destination->zero();
	}
	
	this->copyMIDITo( other );
}




void SoundFrame:: copyMIDITo( SoundFrame& other ) const
{
	if ( this == &other )
		return;
	
	Size buffersToCopy = math::min( numMIDIBuffers, other.numMIDIBuffers );
	
	for ( Index i = 0; i < buffersToCopy; i++ )
	{
		const MIDIBuffer* source = midiBuffers[i];
		MIDIBuffer* destination = other.midiBuffers[i];
		
		if ( source )
		{
			if ( destination )
				source->copyTo( *destination );
		}
		else if ( destination )
			destination->clearEvents();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Reallocation Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundFrame:: reallocateBuffers( Size newCapacity )
{
	// Increase the capacity of the new buffer array.
	bufferCapacity = (UInt16)newCapacity;
	
	// Allocate a new buffer pointer.
	SoundBuffer** newBuffers = om::util::allocate<SoundBuffer*>( bufferCapacity );
	
	// Copy the old buffer pointers.
	om::util::copy( newBuffers, buffers, numBuffers );
	
	// Replace the old buffers, deallocating if necessary.
	if ( buffers != bufferArray )
		om::util::deallocate( buffers );
	
	buffers = newBuffers;
}




void SoundFrame:: reallocateMIDIBuffers( Size newCapacity )
{
	// Increase the capacity of the new buffer array.
	midiBufferCapacity = (UInt16)newCapacity;
	
	// Allocate a new buffer pointer.
	MIDIBuffer** newMIDIBuffers = om::util::allocate<MIDIBuffer*>( midiBufferCapacity );
	
	// Copy the old buffer pointers.
	om::util::copy( newMIDIBuffers, midiBuffers, numMIDIBuffers );
	
	// Replace the old buffers, deallocating if necessary.
	if ( midiBuffers != midiBufferArray )
		om::util::deallocate( midiBuffers );
	
	midiBuffers = newMIDIBuffers;
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
