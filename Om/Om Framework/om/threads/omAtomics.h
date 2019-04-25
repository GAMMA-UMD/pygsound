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

#ifndef INCLUDE_OM_ATOMICS_H
#define INCLUDE_OM_ATOMICS_H


#include "omThreadsConfig.h"


//##########################################################################################
//****************************  Start Om Threads Namespace  ********************************
OM_THREADS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




namespace atomic
{


//##########################################################################################
//##########################################################################################
//############		
//############		Set Method Declarations
//############		
//##########################################################################################
//##########################################################################################




/// Atomically set the operand to the given value and return the previous operand value.
template < typename T >
T readAndSet( T& operand, T value );




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Operate-Then-Read Method Declarations
//############		
//##########################################################################################
//##########################################################################################





template < typename T >
T addAndRead( T& operand, T value );




template < typename T >
T incrementAndRead( T& operand );




template < typename T >
T subAndRead( T& operand, T value );




template < typename T >
T decrementAndRead( T& operand );




template < typename T >
T orAndRead( T& operand, T value );




template < typename T >
T andAndRead( T& operand, T value );




template < typename T >
T xorAndRead( T& operand, T value );




template < typename T >
T nandAndRead( T& operand, T value );




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Read-Then-Operate Method Declarations
//############		
//##########################################################################################
//##########################################################################################




template < typename T >
T readAndIncrement( T& operand );




template < typename T >
T readAndDecrement( T& operand );




template < typename T >
T readAndAdd( T& operand, T value );




template < typename T >
T readAndSub( T& operand, T value );




template < typename T >
T readAndOr( T& operand, T value );




template < typename T >
T readAndAnd( T& operand, T value );




template < typename T >
T readAndXor( T& operand, T value );




template < typename T >
T readAndNand( T& operand, T value );




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Comparison Method Declarations
//############		
//##########################################################################################
//##########################################################################################




/// Test and return whether or not the operand is equal to the comparison value. If so, set the operand to the new value.
template < typename T >
Bool testAndSet( T& operand, T compareValue, T newValue );




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Operate-Then-Read Methods (GCC)
//############		
//##########################################################################################
//##########################################################################################


#if defined(OM_COMPILER_GCC)



template < typename T >
OM_INLINE T readAndSet( T& operand, T value )
{
	__sync_synchronize();
	
	T oldValue = operand;
	operand = value;
	
	__sync_synchronize();
	
	return oldValue;
}



#endif


//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Operate-Then-Read Methods (GCC)
//############		
//##########################################################################################
//##########################################################################################


#if defined(OM_COMPILER_GCC)


template < typename T >
OM_INLINE T addAndRead( T& operand, T value )
{
	return __sync_add_and_fetch( &operand, value );
}




template < typename T >
OM_INLINE T incrementAndRead( T& operand )
{
	return __sync_add_and_fetch( &operand, T(1) );
}




template < typename T >
OM_INLINE T subAndRead( T& operand, T value )
{
	return __sync_sub_and_fetch( &operand, value );
}




template < typename T >
OM_INLINE T decrementAndRead( T& operand )
{
	return __sync_sub_and_fetch( &operand, T(1) );
}




template < typename T >
OM_INLINE T orAndRead( T& operand, T value )
{
	return __sync_or_and_fetch( &operand, value );
}




template < typename T >
OM_INLINE T andAndRead( T& operand, T value )
{
	return __sync_and_and_fetch( &operand, value );
}




template < typename T >
OM_INLINE T xorAndRead( T& operand, T value )
{
	return __sync_xor_and_fetch( &operand, value );
}




template < typename T >
OM_INLINE T nandAndRead( T& operand, T value )
{
	return __sync_nand_and_fetch( &operand, value );
}



#elif defined(OM_COMPILER_MSVC)



template < typename T >
OM_INLINE T subAndRead( T& operand, T value )
{
	return addAndRead( operand, -value );
}



#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Operate-Then-Read Methods (GCC)
//############		
//##########################################################################################
//##########################################################################################


#if defined(OM_COMPILER_GCC)


template < typename T >
OM_INLINE T readAndIncrement( T& operand )
{
	return __sync_fetch_and_add( &operand, T(1) );
}




template < typename T >
OM_INLINE T readAndAdd( T& operand, T value )
{
	return __sync_fetch_and_add( &operand, value );
}




template < typename T >
OM_INLINE T readAndSub( T& operand, T value )
{
	return __sync_fetch_and_sub( &operand, value );
}




template < typename T >
OM_INLINE T readAndDecrement( T& operand )
{
	return __sync_fetch_and_sub( &operand, T(1) );
}




template < typename T >
OM_INLINE T readAndOr( T& operand, T value )
{
	return __sync_fetch_and_or( &operand, value );
}




template < typename T >
OM_INLINE T readAndAnd( T& operand, T value )
{
	return __sync_fetch_and_and( &operand, value );
}




template < typename T >
OM_INLINE T readAndXor( T& operand, T value )
{
	return __sync_fetch_and_xor( &operand, value );
}




template < typename T >
OM_INLINE T readAndNand( T& operand, T value )
{
	return __sync_fetch_and_nand( &operand, value );
}



#elif defined(OM_COMPILER_MSVC)



template < typename T >
OM_INLINE T readAndSub( T& operand, T value )
{
	return readAndAdd( operand, -value );
}



#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Comparison Methods (GCC)
//############		
//##########################################################################################
//##########################################################################################


#if defined(OM_COMPILER_GCC)


template < typename T >
OM_INLINE Bool testAndSet( T& operand, T compareValue, T newValue )
{
	return __sync_val_compare_and_swap( &operand, compareValue, newValue );
}


#endif


}; // End namespace atomic




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Class Declaration
//############		
//##########################################################################################
//##########################################################################################




