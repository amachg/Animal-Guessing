# CMakeList.txt : CMake project for Animal-Guessing

cmake_minimum_required (VERSION 3.21)

project ("Animal-Guessing")

add_executable (${PROJECT_NAME} "Animal-Guessing.cpp")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)	# enable C++/23 standard
