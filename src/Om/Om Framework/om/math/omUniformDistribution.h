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

#ifndef INCLUDE_OM_UNIFORM_DISTRIBUTION_H
#define INCLUDE_OM_UNIFORM_DISTRIBUTION_H


#include "omMathConfig.h"


#include "omRandom.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a uniform probability distribution of the templated type.
template < typename T >
class UniformDistribution
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a uniform distribution on the interval [0,1].
			OM_INLINE UniformDistribution()
				:	minimum( 0 ),
					maximum( 1 ),
					randomVariable()
			{
			}
			
			
			/// Create a uniform distribution on the interval [0,1] with the specified random variable.
			/**
			  * The created uniform distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE UniformDistribution( const Random<T>& newRandom )
				:	minimum( 0 ),
					maximum( 1 ),
					randomVariable( newRandom )
			{
			}
			
			
			/// Create a uniform distribution on the interval [ min, max ].
			OM_INLINE UniformDistribution( T min, T max )
				:	minimum( min ),
					maximum( max ),
					randomVariable()
			{
			}
			
			
			/// Create a uniform distribution on the interval [ min, max ] using the specified random variable.
			/**
			  * The created uniform distribution will produce samples using the
			  * specified random variable.
			  */
			OM_INLINE UniformDistribution( T min, T max, const Random<T>& newRandom )
				:	minimum( min ),
					maximum( max ),
					randomVariable( newRandom )
			{
			}
			
			
		//********************************************************************************
		//******	Distribution Sample Generation Method
			
			
			/// Generate a sample from the uniform distribution.
			OM_INLINE T sample()
			{
				return randomVariable.sample( minimum, maximum );
			}
			
			
		//********************************************************************************
		//******	Distribution Minimum Accessor Methods
			
			
			/// Get the minimum value that the uniform distribution can generate.
			OM_INLINE T getMinimum() const
			{
				return minimum;
			}
			
			
			/// Set the minimum value that the uniform distribution can generate.
			OM_INLINE void setMinimum( T min )
			{
				minimum = min;
			}
			
			
		//********************************************************************************
		//******	Distribution Maximum Accessor Methods
			
			
			/// Get the maximum value that the uniform distribution can generate.
			OM_INLINE T getMaximum() const
			{
				return maximum;
			}
			
			
			/// Set the maximum value that the uniform distribution can generate.
			OM_INLINE void setMaximum( T max )
			{
				maximum = max;
			}
			
			
		//********************************************************************************
		//******	Random Variable Accessor Methods
			
			
			/// Get the random variable used to generate samples for this distribution.
			OM_INLINE Random<T>& getRandom()
			{
				return randomVariable;
			}
			
			
			/// Get the random variable used to generate samples for this distribution.
			OM_INLINE const Random<T>& getRandom() const
			{
				return randomVariable;
			}
			
			
			/// Set the random variable used to generate samples for this distribution.
			OM_INLINE void setRandom( const Random<T>& newRandom )
			{
				randomVariable = newRandom;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The minimum value that can be generated by the distribution.
			T minimum;
			
			
			/// The maximum value that can be generated by the distribution.
			T maximum;
			
			
			/// The random variable that the uniform distribution uses to generate samples.
			Random<T> randomVariable;
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_NORMAL_DISTRIBUTION_H
