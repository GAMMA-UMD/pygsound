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

#ifndef INCLUDE_OM_FUNCTION_DEFINITION_H
#define INCLUDE_OM_FUNCTION_DEFINITION_H


#include "../omLanguageConfig.h"


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
class FunctionDefinition
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual Void operator () ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R >
class FunctionDefinition< R, Void, Void, Void, Void, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () () const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1 >
class FunctionDefinition< R, T1, Void, Void, Void, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2 >
class FunctionDefinition< R, T1, T2, Void, Void, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2, typename T3 >
class FunctionDefinition< R, T1, T2, T3, Void, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2, T3 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4 >
class FunctionDefinition< R, T1, T2, T3, T4, Void, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3,T4> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2, T3, T4 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5 >
class FunctionDefinition< R, T1, T2, T3, T4, T5, Void, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3,T4,T5> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2, T3, T4, T5 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
class FunctionDefinition< R, T1, T2, T3, T4, T5, T6, Void, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3,T4,T5,T6> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2, T3, T4, T5, T6 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
class FunctionDefinition< R, T1, T2, T3, T4, T5, T6, T7, Void, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2, T3, T4, T5, T6, T7 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
class FunctionDefinition< R, T1, T2, T3, T4, T5, T6, T7, T8, Void, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7,T8> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2, T3, T4, T5, T6, T7, T8 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
class FunctionDefinition< R, T1, T2, T3, T4, T5, T6, T7, T8, T9, Void >
{
	public:
		
		typedef FunctionDefinition<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> ThisType;
		
		
		virtual ~FunctionDefinition() {}
		
		virtual R operator () ( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) const = 0;
		
		virtual Bool equals( const ThisType& other ) const = 0;
		
		virtual ThisType* clone() const = 0;
		
};




//##########################################################################################
//************************  End Om Language Internal Namespace  ****************************
OM_LANGUAGE_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FUNCTION_DEFINITION_H
