#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
class DirectOscillator
{
public:
	DirectOscillator() = default;
	DirectOscillator(int sampleRate);
	void PrepareToPlay();
	float ProcessAudioFrame();
	void ChangeState(int controlIndex);

private:

	float m_f_b1;
	float m_f_b2;

	float m_f_y_z1;
	float m_f_y_z2;

	float m_fFrequency_Hz = 700;
	int m_nSampleRate;

	bool m_bRunOscillator;

private:
	void CookFrequency();
};

