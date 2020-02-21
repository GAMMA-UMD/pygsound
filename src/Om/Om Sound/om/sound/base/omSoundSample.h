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

#ifndef INCLUDE_OM_SOUND_SAMPLE_H
#define INCLUDE_OM_SOUND_SAMPLE_H


#include "omSoundBaseConfig.h"


#include "omSoundInt24.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a template primitive-typed sound sample.
/**
  * This class provides a wrapper for basic primitive types which provides
  * automatic sample saturation and proper conversion between different sample
  * types using standard arithmetic and cast operators.
  *
  * Floating-point template instantiations do not do saturating sample arithmetic
  * because of their much greater dynamic range.
  */
template < typename T >
class Sample
{
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			/// The underlying type used to represent this sample.
			typedef T BaseType;
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new Sample with value equal to 0.
			OM_FORCE_INLINE Sample()
				:	sample( 0 )
			{
			}
			
			
			/// Create a new Sample with the specified value.
			OM_FORCE_INLINE Sample( const T& newSample )
				:	sample( newSample )
			{
			}
			
			
			/// Create a new Sample with the specified floating-point value, converted to the sample type.
			OM_FORCE_INLINE Sample( Float32 newSample );
			
			
			/// Create a new Sample with the specified floating-point value, converted to the sample type.
			OM_FORCE_INLINE Sample( Float64 newSample );
			
			
			/// Convert a sample of another templated type to this sample type.
			/**
			  * This method applies the best conversion from the specified sample type
			  * to this sample type. For conversions to the 8-bit sample type, a simple
			  * dither is performed to improve the audio quality. One might want to implement
			  * a higher quality dither for these conversions, depending on the application.
			  */
			template < typename U >
			OM_FORCE_INLINE Sample( const Sample<U>& other );
			
			
		//********************************************************************************
		//******	Cast Operators
			
			
			/// Convert this sample's internal primitive type to the templated primitive type.
			template < typename U >
			OM_FORCE_INLINE operator U () const;
			
			
		//********************************************************************************
		//******	Mixing (Addition) Operators
			
			
			/// Mix this sample with another and return the result.
			OM_FORCE_INLINE Sample operator + ( const Sample& other ) const
			{
				return Sample( sample + other.sample );
			}
			
			
			/// Mix another sample with this sample and overwrite this sample.
			OM_FORCE_INLINE Sample& operator += ( const Sample& other )
			{
				sample += other.sample;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Subtraction Operators
			
			
			/// Subtract another sample from this sample and return the result.
			OM_FORCE_INLINE Sample operator - ( const Sample& other ) const
			{
				return Sample( sample - other.sample );
			}
			
			
			/// Subtract another sample from this sample and overwrite this sample.
			OM_FORCE_INLINE Sample& operator -= ( const Sample& other )
			{
				sample -= other.sample;
				
				return *this;
			}
			
			
		//********************************************************************************
		//******	Gain Scaling Operators
			
			
			/// Scale this sample by a linear gain factor and return the result.
			OM_FORCE_INLINE Sample operator * ( Float gain ) const
			{
				return Sample( sample*gain );
			}
			
			
			/// Scale this sample by a linear gain factor and return the result.
			OM_FORCE_INLINE Sample operator * ( Double gain ) const
			{
				return Sample( sample*gain );
			}
			
			
			/// Scale this sample by a linear gain factor and overwite this sample.
			OM_FORCE_INLINE Sample& operator *= ( Float gain )
			{
				sample *= gain;
				
				return sample;
			}
			
			
			/// Scale this sample by a linear gain factor and overwite this sample.
			OM_FORCE_INLINE Sample& operator *= ( Double gain )
			{
				sample *= gain;
				
				return sample;
			}
			
			
		//********************************************************************************
		//******	Sample Conversion Methods
			
			
			/// Convert a sample value with the specified underlying primitive type to the templated primitive type.
			template < typename U >
			OM_FORCE_INLINE static T convert( U sample );
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The data member which represents this sample's value.
			T sample;
			
			
		//********************************************************************************
		//******	Private Friend Class Declaration
			
			
			/// Declare other types of sample objects to be friend classes.
			template < typename U >
			friend class Sample;
			
			
			
};





//##########################################################################################
//##########################################################################################
//############		
//############		Sample to Sample8 Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
OM_FORCE_INLINE Int8 Sample<Int8>:: convert( Int8 sample )
{
	return sample;
}




template <> template <>
OM_FORCE_INLINE Int8 Sample<Int8>:: convert( Int16 sample )
{
	return Int8(sample >> 8);
}




template <> template <>
OM_FORCE_INLINE Int8 Sample<Int8>:: convert( Int24 sample )
{
	return Int8(sample >> 16);
}




template <> template <>
OM_FORCE_INLINE Int8 Sample<Int8>:: convert( Int32 sample )
{
	return Int8(sample >> 24);
}




template <> template <>
OM_FORCE_INLINE Int8 Sample<Int8>:: convert( Int64 sample )
{
	return Int8(sample >> 56);
}




template <> template <>
OM_FORCE_INLINE Int8 Sample<Int8>:: convert( Float32 sample )
{
	return Int8(math::round( math::clamp( sample + math::random(-0.5f / 128, 0.5f / 127),
												Float32(-1), Float32(1) )*math::max<Int8>() ));
}




template <> template <>
OM_FORCE_INLINE Int8 Sample<Int8>:: convert( Float64 sample )
{
	return Int8(math::round( math::clamp( sample + math::random(-0.5 / 128, 0.5 / 127),
												Float64(-1), Float64(1) )*math::max<Int8>() ));
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample to Sample16 Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
OM_FORCE_INLINE Int16 Sample<Int16>:: convert( Int8 sample )
{
	return Int16(sample) << 8;
}




template <> template <>
OM_FORCE_INLINE Int16 Sample<Int16>:: convert( Int16 sample )
{
	return sample;
}





template <> template <>
OM_FORCE_INLINE Int16 Sample<Int16>:: convert( Int24 sample )
{
	return Int16(sample >> 8);
}




template <> template <>
OM_FORCE_INLINE Int16 Sample<Int16>:: convert( Int32 sample )
{
	return Int16(sample >> 16);
}




template <> template <>
OM_FORCE_INLINE Int16 Sample<Int16>:: convert( Int64 sample )
{
	return Int16(sample >> 48);
}




template <> template <>
OM_FORCE_INLINE Int16 Sample<Int16>:: convert( Float32 sample )
{
	return Int16(math::round( math::clamp( sample + math::random(-0.5f / 32768, 0.5f / 32767),
												Float32(-1), Float32(1) )*math::max<Int16>() ));
}




template <> template <>
OM_FORCE_INLINE Int16 Sample<Int16>:: convert( Float64 sample )
{
	return Int16(math::round( math::clamp( sample + math::random(-0.5 / 32768, 0.5 / 32767),
												Float64(-1), Float64(1) )*math::max<Int16>() ));
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample to Sample24 Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
OM_FORCE_INLINE Int24 Sample<Int24>:: convert( Int8 sample )
{
	return Int24(sample) << 16;
}




template <> template <>
OM_FORCE_INLINE Int24 Sample<Int24>:: convert( Int16 sample )
{
	return Int24(sample) << 8;
}




template <> template <>
OM_FORCE_INLINE Int24 Sample<Int24>:: convert( Int24 sample )
{
	return sample;
}




template <> template <>
OM_FORCE_INLINE Int24 Sample<Int24>:: convert( Int32 sample )
{
	return Int24(sample >> 8);
}




template <> template <>
OM_FORCE_INLINE Int24 Sample<Int24>:: convert( Int64 sample )
{
	return Int24(Int32(sample >> 40));
}




template <> template <>
OM_FORCE_INLINE Int24 Sample<Int24>:: convert( Float32 sample )
{
	return Int24( Int32(math::clamp( sample, Float32(-1), Float32(1) )*8388607.0f ) );
}




template <> template <>
OM_FORCE_INLINE Int24 Sample<Int24>:: convert( Float64 sample )
{
	return Int24( Int32(math::clamp( sample, Float64(-1), Float64(1) )*8388607.0f ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample to Sample32 Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
OM_FORCE_INLINE Int32 Sample<Int32>:: convert( Int8 sample )
{
	return Int32(sample) << 24;
}




template <> template <>
OM_FORCE_INLINE Int32 Sample<Int32>:: convert( Int16 sample )
{
	return Int32(sample) << 16;
}




template <> template <>
OM_FORCE_INLINE Int32 Sample<Int32>:: convert( Int24 sample )
{
	return Int32(sample) << 8;
}




template <> template <>
OM_FORCE_INLINE Int32 Sample<Int32>:: convert( Int32 sample )
{
	return sample;
}




template <> template <>
OM_FORCE_INLINE Int32 Sample<Int32>:: convert( Int64 sample )
{
	return Int32(sample >> 32);
}




template <> template <>
OM_FORCE_INLINE Int32 Sample<Int32>:: convert( Float32 sample )
{
	return Int32(math::clamp( sample, Float32(-1), Float32(1) )*math::max<Int32>());
}




template <> template <>
OM_FORCE_INLINE Int32 Sample<Int32>:: convert( Float64 sample )
{
	return Int32(math::clamp( sample, Float64(-1), Float64(1) )*math::max<Int32>());
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample to Sample64 Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
OM_FORCE_INLINE Int64 Sample<Int64>:: convert( Int8 sample )
{
	return Int64(sample) << 56;
}




template <> template <>
OM_FORCE_INLINE Int64 Sample<Int64>:: convert( Int16 sample )
{
	return Int64(sample) << 48;
}




template <> template <>
OM_FORCE_INLINE Int64 Sample<Int64>:: convert( Int24 sample )
{
	return Int64(sample) << 40;
}




template <> template <>
OM_FORCE_INLINE Int64 Sample<Int64>:: convert( Int32 sample )
{
	return Int64(sample) << 32;
}




template <> template <>
OM_FORCE_INLINE Int64 Sample<Int64>:: convert( Int64 sample )
{
	return sample;
}





template <> template <>
OM_FORCE_INLINE Int64 Sample<Int64>:: convert( Float32 sample )
{
	return Int64(math::clamp( sample, Float32(-1), Float32(1) )*Float32(math::max<Int64>()));
}




template <> template <>
OM_FORCE_INLINE Int64 Sample<Int64>:: convert( Float64 sample )
{
	return Int64(math::clamp( sample, Float64(-1), Float64(1) )*math::max<Int64>());
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample to Sample32f Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
OM_FORCE_INLINE Float32 Sample<Float32>:: convert( Int8 sample )
{
	return sample < 0 ? (Float32)sample / -(Float32)math::min<Int8>() : (Float32)sample / (Float32)math::max<Int8>();
}




template <> template <>
OM_FORCE_INLINE Float32 Sample<Float32>:: convert( Int16 sample )
{
	return sample < 0 ? (Float32)sample / -(Float32)math::min<Int16>() : (Float32)sample / (Float32)math::max<Int16>();
}




template <> template <>
OM_FORCE_INLINE Float32 Sample<Float32>:: convert( Int24 sample )
{
	return sample < 0 ? (Float32)sample / Float32(8388608) : (Float32)sample / Float32(8388607);
}




template <> template <>
OM_FORCE_INLINE Float32 Sample<Float32>:: convert( Int32 sample )
{
	return sample < 0 ? (Float32)sample / -(Float32)math::min<Int32>() : (Float32)sample / (Float32)math::max<Int32>();
}




template <> template <>
OM_FORCE_INLINE Float32 Sample<Float32>:: convert( Int64 sample )
{
	return sample < 0 ? (Float32)sample / -(Float32)math::min<Int64>() : (Float32)sample / (Float32)math::max<Int64>();
}




template <> template <>
OM_FORCE_INLINE Float32 Sample<Float32>:: convert( Float32 sample )
{
	return sample;
}




template <> template <>
OM_FORCE_INLINE Float32 Sample<Float32>:: convert( Float64 sample )
{
	return (Float32)sample;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample to Sample32f Conversion Methods
//############		
//##########################################################################################
//##########################################################################################




template <> template <>
OM_FORCE_INLINE Float64 Sample<Float64>:: convert( Int8 sample )
{
	return sample < 0 ? (Float64)sample / -(Float64)math::min<Int8>() : (Float64)sample / (Float64)math::max<Int8>();
}




template <> template <>
OM_FORCE_INLINE Float64 Sample<Float64>:: convert( Int16 sample )
{
	return sample < 0 ? (Float64)sample / -(Float64)math::min<Int16>() : (Float64)sample / (Float64)math::max<Int16>();
}




template <> template <>
OM_FORCE_INLINE Float64 Sample<Float64>:: convert( Int24 sample )
{
	return sample < 0 ? (Float64)sample / Float64(8388608) : (Float64)sample / Float64(8388607);
}




template <> template <>
OM_FORCE_INLINE Float64 Sample<Float64>:: convert( Int32 sample )
{
	return sample < 0 ? (Float64)sample / -(Float64)math::min<Int32>() : (Float64)sample / (Float64)math::max<Int32>();
}




template <> template <>
OM_FORCE_INLINE Float64 Sample<Float64>:: convert( Int64 sample )
{
	return sample < 0 ? (Float64)sample / -(Float64)math::min<Int64>() : (Float64)sample / (Float64)math::max<Int64>();
}




template <> template <>
OM_FORCE_INLINE Float64 Sample<Float64>:: convert( Float32 sample )
{
	return (Float64)sample;
}




template <> template <>
OM_FORCE_INLINE Float64 Sample<Float64>:: convert( Float64 sample )
{
	return sample;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
OM_FORCE_INLINE Sample<T>:: Sample( Float32 other )
	:	sample( convert( other ) )
{
}




template < typename T >
OM_FORCE_INLINE Sample<T>:: Sample( Float64 other )
	:	sample( convert( other ) )
{
}




template < typename T > template < typename U >
OM_FORCE_INLINE Sample<T>:: Sample( const Sample<U>& other )
	:	sample( convert( other.sample ) )
{
}




template < typename T > template < typename U >
OM_FORCE_INLINE Sample<T>:: operator U () const
{
	return Sample<U>::convert( sample );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Commutative Sample Operators
//############		
//##########################################################################################
//##########################################################################################




/// Multiply a sample by a linear gain factor and return the resulting sample.
template < typename T >
OM_FORCE_INLINE Sample<T> operator * ( const Float& gain, const Sample<T>& sample )
{
	return sample*gain;
}




/// Multiply a sample by a linear gain factor and return the resulting sample.
template < typename T >
OM_FORCE_INLINE Sample<T> operator * ( const Double& gain, const Sample<T>& sample )
{
	return sample*gain;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer Sample Mixing Operations
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_FORCE_INLINE Sample<Int8> Sample<Int8>:: operator + ( const Sample<Int8>& other ) const
{
	return Sample<Int8>( (Int8)math::clamp( (Int32)sample + (Int32)other.sample,
											(Int32)math::min<Int8>(), (Int32)math::max<Int8>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int16> Sample<Int16>:: operator + ( const Sample<Int16>& other ) const
{
	return Sample<Int16>( (Int16)math::clamp( (Int32)sample + (Int32)other.sample,
												(Int32)math::min<Int16>(), (Int32)math::max<Int16>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int24> Sample<Int24>:: operator + ( const Sample<Int24>& other ) const
{
	return Sample<Int24>( Int24( math::clamp( sample + other.sample,
										Int32(-8388608), Int32(8388607) ) ) );
}




template <>
OM_FORCE_INLINE Sample<Int32> Sample<Int32>:: operator + ( const Sample<Int32>& other ) const
{
	return Sample<Int32>( (Int32)math::clamp( (Int64)sample + (Int64)other.sample,
											(Int64)math::min<Int32>(), (Int64)math::max<Int32>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int64> Sample<Int64>:: operator + ( const Sample<Int64>& other ) const
{
	return Sample<Int64>( (Int64)math::clamp( (Double)sample + (Double)other.sample,
											(Double)math::min<Int64>(), (Double)math::max<Int64>() ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer Sample Subtraction Operations
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_FORCE_INLINE Sample<Int8> Sample<Int8>:: operator - ( const Sample<Int8>& other ) const
{
	return Sample<Int8>( (Int8)math::clamp( (Int32)sample - (Int32)other.sample,
											(Int32)math::min<Int8>(), (Int32)math::max<Int8>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int16> Sample<Int16>:: operator - ( const Sample<Int16>& other ) const
{
	return Sample<Int16>( (Int16)math::clamp( (Int32)sample - (Int32)other.sample,
											(Int32)math::min<Int16>(), (Int32)math::max<Int16>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int24> Sample<Int24>:: operator - ( const Sample<Int24>& other ) const
{
	return Sample<Int24>( Int24( math::clamp( sample - other.sample,
										Int32(-8388608), Int32(8388607) ) ) );
}




template <>
OM_FORCE_INLINE Sample<Int32> Sample<Int32>:: operator - ( const Sample<Int32>& other ) const
{
	return Sample<Int32>( (Int32)math::clamp( (Int64)sample - (Int64)other.sample,
											(Int64)math::min<Int32>(), (Int64)math::max<Int32>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int64> Sample<Int64>:: operator - ( const Sample<Int64>& other ) const
{
	return Sample<Int64>( (Int64)math::clamp( (Double)sample - (Double)other.sample,
											(Double)math::min<Int64>(), (Double)math::max<Int64>() ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Integer Sample Scaling Operations
//############		
//##########################################################################################
//##########################################################################################




template <>
OM_FORCE_INLINE Sample<Int8> Sample<Int8>:: operator * ( Gain gain ) const
{
	return Sample<Int8>( (Int8)math::clamp( Float32(sample)*gain,
											(Float32)math::min<Int8>(), (Float32)math::max<Int8>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int16> Sample<Int16>:: operator * ( Gain gain ) const
{
	return Sample<Int16>( (Int16)math::clamp( Float32(sample)*gain,
											(Float32)math::min<Int16>(), (Float32)math::max<Int16>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int24> Sample<Int24>:: operator * ( Gain gain ) const
{
	return Sample<Int24>( Int24( (Int32)math::clamp( Float32(sample)*gain,
											Float32(-8388608.0f), Float32(8388607.0f) ) ) );
}




template <>
OM_FORCE_INLINE Sample<Int32> Sample<Int32>:: operator * ( Gain gain ) const
{
	return Sample<Int32>( (Int32)math::clamp( Float64(sample)*gain,
											(Float64)math::min<Int32>(), (Float64)math::max<Int32>() ) );
}




template <>
OM_FORCE_INLINE Sample<Int64> Sample<Int64>:: operator * ( Gain gain ) const
{
	return Sample<Int64>( (Int64)math::clamp( Float64(sample)*gain,
											(Float64)math::min<Int64>(), (Float64)math::max<Int64>() ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sample Typedefs
//############		
//##########################################################################################
//##########################################################################################




/// Define the type of a integral sample which uses a signed 8-bit representation.
typedef Sample<Int8> Sample8;




/// Define the type of a integral sample which uses a signed 16-bit representation.
typedef Sample<Int16> Sample16;




/// Define the type of a integral sample which uses a signed 24-bit representation.
typedef Sample<Int24> Sample24;




/// Define the type of a integral sample which uses a signed 32-bit representation.
typedef Sample<Int32> Sample32;




/// Define the type of a integral sample which uses a signed 64-bit representation.
typedef Sample<Int64> Sample64;




/// Define the type of a floating-point sample which uses a 32-bit representation.
typedef Float32 Sample32f;




/// Define the type of a floating-point sample which uses a 64-bit representation.
typedef Float64 Sample64f;




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_SAMPLE_H
