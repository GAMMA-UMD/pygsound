/*
 * Project:     GSound
 * 
 * File:        gsound/gsDebugCache.cpp
 * Contents:    gsound::DebugCache class implementation
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

#include "gsDebugCache.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




DebugCache:: DebugCache()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Constructor
//############		
//##########################################################################################
//##########################################################################################




DebugCache:: ~DebugCache()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Debug Data Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void DebugCache:: addListener( const SoundListener* listener )
{
	listeners.add( listener );
}




void DebugCache:: addSource( const SoundSource* source )
{
	sources.add( source );
}




void DebugCache:: addDetector( const SoundDetector* detector )
{
	detectors.add( detector );
}




void DebugCache:: addObjectMesh( const ArrayList<SoundVertex>& vertices,
								const ArrayList<SoundTriangle>& triangles,
								const ArrayList<SoundMaterial>& materials )
{
	mutex.lock();
	objectMeshes.add( ObjectMesh( vertices, triangles, materials ) );
	mutex.unlock();
}




void DebugCache:: addDiffractionEdge( const Vector3f& v1, const Vector3f& v2 )
{
	mutex.lock();
	diffractionEdges.add( DiffractionEdge( v1, v2 ) );
	mutex.unlock();
}




void DebugCache:: addDirectPath( const Vector3f& listenerPosition, const Vector3f& sourcePosition )
{
	mutex.lock();
	directPaths.add( DirectPath( listenerPosition, sourcePosition ) );
	mutex.unlock();
}




void DebugCache:: addDiffractionPath( const ArrayList<Vector3f>& points )
{
	mutex.lock();
	diffractionPaths.add( IndirectPath( points ) );
	mutex.unlock();
}




void DebugCache:: addSpecularPath( const ArrayList<Vector3f>& points )
{
	mutex.lock();
	specularPaths.add( IndirectPath( points ) );
	mutex.unlock();
}




void DebugCache:: addDiffusePath( const ArrayList<Vector3f>& points )
{
	mutex.lock();
	diffusePaths.add( IndirectPath( points ) );
	mutex.unlock();
}




void DebugCache:: addTransmissionPath( const Vector3f& listenerPosition, const Vector3f& sourcePosition )
{
	mutex.lock();
	transmissionPaths.add( DirectPath( listenerPosition, sourcePosition ) );
	mutex.unlock();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Clear Method
//############		
//##########################################################################################
//##########################################################################################




void DebugCache:: clear()
{
	mutex.lock();
	
	listeners.clear();
	sources.clear();
	detectors.clear();
	
	objectMeshes.clear();
	diffractionEdges.clear();
	
	directPaths.clear();
	diffractionPaths.clear();
	specularPaths.clear();
	diffusePaths.clear();
	transmissionPaths.clear();
	
	mutex.unlock();
}




void DebugCache:: reset()
{
	mutex.lock();
	
	listeners.reset();
	sources.reset();
	detectors.reset();
	
	objectMeshes.reset();
	diffractionEdges.reset();
	
	directPaths.reset();
	diffractionPaths.reset();
	specularPaths.reset();
	diffusePaths.reset();
	transmissionPaths.reset();
	
	mutex.unlock();
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
