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

#ifndef INCLUDE_OM_SOUND_PAN_DIRECTION_H
#define INCLUDE_OM_SOUND_PAN_DIRECTION_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a generic panning direction.
/**
  * This class contains a 3D vector representing the panning direction, a
  * 'directivity' value that indicates how localized the panning direction
  * is, and a 'spread' value that indicates how spread out the panned sound field is.
  *
  * The panning direction uses coordinates where the -z axis is the forward direction,
  * the x axis points the right, and the y axis points up. If 2D pan directions are used,
  * the direction is specified as a 2D vector defined as (X,Z) and contains
  * the X and Z components of a 3D direction.
  *
  * The panning directivity ranges from 0 to 1. A value of 0 indicates that the original
  * channel layout for the source audio is kept intact and the channels are merely rotated
  * around the listener by the pannning direction. A value of 1 indicates that all source
  * audio channels are to be clustered in the panning direction. Values inbetween 0 and 1
  * cause the source channels to cluster closer towards the panning direction the closer
  * the directivity gets to 1.
  *
  * The panning spread ranges from 0 to 1 and indicates how far each source audio channel
  * spreads out around the panning sphere. A value of 0 indicates that the sound
  * from each source channel should be as localized as possible, while a value of 1
  * indicates that the source channels should be spread up to +-180 degrees from the actual
  * source channel position.
  */
class PanDirection
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a panning which has a directivity and spread of 0 and direction (0,0,-1), straight ahead.
			OM_INLINE PanDirection()
				:	direction( 0, 0, -1 ),
					directivity( 0 ),
					spread( 0 )
			{
			}
			
			
			/// Create a new panning direction with the specified 2D direction vector, directivity, and spread factor.
			/**
			  * The 2D vector is specified where X+ = right and Y- = front. This corresponds to the XZ coordinates
			  * of a 3D right-handed coordinate system, projected into the listener's local horizontal plane.
			  */
			OM_INLINE PanDirection( const Vector2f& newDirection, Float newDirectivity = Float(0), Float newSpread = Float(0) )
				:	direction( newDirection.x, 0, newDirection.y ),
					directivity( math::clamp( newDirectivity, Float(0), Float(1) ) ),
					spread( math::clamp( newSpread, Float(0), Float(1) ) )
			{
			}
			
			
			/// Create a new panning direction with the specified 3D direction vector, directivity, and spread factor.
			/**
			  * The 3D vector is specified where X+ = right and Y+ = up and Z- = front. This corresponds to the XYZ coordinates
			  * of a 3D right-handed coordinate system, projected into the listener's local horizontal plane.
			  */
			OM_INLINE PanDirection( const Vector3f& newDirection, Float newDirectivity = Float(0), Float newSpread = Float(0) )
				:	direction( newDirection ),
					directivity( math::clamp( newDirectivity, Float(0), Float(1) ) ),
					spread( math::clamp( newSpread, Float(0), Float(1) ) )
			{
			}
			
			
		//********************************************************************************
		//******	Direction Accessor Methods
			
			
			/// Return a reference to the 3D direction vector for this panning direction object.
			OM_INLINE const Vector3f& getDirection() const
			{
				return direction;
			}
			
			
			/// Set the 2D direction vector for this panning direction object.
			/**
			  * The 2D vector is specified where X+ = right and Y- = front. This corresponds to the XZ coordinates
			  * of a 3D right-handed coordinate system, projected into the listener's local horizontal plane.
			  */
			OM_INLINE void setDirection( const Vector2f& newDirection )
			{
				direction = Vector3f( newDirection.x, Float(0), newDirection.y );
			}
			
			
			/// Set the 3D direction vector for this panning direction object.
			/**
			  * The 3D vector is specified where X+ = right and Y+ = up and Z- = front. This corresponds to the XYZ coordinates
			  * of a 3D right-handed coordinate system, projected into the listener's local horizontal plane.
			  */
			OM_INLINE void setDirection( const Vector3f& newDirection )
			{
				direction = newDirection;
			}
			
			
		//********************************************************************************
		//******	Directionality Accessor Methods
			
			
			/// Return the directivity for this panning direction object.
			OM_INLINE Float getDirectivity() const
			{
				return directivity;
			}
			
			
			/// Set the directivity for this panning direction object.
			/**
			  * The new directivity value is clamped to the range [0,1].
			  */
			OM_INLINE void setDirectivity( Float newDirectivity )
			{
				directivity = math::clamp( newDirectivity, Float(0), Float(1) );
			}
			
			
		//********************************************************************************
		//******	Spread Accessor Methods
			
			
			/// Return the spread for this panning direction object.
			OM_INLINE Float getSpread() const
			{
				return spread;
			}
			
			
			/// Set the spread for this panning direction object.
			/**
			  * The new spread value is clamped to the range [0,1].
			  */
			OM_INLINE void setSpread( Float newSpread )
			{
				spread = math::clamp( newSpread, Float(0), Float(1) );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A 3D vector indicating the direction in which the sound is panned.
			Vector3f direction;
			
			
			/// A value ranging from 0 to 1 indicating how directional the panned sound should be.
			Float directivity;
			
			
			/// A value ranging from 0 to 1 indicating how spread out the panned sound should be.
			Float spread;
			
			
			
};





//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_PAN_DIRECTION_H
