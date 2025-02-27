# JUCE Offscreen Rendering Test for Murka

## Project Goal
Create an offscreen rendering solution for Murka UI library in JUCE that allows seamless switching between on-screen and off-screen rendering modes for testing purposes.

## Understanding the Rendering Stack
After investigating JUCE's OpenGL rendering system, I've identified the key components in the rendering stack:

1. **JuceMurkaBaseComponent** - Inherits from `juce::OpenGLAppComponent` and acts as the bridge between Murka and JUCE
2. **OpenGLAppComponent** - Provides the rendering loop and OpenGL context management
3. **OpenGLContext** - Manages the OpenGL context and attaches it to a Component
4. **OpenGLFrameBuffer** - Provides offscreen rendering capabilities

## Implementation Plan

### Phase 1: Basic JUCE App with On-Screen Rendering
1. Create a new JUCE standalone application in a folder called `JUCE_Offscreen_Test`
2. Set up a simple UI that displays "hi from off-screen rendering test"
3. Verify that the application runs and displays correctly

### Phase 2: Create Offscreen Renderer
1. Create a subclass of `OpenGLContext` called `OffscreenOpenGLContext` that:
   - Uses `OpenGLFrameBuffer` for offscreen rendering
   - Implements screenshot capture functionality
   - Maintains rendering compatibility with the original context

2. Create a subclass of `JuceMurkaBaseComponent` called `OffscreenMurkaComponent` that:
   - Uses the `OffscreenOpenGLContext` instead of the standard context
   - It does not open window at all, instead it draws offscreen
   - Implements a screenshot capture and save mechanism

### Phase 3: Testing Infrastructure
1. Create a simple test harness that allows toggling between rendering modes
2. Implement automated screenshot comparison for testing
3. Create a demo that shows both modes functioning with the same codebase

## Key Technical Approach
The implementation will focus on minimizing changes to the existing rendering pipeline by:

1. Subclassing `OpenGLContext` to create an offscreen rendering context
2. Using `OpenGLFrameBuffer` to render to an offscreen buffer
3. Periodically capturing the buffer contents to generate screenshots
4. Providing a simple inheritance swap to toggle between modes

This approach will allow existing Murka applications to switch to offscreen rendering with minimal code changes while maintaining identical visual output.

## Next Steps
1. Set up the JUCE standalone application project
2. Implement the basic on-screen version
3. Analyze rendering pipeline in more detail
4. Implement the offscreen rendering context 