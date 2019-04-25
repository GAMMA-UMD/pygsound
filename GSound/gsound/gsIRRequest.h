/*
 * Project:     GSound
 * 
 * File:        gsound/gsIRRequest.h
 * Contents:    gsound::IRRequest class declaration
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


#ifndef INCLUDE_GSOUND_IR_REQUEST_H
#define INCLUDE_GSOUND_IR_REQUEST_H


#include "gsConfig.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that specifies how an ImpulseResponse should be created from the output of sound propagation.
class IRRequest
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a new IR request with the default parameters.
			GSOUND_INLINE IRRequest()
				:	frequencies(),
					channelLayout( ChannelLayout::STEREO ),
					hrtf( NULL ),
					ir( true ),
					normalize( false ),
					binEnergy( true ),
					metrics( true ),
					binTime( 0.01f )
			{
			}
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// An object that stores the frequency bands that the impulse response should be created for.
			FrequencyBands frequencies;
			
			
			/// The channel layout that should be use for spatializing the IR.
			ChannelLayout channelLayout;
			
			
			/// An optional pointer to the head-related transfer function that should be used to spatialize the IR.
			const HRTF* hrtf;
			
			
			/// If set to TRUE, a spatialized pressure impulse response for auralization is computed.
			/**
			  * If this flag is not set, not IR is computed.
			  */
			Bool ir;
			
			
			/// If set to TRUE, the resulting impulse response is normalized to the -1 to 1 range.
			/**
			  * Otherwise, the IR is scaled based on the source power and listener sensitivity.
			  */
			Bool normalize;
			
			
			/// If set to TRUE, the sound energy in the impulse response is binned per-band.
			Bool binEnergy;
			
			
			/// If set to TRUE, standard acoustic metrics are computed from the impulse response.
			/**
			  * If this flag is set, the energy in the IR is automatically binned.
			  */
			Bool metrics;
			
			
			/// The length in seconds for an impulse response bin.
			/**
			  * When the energy in the IR is binned, all sound energy that arrives within this
			  * time period is added together to get the total energy for the bin.
			  */
			Float binTime;
			
			
			
};




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_IR_REQUEST_H
