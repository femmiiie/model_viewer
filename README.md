# Model Viewer

## Table of Contents
1. [Overview](#overview)
2. [Usage](#usage)
3. [Dependencies](#dependencies)
4. [License](#license)

# Overview

This is a basic 3D model viewer I created for fun!

# Usage

Build using CMake (>= 4.0). All dependencies except OpenGL are provided within `/external`.

# Config

All environment settings for model_viewer are stored in `mv_config.dat`. If no config file is present at runtime, a new one will be created with default values. Editable values are as follows:

- ShowGrid (Bool): Enables displaying a flat grid on the XZ plane with controllable size. Defaults to false. 
- ShowAxes (Bool): Enables displaying axes lines coming from the origin. Defaults to false.
- ShowLightPoints (Bool): Enables showing all lights in a scene as points with size proportional to their power. Defaults to false.
- Projection (Enum): Selects which type of projection should be used for the camera. 0 corresponds to perspective, 1 corresponds to orthographic. Defaults to 0.

# Dependencies

- OpenGL
- GLFW
- GLEW
- GLM
- ImGui
- NativeFileDialog-Extended

# License

This project uses the MIT License. See `LICENSE.md` for the full usability of this codebase.