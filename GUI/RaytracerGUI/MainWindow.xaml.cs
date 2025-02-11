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
using System.Threading.Tasks;
using System.Reflection.PortableExecutable;
using System.Text.Json;
using System;
using System.Reflection;
using System.Windows.Controls.Primitives;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;
using System;
using System.Windows.Forms;
using Xceed.Wpf.Toolkit.Primitives;

namespace RaytracerGUI
{
    public partial class MainWindow : Window
    {
        private EcsApi? _ecsApi;
        public bool shouldUpdate = true;
        private string ReceivedEcsJsonString;
        private GLFWLoader loader;
        private IntPtr hWndParent;

        private TreeBuilder _entityBuilder;
        private TreeBuilder _entityOptionsBuilder;

        private TreeBuilder _componentBuilder;
        private TreeBuilder _componentOptionsBuilder;

        bool connection = false;
        string rotation = "ypos";
        bool textboxChange = false;
        int sliderOffset = 10;

        string currentEntityUUID = "uuid";
        string currentComponentUUID = "uuid";
        string deleteUUID = "uuid";
        public string rootUUID = "uuid";




        public MainWindow()
        {
            InitializeComponent();
            this.Background = (Brush)Application.Current.Resources["WindowBackgroundColor"];
            this.KeyDown += generalKeyDown;

        }

