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

#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const om::resources::ResourceType SoundFilter:: RESOURCE_TYPE("SoundFilter");


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




SoundFilter:: SoundFilter()
	:	parameterMutex(),
		frameIndex( 0 ),
		numInputs( 1 ),
		numOutputs( 1 ),
		numMIDIInputs( 0 ),
		numMIDIOutputs( 0 ),
		isSynchronized( true )
{
}




SoundFilter:: SoundFilter( Size newNumInputs, Size newNumOutputs )
	:	parameterMutex(),
		frameIndex( 0 ),
		numInputs( UInt16(newNumInputs) ),
		numOutputs( UInt16(newNumOutputs) ),
		numMIDIInputs( 0 ),
		numMIDIOutputs( 0 ),
		isSynchronized( true )
{
}




SoundFilter:: SoundFilter( Size newNumInputs, Size newNumOutputs, Size newNumMIDIInputs, Size newNumMIDIOutputs )
	:	parameterMutex(),
		frameIndex( 0 ),
		numInputs( UInt16(newNumInputs) ),
		numOutputs( UInt16(newNumOutputs) ),
		numMIDIInputs( UInt16(newNumMIDIInputs) ),
		numMIDIOutputs( UInt16(newNumMIDIOutputs) ),
		isSynchronized( true )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundFilter:: ~SoundFilter()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Read Methods
//############		
//##########################################################################################
//##########################################################################################




SoundResult SoundFilter:: read( SoundBuffer& outputBuffer, Size numSamples )
{
	if ( outputBuffer.getSize() < numSamples )
		outputBuffer.setSize( numSamples );
	
	const SoundFrame inputFrame;
	SoundFrame outputFrame(&outputBuffer);
	
	//*****************************************************************************
	
	lockMutex();
	
	SoundResult result = this->processFrame( inputFrame, outputFrame, numSamples );
	frameIndex++;
	
	unlockMutex();
	
	return result;
}




