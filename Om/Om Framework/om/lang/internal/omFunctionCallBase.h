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

#ifndef INCLUDE_OM_FUNCTION_CALL_BASE_H
#define INCLUDE_OM_FUNCTION_CALL_BASE_H


#include "../omLanguageConfig.h"


#include "../omFunction.h"


//##########################################################################################
//************************  Start Om Language Internal Namespace  **************************
OM_LANGUAGE_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




template <	typename R, 
			typename T1 = Void, 
			typename T2 = Void, 
			typename T3 = Void, 
			typename T4 = Void, 
			typename T5 = Void, 
			typename T6 = Void, 
			typename T7 = Void, 
			typename T8 = Void, 
			typename T9 = Void, 
			typename T10 = Void >
class FunctionCallBase
{
	public:
		
		//********************************************************************************
		//******	Public Type Definitions
			
			
			typedef FunctionCallBase<R,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> ThisType;
			typedef Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )> FunctionType;
			
			typedef R ReturnType;
			
			typedef T1 ParameterType1;
			typedef T2 ParameterType2;
			typedef T3 ParameterType3;
			typedef T4 ParameterType4;
			typedef T5 ParameterType5;
			typedef T6 ParameterType6;
			typedef T7 ParameterType7;
			typedef T8 ParameterType8;
			typedef T9 ParameterType9;
			typedef T10 ParameterType10;
			
			
		//********************************************************************************
		//******	Function Call Execute Method
			
			
			OM_INLINE R operator () ()
			{
				return function( arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10 );
			}
			
			
	protected:
		
		//********************************************************************************
		//******	Protected Constructor
			
			
			OM_INLINE FunctionCallBase( const FunctionType& f,
										T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10 )
				:	function( f ),
					arg1( a1 ), arg2( a2 ), arg3( a3 ), arg4( a4 ), arg5( a5 ), arg6( a6 ), arg7( a7 ), arg8( a8 ), arg9( a9 ), arg10( a10 )
			{
			}
			
			
		//********************************************************************************
		//******	Protected Data Members
			
			
			FunctionType function;
			
			
			T1 arg1;
			T2 arg2;
			T3 arg3;
			T4 arg4;
			T5 arg5;
			T6 arg6;
			T7 arg7;
			T8 arg8;
			T9 arg9;
			T10 arg10;
			
};




template < typename R >
struct FunctionCallBase< R, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void >
{
	protected:
		
		typedef FunctionCallBase<R> ThisType;
		typedef Function<R ()> FunctionType;
		
		typedef R ReturnType;
		
	public:
		
		OM_INLINE R execute()
		{
			return function();
		}
		
		
		OM_INLINE R operator () ()
		{
			return function();
		}
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f )
			:	function( f )
		{
		}
		
		
		
	protected:
		
		
		
		
		FunctionType function;
		
		
};




template < typename R, typename T1 >
struct FunctionCallBase< R, T1, Void, Void, Void, Void, Void, Void, Void, Void, Void >
{
	protected:
		
		typedef FunctionCallBase<R,T1> ThisType;
		typedef Function<R ( T1 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1 );
		}
		
		
		OM_INLINE R operator () ()
		{
			return function( arg1 );
		}
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f, T1 a1 )
			:	function( f ),
				arg1( a1 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		
};




template < typename R, typename T1, typename T2 >
struct FunctionCallBase< R, T1, T2, Void, Void, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2> ThisType;
		typedef Function<R ( T1, T2 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2 );
		}
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2 );
		}
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f, T1 a1, T2 a2 )
			:	function( f ),
				arg1( a1 ),
				arg2( a2 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		
};