        /// <summary>
        /// When a Option of the "File" Menu is clicked, this method is called and handles every interaction from the dropdown menus
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FileMenuClick(object sender, RoutedEventArgs e)
        {
            //convert sender to type MenuItem
            MenuItem? clickedMenuItem = sender as MenuItem;
            OpenFileDialog openFileDialog;
            SaveFileDialog saveFileDialog;
            string filePath = "";

            //check for click
            if (clickedMenuItem != null)
            {
                // get variable name
                string item = clickedMenuItem.Name;
                //tbxLog.AppendText(item + " was clicked! \n");

                switch (item)
                {
                    //Imports the JSON Scene file and sends it to the ecsapi
                    case "mniImport":

                        if (_ecsApi != null && connection)
                        {
                            openFileDialog = new OpenFileDialog
                            {
                                Filter = "JSON File (*.json)|*.json",
                                Title = "Select a JSON Scene file"
                            };

                            if (openFileDialog.ShowDialog() == true)
                            {
                                filePath = openFileDialog.FileName;
                                tbxLog.AppendText("ImportScenePath : " + filePath + "\n");
                            }

                            if (filePath.Equals(""))
                            {
                                return;
                            }

                            try
                            {
                                // send JSON path
                                string pathSentStatus = _ecsApi.json_import(filePath);
                                tbxLog.AppendText("pathSentStatus : " + pathSentStatus + "\n");


                                _entityBuilder = new TreeBuilder(trvEntities, this);
                                ReceivedEcsJsonString = _ecsApi.get_root();
                                _entityBuilder.BuildTreeFromJson(ReceivedEcsJsonString);

                            }
                            catch (InvalidOperationException ex)
                            {
                                tbxLog.AppendText(ex.ToString());
                            }
                        }
                        else
                        {
                            MessageBox.Show("No active connection!", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                        }
                        break;



                    //Selects the export path + file name and sends it to the ecsapi
                    case "mniExport":

                        if (_ecsApi != null && connection)
                        {
                            saveFileDialog = new SaveFileDialog
                            {
                                Filter = "JSON File (*.json)|*.json",
                                Title = "Save JSON Scene file",
                                FileName = "scene.json"
                            };

                            if (saveFileDialog.ShowDialog() == true)
                            {
                                filePath = saveFileDialog.FileName;
                                tbxLog.AppendText("ExportScenePath : " + filePath + "\n");
                            }

                            if (filePath.Equals(""))
                            {
                                return;
                            }

                            try
                            {
                                // send export path + file name
                                string pathSentStatus = _ecsApi.export_Json(filePath);
                                tbxLog.AppendText("pathSentStatus : " + pathSentStatus + "\n");

                                // checks if export was successful
                                if (pathSentStatus.Contains("Scene exported on path:"))
                                {
                                    MessageBox.Show(pathSentStatus, "Export Complete", MessageBoxButton.OK, MessageBoxImage.Information);
                                }
                                else
                                {
                                    MessageBox.Show(pathSentStatus, "Export Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                                }

                            }
                            catch (InvalidOperationException ex)
                            {
                                tbxLog.AppendText(ex.ToString());
                            }
                            
                        }
                        else
                        {
                            MessageBox.Show("No active connection!", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                        }
                        break;



                    case "mniHelp":
                        MessageBox.Show("ctrl+z = undo last undoable command \n" +
                            "del = remove selected entity/component \n \n" +
                            "click on \"obj_path\" to select a new .obj - file");
                        break;


                    case "mniExit":
                        Application.Current.Shutdown(0);
                        break;


                }
            }
        }
        /// <summary>
        /// When a Option of the "Add" Menu is clicked, this method is called and adds the corresponding 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AddMenuClick(object sender, RoutedEventArgs e)
        {
            if (sender is MenuItem clickedMenuItem)
            {
                // Der Name des angeklickten Menüelements wird verwendet
                string itemName = clickedMenuItem.Name;
                //tbxLog.AppendText($"{itemName} was clicked!\n");
                if (!currentEntityUUID.Equals("uuid"))
                {
                    switch (itemName)
                    {
                        case "mniAddRender":
                            AddRenderComponent();
                            break;

                        case "mniAddLight":
                            AddLightComponent();
                            break;

                        case "mniAddCamera":
                            AddCameraComponent();
                            break;

                        case "mniAddEntity":
                            try
                            {
                                AddEntity();
                                ReceivedEcsJsonString = _ecsApi.get_root();
                                _entityBuilder.BuildTreeFromJson(ReceivedEcsJsonString);
                                break;
                            }
                            catch (Exception ex)
                            {
                                tbxLog.AppendText("Adding Entity failed with Exception: " + ex.Message);
                                break;
                            }
                    }
                }
            }
        }

        private void SettingsMenuClick(object sender, RoutedEventArgs e)
        {
            if (sender is MenuItem clickedMenuItem && _ecsApi != null)
            {
                // Der Name des angeklickten Menüelements wird verwendet
                string itemName = clickedMenuItem.Name;
                //tbxLog.AppendText($"{itemName} was clicked!\n");

                    switch (itemName)
                    {
                        case "mniPrintECS":
                        try
                        {
                            _ecsApi.print_table();
                            
                        }
                        catch (Exception ex)
                        {
                            tbxLog.AppendText("Exception thrown: " + ex);
                        }
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

                //tbxLog.AppendText(button + " was clicked! \n");

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
                            imgRotation.Source = new BitmapImage(new Uri("Images/rotate-xneg.png", UriKind.Relative));
                        }
                        else if (rotation == "xneg")
                        {
                            rotation = "ypos";
                            imgRotation.Source = new BitmapImage(new Uri("Images/rotate-ypos.png", UriKind.Relative));
                        }
                        else if (rotation == "ypos")
                        {
                            rotation = "yneg";
                            imgRotation.Source = new BitmapImage(new Uri("Images/rotate-yneg.png", UriKind.Relative));
                        }
                        else if (rotation == "yneg")
                        {
                            rotation = "zpos";
                            imgRotation.Source = new BitmapImage(new Uri("Images/rotate-zpos.png", UriKind.Relative));
                            imgRotation.Width = 30;
                            imgRotation.Height = 30;
                        }
                        else if (rotation == "zpos")
                        {
                            rotation = "zneg";
                            imgRotation.Source = new BitmapImage(new Uri("Images/rotate-zneg.png", UriKind.Relative));
                        }
                        else if (rotation == "zneg")
                        {
                            rotation = "xpos";
                            imgRotation.Source = new BitmapImage(new Uri("Images/rotate-xpos.png", UriKind.Relative));
                            imgRotation.Width = 40;
                            imgRotation.Height = 40;
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

                        break;

                    case "btnToggleLog":

                        if (tbxLog.Visibility == Visibility.Visible)
                        {
                            tbxLog.Visibility = Visibility.Collapsed;
                            tbxLogEngine.Visibility = Visibility.Visible;


                            // Read Engine Log 
                            string engineLog;
                            string log;

                            engineLog = _ecsApi.get_log_path();

                            try
                            {
                                log = File.ReadAllText(engineLog);
                            }
                            catch (Exception ex)
                            {
                                log = "Engine Log unavailable!" + engineLog;
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
                            if (gridFov.Visibility == Visibility.Visible)
                            {
                                gridFov.Visibility = Visibility.Collapsed;
                                lblFov.Visibility = Visibility.Collapsed;

                                gridSx.Visibility = Visibility.Visible;
                                gridSy.Visibility = Visibility.Visible;
                                gridSz.Visibility = Visibility.Visible;
                                lblSca.Visibility = Visibility.Visible;

                                // adapt column width for horizontal scaling sliders
                                gridSliders.ColumnDefinitions[0].Width = new GridLength(1, GridUnitType.Star);

                            }
                            else
                            {
                                gridSx.Visibility = Visibility.Collapsed;
                                gridSy.Visibility = Visibility.Collapsed;
                                gridSz.Visibility = Visibility.Collapsed;
                                lblSca.Visibility = Visibility.Collapsed;

                                gridFov.Visibility = Visibility.Visible;
                                lblFov.Visibility = Visibility.Visible;

                                // adapt column width for vertical fov slider
                                gridSliders.ColumnDefinitions[0].Width = new GridLength(128);

                            }
                        }
                        else
                        {
                            gridButtons.Visibility = Visibility.Collapsed;
                            gridSliders.Visibility = Visibility.Visible;

                        }
                        break;

                    case "btnScreenshot":
                        TakeScreenshot();
                        break;
                }
            }
        }




        private void SliderValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Slider? changedSlider = sender as Slider;

            if (changedSlider != null && textboxChange)
            {
                SliderPreviewMouseUp(changedSlider.Name, null);
            }

            double roundedValue = Math.Round(e.NewValue, 2);
            tbxLog.AppendText($"{changedSlider.Name} value changed to: {roundedValue}\n");
        }


        private void SliderEcsApiUpdate(int sliderType)
        {
            string UUID = currentEntityUUID;
            float x;
            float y;
            float z;


            if (_ecsApi != null)
            {
                try
                {
                    if (sliderType == 0)
                    {
                        x = (float)sldX.Value;
                        y = (float)sldY.Value;
                        z = (float)sldZ.Value;
                        if (shouldUpdate)
                        {
                            _ecsApi.move_entity(UUID, x, y, z);
                        }
                    }
                    else if (sliderType == 1)
                    {
                        x = (float)sldRx.Value;
                        y = (float)sldRy.Value;
                        z = (float)sldRz.Value;
                        if (shouldUpdate)
                        {
                            _ecsApi.rotate_entity(UUID, x, y, z);
                        }
                    }
                    else if (sliderType == 2)
                    {
                        float fov = (float)sldFov.Value;
                        _ecsApi.set_fov(fov);
                    }
                    else if (sliderType == 3)
                    {
                        x = (float)sldSx.Value;
                        y = (float)sldSy.Value;
                        z = (float)sldSz.Value;
                        if (shouldUpdate)
                        {
                            _ecsApi.scale_entity(UUID, x, y, z);
                        }
                    }

                }
                catch (InvalidOperationException ex)
                {
                    tbxLog.AppendText($"Error updating server: {ex.Message}\n");
                    tbxLog.ScrollToEnd();
                }
            }

        }

        private void SliderPreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            if (_entityOptionsBuilder == null)
            {
                tbxLog.AppendText("No entity or component with options has been selected!");
                return;
            }

            Slider? changedSlider = sender as Slider;

            if (changedSlider != null)
            {
                changedSlider.Minimum = changedSlider.Value - sliderOffset;
                changedSlider.Maximum = changedSlider.Value + sliderOffset;

                string slider = changedSlider.Name;

                // Round the slider values for lables
                double minValue = Math.Round(changedSlider.Minimum, 2);
                double medValue = Math.Round(changedSlider.Value, 2);
                double maxValue = Math.Round(changedSlider.Maximum, 2);


                // Update 
                switch (slider)
                {
                    case "sldX":
                        lblXMin.Content = minValue.ToString().Replace(".", ",");
                        lblXMed.Content = medValue.ToString().Replace(".", ",");
                        lblXMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("traX")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(0);
                        break;

                    case "sldY":
                        lblYMin.Content = minValue.ToString().Replace(".", ",");
                        lblYMed.Content = medValue.ToString().Replace(".", ",");
                        lblYMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("traY")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(0);
                        break;

                    case "sldZ":
                        lblZMin.Content = minValue.ToString().Replace(".", ",");
                        lblZMed.Content = medValue.ToString().Replace(".", ",");
                        lblZMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("traZ")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(0);
                        break;

                    case "sldRx":
                        lblRxMin.Content = minValue.ToString().Replace(".", ",");
                        lblRxMed.Content = medValue.ToString().Replace(".", ",");
                        lblRxMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("rotX")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(1);
                        break;

                    case "sldRy":
                        lblRyMin.Content = minValue.ToString().Replace(".", ",");
                        lblRyMed.Content = medValue.ToString().Replace(".", ",");
                        lblRyMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("rotY")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(1);
                        break;

                    case "sldRz":
                        lblRzMin.Content = minValue.ToString().Replace(".", ",");
                        lblRzMed.Content = medValue.ToString().Replace(".", ",");
                        lblRzMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("rotZ")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(1);
                        break;

                    case "sldFov":
                        if (changedSlider.Value < 10)
                        {
                            changedSlider.Value = 10;
                        }
                        else if (changedSlider.Value > 100)
                        {
                            changedSlider.Value = 100;
                        }

                        changedSlider.Minimum = changedSlider.Value - 25;
                        changedSlider.Maximum = changedSlider.Value + 25;
                        medValue = Math.Round(changedSlider.Value, 0);
                        lblFovMed.Content = medValue;

                        SliderEcsApiUpdate(2);
                        break;

                    case "sldSx":
                        lblSxMin.Content = minValue.ToString().Replace(".", ",");
                        lblSxMed.Content = medValue.ToString().Replace(".", ",");
                        lblSxMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("scaX")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(3);
                        break;

                    case "sldSy":
                        lblSyMin.Content = minValue.ToString().Replace(".", ",");
                        lblSyMed.Content = medValue.ToString().Replace(".", ",");
                        lblSyMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("scaY")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(3);
                        break;

                    case "sldSz":
                        lblSzMin.Content = minValue.ToString().Replace(".", ",");
                        lblSzMed.Content = medValue.ToString().Replace(".", ",");
                        lblSzMax.Content = maxValue.ToString().Replace(".", ",");

                        _entityOptionsBuilder.GetTextBox("scaZ")?.SetValue(TextBox.TextProperty, medValue.ToString());
                        SliderEcsApiUpdate(3);
                        break;

                }


            }
        }

        private void nudBounces_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue != null && _ecsApi != null)
            {
                int newValue = (int)e.NewValue;
                try
                {
                    string? result = _ecsApi.set_bounces(newValue);
                    tbxLog.AppendText(result + "\n");
                }
                catch (Exception ex)
                {
                    tbxLog.AppendText("\n Exception fired" + "\n\n\n" + ex);
                }
            }
        }

        private void nudFrameRate_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue != null && _ecsApi != null)
            {
                int newValue = (int)e.NewValue;
                try
                {
                    string? result = _ecsApi.set_frame_rate(newValue);
                    tbxLog.AppendText(result + "\n");
                }
                catch (Exception ex)
                {
                    tbxLog.AppendText("\n Exception fired" + "\n\n\n" + ex);
                }
            }
        }

