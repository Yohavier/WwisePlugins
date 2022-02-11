/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/

#include "oscillatorSource.h"
#include "../oscillatorConfig.h"

#include <AK/AkWwiseSDKVersion.h>

AK::IAkPlugin* CreateoscillatorSource(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, oscillatorSource());
}

AK::IAkPluginParam* CreateoscillatorSourceParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, oscillatorSourceParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(oscillatorSource, AkPluginTypeSource, oscillatorConfig::CompanyID, oscillatorConfig::PluginID)

oscillatorSource::oscillatorSource()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
{
}

oscillatorSource::~oscillatorSource()
{
}

AKRESULT oscillatorSource::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkSourcePluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
    m_pParams = (oscillatorSourceParams*)in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;

    m_durationHandler.Setup(m_pParams->RTPC.fDuration, in_pContext->GetNumLoops(), in_rFormat.uSampleRate);

    osc = DirectOscillator(in_rFormat.uSampleRate);
    osc.PrepareToPlay();
    osc.ChangeState(1);
    return AK_Success;
}

AKRESULT oscillatorSource::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    osc.ChangeState(2);
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT oscillatorSource::Reset()
{
    return AK_Success;
}

AKRESULT oscillatorSource::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeSource;
    out_rPluginInfo.bIsInPlace = true;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

void oscillatorSource::Execute(AkAudioBuffer* out_pBuffer)
{
    osc.ChangeState(1);
    m_durationHandler.SetDuration(m_pParams->RTPC.fDuration);
    m_durationHandler.ProduceBuffer(out_pBuffer);

    const AkUInt32 uNumChannels = out_pBuffer->NumChannels();

    AkUInt16 uFramesProduced;
    for (AkUInt32 i = 0; i < uNumChannels; ++i)
    {
        AkReal32* AK_RESTRICT pBuf = (AkReal32* AK_RESTRICT)out_pBuffer->GetChannel(i);

        uFramesProduced = 0;
        while (uFramesProduced < out_pBuffer->uValidFrames)
        {
            // Generate output here
            *pBuf++ = osc.ProcessAudioFrame();
            ++uFramesProduced;
        }
    }
}

AkReal32 oscillatorSource::GetDuration() const
{
    return m_durationHandler.GetDuration() * 1000.0f;
}
