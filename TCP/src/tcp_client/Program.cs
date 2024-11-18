// See https://aka.ms/new-console-template for more information

using System.Net.Sockets;
try
{
    // Define and initialize the TcpClient
    TcpClient client = new TcpClient("127.0.0.1", 51234);

    // Send first message
    MyTcpClient.TcpClientProgram.Send(client, "MoveCommand 2132121 2 3 4");

    // Send second message
    MyTcpClient.TcpClientProgram.Send(client, "RotateCommand 2132121 2 3 4");

    //client.Close();
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