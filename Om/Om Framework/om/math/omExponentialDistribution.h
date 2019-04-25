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

#ifndef INCLUDE_OM_EXPONENTIAL_DISTRIBUTION_H
#define INCLUDE_OM_EXPONENTIAL_DISTRIBUTION_H


#include "omMathConfig.h"


#include "omRandom.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// A class that generates an exponential probability distribution for the templated type.
template < typename T >
class ExponentialDistribution;




//********************************************************************************
/// A class that generates an exponential probability distribution for 32-bit floats.
template <>
class ExponentialDistribution<float>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an exponential distribution with lambda equal to 1.
			OM_INLINE ExponentialDistribution()
				:	lambda( 1.0f ),
					randomVariable()
			{
			}
			
			
			/// Create an exponential distribution with lambda equal to 1.
			/**
			  * The created exponential distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE ExponentialDistribution( const Random<float>& newRandom )
				:	lambda( 1.0f ),
					randomVariable( newRandom )
			{
			}
			
			
			/// Create an exponential distribution with lambda equal to the specified value.
			OM_INLINE ExponentialDistribution( float newLambda )
				:	lambda( math::max( newLambda, 0.0f ) ),
					randomVariable()
			{
			}
			
			
			/// Create an exponential distribution with lambda equal to the specified value.
			/**
			  * The created exponential distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE ExponentialDistribution( float newLambda, const Random<float>& newRandom )
				:	lambda( math::max( newLambda, 0.0f ) ),
					randomVariable( newRandom )
			{
			}
			
			
		//********************************************************************************
		//******	Distribution Sample Generation Method
			
			
			/// Generate a sample from the exponential distribution.
			OM_INLINE float sample()
			{
				return -math::ln( randomVariable.sample( 0.0f, 1.0f ) )/lambda;
			}
			
			
		//********************************************************************************
		//******	Lambda Parameter Accessor Methods
			
			
			/// Get the lambda parameter of this exponential distribution.
			OM_INLINE float getLambda() const
			{
				return lambda;
			}
			
			
			/// Set the lambda parameter of this exponential distribution.
			OM_INLINE void setLambda( float newLambda )
			{
				lambda = newLambda;
			}
			
			
		//********************************************************************************
		//******	Random Variable Accessor Methods
			
			
			/// Get the random variable used to generate samples for this distribution.
			OM_INLINE Random<float>& getRandom()
			{
				return randomVariable;
			}
			
			
			/// Get the random variable used to generate samples for this distribution.
			OM_INLINE const Random<float>& getRandom() const
			{
				return randomVariable;
			}
			
			
			/// Set the random variable used to generate samples for this distribution.
			OM_INLINE void getRandom( const Random<float>& newRandom )
			{
				randomVariable = newRandom;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The lambda parameter of the exponential distribution.
			float lambda;
			
			
			/// The random variable that the exponential distribution uses to generate samples.
			Random<float> randomVariable;
			
			
};




//********************************************************************************
/// A class that generates an exponential probability distribution for 64-bit floats.
template <>
class ExponentialDistribution<double>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an exponential distribution with lambda equal to 1.
			OM_INLINE ExponentialDistribution()
				:	lambda( 1.0 ),
					randomVariable()
			{
			}
			
			
			/// Create an exponential distribution with lambda equal to the specified value.
			OM_INLINE ExponentialDistribution( double newLambda )
				:	lambda( math::max( newLambda, 0.0 ) ),
					randomVariable()
			{
			}
			
			
		//********************************************************************************
		//******	Distribution Sample Generation Method
			
			
			/// Generate a sample from the exponential distribution.
			OM_INLINE double sample()
			{
				return -math::ln( randomVariable.sample( 0.0, 1.0 ) )/lambda;
			}
			
			
		//********************************************************************************
		//******	Lambda Parameter Accessor Methods
			
			
			/// Get the lambda parameter of this exponential distribution.
			OM_INLINE double getLambda() const
			{
				return lambda;
			}
			
			
			/// Set the lambda parameter of this exponential distribution.
			OM_INLINE void setLambda( double newLambda )
			{
				lambda = newLambda;
			}
			
			
		//********************************************************************************
		//******	Random Variable Accessor Methods
			
			
			/// Get the random variable used to generate samples for this distribution.
			OM_INLINE Random<double>& getRandom()
			{
				return randomVariable;
			}
			
			
			/// Get the random variable used to generate samples for this distribution.
			OM_INLINE const Random<double>& getRandom() const
			{
				return randomVariable;
			}
			
			
			/// Set the random variable used to generate samples for this distribution.
			OM_INLINE void setRandom( const Random<double>& newRandom )
			{
				randomVariable = newRandom;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The lambda parameter of the exponential distribution.
			double lambda;
			
			
			/// The random variable that the exponential distribution uses to generate samples.
			Random<double> randomVariable;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_EXPONENTIAL_DISTRIBUTION_H
