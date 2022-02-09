
#include "SimpleConvolutionPluginGUI.h"

SimpleConvolutionPluginGUI::SimpleConvolutionPluginGUI()
{
}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    SimpleConvolution,            // Name of the plug-in container for this shared library
    SimpleConvolutionPluginGUI,   // Authoring plug-in class to add to the plug-in container
    SimpleConvolutionFX           // Corresponding Sound Engine plug-in class
);
