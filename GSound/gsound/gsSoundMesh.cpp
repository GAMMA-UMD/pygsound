/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMesh.cpp
 * Contents:    gsound::SoundMesh class implementation
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


#include "gsSoundMesh.h"


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




SoundMesh:: SoundMesh()
	:	vertices(),
		triangles(),
		materials(),
		bvh(),
		diffractionGraph(),
		userData( NULL )
{
}




SoundMesh:: SoundMesh( const SoundMesh& other )
	:	vertices(),
		materials(),
		triangles(),
		bvh( NULL ),
		diffractionGraph(),
		boundingSphere( other.boundingSphere ),
		boundingBox( other.boundingBox ),
		name( other.name ),
		userData( other.userData )
{
	this->setData( other.vertices, other.triangles, other.materials, other.diffractionGraph );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




SoundMesh:: ~SoundMesh()
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




SoundMesh& SoundMesh:: operator = ( const SoundMesh& other )
{
	if ( this != &other )
	{
		vertices.release();
		materials.release();
		triangles.release();
		
		if ( bvh != NULL )
			util::destruct( bvh );
		
		this->setData( other.vertices, other.triangles, other.materials, other.diffractionGraph );
		name = other.name;
		userData = other.userData;
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Size In Bytes Method
//############		
//##########################################################################################
//##########################################################################################




Size SoundMesh:: getSizeInBytes() const
{
	Size totalSize = 0;
	
	totalSize += vertices.isSet() ? vertices->getCapacity()*sizeof(SoundVertex) : 0;
	totalSize += triangles.isSet() ? triangles->getCapacity()*sizeof(TriangleType) : 0;
	totalSize += materials.isSet() ? materials->getCapacity()*sizeof(SoundMaterial) : 0;
	totalSize += bvh ? bvh->bvh.getSizeInBytes() : 0;
	totalSize += diffractionGraph.isSet() ? diffractionGraph->getSizeInBytes() : 0;
	
	return totalSize;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Data Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundMesh:: setData( const Shared<ArrayList<SoundVertex> >& newVertices,
							const Shared<ArrayList<TriangleType> >& newTriangles,
							const Shared<ArrayList<SoundMaterial> >& newMaterials,
							const Shared<internal::DiffractionGraph>& newDiffractionGraph )
{
	vertices = newVertices;
	triangles = newTriangles;
	materials = newMaterials;
	diffractionGraph = newDiffractionGraph;
	
	// Construct the BVH.
	bvh = util::construct<MeshBVH>( this );
	bvh->bvh.rebuild();
	
	// Generate a bounding sphere for the mesh.
	boundingSphere = Sphere3f( vertices->getPointer(), vertices->getSize() );
	boundingBox = AABB3f( vertices->getPointer(), vertices->getSize() );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mesh Saving Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMesh:: save( const char* pathToFile ) const
{
	if ( !pathToFile )
		return false;
	
	om::UTF8String filePathString( reinterpret_cast<const om::UTF8Char*>(pathToFile) );
	om::File file( filePathString );
	
	// Erase the file if it exists.
	if ( !file.erase() )
		return false;
	
	// Open the file.
	om::FileWriter writer( file );
	
	if ( !writer.open() )
		return false;
	
	// Save this mesh to the file.
	Bool result = saveMeshToStream( *this, writer );
	
	// Close the file.
	writer.close();
	
	return result;
}




Bool SoundMesh:: save( om::DataOutputStream& stream ) const
{
	// Save this mesh to the stream.
	return saveMeshToStream( *this, stream );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Mesh Loading Methods
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMesh:: load( const char* pathToFile, SoundMesh& mesh )
{
	if ( !pathToFile )
		return Shared<SoundMesh>();
	
	om::UTF8String filePathString( reinterpret_cast<const om::UTF8Char*>(pathToFile) );
	
	// Open the file.
	om::FileReader reader( filePathString );
	
	if ( !reader.open() )
		return Shared<SoundMesh>();
	
	// Load the mesh from the file.
	Bool result = loadMeshFromStream( reader, mesh );
	
	// Close the file.
	reader.close();
	
	return result;
}




Bool SoundMesh:: load( om::DataInputStream& stream, SoundMesh& mesh )
{
	// Load the mesh from the stream.
	return loadMeshFromStream( stream, mesh );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Save Mesh to Stream Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMesh:: saveMeshToStream( const SoundMesh& mesh, om::DataOutputStream& stream )
{
	//***************************************************************************
	// Write the base header.
	
	// Make the header.
	UByte header[16] =
	{
		'S','O','U','N','D','M','E','S','H', // format specifier.
		1, // format version
#if defined(GSOUND_BIG_ENDIAN)
		1, // endianness
#else
		0, // endianness
#endif
		0, // padding.
		0, 0, 0, 0 // 32-bit checksum.
	};
	
	// Write the header to the stream.
	stream.writeData( header, 16 );
	
	//***************************************************************************
	// Get basic information about the mesh.
	
	const Shared<ArrayList<SoundVertex> >& vertices = mesh.vertices;
	const Shared<ArrayList<TriangleType> >& triangles = mesh.triangles;
	const Shared<ArrayList<SoundMaterial> >& materials = mesh.materials;
	const Shared<internal::DiffractionGraph>& diffractionGraph = mesh.diffractionGraph;
	
	const SoundVertex* const verticesStart = vertices.isSet() ? vertices->getPointer() : NULL;
	const SoundMesh::TriangleType* const trianglesStart = triangles.isSet() ? triangles->getPointer() : NULL;
	const SoundMaterial* const materialsStart = materials.isSet() ? materials->getPointer() : NULL;
	const internal::DiffractionEdge* const edgesStart = diffractionGraph.isSet() ? &diffractionGraph->getEdge(0) : NULL;
	
	const Size numVertices = vertices.isSet() ? vertices->getSize() : 0;
	const Size numTriangles = triangles.isSet() ? triangles->getSize() : 0;
	const Size numEdges = diffractionGraph.isSet() ? diffractionGraph->getEdgeCount() : 0;
	const Size numNeighbors = diffractionGraph.isSet() ? diffractionGraph->getEdgeNeighborCount() : 0;
	const Size numMaterials = materials.isSet() ? materials->getSize() : 0;
	
	//***************************************************************************
	// Write the mesh header.
	
	const Size meshHeaderDataSize = 5*sizeof(UInt64);
	UByte meshHeaderData[meshHeaderDataSize];
	UByte* meshHeader = meshHeaderData;
	
	// Write the number of vertices, triangles, diffraction edges, neighbors, materials.
	writeUInt64( meshHeader, numVertices );
	writeUInt64( meshHeader, numTriangles );
	writeUInt64( meshHeader, numEdges );
	writeUInt64( meshHeader, numNeighbors );
	writeUInt64( meshHeader, numMaterials );
	
	Bool vertices64 = numVertices > math::max<UInt32>();
	Bool triangles64 = numTriangles > math::max<UInt32>();
	Bool edges64 = numTriangles > math::max<UInt32>();
	Bool neighbors64 = numNeighbors > math::max<UInt32>();
	Bool materials64 = numMaterials > math::max<UInt32>();
	
	// Write the mesh header to the stream.
	stream.writeData( meshHeaderData, meshHeaderDataSize );
	
	//***************************************************************************
	// Write the materials in the mesh.
	
	Size dataBufferSize = 0;
	UByte* dataBuffer = NULL;
	
	writeMaterials( *materials, dataBuffer, dataBufferSize, stream );
	
	//***************************************************************************
	// Write the vertices in the mesh.
	
	writeVertices( *vertices, dataBuffer, dataBufferSize, stream );
	
	//***************************************************************************
	// Write the triangles in the mesh.
	
	const Size triangleDataSize = 3*sizeof(UInt64) + 3*sizeof(UInt64) + 4*sizeof(UInt32);
	const Size actualTriangleDataSize = (vertices64 ? 3*sizeof(UInt64) : 3*sizeof(UInt32)) + 
										(edges64 ? 3*sizeof(UInt64) : 3*sizeof(UInt32)) + 
										(materials64 ? sizeof(UInt64) : sizeof(UInt32)) + 3*sizeof(UInt32);
	UByte triangleData[triangleDataSize];
	
	for ( Index i = 0; i < numTriangles; i++ )
	{
		const SoundMesh::TriangleType& triangle = (*triangles)[i];
		UByte* writePosition = triangleData;
		
		// Write vertex indices in 32 or 64 bit.
		if ( vertices64 )
		{
			writeUInt64( writePosition, triangle.getVertex(0) - verticesStart );
			writeUInt64( writePosition, triangle.getVertex(1) - verticesStart );
			writeUInt64( writePosition, triangle.getVertex(2) - verticesStart );
		}
		else
		{
			writeUInt32( writePosition, UInt32(triangle.getVertex(0) - verticesStart) );
			writeUInt32( writePosition, UInt32(triangle.getVertex(1) - verticesStart) );
			writeUInt32( writePosition, UInt32(triangle.getVertex(2) - verticesStart) );
		}
		
		// Write edge indices in 32 or 64 bit.
		if ( edges64 )
		{
			writeUInt64( writePosition, triangle.getDiffractionEdge(0) ? (triangle.getDiffractionEdge(0) - edgesStart) + 1 : 0 );
			writeUInt64( writePosition, triangle.getDiffractionEdge(1) ? (triangle.getDiffractionEdge(1) - edgesStart) + 1 : 0 );
			writeUInt64( writePosition, triangle.getDiffractionEdge(2) ? (triangle.getDiffractionEdge(2) - edgesStart) + 1 : 0 );
		}
		else
		{
			writeUInt32( writePosition, triangle.getDiffractionEdge(0) ? UInt32((triangle.getDiffractionEdge(0) - edgesStart) + 1) : 0 );
			writeUInt32( writePosition, triangle.getDiffractionEdge(1) ? UInt32((triangle.getDiffractionEdge(1) - edgesStart) + 1) : 0 );
			writeUInt32( writePosition, triangle.getDiffractionEdge(2) ? UInt32((triangle.getDiffractionEdge(2) - edgesStart) + 1) : 0 );
		}
		
		if ( materials64 )
			writeUInt64( writePosition, triangle.getMaterial() - materialsStart );
		else
			writeUInt32( writePosition, UInt32(triangle.getMaterial() - materialsStart) );
		
		// Write material index, key vertex, subdivision rows and columns.
		writeUInt32( writePosition, UInt32(triangle.getKeyVertex()) );
		writeUInt32( writePosition, UInt32(triangle.getRowCount()) );
		writeUInt32( writePosition, UInt32(triangle.getColumnCount()) );
		
		// Write the triangle data to the file.
		stream.writeData( triangleData, actualTriangleDataSize );
	}
	
	//***************************************************************************
	// Write the diffraction graph.
	
	const Size edgeDataSize = 6*sizeof(UInt64) + 2*sizeof(UInt16) + 8*sizeof(Float32);
	const Size actualEdgeDataSize = (vertices64 ? 2*sizeof(UInt64) : 2*sizeof(UInt32)) + 
									(triangles64 ? 2*sizeof(UInt64) : 2*sizeof(UInt32)) + 
									(neighbors64 ? 2*sizeof(UInt64) : 2*sizeof(UInt32)) + 2*sizeof(UInt16) + 8*sizeof(Float32);
	UByte edgeData[edgeDataSize];
	
	if ( diffractionGraph.isSet() )
	{
		for ( Index i = 0; i < diffractionGraph->getEdgeCount(); i++ )
		{
			const internal::DiffractionEdge& edge = diffractionGraph->getEdge(i);
			UByte* writePosition = edgeData;
			
			if ( vertices64 )
			{
				writeUInt64( writePosition, edge.v1 - verticesStart );
				writeUInt64( writePosition, edge.v2 - verticesStart );
			}
			else
			{
				writeUInt32( writePosition, UInt32(edge.v1 - verticesStart) );
				writeUInt32( writePosition, UInt32(edge.v2 - verticesStart) );
			}
			
			if ( triangles64 )
			{
				writeUInt64( writePosition, edge.triangle1 - trianglesStart );
				writeUInt64( writePosition, edge.triangle2 - trianglesStart );
			}
			else
			{
				writeUInt32( writePosition, UInt32(edge.triangle1 - trianglesStart) );
				writeUInt32( writePosition, UInt32(edge.triangle2 - trianglesStart) );
			}
			
			if ( neighbors64 )
			{
				writeUInt64( writePosition, edge.numNeighbors );
				writeUInt64( writePosition, edge.neighborListOffset );
			}
			else
			{
				writeUInt32( writePosition, edge.numNeighbors );
				writeUInt32( writePosition, edge.neighborListOffset );
			}
			
			writeUInt16( writePosition, edge.edgeIndex1 );
			writeUInt16( writePosition, edge.edgeIndex2 );
			
			writeFloat32( writePosition, edge.plane1.normal.x );
			writeFloat32( writePosition, edge.plane1.normal.y );
			writeFloat32( writePosition, edge.plane1.normal.z );
			writeFloat32( writePosition, edge.plane1.offset );
			
			writeFloat32( writePosition, edge.plane2.normal.x );
			writeFloat32( writePosition, edge.plane2.normal.y );
			writeFloat32( writePosition, edge.plane2.normal.z );
			writeFloat32( writePosition, edge.plane2.offset );
			
			// Write the edge data to the file.
			stream.writeData( edgeData, actualEdgeDataSize );
		}
		
		// Write edge neighbors.
		for ( Index i = 0; i < diffractionGraph->getEdgeNeighborCount(); i++ )
		{
			UByte* writePosition = edgeData;
			
			if ( neighbors64 )
			{
				*(UInt64*)writePosition = diffractionGraph->getEdgeNeighborIndex(i);
				
				// Write the edge data to the file.
				stream.writeData( edgeData, sizeof(UInt64) );
			}
			else
			{
				*(UInt32*)writePosition = (UInt32)diffractionGraph->getEdgeNeighborIndex(i);
				
				// Write the edge data to the file.
				stream.writeData( edgeData, sizeof(UInt32) );
			}
		}
	}
	
	//***************************************************************************
	// Clean up the temporary buffer.
	
	releaseBuffer( dataBuffer );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Load Mesh from Stream Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMesh:: loadMeshFromStream( om::DataInputStream& stream, SoundMesh& mesh )
{
	//***************************************************************************
	// Read the header.
	
	const Size headerDataSize = 16;
	UByte headerData[headerDataSize];
	
	if ( stream.readData( headerData, headerDataSize ) < headerDataSize )
		return false;
	
	// Check the format code
	if ( headerData[0] != 'S' || headerData[1] != 'O' || headerData[2] != 'U' || headerData[3] != 'N' ||
		headerData[4] != 'D' || headerData[5] != 'M' || headerData[6] != 'E' || headerData[7] != 'S' ||
		headerData[8] != 'H' )
		return false;
	
	// Get the version and endianness of the file.
	Size version = headerData[9];
	om::data::Endianness endianness = headerData[10] ? om::data::Endianness::BIG : om::data::Endianness::LITTLE;
	
	//***************************************************************************
	// Read the file.
	
	switch ( version )
	{
		case 1:
			return loadMeshVersion1( stream, endianness, mesh );
	}
	
	return false;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version 1 Load Method
//############		
//##########################################################################################
//##########################################################################################




Bool SoundMesh:: loadMeshVersion1( om::DataInputStream& stream, om::data::Endianness endianness, SoundMesh& mesh )
{
	//***************************************************************************
	// Read the mesh header.
	
	const Size meshHeaderDataSize = 4*sizeof(UInt64) + 2*sizeof(UInt32);
	UByte meshHeaderData[meshHeaderDataSize];
	
	if ( stream.readData( meshHeaderData, meshHeaderDataSize ) < meshHeaderDataSize )
		return false;
	
	UInt64 numVertices = 0;
	UInt64 numTriangles = 0;
	UInt64 numEdges = 0;
	UInt64 numNeighbors = 0;
	UInt64 numMaterials = 0;
	
	UByte* meshHeader = meshHeaderData;
	
	// Read the number of vertices, triangles, diffraction edges, neighbors, materials, and frequency bands.
	readUInt64( meshHeader, endianness, numVertices );
	readUInt64( meshHeader, endianness, numTriangles );
	readUInt64( meshHeader, endianness, numEdges );
	readUInt64( meshHeader, endianness, numNeighbors );
	readUInt64( meshHeader, endianness, numMaterials );
	
	Bool vertices64 = numVertices > math::max<UInt32>();
	Bool triangles64 = numTriangles > math::max<UInt32>();
	Bool edges64 = numTriangles > math::max<UInt32>();
	Bool neighbors64 = numNeighbors > math::max<UInt32>();
	Bool materials64 = numMaterials > math::max<UInt32>();
	
#if defined(GSOUND_PLATFORM_32_BIT)
	// Make sure the mesh is not too big for the current platform.
	if ( vertices64 || triangles64 || edges64 || neighbors64 )
		return false;
#endif
	
	Size dataBufferSize = 0;
	UByte* dataBuffer = NULL;
	
	//***************************************************************************
	// Read the frequency bands for the mesh, then read the materials for the mesh.
	
	Shared<ArrayList<SoundMaterial> > materials = Shared<ArrayList<SoundMaterial> >::construct( (Size)numMaterials );
	
	if ( !readMaterials( *materials, (Size)numMaterials, dataBuffer, dataBufferSize, endianness, stream ) )
	{
		releaseBuffer( dataBuffer );
		return false;
	}
	
	const SoundMaterial* const materialsStart = materials->getPointer();
	
	//***************************************************************************
	// Read the vertices for the mesh.
	
	Shared<ArrayList<SoundVertex> > vertices = Shared<ArrayList<SoundVertex> >::construct( (Size)numVertices );
	
	if ( !readVertices( *vertices, (Size)numVertices, dataBuffer, dataBufferSize, endianness, stream ) )
	{
		releaseBuffer( dataBuffer );
		return false;
	}
	
	const SoundVertex* const verticesStart = vertices->getPointer();
	
	//***************************************************************************
	// Read the triangles for the mesh.
	
	Shared<ArrayList<TriangleType> > triangles = Shared<ArrayList<TriangleType> >::construct( (Size)numTriangles );
	const Size triangleDataSize = 3*sizeof(UInt64) + 3*sizeof(UInt64) + 4*sizeof(UInt32);
	UByte triangleData[triangleDataSize];
	const Size actualTriangleDataSize = (vertices64 ? 3*sizeof(UInt64) : 3*sizeof(UInt32)) + 
										(edges64 ? 3*sizeof(UInt64) : 3*sizeof(UInt32)) + 4*sizeof(UInt32);
	
	for ( Index i = 0; i < numTriangles; i++ )
	{
		if ( stream.readData( triangleData, actualTriangleDataSize ) < actualTriangleDataSize )
			return false;
		
		UInt64 v0 = 0, v1 = 0, v2 = 0, e0 = 0, e1 = 0, e2 = 0, m;
		UInt32 k = 0, r = 0, c = 0;
		UByte* readPosition = triangleData;
		
		// Read vertex indices in 64 or 32 bit.
		if ( vertices64 )
		{
			readUInt64( readPosition, endianness, v0 );
			readUInt64( readPosition, endianness, v1 );
			readUInt64( readPosition, endianness, v2 );
		}
		else
		{
			UInt32 vv0 = 0, vv1 = 0, vv2 = 0;
			readUInt32( readPosition, endianness, vv0 );
			readUInt32( readPosition, endianness, vv1 );
			readUInt32( readPosition, endianness, vv2 );
			v0 = vv0; v1 = vv1; v2 = vv2;
		}
		
		// Read edge indices in 64 or 32 bit.
		if ( edges64 )
		{
			readUInt64( readPosition, endianness, e0 );
			readUInt64( readPosition, endianness, e1 );
			readUInt64( readPosition, endianness, e2 );
		}
		else
		{
			UInt32 ee0 = 0, ee1 = 0, ee2 = 0;
			readUInt32( readPosition, endianness, ee0 );
			readUInt32( readPosition, endianness, ee1 );
			readUInt32( readPosition, endianness, ee2 );
			e0 = ee0; e1 = ee1; e2 = ee2;
		}
		
		if ( materials64 )
			readUInt64( readPosition, endianness, m );
		else
		{
			UInt32 mm = 0;
			readUInt32( readPosition, endianness, mm );
			m = mm;
		}
		
		// Read material index, key vertex, subdivision rows and columns.
		readUInt32( readPosition, endianness, k );
		readUInt32( readPosition, endianness, r );
		readUInt32( readPosition, endianness, c );
		
		// Create the triangle.
		TriangleType t( verticesStart + v0, verticesStart + v1, verticesStart + v2, materialsStart + m );
		t.setKeyVertex( k );
		t.setRowCount( r );
		t.setColumnCount( c );
		t.setDiffractionEdge( 0, (const internal::DiffractionEdge*)e0 );
		t.setDiffractionEdge( 1, (const internal::DiffractionEdge*)e1 );
		t.setDiffractionEdge( 2, (const internal::DiffractionEdge*)e2 );
		
		triangles->add( t );
	}
	
	const TriangleType* const trianglesStart = triangles->getPointer();
	
	//***************************************************************************
	// Read the diffraction edges.
	
	Shared<ArrayList<internal::DiffractionEdge> > edges = Shared<ArrayList<internal::DiffractionEdge> >::construct( (Size)numEdges );
	const Size edgeDataSize = 6*sizeof(UInt64) + 2*sizeof(UInt16) + 8*sizeof(Float32);
	const Size actualEdgeDataSize = (vertices64 ? 2*sizeof(UInt64) : 2*sizeof(UInt32)) + 
									(triangles64 ? 2*sizeof(UInt64) : 2*sizeof(UInt32)) + 
									(neighbors64 ? 2*sizeof(UInt64) : 2*sizeof(UInt32)) + 2*sizeof(UInt16) + 8*sizeof(Float32);
	UByte edgeData[edgeDataSize];
	
	for ( Index i = 0; i < numEdges; i++ )
	{
		if ( stream.readData( edgeData, actualEdgeDataSize ) < actualEdgeDataSize )
			return false;
		
		UInt64 v0 = 0, v1 = 0, t0 = 0, t1 = 0, nn = 0, no = 0;
		UInt16 e0 = 0, e1 = 0;
		Plane3f p1, p2;
		UByte* readPosition = edgeData;
		
		if ( vertices64 )
		{
			readUInt64( readPosition, endianness, v0 );
			readUInt64( readPosition, endianness, v1 );
		}
		else
		{
			UInt32 vv0 = 0, vv1 = 0;
			readUInt32( readPosition, endianness, vv0 );
			readUInt32( readPosition, endianness, vv1 );
			v0 = vv0; v1 = vv1;
		}
		
		if ( triangles64 )
		{
			readUInt64( readPosition, endianness, t0 );
			readUInt64( readPosition, endianness, t1 );
		}
		else
		{
			UInt32 tt0 = 0, tt1 = 0;
			readUInt32( readPosition, endianness, tt0 );
			readUInt32( readPosition, endianness, tt1 );
			t0 = tt0; t1 = tt1;
		}
		
		if ( neighbors64 )
		{
			readUInt64( readPosition, endianness, nn );
			readUInt64( readPosition, endianness, no );
		}
		else
		{
			UInt32 nnn = 0, nno = 0;
			readUInt32( readPosition, endianness, nnn );
			readUInt32( readPosition, endianness, nno );
			nn = nnn; no = nno;
		}
		
		readUInt16( readPosition, endianness, e0 );
		readUInt16( readPosition, endianness, e1 );
		
		readFloat32( readPosition, endianness, p1.normal.x );
		readFloat32( readPosition, endianness, p1.normal.y );
		readFloat32( readPosition, endianness, p1.normal.z );
		readFloat32( readPosition, endianness, p1.offset );
		
		readFloat32( readPosition, endianness, p2.normal.x );
		readFloat32( readPosition, endianness, p2.normal.y );
		readFloat32( readPosition, endianness, p2.normal.z );
		readFloat32( readPosition, endianness, p2.offset );
		
		internal::DiffractionEdge e( trianglesStart + t0, e0, trianglesStart + t1, e1,
									verticesStart + v0, verticesStart + v1, p1, p2 );
		
		e.numNeighbors = (UInt32)nn;
		e.neighborListOffset = (UInt32)no;
		
		edges->add( e );
	}
	
	const internal::DiffractionEdge* const edgesStart = edges->getPointer();
	
	//***************************************************************************
	// Finalize triangle edge pointers.
	
	// Finalize all triangle edge pointers.
	for ( Index i = 0; i < triangles->getSize(); i++ )
	{
		TriangleType& t = (*triangles)[i];
		
		if ( t.getDiffractionEdge(0) )
			t.setDiffractionEdge( 0, edgesStart + (Index(t.getDiffractionEdge(0)) - 1) );
		
		if ( t.getDiffractionEdge(1) )
			t.setDiffractionEdge( 1, edgesStart + (Index(t.getDiffractionEdge(1)) - 1) );
		
		if ( t.getDiffractionEdge(2) )
			t.setDiffractionEdge( 2, edgesStart + (Index(t.getDiffractionEdge(2)) - 1) );
	}
	
	//***************************************************************************
	// Read the edge neighbor indices.
	
	ArrayList<UInt32> neighbors( (Size)numNeighbors );
	
	for ( Index i = 0; i < numNeighbors; i++ )
	{
		if ( neighbors64 )
		{
			UInt64 e;
			if ( !stream.readData( (UByte*)&e, sizeof(e) ) )
				continue;
			e = endianness.convertToNative(e);
			neighbors.add( (UInt32)e );
		}
		else
		{
			UInt32 e;
			if ( !stream.readData( (UByte*)&e, sizeof(e) ) )
				continue;
			e = endianness.convertToNative(e);
			neighbors.add( e );
		}
	}
	
	// Create the diffraction graph.
	Shared<internal::DiffractionGraph> graph = Shared<internal::DiffractionGraph>::construct( edges, neighbors );
	
	//***************************************************************************
	// Construct the final mesh.
	
	// Set the mesh data.
	mesh.setData( vertices, triangles, materials, graph );
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Reading Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundMesh:: readUInt16( UByte*& data, om::data::Endianness endianness, UInt16& value )
{
	// Native endianness.
	value = endianness.convertToNative( *(UInt16*)data );
	data += sizeof(UInt16);
}




void SoundMesh:: readUInt32( UByte*& data, om::data::Endianness endianness, UInt32& value )
{
	// Native endianness.
	value = endianness.convertToNative( *(UInt32*)data );
	data += sizeof(UInt32);
}




void SoundMesh:: readUInt64( UByte*& data, om::data::Endianness endianness, UInt64& value )
{
	// Native endianness.
	value = endianness.convertToNative( *(UInt64*)data );
	data += sizeof(UInt64);
}




void SoundMesh:: readFloat32( UByte*& data, om::data::Endianness endianness, Float32& value )
{
	// Native endianness.
	value = endianness.convertToNative( *(Float32*)data );
	data += sizeof(Float32);
}




Bool SoundMesh:: readResponse( UByte*& dataBuffer, Size& dataBufferSize, om::data::Endianness endianness,
								om::DataInputStream& stream, FrequencyResponse& response )
{
	//***************************************************************************
	// Read the number of frequencies in the response.
	
	enlargeBuffer( dataBuffer, dataBufferSize, sizeof(UInt32) );
	
	if ( stream.readData( dataBuffer, sizeof(UInt32) ) < sizeof(UInt32) )
		return false;
	
	UByte* readPosition = dataBuffer;
	
	// Determine the number of frequencies in this response.
	UInt32 numFrequencies = 0;
	readUInt32( readPosition, endianness, numFrequencies );
	
	//***************************************************************************
	// Make sure the buffer is big enough to hold the frequencies.
	
	const Size frequencyDataSize = numFrequencies*2*sizeof(Float32);
	enlargeBuffer( dataBuffer, dataBufferSize, frequencyDataSize );
	
	if ( stream.readData( dataBuffer, frequencyDataSize ) < frequencyDataSize )
		return false;
	
	readPosition = dataBuffer;
	
	//***************************************************************************
	// Read each frequency and gain, then add those to the output response.
	
	for ( Index i = 0; i < numFrequencies; i++ )
	{
		Float32 frequency = 0, gain = 0;
		readFloat32( readPosition, endianness, frequency );
		readFloat32( readPosition, endianness, gain );
		response.setFrequency( frequency, gain );
	}
	
	return true;
}




Bool SoundMesh:: readMaterials( ArrayList<SoundMaterial>& materials, Size numMaterials,
								UByte*& dataBuffer, Size& dataBufferSize,
								om::data::Endianness endianness,
								om::DataInputStream& stream )
{
	for ( Index i = 0; i < numMaterials; i++ )
	{
		FrequencyResponse r, s, t;
		
		if ( !readResponse( dataBuffer, dataBufferSize, endianness, stream, r ) ||
			!readResponse( dataBuffer, dataBufferSize, endianness, stream, s ) ||
			!readResponse( dataBuffer, dataBufferSize, endianness, stream, t ) )
			return false;
		
		const Size colorSize = 4*sizeof(Float32);
		UByte colorBytes[colorSize];
		Real color[4];
		
		if ( stream.readData( colorBytes, colorSize ) < colorSize )
			return false;
		
		UByte* colorData = colorBytes;
		readFloat32( colorData, endianness, color[0] );
		readFloat32( colorData, endianness, color[1] );
		readFloat32( colorData, endianness, color[2] );
		readFloat32( colorData, endianness, color[3] );
		
		SoundMaterial material( r, s, t );
		material.setColor( Vector4f( color[0], color[1], color[2], color[3] ) );
		
		materials.add( material );
	}
	
	return true;
}




Bool SoundMesh:: readVertices( ArrayList<SoundVertex>& vertices, Size numVertices,
								UByte*& dataBuffer, Size& dataBufferSize,
								om::data::Endianness endianness,
								om::DataInputStream& stream )
{
	const Size vertexDataSize = numVertices*3*sizeof(Float32);
	
	// Make sure the data buffer is big enough to hold the vertices.
	enlargeBuffer( dataBuffer, dataBufferSize, vertexDataSize );
	
	// Read the vertex data from the stream.
	if ( stream.readData( dataBuffer, vertexDataSize ) < vertexDataSize )
		return false;
	
	UByte* readPosition = dataBuffer;
	
	// Read each vertex and add it to the output list.
	for ( Index i = 0; i < numVertices; i++ )
	{
		Real x, y, z;
		readFloat32( readPosition, endianness, x );
		readFloat32( readPosition, endianness, y );
		readFloat32( readPosition, endianness, z );
		vertices.add( SoundVertex( x, y, z ) );
	}
	
	return true;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Writing Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundMesh:: writeUInt16( UByte*& data, UInt16 value )
{
	// Native endianness.
	*(UInt16*)data = value;
	data += sizeof(UInt16);
}




void SoundMesh:: writeUInt32( UByte*& data, UInt32 value )
{
	// Native endianness.
	*(UInt32*)data = value;
	data += sizeof(UInt32);
}




void SoundMesh:: writeUInt64( UByte*& data, UInt64 value )
{
	// Native endianness.
	*(UInt64*)data = value;
	data += sizeof(UInt64);
}




void SoundMesh:: writeFloat32( UByte*& data, Float32 value )
{
	// Native endianness.
	*(Float32*)data = value;
	data += sizeof(Float32);
}




void SoundMesh:: writeResponse( UByte*& data, const FrequencyResponse& response )
{
	// Write the number of bands in the response.
	writeUInt32( data, (UInt32)response.getFrequencyCount() );
	
	// Write the response frequencies.
	for ( Index f = 0; f < response.getFrequencyCount(); f++ )
	{
		writeFloat32( data, response.getFrequency(f) );
		writeFloat32( data, response.getFrequencyGain(f) );
	}
}




void SoundMesh:: writeMaterials( const ArrayList<SoundMaterial>& materials,
								UByte*& dataBuffer, Size& dataBufferSize,
								om::DataOutputStream& stream )
{
	// Compute the size on disk of the materials in the mesh.
	const Size numMaterials = materials.getSize();
	Size materialDataSize = 0;
	
	for ( Index i = 0; i < numMaterials; i++ )
	{
		const SoundMaterial& material = materials[i];
		materialDataSize += material.getReflectivity().getFrequencyCount()*2*sizeof(Float32) + sizeof(UInt32);
		materialDataSize += material.getScattering().getFrequencyCount()*2*sizeof(Float32) + sizeof(UInt32);
		materialDataSize += material.getTransmission().getFrequencyCount()*2*sizeof(Float32) + sizeof(UInt32);
		materialDataSize += 4*sizeof(Float32);
	}
	
	// Make sure the temporary buffer is big enough.
	enlargeBuffer( dataBuffer, dataBufferSize, materialDataSize );
	
	UByte* writePosition = dataBuffer;
	
	// Write each material.
	for ( Index i = 0; i < numMaterials; i++ )
	{
		const SoundMaterial& material = materials[i];
		
		writeResponse( writePosition, material.getReflectivity() );
		writeResponse( writePosition, material.getScattering() );
		writeResponse( writePosition, material.getTransmission() );
		
		const Vector4f& color = material.getColor();
		
		writeFloat32( writePosition, color.r );
		writeFloat32( writePosition, color.g );
		writeFloat32( writePosition, color.b );
		writeFloat32( writePosition, color.a );
	}
	
	// Write all of the materials to the file.
	stream.writeData( dataBuffer, materialDataSize );
}




void SoundMesh:: writeVertices( const ArrayList<SoundVertex>& vertices,
								UByte*& dataBuffer, Size& dataBufferSize,
								om::DataOutputStream& stream )
{
	// Compute the size on disk of the materials in the mesh.
	const Size numVertices = vertices.getSize();
	const Size vertexDataSize = numVertices*3*sizeof(Float32);
	
	// Make sure the temporary buffer is big enough.
	enlargeBuffer( dataBuffer, dataBufferSize, vertexDataSize );
	
	UByte* writePosition = dataBuffer;
	
	// Write each material.
	for ( Index i = 0; i < numVertices; i++ )
	{
		const SoundVertex& v = vertices[i];
		writeFloat32( writePosition, v.x );
		writeFloat32( writePosition, v.y );
		writeFloat32( writePosition, v.z );
	}
	
	// Write all of the vertices to the file.
	stream.writeData( dataBuffer, vertexDataSize );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Helper Methods
//############		
//##########################################################################################
//##########################################################################################




void SoundMesh:: enlargeBuffer( UByte*& data, Size& dataSize, Size desiredSize )
{
	if ( dataSize < desiredSize )
	{
		if ( data )
			util::deallocate( data );
		
		data = util::allocate<UByte>( desiredSize );
		dataSize = desiredSize;
	}
}




void SoundMesh:: releaseBuffer( const UByte* data )
{
	if ( data )
		util::deallocate( data );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Version 1 Specification
//############		
//##########################################################################################
//##########################################################################################




/**
  * Version 1 of the GSound Sound Mesh binary format.
  *
  * Primitive types:
  * - float32 - single-precision IEEE 754 floating point number.
  * - uint8 - 8-bit unsigned integer.
  * - uint16 - 16-bit unsigned integer.
  * - uint32 - 32-bit unsigned integer.
  * - uint64 - 64-bit unsigned integer.
  *
  * The version-independent 16-byte header for the file is as follows:
  * - Bytes 0 through 8: The ASCII character string 'SOUNDMESH'.
  * - Byte 9: uint8 indicating the version number of the mesh format.
  * - Byte 10: uint8 that specifies the endianness of the file, 0 if little endian, 1 if big endian.
  * - Byte 11: reserved/padding.
  * - Bytes 12 through 15: uint32 indicating a checksum of the rest of the file
  *   after the header. The checksum is computed by summing the value of every byte in the file as
  *   interpreted as 8-bit integers and allowing overflow. This value is provided to detect corruption
  *   of the file. A value of 0 indicates that the checksum is not used.
  *
  * The version-specific mesh header contains basic information about the mesh:
  * - numVertices: uint64 specifying the number of vertices in the mesh.
  * - numTriangles: uint64 specifying the number of triangles in the mesh.
  * - numEdges: uint64 specifying the number of diffraction edges in the mesh.
  * - numNeighbors: uint64 specifying the number of neighbor indices in the mesh.
  * - numMaterials: uint64 specifying the number of materials in the mesh.
  *
  * The materials for the mesh:
  * - reflectivity: numFrequencyBands*float32 in [0,1] indicating how reflective a surface is.
  * - scattering: numFrequencyBands*float32 in [0,1] indicating the fraction of reflected sound that is scattered.
  * - transmission: numFrequencyBands*float32 in [0,1] indicating the fraction of non-reflected sound that
  *   is transmitted through a material.
  *
  * The vertices for the mesh:
  * - vertices: numVertices*3*float32 vertex coordinates specified as (x0, y0, z0), (x1, y1, z1), ...
  *
  * The triangles for the mesh: (per triangle)
  * - vertices: 3*uint64 specifying the indices of the triangle's vertices.
  * - edges: 3*uint64 specifying the 1-offset indices of the triangle's diffraction edges,
  *   or 0 if there is no edge.
  * - material: uint32 indicating the index of the triangle's material in the mesh.
  * - key: uint32 indicating the index (0, 1, 2) of the triangle's subdivision key vertex.
  * - numRows: uint32 indicating the number of rows in the triangle's subdivision.
  * - numColumns: uint32 indicating the number of columns in the triangle's subdivision.
  *
  * The edge graph for the mesh: (per edge)
  * - vertices: 2*uint64 specifying the indices of the edge vertices.
  * - triangles: 2*uint64 specifying the indices of the edge triangles.
  * - edges: 2*uint16 specifying the edge indices of the edge triangles for this edge.
  * - numNeighbors: uint64 specifying the number of neighbors for the edge.
  * - neighborOffset: uint64 specifying the offset of this edge's first neighbor in the neighbor list.
  * - plane1: 4*float32 specifying the normal, then offset for the first triangle's oriented plane.
  * - plane2: 4*float32 specifying the normal, then offset for the second triangle's oriented plane.
  *
  * The edge graph for the mesh:
  * - neighbors: numNeighbors*uint64 specifying the edges neighbors in the mesh.
  */




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
