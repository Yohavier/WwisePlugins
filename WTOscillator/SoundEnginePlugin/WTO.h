#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

enum OSCType
{ 
	sine,
	square,
	triangle,
	saw,
};

class WTO
{
public:
	WTO() = default;
	~WTO();
	WTO(int sampleRate);
	void PrepareForPlay();
	float ProcessAudioFrame();
	void ChangeState(int state);
	float m_Frequency_Hz = 200;
	int m_nSampleRate;

private:
	void CookFrequency();
	float inline LinearInterpolation(float y0, float  x0, float  y1, float x1, float xp);

	void Reset()
	{
		m_fReadIndex = 0;
	}

	float* m_SinArray;
	float* m_SawToothArray;
	float* m_TriangleArray;
	float* m_SquareArray;

	float m_fReadIndex;

	float m_f_inc;

	bool m_bNoteOn =true;

	OSCType oscType = saw;
};

