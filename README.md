LDMM -- Li Ding Molecular Modeling Code (Version 0.4)
=====================================================

C++ Classical Molecular Modeling Code for Pedagogical Purposes

Copyright (C) 2011-2012, Li Ding `dingmaotu at 126.com`
Licenced under GPL v3

Introduction
------------

This is a molecular modeling code I wrote when I entered the Institute of
Geology and Geophysics, CAS. I was a newbie of molecular modeling though I had
taken several related classes in the CAS graduate school. My supervisor
suggested that I start a simple modeling code, with the purpose of learning
inner workings of a real package. It is extremely helpful to really write the
code, which involves searching the internet and reading a lot of related
books. I found it illuminating to know the inner working of molecular
dynamics, so I want to share the experience with others who also want to
really understand how a modeling is performed.

Acctually, this code currently only do classical molecular dynamics with only
one type of atom, and it has only one force field: Lenard Jones 12-6
potential for Ar atoms. This system is the most studied one, however, and you
can learn a lot from it.

This is the very first working version, and the code is a bit nasty. I put it
on github to make a conscience effort to improve it. I want to improve it like
this:

1. Make a clear OO design to show the inner workings of a simulation program
2. Make it easy to add new functionalities and to play with
3. Perform most common types of molecular dynamics and Monte Carlo simulations
4. If possible, show how MPI is used to leverage clusters
5. Last but not least, a collection of easy to understand documentation

Performance is NOT the purpose of LDMM. Its purpose is education.

Installation
------------

Currently There are two very primitive compiling options: Makefile or
Scons. At first I use the Make system but it is quite inflexible: the code is
too cryptic to maitain and extend. Scons is better, but still I think it is
not so good. I will change to *waf* if possible for future versions.

To compile, you need a working intel c++ compiler (free for academic use under
Linux, thought I will change to gcc for future versions). Then type `make
ldmm` or `scons release` to compile.

Usage
-----

To use LDMM, type `ldmm [your config file]`. There is a sample config file called
`ar.config` and associated `ar.gro` in the root directory.

Documentation
-------------

This is a very important part for LDMM but now it is not written. However,
basic documentation will be added soon.

Changes
-------

+ Version 0.4
    1. Add OpenMP support to parallelize the computation. This slightly speeds up the program.
+ Version 0.3
    1. Add Batch Mode, which can do a series of NVT simulations by varying the temperature. 
    2. Automatically average of T and P (after BatchCollect steps)
+ Version 0.2
    1. Change the brute-force LJ12-6 computation to a neighber-list based algorithm, and added cut-off controls.
    2. Implement a Berendson heat bath to perform NVT simulations.
    3. Output common instant properties of the system: T, P, Potential, Kinetic, and Total Energy.
    4. Accessing of configuration files from the exe directory.
+ Version 0.1
    1. NVE LJ12-6 Atomic Fluid Simulation (No cut-off)
