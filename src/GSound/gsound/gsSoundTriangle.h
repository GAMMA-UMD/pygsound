/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundTriangle.h
 * Contents:    gsound::SoundTriangle class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_TRIANGLE_H
#define INCLUDE_GSOUND_SOUND_TRIANGLE_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A simple indexed triangle class that is used by the user to specify sound mesh geometry.
/**
  * When submitting a triangle mesh to the sound propagation system, the user
  * should provide a list of vertex coordinates and a list of SoundTriangle objects
  * that correspond to the model's triangles. This class stores 3 vertex indices
  * and a material index for the triangle.
  */
class SoundTriangle
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a sound triangle with the specified vertex and material indices.
			GSOUND_INLINE SoundTriangle( Index v1, Index v2, Index v3, Index newMaterialIndex )
				:	materialIndex( newMaterialIndex )
			{
				v[0] = v1;
				v[1] = v2;
				v[2] = v3;
			}
			
			
		//********************************************************************************
		//******	Equality Comparison Operators
			
			
			/// Return whether or not the vertex and material indices of this triangle are equal to another's.
			/**
			  * Order of the indicies is not important: any two triangles with the same vertex 
			  * and material indices, regardless of their order, are equal.
			  */
			GSOUND_INLINE Bool operator == ( const SoundTriangle& other ) const
			{
				if ( materialIndex != other.materialIndex )
					return false;
				
				if ( v[0] == other.v[0] )
					return (v[1] == other.v[1] && v[2] == other.v[2]) || (v[1] == other.v[2] && v[2] == other.v[1]);
				else if ( v[0] == other.v[1] )
					return (v[1] == other.v[0] && v[2] == other.v[2]) || (v[1] == other.v[2] && v[2] == other.v[0]);
				else if ( v[0] == other.v[2] )
					return (v[1] == other.v[0] && v[2] == other.v[1]) || (v[1] == other.v[1] && v[2] == other.v[0]);
				
				return false;
			}
			
			
			/// Return whether or not the vertex and material indices of this triangle are not equal to another's.
			/**
			  * Order of the indicies is not important: any two triangles without the same vertex 
			  * and material indices, regardless of their order, are not equal.
			  */
			GSOUND_INLINE Bool operator != ( const SoundTriangle& other ) const
			{
				return !(*this == other);
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The 0-offset indices of the triangle's vertices within a mesh.
			Index v[3];
			
			
			/// The 0-offset index of the triangle's material within a mesh.
			Index materialIndex;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_TRIANGLE_H
