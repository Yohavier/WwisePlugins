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

enum TableMode
{
	normal,
	bandlimit,
};

enum Polarity
{
	Bipolar,
	Unipolar
};

class WTO
{
public:
	WTO() = default;
	~WTO();
	WTO(int sampleRate);
	void PrepareForPlay();
	float ProcessAudioFrame(int channelIndex);
	void ChangeState(int state);
	float m_Frequency_Hz = 200;
	int m_nSampleRate;

private:
	void CookFrequency();
	float inline LinearInterpolation(float y0, float  x0, float  y1, float x1, float xp);

	void Reset()
	{
		m_fReadIndex = 0;
		m_fQuadPhaseReadIndex = 256.0;
	}

	//normal tables
	float* m_SinArray;
	float* m_SawToothArray;
	float* m_TriangleArray;
	float* m_SquareArray;

	//band limited tables to 5 partials
	float* m_SawToothArray_BL5;
	float* m_TriangleArray_BL5;
	float* m_SquareArray_BL5;

	float m_fReadIndex;
	float m_fQuadPhaseReadIndex;

	float m_f_inc;

	bool m_bNoteOn =true;
	bool m_bInvert;

	OSCType oscType = sine;
	TableMode oscTableMode = normal;
	Polarity pol = Bipolar;
};

