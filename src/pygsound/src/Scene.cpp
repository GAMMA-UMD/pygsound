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

	auto rate = _context.getSampleRate();

    auto *sample = result.getChannel( 0 );
	std::vector<float> samples(sample, sample+result.getLengthInSamples());

	m_scene.clearSources();
	m_scene.clearListeners();

	py::dict ret;
	ret["rate"] = rate;
	ret["samples"] = samples;
	return ret;
}

py::dict
Scene::computeIRPairs( std::vector<std::vector<float>> &src_pos, std::vector<std::vector<float>> &lis_pos, Context &_context,
                        float src_radius, float src_power, float lis_radius)
{
    int n_src = src_pos.size();
    int n_lis = lis_pos.size();

    // listener propagation is most expensive
    bool invertBuffer = false;
    if (n_src < n_lis){
        invertBuffer = true;
        // TODO: invert source and listener for computation only
    }

    for (auto p : src_pos){
        auto source = new SoundSource(p);
        source->setRadius(src_radius);
        source->setPower(src_power);
        m_scene.addSource( &source->m_source );
    }
    for (auto p : lis_pos){
        auto listener = new Listener(p);
        listener->setRadius(lis_radius);
        m_scene.addListener( &listener->m_listener );
    }

	if (m_scene.getObjectCount() == 0){
        std::cout << "object count is zero, cannot propagate sound!" << std::endl;
	}

    propagator.propagateSound(m_scene, _context.internalPropReq(), sceneIR);

    py::list IRPairs(n_src);
    auto rate = _context.getSampleRate();
    for (int i_src = 0; i_src < n_src; ++i_src){
        py::list srcSamples(n_src);
        for (int i_lis = 0; i_lis < n_lis; ++i_lis){
            const gs::SoundSourceIR& sourceIR = sceneIR.getListenerIR(i_lis).getSourceIR(i_src);
            gs::ImpulseResponse result;
            result.setIR(sourceIR, *m_scene.getListener(i_lis), _context.internalIRReq());

            auto *sample = result.getChannel( 0 );
            std::vector<float> samples(sample, sample+result.getLengthInSamples());
            srcSamples[i_lis] = samples;
        }
        IRPairs[i_src] = srcSamples;
    }

	m_scene.clearSources();
	m_scene.clearListeners();

	py::dict ret;
	ret["rate"] = rate;
	ret["samples"] = IRPairs;   // index by [i_src, i_lis]
	return ret;
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
	auto rate = _context.getSampleRate();

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
