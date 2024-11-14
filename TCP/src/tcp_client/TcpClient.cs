using System;
using System.Net.Sockets;
using System.Text;
//The socket was taken from the distributed Systems course at THU 
namespace MyTcpClient
{
    class TcpClientProgram
    {
        public static void Main(string[] args)
        {
            try
            {
                TcpClient client = new TcpClient("127.0.0.1", 51234);

                string message = "Hello Server!";
                Byte[] data = Encoding.ASCII.GetBytes(message);

                NetworkStream stream = client.GetStream();

                stream.Write(data, 0, data.Length);
                Console.WriteLine("Sent: {0}", message);

                data = new Byte[256];

                String responseData = String.Empty;

                Int32 bytes = stream.Read(data, 0, data.Length);
                responseData = Encoding.ASCII.GetString(data, 0, bytes);
                Console.WriteLine("Received: {0}", responseData);

                stream.Close();
                client.Close();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
            }

            Console.WriteLine("\n Press Enter to continue...");
            Console.Read();
        }
    }
}