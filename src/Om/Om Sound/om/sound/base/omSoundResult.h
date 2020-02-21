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

#ifndef INCLUDE_OM_SOUND_RESULT_H
#define INCLUDE_OM_SOUND_RESULT_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the result of a sound frame.
class SoundResult
{
	public:
		
		//********************************************************************************
		//******	Status Enum Declaration
			
			
			/// An enum type which describes the different allowed result statuses.
			enum Status
			{
				/// A result status indicating that sound processing was successful.
				/**
				  * The number of samples that were successfully processed is stored in the result.
				  */
				SUCCESS = 1,
				
				/// A result status indicating that sound processing was successful and that the result is silence.
				/**
				  * This status means that all outputs of the given sound processing step should
				  * be interpreted as silent for the number of samples given by the result.
				  * This status can be used to ignore sound outputs that produce no sound and thus
				  * don't need to be processed.
				  */
				SILENCE = 0,
				
				/// A result status indicating that there are no more sound samples to process.
				/**
				  * This status is primarily valid when used by a sound that does not depend on
				  * any input (such as a sound player). It indicates that the end of the given sound
				  * source has been reached and will not produce anymore sound. Therefore, this
				  * status could be used to halt usage of a particular output-only sound once
				  * it has produced all sound it can.
				  */
				END = 2,
				
				/// A result status indicating that an error occurred during processing.
				/**
				  * The number of samples that were successfully processed is stored in the result.
				  * Therefore, a sound can indicate if an error occurred while still producing some
				  * audio.
				  */
				ERROR = -1
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create new result with the END status that has 0 valid output samples.
			OM_INLINE SoundResult()
				:	status( END ),
					numSamples( 0 )
			{
			}
			
			
			/// Create new result with the SUCCESS status that has the specified number of valid output samples.
			OM_INLINE SoundResult( Size newNumSamples )
				:	status( SUCCESS ),
					numSamples( newNumSamples )
			{
			}
			
			
			/// Create new result with the specified status that has 0 valid output samples.
			OM_INLINE SoundResult( Status newStatus )
				:	status( newStatus ),
					numSamples( 0 )
			{
			}
			
			
			/// Create new result with the given status that has the specified number of valid output samples.
			OM_INLINE SoundResult( Status newStatus, Size newNumSamples )
				:	status( newStatus ),
					numSamples( newNumSamples )
			{
			}
			
			
		//********************************************************************************
		//******	Sample Count Accessor Method
			
			
			/// Return the total number of samples that were produced as part of this sound result.
			OM_INLINE Size getSampleCount() const
			{
				return numSamples;
			}
			
			
		//********************************************************************************
		//******	Status Accessor Method
			
			
			/// Return an enum value representing the status of this sound result.
			OM_INLINE Status getStatus() const
			{
				return status;
			}
			
			
		//********************************************************************************
		//******	Result Cast Operators
			
			
			/// Convert this sound result to an enum value representing its status.
			OM_INLINE operator Status () const
			{
				return status;
			}
			
			
			/// Convert this sound result to a boolean value indicating if the result is a successful one.
			/**
			  * The result statuses that indicate a successful result are SUCCESS and SILENCE.
			  */
			OM_INLINE operator Bool () const
			{
				return status == SUCCESS || status == SILENCE;
			}
			
			
			/// Convert this sound result into an integer representing the number of samples that were processed.
			OM_INLINE operator Size () const
			{
				return numSamples;
			}
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value indicating the type of sound result that this is.
			Status status;
			
			
			/// The number of valid samples that were processed by the sound.
			Size numSamples;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_RESULT_H
