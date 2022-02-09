#pragma once

#include "../SimpleConvolutionPlugin.h"

class SimpleConvolutionPluginGUI final
	: public AK::Wwise::Plugin::PluginMFCWindows<>
	, public AK::Wwise::Plugin::GUIWindows
{
public:
	SimpleConvolutionPluginGUI();

};
