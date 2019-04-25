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

#ifndef INCLUDE_OM_SOUND_FRAME_H
#define INCLUDE_OM_SOUND_FRAME_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class SoundBuffer;
class MIDIBuffer;



//********************************************************************************
/// A class that provides all of the information needed for a SoundFilter to process sound data.
/**
  * Primarily, a sound frame holds an internal array of pointers to SoundBuffer objects
  * which represent the buffers for each filter input or output. These buffers are allowed
  * to be NULL, indicating that either the input at that index was not provided or
  * the output at that index is not needed.
  *
  * A sound frame uses a fixed-size internal array of pointers to SoundBuffer objects
  * but can also allocate a variable-sized array of buffer pointers if the capacity of
  * the fixed size array is exceeded. This is done for performance, so that an allocation
  * is not performed in most cases when using multiple buffers.
  *
  * A similar buffer scheme is used for MIDI data. All MIDI events that are part of a 
  * frame are specified with their timestamp relative to the main frame absolute timestamp,
  * rather than as absolute timestamps.
  *
  * Each sound frame has an associated absolute timestamp, measured relative to the
  * Epoch, 1970-01-01 00:00:00 +0000 (UTC). This allows the filter to detect breaks
  * in the sound timeline and react accordingly, or to synchronize sound processing,
  * such as with video.
  */
