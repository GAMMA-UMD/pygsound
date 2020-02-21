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

#ifndef INCLUDE_OM_CONFIG_H
#define INCLUDE_OM_CONFIG_H


#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <new>
#include <limits>


// Get rid of defined junk symbols.
#if defined(BIG_ENDIAN)
	#undef BIG_ENDIAN
#endif


//##########################################################################################
//##########################################################################################
//############		
//############		Library Debugging Configuration
//############		
//##########################################################################################
//##########################################################################################




#ifndef OM_DEBUG
	#if defined(_DEBUG)
		/// Define whether or not internal debugging checks should be force-enabled.
		#define OM_DEBUG
	#endif
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Library Configuration
//############		
//##########################################################################################
//##########################################################################################




/// Define whether or not to turn off all assertion (including assertions active during release-mode builds).
#ifndef OM_DISABLE_ASSERTIONS
	#define OM_DISABLE_ASSERTIONS 0
#endif




/// The function to use for all library memory allocations.
#ifndef OM_MALLOC
	#define OM_MALLOC(X) (std::malloc(X))
#endif




/// The function to use for all library memory deallocations.
#ifndef OM_FREE
	#define OM_FREE(X) (std::free(X))
#endif



/// Define a macro that converts its argument to its literal string representation.
#ifndef OM_STRINGIFY
	#ifndef OM_STRINGIFIY_WRAPPER
		#define OM_STRINGIFIY_WRAPPER( x ) #x
	#endif
	
	#define OM_STRINGIFY( x ) OM_STRINGIFIY_WRAPPER(x)
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Library Platform Configuration
//############		
//##########################################################################################
//##########################################################################################




#if defined(__APPLE__)
	#define OM_PLATFORM_APPLE
	
	// Fix Apple's headers that redefine altivec functions that are internally defined in GCC.
	#undef __APPLE_ALTIVEC__
#elif defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WINDOWS__)
	#define OM_PLATFORM_WINDOWS
#elif defined(__linux) || defined(__unix) || defined(__posix)
	#define OM_PLATFORM_LINUX
#endif




#if defined( __GNUC__ )
	#define OM_COMPILER_GCC __GNUC__
	#define OM_COMPILER OM_COMPILER_GCC
	#define OM_GCC_VERSION( Major, Minor ) (Major*10000 + Minor*100 )
	#define OM_COMPILER_VERSION OM_GCC_VERSION( __GNUC__, __GNUC_MINOR__ )
	
#elif defined( _MSC_VER )
	#define OM_COMPILER_MSVC _MSC_VER
	#define OM_COMPILER OM_COMPILER_MSVC
	#define OM_COMPILER_VERSION _MSC_VER
	
#else
	#error Unsupported compiler.
#endif



#if defined( OM_COMPILER_GCC )
	
	#ifdef OM_DEBUG
		#define OM_INLINE inline
		#define OM_FORCE_INLINE inline
	#else
		#define OM_INLINE inline
		#define OM_FORCE_INLINE __attribute__((__always_inline__)) inline
	#endif
	
	#define OM_NO_INLINE __attribute__((noinline))
	
	#define OM_EXPORT __attribute__((visibility("default")))
	
	#define OM_ALIGN(alignment) __attribute__((aligned(alignment)))
	
	#if defined(OM_PLATFORM_APPLE)
        #if !defined(__MAC_10_6)
			#define OM_ALIGNED_MALLOC( size, alignment ) (OM_MALLOC( size ))
        #else
			namespace om { namespace util {

			OM_FORCE_INLINE void* posix_memalign_wrapper( size_t size, size_t alignment )
			{
				void* pointer;
				posix_memalign( &pointer, alignment, size );
				return pointer;
			}

			}; };

			#include <malloc/malloc.h>
			#define OM_ALIGNED_MALLOC( size, alignment ) (posix_memalign_wrapper( size, alignment ))

        #endif
	#else
		#define OM_ALIGNED_MALLOC( size, alignment ) (OM_MALLOC( size ))
	#endif
	
	#define OM_ALIGNED_FREE(X) (std::free(X))
	
	#define OM_DEPRECATED __attribute__((deprecated))
	
	#define OM_RESTRICT __restrict
	
	#ifdef __GXX_RTTI
		#define OM_RUNTIME_TYPE_INFO
	#endif
	
	#if __LP64__
		#define OM_PLATFORM_64_BIT
	#else
		#define OM_PLATFORM_32_BIT
	#endif

