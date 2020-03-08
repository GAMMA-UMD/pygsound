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

    py::dict computeIR( SoundSource &_source, Listener &_listener, Context &_context );
    py::dict computeMultichannelIR( SoundSource &_source, Listener &_listener, Context &_context );

public:

	gs::SoundScene  m_scene;
	gs::SoundObject m_soundObject;
	gs::SoundPropagator propagator;
	gs::SoundSceneIR sceneIR;

};

#endif  // INC_SCENE_HPP