SoundResult SoundFilter:: read( SoundFrame& outputFrame, Size numSamples )
{
	/// Make sure the output buffers are big enough.
	Size numValidOutputs = math::min( outputFrame.getBufferCount(), Size(numOutputs) );
	
	for ( Index i = 0; i < numValidOutputs; i++ )
	{
		SoundBuffer* outputBuffer = outputFrame.getBuffer(i);
		
		if ( outputBuffer && outputBuffer->getSize() < numSamples )
			outputBuffer->setSize( numSamples );
	}
	
	const SoundFrame inputFrame;
	
	//*****************************************************************************
	
	lockMutex();
	
	SoundResult result = this->processFrame( inputFrame, outputFrame, numSamples );
	frameIndex++;
	
	unlockMutex();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Write Methods
//############		
//##########################################################################################
//##########################################################################################




SoundResult SoundFilter:: write( const SoundBuffer& inputBuffer, Size numSamples )
{
	numSamples = math::min( numSamples, inputBuffer.getSize() );
	
	const SoundFrame inputFrame( (SoundBuffer*)&inputBuffer );
	SoundFrame outputFrame;
	
	//*****************************************************************************
	
	lockMutex();
	
	SoundResult result = this->processFrame( inputFrame, outputFrame, numSamples );
	frameIndex++;
	
	unlockMutex();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Process Methods
//############		
//##########################################################################################
//##########################################################################################




SoundResult SoundFilter:: process( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples )
{
	if ( numInputs > 0 )
		numSamples = math::min( numSamples, inputBuffer.getSize() );
	
	SoundFrame inputFrame( (SoundBuffer*)&inputBuffer );
	SoundFrame outputFrame(&outputBuffer);
	
	//*****************************************************************************
	
	lockMutex();
	
	SoundResult result = this->processFrame( inputFrame, outputFrame, numSamples );
	frameIndex++;
	
	unlockMutex();
	
	return result;
}




SoundResult SoundFilter:: process( const SoundBuffer& inputBuffer, SoundFrame& outputFrame, Size numSamples )
{
	if ( numInputs > 0 )
		numSamples = math::min( numSamples, inputBuffer.getSize() );
	
	SoundFrame inputFrame( (SoundBuffer*)&inputBuffer );
	
	//*****************************************************************************
	
	lockMutex();
	
	SoundResult result = this->processFrame( inputFrame, outputFrame, numSamples );
	frameIndex++;
	
	unlockMutex();
	
	return result;
}




SoundResult SoundFilter:: process( const SoundFrame& inputFrame, SoundFrame& outputFrame,
										Size numSamples )
{
	Size numValidInputs = math::min( inputFrame.getBufferCount(), Size(numInputs) );
	
	for ( Index i = 0; i < numValidInputs; i++ )
	{
		const SoundBuffer* inputBuffer = inputFrame.getBuffer(i);
		
		if ( inputBuffer != NULL )
			numSamples = math::min( inputBuffer->getSize(), numSamples );
	}
	
	//*****************************************************************************
	
	lockMutex();
	
	SoundResult result = this->processFrame( inputFrame, outputFrame, numSamples );
	frameIndex++;
	
	unlockMutex();
	
	return result;
}




SoundResult SoundFilter:: process( const SoundFrame& inputFrame, SoundBuffer& outputBuffer,
										Size numSamples )
{
	Size numValidInputs = math::min( inputFrame.getBufferCount(), Size(numInputs) );
	
	for ( Index i = 0; i < numValidInputs; i++ )
	{
		const SoundBuffer* inputBuffer = inputFrame.getBuffer(i);
		
		if ( inputBuffer != NULL )
			numSamples = math::min( inputBuffer->getSize(), numSamples );
	}
	
	SoundFrame outputFrame(&outputBuffer);
	
	//*****************************************************************************
	
	lockMutex();
	
	SoundResult result = this->processFrame( inputFrame, outputFrame, numSamples );
	frameIndex++;
	
	unlockMutex();
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Methods
//############		
//##########################################################################################
//##########################################################################################





void SoundFilter:: reset()
{
	lockMutex();
	
	// Reset the frame counter.
	frameIndex = 0;
	
	// Reset the subclass.
	this->resetStream();
	
	unlockMutex();
}




void SoundFilter:: resetStream()
{
	// Optionally implemented by subclasses.
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Input and Output Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String SoundFilter:: getInputName( Index inputIndex ) const
{
	if ( inputIndex < numInputs )
	{
		if ( numInputs == 1 )
			return UTF8String("Main Input");
		else
			return UTF8String("Input ") + inputIndex;
	}
	else
		return UTF8String();
}




UTF8String SoundFilter:: getOutputName( Index outputIndex ) const
{
	if ( outputIndex < numOutputs )
	{
		if ( numOutputs == 1 )
			return UTF8String("Main Output");
		else
			return UTF8String("Output ") + outputIndex;
	}
	else
		return UTF8String();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter MIDI Input and Output Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String SoundFilter:: getMIDIInputName( Index inputIndex ) const
{
	if ( inputIndex < Size(numMIDIInputs) )
	{
		if ( numMIDIInputs == 1 )
			return UTF8String("Main MIDI Input");
		else
			return UTF8String("MIDI Input ") + inputIndex;
	}
	else
		return UTF8String();
}




UTF8String SoundFilter:: getMIDIOutputName( Index outputIndex ) const
{
	if ( outputIndex < Size(numMIDIOutputs) )
	{
		if ( numMIDIOutputs == 1 )
			return UTF8String("Main MIDI Output");
		else
			return UTF8String("MIDI Output ") + outputIndex;
	}
	else
		return UTF8String();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String SoundFilter:: getName() const
{
	return UTF8String();
}




UTF8String SoundFilter:: getManufacturer() const
{
	return UTF8String();
}




FilterVersion SoundFilter:: getVersion() const
{
	return FilterVersion();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Latency Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Time SoundFilter:: getLatency() const
{
	return Time();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundFilter:: getParameterCount() const
{
	return 0;
}




Bool SoundFilter:: getParameterIndex( const UTF8String& parameterName, Index& parameterIndex ) const
{
	const Size numParameters = this->getParameterCount();
	FilterParameterInfo parameterInfo;
	
	for ( Index i = 0; i < numParameters; i++ )
	{
		this->getParameterInfo( i, parameterInfo );
		
		if ( parameterName == parameterInfo.getName() )
		{
			parameterIndex = i;
			return true;
		}
	}
	
	return false;
}




Bool SoundFilter:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	return false;
}




Bool SoundFilter:: getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const
{
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Value Read Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundFilter:: getParameter( Index parameterIndex, FilterParameter& value ) const
{
	return this->getParameterValue( parameterIndex, value );
}




Bool SoundFilter:: getParameter( Index parameterIndex, Bool& value ) const
{
	FilterParameter parameter;
	
	if ( this->getParameter( parameterIndex, parameter ) )
		return parameter.getValue( value );
	
	return false;
}




Bool SoundFilter:: getParameter( Index parameterIndex, Int64& value ) const
{
	FilterParameter parameter;
	
	if ( this->getParameter( parameterIndex, parameter ) )
		return parameter.getValue( value );
	
	return false;
}




Bool SoundFilter:: getParameter( Index parameterIndex, Float32& value ) const
{
	FilterParameter parameter;
	
	if ( this->getParameter( parameterIndex, parameter ) )
		return parameter.getValue( value );
	
	return false;
}




Bool SoundFilter:: getParameter( Index parameterIndex, Float64& value ) const
{
	FilterParameter parameter;
	
	if ( this->getParameter( parameterIndex, parameter ) )
		return parameter.getValue( value );
	
	return false;
}




Bool SoundFilter:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Value Write Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundFilter:: setParameter( Index parameterIndex, const FilterParameter& value )
{
	return this->setParameterValue( parameterIndex, value );
}




Bool SoundFilter:: setParameter( Index parameterIndex, Bool value )
{
	return this->setParameter( parameterIndex, FilterParameter( value ) );
}




Bool SoundFilter:: setParameter( Index parameterIndex, Int64 value )
{
	return this->setParameter( parameterIndex, FilterParameter( value ) );
}




Bool SoundFilter:: setParameter( Index parameterIndex, Float32 value )
{
	return this->setParameter( parameterIndex, FilterParameter( value ) );
}




Bool SoundFilter:: setParameter( Index parameterIndex, Float64 value )
{
	return this->setParameter( parameterIndex, FilterParameter( value ) );
}




Bool SoundFilter:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter State Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundFilter:: writeState( DataOutputStream& stream ) const
{
	// Write the native endianness marker to the stream.
	UInt8 endianness = Endianness();
	stream.writeData( &endianness, sizeof(UInt8) );
	
	// Write the number of parameters to the stream.
	const UInt64 numParameters = this->getParameterCount();
	stream.writeData( (const UByte*)&numParameters, sizeof(UInt64) );
	
	// Write all of the filter's parameters to the stream.
	FilterParameter parameter;
	
	for ( UInt64 i = 0; i < numParameters; i++ )
	{
		// Get the value for this parameter.
		if ( !this->getParameter( i, parameter ) )
			continue;
		
		// Write the parameter type.
		UInt64 parameterType = (UInt64)parameter.getType();
		stream.writeData( (const UByte*)&parameterType, sizeof(UInt64) );
		
		// Get the parameter value and write it to the stream.
		switch ( parameter.getType() )
		{
			case FilterParameterType::BOOLEAN:
			{
				// Write the parameter size.
				UInt64 parameterSize = sizeof(UInt64);
				stream.writeData( (const UByte*)&parameterSize, sizeof(UInt64) );
				
				Bool value = 0;
				parameter.getValue( value );
				UInt64 valueWide = (UInt64)value;
				stream.writeData( (const UByte*)&valueWide, sizeof(UInt64) );
			}
			break;
			
			case FilterParameterType::INTEGER:
			case FilterParameterType::ENUMERATION:
			{
				// Write the parameter size.
				UInt64 parameterSize = sizeof(Int64);
				stream.writeData( (const UByte*)&parameterSize, sizeof(UInt64) );
				
				Int64 value = 0;
				parameter.getValue( value );
				stream.writeData( (const UByte*)&value, sizeof(Int64) );
			}
			break;
			
			case FilterParameterType::FLOAT:
			{
				// Write the parameter size.
				UInt64 parameterSize = sizeof(Float);
				stream.writeData( (const UByte*)&parameterSize, sizeof(UInt64) );
				
				Float value = 0;
				parameter.getValue( value );
				stream.writeData( (const UByte*)&value, sizeof(Float) );
			}
			break;
			
			case FilterParameterType::DOUBLE:
			{
				// Write the parameter size.
				UInt64 parameterSize = sizeof(Double);
				stream.writeData( (const UByte*)&parameterSize, sizeof(UInt64) );
				
				Double value = 0;
				parameter.getValue( value );
				stream.writeData( (const UByte*)&value, sizeof(Double) );
			}
			break;
			
			default:
			{
				// Write the parameter size.
				UInt64 parameterSize = 0;
				stream.writeData( (const UByte*)&parameterSize, sizeof(UInt64) );
			}
		}
	}
	
	return true;
}




Bool SoundFilter:: readState( DataInputStream& stream )
{
	// Read the endianness marker from the stream.
	UInt8 endiannessValue = 0;
	stream.readData( (UByte*)&endiannessValue, sizeof(UInt8) );
	Endianness endianness( (Endianness::Enum)endiannessValue );
	
	// Read the number of parameters from the stream.
	UInt64 numParameters = 0;
	stream.readData( (UByte*)&numParameters, sizeof(UInt64) );
	numParameters = endianness.convertToNative( numParameters );
	
	// Read each of the filter's parameters from the stream.
	for ( UInt64 i = 0; i < numParameters; i++ )
	{
		// Read the parameter type.
		UInt64 parameterType = FilterParameterType::UNDEFINED;
		stream.readData( (UByte*)&parameterType, sizeof(UInt64) );
		parameterType = endianness.convertToNative( parameterType );
		
		// Read the parameter size.
		UInt64 parameterSize = 0;
		stream.readData( (UByte*)&parameterSize, sizeof(UInt64) );
		parameterSize = endianness.convertToNative( parameterSize );
		
		// Get the stored parameter state and set it for the filter.
		switch ( parameterType )
		{
			case FilterParameterType::BOOLEAN:
			{
				UInt64 value = 0;
				stream.readData( (UByte*)&value, sizeof(UInt64) );
				value = endianness.convertToNative( value );
				
				this->setParameter( i, value != 0 );
			}
			break;
			
			case FilterParameterType::INTEGER:
			case FilterParameterType::ENUMERATION:
			{
				Int64 value = 0;
				stream.readData( (UByte*)&value, sizeof(Int64) );
				value = endianness.convertToNative( value );
				
				this->setParameter( i, value );
			}
			break;
			
			case FilterParameterType::FLOAT:
			{
				Float value = 0;
				stream.readData( (UByte*)&value, sizeof(Float) );
				value = endianness.convertToNative( value );
				
				this->setParameter( i, value );
			}
			break;
			
			case FilterParameterType::DOUBLE:
			{
				Double value = 0;
				stream.readData( (UByte*)&value, sizeof(Double) );
				value = endianness.convertToNative( value );
				
				this->setParameter( i, value );
			}
			break;
			
			default:
			{
				// Seek past the parameter data if its type is unknown.
				stream.seek( (Int64)parameterSize );
			}
		}
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Preset Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size SoundFilter:: getPresetCount() const
{
	return 0;
}




Bool SoundFilter:: getPreset( Index presetIndex, FilterPreset& preset ) const
{
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Type Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




FilterCategory SoundFilter:: getCategory() const
{
	return FilterCategory::OTHER;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter In-Place Query Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundFilter:: allowsInPlaceProcessing() const
{
	return false;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################