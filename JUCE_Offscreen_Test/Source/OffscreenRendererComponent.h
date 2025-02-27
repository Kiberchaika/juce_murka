#pragma once

#include "JuceHeader.h"
#include "OffscreenOpenGLContext.h"

/**
 * Component that handles rendering with support for offscreen mode
 */
class OffscreenRendererComponent : public juce::Component,
                                  private juce::OpenGLRenderer
{
public:
    OffscreenRendererComponent();
    ~OffscreenRendererComponent() override;
    
    /** Enable or disable offscreen rendering */
    void enableOffscreenRendering(bool enable);
    
    /** Check if offscreen rendering is enabled */
    bool isOffscreenRenderingEnabled() const { return offscreenRenderingEnabled; }
    
    /** Set the directory where screenshots will be saved */
    void setScreenshotDirectory(const juce::File& directory);
    
    /** Set the interval between automatic screenshots (in milliseconds) */
    void setScreenshotInterval(int intervalMs);
    
    /** Capture a screenshot immediately */
    void captureScreenshotNow();
    
    /** Access the OpenGL context */
    OffscreenOpenGLContext& getOpenGLContext() { return openGLContext; }
    
    /** Called when OpenGL context is initialized */
    void newOpenGLContextCreated() override;
    
    /** Called when OpenGL context is about to be destroyed */
    void openGLContextClosing() override;
    
    /** Render the next frame */
    void renderOpenGL() override;
    
    /** Paint the component */
    void paint(juce::Graphics& g) override;
    
    /** Handle component resize */
    void resized() override;
    
private:
    OffscreenOpenGLContext openGLContext;
    bool offscreenRenderingEnabled = false;
    
    // Animation state
    float startTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OffscreenRendererComponent)
}; 