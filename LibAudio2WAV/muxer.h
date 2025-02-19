#pragma once
#include "ffmpeg.h"


class Muxer
{
public:

	explicit Muxer(int bytes_per_sample);
	void frame_to_pcm(AVFrame* frame);
	void pcm_buffer_free() const;
	char* pcm_buffer = nullptr;

private:

	int bytes_per_sample;

	template <typename T>
	void planar(uint8_t** data, AVSampleFormat format, int nb_samples) const;

	template <typename T>
	void packed(uint8_t** data, AVSampleFormat format, int nb_samples) const;

};