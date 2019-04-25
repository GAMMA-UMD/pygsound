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

#ifndef INCLUDE_OM_SOUND_STREAM_PLAYER_H
#define INCLUDE_OM_SOUND_STREAM_PLAYER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that handles complex playback of a streaming sound source.
/**
  * This class takes a pointer to a SoundInputStream and can then play the sound
  * provided by that stream. The player supports basic start-to-stop playback, looping
  * playback (if the stream allows it), and continuous playback from an infinite stream.
  */
class StreamPlayer : public SoundFilter, public SoundInputStream
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sound stream player without any stream to play.
			/**
			  * The constructed object will not produce any output until it has a 
			  * valid SoundInputStream.
			  */
			StreamPlayer();
			
			
			/// Create a sound stream player which plays from the specified sound input stream.
			/**
			  * If the supplied stream is NULL or invalid, the stream player produces no output.
			  * All playback and looping occurs relative to the initial position
			  * within the stream.
			  */
			StreamPlayer( const Shared<SoundInputStream>& newStream );
			
			
			/// Create a copy of this stream player and its state.
			StreamPlayer( const StreamPlayer& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a sound stream player and release all resources associated with it.
			~StreamPlayer();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of one sound stream player to another.
			StreamPlayer& operator = ( const StreamPlayer& other );
			
			
		//********************************************************************************
		//******	Stream Accessor Methods
			
			
			/// Return a const pointer to the SoundInputStream that is being used as a sound source.
			/**
			  * If there is no sound input stream set or if the stream is not valid, a NULL
			  * pointer is returned, indicating the problem.
			  */
			const SoundInputStream* getStream() const;
			
			
			/// Set the SoundInputStream which this player should use as a sound source.
			/**
			  * If the supplied pointer is NULL, the sound player is deactivated and doesn't
			  * produce any more audio. Otherwise, the player resets its current playback
			  * position to be the start of the sound and starts playback from the current position
			  * of the stream. Thus, all playback and looping occurs relative to the initial position
			  * within the stream.
			  */
			void setStream( const Shared<SoundInputStream>& newStream );
			
			
		//********************************************************************************
		//******	Playback Accessor Methods
			
			
			/// Return whether or not this sound player is current playing.
			Bool isPlaying() const;
			
			
			/// Set whether or not this sound player should be playing sound.
			/**
			  * The method returns whether or not playback will occurr, based on the type 
			  * of SoundInputStream which this player has and the requested playback
			  * state.
			  */
			Bool setIsPlaying( Bool newIsPlaying );
			
			
			/// Tell the sound player to start playing the sound from the current position.
			/**
			  * The method returns whether or not playback will occurr, based on the type 
			  * of SoundInputStream that this player has.
			  */
			Bool play();
			
			
			/// Stop playing the sound and keep the playhead at the last position.
			void stop();
			
			
			/// Reset the playback position to the first position within the stream.
			/**
			  * The method returns whether or not the rewind operation was successul.
			  * For SoundInputStream objects that don't allow seeking, this method
			  * will always fail. This method does not affect the playback state of the
			  * player, thus rewinding will cause the playback to jump to the beginning
			  * of the stream if the player is currently playing.
			  */
			Bool rewind();
			
			
		//********************************************************************************
		//******	Looping Accessor Methods
			
			
			/// Return whether or not this sound player is currently looping.
			/**
			  * If the underlying SoundInputStream for the sound player does not allow
			  * seeking within the stream, looping cannot occur.
			  */
			Bool isLooping() const;
			
			
			/// Set whether or not this sound player should try to loop its sound source.
			/**
			  * If the underlying SoundInputStream for the sound player does not allow
			  * seeking within the stream, looping cannot occur. Otherwise, the sound player
			  * loops the sound if the looping flag is set to TRUE.
			  *
			  * The method returns whether or not looping will occurr, based on the type 
			  * of SoundInputStream which this player is playing. This value is independent
			  * of the current playback state of the player.
			  */
			Bool setIsLooping( Bool newIsLooping );
			
			
		//********************************************************************************
		//******	Public Seek Status Accessor Methods
			
			
			/// Return whether or not seeking is allowed in this input stream.
			virtual Bool canSeek() const;
			
			
			/// Return whether or not this input stream's current position can be moved by the specified signed sample offset.
			virtual Bool canSeek( Int64 relativeSampleOffset ) const;
			
			
			/// Move the current sample frame position in the stream by the specified signed amount.
			virtual Int64 seek( Int64 relativeSampleOffset );
			
			
		//********************************************************************************
		//******	Stream Size Accessor Methods
			
			
			/// Return the number of samples remaining in the sound input stream.
			/**
			  * The value returned must only be a lower bound on the total number of sample
			  * frames in the stream. For instance, if there are samples remaining, the method
			  * should return at least 1. If there are no samples remaining, the method should
			  * return 0. This behavior is allowed in order to support never-ending streams
			  * which might not have a defined endpoint.
			  */
			virtual SoundSize getSamplesRemaining() const;
			
			
			/// Return the current position of the stream in samples relative to the start of the stream.
			/**
			  * The returned value indicates the sample index of the current read
			  * position within the sound stream. For unbounded streams, this indicates
			  * the number of samples that have been read by the stream since it was opened.
			  */
			virtual SampleIndex getPosition() const;
			
			
		//********************************************************************************
		//******	Stream Format Accessor Methods
			
			
			/// Return the number of channels that are in the sound input stream.
			virtual Size getChannelCount() const;
			
			
			/// Return the sample rate of the sound input stream's source audio data.
			virtual SampleRate getSampleRate() const;
			
			
			/// Return the actual sample type used in the stream.
			virtual SampleType getNativeSampleType() const;
			
			
		//********************************************************************************
		//******	Stream Status Accessor Method
			
			
			/// Return whether or not the stream has a valid source of sound data.
			virtual Bool isValid() const;
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this stream player.
			/**
			  * The method returns the string "Stream Player".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this stream player.
			/**
			  * The method returns the string "Stream Player".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this stream player.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::PLAYBACK.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this stream player can process audio data in-place.
			/**
			  * This method always returns TRUE, stream players can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this stream player.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this stream player.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this stream player.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Private Filter Processing Methods
			
			
			/// Play the specified number of samples from the sound input stream and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Read the specified number of samples from the input stream into the output buffer.
			virtual SoundResult readSamples( SoundBuffer& outputBuffer, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer to the sound input stream from which this sound's playback samples are read.
			Shared<SoundInputStream> stream;
			
			
			/// The current position within the stream, relative to the initial position within the stream.
			SampleIndex currentStreamPosition;
			
			
			/// The current maximum position that has been reached in the stream.
			/**
			  * This value allows the player to determine the total size of the stream
			  * indirectly by noting the positions within the sound stream where playback
			  * started and ended. The difference is the total length of the sound and it
			  * is used when looping the sound to determine how far to seek backwards in the
			  * stream.
			  */
			SoundSize currentStreamLength;
			
			
			/// A boolean value indicating whether or not the stream player should be playing the stream.
			Bool playingEnabled;
			
			
			/// A boolean value indicating whether or not the sound stream player should loop its sound source.
			Bool loopingEnabled;
			
			
			/// A boolean value indicating whether or not the sound stream supports seeking.
			Bool seekingAllowed;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_STREAM_PLAYER_H
