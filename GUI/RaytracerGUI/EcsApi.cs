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
            throw new InvalidOperationException("Could not establish a TCP connection. No TCP server found.", ex);
        }
    }
    public string close_RS()
    {
        return _client.Send("CloseRender");
    }
    public string get_root()
    {
        return _client.Send("GetRoot");
    }
    public string get_entity(String UUID)
    {
        return "GetEntity";
    }
    public string get_component(String UUID)
    {
        return "GetComponent";
    }
    public string create_entity()
    {
        return "CreateEntity";
    }
    public string create_component()
    {
        String returnValue = _client.Send("CreateCommand Entity");
        return "CreateComponent " + returnValue;
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

}