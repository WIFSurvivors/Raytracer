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
        public MainWindow()
        {
            InitializeComponent();
            this.Background = (Brush)Application.Current.Resources["WindowBackgroundColor"];

            bool connection = false;

            while (!connection)
            {
                try
                {
                    _ecsApi = new EcsApi("127.0.0.1", 51234);

                    // initial root-request
                    selectedUUID = _ecsApi.get_root();
                    tbxLog.AppendText(selectedUUID);
                    connection = true; // connection was successful
                }
                catch (InvalidOperationException ex)
                {
                    MessageBoxResult result = MessageBox.Show(ex.Message + "\n" +
                        "Please start the \"TopLevelProject.exe\" and try again!",
                        "Connection Error",
                        MessageBoxButton.OKCancel,
                        MessageBoxImage.Error);

                    if (result == MessageBoxResult.Cancel)
                    {
                        // close application
                        Application.Current.Shutdown();
                        break;
                    }
                }
            }

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




        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            hWndParent = new WindowInteropHelper(this).Handle;
            loader = new GLFWLoader(this, hWndParent);
            loader.WindowLoaded();

        }



        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _ecsApi.close_RS();


        }

        private void RenderArea_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (loader != null)
            {
                loader.OnResize();
            }
        }


        //Button clicks
        private void generalButtonClick(object sender, RoutedEventArgs e)
        {
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
                        TreeBuilder treeBuilder = new TreeBuilder(selectedUUID, trv_Entities);
                        break;
                }
            }
        }
    }
}