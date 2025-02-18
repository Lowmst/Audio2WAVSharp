#pragma once
#include "ffmpeg.h"


class Muxer
{
public:

	Muxer(int bytes_per_sample);
	void frame_to_pcm(AVFrame* frame);
	char* pcm_buffer;

	void pcm_buffer_free();

private:

	int bytes_per_sample;


	template <typename T>
	void planar(uint8_t** data, AVSampleFormat format, int nb_samples);

	template <typename T>
	void packed(uint8_t** data, AVSampleFormat format, int nb_samples);

};