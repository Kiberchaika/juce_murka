# JUCE Offscreen Test for Murka

This project demonstrates how to implement offscreen rendering for Murka UI components in JUCE. 

## Features

- Toggle between normal on-screen and offscreen rendering modes
- Automatic screenshot capture of the offscreen rendered UI
- Minimal code changes required to switch rendering modes
- Maintains identical visual output between modes

## How It Works

The project includes several key components:

1. **OffscreenOpenGLContext**: Extends JUCE's OpenGLContext to support offscreen rendering and screenshot capture
2. **OffscreenMurkaComponent**: Provides a drop-in replacement for JuceMurkaBaseComponent that supports toggling between rendering modes

## Building the Project

To build this project:

1. Make sure you have CMake 3.15 or newer installed
2. Navigate to the JUCE_Offscreen_Test directory
3. Run the following commands:

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

When running the application:

- Click the "Toggle Offscreen Mode" button to switch between rendering modes
- In offscreen mode, screenshots are automatically saved to a directory in your Documents folder
- The text color changes to yellow when in offscreen mode to visually indicate the current mode

## Integration into Existing Projects

To integrate this offscreen rendering capability into your existing Murka projects:

1. Replace inheritance from `JuceMurkaBaseComponent` to `OffscreenMurkaComponent`
2. Rename your `draw()` method to `drawContent()`
3. Use `setOffscreenRenderingEnabled(bool)` to toggle between modes

## Customization

- Screenshot interval can be adjusted using `setScreenshotInterval(int intervalMs)`
- Screenshot save location can be set with `setScreenshotDirectory(const juce::File&)` 