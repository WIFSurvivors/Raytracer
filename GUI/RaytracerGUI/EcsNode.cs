using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace RaytracerGUI
{
    class EcsNode
    {
        public string uuid { get; set; }
        public string name { get; set; }
        public int children_count { get; set; }
        public List<EcsNode> children { get; set; }
    }
}
