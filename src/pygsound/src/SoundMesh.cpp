#include "SoundMesh.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <gsound/gsPropagationRequest.h>
#include <gsound/gsSoundObject.h>
#include <gsound/gsSoundMesh.h>
#include <gsound/gsSoundMeshPreprocessor.h>
#include <om/omMath.h>
#include <om/omSound.h>

namespace gs = gsound;
namespace oms = om::sound;
namespace omm = om::math;

std::shared_ptr< SoundMesh >
SoundMesh::loadObj( const std::string &_path, float _forceabsorp, float _forcescatter )
{
	tinyobj::attrib_t attrib;
	std::vector< tinyobj::shape_t > shapes;
	std::vector< tinyobj::material_t > materials;

	std::string err;
    std::string basepath;
    auto loc = _path.rfind('/');
    basepath = _path;
    basepath.erase(loc + 1);


	bool errv = tinyobj::LoadObj( &attrib, &shapes, &materials, &err, _path.c_str(), basepath.c_str() );
	if ( !errv )
		throw std::runtime_error( err );
	else if ( !err.empty() )
		std::cerr << "WARNING: " << err << "\n";
    if ( materials.empty() )
        std::cerr << "WARNING: no material loaded for " << _path << "\n";

	std::vector< gs::SoundVertex > verts;
	std::vector< gs::SoundTriangle > tris;
	std::vector< gs::SoundMaterial > mats;

	// reserve
	std::size_t nverts = attrib.vertices.size() / 3;
	std::size_t ntris = 0;
	std::size_t nmats = materials.size();
	for ( const auto &shape : shapes )
	{
		ntris += shape.mesh.indices.size() / 3;
	}

	verts.reserve( nverts );
	tris.reserve( ntris );
	mats.reserve( nmats );

	for ( size_t vid = 0; vid < attrib.vertices.size(); vid += 3 )
	{
		float *vert = &attrib.vertices[vid];
		verts.emplace_back( vert[0], vert[1], vert[2] );
	}


	const std::vector<float> spec{ 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f };

    for ( const auto &material : materials )
	{
		//oms::FrequencyData reflec, scatter, trans;
		gs::FrequencyResponse reflec, scatter, trans;
		if (0 < _forceabsorp)
		{
			auto absorpsample = om::Float(_forceabsorp);
			//std::cout << "Using user-defined absorption " << absorpsample <<  " for " << _path << std::endl;
			for (std::size_t i = 0; i < spec.size(); ++i)
				reflec.setFrequency(spec[i], std::sqrt(1.0f - absorpsample));
		}
		else {
			auto pos = material.unknown_parameter.find("sound_a");
			if (pos == std::end(material.unknown_parameter)) {
				pos = material.unknown_parameter.find("sound_r");
				if (pos == std::end(material.unknown_parameter)) {
					//reflec = oms::FrequencyData(0.1f);
					reflec = gs::FrequencyResponse(0.1f);
				} else {
					// Convet string into list of floats
					std::vector<float> refls;
					std::istringstream iss(pos->second);

					std::copy(std::istream_iterator<float>(iss),
							  std::istream_iterator<float>(),
							  std::back_inserter(refls));

					for (std::size_t i = 0;
						 i < std::min(refls.size(), spec.size()); ++i)
						reflec.setFrequency(spec[i], refls[i]);
				}
			} else {
				// Convet string into list of floats
				std::vector<float> absorps;
				std::istringstream iss(pos->second);

				std::copy(std::istream_iterator<float>(iss),
						  std::istream_iterator<float>(),
						  std::back_inserter(absorps));

				for (std::size_t i = 0; i < std::min(absorps.size(), spec.size()); ++i)
					reflec.setFrequency(spec[i], std::sqrt(1.0f - absorps[i]));
			}
		}
		auto pos = material.unknown_parameter.find( "sound_s" );
		if (0 < _forcescatter)
		{
			auto scattersample = om::Float(_forcescatter);
			//std::cout << "Using user-defined scattering " << scattersample <<  " for " << _path << std::endl;
			for (std::size_t i = 0; i < spec.size(); ++i)
				scatter.setFrequency(spec[i], scattersample);
		}
		else
		{
			if ( pos == std::end( material.unknown_parameter ) )
				scatter = gs::FrequencyResponse(0.5f);
			else {
				// Convet string into list of floats
				std::vector< float > scatters;
				std::istringstream iss( pos->second );

				std::copy(	std::istream_iterator< float >( iss ),
							  std::istream_iterator< float >(),
							  std::back_inserter( scatters ) );

				for ( std::size_t i = 0; i < std::min( scatters.size(), spec.size() ); ++i )
					scatter.setFrequency( spec[i], scatters[i] );
			}
		}

		//trans = oms::FrequencyData( 0.0f );
		trans = gs::FrequencyResponse( 0.0f );

		mats.emplace_back( reflec, scatter, trans );
		//std::cout << "Average scattering: " << mats[mats.size()-1].getScattering().getAverage() << std::endl;
	}

	for ( const auto &shape : shapes )
	{
		if ( ( shape.mesh.indices.size() % 3 ) != 0 )
			throw std::runtime_error( "problem reading indices" );

		for ( size_t fid = 0; fid < shape.mesh.indices.size() / 3; ++fid )
		{
			const auto *fids = &shape.mesh.indices[fid * 3 ];
			gs::Index idxs[] = { static_cast< gs::Index >( fids[0].vertex_index ),
			                     static_cast< gs::Index >( fids[1].vertex_index ),
			                     static_cast< gs::Index >( fids[2].vertex_index ) };
			gs::Index mat_id = shape.mesh.material_ids[fid];

			tris.emplace_back( idxs[0], idxs[1], idxs[2], mat_id );
		}
	}

	auto ret = std::make_shared< SoundMesh >();

	gs::SoundMeshPreprocessor preprocessor;
    gs::MeshRequest meshRequest;
    meshRequest.minDiffractionEdgeAngle = 30;
    meshRequest.minDiffractionEdgeLength = 0.5;
	if ( !preprocessor.processMesh( &verts[0], verts.size(),
	                                &tris[0], tris.size(),
	                                &mats[0], mats.size(), meshRequest, ret->m_mesh ) )
		throw std::runtime_error( "Cannot preprocess sound mesh!" );


	return ret;
}


