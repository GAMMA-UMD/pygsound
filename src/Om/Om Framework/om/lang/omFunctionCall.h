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

#ifndef INCLUDE_OM_FUNCTION_CALL_H
#define INCLUDE_OM_FUNCTION_CALL_H


#include "omLanguageConfig.h"


#include "internal/omFunctionCallBase.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// Prototype for the function call object template class.
template < typename Signature >
class FunctionCall;




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 0 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 0 parameters.
template < typename R >
class FunctionCall< R () > : public internal::FunctionCallBase< R >
{
	private:
		
		typedef internal::FunctionCallBase< R > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		
		
		
		OM_INLINE FunctionCall( const FunctionType& f )
			:	BaseType( f )
		{
		}
		
		
};




template < typename R >
OM_INLINE FunctionCall<R ()> bindCall( R (*functionPointer)() )
{
	return FunctionCall<R ()>( bind( functionPointer ) );
}




template < typename ObjectType, typename ObjectType2, typename R >
OM_INLINE Function<R ()> bindCall( R (ObjectType2::*functionPointer)(),
								ObjectType* objectPointer )
{
	return FunctionCall<R ()>( bind( functionPointer, objectPointer ) );
}




template < typename ObjectType, typename ObjectType2, typename R >
OM_INLINE FunctionCall<R ()> bindCall( R (ObjectType2::*functionPointer)() const,
								const ObjectType* objectPointer )
{
	return FunctionCall<R ()>( bind( functionPointer, objectPointer ) );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 1 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 1 parameters.
template < typename R, typename T1 >
class FunctionCall< R ( T1 ) > : public internal::FunctionCallBase< R, T1 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		
		
		OM_INLINE FunctionCall( const FunctionType& f, ParameterType1 a1 )
			:	BaseType( f, a1 )
		{
		}
		
		
};




