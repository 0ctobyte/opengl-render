# 3D Model Rendering Using OpenGL

I wrote this simple program to learn about OpenGL. It reads a Wavefront object file (specified by the .obj and .mtl file extensions) and displays the wireframe model on the screen.
Uses OpenGL 3.3 and SFML 2 (for cross-patform windowing).
Press `Q` and `A` to zoom-in and zoom-out and the arrow keys to pan

## Build
Requires OpenGL 3.3 and SFML 2.1. Build system uses CMake.

To build:

- `mkdir build && cd build`
- `cmake ..`
- `make`

This generates a binary, `Simple3DModelRenderer`, under `build/src`

To run:
- Copy `Simple3DModelRenderer` binary to the `resources` directory
- Run it: `./Simple3DModelRenderer`

