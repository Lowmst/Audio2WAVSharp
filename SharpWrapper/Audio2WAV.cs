using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SharpWrapper
{
    struct PCM
    {
        public IntPtr data;
        public int length;
    }

    partial class Audio2WAV
    {
        private WavWriter wav;

        public Audio2WAV(string filename)
        {
            wav = new WavWriter(filename);
        }


        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AudioInfo
        {
            public int sample_rate;
            public int bits_per_sample;
        }

        public static void Write(PCM pcm)
        {
            var data = new byte[pcm.length];
            Marshal.Copy(pcm.data, data, 0, pcm.length);

        }

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial AudioInfo init(string filepath);

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial void decode_wav();
    }
}
