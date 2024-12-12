using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Sockets;
using tcp_client;
using Microsoft.Win32;
using System.Diagnostics;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using System.IO;

namespace RaytracerGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private EcsApi? _ecsApi;
        private string selectedUUID;
        private GLFWLoader loader;
        private IntPtr hWndParent;

        bool connection = false;

        public MainWindow()
        {
            InitializeComponent();
            this.Background = (Brush)Application.Current.Resources["WindowBackgroundColor"];

        }

        //Menu clicks
        private void generalMenuClick(object sender, RoutedEventArgs e)
        {
            MenuItem clickedMenuItem = sender as MenuItem;

            if (clickedMenuItem != null)
            {
                // get variable name
                string item = clickedMenuItem.Name;

                switch (item)
                {
                    case "mniOpen":
                        tbxLog.AppendText(item + " was clicked! \n");
                        tbxLog.ScrollToEnd();

                        OpenFileDialog openFileDialog = new OpenFileDialog
                        {
                            Filter = "OBJ and MTL Files (*.obj;*.mtl)|*.obj;*.mtl",
                            Title = "Select a .obj or .mtl file"
                        };

                        if (openFileDialog.ShowDialog() == true)
                        {
                            string filePath = openFileDialog.FileName;
                        }
                        break;

                    case "mniExit":
                        tbxLog.AppendText(item + " was clicked! \n");
                        tbxLog.ScrollToEnd();
                        Application.Current.Shutdown(0);
                        break;


                }
            }
        }


        //Button clicks
        private void generalButtonClick(object sender, RoutedEventArgs e)
        {
            if (!connection)
            {
                MessageBox.Show("No connection established. Please connect to the server first.",
                                "Connection Error",
                                MessageBoxButton.OK,
                                MessageBoxImage.Warning);
                return;
            }

            Button clickedButton = sender as Button;

            if (clickedButton != null)
            {
                // get variable name
                string button = clickedButton.Name;

                switch (button)
                {
                    case "btnLeft":
                        tbxLog.AppendText(button + " was clicked! \n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "btnRight":
                        tbxLog.AppendText(button + " was clicked! \n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "btnUp":
                        tbxLog.AppendText(button + " was clicked! \n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "btnDown":
                        tbxLog.AppendText(button + " was clicked! \n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "btnRx":
                        tbxLog.AppendText(button + "  was clicked! \n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "btnRy":
                        tbxLog.AppendText(button + "  was clicked! \n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "btnLog":
                        tbxLog.AppendText($"{DateTime.Now}: Log entry added.\n");
                        tbxLog.ScrollToEnd();

                        // TreeBuilder testing
                        string jsonTestString;
                        try
                        {
                            jsonTestString = File.ReadAllText(Directory.GetParent(Directory.GetCurrentDirectory()).Parent.Parent.FullName + "\\sampleJSON.txt");
                        }
                        catch (Exception ex)
                        {
                            jsonTestString = selectedUUID;
                        }
                        new TreeBuilder(jsonTestString, trvEntities);
                        jsonTestString = selectedUUID;
                        new TreeBuilder(jsonTestString, trvComponents);
                        break;

                    case "btnToggleB":
                        tbxLog.AppendText(button + "  was clicked! \n");
                        tbxLog.ScrollToEnd();
                        if (gridButtons.Visibility == Visibility.Visible)
                        {
                            gridButtons.Visibility = Visibility.Collapsed;
                        }
                        else
                        {
                            gridSliders.Visibility = Visibility.Collapsed;
                            gridButtons.Visibility = Visibility.Visible;
                        }
                        break;

                    case "btnToggleS":
                        tbxLog.AppendText(button + "  was clicked! \n");
                        tbxLog.ScrollToEnd();
                        if (gridSliders.Visibility == Visibility.Visible)
                        {
                            gridSliders.Visibility = Visibility.Collapsed;
                        }
                        else
                        {
                            gridButtons.Visibility = Visibility.Collapsed;
                            gridSliders.Visibility = Visibility.Visible;
                           
                        }
                        break;
                }
            }
        }


        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            hWndParent = new WindowInteropHelper(this).Handle;
            loader = new GLFWLoader(this, hWndParent);
        
            if (!loader.WindowLoaded() && connection)
            {
                MessageBox.Show("GLFW window not found. Make sure it is created before loading the WPF window.");
            }

        }



        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (loader != null)
            {
                loader.CloseWindow();
            }
            //_ecsApi.close_RS();


        }

        private void btnConnect_Click(object sender, RoutedEventArgs e)
        {
            Button clickedButton = sender as Button;

            while (!connection)
            {
                try
                {
                    StartOtherExe("../../../../../Engine/build/TopLevelProject.exe");
                    _ecsApi = new EcsApi("127.0.0.1", 51234);

                    // initial root-request
                    selectedUUID = _ecsApi.get_root();
                    tbxLog.AppendText(selectedUUID);
                    connection = true; // connection was successful 

                    clickedButton.Background = (Brush)FindResource("WindowPrimaryColor");
                    clickedButton.Foreground = Brushes.Black;

                    Window_Loaded(sender, e);

                }
                catch (InvalidOperationException ex)
                {
                    MessageBoxResult result = MessageBox.Show(ex.Message + "\n" +
                        "Unable to start \"TopLevelProject.exe\"!",
                        "Connection Error",
                        MessageBoxButton.OKCancel,
                        MessageBoxImage.Error);

                    if (result == MessageBoxResult.Cancel)
                    {
                        break;
                    }
                }
            }
        }


        private void rctRenderArea_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (loader != null)
            {
                loader.OnResize();
                this.UpdateLayout();

            }
        }
      

        private void StartOtherExe(string exePath)
        {
            try
            {
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = exePath,
                    UseShellExecute = false,
                    WindowStyle = ProcessWindowStyle.Minimized
                };
                Process.Start(startInfo);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to start the executable. Error: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}