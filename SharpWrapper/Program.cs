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
            var wav = new WavWriter(filename, (uint)info.sample_rate, (ushort)info.bits_per_sample);


            Console.WriteLine(info.sample_rate);
            Console.WriteLine(info.bits_per_sample);


            //Audio2WAV.decode_wav();
            var pcm = Audio2WAV.receive();
            
            while (pcm.buffer_size != 0)
            {
                var pcm_bytes = new byte[pcm.buffer_size];
                Marshal.Copy(pcm.data, pcm_bytes, 0, pcm_bytes.Length);
                wav.WritePCM(pcm_bytes, pcm.buffer_size);
                Audio2WAV.free_buffers();

                pcm = Audio2WAV.receive();
            }
            wav.WriteHead();
            Console.OutputEncoding = System.Text.Encoding.Unicode;

            Console.WriteLine($"PCM 写入: \"{filename}\"");
            Console.WriteLine("Aiobahn, YUC'e - Märchen EP - 03 永久の宴.wav");
        }
    }
}