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

#include "omSoundOggEncoder.h"


#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#include "vorbis/vorbisenc.h"


#define MINIMUM_KBIT_RATE 64.0f
#define MAXIMUM_KBIT_RATE 500.0f


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Ogg Decoder Wrapper Class
//############		
//##########################################################################################
//##########################################################################################




class OggEncoder:: OggEncoderWrapper
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			OM_INLINE OggEncoderWrapper()
			{
				// Reference the ogg callbacks here so that we can avoid some compiler warnings
				// that they are unreferenced.
				OV_CALLBACKS_DEFAULT;
				OV_CALLBACKS_NOCLOSE;
				OV_CALLBACKS_STREAMONLY;
				OV_CALLBACKS_STREAMONLY_NOCLOSE;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The current reading position in bytes within the data stream, relative to the starting position.
			LargeIndex currentStreamPosition;
			
			
			ogg_stream_state os; /* take physical pages, weld into a logical stream of packets */
			ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
			ogg_packet       op; /* one raw packet of data for decode */
			
			vorbis_info      vi; /* struct that stores all the static vorbis bitstream settings */
			vorbis_comment   vc; /* struct that stores all the user comments */
			
			vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
			vorbis_block     vb; /* local working space for packet->PCM decode */
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




OggEncoder:: OggEncoder( DataOutputStream* outputStream, Size newNumChannels, SampleRate newSampleRate,
						const SoundFormatSettings& newFormatSettings )
	:	wrapper( om::util::construct<OggEncoderWrapper>() ),
		numChannels( math::max( newNumChannels, Size(1) ) ),
		formatSettings( newFormatSettings ),
		sampleRate( math::max( newSampleRate, SampleRate(0) ) ),
		lengthInSamples( 0 ),
		currentSampleIndex( 0 ),
		valid( false ),
		writtenHeader( false ),
		stream( outputStream )
{
	if ( stream != NULL )
		valid = true;
}




OggEncoder:: OggEncoder( const OggEncoder& other )
{
	// Empty because it is declared private and shouldn't be called.
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




OggEncoder:: ~OggEncoder()
{
	// Clean up the ogg stuff.
	this->flush();
	
	if ( valid )
	{
		// Tell the encoder that we have reached the end of stream.
		vorbis_analysis_wrote( &wrapper->vd, 0 );
	
		// clean up and exit. vorbis_info_clear() must be called last
		ogg_stream_clear( &wrapper->os );
		vorbis_block_clear( &wrapper->vb );
		vorbis_dsp_clear( &wrapper->vd );
		vorbis_comment_clear( &wrapper->vc );
		vorbis_info_clear( &wrapper->vi );
	}

	// Destroy the wrapper object.
	om::util::destruct( wrapper );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




OggEncoder& OggEncoder:: operator = ( const OggEncoder& other )
{
	// Empty because it is declared private and shouldn't be called.
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Flush Method
//############		
//##########################################################################################
//##########################################################################################




void OggEncoder:: flush()
{
	if ( valid )
		ogg_stream_flush( &wrapper->os, &wrapper->og );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Seek Methods
//############		
//##########################################################################################
//##########################################################################################




Bool OggEncoder:: canSeek() const
{
	return false;
}




Bool OggEncoder:: canSeek( Int64 relativeSampleOffset ) const
{
	return false;
}




Int64 OggEncoder:: seek( Int64 relativeSampleOffset )
{
	return 0;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoder Format Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size OggEncoder:: getChannelCount() const
{
	return numChannels;
}




SampleRate OggEncoder:: getSampleRate() const
{
	return sampleRate;
}




SampleType OggEncoder:: getNativeSampleType() const
{
	return SampleType::SAMPLE_32F;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Reading Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult OggEncoder:: writeSamples( const SoundBuffer& inputBuffer, Size numSamples )
{
	// Acquire the mutex which synchronizes decoding with other class access.
	encodingMutex.lock();
	
	// If the encoder is not valid or there are no samples to write, return that
	// no samples were encoded.
	if ( !valid )
	{
		encodingMutex.unlock();
		return SoundResult::ERROR;
	}
	
	if ( numSamples == 0 )
	{
		encodingMutex.unlock();
		return 0;
	}
	
	//***************************************************************************
	// Detect the input sample rate if the output sample rate hasn't been initialized.
	
	if ( sampleRate == SampleRate(0) )
		sampleRate = (SampleRate)inputBuffer.getSampleRate();
	
	//***************************************************************************
	// Write the ogg file's header if we haven't already.
	
	if ( !writtenHeader )
	{
		// If there is an error in writing the header, return that no samples were written.
		if ( !this->writeHeader() )
		{
			valid = false;
			encodingMutex.unlock();
			return 0;
		}
	}
	
	//***************************************************************************
	// Do sample rate conversion if necessary.
	
	const SoundBuffer* finalInputBuffer;
	
	// Test to see if the input buffer should be sample rate converted.
	if ( inputBuffer.getSampleRate() != sampleRate )
	{
		resampler.setOutputSampleRate( sampleRate );
		numSamples = resampler.process( inputBuffer, sampleRateConversionBuffer, numSamples );
		finalInputBuffer = &sampleRateConversionBuffer;
	}
	else
		finalInputBuffer = &inputBuffer;
	
	
	//***************************************************************************
	// Submit sound data to the encoding library.
	
	{
		// expose the buffer to submit data.
		float** encodeBuffer = vorbis_analysis_buffer( &wrapper->vd, (int)numSamples );
		
		Index c;
		
		// Determine how many channels to copy from the input buffer.
		Size numInputChannels = math::min( finalInputBuffer->getChannelCount(), numChannels );
		
		// Copy input samples to the vorbis buffer for each channel in the input buffer.
		for ( c = 0; c < numInputChannels; c++ )
		{
			float* destination = encodeBuffer[c];
			const Sample32f* source = finalInputBuffer->getChannel(c);
			const Sample32f* const sourceEnd = source + numSamples;
			
			while ( source != sourceEnd )
			{
				*destination = *source;
				
				destination++;
				source++;
			}
		}
		
		// Write zeros for other channels.
		for ( ; c < numChannels; c++ )
		{
			float* destination = encodeBuffer[c];
			const float* const destinationEnd = destination + numSamples;
			
			while ( destination != destinationEnd )
			{
				*destination = float(0);
				destination++;
			}
		}
		
		// tell the library how much we actually submitted.
		vorbis_analysis_wrote( &wrapper->vd, (int)numSamples );
	}
	
	
	//***************************************************************************
	// Encode the data.
	
	// vorbis does some data preanalysis, then divvies up blocks for
	// more involved (potentially parallel) processing.  Get a single
	// block for encoding now.
	while ( vorbis_analysis_blockout( &wrapper->vd, &wrapper->vb ) == 1 )
	{
		// analysis, assume we want to use bitrate management.
		vorbis_analysis( &wrapper->vb, NULL );
		vorbis_bitrate_addblock( &wrapper->vb );

		while ( vorbis_bitrate_flushpacket( &wrapper->vd, &wrapper->op ) )
		{
			// weld the packet into the bitstream.
			ogg_stream_packetin( &wrapper->os, &wrapper->op );
			
			Bool eos = false;
			
			// write out pages (if any).
			while ( !eos )
			{
				int result = ogg_stream_pageout( &wrapper->os, &wrapper->og );
				
				if ( result == 0 )
					break;
				
				stream->writeData( (const UByte*)wrapper->og.header, wrapper->og.header_len );
				stream->writeData( (const UByte*)wrapper->og.body, wrapper->og.body_len );

				// this could be set above, but for illustrative purposes, I do
				// it here (to show that vorbis does know where the stream ends).
				
				if ( ogg_page_eos( &wrapper->og ) )
					eos = true;
			}
		}
	}
	
	//**********************************************************************************
	// Update the total length of the audio file.
	
	currentSampleIndex += numSamples;
	lengthInSamples = math::max( currentSampleIndex, lengthInSamples );
	
	
	encodingMutex.unlock();
	
	return numSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		.ogg Header Read Method
//############		
//##########################################################################################
//##########################################################################################




Bool OggEncoder:: writeHeader()
{
	int error = 0;
	
	//*********************************************************************
	// Setup the encoder and choose the output format.
	
	vorbis_info_init( &wrapper->vi );
	
	// Initialize the encoder differently for variable-bitrate and constant bitrate streams.
	if ( formatSettings.flagIsSet( SoundFormatFlags::VBR ) )
	{
		// Encode using a quality mode, but select that quality mode by asking for
		// an approximate bitrate.  This is not ABR, it is true VBR, but selected
		// using the bitrate interface, and then turning bitrate management off:
		
		error = vorbis_encode_setup_managed( &wrapper->vi, long(numChannels), long(sampleRate),
												long(formatSettings.getMaxBitRate()*1000),
												long(formatSettings.getBitRate()*1000),
												long(formatSettings.getMinBitRate()*1000) );
		
		error |= vorbis_encode_ctl( &wrapper->vi, OV_ECTL_RATEMANAGE2_SET, NULL );
		error |= vorbis_encode_setup_init( &wrapper->vi );
	}
	else
	{
		// Encoding using an average bitrate mode (ABR).
		error = vorbis_encode_init( &wrapper->vi, long(numChannels), long(sampleRate),
									-1, long(formatSettings.getBitRate()*1000), -1 );
	}
	
	// Do not continue if setup failed; this can happen if we ask for a
	// mode that libVorbis does not support (eg, too low a bitrate, etc,
	// will return 'OV_EIMPL')
	if ( error )
		return false;
	
	
	//*********************************************************************
	// Setup more encoder information.
	
	// Add comments.
	vorbis_comment_init( &wrapper->vc );
	//vorbis_comment_add_tag( &wrapper->vc, (char*)"ENCODER", (char*)"Rim Sound Ogg Encoder" );
	
	// Set up the analysis state and auxiliary encoding storage.
	vorbis_analysis_init( &wrapper->vd, &wrapper->vi );
	vorbis_block_init( &wrapper->vd, &wrapper->vb );
	
	// Set up the packet-stream encoder
	ogg_stream_init( &wrapper->os, 0 );
	
	/*
		Vorbis streams begin with three headers; the initial header (with
		most of the codec setup parameters) that is mandated by the Ogg
		bitstream spec.  The second header holds any comment fields.  The
		third header holds the bitstream codebook.  We merely need to
		make the headers, then pass them to libvorbis one at a time;
		libvorbis handles the additional Ogg bitstream constraints
	*/
	ogg_packet header;
	ogg_packet header_comm;
	ogg_packet header_code;

	vorbis_analysis_headerout( &wrapper->vd, &wrapper->vc, &header, &header_comm, &header_code );
	ogg_stream_packetin( &wrapper->os, &header ); // automatically placed in its own page.
	ogg_stream_packetin( &wrapper->os, &header_comm );
	ogg_stream_packetin( &wrapper->os, &header_code );
	
	//*********************************************************************
	// Write the ogg header.
	
	// This ensures the actual audio data will start on a new page, as per spec.
	while ( 1 )
	{
		int result = ogg_stream_flush( &wrapper->os, &wrapper->og );
		
		if ( result == 0 )
			break;
		
		stream->writeData( wrapper->og.header, wrapper->og.header_len );
		stream->writeData( wrapper->og.body, wrapper->og.body_len );
	}
	
	writtenHeader = true;
	
	return true;
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
