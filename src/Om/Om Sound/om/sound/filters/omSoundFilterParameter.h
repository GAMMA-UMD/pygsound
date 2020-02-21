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

#ifndef INCLUDE_OM_SOUND_FILTER_PARAMETER_H
#define INCLUDE_OM_SOUND_FILTER_PARAMETER_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilterParameterValue.h"
#include "omSoundFilterParameterType.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that holds the type and value of a SoundFilter parameter.
/**
  * The class provides ways to access the value of the generic parameter in a 
  * type-safe manner.
  */
class FilterParameter
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a filter parameter with an undefined type and value.
			OM_INLINE FilterParameter()
				:	 type( FilterParameterType::UNDEFINED )
			{
			}
			
			
			/// Create a new filter parameter with the specified boolean value.
			OM_INLINE FilterParameter( Bool newBoolean )
				:	value( newBoolean ),
					type( FilterParameterType::BOOLEAN )
			{
			}
			
			
			/// Create a new filter parameter with the specified integer or enumeration value.
			OM_INLINE FilterParameter( Int64 newInteger )
				:	value( newInteger ),
					type( FilterParameterType::INTEGER )
			{
			}
			
			
			/// Create a new filter parameter with the specified float value.
			OM_INLINE FilterParameter( Float32 newFloat )
				:	value( newFloat ),
					type( FilterParameterType::FLOAT )
			{
			}
			
			
			/// Create a new filter parameter with the specified double value.
			OM_INLINE FilterParameter( Float64 newDouble )
				:	value( newDouble ),
					type( FilterParameterType::DOUBLE )
			{
			}
			
			
			/// Create a new filter parameter with the specified type and generic value union.
			OM_INLINE FilterParameter( FilterParameterType newType, const FilterParameterValue& newValue )
				:	value( newValue ),
					type( newType )
			{
			}
			
			
		//********************************************************************************
		//******	Parameter Type Accessor Methods
			
			
			/// Return the actual type of this filter parameter.
			OM_INLINE FilterParameterType getType() const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	Value Read Methods
			
			
			/// Read this filter parameter as a boolean value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValue( Bool& output ) const
			{
				return value.getValueAsType( type, output );
			}
			
			
			/// Read this filter parameter as an integer value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValue( Int64& output ) const
			{
				return value.getValueAsType( type, output );
			}
			
			
			/// Read this filter parameter as a float value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValue( Float32& output ) const
			{
				return value.getValueAsType( type, output );
			}
			
			
			/// Read this filter parameter as a double value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValue( Float64& output ) const
			{
				return value.getValueAsType( type, output );
			}
			
			
		//********************************************************************************
		//******	Value Write Methods
			
			
			/// Write this filter parameter as a boolean value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no value is set.
			  */
			OM_INLINE Bool setValue( Bool output )
			{
				return value.setValueAsType( type, output );
			}
			
			
			/// Write this filter parameter as an integer or enumeration value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no value is set.
			  */
			OM_INLINE Bool setValue( Int64 output )
			{
				return value.setValueAsType( type, output );
			}
			
			
			/// Write this filter parameter as a float value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no value is set.
			  */
			OM_INLINE Bool setValue( Float32 output )
			{
				return value.setValueAsType( type, output );
			}
			
			
			/// Write this filter parameter as a double value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no value is set.
			  */
			OM_INLINE Bool setValue( Float64 output )
			{
				return value.setValueAsType( type, output );
			}
			
			
		//********************************************************************************
		//******	Value String Representation Accessor Methods
			
			
			/// Return a string representation of this parameter's value.
			UTF8String toString() const;
			
			
			/// Return a string representation of this parameter's value.
			OM_INLINE operator UTF8String () const
			{
				return this->toString();
			}
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that stores the data representation of this parameter.
			FilterParameterValue value;
			
			
			/// The actual type of this filter parameter.
			FilterParameterType type;
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_PARAMETER_H
