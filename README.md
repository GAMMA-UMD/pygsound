Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
All rights reserved.

pygsound
=====

[**GSound**](http://gamma.cs.unc.edu/GSOUND/) is a physically-based sound propagation package used for acoustic simulations in various environments, developed by Carl Schissler. **pygsound** is the Python package that wraps **GSound**'s codebase for efficiently computing room impulse responses (RIRs) with specular and diffuse reflections.

Dependencies
--------

You may need several 3rd-party libraries. Known ones are `vorbis` and `fftw3` and can be installed by running the following code on Linux:
```
sudo apt-get update
sudo apt-get -y install libfftw3-dev
sudo apt-get -y install libvorbis-dev
```

Building
--------

This repo has been configured to build with CMake (version>=12). First clone this repo with:
```
git clone --recurse-submodules https://github.com/RoyJames/pygsound.git
```
Then you can build and test with
```
python3 setup.py develop
python3 setup.py test
```
or directly install it as a python package with
```
pip install .
```

Examples
--------

Not yet ready.
