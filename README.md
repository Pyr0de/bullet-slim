# Bullet Slim
[![CodeQL](https://github.com/Pyr0de/bullet-slim/actions/workflows/codeql.yml/badge.svg)](https://github.com/Pyr0de/bullet-slim/actions/workflows/codeql.yml)

Bullet Slim is a bullet-hell game made using SDL2

## Running
### [WASM build](https://pyr0de.github.io/bullet-slim/)  
OR  
### Native x86_64 Linux binary
- Install [prerequisites](#prerequisites)
- Download latest files from Release section
- Extract files and navigate inside the folder
- Execute the "bullet-slim" binary

## Prerequisites
- SDL2
- SDL2-image
- SDL2-ttf
- Emscripten (WASM build)

### Ubuntu (apt)
```sh
sudo apt update && sudo apt update
sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-ttf-2.0-0
# For building from source
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev git cmake
```

## Building
Install all [prerequisites](#prerequisites) along with all the dev packages
Clone repository
```sh
git clone https://github.com/Pyr0de/bullet-slim
cd bullet-slim
```
Build (Cmake)
```sh
mkdir build && cd build
cmake .. -DDEBUG=0 -DWASM=0 -Wno-dev
```
`-DDEBUG`: Shows hitboxes for entities and other debugging information  
`-DWASM` : Creates the WASM build (requires [Emscripten](https://emscripten.org/docs/getting_started/downloads.html))

Build
```sh
make -j$(nproc)
```
Run
```sh
./bullet-slim
```

If CMake is run with `-DWASM=1`, the files will be present in `~/build/wasm`
