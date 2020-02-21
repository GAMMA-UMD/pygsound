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

#ifndef INCLUDE_OM_POISSON_DISTRIBUTION_H
#define INCLUDE_OM_POISSON_DISTRIBUTION_H


#include "omMathConfig.h"


#include "omRandom.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that generates a Poisson probability distribution.
template < typename T >
class PoissonDistribution
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a poisson distribution with parameter lambda equal to 1.
			OM_INLINE PoissonDistribution()
				:	lambda( 1.0 ),
					lambdaExp( math::exp(-1.0) ),
					randomVariable()
			{
			}
			
			
			/// Create a poisson distribution with parameter lambda equal to 1.
			/**
			  * The created Poisson distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE PoissonDistribution( const Random<double>& newRandom )
				:	lambda( 1.0 ),
					lambdaExp( math::exp(-1.0) ),
					randomVariable( newRandom )
			{
			}
			
			
			/// Create a poisson distribution with the specified parameter lambda.
			OM_INLINE PoissonDistribution( double newLambda )
				:	lambda( newLambda ),
					lambdaExp( math::exp(-newLambda) ),
					randomVariable()
			{
			}
			
			
			/// Create a poisson distribution with the specified parameter lambda.
			/**
			  * The created Poisson distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE PoissonDistribution( double newLambda, const Random<double>& newRandom )
				:	lambda( newLambda ),
					lambdaExp( math::exp(-newLambda) ),
					randomVariable( newRandom )
			{
			}
			
			
			/// Create a poisson distribution which approximates a binomial distribution.
			/**
			  * When the parameter n is large and p is small, the Poisson distribution
			  * created will approximate the behavior of a binomial distribution with the
			  * parameters n and p. This can approximate the number of successes in a 
			  * series of n independent yes/no experiments where the probability of a 
			  * success is p.
			  */
			OM_INLINE PoissonDistribution( double n, double p )
				:	lambda( n*p ),
					lambdaExp( math::exp(-n*p) ),
					randomVariable()
			{
			}
			
			
			/// Create a poisson distribution which approximates a binomial distribution.
			/**
			  * When the parameter n is large and p is small, the Poisson distribution
			  * created will approximate the behavior of a binomial distribution with the
			  * parameters n and p. This can approximate the number of successes in a 
			  * series of n independent yes/no experiments where the probability of a 
			  * success is p. The created Poisson distribution will produce samples
			  * using the specified random variable.
			  */
			OM_INLINE PoissonDistribution( double n, double p, const Random<double>& newRandom )
				:	lambda( n*p ),
					lambdaExp( math::exp(-n*p) ),
					randomVariable( newRandom )
			{
			}
			
			
		//********************************************************************************
		//******	Distribution Sample Generation Method
			
			
			/// Generate a sample from the Poisson distribution.
			OM_INLINE T sample()
			{
				T k = 0;
				double p = 1.0;
				
				do
				{
					k++;
					p *= randomVariable.sample( 0.0, 1.0 );
				}
				while ( p >= lambdaExp );
				
				return T(k - 1);
			}
			
			
		//********************************************************************************
		//******	Lambda Parameter Accessor Methods
			
			
			/// Get the lambda parameter of this Poisson distribution.
			OM_INLINE double getLambda() const
			{
				return lambda;
			}
			
			
			/// Set the lambda parameter of this Poisson distribution.
			OM_INLINE void setLambda( double newLambda )
			{
				lambda = newLambda;
				lambdaExp = math::exp(-lambda);
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
			
			
			/// The lambda parameter of the Poisson distribution.
			double lambda;
			
			
			/// e to the power of the lambda parameter of the Poisson distribution.
			double lambdaExp;
			
			
			/// The random variable that the Poisson distribution uses to generate samples.
			Random<double> randomVariable;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_POISSON_DISTRIBUTION_H
