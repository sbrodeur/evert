
# EVERT

Accelerated beam tracing algorithm adapted from the original source code of Samuli Laine.

![alt tag](https://github.com/sbrodeur/evert/raw/master/doc/images/evert.jpg)

Main requirements:
- OpenGL, GLUT
- autoconf, automake, make

NOTE: The code currently works only in 32-bit mode.

## Dependencies on Ubuntu:

Install the dependencies:
```
sudo apt-get install build-essential
sudo apt-get install autoconf automake make
sudo apt-get install libgl-dev
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev libgl1-mesa-dev
```

You will also need the 32-bit libraries: 
```
sudo apt-get install libgl-dev:i386
sudo apt-get install mesa-common-dev:i386
sudo apt-get install libglu1-mesa-dev:i386 freeglut3-dev:i386 mesa-common-dev:i386 libgl1-mesa-dev:i386
```

## Installing the library

Download the source code from the git repository:
```
mkdir -p $HOME/work
cd $HOME/work
git clone https://github.com/sbrodeur/evert.git
```

Compile the source code (32-bit mode, debug enabled):
```
aclocal
autoconf
automake --add-missing
./configure CFLAGS='-m32 -g -O0' CXXFLAGS='-m32 -g -O0' LDFLAGS='-m32'
make
```

For maximum performance, use this instead (32-bit mode, optimization enabled):
```
aclocal
autoconf
automake --add-missing
./configure CFLAGS='-m32 -O2' CXXFLAGS='-m32 -O2' LDFLAGS='-m32'
make
```

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