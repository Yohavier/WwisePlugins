#include "WTO.h"
WTO::WTO(int sampleRate)
{
	m_fReadIndex = 0;
	m_fQuadPhaseReadIndex = 0.0;
	m_f_inc = 0;
	m_bNoteOn = false;
	m_nSampleRate = sampleRate;
	m_bInvert = false;
	//Triangle rise 1
	float mt1 = 1.0 / 256.0;
	float bt1 = 0.0;

	//Triangle rise 2
	float mt2 = 1.0 / 256.0;
	float bt2 = -1.0;

	//Triangle falling
	float mtf2 = -2.0 / 512.0;
	float btf2 = 1.0;


	//Sawtooth rise 1
	float ms1 = 1.0 / 512.0;
	float bs1 = 0.0;

	//Sawtooth rise 2
	float ms2 = 1.0 / 512.0;
	float bs2 = -1.0;

	float fMaxTri = 0;
	float fMaxSqr = 0;
	float fMaxSaw = 0;

	m_SinArray = new float[1024];
	m_TriangleArray = new float[1024];
	m_SawToothArray = new float[1024];
	m_SquareArray = new float[1024];

	m_SawToothArray_BL5 = new float[1024];
	m_SquareArray_BL5 = new float[1024];
	m_TriangleArray_BL5 = new float[1024];

	for (int i = 0; i < 1024; i++)
	{
		m_SinArray[i] = sin(((float)i / 1024.0) * (2 * M_PI));

		if (i == 1)
			m_SquareArray[i] = 0.0;
		else
			m_SquareArray[i] = i < 512 ? 1.0 : -1.0;
		
		m_SawToothArray_BL5[i] = 0.0;
		m_SquareArray_BL5[i] = 0.0;
		m_TriangleArray_BL5[i] = 0.0;

		for (int g = 1; g < 6; g++)
		{
			double n = double(g);
			m_SawToothArray_BL5[i] += pow((float)-1.0, (float)(g + 1)) * (1.0 / n) * sin(2.0 * M_PI * i * n / 1024.0);
		}

		for (int g = 0; g == 3; g++)
		{
			double n = double(g);
			m_TriangleArray_BL5[i] += pow((float)-1.0, (float)n) * (1.0 / pow((float)(2 * n + 1), (float)2.0)) * sin(2.0 * M_PI * (2.0 * n + 1) * i / 1024);
		}

		for (int g = 1; g <= 5; g += 2)
		{
			double n = double(g);
			m_SquareArray_BL5[i] += (1.0 / n) * sin(2.0 * M_PI * i * n / 1024.0);
		}
		
		if (i == 0)
		{
			fMaxSaw = m_SawToothArray_BL5[i];
			fMaxSqr = m_SquareArray_BL5[i];
			fMaxTri = m_TriangleArray_BL5[i];
		}
		else
		{
			if (m_SawToothArray_BL5[i] > fMaxSaw)
				fMaxSaw = m_SawToothArray_BL5[i];

			if (m_TriangleArray_BL5[i] > fMaxTri)
				fMaxTri = m_TriangleArray_BL5[i];

			if (m_SquareArray_BL5[i] > fMaxSqr)
				fMaxSqr = m_SquareArray_BL5[i];
		}


		m_SawToothArray[i] = i < 512 ? ms1 * i + bs1 : ms2 * (i - 511) + bs2;

		if (i < 256)
		{
			m_TriangleArray[i] = mt1 * i + bt1;
		}
		else if (i < 256 && i < 768)
		{
			m_TriangleArray[i] = mtf2 * (i - 256) + btf2;
		}
		else
		{
			m_TriangleArray[i] = mt2 * (i - 768) + bt2;
		}
	}

	for (int i = 0; i < 1024; i++)
	{
		m_SawToothArray_BL5[i] /= fMaxSaw;
		m_SquareArray_BL5[i] /= fMaxSqr;
		m_TriangleArray_BL5[i] /= fMaxTri;
	}

	CookFrequency();
}

void WTO::CookFrequency()
{
	m_f_inc = 1024 * m_Frequency_Hz / (float)m_nSampleRate;
}

void WTO::PrepareForPlay()
{
	Reset();
	CookFrequency();
}

