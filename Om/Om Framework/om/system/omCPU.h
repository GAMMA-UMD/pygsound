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

#ifndef INCLUDE_OM_CPU_H
#define INCLUDE_OM_CPU_H


#include "omSystemConfig.h"


//##########################################################################################
//**************************     Start Rim System Namespace     ****************************
OM_SYSTEM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that represents a global interface to the system's memory.
class CPU
{
	public:
		
		//********************************************************************************
		//******	CPU Attribute Accessor Methods
			
			
			/// Return the total number of available hardware execution units on this system.
			static Size getCount();
			
			
			/// Return an object that has flags specifying the SIMD (vector) capabilities of this CPU.
			OM_INLINE static SIMDFlags getSIMDFlags()
			{
				return SIMDFlags::get();
			}
			
			
			
		//********************************************************************************
		//******	CPU Denormal Flush-to-Zero Methods
			
			
			/// Return whether or not the CPU should flush denormal floating point numbers to zero.
			OM_INLINE static Bool getZeroFlush()
			{
#if defined(OM_CPU_POWER_PC)
				return false;
#elif defined(OM_CPU_INTEL)
				return _MM_GET_FLUSH_ZERO_MODE() != 0;
#elif defined(OM_CPU_ARM)
				UInt result;
				#if defined(OM_PLATFORM_64_BIT)
					asm volatile("mrs %[result], FPCR" : [result] "=r" (result));
				#else
					asm volatile("vmrs %[result], FPSCR" : [result] "=r" (result));
				#endif
				// Bit 24 is the flush-to-zero mode control bit. Setting it to 1 flushes denormals to 0.
				return (result & (1 << 24)) != 0;
#else
				return false;
#endif
			}
			
			
			/// Set whether or not the CPU should flush denormal floating point numbers to zero.
			OM_INLINE static void setZeroFlush( Bool flush )
			{
#if defined(OM_CPU_POWER_PC)
				// No need, denormals are fast.
#elif defined(OM_CPU_INTEL)
				// Handle x87 FPU
				UInt old;
				#if defined(OM_COMPILER_GCC)
					// Get the previous status word.
					asm volatile("stmxcsr %0" : "=m" (old));
					// Set the flush bits.
					if ( flush )
						old |= 0x8040;
					else
						old &= ~0x8040;
					// Set the status word.
					asm volatile("ldmxcsr %0" : : "m" (old));
				#elif defined(OM_COMPILER_MSVC)
					_controlfp_s(&old, _DN_SAVE, _MCW_DN);
				#endif
				
				// Handle SSE
				_MM_SET_FLUSH_ZERO_MODE( flush ? _MM_FLUSH_ZERO_ON : _MM_FLUSH_ZERO_OFF );
#elif defined(OM_CPU_ARM)
				// Get the previous status word.
				UInt old;
				#if defined(OM_PLATFORM_64_BIT)
					asm volatile("mrs %[old], FPCR" : [old] "=r" (old));
				#else
					asm volatile("vmrs %[old], FPSCR" : [old] "=r" (old));
				#endif
				
				// Bit 24 is the flush-to-zero mode control bit. Setting it to 1 flushes denormals to 0.
				if ( flush )
					old |= (1 << 24);
				else
					old &= ~(1 << 24);
				
				// Set the status word.
				#if defined(OM_PLATFORM_64_BIT)
					asm volatile("msr FPCR, %[old]" : : [old] "r" (old));
				#else
					asm volatile("vmsr FPSCR, %[old]" : : [old] "r" (old));
				#endif
#else
				// Unknown CPU
#endif
			}
			
			
};




//##########################################################################################
//**************************     End Rim System Namespace     ******************************
OM_SYSTEM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif	// INCLUDE_OM_CPU_H
