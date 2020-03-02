/*
 * Project:     GSound
 * 
 * File:        gsound/GSoundConfig.h
 * Contents:    Main configuration for the GSound library
 * 
 * Author(s):   Carl Schissler
 * Website:     http://gamma.cs.unc.edu/GSOUND/
 * 
 * License:
 * 
 *     Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
 *     All rights reserved.
 *     
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for educational, research, and non-profit purposes, without
 *     fee, and without a written agreement is hereby granted, provided that the
 *     above copyright notice, this paragraph, and the following four paragraphs
 *     appear in all copies.
 *     
 *     Permission to incorporate this software into commercial products may be
 *     obtained by contacting the University of North Carolina at Chapel Hill.
 *     
 *     This software program and documentation are copyrighted by Carl Schissler and
 *     the University of North Carolina at Chapel Hill. The software program and
 *     documentation are supplied "as is", without any accompanying services from
 *     the University of North Carolina at Chapel Hill or the authors. The University
 *     of North Carolina at Chapel Hill and the authors do not warrant that the
 *     operation of the program will be uninterrupted or error-free. The end-user
 *     understands that the program was developed for research purposes and is advised
 *     not to rely exclusively on the program for any reason.
 *     
 *     IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR ITS
 *     EMPLOYEES OR THE AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE
 *     UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED
 *     OF THE POSSIBILITY OF SUCH DAMAGE.
 *     
 *     THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 *     DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 *     STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS
 *     ON AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND
 *     THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 *     ENHANCEMENTS, OR MODIFICATIONS.
 */


#ifndef INCLUDE_GSOUND_CONFIG_H
#define INCLUDE_GSOUND_CONFIG_H


#include <cstddef>
#include <cstdlib>
#include <cstdio>


#include "om/omFramework.h"
#include "om/omBVH.h"
#include "om/omSound.h"
#include "om/omResources.h"


//##########################################################################################
//##########################################################################################
//############
//############		Library Version Configuration
//############
//##########################################################################################
//##########################################################################################




/// Define an integer representing the major version number of the GSound library.
/**
  * Major version changes indicate large revisions to the overall structure or function
  * of the library and may produce incompatability with code designed to use older
  * versions. Functions or classes that have been deprecated previously may be removed
  * after a major version change.
  */
#define GSOUND_MAJOR_VERSION 1




/// Define an integer representing the minor version number of the GSound library.
/**
  * Minor version changes indicate that functionality has been improved while
  * keeping the existing API, or that new functionality has been added without
  * disrupting older functionality. Minor version changes can also indicate
  * major bug fixes. Old functionality may become deprecated with a minor version
  * update.
  */
#define GSOUND_MINOR_VERSION 0




/// Define an integer representing the revision version number of the GSound library.
/**
  * Library revisions indicate that bug fixes or minor improvements
  * to the backend codebase have occurred. New functionality may also be
  * added that doesn't disrupt existing use of the library.
  */
#define GSOUND_REVISION 0




/// Define an integer number exactly representing the version of the GSound library.
/**
  * This definition is pomarily useful when performing compilation switching
  * based on the version of GSound being used. More recent versions of GSound
  * will always have a version greater than that of older versions.
  */
#define GSOUND_VERSION (GSOUND_MAJOR_VERSION*1000000 + GSOUND_MINOR_VERSION*1000 + GSOUND_REVISION)




#ifndef GSOUND_VERSION_STRING
	/// A constant C-string representing the version of the GSound library.
	/**
	  * The string will be of the format "A.B.C" where A, B, and C are all
	  * positive integers in decimal notation.
	  */
	#define GSOUND_VERSION_STRING "1.0.0"
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Configuration
//############
//##########################################################################################
//##########################################################################################




#ifndef GSOUND_DISABLE_ASSERTIONS
	/// Define whether or not to turn off all assertion (including assertions active during release-mode builds).
	#define GSOUND_DISABLE_ASSERTIONS 0
#endif




#ifndef GSOUND_USE_SIMD
	/// Determine whether or not SIMD code should be used.
	/**
	  * If set to 1, many operations will be parallelized using SIMD vector operations.
	  * This will generally increase performance but may not work on all hardware. If set to
	  * 0, no SIMD operations will be used. If enabled but the hardware doesn't support SIMD
	  * instructions, a serial fallback implementation will be used.
	  */
	#define GSOUND_USE_SIMD 1
#endif




