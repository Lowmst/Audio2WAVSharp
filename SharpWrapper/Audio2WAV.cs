using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SharpWrapper
{
    partial class Audio2WAV
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AudioInfo
        {
            public int sample_rate;
            public int bits_per_sample;
        }

        public static void Write(byte[] pcm)
        {

        }

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial AudioInfo init(string filepath);

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial void decode_wav();
    }
}
