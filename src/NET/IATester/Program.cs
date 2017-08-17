using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using NetMQ;
using NetMQ.Sockets;

namespace IATester
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Title = "NetMQ HelloWorld";

            using (var client = new RequestSocket("tcp://localhost:36129"))
            {
                using (var image = Image.FromFile(@"D:\Downloads\# Bilder\Austria-Flag-4.jpg"))
                {
                    using (var bitmap = new Bitmap(image).Clone(new Rectangle(0, 0, image.Width, image.Height), PixelFormat.Format32bppArgb))
                    {
                        var data = bitmap.LockBits(new Rectangle(0, 0, image.Width, image.Height),
                            ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);

                        try
                        {
                            var length = data.Stride * data.Height;

                            var buffer = new byte[length];

                            // Copy bitmap to byte[]
                            Marshal.Copy(data.Scan0, buffer, 0, length);

                            var m = new NetMQMessage(5);
                            m.Append(600.ToString());
                            m.Append(350.ToString());
                            m.Append(image.Width.ToString());
                            m.Append(image.Height.ToString());
                            m.Append(buffer);
                            
                            client.SendMultipartMessage(m);
                        }
                        finally
                        {
                            bitmap.UnlockBits(data);
                        }
                    }

                    
                }

                /*
                const int PixelWidth = 3;
                const PixelFormat PixelFormat = PixelFormat.Format24bppRgb;

                // En garde!
                if (image == null) throw new ArgumentNullException("image");
                if (rgbArray == null) throw new ArgumentNullException("rgbArray");
                if (startX < 0 || startX + w > image.Width) throw new ArgumentOutOfRangeException("startX");
                if (startY < 0 || startY + h > image.Height) throw new ArgumentOutOfRangeException("startY");
                if (w < 0 || w > scansize || w > image.Width) throw new ArgumentOutOfRangeException("w");
                if (h < 0 || (rgbArray.Length < offset + h * scansize) || h > image.Height) throw new ArgumentOutOfRangeException("h");

                BitmapData data = image.LockBits(new Rectangle(startX, startY, w, h), System.Drawing.Imaging.ImageLockMode.ReadOnly, PixelFormat);
                try
                {
                    byte[] pixelData = new Byte[data.Stride];
                    for (int scanline = 0; scanline < data.Height; scanline++)
                    {
                        Marshal.Copy(data.Scan0 + (scanline * data.Stride), pixelData, 0, data.Stride);
                        for (int pixeloffset = 0; pixeloffset < data.Width; pixeloffset++)
                        {
                            // PixelFormat.Format32bppRgb means the data is stored
                            // in memory as BGR. We want RGB, so we must do some 
                            // bit-shuffling.
                            rgbArray[offset + (scanline * scansize) + pixeloffset] =
                                (pixelData[pixeloffset * PixelWidth + 2] << 16) +   // R 
                                (pixelData[pixeloffset * PixelWidth + 1] << 8) +    // G
                                pixelData[pixeloffset * PixelWidth];                // B
                        }
                    }
                }
                finally
                {
                    image.UnlockBits(data);
                }
                */


                //client.SendFrame("Hello");

                Console.WriteLine("From Server: {0}", client.ReceiveFrameString());

                Console.WriteLine();
                Console.Write("Press any key to exit...");
                Console.ReadKey();
            }
        }
    }
}
