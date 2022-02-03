#include "Context.hpp"

//std::shared_ptr< Context >
//Context::instance = std::make_shared< Context >();

Context::Context()
{
    const gs::Float f[] = { 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f };
    prop_request.frequencies = gs::FrequencyBands(f);
    prop_request.flags.set(gs::PropagationFlags::DIRECT, true);
    prop_request.flags.set(gs::PropagationFlags::SPECULAR, true);
    prop_request.flags.set(gs::PropagationFlags::DIFFUSE, true);
    prop_request.flags.set(gs::PropagationFlags::DIFFRACTION, true);
    prop_request.flags.set(gs::PropagationFlags::SOURCE_DIRECTIVITY, false);
    prop_request.flags.set(gs::PropagationFlags::DOPPLER_SORTING, false);
    prop_request.flags.set(gs::PropagationFlags::ADAPTIVE_QUALITY, false);
    prop_request.flags.set(gs::PropagationFlags::AIR_ABSORPTION, true);
    prop_request.flags.set(gs::PropagationFlags::ADAPTIVE_IR_LENGTH, true);
    prop_request.flags.set(gs::PropagationFlags::VISIBILITY_CACHE, false);
    prop_request.flags.set(gs::PropagationFlags::IR_THRESHOLD, true);
    prop_request.flags.set(gs::PropagationFlags::IR_CACHE, true);
    prop_request.flags.set(gs::PropagationFlags::SAMPLED_IR, true);
    prop_request.targetDt = 1.0f / 15.0f;
    prop_request.sampleRate = 16000;
    prop_request.numSpecularRays = 20000;
    prop_request.maxSpecularDepth = 200;
    prop_request.numSpecularSamples = 100;
    prop_request.numDiffuseRays = 20000;
    prop_request.maxDiffuseDepth = 200;
    prop_request.numDiffuseSamples = 3;
    prop_request.responseTime = 5.0;
    prop_request.maxIRLength = 3.0;
    prop_request.numThreads = 8;

    ir_request.ir = true;
    ir_request.metrics = false;
    ir_request.normalize = true;
    ir_request.channelLayout = gs::ChannelLayout::MONO;
}

Context::~Context()
= default;

gs::Size Context::getSpecularCount()
{
    return prop_request.numSpecularRays;
}

void Context::setSpecularCount(gs::Size cnt)
{
    prop_request.numSpecularRays = cnt;
}

gs::Size Context::getSpecularDepth()
{
    return prop_request.maxSpecularDepth;
}

void Context::setSpecularDepth(gs::Size cnt)
{
    prop_request.maxSpecularDepth = cnt;
}

gs::Size Context::getDiffuseCount()
{
    return prop_request.numDiffuseRays;
}

void Context::setDiffuseCount(gs::Size cnt)
{
    prop_request.numDiffuseRays = cnt;
}

gs::Size Context::getDiffuseDepth()
{
    return prop_request.maxDiffuseDepth;
}

void Context::setDiffuseDepth(gs::Size cnt)
{
    prop_request.maxDiffuseDepth = cnt;
}

gs::Size Context::getThreadsCount()
{
    return prop_request.numThreads;
}

void Context::setThreadsCount(gs::Size cnt)
{
    prop_request.numThreads = cnt;
}

gs::SampleRate Context::getSampleRate()
{
    return prop_request.sampleRate;
}

void Context::setSampleRate(gs::SampleRate rate)
{
    prop_request.sampleRate = rate;
}

oms::ChannelLayout::Type Context::getChannelLayout()
{
    return ir_request.channelLayout.getType();
}

void Context::setChannelLayout(oms::ChannelLayout::Type type)
{
    ir_request.channelLayout.setType(type);
}

gs::Bool Context::getNormalize()
{
    return ir_request.normalize;
}

void Context::setNormalize(gs::Bool flag)
{
    ir_request.normalize = flag;
}