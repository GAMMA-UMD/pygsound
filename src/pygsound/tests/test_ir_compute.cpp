#include <gsound/gsound.h>
#include <gsound/gsPropagationRequest.h>

#include <om/omResources.h>
#include <om/omSound.h>
#include <om/omIO.h>

#include "SoundMesh.hpp"
#include "Context.hpp"
#include "Scene.hpp"
#include "Listener.hpp"
#include "SoundSource.hpp"

#include <memory>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace gs = gsound;
namespace omr = om::resources;
namespace omm = om::math;
namespace omt = om::time;
namespace oms = om::sound;


float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

std::vector<float> computeIR(const std::shared_ptr< SoundMesh > mesh, const omm::Vector3f lis_loc, const omm::Vector3f src_loc) {
    int nthreads = 8;
    int primaryRayCount = 2000;
    int sampleRate = 16000;

    auto ctx = Context();
    ctx.setDiffuseCount(primaryRayCount);
    ctx.setSampleRate(sampleRate);
    ctx.setSpecularCount(2000);
    ctx.setThreadsCount(nthreads);
    ctx.setChannelLayout(oms::ChannelLayout::MONO);
    auto scene = Scene();

    // Set mesh
    scene.setMesh(*mesh);

    gs::SoundListener listener;
    listener.setRadius(0.01f);
    listener.setPosition(lis_loc);

    gs::SoundSource source;
    source.setRadius(0.01f);
    source.setPosition(src_loc);

    scene.m_scene.addSource(&source);
    scene.m_scene.addListener(&listener);

    int elapsed_time;
    om::data::UTF8String outfilename("/tmp/IR.wav");

    gs::ImpulseResponse niceIR;
    gs::SoundPropagator propagator;
    gs::SoundSceneIR sceneIR;


    propagator.propagateSound(scene.m_scene, ctx.internalPropReq(), sceneIR);

    const gs::SoundSourceIR& sourceIR = sceneIR.getListenerIR(0).getSourceIR(0);
    niceIR.setIR(sourceIR, listener, ctx.internalIRReq());

    auto rate = niceIR.getSampleRate();
    auto *sample = niceIR.getChannel( 0 );
    std::vector<float> samples;
    for ( int iter_sample = 0; iter_sample < niceIR.getLengthInSamples(); ++iter_sample )
    {
        samples.push_back( sample[iter_sample] );
    }

    return samples;
}

int main( int _argc, const char **_argv )
{
    int seed = 0;
    int N = 2;
    float low = 0.5;
    float high = 0.99;
    std::vector<omm::Vector3f> roomsizes, lislocs, srclocs;
    std::vector<float> alphas;
    const float margin = 0.1;
    srand(seed);

    for (int i = 0; i < N; ++i)
    {
        omm::Vector3f roomsize(RandomFloat(0.5, 10.0), RandomFloat(0.5, 10.0), RandomFloat(0.5, 10.0));
        omm::Vector3f lis(RandomFloat(margin, roomsize[0]-margin), RandomFloat(margin, roomsize[1]-margin), RandomFloat(margin, roomsize[2]-margin));
        omm::Vector3f src(RandomFloat(margin, roomsize[0]-margin), RandomFloat(margin, roomsize[1]-margin), RandomFloat(margin, roomsize[2]-margin));
        roomsizes.push_back(roomsize);
        lislocs.push_back(lis);
        srclocs.push_back(src);
        float alpha = 1 - RandomFloat(low, high);
        alphas.push_back(alpha);
    }
    for (int i = 0; i < N; ++i)
    {
        auto mesh = SoundMesh::createBox(roomsizes[i][0], roomsizes[i][1], roomsizes[i][2], alphas[i]);
        auto lis_loc = lislocs[i];
        auto src_loc = srclocs[i];

        auto samples = computeIR(mesh, lis_loc, src_loc);

        //std::ofstream output_file(std::string("/tmp/IR")+std::to_string(i)+std::string(".txt"));
        //std::ostream_iterator<float> output_iterator(output_file, "\n");
        //std::copy(samples.begin(), samples.end(), output_iterator);

        for (auto s:samples)
        {
            if (s!=s)
            {
                std::cout << "detected nan: " << s << std::endl;
                throw std::runtime_error("detected nan!");
            }
        }
        std::cout << i+1 << "/" << N << " tests finished" << std::endl;
    }

    return 0;
}
