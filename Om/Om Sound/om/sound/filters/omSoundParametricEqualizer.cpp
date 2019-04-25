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

#include "omSoundParametricEqualizer.h"


//##########################################################################################
//##########################################################################################
//############		
//############		Parameter Index Declarations
//############		
//##########################################################################################
//##########################################################################################


#define PARAMETER_INDEX_OUTPUT_GAIN 0

#define PARAMETER_INDEX_HIGH_PASS_ENABLED 1
#define PARAMETER_INDEX_HIGH_PASS_FREQUENCY 2
#define PARAMETER_INDEX_HIGH_PASS_ORDER 3

#define PARAMETER_INDEX_LOW_SHELF_ENABLED 4
#define PARAMETER_INDEX_LOW_SHELF_FREQUENCY 5
#define PARAMETER_INDEX_LOW_SHELF_SLOPE 6
#define PARAMETER_INDEX_LOW_SHELF_GAIN 7

#define PARAMETER_INDEX_PARAMETRIC_1_ENABLED 8
#define PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY 9
#define PARAMETER_INDEX_PARAMETRIC_1_Q 10
#define PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH 11
#define PARAMETER_INDEX_PARAMETRIC_1_GAIN 12

#define PARAMETER_INDEX_HIGH_SHELF_ENABLED 33
#define PARAMETER_INDEX_HIGH_SHELF_FREQUENCY 34
#define PARAMETER_INDEX_HIGH_SHELF_SLOPE 35
#define PARAMETER_INDEX_HIGH_SHELF_GAIN 36

#define PARAMETER_INDEX_LOW_PASS_ENABLED 37
#define PARAMETER_INDEX_LOW_PASS_FREQUENCY 38
#define PARAMETER_INDEX_LOW_PASS_ORDER 39

#define PARAMETER_COUNT 40


//##########################################################################################
//##########################################################################################
//############		
//############		Parameter Name Declarations
//############		
//##########################################################################################
//##########################################################################################


#define PARAMETER_NAME_OUTPUT_GAIN "Output Gain"

#define PARAMETER_NAME_HIGH_PASS_ENABLED "High Pass Enabled"
#define PARAMETER_NAME_HIGH_PASS_FREQUENCY "High Pass Frequency"
#define PARAMETER_NAME_HIGH_PASS_ORDER "High Pass Order"

#define PARAMETER_NAME_LOW_SHELF_ENABLED "Low Shelf Enabled"
#define PARAMETER_NAME_LOW_SHELF_FREQUENCY "Low Shelf Frequency"
#define PARAMETER_NAME_LOW_SHELF_SLOPE "Low Shelf Slope"
#define PARAMETER_NAME_LOW_SHELF_GAIN "Low Shelf Gain"

#define PARAMETRIC_PARAMETER_COUNT 5

#define PARAMETER_NAME_HIGH_SHELF_ENABLED "High Shelf Enabled"
#define PARAMETER_NAME_HIGH_SHELF_FREQUENCY "High Shelf Frequency"
#define PARAMETER_NAME_HIGH_SHELF_SLOPE "High Shelf Slope"
#define PARAMETER_NAME_HIGH_SHELF_GAIN "High Shelf Gain"

#define PARAMETER_NAME_LOW_PASS_ENABLED "Low Pass Enabled"
#define PARAMETER_NAME_LOW_PASS_FREQUENCY "Low Pass Frequency"
#define PARAMETER_NAME_LOW_PASS_ORDER "Low Pass Order"



//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String ParametricEqualizer:: NAME( "Parametric Equalizer" );
const UTF8String ParametricEqualizer:: MANUFACTURER( "Om Sound" );
const FilterVersion ParametricEqualizer:: VERSION( 1, 0, 0 );


const Float ParametricEqualizer:: DEFAULT_PARAMETRIC_FREQUENCIES[DEFAULT_NUMBER_OF_PARAMETRIC_FILTERS] = {
	200.0f,
	500.0f,
	1000.0f,
	3000.0f,
	8000.0f
};


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




