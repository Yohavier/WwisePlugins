#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <AK/SoundEngine/Common/IAkPlugin.h>

enum OSCType
{ 
	sine,
	square,
	triangle,
	saw
};

enum Polarity
{
	bipolar,
	unipolar
};

class WTO
{
public:
	WTO() = default;
	~WTO();
	WTO(int sampleRate);

	float ProcessAudioFrame(int channelIndex);
	void ChangePlayingState(int state);
	void ChangeParameters(float fFrequency, float fOscillationType, float fPolarity, bool bBandlimit);
	
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


	float m_fFrequencyHz;
	OSCType m_OscillatioType;
	Polarity m_Polarity;
	bool m_bBandlimit;
};

