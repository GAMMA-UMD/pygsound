#ifndef INC_SCENE_HPP
#define INC_SCENE_HPP

#include "Python.hpp"
#include <gsound/gsSoundScene.h>
#include <gsound/gsSoundObject.h>
#include <gsound/gsSoundPropagator.h>
#include <gsound/gsImpulseResponse.h>
#include <pybind11/stl.h>

namespace gs = gsound;
namespace py = pybind11;

class SoundMesh;
class SoundSource;
class Listener;
class Context;

class Scene
{
public:

	Scene();

	void setMesh( SoundMesh &_mesh );

//	void addSource( SoundSource &_source );
//	void addListener( Listener &_listener );

    py::dict computeIR( SoundSource &_source, Listener &_listener, Context &_context );
//    py::dict computeBinauralIR( SoundSource &_source, Listener &_listener );
    py::dict computeMultichannelIR( SoundSource &_source, Listener &_listener, Context &_context );
//    om::Bool saveIR( gs::ImpulseResponse &IR, std::string filename );

public:

	gs::SoundScene  m_scene;
	gs::SoundObject m_soundObject;
	gs::SoundPropagator propagator;
	gs::SoundSceneIR sceneIR;
//    om::resources::ResourceManager resourceManager;
//    om::sound::resources::SoundResourceModule soundResourceModule;

};

#endif  // INC_SCENE_HPP