template < typename R, typename T1 >
OM_INLINE FunctionCall<R ( T1 )> bindCall( R (*functionPointer)( T1 ),
											T1 p1 )
{
	return FunctionCall<R ( T1 )>( bind( functionPointer ), p1 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1 >
OM_INLINE FunctionCall<R ( T1 )> bindCall( R (ObjectType2::*functionPointer)( T1 ),
											ObjectType* objectPointer,
											T1 p1 )
{
	return FunctionCall<R ( T1 )>( bind( functionPointer, objectPointer ), p1 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1 >
OM_INLINE FunctionCall<R ( T1 )> bindCall( R (ObjectType2::*functionPointer)( T1 ) const,
											const ObjectType* objectPointer,
											T1 p1 )
{
	return FunctionCall<R ( T1 )>( bind( functionPointer, objectPointer ), p1 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 2 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 2 parameters.
template < typename R, typename T1, typename T2 >
class FunctionCall< R ( T1, T2 ) > : public internal::FunctionCallBase< R, T1, T2 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		
		
		OM_INLINE FunctionCall( const FunctionType& f, ParameterType1 a1, ParameterType2 a2 )
			:	BaseType( f, a1, a2 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2 >
OM_INLINE FunctionCall<R ( T1, T2 )> bindCall( R (*functionPointer)( T1, T2 ),
												T1 p1, T2 p2 )
{
	return FunctionCall<R ( T1, T2 )>( bind( functionPointer ), p1, p2 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2 >
OM_INLINE FunctionCall<R ( T1, T2 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2 )
{
	return FunctionCall<R ( T1, T2 )>( bind( functionPointer, objectPointer ), p1, p2 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2 >
OM_INLINE FunctionCall<R ( T1, T2 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2 )
{
	return FunctionCall<R ( T1, T2 )>( bind( functionPointer, objectPointer ), p1, p2 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 3 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 3 parameters.
template < typename R, typename T1, typename T2, typename T3 >
class FunctionCall< R ( T1, T2, T3 ) > : public internal::FunctionCallBase< R, T1, T2, T3 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		
		
		OM_INLINE FunctionCall( const FunctionType& f,
							ParameterType1 a1, ParameterType2 a2, ParameterType3 a3 )
			:	BaseType( f, a1, a2, a3 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3 >
OM_INLINE FunctionCall<R ( T1, T2, T3 )> bindCall( R (*functionPointer)( T1, T2, T3 ),
												T1 p1, T2 p2, T3 p3 )
{
	return FunctionCall<R ( T1, T2, T3 )>( bind( functionPointer ), p1, p2, p3 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3 >
OM_INLINE FunctionCall<R ( T1, T2, T3 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3 )
{
	return FunctionCall<R ( T1, T2, T3 )>( bind( functionPointer, objectPointer ), p1, p2, p3 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3 >
OM_INLINE FunctionCall<R ( T1, T2, T3 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3 )
{
	return FunctionCall<R ( T1, T2, T3 )>( bind( functionPointer, objectPointer ), p1, p2, p3 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 4 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 4 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4 >
class FunctionCall< R ( T1, T2, T3, T4 ) > : public internal::FunctionCallBase< R, T1, T2, T3, T4 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3, T4 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		
		
		OM_INLINE FunctionCall( const FunctionType& f,
							ParameterType1 a1, ParameterType2 a2, ParameterType3 a3,
							ParameterType4 a4 )
			:	BaseType( f, a1, a2, a3, a4 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4 )> bindCall( R (*functionPointer)( T1, T2, T3, T4 ),
												T1 p1, T2 p2, T3 p3, T4 p4 )
{
	return FunctionCall<R ( T1, T2, T3, T4 )>( bind( functionPointer ), p1, p2, p3, p4 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4 )
{
	return FunctionCall<R ( T1, T2, T3, T4 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4 )
{
	return FunctionCall<R ( T1, T2, T3, T4 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 5 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 5 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5 >
class FunctionCall< R ( T1, T2, T3, T4, T5 ) > : public internal::FunctionCallBase< R, T1, T2, T3, T4, T5 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3, T4, T5 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		
		
		OM_INLINE FunctionCall( const FunctionType& f,
							ParameterType1 a1, ParameterType2 a2, ParameterType3 a3,
							ParameterType4 a4, ParameterType5 a5 )
			:	BaseType( f, a1, a2, a3, a4, a5 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5 )> bindCall( R (*functionPointer)( T1, T2, T3, T4, T5 ),
												T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5 )>( bind( functionPointer ), p1, p2, p3, p4, p5 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 6 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 6 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
class FunctionCall< R ( T1, T2, T3, T4, T5, T6 ) > : public internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		typedef T6 ParameterType6;
		
		
		OM_INLINE FunctionCall( const FunctionType& f,
							ParameterType1 a1, ParameterType2 a2, ParameterType3 a3,
							ParameterType4 a4, ParameterType5 a5, ParameterType6 a6 )
			:	BaseType( f, a1, a2, a3, a4, a5, a6 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
			typename T7, typename T8, typename T9, typename T10 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6 )> bindCall( R (*functionPointer)( T1, T2, T3, T4, T5, T6 ),
												T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6 )>( bind( functionPointer ), p1, p2, p3, p4, p5, p6 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 7 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 7 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
class FunctionCall< R ( T1, T2, T3, T4, T5, T6, T7 ) > : public internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		typedef T6 ParameterType6;
		typedef T7 ParameterType7;
		
		
		OM_INLINE FunctionCall( const FunctionType& f,
							ParameterType1 a1, ParameterType2 a2, ParameterType3 a3,
							ParameterType4 a4, ParameterType5 a5, ParameterType6 a6,
							ParameterType7 a7 )
			:	BaseType( f, a1, a2, a3, a4, a5, a6, a7 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
			typename T7 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7 )> bindCall( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ),
												T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7 )>( bind( functionPointer ), p1, p2, p3, p4, p5, p6, p7 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 8 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 8 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
class FunctionCall< R ( T1, T2, T3, T4, T5, T6, T7, T8 ) > : public internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
		typedef R ReturnType;
		typedef T1 ParameterType1;
		typedef T2 ParameterType2;
		typedef T3 ParameterType3;
		typedef T4 ParameterType4;
		typedef T5 ParameterType5;
		typedef T6 ParameterType6;
		typedef T7 ParameterType7;
		typedef T8 ParameterType8;
		
		
		OM_INLINE FunctionCall( const FunctionType& f, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8 )
			:	BaseType( f, a1, a2, a3, a4, a5, a6, a7, a8 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
			typename T7, typename T8 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8 )> bindCall( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ),
												T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8 )>( bind( functionPointer ), p1, p2, p3, p4, p5, p6, p7, p8 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7, typename T8 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7, p8 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7, typename T8 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7, p8 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 9 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 9 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
class FunctionCall< R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) > : public internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8, T9 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8, T9 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
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
		
		
		OM_INLINE FunctionCall( const FunctionType& f, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9 )
			:	BaseType( f, a1, a2, a3, a4, a5, a6, a7, a8, a9 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
			typename T7, typename T8, typename T9 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )> bindCall( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ),
												T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )>( bind( functionPointer ), p1, p2, p3, p4, p5, p6, p7, p8, p9 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7, p8, p9 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7, p8, p9 );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Function Call Object With 10 Parameters Class Definition
//############		
//##########################################################################################
//##########################################################################################




/// Specialization of the function call template class for a method with 10 parameters.
template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
class FunctionCall< R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) > : public internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 >
{
	private:
		
		typedef internal::FunctionCallBase< R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 > BaseType;
		
	public:
		
		typedef typename BaseType::FunctionType FunctionType;
		
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
		
		
		OM_INLINE FunctionCall( const FunctionType& f, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10 )
			:	BaseType( f, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 )
		{
		}
		
		
};




template < typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
			typename T7, typename T8, typename T9, typename T10 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )> bindCall( R (*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ),
												T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )>( bind( functionPointer ), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ),
											ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
}




template < typename ObjectType, typename ObjectType2, typename R, typename T1, typename T2, typename T3,
			typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
OM_INLINE FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )> bindCall( R (ObjectType2::*functionPointer)( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ) const,
											const ObjectType* objectPointer,
											T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10 )
{
	return FunctionCall<R ( T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 )>( bind( functionPointer, objectPointer ), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
}





//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_FUNCTION_CALL_H
