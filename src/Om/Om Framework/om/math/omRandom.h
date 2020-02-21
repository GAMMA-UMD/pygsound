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

#ifndef INCLUDE_OM_RANDOM_VARIABLE_H
#define INCLUDE_OM_RANDOM_VARIABLE_H


#include "omMathConfig.h"


#include "../time/omTime.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		RNG Output Class
//############		
//##########################################################################################
//##########################################################################################




template < Size numBytes >
class RNGOutput;




template <>
class RNGOutput<1>
{
	public:		typedef UInt8 Type;
};




template <>
class RNGOutput<2>
{
	public:		typedef UInt16 Type;
};




template <>
class RNGOutput<4>
{
	public:		typedef UInt32 Type;
};




template <>
class RNGOutput<8>
{
	public:		typedef UInt64 Type;
};




//##########################################################################################
//##########################################################################################
//############		
//############		LCG RNG Class
//############		
//##########################################################################################
//##########################################################################################




/// A fast simple linear congruential generator of pseudorandom numbers.
template < Size numBytes >
class LCG
{
	public:
		
		
		/// The seed type for this generator.
		typedef typename RNGOutput<numBytes>::Type SeedType;
		
		
		/// The output type for this generator.
		typedef typename RNGOutput<numBytes>::Type OutputType;
		
		
		/// The number of seed values that are used as the state for this generator.
		static const Size SEED_SIZE = 1;
		
		
		/// Advance the specified random generator state and return the next output value.
		OM_FORCE_INLINE static OutputType next( SeedType* state )
		{
			SeedType output = state[0]*MULTIPLIER + INCREMENT;
			state[0] = output;
			
			return *reinterpret_cast<const OutputType*>(&output);
		}
		
		
	private:
		
		/// The multiplier for the linear congruential generator. This is the value used by GCC.
		static const SeedType MULTIPLIER = SeedType(1103515245);
		
		/// The increment for the linear congruential generator. This is the value used by GCC.
		static const SeedType INCREMENT = SeedType(12345);
		
};




/// A fast simple linear congruential generator of pseudorandom numbers.
template <>
class LCG<8>
{
	public:
		
		
		/// The seed type for this generator.
		typedef RNGOutput<8>::Type SeedType;
		
		
		/// The output type for this generator.
		typedef RNGOutput<8>::Type OutputType;
		
		
		/// The number of seed values that are used as the state for this generator.
		static const Size SEED_SIZE = 1;
		
		
		/// Advance the specified random generator state and return the next output value.
		OM_FORCE_INLINE static OutputType next( SeedType* state )
		{
			SeedType output = state[0]*MULTIPLIER + INCREMENT;
			state[0] = output;
			
			return *reinterpret_cast<const OutputType*>(&output);
		}
		
		
	private:
		
		/// The multiplier for the linear congruential generator.
		static const SeedType MULTIPLIER = SeedType(2862933555777941757ull);
		
