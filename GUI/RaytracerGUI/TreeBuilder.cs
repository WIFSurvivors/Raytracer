using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Xml.Linq;

namespace RaytracerGUI
{
    /// <summary>
    /// TreeBuilder is responsible for constructing and managing TreeViews 
    /// within the application. It can populate trees from various data 
    /// sources including JSON strings and entity options.
    /// </summary>
    class TreeBuilder
    {
        public TreeView? TreeView { get; private set; }
        private readonly MainWindow _mainWindow;
        private System.Windows.Controls.ListBox? _componentOptionsListbox;
        private Dictionary<string, TextBox> _textBoxMapping = new Dictionary<string, TextBox>();
        public ObservableCollection<JsonKeyValue> _jsonKeyValuePairs = new ObservableCollection<JsonKeyValue>();


        /// <summary>
        /// Initializes a new instance of the <see cref="TreeBuilder"/> class 
        /// with a reference to the main application window and a ListBox for component options.
        /// </summary>
        /// <param name="mainWindow">The main window of the application.</param>
        /// <param name="componentOptionsListbox">The ListBox displaying component options.</param>
        public TreeBuilder(MainWindow mainWindow, ListBox componentOptionsListbox)
        {
            _mainWindow = mainWindow;
            _componentOptionsListbox = componentOptionsListbox;
        }


        /// <summary>
        /// Initializes a new instance of the <see cref="TreeBuilder"/> class 
        /// with a specified TreeView and a reference to the main application window.
        /// </summary>
        /// <param name="treeView">The TreeView to populate.</param>
        /// <param name="mainWindow">The main window of the application.</param>
        public TreeBuilder(TreeView treeView, MainWindow mainWindow)
        {
            TreeView = treeView;
            _mainWindow = mainWindow;
        }


        /// <summary>
        /// Builds a TreeView structure based on the provided JSON string.
        /// </summary>
        /// <param name="jsonString">A JSON-formatted string representing the tree structure.</param>
        /// <exception cref="JsonException">Thrown if the JSON is invalid or cannot be deserialized.</exception>
        /// <remarks>
        /// This method clears the existing TreeView content, parses the JSON, 
        /// and creates a hierarchical structure of items.
        /// </remarks>
        public void BuildTreeFromJson(string jsonString)
        {
            try
            {
                TreeView.Items.Clear();
                var jsonRoot = JsonSerializer.Deserialize<EcsEntityNode>(jsonString);
                if (jsonRoot != null)
                {
                    TreeViewItem rootItem = new TreeViewItem
                    {
                        Header = jsonRoot.name,
                        Tag = new TreeItemData
                        {
                            UUID = jsonRoot.uuid,
                            Name = jsonRoot.name,
                            Children = jsonRoot.children_count
                        }

                    };
                    _mainWindow.rootUUID = jsonRoot.uuid;
                    CreateChildItems(jsonRoot, rootItem);
                    TreeView.Items.Add(rootItem);
                    ToolTipService.SetToolTip(rootItem, jsonRoot.uuid);
                }
            }
            catch (Exception e)
            {
                _mainWindow.tbxLog.AppendText("\n" + e.Message);
            }
        }


        /// <summary>
        /// Recursively creates child TreeView items from the given ECS entity node.
        /// </summary>
        /// <param name="ecsNode">The parent entity node containing child nodes.</param>
        /// <param name="parentItem">The parent TreeView item to which children are added.</param>
        public void CreateChildItems(EcsEntityNode ecsNode, TreeViewItem parentItem)
        {
            if (ecsNode.children != null && ecsNode.children.Count > 0)
            {
                foreach (var child in ecsNode.children)
                {
                    TreeViewItem childItem = new TreeViewItem
                    {
                        Header = child.name,
                        Tag = new TreeItemData
                        {
                            UUID = child.uuid,
                            Name = child.name,
                            Children = child.children_count
                        }
                    };


                    //Add dummy node to tree to show arrow
                    if (child.children_count > 0)
                    {
                        var dummyItem = new TreeViewItem
                        {
                            Header = "",
                            Tag = new TreeItemData
                            {
                                Name = "dummy",
                                UUID = Guid.NewGuid().ToString()
                            }
                        };

                        childItem.Items.Add(dummyItem);
                    }


                    parentItem.Items.Add(childItem);
                    //Expand on mouseclick
                    parentItem.IsExpanded = true;
                    CreateChildItems(child, childItem);
                    ToolTipService.SetToolTip(childItem, child.uuid);
                }
            }
        }


