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


#ifdef OM_PLATFORM_APPLE
#include <Cocoa/Cocoa.h>
#include <IOKit/graphics/IOGraphicsLib.h>


//##########################################################################################
//**************************     Start Rim System Namespace     ****************************
OM_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Private Helper Methods
//############		
//##########################################################################################
//##########################################################################################




#if MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_5
static DisplayMode getDictionaryMode( const CFDictionaryRef mode )
{
	if ( mode == NULL )
		return DisplayMode();
	
	// Get the width.
	const CFNumberRef widthValue = (const CFNumberRef)CFDictionaryGetValue( mode, kCGDisplayWidth );
	Int32 width = 0;
	CFNumberGetValue( widthValue, kCFNumberSInt32Type, &width );
	
	// Get the height.
	const CFNumberRef heightValue = (const CFNumberRef)CFDictionaryGetValue( mode, kCGDisplayHeight );
	Int32 height = 0;
	CFNumberGetValue( heightValue, kCFNumberSInt32Type, &height );
	
	// Get the refresh rate.
	const CFNumberRef refreshRateValue = (const CFNumberRef)CFDictionaryGetValue( mode, kCGDisplayRefreshRate );
	Double refreshRate = 0;
	CFNumberGetValue( refreshRateValue, kCFNumberDoubleType, &refreshRate );
	
	// Get the bits per pixel.
	const CFNumberRef bitsPerPixelValue = (const CFNumberRef)CFDictionaryGetValue( mode, kCGDisplayBitsPerPixel );
	Int bitsPerPixel = 0;
	CFNumberGetValue( bitsPerPixelValue, kCFNumberIntType, &bitsPerPixel );
	
	return DisplayMode( Size2D( width, height ), refreshRate, (Size)bitsPerPixel );
}


static CFDictionaryRef getCGDisplayMode( CGDirectDisplayID displayID, const DisplayMode& desiredMode, Bool strict )
{
	CFArrayRef modeArray = CGDisplayAvailableModes( displayID );
	
	// Check to see if there was an error.
	if ( modeArray == NULL )
		return NULL;
	
	// Get the total number of display modes.
	Size numModes = CFArrayGetCount( modeArray );
	
	if ( strict )
	{
		// For each mode in the array, try to find a match for the desired display mode.
		for ( Index i = 0; i < numModes; i++ )
		{
			// Get the mode at this index from the array.
			CFDictionaryRef mode = (CFDictionaryRef)CFArrayGetValueAtIndex( modeArray, i );
			DisplayMode testMode = getDictionaryMode(mode);
			
			// Test to see if this mode matches the desired mode, if so, return it.
			if ( desiredMode == testMode )
				return mode;
		}
	}
	else
	{
		Vector2f desiredSize = desiredMode.getSize();
		Bool foundMatch = false;
		Float minimumError = math::max<Float>();
		Index bestMatchIndex = 0;
		
		for ( Index i = 0; i < numModes; i++ )
		{
			// Get the mode at this index from the array.
			CFDictionaryRef mode = (CFDictionaryRef)CFArrayGetValueAtIndex( modeArray, i );
			DisplayMode testMode = getDictionaryMode(mode);
			Vector2f testSize = testMode.getSize();
			
			// Compute the squared error for both dimensions.
			Float error = (testSize - desiredSize).getMagnitudeSquared();
			
			// Add the error by the refresh rate fraction closeness.
			if ( desiredMode.getRefreshRate() != Double(0) )
				error += math::abs( testMode.getRefreshRate() - desiredMode.getRefreshRate() ) / desiredMode.getRefreshRate();
			
			// Find the mode with the smallest size error.
			if ( testMode.getBitsPerPixel() >= desiredMode.getBitsPerPixel() && error < minimumError )
			{
				minimumError = error;
				bestMatchIndex = i;
				foundMatch = true;
			}
		}
		
		if ( foundMatch )
			return (CFDictionaryRef)CFArrayGetValueAtIndex( modeArray, bestMatchIndex );
	}
	
	return NULL;
}




#else




