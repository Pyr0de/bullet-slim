cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(bullet-slim)
file(GLOB SOURCES "src/*.cpp")

if (WASM)
	message("EMSCRIPTEN")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/wasm)
	set(CMAKE_CXX_COMPILER "em++")
	set(CMAKE_C_COMPILER "emcc")

    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s USE_SDL_TTF=2")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")

	set(USE_FLAGS "${USE_FLAGS} -s EXPORTED_FUNCTIONS='[_resizeCanvas, _main]' -s EXPORTED_RUNTIME_METHODS='[cwrap]'")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} --preload-file assets")

    set(CMAKE_EXECUTABLE_SUFFIX .js)

else ()
	message("NATIVE")
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)
endif()

if (DEBUG)
	add_compile_definitions(__DEBUG__)
	message("DEBUG MODE")
endif ()

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

if (NOT WASM)
	target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF)
endif ()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
