cmake_minimum_required(VERSION 3.1)

project(Project)

set(SFML_DIR "SFML-2.5.1/lib/cmake/SFML")

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# For debugging
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fno-omit-frame-pointer -fsanitize=address,undefined")
# set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")

# Include all files
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCES "*.cpp" ".hpp" "src/*.hpp" "src/*.cpp")

# main executable
add_executable(main ${SOURCES})

target_link_libraries(main sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
