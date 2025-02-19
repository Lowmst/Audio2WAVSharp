namespace SharpWrapper
{
    public class Program
    {
        public static void Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.Unicode;
            var arguments = Environment.GetCommandLineArgs();

            if (arguments.Length < 2)
            {
                Console.WriteLine($"Usage: {Path.GetFileNameWithoutExtension(arguments[0]) + ".exe"} <input file>");
                return;
            }

            var filepath = args[0];
            var filename = Path.GetFileNameWithoutExtension(filepath) + ".wav";
            
            var info = Audio2WAV.init(filepath);
            var wav = new WavWriter(filename, (uint)info.sample_rate, (ushort)info.bits_per_sample);

            Console.WriteLine($"采样率: {info.sample_rate}");
            Console.WriteLine($"采样位深: {info.bits_per_sample}");

            var pcm = Audio2WAV.decode();
            
            while (pcm.size != 0)
            {
                wav.WritePCM(pcm);

                pcm = Audio2WAV.decode();
            }
            wav.WriteHead();
            
            Console.WriteLine($"PCM 写入: \"{filename}\"");
        }
    }
}