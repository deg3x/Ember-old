# GraphicsPlayground

## Overview
This is a simple graphics engine that I created in order to dive into Engine/Graphics programming. \
At the moment it serves as a great base for learning and experimenting with shaders and simple engine concepts.

The goal is to extend this into a feature-complete graphics engine, including an editor and the basic tools needed to create a game.

## Platforms
* Windows
* OS X

Only 64-bit targets are supported.

## Dependencies
The following libraries are used (and are already included):
* **GLFW** is used for managing the running windows.
* **OpenGL** (4.1) is the main graphics API.
* **GLM** is used as the base mathematics library (will be replaced in the future).
* The project uses the **Premake** (v5.0.0-beta2) build system.

Precompiled Premake binaries for Windows and OS X are also included.

A **Python** script is used to interact with **Premake** and therefore must be present in your system. \
If you want to avoid installing it, you can manually run premake and build the project (see  `generate_project.py`).

## Build
1. Download/Clone the code/repository.
2. Open a terminal and change the working directory to the project root.
3. Execute the project generation python script: `python ./generate_project.py` 

## Incoming Features
- [x] Textures (diffuse, normals, etc.) [not fully implemented]
- [ ] Shadows
- [x] Cubemaps
- [x] Model Loading
- [ ] Deferred Rendering
- [ ] PBR
- [ ] Text Rendering
- [ ] Vulkan support
- [ ] Raytracing
- [ ] Editor (ImGui)

## License
The MIT License (MIT) 
