
#include "Python.hpp"

#include <memory>
#include <string>
#include <sstream>
#include <fftw3.h>

#include <om/omSound.h>
#include <om/omMath.h>

#include "Context.hpp"
#include "SoundMesh.hpp"
#include "Scene.hpp"
#include "SoundSource.hpp"
#include "Listener.hpp"
#include "MicrophoneArrays.hpp"


namespace py = pybind11;
namespace oms = om::sound;
namespace omm = om::math;


PYBIND11_MODULE(pygsound, ps)
{
	fftw_init_threads();
	fftw_plan_with_nthreads( om::CPU::getCount() );

	py::class_< Context, std::shared_ptr< Context > >( ps, "Context")
            .def(py::init<>())
	        .def_property( "specular_count", &Context::getSpecularCount, &Context::setSpecularCount )
            .def_property( "specular_depth", &Context::getSpecularDepth, &Context::setSpecularDepth )
            .def_property( "diffuse_count", &Context::getDiffuseCount, &Context::setDiffuseCount )
            .def_property( "diffuse_depth", &Context::getDiffuseDepth, &Context::setDiffuseDepth )
            .def_property( "threads_count", &Context::getThreadsCount, &Context::setThreadsCount )
            .def_property( "sample_rate", &Context::getSampleRate, &Context::setSampleRate )
            .def_property( "channel_type", &Context::getChannelLayout, &Context::setChannelLayout )
            .def_property( "normalize", &Context::getNormalize, &Context::setNormalize );

	py::class_< SoundMesh, std::shared_ptr< SoundMesh > >( ps, "SoundMesh" )
            .def(py::init<>());

	ps.def( "loadobj", &SoundMesh::loadObj, "A function to load mesh and materials",
            py::arg("_path"), py::arg("_forceabsorp") = -1.0, py::arg("_forcescatter") = -1.0 );
    ps.def( "createbox", py::overload_cast<float, float, float, float, float>(&SoundMesh::createBox),
            "A function to create a simple shoebox mesh", py::arg("_width"), py::arg("_length"), py::arg("_height"),
            py::arg("_absorp") = 0.5, py::arg("_scatter") = 0.1 );
    ps.def( "createbox", py::overload_cast<float, float, float, std::vector<float>, float>(&SoundMesh::createBox),
            "A function to create a simple shoebox mesh", py::arg("_width"), py::arg("_length"), py::arg("_height"),
            py::arg("_absorp"), py::arg("_scatter") = 0.1 );

    py::class_< Scene, std::shared_ptr< Scene > >( ps, "Scene" )
            .def(py::init<>())
			.def( "setMesh", &Scene::setMesh )
			.def( "computeIR", py::overload_cast<std::vector<SoundSource>&, std::vector<Listener>&, Context&>(&Scene::computeIR),
                  "A function to calculate IRs based on pre-defined sources and listeners", py::arg("_sources"), py::arg("_listeners"), py::arg("_context"))
			.def( "computeIR", py::overload_cast<std::vector<std::vector<float>>&, std::vector<std::vector<float>>&, Context&, float, float, float>(&Scene::computeIR),
                  "A function to calculate IRs based on source and listener locations", py::arg("_sources"), py::arg("_listeners"), py::arg("_context"), py::arg("src_radius") = 0.01, py::arg("src_power") = 1.0, py::arg("lis_radius") = 0.01 );

	py::class_< SoundSource, std::shared_ptr< SoundSource > >( ps, "Source" )
            .def( py::init<std::vector<float>>() )
			.def_property( "pos", &SoundSource::getPosition, &SoundSource::setPosition )
			.def_property( "radius", &SoundSource::getRadius, &SoundSource::setRadius  )
			.def_property( "power", &SoundSource::getPower, &SoundSource::setPower  );

	py::class_< Listener, std::shared_ptr< Listener > >( ps, "Listener" )
            .def( py::init<std::vector<float>>() )
			.def_property( "pos", &Listener::getPosition, &Listener::setPosition )
			.def_property( "radius", &Listener::getRadius, &Listener::setRadius  );


	py::enum_< oms::ChannelLayout::Type >( ps, "ChannelLayoutType" )
			.value( "mono", oms::ChannelLayout::MONO )
			.value( "stereo", oms::ChannelLayout::STEREO )
			.value( "binaural", oms::ChannelLayout::BINAURAL )
			.value( "quad", oms::ChannelLayout::QUAD )
			.value( "surround_4", oms::ChannelLayout::SURROUND_4 )
			.value( "surround_5_1", oms::ChannelLayout::SURROUND_5_1 )
			.value( "surround_7_1", oms::ChannelLayout::SURROUND_7_1 )
			.value( "custom", oms::ChannelLayout::CUSTOM )
			.value( "undefined", oms::ChannelLayout::UNDEFINED )
            .export_values();


}
