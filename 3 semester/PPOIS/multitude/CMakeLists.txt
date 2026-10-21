cmake_minimum_required(VERSION 3.14)
project(CantorSetLab1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable gcov code coverage flags" OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(ENABLE_COVERAGE)
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()

# Библиотека класса CantorSet
add_library(cantor_set_lib STATIC
    src/CantorSet.cpp
)
target_include_directories(cantor_set_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Консольная программа с меню
add_executable(cantor_set_app
    src/main.cpp
)
target_link_libraries(cantor_set_app PRIVATE cantor_set_lib)

# Модульные тесты (UnitTest++)
find_path(UNITTESTPP_INCLUDE_DIR NAMES UnitTest++/UnitTest++.h)
find_library(UNITTESTPP_LIBRARY NAMES UnitTest++)

add_executable(cantor_set_tests
    tests/test_cantor_set.cpp
)

if(UNITTESTPP_INCLUDE_DIR AND UNITTESTPP_LIBRARY)
    message(STATUS "Found system UnitTest++: ${UNITTESTPP_LIBRARY}")
    target_include_directories(cantor_set_tests PRIVATE ${UNITTESTPP_INCLUDE_DIR})
    target_link_libraries(cantor_set_tests PRIVATE cantor_set_lib ${UNITTESTPP_LIBRARY})
else()
    message(STATUS "Using bundled third_party/UnitTest++ header")
    target_include_directories(cantor_set_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    )
    target_link_libraries(cantor_set_tests PRIVATE cantor_set_lib)
endif()

enable_testing()
add_test(NAME CantorSetUnitTests COMMAND cantor_set_tests)
