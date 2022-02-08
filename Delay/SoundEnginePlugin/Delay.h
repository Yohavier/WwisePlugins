#pragma once
#include <string.h>
#include <AK/SoundEngine/Common/IAkPlugin.h>

class Delay
{
public:
	Delay() = default;

	Delay(AkUInt32 sampleRate)
		: m_fDelayInSamples(0.0f)
		, m_fFeedback(0.0f)
		, m_fWetLevel(0.0f)
		, m_nSampleRate(sampleRate)
		, m_pBuffer(nullptr)
		, m_nBufferSize(2 * sampleRate)
		, m_nReadIndex(0)
		, m_nWriteIndex(0)
	{}

	~Delay();
	void PrepareToPlay(float wetLevel, float feedback, float delay);
	AkReal32 ProcessAudioFrame(AkReal32 io_pBuffer, int uNumInputChannels, int uNumOutputChannels);


private:
	void CookVariables(float wetLevel, float feedback, float delay);
	void ResetDelay();
	
private:
	AkUInt32 m_nSampleRate;

	float m_fDelayInSamples;
	float m_fFeedback;
	float m_fWetLevel;

	float* m_pBuffer;
	int m_nReadIndex;
	int m_nWriteIndex;
	int m_nBufferSize;

	float output = 0;
};

