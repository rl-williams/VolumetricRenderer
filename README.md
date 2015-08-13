# Volumetric Renderer
- Created from scratch using straight C++ in Visual Studio (no GUI, no Qt, no OpenGL). One month, completed October 2014.
- Attributes (camera position and direction, field of view, any geometry and its attributes, light attributes, etc.) read in via a custom configuration file.
- Ray marching and voxel volume intersection testing for volumetric rendering.
- Three rasterization primitives: spheres, Perlin-based noise, and pyroclastics (computed using Perlin noise and fractal Brownian motion).
