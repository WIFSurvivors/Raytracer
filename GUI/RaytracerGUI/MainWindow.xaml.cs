using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
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

                // Code for handle selected file
                //
                // test
                //
                //
            }
        }

        private void clickSave(object sender, RoutedEventArgs e)
        {

        }


        private void clickExit(object sender, RoutedEventArgs e)
        {

            Application.Current.Shutdown(0);
        }
    }
}