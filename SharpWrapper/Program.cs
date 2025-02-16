using System.Runtime.InteropServices;

namespace SharpWrapper
{
    public class Program
    {
        public static void Main(string[] args)
        {
            //Console.WriteLine("你好");
            
            var info = Audio2WAV.init(args[0]);
            Console.WriteLine(info.sample_rate);
            Console.WriteLine(info.bits_per_sample);
            Audio2WAV.decode_wav();
        }
    }
}