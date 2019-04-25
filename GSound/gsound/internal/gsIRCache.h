/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsIRCache.h
 * Contents:    gsound::internal::IRCache class declaration
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


#ifndef INCLUDE_GSOUND_IR_CACHE_H
#define INCLUDE_GSOUND_IR_CACHE_H


#include "gsInternalConfig.h"


#include "../gsSampledIR.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that caches and smooths changes in the IR computed from frame-to-frame.
/**
  * The output IR is a linear combination of the previous frame's
  * IR and the IR computed on the current frame. The cache acts as a
  * 1st-order low-pass filter on the value of each sample in the IR as it changes
  * over many frames. The result is a much smoother sound field for a
  * stocastic interactive simulation that benefits from many frames worth of
  * computation.
  */
class IRCache
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new IR cache.
			IRCache();
			
			
		//********************************************************************************
		//******	Cache Update Method
			
			
			/// Update the cached IR with the new IR based on the specified IR blending factor.
			/**
			  * This method multiplies the energy in the input/output IR by the specified
			  * energy scale factor. That result is then linearly combined with the cached IR
			  * using the specified blend factor and stored in both the cache and input/output IR.
			  */
			void update( Float blendFactor, Float energyScaleFactor, SampledIR& inputOutputIR );
			
			
		//********************************************************************************
		//******	Cache Accessor Methods
			
			
			/// Return a const reference to the internal cached IR.
			GSOUND_INLINE const SampledIR& getIR() const
			{
				return cachedIR;
			}
			
			
		//********************************************************************************
		//******	Length Accessor Method
			
			
			/// Set the length of the cached IR in samples.
			GSOUND_INLINE void setLengthInSamples( Size newLengthInSamples )
			{
				cachedIR.setLengthInSamples( newLengthInSamples );
			}
			
			
		//********************************************************************************
		//******	Cache Reset Methods
			
			
			/// Clear the contents of this IR cache, reseting it to the default initial state.
			/**
			  * This method doesn't deallocate any memory used for IRs.
			  */
			void clear();
			
			
			/// Clear the contents of this IR cache, reseting it to the default initial state.
			/**
			  * This method deallocates all IR memory.
			  */
			void reset();
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the sample rate of this IR cache in samples per second.
			GSOUND_INLINE SampleRate getSampleRate() const
			{
				return cachedIR.getSampleRate();
			}
			
			
			/// Set the sample rate of this IR cache in samples per second.
			/**
			  * This method causes the IR cache to be reset.
			  */
			GSOUND_INLINE void setSampleRate( SampleRate newSampleRate )
			{
				cachedIR.setSampleRate( newSampleRate );
				cachedIR.clear();
			}
			
			
		//********************************************************************************
		//******	Cache Size Accessor Methods
			
			
			/// Return the approximate size in bytes of the memory used for this IR cache.
			GSOUND_INLINE Size getSizeInBytes() const
			{
				return sizeof(IRCache) + cachedIR.getSizeInBytes();
			}
			
			
	private:
		
		//********************************************************************************
		//******	Cache Update Methods
			
			
			GSOUND_FORCE_INLINE static void updateDirections( Vector3f* cache, Vector3f* output, Float blend, Size length );
			
			
			GSOUND_FORCE_INLINE static void updateIntensity( Float* cache, Float* output, Float blend, Float energyScale, Size length );
			
			
			GSOUND_FORCE_INLINE static void linearCombine( Float* a, Float aWeight, Float* b, Float bWeight, Size length );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// The cached IR that stores the last combined IR computed for a sound source.
			SampledIR cachedIR;
			
			
			/// A boolean value that keeps track of whether or not the cache's first frame has been computed.
			Bool firstFrame;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_IR_CACHE_H
