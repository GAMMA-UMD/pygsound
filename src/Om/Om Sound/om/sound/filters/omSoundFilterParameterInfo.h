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

#ifndef INCLUDE_OM_SOUND_FILTER_PARAMETER_INFO_H
#define INCLUDE_OM_SOUND_FILTER_PARAMETER_INFO_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilterParameterType.h"
#include "omSoundFilterParameterValue.h"
#include "omSoundFilterParameterUnits.h"
#include "omSoundFilterParameterCurve.h"
#include "omSoundFilterParameterFlags.h"
#include "omSoundFilterParameter.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents information about a particular SoundFilter parameter.
class FilterParameterInfo
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an uninitialized filter parameter information class.
			/**
			  * The information stored in this class does not represent any valid
			  * parameter information.
			  */
			OM_INLINE FilterParameterInfo()
				:	index( 0 )
			{
			}
			
			
			/// Create a new parameter information object with the specified attributes.
			OM_INLINE FilterParameterInfo( UInt32 newIndex, const UTF8String& newName, FilterParameterType newType,
								FilterParameterUnits newUnits, FilterParameterCurve newCurve,
								FilterParameterValue newMinimum, FilterParameterValue newMaximum,
								FilterParameterValue newDefault, FilterParameterFlags newFlags )
				:	index( newIndex ),
					name( newName ),
					minimum( newMinimum ),
					maximum( newMaximum ),
					defaultValue( newDefault ),
					type( newType ),
					units( newUnits ),
					curve( newCurve ),
					flags( newFlags )
			{
			}
			
			
		//********************************************************************************
		//******	Parameter Index Accessor Methods
			
			
			/// Return the index of this parameter within its host SoundFilter.
			OM_INLINE UInt32 getIndex() const
			{
				return index;
			}
			
			
		//********************************************************************************
		//******	Parameter Name Accessor Methods
			
			
			/// Return a reference to a human-readable name string for this parameter.
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
		//********************************************************************************
		//******	Parameter Type Accessor Methods
			
			
			/// Return an object indicating the actual type of this filter parameter.
			OM_INLINE FilterParameterType getType() const
			{
				return type;
			}
			
			
		//********************************************************************************
		//******	Parameter Units Type Accessor Methods
			
			
			/// Return an object indicating the units of this filter parameter.
			OM_INLINE FilterParameterUnits getUnits() const
			{
				return units;
			}
			
			
		//********************************************************************************
		//******	Parameter Curve Type Accessor Methods
			
			
			/// Return an object indicating the display curve of this filter parameter.
			OM_INLINE FilterParameterCurve getCurve() const
			{
				return curve;
			}
			
			
		//********************************************************************************
		//******	Parameter Flags Accessor Methods
			
			
			/// Return an object indicating boolean attributes of this filter parameter.
			OM_INLINE FilterParameterFlags getFlags() const
			{
				return flags;
			}
			
			
		//********************************************************************************
		//******	Parameter Minimum Value Accessor Methods
			
			
			/// Query a boolean minimum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of BOOLEAN, the method returns
			  * TRUE and the minimum boolean value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no minimum
			  * value is set.
			  */
			OM_INLINE Bool getMinimum( Bool& booleanValue ) const
			{
				return minimum.getValueAsType( type, booleanValue );
			}
			
			
			/// Query an integer or enumeration minimum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of INTEGER or ENUMERATION,
			  * the method returns TRUE and the minimum integer value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no minimum
			  * value is set.
			  */
			OM_INLINE Bool getMinimum( Int64& integerValue ) const
			{
				return minimum.getValueAsType( type, integerValue );
			}
			
			
			/// Query a float minimum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of FLOAT, the method returns
			  * TRUE and the minimum float value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no minimum
			  * value is set.
			  */
			OM_INLINE Bool getMinimum( Float32& floatValue ) const
			{
				return minimum.getValueAsType( type, floatValue );
			}
			
			
			/// Query a double minimum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of DOUBLE, the method returns
			  * TRUE and the minimum double value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no minimum
			  * value is set.
			  */
			OM_INLINE Bool getMinimum( Float64& doubleValue ) const
			{
				return minimum.getValueAsType( type, doubleValue );
			}
			
			
			/// Return a generic-typed minimum value for this parameter.
			OM_INLINE FilterParameter getMinimum() const
			{
				return FilterParameter( type, minimum );
			}
			
			
		//********************************************************************************
		//******	Parameter Maximum Value Accessor Methods
			
			
			/// Query a boolean maximum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of BOOLEAN, the method returns
			  * TRUE and the maximum boolean value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no maximum
			  * value is set.
			  */
			OM_INLINE Bool getMaximum( Bool& booleanValue ) const
			{
				return maximum.getValueAsType( type, booleanValue );
			}
			
			
			/// Query an integer or enumeration maximum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of INTEGER or ENUMERATION, the method returns
			  * TRUE and the maximum integer value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no maximum
			  * value is set.
			  */
			OM_INLINE Bool getMaximum( Int64& integerValue ) const
			{
				return maximum.getValueAsType( type, integerValue );
			}
			
			
			/// Query a float maximum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of FLOAT, the method returns
			  * TRUE and the maximum float value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no maximum
			  * value is set.
			  */
			OM_INLINE Bool getMaximum( Float32& floatValue ) const
			{
				return maximum.getValueAsType( type, floatValue );
			}
			
			
			/// Query a double maximum value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of DOUBLE, the method returns
			  * TRUE and the maximum double value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no maximum
			  * value is set.
			  */
			OM_INLINE Bool getMaximum( Float64& doubleValue ) const
			{
				return maximum.getValueAsType( type, doubleValue );
			}
			
			
			/// Return a generic-typed maximum value for this parameter.
			OM_INLINE FilterParameter getMaximum() const
			{
				return FilterParameter( type, maximum );
			}
			
			
		//********************************************************************************
		//******	Parameter Default Value Accessor Methods
			
			
			/// Query a boolean default value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of BOOLEAN, the method returns
			  * TRUE and the default boolean value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no default
			  * value is set.
			  */
			OM_INLINE Bool getDefault( Bool& booleanValue ) const
			{
				return defaultValue.getValueAsType( type, booleanValue );
			}
			
			
			/// Query an integer or enumeration default value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of INTEGER or ENUMERATION, the method returns
			  * TRUE and the default integer value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no default
			  * value is set.
			  */
			OM_INLINE Bool getDefault( Int64& integerValue ) const
			{
				return defaultValue.getValueAsType( type, integerValue );
			}
			
			
			/// Query a float default value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of FLOAT, the method returns
			  * TRUE and the default float value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no default
			  * value is set.
			  */
			OM_INLINE Bool getDefault( Float32& floatValue ) const
			{
				return defaultValue.getValueAsType( type, floatValue );
			}
			
			
			/// Query a double default value of this filter parameter.
			/**
			  * If this parameter can be converted to a type of DOUBLE, the method returns
			  * TRUE and the default double value for this parameter is placed in
			  * the output parameter. Otherwise, FALSE is returned and no default
			  * value is set.
			  */
			OM_INLINE Bool getDefault( Float64& doubleValue ) const
			{
				return defaultValue.getValueAsType( type, doubleValue );
			}
			
			
			/// Return a generic-typed default value for this parameter.
			OM_INLINE FilterParameter getDefault() const
			{
				return FilterParameter( type, defaultValue );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The index of this parameter within its host SoundFilter.
			UInt32 index;
			
			
			/// A human-readable name for this filter parameter.
			UTF8String name;
			
			
			/// The minimum allowed value of this filter parameter.
			FilterParameterValue minimum;
			
			
			/// The maximum allowed value of this filter parameter.
			FilterParameterValue maximum;
			
			
			/// The default value of this filter parameter.
			FilterParameterValue defaultValue;
			
			
			/// An object representing the type of this filter parameter.
			FilterParameterType type;
			
			
			/// An object that declares the unit type of this filter parameter.
			FilterParameterUnits units;
			
			
			/// An object that declares the curve type of this filter parameter.
			FilterParameterCurve curve;
			
			
			/// An object that encapsulates boolean flags for this filter parameter.
			FilterParameterFlags flags;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_PARAMETER_INFO_H
