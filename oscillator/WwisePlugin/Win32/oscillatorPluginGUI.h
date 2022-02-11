#pragma once

#include "../oscillatorPlugin.h"

class oscillatorPluginGUI final
	: public AK::Wwise::Plugin::PluginMFCWindows<>
	, public AK::Wwise::Plugin::GUIWindows
{
public:
	oscillatorPluginGUI();

};
