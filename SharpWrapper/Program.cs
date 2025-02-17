using System.Runtime.InteropServices;

namespace SharpWrapper
{
    public class Program
    {
        public static void Main(string[] args)
        {
            //Console.WriteLine("你好");
            var filepath = args[0];
            var filename = Path.GetFileNameWithoutExtension(filepath) + ".wav";
            
            var info = Audio2WAV.init(filepath);
            Console.WriteLine(info.sample_rate);
            Console.WriteLine(info.bits_per_sample);
            Audio2WAV.decode_wav();

            Console.WriteLine($"PCM 写入: \"{filename}\"");
        }
    }
}