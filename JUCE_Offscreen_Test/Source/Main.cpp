#include "JuceHeader.h"
#include "MainComponent.h"
#include "OffscreenRendererComponent.h"

// Define application name and version
#ifndef JUCE_APPLICATION_NAME_STRING
#define JUCE_APPLICATION_NAME_STRING "JUCE Offscreen Test"
#endif

#ifndef JUCE_APPLICATION_VERSION_STRING
#define JUCE_APPLICATION_VERSION_STRING "1.0.0"
#endif

// Headless renderer for offscreen-only mode
class HeadlessRenderer : private juce::Timer
{
public:
    HeadlessRenderer()
    {
        // Create the renderer component
        renderer = std::make_unique<OffscreenRendererComponent>();
        renderer->setSize(800, 600);
        
        // Configure for offscreen rendering
        renderer->enableOffscreenRendering(true);
        
        // Set up screenshot capture - use project root folder
        juce::File projectRoot = juce::File::getCurrentWorkingDirectory();
        juce::File screenshotDir = projectRoot.getChildFile("screenshots");
        
        // Create the directory if it doesn't exist
        if (!screenshotDir.exists())
        {
            juce::Result result = screenshotDir.createDirectory();
            if (result.failed())
            {
                juce::Logger::writeToLog("Failed to create screenshot directory: " + result.getErrorMessage());
            }
            else
            {
                juce::Logger::writeToLog("Created screenshot directory: " + screenshotDir.getFullPathName());
            }
        }
        
        // Initialize the OpenGL context for headless rendering
        renderer->getOpenGLContext().initializeForHeadlessRendering(800, 600);
        
        // Set up screenshot capture
        renderer->setScreenshotDirectory(screenshotDir);
        renderer->setScreenshotInterval(1000); // 1 screenshot per second
        
        // Also create a direct test image in case OpenGL rendering isn't working
        juce::Image testImage(juce::Image::ARGB, 800, 600, true);
        juce::Graphics g(testImage);
        
        // Fill the background
        g.fillAll(juce::Colours::darkblue);
        
        // Draw some text
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(juce::FontOptions().withHeight(32.0f)));
        g.drawText("Direct test image - not from OpenGL", juce::Rectangle<float>(0, 0, 800, 600), 
                  juce::Justification::centred, true);
        
        // Save this test image directly
        juce::File testFile = screenshotDir.getChildFile("direct_test_image.png");
        juce::FileOutputStream stream(testFile);
        if (stream.openedOk())
        {
            juce::PNGImageFormat pngFormat;
            bool success = pngFormat.writeImageToStream(testImage, stream);
            juce::String resultMessage = success ? "saved" : "failed to save";
            juce::Logger::writeToLog("Direct test image " + resultMessage + " to: " + testFile.getFullPathName());
        }
        else
        {
            juce::Logger::writeToLog("Failed to open file stream for direct test image");
        }
        
        // Start timer to quit app after a few seconds
        startTimer(5000); // 5 seconds
        
        juce::Logger::writeToLog("Headless renderer started - capturing screenshots to " + screenshotDir.getFullPathName());
        juce::Logger::writeToLog("Screenshots will be saved for 5 seconds before application quits");
        
        // Take an immediate screenshot
        juce::MessageManager::callAsync([this]() {
            renderer->captureScreenshotNow();
            juce::Logger::writeToLog("Initial screenshot capture requested");
        });
    }
    
    ~HeadlessRenderer() override
    {
        juce::Logger::writeToLog("Headless renderer shutting down");
    }
    
private:
    void timerCallback() override
    {
        // Take one last screenshot before quitting
        renderer->captureScreenshotNow();
        
        // After 5 seconds, quit the application
        juce::Logger::writeToLog("Finished capturing screenshots, quitting application");
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    std::unique_ptr<OffscreenRendererComponent> renderer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeadlessRenderer)
};

class OffscreenTestApplication : public juce::JUCEApplication
{
public:
    OffscreenTestApplication() {}

    const juce::String getApplicationName() override       { return JUCE_APPLICATION_NAME_STRING; }
    const juce::String getApplicationVersion() override    { return JUCE_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise(const juce::String& commandLine) override
    {
        juce::ignoreUnused(commandLine);

        // Force headless mode only - ignore any window flags
        headlessRenderer = std::make_unique<HeadlessRenderer>();
        
        juce::Logger::writeToLog("Running in headless mode - no window will be shown");
        juce::Logger::writeToLog("Screenshots will be saved to: " + 
            juce::File::getCurrentWorkingDirectory().getChildFile("screenshots").getFullPathName());
    }

    void shutdown() override
    {
        headlessRenderer = nullptr;
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        juce::ignoreUnused(commandLine);
    }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                             juce::Desktop::getInstance().getDefaultLookAndFeel()
                                           .findColour(juce::ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
           #else
            setResizable(true, true);
            centreWithSize(800, 600);
           #endif

            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<HeadlessRenderer> headlessRenderer;
};

// This macro defines and creates the main application instance
START_JUCE_APPLICATION(OffscreenTestApplication) 