// See https://aka.ms/new-console-template for more information
//Console.WriteLine("Hello, World!");
using System.Runtime.InteropServices;

namespace SharpWrapper
{
    public class Program()
    {
        public static void Main(string[] args)
        {
            //Console.WriteLine("你好");
            Audio2WAV.libmain(args[0]);
        }
    }

    partial class Audio2WAV
    {
        [LibraryImport("LibAudio2WAV.dll", StringMarshalling = StringMarshalling.Utf8)]
        public static partial void libmain(string filepath);
    }
}