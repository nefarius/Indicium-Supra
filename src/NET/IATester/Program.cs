using System;
using NetMQ;
using NetMQ.Sockets;

namespace IATester
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Title = "NetMQ HelloWorld";

            using (var client = new RequestSocket("tcp://localhost:5556"))
            {
                client.SendFrame("Hello");

                Console.WriteLine("From Server: {0}", client.ReceiveFrameString());

                Console.WriteLine();
                Console.Write("Press any key to exit...");
                Console.ReadKey();
            }
        }
    }
}
