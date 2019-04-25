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

#include "omSoundChannelMixMatrix.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ChannelMixMatrix:: ChannelMixMatrix()
	:	gains( NULL ),
		numInputChannels( 0 ),
		numOutputChannels( 0 ),
		gainCapacity( 0 )
{
}




ChannelMixMatrix:: ChannelMixMatrix( Size newNumInputChannels, Size newNumOutputChannels )
	:	gains( NULL ),
		gainCapacity( 0 )
{
	this->initializeMatrix( newNumInputChannels, newNumOutputChannels );
}




ChannelMixMatrix:: ChannelMixMatrix( const ChannelMixMatrix& other )
{
	// Initialize the matrix.
	this->initializeMatrix( other.numInputChannels, other.numOutputChannels );
	
	// Copy the values from the other matrix.
	if ( gains != NULL )
		om::util::copy( gains, other.gains, numInputChannels*numOutputChannels );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




ChannelMixMatrix:: ~ChannelMixMatrix()
{
	if ( gains != NULL )
		om::util::deallocate( gains );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




ChannelMixMatrix& ChannelMixMatrix:: operator = ( const ChannelMixMatrix& other )
{
	if ( this != &other )
	{
		// Reallocate the gain matrix if necessary.
		if ( gainCapacity < other.gainCapacity )
		{
			if ( gains != NULL )
				om::util::deallocate( gains );
			
			gains = om::util::allocate<Gain>( other.gainCapacity );
			gainCapacity = other.gainCapacity;
		}
		
		// Copy information about the matrix.
		numInputChannels = other.numInputChannels;
		numOutputChannels = other.numOutputChannels;
		
		// Copy the matrix.
		if ( gains != NULL )
			om::util::copy( gains, other.gains, numInputChannels*numOutputChannels );
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Zero Method
//############		
//##########################################################################################
//##########################################################################################




void ChannelMixMatrix:: zero()
{
	om::util::zero( gains, numInputChannels*numOutputChannels );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mix Matrix Initialization Method
//############		
//##########################################################################################
//##########################################################################################




void ChannelMixMatrix:: resizeMatrix( Size newNumInputChannels, Size newNumOutputChannels )
{
	Size newGainCapacity = newNumInputChannels*newNumOutputChannels;
	
	// Reallocate the gain matrix if necessary.
	if ( newGainCapacity > gainCapacity )
	{
		// Deallocate the gain matrix if it has been previously allocated.
		if ( gains != NULL )
			om::util::deallocate( gains );
		
		// Allocate the new gain matrix.
		gains = om::util::allocate<Gain>( newGainCapacity );
		gainCapacity = newGainCapacity;
	}
	
	// Set the new input/ouput channel counts.
	numInputChannels = newNumInputChannels;
	numOutputChannels = newNumOutputChannels;
}




void ChannelMixMatrix:: initializeMatrix( Size newNumInputChannels, Size newNumOutputChannels )
{
	// Set the new input/ouput channel counts.
	numInputChannels = newNumInputChannels;
	numOutputChannels = newNumOutputChannels;
	gainCapacity = newNumInputChannels*newNumOutputChannels;
	
	// Allocate the gain matrix if necessary.
	if ( gainCapacity > 0 )
		gains = om::util::allocate<Gain>( gainCapacity );
	else
		gains = NULL;
}




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
