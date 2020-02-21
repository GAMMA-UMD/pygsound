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

#include "omSoundDirectionalIRResourceTranscoder.h"


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




Bool DirectionalIRResourceTranscoder:: encode( const ResourceID& resourceID, const DirectionalIR& dtf, const ResourceSet& resources,
													ResourceEncoder& encoder, ResourceManager* resourceManager )
{
	// Write the representation of the filter.
	encoder.write( (UInt64)dtf.getRepresentation() );
	
	// Write the domain of the filter.
	encoder.write( (UInt64)dtf.getDomain() );
	
	// Write the number of channels.
	const Size numChannels = dtf.getChannelCount();
	encoder.write( (UInt64)numChannels );
	
	// Write the length of the time-domain filter.
	encoder.write( (UInt64)dtf.getLength() );
	
	// Write the length of the filter data.
	const Size filterLength = dtf.getFilterLength();
	encoder.write( (UInt64)filterLength );
	
	// Write the sample rate.
	encoder.write( (Float64)dtf.getSampleRate() );
	
	// Write the spherical harmonic order of the filter.
	encoder.write( (UInt64)dtf.getSHOrder() );
	
	// Write the orientation.
	encoder.write( dtf.getOrientation() );
	
	// Write the name.
	encoder.writeUTF8String( dtf.getName() );
	
	// Write the description.
	encoder.writeUTF8String( dtf.getDescription() );
	
	//**********************************************************************
	// Write the channel and sample data.
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		// Write the channel info.
		const ChannelInfo& channelInfo = dtf.getChannelInfo(c);
		
		// Write the semantic type of the channel.
		encoder.write( (UInt64)channelInfo.getType() );
		
		// Write the position of the channel.
		encoder.write( channelInfo.getPosition() );
		
		// Write the number of samples.
		const Size numSamples = dtf.getSampleCount(c);
		encoder.write( (UInt64)numSamples );
		
		// Write the sample data for the channel.
		for ( Index i = 0; i < numSamples; i++ )
		{
			// Write the direction for the sample.
			encoder.write( dtf.getSampleDirection(c,i) );
			
			// Write the delay for the sample.
			encoder.write( dtf.getSampleDelay(c,i) );
			
			// Write the sample data.
			const Float32* sampleData = dtf.getSampleData(c,i);
			
			if ( encoder.getFormat().endianness.isNative() )
				encoder.write( (const UByte*)sampleData, filterLength*sizeof(Float32) );
			else
				encoder.writeArray( (const Float32*)sampleData, filterLength );
		}
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




DirectionalIR* DirectionalIRResourceTranscoder:: decode( ResourceID& resourceID, ResourceDecoder& decoder,
																	ResourceReferencePatcher& references,
																	ResourceManager* resourceManager )
{
	// Read the representation type.
	UInt64 representation = DirectionalIR::SAMPLED;
	decoder.read( representation );
	
	// Read the domain type.
	UInt64 domain = DirectionalIR::TIME_DOMAIN;
	decoder.read( domain );
	
	// Read the number of channels.
	UInt64 numChannels = 0;
	decoder.read( numChannels );
	
	// Read the length of the time-domain filter.
	UInt64 length = 0;
	decoder.read( length );
	
	// Read the length of the filter data.
	UInt64 filterLength = 0;
	decoder.read( filterLength );
	
	// Read the sample rate.
	Float64 sampleRate = 0;
	decoder.read( sampleRate );
	
	// Read the length of the filter data.
	UInt64 shOrder = 0;
	decoder.read( shOrder );
	
	// Read the orientation.
	Matrix3f orientation;
	decoder.read( orientation );
	
	// Read the name and description.
	UTF8String name = decoder.readUTF8String();
	UTF8String description = decoder.readUTF8String();
	
	//**********************************************************************
	
	// Create the response object.
	DirectionalIR* dtf = util::construct<DirectionalIR>( (Size)numChannels, (Size)length,
																	(SampleRate)sampleRate, (DirectionalIR::Domain)domain );
	
	// Make sure the DTF has the correct attributes.
	dtf->representation = (DirectionalIR::Representation)representation;
	dtf->shOrder = shOrder;
	dtf->orientation = orientation;
	dtf->name = name;
	dtf->description = description;
	
	//**********************************************************************
	// Read the channel and sample data.
	
	for ( Index c = 0; c < numChannels; c++ )
	{
		DirectionalIR::Channel& channel = dtf->channels[c];
		
		// Read the semantic type of the channel.
		UInt64 channelType = ChannelType::UNDEFINED;
		decoder.read( channelType );
		
		// Read the position of the channel.
		Vector3f position;
		decoder.read( position );
		
		// Set the channel info.
		dtf->setChannelInfo( c, ChannelInfo( (ChannelType::Enum)channelType, position ) );
		
		// Read the number of samples.
		UInt64 numSamples = 0;
		decoder.read( numSamples );
		
		// Make sure the sample array is big enough.
		channel.samples.setCapacity( numSamples );
		
		// Read the sample data for the channel.
		for ( UInt64 i = 0; i < numSamples; i++ )
		{
			// Read the direction of the sample.
			Vector3f direction;
			decoder.read( direction );
			
			// Read the delay of the sample.
			Float delay = 0;
			decoder.read( delay );
			
			// Add a new sample.
			channel.samples.addNew( direction, delay );
			DirectionalIR::Sample& sample = channel.samples.getLast();
			
			// Make sure the sample data is allocated.
			sample.data.allocate( filterLength );
			
			// Read the sample data.
			Float32* sampleData = sample.data.getPointer();
			
			if ( decoder.getFormat().endianness.isNative() )
				decoder.read( (UByte*)sampleData, filterLength*sizeof(Float32) );
			else
				decoder.readArray( (Float32*)sampleData, filterLength );
		}
	}
	
	return dtf;
}




//##########################################################################################
//*************************  End Om Sound Resources Namespace  *****************************
OM_SOUND_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
