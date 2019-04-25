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

#include "omAtomics.h"


#if defined(OM_COMPILER_MSVC) && defined(OM_PLATFORM_WINDOWS)
	#include <Windows.h>
#endif


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
//############		Operate-Then-Read Methods (MSVC)
//############		
//##########################################################################################
//##########################################################################################




#if defined(OM_COMPILER_MSVC)



template <>
Int16 incrementAndRead( Int16& operand )
{
	return _InterlockedIncrement16( (short*)&operand );
}

template <>
UInt16 incrementAndRead( UInt16& operand )
{
	return _InterlockedIncrement16( (short*)&operand );
}

template <>
Int32 incrementAndRead( Int32& operand )
{
	return _InterlockedIncrement( (long*)&operand );
}

template <>
UInt32 incrementAndRead( UInt32& operand )
{
	return _InterlockedIncrement( (long*)&operand );
}

template <>
Int64 incrementAndRead( Int64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedIncrement64( (__int64*)&operand );
#else
	return InterlockedIncrement64( (__int64*)&operand );
#endif
}

template <>
UInt64 incrementAndRead( UInt64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedIncrement64( (__int64*)&operand );
#else
	return InterlockedIncrement64( (__int64*)&operand );
#endif
}




template <>
Int16 decrementAndRead( Int16& operand )
{
	return _InterlockedDecrement16( (short*)&operand );
}

template <>
UInt16 decrementAndRead( UInt16& operand )
{
	return _InterlockedDecrement16( (short*)&operand );
}

template <>
Int32 decrementAndRead( Int32& operand )
{
	return _InterlockedDecrement( (long*)&operand );
}

template <>
UInt32 decrementAndRead( UInt32& operand )
{
	return _InterlockedDecrement( (long*)&operand );
}

template <>
Int64 decrementAndRead( Int64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedDecrement64( (__int64*)&operand );
#else
	return InterlockedDecrement64( (__int64*)&operand );
#endif
}

template <>
UInt64 decrementAndRead( UInt64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedDecrement64( (__int64*)&operand );
#else
	return InterlockedDecrement64( (__int64*)&operand );
#endif
}

#endif // defined(OM_COMPILER_MSVC)




//##########################################################################################
//##########################################################################################
//############		
//############		Read-Then-Operate Methods (MSVC)
//############		
//##########################################################################################
//##########################################################################################




#if defined(OM_COMPILER_MSVC)




template <>
Int32 readAndAdd( Int32& operand, Int32 value )
{
	return _InterlockedExchangeAdd( (long*)&operand, value );
}

template <>
Int64 readAndAdd( Int64& operand, Int64 value )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedExchangeAdd64( (__int64*)&operand, value );
#else
	return InterlockedExchangeAdd64( (__int64*)&operand, value );
#endif
}




template <>
Int32 readAndIncrement( Int32& operand )
{
	return _InterlockedExchangeAdd( (long*)&operand, long(1) );
}

template <>
UInt32 readAndIncrement( UInt32& operand )
{
	return _InterlockedExchangeAdd( (long*)&operand, long(1) );
}





template <>
Int64 readAndIncrement( Int64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedExchangeAdd64( (__int64*)&operand, __int64(1) );
#else
	return InterlockedExchangeAdd64( (__int64*)&operand, __int64(1) );
#endif
}
template <>
UInt64 readAndIncrement( UInt64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedExchangeAdd64( (__int64*)&operand, __int64(1) );
#else
	return InterlockedExchangeAdd64( (__int64*)&operand, __int64(1) );
#endif
}




template <>
Int16 readAndDecrement( Int16& operand )
{
	return _InterlockedDecrement16( (short*)&operand );
}
template <>
UInt16 readAndDecrement( UInt16& operand )
{
	return _InterlockedDecrement16( (short*)&operand );
}




template <>
Int32 readAndDecrement( Int32& operand )
{
	return _InterlockedExchangeAdd( (long*)&operand, long(-1) );
}

template <>
UInt32 readAndDecrement( UInt32& operand )
{
	return _InterlockedExchangeAdd( (long*)&operand, long(-1) );
}




template <>
Int64 readAndDecrement( Int64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedExchangeAdd64( (__int64*)&operand, __int64(-1) );
#else
	return InterlockedExchangeAdd64( (__int64*)&operand, __int64(-1) );
#endif
}

template <>
UInt64 readAndDecrement( UInt64& operand )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedExchangeAdd64( (__int64*)&operand, __int64(-1) );
#else
	return InterlockedExchangeAdd64( (__int64*)&operand, __int64(-1) );
#endif
}




template <>
Int8 readAndOr( Int8& operand, Int8 value )
{
	return _InterlockedOr8( (char*)&operand, value );
}

template <>
Int16 readAndOr( Int16& operand, Int16 value )
{
	return _InterlockedOr16( &operand, value );
}

template <>
Int32 readAndOr( Int32& operand, Int32 value )
{
	return _InterlockedOr( (long*)&operand, value );
}

template <>
Int64 readAndOr( Int64& operand, Int64 value )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedOr64( &operand, value );
#else
	return InterlockedOr64( &operand, value );
#endif
}




template <>
Int8 readAndAnd( Int8& operand, Int8 value )
{
	return _InterlockedAnd8( (char*)&operand, value );
}

template <>
Int16 readAndAnd( Int16& operand, Int16 value )
{
	return _InterlockedAnd16( &operand, value );
}

