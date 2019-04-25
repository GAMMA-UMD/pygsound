/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundSceneIR.cpp
 * Contents:    gsound::SoundSceneIR class implementation
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


#include "gsSoundSceneIR.h"


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




SoundSceneIR:: SoundSceneIR()
	:	numListeners( 0 )
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Destructor
//############
//##########################################################################################
//##########################################################################################




SoundSceneIR:: ~SoundSceneIR()
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Listener Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void SoundSceneIR:: setListenerCount( Size newNumListeners )
{
	if ( listenerIRs.getSize() < newNumListeners )
		listenerIRs.setSize( newNumListeners );
	
	numListeners = newNumListeners;
}




//##########################################################################################
//##########################################################################################
//############
//############		Listener IR Accessor Methods
//############
//##########################################################################################
//##########################################################################################




SoundListenerIR* SoundSceneIR:: findListenerIR( const SoundListener* listener )
{
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listener == listenerIRs[i].getListener() )
			return &listenerIRs[i];
	}
	
	return NULL;
}




const SoundListenerIR* SoundSceneIR:: findListenerIR( const SoundListener* listener ) const
{
	for ( Index i = 0; i < numListeners; i++ )
	{
		if ( listener == listenerIRs[i].getListener() )
			return &listenerIRs[i];
	}
	
	return NULL;
}




//##########################################################################################
//##########################################################################################
//############
//############		Clear Methods
//############
//##########################################################################################
//##########################################################################################




void SoundSceneIR:: clear()
{
	for ( Index i = 0; i < listenerIRs.getSize(); i++ )
	{
		SoundListenerIR& listenerIR = listenerIRs[i];
		
		listenerIR.clear();
	}
}




void SoundSceneIR:: reset()
{
	for ( Index i = 0; i < listenerIRs.getSize(); i++ )
	{
		SoundListenerIR& listenerIR = listenerIRs[i];
		
		listenerIR.reset();
	}
	
	numListeners = 0;
}




//##########################################################################################
//##########################################################################################
//############
//############		Path Count Accessor Methods
//############
//##########################################################################################
//##########################################################################################




Size SoundSceneIR:: getPathCount() const
{
	Size total = 0;
	
	for ( Index i = 0; i < listenerIRs.getSize(); i++ )
		total += listenerIRs[i].getPathCount();
	
	return total;
}




//##########################################################################################
//##########################################################################################
//############
//############		Storage Size Accessor Methods
//############
//##########################################################################################
//##########################################################################################




Size SoundSceneIR:: getSizeInBytes() const
{
	Size totalSize = 0;
	
	for ( Index i = 0; i < listenerIRs.getSize(); i++ )
		totalSize += listenerIRs[i].getSizeInBytes();
	
	return totalSize + sizeof(SoundSceneIR);
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
