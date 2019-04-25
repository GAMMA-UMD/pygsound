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

#ifndef INCLUDE_OM_SOUND_TIME_SIGNATURE_H
#define INCLUDE_OM_SOUND_TIME_SIGNATURE_H


#include "omSoundBaseConfig.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a standard musical time signature.
/**
  * The numberator of the time signature indicates the number of beats per measure,
  * and the denominator indicates what note value represents a beat. The note value
  * is usually a power of 2: 1, 2, 4, 8, 16, 32, etc.
  */
class TimeSignature
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new default time signature representing 4/4 time.
			OM_INLINE TimeSignature()
				:	numerator( 4 ),
					denominator( 4 )
			{
			}
			
			
			/// Create a new time signature with the specified time signature numberator and denominator.
			OM_INLINE TimeSignature( UInt newNumerator, UInt newDenominator )
				:	numerator( newNumerator ),
					denominator( newDenominator )
			{
			}
			
			
		//********************************************************************************
		//******	Numerator and Denominator Accessor Methods
			
			
			/// Return the numerator value of this time signature, indicating the number of beats per measure.
			OM_INLINE UInt getNumerator() const
			{
				return numerator;
			}
			
			
			/// Set the numerator value of this time signature, indicating the number of beats per measure.
			OM_INLINE void setNumerator( UInt newNumerator )
			{
				numerator = UInt8(newNumerator);
			}
			
			
			/// Return the denominator value of this time signature, indicating which note value gets a beat.
			/**
			  * The note value is almost always a power of 2: 1, 2, 4, 8, 16, 32, etc.
			  */
			OM_INLINE UInt getDenominator() const
			{
				return denominator;
			}
			
			
			/// Set the denominator value of this time signature, indicating which note value gets a beat.
			/**
			  * The note value is almost always a power of 2: 1, 2, 4, 8, 16, 32, etc.
			  */
			OM_INLINE void setDenominator( UInt newDenominator )
			{
				denominator = UInt8(newDenominator);
			}
			
			
		//********************************************************************************
		//******	String Representation Accessor Methods
			
			
			/// Return a string representation of the time signature, 'N/D'
			data::String toString() const
			{
				return String(numerator) + '/' + String(denominator);
			}
			
			
			/// Convert this time signature into a string representation, 'N/D'.
			OM_INLINE operator data::String () const
			{
				return this->toString();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The number of beats per measure of this time signature.
			UInt8 numerator;
			
			
			/// The note type that represents a beat, usually a power of two: 1, 2, 4, 8, 16, 32.
			UInt8 denominator;
			
			
			
};




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_TIME_SIGNATURE_H
