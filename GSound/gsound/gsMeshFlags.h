/*
 * Project:     GSound
 * 
 * File:        gsound/gsMeshFlags.h
 * Contents:    gsound::MeshFlags class declaration
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


#ifndef INCLUDE_GSOUND_PREPROCESSOR_FLAGS_H
#define INCLUDE_GSOUND_PREPROCESSOR_FLAGS_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies boolean configuration flags for sound mesh preprocessing.
class MeshFlags
{
	public:
		
		//********************************************************************************
		//******	Preprocessor Flags Enum Declaration
			
			
			/// An enum that specifies the different sound mesh preprocessing falgs.
			enum Flag
			{
				/// A flag which indicates that diffraction edges should be found for the preprocessed mesh.
				/**
				  * Setting this flag enables 1st order diffraction for a mesh, but high-order diffraction
				  * may not be available with just this option. Diffraction edges are found by looking
				  * at the curvature of the mesh at each edge. Edges where there is high curvature are
				  * marked as diffraction edges.
				  */
				DIFFRACTION_EDGES = (1 << 0),
				
				/// A flag which indicates that diffraction edge visibility should be determined for the preprocessed mesh.
				/**
				  * Setting this flag enables fast high-order diffraction computation by using a static
				  * per-object visiblity graph. Computing this graph may be time consuming due to the
				  * large number of edge pairs that need to be considered and it may take up considerable storage.
				  * For complex models, it may be good to simplify the mesh to reduce the number of edges that
				  * need to be considered.
				  */
				DIFFRACTION_GRAPH = (1 << 1),
				
				/// A flag which indicates whether or not voxel-based mesh simplification should be done.
				/**
				  * If enabled, the mesh is voxelized to the specified resolution,
				  * then retriangulated using the marching cubes algorithm.
				  */
				VOXELIZE = (1 << 2),
				
				/// A flag which indicates whether or not the mesh preprocessor should weld vertices.
				/**
				  * If enabled, the multiple vertices may be combined into one if they are less than
				  * the welding distance away from each other.
				  */
				WELD = (1 << 3),
				
				/// A flag which indicates whether or not mesh surface simplification should be done.
				/**
				  * If enabled, the mesh's surface is simplified based on the simplification tolerance parameter.
				  */
				SIMPLIFY = (1 << 4),
				
				/// A flag indicating whether or not analytical information about the preprocessing system should be output.
				/**
				  * If this flag is set and a corresponding statistics object is set in the request,
				  * the mesh preprocessor provides debug information about the last mesh processed to the user.
				  */
				STATISTICS = (1 << 30),
				
				/// The default flags to use for preprocessing a mesh.
				DEFAULT = DIFFRACTION_EDGES | DIFFRACTION_GRAPH | SIMPLIFY | WELD,
				
				/// The flag value when all flags are not set.
				UNDEFINED = 0
			};
			
			
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new mesh flags object with no flags set.
			GSOUND_INLINE MeshFlags()
				:	flags( UNDEFINED )
			{
			}
			
			
			/// Create a new mesh flags object with the specified flag value initially set.
			GSOUND_INLINE MeshFlags( Flag flag )
				:	flags( flag )
			{
			}
			
			
			/// Create a new mesh flags object with the specified initial combined flags value.
			GSOUND_INLINE MeshFlags( UInt32 newFlags )
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
			GSOUND_INLINE operator UInt32 () const
			{
				return flags;
			}
			
			
		//********************************************************************************
		//******	Name Value Status Accessor Methods
			
			
			/// Return whether or not the specified flag value is set for this flags object.
			GSOUND_INLINE Bool isSet( Flag flag ) const
			{
				return (flags & flag) != UNDEFINED;
			}
			
			
			/// Set whether or not the specified flag value is set for this flags object.
			GSOUND_INLINE void set( Flag flag, Bool newIsSet )
			{
				if ( newIsSet )
					flags |= flag;
				else
					flags &= ~flag;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A value indicating the flags that are set.
			UInt32 flags;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PREPROCESSOR_FLAGS_H
