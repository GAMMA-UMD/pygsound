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

#ifndef INCLUDE_OM_SOUND_TREMOLO_H
#define INCLUDE_OM_SOUND_TREMOLO_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that periodically modulates the amplitude of an input signal.
/**
  * A Tremolo filter takes the input sound and modulates the amplitude of that
  * sound with a repeating wave function. This class supports several tremolo wave
  * types: sinusoidal, square, saw, and triangle.
  */
class Tremolo : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Tone Type Enum
			
			
			/// An enum type which describes the various types of wave shapes that a Tremolo can use.
			enum WaveType
			{
				/// A pure sinusoidal oscillation of the input sound's amplitude.
				SINE = 0,
				
				/// A softened square-wave oscillation of the input sound's amplitude.
				SQUARE = 1,
				
				/// A softened saw-wave oscillation of the input sound's amplitude.
				SAW = 2,
				
				/// A triangle-wave oscillation of the input sound's amplitude.
				TRIANGLE = 3,
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sine-based tremolo filter with a depth of 6dB and frequency of 1Hz.
			Tremolo();
			
			
			/// Create a tremolo with the specified modulation wave type, output gain, and frequency.
			Tremolo( WaveType newType, Float newFrequency, Gain newDepth );
			
			
		//********************************************************************************
		//******	Wave Type Accessor Methods
			
			
			/// Return the type of modulation wave that this tremolo is using.
			/**
			  * The wave type can be one of several types of audio test tones
			  * (sine waves, square waves, saw waves, triangle waves).
			  */
			OM_INLINE WaveType getType() const
			{
				return type;
			}
			
			
			/// Set the type of modulation wave that this tremolo is using.
			/**
			  * The wave type can be one of several types of audio test tones
			  * (sine wave, square wave, saw wave, triangle wave). The tremolo
			  * filter switches wave types at the beginning of the next wave period
			  * so that it avoids discontinuities between the waves.
			  */
			OM_INLINE void setType( WaveType newType )
			{
				lockMutex();
				type = newType;
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Tremolo Frequency Accessor Methods
			
			
			/// Return the frequency of this tremolo's modulation wave in hertz.
			OM_INLINE Float getFrequency() const
			{
				return targetFrequency;
			}
			
			
			/// Set the frequency of this tremolo's modulation wave in hertz.
			OM_INLINE void setFrequency( Float newFrequency )
			{
				lockMutex();
				targetFrequency = math::max( newFrequency, Float(0) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Tremolo Depth Accessor Methods
			
			
			/// Return the maximum attenuation of the tremolo modulation in decibels.
			/**
			  * This is the amount that the input signal is attenuated by whenever the modulation
			  * wave is at its lowest point. At the wave's peaks, the gain reduction is 0dB.
			  */
			OM_INLINE Float getDepth() const
			{
				return -math::linearToDB( targetDepth );
			}
			
			
			/// Set the maximum attenuation of the tremolo modulation in decibels.
			/**
			  * This is the amount that the input signal is attenuated by whenever the modulation
			  * wave is at its lowest point. At the wave's peaks, the gain reduction is 0dB.
			  *
			  * The new depth value is clamped to the range of [0,+infinity].
			  */
			OM_INLINE void setDepth( Float newDepth )
			{
				lockMutex();
				targetDepth = math::dbToLinear( -math::abs(newDepth) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Tremolo Frequency Accessor Methods
			
			
			/// Return the smoothing amount for the tremolo's modulation wave.
			/**
			  * This parameter is used to smooth abrupt amplitude transitions that are
			  * present for square and saw modulation waves. It has the range [0,1], where
			  * 0 indicates no smoothing and 1 indicates full smoothing. The smoothing value
			  * can be interpreted as the time (expressed as a fraction of a full modulation period)
			  * that it takes for the gain reduction stage to react to a change in the modulation wave.
			  */
			OM_INLINE Float getSmoothing() const
			{
				return smoothing;
			}
			
			
			/// Set the smoothing amount for the tremolo's modulation wave.
			/**
			  * This parameter is used to smooth abrupt amplitude transitions that are
			  * present for square and saw modulation waves. It has the range [0,1], where
			  * 0 indicates no smoothing and 1 indicates full smoothing. The smoothing value
			  * can be interpreted as the time (expressed as a fraction of a full modulation period)
			  * that it takes for the gain reduction stage to react to a change in the modulation wave.
			  *
			  * The new smoothing amount is clamped to the valid range of [0,1].
			  */
			OM_INLINE void setSmoothing( Float newSmoothing )
			{
				lockMutex();
				smoothing = math::clamp( newSmoothing, Float(0), Float(1) );
				unlockMutex();
			}
			
			
		//********************************************************************************
		//******	Tremolo Frequency Accessor Methods
			
			
			/// Return the modulation phase offset of the channel with the specified index.
			/**
			  * This value, specified in degrees, indicates how much the phase of the channel
			  * should be shifted by. This parameter allows the creation of stereo (or higher)
			  * modulation effects. For example, if the phase of the left channel is 0 and the phase
			  * of the right channel is 180, the channels' modulation will be completely out-of-phase.
			  */
			OM_INLINE Float getChannelPhase( Index channelIndex ) const
			{
				if ( channelIndex < channelPhase.getSize() )
					return math::radiansToDegrees( channelPhase[channelIndex] );
				else
					return math::radiansToDegrees( globalChannelPhase );
			}
			
			
			/// Set the modulation phase offset of the channel with the specified index.
			/**
			  * This value, specified in degrees, indicates how much the phase of the channel
			  * should be shifted by. This parameter allows the creation of stereo (or higher)
			  * modulation effects. For example, if the phase of the left channel is 0 and the phase
			  * of the right channel is 180, the channels' modulation will be completely out-of-phase.
			  *
			  * The input phase value is clamped so that the new phase value lies between -180 and 180 degrees.
			  */
			void setChannelPhase( Index channelIndex, Float newPhase );
			
			
			/// Set the modulation phase offset for all channels.
			/**
			  * Doing this brings all channels into phase with each other (regardless of what phase that is).
			  *
			  * The input phase value is clamped so that the new phase value lies between -180 and 180 degrees.
			  */
			void setChannelPhase( Float newPhase );
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this tremolo.
			/**
			  * The method returns the string "Tremolo".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this tremolo.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this tremolo.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::MODULATION.
			  */
			virtual FilterCategory getCategory() const;
			
			
			/// Return whether or not this tremolo can process audio data in-place.
			/**
			  * This method always returns TRUE, tremolos can process audio data in-place.
			  */
			virtual Bool allowsInPlaceProcessing() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this tremolo has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the tremolo parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
			/// Get any special name associated with the specified value of an indexed parameter.
			virtual Bool getParameterValueName( Index parameterIndex, const FilterParameter& value, UTF8String& name ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this tremolo.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this tremolo.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this tremolo.
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
			
			
			/// Fill the output frame with the amplitude modulated input sound.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
			/// Modulate the given input buffer by the template wave function.
			template < Sample32f (*waveFunction)( Float ) >
			OM_INLINE void modulate( const SoundBuffer& inputBuffer, SoundBuffer& outputBuffer, Size numSamples,
										Float frequencyChangePerSample, Gain depthChangePerSample, Gain envelopeChange );
			
			
		//********************************************************************************
		//******	Private Wave Generation Methods
			
			
			/// Compute the value of a cosine wave, given the specified phase value in radians.
			/**
			  * The output of this function is biased so that the sine wave has minima
			  * and maxima at y=0 and y=1.
			  */
			OM_FORCE_INLINE static Sample32f cosine( Float phase )
			{
				return Float(0.5)*(math::cos( phase + math::pi<Float>() ) + Float(1));
			}
			
			
			/// Compute the value of a square wave, given the specified phase value in radians.
			/**
			  * The output of this function is biased so that the square wave has minima
			  * and maxima at y=0 and y=1.
			  */
			OM_FORCE_INLINE static Sample32f square( Float phase )
			{
				return math::mod( phase, Float(2)*math::pi<Float>() ) <= math::pi<Float>() ? 
									Sample32f(0) : Sample32f(1);
			}
			
			
			/// Compute the value of a saw wave, given the specified phase value in radians.
			/**
			  * The output of this function is biased so that the saw wave has minima
			  * and maxima at y=0 and y=1.
			  */
			OM_FORCE_INLINE static Sample32f saw( Float phase )
			{
				Float phaseOverTwoPi = -phase / (Float(2)*math::pi<Float>());
				return phaseOverTwoPi - math::floor(phaseOverTwoPi);
			}
			
			
			/// Compute the value of a triangle wave, given the specified phase value in radians.
			/**
			  * The output of this function is biased so that the triangle wave has minima
			  * and maxima at y=0 and y=1.
			  */
			OM_FORCE_INLINE static Sample32f triangle( Float phase )
			{
				Float phaseOverTwoPi = phase / (Float(2)*math::pi<Float>());
				Float saw = (phaseOverTwoPi - math::floor(phaseOverTwoPi + Float(0.5)));
				
				return Float(2)*math::abs(saw);
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value indicating the type of modulation wave to use for this tremolo.
			WaveType type;
			
			
			/// The frequency of the tremolo's modulation wave in hertz.
			Float frequency;
			
			
			/// The target frequency of the tremolo's modulation wave in hertz.
			/**
			  * This value allows the tremolo to do smooth transitions between
			  * different modulation frequencies.
			  */
			Float targetFrequency;
			
			
			/// The linear gain factor applied to the input when the modulation wave is at its lowest point.
			/**
			  * The output signal is at unity (0dB) gain with respect to the input signal whenever the
			  * modulation wave is at its peak. When the modulation wave is at its lowest
			  * point, the signal is affected totally by this linear gain factor.
			  */
			Gain depth;
			
			
			/// The target depth for this tremolo.
			/**
			  * This value allows the tremolo to do smooth transitions between
			  * different depths.
			  */
			Gain targetDepth;
			
			
			/// The fractional amount of one modulation period that it takes for an impulse to exponentially decay.
			/**
			  * This value range from 0 to 1 and is used to smooth the abrubt amplitude
			  * changes caused by square and saw waves.
			  */
			Float smoothing;
			
			
			/// The current envelope of the modulation wave for each channel.
			Array<Gain> envelope;
			
			
			/// The modulation phase offset of each channel (in radians).
			/**
			  * This allows different channels to be in different phases,
			  * creating a stereo (or higher) tremolo effect.
			  */
			Array<Float> channelPhase;
			
			
			/// The channel phase offset to use for all channels for which the phase has not been set.
			Float globalChannelPhase;
			
			
			/// The current phase of the tremolo's modulation wave (in radians).
			Float phase;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_TREMOLO_H
