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

#ifndef INCLUDE_OM_SOUND_CHANNEL_MIX_MATRIX_H
#define INCLUDE_OM_SOUND_CHANNEL_MIX_MATRIX_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that holds a matrix of gain coefficients mapping from one channel configuration to another.
/**
  * This class is used to represent the mapping from one channel configuration to another,
  * where the input configuration with N channels is mapped to the output configuration with
  * M channels using an NxM matrix of linear gain coefficients.
  */
class ChannelMixMatrix
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new channel mix matrix with 0 input and output channels.
			ChannelMixMatrix();
			
			
			/// Create a new channel mix matrix with the specified number of input and output channels.
			ChannelMixMatrix( Size newNumInputChannels, Size newNumOutputChannels );
			
			
			/// Create a copy of the specified channel mix matrix object.
			ChannelMixMatrix( const ChannelMixMatrix& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this channel mix matrix object and release its resources.
			~ChannelMixMatrix();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of the specified channel mix matrix to this mix matrix.
			ChannelMixMatrix& operator = ( const ChannelMixMatrix& other );
			
			
		//********************************************************************************
		//******	Channel Count Accessor Methods
			
			
			/// Return the current number of input channels for this channel mix matrix.
			OM_INLINE Size getInputCount() const
			{
				return numInputChannels;
			}
			
			
			/// Set the number of input channels that this channel mix matrix should have.
			/**
			  * This method reallocates the internal gain matrix if necessary
			  * and causes the values of all previously stored gain values to become
			  * undefined.
			  */
			OM_INLINE void setInputCount( Size newNumInputChannels )
			{
				this->resizeMatrix( newNumInputChannels, numOutputChannels );
			}
			
			
			/// Return the current number of output channels for this channel mix matrix.
			OM_INLINE Size getOutputCount() const
			{
				return numOutputChannels;
			}
			
			
			/// Set the number of output channels that this channel mix matrix should have.
			/**
			  * This method reallocates the internal gain matrix if necessary
			  * and causes the values of all previously stored gain values to become
			  * undefined.
			  */
			OM_INLINE void setOutputCount( Size newNumOutputChannels )
			{
				this->resizeMatrix( numInputChannels, newNumOutputChannels );
			}
			
			
			/// Set the number of input and output channels that this channel mix matrix should have.
			/**
			  * This method reallocates the internal gain matrix if necessary
			  * and causes the values of all previously stored gain values to become
			  * undefined.
			  */
			OM_INLINE void setSize( Size newNumInputChannels, Size newNumOutputChannels )
			{
				this->resizeMatrix( newNumInputChannels, newNumOutputChannels );
			}
			
			
		//********************************************************************************
		//******	Gain Accessor Methods
			
			
			/// Return the linear gain associated with the input and output channels at the specified indices.
			OM_INLINE Gain getGain( Index inputChannelIndex, Index outputChannelIndex ) const
			{
				return gains[inputChannelIndex*numOutputChannels + outputChannelIndex];
			}
			
			
			/// Set the linear gain associated with the input and output channels at the specified indices.
			OM_INLINE void setGain( Index inputChannelIndex, Index outputChannelIndex, Gain newGain )
			{
				gains[inputChannelIndex*numOutputChannels + outputChannelIndex] = newGain;
			}
			
			
			/// Set every input-to-output channel pair to have the specified linear gain value.
			OM_INLINE void setGains( Gain newGain )
			{
				Gain* currentGain = gains;
				const Gain* const gainsEnd = gains + numInputChannels*numOutputChannels;
				
				while ( currentGain != gainsEnd )
				{
					*currentGain = newGain;
					currentGain++;
				}
			}
			
			
		//********************************************************************************
		//******	Helper Methods
			
			
			/// Set every gain in the channel mix matrix to be 0.
			void zero();
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Initialize the channel mix matrix using the specified number of input and output channels.
			void initializeMatrix( Size newNumInputChannels, Size newNumOutputChannels );
			
			
			/// Resize the channel mix matrix to the specified number of input and output channels.
			void resizeMatrix( Size newNumInputChannels, Size newNumOutputChannels );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A pointer which points to a 2D matrix of gain values stored in a 1D array.
			Gain* gains;
			
			
			/// The number of input channels that this channel mix matrix has.
			Size numInputChannels;
			
			
			/// The number of output channels that this channel mix matrix has.
			Size numOutputChannels;
			
			
			/// The total size of the allocated gain array.
			Size gainCapacity;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_CHANNEL_MIX_MATRIX_H
