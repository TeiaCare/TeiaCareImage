set(TARGET_NAME teiacare_image)
add_library(${TARGET_NAME})
add_library(teiacare::image ALIAS ${TARGET_NAME})

find_package(stb CONFIG REQUIRED)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp
)

set(TARGET_HEADERS
    include/teiacare/image/img.hpp
    include/teiacare/image/version.hpp
)

set(TARGET_SOURCES
    src/img.cpp
    src/version.cpp
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
target_sources(${TARGET_NAME} PUBLIC ${TARGET_HEADERS} PRIVATE ${TARGET_SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE stb::stb)
target_include_directories(${TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>
)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${TARGET_HEADERS}")
install(TARGETS ${TARGET_NAME} PUBLIC_HEADER DESTINATION include/teiacare/image)

if(TC_ENABLE_WARNINGS_ERROR)
    include(warnings)
    add_warnings(${TARGET_NAME})
    add_warnings_as_errors(${TARGET_NAME})
endif()

if(TC_ENABLE_SANITIZER_ADDRESS)
    include(sanitizer_address)
    add_sanitizer_address(${TARGET_NAME})
endif()

if(TC_ENABLE_SANITIZER_THREAD)
    include(sanitizer_thread)
    add_sanitizer_thread(${TARGET_NAME})
endif()

#################################################################
# Unit Tests
if(TC_ENABLE_UNIT_TESTS)
    set(TEST_TARGET_NAME teiacare_image_unittests)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    # set(DATA_ABS_PATH ${CMAKE_SOURCE_DIR}/data/)
    # configure_file(
    #     ${CMAKE_CURRENT_SOURCE_DIR}/tests/utils/data_path.hpp.in
    #     ${CMAKE_CURRENT_SOURCE_DIR}/tests/utils/data_path.hpp)

    set(UNIT_TESTS_SRC
        tests/main.cpp
    )

    add_executable(${TEST_TARGET_NAME})
    target_sources(${TEST_TARGET_NAME} PRIVATE ${UNIT_TESTS_SRC})
    target_include_directories(${TEST_TARGET_NAME} PRIVATE  ${CMAKE_CURRENT_BINARY_DIR}/proto)
    target_compile_features(${TEST_TARGET_NAME} PUBLIC cxx_std_20)
    target_link_libraries(${TEST_TARGET_NAME}
        PRIVATE
            GTest::gtest
            GTest::gmock
            teiacare::image
    )

    gtest_discover_tests(${TEST_TARGET_NAME})
    install(TARGETS ${TEST_TARGET_NAME} DESTINATION unit_tests)

    if(TC_ENABLE_WARNINGS_ERROR)
        add_warnings(${TEST_TARGET_NAME})
        add_warnings_as_errors(${TEST_TARGET_NAME})
    endif()

    if(TC_ENABLE_UNIT_TESTS_COVERAGE)
        include(coverage)
        add_coverage(${TARGET_NAME})
    endif()
endif()

#################################################################
# Examples
if(TC_ENABLE_EXAMPLES)
    add_executable(example_image examples/example.cpp)
    target_compile_features(example_image PUBLIC cxx_std_20)
    target_link_libraries(example_image PRIVATE teiacare::image)
    install(TARGETS example_image DESTINATION examples)
endif()
