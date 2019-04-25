/*
 * Project:     GSound
 * 
 * File:        gsound/gsSoundMesh.h
 * Contents:    gsound::SoundMesh class declaration
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


#ifndef INCLUDE_GSOUND_SOUND_MESH_H
#define INCLUDE_GSOUND_SOUND_MESH_H


#include "gsConfig.h"


#include "internal/gsInternalSoundTriangle.h"
#include "internal/gsDiffractionGraph.h"
#include "gsSoundTriangle.h"
#include "gsSoundMaterial.h"
#include "gsSoundRay.h"


//##########################################################################################
//******************************  Start GSound Namespace  **********************************
GSOUND_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class representing a static triangle mesh that is used in sound propagation.
/**
  * A mesh contains static preprocessed geometry data that can be used as
  * an obstacle in a scene and shared among multiple SoundObject's.
  *
  * Meshes should not be created directly, but instead created through
  * a SoundMeshPreprocessor that sanitizes the geometry and builds
  * a BVH for fast ray tracing.
  *
  * Once a mesh is preprocessed, it can be
  */
class SoundMesh
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a default sound mesh that has no triangles.
			/**
			  * This constructor is used to create a sound mesh object that is
			  * filled with mesh data from a SoundMeshPreprocessor.
			  */
			SoundMesh();
			
			
			/// Create a copy of another mesh. The new mesh shares the other mesh's internal data.
			SoundMesh( const SoundMesh& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy this mesh and all data that it contains.
			~SoundMesh();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the contents of one mesh to another.
			/**
			  * The new mesh shares the other mesh's internal data.
			  * 
			  * @param other - the mesh object whose state will be copied.
			  * @return a reference to this mesh object in order to allow assignment chaining.
			  */
			SoundMesh& operator = ( const SoundMesh& other );
			
			
		//********************************************************************************
		//******	Triangle Accessor Methods
			
			
			/// Return the number of triangles in this mesh.
			/**
			  * @return the number of triangles in this mesh.
			  */
			GSOUND_FORCE_INLINE Size getTriangleCount() const
			{
				return triangles->getSize();
			}
			
			
			/// Return the triangle in this mesh at the specified index.
			/**
			  * If the specified triangle index is outside the valid range of triangle
			  * indices, an assertion is raised. Otherwise, the triangle at the specified
			  * index is returned.
			  * 
			  * @param triangleIndex - the index of the triangle to get a reference to.
			  * @return the triangle at the specified index in this mesh.
			  */
			GSOUND_INLINE SoundTriangle getTriangle( Index triangleIndex ) const
			{
				GSOUND_DEBUG_ASSERT( triangleIndex < triangles->getSize() );
				
				const SoundVertex* verticesStart = vertices->getPointer();
				const SoundMaterial* materialsStart = materials->getPointer();
				const TriangleType& triangle = (*triangles)[triangleIndex];
				
				return SoundTriangle( triangle.getVertex(0) - verticesStart, triangle.getVertex(1) - verticesStart,
										triangle.getVertex(2) - verticesStart, triangle.getMaterial() - materialsStart );
			}
			
			
		//********************************************************************************
		//******	Vertex Accessor Methods
			
			
			/// Return the number of vertices in this SoundMesh.
			/**
			  * @return the number of vertices in this SoundMesh.
			  */
			GSOUND_FORCE_INLINE Size getVertexCount() const
			{
				return vertices->getSize();
			}
			
			
			/// Return a reference to the vertex at the specified index in this SoundMesh.
			/**
			  * If the specified vertex index is outside the valid range of vertex
			  * indices, an assertion is raised. Otherwise, a reference to the
			  * vertex at the specified index is returned.
			  * 
			  * @param vertexIndex - the index of the vertex to get a reference to.
			  * @return a reference to the vertex at the specified index in this SoundMesh.
			  */
			GSOUND_INLINE const SoundVertex& getVertex( Index vertexIndex ) const
			{
				GSOUND_DEBUG_ASSERT( vertexIndex < vertices->getSize() );
				
				return (*vertices)[vertexIndex];
			}
			
			
		//********************************************************************************
		//******	Material Accessor Methods
			
			
			/// Return the number of materials in this mesh.
			GSOUND_FORCE_INLINE Size getMaterialCount() const
			{
				return materials->getSize();
			}
			
			
			/// Return a reference to the material at the specified index in this mesh.
			GSOUND_FORCE_INLINE SoundMaterial& getMaterial( Index materialIndex )
			{
				GSOUND_DEBUG_ASSERT( materialIndex < materials->getSize() );
				
				return (*materials)[materialIndex];
			}
			
			
			/// Return a const reference to the material at the specified index in this mesh.
			GSOUND_FORCE_INLINE const SoundMaterial& getMaterial( Index materialIndex ) const
			{
				GSOUND_DEBUG_ASSERT( materialIndex < materials->getSize() );
				
				return (*materials)[materialIndex];
			}
			
			
			/// Set the material at the specified index in this mesh.
			GSOUND_INLINE void setMaterial( Index materialIndex, const SoundMaterial& newMaterial )
			{
				GSOUND_DEBUG_ASSERT( materialIndex < materials->getSize() );
				
				(*materials)[materialIndex] = newMaterial;
			}
			
			
		//********************************************************************************
		//******	Diffraction Graph Accessor Methods
			
			
			/// Return a pointer to the diffraction graph information for this mesh level of detail.
			/**
			  * If NULL is returned, it means that the mesh has no preprocessed diffraction information.
			  */
			GSOUND_FORCE_INLINE const internal::DiffractionGraph* getDiffractionGraph() const
			{
				return diffractionGraph;
			}
			
			
		//********************************************************************************
		//******	Bounding Volume Accessor Methods
			
			
			/// Return a reference to the bounding sphere of this mesh.
			GSOUND_FORCE_INLINE const Sphere3f& getBoundingSphere() const
			{
				return boundingSphere;
			}
			
			
			/// Return a reference to the axis-aligned bounding box of this mesh.
			GSOUND_FORCE_INLINE const AABB3f& getBoundingBox() const
			{
				return boundingBox;
			}
			
			
			/// Return a pointer to this mesh's bounding volume hierarchy.
			GSOUND_FORCE_INLINE const BVH* getBVH() const;
			
			
		//********************************************************************************
		//******	Name String Accessor Method
			
			
			/// Return a reference to a string containing a human-readable name for this sound mesh.
			GSOUND_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set a string containing a human-readable name for this sound mesh.
			GSOUND_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	User Data Accessor Methods
			
			
			/// Return an opaque pointer to user-defined data for this SoundMesh.
			GSOUND_FORCE_INLINE void* getUserData() const
			{
				return userData;
			}
			
			
			/// Set an opaque pointer to user-defined data for this SoundMesh.
			GSOUND_FORCE_INLINE void setUserData( void* newUserData )
			{
				userData = newUserData;
			}
			
			
		//********************************************************************************
		//******	Size In Bytes Accessor Method
			
			
			/// Return the approximate size in bytes of this mesh's allocated memory.
			Size getSizeInBytes() const;
			
			
		//********************************************************************************
		//******	Ray Tracing Methods
			
			
			/// Trace a ray through the mesh and return whether or not the ray hit anything.
			GSOUND_FORCE_INLINE Bool intersectRay( const Ray3f& ray, Float maxDistance ) const
			{
				SoundRay soundRay( ray, 0.0f, maxDistance );
				
				this->intersectRay( soundRay );
				
				return soundRay.hitValid();
			}
			
			
			/// Trace a ray through this mesh and compute the closest intersection.
			GSOUND_FORCE_INLINE void intersectRay( SoundRay& ray ) const;
			
			
		//********************************************************************************
		//******	Mesh Load/Save Methods
			
			
			/// Save this mesh to the specified NULL-terminated UTF-8 file path.
			/**
			  * The method returns whether or not the mesh was able to be successfully written.
			  */
			Bool save( const char* pathToFile ) const;
			
			
			/// Save a mesh to the specified data output stream, starting at the stream's current position.
			/**
			  * The stream must be valid, open, and ready to be written to.
			  * The method returns whether or not the mesh was able to be successfully written.
			  */
			Bool save( om::DataOutputStream& stream ) const;
			
			
			/// Load a mesh from the specified NULL-terminated UTF-8 file path.
			/**
			  * If the mesh loading fails, a NULL pointer is returned.
			  */
			static Bool load( const char* pathToFile, SoundMesh& mesh );
			
			
			/// Load a mesh from the specified data input stream, starting at the stream's current position.
			/**
			  * The stream must be valid, open, and ready to be read from.
			  * If the mesh loading fails, a NULL pointer is returned.
			  */
			static Bool load( om::DataInputStream& stream, SoundMesh& mesh );
			
			
	private:
		
		//********************************************************************************
		//******	Friend Class Declarations
			
			
			/// A class that manages the BVH for this mesh.
			class MeshBVH;
			
			
			/// Mark the SoundScene class as a friend so that it can access internal data.
			friend class SoundObject;
			
			
			/// Mark the SoundScene class as a friend so that it can access internal data.
			friend class SoundScene;
			
			
			/// Mark the SoundMeshPreprocessor class as a friend so that it can create meshes using a private constructor.
			friend class SoundMeshPreprocessor;
			
			
			/// Define the type that is used for a meshes's triangles.
			typedef internal::InternalSoundTriangle TriangleType;
			
			
		//********************************************************************************
		//******	Mesh Data Accessor Method
			
			
			/// Create a new mesh which uses the given vertices, triangles, materials, and diffraction data.
			/**
			  * The new mesh uses the given edge and edge visibility data for diffraction queries.
			  */
			void setData( const Shared<ArrayList<SoundVertex> >& newVertices,
						const Shared<ArrayList<TriangleType> >& newTriangles,
						const Shared<ArrayList<SoundMaterial> >& newMaterials,
						const Shared<internal::DiffractionGraph>& newDiffractionGraph );
			
			
		//********************************************************************************
		//******	Mesh I/O Methods
			
			
			/// Save the specified sound mesh to a data output stream.
			static Bool saveMeshToStream( const SoundMesh& mesh, om::DataOutputStream& stream );
			
			
			/// Load a sound mesh from the specified data stream.
			static Bool loadMeshFromStream( om::DataInputStream& stream, SoundMesh& mesh );
			
			
			/// Load a version-1 sound mesh from the specified data stream with the given endianness.
			static Bool loadMeshVersion1( om::DataInputStream& stream, om::data::Endianness endianness, SoundMesh& mesh );
			
			
		//********************************************************************************
		//******	Mesh Reading Helper Methods
			
			
			GSOUND_FORCE_INLINE static void readUInt16( UByte*& data, om::data::Endianness endianness, UInt16& value );
			GSOUND_FORCE_INLINE static void readUInt32( UByte*& data, om::data::Endianness endianness, UInt32& value );
			GSOUND_FORCE_INLINE static void readUInt64( UByte*& data, om::data::Endianness endianness, UInt64& value );
			GSOUND_FORCE_INLINE static void readFloat32( UByte*& data, om::data::Endianness endianness, Float32& value );
			
			
			GSOUND_FORCE_INLINE static Bool readResponse( UByte*& dataBuffer, Size& dataBufferSize,
															om::data::Endianness endianness,
															om::DataInputStream& stream,
															FrequencyResponse& response );
			
			
			GSOUND_INLINE Bool static readMaterials( ArrayList<SoundMaterial>& materials, Size numMaterials,
													UByte*& dataBuffer, Size& dataBufferSize,
													om::data::Endianness endianness,
													om::DataInputStream& stream );
			
			
			GSOUND_INLINE Bool static readVertices( ArrayList<SoundVertex>& vertices, Size numVertices,
													UByte*& dataBuffer, Size& dataBufferSize,
													om::data::Endianness endianness,
													om::DataInputStream& stream );
			
			
		//********************************************************************************
		//******	Mesh Writing Helper Methods
			
			
			GSOUND_FORCE_INLINE static void writeUInt16( UByte*& data, UInt16 value );
			GSOUND_FORCE_INLINE static void writeUInt32( UByte*& data, UInt32 value );
			GSOUND_FORCE_INLINE static void writeUInt64( UByte*& data, UInt64 value );
			GSOUND_FORCE_INLINE static void writeFloat32( UByte*& data, Float32 value );
			
			
			GSOUND_FORCE_INLINE static void writeResponse( UByte*& data, const FrequencyResponse& response );
			
			
			GSOUND_INLINE void static writeMaterials( const ArrayList<SoundMaterial>& materials,
													UByte*& dataBuffer, Size& dataBufferSize,
													om::DataOutputStream& stream );
			
			
			GSOUND_INLINE void static writeVertices( const ArrayList<SoundVertex>& vertices,
													UByte*& dataBuffer, Size& dataBufferSize,
													om::DataOutputStream& stream );
			
			
		//********************************************************************************
		//******	Buffer Helper Methods
			
			
			GSOUND_FORCE_INLINE static void enlargeBuffer( UByte*& data, Size& dataSize, Size desiredSize );
			
			
			GSOUND_FORCE_INLINE static void releaseBuffer( const UByte* data );
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of all of the vertices in this mesh.
			Shared<ArrayList<SoundVertex> > vertices;
			
			
			/// A list of all of the triangles in this mesh.
			Shared<ArrayList<TriangleType> > triangles;
			
			
			/// A list of all of the materials in this mesh.
			Shared<ArrayList<SoundMaterial> > materials;
			
			
			/// A pointer to an object that manages the BVH for this mesh.
			MeshBVH* bvh;
			
			
			/// An object which describes the diffraction edges for this mesh.
			Shared<internal::DiffractionGraph> diffractionGraph;
			
			
			/// A bounding box for the triangle mesh.
			AABB3f boundingBox;
			
			
			/// A bounding sphere for the triangle mesh.
			Sphere3f boundingSphere;
			
			
			/// A string that contains a human-readable name for this sound mesh.
			UTF8String name;
			
			
			/// An opaque pointer to user-defined data for this SoundMesh.
			void* userData;
			
			
			
};


