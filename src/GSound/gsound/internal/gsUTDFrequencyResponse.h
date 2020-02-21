/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsUTDFrequencyResponse.h
 * Contents:    A function to compute UTD (Uniform Theory of Diffraction) attenuation coefficients.
 * 
 * Author(s):   Carl Schissler, Micah Taylor, Anish Chandak, Christian Lauterbach
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


#ifndef INCLUDE_GSOUND_UTD_FREQUENCY_RESPONSE_H
#define INCLUDE_GSOUND_UTD_FREQUENCY_RESPONSE_H


#include "gsInternalConfig.h"


#include "../gsFrequencyBandResponse.h"
#include "../gsFrequencyBands.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




/// Create a frequency band response for diffraction under the specified parameters.
/**
  * This constructor uses the UTD formulation for diffraction to calculate the
  * gain coefficients for each frequency band in the resulting frequency band response.
  * It uses code adapted from RESound/iSound, originally written by Micah Taylor,
  * Anish Chandak, and Christian Lauterbach.
  * 
  * @param sourcePosition - the position of the sound source whose sound is diffracting.
  * @param listenerPosition - the position of the sound listener which is receiving the diffracted sound.
  * @param diffractionPoint - the point on the edge over which the sound is diffracting.
  * @param sourceFaceNormal - the normal (unit length) of the triangle facing the source.
  * @param listenerFaceNormal - the normal (unit length) of the triangle facing the listener.
  * @param edgeAxis - the unit length direction along the diffraction edge.
  * @param speedOfSound - the speed of sound in the medium where the diffraction occurs.
  * @param frequencies - the frequency bands for which diffraction should be computed.
  */
FrequencyBandResponse computeUTDAttenuation(
			const Vector3f& sourcePosition,
			const Vector3f& diffractionPoint,
			const Vector3f& listenerPosition,
			const Vector3f& sourceFaceNormal,
			const Vector3f& listenerFaceNormal,
			const Vector3f& edgeAxis,
			Real speedOfSound,
			const FrequencyBands& frequencies );




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_UTD_FREQUENCY_RESPONSE_H
