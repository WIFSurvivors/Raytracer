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
                CreateChildItems(ecsRoot);
            }
        }

        private void CreateChildItems(EcsNode ecsNode)
        {
            TreeViewItem currentItem = new TreeViewItem();
            currentItem.Header = ecsNode.name;
            TreeView.Items.Add(currentItem);

            if (ecsNode.children != null && ecsNode.children.Count > 0)
            {
                foreach (var child in ecsNode.children)
                {
                    CreateChildItems(child);
                }
            }
        }
    }
}
