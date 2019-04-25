#include <pybind11/pybind11.h>
#include <gsound/gsound.h>
//#include <gsound/gsGSoundContext.h>
#include <gsound/gsPropagationRequest.h>
#include <gsound/gsSoundObject.h>
#include <gsound/gsSoundMesh.h>
//#include <gsound/gsSoundScene.h>
//#include <gsound/gsSoundSceneIR.h>
#include <gsound/gsSoundMeshPreprocessor.h>

#include <om/omResources.h>
#include <om/omMath.h>
#include <om/omSound.h>
#include <om/omIO.h>
#include <om/omData.h>

//#include <rapidxml/rapidxml.hpp>

#include "SoundMesh.hpp"
#include "Context.hpp"
#include "Scene.hpp"
#include "Listener.hpp"
#include "SoundSource.hpp"

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <ctime>


namespace gs = gsound;
namespace omr = om::resources;
namespace omm = om::math;
namespace omt = om::time;
namespace oms = om::sound;
//namespace po = boost::program_options;
//using namespace rapidxml;

int main( int _argc, const char **_argv )
{

	int nthreads = 4;
	int primaryRayCount = 20000;
	int frameCount = 1;
	std::string obj_path("/home/zhy/Codes/scenes/cube/cube.obj");

	auto ctx = Context();
    ctx.setDiffuseCount(2000);
    ctx.setSpecularCount(2000);
    ctx.setChannelLayout(oms::ChannelLayout::AMBISONIC_B);
    auto scene = Scene();

    // Load the obj file
    auto mesh = SoundMesh::loadObj(obj_path, "", 0.5);
    scene.setMesh(*mesh);

	gs::SoundListener listener;
	listener.setRadius(0.2f);
	listener.setPosition(omm::Vector3f(0, 0, 0));

	gs::SoundSource source;
	source.setRadius(0.2f);
	source.setPosition(omm::Vector3f(1, 1, 1));

	scene.m_scene.addSource(&source);
	scene.m_scene.addListener(&listener);

    int elapsed_time;
	om::data::UTF8String outfilename("IR.wav");

	gs::ImpulseResponse niceIR;
	gs::SoundPropagator propagator;
	gs::SoundSceneIR sceneIR;

	propagator.propagateSound(scene.m_scene, ctx.internalPropReq(), sceneIR);
	const gs::SoundSourceIR& sourceIR = sceneIR.getListenerIR(0).getSourceIR(0);
	niceIR.setIR(sourceIR, listener, ctx.internalIRReq());

//	resourceManager.save(outfilename, om::sound::Sound(niceIR.getBuffer()));

    return 0;
}
