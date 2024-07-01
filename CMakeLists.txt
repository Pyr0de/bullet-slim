cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(platformer-sdl)
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS)
find_package(SDL2_ttf REQUIRED CONFIG REQUIRED COMPONENTS)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image::SDL2_image)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_ttf::SDL2_ttf)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
