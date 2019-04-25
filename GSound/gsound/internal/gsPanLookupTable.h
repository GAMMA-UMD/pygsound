/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsPanLookupTable.h
 * Contents:    gsound::internal::PanLookupTable class declaration
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


#ifndef INCLUDE_GSOUND_PAN_LOOKUP_TABLE_H
#define INCLUDE_GSOUND_PAN_LOOKUP_TABLE_H


#include "gsInternalConfig.h"


#include "gsObjectSpaceTriangle.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that provides a lookup table for fast stereo directional panning.
template < int numSamples >
class PanLookupTable
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new lookup table.
			GSOUND_INLINE PanLookupTable()
				:	leftTable( util::allocate<Float>(numSamples*numSamples) ),
					rightTable( util::allocate<Float>(numSamples*numSamples) )
			{
				for ( Index i = 0; i < numSamples; i++ )
				{
					const Float x = Float(-1) + Float(2)*Float(i) / (numSamples - 1);
					
					for ( Index j = 0; j < numSamples; j++ )
					{
						const Float y = Float(-1) + Float(2)*Float(j) / (numSamples - 1);
						const Index offset = i*numSamples + j;
						
						const Float angle = Float(0.5)*math::atan2( y, math::abs( x ) ) +
															Float(0.25)*math::pi<Float>();
						
						leftTable[offset] = math::cos( angle );
						rightTable[offset] = math::sin( angle );
					}
				}
			}
			
			
			/// Create a copy of the specified lookup table.
			GSOUND_INLINE PanLookupTable( const PanLookupTable& other )
				:	leftTable( util::allocate<Float>(numSamples*numSamples) ),
					rightTable( util::allocate<Float>(numSamples*numSamples) )
			{
				om::util::copy( leftTable, other.leftTable, numSamples*numSamples );
				om::util::copy( rightTable, other.rightTable, numSamples*numSamples );
			}
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this lookup table, releasing all internal data.
			~PanLookupTable()
			{
				util::deallocate( leftTable );
				util::deallocate( rightTable );
			}
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of another lookup table to this one.
			GSOUND_INLINE PanLookupTable& operator = ( const PanLookupTable& other )
			{
				return *this;
			}
			
			
		//********************************************************************************
		//******	Panning Method
			
			
			/// Approximate the panning for the given normalized direction vector in listener-local coordinates.
			GSOUND_FORCE_INLINE void pan( const Vector3f& direction, Float& left, Float& right ) const
			{
				const Float numSamplesF1 = Float(0.5)*(numSamples - 1);
				
				int i = (int)(numSamplesF1*(direction.z + Float(1)));
				int j = (int)(numSamplesF1*(direction.x + Float(1)));
				int offset = i*numSamples + j;
				
				left = leftTable[offset];
				right = rightTable[offset];
			}
			
			
	private:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// The lookup table for the left channel.
			Float* leftTable;
			
			
			/// The lookup table for the right channel.
			Float* rightTable;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_PAN_LOOKUP_TABLE_H
