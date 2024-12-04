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

        public TreeBuilder(string jsonString, TreeView treeView)
        {
            TreeView = treeView;
            BuildTreeFromJson(jsonString);
        }

        private void BuildTreeFromJson(string jsonString)
        {
            TreeView.Items.Clear();
            var ecsRoot = JsonSerializer.Deserialize<EcsNode>(jsonString);
            if (ecsRoot != null)
            {
                TreeViewItem rootItem = new TreeViewItem
                {
                    Header = ecsRoot.name
                };
                CreateChildItems(ecsRoot, rootItem);
                TreeView.Items.Add(rootItem);
            }
        }

        private void CreateChildItems(EcsNode ecsNode, TreeViewItem parentItem)
        {
            if (ecsNode.children != null && ecsNode.children.Count > 0)
            {
                foreach (var child in ecsNode.children)
                {
                    TreeViewItem childItem = new TreeViewItem
                    {
                        Header = child.name
                    };

                    parentItem.Items.Add(childItem);
                    CreateChildItems(child, childItem);
                }
            }
        }
    }
}
