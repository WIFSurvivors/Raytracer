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

using Microsoft.Win32;

namespace RaytracerGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.Background = (Brush)Application.Current.Resources["WindowBackgroundColor"];
        }

        private void clickOpen(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Filter = "OBJ and MTL Files (*.obj;*.mtl)|*.obj;*.mtl",
                Title = "Select a .obj or .mtl file"
            };



            if (openFileDialog.ShowDialog() == true)
            {
                string filePath = openFileDialog.FileName;

                // use filePath in TCP paket

            }
        }
        
        //Menu clicks
        private void clickSave(object sender, RoutedEventArgs e)
        {
           
        }

        private void clickImportScene(object sender, RoutedEventArgs e)
        {

        }
        private void clickExportScene(object sender, RoutedEventArgs e)
        {

        }

        private void clickExit(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown(0); 
        }


        //Button clicks
        private void generalButtonClick(object sender, RoutedEventArgs e)
        {
            Button clickedButton = sender as Button;

            if (clickedButton != null)
            {
                // get variable name
                string button = clickedButton.Name;
                // string buttonContent = clickedButton.Content.ToString();

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
                        break;
                }
            }
        }

        private void btn_TCPTest_Click(object sender, RoutedEventArgs e)
        {
            tbxLog.AppendText($"{DateTime.Now}: Log entry added.\n");

            
            tbxLog.ScrollToEnd();
        }
    }
}