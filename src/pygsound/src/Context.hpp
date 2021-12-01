#ifndef INC_CONTEXT_HPP
#define INC_CONTEXT_HPP

#include <gsound/gsPropagationRequest.h>
#include <gsound/gsIRRequest.h>
#include <memory>

namespace gs = gsound;
namespace oms = om::sound;

class Context
{
public:

	//static std::shared_ptr< Context > instance;

	Context();
	~Context();

	gs::IRRequest &internalIRReq() { return ir_request; }
	gs::PropagationRequest &internalPropReq() { return prop_request; }

    gs::Size getSpecularCount();
	void setSpecularCount(gs::Size cnt);
    gs::Size getSpecularDepth();
    void setSpecularDepth(gs::Size cnt);

    gs::Size getDiffuseCount();
    void setDiffuseCount(gs::Size cnt);
    gs::Size getDiffuseDepth();
    void setDiffuseDepth(gs::Size cnt);

    gs::Size getThreadsCount();
    void setThreadsCount(gs::Size cnt);

    gs::SampleRate getSampleRate();
    void setSampleRate(gs::SampleRate rate);

    oms::ChannelLayout::Type getChannelLayout();
    void setChannelLayout(oms::ChannelLayout::Type type);

    gs::Bool getNormalize();
    void setNormalize(gs::Bool flag);

private:

	gs::IRRequest ir_request;
	gs::PropagationRequest  prop_request;
};

#endif  // INC_CONTEXT_HPP

