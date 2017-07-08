using System;
using DX9OverlayAPI;

namespace hello_world
{
    class Program
    {
        static int overlayText = -1;

        static void Main(string[] args)
        {
            //DX9Overlay.SetParam("use_window", "1"); // Use the window name to find the process
            //DX9Overlay.SetParam("window", "GTA:SA:MP"); // Set the window name
            //DX9Overlay.SetParam("process", "CastlevaniaLoSUE.exe");

            if(args.Length < 1)
                return;

            var proc = args[0];

            DX9Overlay.SetParam("process", proc);
            DX9Overlay.DestroyAllVisual();

            overlayText = DX9Overlay.TextCreate("Consolas", 12, false, false, 10, 14, 0xFF00FFFF, "DualShock 3 #1 (USB, Battery: 80%)", true, true); // Initialize 'overlayText'
            if(overlayText == -1)
            {
                Console.WriteLine("A error is occurred at the intializing.");
                Console.WriteLine("Press any key to close the application!");

                Console.ReadKey();
                return;
            }

            DX9Overlay.ImageCreate(@"D:\Temp\battery_discharging_080.png", 165, 5, 0.7f, 0.7f, 90, 1, true);

            //DX9Overlay.BoxCreate(10, 50, 100, 100, 0x15FF0000, true);

            Console.WriteLine("Enter 'exit' to close the application!");
            string input = Console.ReadLine(); // Get the input

            while(input != "exit")
            {
                Console.WriteLine("FPS = {0}", DX9Overlay.GetFrameRate());
                DX9Overlay.TextSetString(overlayText, input); // Set 'overlayText' string to the value of 'input'
                input = Console.ReadLine();
            }

            DX9Overlay.DestroyAllVisual();
        }
    }
}
