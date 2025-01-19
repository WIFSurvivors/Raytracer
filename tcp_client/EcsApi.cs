using System.Net.Sockets;

namespace tcp_client;

public class EcsApi
{
    private MyTcpClient _client;
    private string _separator = ";";
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
        string returnValue = _client.Send(String.Format(adjustSeparator("ImportJson null {0}"), path));
        return returnValue;
    }

    private string adjustSeparator(string input)
    {
        return input.Replace(" ", _separator);
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
        return _client.Send(string.Format(adjustSeparator("GetChildEntities {0}"), UUID));
    }
    public string get_entity_options(string UUID)
    {
        return _client.Send(String.Format(adjustSeparator("GetEntityOptions {0}"), UUID));
    }
    public string get_components(String UUID)
    {
        return _client.Send(String.Format(adjustSeparator("GetComponents {0}"), UUID));
    }
    public string create_entity(String UUID,String name)
    {
        return _client.Send(String.Format(adjustSeparator("CreateEntity {0} {1}"), UUID, name));
        
    }
    public string create_component(String UUID, string type)
    {
        return _client.Send(String.Format(adjustSeparator("CreateComponent {0} {1}"), UUID, type));
    }
    public string move_entity(String UUID, float x, float y, float z)
    {
        String returnValue = _client.Send(String.Format(adjustSeparator("MoveCommand {0} {1} {2} {3}"), UUID, x, y, z));
        return "CreateComponent " + returnValue;
    }
    public string scale_entity(String UUID, float x, float y, float z)
    {
        String returnValue = _client.Send(String.Format(adjustSeparator("ScaleCommand {0} {1} {2} {3}"), UUID, x, y, z));
        return "ScaleEntity";
    }
    public string rotate_entity(String UUID, float x, float y, float z)
    {
        String returnValue = _client.Send(String.Format(adjustSeparator("RotateCommand {0} {1} {2} {3}"), UUID, x, y, z));
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
        return _client.Send(String.Format(adjustSeparator("GetComponentOptions {0}"), UUID));
    }

    public string post_ScenePath(String path)
    {
        String returnValue = _client.Send(String.Format("PostScenePathCommand"));
        return "ScenePath " + returnValue;
    }

    public string set_component_option(String UUID, String json)
    {
        return _client.Send(String.Format(adjustSeparator("SetComponentOptions {0} {1}"), UUID, json));
    }

    public string get_log_path()
    {
        return _client.Send("GetLogPath null");
    }

    public string export_Json(String path)
    {
        return _client.Send(String.Format(adjustSeparator("ExportJson null {0}"), path));
    }

    public string get_bounces()
    {
        return _client.Send("GetBounces");
    }
    public string set_bounces(int bounces)
    {
        return _client.Send(String.Format(adjustSeparator("SetBounces null {0}"), bounces));
    }

    public string set_fov(float fov)
    {
        return _client.Send(String.Format(adjustSeparator("SetFov null {0}"), fov));
    }

    public string get_fov()
    {
        return _client.Send("GetFov");
    }

    public string undo(int steps)
    {
        return _client.Send(String.Format(adjustSeparator("Undo null {0}"), steps));
    }
}