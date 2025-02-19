#pragma once
#define  dll extern "C" __declspec(dllexport)

typedef struct
{
	int sample_rate;
	int bits_per_sample;
} WAVParameters;

typedef struct
{
	int size;
	char* data;
} PCMPacket;