#elif defined( OM_COMPILER_MSVC )
	
	#include <malloc.h>
	#include <intrin.h>
	
	#ifdef OM_DEBUG
		#define OM_INLINE inline
		#define OM_FORCE_INLINE inline
	#else
		#if OM_COMPILER_VERSION >= 1200
			#define OM_INLINE inline
			#define OM_FORCE_INLINE __forceinline
		#else
			#define OM_INLINE inline
			#define OM_FORCE_INLINE inline
		#endif
	#endif
	
	#define OM_NO_INLINE __declspec(noinline)
	
	#define OM_EXPORT __declspec(dllexport)
	
	#define OM_ALIGN(alignment) __declspec(align(alignment))
	
	#define OM_ALIGNED_MALLOC( size, alignment ) (_aligned_malloc( size, alignment ))
	
	#define OM_ALIGNED_FREE(X) (_aligned_free(X))
	
	#define OM_DEPRECATED __declspec(deprecated)
	
	#define OM_RESTRICT __restrict
	
	#ifdef _CPPRTTI
		#define OM_RUNTIME_TYPE_INFO
	#endif
	
	#if defined(_WIN64)
		#define OM_PLATFORM_64_BIT
	#else
		#define OM_PLATFORM_32_BIT
	#endif
	
#else
	
	/// The inlining procedure to use for methods that should be inlined.
	#define OM_INLINE inline
	#define OM_FORCE_INLINE inline
	#define OM_NO_INLINE 
	
	/// The alignment declaration to be use when aligning structure/class members.
	#define OM_ALIGN(alignment) 
	
	#define OM_EXPORT 
	
	/// The function to use when allocating aligned blocks of memory.
	#define OM_ALIGNED_MALLOC( size, alignment ) (OM_MALLOC(size))
	
	/// The function to use when freeing an aligned block of memory.
	#define OM_ALIGNED_FREE(X) (OM_FREE(X))
	
	/// The marker that is used when defining methods, types, and variables as deprecated.
	#define OM_DEPRECATED 
	
	/// A parameter attribute that indicates a pointer is not aliased.
	#define OM_RESTRICT 
	
#endif




#if defined(_M_PPC) || defined(__ppc__) || defined(__POWERPC__)
	/// The Motorola/IBM PowerPC architecture.
	#define OM_CPU_POWER_PC
	
#elif defined(_M_X64) || defined(_M_IX86) || defined(__x86_64__) || defined(__i386__)
	/// The Intel x86/x64 architecture.
	#define OM_CPU_INTEL
	
#elif defined(__arm__)
	/// The ARM CPU architecture.
	#define OM_CPU_ARM
	
#else
	#error Unsupported CPU type.
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Library Assertion Configuration
//############		
//##########################################################################################
//##########################################################################################



#if !OM_DISABLE_ASSERTIONS

	// The assertion to use in only debug builds.
	#ifdef OM_DEBUG
		// Make sure that debugging is enabled.
		#ifdef NDEBUG
			#undef NDEBUG

			#include <assert.h>
			#define OM_DEBUG_ASSERT(X) assert(X)
			#define OM_DEBUG_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
			#define OM_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %X\n",MESSAGE,(int)CODE)); assert(X); }

			#define NDEBUG
		#else
			#include <assert.h>
			#define OM_DEBUG_ASSERT(X) assert(X)
			#define OM_DEBUG_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
			#define OM_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %X\n",MESSAGE,(int)CODE)); assert(X); }
		#endif
	#else
		#define OM_DEBUG_ASSERT(X) ((void)0)
		#define OM_DEBUG_ASSERT_MESSAGE(X, MESSAGE) ((void)0)
		#define OM_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) ((void)0)
	#endif


	// The assertion to use for release builds.
	#ifdef NDEBUG
		#undef NDEBUG
		#include <assert.h>
		#define OM_ASSERT(X) assert(X)
		#define OM_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
		#define OM_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %X\n",MESSAGE,(int)CODE)); assert(X); }
		#define NDEBUG
	#else
		#include <assert.h>
		#define OM_ASSERT(X) assert(X)
		#define OM_ASSERT_MESSAGE(X, MESSAGE) { ( X ? 0 : std::printf("%s\n",MESSAGE)); assert(X); }
		#define OM_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) { ( X ? 0 : \
					std::printf("%s\nError Code: %X\n",MESSAGE,(int)CODE)); assert(X); }
	#endif
	
#else

	// Define dummy assertion macros if assertions are disabled.
	#define OM_ASSERT(X) ((void)0)
	#define OM_ASSERT_MESSAGE(X, MESSAGE) ((void)0)
	#define OM_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) ((void)0)
	#define OM_DEBUG_ASSERT(X) ((void)0)
	#define OM_DEBUG_ASSERT_MESSAGE(X, MESSAGE) ((void)0)
	#define OM_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) ((void)0)

