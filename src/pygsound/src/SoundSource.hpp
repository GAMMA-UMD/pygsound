#ifndef INC_SOUNDSOURCE_HPP
#define INC_SOUNDSOURCE_HPP

#include "Python.hpp"
#include <gsound/gsSoundSource.h>

namespace gs = gsound;
namespace py = pybind11;

class SoundSource
{
public:

	explicit SoundSource( std::vector<float> _loc );

	void setPosition( const std::vector<float> &_loc );
	std::vector<float> getPosition() const;

	void setRadius( float _radius );
	float getRadius() const;

	void setPower( float _power );
	float getPower() const;

private:

	friend class Scene;

	gs::SoundSource m_source;
};

#endif  // INC_SOUNDSOURCE_HPP