/// A class that wraps a primitive-type value in atomic operations.
template < typename T >
class Atomic
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new atomic variable with the default initial value.
			OM_FORCE_INLINE Atomic()
			{
				atomic::readAndSet( value, T(0) );
			}
			
			
			/// Create a new atomic variable with the specified initial value.
			OM_FORCE_INLINE Atomic( T newValue )
			{
				atomic::readAndSet( value, newValue );
			}
			
			
		//********************************************************************************
		//******	Conversion Operators
			
			
			/// Return the current un-boxed atomic value.
			OM_FORCE_INLINE operator T () const
			{
				return value;
			}
			
			
		//********************************************************************************
		//******	Increment Operators
			
			
			/// Prefix increment operator.
			OM_INLINE T operator ++ ()
			{
				return atomic::incrementAndRead( value );
			}
			
			
			/// Postfix increment operator.
			OM_INLINE T operator ++ ( int )
			{
				return atomic::readAndIncrement( value );
			}
			
			
		//********************************************************************************
		//******	Decrement Operators
			
			
			/// Prefix increment operator.
			OM_INLINE T operator -- ()
			{
				return atomic::decrementAndRead( value );
			}
			
			
			/// Postfix increment operator.
			OM_INLINE T operator -- ( int )
			{
				return atomic::readAndDecrement( value );
			}
			
			
		//********************************************************************************
		//******	Add and Subtract Operators
			
			
			/// Add the specified value to this atomic value, returning the result.
			OM_INLINE T operator += ( T a )
			{
				return atomic::addAndRead( value, a );
			}
			
			
			/// Subtract the specified value from this atomic value, returning the result.
			OM_INLINE T operator -= ( T a )
			{
				return atomic::subAndRead( value, a );
			}
			
			
		//********************************************************************************
		//******	Bitwise Operators
			
			
			/// Bitwise AND the specified value and this atomic value, returning the result.
			OM_INLINE T operator &= ( T a )
			{
				return atomic::andAndRead( value, a );
			}
			
			
			/// Bitwise OR the specified value and this atomic value, returning the result.
			OM_INLINE T operator |= ( T a )
			{
				return atomic::orAndRead( value, a );
			}
			
			
			/// Bitwise XOR the specified value and this atomic value, returning the result.
			OM_INLINE T operator ^= ( T a )
			{
				return atomic::orAndRead( value, a );
			}
			
			
		//********************************************************************************
		//******	Comparison Operations
			
			
			/// Return whether or not the atomic value was set to the comparison value. If so, set the atomic to the new value.
			OM_INLINE Bool testAndSet( T compareValue = T(0), T newValue = T(1) )
			{
				return atomic::testAndSet( value, compareValue, newValue );
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			/// The primitive-typed value stored by this atomic object.
			T value;
			
			
};




//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_ATOMICS_H