#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Library Endian-ness Configuration
//############		
//##########################################################################################
//##########################################################################################




#if	defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__) || defined(__BIG_ENDIAN__)
	#define OM_BIG_ENDIAN
#else
	#define OM_LITTLE_ENDIAN
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Define NULL
//############		
//##########################################################################################
//##########################################################################################




#ifndef NULL
	#ifndef __cplusplus
		#define NULL ((void *)0)
	#else
		#define NULL 0
	#endif
#endif




//##########################################################################################
//##########################################################################################
//############		
//############		Library Namespace Configuration
//############		
//##########################################################################################
//##########################################################################################




#define OM_NAMESPACE_START		namespace om {
#define OM_NAMESPACE_END		};




/// The enclosing namespace for the entire Om library.
/**
  * @namespace om
  */
namespace om
{
	/// A namespace containing data manipulation classes: strings, buffers, and hashing.
	/**
	  * @namespace om::data
	  */
	namespace data {};
	#define OM_DATA_NAMESPACE_START			namespace om { namespace data {
	#define OM_DATA_NAMESPACE_END				}; };
	
	
	/// A namespace containing various types of commonly used exception classes.
	/**
	  * @namespace om::exceptions
	  */
	namespace exceptions {};
	#define OM_EXCEPTIONS_NAMESPACE_START		namespace om { namespace exceptions {
	#define OM_EXCEPTIONS_NAMESPACE_END		}; };
	
	
	/// A namespace containing classes that provide ways to access and manipulate file systems.
	/**
	  * @namespace om::fs
	  */
	namespace fs {};
	#define OM_FILE_SYSTEM_NAMESPACE_START		namespace om { namespace fs {
	#define OM_FILE_SYSTEM_NAMESPACE_END		}; };
	
	
	/// A namespace containing classes that provide functionality for streaming data input and output.
	/**
	  * @namespace om::io
	  */
	namespace io {};
	#define OM_IO_NAMESPACE_START				namespace om { namespace io {
	#define OM_IO_NAMESPACE_END				}; };
	
	
	/// A namespace containing classes that extend the basic functionality of C++.
	/**
	  * @namespace om::lang
	  */
	namespace lang
	{
		/// A namespace containing classes that handle animation in a dynamic scene.
		/**
		  * @namespace om::lang::internal
		  */
		namespace internal {};
		#define OM_LANGUAGE_INTERNAL_NAMESPACE_START		namespace om { namespace lang { namespace internal {
		#define OM_LANGUAGE_INTERNAL_NAMESPACE_END			}; }; };
	};
	#define OM_LANGUAGE_NAMESPACE_START		namespace om { namespace lang {
	#define OM_LANGUAGE_NAMESPACE_END			}; };
	
	
	/// A namespace containing classes and functions that do fast math operations.
	/**
	  * @namespace om::math
	  */
	namespace math {};
	#define OM_MATH_NAMESPACE_START			namespace om { namespace math {
	#define OM_MATH_NAMESPACE_END				}; };
	
	
	/// A namespace containing classes that manage system resources like CPU, GPU, memory, and displays.
	/**
	  * @namespace om::system
	  */
	namespace system {};
	#define OM_SYSTEM_NAMESPACE_START			namespace om { namespace system {
	#define OM_SYSTEM_NAMESPACE_END			}; };
	
	
	/// A namespace containing classes that provide ways to create, manage, and synchronize threads.
	/**
	  * @namespace om::threads
	  */
	namespace threads {};
	#define OM_THREADS_NAMESPACE_START			namespace om { namespace threads {
	#define OM_THREADS_NAMESPACE_END			}; };
	
	
	/// A namespace containing classes that are related to keeping track of time.
	/**
	  * @namespace om::time
	  */
	namespace time {};
	#define OM_TIME_NAMESPACE_START			namespace om { namespace time {
	#define OM_TIME_NAMESPACE_END				}; };
	
	
	/// A namespace containing data structure classes and memory manipulation functions.
	/**
	  * @namespace om::util
	  */
	namespace util {};
	#define OM_UTILITIES_NAMESPACE_START		namespace om { namespace util {
	#define OM_UTILITIES_NAMESPACE_END			}; };
};




//##########################################################################################
//*******************************  Start Om Namespace  **********************************
OM_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Sized Floating-Point Primitive Type Definitions
//############		
//##########################################################################################
//##########################################################################################