ParametricEqualizer:: ParametricEqualizer()
	:	highPass( CutoffFilter::BUTTERWORTH, CutoffFilter::HIGH_PASS, 2, 80 ),
		lowPass( CutoffFilter::BUTTERWORTH, CutoffFilter::LOW_PASS, 2, 20000 ),
		lowShelf( ShelfFilter::LOW_SHELF, 120, 1.0 ),
		highShelf( ShelfFilter::HIGH_SHELF, 8000, 1.0 ),
		parametrics( DEFAULT_NUMBER_OF_PARAMETRIC_FILTERS ),
		highPassEnabled( false ),
		lowPassEnabled( false ),
		lowShelfEnabled( true ),
		highShelfEnabled( true )
{
	// Initialize parametric bands.
	for ( Index i = 0; i < parametrics.getSize(); i++ )
	{
		parametrics[i].filter.setFrequency( DEFAULT_PARAMETRIC_FREQUENCIES[i] );
		parametrics[i].filter.setIsSynchronized( false );
	}
	
	// Disable thread synchronization for internal filters.
	highPass.setIsSynchronized( false );
	lowPass.setIsSynchronized( false );
	lowShelf.setIsSynchronized( false );
	highShelf.setIsSynchronized( false );
	gainFilter.setIsSynchronized( false );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String ParametricEqualizer:: getName() const
{
	return NAME;
}




UTF8String ParametricEqualizer:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion ParametricEqualizer:: getVersion() const
{
	return VERSION;
}




FilterCategory ParametricEqualizer:: getCategory() const
{
	return FilterCategory::EQUALIZER;
}




Bool ParametricEqualizer:: allowsInPlaceProcessing() const
{
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Size ParametricEqualizer:: getParameterCount() const
{
	return PARAMETER_COUNT;
}




Bool ParametricEqualizer:: getParameterInfo( Index parameterIndex, FilterParameterInfo& info ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_OUTPUT_GAIN, PARAMETER_NAME_OUTPUT_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 20.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_HIGH_PASS_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_PASS_ENABLED, PARAMETER_NAME_HIGH_PASS_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_PASS_FREQUENCY, PARAMETER_NAME_HIGH_PASS_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 80.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_ORDER:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_PASS_ORDER, PARAMETER_NAME_HIGH_PASS_ORDER,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										(Int64)1, (Int64)10, (Int64)1,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_LOW_SHELF_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_SHELF_ENABLED, PARAMETER_NAME_LOW_SHELF_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_SHELF_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_SHELF_FREQUENCY, PARAMETER_NAME_LOW_SHELF_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 120.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_SHELF_SLOPE:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_SHELF_SLOPE, PARAMETER_NAME_LOW_SHELF_SLOPE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LOGARITHMIC,
										0.1f, 10.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_SHELF_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_SHELF_GAIN, PARAMETER_NAME_LOW_SHELF_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 20.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_ENABLED) / PARAMETRIC_PARAMETER_COUNT;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Parametric ") + (parametricIndex + 1) + " Enabled",
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY) / PARAMETRIC_PARAMETER_COUNT;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Parametric ") + (parametricIndex + 1) + " Frequency",
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, DEFAULT_PARAMETRIC_FREQUENCIES[parametricIndex],
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_Q:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_Q) / PARAMETRIC_PARAMETER_COUNT;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Parametric ") + (parametricIndex + 1) + " Q",
										FilterParameterType::FLOAT,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::SQUARE,
										0.05f, 30.0f, 1.414f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH) / PARAMETRIC_PARAMETER_COUNT;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Parametric ") + (parametricIndex + 1) + " Bandwidth",
										FilterParameterType::FLOAT,
										FilterParameterUnits::OCTAVES, FilterParameterCurve::SQUARE,
										0.05f, 8.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_GAIN) / PARAMETRIC_PARAMETER_COUNT;
			
			info = FilterParameterInfo( (UInt32)parameterIndex, UTF8String("Parametric ") + (parametricIndex + 1) + " Gain",
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 20.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		}
		//*******************************************************************************
		case PARAMETER_INDEX_HIGH_SHELF_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_SHELF_ENABLED, PARAMETER_NAME_HIGH_SHELF_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, true,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_SHELF_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_SHELF_FREQUENCY, PARAMETER_NAME_HIGH_SHELF_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 120.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_SHELF_SLOPE:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_SHELF_SLOPE, PARAMETER_NAME_HIGH_SHELF_SLOPE,
										FilterParameterType::FLOAT,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LOGARITHMIC,
										0.1f, 10.0f, 1.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_HIGH_SHELF_GAIN:
			info = FilterParameterInfo( PARAMETER_INDEX_HIGH_SHELF_GAIN, PARAMETER_NAME_HIGH_SHELF_GAIN,
										FilterParameterType::FLOAT,
										FilterParameterUnits::DECIBELS, FilterParameterCurve::LINEAR,
										-20.0f, 20.0f, 0.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_LOW_PASS_ENABLED:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_ENABLED, PARAMETER_NAME_LOW_PASS_ENABLED,
										FilterParameterType::BOOLEAN,
										FilterParameterUnits::UNDEFINED, FilterParameterCurve::LINEAR,
										false, true, false,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FREQUENCY:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_FREQUENCY, PARAMETER_NAME_LOW_PASS_FREQUENCY,
										FilterParameterType::FLOAT,
										FilterParameterUnits::HERTZ, FilterParameterCurve::LOGARITHMIC,
										20.0f, 20000.0f, 20000.0f,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
		case PARAMETER_INDEX_LOW_PASS_ORDER:
			info = FilterParameterInfo( PARAMETER_INDEX_LOW_PASS_ORDER, PARAMETER_NAME_LOW_PASS_ORDER,
										FilterParameterType::INTEGER,
										FilterParameterUnits::INDEX, FilterParameterCurve::LINEAR,
										(Int64)1, (Int64)10, (Int64)1,
										FilterParameterFlags::READ_ACCESS | FilterParameterFlags::WRITE_ACCESS );
			return true;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Parameter Value Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool ParametricEqualizer:: getParameterValue( Index parameterIndex, FilterParameter& value ) const
{
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_GAIN:
			value = FilterParameter( this->getOutputGainDB() );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_HIGH_PASS_ENABLED:
			value = FilterParameter( this->getHighPassIsEnabled() );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_FREQUENCY:
			value = FilterParameter( this->getHighPassFrequency() );
			return true;
		case PARAMETER_INDEX_HIGH_PASS_ORDER:
			value = FilterParameter( (Int64)this->getHighPassOrder() );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_LOW_SHELF_ENABLED:
			value = FilterParameter( this->getLowShelfIsEnabled() );
			return true;
		case PARAMETER_INDEX_LOW_SHELF_FREQUENCY:
			value = FilterParameter( this->getLowShelfFrequency() );
			return true;
		case PARAMETER_INDEX_LOW_SHELF_SLOPE:
			value = FilterParameter( this->getLowShelfSlope() );
			return true;
		case PARAMETER_INDEX_LOW_SHELF_GAIN:
			value = FilterParameter( this->getLowShelfGainDB() );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_ENABLED) / PARAMETRIC_PARAMETER_COUNT;
			
			value = FilterParameter( this->getParametricIsEnabled( parametricIndex ) );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY) / PARAMETRIC_PARAMETER_COUNT;
			
			value = FilterParameter( this->getParametricFrequency( parametricIndex ) );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_Q:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_Q) / PARAMETRIC_PARAMETER_COUNT;
			
			value = FilterParameter( this->getParametricQ( parametricIndex ) );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH) / PARAMETRIC_PARAMETER_COUNT;
			
			value = FilterParameter( this->getParametricBandwidth( parametricIndex ) );
			return true;
		}
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_GAIN) / PARAMETRIC_PARAMETER_COUNT;
			
			value = FilterParameter( this->getParametricGainDB( parametricIndex ) );
			return true;
		}
		//*******************************************************************************
		case PARAMETER_INDEX_HIGH_SHELF_ENABLED:
			value = FilterParameter( this->getHighShelfIsEnabled() );
			return true;
		case PARAMETER_INDEX_HIGH_SHELF_FREQUENCY:
			value = FilterParameter( this->getHighShelfFrequency() );
			return true;
		case PARAMETER_INDEX_HIGH_SHELF_SLOPE:
			value = FilterParameter( this->getHighShelfSlope() );
			return true;
		case PARAMETER_INDEX_HIGH_SHELF_GAIN:
			value = FilterParameter( this->getHighShelfGain() );
			return true;
		//*******************************************************************************
		case PARAMETER_INDEX_LOW_PASS_ENABLED:
			value = FilterParameter( this->getLowPassIsEnabled() );
			return true;
		case PARAMETER_INDEX_LOW_PASS_FREQUENCY:
			value = FilterParameter( this->getLowPassFrequency() );
			return true;
		case PARAMETER_INDEX_LOW_PASS_ORDER:
			value = FilterParameter( (Int64)this->getLowPassOrder() );
			return true;
	}
	
	return false;
}