#ifndef GSOUND_SSE_MAX_MAJOR_VERSION
	/// Define the newest major version of SSE that can be used by GSound.
	/**
	  * This value can be used to limit the complexity of the SSE operations
	  * performed when compiling for hardware that doesn't support newer SSE versions.
	  * Only SSE versions up to this version (specified as an integer number)
	  * can be used.
	  */
	#define GSOUND_SSE_MAX_MAJOR_VERSION 3
#endif




#ifndef GSOUND_SSE_MAX_MINOR_VERSION
	/// Define the newest minor version of SSE that can be used by GSound.
	/**
	  * This value can be used to limit the complexity of the SSE operations
	  * performed when compiling for hardware that doesn't support newer SSE versions.
	  * Only SSE versions up to this version (specified as an integer number)
	  * can be used.
	  */
	#define GSOUND_SSE_MAX_MINOR_VERSION 0
#endif




#ifndef GSOUND_USE_OPEN_CL
	/// Determine whether or not OpenCL code should be used.
	/**
	  * If set to 1, GSound will test to see if any OpenCL devices
	  * are available. If so, it will use these devices to perform
	  * various operations. Otherwise, a fallback CPU implementation
	  * will be used.
	  */
	#define GSOUND_USE_OPEN_CL 0
#endif




#ifndef GSOUND_FREQUENCY_COUNT
	/// Define the number of frequencies that are used for sound propagation and rendering.
	/**
	  * This value must be a multiple of 4 in order to allow fast SIMD processing.
	  * There is additional memory and CPU overhead for higher numbers of frequency bands.
	  * The frequency-band filtering CPU cost scales O(N^2) for N bands, memory overhead for IRs is
	  * linear in the number of bands. Too many bands (e.g. more than 8) may result in an
	  * unreasonably large amount of phase distortion due to excessive IIR crossover filtering.
	  */
	#define GSOUND_FREQUENCY_COUNT 8
	
	#if GSOUND_FREQUENCY_COUNT % 4 != 0 || GSOUND_FREQUENCY_COUNT <= 0
		#error frequency count must be a non-zero positive multiple of 4.
	#endif
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Macro Configuration
//############
//##########################################################################################
//##########################################################################################




#define GSOUND_DEBUG OM_DEBUG
#define GSOUND_INLINE OM_INLINE
#define GSOUND_FORCE_INLINE OM_FORCE_INLINE
#define GSOUND_NO_INLINE OM_NO_INLINE
#define GSOUND_EXPORT OM_EXPORT
#define GSOUND_ALIGN OM_ALIGN
#define GSOUND_ALIGNED_MALLOC OM_ALIGNED_MALLOC
#define GSOUND_ALIGNED_FREE OM_ALIGNED_FREE
#define GSOUND_DEPRECATED OM_DEPRECATED




#if !GSOUND_DISABLE_ASSERTIONS
	#define GSOUND_ASSERT(X) OM_ASSERT(X)
	#define GSOUND_ASSERT_MESSAGE(X, MESSAGE) OM_ASSERT_MESSAGE(X, MESSAGE)
	#define GSOUND_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) OM_ASSERT_MESSAGE(X, MESSAGE, CODE)
	#define GSOUND_DEBUG_ASSERT(X) OM_DEBUG_ASSERT(X)
	#define GSOUND_DEBUG_ASSERT_MESSAGE(X, MESSAGE) OM_DEBUG_ASSERT_MESSAGE(X, MESSAGE)
	#define GSOUND_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) OM_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE)
#else
	// Define dummy assertion macros if assertions are disabled.
	#define GSOUND_ASSERT(X) ((void)0)
	#define GSOUND_ASSERT_MESSAGE(X, MESSAGE) ((void)0)
	#define GSOUND_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) ((void)0)
	#define GSOUND_DEBUG_ASSERT(X) ((void)0)
	#define GSOUND_DEBUG_ASSERT_MESSAGE(X, MESSAGE) ((void)0)
	#define GSOUND_DEBUG_ASSERT_MESSAGE_CODE(X, MESSAGE, CODE) ((void)0)
#endif




//##########################################################################################
//##########################################################################################
//############
//############		Library Namespace Configuration
//############
//##########################################################################################
//##########################################################################################




#define GSOUND_NAMESPACE_START		namespace gsound {
#define GSOUND_NAMESPACE_END		};


/// The main namespace for the GSound library.
/**
  * All members of this namespace are considered public. All classes that are necessary for necessary
  * use of the GSound library are either declared in or imported into this namespace.
  *
  * @namespace gsound
  */
