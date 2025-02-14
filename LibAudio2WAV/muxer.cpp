#include "ffmpeg.h"
#include "muxer.h"

muxer::muxer(int sample_rate, int bits_per_sample)
{
	this->sample_rate = sample_rate;
	this->bits_per_sample = bits_per_sample;
	this->bytes_per_sample = (int)ceil((float)bits_per_sample / 8);
}

char* muxer::frame_to_pcm(AVFrame* frame)
{
	int format = frame->format;
	int nb_samples = frame->nb_samples;
	uint8_t** data = frame->data;

	if (av_sample_fmt_is_planar(AVSampleFormat(format)))
	{
		switch (format)
		{
		case AV_SAMPLE_FMT_U8P:
			return planar<uint8_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S16P:
			return planar<int16_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S32P:
			return planar<int32_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S64P:
			return planar<int64_t>(data, format, nb_samples);
			break;
		}
	}
	else
	{
		switch (format)
		{
		case AV_SAMPLE_FMT_U8:
			return packed<uint8_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S16:
			return packed<int16_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S32:
			return packed<int32_t>(data, format, nb_samples);
			break;
		case AV_SAMPLE_FMT_S64:
			return packed<int64_t>(data, format, nb_samples);
			break;
		}
	}
}

template <typename T>
char* muxer::planar(uint8_t** data, int format, int nb_samples)
{
	this->nb_samples_total += 2 * nb_samples;
	int bytes_per_data_sample = av_get_bytes_per_sample(AVSampleFormat(format));


	if (this->bytes_per_sample == bytes_per_data_sample)
	{
		this->file.write((char*)data[0], 2 * sizeof(T) * frame_nb_samples);
	}
	else
	{
		char* bytes = new char[2 * frame_nb_samples * this->bytes_per_sample];
		int shift = (bytes_per_data_sample - this->bytes_per_sample) * 8;

		for (int i = 0; i < 2 * frame_nb_samples; i++)
		{
			for (int j = 0; j < this->bytes_per_sample; j++)
			{
				bytes[this->bytes_per_sample * i + j] = (char)((((T*)data[0])[i] >> (8 * j + shift)) & 0xFF);
			}
		}

		return bytes;
	}
}
template <typename T>
char* muxer::packed(uint8_t** data, int format, int nb_samples)
{

}