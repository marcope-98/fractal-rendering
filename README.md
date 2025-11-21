# Mandelbrot Renderer - C++17 + Raylib

This project is a real-time Mandelbrot Set renderer built with C++17 and raylib 5.5, exploring the impact of several optimization strategies on fractal rendering performance.

![](/media/MandelbrotSet.png?raw=true "Mandelbrot set")

## Overview
The renderer visualizes the Mandelbrot Set using multiple algorithmic approaches: from a simple, naive escape-time algorithm to AVX/AVX2-optimized versions running across multiple threads and a thread pool.

By leveraging raylib, the user can interactively explore the fractal space with smooth zooming, panning, and algorithm switching; all rendered in real time.

## Features
- Interactive controls (via raylib)
  - **Mouse wheel**: Zoom in/out
  - **Hold Left Mouse Button + Move**: Pan view
  - **Right Mouse Button**: Reset to original view
  - **+ / -**: Increase or decrease maximum number of iterations
  - **1**: Select escape-time algorithm
  - **2**: Select AVX/AVX2 implementation
  - **3**: Select AVX/AVX2 + Threads implementation
  - **4**: Select AVX/AVX2 + ThreadPool implementation
- Fixed window size (1280x720) is used to keep rendering performance consistent and predictable and to apply some precomputation.

## Optimization Techniques
The project explores and compares the following implementations
1. **Escape-Time Algorithm**: Straightforward pixel-by-pixel computation.
2. **AVX/AVX2 Intrinsics**: Vectorized computations for parallel pixel evaluation using SIMD instructions.
3. **AVX/AVX2 + Threads**: Workload distributed among multiple threads (36 in total)
4. **AVX/AVX2 + Thread Pool**: Tasks dispatched to a persistent thread pool to avoid repeated thread creation/destruction overhead.

## Observations
While each optimization step brought measurable speedups over the naive approach, the performance difference between the threaded and thread pool implementations was negligible.

Possible explanations include:
- **Improved cache locality**: Each thread processes a contiguous pixel region reducing cache misses.
- **Thread count overhead balance**: With 36 threads, the creation/destruction cost in method 3 may be effectively masked by the workload size.

In fact, later testing revealed that pushing the number of threads to the number of rows in the screen buffer (720) resulted in a substantial improvement of the threadpool implementation performance over the other methods.

## Shaders
Originally, this project computed the colorouing of the Mandelbrot set in the implementation code. As time passed I have moved this responsibility to the GPU by providing shaders that can be found... wait for it... in the `shaders` folder.

So far three shaders are provided:
- `grayscale.fs` containing the original palette used during development
- `eriksonn.fs` containing the palette used by OneLoneCoder in its [youtube video](https://youtu.be/PBvLs88hvJ8?si=yB6VJUSON7TMARbi)
- `wikipedia.fs` containing the [reversed engineered](https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia) palette used in the [Wikipedia article](https://en.wikipedia.org/wiki/Mandelbrot_set) about the Manderlbrot set.

## Dependencies
- C++17 or newer
- raylib 5.5 (graphics and input handling)
- A CPU supporting AVX/AVX2 instructions for SIMD acceleration

## Building
```bash
$ git clone https://github.com/marcope-98/fractal-rendering.git
$ cd fractal-rendering
$ mkdir build && cd build
$ cmake ..
$ make -j4
$ ./fractal_rendering <path-to-shader.fs>
```