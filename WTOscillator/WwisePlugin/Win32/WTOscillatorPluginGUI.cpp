
#include "WTOscillatorPluginGUI.h"

WTOscillatorPluginGUI::WTOscillatorPluginGUI()
{
}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    WTOscillator,            // Name of the plug-in container for this shared library
    WTOscillatorPluginGUI,   // Authoring plug-in class to add to the plug-in container
    WTOscillatorSource       // Corresponding Sound Engine plug-in class
);
