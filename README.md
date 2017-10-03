
# EVERT

Accelerated beam tracing algorithm adapted from the original source code by Samuli Laine.

![alt tag](https://github.com/sbrodeur/evert/raw/master/doc/images/evert.jpg)

Main requirements:
- autoconf, automake, make, swig
- OpenGL, GLUT

## Dependencies on Ubuntu:

Install the basic dependencies:
```
sudo apt-get install build-essential
sudo apt-get install autoconf automake libtool swig make
```

Install the OpenGL and GLUT dependencies:
```
sudo apt-get install libgl-dev
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev libgl1-mesa-dev
```

## Installing the library

Download the source code from the git repository:
```
mkdir -p $HOME/work
cd $HOME/work
git clone https://github.com/sbrodeur/evert.git
```

Compile the source code (debug enabled):
```
cd $HOME/work/evert
aclocal
autoconf
automake --add-missing
./configure CFLAGS='-g -O0' CXXFLAGS='-g -O0'
make
```

For maximum performance, use this instead (optimization enabled):
```
cd $HOME/work/evert
aclocal
autoconf
automake --add-missing
./configure CFLAGS='-O2' CXXFLAGS='-O2'
make
```

## Compiling the Python bindings

To install the Python bindings system-wide:
```
cd $HOME/work/evert/python
swig -c++ -python -I../include evert.i
sudo python setup.py install
```

To compile and install it locally (development mode):
```
cd $HOME/work/evert/python
swig -c++ -python -I../include evert.i
python setup.py build_ext --inplace
python -c 'import evert'
```

Note that in development mode the library must be in the PYTHONPATH environment variable for Python to be able to find it:
```
export PYTHONPATH=$HOME/work/evert/python:$PYTHONPATH
```
This can also be added at the end of the configuration file $HOME/.bashrc

## Citation

Please cite EVERT in publications when used:
> S. Laine, S. Siltanen, T. Lokki, and L. Savioja, “Accelerated beam tracing algorithm,” Applied Acoustics, vol. 70, no. 1, pp. 172–181, Jan. 2009 [Online]. Available: http://dx.doi.org/10.1016/j.apacoust.2007.11.011

BibTeX entry for LaTeX:
```
@article{Laine2009,
title = "Accelerated beam tracing algorithm",
journal = "Applied Acoustics",
volume = "70",
number = "1",
pages = "172 - 181",
year = "2009",
issn = "0003-682X",
doi = "https://doi.org/10.1016/j.apacoust.2007.11.011",
url = "http://www.sciencedirect.com/science/article/pii/S0003682X07001910",
author = "Samuli Laine and Samuel Siltanen and Tapio Lokki and Lauri Savioja",
keywords = "Beam tracing",
abstract = "Abstract Determining early specular reflection paths is essential for room acoustics modeling. Beam tracing algorithms have been used to calculate these paths efficiently, thus allowing modeling of acoustics in real-time with a moving listener in simple, or complex but densely occluded, environments with a stationary sound source. In this paper, it is shown that beam tracing algorithms can still be optimized by utilizing the spatial coherence in path validation with a moving listener. Since the precalculations required for the presented technique are relatively fast, the acoustic reflection paths can be calculated even for a moving source in simple cases. Simulations were performed to show how the accelerated algorithm compares with the basic algorithm with varying scene complexity and occlusion. Up to two-orders of magnitude speed-up was achieved."
}
```