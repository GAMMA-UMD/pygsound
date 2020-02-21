/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsDiffusePathInfo.h
 * Contents:    gsound::internal::DiffusePathInfo class declaration
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


#ifndef INCLUDE_GSOUND_DIFFUSE_PATH_INFO_H
#define INCLUDE_GSOUND_DIFFUSE_PATH_INFO_H


#include "gsInternalConfig.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores accumulated information about a particular set of diffuse paths.
class DiffusePathInfo
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new diffuse path info object with the specified path hash code and ray contribution attributes.
			GSOUND_INLINE DiffusePathInfo( SoundPathHash newPathHash, const FrequencyBandResponse& newResponse,
											const Vector3f& newDirection, const Vector3f& newSourceDirection,
											Real newDistance, Real newRelativeSpeed, Index newTimeStamp )
				:	pathHash( newPathHash ),
					numPathRays( 1 ),
					totalNumRays( 0 ),
					timeStamp( newTimeStamp ),
					response( newResponse ),
					direction( newDirection ),
					sourceDirection( newSourceDirection ),
					distance( newDistance ),
					relativeSpeed( newRelativeSpeed )
			{
			}
			
			
		//********************************************************************************
		//******	Update Method
			
			
			/// Add a contribution with the given attributes to this path info.
			GSOUND_INLINE void addContribution( const FrequencyBandResponse& newResponse,
												const Vector3f& newDirection, const Vector3f& newSourceDirection,
												Real newDistance, Real newRelativeSpeed, Index newTimeStamp )
			{
				numPathRays++;
				timeStamp = (UInt32)newTimeStamp;
				response += newResponse;
				direction += newDirection;
				sourceDirection += newSourceDirection;
				distance += newDistance;
				relativeSpeed += newRelativeSpeed;
			}
			
			
		//********************************************************************************
		//******	Sound Path Hash Code Accessor Methods
			
			
			/// Return an integer representing a semi-unique ID for this diffuse path.
			GSOUND_FORCE_INLINE SoundPathHash getHashCode() const
			{
				return pathHash;
			}
			
			
			/// Set an integer representing a semi-unique ID for this diffuse path.
			GSOUND_FORCE_INLINE void setHashCode( SoundPathHash newPathHash )
			{
				pathHash = newPathHash;
			}
			
			
		//********************************************************************************
		//******	Path Ray Accessor Methods
			
			
			/// Return the number of rays that have traveled along this diffuse path.
			GSOUND_INLINE Size getRayCount() const
			{
				return numPathRays;
			}
			
			
			/// Return the number of rays that have traveled along this diffuse path.
			GSOUND_INLINE void setRayCount( Size newNumPathRays )
			{
				numPathRays = newNumPathRays;
			}
			
			
		//********************************************************************************
		//******	Total Ray Accessor Methods
			
			
			/// Return the total number of rays that have been traced while this path has been valid.
			GSOUND_INLINE Size getTotalRayCount() const
			{
				return totalNumRays;
			}
			
			
			/// Set the total number of rays that have been traced while this path has been valid.
			GSOUND_INLINE void setTotalRayCount( Size newTotalNumRays )
			{
				totalNumRays = newTotalNumRays;
			}
			
			
		//********************************************************************************
		//******	Response Accessor Methods
			
			
			/// Return the total frequency-dependent energy spectrum for all rays that have traveled this path.
			GSOUND_INLINE const FrequencyBandResponse& getResponse() const
			{
				return response;
			}
			
			
			/// Set the total frequency-dependent energy spectrum for all rays that have traveled this path.
			GSOUND_INLINE void setResponse( const FrequencyBandResponse& newResponse )
			{
				response = newResponse;
			}
			
			
		//********************************************************************************
		//******	Distance Accessor Methods
			
			
			/// Return the total distance along the path for all rays that have traveled this path.
			GSOUND_INLINE Real getDistance() const
			{
				return distance;
			}
			
			
			/// Set the total distance along the path for all rays that have traveled this path.
			GSOUND_INLINE void setDistance( Real newDistance )
			{
				distance = newDistance;
			}
			
			
		//********************************************************************************
		//******	Direction Accessor Methods
			
			
			/// Return the sum of all listener directions (from the listener to the last reflection).
			GSOUND_INLINE const Vector3f& getDirection() const
			{
				return direction;
			}
			
			
			/// Set the sum of all listener directions (from the listener to the last reflection).
			GSOUND_INLINE void setDirection( const Vector3f& newDirection )
			{
				direction = newDirection;
			}
			
			
			/// Return the sum of all source directions (from the source to the first reflection).
			GSOUND_INLINE const Vector3f& getSourceDirection() const
			{
				return sourceDirection;
			}
			
			
			/// Set the sum of all source directions (from the source to the first reflection).
			GSOUND_INLINE void setSourceDirection( const Vector3f& newSourceDirection )
			{
				sourceDirection = newSourceDirection;
			}
			
			
		//********************************************************************************
		//******	Relative Speed Accessor Methods
			
			
			/// Return the total relative speed along the path for all rays that have traveled this path.
			GSOUND_INLINE Real getRelativeSpeed() const
			{
				return relativeSpeed;
			}
			
			
			/// Set the total relative speed along the path for all rays that have traveled this path.
			GSOUND_INLINE void setRelativeSpeed( Real newRelativeSpeed )
			{
				relativeSpeed = newRelativeSpeed;
			}
			
			
		//********************************************************************************
		//******	Timestamp Accessor Methods
			
			
			/// Return the frame timestamp when this path was last updated.
			GSOUND_INLINE Index getTimeStamp() const
			{
				return timeStamp;
			}
			
			
			/// Set the frame timestamp when this path was last updated.
			GSOUND_INLINE void setTimeStamp( Index newTimeStamp )
			{
				timeStamp = newTimeStamp;
			}
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An integer representing a semi-unique ID for this diffuse path.
			SoundPathHash pathHash;
			
			
			/// The number of rays that have traveled along this diffuse path.
			Size numPathRays;
			
			
			/// The total number of rays that have been traced while this path has been valid.
			/**
			  * The total energy for this path is proportional to the number of rays for this
			  * path divided by this value.
			  */
			Size totalNumRays;
			
			
			/// The frame timestamp when this path was last updated.
			Index timeStamp;
			
			
			/// The total frequency-dependent energy spectrum for all rays that have traveled this path.
			FrequencyBandResponse response;
			
			
			/// The sum of all listener directions (from the listener to the last reflection).
			Vector3f direction;
			
			
			/// The sum of all source directions (from the source to the first reflection).
			Vector3f sourceDirection;
			
			
			/// The total distance along the path for all rays that have traveled this path.
			Real distance;
			
			
			/// The total relative speed along the path for all rays that have traveled this path.
			Real relativeSpeed;
			
			
			
};




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_DIFFUSE_PATH_INFO_H
