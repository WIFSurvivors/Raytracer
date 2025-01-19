using System.Net.Sockets;

namespace tcp_client;

/// <summary>
/// Provides an API for interacting with the ECS system over a TCP connection.
/// </summary>
public class EcsApi
{
    private MyTcpClient _client;
    private string _separator = ";";

    /// <summary>
    /// Initializes a new instance of the <see cref="EcsApi"/> class.
    /// </summary>
    /// <param name="host">The host address of the TCP server.</param>
    /// <param name="port">The port number of the TCP server.</param>
    /// <exception cref="InvalidOperationException">Thrown when a TCP connection cannot be established.</exception>
    public EcsApi(string host, int port)
    {
        try
        {
            _client = new MyTcpClient(host, port);
        }
        catch (SocketException ex)
        {
            throw new InvalidOperationException("Could not establish a TCP connection.", ex);
        }
    }

    /// <summary>
    /// Imports a JSON file to the ECS system.
    /// </summary>
    /// <param name="path">The path to the JSON file.</param>
    /// <returns>The response from the server.</returns>
    public string json_import(string path)
    {
        string returnValue = _client.Send(string.Format(adjustSeparator("ImportJson null {0}"), path));
        return returnValue;
    }

    /// <summary>
    /// Adjusts the separator in the input string.
    /// </summary>
    /// <param name="input">The input string.</param>
    /// <returns>The adjusted string.</returns>
    private string adjustSeparator(string input)
    {
        return input.Replace(" ", _separator);
    }

    /// <summary>
    /// Closes the render session.
    /// </summary>
    /// <returns>The response from the server.</returns>
    public string close_RS()
    {
        return _client.Send("CloseRender");
    }

    /// <summary>
    /// Gets the root entity.
    /// </summary>
    /// <returns>The response from the server.</returns>
    public string get_root()
    {
        return _client.Send("GetRoot");
    }

    /// <summary>
    /// Gets the child entities of a specified entity.
    /// </summary>
    /// <param name="UUID">The UUID of the parent entity.</param>
    /// <returns>The response from the server.</returns>
    public string get_child_entities(string UUID)
    {
        return _client.Send(string.Format(adjustSeparator("GetChildEntities {0}"), UUID));
    }

    /// <summary>
    /// Gets the options of a specified entity.
    /// </summary>
    /// <param name="UUID">The UUID of the entity.</param>
    /// <returns>The response from the server.</returns>
    public string get_entity_options(string UUID)
    {
        return _client.Send(string.Format(adjustSeparator("GetEntityOptions {0}"), UUID));
    }

    /// <summary>
    /// Gets the components of a specified entity.
    /// </summary>
    /// <param name="UUID">The UUID of the entity.</param>
    /// <returns>The response from the server.</returns>
    public string get_components(string UUID)
    {
        return _client.Send(string.Format(adjustSeparator("GetComponents {0}"), UUID));
    }

    /// <summary>
    /// Creates a new entity.
    /// </summary>
    /// <param name="UUID">The UUID of the parent entity.</param>
    /// <param name="name">The name of the new entity.</param>
    /// <returns>The response from the server.</returns>
    public string create_entity(string UUID, string name)
    {
        return _client.Send(string.Format(adjustSeparator("CreateEntity {0} {1}"), UUID, name));
    }

    /// <summary>
    /// Creates a new component for a specified entity.
    /// </summary>
    /// <param name="UUID">The UUID of the entity.</param>
    /// <param name="type">The type of the new component.</param>
    /// <returns>The response from the server.</returns>
    public string create_component(string UUID, string type)
    {
        return _client.Send(string.Format(adjustSeparator("CreateComponent {0} {1}"), UUID, type));
    }

    /// <summary>
    /// Moves a specified entity to a new position.
    /// </summary>
    /// <param name="UUID">The UUID of the entity.</param>
    /// <param name="x">The new x-coordinate.</param>
    /// <param name="y">The new y-coordinate.</param>
    /// <param name="z">The new z-coordinate.</param>
    /// <returns>The response from the server.</returns>
    public string move_entity(string UUID, float x, float y, float z)
    {
        string returnValue = _client.Send(string.Format(adjustSeparator("MoveCommand {0} {1} {2} {3}"), UUID, x, y, z));
        return "CreateComponent " + returnValue;
    }

    /// <summary>
    /// Scales a specified entity.
    /// </summary>
    /// <param name="UUID">The UUID of the entity.</param>
    /// <param name="x">The scale factor along the x-axis.</param>
    /// <param name="y">The scale factor along the y-axis.</param>
    /// <param name="z">The scale factor along the z-axis.</param>
    /// <returns>The response from the server.</returns>
    public string scale_entity(string UUID, float x, float y, float z)
    {
        string returnValue = _client.Send(string.Format(adjustSeparator("ScaleCommand {0} {1} {2} {3}"), UUID, x, y, z));
        return "ScaleEntity " + returnValue;
    }

