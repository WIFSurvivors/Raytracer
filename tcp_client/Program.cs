// See https://aka.ms/new-console-template for more information

using tcp_client;

class Program
{
    static void Main(string[] args)
    {
        if (args.Length > 0)
        {
            Gui.Loop(args[0]);
        }
        else
        {
            Console.WriteLine("Please provide an argument.");
        }
    }
}