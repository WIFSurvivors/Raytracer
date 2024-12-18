﻿﻿using System.Text;
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
using System.Reflection.PortableExecutable;
using System.Text.Json;
using System;

namespace RaytracerGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private EcsApi? _ecsApi;
        private string ReceivedEcsJsonString;
        private GLFWLoader loader;
        private IntPtr hWndParent;
        private TreeBuilder entityBuilder;
        private TreeBuilder entityOptionsBuilder;

        private TreeBuilder componentBuilder;
        private TreeBuilder componentOptionsBuilder;

        bool connection = false;

        public MainWindow()
        {
            InitializeComponent();
            this.Background = (Brush)Application.Current.Resources["WindowBackgroundColor"];

        }

        //Menu clicks
        private void generalMenuClick(object sender, RoutedEventArgs e)
        {
            MenuItem? clickedMenuItem = sender as MenuItem;
            OpenFileDialog openFileDialog;
            string filePath = "";

            if (clickedMenuItem != null)
            {
                // get variable name
                string item = clickedMenuItem.Name;


                switch (item)
                {
                    case "mniOpen":
                        tbxLog.AppendText(item + " was clicked! \n");
                        tbxLog.ScrollToEnd();

                        openFileDialog = new OpenFileDialog
                        {
                            Filter = "OBJ and MTL Files (*.obj;*.mtl)|*.obj;*.mtl",
                            Title = "Select a .obj or .mtl file"
                        };

                        if (openFileDialog.ShowDialog() == true)
                        {
                            filePath = openFileDialog.FileName;
                        }
                        break;

                    case "mniImport":
                        tbxLog.AppendText(item + " was clicked! \n");
                        tbxLog.ScrollToEnd();

                        openFileDialog = new OpenFileDialog
                        {
                            Filter = "JSON File (*.json)|*.json",
                            Title = "Select a JSON Scene file",
                            InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile) + "\\Downloads"
                        };

                        if (openFileDialog.ShowDialog() == true)
                        {
                            filePath = openFileDialog.FileName;
                            tbxLog.AppendText("ScenePath : " + filePath);

                        }

                        if (_ecsApi != null)
                        {

                            try
                            {
                                // send JSON path
                                string pathSentStatus = _ecsApi.post_ScenePath(filePath);
                                tbxLog.AppendText("pathSentStatus : " + pathSentStatus);


                            }
                            catch (InvalidOperationException ex)
                            {
                                //TODO
                            }
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

            Button? clickedButton = sender as Button;

            if (clickedButton != null)
            {
                // get variable name
                string button = clickedButton.Name;

                tbxLog.AppendText(button + " was clicked! \n");
                tbxLog.ScrollToEnd();

                switch (button)
                {
                    case "btnLeft":
                        //_ecsApi.post...
                        break;

                    case "btnRight":
                        //_ecsApi.post...
                        break;

                    case "btnUp":
                        //_ecsApi.post...
                        break;

                    case "btnDown":
                        //_ecsApi.post...
                        break;

                    case "btnBack":
                        //_ecsApi.post...
                        break;

                    case "btnForward":
                        //_ecsApi.post...
                        break;

                    case "btnRx":
                        //_ecsApi.post...
                        break;

                    case "btnRy":
                        //_ecsApi.post...
                        break;

                    case "btnRz":
                        //_ecsApi.post...
                        break;

                    case "btnLog":

                        if (tbxLog.Visibility == Visibility.Collapsed)
                        {
                            tbxLog.Visibility = Visibility.Visible;
                            ColumnLog.Width = new GridLength(1, GridUnitType.Star);
                        }
                        else
                        {
                            tbxLog.Visibility = Visibility.Collapsed;
                            ColumnLog.Width = new GridLength(0);
                        }

                        // TreeBuilder testing
                        entityBuilder = new TreeBuilder(trvEntities, this);
                       entityBuilder.BuildTreeFromJson(ReceivedEcsJsonString);
                       break;

                    case "btnToggleB":

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

        private void SliderValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Slider? changedSlider = sender as Slider;

            //TODO get information (which textbox must be updated)
            //TODO get xyz from textbox and update _ecsapi
            //var UUID;
            //var x;
            //var y;
            //var z;

            if (changedSlider != null)
            {
                string slider = changedSlider.Name;

                // Round the slider value to 2 decimal places
                double roundedValue = Math.Round(e.NewValue, 2);

                // Prevent unnecessary updates
                if (changedSlider.Value != roundedValue)
                {
                    changedSlider.Value = roundedValue;
                }

                // Log the updated value
                tbxLog.AppendText($"{slider} value changed to: {roundedValue}\n");
                tbxLog.ScrollToEnd();

                if (_ecsApi != null)
                {
                    try
                    {
                        switch (slider)
                        {
                            // Send rounded values to the server
                            case "sldX":
                                //_ecsApi.move_entity(UUID, x, y, z);
                                break;

                            case "sldY":
                                //_ecsApi.move_entity(UUID, x, y, z);
                                break;

                            case "sldZ":
                                //_ecsApi.move_entity(UUID, x, y, z);
                                break;

                            case "sldRx":
                                //_ecsApi.rotate_entity(UUID, x, y, z);
                                break;

                            case "sldRy":
                                //_ecsApi.rotate_entity(UUID, x, y, z);
                                break;

                            case "sldRz":
                                //_ecsApi.rotate_entity(UUID, x, y, z);
                                break;

                            case "sldZoom":
                                //_ecsApi.scale_entity(UUID, x, y, z);
                                break;
                        }
                    }
                    catch (InvalidOperationException ex)
                    {
                        tbxLog.AppendText($"Error updating server with {slider}: {ex.Message}\n");
                        tbxLog.ScrollToEnd();
                    }
                }
            }
        }



        //Window Handling and first connect 
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
            Button? clickedButton = sender as Button;

            while (!connection)
            {
                try
                {
                    StartOtherExe("../../../../../Engine/build/Engine.exe");
                    _ecsApi = new EcsApi("127.0.0.1", 51234);

                    // initial root-request
                    ReceivedEcsJsonString = _ecsApi.get_root();
                    tbxLog.AppendText(ReceivedEcsJsonString);
                    connection = true; // connection was successful 

                    clickedButton.Background = (Brush)FindResource("WindowPrimaryColor");
                    clickedButton.Foreground = Brushes.Black;

                    Window_Loaded(sender, e);

                }
                catch (InvalidOperationException ex)
                {
                    MessageBoxResult result = MessageBox.Show(ex.Message + "\n" +
                        "Unable to start \"Engine.exe\"!",
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
        private void tbxLog_TextChanged(object sender, TextChangedEventArgs e)
        {
            tbxLog.ScrollToEnd();
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





        //Entities Update
        private void trvEntities_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue is TreeViewItem selectedItem)
            {
                string header = selectedItem.Header.ToString();
                if (selectedItem.Tag is TreeItemData tagData)
                {
                    string uuid = tagData.UUID;       // Access UUID
                    string name = tagData.Name;       // Access Name
                    int childrenCount = tagData.Children; // Access Children count

                    //tbxLog.AppendText($"{header} was clicked with UUID: {uuid}, Name: {name}, Children: {childrenCount}\n");

                    UpdateEntities(uuid, e);
                    UpdateEntitiesOptions(uuid, e);
                    UpdateComponents(uuid, e);
                }
                else
                {
                    tbxLog.AppendText($"{header} was clicked, but no valid Tag data found.\n");
                }

            }
        }
        private void UpdateEntities(string uuid, RoutedPropertyChangedEventArgs<object> e)
        {
            string? ecsJsonNode = _ecsApi.get_child_entities(uuid);
            //tbxLog.AppendText("Entity Update : JSON\n\n " + ecsJsonNode + "\n\n");


            // Deserialize the JSON
            var updatedNode = JsonSerializer.Deserialize<EcsEntityNode>(ecsJsonNode);
            if (updatedNode == null)
            {
                tbxLog.AppendText("EntitiyJSON = null.\n");
                return;
            }

            // Locate the TreeViewItem in the TreeView
            if (e.NewValue is TreeViewItem selectedItem)
            {
                // Clear existing children
                selectedItem.Items.Clear();

                // Rebuild the children based on the new data
                entityBuilder.CreateChildItems(updatedNode, selectedItem);
                //tbxLog.AppendText($"Updated children for UUID: {uuid}\n");
            }
        }
        public void UpdateEntitiesOptions(string uuid, RoutedPropertyChangedEventArgs<object>? e)
        {
            string? ecsJsonNode = _ecsApi.get_entity_options(uuid);
            tbxLog.AppendText("EntityList Update : JSON\n\n " + ecsJsonNode + "\n\n");

            if (ecsJsonNode == null)
            {
                tbxLog.AppendText("JSON = null.\n");
                return;
            }

            entityOptionsBuilder = new TreeBuilder(trvEntitiesOptions,this);
            entityOptionsBuilder.BuildTreeFromOptions(ecsJsonNode);
        }
        public void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (sender is TextBox textBox)
            {
                // Log the TextBox change
                tbxLog.AppendText($"TextBox '{textBox.Name}' text changed to: {textBox.Text}\n");

                // Attempt to parse the text and update the appropriate slider
                switch (textBox.Name)
                {
                    case "x":
                        if (double.TryParse(textBox.Text, out double valueX))
                        {
                            sldX.Value = valueX;
                        }
                        else
                        {
                            tbxLog.AppendText("Invalid value for X.\n");
                        }
                        break;

                    case "y":
                        if (double.TryParse(textBox.Text, out double valueY))
                        {
                            sldY.Value = valueY;
                        }
                        else
                        {
                            tbxLog.AppendText("Invalid value for Y.\n");
                        }
                        break;

                    case "z":
                        if (double.TryParse(textBox.Text, out double valueZ))
                        {
                            sldZ.Value = valueZ;
                        }
                        else
                        {
                            tbxLog.AppendText("Invalid value for Z.\n");
                        }
                        break;

                    default:
                        tbxLog.AppendText($"TextBox '{textBox.Name}' does not match known sliders.\n");
                        break;
                }
            }
        }
        private void trvEntitiesOptions_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            return;
        }


        /// Components Update
        private void trvComponents_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue is TreeViewItem selectedItem)
            {
                string? header = selectedItem.Header.ToString();
                if (selectedItem.Tag is TreeItemData tagData)
                {
                    string uuid = tagData.UUID;       // Access UUID
                    string name = tagData.Name;       // Access Name
                    int childrenCount = tagData.Children; // Access Children count

                    UpdateComponentsOptions(uuid, e);
                }
                else
                {
                    tbxLog.AppendText($"{header} was clicked, but no valid Tag data found.\n");
                }
            }
        }

        private void UpdateComponents(string uuidEntity, RoutedPropertyChangedEventArgs<object> e)
        {
            string? componentsJsonNode = _ecsApi.get_components(uuidEntity);
            try
            {
                //PROBLEM: Exception when deserialized, no class for object existent:
                //JSON: {"components":[{"uuid":"67949d3e-de96-487f-a03c-5b900da73e4b"}]}
                var updatedNode = JsonSerializer.Deserialize<string>(componentsJsonNode);
                componentBuilder = new TreeBuilder(trvComponents, this);

            if (componentsJsonNode == null)
            {
                tbxLog.AppendText($"No valid component data found for UUID: {uuidEntity}.\n");
                return;
            }

            componentBuilder.BuildTreeFromJson(updatedNode);
            }
            catch (Exception ex)
            {
                tbxLog.AppendText("JSON is null for uuid " + uuidEntity+ "\n");
            }

        }

        public void UpdateComponentsOptions(string uuid, RoutedPropertyChangedEventArgs<object>? e)
        {
            string? componentsJsonNode = _ecsApi.get_component_options(uuid);
            tbxLog.AppendText("ComponentList Update : JSON\n\n " + componentsJsonNode + "\n\n");

            if (componentsJsonNode == null)
            {
                tbxLog.AppendText("JSON = null.\n");
                return;
            }

            componentOptionsBuilder = new TreeBuilder(trvComponentsOptions, this);
            componentOptionsBuilder.BuildTreeFromOptions(componentsJsonNode);
        }

        private void trvComponentsOptions_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
           
        }




    }
}