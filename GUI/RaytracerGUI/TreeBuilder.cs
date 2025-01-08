﻿using System;
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
    class TreeBuilder
    {
        private readonly MainWindow _mainWindow;
        private System.Windows.Controls.ListBox? _componentOptionsListbox;
        private Dictionary<string, TextBox> _textBoxMapping = new Dictionary<string, TextBox>();
        public ObservableCollection<JsonKeyValue> _jsonKeyValuePairs = new ObservableCollection<JsonKeyValue>();


        public TreeView? TreeView { get; private set; }

        public TreeBuilder(MainWindow mainWindow, ListBox componentOptionsListbox)
        {
            _mainWindow = mainWindow;
            _componentOptionsListbox = componentOptionsListbox;
        }

        public TreeBuilder(TreeView treeView, MainWindow mainWindow)
        {
            TreeView = treeView;
            _mainWindow = mainWindow;
        }

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
                                Tag = property.Key // Store the property key for easy reference
                            };


                            string textBoxName = $"{category.Key.Substring(0, 3).ToLower()}{property.Key.First().ToString().ToUpper()}{property.Key.Substring(1)}";
                            _mainWindow.tbxLog.AppendText("_treebuilder.Textbox added: " + textBoxName + " , " + property.Value);
                            // Create a TextBox to make the property value editable
                            var textBox = new TextBox
                            {
                                Name = textBoxName, // Set the name (e.g., "x", "y", "z") for reference
                                Text = property.Value, // Set initial value
                                VerticalAlignment = VerticalAlignment.Center
                            };
                            _textBoxMapping[textBoxName] = textBox;

                            // Handle TextChanged event to update value when user edits it
                            //textBox.TextChanged += _mainWindow.TextBox_TextChanged;
                            // Attach KeyDown and LostFocus event handlers
                            textBox.KeyDown += TextBox_KeyDown;
                            textBox.LostFocus += TextBox_LostFocus;




                            // Add the TextBox to the TreeViewItem
                            propertyItem.Items.Add(textBox);
                            categoryItem.Items.Add(propertyItem);
                            propertyItem.IsExpanded = true;

                            // Simulate a TextChanged event
                            SimulateTextChanged(textBox);
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

        private T FindParent<T>(DependencyObject child) where T : DependencyObject
        {
            DependencyObject parent = VisualTreeHelper.GetParent(child);
            if (parent == null) return null;

            if (parent is T)
                return (T)parent;

            return FindParent<T>(parent);
        }

        public TextBox? GetTextBox(string key)
        {
            return _textBoxMapping.TryGetValue(key, out var textBox) ? textBox : null;
        }

        // Simulate a TextChanged event for a TextBox
        private void SimulateTextChanged(TextBox textBox)
        {
            var textChangedEventArgs = new TextChangedEventArgs(TextBox.TextChangedEvent, UndoAction.None);
            _mainWindow.TextBox_TextChanged(textBox, textChangedEventArgs);
        }

        // Handle the Enter key press
        private void TextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (sender is TextBox textBox && e.Key == Key.Enter)
            {
                ProcessTextBoxChange(textBox);
                e.Handled = true; // Prevent further processing of the Enter key
            }
        }

        // Handle the LostFocus event
        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (sender is TextBox textBox)
            {
                ProcessTextBoxChange(textBox);
            }
        }

        // Common method to process the change
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

        public void BuildTreeFromComponentOptions(string jsonString)
        {
            _jsonKeyValuePairs.Clear();

            var componentOptions = JsonSerializer.Deserialize<Dictionary<string, object>>(jsonString);


            if (componentOptions != null && componentOptions.Count > 0)
            {
                foreach (var kvp in componentOptions)
                {
                    // Add key-value pairs to the ListBox
                    _jsonKeyValuePairs.Add(new JsonKeyValue { Key = kvp.Key, Value = kvp.Value });
                }

            _componentOptionsListbox.ItemsSource = _jsonKeyValuePairs;
            }

            else
            {
                _mainWindow.tbxLog.AppendText("No components found in JSON.\n");
            }

        }
    }

}