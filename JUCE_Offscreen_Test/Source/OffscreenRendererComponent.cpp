#include "OffscreenRendererComponent.h"

OffscreenRendererComponent::OffscreenRendererComponent()
{
    // Set up the OpenGL context
    openGLContext.setRenderer(this);
    openGLContext.setComponentPaintingEnabled(true);
    openGLContext.setContinuousRepainting(true);
    
    // Attach the context to this component
    openGLContext.attachTo(*this);
    
    // Initialize animation time
    startTime = static_cast<float>(juce::Time::getMillisecondCounterHiRes() * 0.001);
}

OffscreenRendererComponent::~OffscreenRendererComponent()
{
    // Make sure we detach the context before destruction
    openGLContext.detach();
}

void OffscreenRendererComponent::enableOffscreenRendering(bool enable)
{
    if (offscreenRenderingEnabled != enable)
    {
        offscreenRenderingEnabled = enable;
        
        // Update the context's screenshot capture setting
        if (enable)
        {
            juce::Logger::writeToLog("Enabling offscreen rendering");
        }
        else
        {
            juce::Logger::writeToLog("Disabling offscreen rendering");
        }
    }
}

void OffscreenRendererComponent::setScreenshotDirectory(const juce::File& directory)
{
    // Pass through to the context
    openGLContext.enableScreenCapture(true, directory);
}

void OffscreenRendererComponent::setScreenshotInterval(int intervalMs)
{
    // Pass through to the context
    openGLContext.setScreenshotInterval(intervalMs);
}

void OffscreenRendererComponent::captureScreenshotNow()
{
    // Pass through to the context
    openGLContext.captureScreenshotNow();
}

void OffscreenRendererComponent::newOpenGLContextCreated()
{
    // Just log that the context was created
    juce::Logger::writeToLog("OpenGL context created");
}

void OffscreenRendererComponent::openGLContextClosing()
{
    // Clean up OpenGL resources
    juce::Logger::writeToLog("OpenGL context closing");
}

void OffscreenRendererComponent::renderOpenGL()
{
    // Simple rendering - just clear to a color
    juce::OpenGLHelpers::clear(juce::Colours::darkblue);
    
    // Log that rendering occurred
    static int frameCount = 0;
    if (frameCount++ % 60 == 0) // Log every 60 frames to avoid spamming
    {
        juce::Logger::writeToLog("Rendered frame " + juce::String(frameCount));
    }
}

void OffscreenRendererComponent::paint(juce::Graphics& g)
{
    // This will only be called when OpenGL rendering is disabled
    if (!openGLContext.isActive())
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);
        g.setFont(16.0f);
        g.drawText("OpenGL not active", getLocalBounds(), juce::Justification::centred, true);
    }
}

void OffscreenRendererComponent::resized()
{
    // Nothing to do here
} 