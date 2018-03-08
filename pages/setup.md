---
layout: default
title: Setup
permalink: setup.html
---

# Setting Up the ForSyDe-SystemC Modeling Environment

This page provides information on how to set up the !ForSyDe-SystemC modeling library and simulate a model using it.


## Prerequisites

In order to model systems using ForSyDe-SystemC one would need
- a C++ compiler with proper support for newly added C++11 features,
- an installation of the SystemC system modeling library,
- an installation of the Boost uBLAS library,
- a copy of the (header only) ForSyDe-SystemC library on your system,
- a text editor or an IDE of your choice for developing C++ programs.

### The C++ Compiler 

Depending on the platform of your choice, you might have different choices for the compiler.
!ForSyDe-SystemC is mainly developed in Linux using gcc (but also tested with clang). Note that if you decide to use `gcc`, you will need a version newer or equal to `gcc4.7` and use the `-std#c++11` compiler flag.

### The SystemC Modeling Library 

The reference implementation of the SystemC library can be obtained from the [accellera systems initiative web page](http://www.accellera.org/).
The installation instructions are included in the package.

### The Boost Library

Boost is a collection of peer-reviewed portable C++ libraries which can be obtained from the [Boost C++ libraries home page](http://www.boost.org/).
At the moment, ForSyDe-SystemC mainly uses the uBLAS library of Boost for matrix operations.

### The ForSyDe-SystemC Library

ForSyDe-SystemC is a C++ header-only library which means it does not need to be compiled during installation and linked againsted during model development.
The reason for that is the heavy use of template classes and functions which provide type-safe polymorphism.
After obtaining the library, either via the download section or the development code repository, it needs to be copied to a desired place on your system and referred to during compilation.
Although there will be regular releases of the library available in the downloads section, we currently recommend using the HEAD version obtained directly from the svn code repository.
this is because ForSyDe-SystemC is actively developed and debugged and we try to keep the online tutorials synced with the latest version of the library.

### A Text Editor/IDE

Any C++ code editor can used developing ForSyDe-SystemC models.
However, we recommend choosing one which also supports recent C++11 additions to the language.

## Example Setups

### Ubuntu Linux

In case you have a recent version of [http://www.ubuntu.com/ Ubuntu] installed (13.10 at the moment of writing this tutorial),
- install the required tools needed for development using `gcc` by issuing the following command in a terminal

      sudo apt-get install build-essential

- once you have downloaded the SystemC library, you can extract, build, and install it by

      tar xvzf systemc-<version>.tgz
      cd systemc-<version>
      mkdir objdir
      cd objdir
      ../configure
      make
      make install

Note that you can use the `--prefix#/your/install/dir/` switch in the configuration stage to install the libraries to a different location than the default system libraries path. In case you do this, do not forget to add the library paths during compilation and set the `LD_LIBRARY_PATH` environment variable during the execution of your models.
- you can install the Boost libraries by

      sudo apt-get install libboost-all-dev

- if you have not installed Subversion, you can install it first

      sudo apt-get install git

  and then use it to check out the latest version of the ForSyDe-SystemC library from the repositories to the current folder by

      git clone {{ site.github.repository_url }}

- either use the editors which are provided by Ubuntu by default such as `nano`, `vm`, `emacs`, `gedit`, or install an editor of your choice like `geany`, `eclipse`, etc.

You should now be able to build your system models against the SystemC and ForSyDe-SystemC libraries and execute them.
