#pragma once
#include <string.h>
#include <AK/SoundEngine/Common/IAkPlugin.h>

enum ConvolutionType {
	MovingAverageInterpolator,
	MedianFilter,
};

class SimpleConvolution
{
public:
	SimpleConvolution();
	~SimpleConvolution();
	void PrepareToPlay();
	AkReal32 ProcessAudioFrame(AkReal32 frame);

	float* m_pBuffer;
	float* m_h;

	int m_nReadIndexDL;
	int m_nReadIndexH;
	int m_nWriteIndex;

	int m_nIRLenght;
};

