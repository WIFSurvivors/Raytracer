namespace tcp_client;

using System.Net.Sockets;
public class Gui
{
    public static void Loop(string uuid)
    {
        try
        {
            // Define and initialize the TcpClient
            String host = "127.0.0.1";
            int port = 51234;
            EcsApi api = new EcsApi(host, port);
            // Send first message
            string thing = uuid;
            string message = api.get_component_options(thing);
            String uuid1 = api.move_entity(thing, 1.0f, 2.0f, 3.0f);
            // Send second message
            String uuid2 = api.rotate_entity(thing, 1.0f, 2.0f, 3.0f);
            //client.Close();
            string test = api.get_entity_options(thing);
            string message1 = api.get_child_entities(thing);
            string message2 = api.create_entity(thing, "test");
            string message6 = api.create_component(thing, "light");
            string message3 = api.get_components(thing);
            string message4 = api.get_root();
            string message22 = api.get_log_path();
            //uncomment which one you dont need
            //camera
            string message5 = api.set_component_option(thing, "{\"is_main_camera\":false,\"fov\":6E33}");
            //light
            string message33 = api.set_component_option(thing, "{\"r\":1.0000000149011612E1,\"g\":1.0000000149011612E1,\"b\":1E-2,\"intensity\":1.5E1}");
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