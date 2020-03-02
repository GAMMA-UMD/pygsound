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

#ifndef INCLUDE_OM_SOUND_SAMPLER_H
#define INCLUDE_OM_SOUND_SAMPLER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"
#include "omSoundResampler.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that allows the user to trigger multiple sound stream to be played.
/**
  * These triggered streams are then played back using the provided parameters
  * and mixed together at the output of the player. The streams are automatically
  * mapped to the output channel layout and sample rate converted to the output
  * sample rate if necessary.
  */
class SoundPlayer : public SoundFilter, public SoundInputStream
{
	public:
		
		//********************************************************************************
		//******	Public Type Declarations
			
			
			/// The type to use to represent the ID of a instance that is currently playing in the player.
			typedef Index InstanceID;
			
			
			/// A class that contains information about a single playback instance for a sound player.
			class Instance
			{
				public:
					
					//********************************************************************************
					//******	Constructors
						
						
						/// Create a new default playback instance without a valid sound resource.
						OM_INLINE Instance()
							:	stream( NULL ),
								insert( NULL ),
								start(),
								length(),
								fadeInTime(),
								fadeOutTime(),
								pan(),
								speed( 1 ),
								gain( 1 ),
								priority( 0 ),
								loopCount( 0 ),
								userID( 0 ),
								loop( false )
						{
						}
						
						
						/// Create a new default playback instance with the specified sound stream.
						OM_INLINE Instance( SoundInputStream* newStream )
							:	stream( newStream ),
								insert( NULL ),
								start(),
								length(),
								fadeInTime(),
								fadeOutTime(),
								pan(),
								speed( 1 ),
								gain( 1 ),
								priority( 0 ),
								loopCount( 0 ),
								userID( 0 ),
								loop( false )
						{
						}
						
						
					//********************************************************************************
					//******	Public Data Members
						
						
						/// A pointer to the sound stream that this instance uses for sound data.
						SoundInputStream* stream;
						
						
						/// A filter that should be used to process the audio for the instance.
						SoundFilter* insert;
						
						
						/// The starting playback position in the sound resource of this instance.
						Time start;
						
						
						/// The length of time to play from the sound stream.
						/**
						  * A value of zero indicates the entire sound should be played.
						  */
						Time length;
						
						
						/// The length of time that should be used to fade in the sound stream to its full volume.
						/**
						  * A value of zero indicates there is no fading performed.
						  */
						Time fadeInTime;
						
						
						/// The length of time that should be used to fade out the sound stream from its full volume to silence.
						/**
						  * A value of zero indicates there is no fading performed.
						  */
						Time fadeOutTime;
						
						
						/// The panning direction that should be used when playing this instance.
						PanDirection pan;
						
						
						/// A scale factor for the playback speed, where 1 is the normal speed.
						Float speed;
						
						
						/// A linear gain factor for the sound from this playback instance.
						Float gain;
						
						
						/// A value that indicates the priority for the playback instance.
						Float priority;
						
						
						/// The number of times that the stream should be repeated after the first playback if looping is enabled.
						/**
						  * A value of 0 indicates that the stream should be looped indefinitely.
						  */
						Size loopCount;
						
						
						/// A user defined identifier for this playback instance.
						Index userID;
						
						
						/// A boolean value indicating whether or not looping is enabled for this instance.
						Bool loop;
						
						
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default player which has no sounds currently playing.
			/**
			  * The player has the default stereo output channel layout and output sample rate of
			  * 44100 kHz.
			  */
			SoundPlayer();
			
			
			/// Create a new player which has no sounds currently playing and the specified attributes.
			/**
			  * This constructor allows the user to specify the output channel layout for the player,
			  * the output sample rate, and the maximum number of simultaneously playing instances.
			  *
			  * If the specified sample rate is invalid, the default sample rate of
			  * 44100 kHz is used instead.
			  */
			SoundPlayer( const ChannelLayout& newOutputChannelLayout, SampleRate newSampleRate = SampleRate(44100),
						Size newMaxInstanceCount = Size(100) );
			
			
			/// Create a copy of the specified player with all stream playback state exactly the same.
			SoundPlayer( const SoundPlayer& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this player and any associated resources.
			~SoundPlayer();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the entire state of another player to this one, including stream playback state.
			SoundPlayer& operator = ( const SoundPlayer& other );
			
			
		//********************************************************************************
		//******	Instance Count Limit Accessor Methods
			
			
			/// Return the maximum number of simultaneous playback instances that this player is allowed to play.
			/**
			  * If an attempt is made to play an instance using a player that is at its maximum capacity will
			  * cause the player to replace the previously playing stream with the lowest priority
			  * and/or the oldest age.
			  */
			OM_INLINE Size getMaxInstanceCount() const
			{
				return maxInstanceCount;
			}
			
			
			/// Set the maximum number of simultaneous playback instances that this player is allowed to play.
			/**
			  * If an attempt is made to play an instance using a player that is at its maximum capacity will
			  * cause the player to replace the previously playing stream with the lowest priority
			  * and/or the oldest age.
			  */
			OM_INLINE void setMaxInstanceCount( Size newMaxStreamCount )
			{
				maxInstanceCount = newMaxStreamCount;
			}
			
			
		//********************************************************************************
		//******	Instance Accessor Methods
			
			
			/// Return the total number of streams that are currently playing in this player.
			OM_INLINE Size getInstanceCount() const
			{
				return numPlaying;
			}
			
			
			/// Return a reference to the instance data for the playback instance with the given ID.
			OM_INLINE const Instance& getInstance( InstanceID instanceID ) const
			{
				return instances[instanceID-1].instance;
			}
			
			
			/// Play a sound playback instance with the specified instance data.
			/**
			  * The method returns 0 if playing the stream failed, otherwise, the returned
			  * integer indicates an ID for the new playback instance which can be used to modify
			  * its parameters during playback.
			  */
			InstanceID play( const Instance& instance );
			
			
			/// Return whether or not the stream with the specified ID is currently playing.
			OM_INLINE Bool isPlaying( InstanceID instanceID ) const
			{
				return instances[instanceID-1].playing;
			}
			
			
			/// Update the playback state for the instance with the specified ID.
			Bool update( InstanceID instanceID, const Instance& instance );
			
			
			/// Pause playback for the instance with the specified ID.
			Bool pause( InstanceID instanceID );
			
			
			/// Resume playback for the previously paused instance with the specified ID.
			Bool resume( InstanceID instanceID );
			
			
			/// Stop playback for the instance with the specified ID and remove it from the player.
			Bool stop( InstanceID instanceID );
			
			
			/// Pause playback for all currently playing instances in this player.
			void pauseAll();
			
			
			/// Resume playback for all currently paused instances in this player.
			void resumeAll();
			
			
			/// Remove and stop playing all currently playing streams from this player.
			void stopAll();
			
			
		//********************************************************************************
		//******	User Instance Accessor Methods
			
			
			/// Update playback for all instances that have the specified user ID.
			Bool updateUserID( Index userID, const Instance& instance );
			
			
			/// Pause playback for all instances that have the specified user ID.
			Bool pauseUserID( Index userID );
			
			
			/// Resume playback for all instances that have the specified user ID.
			Bool resumeUserID( Index userID );
			
			
			/// Stop playing all instances that have the specified user ID.
			Bool stopUserID( Index userID );
			
			
		//********************************************************************************
		//******	Output Channel Layout Accessor Methods
			
			
			/// Return a reference to an object that describes the output channel format for this player.
			OM_INLINE const ChannelLayout& getOutputChannelLayout() const
			{
				return outputChannelLayout;
			}
			
			
			/// Change the output channel format for this player.
			void setOutputChannelLayout( const ChannelLayout& newChannelLayout );
			
			
		//********************************************************************************
		//******	Output Sample Rate Accessor Methods
			
			
			/// Return the output sample rate for this player.
			/**
			  * The default sample rate is 44100 kHz.
			  */
			OM_INLINE SampleRate getOutputSampleRate() const
			{
				return outputSampleRate;
			}
			
			
			/// Set the output sample rate for this player.
			/**
			  * If the new sample rate is less than or equal to 0, the method
			  * fails and has no effect.
			  * The default sample rate is 44100 kHz.
			  */
			void setOutputSampleRate( SampleRate newSampleRate );
			
			
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
			
			
			/// Return a human-readable name for this player.
			/**
			  * The method returns the string "SoundPlayer".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this player.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this player.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::PLAYBACK.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this player can process audio data in-place.
			/**
			  * This method always returns TRUE, player can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this player.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this player.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this player.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that holds information about a playback instance.
			class InstanceInfo
			{
				public:
					
					/// Create a new instance information object for the specified instance.
					OM_INLINE InstanceInfo( const Instance& newInstance )
						:	instance( newInstance ),
							targetGain( newInstance.gain ),
							resampler( NULL ),
							currentStreamPosition( 0 ),
							playing( true )
					{
					}
					
					
					/// Return whether or not this playback instance is unused.
					OM_INLINE Bool isUnused() const
					{
						return instance.stream == NULL;
					}
					
					
					/// An object that contains the user's instance data.
					Instance instance;
					
					
					/// The target linear gain factor that is used to interpolate changes in the playback gain.
					Gain targetGain;
					
					
					/// A pointer to an object (sometimes NULL) which handles sample rate conversion for this stream.
					Resampler* resampler;
					
					
					/// The current position within the stream, relative to the stream's starting position.
					SampleIndex currentStreamPosition;
					
					
					/// A boolean value indicating whether or not the player should be playing the stream.
					Bool playing;
					
					
			};
			
			
		//********************************************************************************
		//******	Private Filter Processing Methods
			
			
			/// Reset the processing stream for this player.
			virtual void resetStream();
			
			
			/// Play the specified number of samples from the sound input stream and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
												SoundFrame& outputFrame, Size numSamples );
			
			
			/// Read the specified number of samples from the input stream into the output buffer.
			virtual SoundResult readSamples( SoundBuffer& outputBuffer, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Static Data Members
			
			
			/// The default maximum number of simultaneously playing instances.
			static const Size DEFAULT_MAX_INSTANCE_COUNT = 100;
			
			
			/// An object representing the pan direction of an invalid instance.
			static const PanDirection UNDEFINED_STREAM_PAN;
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of all of the slots for instances that can play as part of this player.
			ArrayList<InstanceInfo> instances;
			
			
			/// An object describing the output channel layout for this player.
			ChannelLayout outputChannelLayout;
			
			
			/// The output sample rate for this player.
			SampleRate outputSampleRate;
			
			
			/// The total number of instances in the player which are currently playing.
			Size numPlaying;
			
			
			/// An integer representing the maximum number of simultaneously playing streams.
			Size maxInstanceCount;
			
			
			/// A mix matrix which stores the current channel mix matrix of the stream panning.
			ChannelMixMatrix channelGains;
			
			
			/// A mix matrix which stores the target channel mix matrix of the stream panning.
			ChannelMixMatrix targetChannelGains;
			
			
			/// The current position of this player's output stream, releative to the stream start.
			SampleIndex currentPosition;
			
			
			/// A boolean value indicating whether or not the player should be playing any stream.
			Bool globalPlayingEnabled;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SAMPLER_H
