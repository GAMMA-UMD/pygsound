#include "SoundSource.hpp"
#include "Scene.hpp"
#include "SoundMesh.hpp"
#include "Listener.hpp"
#include "Context.hpp"
#include <iostream>

namespace omm = om::math;
namespace omt = om::time;

Scene::Scene()
{
	m_scene.addObject( &m_soundObject );
}

void
Scene::setMesh( SoundMesh &_mesh )
{
	m_soundObject.setMesh( &_mesh.m_mesh );
	m_soundObject.setTransform( omm::Transform3f( omm::Vector3f( 0.0f, 0.0f, 0.0f ) ) );
}

py::dict
Scene::computeIR( SoundSource &_source, Listener &_listener, Context &_context )
{
	m_scene.addSource( &_source.m_source);
	m_scene.addListener( &_listener.m_listener );
	if (m_scene.getObjectCount() == 0){
        std::cout << "object count is zero, cannot propagate sound!" << std::endl;
	}
    propagator.propagateSound(m_scene, _context.internalPropReq(), sceneIR);
	const gs::SoundSourceIR& sourceIR = sceneIR.getListenerIR(0).getSourceIR(0);
	gs::ImpulseResponse result;
	result.setIR(sourceIR, _listener.m_listener, _context.internalIRReq());

	auto rate = result.getSampleRate();

    auto *sample = result.getChannel( 0 );
	std::vector<float> samples(sample, sample+result.getLengthInSamples());

	m_scene.clearSources();
	m_scene.clearListeners();

	py::dict ret;
	ret["rate"] = rate;
	ret["samples"] = samples;
	return ret;

//    return result;
}

py::dict
Scene::computeMultichannelIR( SoundSource &_source, Listener &_listener, Context &_context )
{
	m_scene.addSource( &_source.m_source);
	m_scene.addListener( &_listener.m_listener );
	if (m_scene.getObjectCount() == 0){
		std::cout << "object count is zero, cannot propagate sound!" << std::endl;
	}
	propagator.propagateSound(m_scene, _context.internalPropReq(), sceneIR);
	const gs::SoundSourceIR& sourceIR = sceneIR.getListenerIR(0).getSourceIR(0);
//	gs::ImpulseResponse result;
//	result.setIR(sourceIR, _listener.m_listener, _context.internalIRReq());
	auto result = std::make_shared<gs::ImpulseResponse>();
	result->setIR(sourceIR, _listener.m_listener, _context.internalIRReq());
	auto rate = result->getSampleRate();

    auto numOfChannels = int(result->getChannelCount());
    assert(numOfChannels > 0);

    py::list samples;
    for (int ch = 0; ch < numOfChannels; ch++)
    {
        auto *sample_ch = result->getChannel(ch);
		std::vector<float> samples_ch(sample_ch, sample_ch+result->getLengthInSamples());
        samples.append(samples_ch);
    }
    py::dict ret;
    ret["rate"] = rate;
    ret["samples"] = samples;
    m_scene.clearSources();
    m_scene.clearListeners();

    return ret;
}
