cmake_minimum_required(VERSION 3.15.0)
project(murka-tests VERSION 1.2.3)

### IDE Generator pre-config ###
# Xcode: Disable automatic build scheme generation globally.
#        Instead, we explicitely specify which targets require schemes.
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
# Enable to build universal binaries on macOS, increasing build time
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version" FORCE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Architectures" FORCE)
    set(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET[arch=arm64] "11.0" CACHE STRING "arm 64 minimum deployment target" FORCE)
endif()# IDEs:  Enable grouping of source files into folders in IDEs.

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# IDEs:  Create a folder in the IDE with the JUCE Module code.
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

# add the plugin targets
juce_add_plugin(${PROJECT_NAME}
                COMPANY_NAME "Kiberchaika"
                PLUGIN_MANUFACTURER_CODE "mrka"
                PLUGIN_CODE test
                FORMATS VST3 AAX Standalone   
                PLUGIN_NAME ${PROJECT_NAME}
                PRODUCT_NAME ${PROJECT_NAME}
                IS_SYNTH FALSE)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC 
    JUCE_MODAL_LOOPS_PERMITTED=1
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

# Set the C++ language standard requirenment for the "shared code" library target.
# Setting this to PUBLIC ensures that all dependent targets will inherit the specified C++ standard.
target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_17)
# Disable compiler extensions for the project targets (e.g. use -std=c++17 instead of -std=gnu++17).
get_property(project_targets DIRECTORY "${PROJECT_SOURCE_DIR}" PROPERTY BUILDSYSTEM_TARGETS)
set_target_properties(${project_targets} PROPERTIES CXX_EXTENSIONS OFF)

# add the sources
target_sources(${PROJECT_NAME} PRIVATE 
                                    Source/PluginEditor.cpp
                                    Source/PluginEditor.h
                                    Source/PluginProcessor.cpp
                                    Source/PluginProcessor.h
                                    Source/Label.h
                                    Source/Checkbox.h
                                    Source/Knob.h
                                    Source/UIBaseComponent.h
                                    Source/UIBaseComponent.cpp)
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})

# include for default font
add_subdirectory(Resources)

# add required flags
juce_generate_juce_header(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE juce::juce_audio_basics juce::juce_audio_devices juce::juce_audio_formats juce::juce_audio_plugin_client juce::juce_audio_processors juce::juce_audio_utils juce::juce_core juce::juce_data_structures juce::juce_events juce::juce_graphics juce::juce_gui_basics juce::juce_gui_extra juce::juce_opengl juce_murka)

### IDE Generator post-config ###
# IDEs:  Move the "shared code" source group (main target) out of the "Targets" folder
#        to the top level.
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

# IDEs:  Organise source file grouping.
get_target_property(project_sources "${PROJECT_NAME}" SOURCES)

#   If JuceHeader.h is generated, remove it from the source file list and handle it individually.
get_target_property(juce_library_code "${PROJECT_NAME}" JUCE_GENERATED_SOURCES_DIRECTORY)
set(juce_header "${juce_library_code}/JuceHeader.h")
list(REMOVE_ITEM project_sources "${juce_header}")

#   Place JuceHeader.h and the associated CMake Rule file into a "JUCE Library Code" folder.
source_group("JUCE Library Code" FILES "${juce_header}")
source_group("JUCE Library Code/CMake Rules" FILES "${juce_header}.rule")