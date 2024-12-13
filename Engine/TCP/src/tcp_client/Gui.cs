namespace tcp_client;

using System.Net.Sockets;
public class Gui
{
    public static void Loop()
    {
        try
        {
            // Define and initialize the TcpClient
            String host = "127.0.0.1";
            int port = 51234;
            EcsApi api = new EcsApi(host, port);
            // Send first message
            //String uuid1 = api.move_entity("213123", 1.0f, 2.0f, 3.0f);
            // Send second message
            //String uuid2 = api.rotate_entity("213213", 1.0f, 2.0f, 3.0f);
            //client.Close();
            //string test = api.get_entity_options("c9ba5b5b-eb8e-430b-badf-e4ec7c68e9e2");
            //string message = api.get_root();
            string message = api.get_components("35d84bb9-4289-4f10-8ca6-1c07c04b6cd4");
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