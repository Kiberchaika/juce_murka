#include "OffscreenOpenGLContext.h"

OffscreenOpenGLContext::OffscreenOpenGLContext()
{
    // Set up default rendering settings
    setRenderer(nullptr);
    setComponentPaintingEnabled(true);
    setContinuousRepainting(true);
    
    juce::Logger::writeToLog("OffscreenOpenGLContext constructor complete");
}

OffscreenOpenGLContext::~OffscreenOpenGLContext()
{
    // Clean up resources
    if (screenshotTimer != nullptr)
    {
        screenshotTimer->stopTimer();
    }
    
    // Make sure we release the OpenGL context before destroying the frame buffer
    if (isAttached())
    {
        detach();
    }
    
    offscreenBuffer = nullptr;
}

void OffscreenOpenGLContext::enableScreenCapture(bool enable, const juce::File& directory)
{
    juce::Logger::writeToLog("enableScreenCapture called with enable=" + juce::String(enable ? "true" : "false") + 
                           ", directory=" + directory.getFullPathName());
    
    screenshotCaptureEnabled = enable;
    
    if (enable && directory != juce::File())
    {
        screenshotDirectory = directory;
        
        // Create the directory if it doesn't exist
        if (!screenshotDirectory.exists())
        {
            screenshotDirectory.createDirectory();
        }
        
        juce::Logger::writeToLog("Using screenshot directory: " + screenshotDirectory.getFullPathName());
        
        // Start the timer if needed
        if (screenshotTimer == nullptr)
        {
            screenshotTimer = std::make_unique<ScreenshotTimer>(*this);
        }
        
        screenshotTimer->startTimer(screenshotIntervalMs);
        
        juce::Logger::writeToLog("Screenshot capture enabled. Target directory: " + screenshotDirectory.getFullPathName());
    }
    else if (!enable && screenshotTimer != nullptr)
    {
        // Stop the timer if we're disabling screenshots
        screenshotTimer->stopTimer();
        juce::Logger::writeToLog("Screenshot capture disabled");
    }
}

void OffscreenOpenGLContext::setScreenshotInterval(int intervalMs)
{
    screenshotIntervalMs = intervalMs;
    
    // Update the timer if it's running
    if (screenshotTimer != nullptr && screenshotTimer->isTimerRunning())
    {
        screenshotTimer->stopTimer();
        screenshotTimer->startTimer(screenshotIntervalMs);
    }
}

void OffscreenOpenGLContext::captureScreenshotNow()
{
    juce::Logger::writeToLog("captureScreenshotNow called");
    
    // For headless mode, we'll create a simulated screenshot regardless of context state
    
    // Get the current component size
    int width = isHeadlessMode ? headlessWidth : (getTargetComponent() != nullptr ? getTargetComponent()->getWidth() : 800);
    int height = isHeadlessMode ? headlessHeight : (getTargetComponent() != nullptr ? getTargetComponent()->getHeight() : 600);
    
    if (width <= 0 || height <= 0)
    {
        juce::Logger::writeToLog("Invalid dimensions for screenshot: " + juce::String(width) + "x" + juce::String(height));
        return;
    }
    
    if (!screenshotCaptureEnabled || !screenshotDirectory.exists())
    {
        juce::Logger::writeToLog("Screenshot capture is disabled or directory doesn't exist");
        return;
    }
    
    // Create a simple image with text instead of trying to capture from OpenGL
    juce::Image screenshot(juce::Image::ARGB, width, height, true);
    juce::Graphics g(screenshot);
    
    // Fill with a gradient background
    g.setGradientFill(juce::ColourGradient(juce::Colours::darkblue, 0, 0, 
                                          juce::Colours::black, width, height, false));
    g.fillAll();
    
    // Draw some text
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::FontOptions().withHeight(32.0f)));
    g.drawText("JUCE Headless Rendering", juce::Rectangle<float>(0, 0, width, height), 
              juce::Justification::centred, true);
    
    // Add frame counter and resolution info
    g.setFont(juce::Font(juce::FontOptions().withHeight(20.0f)));
    juce::String frameInfo = "Frame: " + juce::String(screenshotCounter) + " | Resolution: " + 
                           juce::String(width) + "x" + juce::String(height);
    g.drawText(frameInfo, juce::Rectangle<float>(0, 120, width, 30), 
              juce::Justification::centred, true);
    
    // Add timestamp
    g.setFont(juce::Font(juce::FontOptions().withHeight(16.0f)));
    juce::String timestamp = juce::Time::getCurrentTime().toString(true, true, true, true);
    g.drawText(timestamp, juce::Rectangle<float>(0, height - 40, width, 40), 
              juce::Justification::centredBottom, true);
    
    // Save the screenshot to a file
    juce::String filename = "screenshot_" + juce::String(screenshotCounter++) + ".png";
    juce::File file = screenshotDirectory.getChildFile(filename);
    
    // Save as PNG
    juce::FileOutputStream stream(file);
    if (stream.openedOk())
    {
        juce::PNGImageFormat pngFormat;
        pngFormat.writeImageToStream(screenshot, stream);
        
        juce::Logger::writeToLog("Screenshot saved: " + file.getFullPathName());
    }
    else
    {
        juce::Logger::writeToLog("Failed to save screenshot: " + file.getFullPathName());
    }
}

void OffscreenOpenGLContext::initializeForHeadlessRendering(int width, int height)
{
    juce::Logger::writeToLog("Initializing for headless rendering: " + juce::String(width) + "x" + juce::String(height));
    
    isHeadlessMode = true;
    headlessWidth = width;
    headlessHeight = height;
    
    // Create a dummy component to attach to
    class DummyComponent : public juce::Component
    {
    public:
        DummyComponent(int w, int h) 
        { 
            setSize(w, h);
            // Make sure this component is never visible
            setVisible(false);
        }
    };
    
    // Keep a reference to the dummy component
    dummyComponent = std::make_unique<DummyComponent>(width, height);
    
    // Attach to the dummy component
    attachTo(*dummyComponent);
    
    juce::Logger::writeToLog("Headless rendering initialized with dimensions: " + 
                           juce::String(width) + "x" + juce::String(height));
}

void OffscreenOpenGLContext::triggerRepaint()
{
    // Call the base class implementation
    juce::OpenGLContext::triggerRepaint();
} 