template <>
Int32 readAndAnd( Int32& operand, Int32 value )
{
	return _InterlockedAnd( (long*)&operand, value );
}

template <>
Int64 readAndAnd( Int64& operand, Int64 value )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedAnd64( &operand, value );
#else
	return InterlockedAnd64( &operand, value );
#endif
}




template <>
Int8 readAndXor( Int8& operand, Int8 value )
{
	return _InterlockedXor8( (char*)&operand, value );
}

template <>
Int16 readAndXor( Int16& operand, Int16 value )
{
	return _InterlockedXor16( &operand, value );
}

template <>
Int32 readAndXor( Int32& operand, Int32 value )
{
	return _InterlockedXor( (long*)&operand, value );
}

template <>
Int64 readAndXor( Int64& operand, Int64 value )
{
#if defined(OM_PLATFORM_64_BIT)
	return _InterlockedXor64( &operand, value );
#else
	return InterlockedXor64( &operand, value );
#endif
}


#endif // defined(OM_COMPILER_MSVC)




//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Comparison Methods (MSVC)
//############		
//##########################################################################################
//##########################################################################################




#if defined(OM_COMPILER_MSVC)




template <>
Bool testAndSet( Int8& operand, Int8 compareValue, Int8 newValue )
{
	return _InterlockedCompareExchange8( (char*)&operand, (char)newValue, (char)compareValue ) == (char)compareValue;
}

template <>
Bool testAndSet( UInt8& operand, UInt8 compareValue, UInt8 newValue )
{
	return _InterlockedCompareExchange8( (char*)&operand, *(char*)&newValue, *(char*)&compareValue ) == *(char*)&compareValue;
}



template <>
Bool testAndSet( Int16& operand, Int16 compareValue, Int16 newValue )
{
	return _InterlockedCompareExchange16( (short*)&operand, (short)newValue, (short)compareValue ) == (short)compareValue;
}

template <>
Bool testAndSet( UInt16& operand, UInt16 compareValue, UInt16 newValue )
{
	return _InterlockedCompareExchange16( (short*)&operand, *(short*)&newValue, *(short*)&compareValue ) == *(short*)&compareValue;
}




template <>
Bool testAndSet( Int32& operand, Int32 compareValue, Int32 newValue )
{
	return _InterlockedCompareExchange( (long*)&operand, (long)newValue, (long)compareValue ) == (long)compareValue;
}

template <>
Bool testAndSet( UInt32& operand, UInt32 compareValue, UInt32 newValue )
{
	return _InterlockedCompareExchange( (long*)&operand, *(long*)&newValue, *(long*)&compareValue ) == *(long*)&compareValue;
}




template <>
Bool testAndSet( Int64& operand, Int64 compareValue, Int64 newValue )
{
	return _InterlockedCompareExchange64( (__int64*)&operand, (__int64)newValue, (__int64)compareValue ) == (long)compareValue;
}

template <>
Bool testAndSet( UInt64& operand, UInt64 compareValue, UInt64 newValue )
{
	return _InterlockedCompareExchange64( (__int64*)&operand, *(__int64*)&newValue, *(__int64*)&compareValue ) == *(__int64*)&compareValue;
}




#endif // defined(OM_COMPILER_MSVC)






//##########################################################################################
//##########################################################################################
//############		
//############		Atomic Operate-Then-Read Methods (GCC)
//############		
//##########################################################################################
//##########################################################################################


#if defined(OM_COMPILER_MSVC)



template <>
Int8 readAndSet( Int8& operand, Int8 value )
{
	return _InterlockedExchange8( (char*)&operand, (char)value );
}

template <>
UInt8 readAndSet( UInt8& operand, UInt8 value )
{
	char result = _InterlockedExchange8( (char*)&operand, *(char*)&value );
	return *(UInt8*)(&result);
}




template <>
Int16 readAndSet( Int16& operand, Int16 value )
{
	return _InterlockedExchange16( (short*)&operand, (short)value );
}

template <>
UInt16 readAndSet( UInt16& operand, UInt16 value )
{
	short result = _InterlockedExchange16( (short*)&operand, *(short*)&value );
	return *(UInt16*)(&result);
}




template <>
Int32 readAndSet( Int32& operand, Int32 value )
{
	return (Int32)_InterlockedExchange( (long*)&operand, (long)value );
}

template <>
UInt32 readAndSet( UInt32& operand, UInt32 value )
{
	long result = _InterlockedExchange( (long*)&operand, *(long*)&value );
	return *(UInt32*)(&result);
}




template <>
Int64 readAndSet( Int64& operand, Int64 value )
{
#if defined(OM_PLATFORM_64_BIT)
	return (Int64)_InterlockedExchange64( (__int64*)&operand, (__int64)value );
#else
	return (Int64)InterlockedExchange64( (__int64*)&operand, (__int64)value );
#endif
}

template <>
UInt64 readAndSet( UInt64& operand, UInt64 value )
{
#if defined(OM_PLATFORM_64_BIT)
	__int64 result = _InterlockedExchange64( (__int64*)&operand, *(__int64*)&value );
#else
	__int64 result = InterlockedExchange64( (__int64*)&operand, *(__int64*)&value );
#endif
	return *(UInt64*)(&result);
}



#endif // defined(OM_COMPILER_MSVC)




};



//##########################################################################################
//****************************  End Om Threads Namespace  **********************************
OM_THREADS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