std::shared_ptr< SoundMesh >
SoundMesh::createBox( float _width, float _length, float _height, float _absorp, float _scatter )
{
	std::vector< gs::SoundVertex > verts;
	std::vector< gs::SoundTriangle > tris;
	std::vector< gs::SoundMaterial > mats;

	// reserve
	std::size_t nverts = 8;
	std::size_t ntris = 12;
	std::size_t nmats = 1;

	verts.reserve( nverts );
	tris.reserve( ntris );
	mats.reserve( nmats );

	verts.emplace_back( 0.0f, 0.0f, 0.0f );	//0
	verts.emplace_back( _width, 0.0f, 0.0f );	//1
	verts.emplace_back( 0.0f, 0.0f, _height );	//2
	verts.emplace_back( _width, 0.0f, _height );	//3
	verts.emplace_back( 0.0f, _length, 0.0f );	//4
	verts.emplace_back( _width, _length, 0.0f );	//5
	verts.emplace_back( 0.0f, _length, _height );	//6
	verts.emplace_back( _width, _length, _height );	//7

	tris.emplace_back(1, 2, 0, 0);
	tris.emplace_back(3, 6, 2, 0);
	tris.emplace_back(7, 4, 6, 0);
	tris.emplace_back(5, 0, 4, 0);
	tris.emplace_back(6, 0, 2, 0);
	tris.emplace_back(3, 5, 7, 0);
	tris.emplace_back(1, 3, 2, 0);
	tris.emplace_back(3, 7, 6, 0);
	tris.emplace_back(7, 5, 4, 0);
	tris.emplace_back(5, 1, 0, 0);
	tris.emplace_back(6, 4, 0, 0);
	tris.emplace_back(3, 1, 5, 0);

	const std::vector<float> spec{ 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f };

	gs::FrequencyResponse reflec, scatter, trans;

	for (std::size_t i = 0; i < spec.size(); ++i)
		reflec.setFrequency(spec[i], std::sqrt(1.0f - _absorp));

	for (std::size_t i = 0; i < spec.size(); ++i)
		scatter.setFrequency(spec[i], _scatter);

	trans = gs::FrequencyResponse( 0.0f );

	mats.emplace_back( reflec, scatter, trans );

	auto ret = std::make_shared< SoundMesh >();

	gs::SoundMeshPreprocessor preprocessor;

	if ( !preprocessor.processMesh( &verts[0], verts.size(),
									&tris[0], tris.size(),
									&mats[0], mats.size(), gs::MeshRequest(), ret->m_mesh ) )
		throw std::runtime_error( "Cannot preprocess sound mesh!" );


	return ret;
}


