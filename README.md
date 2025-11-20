# VGM Creator (Video Game Music Creator)

VGM Creator is a simple music-creation tool designed for game developers.  
It allows you to load custom instruments, edit notes using a beginner-friendly interface, and prepare audio for use in games.

This project focuses on being easy to use for people with little or no DAW experience.

## Features

- Simple and fast UI built with Dear ImGui
- Custom instrument packs stored as `.zip` files
- Built-in audio playback using miniaudio
- Native file dialogs for loading instruments
- ZIP reading using minizip-ng
- OpenGL backend using GLFW + GLAD

## Build Instructions (Windows)

Requirements:

- Visual Studio 2022 or newer
- CMake 3.6+
- Git

### Quick Build (recommended)

Run:

```
build.bat
```

This script:

- Configures CMake
- Builds the project
- Runs the executable

### Manual Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

Run the program:

```
Debug/VGMCreator.exe
```

## Instrument Pack Format

A custom instrument is packaged as a `.zip` file containing:

```
instrument.json
samples/
    c.wav
    d.wav
    e.wav
    f.wav
    g.wav
    a.wav
    b.wav
```

### Example `instrument.json`

```json
{
  "Name": "Electric Piano",
  "Description": "A simple electric piano.",
  "Samples": {
    "BaseNote": "C",
    "Files": {
      "A": "samples/a.wav",
      "B": "samples/b.wav",
      "C": "samples/c.wav",
      "D": "samples/d.wav",
      "E": "samples/e.wav",
      "F": "samples/f.wav",
      "G": "samples/g.wav"
    }
  },
  "Settings": {
    "Volume": 0.8,
    "Attack": 0.02,
    "Release": 0.4,
    "Reverb": 0.25,
    "Chorus": 0.2
  }
}
```

## Libraries Used

- Dear ImGui  
  https://github.com/ocornut/imgui
- GLFW  
  https://github.com/glfw/glfw
- GLAD  
  https://github.com/Dav1dde/glad
- miniaudio  
  https://github.com/mackron/miniaudio
- miniz
  https://github.com/richgel999/miniz
- Native File Dialog (NFD)  
  https://github.com/btzy/nativefiledialog-extended
