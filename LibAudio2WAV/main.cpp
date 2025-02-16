//#include <chrono>
#include <filesystem>
//#include <iostream>
#include <print>
//#include <vector>

#include "ffmpeg.h"
#include "wav.h"
#include "muxer.h"
#include <windows.h>

typedef struct
{
	int sample_rate;
	int bits_per_sample;
}audio_info;

typedef struct
{
	int buffer_size;
	char* data;
}pcm;

Muxer* muxer;


AVFormatContext* format_context = avformat_alloc_context();

AVPacket* packet = av_packet_alloc();
AVFrame* frame = av_frame_alloc();

const AVCodec* codec;
AVCodecParameters* codec_parameters;
AVCodecContext* codec_context;

int audio_stream_index;
int bits_per_sample;
std::filesystem::path filename;

extern "C"
{
	__declspec(dllexport) audio_info init(char* filepath)
	{
		audio_info info;

		avformat_open_input(&format_context, filepath, NULL, NULL);
		avformat_find_stream_info(format_context, NULL);

		for (unsigned int i = 0; i < format_context->nb_streams; i++)
		{
			std::println("第 {} 轨媒体类型: {} , 编码格式: {}",
				i, av_get_media_type_string(format_context->streams[i]->codecpar->codec_type), avcodec_get_name(format_context->streams[i]->codecpar->codec_id));
			if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				audio_stream_index = i;
			}
		}
		codec_parameters = format_context->streams[audio_stream_index]->codecpar;
		codec = avcodec_find_decoder(codec_parameters->codec_id);
		codec_context = avcodec_alloc_context3(codec);

		avcodec_parameters_to_context(codec_context, codec_parameters);
		avcodec_open2(codec_context, codec, NULL);

		filename = std::filesystem::path(reinterpret_cast<char8_t*>(filepath)).filename().stem();
		filename += std::filesystem::path(".wav");

		bits_per_sample = codec_parameters->bits_per_raw_sample ?
			codec_parameters->bits_per_raw_sample : codec_parameters->bits_per_coded_sample;

		info.sample_rate = codec_parameters->sample_rate;
		info.bits_per_sample = bits_per_sample;

		muxer = new Muxer(codec_parameters->sample_rate, bits_per_sample);

		return info;
	}

	__declspec(dllexport) void decode_wav()
	{
		wav_output wav(filename, codec_parameters->sample_rate, bits_per_sample);
		while (av_read_frame(format_context, packet) == 0) // 循环读数据包
		{
			if (packet->stream_index == audio_stream_index)
			{
				avcodec_send_packet(codec_context, packet);

				while (avcodec_receive_frame(codec_context, frame) == 0) // 发送单个数据包后循环取解码后数据
				{
					wav.write_data(frame->data, frame->format, frame->nb_samples);
					av_frame_unref(frame);
				}
				av_packet_unref(packet);
			}
		}
		wav.write_head();
	}

	__declspec(dllexport) pcm get_pcm()
	{
		if (avcodec_receive_frame(codec_context, frame) == 0)
		{
			muxer->frame_to_pcm(frame);
			return pcm{ frame->nb_samples, muxer->pcm_buffer };
		}
		else
		{
			av_packet_unref(packet);
			if (av_read_frame(format_context, packet) == 0)
			{
				avcodec_send_packet(codec_context, packet);
				avcodec_receive_frame(codec_context, frame);
				muxer->frame_to_pcm(frame);
				return pcm{ frame->nb_samples, muxer->pcm_buffer };
			}
			else
			{
				return pcm{ 0,NULL };
			}
		}
	}

	__declspec(dllexport) int get_pcm_size()
	{
		return frame->nb_samples;
	}

	__declspec(dllexport) void free_buffers()
	{
		muxer->pcm_buffer_free();
	}
}

int main()
{

}

//int main()
//{
//	int argc;
//	wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
//	wchar_t* file = argv[1];
//	int utf8_size = WideCharToMultiByte(CP_UTF8, 0, file, -1, NULL, 0, NULL, NULL);
//	char* filep = (char*)malloc(utf8_size);
//	WideCharToMultiByte(CP_UTF8, 0, file, -1, filep, utf8_size, NULL, NULL);
//
//	init(filep);
//	decode_wav();
//}
//__declspec(dllexport) 

