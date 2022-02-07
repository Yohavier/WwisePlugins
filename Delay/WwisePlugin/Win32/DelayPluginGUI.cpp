
#include "DelayPluginGUI.h"

DelayPluginGUI::DelayPluginGUI()
{
}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    Delay,            // Name of the plug-in container for this shared library
    DelayPluginGUI,   // Authoring plug-in class to add to the plug-in container
    DelayFX           // Corresponding Sound Engine plug-in class
);
