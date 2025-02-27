cmake_minimum_required(VERSION 3.15)

project(JUCE_Offscreen_Test VERSION 1.0.0)

# Include JUCE
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../JUCE ${CMAKE_CURRENT_BINARY_DIR}/JUCE)

# Create app
juce_add_gui_app(OffscreenTest
    PRODUCT_NAME "JUCE Offscreen Test"
    COMPANY_NAME "Murka"
    VERSION "1.0.0"
)

# Define source files
target_sources(OffscreenTest
    PRIVATE
        Source/Main.cpp
        Source/MainComponent.cpp
        Source/OffscreenOpenGLContext.cpp
        Source/OffscreenRendererComponent.cpp
)

# Set up basic app settings
target_compile_definitions(OffscreenTest
    PRIVATE
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:OffscreenTest,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:OffscreenTest,JUCE_VERSION>"
)

# Link to the JUCE modules we need
target_link_libraries(OffscreenTest
    PRIVATE
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_opengl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Add Murka module and related paths
target_include_directories(OffscreenTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Murka
    ${CMAKE_CURRENT_SOURCE_DIR}/../fontstash
) 