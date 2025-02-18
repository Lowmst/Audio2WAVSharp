using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SharpWrapper
{
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    struct PCM
    {
        public int buffer_size;
        public IntPtr data;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    struct AudioInfo
    {
        public int sample_rate;
        public int bits_per_sample;
    }

    partial class Audio2WAV
    {

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial AudioInfo init(string filepath);

        [LibraryImport("LibAudio2WAV.dll")]
        public static partial void decode_wav();

        [LibraryImport("LibAudio2WAV.dll")]
        public static partial PCM receive();

        [LibraryImport("LibAudio2WAV.dll")]
        public static partial void free_buffers();
    }
}
