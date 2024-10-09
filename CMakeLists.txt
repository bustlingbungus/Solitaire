cmake_minimum_required(VERSION 3.15)

# Set to the folder containing SDL libraries
# set(CMAKE_PREFIX_PATH "C:/msys64/Libraries")

# change to the project's name
project(project)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# puts the executable in build/bin
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# static link to SDL libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image-static)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf-static)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer-static)

# Add flags based on the compiler
if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")  # MinGW/GCC
        target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:CONSOLE)
    endif()
endif()