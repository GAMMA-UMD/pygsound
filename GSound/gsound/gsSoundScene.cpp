/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundScene.cpp
 * Contents:    gsound::SoundScene class implementation
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


#include "gsSoundScene.h"


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




SoundScene:: SoundScene()
	:	userData( NULL ),
		medium( SoundMedium::AIR ),
		reverbTime( 0 ),
		bvh( NULL )
{
}




SoundScene:: SoundScene( const SoundScene& other )
	:	sources( other.sources ),
		listeners( other.listeners ),
		objects( other.objects ),
		bvh( NULL ),
		sourceClusterer( other.sourceClusterer ),
		medium( other.medium ),
		reverbTime( other.reverbTime ),
		userData( other.userData )
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundScene:: ~SoundScene()
{
	if ( bvh != NULL )
		util::destruct( bvh );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Source Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundScene:: addSource( SoundSource* newSource )
{
	if ( newSource == NULL )
		return false;
	
	sources.add( newSource );
	sourceClusterer.addSource( newSource );
	
	return true;
}




Bool SoundScene:: removeSource( SoundSource* source )
{
	if ( source == NULL )
		return false;
	
	return sources.remove( source );
}




void SoundScene:: clearSources()
{
	sources.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Listener Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundScene:: addListener( SoundListener* newListener )
{
	if ( newListener == NULL )
		return false;
	
	listeners.add( newListener );
	
	return true;
}




Bool SoundScene:: removeListener( SoundListener* listener )
{
	if ( listener == NULL )
		return false;
	
	return listeners.remove( listener );
}




void SoundScene:: clearListeners()
{
	listeners.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Sound Object Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundScene:: addObject( SoundObject* newObject )
{
	if ( newObject == NULL )
		return false;
	
	objects.add( newObject );
	return true;
}




Bool SoundScene:: removeObject( SoundObject* object )
{
	if ( object == NULL )
		return false;
	
	Bool result = objects.remove( object );
	
	return result;
}




void SoundScene:: clearObjects()
{
	objects.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		BVH Rebuilding Method
//############		
//##########################################################################################
//##########################################################################################




void SoundScene:: rebuildBVH() const
{
	if ( objects.getSize() >= OBJECT_COUNT_THRESHOLD )
		bvh->bvh.rebuild();
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
