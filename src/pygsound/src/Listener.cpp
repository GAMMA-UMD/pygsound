#include "Listener.hpp"
#include <stdexcept>

namespace omm = om::math;

Listener::Listener( std::vector<float> _loc )
{
	setPosition( _loc );
	m_listener.setSensitivity( 0 );
}

void
Listener::setPosition( const std::vector<float> &_loc )
{
	m_listener.setTransform( omm::Transform3f( omm::Vector3f( _loc.data()  ) ));
}

std::vector<float>
Listener::getPosition() const
{
	auto vec = m_listener.getTransform();

	std::vector<float> ret;
	for ( int i = 0; i < 3; ++i )
		ret.push_back( vec.position[i] );

	return ret;
}

void
Listener::setRadius( float _radius )
{
	m_listener.setRadius( _radius );
}

float
Listener::getRadius() const
{
	return m_listener.getRadius();
}


//void
//Listener::setChannelLayoutType( oms::ChannelLayout::Type _layout )
//{
//	m_listener.setChannelLayout( _layout );
//}
//
//oms::ChannelLayout::Type
//Listener::getChannelLayoutType() const
//{
//	return m_listener.getChannelLayout().getType();
//}
//
//
//void
//Listener::setChannelLayout( oms::ChannelLayout _layout )
//{
//	m_listener.setChannelLayout( _layout );
//}
//
//oms::ChannelLayout
//Listener::getChannelLayout() const
//{
//	return m_listener.getChannelLayout();
//}
//
//void
//Listener::setArrayLayout( oms::MicrophoneArrays _layout )
//{
//	m_listener.setChannelLayout( _layout );
//}

std::ostream &
operator<<( std::ostream &_strm, const Listener &_list )
{
	auto vec = _list.m_listener.getTransform().position;

	_strm << "[ " << vec.x << ", " << vec.y << ", " << vec.z << " ]";
	
	return _strm;
}
