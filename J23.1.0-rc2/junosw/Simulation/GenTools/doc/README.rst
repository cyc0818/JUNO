========
GenTools
========

This ideal of this package is from the ``GenTools`` in **NuWa**.
I migrate this from **Gaudi** to **Sniper**. The work is ongoing.

You can found the original code in the svn of dayabay:
http://dayabay.ihep.ac.cn/svn/dybsvn/dybgaudi/trunk/Simulation/GenTools

Usage
=====

This tool is still in developing, because the memory management is
still missing, we can only transfer the data by other methods.

Development
===========

GtGunGenTool
------------

This tool will generate particles in a simple algorithmic way.  
It only generates a particle with a momentum or total energy 
or kinetic energy.

It will place it in the event's signal process vertex 
(or create a signal process vertex if one is missing).
The vertices time and position coordinates will not be touched.
See ``GtPositionerTool`` and ``GtTimeratorTool`` for how
one might set these to non-trivial values.

The direction of the gun is generated in the coordinate system of
the physical volume of the given detector element.
The detector element is given by the "Volume" property.
After generation the direction is transformed into global coordinates.
If no detector element is given, then the direction is generated 
directly in global coordinates.

Now, the geometry service is still missing, so we can skip 
transformation first. Just use the global coordinate.

GtHepEvtGenTool
---------------

This tool will import the data from:

* the HepEvt file.
* the output of generator executable 

The original version will cache all the data.
It's a waste of memory if the data is huge.
So, the current version only generate one event every time.

GtPositionerTool
----------------

Place the particles in specific volume.
Now, we can just use the global coordinate.

I think, now we can use the geant4 geometry kernel to manage the 
affine transformation.

I need to combine the work in DetSim.

GtHepMCDumper
-------------

Dump the ``GenEvent`` information.

GenEventBuffer
--------------

We use a singleton pattern to implement the buffer for ``GenEvent``.
It is a good ideal to use smart pointer to manage the data.

Notes
=====

If you add or modify the source code, please use ``make clean`` and 
``cmt config && cmt make`` to recompile the library.
There are caches so sometimes it's not good.


Design
======

``GenTools`` is an algorithm which can load several ``IGenTool``
implementation. ``IGenTool`` derived class can implement the interface
so that it can access or modify the ``HepMC::GenEvent`` object.
There is a chain of ``IGenTool`` objects so that we can build the ``GenEvent``
as we want.

In this design, we can reuse the tools, and combine them together.
