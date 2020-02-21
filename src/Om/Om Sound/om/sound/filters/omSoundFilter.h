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

#ifndef INCLUDE_OM_SOUND_FILTER_H
#define INCLUDE_OM_SOUND_FILTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilterVersion.h"
#include "omSoundFilterParameterInfo.h"
#include "omSoundFilterParameter.h"
#include "omSoundFilterState.h"
#include "omSoundFilterPreset.h"
#include "omSoundFilterCategory.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a lightweight audio processing unit.
/**
  * A SoundFilter object takes a buffer of N input channels and performs some DSP computation
  * on those samples and places some number of samples in an output buffer of M channels.
  *
  * Typically, implementors of this class's interface should perform a single DSP computation
  * such as applying an EQ filter or delay filter, but they are also allowed to perform
  * sample rate conversion, pitch shit, and other time-related functions.
  *
  *	The number of input and output channels do not have to match. Each filter is responsible
  * for determining the format of its output(s) given the input buffer format(s). This includes
  * the channel count, number of samples, and sample rate.
  *
  * For filters that have no inputs that could inform the output format (such as a tone generator
  * or sound player), the filter should use the format of the output buffer(s) as a hint
  * for determining the output format. The filter is of course able to ignore this hint
  * and change the buffer format(s) to something else.
  *
  * The SoundFilter object is expected to tolerate input buffers of any sample
  * rate. If the sample rate or channel count of a stream changes mid-stream,
  * the sound filter is allowed to reset its internal processing (possibly with audible glitches).
  */
class SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sound filter object.
			virtual ~SoundFilter();
			
			
		//********************************************************************************
		//******	Filter Read Methods
			
			
			/// Fill the specified output buffer with the requested number of samples, based on internal filter state.
			/**
			  * Calling this method causes this filter to behave
			  * as a sound output (with no input). Samples are read from the filter and
			  * written to the output buffer. The filter is not provided any input buffers
			  * and must generate its own output sample data.
			  *
			  * The output buffer is enlarged to the specified number of samples if it is smaller
			  * than that number of samples. The method returns the number of valid samples
			  * written to the output buffer.
			  */
			SoundResult read( SoundBuffer& outputBuffer, Size numSamples );
			
			
			/// Fill the specified output frame with the requested number of samples, based on internal filter state.
			/**
			  * Calling this method causes this filter to behave
			  * as a sound output (with no input). Samples are read from the filter and
			  * written to the output frame. The filter is not provided any input buffers
			  * and must generate its own output sample data.
			  *
			  * The output frame is enlarged to the specified number of samples if it is smaller
			  * than that number of samples. The method returns the number of valid samples
			  * written to the output frame.
			  */
			SoundResult read( SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Filter Write Methods
			
			
			/// Process the specified input buffer samples and do something with them.
			/**
			  * The specified number of samples to process is clamped to be no larger than the size of the
			  * input buffer. Calling this method causes this filter to behave
			  * as a sound input (with no output). The default implementation doesn't do anything with
			  * the input samples but classes may inherit from this class to provide specific
			  * functionality (such as a signal analyzer or sound file encoder).
			  */
			SoundResult write( const SoundBuffer& inputBuffer, Size numSamples );
			
			
			/// Process the specified input frame samples and do something with them.
			/**
			  * The specified number of samples to process is clamped to be no larger than the size of the
			  * input frame. Calling this method causes this filter to behave
			  * as a sound input (with no output). The default implementation doesn't do anything with
			  * the input samples but classes may inherit from this class to provide specific
			  * functionality (such as a signal analyzer or sound file encoder).
			  */
			SoundResult write( const SoundFrame& inputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Filter Processing Methods
			
			
			/// Apply this filter to the specified input buffer data, placing the result in the output buffer.
			/**
			  * The specified number of samples to process is clamped to be no larger than the size of the
			  * input buffer.
			  *
			  * Call this method when using a sound filter that has 1 input and 1 output.
			  *
			  * This method returns an object indicating the result of the filter processing step.
			  * This result contains the number of valid samples that were placed in the output buffer.
			  */
			SoundResult process( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples );
			
			
			/// Apply this filter to the specified input buffer data, placing the result in the output frame.
			/**
			  * The specified number of samples to process is clamped to be no larger than the size of the
			  * input buffer.
			  *
			  * Call this method when using a sound filter that has 1 input and more than one output.
			  *
			  * This method returns an object indicating the result of the filter processing step.
			  * This result contains the number of valid samples that were placed in the output frame.
			  */
			SoundResult process( const SoundBuffer& inputBuffer, SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply this filter to the specified input frame data, placing the result in the output frame.
			/**
			  * The specified number of samples to process is clamped to be no larger than the size of the
			  * smallest buffer in the input frame.
			  *
			  * Call this method when using a sound filter that has multiple inputs or multiple outputs.
			  *
			  * This method returns an object indicating the result of the filter processing step.
			  * This result contains the number of valid samples that were placed in the output frame.
			  */
			SoundResult process( const SoundFrame& inputFrame, SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply this filter to the specified input frame data, placing the result in the output buffer.
			/**
			  * The specified number of samples to process is clamped to be no larger than the size of the
			  * input frame.
			  *
			  * Call this method when using a sound filter that has more than one input and only one output.
			  *
			  * This method returns an object indicating the result of the filter processing step.
			  * This result contains the number of valid samples that were placed in the output buffer.
			  */
			SoundResult process( const SoundFrame& inputBuffer, SoundBuffer& outputBuffer, Size numSamples );
			
			
		//********************************************************************************
		//******	Filter Reset Method
			
			
			/// Signal to the filter that the audio stream is restarting.
			/**
			  * This method allows the filter to reset all parameter interpolation
			  * and processing to its initial state to avoid coloration from previous
			  * audio or parameter values.
			  */
			void reset();
			
			
		//********************************************************************************
		//******	Frame Index Accessor Method
			
			
			/// Return the index of the next frame to be processed (or the current one if currently processing).
			OM_INLINE UInt64 getFrameIndex() const
			{
				return frameIndex;
			}
			
			
		//********************************************************************************
		//******	Input and Output Accessor Methods
			
			
			/// Return the current number of audio inputs that this filter has.
			OM_INLINE Size getInputCount() const
			{
				return (Size)numInputs;
			}
			
			
			/// Return a human-readable name of the filter audio input at the specified index.
			/**
			  * The default implementation for this method returns a string
			  * consisting of 'Input N' where N is the input index when the number
			  * of inputs is more than 1. If the number of inputs is 1, 'Main Input' is returned.
			  * If the index is invalid, the empty string is returned.
			  *
			  * Subclasses may override this method to return a more descriptive
			  * name (such as 'Sidechain').
			  */
			virtual UTF8String getInputName( Index inputIndex ) const;
			
			
			/// Return the current number of audio outputs that this filter has.
			OM_INLINE Size getOutputCount() const
			{
				return (Size)numOutputs;
			}
			
			
			/// Return a human-readable name of the filter audio output at the specified index.
			/**
			  * The default implementation for this method returns a string
			  * consisting of 'Output N' where N is the output index when the number
			  * of outputs is more than 1. If the number of outputs is 1, 'Main Output' is returned.
			  * If the index is invalid, the empty string is returned.
			  *
			  * Subclasses may override this method to return a more descriptive
			  * name.
			  */
			virtual UTF8String getOutputName( Index outputIndex ) const;
			
			
		//********************************************************************************
		//******	MIDI Input and Output Accessor Methods
			
			
			/// Return the current number of MIDI inputs that this filter has.
			OM_INLINE Size getMIDIInputCount() const
			{
				return numMIDIInputs;
			}
			
			
			/// Return a human-readable name of the filter MIDI input at the specified index.
			/**
			  * The default implementation for this method returns a string
			  * consisting of 'MIDI Input N' where N is the input index when the number
			  * of MIDI inputs is more than 1. If the number of inputs is 1, 'Main MIDI Input' is returned.
			  * If the index is invalid, the empty string is returned.
			  *
			  * Subclasses may override this method to return a more descriptive
			  * name.
			  */
			virtual UTF8String getMIDIInputName( Index inputIndex ) const;
			
			
			/// Return the current number of MIDI outputs that this filter has.
			OM_INLINE Size getMIDIOutputCount() const
			{
				return numMIDIOutputs;
			}
			
			
			/// Return a human-readable name of the filter MIDI output at the specified index.
			/**
			  * The default implementation for this method returns a string
			  * consisting of 'MIDI Output N' where N is the output index when the number
			  * of MIDI outputs is more than 1. If the number of outputs is 1, 'Main MIDI Output' is returned.
			  * If the index is invalid, the empty string is returned.
			  *
			  * Subclasses may override this method to return a more descriptive
			  * name.
			  */
			virtual UTF8String getMIDIOutputName( Index outputIndex ) const;
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this filter.
			/**
			  * The default implementation uses typeinfo to determine the
			  * actual subclass type of the filter and returns a string representing
			  * that type. This string usually is not formatted properly so the
			  * filter subclass should override this method to return a proper name.
			  */
			virtual UTF8String getName() const;
			
			
			/// Return a human-readable name for this filter's manufacturer.
			/**
			  * The default implementation returns the empty string.
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this sound filter.
			/**
			  * The default version returned is '0.0.0'. Subclasses should override
			  * this method to return a more meaningful version number.
			  */
			virtual FilterVersion getVersion() const;
			
			
		//********************************************************************************
		//******	Filter Latency Accessor Method
			
			
			/// Return a Time value indicating the latency of this sound filter in seconds.
			/**
			  * The default implementation returns a latency of 0. Filter subclasses should
			  * override this implementation if they have a non-zero latency.
			  */
			virtual Time getLatency() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this filter has.
			/**
			  * The default implementation for this method returns that the filter has 0
			  * parameters. SoundFilter subclasses should override this method to provide
			  * generic access to their parameters.
			  */
			virtual Size getParameterCount() const;
			
			
			/// Query the index of the parameter with the specified name.
			/**
			  * If a parameter with the name exists in the SoundFilter, TRUE is returned
			  * and the index of that parameter is placed in the output parameter index parameter.
			  * Otherwise, FALSE is returned and no parameter index is set.
			  *
			  * The default implementation calls getParameterInfo() for all each declared parameter
			  * and checks to see if its name is equal to the specified name. This is rather inefficient
			  * but is provided so that SoundFilter subclasses don't have to implement this method if they
			  * don't want to.
			  */
			virtual Bool getParameterIndex( const UTF8String& parameterName, Index& parameterIndex ) const;
			
			
			/// Get information about the filter parameter at the specified index.
			/**
			  * If a parameter with the specified index exists in this filter, the
			  * method should return TRUE and place information about the parameter
			  * in the output parameter information object. Otherwise, the method
			  * should return FALSE and return no parameter information.
			  *
			  * SoundFilter subclasses should override this method to provide information
			  * about their generic parameters.
			  */
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
			/// Get any special name associated with the specified value of an indexed parameter.
			/**
			  * If a parameter with the specified index exists in this filter and there is
			  * a special name associated with the given value, TRUE is returned and the
			  * output string is set to reflect the name of the special value.
			  * Otherwise, the method fails and returns FALSE, indicating that there is no
			  * special name for that parameter value.
			  */
			virtual Bool getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const;
			
			
		//********************************************************************************
		//******	Filter Parameter Value Read Methods

			
			/// Place the value of the parameter at the specified index in the output parameter.
			/**
			  * This method accesses the value for a parameter. If if the parameter index
			  * is out-of-bounds, FALSE is returned. Otherwise, TRUE is returned and the
			  * value is placed in the output parameter.
			  */
			Bool getParameter( Index parameterIndex, FilterParameter& value ) const;
			
			
			/// Place the value of the parameter at the specified index in the output parameter.
			/**
			  * This method accesses the value for a boolean parameter. If this type
			  * is not allowed for the given parameter index (or no suitable conversion is
			  * possible), or if the parameter index is out-of-bounds, FALSE is returned.
			  * Otherwise, TRUE is returned and the value is placed in the output parameter.
			  */
			Bool getParameter( Index parameterIndex, Bool& value ) const;

			
			/// Place the value of the parameter at the specified index in the output parameter.
			/**
			  * This method accesses the value for an integer or enumeration parameter. If this type
			  * is not allowed for the given parameter index (or no suitable conversion is
			  * possible), or if the parameter index is out-of-bounds, FALSE is returned.
			  * Otherwise, TRUE is returned and the value is placed in the output parameter.
			  */
			Bool getParameter( Index parameterIndex, Int64& value ) const;

			
			/// Place the value of the parameter at the specified index in the output parameter.
			/**
			  * This method accesses the value for a floating-point parameter. If this type
			  * is not allowed for the given parameter index (or no suitable conversion is
			  * possible), or if the parameter index is out-of-bounds, FALSE is returned.
			  * Otherwise, TRUE is returned and the value is placed in the output parameter.
			  */
			Bool getParameter( Index parameterIndex, Float32& value ) const;

			
			/// Place the value of the parameter at the specified index in the output parameter.
			/**
			  * This method accesses the value for a double floating-point parameter. If this type
			  * is not allowed for the given parameter index (or no suitable conversion is
			  * possible), or if the parameter index is out-of-bounds, FALSE is returned.
			  * Otherwise, TRUE is returned and the value is placed in the output parameter.
			  */
			Bool getParameter( Index parameterIndex, Float64& value ) const;
			
			
			/// Place the value of the parameter with the specified name in the output parameter.
			/**
			  * This method accesses the value for a template type parameter. If this type
			  * is not allowed for the given parameter name (or no suitable conversion is
			  * possible), or if the parameter name does not represent a valid parameter, FALSE is returned.
			  * Otherwise, TRUE is returned and the value is placed in the output parameter.
			  */
			template < typename ParameterType >
			OM_INLINE Bool getParameter( const UTF8String& name, ParameterType& value ) const
			{
				Index index;
				
				return this->getParameterIndex( name, index ) && this->getParameter( index, value );
			}
			
			
			/// Place the value of the parameter with the specified name in the output parameter.
			/**
			  * This method accesses the value for a template type parameter. If this type
			  * is not allowed for the given parameter name (or no suitable conversion is
			  * possible), or if the parameter name does not represent a valid parameter, FALSE is returned.
			  * Otherwise, TRUE is returned and the value is placed in the output parameter.
			  */
			template < typename ParameterType >
			OM_INLINE Bool getParameter( const char* name, ParameterType& value ) const
			{
				Index index;
				
				return this->getParameterIndex( UTF8String(name), index ) && this->getParameter( index, value );
			}
			
			
		//********************************************************************************
		//******	Filter Parameter Value Write Methods

			
			/// Attempt to set the parameter value at the specified index.
			/**
			  * This method accesses the value for a generic parameter. If if the parameter index
			  * is out-of-bounds or if the type of the given value can't be converted
			  * to the parameter's type, FALSE is returned. Otherwise, TRUE is returned and the
			  * parameter's value is set to the specified value.
			  */
			Bool setParameter( Index parameterIndex, const FilterParameter& value );

			
			/// Attempt to set the parameter value at the specified index.
			/**
			  * This method accesses the value for a boolean parameter. If if the parameter index
			  * is out-of-bounds or if the type of the given value can't be converted
			  * to the parameter's type, FALSE is returned. Otherwise, TRUE is returned and the
			  * parameter's value is set to the specified value.
			  */
			Bool setParameter( Index parameterIndex, Bool value );

			
			/// Attempt to set the parameter value at the specified index.
			/**
			  * This method accesses the value for an integer or enumeration parameter. If if the parameter index
			  * is out-of-bounds or if the type of the given value can't be converted
			  * to the parameter's type, FALSE is returned. Otherwise, TRUE is returned and the
			  * parameter's value is set to the specified value.
			  */
			Bool setParameter( Index parameterIndex, Int64 value );

			
			/// Attempt to set the parameter value at the specified index.
			/**
			  * This method accesses the value for a float parameter. If if the parameter index
			  * is out-of-bounds or if the type of the given value can't be converted
			  * to the parameter's type, FALSE is returned. Otherwise, TRUE is returned and the
			  * parameter's value is set to the specified value.
			  */
			Bool setParameter( Index parameterIndex, Float32 value );

			
			/// Attempt to set the parameter value at the specified index.
			/**
			  * This method accesses the value for a double parameter. If if the parameter index
			  * is out-of-bounds or if the type of the given value can't be converted
			  * to the parameter's type, FALSE is returned. Otherwise, TRUE is returned and the
			  * parameter's value is set to the specified value.
			  */
			Bool setParameter( Index parameterIndex, Float64 value );
			
			
			/// Attempt to set the parameter value with the specified name.
			/**
			  * This method accesses the value for an boolean parameter. If this type
			  * is not allowed for the given parameter name (or no suitable conversion is
			  * possible), or if the parameter name does not represent a valid parameter, FALSE is returned.
			  * Otherwise, TRUE is returned and the new parameter value is set.
			  */
			template < typename ParameterType >
			OM_INLINE Bool setParameter( const UTF8String& name, ParameterType value )
			{
				Index index;
				
				return this->getParameterIndex( name, index ) && this->setParameter( index, value );
			}
			
			
			/// Attempt to set the parameter value with the specified name.
			/**
			  * This method accesses the value for an boolean parameter. If this type
			  * is not allowed for the given parameter name (or no suitable conversion is
			  * possible), or if the parameter name does not represent a valid parameter, FALSE is returned.
			  * Otherwise, TRUE is returned and the new parameter value is set.
			  */
			template < typename ParameterType >
			OM_INLINE Bool setParameter( const char* name, ParameterType value )
			{
				Index index;
				
				return this->getParameterIndex( UTF8String(name), index ) && this->setParameter( index, value );
			}
			
			
		//********************************************************************************
		//******	Filter State Accessor Methods
			
			
			/// Write the current state of this sound filter to a data output stream.
			/**
			  * This method writes the complete current state of the sound filter to the specified
			  * output stream. The method returns whether or not the operation was successful. This
			  * method is useful for easily serializing the state of a filter. Use the writeState()
			  * method to save a filter state.
			  *
			  * The base implementation iterates over the filter's public parameters and stores them
			  * in a generic representation in the output stream.
			  *
			  * Override this method in a subclass to store private data or to store more efficiently.
			  * In general, if one overrides this method, one should also override readState() in
			  * order to make sure that filter data is stored in an expected manner.
			  */
			virtual Bool writeState( DataOutputStream& stream ) const;
			
			
			/// Read a previously saved filter state from the specified data input stream and replace the filter's state.
			/**
			  * This method reads data from the stream and uses it to update all
			  * of the filter's parameters with new values. This method
			  * is used to deserialize the stored state of a filter.
			  *
			  * The base implementation uses the filter's public parameters to update the filter's
			  * state from a generic representation.
			  *
			  * Override this method in a subclass to restore private data.
			  * In general, if one overrides this method, one should also override writeState() in
			  * order to make sure that filter data is stored in an expected manner.
			  */
			virtual Bool readState( DataInputStream& stream );
			
			
		//********************************************************************************
		//******	Filter Preset Accessor Methods
			
			
			/// Return the number of standard configuration presets that this sound filter has.
			/**
			  * A preset is a collection of information which encapsulates the entire state
			  * of a filter. Filters can have any number of standard presets which the user
			  * can choose from to save time when configuring filters. The preset with index
			  * 0 is always considered the default preset.
			  *
			  * The base implementation returns that there are 0 presets. Override this method
			  * along with the other preset accessor methods to provide a set of standard presets.
			  */
			virtual Size getPresetCount() const;
			
			
			/// Get the standard preset for this sound filter with the specified index.
			/**
			  * This method stores the standard preset with the specified index in the
			  * output preset parameter. If the method succeeds, TRUE is returned. Otherwise,
			  * if the method fails, FALSE is returned and the preset parameter is unmodified.
			  *
			  * The base implementation always returns FALSE because are no presets
			  * by default.
			  */
			virtual Bool getPreset( Index presetIndex, FilterPreset& preset ) const;
			
			
		//********************************************************************************
		//******	Filter Type Accessor Method
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * The base implementation returns FilterCategory::OTHER. Override the method
			  * to return more meaningful information. This is used to categorize filters
			  * in a host application, or to provide useful metadata to users.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Filter Synchronization Accessor Methods
			
			
			/// Return whether or not this sound filter performs thread synchronization.
			/**
			  * If the return value is TRUE, every time that a sound filter parameter
			  * is changed or a frame is rendered, a mutex is locked which prevents unsafe
			  * thread access to internal state. Otherwise, if the return value is FALSE,
			  * the synchronization is disabled. Disabling synchronization can improve
			  * performance in situations where the user is sure that there will be no unsafe
			  * thread access.
			  *
			  * Sound filters are synchronized by default.
			  */
			OM_INLINE Bool getIsSynchronized() const
			{
				return isSynchronized;
			}
			
			
			/// Set whether or not this sound filter performs thread synchronization.
			/**
			  * If the new value is TRUE, every time that a sound filter parameter
			  * is changed or a frame is rendered, a mutex is locked which prevents unsafe
			  * thread access to internal state. Otherwise, if the new value is FALSE,
			  * the synchronization is disabled. Disabling synchronization can improve
			  * performance in situations where the user is sure that there will be no unsafe
			  * thread access.
			  *
			  * Sound filters are synchronized by default.
			  */
			OM_INLINE void setIsSynchronized( Bool newIsSynchronized )
			{
				isSynchronized = newIsSynchronized;
			}
			
			
		//********************************************************************************
		//******	Filter In-Place Query Method
			
			
			/// Return whether or not this sound filter can process audio data in-place.
			/**
			  * If TRUE, this means that the filter can use the same SoundBuffer for input
			  * and output without any processing errors. This reduces memory requirements
			  * for the user of the filter and increases the filter's flexibility. If FALSE,
			  * the filter requires different input and output sound buffers.
			  *
			  * The default implementation returns FALSE, indicating that in-place processing
			  * is not supported.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Filter Input/Output Limit Accessor Methods
			
			
			/// Return the maximum number of audio inputs that a SoundFilter can support.
			OM_INLINE static Size getMaximumNumberOfInputs()
			{
				return Size(math::max<UInt16>());
			}
			
			
			/// Return the maximum number of audio outputs that a SoundFilter can support.
			OM_INLINE static Size getMaximumNumberOfOutputs()
			{
				return Size(math::max<UInt16>());
			}
			
			
			/// Return the maximum number of MIDI inputs that a SoundFilter can support.
			OM_INLINE static Size getMaximumNumberOfMIDIInputs()
			{
				return Size(math::max<UInt16>());
			}
			
			
			/// Return the maximum number of MIDI outputs that a SoundFilter can support.
			OM_INLINE static Size getMaximumNumberOfMIDIOutputs()
			{
				return Size(math::max<UInt16>());
			}
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a sound filter.
			static const om::resources::ResourceType RESOURCE_TYPE;
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Constructor
			
			
			/// Create a new sound filter with 1 audio input and output, and no MIDI inputs or outputs.
			SoundFilter();
			
			
			/// Create a new sound filter with the specified number of audio inputs and outputs.
			SoundFilter( Size numInputs, Size numOutputs );
			
			
			/// Create a new sound filter with the specified number of audio inputs and outputs.
			SoundFilter( Size numInputs, Size numOutputs, Size numMIDIInputs, Size numMIDIOutputs );
			
			
		//********************************************************************************
		//******	Filter Parameter Value Accessor Methods

			
			/// Place the value of the parameter at the specified index in the output parameter.
			/**
			  * This method accesses the value for a parameter. If if the parameter index
			  * is out-of-bounds, FALSE is returned. Otherwise, TRUE is returned and the
			  * value is placed in the output parameter.
			  */
			virtual Bool getParameterValue( Index parameterIndex, FilterParameter& value ) const;

			
			/// Attempt to set the parameter value at the specified index.
			/**
			  * This method accesses the value for a parameter. If if the parameter index
			  * is out-of-bounds or if the type of the given value can't be converted
			  * to the parameter's type, FALSE is returned. Otherwise, TRUE is returned and the
			  * parameter's value is set to the specified value.
			  */
			virtual Bool setParameterValue( Index parameterIndex, const FilterParameter& value );
			
			
		//********************************************************************************
		//******	Protected Stream Reset Method
			
			
			/// A method that is called whenever the filter's stream of audio is being reset.
			/**
			  * This method can be overridden by a subclass to perform any initialization
			  * or preprocessing necessary when restarting the filter's audio stream.
			  * This can include reseting all parameter interpolation and processing to
			  * its initial state to avoid coloration from previous audio or parameter values.
			  *
			  * The base implementation does nothing. This method is automatically synchronized
			  * with the filter processing method, allowing it to be called from another thread
			  * without consequences. As such, calling lockMutex() within this method will cause a deadlock.
			  */
			virtual void resetStream();
			
			
		//********************************************************************************
		//******	Protected Filter Processing Method
			
			
			/// Process the given input frame and write the resulting audio to the given output frame.
			/**
			  * If the number of input frame buffers is 0, it means that the filter should
			  * behave as an output only. Likewise, if the number of output frame buffers is 0,
			  * the filter should only read and process data from the input frame and not write
			  * data to the output frame. The implementator of this method should detect these
			  * cases and handle them without error.
			  *
			  * This method should return the number of samples successfully written to the output frame.
			  * A value less than the number of input samples indicates an error occurred.
			  *
			  * The default implementation doesn't write anything to the output buffer and just
			  * returns 0. Override it to provide more specific functionality.
			  *
			  * This method is automatically synchronized using lockMutex(), which keeps parameter
			  * value changes from happening while a frame is being rendered. In order for this
			  * to work properly, the filter subclass should call lockMutex() and unlockMutex() around
			  * every method that modifies state used when processing frames. Calling lockMutex() within
			  * this method will cause a deadlock.
			  */
			virtual SoundResult processFrame( const SoundFrame& inputFrame, SoundFrame& outputFrame,
													Size numSamples ) = 0;
			
			
		//********************************************************************************
		//******	Protected Input and Output Number Accessor Methods
			
			
			/// Set the number of inputs that this filter should have.
			/**
			  * Most filters will have only 1 input and 1 output, though some
			  * filters may have have no input or no output. Some filters (like a sidechain
			  * compressor or crossover) may have multiple inputs or outputs.
			  *
			  * This method is not thread synchronized, so subclasses must
			  * provide their own means of synchronization when calling this method.
			  *
			  * The requested number of inputs is clamped to be no larger than
			  * the maximum number of inputs, returned by SoundFilter::getMaximumNumberOfInputs().
			  */
			OM_INLINE void setInputCount( Size newNumInputs )
			{
				numInputs = (UInt16)math::min( newNumInputs, SoundFilter::getMaximumNumberOfInputs() );
			}
			
			
			/// Set the number of outputs that this filter should have.
			/**
			  * Most filters will have only 1 input and 1 output, though some
			  * filters may have have no input or no output. Some filters (like a sidechain
			  * compressor or crossover) may have multiple inputs or outputs.
			  *
			  * This method is not thread synchronized, so subclasses must
			  * provide their own means of synchronization when calling this method.
			  *
			  * The requested number of outputs is clamped to be no larger than
			  * the maximum number of outputs, returned by SoundFilter::getMaximumNumberOfOutputs().
			  */
			OM_INLINE void setOutputCount( Size newNumOutputs )
			{
				numOutputs = (UInt16)math::min( newNumOutputs, SoundFilter::getMaximumNumberOfOutputs() );
			}
			
			
		//********************************************************************************
		//******	Protected MIDI Input and Output Number Accessor Methods
			
			
			/// Set the number of MIDI inputs that this filter should have.
			/**
			  * This method can be used by subclasses to change the number of recognized
			  * MIDI inputs for the filter. The default number of inputs is 0.
			  *
			  * This method is not thread synchronized, so subclasses must
			  * provide their own means of synchronization when calling this method.
			  *
			  * The requested number of inputs is clamped to be no larger than
			  * the maximum number of inputs, returned by SoundFilter::getMaximumNumberOfMIDIInputs().
			  */
			OM_INLINE void setMIDIInputCount( Size newNumMIDIInputs )
			{
				numMIDIInputs = (UInt16)math::min( newNumMIDIInputs, SoundFilter::getMaximumNumberOfMIDIInputs() );;
			}
			
			
			/// Set the number of MIDI outputs that this filter should have.
			/**
			  * This method can be used by subclasses to change the number of recognized
			  * MIDI outputs for the filter. The default number of outputs is 0.
			  *
			  * This method is not thread synchronized, so subclasses must
			  * provide their own means of synchronization when calling this method.
			  *
			  * The requested number of outputs is clamped to be no larger than
			  * the maximum number of outputs, returned by SoundFilter::getMaximumNumberOfMIDIOutputs().
			  */
			OM_INLINE void setMIDIOutputCount( Size newNumMIDIOutputs )
			{
				numMIDIOutputs = (UInt16)math::min( newNumMIDIOutputs, SoundFilter::getMaximumNumberOfMIDIOutputs() );
			}
			
			
		//********************************************************************************
		//******	Protected Sound Stream State Accessor Methods
			
			
			/// Return whether or not the filter's next frame to be processed is also its first frame.
			/**
			  * This method can be queried by subclasses to determine if the filter should reset
			  * any parameter interpolation or buffers.
			  */
			OM_INLINE Bool isFirstFrame() const
			{
				return frameIndex == 0;
			}
			
			
		//********************************************************************************
		//******	Protected Parameter Mutex Accessor Methods
			
			
			/// Acquire a mutex which handles subclass rendering parameter synchronization.
			/**
			  * This method should be called before a rendering parameter is modified
			  * in a SoundFilter subclass. This operation may potentially block the
			  * calling thread if the mutex is already acquired by another thread until
			  * the mutex is released with a corresponding call to unlockMutex().
			  *
			  * Every call to this method should be paired with a call to unlockMutex().
			  * Failure to observe this requirement will result in application deadlock.
			  *
			  * If the filter is not synchronized, via setIsSynchronized(), the method has
			  * no effect. This improves performance in situations where the user knows
			  * that there will not be any unsafe thread access to the filter.
			  */
			OM_INLINE void lockMutex() const
			{
				if ( isSynchronized )
					parameterMutex.lock();
			}
			
			
			/// Release a mutex which handles subclass rendering parameter synchronization.
			/**
			  * This method should be called after a rendering parameter is modified
			  * in a SoundFilter subclass. It releases the parameter change mutex and
			  * may potentially awaken threads that are waiting on the mutex.
			  *
			  * If the filter is not synchronized, via setIsSynchronized(), the method has
			  * no effect. This improves performance in situations where the user knows
			  * that there will not be any unsafe thread access to the filter.
			  */
			OM_INLINE void unlockMutex() const
			{
				if ( isSynchronized )
					parameterMutex.unlock();
			}
			
			
	private:
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The current number of audio inputs that this sound filter has.
			UInt16 numInputs;
			
			
			/// The current number of audio outputs that this sound filter has.
			UInt16 numOutputs;
			
			
			/// The current number of MIDI inputs that this sound filter has.
			UInt16 numMIDIInputs;
			
			
			/// The current number of MIDI outputs that this sound filter has.
			UInt16 numMIDIOutputs;
			
			
			/// The index of the next frame to be processed by this filter, or the current frame if the filter is currently processing.
			UInt64 frameIndex;
			
			
			/// A mutex object that can provide thread synchronization for SoundFilter subclasses.
			/**
			  * This mutex is automatically locked whenever a filter is processing and released
			  * when it done processing. A SoundFilter subclass can acquire this mutex indirectly
			  * through the lockMutex()
			  */
			mutable threads::Mutex parameterMutex;
			
			
			/// A boolean value indicating whether or not this sound filter performs thread synchronization.
			Bool isSynchronized;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::sound::filters::SoundFilter, om::sound::filters::SoundFilter::RESOURCE_TYPE );


#endif // INCLUDE_OM_SOUND_FILTER_H
