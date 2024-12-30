using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RaytracerGUI
{
    public class EcsNode
    {
        public string UUID { get; set; }
        public string Name { get; set; }
        public int ChildrenCount { get; set; }
        public List<EcsNode> Children { get; set; } = new List<EcsNode>();

        
        public Dictionary<string, Dictionary<string, string>>? Options { get; set; }

        
        public EcsNode(string uuid, string name, int childrenCount)
        {
            UUID = uuid;
            Name = name;
            ChildrenCount = childrenCount;
        }

       
        public void AddChild(EcsNode child)
        {
            Children.Add(child);
            ChildrenCount = Children.Count;
        }
    }

}
