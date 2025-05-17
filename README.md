# random-tiling-generator

A random tiling generator which uses a list of tiles that follow certain adjacency rules to procedurally generate a tiling. The possible states each tile can occupy are continually collapsed until all tiles are fixed.

Tile features can be adjusted through user input to produce different effects.

Download the [latest release](https://github.com/Varulli/random-tiling-generator/releases/latest) or [build it yourself](https://github.com/Varulli/random-tiling-generator#build-instructions).

![random-tiling-generator-demo-1](https://github.com/user-attachments/assets/a7567e89-9515-4492-b190-aee74fd8848c)
![random-tiling-generator-demo-2](https://github.com/user-attachments/assets/9f4446bd-da56-4c8e-a0a2-12b53865c0d2)

## Build Instructions

### Requirements
- C compiler
- CMake 3.5+

### Steps
```
git clone https://github.com/Varulli/random-tiling-generator.git
cd random-tiling-generator
mkdir build
cd build
cmake ..
cmake --build .
```
