#include "DirectOscillator.h"

DirectOscillator::DirectOscillator(int sampleRate)
{
	m_nSampleRate = sampleRate;
	m_bRunOscillator = false;
	m_f_b1 = 0;
	m_f_b2 = 0;

	m_f_y_z1 = 0;
	m_f_y_z2 = 0;

	CookFrequency();
}

void DirectOscillator::CookFrequency()
{
	float f_wT = (2.0 * M_PI * m_fFrequency_Hz) / (float)m_nSampleRate;
	m_f_b1 = -2.0 * cos(f_wT);
	m_f_b2 = 1.0;

	m_f_y_z1 = sin(-1.0 * f_wT);
	m_f_y_z2 = sin(-2.0 * f_wT);
}

void DirectOscillator::PrepareToPlay()
{
	CookFrequency();
}

float DirectOscillator::ProcessAudioFrame()
{
	float output = 0;
	if (!m_bRunOscillator)
	{
		return output;
	}

	output = -m_f_b1 * m_f_y_z1 - m_f_b2 * m_f_y_z2;
	m_f_y_z2 = m_f_y_z1;
	m_f_y_z1 = output;

	return output;
}

void DirectOscillator::ChangeState(int controlIndex)
{
	switch (controlIndex)
	{
	case 0:
		CookFrequency();
		break;
	case 1:
		m_bRunOscillator = true;
		break;
	case 2:
		m_bRunOscillator = false;
		break;
	default:
		break;
	}
}
