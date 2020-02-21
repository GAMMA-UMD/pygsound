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

#ifndef INCLUDE_OM_SOUND_BITCRUSHER_H
#define INCLUDE_OM_SOUND_BITCRUSHER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"
#include "omSoundCutoffFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that uses quantization-based methods to produce distortion.
/**
  * The class uses a conversion to/from a lower bit-depth audio stream
  * in order to produce unique kinds of distortion. It also provides a way
  * to process that audio through a conversion to a lower sample rate, effectively
  * producing other kinds of distortion.
  */
class Bitcrusher : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Distortion Type Enum Declaration
			
			
			/// Define the different kinds of distortion effects that this filter can produce.
			enum ClipMode
			{
				/// A kind of clipping where the waveform is chopped off when it goes above the threshold.
				HARD = 0,
				
				/// A kind of clipping where the waveform is inverted when it goes above the threshold.
				INVERT = 1,
				
				/// A kind of clipping where the waveform wraps around to 0 when it goes above the threshold.
				WRAP = 2
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new distortion filter with the default input and output gains of 1 and hardness of 0.
			Bitcrusher();
			
			
			/// Create an exact copy of another bitcrusher.
			Bitcrusher( const Bitcrusher& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this bitcrusher and clean up any resources.
			~Bitcrusher();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another bitcrusher to this one.
			Bitcrusher& operator = ( const Bitcrusher& other );
			
			
		//********************************************************************************
		//******	Clipping Mode Accessor Methods
			
			
			/// Return the type of clipping that this bitcrusher is using.
			/**
			  * This value determines what happens when the waveform exceeds the clipping threshold.
			  */
			OM_INLINE ClipMode getClipMode() const
			{
				return clipMode;
			}
			
			
			/// Return the type of clipping that this bitcrusher is using.
			/**
			  * This value determines what happens when the waveform exceeds the clipping threshold.
			  */
			OM_INLINE void setClipMode( ClipMode newClipMode )
			{
				lockMutex();
				clipMode = newClipMode;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Input Gain Accessor Methods
			
			
			/// Return the current linear input gain factor of this bitcrusher.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * clipping function. A higher value will result in more noticeable clipping.
			  */
			OM_INLINE Gain getInputGain() const
			{
				return targetInputGain;
			}
			
			
			/// Return the current input gain factor in decibels of this bitcrusher.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * clipping function. A higher value will result in more noticeable clipping.
			  */
			OM_INLINE Gain getInputGainDB() const
			{
				return math::linearToDB( targetInputGain );
			}
			
			
			/// Set the target linear input gain for this bitcrusher.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * clipping function. A higher value will result in more noticeable clipping.
			  */
			OM_INLINE void setInputGain( Gain newInputGain )
			{
				lockMutex();
				targetInputGain = newInputGain;
				unlockMutex();
			}
			
			
			/// Set the target input gain in decibels for this bitcrusher.
			/**
			  * This is the gain applied to the input signal before being sent to the
			  * clipping function. A higher value will result in more noticeable clipping.
			  */
			OM_INLINE void setInputGainDB( Gain newDBInputGain )
			{
				lockMutex();
				targetInputGain = math::dbToLinear( newDBInputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Output Gain Accessor Methods
			
			
			/// Return the current linear output gain factor of this bitcrusher.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * clipping function. This value is used to modify the final level
			  * of the clipped signal.
			  */
			OM_INLINE Gain getOutputGain() const
			{
				return targetOutputGain;
			}
			
			
			/// Return the current output gain factor in decibels of this bitcrusher.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * clipping function. This value is used to modify the final level
			  * of the clipped signal.
			  */
			OM_INLINE Gain getOutputGainDB() const
			{
				return math::linearToDB( targetOutputGain );
			}
			
			
			/// Set the target linear output gain for this bitcrusher.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * clipping function. This value is used to modify the final level
			  * of the clipped signal.
			  */
			OM_INLINE void setOutputGain( Gain newOutputGain )
			{
				lockMutex();
				targetOutputGain = newOutputGain;
				unlockMutex();
			}
			
			
			/// Set the target output gain in decibels for this bitcrusher.
			/**
			  * This is the gain applied to the signal after being sent to the
			  * clipping function. This value is used to modify the final level
			  * of the clipped signal.
			  */
			OM_INLINE void setOutputGainDB( Gain newDBOutputGain )
			{
				lockMutex();
				targetOutputGain = math::dbToLinear( newDBOutputGain );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Mix Accessor Methods
			
			
			/// Return the ratio of input signal to distorted signal sent to the output of the bitcrusher.
			/**
			  * Valid mix values are in the range [0,1].
			  * A mix value of 1 indicates that only the output of the distortion should be
			  * heard at the output, while a value of 0 indicates that only the input of the
			  * distortion should be heard at the output. A value of 0.5 indicates that both
			  * should be mixed together equally at -6dB.
			  */
			OM_INLINE Gain getMix() const
			{
				return targetMix;
			}
			
			
			/// Set the ratio of input signal to distorted signal sent to the output of the bitcrusher.
			/**
			  * Valid mix values are in the range [0,1].
			  * A mix value of 1 indicates that only the output of the distortion should be
			  * heard at the output, while a value of 0 indicates that only the input of the
			  * distortion should be heard at the output. A value of 0.5 indicates that both
			  * should be mixed together equally at -6dB.
			  *
			  * The new mix value is clamped to the valid range of [0,1].
			  */
			OM_INLINE void setMix( Gain newMix )
			{
				lockMutex();
				targetMix = math::clamp( newMix, Gain(0), Gain(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Threshold Accessor Methods
			
			
			/// Return the linear full-scale value at which clipping first occurs.
			OM_INLINE Gain getThreshold() const
			{
				return targetThreshold;
			}
			
			
			/// Return the full-scale value in decibels at which clipping first occurs.
			OM_INLINE Gain getThresholdDB() const
			{
				return math::linearToDB( targetThreshold );
			}
			
			
			/// Set the linear full-scale value at which clipping first occurs.
			/**
			  * The value is clamped to the valid range of [0,infinity] before being stored.
			  */
			OM_INLINE void setThreshold( Gain newThreshold )
			{
				lockMutex();
				targetThreshold = math::max( newThreshold, Gain(0) );
				unlockMutex();
			}
			
			
			/// Set the full-scale value in decibels at which clipping first occurs.
			OM_INLINE void setThresholdDB( Gain newThresholdDB )
			{
				lockMutex();
				targetThreshold = math::dbToLinear( newThresholdDB );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Bit Reduction Accessor Methods
			
			
			/// Return whether or not this bitcrusher's bit reduction stage is enabled.
			OM_INLINE Bool getBitReductionIsEnabled() const
			{
				return bitReduceEnabled;
			}
			
			
			/// Set whether or not this bitcrusher's bit reduction stage is enabled.
			OM_INLINE void setBitReductionIsEnabled( Bool newBitReduceEnabled )
			{
				lockMutex();
				bitReduceEnabled = newBitReduceEnabled;
				unlockMutex();
			}
			
			
			/// Return the amount of dithering that should be applied before bit reduction.
			/**
			  * Valid dithering amounts are in the range [1,24].
			  *
			  * This value indicates the number of bits of precision used when reducing
			  * the bit depth of the input signal.
			  */
			OM_INLINE UInt getBitResolution() const
			{
				return bitResolution;
			}
			
			
			/// Set the bit resolution of the bit reduction stage.
			/**
			  * Valid bit resolutions are in the range [1,24].
			  *
			  * This value indicates the number of bits of precision used when reducing
			  * the bit depth of the input signal.
			  *
			  * The new bit resolution is clamped to the valid range of [1,24].
			  */
			OM_INLINE void setBitResolution( UInt newResolution )
			{
				lockMutex();
				bitResolution = math::clamp( newResolution, UInt(1), UInt(24) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Dithering Accessor Methods
			
			
			/// Return whether or not this bitcrusher's dithering stage is enabled.
			OM_INLINE Bool getDitherIsEnabled() const
			{
				return ditherEnabled;
			}
			
			
			/// Set whether or not this bitcrusher's dithering stage is enabled.
			OM_INLINE void setDitherIsEnabled( Bool newDitherEnabled )
			{
				lockMutex();
				ditherEnabled = newDitherEnabled;
				unlockMutex();
			}
			
			
			/// Return the amount of dithering that should be applied before bit reduction.
			/**
			  * Valid dithering amounts are in the range [0,1].
			  *
			  * A dither amount of 1 indicates that 100% of the dithering for the current
			  * bit resolution should be applied. A value of 0 indicates that no dithering
			  * should be applied.
			  */
			OM_INLINE Float getDitherAmount() const
			{
				return targetDither;
			}
			
			
			/// Set the amount of dithering that should be applied before bit reduction.
			/**
			  * Valid dithering amounts are in the range [0,1].
			  *
			  * A dither amount of 1 indicates that 100% of the dithering for the current
			  * bit resolution should be applied. A value of 0 indicates that no dithering
			  * should be applied.
			  *
			  * The new dithering amount is clamped to the valid range of [0,1].
			  */
			OM_INLINE void setDitherAmount( Float newDither )
			{
				lockMutex();
				targetDither = math::clamp( newDither, Float(0), Float(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Downsampling Accessor Methods
			
			
			/// Return the amount of dithering that should be applied before bit reduction.
			/**
			  * Valid dithering amounts are in the range [1,24].
			  *
			  * This value indicates the effective sample rate divisor used. A value of 1
			  * indicates the current sampling rate is used. A value of 2 indicates the sampling
			  * rate is halved, etc.
			  */
			OM_INLINE UInt getDownsampling() const
			{
				return downsampling;
			}
			
			
			/// Set the amount of downsampling performed.
			/**
			  * Valid downsampling amounts are in the range [1,24].
			  *
			  * This value indicates the effective sample rate divisor used. A value of 1
			  * indicates the current sampling rate is used. A value of 2 indicates the sampling
			  * rate is halved, etc.
			  *
			  * The new downsampling amount is clamped to the valid range of [1,24].
			  */
			OM_INLINE void setDownsampling( UInt newDownsampling )
			{
				lockMutex();
				downsampling = math::clamp( newDownsampling, UInt(1), UInt(24) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Low Pass Filter Attribute Accessor Methods
			
			
			/// Return whether or not this bitcrusher's low pass filter is enabled.
			OM_INLINE Bool getLowPassIsEnabled() const
			{
				return lowPassEnabled;
			}
			
			
			/// Set whether or not this bitcrusher's low pass filter is enabled.
			OM_INLINE void setLowPassIsEnabled( Bool newLowPassIsEnabled )
			{
				lockMutex();
				lowPassEnabled = newLowPassIsEnabled;
				unlockMutex();
			}
			
			
			/// Return the low pass filter frequency of this bitcrusher.
			OM_INLINE Float getLowPassFrequency() const
			{
				return lowPassFrequency;
			}
			
			
			/// Set the low pass filter frequency of this bitcrusher.
			/**
			  * The new low pass frequency is clamped to the range [0,infinity].
			  */
			OM_INLINE void setLowPassFrequency( Float newLowPassFrequency )
			{
				lockMutex();
				lowPassFrequency = math::max( newLowPassFrequency, Float(0) );
				unlockMutex();
			}
			
			
			/// Return the low pass filter order of this distortion filter.
			OM_INLINE Size getLowPassOrder() const
			{
				return lowPassOrder;
			}
			
			
			/// Set the low pass filter order of this distortion filter.
			/**
			  * The new low pass order is clamped to the range [1,100].
			  */
			OM_INLINE void setLowPassOrder( Size newLowPassOrder )
			{
				lockMutex();
				lowPassOrder = math::clamp( newLowPassOrder, Size(1), Size(100) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this bitcrusher.
			/**
			  * The method returns the string "Bitcrusher".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this bitcrusher.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this bitcrusher.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::DISTORTION.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this bitcrusher has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the bitcrusher parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
			/// Get any special name associated with the specified value of an indexed parameter.
			virtual Bool getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this bitcrusher.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this bitcrusher.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this distortion filter.
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
		//******	Private Filter Processing Methods
			
			
			/// Apply a distortion function to the samples in the input frame and write the output to the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Apply the specified clipping function to the input buffer, placing the result in the output buffer.
			template < Float (*clippingFunction)( Float /*input*/, Float /*threshold*/ ) >
			void processClipping( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples );
			
			
			/// Apply the specified clipping function to the input buffer, placing the result in the output buffer.
			template < Float (*clippingFunction)( Float /*input*/, Float /*threshold*/ ) >
			void processClipping( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
									Gain inputGainChangePerSample, Float thresholdChangePerSample );
			
			
			/// Reduce the bit resolution of the audio in the specified buffer, optionally applying dithering and downsampling.
			template < Bool reductionEnabled, Bool ditherEnabled, Bool downsampleEnabled >
			void processBitReduction( SoundBuffer& ioBuffer, Size numSamples, Float ditherChangePerSample );
			
			
		//********************************************************************************
		//******	Private Clipping Functions
			
			
			/// Apply standard hard clipping to the input signal.
			OM_FORCE_INLINE static Float clipHard( Float input, Float threshold )
			{
				if ( input > threshold )
					return threshold;
				else if ( input < -threshold )
					return -threshold;
				else
					return input;
			}
			
			
			/// Apply a clipping where the waveform is inverted when it goes above the threshold.
			OM_FORCE_INLINE static Float clipInvert( Float input, Float threshold )
			{
				Float sign = math::sign(input);
				Float absIn = sign*input;
				Float n = math::floor(absIn / threshold);
				Float remainder = absIn - n*threshold;
				Float output = absIn;
				
				if ( UInt(n) & 0x1 )
				{
					if ( absIn > threshold )
						output = threshold - remainder;
				}
				else
				{
					if ( absIn > threshold )
						output = remainder;
				}
				
				return sign*output;
			}
			
			
			/// Apply a clipping where the waveform is wraps around to 0 when it goes above the threshold.
			OM_FORCE_INLINE static Float clipWrap( Float input, Float threshold )
			{
				if ( input > threshold )
					return threshold - math::mod( input, threshold );
				else if ( input < -threshold )
					return -threshold - math::mod( input, threshold );
				else
					return input;
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The current linear input gain factor applied to all input audio before being clipped.
			Gain inputGain;
			
			
			/// The target linear input gain factor, used to smooth changes in the input gain.
			Gain targetInputGain;
			
			
			/// The current linear output gain factor applied to all input audio after being clipped.
			Gain outputGain;
			
			
			/// The target linear output gain factor, used to smooth changes in the output gain.
			Gain targetOutputGain;
			
			
			/// The current ratio of distorted to unaffected signal that is sent to the output.
			Float mix;
			
			
			/// The target mix, used to smooth changes in the mix parameter.
			Float targetMix;
			
			
			/// The type of clipping that this bitcrusher uses.
			ClipMode clipMode;
			
			
			/// The current threshold which indicates the full-scale threshold at which clipping first occurs.
			Float threshold;
			
			
			/// The target threshold, used to smooth changes in the threshold parameter.
			Float targetThreshold;
			
			
			/// The number of bits that the audio signal should be reduced to, between 1 and 24.
			UInt bitResolution;
			
			
			/// A value between 0 and 1 indicating the amount of dithering to apply to the audio signal.
			/**
			  * This dither is applied based on the target bit resolution, so a value of 1 will always dither
			  * the most necessary for a given resolution.
			  */
			Float dither;
			
			
			/// The target dither amount, used to smooth changes in the dither parameter.
			Float targetDither;
			
			
			/// The sample rate downsampling amount, an integer indicating the sample rate divisor.
			UInt downsampling;
			
			
			/// The number of remaining instances of the last sample that should be repeated during the next processing frame.
			UInt downsampleRemainder;
			
			
			/// The last samples for each channel that should be repeated as part of downsampling.
			Array<Sample32f> lastSamples;
			
			
			/// A low-pass filter used to smooth the output of the bitcrusher.
			CutoffFilter* lowPass;
			
			
			/// The frequency at which the low pass filter for the bitcrusher is at -3dB.
			Float lowPassFrequency;
			
			
			/// The order of the bitcrusher's low pass filter that determines its slope.
			Size lowPassOrder;
			
			
			/// A boolean value indicating whether or not this bitcrusher's low-pass filter is enabled.
			Bool lowPassEnabled;
			
			
			/// A boolean value indicating whether or not bit reduction should be performed.
			Bool bitReduceEnabled;
			
			
			/// A boolean value indicating whether or not a dithering step should be performed.
			Bool ditherEnabled;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_BITCRUSHER_H
