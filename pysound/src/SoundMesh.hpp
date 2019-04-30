#ifndef INC_SOUNDMESH_HPP
#define INC_SOUNDMESH_HPP

#include <memory>
#include <string>

#include <gsound/gsSoundMesh.h>

class SoundMesh
{
public:

	static std::shared_ptr< SoundMesh > loadObj( const std::string &_path, std::string _basepath, float _forceabsorp = -1.0, float _forcescatter = -1.0 );
	static std::shared_ptr< SoundMesh > createBox( float _width, float _length, float _height, float _absorp = 0.5, float _scatter = 0.5 );

	gsound::SoundMesh &mesh() { return m_mesh; }

private:

	friend class Scene;

	gsound::SoundMesh   m_mesh;
};

#endif  // INC_SOUNDMESH_HPP
