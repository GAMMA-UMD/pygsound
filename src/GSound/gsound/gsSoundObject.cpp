/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundObject.cpp
 * Contents:    gsound::SoundObject class implementation
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


#include "gsSoundObject.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############
//############		Constructors
//############
//##########################################################################################
//##########################################################################################




SoundObject:: SoundObject()
	:	flags( SoundObjectFlags::DEFAULT ),
		transform(),
		mesh( NULL ),
		userData( NULL )
{
	updateWorldSpaceBoundingSphere();
}




SoundObject:: SoundObject( SoundMesh* newMesh )
	:	flags( SoundObjectFlags::DEFAULT ),
		transform(),
		mesh( newMesh ),
		userData( NULL )
{
	updateWorldSpaceBoundingSphere();
}




SoundObject:: SoundObject( SoundMesh* newMesh, const Transform3f& newTransform )
	:	flags( SoundObjectFlags::DEFAULT ),
		transform( newTransform ),
		mesh( newMesh ),
		userData( NULL )
{
	updateWorldSpaceBoundingSphere();
}




//##########################################################################################
//##########################################################################################
//############
//############		Destructor
//############
//##########################################################################################
//##########################################################################################




SoundObject:: ~SoundObject()
{
}




//##########################################################################################
//##########################################################################################
//############
//############		Transform Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void SoundObject:: setTransform( const Transform3f& newTransform )
{
	transform = Transform3f( newTransform.position,
									newTransform.orientation.orthonormalize(),
									newTransform.scale );
	
	updateWorldSpaceBoundingSphere();
}




void SoundObject:: setPosition( const Vector3f& newPosition )
{
	transform.position = newPosition;
	
	updateWorldSpaceBoundingSphere();
}




void SoundObject:: setOrientation( const Matrix3f& newOrientation )
{
	transform.orientation = newOrientation.orthonormalize();
}




void SoundObject:: setScale( const Vector3f& newScale )
{
	transform.scale = newScale;
	
	updateWorldSpaceBoundingSphere();
}




//##########################################################################################
//##########################################################################################
//############
//############		Mesh Accessor Methods
//############
//##########################################################################################
//##########################################################################################




void SoundObject:: setMesh( SoundMesh* newMesh )
{
	mesh = newMesh;
	
	updateWorldSpaceBoundingSphere();
}




//##########################################################################################
//##########################################################################################
//############
//############		Bounding Sphere Update Method
//############
//##########################################################################################
//##########################################################################################




void SoundObject:: updateWorldSpaceBoundingSphere()
{
	if ( mesh != NULL )
	{
		const Sphere3f& meshBoundingSphere = mesh->getBoundingSphere();
		
		worldSpaceBoundingSphere.position = transform.transformToWorld( meshBoundingSphere.position );
		worldSpaceBoundingSphere.radius = transform.transformToWorld( meshBoundingSphere.radius ).getMax();
	}
	else
		worldSpaceBoundingSphere = Sphere3f();
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