float WTO::ProcessAudioFrame(int channelIndex)
{
	float fOutSample = 0;
	float fQuadSample = 0;

	if (!m_bNoteOn)
	{
		return fOutSample;
	}

	int nReadIndex = (int)m_fReadIndex;
	int nQuadPhaseReadIndex = (int)m_fQuadPhaseReadIndex;

	float fFrac = m_fReadIndex - nReadIndex;

	int nReadIndexNext = nReadIndex + 1 > 1023 ? 0 : nReadIndex + 1;
	int nQuadPhaseReadIndexNext = nQuadPhaseReadIndex + 1 > 1023 ? 0 : nQuadPhaseReadIndex + 1;

	switch (oscType)
	{
		case OSCType::saw:
			if (oscTableMode == normal)
			{
				fOutSample = LinearInterpolation(0, 1, m_SawToothArray[nReadIndex], m_SawToothArray[nReadIndexNext], fFrac);
				fQuadSample = LinearInterpolation(0, 1, m_SawToothArray[nQuadPhaseReadIndex], m_SawToothArray[nQuadPhaseReadIndexNext], fFrac);
			}
			else
			{
				fOutSample = LinearInterpolation(0, 1, m_SawToothArray_BL5[nReadIndex], m_SawToothArray_BL5[nReadIndexNext], fFrac);
				fQuadSample = LinearInterpolation(0, 1, m_SawToothArray_BL5[nQuadPhaseReadIndex], m_SawToothArray_BL5[nQuadPhaseReadIndexNext], fFrac);
			}
			break;

		case OSCType::sine:
			fOutSample = LinearInterpolation(0, 1, m_SinArray[nReadIndex], m_SinArray[nReadIndexNext], fFrac);
			fQuadSample = LinearInterpolation(0, 1, m_SinArray[nQuadPhaseReadIndex], m_SinArray[nQuadPhaseReadIndexNext], fFrac);
			break;

		case OSCType::square:
			if (oscTableMode == normal)
			{
				fOutSample = LinearInterpolation(0, 1, m_SquareArray[nReadIndex], m_SquareArray[nReadIndexNext], fFrac);
				fQuadSample = LinearInterpolation(0, 1, m_SquareArray[nQuadPhaseReadIndex], m_SquareArray[nQuadPhaseReadIndexNext], fFrac);
			}
			else
			{
				fOutSample = LinearInterpolation(0, 1, m_SquareArray_BL5[nReadIndex], m_SquareArray_BL5[nReadIndexNext], fFrac);
				fQuadSample = LinearInterpolation(0, 1, m_SquareArray_BL5[nQuadPhaseReadIndex], m_SquareArray_BL5[nQuadPhaseReadIndexNext], fFrac);
			}
			break;

		case OSCType::triangle:
			if (oscTableMode == normal)
			{
				fOutSample = LinearInterpolation(0, 1, m_TriangleArray[nReadIndex], m_TriangleArray[nReadIndexNext], fFrac);
				fQuadSample = LinearInterpolation(0, 1, m_TriangleArray[nQuadPhaseReadIndex], m_TriangleArray[nQuadPhaseReadIndexNext], fFrac);
			}
			else
			{
				fOutSample = LinearInterpolation(0, 1, m_TriangleArray_BL5[nReadIndex], m_TriangleArray_BL5[nReadIndexNext], fFrac);
				fQuadSample = LinearInterpolation(0, 1, m_TriangleArray_BL5[nReadIndex], m_TriangleArray_BL5[nReadIndexNext], fFrac);
			}		
			break;

		default:
			fOutSample = LinearInterpolation(0, 1, m_SinArray[nReadIndex], m_SinArray[nReadIndexNext], fFrac);
			fQuadSample = LinearInterpolation(0, 1, m_SinArray[nReadIndex], m_SinArray[nReadIndexNext], fFrac);
			break;
	}
	
	m_fReadIndex += m_f_inc;
	m_fQuadPhaseReadIndex += m_f_inc;

	if (m_fReadIndex > 1024)
		m_fReadIndex = m_fReadIndex - 1024;

	if (m_fQuadPhaseReadIndex > 1024)
		m_fQuadPhaseReadIndex = m_fQuadPhaseReadIndex - 1024;

	if (channelIndex == 0)
		return fOutSample;
	else
		return fQuadSample;
}

void WTO::ChangeState(int state)
{
	switch (state)
	{
	case 0:
		CookFrequency();
		break;
	case 1:
		Reset();
		CookFrequency();
		m_bNoteOn = true;
		break;
	case 2:
		m_bNoteOn = false;
		break;
	default:
		break;
	}
}

float inline WTO::LinearInterpolation(float x0, float  x1, float  y0, float y1, float xp)
{
	return y0 + ((y1 - y0) / (x1 - x0)) * (xp - x0);
}

WTO::~WTO()
{
	delete m_SawToothArray;
	delete m_SquareArray;
	delete m_TriangleArray;
	delete m_SinArray;
}