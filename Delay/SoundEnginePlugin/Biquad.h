#pragma once
#include "AK/SoundEngine/Common/AkCommonDefs.h"

#define _USE_MATH_DEFINES
#include <cmath>

enum BiquadType
{
	None,
	Lowpass,
	Highpass,
	Peak,
	Bandpass,
	Notch,
	Lowshelf,
	Highshelf,
};

class Biquad
{
public:

	~Biquad() = default;

	Biquad(const float sampleRate, const BiquadType bTypeParam)
		:m_sampleRate(sampleRate)
		, type(bTypeParam)
		, in_z1(0.0f)
		, in_z2(0.0f)
		, coeff_a0(0.0f)
		, coeff_a1(0.0f)
		, coeff_a2(0.0f)
		, coeff_b1(0.0f)
		, coeff_b2(0.0f)
	{}

	// if bTypeParam is AkInt32 weird values come out
	void ComputeCoeff(const AkReal32 frequency, const AkReal32 qualityFactor, const AkReal32 peakGain);
	AkReal32 ProcessSample(AkReal32 const sample);

private:
	float m_sampleRate;
	BiquadType type;

	AkReal32 in_z1;
	AkReal32 in_z2;

	AkReal32 coeff_a0;
	AkReal32 coeff_a1;
	AkReal32 coeff_a2;
	AkReal32 coeff_b1;
	AkReal32 coeff_b2;
};