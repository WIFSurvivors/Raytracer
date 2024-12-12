using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
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

                    if (child.children_count > 0)
                    {
                        childItem.Items.Add(new TreeItemData());
                    }

                    parentItem.Items.Add(childItem);
                    parentItem.IsExpanded = true;
                    CreateChildItems(child, childItem);
                    ToolTipService.SetToolTip(childItem, child.uuid);
                }
            }
        }
    }
}