		/// The increment for the linear congruential generator.
		static const SeedType INCREMENT = SeedType(3037000493ull);
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Xoroshiro128+ RNG Class
//############		
//##########################################################################################
//##########################################################################################




/// An implementation of the Xoroshiro128+ pseudorandom number generator.
/**
  * Original version by David Blackman and Sebastiano Vigna.
  * A comparison of the performance versus other PRNGs can be found
  * at the author's website: http://xoroshiro.di.unimi.it
  */
template < Size numBytes >
class Xoroshiro128
{
	public:
		
		
		/// The seed type for this generator.
		typedef UInt64 SeedType;
		
		
		/// The output type for this generator.
		typedef typename RNGOutput<numBytes>::Type OutputType;
		
		
		/// The number of seed values that are used as the state for this generator.
		static const Size SEED_SIZE = 2;
		
		
		/// Advance the specified random generator state and return the next output value.
		OM_FORCE_INLINE static OutputType next( SeedType* state )
		{
			const SeedType s0 = state[0];
			SeedType s1 = state[1];
			const SeedType result = s0 + s1;
			
			s1 ^= s0;
			state[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
			state[1] = rotl(s1, 36); // c
			
			return *reinterpret_cast<const OutputType*>(&result);
		}
		
		
	private:
		
		/// Swap the first and last parts of the specified value around at the kth bit.
		OM_FORCE_INLINE static SeedType rotl( const SeedType x, int k )
		{
			return (x << k) | (x >> (64 - k));
		}
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
class RandomBase
{
	protected:
		
		//********************************************************************************
		//******	Protected Methods
			
			
			/// Convert the unsigned integer output of the RNG to the output type.
			template < typename OutputType >
			OM_FORCE_INLINE static T convertOutput( OutputType seed )
			{
				return *((T*)&seed);
			}
			
			
			/// Convert the unsigned integer output of the RNG to the output type.
			template < typename OutputType >
			OM_FORCE_INLINE static T convertOutput( OutputType seed, T min, T max )
			{
				OutputType uMin = *((OutputType*)&min) + (OutputType)math::min<T>();
				OutputType uMax = *((OutputType*)&max) - (OutputType)math::min<T>();
				OutputType a = (seed % (uMax - uMin + 1)) + *((OutputType*)&min);
				
				return *((T*)&a);
			}
			
			
};




/// A class representing a random number generator for the templated data type.
template < typename T, template < Size numBytes > class RNG = Xoroshiro128 >
class Random : public RandomBase<T>
{
	public:
		
		//********************************************************************************
		//******	Seed Type Declaration
			
			
			/// The number of seed values used as the state of this random variable.
			static const Size WIDTH = sizeof(T);
			
			
			/// The type used for the internal random number generator.
			typedef RNG<WIDTH> RNGType;
			
			
			/// The underlying type used to represent the random seed.
			typedef typename RNGType::SeedType SeedType;
			
			
			/// The underlying unsigned integer type that is the output of the PRNG.
			typedef typename RNGType::OutputType OutputType;
			
			
			/// The number of seed values that are part of the random state.
			static const Size SEED_SIZE = RNGType::SEED_SIZE;
			
			
			class State
			{
				public:
					
					/// The current state of the random variable.
					SeedType state[SEED_SIZE];
					
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a random variable with a default initial seed value derived from the current system time.
			OM_INLINE Random()
			{
				this->setSeed( (SeedType)time::Time::getCurrent().getNanoseconds() );
			}
			
			
			/// Create a random variable with the specified initial seed value.
			OM_INLINE Random( SeedType newSeed )
			{
				this->setSeed( newSeed );
			}
			
			
		//********************************************************************************
		//******	Sample Methods
			
			
			/// Generate a sample from the random variable and return the result.
			/**
			  * The value returned may have any numerical value representable by the
			  * random variable's data type. The pseudorandom values returned over
			  * successive calls to this function will lie in a uniform distribution.
			  */
			OM_INLINE T sample()
			{
				return RandomBase<T>::convertOutput( RNGType::next( state.state ) );
			}
			
			
			/// Generate a sample from the random variable in the specified range.
			/**
			  * The value returned can have any numerical value between and including
			  * the minimum and maximum values specified. The pseudorandom values
			  * returned over successive calls to this function will lie in a uniform
			  * distribution.
			  */
			OM_INLINE T sample( T min, T max )
			{
				return RandomBase<T>::convertOutput( RNGType::next( state.state ), min, max );
			}
			
			
		//********************************************************************************
		//******	Random Seed Accessor Methods
			
			
			/// Set the initial seed for the random variable.
			/**
			  * If the PRNG state is larger than one seed value, a simple RNG is used
			  * to generate the rest of the state from the initial seed.
			  * After setting the seed for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			OM_INLINE void setSeed( SeedType newSeed )
			{
				const SeedType multiplier(1664525);
				const SeedType increment(1013904223);
				
				// Populate the state with output of a low-quality RNG.
				state.state[0] = newSeed*multiplier + increment;
				
				for ( Index i = 1; i < SEED_SIZE; i++ )
					state.state[i] = (multiplier*state.state[i-1] + increment);
			}
			
			
			/// Set the entire state for the random variable.
			/**
			  * After setting the state for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			OM_INLINE void setState( const SeedType newState[WIDTH] )
			{
				for ( Index i = 0; i < SEED_SIZE; i++ )
					state.state[i] = newState[i];
			}
			
			
			/// Set the entire state for the random variable.
			/**
			  * After setting the state for the random variable, calls to the sample()
			  * methods will produce the same sequence of values for equal initial seed
			  * values.
			  */
			OM_INLINE void setState( const State& newState )
			{
				state = newState;
			}
			
			
			/// Return an object containing the internal state storage of this random variable.
			OM_INLINE const State& getState() const
			{
				return state;
			}
			
			
			/// Return the number of elements that make up the state of this random variable.
			OM_INLINE Size getStateSize() const
			{
				return SEED_SIZE;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Member
			
			
			/// The current state of the random variable.
			State state;
			
			
};




//##########################################################################################
//##########################################################################################
//############		
//############		Random Variable Specializations
//############		
//##########################################################################################
//##########################################################################################




template <> template < typename OutputType >
OM_INLINE float RandomBase<float>:: convertOutput( OutputType seed )
{
	OutputType a = (seed & OutputType(0x007FFFFF)) | OutputType(0x3F800000);
	
	return (*((float*)&a) - 1.5f)*2.0f*math::max<float>();
}




template <> template < typename OutputType >
OM_INLINE float RandomBase<float>:: convertOutput( OutputType seed, float min, float max )
{
	OutputType a = (seed & OutputType(0x007FFFFF)) | OutputType(0x3F800000);
	
	return (*((float*)&a) - 1.0f)*(max - min) + min;
}




template <> template < typename OutputType >
OM_INLINE double RandomBase<double>:: convertOutput( OutputType seed )
{
	OutputType a = (seed & OutputType(0x000FFFFFFFFFFFFFull)) | OutputType(0x3FF0000000000000ull);
	
	return (*((double*)&a) - 1.5)*2.0*math::max<double>();
}




template <> template < typename OutputType >
OM_INLINE double RandomBase<double>:: convertOutput( OutputType seed, double min, double max )
{
	OutputType a = (seed & OutputType(0x000FFFFFFFFFFFFFull)) | OutputType(0x3FF0000000000000ull);
	
	return (*((double*)&a) - 1.0)*(max - min) + min;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Global Random Number Generation Methods
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
OM_INLINE static Random<T>& getGlobalRandom()
{
	static Random<T> randomVariable;
	
	return randomVariable;
}




template < typename T >
OM_INLINE T random()
{
	return getGlobalRandom<T>().sample();
}




template < typename T >
OM_INLINE T random( T min, T max )
{
	return getGlobalRandom<T>().sample( min, max );
}




template < typename T >
OM_INLINE void setRandomSeed( T newSeed )
{
	getGlobalRandom<T>().setSeed( *(typename Random<T>::SeedType*)(&newSeed) );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_RANDOM_VARIABLE_H
