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

#ifndef INCLUDE_OM_SIMD_FLAGS_H
#define INCLUDE_OM_SIMD_FLAGS_H


#include "omSIMDConfig.h"


//##########################################################################################
//******************************  Start Om Math Namespace  *********************************
OM_MATH_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that contains flags that specify the type of SIMD operations supported.
/**
  * These flags allow the user to determine at runtime the capabilities of the CPU,
  * and to then choose one code path or another based on the result.
  */
class SIMDFlags
{
	public:
		
		//********************************************************************************
		//******	SIMD Flags Enum Declaration
			
			
			/// An enum that specifies the different SIMD flags specifing the CPU capabilities.
			enum Flag
			{
				/// A flag indicating that SSE is supported by the CPU.
				SSE = (1 << 0),
				
				/// A flag indicating that SSE2 is supported by the CPU.
				SSE_2 = (1 << 1),
				
				/// A flag indicating that SSE3 is supported by the CPU.
				SSE_3 = (1 << 2),
				
				/// A flag indicating that SSSE3 is supported by the CPU.
				SSSE_3 = (1 << 3),
				
				/// A flag indicating that SSE 4.1 is supported by the CPU.
				SSE_4_1 = (1 << 4),
				
				/// A flag indicating that SSE 4.2 is supported by the CPU.
				SSE_4_2 = (1 << 5),
				
				/// A flag indicating that SSE 4.1 and SSE 4.2 is supported by the CPU.
				SSE_4 = SSE_4_1 | SSE_4_2,
				
				/// A flag indicating that AVX is supported by the CPU.
				AVX = (1 << 6),
				
				/// A flag indicating that AVX2 is supported by the CPU.
				AVX_2 = (1 << 7),
				
				/// A flag indicating that AVX-512F is supported by the CPU.
				AVX_512F = (1 << 8),
				
				/// A flag indicating that AVX-512PF is supported by the CPU.
				AVX_512PF = (1 << 9),
				
				/// A flag indicating that AVX-512ER is supported by the CPU.
				AVX_512ER = (1 << 10),
				
				/// A flag indicating that AVX-512CD is supported by the CPU.
				AVX_512CD = (1 << 11),
				
				//********************************************************************************
				
				/// A flag indicating that the ARM NEON SIMD is supported by the CPU.
				ARM_NEON = (1 << 30),
				
				//********************************************************************************
				
				/// A flag indicating that Altivec is supported by the CPU.
				ALTIVEC = (1 << 31),
				
				//********************************************************************************
				
				/// The flag value when no flags are set.
				UNDEFINED = 0
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new SIMD flags object with no flags set.
			OM_INLINE SIMDFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new SIMD flags object with the specified flag value initially set.
			OM_INLINE SIMDFlags( Flag flag )
				:	flags( flag )
			{
			}
			
			
			/// Create a new SIMD flags object with the specified initial combined flags value.
			OM_INLINE SIMDFlags( UInt32 newFlags )
				:	flags( newFlags )
			{
			}
			
			
		//********************************************************************************
		//******	Integer Cast Operator
			
			
			/// Convert this flags object to an integer value.
			/**
			  * This operator is provided so that the object
			  * can be used as an integer value for bitwise logical operations.
			  */
			OM_INLINE operator UInt32 () const
			{
				return flags;
			}
			
			
		//********************************************************************************
		//******	Flag Is Set Accessor Methods
			
			
			/// Return whether or not the specified flag value is set for this flags object.
			OM_INLINE Bool isSet( Flag flag ) const
			{
				return (flags & flag) != UNDEFINED;
			}
			
			
			/// Set whether or not the specified flag value is set for this flags object.
			OM_INLINE void set( Flag flag, Bool newIsSet )
			{
				if ( newIsSet )
					flags |= flag;
				else
					flags &= ~flag;
			}
			
			
		//********************************************************************************
		//******	Current CPU Flags Accessor
			
			
			/// Return an object containing the SIMD flags for the current CPU.
			static SIMDFlags get();
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A value indicating the flags that are set.
			UInt32 flags;
			
			
			
};




//##########################################################################################
//******************************  End Om Math Namespace  ***********************************
OM_MATH_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SIMD_FLAGS_H
