cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(teiacare_image_test_package CXX)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)
find_package(teiacare_image CONFIG REQUIRED)

add_executable(teiacare_image_test_package main.cpp)
target_link_libraries(teiacare_image_test_package PRIVATE teiacare::image)
target_compile_features(teiacare_image_test_package PRIVATE cxx_std_20)
install(TARGETS teiacare_image_test_package DESTINATION .)
