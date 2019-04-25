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

#ifndef INCLUDE_OM_TUPLE_H
#define INCLUDE_OM_TUPLE_H


#include "omLanguageConfig.h"


//##########################################################################################
//****************************  Start Om Language Namespace  *******************************
OM_LANGUAGE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Tuple Accessor Class Template
//############		
//##########################################################################################
//##########################################################################################




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7, Index index >
class TupleIndex;




//##########################################################################################
//##########################################################################################
//############		
//############		7-Element Tuple Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void,
			typename T5 = Void, typename T6 = Void, typename T7 = Void >
class Tuple
{
	public:
		
		OM_INLINE Tuple()
			:	v1(), v2(), v3(), v4(), v5(), v6(), v7()
		{
		}
		
		
		OM_INLINE Tuple( const T1& p1, const T2& p2, const T3& p3, const T4& p4,
						const T5& p5, const T6& p6, const T7& p7 )
			:	v1( p1 ), v2( p2 ), v3( p3 ), v4( p4 ), v5( p5 ), v6( p6 ), v7( p7 )
		{
		}
		
		
		template < Index index >
		OM_INLINE typename TupleIndex<T1,T2,T3,T4,T5,T6,T7,index>::T& get()
		{
			return TupleIndex<T1,T2,T3,T4,T5,T6,T7,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE const typename TupleIndex<T1,T2,T3,T4,T5,T6,T7,index>::T& get() const
		{
			return TupleIndex<T1,T2,T3,T4,T5,T6,T7,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE void set( const typename TupleIndex<T1,T2,T3,T4,T5,T6,T7,index>::T& value )
		{
			TupleIndex<T1,T2,T3,T4,T5,T6,T7,index>::set(*this,value);
		}
		
		
		T1 v1;
		T2 v2;
		T3 v3;
		T4 v4;
		T5 v5;
		T6 v6;
		T7 v7;
		
};



//##########################################################################################
//##########################################################################################
//############		
//############		1-Element Tuple Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T1 >
class Tuple<T1, Void, Void, Void, Void, Void, Void>
{
	public:
		
		
		OM_INLINE Tuple()
			:	v1()
		{
		}
		
		
		OM_INLINE Tuple( const T1& p1 )
			:	v1( p1 )
		{
		}
		
		
		template < Index index >
		OM_INLINE typename TupleIndex<T1,Void, Void, Void, Void, Void, Void,index>::T& get()
		{
			return TupleIndex<T1,Void, Void, Void, Void, Void, Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE const typename TupleIndex<T1,Void, Void, Void, Void, Void, Void,index>::T& get() const
		{
			return TupleIndex<T1,Void, Void, Void, Void, Void, Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE void set( const typename TupleIndex<T1,Void, Void, Void, Void, Void, Void,index>::T& value )
		{
			TupleIndex<T1,Void, Void, Void, Void, Void, Void,index>::set(*this,value);
		}
		
		
		T1 v1;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		2-Element Tuple Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T1, typename T2 >
class Tuple<T1, T2, Void, Void, Void, Void, Void>
{
	public:
		
		OM_INLINE Tuple()
			:	v1(), v2()
		{
		}
		
		
		OM_INLINE Tuple( const T1& p1, const T2& p2 )
			:	v1( p1 ), v2( p2 )
		{
		}
		
		
		template < Index index >
		OM_INLINE typename TupleIndex<T1,T2,Void,Void,Void,Void,Void,index>::T& get()
		{
			return TupleIndex<T1,T2,Void,Void,Void,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE const typename TupleIndex<T1,T2,Void,Void,Void,Void,Void,index>::T& get() const
		{
			return TupleIndex<T1,T2,Void,Void,Void,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE void set( const typename TupleIndex<T1,T2,Void,Void,Void,Void,Void,index>::T& value )
		{
			TupleIndex<T1,T2,Void,Void,Void,Void,Void,index>::set(*this,value);
		}
		
		
		T1 v1;
		T2 v2;
		
		
};



//##########################################################################################
//##########################################################################################
//############		
//############		3-Element Tuple Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T1, typename T2, typename T3 >
class Tuple<T1, T2, T3, Void, Void, Void, Void>
{
	public:
		
		
		OM_INLINE Tuple()
			:	v1(), v2(), v3()
		{
		}
		
		
		OM_INLINE Tuple( const T1& p1, const T2& p2, const T3& p3 )
			:	v1( p1 ), v2( p2 ), v3( p3 )
		{
		}
		
		
		template < Index index >
		OM_INLINE typename TupleIndex<T1,T2,T3,Void,Void,Void,Void,index>::T& get()
		{
			return TupleIndex<T1,T2,T3,Void,Void,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE const typename TupleIndex<T1,T2,T3,Void,Void,Void,Void,index>::T& get() const
		{
			return TupleIndex<T1,T2,T3,Void,Void,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE void set( const typename TupleIndex<T1,T2,T3,Void,Void,Void,Void,index>::T& value )
		{
			TupleIndex<T1,T2,T3,Void,Void,Void,Void,index>::set(*this,value);
		}
		
		
		T1 v1;
		T2 v2;
		T3 v3;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		4-Element Tuple Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T1, typename T2, typename T3, typename T4 >
class Tuple<T1, T2, T3, T4, Void, Void, Void>
{
	public:
		
		OM_INLINE Tuple()
			:	v1(), v2(), v3(), v4()
		{
		}
		
		
		OM_INLINE Tuple( const T1& p1, const T2& p2, const T3& p3, const T4& p4 )
			:	v1( p1 ), v2( p2 ), v3( p3 ), v4( p4 )
		{
		}
		
		
		template < Index index >
		OM_INLINE typename TupleIndex<T1,T2,T3,T4,Void,Void,Void,index>::T& get()
		{
			return TupleIndex<T1,T2,T3,T4,Void,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE const typename TupleIndex<T1,T2,T3,T4,Void,Void,Void,index>::T& get() const
		{
			return TupleIndex<T1,T2,T3,T4,Void,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE void set( const typename TupleIndex<T1,T2,T3,T4,Void,Void,Void,index>::T& value )
		{
			TupleIndex<T1,T2,T3,T4,Void,Void,Void,index>::set(*this,value);
		}
		
		
		T1 v1;
		T2 v2;
		T3 v3;
		T4 v4;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		5-Element Tuple Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T1, typename T2, typename T3, typename T4, typename T5 >
class Tuple<T1, T2, T3, T4, T5, Void, Void>
{
	public:
		
		
		OM_INLINE Tuple()
			:	v1(), v2(), v3(), v4(), v5()
		{
		}
		
		
		OM_INLINE Tuple( const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5 )
			:	v1( p1 ), v2( p2 ), v3( p3 ), v4( p4 ), v5( p5 )
		{
		}
		
		
		template < Index index >
		OM_INLINE typename TupleIndex<T1,T2,T3,T4,T5,Void,Void,index>::T& get()
		{
			return TupleIndex<T1,T2,T3,T4,T5,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE const typename TupleIndex<T1,T2,T3,T4,T5,Void,Void,index>::T& get() const
		{
			return TupleIndex<T1,T2,T3,T4,T5,Void,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE void set( const typename TupleIndex<T1,T2,T3,T4,T5,Void,Void,index>::T& value )
		{
			TupleIndex<T1,T2,T3,T4,T5,Void,Void,index>::set(*this,value);
		}
		
		
		T1 v1;
		T2 v2;
		T3 v3;
		T4 v4;
		T5 v5;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		6-Element Tuple Class
//############		
//##########################################################################################
//##########################################################################################




template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
class Tuple<T1, T2, T3, T4, T5, T6, Void>
{
	public:
		
		
		OM_INLINE Tuple()
			:	v1(), v2(), v3(), v4(), v5(), v6()
		{
		}
		
		
		OM_INLINE Tuple( const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6 )
			:	v1( p1 ), v2( p2 ), v3( p3 ), v4( p4 ), v5( p5 ), v6( p6 )
		{
		}
		
		
		template < Index index >
		OM_INLINE typename TupleIndex<T1,T2,T3,T4,T5,T6,Void,index>::T& get()
		{
			return TupleIndex<T1,T2,T3,T4,T5,T6,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE const typename TupleIndex<T1,T2,T3,T4,T5,T6,Void,index>::T& get() const
		{
			return TupleIndex<T1,T2,T3,T4,T5,T6,Void,index>::get(*this);
		}
		
		
		template < Index index >
		OM_INLINE void set( const typename TupleIndex<T1,T2,T3,T4,T5,T6,Void,index>::T& value )
		{
			TupleIndex<T1,T2,T3,T4,T5,T6,Void,index>::set(*this,value);
		}
		
		
		T1 v1;
		T2 v2;
		T3 v3;
		T4 v4;
		T5 v5;
		T6 v6;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Tuple Accessor Class Template
//############		
//##########################################################################################
//##########################################################################################




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
class TupleIndex<T1,T2,T3,T4,T5,T6,T7,0>
{
	public:
		
		typedef T1 T;
		
		OM_INLINE static T& get( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v1;
		}
		
		OM_INLINE static const T& get( const Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v1;
		}
		
		OM_INLINE static void set( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple, const T& value )
		{
			tuple.v1 = value;
		}
		
};




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
class TupleIndex<T1,T2,T3,T4,T5,T6,T7,1>
{
	public:
		
		typedef T2 T;
		
		OM_INLINE static T& get( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v2;
		}
		
		OM_INLINE static const T& get( const Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v2;
		}
		
		OM_INLINE static void set( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple, const T& value )
		{
			tuple.v2 = value;
		}
		
};




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
class TupleIndex<T1,T2,T3,T4,T5,T6,T7,2>
{
	public:
		
		typedef T3 T;
		
		OM_INLINE static T& get( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v3;
		}
		
		OM_INLINE static const T& get( const Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v3;
		}
		
		OM_INLINE static void set( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple, const T& value )
		{
			tuple.v3 = value;
		}
		
};




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
class TupleIndex<T1,T2,T3,T4,T5,T6,T7,3>
{
	public:
		
		typedef T4 T;
		
		OM_INLINE static T& get( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v4;
		}
		
		OM_INLINE static const T& get( const Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v4;
		}
		
		OM_INLINE static void set( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple, const T& value )
		{
			tuple.v4 = value;
		}
		
};




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
class TupleIndex<T1,T2,T3,T4,T5,T6,T7,4>
{
	public:
		
		typedef T5 T;
		
		OM_INLINE static T& get( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v5;
		}
		
		OM_INLINE static const T& get( const Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v5;
		}
		
		OM_INLINE static void set( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple, const T& value )
		{
			tuple.v5 = value;
		}
		
};




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
class TupleIndex<T1,T2,T3,T4,T5,T6,T7,5>
{
	public:
		
		typedef T6 T;
		
		OM_INLINE static T& get( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v6;
		}
		
		OM_INLINE static const T& get( const Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v6;
		}
		
		OM_INLINE static void set( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple, const T& value )
		{
			tuple.v6 = value;
		}
		
};




template < typename T1, typename T2, typename T3, typename T4,
			typename T5, typename T6, typename T7 >
class TupleIndex<T1,T2,T3,T4,T5,T6,T7,6>
{
	public:
		
		typedef T7 T;
		
		OM_INLINE static T& get( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v7;
		}
		
		OM_INLINE static const T& get( const Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple )
		{
			return tuple.v7;
		}
		
		OM_INLINE static void set( Tuple<T1,T2,T3,T4,T5,T6,T7>& tuple, const T& value )
		{
			tuple.v7 = value;
		}
		
};





//##########################################################################################
//****************************  End Om Language Namespace  *********************************
OM_LANGUAGE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_TUPLE_H