/// The type used to represent a 32-bit floating point number.
typedef float				Float32;




/// The type used to represent a 64-bit floating point number.
typedef double				Float64;




//##########################################################################################
//##########################################################################################
//############		
//############		Sized Integer Primitive Type Definitions
//############		
//##########################################################################################
//##########################################################################################




/// The type used to represent an 8-bit signed integral number.
typedef signed char			Int8;


/// The type used to represent an 8-bit unsigned integral number.
typedef unsigned char		UInt8;




/// The type used to represent a 16-bit signed integral number.
typedef signed short		Int16;


/// The type used to represent a 16-bit unsigned integral number.
typedef unsigned short		UInt16;




/// The type used to represent a 32-bit signed integral number.
typedef signed int			Int32;


/// The type used to represent a 32-bit unsigned integral number.
typedef unsigned int		UInt32;




/// The type used to represent a 64-bit signed integral number.
typedef signed long long	Int64;


/// The type used to represent a 64-bit unsigned integral number.
typedef unsigned long long	UInt64;




//##########################################################################################
//##########################################################################################
//############		
//############		Standard Primitive Type Redefinitions
//############		
//##########################################################################################
//##########################################################################################




/// Redefine the standard 'bool' primitive type to use the library's type naming conventions.
typedef bool Bool;




/// Redefine the standard 'short' primitive type to use the library's type naming conventions.
typedef short				Short;


/// Redefine the standard 'unsigned short' primitive type to use the library's type naming conventions.
typedef unsigned short		UShort;




/// Redefine the standard 'int' primitive type to use the library's type naming conventions.
typedef int					Int;


/// Redefine the standard 'unsigned int' primitive type to use the library's type naming conventions.
typedef unsigned int		UInt;




/// Redefine the standard 'long' primitive type to use the library's type naming conventions.
typedef long				Long;


/// Redefine the standard 'unsigned long' primitive type to use the library's type naming conventions.
typedef unsigned long		ULong;




/// Redefine the standard 'long long' primitive type to use the library's type naming conventions.
typedef long long			LongLong;


/// Redefine the standard 'unsigned long long' primitive type to use the library's type naming conventions.
typedef unsigned long long	ULongLong;




/// Redefine the standard 'float' primitive type to use the library's type naming conventions.
typedef float				Float;




/// Redefine the standard 'double' primitive type to use the library's type naming conventions.
typedef double				Double;




//##########################################################################################
//##########################################################################################
//############		
//############		Application-Specific Primitive Type Definitions
//############		
//##########################################################################################
//##########################################################################################




/// The type to use when holding signed data, should be 8 bits wide.
typedef Int8				Byte;




/// The type to use when holding generic data, should be 8 bits wide.
typedef UInt8				UByte;




/// The type to use when working with ASCII character data.
typedef char				Char;




/// The type to use for hash codes in hash tables, should be an unsigned integer.
typedef UInt32			Hash;




#if defined(OM_PLATFORM_32_BIT)
	/// The type on 32-bit systems that is the same size as a pointer.
	typedef UInt32				PointerInt;
	/// The type that is used to represent a signed index.
	typedef Int32				SignedIndex;
#elif defined(OM_PLATFORM_64_BIT)
	/// The type on 64-bit systems that is the same size as a pointer.
	typedef UInt64				PointerInt;
	/// The type that is used to represent a signed index.
	typedef Int64				SignedIndex;
#else
	/// The type that is the same size as a pointer.
	typedef std::size_t		PointerInt;
	/// The type that is used to represent a signed index.
	typedef long				SignedIndex;
#endif




/// The unsigned size type that is the same width as a memory address.
typedef PointerInt			Size;




/// The type to use for a large-scale size. This is the largest supported unsigned integer type.
typedef UInt64				LargeSize;




/// The type to use to represent an offset (index) in an array or sequence of things.
typedef PointerInt			Index;




/// The type to use for a large-scale index. This is the largest supported unsigned integer type.
typedef UInt64				LargeIndex;



/// The type for a UTF-8 unicode character.
typedef UInt8				UTF8Char;




/// The type for a UTF-16 unicode character.
typedef UInt16				UTF16Char;




/// The type for a UTF-32 unicode character.
typedef UInt32				UTF32Char;




/// The type for a unicode character that can hold the data for any other unicode character type.
typedef UTF32Char			UniChar;




/// An opaque undefined type used to represent an invalid or NULL type.
class Void;




//##########################################################################################
//*******************************  End Om Namespace  ************************************
OM_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_CONFIG_H
