using System.Runtime.InteropServices;
using System.Text;

namespace SharpWrapper
{
    class WavWriter
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct WavHead
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] ID = Encoding.ASCII.GetBytes(['R', 'I', 'F', 'F']); // "RIFF"

            public uint Size = 36;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] FourCC = Encoding.ASCII.GetBytes(['W', 'A', 'V', 'E']); // "WAVE"

            // SubChunk "fmt"
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] fmtID = Encoding.ASCII.GetBytes(['f', 'm', 't', ' ']); // "fmt "
            public uint fmtSize = 16; // 16
            // "fmt" Data
            public ushort encodeMode; // 1 for PCM, ...
            public ushort numChannel; // 1 for Mono, 2 for Stereo
            public uint samplingRate; // usually 44100 or higher for lossless
            public uint byteRate; // numChannel * samplingRate * bitDepth / 8
            public ushort blockAlign; // numChannel * bitDepth / 8
            public ushort bitDepth; // usually 16 for lossless

            // SubChunk "data"
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] dataID; // "data"
            public uint dataSize;
            // "data" Data is the kind chosen in "encodeMode"

            public WavHead()
            {
            }
        }

        public void WritePCM(byte[] pcm, int nb_samples)
        {

        }

        public void WriteHead()
        {

        }
    }
}