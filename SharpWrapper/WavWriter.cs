using System.Runtime.InteropServices;
using System.Text;

namespace SharpWrapper
{
    struct WavHead
    {
        public byte[] ID = Encoding.ASCII.GetBytes(['R', 'I', 'F', 'F']); // "RIFF"

        public uint Size = 36;

        public byte[] FourCC = Encoding.ASCII.GetBytes(['W', 'A', 'V', 'E']); // "WAVE"

        // SubChunk "fmt"
        public byte[] fmtID = Encoding.ASCII.GetBytes(['f', 'm', 't', ' ']); // "fmt "
        public uint fmtSize = 16; // 16
        // "fmt" Data
        public ushort encodeMode = 1; // 1 for PCM, ...
        public ushort numChannel = 2; // 1 for Mono, 2 for Stereo
        public uint samplingRate; // usually 44100 or higher for lossless
        public uint byteRate; // numChannel * samplingRate * bitDepth / 8
        public ushort blockAlign; // numChannel * bitDepth / 8
        public ushort bitDepth; // usually 16 for lossless

        // SubChunk "data"
        public byte[] dataID = Encoding.ASCII.GetBytes(['d', 'a', 't', 'a']); // "data"
        public uint dataSize;
        // "data" Data is the kind chosen in "encodeMode"

        public WavHead() {}
    }

    class WavWriter
    {
        private BinaryWriter file;
        private WavHead head;

        private int nb_samples = 0;

        public WavWriter(string filename)
        {
            this.file = new BinaryWriter(File.Create(filename));
            this.head = new WavHead();
            file.Seek(44, SeekOrigin.Begin);
        }

        

        public void WritePCM(byte[] pcm, int nb_samples)
        {
            file.Write(pcm);
            this.nb_samples += nb_samples;
        }

        public void WriteHead()
        {
            this.file.Seek(0, SeekOrigin.Begin);

            file.Write(head.ID);
            file.Write(head.Size);
            file.Write(head.FourCC);
            file.Write(head.fmtID);
            file.Write(head.fmtSize);
            file.Write(head.encodeMode);
            file.Write(head.numChannel);
            file.Write(head.samplingRate);
            file.Write(head.byteRate);
            file.Write(head.blockAlign);
            file.Write(head.bitDepth);
            file.Write(head.dataID);
            file.Write(head.dataSize);
        }
    }
}