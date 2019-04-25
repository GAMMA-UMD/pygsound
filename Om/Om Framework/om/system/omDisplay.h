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

#ifndef INCLUDE_OM_DISPLAY_H
#define INCLUDE_OM_DISPLAY_H


#include "omSystemConfig.h"


#include "omDisplayID.h"
#include "omDisplayMode.h"


//##########################################################################################
//**************************     Start Rim System Namespace     ****************************
OM_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents an interface to a single connected video display.
/**
  * This class allows the user to query available displays, their supported
  * modes, and allows the user to change display parameters such as size (resolution)
  * and bit depth.
  */
class Display
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a display object that interfaces with the physical display with the specified ID.
			Display( DisplayID newDisplayID );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this display object and release all state associated with it.
			/**
			  * Destructing a display object does not effect the physical display device.
			  */
			~Display();
			
			
		//********************************************************************************
		//******	Display Mode Accessor Methods
			
			
			/// Return the number of valid modes there are for this display.
			Size getModeCount() const;
			
			
			/// Return the mode for this display at the specified index.
			DisplayMode getMode( Index modeIndex ) const;
			
			
			/// Return the current mode for this display.
			DisplayMode getCurrentMode() const;
			
			
			/// Attempt to change the display's current mode to the specified mode.
			/**
			  * This method synchronously changes the mode of this display and won't
			  * return until the mode change is complete. If the method can't find
			  * a display mode for this display that matches the specified mode,
			  * FALSE is returned and the display's mode is not changed. Otherwise,
			  * TRUE is returned and the display's mode is changed to the specified mode.
			  */
			Bool setMode( const DisplayMode& newMode );
			
			
			/// Attempt to change the display's current mode to the valid mode closest to the specified mode.
			/**
			  * This method synchronously changes the mode of this display and won't
			  * return until the mode change is complete. If the method can't find
			  * a display mode for this display that is close to the specified mode,
			  * FALSE is returned and the display's mode is not changed. Otherwise,
			  * TRUE is returned and the display's mode is changed.
			  */
			Bool setBestMode( const DisplayMode& newMode );
			
			
		//********************************************************************************
		//******	Display Capturing Methods
			
			
			/// Attempt to capture this display for exclusive use by the application.
			/**
			  * The method returns whether or not the capture operation was successful.
			  * Capturing the display prevents other applications from detecting that the
			  * display mode has changed and will keep their windows from being resized.
			  * One should always call this method when changing the display's size as
			  * a courtesy to other applications.
			  */
			Bool capture();
			
			
			/// Release this display if it was previously captured by the application.
			/**
			  * The method returns whether or not the capture operation was successful.
			  */
			void release();
			
			
			/// Return whether or not this display is currently captured for exclusive use by the application.
			Bool isCaptured() const;
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return a 2D vector indicating the resolution in pixels of this display.
			math::Size2D getResolution() const;
			
			
			/// Attempt to change the resolution in pixels of this display.
			/**
			  * The display chooses the available display mode with the size closest
			  * to the given size and changes the display to that size. If this operation
			  * is successful, TRUE is returned. Otherwise, no size change is performed and
			  * FALSE is returned.
			  *
			  * This method may change other display attributes like the refresh rate, but
			  * it will use the display mode that most closely matches the current values of
			  * those attributes while satisfying the size requirement.
			  */
			Bool setResolution( const math::Size2D& newResolution );
			
			
			/// Attempt to change the resolution in pixels of this display.
			/**
			  * The display chooses the available display mode with the size closest
			  * to the given size and changes the display to that size. If this operation
			  * is successful, TRUE is returned. Otherwise, no size change is performed and
			  * FALSE is returned.
			  *
			  * This method may change other display attributes like the refresh rate, but
			  * it will use the display mode that most closely matches the current values of
			  * those attributes while satisfying the size requirement.
			  */
			OM_INLINE Bool setResolution( Size width, Size height )
			{
				return this->setResolution( math::Size2D( width, height ) );
			}
			
			
		//********************************************************************************
		//******	Refresh Rate Accessor Method
			
			
			/// Return the frequency of this display's refresh rate, in frames per second (Hz).
			Double getRefreshRate() const;
			
			
			/// Attempt to change the refresh rate of this display.
			/**
			  * The display chooses the available display mode with the refresh rate closest
			  * to the given one and changes the display to that refresh rate. If this operation
			  * is successful, TRUE is returned. Otherwise, no refresh rate change is performed and
			  * FALSE is returned.
			  */
			Bool setRefreshRate( Double newRefreshRate );
			
			
		//********************************************************************************
		//******	Bits Per Pixel Accessor Method
			
			
			/// Return number of bits used to represent each pixel of this display's current mode.
			Size getBitsPerPixel() const;
			
			
			/// Attempt to change the bit depth of this display.
			/**
			  * The display chooses the available display mode with the bit depth closest
			  * to the given one and changes the display to that bit depth. If this operation
			  * is successful, TRUE is returned. Otherwise, no bit depth change is performed and
			  * FALSE is returned.
			  */
			Bool setBitsPerPixel( Size newBitsPerPixel );
			
			
		//********************************************************************************
		//******	Display Status Accessor Method
			
			
			/// Return whether or not this display object represents a valid physical video display.
			Bool isValid() const;
			
			
			/// Return whether or not this display is the main display for the system.
			Bool isMain() const;
			
			
		//********************************************************************************
		//******	Display ID Accessor Method
			
			
			/// Return a display ID which uniquely identifies this display on this system.
			DisplayID getID() const;
			
			
			/// Return a human-readable name for this display.
			data::UTF8String getName() const;
			
			
		//********************************************************************************
		//******	Static Display Accessor Methods
			
			
			/// Return the total number of displays that are connected to the system.
			static Size getCount();
			
			
			/// Return an ID representing the display connected to the system at the specified index.
			static DisplayID getID( Index displayIndex );
			
			
			/// Return the ID of the display which represents the main video display for the system.
			static DisplayID getMainID();
			
			
	private:
		
		//********************************************************************************
		//******	Private Helper Methods
			
			
			/// Refresh the cache of the valid modes for this display so that they can be reused efficiently.
			void refreshDisplayModes();
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A value that represents a unique identifier for this connected display.
			DisplayID displayID;
			
			
			/// A list of all of the valid display modes for this display.
			util::ArrayList<DisplayMode> modes;
			
			
			/// A boolean value indicating whether or not this display object has cached its valid modes.
			mutable Bool hasCachedModes;
			
			
};




//##########################################################################################
//**************************     End Rim System Namespace     ******************************
OM_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_DISPLAY_H
