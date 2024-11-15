using System;
using System.Net.Sockets;
using System.Text;
//The socket was taken from the distributed Systems course at THU 
namespace MyTcpClient
{
    class TcpClientProgram
    {
         public static void Send(TcpClient client, string message)
        {
            try
            {
                NetworkStream stream = client.GetStream();

                // Send message
                Byte[] data = Encoding.ASCII.GetBytes(message);
                stream.Write(data, 0, data.Length);
                Console.WriteLine("Sent: {0}", message);

                // Receive response
                data = new Byte[256];
                String responseData = String.Empty;
                Int32 bytes = stream.Read(data, 0, data.Length);
                responseData = Encoding.ASCII.GetString(data, 0, bytes);
                Console.WriteLine("Received: {0}", responseData);
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
            }
        }
    }
}