class SoundFrame
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new sound sound frame which has no buffers and the default capacity.
			OM_INLINE SoundFrame()
				:	buffers( bufferArray ),
					numBuffers( 0 ),
					bufferCapacity( FIXED_BUFFER_ARRAY_SIZE ),
					midiBuffers( midiBufferArray ),
					numMIDIBuffers( 0 ),
					midiBufferCapacity( FIXED_MIDI_BUFFER_ARRAY_SIZE ),
					time()
			{
			}
			
			
			/// Create a new sound sound frame which has the specified number of buffers (initially all NULL).
			OM_INLINE SoundFrame( Size newNumBuffers )
				:	numBuffers( (UInt16)newNumBuffers ),
					bufferCapacity( (UInt16)math::max( newNumBuffers, FIXED_BUFFER_ARRAY_SIZE ) ),
					midiBuffers( midiBufferArray ),
					numMIDIBuffers( 0 ),
					midiBufferCapacity( FIXED_MIDI_BUFFER_ARRAY_SIZE ),
					time()
			{
				if ( bufferCapacity > FIXED_BUFFER_ARRAY_SIZE )
					buffers = om::util::allocate<SoundBuffer*>( bufferCapacity );
				else
					buffers = bufferArray;
				
				this->setBuffersToNull();
			}
			
			
			/// Create a new sound sound frame which wraps a single SoundBuffer object pointer.
			OM_INLINE SoundFrame( SoundBuffer* newBuffer )
				:	buffers( bufferArray ),
					numBuffers( 1 ),
					bufferCapacity( FIXED_BUFFER_ARRAY_SIZE ),
					midiBuffers( midiBufferArray ),
					numMIDIBuffers( 0 ),
					midiBufferCapacity( FIXED_MIDI_BUFFER_ARRAY_SIZE ),
					time()
			{
				buffers[0] = newBuffer;
			}
			
			
			/// Create a new sound sound frame which wraps two SoundBuffer object pointers.
			OM_INLINE SoundFrame( SoundBuffer* newBuffer1, SoundBuffer* newBuffer2 )
				:	buffers( bufferArray ),
					numBuffers( 2 ),
					bufferCapacity( FIXED_BUFFER_ARRAY_SIZE ),
					midiBuffers( midiBufferArray ),
					numMIDIBuffers( 0 ),
					midiBufferCapacity( FIXED_MIDI_BUFFER_ARRAY_SIZE ),
					time()
			{
				buffers[0] = newBuffer1;
				buffers[1] = newBuffer2;
			}
			
			
			/// Create an exact copy of the specified sound frame, copying all of its buffer pointers.
			SoundFrame( const SoundFrame& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound sound frame and release all of its resources.
			OM_INLINE ~SoundFrame()
			{
				// Deallocate the buffer array if necessary.
				if ( buffers != bufferArray )
					om::util::deallocate( buffers );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of the specified sound frame, including its buffer pointers.
			SoundFrame& operator = ( const SoundFrame& other );
			
			
		//********************************************************************************
		//******	Buffer Accessor Methods
			
			
			/// Get the number of buffers that this sound frame contains.
			OM_INLINE Size getBufferCount() const
			{
				return numBuffers;
			}
			
			
			/// Change the size of the internal buffer array, padding any new buffer pointers with NULL.
			/**
			  * If the number of buffers is increasing, the new buffer pointers for the
			  * sound frame are set to NULL. Otherwise, if the number is decreasing, the extra
			  * buffer pointers are discarded.
			  */
			OM_INLINE void setBufferCount( Size newNumBuffers )
			{
				// Resize the buffer array if necessary.
				if ( newNumBuffers > bufferCapacity )
					this->reallocateBuffers( newNumBuffers );
				
				// Set all extra buffer pointers to NULL.
				for ( Index i = numBuffers; i < newNumBuffers; i++ )
					buffers[i] = NULL;
				
				numBuffers = (UInt16)newNumBuffers;
			}
			
			
			/// Return the SoundBuffer within this sound frame at the specified index.
			OM_INLINE SoundBuffer* getBuffer( Index bufferIndex )
			{
				return buffers[bufferIndex];
			}
			
			
			/// Return the SoundBuffer within this sound frame at the specified index.
			OM_INLINE const SoundBuffer* getBuffer( Index bufferIndex ) const
			{
				return buffers[bufferIndex];
			}
			
			
			/// Replace the SoundBuffer pointer at the specified index with a new pointer.
			OM_INLINE void setBuffer( Index bufferIndex, SoundBuffer* newBuffer )
			{
				buffers[bufferIndex] = newBuffer;
			}
			
			
			/// Add the specified buffer pointer to the end of this frame's list of buffers.
			OM_INLINE void addBuffer( SoundBuffer* newBuffer )
			{
				if ( numBuffers == bufferCapacity )
					this->reallocateBuffers( bufferCapacity*2 );
				
				buffers[numBuffers] = newBuffer;
				numBuffers++;
			}
			
			
			/// Insert the specified buffer pointer at the specified index in this frame's list of buffers.
			OM_INLINE void insertBuffer( Index newBufferIndex, SoundBuffer* newBuffer )
			{
				if ( numBuffers == bufferCapacity )
					this->reallocateBuffers( bufferCapacity*2 );
				
				for ( Index i = numBuffers; i > newBufferIndex; i-- )
					buffers[i] = buffers[i - 1];
				
				buffers[newBufferIndex] = newBuffer;
				numBuffers++;
			}
			
			
			/// Remove the buffer from this sound frame at the specified index.
			/**
			  * This method shifts all buffer pointers after the specified index
			  * back by one index to replace the removed buffer.
			  */
			OM_INLINE void removeBuffer( Index bufferIndex )
			{
				numBuffers--;
				
				for ( Index i = bufferIndex; i < numBuffers; i++ )
					buffers[i] = buffers[i + 1];
			}
			
			
			/// Remove all buffers from this sound frame.
			OM_INLINE void clearBuffers()
			{
				numBuffers = 0;
			}
			
			
			/// Keep the current number of valid buffers the same, but set all buffer pointers to NULL.
			OM_INLINE void setBuffersToNull()
			{
				for ( Index i = 0; i < numBuffers; i++ )
					buffers[i] = NULL;
			}
			
			
		//********************************************************************************
		//******	MIDI Buffer Accessor Methods
			
			
			/// Get the number of MIDI buffers that this sound frame contains.
			OM_INLINE Size getMIDIBufferCount() const
			{
				return numMIDIBuffers;
			}
			
			
			/// Change the size of the internal MIDI buffer array, padding any new buffer pointers with NULL.
			/**
			  * If the number of MIDI buffers is increasing, the new buffer pointers for the
			  * sound frame are set to NULL. Otherwise, if the number is decreasing, the extra
			  * buffer pointers are discarded.
			  */
			OM_INLINE void setMIDIBufferCount( Size newNumBuffers )
			{
				// Resize the buffer array if necessary.
				if ( newNumBuffers > midiBufferCapacity )
					this->reallocateMIDIBuffers( newNumBuffers );
				
				// Set all extra buffer pointers to NULL.
				for ( Index i = numMIDIBuffers; i < newNumBuffers; i++ )
					midiBuffers[i] = NULL;
				
				numMIDIBuffers = (UInt16)newNumBuffers;
			}
			
			
			/// Return a pointer to the MIDI buffer within this sound frame at the specified index.
			OM_INLINE MIDIBuffer* getMIDIBuffer( Index bufferIndex )
			{
				return midiBuffers[bufferIndex];
			}
			
			
			/// Return the MIDI buffer within this sound frame at the specified index.
			OM_INLINE const MIDIBuffer* getMIDIBuffer( Index bufferIndex ) const
			{
				return midiBuffers[bufferIndex];
			}
			
			
			/// Replace the MIDI buffer pointer at the specified index with a new pointer.
			OM_INLINE void setMIDIBuffer( Index bufferIndex, MIDIBuffer* newBuffer )
			{
				midiBuffers[bufferIndex] = newBuffer;
			}
			
			
			/// Add the specified MIDI buffer pointer to the end of this frame's list of MIDI buffers.
			OM_INLINE void addMIDIBuffer( MIDIBuffer* newBuffer )
			{
				if ( numMIDIBuffers == midiBufferCapacity )
					this->reallocateMIDIBuffers( midiBufferCapacity*2 );
				
				midiBuffers[numMIDIBuffers] = newBuffer;
				numMIDIBuffers++;
			}
			
			
			/// Insert the specified MIDI buffer pointer at the specified index in this frame's list of MIDI buffers.
			OM_INLINE void insertMIDIBuffer( Index newBufferIndex, MIDIBuffer* newBuffer )
			{
				if ( numMIDIBuffers == midiBufferCapacity )
					this->reallocateMIDIBuffers( midiBufferCapacity*2 );
				
				for ( Index i = numMIDIBuffers; i > newBufferIndex; i-- )
					midiBuffers[i] = midiBuffers[i - 1];
				
				midiBuffers[newBufferIndex] = newBuffer;
				numMIDIBuffers++;
			}
			
			
			/// Remove the MIDI buffer from this sound frame at the specified index.
			/**
			  * This method shifts all MIDI buffer pointers after the specified index
			  * back by one index to replace the removed buffer.
			  */
			OM_INLINE void removeMIDIBuffer( Index bufferIndex )
			{
				numMIDIBuffers--;
				
				for ( Index i = bufferIndex; i < numMIDIBuffers; i++ )
					midiBuffers[i] = midiBuffers[i + 1];
			}
			
			
			/// Remove all MIDI buffers from this sound frame.
			OM_INLINE void clearMIDIBuffers()
			{
				numMIDIBuffers = 0;
			}
			
			
			/// Keep the current number of valid MIDI buffers the same, but set all buffer pointers to NULL.
			OM_INLINE void setMIDIBuffersToNull()
			{
				for ( Index i = 0; i < numMIDIBuffers; i++ )
					midiBuffers[i] = NULL;
			}
			
			
		//********************************************************************************
		//******	Copy Methods
			
			
			/// Copy this sound sound frame's entire data to another frame, replacing its data.
			/**
			  * This method is provided as a convenient way to pass through MIDI and audio from an input
			  * frame to an output frame if the filter doesn't need to modify the data.
			  *
			  * The method iterates over the MIDI and sound buffers in this frame and copies their contents
			  * to the corresponding buffers of the other frame if it exists, replacing
			  * the previous contents of the other buffer.
			  */
			void copyTo( SoundFrame& other ) const;
			
			
			/// Copy this sound sound frame's MIDI data to another frame, replacing its MIDI data.
			/**
			  * This method is provided as a convenient way to pass through MIDI from an input
			  * frame to an output frame if the filter doesn't need to modify the MIDI data.
			  *
			  * The method iterates over the MIDI buffers in this frame and copies their contents
			  * to the corresponding MIDI buffer of the other frame if it exists, replacing
			  * the previous contents of the other buffer.
			  */
			void copyMIDITo( SoundFrame& other ) const;
			
			
		//********************************************************************************
		//******	Frame Time Accessor Methods
			
			
			/// Return the absolute time of the start of this sound frame.
			/**
			  * This is measured relative to the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  */
			OM_INLINE const Time& getTime() const
			{
				return time;
			}
			
			
			/// Set the absolute time of the start of this sound frame.
			/**
			  * This is measured relative to the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  */
			OM_INLINE void setTime( const Time& newTime )
			{
				time = newTime;
			}
			
			
		//********************************************************************************
		//******	Frame Limit Accessor Methods
			
			
			/// Return the maximum number of sound buffers that a sound sound frame can have.
			OM_INLINE static Size getMaximumNumberOfBuffers()
			{
				return Size(math::max<UInt16>());
			}
			
			
			/// Return the maximum number of MIDI buffers that a sound sound frame can have.
			OM_INLINE static Size getMaximumNumberOfMIDIBuffers()
			{
				return Size(math::max<UInt16>());
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Method
			
			
			/// Double the capacity of the internal buffer array, copying the old buffer pointers.
			void reallocateBuffers( Size newCapacity );
			
			
			/// Double the capacity of the internal MIDI buffer array, copying the old buffer pointers.
			void reallocateMIDIBuffers( Size newCapacity );
			
			
			/// A helper method that copies all of the MIDI data to another sound frame.
			/**
			  * This allows the fast parts of copyMIDITo() to be inlined, while the longer
			  * code for copying (if there are any MIDI events) is in this method.
			  */
			void copyMIDIToInternal( SoundFrame& other ) const;
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// Define the size of the fixed-size array of sound buffers that is part of a SoundFrame.
			static const Size FIXED_BUFFER_ARRAY_SIZE = 2;
			
			
			/// Define the size of the fixed-size array of MIDI buffers that is part of a SoundFrame.
			static const Size FIXED_MIDI_BUFFER_ARRAY_SIZE = 1;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to an array of SoundBuffer pointers which represent the buffers for this frame.
			SoundBuffer** buffers;
			
			
			/// A pointer to an array of MIDIBuffer pointers which represent the MIDI buffers for this frame.
			MIDIBuffer** midiBuffers;
			
			
			/// The number of buffers that this sound frame has.
			UInt16 numBuffers;
			
			
			/// The maximum number of buffers that this sound frame can hold.
			UInt16 bufferCapacity;
			
			
			/// The number of MIDI buffers that this sound frame has.
			UInt16 numMIDIBuffers;
			
			
			/// The maximum number of MIDI buffers that this sound frame can hold.
			UInt16 midiBufferCapacity;
			
			
			/// The absolute time of the start of this sound frame.
			/**
			  * This is measured relative to the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
			  */
			Time time;
			
			
			/// A fixed-size array of buffers that are part of the frame object to avoid excessive allocations.
			SoundBuffer* bufferArray[FIXED_BUFFER_ARRAY_SIZE];
			
			
			/// A fixed-size array of MIDI buffers that are part of the frame object to avoid excessive allocations.
			MIDIBuffer* midiBufferArray[FIXED_MIDI_BUFFER_ARRAY_SIZE];
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FRAME_H
