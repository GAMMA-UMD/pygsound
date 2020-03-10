Copyright (C) 2010-16 Carl Schissler, University of North Carolina at Chapel Hill.
All rights reserved.

pygsound
=====

[**GSound**](http://gamma.cs.unc.edu/GSOUND/) is a physically-based sound propagation package used for acoustic simulations in various environments, developed by **Dr Carl Schissler**. **pygsound** is the Python package that wraps **GSound**'s codebase for efficiently computing room impulse responses (RIRs) with specular and diffuse reflections. **GSound** is powerful enough to be used for sound simulation in 3D scenes with complicated geometry and acoustic materials. This repo has not exposed all of **GSound**'s components. But we do provide the complete C++ source code and welcome pull requests if you find useful modifications.

Dependencies
--------

This repo has been configured to build with CMake (version>=12), and mainly tested on Linux and MacOS. 

On Linux, install dependencies using:
```
sudo apt-get update
sudo apt-get -y install cmake python3-dev gobjc++ libfftw3-dev
```

On MacOS, install dependencies using:
```
brew update
brew install cmake python3 fftw
```

Building
--------

Ideally, when you have all the dependencies, you may install the latest release using
```
pip3 install pygsound
```

If you want to build from the latest source code, first clone this repo with:
```
git clone --recurse-submodules https://github.com/RoyJames/pygsound.git
``` 
Then you can build and test with
```
cd pygsound
python3 setup.py develop
python3 setup.py test
```
or directly install it as a python package with
```
pip3 install .
```

Usage
--------

To be updated soon.

Examples
--------

See `examples` folder. You need to `cd examples` and run `python3 [example_script].py`. You are responsible for installing missing packages.

Citations
--------

To be added soon.
