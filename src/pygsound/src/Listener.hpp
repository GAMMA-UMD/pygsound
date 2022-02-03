#ifndef INC_LISTENER_HPP
#define INC_LISTENER_HPP

#include <pybind11/pybind11.h>
#include <gsound/gsSoundListener.h>
#include <ostream>
#include "MicrophoneArrays.hpp"

namespace gs = gsound;
namespace py = pybind11;
namespace oms = om::sound;

class Listener
{
public:

	explicit Listener( std::vector<float> _loc );

	void setPosition( const std::vector<float> &_loc );
	std::vector<float> getPosition() const;

	void setRadius( float _radius );
	float getRadius() const;

//	void setChannelLayoutType( oms::ChannelLayout::Type _layout );
//	oms::ChannelLayout::Type getChannelLayoutType() const;
//
//    void setChannelLayout( oms::ChannelLayout _layout );
//    oms::ChannelLayout getChannelLayout() const;
//
//    void setArrayLayout( oms::MicrophoneArrays _layout );

private:

	friend class Scene;
	friend std::ostream &operator<<( std::ostream &_strm,
	                                 const Listener &_list );

	gs::SoundListener m_listener;
};

#endif // INC_LISTENER_HPP
