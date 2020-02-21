#include "SoundSource.hpp"
#include <stdexcept>

namespace omm = om::math;

SoundSource::SoundSource( std::vector<float> _loc )
{
	setPosition( _loc );
}

void
SoundSource::setPosition( const std::vector<float> &_loc )
{
    m_source.setTransform( omm::Transform3f( omm::Vector3f( _loc.data() ) ) );
}

std::vector<float>
SoundSource::getPosition() const
{
	auto vec = m_source.getTransform();

	std::vector<float> ret;
	for ( int i = 0; i < 3; ++i )
		ret.push_back( vec.position[i] );

	return ret;
}

void
SoundSource::setRadius( float _radius )
{
	m_source.setRadius( _radius );
}

float
SoundSource::getRadius() const
{
	return m_source.getRadius();
}

void
SoundSource::setPower( float _power )
{
	m_source.setPower( _power );
}

float
SoundSource::getPower() const
{
	return m_source.getPower();
}
