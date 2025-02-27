#pragma once

#include "JuceHeader.h"

/**
 * Extended OpenGL context that supports offscreen rendering and screenshot capture
 */
class OffscreenOpenGLContext : public juce::OpenGLContext
{
public:
    OffscreenOpenGLContext();
    ~OffscreenOpenGLContext();
    
    /** Enable or disable screenshot capture */
    void enableScreenCapture(bool enable, const juce::File& directory);
    
    /** Capture a screenshot now */
    void captureScreenshotNow();
    
    /** Set the interval between automatic screenshots (in milliseconds) */
    void setScreenshotInterval(int intervalMs);
    
    /** Check if screenshot capture is enabled */
    bool isScreenCaptureEnabled() const { return screenshotCaptureEnabled; }
    
    /** Get the current screenshot directory */
    juce::File getScreenshotDirectory() const { return screenshotDirectory; }
    
    /** Get the current screenshot interval */
    int getScreenshotInterval() const { return screenshotIntervalMs; }
    
    /** Force the context to initialize for headless rendering */
    void initializeForHeadlessRendering(int width, int height);
    
    /** Trigger a repaint - overridden to handle offscreen mode */
    void triggerRepaint();
    
private:
    class ScreenshotTimer : public juce::Timer
    {
    public:
        ScreenshotTimer(OffscreenOpenGLContext& ctx) : context(ctx) {}
        
        void timerCallback() override
        {
            context.captureScreenshotNow();
        }
        
    private:
        OffscreenOpenGLContext& context;
    };
    
    bool screenshotCaptureEnabled = false;
    juce::File screenshotDirectory;
    int screenshotIntervalMs = 1000;
    int screenshotCounter = 0;
    
    std::unique_ptr<ScreenshotTimer> screenshotTimer;
    std::unique_ptr<juce::OpenGLFrameBuffer> offscreenBuffer;
    
    bool isHeadlessMode = false;
    int headlessWidth = 800;
    int headlessHeight = 600;
    
    // Keep a reference to the dummy component to make sure it stays alive
    std::unique_ptr<juce::Component> dummyComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OffscreenOpenGLContext)
}; 