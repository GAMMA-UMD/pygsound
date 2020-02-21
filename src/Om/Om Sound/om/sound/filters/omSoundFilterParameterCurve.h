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

#ifndef INCLUDE_OM_SOUND_FILTER_PARAMETER_CURVE_H
#define INCLUDE_OM_SOUND_FILTER_PARAMETER_CURVE_H


#include "omSoundFiltersConfig.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents the scaling curve to use when displaying a SoundFilter parameter.
/**
  * This class allows the user to specify how to display slider values, graphs, etc.
  * Certain types of data are better visualized with a log scale, for instance.
  */
class FilterParameterCurve
{
	public:
		
		//********************************************************************************
		//******	Parameter Type Enum Declaration
			
			
			/// An enum that specifies the different kinds of scaling curves.
			enum Enum
			{
				/// The values are spaced evenly in a linear fashion.
				LINEAR = 0,
				
				/// The value are spaced logarithmically along the number line from the minimum to maximum value.
				LOGARITHMIC = 1,
				
				/// The values are spaced using the function x^2.
				SQUARE = 2,
				
				/// The values are spaced using the function sqrt(x). 
				SQUARE_ROOT = 3,
				
				/// The values are spaced using the function x^3.
				CUBE = 4,
				
				/// The values are spaced using the function x^(1/3).
				CUBE_ROOT = 5
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new filter parameter curve object with the LINEAR parameter curve.
			OM_INLINE FilterParameterCurve()
				:	curve( (UByte)LINEAR )
			{
			}
			
			
			/// Create a new filter parameter curve object with the specified units enum value.
			OM_INLINE FilterParameterCurve( Enum newCurve )
				:	curve( (UByte)newCurve )
			{
			}
			
			
		//********************************************************************************
		//******	Enum Cast Operator
			
			
			/// Convert this filter parameter curve type to an enum value.
			/**
			  * This operator is provided so that the FilterParameterCurve object can be used
			  * directly in a switch statement without the need to explicitly access
			  * the underlying enum value.
			  * 
			  * @return the enum representation of this parameter curve type.
			  */
			OM_INLINE operator Enum () const
			{
				return (Enum)curve;
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the parameter curve type.
			UTF8String toString() const;
			
			
			/// Convert this parameter curve type into a string representation.
			OM_INLINE operator UTF8String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An enum value indicating the scaling curve type of a SoundFilter parameter.
			UByte curve;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_PARAMETER_CURVE_H
