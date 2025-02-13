using System.Runtime.InteropServices;

namespace SharpWrapper
{
    public class Program()
    {
        public static void Main(string[] args)
        {
            //Console.WriteLine("你好");
            Audio2WAV.init(args[0]);
            Audio2WAV.decode_wav();
        }
    }

    partial class Audio2WAV
    {
        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial void init(string filepath);

        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial void decode_wav();
    }
}