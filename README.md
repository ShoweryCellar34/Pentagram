
# What is Pentagram dev branch?

Pentagram dev branch is where big changes will happen, it is not kept up to date with main, just don't use it beacuse most commits will not compile and are broken.

## Platform support

Ghoul Engine uses strictly cross-platform libraries \(Windows, Mac, and Linux support\) and [CMake](https://cmake.org/), and is tested on Arch linux with KDE plasma and Windows 11/10.

## How to clone

Run this command with [git](https://git-scm.com/) installed to clone this repository: `git clone --recursive https://github.com/ShoweryCellar34/Ghoul-Engine.git`

## How to build \(CMake required\)

Warning: the "res/" directory may not be copied over to the binary directory if using MSVC.
### [CMake](https://cmake.org/)
1. Move into the cloned directory `cd Ghoul-Engine`.
2. Make the build directory `mkdir build` and move into it `cd build`.
3. Generate the project file `cmake ..`.
4. Build with `cmake --build .`
5. Run the produced `Ghoul-Engine` file.


## How to clone

Run this command with [git](https://git-scm.com/) installed to clone this repository: `git clone --recursive https://github.com/ShoweryCellar34/Pentagram.git`

## Platform support

Ghoul Engine uses strictly cross-platform libraries \(Windows, Mac, and Linux support\) and [CMake](https://cmake.org/), and is tested on Arch linux with KDE plasma and Windows 11/10.

## Disclaimer

Wayland can be used if you don't ever query or set the window position, this includes using imgui viewports.

## How to use with cmake

Pentagram was designed only with cmake support in mind so precompiled binaries aren't provided.
Add these lines to your CmakeListst.txt file after declaring your project:
```
add_subdirectory(*path to your copy of Peantagram*)
target_link_libraries(*your desired cmake target to link Peantagram to* PUBLIC Pentagram)
```
