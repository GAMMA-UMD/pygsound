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

#ifndef INCLUDE_OM_SOUND_SAMPLE_RATE_CONVERTER_H
#define INCLUDE_OM_SOUND_SAMPLE_RATE_CONVERTER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class CutoffFilter;




//********************************************************************************
/// A class that converts samples from an input stream to a set output sample rate.
/**
  * This class should be used wherever a mismatched sample rate could occur and needs
  * to be corrected for. For instance: sound file I/O, sound device I/O, mixing.
  *
  * Several different methods of sample rate conversion are part of the class
  * which provide a range of quality-speed tradeoffs.
  */
class Resampler : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Sample Rate Conversion Type Enum Declaration
			
			
			enum Type
			{
				/// The best quality sample rate conversion available.
				/**
				  * If this conversion type enum value is supplied, the converter chooses
				  * the best available sample rate converter and uses that algorithm.
				  */
				BEST = 0,
				
				/// The fastest sample rate conversion available.
				/**
				  * If this conversion type enum value is supplied, the converter chooses
				  * the fastest available sample rate converter and uses that algorithm.
				  */
				FASTEST = 1,
				
				/// The sample rate converter does a simple interpolation of the input samples.
				/**
				  * This is the fastest way to do sample rate conversion but it also has the
				  * worst quality. There may be audible artifacts in the output audio if this
				  * conversion type is used.
				  */
				INTERPOLATE = 2,
				
				/// The sample rate converter does an interpolation of the input samples with antialiasing.
				/**
				  * This is the second fastest way to do sample rate conversion and has better
				  * quality for downsampling than INTERPOLATE because it applies a steep low pass filter
				  * at the Nyquist frequency of the output sample rate. This filter helps avoid
				  * aliasing artifacts when downsampling and corrects for added noise when upsampling.
				  */
				INTERPOLATE_FILTERED = 3
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sample rate converter with the best conversion type and 44.1 kHz output sample rate.
			Resampler();
			
			
			/// Create a sample rate converter with the specified conversion type and 44.1 kHz output sample rate.
			Resampler( Type newConversionType );
			
			
			/// Create a sample rate converter with the specified conversion type and output sample rate.
			Resampler( Type newConversionType, SampleRate newOutputSampleRate );
			
			
			/// Create an exact copy of the specified sample rate converter.
			Resampler( const Resampler& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this sample rate converter and release all associated resources.
			~Resampler();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of the specified sample rate converter to this one.
			Resampler& operator = ( const Resampler& other );
			
			
		//********************************************************************************
		//******	Destination Sample Rate Accessor Methods
			
			
			/// Return the sampling rate to which all input audio is being converted.
			OM_INLINE SampleRate getOutputSampleRate() const
			{
				return outputSampleRate;
			}
			
			
			/// Set the sampling rate to which all input audio should be converted.
			/**
			  * If the sample rate of 0 is supplied, no sample rate conversion will
			  * be done in the filter's processing method. Otherwise, the input audio
			  * will be resampled to the desired sampling rate.
			  */
			OM_INLINE void setOutputSampleRate( SampleRate newOutputSampleRate )
			{
				lockMutex();
				outputSampleRate = math::max( newOutputSampleRate, SampleRate(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Destination Sample Rate Accessor Methods
			
			
			/// Return the type of sample rate conversion that is being performed.
			/**
			  * Some types of sample rate conversion are faster or have better quality than
			  * others. This value allows the user to specify which conversion algorithm to
			  * use based on the application.
			  */
			OM_INLINE Type getType() const
			{
				return conversionType;
			}
			
			
			/// Set the type of sample rate conversion that is being performed.
			/**
			  * Some types of sample rate conversion are faster or have better quality than
			  * others. This value allows the user to specify which conversion algorithm to
			  * use based on the application.
			  */
			OM_INLINE void setType( Type newSampleRateConversionType )
			{
				lockMutex();
				conversionType = newSampleRateConversionType;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this sample rate converter.
			/**
			  * The method returns the string "Sample Rate Converter".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this sample rate converter.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this sample rate converter.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::UTILITY.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this sample rate converter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the sample rate converter parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
			/// Get any special name associated with the specified value of an indexed parameter.
			virtual Bool getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this sample rate converter.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this sample rate converter.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this sample rate converter.
			static const FilterVersion VERSION;
			
			
	private:
		
		//********************************************************************************
		//******	Filter Parameter Value Accessor Methods
			
			
			/// Place the value of the parameter at the specified index in the output parameter.
			virtual Bool getParameterValue( Index parameterIndex, FilterParameter& value ) const;
			
			
			/// Attempt to set the parameter value at the specified index.
			virtual Bool setParameterValue( Index parameterIndex, const FilterParameter& value );
			
			
		//********************************************************************************
		//******	Private Stream Reset Method
			
			
			/// A method that is called whenever the filter's stream of audio is being reset.
			/**
			  * This method allows the filter to reset all parameter interpolation
			  * and processing to its initial state to avoid coloration from previous
			  * audio or parameter values.
			  */
			virtual void resetStream();
			
			
		//********************************************************************************
		//******	Private Filter Processing Method
			
			
			/// Convert the specified number of samples from the input frame to the desired sample rate.
			/**
			  * This method changes the sample rate of the output frame (and may enlarge it if
			  * necessary to hold extra sample information) and returns the total number of
			  * samples written to the output frame.
			  */
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Sample Rate Conversion Methods
			
			
			/// Do a simple linear sample rate conversion on the input and place it in the output buffer.
			Size interpolateBuffers( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numInputSamples );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum representing the type of sample rate conversion that should be done.
			/**
			  * Some types of sample rate conversion are faster or have better quality than
			  * others. This value allows the user to specify which conversion algorithm to
			  * use based on the application.
			  */
			Type conversionType;
			
			
			/// The sampling rate to which all input audio will converted.
			SampleRate outputSampleRate;
			
			
			/// When using an INTERPOLATE sample rate converter, this is the offset between adjacent samples.
			/**
			  * This value is stored so that process buffers from the same sound source will produce
			  * glitch-free audio at the buffer boundaries.
			  */
			Float interpolationSampleOffset;
			
			
			/// An array holding the last input sample from the last buffer process cycle for each channel.
			Array<Sample32f> lastInputSample;
			
			
			/// A cutoff filter, optionally NULL, that is used to antialias the converted audio.
			CutoffFilter* lowPass;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SAMPLE_RATE_CONVERTER_H