Bool ParametricEqualizer:: setParameterValue( Index parameterIndex, const FilterParameter& value )
{
	Float floatValue;
	Gain gainValue;
	Bool boolValue;
	Int64 intValue;
	
	switch ( parameterIndex )
	{
		case PARAMETER_INDEX_OUTPUT_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setOutputGainDB( gainValue );
				return true;
			}
			break;
		//*******************************************************************************
		case PARAMETER_INDEX_HIGH_PASS_ENABLED:
			if ( value.getValue( boolValue ) )
			{
				this->setHighPassIsEnabled( boolValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_PASS_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setHighPassFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_PASS_ORDER:
			if ( value.getValue( intValue ) )
			{
				this->setHighPassOrder( (Size)intValue );
				return true;
			}
			break;
		//*******************************************************************************
		case PARAMETER_INDEX_LOW_SHELF_ENABLED:
			if ( value.getValue( boolValue ) )
			{
				this->setLowShelfIsEnabled( boolValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_SHELF_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setLowShelfFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_SHELF_SLOPE:
			if ( value.getValue( floatValue ) )
			{
				this->setLowShelfSlope( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_SHELF_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setLowShelfGainDB( gainValue );
				return true;
			}
			break;
		//*******************************************************************************
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_ENABLED + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_ENABLED) / PARAMETRIC_PARAMETER_COUNT;
			
			if ( value.getValue( boolValue ) )
			{
				this->setParametricIsEnabled( parametricIndex, boolValue );
				return true;
			}
		}
		break;
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_FREQUENCY) / PARAMETRIC_PARAMETER_COUNT;
			
			if ( value.getValue( floatValue ) )
			{
				this->setParametricFrequency( parametricIndex, floatValue );
				return true;
			}
		}
		break;
		case PARAMETER_INDEX_PARAMETRIC_1_Q:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_Q + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_Q) / PARAMETRIC_PARAMETER_COUNT;
			
			if ( value.getValue( floatValue ) )
			{
				this->setParametricQ( parametricIndex, floatValue );
				return true;
			}
		}
		break;
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_BANDWIDTH) / PARAMETRIC_PARAMETER_COUNT;
			
			if ( value.getValue( floatValue ) )
			{
				this->setParametricBandwidth( parametricIndex, floatValue );
				return true;
			}
		}
		break;
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 1*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 2*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 3*PARAMETRIC_PARAMETER_COUNT:
		case PARAMETER_INDEX_PARAMETRIC_1_GAIN + 4*PARAMETRIC_PARAMETER_COUNT:
		{
			Index parametricIndex = (parameterIndex - PARAMETER_INDEX_PARAMETRIC_1_GAIN) / PARAMETRIC_PARAMETER_COUNT;
			
			if ( value.getValue( floatValue ) )
			{
				this->setParametricGainDB( parametricIndex, floatValue );
				return true;
			}
		}
		break;
		//*******************************************************************************
		case PARAMETER_INDEX_HIGH_SHELF_ENABLED:
			if ( value.getValue( boolValue ) )
			{
				this->setHighShelfIsEnabled( boolValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_SHELF_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setHighShelfFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_SHELF_SLOPE:
			if ( value.getValue( floatValue ) )
			{
				this->setHighShelfSlope( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_HIGH_SHELF_GAIN:
			if ( value.getValue( gainValue ) )
			{
				this->setHighShelfGainDB( gainValue );
				return true;
			}
			break;
		//*******************************************************************************
		case PARAMETER_INDEX_LOW_PASS_ENABLED:
			if ( value.getValue( boolValue ) )
			{
				this->setLowPassIsEnabled( boolValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_PASS_FREQUENCY:
			if ( value.getValue( floatValue ) )
			{
				this->setLowPassFrequency( floatValue );
				return true;
			}
			break;
		case PARAMETER_INDEX_LOW_PASS_ORDER:
			if ( value.getValue( intValue ) )
			{
				this->setLowPassOrder( (Size)intValue );
				return true;
			}
			break;
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Method
//############		
//##########################################################################################
//##########################################################################################




void ParametricEqualizer:: resetStream()
{
	highPass.reset();
	lowShelf.reset();
	
	// Reset the parametric filters.
	for ( Index c = 0; c < parametrics.getSize(); c++ )
		parametrics[c].filter.reset();
	
	highShelf.reset();
	lowPass.reset();
	gainFilter.reset();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult ParametricEqualizer:: processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples )
{
	const SoundBuffer* inputBuffer;
	
	// If there is no input buffer or if it is NULL, return that an error occurred.
	if ( inputFrame.getBufferCount() == 0 || (inputBuffer = inputFrame.getBuffer(0)) == NULL )
		return SoundResult::ERROR;
	
	SoundBuffer* outputBuffer;
	
	// If there is no output buffer or if it is NULL, return that no samples were processed.
	if ( outputFrame.getBufferCount() == 0 || (outputBuffer = outputFrame.getBuffer(0)) == NULL )
		return 0;
	
	const SampleRate inputSampleRate = inputBuffer->getSampleRate();
	
	// If the input sample rate is zero, return that an error occurred.
	if ( inputSampleRate <= SampleRate(0) )
		return SoundResult::ERROR;
	
	// Make sure that the output buffer has the right size and format.
	inputBuffer->copyFormatTo( *outputBuffer, numSamples );
	
	// Pass through MIDI data from input to output.
	inputFrame.copyMIDITo( outputFrame );
	
	//*************************************************************************
	// Process the high-pass and low-pass filters.
	
	if ( highPassEnabled )
	{
		highPass.process( *inputBuffer, *outputBuffer, numSamples );
		inputBuffer = outputBuffer;
	}
	
	if ( lowPassEnabled )
	{
		lowPass.process( *inputBuffer, *outputBuffer, numSamples );
		inputBuffer = outputBuffer;
	}
	
	//*************************************************************************
	// Process the low shelf and high shelf filters.
	
	if ( lowShelfEnabled && !gainIsUnity(lowShelf.getGain()) )
	{
		lowShelf.process( *inputBuffer, *outputBuffer, numSamples );
		inputBuffer = outputBuffer;
	}
	
	if ( highShelfEnabled && !gainIsUnity(highShelf.getGain()) )
	{
		highShelf.process( *inputBuffer, *outputBuffer, numSamples );
		inputBuffer = outputBuffer;
	}
	
	//*************************************************************************
	// Process each parametric filter.
	
	for ( Index i = 0; i < parametrics.getSize(); i++ )
	{
		ParametricFilterBand& band = parametrics[i];
		
		if ( band.isEnabled && !gainIsUnity(band.filter.getGain()) )
		{
			band.filter.process( *inputBuffer, *outputBuffer, numSamples );
			inputBuffer = outputBuffer;
		}
	}
	
	// Apply the output gain if the gain is not unity or no filters have been processed.
	if ( !gainIsUnity(gainFilter.getGain()) || inputBuffer != outputBuffer )
		gainFilter.process( *inputBuffer, *outputBuffer, numSamples );
	
	return numSamples;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
