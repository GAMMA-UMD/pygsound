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

#include "omPrimes.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		32-bit Prime Numbers
//############		
//##########################################################################################
//##########################################################################################




static const UInt32 PRIMES_32 [] =
{
	2,				//	Between 2^0 and 2^1
	3,				//	Between 2^1 and 2^2
	5,				//	Between 2^2 and 2^3
	11,				//	Between 2^3 and 2^4
	23,				//	Between 2^4 and 2^5
	53,				//	Between 2^5 and 2^6
	97,				//	Between 2^6 and 2^7
	193,			//	Between 2^7 and 2^8
	389,			//	Between 2^8 and 2^9
	769,			//	Between 2^9 and 2^10
	1543,			//	Between 2^10 and 2^11
	3079,			//	Between 2^11 and 2^12
	6151,			//	Between 2^12 and 2^13
	12289,			//	Between 2^13 and 2^14
	24593,			//	Between 2^14 and 2^15
	49157,			//	Between 2^15 and 2^16
	98317,			//	Between 2^16 and 2^17
	196613,			//	Between 2^17 and 2^18
	393241,			//	Between 2^18 and 2^19
	786433,			//	Between 2^19 and 2^20
	1572869,		//	Between 2^20 and 2^21
	3145739,		//	Between 2^21 and 2^22
	6291469,		//	Between 2^22 and 2^23
	12582917,		//	Between 2^23 and 2^24
	25165843,		//	Between 2^24 and 2^25
	50331653,		//	Between 2^25 and 2^26
	100663319,		//	Between 2^26 and 2^27
	201326611,		//	Between 2^27 and 2^28
	402653189,		//	Between 2^28 and 2^29
	805306457,		//	Between 2^29 and 2^30
	1610612741,		//	Between 2^30 and 2^31
	4294967291ul	//	Between 2^31 and 2^32
};




template < typename T >
OM_FORCE_INLINE static T nextPowerOf2Prime32( T value )
{
	UInt32 v = (UInt32)value;
	
	for ( Index i = 0; i < 32; i++ )
	{
		if ( PRIMES_32[i] > v )
			return (T)PRIMES_32[i];
	}
	
	return value;
}




//##########################################################################################
//##########################################################################################
//############		
//############		64-bit Prime Numbers
//############		
//##########################################################################################
//##########################################################################################




static const UInt64 PRIMES_64 [] =
{
	2ull,			//	Between 2^0 and 2^1
	3ull,			//	Between 2^1 and 2^2
	5ull,			//	Between 2^2 and 2^3
	11ull,			//	Between 2^3 and 2^4
	23ull,			//	Between 2^4 and 2^5
	53ull,			//	Between 2^5 and 2^6
	97ull,			//	Between 2^6 and 2^7
	193ull,			//	Between 2^7 and 2^8
	389ull,			//	Between 2^8 and 2^9
	769ull,			//	Between 2^9 and 2^10
	1543ull,		//	Between 2^10 and 2^11
	3079ull,		//	Between 2^11 and 2^12
	6151ull,		//	Between 2^12 and 2^13
	12289ull,		//	Between 2^13 and 2^14
	24593ull,		//	Between 2^14 and 2^15
	49157ull,		//	Between 2^15 and 2^16
	98317ull,		//	Between 2^16 and 2^17
	196613ull,		//	Between 2^17 and 2^18
	393241ull,		//	Between 2^18 and 2^19
	786433ull,		//	Between 2^19 and 2^20
	1572869ull,		//	Between 2^20 and 2^21
	3145739ull,		//	Between 2^21 and 2^22
	6291469ull,		//	Between 2^22 and 2^23
	12582917ull,	//	Between 2^23 and 2^24
	25165843ull,	//	Between 2^24 and 2^25
	50331653ull,	//	Between 2^25 and 2^26
	100663319ull,	//	Between 2^26 and 2^27
	201326611ull,	//	Between 2^27 and 2^28
	402653189ull,	//	Between 2^28 and 2^29
	805306457ull,	//	Between 2^29 and 2^30
	1610612741ull,	//	Between 2^30 and 2^31
	4294967291ull,	//	Between 2^31 and 2^32
	
	8589934583ull,		//	Between 2^32 and 2^33
	17179869143ull,		//	Between 2^33 and 2^34
	34359738337ull,		//	Between 2^34 and 2^35
	68719476731ull,		//	Between 2^35 and 2^36
	137438953447ull,	//	Between 2^36 and 2^37
	274877906899ull,	//	Between 2^37 and 2^38
	549755813881ull,	//	Between 2^38 and 2^39
	1099511627689ull,	//	Between 2^39 and 2^40
	2199023255531ull,	//	Between 2^40 and 2^41
	4398046511093ull,	//	Between 2^41 and 2^42
	8796093022151ull,	//	Between 2^42 and 2^43
	17592186044399ull,	//	Between 2^43 and 2^44
	35184372088777ull,	//	Between 2^44 and 2^45
	70368744177643ull,	//	Between 2^45 and 2^46
	140737488355213ull,	//	Between 2^46 and 2^47
	281474976710597ull,	//	Between 2^47 and 2^48
	562949953421231ull,	//	Between 2^48 and 2^49
	1125899906842597ull,	//	Between 2^49 and 2^50
	2251799813685119ull,	//	Between 2^50 and 2^51
	4503599627370449ull,	//	Between 2^51 and 2^52
	9007199254740881ull,	//	Between 2^52 and 2^53
	18014398509481951ull,	//	Between 2^53 and 2^54
	36028797018963913ull,	//	Between 2^54 and 2^55
	72057594037927931ull,	//	Between 2^55 and 2^56
	144115188075855859ull,	//	Between 2^56 and 2^57
	288230376151711717ull,	//	Between 2^57 and 2^58
	576460752303423433ull,	//	Between 2^58 and 2^59
	1152921504606846883ull,	//	Between 2^59 and 2^60
	2305843009213693951ull,	//	Between 2^60 and 2^61
	4611686018427387847ull,	//	Between 2^61 and 2^62
	9223372036854775783ull,	//	Between 2^62 and 2^63
	18446744073709551557ull	//	Between 2^63 and 2^64
};




template < typename T >
OM_FORCE_INLINE static T nextPowerOf2Prime64( T value )
{
	for ( Index i = 0; i < 64; i++ )
	{
		if ( PRIMES_64[i] > value )
			return PRIMES_64[i];
	}
	
	return value;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Next Power of 2 Methods
//############		
//##########################################################################################
//##########################################################################################




template <>
unsigned int nextPowerOf2Prime( unsigned int value )
{
	return nextPowerOf2Prime32( value );
}

template <>
unsigned long nextPowerOf2Prime( unsigned long value )
{
	return nextPowerOf2Prime32( value );
}

template <>
unsigned long long nextPowerOf2Prime( unsigned long long value )
{
	return nextPowerOf2Prime64( value );
}




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
