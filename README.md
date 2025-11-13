# Mandelbrot Renderer - C++17 + Raylib

This project is a real-time Mandelbrot Set renderer build with C++17 and raylib 5.5, exploring the impact of several optimization strategies on fractal rendering performance.

![](/media/MandelbrotSet.png?raw=true "Mandelbrot set")

## Overview
The renderer visualizes the Mandelbrot Set using multiple algorithm approaches: from a simple, naive escape-time algorithm to AVX/AVX2-optimized versions runnign across multiple threads and a thread pool.

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
- Fixed window size (1280x720) and grayscale coloring are used to keep rendering performance consistent and predictable.

## Optimization Techniques
The project explores and compares the following implementations
1. **Escape-Time Algorithm**: Straightforward pixel-by-pixel computation.
2. **AVX/AVX2 Intrinsics**: Vectorized computations for parallel pixel evaluation using SIMD instructions.
3. **AVX/AVX2 + Threads**: Workload distributed among multiple threads (36 in total)
4. **AVX/AVX2 + Thread Pool**: Tasks dispatched to a persisten thread pool to avoid repeated thread creation/destruction overhead.

## Observations
While each optimization step brought measurable speedups over the naive approach, the performance difference between threads and thread pool was negligible.

Possible explanations include:
- **Improved cache locality**: Each thread processes a contigous pixel region reducing cache misses.
- **Thread count overhead balance**: With 36 threads, the creation/destruction cost in method 3 may be effectively masked by the workload size.

## Dependencies
- C++17 or newer
- raylib 5.5 (graphics and input handling)
- a CPU supporting AVX/AVX2 instructions for SIMD acceleration

## Building
```bash
$ git clone https://github.com/marcope-98/fractal-rendering.git
$ cd fractal-rendering
$ mkdir build && cd build
$ cmake ..
$ make -j4
$ ./fractal_rendering
```