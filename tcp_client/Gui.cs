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
            //String uuid1 = api.move_entity(thing, 0.0f, 0.0f, -30.0f);
            /*// Send second message*/
            //String uuid2 = api.scale_entity(thing, 0.1f, 0.1f, 0.1f);
            String uuid3 = api.rotate_entity(thing, 45.0f, 0.0f, 0.0f);
            //string test23 = api.undo(3);
            //string test = api.get_entity_options(thing);
            //string message1 = api.json_import("C:\\Users\\Spyros\\projects\\Raytracer\\Engine\\json\\JsonParser_DummyFile.json");
            /*//client.Close();*/
            /*string message1 = api.get_child_entities(thing);*/
            /*string message2 = api.create_entity(thing, "test");*/
            /*string message6 = api.create_component(thing, "light");*/
            /*string message7 = api.create_component(thing, "camera");*/
            /*string message8 = api.create_component(thing, "render");*/
            /*string message9 = api.get_bounces();*/
            //string message10 = api.set_bounces(2);
            /*string message11 = api.export_Json("test");*/
            /*string message3 = api.get_components(thing);*/
            /*string message4 = api.get_root();*/
            /*string message22 = api.get_log_path();*/
            //string message222 = api.get_fov();
            //string message2222 = api.set_fov(120.0f);
            //string message22222 = api.get_fov();
            /*//uncomment which one you dont need*/
            /*//camera*/
            /*string message5 = api.set_component_option(thing, "{\"is_main_camera\":false,\"fov\":120.0}");*/
            /*//light*/
            string message = api.get_component_options(thing);
            //string message33 = api.set_component_option(thing, "{\"r\":\"1.000000014901\",\"g\":\"1.000000014901161\",\"b\":\"12\",\"intensity\":\"1.5\"}");
            string message3443 = api.set_component_option(thing, "{\"obj_path\":\"test_hier_und_da\",\"mat_path\":\"test_da_und_hier\"}");
            string testetete = api.undo(1);
            string message1231 = api.get_component_options(thing);

            /**/
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
