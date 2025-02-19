#include "ffmpeg.h"
#include "muxer.h"

Muxer::Muxer(const int bytes_per_sample)
{
	this->bytes_per_sample = bytes_per_sample;
}

void Muxer::frame_to_pcm(AVFrame* frame)
{
	const auto format = static_cast<AVSampleFormat>(frame->format);
	const int nb_samples = frame->nb_samples;
	uint8_t** data = frame->data;

	this->pcm_buffer = new char[2ll * nb_samples * this->bytes_per_sample];

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
		default:
			return;
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
		default:
			return;
		}
	}
	av_frame_unref(frame);
}

template <typename T>
void Muxer::planar(uint8_t** data, const AVSampleFormat format, const int nb_samples) const
{
	int bytes_per_data_sample = av_get_bytes_per_sample(format);

	T* samples = new T[2ll * nb_samples];
	for (int i = 0; i < nb_samples; i++)
	{
		samples[2ll * i] = reinterpret_cast<T*>(data[0])[i];
		samples[2 * i + 1] = reinterpret_cast<T*>(data[1])[i];
	}

	packed<T>(reinterpret_cast<uint8_t**>(&samples), format, nb_samples);
	delete[] samples;
}

template <typename T>
void Muxer::packed(uint8_t** data, const AVSampleFormat format, const int nb_samples) const
{

	int bytes_per_data_sample = av_get_bytes_per_sample(format);

	if (this->bytes_per_sample == bytes_per_data_sample)
	{
		memcpy(this->pcm_buffer, *data, 2 * sizeof(T) * nb_samples);
	}
	else
	{
		const int shift = (bytes_per_data_sample - this->bytes_per_sample) * 8;

		for (int i = 0; i < 2 * nb_samples; i++)
		{
			for (int j = 0; j < this->bytes_per_sample; j++)
			{
				this->pcm_buffer[this->bytes_per_sample * i + j] =
					static_cast<char>((reinterpret_cast<T*>(*data)[i] >> (8 * j + shift)) & 0xFF);
			}
		}
	}
}

void Muxer::pcm_buffer_free() const
{
	delete[] this->pcm_buffer;
}