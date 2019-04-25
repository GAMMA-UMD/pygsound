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

#ifndef INCLUDE_OM_NORMAL_DISTRIBUTION_H
#define INCLUDE_OM_NORMAL_DISTRIBUTION_H


#include "omMathConfig.h"


#include "omRandom.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// A class that generates a normal (gaussian) probability distribution for the templated type.
template < typename T >
class NormalDistribution;



//********************************************************************************
/// A class that generates a normal (gaussian) probability distribution for 32-bit floats.
template <>
class NormalDistribution<float>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a standard normal distribution with mean of 0 and standard deviation 1.
			OM_INLINE NormalDistribution()
				:	mean( 0.0f ),
					standardDeviation( 1.0f ),
					randomVariable()
			{
			}
			
			
			/// Create a standard normal distribution with mean of 0 and standard deviation 1.
			/**
			  * The created normal distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE NormalDistribution( const Random<float>& newRandom )
				:	mean( 0.0f ),
					standardDeviation( 1.0f ),
					randomVariable( newRandom )
			{
			}
			
			/// Create a normal distribution with the specified mean and standard deviation.
			OM_INLINE NormalDistribution( float newMean, float newStandardDeviation )
				:	mean( newMean ),
					standardDeviation( newStandardDeviation ),
					randomVariable()
			{
			}
			
			/// Create a normal distribution with the specified mean and standard deviation.
			/**
			  * The created normal distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE NormalDistribution( float newMean, float newStandardDeviation,
										const Random<float>& newRandom )
				:	mean( newMean ),
					standardDeviation( newStandardDeviation ),
					randomVariable( newRandom )
			{
			}
			
			
		//********************************************************************************
		//******	Distribution Sample Generation Method
			
			
			/// Generate a sample from the normal distribution.
			OM_INLINE float sample()
			{
				float a = randomVariable.sample( 0.0f, 1.0f );
				float b = randomVariable.sample( 0.0f, 1.0f );
				
				// standard normal random sample
				float z = math::sqrt( -2.0f*math::ln(a) ) * math::cos( 2.0f*float(math::pi<float>())*b );
				
				// scale and translate the standard normal to match this distribution.
				return z*standardDeviation + mean;
			}
			
			
		//********************************************************************************
		//******	Distribution Mean Accessor Methods
			
			
			/// Get the mean of the normal distribution.
			OM_INLINE float getMean() const
			{
				return mean;
			}
			
			
			/// Set the mean of the normal distribution.
			OM_INLINE void setMean( float newMean )
			{
				mean = newMean;
			}
			
			
		//********************************************************************************
		//******	Distribution Standard Deviation Accessor Methods
			
			
			/// Get the standard deviation of the normal distribution.
			OM_INLINE float getStandardDeviation() const
			{
				return standardDeviation;
			}
			
			
			/// Set the standard deviation of the normal distribution.
			OM_INLINE void setStandardDeviation( float newStandardDeviation )
			{
				standardDeviation = newStandardDeviation;
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
			OM_INLINE void setRandom( const Random<float>& newRandom )
			{
				randomVariable = newRandom;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members;
			
			
			/// The mean, or average, of the normal distribution.
			float mean;
			
			
			/// The standard deviation of the normal distribution.
			float standardDeviation;
			
			
			/// The random variable that the normal distribution uses to generate samples.
			Random<float> randomVariable;
			
			
};




//********************************************************************************
/// A class that generates a normal (gaussian) probability distribution for 64-bit floats.
template <>
class NormalDistribution<double>
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a standard normal distribution with mean of 0 and standard deviation 1.
			OM_INLINE NormalDistribution()
				:	mean( 0.0f ),
					standardDeviation( 1.0f ),
					randomVariable()
			{
			}
			
			
			/// Create a standard normal distribution with mean of 0 and standard deviation 1.
			/**
			  * The created normal distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE NormalDistribution( const Random<double>& newRandom )
				:	mean( 0.0f ),
					standardDeviation( 1.0f ),
					randomVariable( newRandom )
			{
			}
			
			/// Create a normal distribution with the specified mean and standard deviation.
			OM_INLINE NormalDistribution( double newMean, double newStandardDeviation )
				:	mean( newMean ),
					standardDeviation( newStandardDeviation ),
					randomVariable()
			{
			}
			
			/// Create a normal distribution with the specified mean and standard deviation.
			/**
			  * The created normal distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE NormalDistribution( double newMean, double newStandardDeviation,
										const Random<double>& newRandom )
				:	mean( newMean ),
					standardDeviation( newStandardDeviation ),
					randomVariable( newRandom )
			{
			}
			
			
		//********************************************************************************
		//******	Distribution Sample Generation Method
			
			
			/// Generate a sample from the normal distribution.
			OM_INLINE double sample()
			{
				double a = randomVariable.sample( 0.0f, 1.0f );
				double b = randomVariable.sample( 0.0f, 1.0f );
				
				// standard normal random sample
				double z = math::sqrt( -2.0f*math::ln(a) ) * math::cos( 2.0f*math::pi<double>()*b );
				
				// scale and translate the standard normal to match this distribution.
				return z*standardDeviation + mean;
			}
			
			
		//********************************************************************************
		//******	Distribution Mean Accessor Methods
			
			
			/// Get the mean of the normal distribution.
			OM_INLINE double getMean() const
			{
				return mean;
			}
			
			
			/// Set the mean of the normal distribution.
			OM_INLINE void setMean( double newMean )
			{
				mean = newMean;
			}
			
			
		//********************************************************************************
		//******	Distribution Standard Deviation Accessor Methods
			
			
			/// Get the standard deviation of the normal distribution.
			OM_INLINE double getStandardDeviation() const
			{
				return standardDeviation;
			}
			
			
			/// Set the standard deviation of the normal distribution.
			OM_INLINE void setStandardDeviation( double newStandardDeviation )
			{
				standardDeviation = newStandardDeviation;
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
			OM_INLINE void getRandom( const Random<double>& newRandom )
			{
				randomVariable = newRandom;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members;
			
			
			/// The mean, or average, of the normal distribution.
			double mean;
			
			
			/// The standard deviation of the normal distribution.
			double standardDeviation;
			
			
			/// The random variable that the normal distribution uses to generate samples.
			Random<double> randomVariable;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_NORMAL_DISTRIBUTION_H
