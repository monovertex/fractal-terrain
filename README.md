# Fractal Terrain Generation #

## Overview ##

This project is a college assignment which contains a fractal generated terrain with various levels of tessellation. It also has a functional Phong lighting system and infinite world.

For more information about the fractal algorithm used, view [this document][1].

For more project details and work in progress screenshots, view my blog posts: [#1][2], [#2][3], [#3][4].

A short video can be seen [here][5].

The project is written using OpenGL, GLEW, freeglut and a framework provided by the professors (everything starting with the "lab_" prefix).


_Note: this project is not in active development._


## Usage ##

* __W__ - move camera forward
* __A__ - strafe camera left
* __D__ - strafe camera right
* __S__ - move camera backwards
* __Mouse__ - move camera on Y / Z axes

---

* __I__ - move light system forward
* __J__ - move light system left
* __L__ - move light system right
* __K__ - move light system backwards
* __U__ - move light system down
* __O__ - move light system up

---

* __Q__ - add a new light
* __E__ - toggle lights between point and spotlight

---

* __Z__ - toggle fog on and off
* __X__ - re-load shaders
* __C__ - turn lights on and off
* __Space__ - turn wireframe drawing on and off

---

* __1__ - basic map (flat)
* __2__ - fractal map, no tessellation
* __3__ - fractal map, 1x tessellation
* __4__ - fractal map, 2x tessellation
* __5__ - fractal map, 4x tessellation

---

* __F1__ - fullscreen
* __F2__ - exit fullscreen


## Compiling ##

To compile the project you need to make sure to link the libraries freeglut.dll and glew32.dll. Also remember to include the folders.

This project was initially created using Visual Studio 2013, but I did not include the project files for obvious reasons.

You will also most probably require the two .dll libraries inside the folder where your executable resides.


[1]: http://www.gameprogrammer.com/fractal.html
[2]: http://blog.monovertex.com/post/66696528644/as-part-of-an-assignment-for-college-graphic
[3]: http://blog.monovertex.com/post/66809082842/part-ii-in-my-terrain-generation-assignment
[4]: http://blog.monovertex.com/post/67157571371/and-its-finally-finished-it-is-now
[5]: http://www.youtube.com/watch?v=IT8w5v0dPm0