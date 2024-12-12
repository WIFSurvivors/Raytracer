﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RaytracerGUI
{
    public class EntityOptionsData
    {
        public string UUID { get; set; }
        public string Name { get; set; }
        public Dictionary<string, Dictionary<string, string>>? Options { get; set; }
    }

}
