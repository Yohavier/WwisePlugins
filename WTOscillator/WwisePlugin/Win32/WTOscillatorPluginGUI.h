#pragma once

#include "../WTOscillatorPlugin.h"

class WTOscillatorPluginGUI final
	: public AK::Wwise::Plugin::PluginMFCWindows<>
	, public AK::Wwise::Plugin::GUIWindows
{
public:
	WTOscillatorPluginGUI();

};
