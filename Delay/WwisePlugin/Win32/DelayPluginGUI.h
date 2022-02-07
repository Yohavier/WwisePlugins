#pragma once

#include "../DelayPlugin.h"

class DelayPluginGUI final
	: public AK::Wwise::Plugin::PluginMFCWindows<>
	, public AK::Wwise::Plugin::GUIWindows
{
public:
	DelayPluginGUI();

};
