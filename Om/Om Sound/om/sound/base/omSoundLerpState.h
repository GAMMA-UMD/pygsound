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

#ifndef INCLUDE_OM_SOUND_LERP_STATE_H
#define INCLUDE_OM_SOUND_LERP_STATE_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores the state for a time-domain linear interpolation.
template < typename T, typename TimeType = Float >
class LerpState
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default linear interpolation state.
			OM_INLINE LerpState()
				:	current( T(0) ),
					target( T(0) ),
					time( TimeType(0) )
			{
			}
			
			
			/// Create a new linear interpolation state object with the given current, target, and time values.
			OM_INLINE LerpState( T newCurrent, T newTarget, TimeType newTime )
				:	current( newCurrent ),
					target( newTarget ),
					time( newTime )
			{
			}
			
			
		//********************************************************************************
		//******	State Accessor Methods
			
			
			/// Return whether or not the interpolation is finished.
			/**
			  * The interpolation is finished when the time remaining on the
			  * interpolation is equal to zero.
			  */
			OM_INLINE Bool isFinished() const
			{
				return time == TimeType(0);
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The current interpolation value.
			T current;
			
			
			/// The target value for the interpolation.
			T target;
			
			
			/// The time that it should take for the remaining interpolation to occur.
			TimeType time;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_LERP_STATE_H