//##########################################################################################
//##########################################################################################
//############		
//############		Mesh BVH Class Declaration
//############		
//##########################################################################################
//##########################################################################################




class SoundMesh:: MeshBVH : public BVHGeometry
{
	public:
		
		//********************************************************************************
		//******	Constructors
			
			
			/// Create a triangle interface for the specified mesh shape.
			GSOUND_INLINE MeshBVH( const SoundMesh* newShape )
				:	shape( newShape )
			{
				bvh.setGeometry( this );
			}
			
			
		//********************************************************************************
		//******	Primitive-BVH Interface Methods
			
			
			/// Return the number of BVH's contained in this scene geometry.
			virtual om::bvh::PrimitiveCount getPrimitiveCount() const
			{
				return (om::bvh::PrimitiveCount)(*shape->triangles).getSize();
			}
			
			
			/// Return the number of BVH's contained in this scene geometry.
			virtual BVHGeometry::Type getPrimitiveType() const
			{
				return BVHGeometry::TRIANGLES;
			}
			
			
			/// Return an axis-aligned bounding box for the BVH with the specified index.
			virtual AABB3f getPrimitiveAABB( om::bvh::PrimitiveIndex primitiveIndex ) const
			{
				const internal::InternalSoundTriangle& t = (*shape->triangles)[primitiveIndex];
				
				AABB3f result( *t.vertex[0] );
				result.enlargeFor( *t.vertex[1] );
				result.enlargeFor( *t.vertex[2] );
				
				return result;
			}
			
			
			/// Return an axis-aligned bounding box for the BVH with the specified index.
			virtual Sphere3f getPrimitiveBoundingSphere( om::bvh::PrimitiveIndex primitiveIndex ) const
			{
				const internal::InternalSoundTriangle& t = (*shape->triangles)[primitiveIndex];
				
				return Sphere3f( *t.vertex[0], *t.vertex[1], *t.vertex[2] );
			}
			
			
			/// Get the vertices of the triangle at the specified index in this primitive set.
			virtual Bool getTriangle( om::bvh::PrimitiveIndex primitiveIndex, Vector3f& v0, Vector3f& v1, Vector3f& v2 ) const
			{
				if ( primitiveIndex >= (*shape->triangles).getSize() )
					return false;
				
				const internal::InternalSoundTriangle& t = (*shape->triangles)[primitiveIndex];
				v0 = *t.vertex[0];
				v1 = *t.vertex[1];
				v2 = *t.vertex[2];
				
				return true;
			}
			
			
		//********************************************************************************
		//******	Public Data Members
			
			
			/// The BVH that holds the mesh geometry.
			om::bvh::AABBTree4 bvh;
			
			
			/// A pointer to the mesh shape that is a source of primitives.
			const SoundMesh* shape;
			
			
};




const BVH* SoundMesh:: getBVH() const
{
	return &bvh->bvh;
}




void SoundMesh:: intersectRay( SoundRay& ray ) const
{
	bvh->bvh.intersectRay( ray );
}




//##########################################################################################
//******************************  End GSound Namespace  ************************************
GSOUND_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_GSOUND_SOUND_MESH_H
