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

#ifndef INCLUDE_OM_SOUND_TRACK_H
#define INCLUDE_OM_SOUND_TRACK_H


#include "omSoundBaseConfig.h"


#include "omSoundRegion.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a sorted time sequence of sound regions.
class SoundTrack
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new empty track with no regions.
			SoundTrack();
			
			
		//********************************************************************************
		//******	Region Accessor Methods
			
			
			/// Return the number of regions there are in this track.
			OM_INLINE Size getRegionCount() const
			{
				return regions.getSize();
			}
			
			
			/// Return a reference to the sound region at the specified index in this track.
			OM_INLINE SoundRegion& getRegion( Index regionIndex )
			{
				return regions[regionIndex].region;
			}
			
			
			/// Return a const reference to the sound region at the specified index in this track.
			OM_INLINE const SoundRegion& getRegion( Index regionIndex ) const
			{
				return regions[regionIndex].region;
			}
			
			
			/// Replace the sound region at the specified index in this track, keeping the previous start time.
			void setRegion( Index regionIndex, const SoundRegion& newRegion );
			
			
			/// Return the start time of the sound region at the specified index in this track.
			OM_INLINE const Time& getRegionStart( Index regionIndex ) const
			{
				return regions[regionIndex].start;
			}
			
			
			/// Update the start time of the region at the specified index.
			/**
			  * After the method call, the regions in the track may have been reordered to keep
			  * the regions sorted.
			  */
			void setRegionStart( Index regionIndex, const Time& newStart );
			
			
			/// Add a new region to this track that is played back at the specified time.
			/**
			  * The new region is inserted in the track's list of regions at the correct place
			  * so that the playback start times are sorted in increasing order.
			  */
			void addRegion( const SoundRegion& region, const Time& start );
			
			
			/// Remove the region from this track at the specified index.
			void removeRegion( Index regionIndex );
			
			
			/// Remove all regions from this track.
			void clearRegions();
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a sound track.
			static const om::resources::ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// A class that stores information about a region within a track.
			class RegionInfo
			{
				public:
					
					/// Create a new region for the specified region and start time.
					OM_INLINE RegionInfo( const SoundRegion& newRegion, const Time& newStart )
						:	region( newRegion ),
							start( newStart )
					{
					}
					
					
					/// Return whether or not this region comes before another on the track timeline.
					OM_INLINE Bool operator < ( const RegionInfo& other ) const
					{
						return start < other.start;
					}
					
					
					/// The sound region that is part of the track.
					SoundRegion region;
					
					/// The start time of this region from the start of the track.
					Time start;
					
			};
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the regions in this track.
			ArrayList<RegionInfo> regions;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::sound::base::SoundTrack, om::sound::base::SoundTrack::RESOURCE_TYPE );


#endif // INCLUDE_OM_SOUND_TRACK_H
