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

#include "WTOscillatorSourceParams.h"

#include <AK/Tools/Common/AkBankReadHelpers.h>

WTOscillatorSourceParams::WTOscillatorSourceParams()
{
}

WTOscillatorSourceParams::~WTOscillatorSourceParams()
{
}

WTOscillatorSourceParams::WTOscillatorSourceParams(const WTOscillatorSourceParams& in_rParams)
{
    RTPC = in_rParams.RTPC;
    NonRTPC = in_rParams.NonRTPC;
    m_paramChangeHandler.SetAllParamChanges();
}

AK::IAkPluginParam* WTOscillatorSourceParams::Clone(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, WTOscillatorSourceParams(*this));
}

AKRESULT WTOscillatorSourceParams::Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    if (in_ulBlockSize == 0)
    {
        // Initialize default parameters here
        RTPC.m_fDuration = 0.0f;
        RTPC.m_bBandlimit = false;
        RTPC.m_fPolarity = 0;
        RTPC.m_fOscillationType = 0;
        RTPC.m_fFrequency = 0.1;
        m_paramChangeHandler.SetAllParamChanges();
        return AK_Success;
    }

    return SetParamsBlock(in_pParamsBlock, in_ulBlockSize);
}

AKRESULT WTOscillatorSourceParams::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT WTOscillatorSourceParams::SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    AKRESULT eResult = AK_Success;
    AkUInt8* pParamsBlock = (AkUInt8*)in_pParamsBlock;

    // Read bank data here
    RTPC.m_fDuration = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.m_bBandlimit = READBANKDATA(bool, pParamsBlock, in_ulBlockSize);
    RTPC.m_fFrequency = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.m_fOscillationType = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.m_fPolarity = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    CHECKBANKDATASIZE(in_ulBlockSize, eResult);
    m_paramChangeHandler.SetAllParamChanges();

    return eResult;
}

AKRESULT WTOscillatorSourceParams::SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize)
{
    AKRESULT eResult = AK_Success;

    // Handle parameter change here
    switch (in_paramID)
    {
    case PARAM_DURATION_ID:
        RTPC.m_fDuration = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_DURATION_ID);
        break;
    case PARAM_OSCILLATIONTYPE_ID:
        RTPC.m_fOscillationType = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_OSCILLATIONTYPE_ID);
        break;
    case PARAM_POLARITY_ID:
        RTPC.m_fPolarity = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_POLARITY_ID);
        break;
    case PARAM_BANDLIMIT_ID:
        RTPC.m_bBandlimit = *((bool*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_BANDLIMIT_ID);
        break;
    case PARAM_FREQUENCY_ID:    
        RTPC.m_fFrequency = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_FREQUENCY_ID);
        break;
    default:
        eResult = AK_InvalidParameter;
        break;
    }

    return eResult;
}
