cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(platformer-sdl)
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

if (WASM)
	message("EMSCRIPTEN")
	#set(CMAKE_CXX_COMPILER "emcc")
    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s USE_SDL_TTF=2")
	#set(USE_FLAGS "${USE_FLAGS} -Iinclude --preload-file assets")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

else ()
	message("NATIVE")
	find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS)
	find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS)
	find_package(SDL2_ttf REQUIRED CONFIG REQUIRED COMPONENTS)
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image::SDL2_image)
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_ttf::SDL2_ttf)
endif()

if (DEBUG)
	add_compile_definitions(__DEBUG__)
	message("DEBUG MODE")
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
