/*
 * Project:     GSound
 * 
 * File:        gsound/gsIRMetrics.h
 * Contents:    gsound::IRMetrics class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_RAY_H
#define INCLUDE_GSOUND_SOUND_RAY_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class SoundObject;

namespace internal
{
	class InternalSoundTriangle;
};



//********************************************************************************
/// A class that represents a ray intersection query for a sound scene.
class OM_ALIGN(16) SoundRay : public BVHRay
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new uninitialized ray.
			GSOUND_FORCE_INLINE SoundRay()
				:	BVHRay()
			{
			}
			
			
			/// Construct a sound ray and initialize it for the specified ray.
			/**
			  * The ray intersection query considers the distance range [0,infinity].
			  */
			GSOUND_FORCE_INLINE SoundRay( const Ray3f& ray )
				:	BVHRay( ray )
			{
			}
			
			
			/// Construct a sound ray and initialize it for the specified ray information.
			GSOUND_FORCE_INLINE SoundRay( const Ray3f& ray, Float newTMin, Float newTMax )
				:	BVHRay( ray, newTMin, newTMax )
			{
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to the object that was intersected.
			SoundObject* object;
			
			
			/// A pointer to the triangle that was intersected.
			const internal::InternalSoundTriangle* triangle;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_RAY_H
