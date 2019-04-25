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

#ifndef INCLUDE_OM_DISPLAY_ID_H
#define INCLUDE_OM_DISPLAY_ID_H


#include "omSystemConfig.h"


//##########################################################################################
//**************************     Start Rim System Namespace     ****************************
OM_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that is used to encapsulate a unique identifier for a system video display.
/**
  * This opaque type uses a platform-dependent internal representation which uniquely
  * identifies a connected video display on this system.
  */
class DisplayID
{
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
#if defined(OM_PLATFORM_WINDOWS)
			/// The underlying representation of a DisplayID on windows, a UTF-16 encoded ID string.
			typedef data::UTF16String IDType;
#else
			/// The underlying representation of a DisplayID, an unsigned integer.
			typedef PointerInt IDType;
#endif
			
			
		//********************************************************************************
		//******	Private Constructor
			
			
			/// Create a DisplayID object that represents an invalid display ID.
			OM_INLINE explicit DisplayID()
				:	displayID( INVALID_ID )
			{
			}
			
			
			/// Create a DisplayID object that represents the display with the specified device ID.
			OM_INLINE explicit DisplayID( const IDType& newDisplayID )
				:	displayID( newDisplayID )
			{
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this display ID represents the same display as another.
			OM_INLINE Bool operator == ( const DisplayID& other ) const
			{
				return displayID == other.displayID;
			}
			
			
			/// Return whether or not this display ID represents a different display than another.
			OM_INLINE Bool operator != ( const DisplayID& other ) const
			{
				return displayID != other.displayID;
			}
			
			
		//********************************************************************************
		//******	Device Status Accessor Method
			
			
			/// Return whether or not this DisplayID represents a valid display.
			/**
			  * This condition is met whenever the display ID is not equal to INVALID_DISPLAY_ID.
			  */
			OM_INLINE Bool isValid() const
			{
				return displayID != INVALID_ID;
			}
			
			
		//********************************************************************************
		//******	Platform-Specific ID Accessor Methods
			
			
			/// Return a const reference to the internal ID value representing a video display on this system.
			OM_INLINE const IDType& getID() const
			{
				return displayID;
			}
			
			
			/// Return a const reference to the internal ID value representing a video display on this system.
			OM_INLINE operator const IDType& () const
			{
				return displayID;
			}
			
			
		//********************************************************************************
		//******	Public Static Data Members
			
			
			/// An instance of DisplayID that represents an invalid video display.
			static const DisplayID INVALID;
			
			
			/// The reserved internal ID used to indicate an invalid display.
			static const IDType INVALID_ID;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The underlying representation of a DisplayID.
			IDType displayID;
			
			
};




//##########################################################################################
//**************************     End Rim System Namespace     ******************************
OM_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_DISPLAY_ID_H
