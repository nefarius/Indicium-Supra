using System;
using DX9OverlayAPI;

namespace hello_world
{
    class Program
    {
        static int overlayText = -1;

        static void Main(string[] args)
        {
            DX9Overlay.SetParam("use_window", "1"); // Use the window name to find the process
            DX9Overlay.SetParam("window", "GTA:SA:MP"); // Set the window name

            overlayText = DX9Overlay.TextCreate("Arial", 12, false, false, 200, 200, 0xFF00FFFF, "Hello world", true, true); // Initialize 'overlayText'
            if(overlayText == -1)
            {
                Console.WriteLine("A error is occurred at the intializing.");
                Console.WriteLine("Press any key to close the application!");

                Console.ReadKey();
                return;
            }
            Console.WriteLine("Enter 'exit' to close the application!");
            string input = Console.ReadLine(); // Get the input

            while(input != "exit")
            {
                DX9Overlay.TextSetString(overlayText, input); // Set 'overlayText' string to the value of 'input'
                input = Console.ReadLine();
            }

            DX9Overlay.TextDestroy(overlayText); // Destroy text if the input is 'exit'
        }
    }
}
