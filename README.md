Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
All rights reserved.

GeoSound
=====

GeoSound is a physically-based sound propagation package used for 
acoustic simulations in various environments. This package is NOT 
ready for distribution.

Dependencies
--------

You may need several 3rd-party libraries. Known ones are:

	ogg
	vorbis
	fftw3
	pybind11


Building
--------

To build GeoSound with the CMake toolchain, please run

    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=release
    make -j

GeoSound is configured as a dynamically-linked shared library. After 
building, you'll find the compiled library in 
build/pysound/release/lib/pysound.so

Please add the pysound.so path to your PYTHONPATH enviroment variable 
so that the GeoSound library can be imported by your python code.

Examples
--------

See `examples` for example source code for simple tests:

  - `test.py` generates Room Impulse Responses (RIR) in a simple shoebox room. It demonstrates two different ways of specifying the 3D simulation environment: specifying shoebox room dimension vs. loading .obj files. The latter approach allows simulation in highly complicated acoustic environments including occlusion.

  - `custom_array.py` shows how to define your custom array layout and obtain multi-channel RIRs.

  - `compare_image.py` compares our simulation with the classical image source method by giving them the same input.

Extra libraries may be needed to run the above examples. Install them when prompted.
