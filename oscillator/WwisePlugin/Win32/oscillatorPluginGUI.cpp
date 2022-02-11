
#include "oscillatorPluginGUI.h"

oscillatorPluginGUI::oscillatorPluginGUI()
{
}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    oscillator,            // Name of the plug-in container for this shared library
    oscillatorPluginGUI,   // Authoring plug-in class to add to the plug-in container
    oscillatorSource       // Corresponding Sound Engine plug-in class
);
