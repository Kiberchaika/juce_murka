# Murka Offscreen Rendering Test Plan

## Overview
This document outlines the plan for implementing offscreen rendering and automated testing for the Murka UI library by extending the existing juce_murka infrastructure. The key design goal is to minimize changes to both juce_murka and JUCE while enabling applications to use offscreen rendering with minimal code changes.

## Goals
1. Create a headless rendering pipeline for Murka UI components
2. Enable automated screenshot capture of rendered Murka UI
3. Allow for both interactive and fully automated testing modes
4. Support integration into CI/CD pipelines
5. Ensure applications only need to change one line (class inheritance) to switch to offscreen rendering

## Implementation Plan

### Phase 1: JuceMurkaOffscreenComponent

1. Create a `JuceMurkaOffscreenComponent` class that:
   - Extends from `JuceMurkaBaseComponent` with minimal changes
   - Redirects rendering to an offscreen framebuffer instead of the screen
   - Maintains identical API surface so existing apps can simply swap the base class

2. Strategically fork only necessary JUCE classes:
   - Identify which JUCE classes are essential for offscreen rendering
   - Create minimally modified versions of only those classes
   - Place them in this repository to avoid JUCE dependencies

3. Modify OpenGL context handling:
   - Ensure OpenGL context initialization works without a visible window
   - Implement framebuffer capture for screenshots
   - Maintain all existing event handling logic

### Phase 2: Murka Test Harness

1. Create a `MurkaTestHarness` class that:
   - Uses the `JuceMurkaOffscreenComponent` for rendering
   - Provides mechanisms to simulate user interaction with Murka components
   - Supports programmatic control of the test environment

2. Implement screenshot comparison utilities:
   - Allow comparison of generated screenshots against reference images
   - Implement similarity metrics for automated validation
   - Support pixel-perfect testing where required

### Phase 3: Integration with Existing Code

1. Ensure backward compatibility:
   - Applications should only need to change `JuceMurkaBaseComponent` to `JuceMurkaOffscreenComponent`
   - All existing Murka features should work identically in both rendering modes
   - Event simulation should mimic real user interaction

2. Create a command-line interface for:
   - Specifying which Murka tests to run
   - Controlling test parameters (resolution, duration, etc.)
   - Generating reports on test outcomes

## Key Technical Aspects

### Offscreen Rendering Implementation
We'll leverage the existing `JuceMurkaBaseComponent` infrastructure with these modifications:
- Replace window-based rendering with framebuffer rendering
- Maintain the same OpenGL initialization sequence
- Ensure texture and shader handling works identically

### Event Simulation
We'll reuse the existing event handlers in `JuceMurkaBaseComponent`:
- Simulate mouse and keyboard events using the same methods
- Ensure events follow the same processing path as in interactive mode
- Verify that Murka responds correctly to simulated events

### Test Integration
The test system will:
- Run alongside existing tests without interference
- Support automated execution in CI environments
- Produce comparable outputs for validation

## Implementation Strategy
1. Create minimal fork of required JUCE classes
2. Implement `JuceMurkaOffscreenComponent` extending `JuceMurkaBaseComponent`
3. Add framebuffer capture capabilities
4. Build test harness and event simulation
5. Document usage pattern for applications

## Timeline
- Week 1: Fork required JUCE classes and create `JuceMurkaOffscreenComponent`
- Week 2: Implement framebuffer capture and screenshot utilities
- Week 3: Build test harness and event simulation system
- Week 4: Integration testing and documentation

## Required Files
- `JuceMurkaOffscreenComponent.h/cpp`
- Forked JUCE classes (minimal set)
- `MurkaTestHarness.h/cpp`
- `MurkaScreenshotComparator.h/cpp`
- `MurkaTestRunner.h/cpp`
- Example tests and reference images 