    /// <summary>
    /// Rotates a specified entity.
    /// </summary>
    /// <param name="UUID">The UUID of the entity.</param>
    /// <param name="x">The rotation angle around the x-axis.</param>
    /// <param name="y">The rotation angle around the y-axis.</param>
    /// <param name="z">The rotation angle around the z-axis.</param>
    /// <returns>The response from the server.</returns>
    public string rotate_entity(string UUID, float x, float y, float z)
    {
        string returnValue = _client.Send(string.Format(adjustSeparator("RotateCommand {0} {1} {2} {3}"), UUID, x, y, z));
        return "RotateEntity " + returnValue;
    }

    /// <summary>
    /// Removes a specified entity.
    /// </summary>
    /// <param name="UUID">The UUID of the entity.</param>
    /// <returns>The response from the server.</returns>
    public string remove_entity(string UUID)
    {
        return _client.Send(string.Format(adjustSeparator("RemoveEntity {0}"), UUID));
    }

    /// <summary>
    /// Removes a specified component.
    /// </summary>
    /// <param name="UUID">The UUID of the component.</param>
    /// <returns>The response from the server.</returns>
    public string remove_component(string UUID)
    {
        return _client.Send(string.Format(adjustSeparator("RemoveComponent {0}"), UUID));
    }

    /// <summary>
    /// Gets the options of a specified component.
    /// </summary>
    /// <param name="UUID">The UUID of the component.</param>
    /// <returns>The response from the server.</returns>
    public string get_component_options(string UUID)
    {
        return _client.Send(string.Format(adjustSeparator("GetComponentOptions {0}"), UUID));
    }

    /// <summary>
    /// Sets the options of a specified component.
    /// </summary>
    /// <param name="UUID">The UUID of the component.</param>
    /// <param name="json">The JSON string containing the options.</param>
    /// <returns>The response from the server.</returns>
    public string set_component_option(string UUID, string json)
    {
        return _client.Send(string.Format(adjustSeparator("SetComponentOptions {0} {1}"), UUID, json));
    }

    /// <summary>
    /// Gets the log path.
    /// </summary>
    /// <returns>The response from the server.</returns>
    public string get_log_path()
    {
        return _client.Send(adjustSeparator("GetLogPath null"));
    }

    /// <summary>
    /// Exports the ECS system to a JSON file.
    /// </summary>
    /// <param name="path">The path to the JSON file.</param>
    /// <returns>The response from the server.</returns>
    public string export_Json(string path)
    {
        return _client.Send(string.Format(adjustSeparator("ExportJson null {0}"), path));
    }

    /// <summary>
    /// Gets the number of bounces.
    /// </summary>
    /// <returns>The response from the server.</returns>
    public string get_bounces()
    {
        return _client.Send("GetBounces");
    }

    /// <summary>
    /// Sets the number of bounces.
    /// </summary>
    /// <param name="bounces">The number of bounces.</param>
    /// <returns>The response from the server.</returns>
    public string set_bounces(int bounces)
    {
        return _client.Send(string.Format(adjustSeparator("SetBounces null {0}"), bounces));
    }

    /// <summary>
    /// Sets the frame rate.
    /// </summary>
    /// <param name="frame_rate">The frame rate.</param>
    /// <returns>The response from the server.</returns>
    public string set_frame_rate(int frame_rate)
    {
        return _client.Send(string.Format(adjustSeparator("SetFrameRate null {0}"), frame_rate));
    }

    /// <summary>
    /// Gets the frame rate.
    /// </summary>
    /// <returns>The response from the server.</returns>
    public string get_frame_rate()
    {
        return _client.Send("GetFrameRate");
    }

    /// <summary>
    /// Sets the field of view (FOV).
    /// </summary>
    /// <param name="fov">The field of view.</param>
    /// <returns>The response from the server.</returns>
    public string set_fov(float fov)
    {
        return _client.Send(string.Format(adjustSeparator("SetFov null {0}"), fov));
    }

    /// <summary>
    /// Gets the field of view (FOV).
    /// </summary>
    /// <returns>The response from the server.</returns>
    public string get_fov()
    {
        return _client.Send("GetFov");
    }

    /// <summary>
    /// Undoes the specified number of steps.
    /// </summary>
    /// <param name="steps">The number of steps to undo.</param>
    /// <returns>The response from the server.</returns>
    public string undo(int steps)
    {
        return _client.Send(string.Format(adjustSeparator("Undo null {0}"), steps));
    }
}
