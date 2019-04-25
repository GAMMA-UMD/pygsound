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

#include "omDisplay.h"


#ifdef OM_PLATFORM_WINDOWS

#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>


//##########################################################################################
//**************************     Start Rim System Namespace     ****************************
OM_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// Find a suitable native display mode for the device with the given ID string and the desired mode.
static Bool getDisplayMode( const data::UTF16String& deviceString, const DisplayMode& desiredMode, DEVMODE& mode, Bool strict )
{
	// Enumerate all of the modes for this display.
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	DWORD currentMode = 0;
	
	if ( strict )
	{
		for ( ; EnumDisplaySettingsEx( (LPCTSTR)deviceString.getCString(), currentMode, &mode, 0 ); currentMode++ )
		{
			DisplayMode testMode;
			math::Size2D resolution;

			// Is the display width initialized for this mode?
			if ( mode.dmFields & DM_PELSWIDTH )
				resolution.x = (Size)mode.dmPelsWidth;
			
			// Is the display height initialized for this mode?
			if ( mode.dmFields & DM_PELSHEIGHT )
				resolution.y = (Size)mode.dmPelsHeight;
			
			testMode.setResolution( resolution );

			// Is the display refresh rate initialized for this mode?
			if ( mode.dmFields & DM_DISPLAYFREQUENCY )
				testMode.setRefreshRate( (Double)mode.dmDisplayFrequency );
			
			// Is the display bits per pixel initialized for this mode?
			if ( mode.dmFields & DM_BITSPERPEL )
				testMode.setBitsPerPixel( (Size)mode.dmBitsPerPel );
			
			// Check to see if the modes are exactly equal. If so, return that we found a valid mode.
			if ( desiredMode == testMode )
				return true;
		}
	}
	else
	{
		math::Vector2f desiredSize = math::Vector2f( desiredMode.getResolution() );
		Bool foundMatch = false;
		Float minimumError = math::max<Float>();
		DWORD bestMatchIndex = 0;
		
		
		for ( ; EnumDisplaySettingsEx( (LPCTSTR)deviceString.getCString(), currentMode, &mode, 0 ); currentMode++ )
		{
			math::Vector2f testSize;
			Size bitsPerPixel = 0;
			Double refreshRate = 0;
			
			// Is the display width initialized for this mode?
			if ( mode.dmFields & DM_PELSWIDTH )
				testSize.x = (Float)mode.dmPelsWidth;
			
			// Is the display height initialized for this mode?
			if ( mode.dmFields & DM_PELSHEIGHT )
				testSize.y = (Float)mode.dmPelsHeight;
			
			// Is the display refresh rate initialized for this mode?
			if ( mode.dmFields & DM_DISPLAYFREQUENCY )
				refreshRate = (Double)mode.dmDisplayFrequency;
			
			// Is the display bits per pixel initialized for this mode?
			if ( mode.dmFields & DM_BITSPERPEL )
				bitsPerPixel = (Size)mode.dmBitsPerPel;
			
			// Compute the squared error for both dimensions.
			Float error = (testSize - desiredSize).getMagnitudeSquared();
			
			// Add the error by the refresh rate fraction closeness.
			if ( desiredMode.getRefreshRate() != Double(0) )
				error += (Float)(math::abs( refreshRate - desiredMode.getRefreshRate() ) / desiredMode.getRefreshRate());
			
			// Find the mode with the smallest size error.
			if ( bitsPerPixel >= desiredMode.getBitsPerPixel() && error < minimumError )
			{
				minimumError = error;
				bestMatchIndex = currentMode;
				foundMatch = true;
			}
		}
		
		if ( foundMatch )
		{
			EnumDisplaySettingsEx( (LPCTSTR)deviceString.getCString(), bestMatchIndex, &mode, 0 );
			return true;
		}
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




Display:: Display( DisplayID newDisplayID )
	:	displayID( newDisplayID ),
		hasCachedModes( false )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




Display:: ~Display()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Display Mode Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size Display:: getModeCount() const
{
	if ( !this->isValid() )
		return 0;
	
	if ( !hasCachedModes )
	{
		const_cast<Display*>(this)->refreshDisplayModes();
		hasCachedModes = true;
	}
	
	return modes.getSize();
}




DisplayMode Display:: getMode( Index modeIndex ) const
{
	if ( !this->isValid() )
		return DisplayMode();
	
	if ( !hasCachedModes )
	{
		const_cast<Display*>(this)->refreshDisplayModes();
		hasCachedModes = true;
	}
	
	if ( modeIndex < modes.getSize() )
		return modes[modeIndex];
	else
		return DisplayMode();
}




DisplayMode Display:: getCurrentMode() const
{
	return DisplayMode( this->getResolution(), this->getRefreshRate(), this->getBitsPerPixel() );
}




Bool Display:: setMode( const DisplayMode& desiredMode )
{
	if ( !this->isValid() )
		return false;
	
	DEVMODE mode;
	
	// Get a device mode structure which exactly matches the desired display mode.
	// If none is found, return that the mode could not be changed.
	if ( !getDisplayMode( displayID.getID(), desiredMode, mode, true ) )
		return false;
	
	// Attempt to change the display setting.
	return ChangeDisplaySettings( &mode, CDS_FULLSCREEN ) == DISP_CHANGE_SUCCESSFUL;
}




Bool Display:: setBestMode( const DisplayMode& desiredMode )
{
	if ( !this->isValid() )
		return false;
	
	DEVMODE mode;
	
	// Get a device mode structure which best matches the desired display mode.
	// If none is found, return that the mode could not be changed.
	if ( !getDisplayMode( displayID.getID(), desiredMode, mode, false ) )
		return false;
	
	// Attempt to change the display setting.
	return ChangeDisplaySettings( &mode, CDS_FULLSCREEN ) == DISP_CHANGE_SUCCESSFUL;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Display Capturing Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Display:: capture()
{
	return false;
}




void Display:: release()
{
}




Bool Display:: isCaptured() const
{
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Size Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




math::Size2D Display:: getResolution() const
{
	if ( !this->isValid() )
		return math::Size2D( 0, 0 );
	
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	
	// Get the current display mode for the display.
	if ( !EnumDisplaySettingsEx( (LPCTSTR)displayID.getID().getCString(), ENUM_CURRENT_SETTINGS, &mode, 0 ) )
		return math::Size2D( 0, 0 );
	
	math::Size2D resolution( 0, 0 );
	
	// Set the width of the display if it was initialized.
	if ( mode.dmFields & DM_PELSWIDTH )
		resolution.x = (Size)mode.dmPelsWidth;
	
	// Set the height of the display if it was initialized.
	if ( mode.dmFields & DM_PELSHEIGHT )
		resolution.y = (Size)mode.dmPelsHeight;
	
	return resolution;
}




Bool Display:: setResolution( const math::Size2D& newSize )
{
	DisplayMode mode = this->getCurrentMode();
	mode.setResolution( newSize );
	
	return this->setBestMode( mode );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Refresh Rate Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Double Display:: getRefreshRate() const
{
	if ( !displayID.isValid() )
		return Double(0);
	
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	
	// Get the current display mode for the display.
	if ( !EnumDisplaySettingsEx( (LPCTSTR)displayID.getID().getCString(), ENUM_CURRENT_SETTINGS, &mode, 0 ) )
		return Double(0);
	
	// If the refresh rate was initialized, return it.
	if ( mode.dmFields & DM_DISPLAYFREQUENCY )
		return Double(mode.dmDisplayFrequency);
	else
		return Double(0);
}




Bool Display:: setRefreshRate( Double newRefreshRate )
{
	DisplayMode mode = this->getCurrentMode();
	mode.setRefreshRate( newRefreshRate );
	
	return this->setBestMode( mode );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Bits Per Pixel Accessor Method
//############		
//##########################################################################################
//##########################################################################################




Size Display:: getBitsPerPixel() const
{
	if ( !displayID.isValid() )
		return Size(0);
	
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	
	// Get the current display mode for the display.
	if ( !EnumDisplaySettingsEx( (LPCTSTR)displayID.getID().getCString(), ENUM_CURRENT_SETTINGS, &mode, 0 ) )
		return Size(0);
	
	// If the bits per pixel was initialized, return it.
	if ( mode.dmFields & DM_BITSPERPEL )
		return Size(mode.dmBitsPerPel);
	else
		return Size(0);
}




Bool Display:: setBitsPerPixel( Size newBitsPerPixel )
{
	DisplayMode mode = this->getCurrentMode();
	mode.setBitsPerPixel( newBitsPerPixel );
	
	return this->setMode( mode );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Display Status Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool Display:: isValid() const
{
	if ( !displayID.isValid() )
		return false;
	
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	
	return EnumDisplaySettingsEx( (LPCTSTR)displayID.getID().getCString(), 0, &mode, 0 ) != BOOL(0);
}




Bool Display:: isMain() const
{
	if ( !displayID.isValid() )
		return false;
	
	DISPLAY_DEVICE device;
	device.cb = sizeof(DISPLAY_DEVICE);
	
	if ( !EnumDisplayDevices( (LPCTSTR)displayID.getID().getCString(), 0, &device, 0 ) )
		return false;
	
	return (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != DWORD(0);
}




//##########################################################################################
//##########################################################################################
//############		
//############		Display ID Accessor Method
//############		
//##########################################################################################
//##########################################################################################




DisplayID Display:: getID() const
{
	return displayID;
}




data::UTF8String Display:: getName() const
{
	if ( !displayID.isValid() )
		return data::UTF8String();
	
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	
	// Get the current display mode for the display.
	if ( !EnumDisplaySettingsEx( (LPCTSTR)displayID.getID().getCString(), ENUM_CURRENT_SETTINGS, &mode, 0 ) )
		return data::UTF8String();
	
	// Copy the name and convert it to UTF8 encoding.
	return data::UTF8String( data::UTF16String( (const UTF16Char*)mode.dmDeviceName ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Static Display Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size Display:: getCount()
{
	// Declare some variables that are necessary to call the function that enumerates displays.
	DISPLAY_DEVICE device;
	device.cb = sizeof(DISPLAY_DEVICE);
	DWORD currentDevice = 0;
	Size numDisplays = 0;
	
	// Enumerate the displays, counting how many are available.
	for ( ; EnumDisplayDevices( NULL, currentDevice, &device, 0 ); currentDevice++ )
	{
		// Ignore display devices that are not part of the desktop.
		if ( device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP )
			numDisplays++;
	}
	
	// Return the total number of available displays.
	return numDisplays;
}




DisplayID Display:: getID( Index displayIndex )
{
	// Declare some variables that are necessary to call the function that enumerates displays.
	DISPLAY_DEVICE device;
	device.cb = sizeof(DISPLAY_DEVICE);
	DWORD currentDevice = 0;
	Index currentDisplay = 0;
	
	// Enumerate the displays until we have reached the end, indicating an invalid
	// index, or until we reach the required display index.
	for ( ; EnumDisplayDevices( NULL, currentDevice, &device, 0 ) && currentDisplay < displayIndex; currentDevice++ )
	{
		// Ignore display devices that are not part of the desktop.
		if ( device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP )
			currentDisplay++;
	}
	
	// If the index was invalid, return an invalid display ID.
	if ( currentDisplay != displayIndex )
		return DisplayID::INVALID;
	
	// Get the name (ID) of the display device.
	return DisplayID( data::UTF16String( (const UTF16Char*)device.DeviceName ) );
}




DisplayID Display:: getMainID()
{
	// Declare some variables that are necessary to call the function that enumerates displays.
	DISPLAY_DEVICE device;
	device.cb = sizeof(DISPLAY_DEVICE);
	DWORD currentDisplay = 0;
	
	// Enumerate the displays until we find one that is the primary display.
	for ( ; EnumDisplayDevices( NULL, currentDisplay, &device, 0 ); currentDisplay++ )
	{
		// Is this display the primary one?
		if ( (device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) &&
			(device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) )
		{
			// Get the name (ID) of the main display device.
			return DisplayID( data::UTF16String( (const UTF16Char*)device.DeviceName ) );
		}
	}
	
	// If there is no main display, return an invalid ID.
	return DisplayID::INVALID;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Display Mode Refresing Method
//############		
//##########################################################################################
//##########################################################################################




void Display:: refreshDisplayModes()
{
	// Don't bother if the display is invalid.
	if ( !this->isValid() )
		return;
	
	// Clear the old list of valid modes so that we can refresh them.
	modes.clear();
	
	// Enumerate all of the modes for this display.
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	DWORD currentMode = 0;
	
	for ( ; EnumDisplaySettingsEx( (LPCTSTR)displayID.getID().getCString(), currentMode, &mode, 0 ); currentMode++ )
	{
		math::Size2D size( 0, 0 );
		Double refreshRate = 0;
		Size bitsPerPixel = 0;
		
		// Is the display width initialized for this mode?
		if ( mode.dmFields & DM_PELSWIDTH )
			size.x = (Size)mode.dmPelsWidth;
		
		// Is the display height initialized for this mode?
		if ( mode.dmFields & DM_PELSHEIGHT )
			size.y = (Size)mode.dmPelsHeight;
		
		// Is the display refresh rate initialized for this mode?
		if ( mode.dmFields & DM_DISPLAYFREQUENCY )
			refreshRate = (Double)mode.dmDisplayFrequency;
		
		// Is the display bits per pixel initialized for this mode?
		if ( mode.dmFields & DM_BITSPERPEL )
			bitsPerPixel = (Size)mode.dmBitsPerPel;
		
		// Add the new mode.
		modes.add( DisplayMode( size, refreshRate, bitsPerPixel ) );
	}
}




//##########################################################################################
//**************************     End Rim System Namespace     ******************************
OM_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // #ifdef OM_PLATFORM_WINDOWS
