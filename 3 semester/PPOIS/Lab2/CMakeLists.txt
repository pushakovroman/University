cmake_minimum_required(VERSION 3.14)
project(MuseumLab2 VERSION 1.0.0 LANGUAGES CXX)

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

set(MUSEUM_SOURCES
    src/MuseumExceptions.cpp
    src/Artifacts.cpp
    src/HallsAndClimate.cpp
    src/StorageAndLogistics.cpp
    src/ExhibitionsAndEvents.cpp
    src/VisitorsAndTickets.cpp
    src/StaffAndHR.cpp
    src/RestorationLab.cpp
    src/SecuritySystem.cpp
    src/FinanceAndShop.cpp
)

add_library(museum_lib STATIC ${MUSEUM_SOURCES})
target_include_directories(museum_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(museum_app
    src/main.cpp
)
target_link_libraries(museum_app PRIVATE museum_lib)

find_path(UNITTESTPP_INCLUDE_DIR NAMES UnitTest++/UnitTest++.h)
find_library(UNITTESTPP_LIBRARY NAMES UnitTest++)

add_executable(museum_tests
    tests/test_museum.cpp
)

if(UNITTESTPP_INCLUDE_DIR AND UNITTESTPP_LIBRARY)
    target_include_directories(museum_tests PRIVATE ${UNITTESTPP_INCLUDE_DIR})
    target_link_libraries(museum_tests PRIVATE museum_lib ${UNITTESTPP_LIBRARY})
else()
    target_include_directories(museum_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    )
    target_link_libraries(museum_tests PRIVATE museum_lib)
endif()

enable_testing()
add_test(NAME MuseumUnitTests COMMAND museum_tests)
