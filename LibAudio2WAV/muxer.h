#pragma once
#include "ffmpeg.h"

class muxer
{
public:
	muxer(int sample_rate, int bit_per_sample);
	char* frame_to_pcm(AVFrame* frame);
private:
	int sample_rate;
	int bits_per_sample;
	int bytes_per_sample;

	int nb_samples_total = 0;

	template <typename T>
	char* planar(uint8_t** data, int format, int nb_samples);

	template <typename T>
	char* packed(uint8_t** data, int format, int nb_samples);
};