namespace gsound
{
	/// A namespace containing internal library functionality.
	/**
	  * The contents of this namespace are not designed to be publicly used and
	  * are subject to change without notice as long as public functionality of
	  * the library is not affected.
	  *
	  * @namespace gsound::internal
	  */
	namespace internal {};
	#define GSOUND_INTERNAL_NAMESPACE_START			namespace gsound { namespace internal {
	#define GSOUND_INTERNAL_NAMESPACE_END			}; };
};




//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############
//############		Namespace Imports
//############
//##########################################################################################
//##########################################################################################



using om::Bool;
using om::Byte;
using om::UByte;
using om::Int;
using om::UInt;
using om::Float;
using om::Double;

using om::Int8;
using om::UInt8;
using om::Int16;
using om::UInt16;
using om::Int32;
using om::UInt32;
using om::Int64;
using om::UInt64;
using om::Float32;
using om::Float64;

using om::Hash;
using om::Size;
using om::Index;
using om::PointerInt;

using om::Time;


using om::bind;
using om::FunctionCall;
using om::Thread;
using om::ThreadPriority;
using om::ThreadPool;
using om::Atomic;
using om::Shared;
using om::PriorityQueue;
using om::math::Triangle;
using om::UTF8String;


/// Define the type to use for a complex-valued audio sample.
typedef om::math::Complex<Float32> ComplexSample;



/// Define the type to be used for all world-space floating-point computations.
typedef Float Real;


/// The type to use for sound path hash codes.
typedef UInt64 SoundPathHash;


using om::math::AABB1f;
using om::math::AABB3f;

using om::math::Vector2f;
using om::math::Vector3f;
using om::math::Vector4f;

using om::math::Matrix3f;
using om::math::Matrix4f;

using om::math::Transform3f;

using om::math::Ray3f;
using om::math::Plane3f;
using om::math::Sphere3f;


using om::math::SH;
using om::math::SHExpansion;


typedef om::math::SIMDScalar<Float32,om::math::SIMDType<Float32>::WIDTH> SIMDFloat;
typedef om::math::SIMDArray<Float32,GSOUND_FREQUENCY_COUNT> SIMDBands;
typedef om::math::SIMDArray<Int32,GSOUND_FREQUENCY_COUNT> SIMDIntBands;
using om::math::SIMDArray;


using om::bvh::BVHRay;
using om::bvh::BVHGeometry;
using om::bvh::BVH;
using om::bvh::BVHInstance;
using om::bvh::AABBTree4;
using om::bvh::BVHScene;


/// Define the type to use for a vertex in a SoundMesh as a 3D vector type.
typedef Vector3f SoundVertex;

using om::math::Vector3f;
using om::math::AABB3f;
using om::math::Sphere3f;
using om::math::Transform3f;



using om::Allocator;
using om::AlignedAllocator;
using om::ArrayList;
using om::ShortArrayList;
using om::Array;
using om::HashMap;
using om::String;

using om::StaticArray;

using om::Mutex;
using om::ScopedMutex;
using om::Signal;
using om::FunctionThread;

using om::Timer;
using om::Time;

using om::Console;


using om::CPU;


using om::sound::SampleRate;
using om::sound::SampleIndex;
using om::sound::SoundSize;
using om::sound::base::Gain;
using om::sound::base::Sample32f;
using om::sound::base::SampleType;
using om::sound::base::SoundInputStream;
using om::sound::base::SoundBuffer;
using om::sound::base::ChannelLayout;
using om::sound::base::ChannelLayoutType;
using om::sound::base::SharedSoundBuffer;
using om::sound::base::SharedBufferPool;
using om::sound::base::HRTF;
using om::sound::base::LerpState;
using om::sound::filters::SoundFilter;
using om::sound::filters::SoundFrame;
using om::sound::filters::SoundResult;
using om::sound::Resampler;


/// A namespace containing math type definitions.
/**
  * This namespace imports the entire om::math namespace.
  */
namespace math
{
	using namespace om::math;
};



/// A namespace containing allocator and utility functions.
namespace util
{
	using om::util::allocate;
	using om::util::deallocate;
	
	using om::util::allocateAligned;
	using om::util::deallocateAligned;
	
	using om::util::construct;
	using om::util::destruct;
	
	using om::util::constructArray;
	using om::util::destructArray;
	using om::util::copyArray;
	using om::util::copyArrayAligned;
};




class SoundDirectivity;



//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_CONFIG_H
