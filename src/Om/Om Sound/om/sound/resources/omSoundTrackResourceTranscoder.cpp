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

#include "omSoundTrackResourceTranscoder.h"


#define SOUND_REFERENCE 0
#define PLUGIN_REFERENCE 1


//##########################################################################################
//*************************  Start Om Sound Resources Namespace  ***************************
OM_SOUND_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Sound Encoding Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundTrackResourceTranscoder:: encode( const ResourceID& resourceID, const SoundTrack& track, const ResourceSet& resources,
										ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	// Write the number of regions in the track.
	const Size numRegions = track.getRegionCount();
	encoder.write( (UInt64)numRegions );
	
	// Read the data for each region.
	for ( Index i = 0; i < numRegions; i++ )
	{
		const SoundRegion& region = track.getRegion(i);
		const Time& regionStart = track.getRegionStart(i);
		
		// Write the local ID for the sound.
		encoder.writeReference( region.sound, resources );
		
		// Write the local ID for the insert plugin.
		encoder.writeReference( region.insert, resources );
		
		// Write the start time of the region.
		encoder.write( (Int64)regionStart );
		
		// Write the start time within the sound.
		encoder.write( (Int64)region.start );
		
		// Write the length within the sound.
		encoder.write( (Int64)region.length );
		
		// Write the fade in time.
		encoder.write( (Int64)region.fadeInTime );
		
		// Write the fade out time.
		encoder.write( (Int64)region.fadeOutTime );
		
		// Write the playback speed.
		encoder.write( region.speed );
		
		// Write the playback gain.
		encoder.write( region.gain );
		
		// Write the pan direction.
		encoder.write( region.pan.getDirection() );
		encoder.write( region.pan.getDirectivity() );
		encoder.write( region.pan.getSpread() );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Decoding Method
//############		
//##########################################################################################
//##########################################################################################




SoundTrack* SoundTrackResourceTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
													ResourceReferencePatcher& references,
													ResourceManager* resourceManager )
{
	SoundTrack* track = util::construct<SoundTrack>();
	
	// Read the number of regions in the track.
	UInt64 numRegions = 0;
	decoder.read( numRegions );
	
	// Read the data for each region.
	for ( Index i = 0; i < numRegions; i++ )
	{
		// Read the sound reference and add a reference to the reference patcher.
		decoder.readReference( references, SOUND_REFERENCE, i );
		
		// Read the plugin reference and add a reference to the reference patcher.
		decoder.readReference( references, PLUGIN_REFERENCE, i );
		
		// Read the start time of the region.
		Int64 regionStartNanoseconds = 0;
		decoder.read( regionStartNanoseconds );
		
		// Read the start time within the sound.
		Int64 soundStartNanoseconds = 0;
		decoder.read( soundStartNanoseconds );
		
		// Read the length within the sound.
		Int64 soundLengthNanoseconds = 0;
		decoder.read( soundLengthNanoseconds );
		
		// Read the fade in time.
		Int64 fadeInNanoseconds = 0;
		decoder.read( fadeInNanoseconds );
		
		// Read the fade out time.
		Int64 fadeOutNanoseconds = 0;
		decoder.read( fadeOutNanoseconds );
		
		// Read the playback speed.
		Float speed = 1.0f;
		decoder.read( speed );
		
		// Read the playback gain.
		Float gain = 1.0f;
		decoder.read( gain );
		
		// Read the pan direction.
		Vector3f panDirection( 0, 0, -1 );
		Float panDirectivity = 0.0f;
		Float panSpread = 0.0f;
		decoder.read( panDirection );
		decoder.read( panDirectivity );
		decoder.read( panSpread );
		
		// Create the region object.
		SoundRegion region;
		region.start = soundStartNanoseconds;
		region.length = soundLengthNanoseconds;
		region.fadeInTime = fadeInNanoseconds;
		region.fadeOutTime = fadeOutNanoseconds;
		region.speed = speed;
		region.gain = gain;
		region.pan = PanDirection( panDirection, panDirectivity, panSpread );
		
		// Add the region to the track.
		track->addRegion( region, Time(regionStartNanoseconds) );
	}
	
	return track;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reference Patching Method
//############		
//##########################################################################################
//##########################################################################################




void SoundTrackResourceTranscoder:: patchReferences( const ResourceReference* references, Size numReferences,
													const ResourceSet& resources, SoundTrack& track )
{
	for ( Index i = 0; i < numReferences; i++ )
	{
		const ResourceReference& reference = references[i];
		
		switch ( reference.userType )
		{
			case SOUND_REFERENCE:
			{
				const Resource<Sound>* sound = resources.getResource<Sound>( reference.localID );
				
				if ( sound && sound->load() )
					track.getRegion( reference.userID ).sound = *sound;
			}
			break;
			
			case PLUGIN_REFERENCE:
			{
				const Resource<SoundPlugin>* plugin = resources.getResource<SoundPlugin>( reference.localID );
				
				if ( plugin && plugin->load() )
					track.getRegion( reference.userID ).insert = *plugin;
			}
			break;
		}
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Instancing Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundTrackResourceTranscoder:: replaceInstances( SoundTrack& track, const Prototype& prototype, const Prototype& instance,
														const ResourceSetTranscoder& transcoder, ResourceSet& resources ) const
{
	const Size numRegions = track.getRegionCount();
	
	for ( Index i = 0; i < numRegions; i++ )
	{
		SoundRegion& region = track.getRegion(i);
		Sound* sound2 = replaceInstance( region.sound, prototype, instance, transcoder, resources );
		SoundPlugin* insert2 = replaceInstance( region.insert, prototype, instance, transcoder, resources );
		
		// Replace the original if the child object was instanced.
		if ( sound2 != region.sound )
			region.sound = sound2;
		
		if ( insert2 != region.insert )
			region.insert = insert2;
	}
}




Bool SoundTrackResourceTranscoder:: findInstances( const SoundTrack& track, const Prototype& prototype, const Prototype& instance,
													const ResourceSetTranscoder& transcoder ) const
{
	const Size numRegions = track.getRegionCount();
	
	for ( Index i = 0; i < numRegions; i++ )
	{
		const SoundRegion& region = track.getRegion(i);
		
		if ( findInstance( region.sound, prototype, instance, transcoder ) ||
			findInstance( region.insert, prototype, instance, transcoder ) )
			return true;
	}
	
	return false;
}




//##########################################################################################
//*************************  End Om Sound Resources Namespace  *****************************
OM_SOUND_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
