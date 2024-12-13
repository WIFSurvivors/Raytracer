using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Xml.Linq;

namespace RaytracerGUI
{
    class TreeBuilder
    {
        public TreeView TreeView { get; private set; }

        public TreeBuilder(TreeView treeView)
        {
            TreeView = treeView;
        }

        public void BuildTreeFromJson(string jsonString)
        {
            TreeView.Items.Clear();
            var jsonRoot = JsonSerializer.Deserialize<EcsNode>(jsonString);
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

        public void CreateChildItems(EcsNode ecsNode, TreeViewItem parentItem)
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
                        childItem.Items.Add(new TreeItemData());
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
                    Header = "Name: "+ jsonData.Name,
                    Tag = jsonData.UUID
                };


                if (jsonData.Options == null)
                {
                    jsonData.Options = new Dictionary<string, Dictionary<string, string>>(); // Initialize if null

                }

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

                        // Create a TextBox to make the property value editable
                        var textBox = new TextBox
                        {
                            Text = property.Value, // Set initial value
                            VerticalAlignment = VerticalAlignment.Center
                        };

                        // Handle TextChanged event to update value when user edits it
                        textBox.TextChanged += (sender, e) =>
                        {
                        
                        };

                        // Add the TextBox to the TreeViewItem
                        propertyItem.Items.Add(textBox);
                        categoryItem.Items.Add(propertyItem);
                        propertyItem.IsExpanded = true;
                    }

                    rootItem.Items.Add(categoryItem);
                    categoryItem.IsExpanded = true;
                    
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

    }
    }
