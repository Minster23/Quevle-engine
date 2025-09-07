# Quavle Engine

![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/language-C++-blue)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey)
![Status](https://img.shields.io/badge/status-early%20development-orange)

**Quavle Engine** adalah sebuah *open-source 3D game engine* ringan dan modular, ditulis dalam C++ dengan fokus pada performa, fleksibilitas, dan kemudahan integrasi. Dirancang untuk pengembang indie dan pembelajar grafis komputer yang ingin memahami atau membangun mesin game dari bawah.

---
## Preview
<img width="1920" height="1002" alt="image" src="https://github.com/user-attachments/assets/d1893b62-82a8-41ac-9d02-fe8932e3f935" />
---

## ✨ Fitur Utama

- 🔹 **Modular Renderer**: Dukungan OpenGL .
- 🔹 **Entity-Component System (ECS)** sederhana dan efisien.
- 🔹 **Multi-threaded asset loading** (model, material, texture).
- 🔹 **Support Model Format**: `.obj`, `.fbx` (dengan Assimp).
- 🔹 **Pencahayaan Dinamis**: Directional, Point, dan Spot light.
- 🔹 **Post-processing & AO**: SSAO, HBAO, Bloom, Tone Mapping.
- 🔹 **Shader System**: GLSL custom shaders.
- 🔹 **Input & Window Handling**: GLFW + ImGui untuk debug UI.
- 🔹 **Cross-platform (Windows)**.
- 🔹 **MIT License**: Gratis digunakan untuk tujuan komersial dan pribadi.

---

## 🛠️ Instalasi

### Prasyarat

- CMake 3.16+
- Compiler C++17 (GCC/Clang/MSVC)
- OpenGL 3.3+
- Git
- GLFW
- GLEW
- GLM
- GLAD
- ImGui

### Build (Windows/Linux)

```bash
git clone https://github.com/username/quavle-engine.git
cd quavle-engine
mkdir build && cd build
cmake ..
cmake --build . --config Release
