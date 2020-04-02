Copyright (C) 2010-2020 Carl Schissler, University of North Carolina at Chapel Hill.
All rights reserved.

pygsound
=====

[**GSound**](http://gamma.cs.unc.edu/GSOUND/) is a physically-based sound propagation package used for acoustic simulations in various environments, developed by **Dr Carl Schissler**. **pygsound** is the Python package that wraps **GSound**'s codebase for efficiently computing room impulse responses (RIRs) with specular and diffuse reflections. **GSound** is powerful enough to be used for sound simulation in 3D scenes with complicated geometry and acoustic materials. This repo's python API has not exposed all of **GSound**'s components. But we do provide the complete C++ source code and welcome pull requests if you made useful modifications (mainly the python API).

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

*Note: if you use a virtual environment, please make sure the python version in your virtual env is consistent with your system-wide python version. Otherwise the `python3-dev` package may not be found by the CMake and is unusable.*

Installation
--------

First clone this repo with:
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
cd pygsound
pip3 install .
```

Usage
--------

See `examples` folder. You need to `cd examples` and run `python3 mesh_sim.py` (we recommend starting with this one). This script demonstrates two equivalent ways to define the environment for sound propagation, and save the impulse response as an audio file. You can use a `.obj` file with an optional `.mtl` file with the same name to define the room geometry and materials. In this case, the `.mtl` file has two extra rows compared with conventional `.mtl` file used for visual rendering:
```
sound_a 0.5 0.6 0.6 0.7 0.75 0.8 0.9 0.9	# sound absorption coefficients, for 8 octave bands [62.5, 125, 250, 500, 1000, 2000, 4000, 8000]Hz
sound_s 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5	# sound scattering coefficients, if you don't know the details of diffuse/specular reflections, keep it 0.5
```
or directly create a shoebox shaped room using our API:
```
room = ps.createbox(dim_x, dim_y, dim_z, absorption_coefficient, scattering_coefficient)
```
The benefit of using the `.obj` style is that you can easily define different reflection/absorption coefficients for each triangle element for each frequency sub-band.

Citations
--------

This sound propagation engine has been used for many research work of **Dr Carl Schissler** and other researchers in the [UMD GAMMA](https://gamma.umd.edu) group for audio rendering and impulse response generation purposes. For example:
```
@inproceedings{schissler2011gsound,
  title={Gsound: Interactive sound propagation for games},
  author={Schissler, Carl and Manocha, Dinesh},
  booktitle={Audio Engineering Society Conference: 41st International Conference: Audio for Games},
  year={2011},
  organization={Audio Engineering Society}
}

@article{schissler2017interactive,
  title={Interactive sound propagation and rendering for large multi-source scenes},
  author={Schissler, Carl and Manocha, Dinesh},
  journal={ACM Transactions on Graphics (TOG)},
  volume={36},
  number={1},
  pages={2},
  year={2017},
  publisher={ACM}
}

@article{tang2019improving,
  title={Improving reverberant speech training using diffuse acoustic simulation},
  author={Tang, Zhenyu and Chen, Lianwu and Wu, Bo and Yu, Dong and Manocha, Dinesh},
  journal={arXiv preprint arXiv:1907.03988},
  year={2019}
}
```
For a complete list of relevant work you may want to cite depending on how you use this repo, see our [speech related research](https://gamma.umd.edu/researchdirections/speech/main) and [sound related research](https://gamma.umd.edu/researchdirections/sound/main).

