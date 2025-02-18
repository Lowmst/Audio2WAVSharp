#include "ffmpeg.h"
#include "muxer.h"

Muxer::Muxer(int bytes_per_sample)
{
	this->bytes_per_sample = bytes_per_sample;
}

void Muxer::frame_to_pcm(AVFrame* frame)
{
	AVSampleFormat format = static_cast<AVSampleFormat>(frame->format);
	int nb_samples = frame->nb_samples;
	uint8_t** data = frame->data;

	if (av_sample_fmt_is_planar(format))
	{
		switch (format)
		{
		case AV_SAMPLE_FMT_U8P:
			planar<uint8_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S16P:
			planar<int16_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S32P:
			planar<int32_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S64P:
			planar<int64_t>(data, format, nb_samples);
			break;
		}
	}
	else
	{
		switch (format)
		{
		case AV_SAMPLE_FMT_U8:
			packed<uint8_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S16:
			packed<int16_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S32:
			packed<int32_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S64:
			packed<int64_t>(data, format, nb_samples);
			break;
		}
	}
	av_frame_unref(frame);
}

template <typename T>
void Muxer::planar(uint8_t** data, AVSampleFormat format, int nb_samples)
{

	int bytes_per_data_sample = av_get_bytes_per_sample(format);

	this->pcm_buffer = new char[2 * nb_samples * this->bytes_per_sample];

	T* samples = new T[2 * nb_samples];
	for (int i = 0; i < nb_samples; i++)
	{
		samples[2 * i] = ((T*)data[0])[i];
		samples[2 * i + 1] = ((T*)data[1])[i];
	}

	if (this->bytes_per_sample == bytes_per_data_sample)
	{
		//this->file.write((char*)samples, 2 * sizeof(T) * frame_nb_samples);
		memcpy(this->pcm_buffer, samples, 2 * sizeof(T) * nb_samples);
	}
	else
	{
		int shift = (bytes_per_data_sample - this->bytes_per_sample) * 8;

		for (int i = 0; i < 2 * nb_samples; i++)
		{
			for (int j = 0; j < this->bytes_per_sample; j++)
			{
				this->pcm_buffer[this->bytes_per_sample * i + j] = (char)((samples[i] >> (8 * j + shift)) & 0xFF);
			}
		}

		//this->file.write(bytes, 2 * frame_nb_samples * this->bytes_per_sample);
		//delete[] bytes;
	}
	delete[] samples;
}


template <typename T>
void Muxer::packed(uint8_t** data, AVSampleFormat format, int nb_samples)
{

	int bytes_per_data_sample = av_get_bytes_per_sample(format);

	this->pcm_buffer = new char[2 * nb_samples * this->bytes_per_sample];

	if (this->bytes_per_sample == bytes_per_data_sample)
	{
		//this->file.write((char*)data[0], 2 * sizeof(T) * frame_nb_samples);
		memcpy(this->pcm_buffer, data[0], 2 * sizeof(T) * nb_samples);
	}
	else
	{
		//this->pcm_buffer = new char[2 * frame_nb_samples * this->bytes_per_sample];
		int shift = (bytes_per_data_sample - this->bytes_per_sample) * 8;

		for (int i = 0; i < 2 * nb_samples; i++)
		{
			for (int j = 0; j < this->bytes_per_sample; j++)
			{
				this->pcm_buffer[this->bytes_per_sample * i + j] = (char)((((T*)data[0])[i] >> (8 * j + shift)) & 0xFF);
			}
		}
	}
}

void Muxer::pcm_buffer_free()
{
	delete[] this->pcm_buffer;
}