BadBehavior
==========
Behavior Tree System for Torque 3D
----------------------------------
### Introduction

The BadBehavior system is a behavior tree implementation and editor for the Torque3D game engine.

### Features

* Implemented in C++ for efficiency.
* Supports the common behavior tree node types including sequences, selectors, parallels and a range of decorators.
* Shared behavior trees - a single behavior tree definition can be used by multiple objects.
* Behavior trees can be used by any type of SimObject
* Trees can be assembled from smaller sub-trees in a modular fashion
* A multi-page graphical editor with undo/redo support
* Flexible behavior leaf nodes:
  * Actions/conditions can be created entirely in the editor for rapid prototyping
  * Structured behaviors can be written in script
  * Performance critical behaviors can be written entirely in C++
* Self contained - no changes to existing engine classes

### Further information

For tutorials and guides on how to use BadBehavior in your project, refer to:
* BadBehavior [GitHub Wiki](https://github.com/BadBehavior/BadBehavior_T3D/wiki)
* BadBehavior [GitHub page](http://badbehavior.github.io/BadBehavior_T3D)

For information specific to the Torque 3D game engine:
* Torque 3D [main repository](https://github.com/GarageGames/Torque3D)
* Torque 3D [GitHub Wiki](https://github.com/GarageGames/Torque3D/wiki)

License
-------

The BadBehavior system is Copyright (c) 2014 Guy Allard

Torque 3D is Copyright (c) 2012 GarageGames, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
