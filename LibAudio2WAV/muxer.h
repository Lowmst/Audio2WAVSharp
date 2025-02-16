#pragma once
#include "ffmpeg.h"

class Muxer
{
public:
	Muxer(int sample_rate, int bits_per_sample);
	void frame_to_pcm(AVFrame* frame);
	char* pcm_buffer;

	void pcm_buffer_free();
private:
	int sample_rate;
	int bits_per_sample;
	int bytes_per_sample;

	int nb_samples_total = 0;

	

	template <typename T>
	void planar(uint8_t** data, AVSampleFormat format, int nb_samples);

	template <typename T>
	void packed(uint8_t** data, int format, int nb_samples);

	
};