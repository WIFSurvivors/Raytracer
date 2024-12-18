using System.Net.Sockets;
using System.IO;

namespace tcp_client;

public class EcsApi
{
    private MyTcpClient _client;
    public EcsApi(String host, int port) {
        _client = new MyTcpClient(host, port);
    }
    public string get_root() {
        String returnValue = _client.Send("GetRoot");
        return "GetRoot " + returnValue;
    }   
    public string get_entity(String UUID)
    {
        return "GetEntity";
    }
    public string get_component(String UUID) {
        return "GetComponent";
    }
    public string get_components(String UUID) {
        return _client.Send(string.Format("GetComponents {0}", UUID));
    }
    public string create_entity() {
        return "CreateEntity";
    }
    public string json_import() {
        string filePath = Path.Combine(Directory.GetCurrentDirectory(), "data.json");
    using (FileStream fs = File.Create(filePath)) {
        using (StreamWriter writer = new StreamWriter(fs)) {
            writer.WriteLine("{ \"example\": \"data\" }");
        }
    }   
        string returnValue = _client.Send(String.Format("ImportJson null {0}",filePath));
        return returnValue;
    }

    public string create_component()
    {
        String returnValue = _client.Send("CreateCommand Entity");
        return "CreateComponent " +returnValue;
    }
    public string move_entity(String UUID, float x, float y, float z) {
        String returnValue = _client.Send(String.Format("MoveCommand {0} {1} {2} {3}",UUID,x,y,z ));
        return "CreateComponent " +returnValue;
    }
    public string scale_entity(String UUID, float x, float y, float z) {    
        String returnValue = _client.Send(String.Format("ScaleCommand {0} {1} {2} {3}",UUID,x,y,z ));
        return "ScaleEntity";
    }
    public string rotate_entity(String UUID, float x, float y, float z) {
        String returnValue = _client.Send(String.Format("RotateCommand {0} {1} {2} {3}",UUID,x,y,z ));
        return "RotateEntity " + returnValue;
    }
    public string delete_entity() {
        return "DeleteEntity";
    }
    public string delete_component() {
        return "DeleteComponent";
    }
    public string get_child_entities(string UUID) {
        return _client.Send(string.Format("GetChildEntities {0}", UUID));
    }
    public string get_entity_options(string UUID) {
        return _client.Send(string.Format("GetEntityOptions {0}", UUID));
    }
    
}