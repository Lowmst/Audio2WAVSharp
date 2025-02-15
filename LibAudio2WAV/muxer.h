#pragma once
#include "ffmpeg.h"

class Muxer
{
public:
	Muxer(int sample_rate, int bit_per_sample);
	void frame_to_pcm(AVFrame* frame);
private:
	int sample_rate;
	int bits_per_sample;
	int bytes_per_sample;

	int nb_samples_total = 0;

	char* pcm_buffer;

	template <typename T>
	void planar(uint8_t** data, int format, int nb_samples);

	template <typename T>
	void packed(uint8_t** data, int format, int nb_samples);

	void pcm_buffer_free();
};