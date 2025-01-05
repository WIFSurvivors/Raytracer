﻿using System.Text;
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
using System.Reflection;

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
        string rotation = "xpos";

        private EcsNode selectedEntityOptionItem;

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
                                string pathSentStatus = _ecsApi.json_import('"' + filePath + '"');
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
                        sldX.Value -= 10;
                        SliderPreviewMouseUp(sldX, null);
                        break;

                    case "btnRight":
                        sldX.Value += 10;
                        SliderPreviewMouseUp(sldX, null);
                        break;

                    case "btnUp":
                        sldY.Value += 10;
                        SliderPreviewMouseUp(sldY, null);
                        break;

                    case "btnDown":
                        sldY.Value -= 10;
                        SliderPreviewMouseUp(sldY, null);
                        break;

                    case "btnBack":
                        sldZ.Value -= 10;
                        SliderPreviewMouseUp(sldZ, null);
                        break;

                    case "btnForward":
                        sldZ.Value += 10;
                        SliderPreviewMouseUp(sldZ, null);
                        break;

                    case "btnRotate":

                        switch (rotation)
                        {
                            case "xpos":
                                sldRx.Value += 10;
                                SliderPreviewMouseUp(sldRx, null);
                                break;

                            case "xneg":
                                sldRx.Value -= 10;
                                SliderPreviewMouseUp(sldRx, null);
                                break;

                            case "ypos":
                                sldRy.Value += 10;
                                SliderPreviewMouseUp(sldRy, null);
                                break;

                            case "yneg":
                                sldRy.Value -= 10;
                                SliderPreviewMouseUp(sldRy, null);
                                break;

                            case "zpos":
                                sldRz.Value += 10;
                                SliderPreviewMouseUp(sldRz, null);
                                break;

                            case "zneg":
                                sldRz.Value -= 10;
                                SliderPreviewMouseUp(sldRz, null);
                                break;
                        }

                        break;

                    case "btnSwitchR":
                        if (rotation == "xpos")
                        {
                            rotation = "xneg";
                            imgRotation.Source = new BitmapImage(new Uri("Images/arrow-rotate-xneg.png", UriKind.Relative));
                        }
                        else if (rotation == "xneg")
                        {
                            rotation = "ypos";
                            imgRotation.Source = new BitmapImage(new Uri("Images/arrow-rotate-ypos.png", UriKind.Relative));
                        }
                        else if (rotation == "ypos")
                        {
                            rotation = "yneg";
                            imgRotation.Source = new BitmapImage(new Uri("Images/arrow-rotate-yneg.png", UriKind.Relative));
                        }
                        else if (rotation == "yneg")
                        {
                            rotation = "zpos";
                            imgRotation.Source = new BitmapImage(new Uri("Images/arrow-rotate-zpos.png", UriKind.Relative));
                        }
                        else if (rotation == "zpos")
                        {
                            rotation = "zneg";
                            imgRotation.Source = new BitmapImage(new Uri("Images/arrow-rotate-zneg.png", UriKind.Relative));
                        }
                        else if (rotation == "zneg")
                        {
                            rotation = "xpos";
                            imgRotation.Source = new BitmapImage(new Uri("Images/arrow-rotate-xpos.png", UriKind.Relative));
                        }
                        break;

                    case "btnRz":
                        sldRz.Value += 10;
                        break;

                    case "btnLog":

                        if (gridLog.Visibility == Visibility.Collapsed)
                        {
                            gridLog.Visibility = Visibility.Visible;
                            ColumnLog.Width = new GridLength(1, GridUnitType.Star);
                        }
                        else
                        {
                            gridLog.Visibility = Visibility.Collapsed;
                            ColumnLog.Width = new GridLength(0);
                        }

                        // TreeBuilder testing
                        entityBuilder = new TreeBuilder(trvEntities, this);
                       entityBuilder.BuildTreeFromJson(ReceivedEcsJsonString);
                       break;

                    case "btnToggleLog":

                        if (tbxLog.Visibility == Visibility.Visible)
                        {
                            tbxLog.Visibility = Visibility.Collapsed;
                            tbxLogEngine.Visibility = Visibility.Visible;

                            // Read Engine Log 
                            string log;
                            try
                            {
                                log = File.ReadAllText(Directory.GetParent(Directory.GetCurrentDirectory()).Parent.Parent.FullName + "\\sampleJSON.txt"); // to replace with log directory
                            }
                            catch (Exception ex)
                            {
                                log = "Engine Log unavailable!";
                            }

                            tbxLogEngine.AppendText(log);
                            tbxLogEngine.ScrollToEnd();

                        }
                        else
                        {
                            tbxLogEngine.Visibility = Visibility.Collapsed;
                            tbxLog.Visibility = Visibility.Visible;
                            
                        };
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

        private void SliderPreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            Slider? changedSlider = sender as Slider;

            if (changedSlider != null)
            { 
                changedSlider.Minimum = changedSlider.Value - 10;
                changedSlider.Maximum = changedSlider.Value + 10;

                string slider = changedSlider.Name;

                // Round the slider values for lables
                double minValue = Math.Round(changedSlider.Minimum, 2);
                double medValue = Math.Round(changedSlider.Value, 2);
                double maxValue = Math.Round(changedSlider.Maximum, 2);


                // Update min, med, max labels
                switch (slider)
                {
                    case "sldX":
                        lblXMin.Content = minValue;
                        lblXMed.Content = medValue;
                        lblXMax.Content = maxValue;
                        break;

                    case "sldY":
                        lblYMin.Content = minValue;
                        lblYMed.Content = medValue;
                        lblYMax.Content = maxValue;
                        break;

                    case "sldZ":
                        lblZMin.Content = minValue;
                        lblZMed.Content = medValue;
                        lblZMax.Content = maxValue;
                        break;

                    case "sldRx":
                        lblRxMin.Content = minValue;
                        lblRxMed.Content = medValue;
                        lblRxMax.Content = maxValue;
                        break;

                    case "sldRy":
                        lblRyMin.Content = minValue;
                        lblRyMed.Content = medValue;
                        lblRyMax.Content = maxValue;
                        break;

                    case "sldRz":
                        lblRzMin.Content = minValue;
                        lblRzMed.Content = medValue;
                        lblRzMax.Content = maxValue;
                        break;

                    case "sldZoom":
                        lblZoomMin.Content = minValue; 
                        lblZoomMed.Content = medValue;
                        lblZoomMax.Content = maxValue;
                        break;
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
        private async void btnConnect_Click(object sender, RoutedEventArgs e)
        {
            Button? clickedButton = sender as Button;

            while (!connection)
            {
                try
                {
                    StartOtherExe("../../../../../Engine/build/Engine.exe");
                    await Task.Delay(500);
                    _ecsApi = new EcsApi("127.0.0.1", 51234);

                    // initial root-request
                    ReceivedEcsJsonString = _ecsApi.get_root();
                    //TODO set Bounces from ECS
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
            selectedEntityOptionItem = e.NewValue as EcsNode;
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


        private void cmbAddComponent_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cmbAddComponent.SelectedItem is ComboBoxItem selectedItem && selectedItem.Tag != null)
            {
                string methodTag = selectedItem.Tag.ToString();
                switch (methodTag)
                {
                    case "RenderMethod":
                        AddRenderComponent();
                        break;
                    case "LightMethod":
                        AddLightComponent();
                        break;
                    case "CameraMethod":
                        AddCameraComponent();
                        break;
                    case "NoMethod":
                        break;
                }
                // Reset the ComboBox
                cmbAddComponent.SelectedIndex = -1; // Clear selection
                cmbAddComponent.Text = "Select Component";

            }
        }

        
        private void AddRenderComponent()
        {
            if (_ecsApi != null)
            {

                try
                {
                    // TODO: get the parent Entity to send with command 
                    //string pathSentStatus = _ecsApi.create_component( type siehe createComponent command im if statement);
                    tbxLog.AppendText("Render Component Added!\n");
                }
                catch (InvalidOperationException ex)
                {
                    //TODO
                }
            }
        }

        private void AddLightComponent()
        {
            if (_ecsApi != null)
            {

                try
                {
                    // TODO: get the parent Entity to send with command 
                    //string pathSentStatus = _ecsApi.create_component( type siehe createComponent command im if statement);
                    tbxLog.AppendText("Render Component Added!\n");
                }
                catch (InvalidOperationException ex)
                {
                    //TODO
                }
            }
        }

        private void AddCameraComponent()
        {
            if (_ecsApi != null)
            {

                try
                {
                    // TODO: get the parent Entity to send with command 
                    //string pathSentStatus = _ecsApi.create_component( type siehe createComponent command im if statement);
                    tbxLog.AppendText("Render Component Added!\n");
                }
                catch (InvalidOperationException ex)
                {
                    //TODO
                }
            }
        }




    }
}