        public void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (sender is TextBox textBox)
            {
                // Log the TextBox change
                tbxLog.AppendText($"TextBox '{textBox.Name}' text changed to: {textBox.Text}\n");
                textboxChange = true;

                if (double.TryParse(textBox.Text, out double value))
                {


                    // Attempt to parse the text and update the appropriate slider
                    switch (textBox.Name)
                    {
                        case "traX":
                            sldX.Value = value;
                            sldX.Minimum = value - sliderOffset;
                            sldX.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldX, null);
                            break;

                        case "traY":
                            sldY.Value = value;
                            sldY.Minimum = value - sliderOffset;
                            sldY.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldY, null);
                            break;

                        case "traZ":
                            sldZ.Value = value;
                            sldZ.Minimum = value - sliderOffset;
                            sldZ.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldZ, null);
                            break;

                        case "rotX":
                            sldRx.Value = value;
                            sldRx.Minimum = value - sliderOffset;
                            sldRx.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldRx, null);
                            break;

                        case "rotY":
                            sldRy.Value = value;
                            sldRy.Minimum = value - sliderOffset;
                            sldRy.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldRy, null);
                            break;

                        case "rotZ":
                            sldRz.Value = value;
                            sldRz.Minimum = value - sliderOffset;
                            sldRz.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldRz, null);
                            break;

                        case "fov":
                            sldFov.Value = value;
                            sldFov.Minimum = value - sliderOffset;
                            sldFov.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldFov, null);
                            break;

                        case "scaX":
                            sldSx.Value = value;
                            sldSx.Minimum = value - sliderOffset;
                            sldSx.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldSx, null);
                            break;

                        case "scaY":
                            sldSy.Value = value;
                            sldSy.Minimum = value - sliderOffset;
                            sldSy.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldSy, null);
                            break;

                        case "scaZ":
                            sldSz.Value = value;
                            sldSz.Minimum = value - sliderOffset;
                            sldSz.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldSz, null);
                            break;




                        default:
                            tbxLog.AppendText($"TextBox '{textBox.Name}' does not match known sliders.\n");
                            break;
                    }
                }
                else
                {
                    tbxLog.AppendText("Invalid value for Slider" + textBox.Name + ".\n");
                }
            }
        }

        private void TextBoxComponentOptions_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            // Check if Enter key was pressed
            if (e.Key == Key.Enter)
            {
                // Prevent the Enter key from propagating further
                e.Handled = true;

                // Get the TextBox and its DataContext (the bound JsonKeyValue object)
                if (sender is TextBox textBox && textBox.DataContext is JsonKeyValue keyValue)
                {

                    OnComponentsTextboxValueChanged(keyValue.Key, textBox.Text);
                }
            }
        }

        private void OnComponentsTextboxValueChanged(string key, string newValue)
        {
            string? componentOptionsJSON = _componentOptionsBuilder.CreateJsonFromListBox();
            if (componentOptionsJSON != null && _ecsApi != null)
            {
                try
                {
                    _ecsApi.set_component_option(currentComponentUUID, componentOptionsJSON);
                    tbxLog.AppendText("\n ComponentOptions updated with UUID: " + currentComponentUUID + " and JSON: \n" + componentOptionsJSON.ToString());

                }
                catch (Exception ex)
                {
                    tbxLog?.AppendText(ex.ToString());
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
                    StartOtherExe("Engine.exe");
                    await Task.Delay(1000);
                    _ecsApi = new EcsApi("127.0.0.1", 51234);

                    // initial root-request
                    ReceivedEcsJsonString = _ecsApi.get_root();
                    //TODO set Bounces from ECS
                    tbxLog.AppendText(ReceivedEcsJsonString);
                    connection = true; // connection was successful 

                    clickedButton.Background = (Brush)FindResource("WindowPrimaryColor");
                    clickedButton.Foreground = Brushes.Black;

                    Window_Loaded(sender, e);


                    // initial Treebuilding
                    _entityBuilder = new TreeBuilder(trvEntities, this);
                    _entityBuilder.BuildTreeFromJson(ReceivedEcsJsonString);

                    if (int.TryParse(_ecsApi.get_bounces(), out int bounces))
                    {
                        nudBounces.Value = bounces;
                    }

                    if (int.TryParse(_ecsApi.get_frame_rate(), out int fps))
                    {
                        nudFrameRate.Value = fps;
                    }

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
                string engineExeDirPath;
                string rootDir = Directory.GetCurrentDirectory();

#if DEBUG
                // Running inside Visual Studio (Debug mode)
                engineExeDirPath = System.IO.Path.Combine(rootDir, @"..\..\..\..\..\Engine\");
                string engineExePath = System.IO.Path.Combine(engineExeDirPath, @"build\Engine.exe");
                string workingDirectory = System.IO.Path.GetDirectoryName(engineExeDirPath);
#else
        // Running in Release mode (installed or direct execution)
        engineExeDirPath = System.IO.Path.Combine(rootDir, @"..\Engine\");
        string engineExePath = System.IO.Path.Combine(rootDir, @"Engine\Engine.exe");
        string workingDirectory = rootDir;
#endif

                // Create the ProcessStartInfo
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = engineExePath,
                    WorkingDirectory = workingDirectory,  // Set the working directory
                    UseShellExecute = false,  // Optionally, to redirect input/output (if needed)
                    CreateNoWindow = false,   // Optionally, run without a window
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

                    currentEntityUUID = uuid;
                    deleteUUID = uuid;

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
                _entityBuilder.CreateChildItems(updatedNode, selectedItem);
                //tbxLog.AppendText($"Updated children for UUID: {uuid}\n");
            }
        }
        public void UpdateEntitiesOptions(string uuid, RoutedPropertyChangedEventArgs<object>? e)
        {
            try
            {
                string? ecsJsonNode = _ecsApi.get_entity_options(uuid);
                tbxLog.AppendText("EntityList Update : JSON\n\n " + ecsJsonNode + "\n\n");

                if (ecsJsonNode == null)
                {
                    tbxLog.AppendText("JSON = null.\n");
                    return;
                }

                _entityOptionsBuilder = new TreeBuilder(trvEntitiesOptions, this);
                _entityOptionsBuilder.BuildTreeFromEntityOptions(ecsJsonNode);
            }
            catch (Exception ex)
            {
                tbxLog.AppendText("\n Exception fired" + "\n\n\n" + ex);
            }
        }

        // Components Update
        private void trvComponents_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue is TreeViewItem selectedItem)
            {
                string? header = selectedItem.Header.ToString();
                if (selectedItem.Tag is TreeItemData tagData)
                {
                    string uuid = tagData.UUID;       // Access UUID
                    string name = tagData.Name;       // Access Name

                    currentComponentUUID = uuid;
                    deleteUUID = uuid;

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
            tbxLog.AppendText("ComponentList Update : JSON\n\n " + componentsJsonNode + "\n\n");

            if (componentsJsonNode == null)
            {
                tbxLog.AppendText("JSON = null.\n");
                return;
            }
            _componentBuilder = new TreeBuilder(trvComponents, this);

            try
            {
                _componentBuilder.BuildTreeFromComponents(componentsJsonNode);
            }
            catch (Exception ex)
            {
                tbxLog.AppendText("JSON is null for uuid " + uuidEntity + "\n");
            }

        }

        public void UpdateComponentsOptions(string uuid, RoutedPropertyChangedEventArgs<object>? e)
        {
            try
            {
                string? ecsJsonNode = _ecsApi.get_component_options(uuid);
                tbxLog.AppendText("ComponentOptions Update : JSON\n\n " + ecsJsonNode + "\n\n");

                if (ecsJsonNode == null)
                {
                    tbxLog.AppendText("JSON = null.\n");
                    return;
                }

                _componentOptionsBuilder = new TreeBuilder(this, trvComponentsOptions);
                _componentOptionsBuilder.BuildTreeFromComponentOptions(ecsJsonNode);
            }
            catch (Exception ex)
            {
                tbxLog.AppendText("\n Exception fired" + "\n\n\n" + ex);
            }
        }

        private void trvComponentsOptions_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {

        }

        //Add Components
        private void AddRenderComponent()
        {
            if (_ecsApi != null)
            {

                try
                {

                    string pathSentStatus = _ecsApi.create_component(currentEntityUUID, "render");
                    tbxLog.AppendText("Render Component Added!\n");
                    UpdateComponents(currentEntityUUID, null);

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
                    string pathSentStatus = _ecsApi.create_component(currentEntityUUID, "light");
                    tbxLog.AppendText("Render Component Added!\n");
                    UpdateComponents(currentEntityUUID, null);

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

                    string pathSentStatus = _ecsApi.create_component(currentEntityUUID, "camera");
                    tbxLog.AppendText("Render Component Added!\n");
                    UpdateComponents(currentEntityUUID, null);
                }
                catch (InvalidOperationException ex)
                {
                    //TODO
                }
            }
        }

        private void AddEntity()
        {
            String name;
            if (_ecsApi != null)
            {
                try
                {
                    // Open dialog to get the name
                    var nameDialog = new NameInputDialog();
                    if (nameDialog.ShowDialog() == true)
                    {
                        name = nameDialog.EntityName;

                        // Proceed with entity creation
                        string pathSentStatus = _ecsApi.create_entity(currentEntityUUID, name);
                        tbxLog.AppendText("Render Component Added!\n");
                        UpdateComponents(currentEntityUUID, null);
                    }
                    else
                    {
                        tbxLog.AppendText("Operation cancelled by the user.\n");
                    }
                }
                catch (InvalidOperationException ex)
                {
                    tbxLog.AppendText($"Error: {ex.Message}\n");
                }
            }
        }



        //Items



        private void TakeScreenshot()
        {
            SaveFileDialog saveFileDialog;
            string filePath = "";

            saveFileDialog = new SaveFileDialog
            {
                Filter = "PNG File (*.png)|*.png",
                Title = "Save PNG Image file",
                FileName = "image.png"
            };

            if (saveFileDialog.ShowDialog() == true)
            {
                filePath = saveFileDialog.FileName;
                tbxLog.AppendText("PNG saved at: " + filePath + "\n");
            }

            if (filePath.Equals(""))
            {
                return;
            }
            try
            {
                GdiCapture.CaptureWindow(loader.hWndGLFW, filePath);
                Process.Start(new ProcessStartInfo(filePath) { UseShellExecute = true });
            }
            catch (System.InvalidOperationException e)
            {
                MessageBox.Show("Capturing failed!" + "\n" + e.Message);
                return;
            }
            
            tbxLog.AppendText("Captured!" + "\n");

        }

        private void generalKeyDown(object sender, KeyEventArgs e)
        {

            if (e.Key == Key.Z && Keyboard.Modifiers == ModifierKeys.Control)
            {
                if (!connection)
                {
                    MessageBox.Show("No connection established. Please connect to the server first.",
                                    "Connection Error",
                                    MessageBoxButton.OK,
                                    MessageBoxImage.Warning);
                    return;
                }

                _ecsApi.undo(1);

                if (!currentEntityUUID.Equals("uuid"))
                {
                    UpdateEntitiesOptions(currentEntityUUID, null);
                }
                if (!currentComponentUUID.Equals("uuid"))
                {
                    UpdateComponentsOptions(currentComponentUUID, null);
                }
                return;
            }

            if (e.Key == Key.Delete)
            {
                if (deleteUUID.Equals("uuid"))
                {
                    return;
                }

                if (deleteUUID.Equals(rootUUID))
                {
                    MessageBox.Show("Root cannot be deleted!",
                                    "Invalid UUID",
                                    MessageBoxButton.OK,
                                    MessageBoxImage.Warning);
                    return;
                }

                try
                {



                    if (deleteUUID.Equals(currentEntityUUID))
                    {
                        _ecsApi.remove_entity(deleteUUID);
                        deleteUUID = "uuid";

                        foreach (TreeViewItem item in trvEntities.Items)
                        {
                            if (item.Tag is TreeItemData tagData)
                            {
                                string uuid = tagData.UUID;

                                if (uuid.Equals(rootUUID))
                                {
                                    item.Focus();
                                    item.IsSelected = true;
                                }
                            }
                        }
                    }
                    if (deleteUUID.Equals(currentComponentUUID))
                    {
                        _ecsApi.remove_component(deleteUUID);
                        currentComponentUUID = "uuid";
                        deleteUUID = "uuid";

                        foreach (TreeViewItem item in trvEntities.Items)
                        {
                            if (item.Tag is TreeItemData tagData)
                            {
                                string uuid = tagData.UUID;

                                if (uuid.Equals(rootUUID))
                                {
                                    item.Focus();
                                    item.IsSelected = true;
                                }
                            }
                        }

                    }
                }
                catch (Exception ex)
                {
                    tbxLog.AppendText(ex.ToString());
                }
                return;

            }

        }


        private void OnBrowseButtonClick(object sender, RoutedEventArgs e)
        {
            if (sender is Button button && button.DataContext is JsonKeyValue jsonKeyValue)
            {
                OpenFileDialog openFileDialog;

                openFileDialog = new OpenFileDialog
                {
                    Filter = "OBJ Files (*.obj)|*.obj",
                    Title = "Select a .obj file"
                };

                if (openFileDialog.ShowDialog() == true)
                {
                    jsonKeyValue.Path = openFileDialog.FileName;
                    jsonKeyValue.Value = openFileDialog.FileName; // Update the value as well

                    // Manually call the method after the text update (file path)
                    OnComponentsTextboxValueChanged(jsonKeyValue.Key, openFileDialog.FileName);
                }
            }
        }

    }
}
