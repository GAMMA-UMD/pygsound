/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsSoundSourceCluster.h
 * Contents:    gsound::SoundSourceCluster class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_SOURCE_CLUSTER_H
#define INCLUDE_GSOUND_SOUND_SOURCE_CLUSTER_H


#include "gsInternalConfig.h"


#include "../gsSoundDetector.h"
#include "../gsSoundSource.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that corresponds to a cluster of sound sources in a 3D environment.
class SoundSourceCluster : public SoundDetector
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			GSOUND_INLINE SoundSourceCluster()
				:	merged( false )
			{
			}
			
			
		//********************************************************************************
		//******	Source Accessor Methods
			
			
			/// Return the number of sources that are in this cluster.
			GSOUND_FORCE_INLINE Size getSourceCount() const
			{
				return sources.getSize();
			}
			
			
			/// Return a pointer to the source at the specified index in this cluster.
			GSOUND_FORCE_INLINE SoundSource* getSource( Index sourceIndex ) const
			{
				return sources[sourceIndex].source;
			}
			
			
			/// Return the interpolated detector of the source at the specified index.
			GSOUND_FORCE_INLINE const SoundDetector* getClusteredSource( Index sourceIndex ) const
			{
				return &sources[sourceIndex];
			}
			
			
			/// Return the interpolated position of the source at the specified index.
			GSOUND_FORCE_INLINE const Vector3f& getSourcePosition( Index sourceIndex ) const
			{
				return sources[sourceIndex].getPosition();
			}
			
			
			/// Set the interpolated position of the source at the specified index.
			GSOUND_FORCE_INLINE void setSourcePosition( Index sourceIndex, const Vector3f& newPosition )
			{
				sources[sourceIndex].setPosition( newPosition );
			}
			
			
			/// Add a new source to this cluster.
			GSOUND_INLINE void addSource( SoundSource* newSource )
			{
				sources.add( ClusteredSource( newSource ) );
			}
			
			
			/// Remove a source from this cluster.
			GSOUND_INLINE void removeSource( SoundSource* source )
			{
				for ( Index i = 0; i < sources.getSize(); i++ )
				{
					if ( sources[i].source == source )
						sources.removeAtIndexUnordered( i );
				}
			}
			
			
			/// Remove all sources from this cluster.
			GSOUND_INLINE void clearSources()
			{
				sources.clear();
			}
			
			
		//********************************************************************************
		//******	Centroid Accessor Methods
			
			
			/// Compute and return the centroid of the sources in this cluster.
			GSOUND_INLINE Vector3f getCentroid() const
			{
				if ( sources.getSize() > 0 )
				{
					Vector3f centroid = sources[0].source->getPosition();
					
					for ( Index i = 1; i < sources.getSize(); i++ )
						centroid += sources[i].source->getPosition();
					
					return centroid / Real(sources.getSize());
				}
				else
					return this->getPosition();
			}	
			
			
		//********************************************************************************
		//******	Merged Status Accessor Methods
			
			
			/// Return whether or not this cluster of sources should be merged into one big source.
			GSOUND_INLINE Bool getIsMerged() const
			{
				return merged;
			}
			
			
			/// Set whether or not this cluster of sources should be merged into one big source.
			GSOUND_INLINE void setIsMerged( Bool newIsMerged )
			{
				merged = newIsMerged;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declarations
			
			
			class ClusteredSource : public SoundDetector
			{
				public:
					
					GSOUND_INLINE ClusteredSource( SoundSource* newSource )
						:	SoundDetector( newSource->getPosition(), newSource->getRadius() ),
							source( newSource )
					{
					}
					
					SoundSource* source;
					
			};
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the sources that are part of this cluster.
			ArrayList<ClusteredSource> sources;
			
			
			/// A boolean value indicating whether or not this cluster of sources should be merged into one big source.
			Bool merged;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_SOURCE_CLUSTER_H
