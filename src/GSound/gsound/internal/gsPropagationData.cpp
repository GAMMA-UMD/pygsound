/*
 * Project:     GSound
 * 
 * File:        gsound/internal/gsPropagationData.cpp
 * Contents:    gsound::internal::PropagationData class implementation
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


#include "gsPropagationData.h"


#include "../gsSoundListener.h"


//##########################################################################################
//**************************  Start GSound Internal Namespace  *****************************
GSOUND_INTERNAL_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




PropagationData:: PropagationData()
	:	timeStamp( 0 ),
		time( 0 )
{
}




PropagationData:: PropagationData( const PropagationData& other )
	:	timeStamp( other.timeStamp ),
		time( other.time )
{
	// Copy each listener's data in the other data object.
	HashMap< const SoundListener*, Shared<ListenerData> >::ConstIterator iterator = other.listeners.getIterator();
	
	while ( iterator )
	{
		const SoundListener* listener = iterator.getKey();
		listeners.add( listener->getHashCode(), listener, Shared<ListenerData>::construct( *iterator.getValue() ) );
		
		iterator++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructors
//############		
//##########################################################################################
//##########################################################################################




PropagationData:: ~PropagationData()
{
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




PropagationData& PropagationData:: operator = ( const PropagationData& other )
{
	if ( this != &other )
	{
		timeStamp = other.timeStamp;
		time = other.time;
		
		// Clear the previous listener data.
		listeners.clear();
		
		// Copy each listener's data in the other data object.
		HashMap< const SoundListener*, Shared<ListenerData> >::ConstIterator iterator = other.listeners.getIterator();
		
		while ( iterator )
		{
			const SoundListener* listener = iterator.getKey();
			listeners.add( listener->getHashCode(), listener, Shared<ListenerData>::construct( *iterator.getValue() ) );
			
			iterator++;
		}
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Old Data Removal Method
//############		
//##########################################################################################
//##########################################################################################




void PropagationData:: removeOldData()
{
	//*************************************************************************************
	// Remove listener data objects for listeners that are no longer used.
	
	HashMap< const SoundListener*, Shared<ListenerData> >::Iterator listener = listeners.getIterator();
	
	while ( listener )
	{
		// If the listener data is old, remove it.
		if ( (*listener)->timeStamp < timeStamp )
		{
			listener.remove();
			continue;
		}
		
		// Check each source data in this listener.
		HashMap< const SoundSource*, Shared<SourceData> >::Iterator source = (*listener)->sources.getIterator();
		
		while ( source )
		{
			// If the source data is old, remove it.
			if ( (*source)->timeStamp < timeStamp )
			{
				source.remove();
				continue;
			}
			
			source++;
		}
		
		listener++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Clear Method
//############		
//##########################################################################################
//##########################################################################################




void PropagationData:: reset()
{
	listeners.clear();
}




void PropagationData:: resetPaths()
{
	HashMap< const SoundListener*, Shared<ListenerData> >::Iterator listener = listeners.getIterator();
	
	while ( listener )
	{
		(*listener)->soundPathCache.clear();
		listener++;
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Data Size Method
//############		
//##########################################################################################
//##########################################################################################




Size PropagationData:: getSizeInBytes() const
{
	Size totalSize = 0;
	
	HashMap< const SoundListener*, Shared<ListenerData> >::ConstIterator listener = listeners.getIterator();
	
	while ( listener )
	{
		// Check each source data in this listener.
		HashMap< const SoundSource*, Shared<SourceData> >::Iterator source = (*listener)->sources.getIterator();
		
		while ( source )
		{
			totalSize += sizeof(SourceData) +
						(*source)->irCache.getSizeInBytes() + (*source)->visibilityCache.getSizeInBytes() +
						(*source)->directivity.getSizeInBytes();
			
			source++;
		}
		
		totalSize += sizeof(ListenerData) + (*listener)->soundPathCache.getSizeInBytes();
		listener++;
	}
	
	return totalSize + sizeof(PropagationData);
}




//##########################################################################################
//**************************  End GSound Internal Namespace  *******************************
GSOUND_INTERNAL_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

