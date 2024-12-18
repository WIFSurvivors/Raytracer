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
            String uuid1 = api.move_entity("76e596f1-0edc-483b-bdc3-92a42bac942d", 1.0f, 2.0f, 3.0f);
            // Send second message
            String uuid2 = api.rotate_entity("76e596f1-0edc-483b-bdc3-92a42bac942d", 1.0f, 2.0f, 3.0f);
            //client.Close();
            string thing = "04dcd96a-fe38-49c6-bc64-704ed00c4db3";
            string test = api.get_entity_options(thing);
            string message1 = api.get_child_entities(thing);
            string message = api.get_components(thing);
            string message2 = api.get_root();
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