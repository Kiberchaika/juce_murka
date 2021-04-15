/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 juce_murka
  vendor:             kiberchaika
  version:            0.0.1
  name:               JUCE Murka UI
  description:
  website:            https://github.com/Kiberchaika/juce_murka
  license:            Free

  dependencies:       juce_opengl
  searchpaths:        fontstash/src Murka
  defines:            MURKA_JUCE JUCE_OPENGL3

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once

#define MURKA_JUCE

#include "Murka.h"
#include "MurkaBasicWidgets.h"
#include "DraggableNumberEditor.h"
#include "TextField.h"
#include "LambdaButton.h"
#include "LambdaPanel.h"
#include "Pane.h"
#include "ScrollBar.h"
#include "MosaicLayout.h"
#include "MurkaRenderer.h"
