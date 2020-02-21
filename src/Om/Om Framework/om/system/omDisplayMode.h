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

#ifndef INCLUDE_OM_DISPLAY_MODE_H
#define INCLUDE_OM_DISPLAY_MODE_H


#include "omSystemConfig.h"


//##########################################################################################
//**************************     Start Rim System Namespace     ****************************
OM_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that encapsulates a single possible configuration for a system video display.
/**
  * A display mode configuration contains information related to a particular
  * mode of operation for a physical video display. This information includes the 
  * size of the display in pixels (width and height), the refresh rate, and the
  * pixel type (color depth) for the display.
  */
class DisplayMode
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default display mode with (0,0) size and 0 refresh rate.
			OM_INLINE DisplayMode()
				:	resolution( 0, 0 ),
					refreshRate( 0 ),
					bitsPerPixel( 0 )
			{
			}
			
			
			/// Create a new display mode object with the specified size, refresh rate, and bits per pixel.
			OM_INLINE DisplayMode( const math::Size2D& newSize, Double newRefreshRate, Size newBitsPerPixel )
				:	resolution( newSize ),
					refreshRate( newRefreshRate ),
					bitsPerPixel( newBitsPerPixel )
			{
			}
			
			
		//********************************************************************************
		//******	Size Accessor Methods
			
			
			/// Return a 2D vector representing the horizontal and vertical resolution of this display mode in pixels.
			OM_INLINE math::Size2D getResolution() const
			{
				return resolution;
			}
			
			
			/// Set the horizontal and vertical resolution of this display mode in pixels.
			OM_INLINE void setResolution( Size width, Size height )
			{
				resolution.x = width;
				resolution.y = height;
			}
			
			
			/// Set the horizontal and vertical resolution of this display mode in pixels.
			OM_INLINE void setResolution( const math::Size2D& newResolution )
			{
				resolution = newResolution;
			}
			
			
		//********************************************************************************
		//******	Refresh Rate Accessor Methods
			
			
			/// Return the refresh rate of this display mode in cycles per second (hertz).
			OM_INLINE Double getRefreshRate() const
			{
				return refreshRate;
			}
			
			
			/// Set the refresh rate of this display mode in cycles per second (hertz).
			/**
			  * The new refresh rate is clamped to the range of [0,+infinity].
			  */
			OM_INLINE void setRefreshRate( Double newRefreshRate )
			{
				refreshRate = math::max( newRefreshRate, Double(0) );
			}
			
			
		//********************************************************************************
		//******	Bits Per Pixel Accessor Methods
			
			
			/// Return the number of bits used to represent each pixel of this display mode.
			OM_INLINE Size getBitsPerPixel() const
			{
				return bitsPerPixel;
			}
			
			
			/// Set the number of bits used to represent each pixel of this display mode.
			OM_INLINE void setBitsPerPixel( Size newBitsPerPixel )
			{
				bitsPerPixel = newBitsPerPixel;
			}
			
			
		//********************************************************************************
		//******	Comparison Operators
			
			
			/// Return whether or not this display mode is equivalent to another.
			OM_INLINE Bool operator == ( const DisplayMode& other ) const
			{
				return resolution == other.resolution && refreshRate == other.refreshRate &&
						bitsPerPixel == other.bitsPerPixel;
			}
			
			
			/// Return whether or not this display mode is not equivalent to another.
			OM_INLINE Bool operator != ( const DisplayMode& other ) const
			{
				return !(*this == other);
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The horizontal and vertical resolution of this display mode in pixels.
			math::Size2D resolution;
			
			
			/// The vertical refresh rate of this display mode in hertz.
			Double refreshRate;
			
			
			/// The number of bits per pixel of this display mode.
			Size bitsPerPixel;
			
			
};




//##########################################################################################
//**************************     End Rim System Namespace     ******************************
OM_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_DISPLAY_MODE_H
