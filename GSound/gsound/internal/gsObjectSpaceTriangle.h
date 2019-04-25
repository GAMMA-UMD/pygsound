/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsObjectSpaceTriangle.h
 * Contents:    gsound::internal::ObjectSpaceTriangle class declaration
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


#ifndef INCLUDE_GSOUND_OBJECT_SPACE_TRIANGLE_H
#define INCLUDE_GSOUND_OBJECT_SPACE_TRIANGLE_H


#include "gsInternalConfig.h"


#include "../gsSoundObject.h"
#include "gsInternalSoundTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that holds a pointer to a triangle and SoundObject.
/**
  * An ObjectSpaceTriangle is used to keep track of which SoundObject a particular
  * object-space triangle belongs to, so that it can be later transformed into
  * world space for further processing.
  */
class ObjectSpaceTriangle
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create an ObjectSpaceTriangle object which doesn't point to any triangle or object.
			/**
			  * Both triangle and object pointers are set to NULL.
			  */
			GSOUND_INLINE ObjectSpaceTriangle()
				:	triangle( NULL ),
					object( NULL )
			{
			}
			
			
			/// Create an ObjectSpaceTriangle object which points to the specified triangle/object pair.
			GSOUND_INLINE ObjectSpaceTriangle( const InternalSoundTriangle* newTriangle, const SoundObject* newObject )
				:	triangle( newTriangle ),
					object( newObject )
			{
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not this ObjectSpaceTriangle is equal to another.
			/**
			  * Two ObjectSpaceTriangle objects are equal if and only if both the triangle
			  * and object pointers are equal.
			  * 
			  * @param other - the ObjectSpaceTriangle to test for equality with this triangle.
			  * @return whether or not this ObjectSpaceTriangle is equal to another.
			  */
			GSOUND_INLINE Bool operator == ( const ObjectSpaceTriangle& other ) const
			{
				return triangle == other.triangle && object == other.object;
			}
			
			
			/// Return whether or not this ObjectSpaceTriangle is not equal to another.
			/**
			  * Two ObjectSpaceTriangle objects are not equal if and only if either the triangle
			  * or object pointers are not equal.
			  * 
			  * @param other - the ObjectSpaceTriangle to test for inequality with this triangle.
			  * @return whether or not this ObjectSpaceTriangle is not equal to another.
			  */
			GSOUND_INLINE Bool operator != ( const ObjectSpaceTriangle& other ) const
			{
				return triangle != other.triangle || object != other.object;
			}
			
			
		//********************************************************************************
		//******	Hash Code Accessor Method
			
			
			/// Return a hash code for this ObjectSpaceTriangle.
			GSOUND_INLINE SoundPathHash getHashCode() const
			{
				return (PointerInt(object)*triangle->getHashCode()*Hash(0x8DA6B343));
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// A pointer to the object-space triangle that this object represents.
			const InternalSoundTriangle* triangle;
			
			
			/// A pointer to the SoundObject that contains this object's triangle.
			const SoundObject* object;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_OBJECT_SPACE_TRIANGLE_H
