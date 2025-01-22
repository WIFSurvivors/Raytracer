# WIFSurvivors Raytracer

Welcome to the **WIFSurvivors Raytracer**, a self-designed raytracing engine built for experimentation and learning. This readme provides a brief overview of the project, installation instructions, and usage details. For detailed documentation, refer to our [Wiki](https://github.com/WIFSurvivors/Raytracer/wiki).

---
![GitHub Actions Status](https://img.shields.io/github/actions/workflow/status/WIFSurvivors/Raytracer/CI_WF.yml?branch=main)
![Latest Release](https://img.shields.io/github/v/release/WIFSurvivors/Raytracer?display_name=tag)
---


## Overview

The WIFSurvivors Raytracer is based on the **Entity-Component-System (ECS)** paradigm, ensuring logical independence and modularity. Each entity in the system has options (e.g., translation, rotation, scale) and components (e.g., light, render, camera) that define its behavior.

For more details, visit:
- [References and Drawings](https://github.com/WIFSurvivors/Raytracer/wiki/Documents-and-References)
- [UML Diagrams and Architecture](https://github.com/WIFSurvivors/Raytracer/wiki/UML-Diagrams)

---

## Installation

### Windows Installation (Recommended)
1. Navigate to the `Engine` folder in the repository.
2. Run **Engine-Version-win64.exe** as administrator and follow on-screen instructions.
3. Find the Raytracer under `{InstallDirectory}/Raytracer.lnk`.

### Building from Source
| Software                | Version         |
|-------------------------|-----------------|
| GnuWin32               | 3.81            |
| MSYS2                  | 20241208 or later |
| CMake                  | 3.31.1          |
| Visual Studio | 2022 Community  |

Steps:
1. Add `CMake` and `GnuWin32` to your system's PATH.
2. Clone the repository:  
   `git clone https://github.com/WIFSurvivors/Raytracer.git`
3. Navigate to `./Engine` and run:  
   - Create Makefiles: `cmake -B build -G "Unix Makefiles"`
   - Build: `cmake --build build -j5` (replace `5` with your CPU core count).
4. Open the `.sln` file under `./GUI/RaytracerGUI` to start the program.

---

## Usage

### First Start
Upon launch, the Raytracer starts empty. Click **Connect** to establish the engine connection. 

### Layout Overview
- **Menu**: Access actions and settings.
- **Entities and Components**:  
  - Manage entities (left panel).  
  - Adjust options for entities and components (right panels).  
- **Realtime Renderer**: Preview the scene dynamically.  
- **Interaction Zone**: Manipulate objects in the scene.

For detailed instructions, refer to the [User Guide](https://github.com/WIFSurvivors/Raytracer/wiki/User-Guide).

### Options Menu
- **File**: Import/export scenes, access help, or exit.
- **Add**: Add new entities or components (e.g., lights, cameras, renders).

---

## Features
- **Entity-Component-System (ECS)** for modular scene management.
- Import and export scenes as JSON files.
- Realtime raytracing and manipulation tools.
- Flexible entity management with customizable components.

---

## Known Limitations
This project is a work in progress (WIP). Current issues include:
- Entity display issues after loading a new object file (requires manual refresh).
- No support for switching main cameras.
- Engine crashes when a main camera entity with multiple components is deleted.

---

## Contributing
Please submit issues or pull requests via our [GitHub Repository](https://github.com/WIFSurvivors/Raytracer).

---

## License
This project is licensed under the MIT License. See `LICENSE` for details.
