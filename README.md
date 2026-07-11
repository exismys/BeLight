## BeLight
Experiments on graphic programming.

The actual goal is to make a minimal 3D software renderer for learning purposes.

## Dependencies

This project requires SDL2 for windowing and framebuffer. Install following packages:

SDL2:

```sudo apt install libsdl2-dev```

## Ray Traced Scenes

### Trace Ray recursion_depth = 1
![Ray Tracing Result](renders/rendered_ray_traced_scene.png)

### Trace Ray recursion_depth = 3
![Ray Tracing Result](renders/rendered_ray_traced_scene_rd3.png)

## Rasterized Scenes

### Flat Shaded Cube
![Rasterization (flat shaded cube) resutl](renders/rendered_rasterized_cube_flat_shaded.png)

### Flat Shaded Sphere

![Rasterization (flat shaded sphere) resutl](renders/rendered_rasterized_sphere_flat_shaded.png)


## Build
```
cmake -B build && \
cmake --build build && \
./build/bin/BeLight
```
or

`./build.sh
`