static Size getNumberOfBitsForDisplayMode( CGDisplayModeRef mode )
{
	Size bitsPerPixel = 0;
	
	CFStringRef pixelEncoding = CGDisplayModeCopyPixelEncoding( mode );
	
	if ( CFStringCompare( pixelEncoding, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 32;
	else if ( CFStringCompare( pixelEncoding, CFSTR(kIO30BitDirectPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 30;
	else if ( CFStringCompare( pixelEncoding, CFSTR(kIO64BitDirectPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 64;
	else if ( CFStringCompare( pixelEncoding, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 16;
	else if ( CFStringCompare( pixelEncoding, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 8;
	else if ( CFStringCompare( pixelEncoding, CFSTR(IO4BitIndexedPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 4;
	else if ( CFStringCompare( pixelEncoding, CFSTR(IO2BitIndexedPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 2;
	else if ( CFStringCompare( pixelEncoding, CFSTR(IO1BitIndexedPixels), kCFCompareCaseInsensitive ) == kCFCompareEqualTo )
		bitsPerPixel = 1;
	
	CFRelease( pixelEncoding );
	
	return bitsPerPixel;
}




static CGDisplayModeRef getCGDisplayMode( CGDirectDisplayID displayID, const DisplayMode& desiredMode, Bool strict )
{
	CFArrayRef modeArray = CGDisplayCopyAllDisplayModes( displayID, NULL );
	
	// Check to see if there was an error.
	if ( modeArray == NULL )
		return NULL;
	
	// Get the total number of display modes.
	Size numModes = CFArrayGetCount( modeArray );
	
	if ( strict )
	{
		// For each mode in the array, try to find a match for the desired display mode.
		for ( Index i = 0; i < numModes; i++ )
		{
			// Get the mode at this index from the array.
			CGDisplayModeRef mode = (CGDisplayModeRef)CFArrayGetValueAtIndex( modeArray, i );
			
			// Get information about the mode.
			math::Size2D size( CGDisplayModeGetWidth(mode), CGDisplayModeGetHeight(mode) );
			Double refreshRate = CGDisplayModeGetRefreshRate( mode );
			Size bitsPerPixel = getNumberOfBitsForDisplayMode( mode );
			
			DisplayMode testMode( size, refreshRate, bitsPerPixel );
			
			// Test to see if this mode matches the desired mode, if so, return it.
			if ( desiredMode == testMode )
			{
				CGDisplayModeRetain( mode );
				CFRelease( modeArray );
				return mode;
			}
		}
	}
	else
	{
		math::Vector2f desiredSize = desiredMode.getResolution();
		Bool foundMatch = false;
		Float minimumError = math::max<Float>();
		Index bestMatchIndex = 0;
		
		for ( Index i = 0; i < numModes; i++ )
		{
			// Get the mode at this index from the array.
			const CGDisplayModeRef mode = (const CGDisplayModeRef)CFArrayGetValueAtIndex( modeArray, i );
			
			// Get information about the mode.
			math::Vector2f testSize( CGDisplayModeGetWidth(mode), CGDisplayModeGetHeight(mode) );
			Double refreshRate = CGDisplayModeGetRefreshRate( mode );
			Size bitsPerPixel = getNumberOfBitsForDisplayMode( mode );
			
			// Compute the squared error for both dimensions.
			Float error = (testSize - desiredSize).getMagnitudeSquared();
			
			// Add the error by the refresh rate fraction closeness.
			if ( desiredMode.getRefreshRate() != Double(0) )
				error += math::abs( refreshRate - desiredMode.getRefreshRate() ) / desiredMode.getRefreshRate();
			
			// Find the mode with the smallest size error.
			if ( bitsPerPixel >= desiredMode.getBitsPerPixel() && error < minimumError )
			{
				minimumError = error;
				bestMatchIndex = i;
				foundMatch = true;
			}
		}
		
		if ( foundMatch )
		{
			CFRelease( modeArray );
			CGDisplayModeRef mode = (CGDisplayModeRef)CFArrayGetValueAtIndex( modeArray, bestMatchIndex );
			CGDisplayModeRetain( mode );
			return mode;
		}
	}
	
	// Clean up the array of display modes.
	CFRelease( modeArray );
	
	return NULL;
}
#endif




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
	
#if MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_5
	// The old implementation, for OS SDK versions 10.5 and older.
	
	// Get a mode which matches the desired mode.
	CFDictionaryRef newMode = getCGDisplayMode( displayID, desiredMode, true );
	
	// Check to see if a matching mode was found. If not, return failure.
	if ( newMode == NULL )
		return false;
	
	// Change the mode.
	CGError result = CGDisplaySwitchToMode( displayID, newMode );
	
	return result == kCGErrorSuccess;
#else
	// The current implementation, for OS SDK versions newer than 10.5
	
	// Get a mode which matches the desired mode.
	CGDisplayModeRef newMode = getCGDisplayMode( displayID, desiredMode, true );
	
	// Check to see if a matching mode was found. If not, return failure.
	if ( newMode == NULL )
		return false;
	
	// Change the mode.
	CGError result = CGDisplaySetDisplayMode( displayID, newMode, NULL );
	
	// Release the mode object.
	CGDisplayModeRelease( newMode );
	
	return result == kCGErrorSuccess;
#endif
}




Bool Display:: setBestMode( const DisplayMode& desiredMode )
{
	if ( !this->isValid() )
		return false;
	
#if MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_5
	// The old implementation, for OS SDK versions 10.5 and older.
	
	// Get a mode which matches the desired mode.
	CFDictionaryRef newMode = getCGDisplayMode( displayID, desiredMode, false );
	
	// Check to see if a matching mode was found. If not, return failure.
	if ( newMode == NULL )
		return false;
	
	// Change the mode.
	CGError result = CGDisplaySwitchToMode( displayID, newMode );
	
	return result == kCGErrorSuccess;
#else
	// The current implementation, for OS SDK versions newer than 10.5
	
	// Get a mode which matches the desired mode.
	CGDisplayModeRef newMode = getCGDisplayMode( displayID, desiredMode, false );
	
	// Check to see if a matching mode was found. If not, return failure.
	if ( newMode == NULL )
		return false;
	
	// Change the mode.
	CGError result = CGDisplaySetDisplayMode( displayID, newMode, NULL );
	
	// Release the mode object.
	CGDisplayModeRelease( newMode );
	
	return result == kCGErrorSuccess;
#endif
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
	CGError result = CGDisplayCapture( (CGDirectDisplayID)displayID );
	
	return result == kCGErrorSuccess;
}




void Display:: release()
{
	CGDisplayRelease( (CGDirectDisplayID)displayID );
}




Bool Display:: isCaptured() const
{
	return CGDisplayIsCaptured( (CGDirectDisplayID)displayID );
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
	
	CGRect bounds = CGDisplayBounds( (CGDirectDisplayID)displayID );
	
	return math::Size2D( bounds.size.width, bounds.size.height );
}




Bool Display:: setResolution( const math::Size2D& newResolution )
{
	DisplayMode mode = this->getCurrentMode();
	mode.setResolution( newResolution );
	
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
	if ( !this->isValid() )
		return Double(0);
	
#if MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_5
	// The old implementation, for OS SDK versions 10.5 and older.
	CFDictionaryRef displayMode = CGDisplayCurrentMode( (CGDirectDisplayID)displayID );
	
	if ( displayMode == NULL )
		return Double(0);
	
	// Get the refresh rate.
	const CFNumberRef value = (const CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayRefreshRate );
	Double refreshRate;
	
	if ( !CFNumberGetValue( value, kCFNumberDoubleType, &refreshRate ) )
		return Double(0);
	
	return refreshRate;
#else
	// The current implementation, for OS SDK versions newer than 10.5
	CGDisplayModeRef displayMode = CGDisplayCopyDisplayMode( (CGDirectDisplayID)displayID );
	
	if ( displayMode == NULL )
		return Float(0);
	
	Double refreshRate = CGDisplayModeGetRefreshRate( displayMode );
	
	// Release the temporary display mode.
	CGDisplayModeRelease( displayMode );
	
	return refreshRate;
#endif
}




Bool Display:: setRefreshRate( Double newRefreshRate )
{
	DisplayMode mode = this->getCurrentMode();
	mode.setRefreshRate( newRefreshRate );
	
	return this->setMode( mode );
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
	if ( !this->isValid() )
		return Size(0);
	
#if MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_5
	// The old implementation, for OS SDK versions 10.5 and older.
	CFDictionaryRef displayMode = CGDisplayCurrentMode( (CGDirectDisplayID)displayID );
	
	if ( displayMode == NULL )
		return Size(0);
	
	// Get the bits per pixel
	const CFNumberRef value = (const CFNumberRef)CFDictionaryGetValue( displayMode, kCGDisplayBitsPerPixel );
	Int bitsPerPixel;
	
	if ( !CFNumberGetValue( value, kCFNumberIntType, &bitsPerPixel ) )
		return Size(0);
	
	return Size(bitsPerPixel);
#else
	// The current implementation, for OS SDK versions newer than 10.5
	CGDisplayModeRef mode = CGDisplayCopyDisplayMode( (CGDirectDisplayID)displayID );
	
	if ( mode == NULL )
		return Float(0);
	
	Size bitsPerPixel = getNumberOfBitsForDisplayMode( mode );
	
	// Release the temporary display mode.
	CGDisplayModeRelease( mode );
	
	return bitsPerPixel;
#endif
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
	return displayID.isValid() && CGDisplayIsActive( (CGDirectDisplayID)displayID );
}




Bool Display:: isMain() const
{
	return displayID.isValid() && CGDisplayIsMain( (CGDirectDisplayID)displayID );
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
	// If the device is known to be invalid, return the empty string.
	if ( !this->isValid() )
		return data::UTF8String();
	
	// Determine the port for the connected display.
	io_service_t displayPort = CGDisplayIOServicePort( (CGDirectDisplayID)displayID );
	
	// No port found for that display, return the empty string.
	if ( displayPort == MACH_PORT_NULL )
		return data::UTF8String();
	
	// Get a dictionary of values describing this display.
	CFDictionaryRef deviceInfo = IODisplayCreateInfoDictionary( displayPort, kIODisplayOnlyPreferredName );
	
	// Get the names for the display.
	CFDictionaryRef nameDictionary = (CFDictionaryRef)CFDictionaryGetValue( deviceInfo, CFSTR(kDisplayProductName) );
	Size numNames = 0;
	
	// Check to see if no name was found. If so, return an empty sring.
	if ( nameDictionary == NULL || (numNames = CFDictionaryGetCount(nameDictionary)) == 0 )
		return data::UTF8String();
	
	// Allocate space for all of the names that this display has. (for different localizations)
	const CFTypeRef* names = util::allocate<const CFTypeRef>( numNames );
	
	// Get all of the names for this display.
	CFDictionaryGetKeysAndValues( nameDictionary, NULL, (const void**)names );
	
	// Get the first name.
	const NSString* name = (const NSString*)names[0];
	
	// Convert the name to a string we can use and return it;
	return data::UTF8String( (const UTF8Char*)[name UTF8String] );
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
	uint32_t numDisplays = 0;
	CGError result = CGGetActiveDisplayList( 0, NULL, &numDisplays );
	
	if ( result == kCGErrorSuccess )
		return Size(numDisplays);
	else
		return Size(0);
}




DisplayID Display:: getID( Index displayIndex )
{
	// Determine the currently connected number of active displays.
	uint32_t numDisplays = 0;
	CGError result = CGGetActiveDisplayList( 0, NULL, &numDisplays );
	
	// If the function failed or if the requested display is out of bounds, return an invalid ID.
	if ( result != kCGErrorSuccess || displayIndex >= numDisplays )
		return DisplayID::INVALID;
	
	// Allocate an array of display IDs which will hold all of the connected displays.
	// We could cache this but it would be hard to do if the number of displays changes.
	CGDirectDisplayID* displayIDs = util::allocate<CGDirectDisplayID>( numDisplays );
	
	// Get the display ID for the display at the requested index.
	result = CGGetActiveDisplayList( numDisplays, displayIDs, &numDisplays );
	
	if ( result != kCGErrorSuccess )
	{
		util::deallocate( displayIDs );
		return DisplayID::INVALID;
	}
	
	// Get the ID at the specified index.
	CGDirectDisplayID finalID = displayIDs[displayIndex];
	
	// Clean up the temporary array of display IDs.
	util::deallocate( displayIDs );
	
	return DisplayID( finalID );
}




DisplayID Display:: getMainID()
{
	return DisplayID( CGMainDisplayID() );
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
	
#if MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_5
	// The old implementation, for OS SDK versions 10.5 and older.
	
	// Get an array of all of the valid modes for this display.
	CFArrayRef modeArray = CGDisplayAvailableModes( (CGDirectDisplayID)displayID );
	
	// Check to see if there was an error.
	if ( modeArray == NULL )
		return;
	
	// Get the total number of display modes.
	Size numModes = CFArrayGetCount( modeArray );
	
	// For each mode in the array, create a new DisplayMode object for the mode cache.
	for ( Index i = 0; i < numModes; i++ )
	{
		// Get the mode at this index from the array.
		const CFDictionaryRef mode = (const CFDictionaryRef)CFArrayGetValueAtIndex( modeArray, i );
		
		// Add the new DisplayMode object to the list of cached modes.
		modes.add( getDictionaryMode( mode ) );
	}
#else
	// The current implementation, for OS SDK versions newer than 10.5
	
	// Get an array of all of the valid modes for this display.
	CFArrayRef modeArray = CGDisplayCopyAllDisplayModes( (CGDirectDisplayID)displayID, NULL );
	
	// Check to see if there was an error.
	if ( modeArray == NULL )
		return;
	
	// Get the total number of display modes.
	Size numModes = CFArrayGetCount( modeArray );
	
	// For each mode in the array, create a new DisplayMode object for the mode cache.
	for ( Index i = 0; i < numModes; i++ )
	{
		// Get the mode at this index from the array.
		const CGDisplayModeRef mode = (const CGDisplayModeRef)CFArrayGetValueAtIndex( modeArray, i );
		
		// Get information about the mode.
		math::Size2D size( CGDisplayModeGetWidth(mode), CGDisplayModeGetHeight(mode) );
		Double refreshRate = CGDisplayModeGetRefreshRate( mode );
		Size bitsPerPixel = getNumberOfBitsForDisplayMode( mode );
		
		// Add the new DisplayMode object to the list of cached modes.
		modes.add( DisplayMode( size, refreshRate, bitsPerPixel ) );
	}
	
	// Clean up the array of display modes.
	CFRelease( modeArray );
#endif
}



//##########################################################################################
//**************************     End Rim System Namespace     ******************************
OM_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // #ifdef OM_PLATFORM_APPLE
