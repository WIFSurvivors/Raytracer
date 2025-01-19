using System;
using System.Net.Sockets;
using System.Text;
using System.Windows;
//The socket was taken from the distributed Systems course at THU 
namespace tcp_client
{
    class MyTcpClient
    {
        private TcpClient client;
        public MyTcpClient(String host, int port)
        {
            client = new TcpClient(host, port);
        }
        public string Send(string message)
        {
            try
            {
                NetworkStream stream = client.GetStream();

                // Send message
                Byte[] data = Encoding.ASCII.GetBytes(message);
                stream.Write(data, 0, data.Length);
                Console.WriteLine("Sent: {0}", message);

                // Receive response
                data = new Byte[4096];
                String responseData = String.Empty;
                Int32 bytes = stream.Read(data, 0, data.Length);
                responseData = Encoding.ASCII.GetString(data, 0, bytes);
                Console.WriteLine("Received: {0}", responseData);
                return responseData;
            }

            catch (System.Net.Sockets.SocketException ex)
            {
                Console.WriteLine($"SocketException: {ex.Message}");
                Console.WriteLine($"ErrorCode: {ex.ErrorCode}");
                Console.WriteLine($"StackTrace: {ex.StackTrace}");
                return "SocketException";
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
                return "ArgumentNullException";
            }

        }
    }
}