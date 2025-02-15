using System.Runtime.InteropServices;

namespace SharpWrapper
{
    public class Program()
    {
        public static void Main(string[] args)
        {
            //Console.WriteLine("你好");
            
            Audio2WAV.AudioInfo info = Audio2WAV.init(args[0]);
            Console.WriteLine(info.sample_rate);
            Console.WriteLine(info.bits_per_sample);
            Audio2WAV.decode_wav();
        }
    }

    partial class Audio2WAV
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AudioInfo
        {
            public int sample_rate;
            public int bits_per_sample;
        }

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial AudioInfo init(string filepath);

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial void decode_wav();
    }
}