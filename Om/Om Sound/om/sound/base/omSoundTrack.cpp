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

#include "omSoundTrack.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const om::resources::ResourceType SoundTrack:: RESOURCE_TYPE("SoundTrack");


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundTrack:: SoundTrack()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Region Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundTrack:: setRegion( Index regionIndex, const SoundRegion& newRegion )
{
	if ( regionIndex >= regions.getSize() )
		return;
	
	regions[regionIndex].region = newRegion;
}




void SoundTrack:: setRegionStart( Index regionIndex, const Time& newStart )
{
	if ( regionIndex >= regions.getSize() )
		return;
	
	SoundRegion region = regions[regionIndex].region;
	
	regions.removeAtIndex(regionIndex);
	
	this->addRegion( region, newStart );
}




void SoundTrack:: addRegion( const SoundRegion& region, const Time& start )
{
	// Find the position to insert the new region.
	const Size numRegions = regions.getSize();
	Index i = 0;
	
	for ( ; i < numRegions && start < regions[i].start; i++ );
	
	// Insert the region.
	regions.insert( i, RegionInfo( region, start ) );
}




void SoundTrack:: removeRegion( Index regionIndex )
{
	if ( regionIndex >= regions.getSize() )
		return;
	
	regions.removeAtIndex( regionIndex );
}




void SoundTrack:: clearRegions()
{
	regions.clear();
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
