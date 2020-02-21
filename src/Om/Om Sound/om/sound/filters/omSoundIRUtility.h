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

#ifndef INCLUDE_OM_SOUND_IR_UTILITY_H
#define INCLUDE_OM_SOUND_IR_UTILITY_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that measures the impulse response of a sound signal processing chain.
class IRUtility : public SoundFilter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a IR utility with the default initial state.
			IRUtility();
			
			
		//********************************************************************************
		//******	Measurement Methods
			
			
			/// Start measuring the impulse response.
			/**
			  * If the response was already being measured, the utility aborts that
			  * measurement and restarts the process.
			  */
			void start();
			
			
			/// Stop measuring the impulse response if it was being measured.
			/**
			  * The method aborts the measurement.
			  */
			void stop();
			
			
			/// Return whether or not the IR utility is currently measuring an impulse response.
			OM_INLINE Bool isMeasuring() const
			{
				return measuring;
			}
			
			
			/// Return whether or not the IR utility has finished measuring data for an impulse resopnse.
			OM_INLINE Bool hasMeasurement() const
			{
				return deconvolution;
			}
			
			
			/// Deconvolve the measured IR and write it to the buffer output parameter.
			/**
			  * Depending on the length of the stimulus signal, this is a potentially
			  * slow operation.
			  * This method should only be called when the hasMeasurement() function returns
			  * TRUE.
			  */
			Bool deconvolve( SoundBuffer& ir ) const;
			
			
		//********************************************************************************
		//******	Channel Count Accessor Methods
			
			
			/// Return the number of channels there are in the measured impulse response.
			OM_INLINE Size getChannelCount() const
			{
				return channelCount;
			}
			
			
			/// Set the number of channels there are in the measured impulse response.
			/**
			  * If the IR was in the process of being measured, the measurement is aborted
			  * and the channel count is changed.
			  */
			void setChannelCount( Size newChannelCount );
			
			
		//********************************************************************************
		//******	IR Length Accessor Methods
			
			
			/// Return the length of the measured impulse response.
			OM_INLINE const Time& getIRLength() const
			{
				return irLength;
			}
			
			
			/// Set the length of the measured impulse response.
			/**
			  * If the IR was in the process of being measured, the measurement is aborted
			  * and the IR length is changed.
			  */
			void setIRLength( const Time& newIRLength );
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the sample rate of the impulse response that is being measured.
			OM_INLINE SampleRate getSampleRate() const
			{
				return sampleRate;
			}
			
			
			/// Set the sample rate of the impulse response that is being measured.
			/**
			  * If the IR was in the process of being measured, the measurement is aborted
			  * and the sample rate is changed.
			  */
			void setSampleRate( SampleRate newSampleRate );
			
			
		//********************************************************************************
		//******	Stimulus Length Accessor Methods
			
			
			/// Return the length of the IR test signal.
			/**
			  * This influences the signal to noise ratio of the resulting impulse response
			  * as well as the computation time. A longer stimulus results in a better IR but
			  * takes longer to compute.
			  */
			OM_INLINE const Time& getStimulusLength() const
			{
				return stimulusLength;
			}
			
			
			/// Set the length of the IR test signal.
			/**
			  * This influences the signal to noise ratio of the resulting impulse response
			  * as well as the computation time. A longer stimulus results in a better IR but
			  * takes longer to compute.
			  * If the IR was in the process of being measured, the measurement is aborted
			  * and the IR length is changed.
			  */
			void setStimulusLength( const Time& newStimulusLength );
			
			
		//********************************************************************************
		//******	Measurement Count Accessor Methods
			
			
			/// Return the number of repeated measurements that should be taken for robustness.
			OM_INLINE Size getMeasurementCount() const
			{
				return measurementCount;
			}
			
			
			/// Set the number of repeated measurements that should be taken for robustness.
			/**
			  * If the IR was in the process of being measured, the measurement is aborted
			  * and the measurement count is changed.
			  */
			void setMeasurementCount( Size newMeasurementCount );
			
			
		//********************************************************************************
		//******	Filter Attribute Accessor Methods
			
			
			/// Return a human-readable name for this IR utility.
			/**
			  * The method returns the string "IR Utility".
			  */
			virtual UTF8String getName() const;
			
			
			/// Return the manufacturer name of this IR utility.
			/**
			  * The method returns the string "Om Sound".
			  */
			virtual UTF8String getManufacturer() const;
			
			
			/// Return an object representing the version of this IR utility.
			virtual FilterVersion getVersion() const;
			
			
			/// Return an object that describes the category of effect that this filter implements.
			/**
			  * This method returns the value FilterCategory::ANALYSIS.
			  */
			virtual FilterCategory getCategory() const;
			
			
		//********************************************************************************
		//******	Filter Parameter Accessor Methods
			
			
			/// Return the total number of generic accessible parameters this filter has.
			virtual Size getParameterCount() const;
			
			
			/// Get information about the parameter at the specified index.
			virtual Bool getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const;
			
			
		//********************************************************************************
		//******	Public Static Property Objects
			
			
			/// A string indicating the human-readable name of this IR utility.
			static const UTF8String NAME;
			
			
			/// A string indicating the manufacturer name of this IR utility.
			static const UTF8String MANUFACTURER;
			
			
			/// An object indicating the version of this IR utility.
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
			
			
			/// Apply this IR utility to the samples in the input frame and place them in the output frame.
			virtual SoundResult processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples );
			
			
		//********************************************************************************
		//******	Private Helper Methods Methods
			
			
			/// Deconvolve the IR from the two golay code sequences and the recorded responses.
			void deconvolveIR();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of channels in the impulse response that the utility is measuring.
			Size channelCount;
			
			
			/// The sample rate of the impulse response that this utility is measuring.
			SampleRate sampleRate;
			
			
			/// The length of the IR that the utility is measuring.
			Time irLength;
			
			
			/// The number of samples that must be recorded when measuring the IR.
			Size irLengthInSamples;
			
			
			/// The length of the IR test signal.
			/**
			  * This influences the signal to noise ratio of the resulting impulse response
			  * as well as the computation time. A longer stimulus results in a better IR but
			  * takes longer to compute.
			  */
			Time stimulusLength;
			
			
			/// The number of samples that must be recorded when measuring the IR.
			Size measurementLength;
			
			
			/// The number of repeated measurements that should be taken for robustness.
			Size measurementCount;
			
			
			/// The index of the current IR measurement that is being performed.
			Index measurementIndex;
			
			
			/// The sample index for the current measurement in the recorded response length.
			Index measurementPosition;
			
			
			/// The length of the generated golay sequence.
			Size golayLength;
			
			
			/// An array that stores a pair of golay codes that are used to measure an impulse response.
			PODArray<Sample32f,2> golay;
			
			
			/// An array that stores the recorded response produced by the golay codes.
			PODArray<Sample32f,2> response;
			
			
			/// A boolean value that keeps track of whether or not the utility is currently measuring an IR.
			Bool measuring;
			
			
			/// A boolean value that keeps track of whether or not the IR should be deconvolved.
			Bool deconvolution;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_IR_UTILITY_H
