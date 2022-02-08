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
	m_fWetLevel = wetLevel / 100;
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
	float xn = frame;
	float yn = m_pBuffer[m_nReadIndex];

	if (m_nReadIndex == m_nWriteIndex && m_fDelayInSamples < 1.00f)
	{
		yn = xn;
	}
	int nReadIndex_1 = m_nReadIndex - 1;
	if (nReadIndex_1 < 0)
	{
		nReadIndex_1 = m_nBufferSize - 1;
	}
	float yn_1 = m_pBuffer[nReadIndex_1];

	float fFracDelay = m_fDelayInSamples - (int)m_fDelayInSamples;
	float fInterp = LinearInterpolation(0, 1, yn, yn_1, fFracDelay);
	
	if (m_fDelayInSamples == 0)
	{
		yn = xn;
	}
	else
	{
		yn = fInterp;
	}

	m_pBuffer[m_nWriteIndex] = xn + m_fFeedback * yn;


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

	return m_fWetLevel * yn + (1.0 - m_fWetLevel) * xn;
}

float inline Delay::LinearInterpolation(float y0, float  x0, float  y1, float x1, float xp)
{
	return y0 + ((y1 - y0) / (x1 - x0)) * (xp - x0);
}