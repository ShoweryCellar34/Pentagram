
# What is Pentagram?

Pentagram is a basic application framework whuch uses GLFW, ImGui, GLAD, stb, glm, and spdlog for convenient, easy window creation and frameloop. I have created an [example app](https://github.com/ShoweryCellar34/Pentagram-Demo.git) for demonstration.

## How to clone

Run this command with [git](https://git-scm.com/) installed to clone this repository: `git clone --recursive https://github.com/ShoweryCellar34/Pentagram.git`

## Platform support

Penagram uses strictly cross-platform libraries \(Windows, Mac, and Linux support\) and [CMake](https://cmake.org/), tested on Arch linux X11 (with GNOME, idk if that matters), and has been tested on Windows 11/10.

## Disclaimer

Wayland is not supported as the restrictions on windows position query and modification with imgui \(internaly imgui calls a fuction that segfaults on wayland, may fix in the future. DO NOT EXPECT THIS\). Using linux works, but it is buggier that windows.

## How to use with cmake

Pentagram was designed only with cmake support in mind so precompiled binaries aren't provided.
Add these lines to your CmakeListst.txt file after declaring your project:
```
add_subdirectory(*path to your copy of Peantagram*)
target_link_libraries(*your desired cmake target to link Peantagram to* PUBLIC Pentagram)
```
