/*
 *  rimSoundFilterParameterValue.h
 *  Rim Sound
 *
 *  Created by Carl Schissler on 8/21/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_OM_SOUND_FILTER_PARAMETER_VALUE_H
#define INCLUDE_OM_SOUND_FILTER_PARAMETER_VALUE_H


#include "omSoundFiltersConfig.h"


#include "omSoundFilterParameterType.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that holds a union of data representing a filter parameter value.
/**
  * This class is a wrapper around a union value that contains accessor methods
  * and conversion methods for all necessary types.
  * 
  * The user shouldn't have to interact with this class directly - the filter
  * framework should automatically handle value conversions from this class to
  * user types.
  */
class FilterParameterValue
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a filter parameter value with an undefined (unset) value.
			OM_INLINE FilterParameterValue()
			{
			}
			
			
			/// Create a new filter parameter value with the specified boolean value.
			OM_INLINE FilterParameterValue( Bool newBoolean )
				:	booleanValue( newBoolean )
			{
			}
			
			
			/// Create a new filter parameter value with the specified integer value.
			/**
			  * This constructor works for parameters with INTEGER or ENUMERATION
			  * types.
			  */
			OM_INLINE FilterParameterValue( Int64 newInteger )
				:	integerValue( newInteger )
			{
			}
			
			
			/// Create a new filter parameter value with the specified float value.
			OM_INLINE FilterParameterValue( Float32 newFloat )
				:	floatValue( newFloat )
			{
			}
			
			
			/// Create a new filter parameter value with the specified double value.
			OM_INLINE FilterParameterValue( Float64 newDouble )
				:	doubleValue( newDouble )
			{
			}
			
			
		//********************************************************************************
		//******	Value Read Methods
			
			
			/// Interpret this value as the specified type and convert it to a boolean value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValueAsType( FilterParameterType type, Bool& output ) const
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						output = (booleanValue != UInt64(0));
						return true;
					case FilterParameterType::INTEGER:
						output = (integerValue != Int64(0));
						return true;
					case FilterParameterType::FLOAT:
						output = (floatValue != Float32(0));
						return true;
					case FilterParameterType::DOUBLE:
						output = (doubleValue != Float64(0));
						return true;
					default:
						return false;
				}
			}
			
			
			/// Interpret this value as the specified type and convert it to an integer value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValueAsType( FilterParameterType type, Int64& output ) const
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						output = (Int64)booleanValue;
						return true;
					case FilterParameterType::INTEGER:
						output = integerValue;
						return true;
					case FilterParameterType::ENUMERATION:
						output = enumValue;
						return true;
					case FilterParameterType::FLOAT:
						output = (Int64)floatValue;
						return true;
					case FilterParameterType::DOUBLE:
						output = (Int64)doubleValue;
						return true;
					default:
						return false;
				}
			}
			
			
			/// Interpret this value as the specified type and convert it to a float value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValueAsType( FilterParameterType type, Float32& output ) const
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						output = (Float32)booleanValue;
						return true;
					case FilterParameterType::INTEGER:
						output = (Float32)integerValue;
						return true;
					case FilterParameterType::FLOAT:
						output = floatValue;
						return true;
					case FilterParameterType::DOUBLE:
						output = (Float32)doubleValue;
						return true;
					default:
						return false;
				}
			}
			
			
			/// Interpret this value as the specified type and convert it to a double value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool getValueAsType( FilterParameterType type, Float64& output ) const
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						output = (Float64)booleanValue;
						return true;
					case FilterParameterType::INTEGER:
						output = (Float64)integerValue;
						return true;
					case FilterParameterType::FLOAT:
						output = (Float64)floatValue;
						return true;
					case FilterParameterType::DOUBLE:
						output = doubleValue;
						return true;
					default:
						return false;
				}
			}
			
			
		//********************************************************************************
		//******	Value Write Methods
			
			
			/// Interpret this value as the specified type and set it to a boolean value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool setValueAsType( FilterParameterType type, Bool newValue )
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						booleanValue = newValue;
						return true;
					case FilterParameterType::INTEGER:
						integerValue = (Int64)newValue;
						return true;
					case FilterParameterType::FLOAT:
						floatValue = (Float32)newValue;
						return true;
					case FilterParameterType::DOUBLE:
						doubleValue = (Float64)newValue;
						return true;
					default:
						return false;
				}
			}
			
			
			/// Interpret this value as the specified type and set it to an integer value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool setValueAsType( FilterParameterType type, Int64 newValue )
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						booleanValue = (newValue != Int64(0));
						return true;
					case FilterParameterType::INTEGER:
						integerValue = (Int64)newValue;
						return true;
					case FilterParameterType::ENUMERATION:
						enumValue = (Int64)newValue;
						return true;
					case FilterParameterType::FLOAT:
						floatValue = (Float32)newValue;
						return true;
					case FilterParameterType::DOUBLE:
						doubleValue = (Float64)newValue;
						return true;
					default:
						return false;
				}
			}
			
			
			/// Interpret this value as the specified type and set it to a float value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool setValueAsType( FilterParameterType type, Float32 newValue )
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						booleanValue = (newValue != Float32(0));
						return true;
					case FilterParameterType::INTEGER:
						integerValue = (Int64)newValue;
						return true;
					case FilterParameterType::FLOAT:
						floatValue = newValue;
						return true;
					case FilterParameterType::DOUBLE:
						doubleValue = (Float64)newValue;
						return true;
					default:
						return false;
				}
			}
			
			
			/// Interpret this value as the specified type and set it to a double value.
			/**
			  * If the conversion succeeds, TRUE is returned. Otherwise, if the conversion fails,
			  * FALSE is returned and no output is converted.
			  */
			OM_INLINE Bool setValueAsType( FilterParameterType type, Float64 newValue )
			{
				switch ( type )
				{
					case FilterParameterType::BOOLEAN:
						booleanValue = (newValue != Float64(0));
						return true;
					case FilterParameterType::INTEGER:
						integerValue = (Int64)newValue;
						return true;
					case FilterParameterType::FLOAT:
						floatValue = (Float32)newValue;
						return true;
					case FilterParameterType::DOUBLE:
						doubleValue = newValue;
						return true;
					default:
						return false;
				}
			}
			
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A union which contains the possible storage types for a parameter value.
			union
			{
				UInt64 booleanValue;
				Int64 integerValue;
				Int64 enumValue;
				Float32 floatValue;
				Float64 doubleValue;
			};
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_FILTER_PARAMETER_VALUE_H
