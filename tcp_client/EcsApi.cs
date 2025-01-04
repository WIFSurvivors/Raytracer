using System.Net.Sockets;

namespace tcp_client;

public class EcsApi
{
    private MyTcpClient _client;
    public EcsApi(String host, int port)
    {
        try
        {
            _client = new MyTcpClient(host, port);
        }
        catch (System.Net.Sockets.SocketException ex)
        {
            throw new InvalidOperationException("Could not establish a TCP connection.", ex);
        }
    }

    public string json_import(string path) {
        //Aktuell gibt die Engine den Inhalt der Datei an. Wird noch geändert.
        string returnValue = _client.Send(String.Format("ImportJson null {0}", path));
        return returnValue;
    }

    public string close_RS()
    {
        return _client.Send("CloseRender");
    }
    public string get_root()
    {
        return _client.Send("GetRoot");
    }
    public string get_child_entities(string UUID)
    {
        return _client.Send(string.Format("GetChildEntities {0}", UUID));
    }
    public string get_entity_options(string UUID)
    {
        return _client.Send(String.Format("GetEntityOptions {0}", UUID));
    }
    public string get_components(String UUID)
    {
        return _client.Send(String.Format("GetComponents {0}", UUID));
    }
    public string create_entity(String UUID,String name)
    {
        return _client.Send(String.Format("CreateEntity {0} {1}", UUID, name));
        
    }
    public string create_component(String UUID, string type)
    {
        return _client.Send(String.Format("CreateComponent {0} {1}", UUID, type));
    }
    public string move_entity(String UUID, float x, float y, float z)
    {
        String returnValue = _client.Send(String.Format("MoveCommand {0} {1} {2} {3}", UUID, x, y, z));
        return "CreateComponent " + returnValue;
    }
    public string scale_entity(String UUID, float x, float y, float z)
    {
        String returnValue = _client.Send(String.Format("ScaleCommand {0} {1} {2} {3}", UUID, x, y, z));
        return "ScaleEntity";
    }
    public string rotate_entity(String UUID, float x, float y, float z)
    {
        String returnValue = _client.Send(String.Format("RotateCommand {0} {1} {2} {3}", UUID, x, y, z));
        return "RotateEntity " + returnValue;
    }
    public string delete_entity()
    {
        return "DeleteEntity";
    }
    public string delete_component()
    {
        return "DeleteComponent";
    }

    public string get_component_options(String UUID)
    {
        return _client.Send(String.Format("GetComponentOptions {0}", UUID));
    }

    public string post_ScenePath(String path)
    {
        String returnValue = _client.Send(String.Format("PostScenePathCommand"));
        return "ScenePath " + returnValue;
    }

    public string set_component_option(String UUID, String json)
    {
        return _client.Send(String.Format("SetComponentOptions {0} {1}", UUID, json));
    }

    public string get_log_path()
    {
        return _client.Send("GetLogPath null");
    }
}