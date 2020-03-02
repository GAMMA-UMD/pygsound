/*
 * Project:     GSound
 * 
 * File:        gsound/GSoundC.cpp
 * Contents:    C interface implementation for the GSound library
 * 
 * Author(s):   Carl Schissler
 * Website:     http://gamma.cs.unc.edu/GSOUND/
 * 
 * License:
 * 
 *     Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
 *     All rights reserved.
 *     
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for educational, research, and non-profit purposes, without
 *     fee, and without a written agreement is hereby granted, provided that the
 *     above copyright notice, this paragraph, and the following four paragraphs
 *     appear in all copies.
 *     
 *     Permission to incorporate this software into commercial products may be
 *     obtained by contacting the University of North Carolina at Chapel Hill.
 *     
 *     This software program and documentation are copyrighted by Carl Schissler and
 *     the University of North Carolina at Chapel Hill. The software program and
 *     documentation are supplied "as is", without any accompanying services from
 *     the University of North Carolina at Chapel Hill or the authors. The University
 *     of North Carolina at Chapel Hill and the authors do not warrant that the
 *     operation of the program will be uninterrupted or error-free. The end-user
 *     understands that the program was developed for research purposes and is advised
 *     not to rely exclusively on the program for any reason.
 *     
 *     IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR ITS
 *     EMPLOYEES OR THE AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE
 *     UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *     THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 *     DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 *     STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS
 *     ON AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND
 *     THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 *     ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "gsound.h"


#include "gsoundC.h"


using namespace gsound;


//##########################################################################################
//##########################################################################################
//############		
//############		Object Allocator Class
//############		
//##########################################################################################
//##########################################################################################




/// A class that manages a pool of object references with associated object IDs.
template < typename ObjectType >
class ObjectAllocator
{
	public:	
		
		/// Return the number of valid objects there are in this object allocator.
		GSOUND_INLINE Size getSize() const
		{
			return objects.getSize() - freeList.getSize();
		}
		
		
		/// Add a new object to this object allocator, returning the new ID for the object.
		GSOUND_INLINE gsID add( const Shared<ObjectType>& newObject )
		{
			// Is there space in the free list?
			if ( freeList.getSize() > 0 )
			{
				const Index objectIndex = freeList.getLast();
				freeList.removeLast();
				objects[objectIndex] = newObject;
				
				return indexToID( objectIndex );
			}
			
			// Otherwise, add to the end of the object list.
			objects.add( newObject );
			return indexToID( objects.getSize() - 1 );
		}
		
		
		/// Find the object with the specified ID in this allocator.
		GSOUND_INLINE Bool find( gsID objectID, Shared<ObjectType>& object )
		{
			const Index objectIndex = idToIndex( objectID );
			
			if ( objectIndex < objects.getSize() && objects[objectIndex].isSet() )
			{
				object = objects[objectIndex];
				return true;
			}
			
			return false;
		}
		
		
		/// Find the object with the specified ID in this allocator.
		GSOUND_INLINE Bool set( gsID objectID, const Shared<ObjectType>& object )
		{
			const Index objectIndex = idToIndex( objectID );
			
			if ( objectIndex < objects.getSize() )
			{
				objects[objectIndex] = object;
				return true;
			}
			
			return false;
		}
		
		
		/// Remove the object with the specified ID in this allocator, releasing the object.
		GSOUND_INLINE Bool remove( gsID objectID )
		{
			const Index objectIndex = idToIndex( objectID );
			
			if ( objectIndex < objects.getSize() && objects[objectIndex].isSet() )
			{
				objects[objectIndex].release();
				freeList.add( objectIndex );
				return true;
			}
			
			return false;
		}
		
		
		/// Remove all objects from this allocator, reclaiming their memory.
		GSOUND_INLINE void clear()
		{
			objects.clear();
			freeList.clear();
		}
		
		
		
	private:
		
		/// Return the object ID for the given index.
		GSOUND_INLINE static gsID indexToID( Index i )
		{
			return gsID(i + 1);
		}
		
		
		/// Return the object index for the given object ID.
		GSOUND_INLINE static Index idToIndex( gsID id )
		{
			return id - 1;
		}
		
		
		/// A list of the objects that are allocated by this object allocator.
		ArrayList< Shared<ObjectType> > objects;
		
		
		/// A list of the unused objects in the object allcoator.
		ArrayList<Index> freeList;
		
		
};



//##########################################################################################
//##########################################################################################
//############		
//############		Source Callback Stream
//############		
//##########################################################################################
//##########################################################################################




/// A class that wraps a callback function in a sound input stream so that it can be used as a sound source.
class SourceCallbackStream : public SoundInputStream
{
	public:
		
		GSOUND_INLINE SourceCallbackStream( gsSourceID newSourceID, gsSourceCallback newCallback )
			:	sourceID( newSourceID ),
				callback( newCallback ),
				lastSampleRate( 44100.0 )
		{
		}
		
		GSOUND_INLINE gsSourceCallback getCallback() const
		{
			return callback;
		}
		
		GSOUND_INLINE void setCallback( gsSourceCallback newCallback )
		{
			callback = newCallback;
		}
		
		virtual Bool canSeek() const
		{
			return false;
		}
		
		virtual Bool canSeek( Int64 offset ) const
		{
			return false;
		}
		
		virtual Int64 seek( Int64 offset )
		{
			return 0;
		}
		
		virtual om::sound::SoundSize getSamplesRemaining() const
		{
			return math::max<om::sound::SoundSize>();
		}
		
		virtual om::sound::SampleIndex getPosition() const
		{
			return 0;
		}
		
		virtual Size getChannelCount() const
		{
			return 1;
		}
		
		virtual SampleRate getSampleRate() const
		{
			return lastSampleRate;
		}
		
		virtual om::sound::SampleType getNativeSampleType() const
		{
			return om::sound::SampleType::SAMPLE_32F;
		}
		
		virtual Bool isValid() const
		{
			return callback != NULL;
		}
		
		
		
	protected:
		
		virtual SoundResult readSamples( SoundBuffer& outputBuffer, Size numSamples )
		{
			// Remember the current sample rate of the stream.
			lastSampleRate = outputBuffer.getSampleRate();
			
			// Don't read any samples if the callback is not set.
			if ( callback == NULL )
				return SoundResult::ERROR;
			
			// Compute the required size of the temporary buffer.
			const Size numChannels = outputBuffer.getChannelCount();
			const Size bufferSize = numChannels*numSamples;
			
			// Make sure the temporary buffer is big enough.
			if ( buffer.getSize() < bufferSize )
				buffer.setSize( bufferSize );
			
			// Get the audio from the client in the temporary buffer.
			callback( sourceID, buffer.getPointer(), numSamples,
					outputBuffer.getChannelCount(), outputBuffer.getSampleRate() );
			
			// Copy the interleaved data into the output buffer.
			for ( Index c = 0; c < numChannels; c++ )
			{
				Sample32f* output = outputBuffer.getChannel(c);
				Sample32f* const outputEnd = output + numSamples;
				const Sample32f* temp = (Sample32f*)buffer.getPointer() + c;
				
				while ( output != outputEnd )
				{
					*output = *temp;
					output++;
					temp += numChannels;
				}
			}
			
			return numSamples;
		}
		
		
	private:
		
		gsSourceID sourceID;
		gsSourceCallback callback;
		
		SampleRate lastSampleRate;
		
		Array<Float32> buffer;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Channel Layout Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




/// Convert the specified channel layout to a channel layout enum value.
static gsChannelLayout convertChannelLayout( const ChannelLayout& layout )
{
	switch ( layout.getChannelCount() )
	{
		case 1:			return GS_CHANNEL_LAYOUT_MONO;
		case 2:			return GS_CHANNEL_LAYOUT_STEREO;
		case 4:			return GS_CHANNEL_LAYOUT_QUAD;
		case 5:			return GS_CHANNEL_LAYOUT_5_1;
		case 6:			return GS_CHANNEL_LAYOUT_5_1;
		case 7:			return GS_CHANNEL_LAYOUT_7_1;
		case 8:			return GS_CHANNEL_LAYOUT_7_1;
	}
	
	return GS_CHANNEL_LAYOUT_UNDEFINED;
}




/// Convert the specified channel layout enum value to a channel layout.
static ChannelLayout convertChannelLayout( gsChannelLayout layout )
{
	switch ( layout )
	{
		case GS_CHANNEL_LAYOUT_MONO:		return ChannelLayout::MONO;
		case GS_CHANNEL_LAYOUT_STEREO:		return ChannelLayout::STEREO;
		case GS_CHANNEL_LAYOUT_BINAURAL:	return ChannelLayout::BINAURAL;
		case GS_CHANNEL_LAYOUT_QUAD:		return ChannelLayout::QUAD;
		case GS_CHANNEL_LAYOUT_5_1:			return ChannelLayout::SURROUND_5_1;
		case GS_CHANNEL_LAYOUT_7_1:			return ChannelLayout::SURROUND_7_1;
	}
	
	return ChannelLayout();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Library Class
//############		
//##########################################################################################
//##########################################################################################




/// A class that stores the entire state of the sound library.
class SoundLibrary
{
	public:
		
		/// Create a new default sound library with no objects allocated.
		GSOUND_INLINE SoundLibrary()
		{
		}
		
		
		/// Return a reference to the resource manager for this sound library.
		GSOUND_INLINE om::resources::ResourceManager& getResourceManager()
		{
			return resourceManager;
		}
		
		
		/// Reset the library, releasing all allocated objects.
		GSOUND_INLINE void reset()
		{
			systems.clear();
			requests.clear();
			renderRequests.clear();
			meshRequests.clear();
			scenes.clear();
			sources.clear();
			listeners.clear();
			objects.clear();
			meshes.clear();
			materials.clear();
			sounds.clear();
			hrtfs.clear();
			resourceManager.clearResources();
		}
		
		
		ObjectAllocator<PropagationRequest> requests;
		ObjectAllocator<RenderRequest> renderRequests;
		ObjectAllocator<MeshRequest> meshRequests;
		ObjectAllocator<SoundPropagationSystem> systems;
		ObjectAllocator<SoundScene> scenes;
		ObjectAllocator<SoundSource> sources;
		ObjectAllocator<SoundListener> listeners;
		ObjectAllocator<SoundObject> objects;
		ObjectAllocator<SoundMesh> meshes;
		ObjectAllocator<SoundMaterial> materials;
		ObjectAllocator<om::sound::Sound> sounds;
		ObjectAllocator<om::sound::HRTF> hrtfs;
		
		
		/// An object that manages the on-disk resources for the sound library.
		om::resources::ResourceManager resourceManager;

		/// A mutex that handles synchronizing access to the library.
		Mutex mutex;
		
};




static Shared<SoundLibrary> library;




//##########################################################################################
//##########################################################################################
//############		
//############		Library Attribute Functions
//############		
//##########################################################################################
//##########################################################################################




extern "C" void GSOUND_EXPORT gsInit()
{
	if ( library.isNull() )
		library = Shared<SoundLibrary>::construct();
}




extern "C" void GSOUND_EXPORT gsReset()
{
	ScopedMutex lock( library->mutex );
	library->reset();
}




extern "C" void GSOUND_EXPORT gsGetVersion( gsSize* major, gsSize* minor, gsSize* revision )
{
	if ( major ) *major = GSOUND_MAJOR_VERSION;
	if ( minor ) *minor = GSOUND_MINOR_VERSION;
	if ( revision ) *revision = GSOUND_REVISION;
}



//##########################################################################################
//##########################################################################################
//############		
//############		System Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsSystemID GSOUND_EXPORT gsNewSystem()
{
	ScopedMutex lock( library->mutex );
	gsSystemID systemID = library->systems.add( Shared<SoundPropagationSystem>::construct() );
	
	return systemID;
}




extern "C" void GSOUND_EXPORT gsDeleteSystem( gsSystemID systemID )
{
	ScopedMutex lock( library->mutex );
	library->systems.remove( systemID );
}




extern "C" gsBool GSOUND_EXPORT gsSystemUpdate( gsSystemID systemID, gsFloat dt, gsBool synchronous )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundPropagationSystem> system;
	
	if ( library->systems.find( systemID, system ) )
	{
		system->update( dt, synchronous != false );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSystemSetScene( gsSystemID systemID, gsSceneID sceneID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundPropagationSystem> system;
	Shared<SoundScene> scene;
	
	if ( library->systems.find( systemID, system ) && library->scenes.find( sceneID, scene ) )
	{
		system->setScene( scene );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSystemAddListener( gsSystemID systemID, gsListenerID listenerID, gsRenderRequestID renderingRequestID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundPropagationSystem> system;
	Shared<SoundListener> listener;
	Shared<RenderRequest> renderingRequest;
	
	if ( library->systems.find( systemID, system ) && library->listeners.find( listenerID, listener ) &&
		library->renderRequests.find( renderingRequestID, renderingRequest ) )
	{
		// Add the new listener.
		system->addListener( listener, renderingRequest );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSystemSetRequest( gsSystemID systemID, gsRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundPropagationSystem> system;
	Shared<PropagationRequest> request;
	
	if ( library->systems.find( systemID, system ) && library->requests.find( requestID, request ) )
	{
		system->setRequest( request );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSystemReadSamples( gsSystemID systemID, gsRequestID listenerID,
													gsFloat* samples, gsSize numSamples, gsSize numChannels )
{
	if ( samples == NULL || numSamples == 0 || numChannels == 0 )
		return false;
	
	ScopedMutex lock( library->mutex );
	Shared<SoundPropagationSystem> system;
	Shared<SoundListener> listener;
	
	if ( library->systems.find( systemID, system ) && library->listeners.find( listenerID, listener ) )
	{
		// Get a temporary shared buffer to request the audio data in.
		SharedSoundBuffer sharedBuffer = SharedBufferPool::getGlobalBuffer();
		SoundBuffer& tempBuffer = sharedBuffer.getBuffer();
		
		// Render the listener audio into the temporary buffer.
		Size numRenderedSamples = system->renderListener( listener, tempBuffer, (Size)numSamples );
		
		if ( numRenderedSamples < (Size)numSamples )
			tempBuffer.zero( numRenderedSamples, (Size)numSamples );
		
		// Copy the samples into the output buffer in interleaved format.
		const Size numOutputChannels = math::min( (Size)numChannels, tempBuffer.getChannelCount() );
		
		for ( Index c = 0; c < numOutputChannels; c++ )
		{
			const Sample32f* temp = tempBuffer.getChannel(c);
			const Sample32f* const tempEnd = temp + numSamples;
			Sample32f* output = (Sample32f*)samples + c;
			
			while ( temp != tempEnd )
			{
				*output = *temp;
				output += numChannels;
				temp++;
			}
		}
		
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Propagation Request Functions
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsRequestID GSOUND_EXPORT gsNewRequest()
{
	ScopedMutex lock( library->mutex );
	gsRequestID requestID = library->requests.add( Shared<PropagationRequest>::construct() );
	
	return requestID;
}




extern "C" void GSOUND_EXPORT gsDeleteRequest( gsRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	library->requests.remove( requestID );
}




extern "C" gsBool GSOUND_EXPORT gsRequestGetFlag( gsRequestID requestID, gsFlag flag, gsBool* value )
{
	ScopedMutex lock( library->mutex );
	Shared<PropagationRequest> request;
	
	if ( library->requests.find( requestID, request ) )
	{
		switch ( flag )
		{
			case GS_DIRECT:				*value = request->flags.isSet( PropagationFlags::DIRECT );				break;
			case GS_TRANSMISSION:		*value = request->flags.isSet( PropagationFlags::TRANSMISSION );		break;
			case GS_SPECULAR:			*value = request->flags.isSet( PropagationFlags::SPECULAR );			break;
			case GS_DIFFUSE:			*value = request->flags.isSet( PropagationFlags::DIFFUSE );				break;
			case GS_DIFFRACTION:		*value = request->flags.isSet( PropagationFlags::DIFFRACTION );			break;
			case GS_SPECULAR_CACHE:		*value = request->flags.isSet( PropagationFlags::SPECULAR_CACHE );		break;
			case GS_IR_CACHE:			*value = request->flags.isSet( PropagationFlags::IR_CACHE );			break;
			case GS_VISIBILITY_CACHE:	*value = request->flags.isSet( PropagationFlags::VISIBILITY_CACHE );	break;
			case GS_SOURCE_DIFFUSE:		*value = request->flags.isSet( PropagationFlags::SOURCE_DIFFUSE );		break;
			case GS_SOURCE_DIRECTIVITY:	*value = request->flags.isSet( PropagationFlags::SOURCE_DIRECTIVITY );	break;
			case GS_SOURCE_CLUSTERING:	*value = request->flags.isSet( PropagationFlags::SOURCE_CLUSTERING );	break;
			case GS_AIR_ABSORPTION:		*value = request->flags.isSet( PropagationFlags::AIR_ABSORPTION );		break;
			case GS_SAMPLED_IR:			*value = request->flags.isSet( PropagationFlags::SAMPLED_IR );			break;
			case GS_SAMPLED_IR_SOURCE_DIRECTIONS:		*value = request->flags.isSet( PropagationFlags::SAMPLED_IR_SOURCE_DIRECTIONS );	break;
			case GS_IR_THRESHOLD:		*value = request->flags.isSet( PropagationFlags::IR_THRESHOLD );		break;
			case GS_ADAPTIVE_IR_LENGTH:	*value = request->flags.isSet( PropagationFlags::ADAPTIVE_IR_LENGTH );	break;
			case GS_ADAPTIVE_QUALITY:	*value = request->flags.isSet( PropagationFlags::ADAPTIVE_QUALITY );	break;
			case GS_DOPPLER_SORTING:	*value = request->flags.isSet( PropagationFlags::DOPPLER_SORTING );		break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRequestSetFlag( gsRequestID requestID, gsFlag flag, gsBool value )
{
	ScopedMutex lock( library->mutex );
	Shared<PropagationRequest> request;
	
	if ( library->requests.find( requestID, request ) )
	{
		const Bool boolValue = value != false;
		
		switch ( flag )
		{
			case GS_DIRECT:				request->flags.set( PropagationFlags::DIRECT, boolValue );				break;
			case GS_TRANSMISSION:		request->flags.set( PropagationFlags::TRANSMISSION, boolValue );		break;
			case GS_SPECULAR:			request->flags.set( PropagationFlags::SPECULAR, boolValue );			break;
			case GS_DIFFUSE:			request->flags.set( PropagationFlags::DIFFUSE, boolValue );				break;
			case GS_DIFFRACTION:		request->flags.set( PropagationFlags::DIFFRACTION, boolValue );			break;
			case GS_SPECULAR_CACHE:		request->flags.set( PropagationFlags::SPECULAR_CACHE, boolValue );		break;
			case GS_IR_CACHE:			request->flags.set( PropagationFlags::IR_CACHE, boolValue );			break;
			case GS_VISIBILITY_CACHE:	request->flags.set( PropagationFlags::VISIBILITY_CACHE, boolValue );	break;
			case GS_SOURCE_DIFFUSE:		request->flags.set( PropagationFlags::SOURCE_DIFFUSE, boolValue );		break;
			case GS_SOURCE_DIRECTIVITY:	request->flags.set( PropagationFlags::SOURCE_DIRECTIVITY, boolValue );	break;
			case GS_SOURCE_CLUSTERING:	request->flags.set( PropagationFlags::SOURCE_CLUSTERING, boolValue );	break;
			case GS_AIR_ABSORPTION:		request->flags.set( PropagationFlags::AIR_ABSORPTION, boolValue );		break;
			case GS_SAMPLED_IR:			request->flags.set( PropagationFlags::SAMPLED_IR, boolValue );			break;
			case GS_SAMPLED_IR_SOURCE_DIRECTIONS:		request->flags.set( PropagationFlags::SAMPLED_IR_SOURCE_DIRECTIONS, boolValue );	break;
			case GS_IR_THRESHOLD:		request->flags.set( PropagationFlags::IR_THRESHOLD, boolValue );		break;
			case GS_ADAPTIVE_IR_LENGTH:	request->flags.set( PropagationFlags::ADAPTIVE_IR_LENGTH, boolValue );	break;
			case GS_ADAPTIVE_QUALITY:	request->flags.set( PropagationFlags::ADAPTIVE_QUALITY, boolValue );	break;
			case GS_DOPPLER_SORTING:	request->flags.set( PropagationFlags::DOPPLER_SORTING, boolValue );		break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRequestGetParamF( gsRequestID requestID, gsParameter parameter, gsFloat* value )
{
	ScopedMutex lock( library->mutex );
	Shared<PropagationRequest> request;
	
	if ( value && library->requests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_TARGET_DT:					*value = request->targetDt;					break;
			case GS_IR_MIN_LENGTH:				*value = request->minIRLength;				break;
			case GS_IR_MAX_LENGTH:				*value = request->maxIRLength;				break;
			case GS_IR_GROWTH_RATE:				*value = request->irGrowthRate;				break;
			case GS_QUALITY:					*value = request->quality;					break;
			case GS_MIN_QUALITY:				*value = request->minQuality;				break;
			case GS_MAX_QUALITY:				*value = request->maxQuality;				break;
			case GS_SAMPLE_RATE:				*value = (gsFloat)request->sampleRate;		break;
			case GS_DOPPLER_THRESHOLD:			*value = request->dopplerThreshold;			break;
			case GS_RAY_OFFSET:					*value = request->rayOffset;				break;
			case GS_RESPONSE_TIME:				*value = request->responseTime;				break;
			case GS_VISIBILITY_CACHE_TIME:		*value = request->visibilityCacheTime;		break;
			
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRequestSetParamF( gsRequestID requestID, gsParameter parameter, gsFloat value )
{
	ScopedMutex lock( library->mutex );
	Shared<PropagationRequest> request;
	
	if ( library->requests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_TARGET_DT:					request->targetDt = math::clamp( value, Float(0.001), Float(1000.0) );			break;
			case GS_IR_MIN_LENGTH:				request->minIRLength = math::clamp( value, Float(0.001), Float(100.0) );		break;
			case GS_IR_MAX_LENGTH:				request->maxIRLength = math::clamp( value, Float(0.001), Float(100.0) );		break;
			case GS_IR_GROWTH_RATE:				request->irGrowthRate = math::clamp( value, Float(0.001), Float(100.0) );		break;
			case GS_QUALITY:					request->quality = math::clamp( value, Float(0.001), Float(100.0) );			break;
			case GS_MIN_QUALITY:				request->minQuality = math::clamp( value, Float(0.001), Float(100.0) );			break;
			case GS_MAX_QUALITY:				request->maxQuality = math::clamp( value, Float(0.001), Float(100.0) );			break;
			case GS_SAMPLE_RATE:				request->sampleRate = math::max( SampleRate(value), SampleRate(0) );			break;
			case GS_DOPPLER_THRESHOLD:			request->dopplerThreshold = math::clamp( value, Float(0.001), Float(1000.0) );	break;
			case GS_RAY_OFFSET:					request->rayOffset = math::clamp( value, math::epsilon<Float>(), Float(100.0) );	break;
			case GS_RESPONSE_TIME:				request->responseTime = math::clamp( value, Float(0.0), Float(100.0) );			break;
			case GS_VISIBILITY_CACHE_TIME:		request->visibilityCacheTime = math::clamp( value, Float(0.0), Float(100.0) );		break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRequestGetParamI( gsRequestID requestID, gsParameter parameter, gsSize* value )
{
	ScopedMutex lock( library->mutex );
	Shared<PropagationRequest> request;
	
	if ( value && library->requests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_PROPAGATION_THREAD_COUNT:	*value = (gsSize)request->numThreads;					break;
			case GS_DIRECT_RAY_COUNT:			*value = (gsSize)request->numDirectRays;				break;
			case GS_DIFFRACTION_MAX_DEPTH:		*value = (gsSize)request->maxDiffractionDepth;			break;
			case GS_DIFFRACTION_MAX_ORDER:		*value = (gsSize)request->maxDiffractionOrder;			break;
			case GS_SPECULAR_MAX_DEPTH:			*value = (gsSize)request->maxSpecularDepth;				break;
			case GS_SPECULAR_RAY_COUNT:			*value = (gsSize)request->numSpecularRays;				break;
			case GS_SPECULAR_SAMPLE_COUNT:		*value = (gsSize)request->numSpecularSamples;			break;
			case GS_DIFFUSE_MAX_DEPTH:			*value = (gsSize)request->maxDiffuseDepth;				break;
			case GS_DIFFUSE_RAY_COUNT:			*value = (gsSize)request->numDiffuseRays;				break;
			case GS_DIFFUSE_SAMPLE_COUNT:		*value = (gsSize)request->numDiffuseSamples;			break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRequestSetParamI( gsRequestID requestID, gsParameter parameter, gsSize value )
{
	ScopedMutex lock( library->mutex );
	Shared<PropagationRequest> request;
	
	if ( library->requests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_PROPAGATION_THREAD_COUNT:	request->numThreads = math::clamp( (Size)value, Size(1), 2*CPU::getCount() );	break;
			case GS_DIRECT_RAY_COUNT:			request->numDirectRays = math::clamp( (Size)value, Size(1), Size(1000000000) );	break;
			case GS_DIFFRACTION_MAX_DEPTH:		request->maxDiffractionDepth = math::min( (Size)value, Size(1000) );			break;
			case GS_DIFFRACTION_MAX_ORDER:		request->maxDiffractionOrder = math::min( (Size)value, Size(10) );				break;
			case GS_SPECULAR_MAX_DEPTH:			request->maxSpecularDepth = math::min( (Size)value, Size(1000) );				break;
			case GS_SPECULAR_RAY_COUNT:			request->numSpecularRays = math::min( (Size)value, Size(1000000000) );			break;
			case GS_SPECULAR_SAMPLE_COUNT:		request->numSpecularSamples = math::clamp( (Size)value, Size(1), Size(10000) );	break;
			case GS_DIFFUSE_MAX_DEPTH:			request->maxDiffuseDepth = math::min( (Size)value, Size(1000) );				break;
			case GS_DIFFUSE_RAY_COUNT:			request->numDiffuseRays = math::min( (Size)value, Size(1000000000) );			break;
			case GS_DIFFUSE_SAMPLE_COUNT:		request->numDiffuseSamples = math::clamp( (Size)value, Size(1), Size(10000) );	break;
			case GS_VISIBILITY_RAY_COUNT:		request->numVisibilityRays = math::min( (Size)value, Size(1000000000) );		break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRequestReset( gsRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	Shared<PropagationRequest> request;
	
	if ( library->requests.find( requestID, request ) )
	{
		request->internalData.reset();
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Render Request Functions
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsRenderRequestID GSOUND_EXPORT gsNewRenderRequest()
{
	ScopedMutex lock( library->mutex );
	gsRequestID requestID = library->renderRequests.add( Shared<RenderRequest>::construct() );
	
	return requestID;
}




extern "C" void GSOUND_EXPORT gsDeleteRenderRequest( gsRenderRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	library->renderRequests.remove( requestID );
}




extern "C" gsBool GSOUND_EXPORT gsRenderRequestGetFlag( gsRenderRequestID requestID, gsFlag flag, gsBool* value )
{
	ScopedMutex lock( library->mutex );
	Shared<RenderRequest> request;
	
	if ( library->renderRequests.find( requestID, request ) )
	{
		switch ( flag )
		{
			case GS_CONVOLUTION:		*value = request->flags.isSet( RenderFlags::CONVOLUTION );		break;
			case GS_DISCRETE_PATHS:		*value = request->flags.isSet( RenderFlags::DISCRETE_PATHS );	break;
			case GS_HRTF:				*value = request->flags.isSet( RenderFlags::HRTF );				break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRenderRequestSetFlag( gsRenderRequestID requestID, gsFlag flag, gsBool value )
{
	ScopedMutex lock( library->mutex );
	Shared<RenderRequest> request;
	
	if ( library->renderRequests.find( requestID, request ) )
	{
		const Bool boolValue = value != false;
		
		switch ( flag )
		{
			case GS_CONVOLUTION:		request->flags.set( RenderFlags::CONVOLUTION, boolValue );		break;
			case GS_DISCRETE_PATHS:		request->flags.set( RenderFlags::DISCRETE_PATHS, boolValue );	break;
			case GS_HRTF:				request->flags.set( RenderFlags::HRTF, boolValue );				break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRenderRequestGetParamF( gsRenderRequestID requestID, gsParameter parameter, gsFloat* value )
{
	ScopedMutex lock( library->mutex );
	Shared<RenderRequest> request;
	
	if ( value && library->renderRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_SAMPLE_RATE:				*value = (gsFloat)request->sampleRate;		break;
			case GS_IR_MAX_LENGTH:				*value = request->maxIRLength;				break;
			case GS_MAX_LATENCY:				*value = request->maxLatency;				break;
			case GS_MAX_PATH_DELAY:				*value = request->maxPathDelay;				break;
			case GS_IR_FADE_TIME:				*value = request->irFadeTime;				break;
			case GS_PATH_FADE_TIME:				*value = request->pathFadeTime;				break;
			case GS_SOURCE_FADE_TIME:			*value = request->sourceFadeTime;			break;
			case GS_CLUSTER_FADE_IN_TIME:		*value = request->clusterFadeInTime;		break;
			case GS_CLUSTER_FADE_OUT_TIME:		*value = request->clusterFadeOutTime;		break;
			case GS_VOLUME:						*value = request->volume;					break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRenderRequestSetParamF( gsRequestID requestID, gsParameter parameter, gsFloat value )
{
	ScopedMutex lock( library->mutex );
	Shared<RenderRequest> request;
	
	if ( library->renderRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_SAMPLE_RATE:				request->sampleRate = math::max( SampleRate(value), SampleRate(0) );			break;
			case GS_IR_MAX_LENGTH:				request->maxIRLength = math::max( value, Float(0) );							break;
			case GS_MAX_LATENCY:				request->maxLatency = math::max( value, Float(0) );								break;
			case GS_MAX_PATH_DELAY:				request->maxPathDelay = math::max( value, Float(0) );							break;
			case GS_IR_FADE_TIME:				request->irFadeTime = math::clamp( value, Float(0.01), Float(10) );				break;
			case GS_PATH_FADE_TIME:				request->pathFadeTime = math::clamp( value, Float(0.01), Float(10) );			break;
			case GS_SOURCE_FADE_TIME:			request->sourceFadeTime = math::clamp( value, Float(0.01), Float(10) );			break;
			case GS_CLUSTER_FADE_IN_TIME:		request->clusterFadeInTime = math::clamp( value, Float(0.01), Float(10) );		break;
			case GS_CLUSTER_FADE_OUT_TIME:		request->clusterFadeOutTime = math::clamp( value, Float(0.01), Float(10) );		break;
			case GS_VOLUME:						request->volume = math::max( value, Float(0) );									break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRenderRequestGetParamI( gsRequestID requestID, gsParameter parameter, gsSize* value )
{
	ScopedMutex lock( library->mutex );
	Shared<RenderRequest> request;
	
	if ( value && library->renderRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_CHANNEL_LAYOUT:				*value = convertChannelLayout( request->channelLayout );	break;
			case GS_UPDATE_THREAD_COUNT:		*value = (gsSize)request->numUpdateThreads;					break;
			case GS_MAX_SOURCE_PATH_COUNT:		*value = (gsSize)request->maxSourcePathCount;				break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRenderRequestSetParamI( gsRequestID requestID, gsParameter parameter, gsSize value )
{
	ScopedMutex lock( library->mutex );
	Shared<RenderRequest> request;
	
	if ( library->renderRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_CHANNEL_LAYOUT:				request->channelLayout = convertChannelLayout( (gsChannelLayout)value );	break;
			case GS_UPDATE_THREAD_COUNT:		request->numUpdateThreads = math::clamp( (Size)value, Size(1), 2*CPU::getCount() ); break;
			case GS_MAX_SOURCE_PATH_COUNT:		request->maxSourcePathCount = (Size)value;									break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsRenderRequestSetHRTF( gsRequestID requestID, gsHRTFID hrtfID )
{
	ScopedMutex lock( library->mutex );
	Shared<RenderRequest> request;
	Shared<om::sound::HRTF> hrtf;
	
	if ( library->renderRequests.find( requestID, request ) &&
		library->hrtfs.find( hrtfID, hrtf ) )
	{
		request->hrtf = hrtf;
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Render Request Functions
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsMeshRequestID GSOUND_EXPORT gsNewMeshRequest()
{
	ScopedMutex lock( library->mutex );
	gsRequestID requestID = library->meshRequests.add( Shared<MeshRequest>::construct() );
	
	return requestID;
}




extern "C" void GSOUND_EXPORT gsDeleteMeshRequest( gsMeshRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	library->meshRequests.remove( requestID );
}




extern "C" gsBool GSOUND_EXPORT gsMeshRequestGetFlag( gsMeshRequestID requestID, gsFlag flag, gsBool* value )
{
	ScopedMutex lock( library->mutex );
	Shared<MeshRequest> request;
	
	if ( library->meshRequests.find( requestID, request ) )
	{
		switch ( flag )
		{
			case GS_DIFFRACTION_EDGES:	*value = request->flags.isSet( MeshFlags::DIFFRACTION_EDGES );	break;
			case GS_DIFFRACTION_GRAPH:	*value = request->flags.isSet( MeshFlags::DIFFRACTION_GRAPH );	break;
			case GS_VOXELIZE:			*value = request->flags.isSet( MeshFlags::VOXELIZE );			break;
			case GS_WELD:				*value = request->flags.isSet( MeshFlags::WELD );				break;
			case GS_SIMPLIFIY:			*value = request->flags.isSet( MeshFlags::SIMPLIFY );			break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMeshRequestSetFlag( gsMeshRequestID requestID, gsFlag flag, gsBool value )
{
	ScopedMutex lock( library->mutex );
	Shared<MeshRequest> request;
	
	if ( library->meshRequests.find( requestID, request ) )
	{
		const Bool boolValue = value != false;
		
		switch ( flag )
		{
			case GS_DIFFRACTION_EDGES:	request->flags.set( MeshFlags::DIFFRACTION_EDGES, boolValue );	break;
			case GS_DIFFRACTION_GRAPH:	request->flags.set( MeshFlags::DIFFRACTION_GRAPH, boolValue );	break;
			case GS_VOXELIZE:			request->flags.set( MeshFlags::VOXELIZE, boolValue );			break;
			case GS_WELD:				request->flags.set( MeshFlags::WELD, boolValue );				break;
			case GS_SIMPLIFIY:			request->flags.set( MeshFlags::SIMPLIFY, boolValue );			break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMeshRequestGetParamF( gsMeshRequestID requestID, gsParameter parameter, gsFloat* value )
{
	ScopedMutex lock( library->mutex );
	Shared<MeshRequest> request;
	
	if ( value && library->meshRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_VOXEL_SIZE:						*value = request->voxelSize;					break;
			case GS_WELD_TOLERANCE:					*value = request->weldTolerance;				break;
			case GS_SIMPLIFY_TOLERANCE:				*value = request->simplifyTolerance;			break;
			case GS_MIN_DIFFRACTION_EDGE_ANGLE:		*value = request->minDiffractionEdgeAngle;		break;
			case GS_MIN_DIFFRACTION_EDGE_LENGTH:	*value = request->minDiffractionEdgeLength;		break;
			case GS_EDGE_OFFSET:					*value = request->edgeOffset;					break;
			case GS_DIFFUSE_RESOLUTION:				*value = request->diffuseResolution;			break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMeshRequestSetParamF( gsRequestID requestID, gsParameter parameter, gsFloat value )
{
	ScopedMutex lock( library->mutex );
	Shared<MeshRequest> request;
	
	if ( library->meshRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_VOXEL_SIZE:						request->voxelSize = math::max( value, Float(0) );									break;
			case GS_WELD_TOLERANCE:					request->weldTolerance = math::max( value, Float(0) );								break;
			case GS_SIMPLIFY_TOLERANCE:				request->simplifyTolerance = math::max( value, Float(0) );							break;
			case GS_MIN_DIFFRACTION_EDGE_ANGLE:		request->minDiffractionEdgeAngle = math::clamp( value, Float(0.01), Float(10) );	break;
			case GS_MIN_DIFFRACTION_EDGE_LENGTH:	request->minDiffractionEdgeLength = math::clamp( value, Float(0.01), Float(10) );	break;
			case GS_EDGE_OFFSET:					request->edgeOffset = math::clamp( value, Float(0.01), Float(10) );					break;
			case GS_DIFFUSE_RESOLUTION:				request->diffuseResolution = math::clamp( value, Float(0.01), Float(10) );			break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMeshRequestGetParamI( gsRequestID requestID, gsParameter parameter, gsSize* value )
{
	ScopedMutex lock( library->mutex );
	Shared<MeshRequest> request;
	
	if ( value && library->meshRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_EDGE_RAY_COUNT_MIN:			*value = (gsSize)request->minRaysPerEdge;		break;
			case GS_EDGE_RAY_COUNT_MAX:			*value = (gsSize)request->maxRaysPerEdge;		break;
			case GS_PREPROCESS_THREAD_COUNT:	*value = (gsSize)request->numThreads;			break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMeshRequestSetParamI( gsRequestID requestID, gsParameter parameter, gsSize value )
{
	ScopedMutex lock( library->mutex );
	Shared<MeshRequest> request;
	
	if ( library->meshRequests.find( requestID, request ) )
	{
		switch ( parameter )
		{
			case GS_EDGE_RAY_COUNT_MIN:			request->minRaysPerEdge = (Size)value;		break;
			case GS_EDGE_RAY_COUNT_MAX:			request->maxRaysPerEdge = (Size)value;		break;
			case GS_PREPROCESS_THREAD_COUNT:	request->numThreads = math::clamp( (Size)value, Size(1), 2*CPU::getCount() ); break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Scene Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsSceneID GSOUND_EXPORT gsNewScene()
{
	ScopedMutex lock( library->mutex );
	gsSceneID sceneID = library->scenes.add( Shared<SoundScene>::construct() );
	
	return sceneID;
}




extern "C" void GSOUND_EXPORT gsDeleteScene( gsSceneID sceneID )
{
	ScopedMutex lock( library->mutex );
	library->scenes.remove( sceneID );
}




extern "C" gsBool GSOUND_EXPORT gsSceneGetObjectCount( gsSceneID sceneID, gsSize* objectCount )
{
	Shared<SoundScene> scene;
	
	if ( objectCount && library->scenes.find( sceneID, scene ) )
	{
		*objectCount = scene->getObjectCount();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneAddObject( gsSceneID sceneID, gsObjectID objectID )
{
	Shared<SoundScene> scene;
	Shared<SoundObject> object;
	
	if ( library->scenes.find( sceneID, scene ) && library->objects.find( objectID, object ) )
	{
		scene->addObject( object );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneRemoveObject( gsSceneID sceneID, gsObjectID objectID )
{
	Shared<SoundScene> scene;
	Shared<SoundObject> object;
	
	if ( library->scenes.find( sceneID, scene ) && library->objects.find( objectID, object ) )
		return scene->removeObject( object );
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneClearObjects( gsSceneID sceneID )
{
	Shared<SoundScene> scene;
	
	if ( library->scenes.find( sceneID, scene ) )
	{
		scene->clearObjects();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneGetSourceCount( gsSceneID sceneID, gsSize* sourceCount )
{
	Shared<SoundScene> scene;
	
	if ( sourceCount && library->scenes.find( sceneID, scene ) )
	{
		*sourceCount = scene->getSourceCount();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneAddSource( gsSceneID sceneID, gsSourceID sourceID )
{
	Shared<SoundScene> scene;
	Shared<SoundSource> source;
	
	if ( library->scenes.find( sceneID, scene ) && library->sources.find( sourceID, source ) )
	{
		scene->addSource( source );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneRemoveSource( gsSceneID sceneID, gsSourceID sourceID )
{
	Shared<SoundScene> scene;
	Shared<SoundSource> source;
	
	if ( library->scenes.find( sceneID, scene ) && library->sources.find( sourceID, source ) )
		return scene->removeSource( source );
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneClearSources( gsSceneID sceneID )
{
	Shared<SoundScene> scene;
	
	if ( library->scenes.find( sceneID, scene ) )
	{
		scene->clearSources();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneGetListenerCount( gsSceneID sceneID, gsSize* listenerCount )
{
	Shared<SoundScene> scene;
	
	if ( listenerCount && library->scenes.find( sceneID, scene ) )
	{
		*listenerCount = scene->getListenerCount();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneAddListener( gsSceneID sceneID, gsListenerID listenerID )
{
	Shared<SoundScene> scene;
	Shared<SoundListener> listener;
	
	if ( library->scenes.find( sceneID, scene ) && library->listeners.find( listenerID, listener ) )
	{
		scene->addListener( listener );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneRemoveListener( gsSceneID sceneID, gsListenerID listenerID )
{
	Shared<SoundScene> scene;
	Shared<SoundListener> listener;
	
	if ( library->scenes.find( sceneID, scene ) && library->listeners.find( listenerID, listener ) )
		return scene->removeListener( listener );
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSceneClearListeners( gsSceneID sceneID )
{
	Shared<SoundScene> scene;
	
	if ( library->scenes.find( sceneID, scene ) )
	{
		scene->clearListeners();
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Source Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsSourceID GSOUND_EXPORT gsNewSource()
{
	Shared<SoundSource> source = Shared<SoundSource>::construct();
	
	ScopedMutex lock( library->mutex );
	gsSourceID sourceID = library->sources.add( source );
	
	return sourceID;
}




extern "C" void GSOUND_EXPORT gsDeleteSource( gsSourceID sourceID )
{
	ScopedMutex lock( library->mutex );
	library->sources.remove( sourceID );
}




extern "C" gsBool GSOUND_EXPORT gsSourceGetPosition( gsSourceID sourceID, gsFloat* x, gsFloat* y, gsFloat* z )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		const Vector3f& p = source->getPosition();
		
		if ( x ) *x = p.x;
		if ( y ) *y = p.y;
		if ( z ) *z = p.z;
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceSetPosition( gsSourceID sourceID, gsFloat x, gsFloat y, gsFloat z )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		source->setPosition( Vector3f( x, y, z ) );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceGetVelocity( gsSourceID sourceID, gsFloat* vx, gsFloat* vy, gsFloat* vz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		const Vector3f& v = source->getVelocity();
		
		if ( vx ) *vx = v.x;
		if ( vy ) *vy = v.y;
		if ( vz ) *vz = v.z;
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceSetVelocity( gsSourceID sourceID, gsFloat vx, gsFloat vy, gsFloat vz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		source->setVelocity( Vector3f( vx, vy, vz ) );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceGetRadius( gsSourceID sourceID, gsFloat* radius )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( radius && library->sources.find( sourceID, source ) )
	{
		*radius = source->getRadius();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceSetRadius( gsSourceID sourceID, gsFloat radius )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		source->setRadius( radius );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceGetOrientation( gsSourceID sourceID, gsFloat* xx, gsFloat* xy, gsFloat* xz,
																		gsFloat* yx, gsFloat* yy, gsFloat* yz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		const Matrix3f& o = source->getOrientation();
		
		if ( xx ) *xx = o.x.x;
		if ( xy ) *xy = o.x.y;
		if ( xz ) *xz = o.x.z;
		if ( yx ) *yx = o.y.x;
		if ( yy ) *yy = o.y.y;
		if ( yz ) *yz = o.y.z;
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceSetOrientation( gsSourceID sourceID, gsFloat xx, gsFloat xy, gsFloat xz,
																		gsFloat yx, gsFloat yy, gsFloat yz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		Vector3f x( xx, xy, xz );
		Vector3f y( yx, yy, yz );
		Vector3f z = math::cross( x, y );
		
		source->setOrientation( Matrix3f( x, y, z ) );
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceGetPowerLevel( gsSourceID sourceID, gsFloat* powerDBSWL )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( powerDBSWL && library->sources.find( sourceID, source ) )
	{
		*powerDBSWL = source->getPowerLevel();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceSetPowerLevel( gsSourceID sourceID, gsFloat powerDBSWL )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		source->setPowerLevel( powerDBSWL );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceGetPower( gsSourceID sourceID, gsFloat* power )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( power && library->sources.find( sourceID, source ) )
	{
		*power = source->getPower();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceSetPower( gsSourceID sourceID, gsFloat power )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		source->setPower( power );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceGetSampleRate( gsSourceID sourceID, gsSampleRate* sampleRate )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( sampleRate && library->sources.find( sourceID, source ) )
	{
		*sampleRate = source->getSampleRate();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceSetSampleRate( gsSourceID sourceID, gsSampleRate sampleRate )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		source->setSampleRate( sampleRate );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourcePlaySound( gsSourceID sourceID, gsSoundID soundID, gsFloat volume, gsBool loop )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	Shared<om::sound::Sound> sound;
	
	if ( library->sources.find( sourceID, source ) && library->sounds.find( soundID, sound ) )
	{
		Bool result = source->playSound( sound, volume, loop != false ) != 0;
		return result;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourcePauseSound( gsSourceID sourceID, gsSoundID soundID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	Shared<om::sound::Sound> sound;
	/*
	if ( library->sources.find( sourceID, source ) && library->sounds.find( soundID, sound ) )
	{
		Bool result = source->pauseSound( sound.getPointer() );
		return result;
	}
	*/
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceResumeSound( gsSourceID sourceID, gsSoundID soundID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	Shared<om::sound::Sound> sound;
	/*
	if ( library->sources.find( sourceID, source ) && library->sounds.find( soundID, sound ) )
	{
		Bool result = source->resumeSound( sound.getPointer() );
		return result;
	}
	*/
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceStopSound( gsSourceID sourceID, gsSoundID soundID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	Shared<om::sound::Sound> sound;
	/*
	if ( library->sources.find( sourceID, source ) && library->sounds.find( soundID, sound ) )
	{
		Bool result = source->stopSound( sound.getPointer() );
		return result;
	}
	*/
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSourceStopSounds( gsSourceID sourceID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundSource> source;
	
	if ( library->sources.find( sourceID, source ) )
	{
		source->stopSounds();
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Listener Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsListenerID GSOUND_EXPORT gsNewListener()
{
	ScopedMutex lock( library->mutex );
	gsListenerID listenerID = library->listeners.add( Shared<SoundListener>::construct() );
	
	return listenerID;
}




extern "C" void GSOUND_EXPORT gsDeleteListener( gsListenerID listenerID )
{
	ScopedMutex lock( library->mutex );
	library->listeners.remove( listenerID );
}




extern "C" gsBool GSOUND_EXPORT gsListenerGetPosition( gsListenerID listenerID, gsFloat* x, gsFloat* y, gsFloat* z )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		const Vector3f& p = listener->getPosition();
		if ( x ) *x = p.x;
		if ( y ) *y = p.y;
		if ( z ) *z = p.z;
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerSetPosition( gsListenerID listenerID, gsFloat x, gsFloat y, gsFloat z )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		listener->setPosition( Vector3f( x, y, z ) );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerGetVelocity( gsListenerID listenerID, gsFloat* vx, gsFloat* vy, gsFloat* vz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		const Vector3f& v = listener->getVelocity();
		if ( vx ) *vx = v.x;
		if ( vy ) *vy = v.y;
		if ( vz ) *vz = v.z;
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerSetVelocity( gsListenerID listenerID, gsFloat vx, gsFloat vy, gsFloat vz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		listener->setVelocity( Vector3f( vx, vy, vz ) );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerGetRadius( gsListenerID listenerID, gsFloat* radius )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( radius && library->listeners.find( listenerID, listener ) )
	{
		*radius = listener->getRadius();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerSetRadius( gsListenerID listenerID, gsFloat radius )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		listener->setRadius( radius );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerGetSensitivity( gsListenerID listenerID, gsFloat* sensitivity )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( sensitivity && library->listeners.find( listenerID, listener ) )
	{
		*sensitivity = listener->getSensitivity();
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerSetSensitivity( gsListenerID listenerID, gsFloat sensitivity )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		listener->setSensitivity( sensitivity );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerGetOrientation( gsListenerID listenerID, gsFloat* xx, gsFloat* xy, gsFloat* xz,
																			gsFloat* yx, gsFloat* yy, gsFloat* yz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		const Matrix3f& o = listener->getOrientation();
		if ( xx ) *xx = o.x.x;
		if ( xy ) *xy = o.x.y;
		if ( xz ) *xz = o.x.z;
		if ( yx ) *yx = o.y.x;
		if ( yy ) *yy = o.y.y;
		if ( yz ) *yz = o.y.z;
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsListenerSetOrientation( gsListenerID listenerID, gsFloat xx, gsFloat xy, gsFloat xz,
																			gsFloat yx, gsFloat yy, gsFloat yz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundListener> listener;
	
	if ( library->listeners.find( listenerID, listener ) )
	{
		Vector3f x( xx, xy, xz );
		Vector3f y( yx, yy, yz );
		Vector3f z = math::cross( x, y );
		
		listener->setOrientation( Matrix3f( x, y, z ) );
		
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Object Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsObjectID GSOUND_EXPORT gsNewObject()
{
	ScopedMutex lock( library->mutex );
	gsObjectID objectID = library->objects.add( Shared<SoundObject>::construct() );
	
	return objectID;
}




extern "C" void GSOUND_EXPORT gsDeleteObject( gsObjectID objectID )
{
	ScopedMutex lock( library->mutex );
	library->objects.remove( objectID );
}




extern "C" gsBool GSOUND_EXPORT gsObjectSetMesh( gsObjectID objectID, gsMeshID meshID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundObject> object;
	Shared<SoundMesh> mesh;
	
	if ( library->objects.find( objectID, object ) && library->meshes.find( meshID, mesh ) )
	{
		object->setMesh( mesh );
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsObjectGetPosition( gsObjectID objectID, gsFloat* x, gsFloat* y, gsFloat* z )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundObject> object;
	
	if ( library->objects.find( objectID, object ) )
	{
		const Vector3f& p = object->getPosition();
		
		if ( x ) *x = p.x;
		if ( y ) *y = p.y;
		if ( z ) *z = p.z;
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsObjectSetPosition( gsObjectID objectID, gsFloat x, gsFloat y, gsFloat z )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundObject> object;
	
	if ( library->objects.find( objectID, object ) )
	{
		object->setPosition( Vector3f( x, y, z ) );
		return true;
	}
	
	return false;
}





extern "C" gsBool GSOUND_EXPORT gsObjectGetOrientation( gsObjectID objectID, gsFloat* xx, gsFloat* xy, gsFloat* xz,
																		gsFloat* yx, gsFloat* yy, gsFloat* yz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundObject> object;
	
	if ( library->objects.find( objectID, object ) )
	{
		const Matrix3f& o = object->getOrientation();
		
		if ( xx ) *xx = o.x.x;
		if ( xy ) *xy = o.x.y;
		if ( xz ) *xz = o.x.z;
		if ( yx ) *yx = o.y.x;
		if ( yy ) *yy = o.y.y;
		if ( yz ) *yz = o.y.z;
		
		return true;
	}
	
	return false;
}





extern "C" gsBool GSOUND_EXPORT gsObjectSetOrientation( gsObjectID objectID, gsFloat xx, gsFloat xy, gsFloat xz,
																		gsFloat yx, gsFloat yy, gsFloat yz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundObject> object;
	
	if ( library->objects.find( objectID, object ) )
	{
		Vector3f x( xx, xy, xz );
		Vector3f y( yx, yy, yz );
		Vector3f z = math::cross( x, y );
		
		object->setOrientation( Matrix3f( x, y, z ) );
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsObjectGetScale( gsObjectID objectID, gsFloat* sx, gsFloat* sy, gsFloat* sz )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundObject> object;
	
	if ( library->objects.find( objectID, object ) )
	{
		const Vector3f& s = object->getScale();
		
		if ( sx ) *sx = s.x;
		if ( sy ) *sy = s.y;
		if ( sz ) *sz = s.z;
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsObjectSetScale( gsObjectID objectID, gsFloat scaleX, gsFloat scaleY, gsFloat scaleZ )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundObject> object;
	
	if ( library->objects.find( objectID, object ) )
	{
		object->setScale( Vector3f( scaleX, scaleY, scaleZ ) );
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mesh Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsMeshID GSOUND_EXPORT gsNewMesh()
{
	ScopedMutex lock( library->mutex );
	gsMeshID meshID = library->meshes.add( Shared<SoundMesh>::construct() );
	
	return meshID;
}




extern "C" void GSOUND_EXPORT gsDeleteMesh( gsMeshID meshID )
{
	ScopedMutex lock( library->mutex );
	library->meshes.remove( meshID );
}




extern "C" gsBool GSOUND_EXPORT gsMeshSetData( gsMeshID meshID, gsSystemID systemID, gsMeshRequestID meshRequestID,
											const gsFloat* vertices, gsSize numVertices,
											const gsIndex* triangles, gsSize numTriangles,
											const gsMaterialID* materialIDs, gsSize numMaterials )
{
	Shared<SoundMesh> mesh;
	Shared<SoundPropagationSystem> system;
	Shared<MeshRequest> meshRequest;
	
	ScopedMutex lock( library->mutex );
	
	if ( vertices && triangles &&
		library->meshes.find( meshID, mesh ) &&
		library->systems.find( systemID, system ) &&
		library->meshRequests.find( meshRequestID, meshRequest ) )
	{
		// Unpack the vertices, triangles, and materials.
		ArrayList<SoundVertex> newVertices( (Size)numVertices );
		ArrayList<SoundTriangle> newTriangles( (Size)numTriangles );
		ArrayList<SoundMaterial> newMaterials( (Size)numMaterials );
		
		for ( Index i = 0; i < numVertices; i++ )
			newVertices.add( SoundVertex( vertices[i*3 + 0], vertices[i*3 + 1], vertices[i*3 + 2] ) );
		
		for ( Index i = 0; i < numTriangles; i++ )
			newTriangles.add( SoundTriangle( (Index)triangles[i*4 + 0], (Index)triangles[i*4 + 1], (Index)triangles[i*4 + 2], (Index)triangles[i*4 + 3] ) );
		
		for ( Index i = 0; i < numMaterials; i++ )
		{
			Shared<SoundMaterial> material;
			
			if ( materialIDs && library->materials.find( materialIDs[i], material ) )
				newMaterials.add( *material );
			else
				newMaterials.add( SoundMaterial() );
		}
		
		return system->processMesh( newVertices.getPointer(), newVertices.getSize(),
									newTriangles.getPointer(), newTriangles.getSize(),
									newMaterials.getPointer(), newMaterials.getSize(), *meshRequest, *mesh );
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMeshSetMaterial( gsMeshID meshID, gsIndex materialIndex, gsMaterialID materialID )
{
	ScopedMutex lock( library->mutex );
	Shared<SoundMesh> mesh;
	Shared<SoundMaterial> material;
	
	if ( library->meshes.find( meshID, mesh ) && library->materials.find( materialID, material ) )
	{
		mesh->setMaterial( (Index)materialIndex, *material );
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Material Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsMaterialID GSOUND_EXPORT gsNewMaterial()
{
	ScopedMutex lock( library->mutex );
	gsMaterialID materialID = library->materials.add( Shared<SoundMaterial>::construct() );
	
	return materialID;
}




extern "C" void GSOUND_EXPORT gsDeleteMaterial( gsMaterialID materialID )
{
	ScopedMutex lock( library->mutex );
	library->materials.remove( materialID );
}




extern "C" gsBool GSOUND_EXPORT gsMaterialGet( gsMaterialID materialID, gsResponseType responseType, gsFloat frequency, gsFloat* value )
{
	Shared<SoundMaterial> material;
	ScopedMutex lock( library->mutex );
	
	if ( value && library->materials.find( materialID, material ) )
	{
		switch ( responseType )
		{
			case GS_REFLECTIVITY:	*value = material->getReflectivity().getFrequencyGain( frequency ); break;
			case GS_SCATTERING:		*value = material->getScattering().getFrequencyGain( frequency );	break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMaterialSet( gsMaterialID materialID, gsResponseType responseType, gsFloat frequency, gsFloat value )
{
	Shared<SoundMaterial> material;
	ScopedMutex lock( library->mutex );
	
	if ( value && library->materials.find( materialID, material ) )
	{
		switch ( responseType )
		{
			case GS_REFLECTIVITY:	material->getReflectivity().setFrequency( frequency, value ); break;
			case GS_SCATTERING:		material->getScattering().setFrequency( frequency, value );	break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsMaterialReset( gsMaterialID materialID, gsResponseType responseType, gsFloat value )
{
	Shared<SoundMaterial> material;
	ScopedMutex lock( library->mutex );
	
	if ( value && library->materials.find( materialID, material ) )
	{
		switch ( responseType )
		{
			case GS_REFLECTIVITY:	material->getReflectivity().reset( value ); break;
			case GS_SCATTERING:		material->getScattering().reset( value );	break;
			default:
				return false;
		}
		
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsSoundID GSOUND_EXPORT gsNewSound()
{
	ScopedMutex lock( library->mutex );
	gsSoundID soundID = library->sounds.add( Shared<om::sound::Sound>::construct() );
	
	return soundID;
}




extern "C" void GSOUND_EXPORT gsDeleteSound( gsSoundID soundID )
{
	ScopedMutex lock( library->mutex );
	library->sounds.remove( soundID );
}




extern "C" gsBool GSOUND_EXPORT gsSoundSetFile( gsSoundID soundID, const char* filePath, gsBool streaming )
{
	ScopedMutex lock( library->mutex );
	Shared<om::sound::Sound> sound;
	
	if ( filePath && library->sounds.find( soundID, sound ) )
	{
		om::UTF8String path( filePath );
		om::resources::ResourceID resID( path );
		om::resources::ResourceManager& resourceManager = library->getResourceManager();
		om::resources::Resource<om::sound::Sound> resource = resourceManager.load<om::sound::Sound>( resID );
		
		if ( resource.isNull() )
			return false;
		
		sound->setStream( resource.getData() );
		
		if ( !streaming )
			sound->loadBuffer( true );
		
		return true;
	}
	
	return false;
}




extern "C" gsBool GSOUND_EXPORT gsSoundSetData( gsSoundID soundID, const gsFloat* samples, gsSize numChannels,
												gsSize numSamples, gsSampleRate sampleRate )
{
	ScopedMutex lock( library->mutex );
	Shared<om::sound::Sound> sound;
	
	if ( samples && library->sounds.find( soundID, sound ) )
	{
		Shared<SoundBuffer> buffer = Shared<SoundBuffer>::construct( numChannels, numSamples, sampleRate );
		
		for ( Index c = 0; c < numChannels; c++ )
		{
			Sample32f* channel = buffer->getChannel(c);
			const gsFloat* sample = samples + c;
			const gsFloat* const samplesEnd = sample + numChannels*numSamples;
			
			while ( sample != samplesEnd )
			{
				*channel = *sample;
				channel++;
				sample++;
			}
		}
		
		sound->setBuffer( buffer );
		
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		HRTF Methods
//############		
//##########################################################################################
//##########################################################################################




extern "C" gsHRTFID GSOUND_EXPORT gsNewHRTF()
{
	ScopedMutex lock( library->mutex );
	gsHRTFID hrtfID = library->hrtfs.add( Shared<om::sound::HRTF>::construct() );
	
	return hrtfID;
}




extern "C" void GSOUND_EXPORT gsDeleteHRTF( gsHRTFID hrtfID )
{
	ScopedMutex lock( library->mutex );
	library->hrtfs.remove( hrtfID );
}




extern "C" gsBool GSOUND_EXPORT gsHRTFSetFile( gsHRTFID hrtfID, const char* filePath )
{
	ScopedMutex lock( library->mutex );
	Shared<om::sound::HRTF> hrtf;
	
	if ( filePath && library->hrtfs.find( hrtfID, hrtf ) )
	{
		om::UTF8String path( filePath );
		om::resources::ResourceID resID( path );
		om::resources::ResourceManager& resourceManager = library->getResourceManager();
		om::resources::Resource<om::sound::HRTF> resource = resourceManager.load<om::sound::HRTF>( resID );
		
		if ( resource.isNull() )
			return false;
		
		library->hrtfs.set( hrtfID, resource.getData() );
		return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		C++ Interface Extension
//############		
//##########################################################################################
//##########################################################################################




PropagationRequest* gsGetRequest( gsRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<PropagationRequest> request;
	library->requests.find( requestID, request );
	
	return request;
}




RenderRequest* gsGetRenderRequest( gsRenderRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<RenderRequest> request;
	library->renderRequests.find( requestID, request );
	
	return request;
}




MeshRequest* gsGetMeshRequest( gsMeshRequestID requestID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<MeshRequest> request;
	library->meshRequests.find( requestID, request );
	
	return request;
}




SoundPropagationSystem* gsGetSystem( gsSystemID systemID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<SoundPropagationSystem> system;
	library->systems.find( systemID, system );
	
	return system;
}




SoundScene* gsGetScene( gsSceneID sceneID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<SoundScene> scene;
	library->scenes.find( sceneID, scene );
	
	return scene;
}



SoundSource* gsGetSource( gsSourceID sourceID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<SoundSource> source;
	library->sources.find( sourceID, source );
	
	return source;
}




SoundListener* gsGetListener( gsListenerID listenerID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<SoundListener> listener;
	library->listeners.find( listenerID, listener );
	
	return listener;
}




SoundObject* gsGetObject( gsObjectID objectID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<SoundObject> object;
	library->objects.find( objectID, object );
	
	return object;
}




SoundMesh* gsGetMesh( gsMeshID meshID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<SoundMesh> mesh;
	library->meshes.find( meshID, mesh );
	
	return mesh;
}




SoundMaterial* gsGetMaterial( gsMaterialID materialID )
{
	ScopedMutex lock( library->mutex );
	
	Shared<SoundMaterial> material;
	library->materials.find( materialID, material );
	
	return material;
}




