#include "Delay.h"

Delay::~Delay()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
	}
}

void Delay::CookVariables(float wetLevel, float feedback, float delay)
{
	m_fFeedback = feedback / 100.0;
	m_fWetLevel = wetLevel / 100.0;
	m_fDelayInSamples = delay * ((float)m_nSampleRate / 1000.0);


	m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples;

	if (m_nReadIndex < 0)
	{
		m_nReadIndex += m_nBufferSize;
	}
}

void Delay::PrepareToPlay(float wetLevel, float feedback, float delay)
{
	m_nBufferSize = 2 * m_nSampleRate;

	if (m_pBuffer)
	{
		delete[] m_pBuffer;
	}
	m_pBuffer = new float[m_nBufferSize];

	ResetDelay();
	CookVariables(wetLevel, feedback, delay);
}

void Delay::ResetDelay()
{
	if (m_pBuffer)
	{
		memset(m_pBuffer, 0, m_nBufferSize * sizeof(float));
	}	

	m_nWriteIndex = 0;
	m_nReadIndex = 0;
}

AkReal32 Delay::ProcessAudioFrame(AkReal32 frame, int uNumInputChannels, int uNumOutputChannels)
{
	float yn = m_pBuffer[m_nReadIndex];

	if (m_fDelayInSamples == 0)
	{
		yn = frame;
	}

	m_pBuffer[m_nWriteIndex] = frame;
	
	m_nWriteIndex++;
	if (m_nWriteIndex >= m_nBufferSize)
	{
		m_nWriteIndex = 0;
	}

	m_nReadIndex++;
	if(m_nReadIndex >= m_nBufferSize)
	{
		m_nReadIndex = 0;
	}

	return yn;
}
