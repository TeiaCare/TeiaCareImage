cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
file(STRINGS "VERSION" VERSION_STR)
project(teiacare_image
    VERSION ${VERSION_STR}
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/TeiaCare/TeiaCareImage"
    DESCRIPTION "TeiaCareImage is a collection of C++ image processing utilities"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)

include(GNUInstallDirs)
include(CMakePrintHelpers)

include(compiler_info)
include(warnings)
include(options)
validate_project_options()

enable_testing()
add_subdirectory(image)
