# CMakeList.txt : CMake project for Skepsou-Zoo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Skepsou-Zoo")

# Add source to this project's executable.
add_executable (Skepsou-Zoo "Skepsou-Zoo.cpp" "Skepsou-Zoo.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Skepsou-Zoo PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