        /// <summary>
        /// Builds a TreeView structure based on entity options provided as a JSON string.
        /// </summary>
        /// <param name="jsonString">A JSON-formatted string containing entity options.</param>
        /// <remarks>
        /// Each option category is represented as a TreeView item containing editable 
        /// TextBox elements for individual properties.
        /// </remarks>
        public void BuildTreeFromEntityOptions(string jsonString)
        {
            TreeView.Items.Clear();

            var options = new JsonSerializerOptions
            {
                PropertyNameCaseInsensitive = true
            };

            // Deserialize with the case-insensitive option
            var jsonData = JsonSerializer.Deserialize<EntityOptionsData>(jsonString, options);

            if (jsonData != null)
            {
                var rootItem = new TreeViewItem
                {
                    Header = "Name: " + jsonData.Name,
                    Tag = jsonData.UUID
                };


                if (jsonData.Options == null)
                {
                    jsonData.Options = new Dictionary<string, Dictionary<string, string>>(); // Initialize if null

                }
                try
                {
                    // Populate the options as child nodes
                    foreach (var category in jsonData.Options)
                    {
                        var categoryItem = new TreeViewItem
                        {
                            Header = category.Key, // Translation, rotation, scale
                        };

                        foreach (var property in category.Value)
                        {
                            var propertyItem = new TreeViewItem
                            {
                                Header = $"{property.Key}: ",
                                Tag = property.Key 
                            };


                            string textBoxName = $"{category.Key.Substring(0, 3).ToLower()}{property.Key.First().ToString().ToUpper()}{property.Key.Substring(1)}";
                            _mainWindow.tbxLog.AppendText("_treebuilder.Textbox added: " + textBoxName + " , " + property.Value);
                            // Create a TextBox to make the property value editable
                            var textBox = new TextBox
                            {
                                Name = textBoxName, // Set the name (e.g., "x", "y", "z") for reference
                                Text = property.Value.Replace(".", ","), // Set initial value
                                VerticalAlignment = VerticalAlignment.Center
                            };
                            _textBoxMapping[textBoxName] = textBox;

                            // Handle TextChanged event to update value when user edits it
                            // Attach KeyDown and LostFocus event handlers
                            textBox.KeyDown += TextBox_KeyDown;
                            textBox.LostFocus += TextBox_LostFocus;




                            // Add the TextBox to the TreeViewItem
                            propertyItem.Items.Add(textBox);
                            categoryItem.Items.Add(propertyItem);
                            propertyItem.IsExpanded = true;

                            // Simulate a TextChanged event
                            _mainWindow.shouldUpdate = false;
                            SimulateTextChanged(textBox);
                            _mainWindow.shouldUpdate = true;

                        }

                        rootItem.Items.Add(categoryItem);
                        categoryItem.IsExpanded = true;

                    }
                }
                catch (Exception ex)
                {
                    _mainWindow.tbxLog.AppendText("\n" + ex.Message);
                }

                // Add root to the TreeView
                TreeView.Items.Add(rootItem);
                rootItem.IsExpanded = true;
            }
        }


        /// <summary>
        /// Finds the first parent of a specified type in the visual tree.
        /// </summary>
        /// <typeparam name="T">The type of the parent to find.</typeparam>
        /// <param name="child">The starting point for the search.</param>
        /// <returns>The first parent of type T, or null if none is found.</returns>

        private T FindParent<T>(DependencyObject child) where T : DependencyObject
        {
            DependencyObject parent = VisualTreeHelper.GetParent(child);
            if (parent == null) return null;

            if (parent is T)
                return (T)parent;

            return FindParent<T>(parent);
        }


        /// <summary>
        /// Retrieves a TextBox by its key from the internal mapping.
        /// </summary>
        /// <param name="key">The key associated with the desired TextBox.</param>
        /// <returns>The corresponding TextBox if found; otherwise, null.</returns>
        public TextBox? GetTextBox(string key)
        {
            return _textBoxMapping.TryGetValue(key, out var textBox) ? textBox : null;
        }


        /// <summary>
        /// Simulates a TextChanged event for a given TextBox.
        /// </summary>
        /// <param name="textBox">The TextBox for which the event is simulated.</param>
        private void SimulateTextChanged(TextBox textBox)
        {
            var textChangedEventArgs = new TextChangedEventArgs(TextBox.TextChangedEvent, UndoAction.None);
            _mainWindow.TextBox_TextChanged(textBox, textChangedEventArgs);
        }


        /// <summary>
        /// Handles the KeyDown event for a TextBox, processing changes when Enter is pressed.
        /// </summary>
        /// <param name="sender">The source TextBox.</param>
        /// <param name="e">The KeyEventArgs containing event data.</param>
        private void TextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (sender is TextBox textBox && e.Key == Key.Enter)
            {
                ProcessTextBoxChange(textBox);
                e.Handled = true; // Prevent further processing of the Enter key
            }
        }


