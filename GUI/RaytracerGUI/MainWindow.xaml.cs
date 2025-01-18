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
using System.Threading.Tasks;
using System.Reflection.PortableExecutable;
using System.Text.Json;
using System;
using System.Reflection;
using System.Windows.Controls.Primitives;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;
using System;
using System.Windows.Forms;

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
        string rotation = "xpos";
        bool textboxChange = false;
        int sliderOffset = 10;

        string currentEntityUUID = "uuid";
        string currentComponentUUID = "uuid";


        public MainWindow()
        {
            InitializeComponent();
            this.Background = (Brush)Application.Current.Resources["WindowBackgroundColor"];
            this.KeyDown += KeyDownCtrlZ;

        }

        //Menu clicks
        private void FileMenuClick(object sender, RoutedEventArgs e)
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

        private void AddMenuClick(object sender, RoutedEventArgs e)
        {
            if (sender is MenuItem clickedMenuItem)
            {
                // Der Name des angeklickten Menüelements wird verwendet
                string itemName = clickedMenuItem.Name;

                switch (itemName)
                {
                    case "mniAddRender":
                        AddRenderComponent();
                        tbxLog.AppendText($"{itemName} was clicked!\n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "mniAddLight":
                        AddLightComponent();
                        tbxLog.AppendText($"{itemName} was clicked!\n");
                        tbxLog.ScrollToEnd();
                        break;

                    case "mniAddCamera":
                        AddCameraComponent();
                        tbxLog.AppendText($"{itemName} was clicked!\n");
                        tbxLog.ScrollToEnd();
                        break;
                    case "mniAddEntity":
                        AddEntity();
                        tbxLog.AppendText($"{itemName} was clicked!\n");
                        tbxLog.ScrollToEnd();
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
                            imgRotation.Width = 30;
                            imgRotation.Height = 30;
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
                            if (gridZoom.Visibility == Visibility.Visible)
                            {
                                gridZoom.Visibility = Visibility.Collapsed;
                                lblZoom.Visibility = Visibility.Collapsed;

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

                                gridZoom.Visibility = Visibility.Visible;
                                lblZoom.Visibility = Visibility.Visible;

                                // adapt column width for vertical zoom slider
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
                        _ecsApi.undo(1);
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
                        float zoom = (float)sldZoom.Value;
                        _ecsApi.set_fov(zoom);
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

                    case "sldZoom":
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
                        lblZoomMed.Content = medValue;

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

                        case "zoom":
                            sldZoom.Value = value;
                            sldZoom.Minimum = value - sliderOffset;
                            sldZoom.Maximum = value + sliderOffset;
                            SliderPreviewMouseUp(sldZoom, null);
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
                    // Call your desired function
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
                    StartOtherExe("../../../../../Engine/Engine.exe");
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


                    // initial Treebuilding
                    _entityBuilder = new TreeBuilder(trvEntities, this);
                    _entityBuilder.BuildTreeFromJson(ReceivedEcsJsonString);

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
                string engineExeDirPath = System.IO.Path.Combine(Directory.GetCurrentDirectory(), @"..\..\..\..\..\Engine\");

                // Create the ProcessStartInfo
                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = engineExeDirPath + @"build\Engine.exe",
                    WorkingDirectory = System.IO.Path.GetDirectoryName(engineExeDirPath),  // Set the working directory
                    UseShellExecute = false,  // Optionally, to redirect input/output (if needed)
                    CreateNoWindow = false,     // Optionally, run without a window
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

            try
            {
            
                GdiCapture.CaptureWindow(loader.hWndGLFW, "screenshot.png");
            }
            catch (System.InvalidOperationException e)
            {
                MessageBox.Show("Capturing failed!" + "\n" + e.Message);
                return;
            }

            tbxLog.AppendText("Captured!" + "\n");

        }

        private void KeyDownCtrlZ(object sender, KeyEventArgs e)
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
                if (!currentEntityUUID.Equals("uuid")) { 
                UpdateEntitiesOptions(currentEntityUUID, null);
                }
            if (!currentComponentUUID.Equals("uuid"))
            {
                UpdateComponentsOptions(currentComponentUUID, null);
            }


            }
        }




    }

}