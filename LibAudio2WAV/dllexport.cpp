import std;

#include "dllexport.h"
#include "ffmpeg.h"
#include "muxer.h"

namespace
{
	Muxer* muxer;

	AVFormatContext* format_context = avformat_alloc_context();

	AVPacket* packet = av_packet_alloc();
	AVFrame* frame = av_frame_alloc();

	const AVCodec* codec;
	AVCodecParameters* codec_parameters;
	AVCodecContext* codec_context;

	int audio_stream_index;
	int bits_per_sample;
	int bytes_per_sample;


	dll WAVParameters init(const char* url)
	{
		avformat_open_input(&format_context, url, nullptr, nullptr);

		avformat_find_stream_info(format_context, nullptr);

		for (int i = 0; i < format_context->nb_streams; i++)
		{
			if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				audio_stream_index = i;
			}
		}

		codec_parameters = format_context->streams[audio_stream_index]->codecpar;

		bits_per_sample = codec_parameters->bits_per_raw_sample ?
			codec_parameters->bits_per_raw_sample : codec_parameters->bits_per_coded_sample;
		bytes_per_sample = std::ceil(static_cast<float>(bits_per_sample) / 8);

		codec = avcodec_find_decoder(codec_parameters->codec_id);
		codec_context = avcodec_alloc_context3(codec);

		avcodec_parameters_to_context(codec_context, codec_parameters);
		avcodec_open2(codec_context, codec, nullptr);

		muxer = new Muxer(bytes_per_sample);

		return WAVParameters{ codec_parameters->sample_rate, bits_per_sample };
	}

	dll PCMPacket decode()
	{
		if (avcodec_receive_frame(codec_context, frame) == 0)
		{
			const int size = 2 * bytes_per_sample * frame->nb_samples;
			muxer->frame_to_pcm(frame);
			return PCMPacket{ size, muxer->pcm_buffer };
		}

		av_packet_unref(packet);
		while (av_read_frame(format_context, packet) == 0)
		{
			if (packet->stream_index == audio_stream_index)
			{
				avcodec_send_packet(codec_context, packet);
				if (avcodec_receive_frame(codec_context, frame) == 0)
				{
					const int size = 2 * bytes_per_sample * frame->nb_samples;
					muxer->frame_to_pcm(frame);
					return PCMPacket{ size, muxer->pcm_buffer };
				}
			}
		}
		return PCMPacket{ 0, nullptr };
	}

	dll void free_buffer()
	{
		muxer->pcm_buffer_free();
	}
}