        /// <summary>
        /// Handles the LostFocus event for a TextBox, processing changes when the TextBox loses focus.
        /// </summary>
        /// <param name="sender">The source TextBox.</param>
        /// <param name="e">The RoutedEventArgs containing event data.</param>
        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (sender is TextBox textBox)
            {
                ProcessTextBoxChange(textBox);
            }
        }


        /// <summary>
        /// Processes changes to a TextBox value, updating logs and triggering change handlers.
        /// </summary>
        /// <param name="textBox">The TextBox that was modified.</param>
        private void ProcessTextBoxChange(TextBox textBox)
        {
            // Log and process the change
            string name = textBox.Name;
            string value = textBox.Text;

            _mainWindow.tbxLog.AppendText($"TextBox '{name}' changed to: {value}\n");

            // Call the original TextBox_TextChanged method if needed
            var textChangedEventArgs = new TextChangedEventArgs(TextBox.TextChangedEvent, UndoAction.None);
            _mainWindow.TextBox_TextChanged(textBox, textChangedEventArgs);
        }


        /// <summary>
        /// Builds a TreeView structure from a list of ECS components provided as a JSON string.
        /// </summary>
        /// <param name="jsonString">A JSON-formatted string representing ECS components.</param>
        /// <remarks>
        /// Each component is represented as a TreeView item under a root node.
        /// </remarks>
        public void BuildTreeFromComponents(string jsonString)
        {
            TreeView.Items.Clear();

            List<EcsComponentNode>? componentList = JsonSerializer.Deserialize<List<EcsComponentNode>>(jsonString);

            if (componentList != null && componentList.Count > 0)
            {
                TreeViewItem rootItem = new TreeViewItem
                {
                    Header = "root",
                    Tag = "Root"
                };

                foreach (var component in componentList)
                {
                    TreeViewItem componentItem = new TreeViewItem
                    {
                        Header = component.name,
                        Tag = new TreeItemData
                        {
                            UUID = component.uuid,
                            Name = component.name
                        }
                    };

                    rootItem.Items.Add(componentItem);
                }

                TreeView.Items.Add(rootItem);
                rootItem.IsExpanded = true;
            }
            else
            {
                _mainWindow.tbxLog.AppendText("No components found in JSON.\n");
            }
        }


        /// <summary>
        /// Populates a ListBox with key-value pairs extracted from component options 
        /// provided as a JSON string.
        /// </summary>
        /// <param name="jsonString">A JSON-formatted string containing component options.</param>
        /// <remarks>
        /// Entries containing "path" in their key are marked as path entries.
        /// </remarks>
        public void BuildTreeFromComponentOptions(string jsonString)
        {
            _jsonKeyValuePairs.Clear();

            var componentOptions = JsonSerializer.Deserialize<Dictionary<string, object>>(jsonString);


            if (componentOptions != null && componentOptions.Count > 0)
            {
                foreach (var kvp in componentOptions)
                {
                    var key = kvp.Key;
                    var value = kvp.Value;

                    // Add key-value pairs to the ListBox
                    var jsonKeyValue = new JsonKeyValue { Key = key, Value = value };

                    // Check if the key includes "path"
                    if (key.Contains("path", StringComparison.OrdinalIgnoreCase))
                    {
                        jsonKeyValue.IsPath = true; // Mark as a path entry
                    }

                    // Add key-value pairs to the ListBox
                    _jsonKeyValuePairs.Add(jsonKeyValue);
                }

                _componentOptionsListbox.ItemsSource = _jsonKeyValuePairs;
            }

            else
            {
                _mainWindow.tbxLog.AppendText("No components found in JSON.\n");
            }

        }


        /// <summary>
        /// Creates a JSON string from the current contents of the ListBox.
        /// </summary>
        /// <returns>A JSON-formatted string representing the ListBox contents.</returns>
        /// <remarks>
        /// Logs a message if the ListBox is empty.
        /// </remarks>
        public string CreateJsonFromListBox()
        {
            if (_jsonKeyValuePairs == null || _jsonKeyValuePairs.Count == 0)
            {
                _mainWindow.tbxLog.AppendText("No data in ListBox to create JSON.\n");
                return "";
            }

            // Create a dictionary from the key-value pairs
            var dictionary = _jsonKeyValuePairs.ToDictionary(kvp => kvp.Key, kvp => kvp.Value);

            // Serialize the dictionary to a JSON string
            string jsonString = JsonSerializer.Serialize(dictionary, new JsonSerializerOptions
            {
                WriteIndented = true 
            });

            return jsonString;
        }
    }


}