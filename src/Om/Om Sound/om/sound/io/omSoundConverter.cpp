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

#include "omSoundConverter.h"


#include "omSoundAIFFTranscoder.h"
#include "omSoundWaveTranscoder.h"
#include "omSoundOggTranscoder.h"


//##########################################################################################
//****************************  Start Om Sound IO Namespace  *******************************
OM_SOUND_IO_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




SoundConverter:: SoundConverter()
{
	// Add all of the default transcoders to the internal map of image format transcoders.
	transcoders.add( SoundFormat::AIFF, SoundFormat::AIFF, &aiff );
	transcoders.add( SoundFormat::WAVE, SoundFormat::WAVE, &wave );
	transcoders.add( SoundFormat::OGG, SoundFormat::OGG, &ogg );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Transcoder Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




SoundTranscoder* SoundConverter:: getTranscoder( SoundFormat format ) const
{
	SoundTranscoder* const * result;
	
	if ( transcoders.find( (Hash)format, format, result ) )
		return *result;
	
	return NULL;
}




Bool SoundConverter:: addTranscoder( SoundTranscoder* newTranscoder )
{
	if ( newTranscoder == NULL )
		return false;
	
	// Get the transcoder's format.
	SoundFormat format = newTranscoder->getSoundFormat();
	
	// Replace any existing format.
	transcoders.set( (Hash)format, format, newTranscoder );
	
	return true;
}




Bool SoundConverter:: removeTranscoder( SoundFormat format )
{
	return transcoders.remove( (Hash)format, format );
}




void SoundConverter:: clearTranscoders()
{
	transcoders.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Encoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundConverter:: canEncode( const SoundFormat& format ) const
{
	if ( format == SoundFormat::UNDEFINED )
		return false;
	
	return transcoders.contains( (Hash)format, format );
}




Bool SoundConverter:: canEncode( const SoundFormat& format, const Sound& sound ) const
{
	if ( format == SoundFormat::UNDEFINED )
	{
		// Since the format is undefined, we have to check all transcoders to see if they
		// can decode the sound data.
		HashMap<SoundFormat,SoundTranscoder*>::ConstIterator i = transcoders.getIterator();
		
		for ( ; i; i++ )
		{
			if ( (*i)->canEncode( sound ) )
				return true;
		}
	}
	else
	{
		// Find the transcoder for the specified image format and check to see if it
		// can decode the sound.
		SoundTranscoder* const * transcoder;
		
		if ( transcoders.find( (Hash)format, format, transcoder ) )
			return (*transcoder)->canEncode( sound );
	}
	
	return false;
}




Bool SoundConverter:: encode( const SoundFormat& format, const UTF8String& filePath, const Sound& sound ) const
{
	FileWriter writer( filePath );
	
	if ( !writer.open() )
		return false;
	
	return this->encode( format, writer, sound );
}




Bool SoundConverter:: encode( const SoundFormat& format, DataOutputStream& stream, const Sound& sound ) const
{
	if ( format == SoundFormat::UNDEFINED )
	{
		// Since the format is undefined, we have to check all transcoders to see if they
		// can encode the sound data.
		HashMap<SoundFormat,SoundTranscoder*>::ConstIterator i = transcoders.getIterator();
		
		// Store the start position in the stream.
		LargeIndex streamPosition = stream.getPosition();
		
		for ( ; i; i++ )
		{
			if ( (*i)->encode( stream, sound ) )
				return true;
			
			Int64 seekAmount = (Int64)streamPosition - (Int64)stream.getPosition();
			
			// If we can't seek back to the beginning, fail.
			if ( !stream.canSeek( seekAmount ) )
				return false;
			
			// Seek back to the beginning of the stream to try again.
			if ( stream.seek( seekAmount ) != seekAmount )
				return false;
		}
	}
	else
	{
		// Find the transcoder for the specified image format and check to see if it
		// can decode the sound.
		SoundTranscoder* const * transcoder;
		
		if ( transcoders.find( (Hash)format, format, transcoder ) )
			return (*transcoder)->encode( stream, sound );
	}
	
	return false;
}




Bool SoundConverter:: encode( const UTF8String& filePath, const Sound& sound ) const
{
	Path path( filePath );
	UTF8String extension = path.getExtension();
	SoundFormat format = SoundFormat::getFormatForExtension( extension );
	
	if ( format == SoundFormat::UNDEFINED )
		return false;
	else
		return this->encode( format, filePath, sound );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Decoding Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundConverter:: canDecode( const SoundFormat& format ) const
{
	if ( format == SoundFormat::UNDEFINED )
		return false;
	
	return transcoders.contains( (Hash)format, format );
}




Bool SoundConverter:: canDecode( const SoundFormat& format, const UTF8String& filePath ) const
{
	const ResourceID resourceID( filePath );
	
	if ( format == SoundFormat::UNDEFINED )
	{
		// Since the format is undefined, we have to check all transcoders to see if they
		// can decode the image data.
		HashMap<SoundFormat,SoundTranscoder*>::ConstIterator i = transcoders.getIterator();
		
		for ( ; i; i++ )
		{
			if ( (*i)->canDecode( resourceID ) )
				return true;
		}
	}
	else
	{
		// Find the transcoder for the specified image format and check to see if it
		// can decode the image.
		SoundTranscoder* const * transcoder;
		
		if ( transcoders.find( (Hash)format, format, transcoder ) )
			return (*transcoder)->canDecode( resourceID );
	}
	
	return false;
}




Bool SoundConverter:: decode( const SoundFormat& format, const UTF8String& filePath, Sound& sound ) const
{
	Shared<FileReader> reader = Shared<FileReader>::construct( filePath );
	
	if ( !reader->open() )
		return false;
	
	return this->decode( format, reader, sound );
}




Bool SoundConverter:: decode( const SoundFormat& format, DataInputStream& stream, Sound& sound ) const
{
	if ( format == SoundFormat::UNDEFINED )
	{
		// Since the format is undefined, we have to check all transcoders to see if they
		// can decode the image data.
		HashMap<SoundFormat,SoundTranscoder*>::ConstIterator i = transcoders.getIterator();
		
		// Store the start position in the stream.
		LargeIndex streamPosition = stream.getPosition();
		
		for ( ; i; i++ )
		{
			if ( (*i)->decode( stream, sound ) )
				return true;
			
			Int64 seekAmount = (Int64)streamPosition - (Int64)stream.getPosition();
			
			// If we can't seek back to the beginning, fail.
			if ( !stream.canSeek( seekAmount ) )
				return false;
			
			// Seek back to the beginning of the stream to try again.
			if ( stream.seek( seekAmount ) != seekAmount )
				return false;
		}
	}
	else
	{
		// Find the transcoder for the specified image format and check to see if it
		// can decode the image.
		SoundTranscoder* const * transcoder;
		
		if ( transcoders.find( (Hash)format, format, transcoder ) )
			return (*transcoder)->decode( stream, sound );
	}
	
	return false;
}




Bool SoundConverter:: decode( const SoundFormat& format, const Shared<DataInputStream>& stream, Sound& sound ) const
{
	if ( stream.isNull() )
		return false;
	
	if ( format == SoundFormat::UNDEFINED )
	{
		// Since the format is undefined, we have to check all transcoders to see if they
		// can decode the image data.
		HashMap<SoundFormat,SoundTranscoder*>::ConstIterator i = transcoders.getIterator();
		
		// Store the start position in the stream.
		LargeIndex streamPosition = stream->getPosition();
		
		for ( ; i; i++ )
		{
			if ( (*i)->decode( stream, sound ) )
				return true;
			
			Int64 seekAmount = (Int64)streamPosition - (Int64)stream->getPosition();
			
			// If we can't seek back to the beginning, fail.
			if ( !stream->canSeek( seekAmount ) )
				return false;
			
			// Seek back to the beginning of the stream to try again.
			if ( stream->seek( seekAmount ) != seekAmount )
				return false;
		}
	}
	else
	{
		// Find the transcoder for the specified image format and check to see if it
		// can decode the image.
		SoundTranscoder* const * transcoder;
		
		if ( transcoders.find( (Hash)format, format, transcoder ) )
			return (*transcoder)->decode( stream, sound );
	}
	
	return false;
}




Bool SoundConverter:: decode( const UTF8String& filePath, Sound& sound ) const
{
	Path path( filePath );
	SoundFormat format = SoundFormat::getFormatForExtension( path.getExtension() );
	
	if ( format == SoundFormat::UNDEFINED )
		return false;
	
	return this->decode( format, filePath, sound );
}




//##########################################################################################
//****************************  End Om Sound IO Namespace  *********************************
OM_SOUND_IO_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