//AVFormatContext* fmt_ctx = avformat_alloc_context();
//
//void init(char8_t* filename)
//{
//	avformat_open_input(&fmt_ctx, reinterpret_cast<const char*>(filename), NULL, NULL);
//	avformat_find_stream_info(fmt_ctx, NULL);
//}
//
//extern "C"
//{
//
//
//	__declspec(dllexport) void libmain(char8_t* filepath)
//	{
//		//int argc;
//		//wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
//
//		//if (argc < 2) {
//		//	std::filesystem::path input_path(argv[0]);
//		//	std::string output_filename = input_path.filename().stem().string() + ".exe";
//		//	std::println("Usage: {} <input file>", output_filename);
//		//	//return 1;
//		//}
//		//auto start = std::chrono::high_resolution_clock::now();
//
//		// 初始化格式上下文
//		AVFormatContext* fmt_ctx = avformat_alloc_context();
//
//		// 打开文件并读取流信息
//
//		//wchar_t* filename = argv[1];
//		//int utf8_size = WideCharToMultiByte(CP_UTF8, 0, filename, -1, NULL, 0, NULL, NULL);
//		//char* filepath = (char*)malloc(utf8_size);
//		//WideCharToMultiByte(CP_UTF8, 0, filename, -1, filepath, utf8_size, NULL, NULL);
//
//
//		// 打开文件并读取流信息
//
//		avformat_open_input(&fmt_ctx, reinterpret_cast<const char*>(filepath), NULL, NULL);
//		avformat_find_stream_info(fmt_ctx, NULL);
//
//		//init(reinterpret_cast<const char*>filepath);
//
//		std::println("容器格式: {}", fmt_ctx->iformat->long_name);
//		std::println("轨道数: {}", fmt_ctx->nb_streams);
//
//		int audio_stream_index = 0; // 查找音频轨道索引
//		for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++)
//		{
//			std::println("第 {} 轨媒体类型: {} , 编码格式: {}",
//				i, av_get_media_type_string(fmt_ctx->streams[i]->codecpar->codec_type), avcodec_get_name(fmt_ctx->streams[i]->codecpar->codec_id));
//			if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
//			{
//				audio_stream_index = i;
//			}
//		}
//
//		// 获取音频轨道的编码参数
//		AVCodecParameters* codecpar = fmt_ctx->streams[audio_stream_index]->codecpar;
//		std::println("音频轨时间基 : {}/{} s", fmt_ctx->streams[audio_stream_index]->time_base.num, fmt_ctx->streams[audio_stream_index]->time_base.den);
//		std::println("音频轨时间基时长 : {}", fmt_ctx->streams[audio_stream_index]->duration);
//
//		std::println("音频轨时长 : {} s", av_rescale_q(fmt_ctx->streams[audio_stream_index]->duration, fmt_ctx->streams[audio_stream_index]->time_base, { 1, 1 }));
//		std::println("音频轨采样率 : {} Hz", codecpar->sample_rate);
//
//		int bits_per_sample = codecpar->bits_per_raw_sample ? codecpar->bits_per_raw_sample : codecpar->bits_per_coded_sample;
//		std::println("音频轨位深 : {} bit", bits_per_sample);
//		std::println("音频轨声道数 : {}", codecpar->ch_layout.nb_channels);
//
//		// 初始化数据包
//		AVPacket* pkt = av_packet_alloc();
//
//		// 查找解码器并初始化解码器上下文
//		const AVCodec* codec = avcodec_find_decoder(fmt_ctx->streams[audio_stream_index]->codecpar->codec_id);
//		AVCodecContext* avc_ctx = avcodec_alloc_context3(codec);
//
//		// 将解码参数应用于解码器
//		avcodec_parameters_to_context(avc_ctx, codecpar);
//		avcodec_open2(avc_ctx, codec, NULL);
//
//		// 初始化帧
//		AVFrame* frame = av_frame_alloc();
//
//
//		std::filesystem::path input_path(reinterpret_cast<char8_t*>(filepath));
//
//
//
//		//std::filesystem::path input_path(filename);
//		//std::wstring output_filename = input_path.filename().stem().wstring() + L".wav";
//
//		std::filesystem::path output_filename = input_path.filename().stem();
//		output_filename += std::filesystem::path(".wav");
//
//		//std::filesystem::path f(utf8_filename);
//		//std::println("PCM 写入 {}", f.wstring());
//		//std::wcout << output_filename << std::endl;
//		//utf8_size = WideCharToMultiByte(CP_UTF8, 0, output_filename.c_str(), -1, NULL, 0, NULL, NULL);
//		//char* utf8_filename = (char*)malloc(utf8_size);
//		//WideCharToMultiByte(CP_UTF8, 0, filename, -1, utf8_filename, utf8_size, NULL, NULL);
//
//
//
//
//
//
//		wav_output wav(output_filename, codecpar->sample_rate, bits_per_sample);
//
//		auto codec_start = std::chrono::high_resolution_clock::now();
//		while (av_read_frame(fmt_ctx, pkt) == 0) // 循环读数据包
//		{
//			if (pkt->stream_index == audio_stream_index)
//			{
//				avcodec_send_packet(avc_ctx, pkt);
//
//				while (avcodec_receive_frame(avc_ctx, frame) == 0) // 发送单个数据包后循环取解码后数据
//				{
//					wav.write_data(frame->data, frame->format, frame->nb_samples);
//				}
//			}
//		}
//		wav.write_head();
//
//		// 最后发一次空包 // 音频流大概率不需要
//		//int ret_send = avcodec_send_packet(avc_ctx, NULL);
//		//if (ret_send == 0) {
//		//	for (;;)
//		//	{
//		//		int ret_receive = avcodec_receive_frame(avc_ctx, frame);
//		//		if (ret_receive == 0)
//		//		{
//		//			
//		//		}
//		//		else if (ret_receive == AVERROR_EOF)
//		//		{
//		//			break;
//		//		}
//		//	}
//		//}
//
//		//auto end = std::chrono::high_resolution_clock::now();
//		//std::chrono::duration<double> duration = end - start;
//		//std::println("time: {} s", duration.count());
//
//	}
//}