/*
 *  rimSoundSharedBufferPool.h
 *  Rim Sound
 *
 *  Created by Carl Schissler on 12/1/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_OM_SOUND_SHARED_BUFFER_POOL_H
#define INCLUDE_OM_SOUND_SHARED_BUFFER_POOL_H


#include "omSoundBaseConfig.h"


#include "omSoundBuffer.h"
#include "omSoundSharedBufferInfo.h"
#include "omSoundSharedSoundBuffer.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that provides a pool of thread-safe temporary SoundBuffer objects for efficient DSP processing.
/**
  * Often when doing DSP, a temporary buffer of sound samples is needed for intermediate
  * processing. This class provides a way for DSP classes to access a reference to a
  * temporary buffer that is shared among them. These buffers are locked for use when requested
  * and unlocked when the returned SharedSoundBuffer goes out of scope.
  *
  * When requesting a buffer, the user can specify the attributes of that buffer,
  * and the buffer pool will return a buffer (creating one if necessary) that matches
  * those characteristics.
  */
class SharedBufferPool
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new empty shared buffer pool.
			OM_INLINE SharedBufferPool()
			{
			}
			
			
		//********************************************************************************
		//******	Buffer Accessor Methods
			
			
			/// Return a handle to a shared sound buffer.
			/**
			  * This method looks at the available buffers in the pool and returns the first one,
			  * ignoring its format. It then locks that buffer and returns a handle to it.
			  * When that handle is destructed, the buffer is reclaimed by the pool and can no longer be used.
			  */
			SharedSoundBuffer getBuffer();
			
			
			/// Return a handle to a shared sound buffer with the specified attributes.
			/**
			  * This method looks at the available buffers in the pool and picks one that
			  * matches the requested number of channels, number of samples, and sample rate.
			  * It then locks that buffer and returns a handle to it. When that handle is destructed,
			  * the buffer is reclaimed by the pool and can no longer be used.
			  */
			SharedSoundBuffer getBuffer( Size numChannels, Size numSamples, SampleRate sampleRate );
			
			
		//********************************************************************************
		//******	Pool Management Methods
			
			
			/// Clear all buffers from this buffer pool that are not in use.
			void reset();
			
			
		//********************************************************************************
		//******	Global Buffer Accessor Methods
			
			
			/// Return a handle to a shared global sound buffer.
			OM_INLINE static SharedSoundBuffer getGlobalBuffer()
			{
				return staticPool->getBuffer();
			}
			
			
			/// Return a handle to a shared global sound buffer with the specified attributes.
			OM_INLINE static SharedSoundBuffer getGlobalBuffer( Size numChannels, Size numSamples, SampleRate sampleRate )
			{
				return staticPool->getBuffer( numChannels, numSamples, sampleRate );
			}
			
			
			/// Clear all buffers from the global buffer pool that are not in use.
			OM_INLINE static void globalReset()
			{
				staticPool->reset();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// A pointer to a global shared buffer pool.
			static SharedBufferPool* staticPool;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the buffers that are a part of this shared buffer pool.
			ArrayList<SharedBufferInfo*> buffers;
			
			
			/// A mutex which prevents concurrent access to the list of shared buffers.
			Mutex bufferMutex;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SHARED_BUFFER_POOL_H
