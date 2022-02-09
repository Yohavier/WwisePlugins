#include "SimpleConvolution.h"

SimpleConvolution::SimpleConvolution()
{
	m_nIRLenght = 5;

	m_pBuffer = new float[m_nIRLenght];
	m_h = new float[m_nIRLenght];

	memset(m_pBuffer, 0, m_nIRLenght * sizeof(float));

	memset(m_h, 0, m_nIRLenght * sizeof(float));

	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;

	m_h[0] = 0.1;
	m_h[1] = 0.1;
	m_h[2] = 0.1;
	m_h[3] = 0.1;
	m_h[4] = 0.1;
}

SimpleConvolution::~SimpleConvolution() 
{
	//Why heap corruption error
	//if(m_pBuffer)
		//delete[] m_pBuffer;
	//if(m_h)
		//delete[] m_h;
}

void SimpleConvolution::PrepareToPlay() 
{
	memset(m_pBuffer, 0, m_nIRLenght * sizeof(float));

	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;
}

AkReal32 SimpleConvolution::ProcessAudioFrame(AkReal32 frame)
{
	float xn = frame;
	m_pBuffer[m_nWriteIndex] = xn;
	m_nReadIndexDL = m_nWriteIndex;
	m_nReadIndexH = 0;
	float yn_accum = 0;

	for (int i = 0; i < m_nIRLenght; i++)
	{
		yn_accum += m_pBuffer[m_nReadIndexDL] * m_h[m_nReadIndexH];
		m_nReadIndexH++;
		m_nReadIndexDL--;

		if (m_nReadIndexDL < 0)
		{
			m_nReadIndexDL = m_nIRLenght - 1;
		}
	}
	m_nWriteIndex++;
	if (m_nWriteIndex >= m_nIRLenght)
	{
		m_nWriteIndex = 0;
	}
	return yn_accum;
}