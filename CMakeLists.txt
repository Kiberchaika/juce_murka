cmake_minimum_required(VERSION 3.15)

project(juce_murka VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

include(cmake/CPM.cmake)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    CPMAddPackage("gh:juce-framework/JUCE#8.0.1")
endif()

include(GNUInstallDirs)

juce_add_module(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

option(JUCE_MURKA_TESTS OFF "Enable juce_murka testint")

if (JUCE_MURKA_TESTS)
    add_subdirectory(tests/murka-test)
endif()