std::shared_ptr< SoundMesh >
SoundMesh::createBox( float _width, float _length, float _height, std::vector<float> _absorp, float _scatter )
{
    std::vector< gs::SoundVertex > verts;
    std::vector< gs::SoundTriangle > tris;
    std::vector< gs::SoundMaterial > mats;

    // reserve
    std::size_t nverts = 8;
    std::size_t ntris = 12;
    std::size_t nmats = 1;

    verts.reserve( nverts );
    tris.reserve( ntris );
    mats.reserve( nmats );

    verts.emplace_back( 0.0f, 0.0f, 0.0f );	//0
    verts.emplace_back( _width, 0.0f, 0.0f );	//1
    verts.emplace_back( 0.0f, 0.0f, _height );	//2
    verts.emplace_back( _width, 0.0f, _height );	//3
    verts.emplace_back( 0.0f, _length, 0.0f );	//4
    verts.emplace_back( _width, _length, 0.0f );	//5
    verts.emplace_back( 0.0f, _length, _height );	//6
    verts.emplace_back( _width, _length, _height );	//7

    tris.emplace_back(1, 2, 0, 0);
    tris.emplace_back(3, 6, 2, 0);
    tris.emplace_back(7, 4, 6, 0);
    tris.emplace_back(5, 0, 4, 0);
    tris.emplace_back(6, 0, 2, 0);
    tris.emplace_back(3, 5, 7, 0);
    tris.emplace_back(1, 3, 2, 0);
    tris.emplace_back(3, 7, 6, 0);
    tris.emplace_back(7, 5, 4, 0);
    tris.emplace_back(5, 1, 0, 0);
    tris.emplace_back(6, 4, 0, 0);
    tris.emplace_back(3, 1, 5, 0);

    const std::vector<float> spec{ 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f };
    if ( spec.size() != _absorp.size() )
        throw std::runtime_error( "Absorption coefficient list has incompatible length!" );

    gs::FrequencyResponse reflec, scatter, trans;

    for (std::size_t i = 0; i < spec.size(); ++i)
        reflec.setFrequency(spec[i], std::sqrt(1.0f - _absorp[i]));

    for (std::size_t i = 0; i < spec.size(); ++i)
        scatter.setFrequency(spec[i], _scatter);

    trans = gs::FrequencyResponse( 0.0f );

    mats.emplace_back( reflec, scatter, trans );

    auto ret = std::make_shared< SoundMesh >();

    gs::SoundMeshPreprocessor preprocessor;

    if ( !preprocessor.processMesh( &verts[0], verts.size(),
                                    &tris[0], tris.size(),
                                    &mats[0], mats.size(), gs::MeshRequest(), ret->m_mesh ) )
        throw std::runtime_error( "Cannot preprocess sound mesh!" );


    return ret;
}