template < typename R, typename T1, typename T2, typename T3 >
struct FunctionCallBase< R, T1, T2, T3, Void, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2,T3> ThisType;
		typedef Function<R ( T1, T2, T3 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2, arg3 );
		}
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2, arg3 );
		}
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f, T1 a1, T2 a2, T3 a3 )
			:	function( f ),
				arg1( a1 ),
				arg2( a2 ),
				arg3( a3 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		T3 arg3;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4 >
struct FunctionCallBase< R, T1, T2, T3, T4, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2,T3,T4> ThisType;
		typedef Function<R ( T1, T2, T3, T4 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2, arg3, arg4 );
		}
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2, arg3, arg4 );
		}
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f, T1 a1, T2 a2, T3 a3, T4 a4 )
			:	function( f ),
				arg1( a1 ),
				arg2( a2 ),
				arg3( a3 ),
				arg4( a4 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5 >
struct FunctionCallBase< R, T1, T2, T3, T4, T5, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2,T3,T4,T5> ThisType;
		typedef Function<R ( T1, T2, T3, T4, T5 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2, arg3, arg4, arg5 );
		}
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2, arg3, arg4, arg5 );
		}
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f,
									T1 a1, T2 a2, T3 a3, T4 a4, T5 a5 )
			:	function( f ),
				arg1( a1 ), arg2( a2 ), arg3( a3 ), arg4( a4 ), arg5( a5 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
struct FunctionCallBase< R, T1, T2, T3, T4, T5, T6, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2,T3,T4,T5,T6> ThisType;
		typedef Function<R ( T1, T2, T3, T4, T5, T6 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		typedef T6 ParameterType6;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6 );
		}
		
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6 );
		}
		
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6 )
			:	function( f ),
				arg1( a1 ), arg2( a2 ), arg3( a3 ), arg4( a4 ), arg5( a5 ), arg6( a6 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
struct FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, Void, Void, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2,T3,T4,T5,T6,T7> ThisType;
		typedef Function<R ( T1, T2, T3, T4, T5, T6, T7 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		typedef T6 ParameterType6;
		typedef T7 ParameterType7;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6, arg7 );
		}
		
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6, arg7 );
		}
		
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f,
									T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7 )
			:	function( f ),
				arg1( a1 ), arg2( a2 ), arg3( a3 ), arg4( a4 ), arg5( a5 ), arg6( a6 ), arg7( a7 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
		T7 arg7;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
struct FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8, Void, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2,T3,T4,T5,T6,T7,T8> ThisType;
		typedef Function<R ( T1, T2, T3, T4, T5, T6, T7, T8 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		typedef T6 ParameterType6;
		typedef T7 ParameterType7;
		typedef T8 ParameterType8;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8 );
		}
		
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8 );
		}
		
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f,
									T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8 )
			:	function( f ),
				arg1( a1 ), arg2( a2 ), arg3( a3 ), arg4( a4 ), arg5( a5 ), arg6( a6 ), arg7( a7 ), arg8( a8 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
		T7 arg7;
		T8 arg8;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
struct FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8, T9, Void >
{
	public:
		
		typedef FunctionCallBase<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> ThisType;
		typedef Function<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )> FunctionType;
		
		typedef R ReturnType;
		
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		typedef T6 ParameterType6;
		typedef T7 ParameterType7;
		typedef T8 ParameterType8;
		
		
	public:
		
		OM_INLINE R execute()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9 );
		}
		
		
		OM_INLINE R operator () ()
		{
			return function( arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9 );
		}
		
		
		
		OM_INLINE FunctionCallBase( const FunctionType& f,
									T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9 )
			:	function( f ),
				arg1( a1 ), arg2( a2 ), arg3( a3 ), arg4( a4 ), arg5( a5 ), arg6( a6 ), arg7( a7 ), arg8( a8 ), arg9( a9 )
		{
		}
		
		
		
	protected:
		
		
		FunctionType function;
		
		
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
		T7 arg7;
		T8 arg8;
		T9 arg9;
		
};




//##########################################################################################
//************************  End Om Language Internal Namespace  ****************************
OM_LANGUAGE_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FUNCTION_CALL_BASE_H
