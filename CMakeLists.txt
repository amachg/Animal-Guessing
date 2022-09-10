# CMakeList.txt : CMake project for Skepsou-Zoo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Animal-Guessing")

# Add source to this project's executable.
add_executable (Animal-Guessing "Animal-Guessing.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Animal-Guessing PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
