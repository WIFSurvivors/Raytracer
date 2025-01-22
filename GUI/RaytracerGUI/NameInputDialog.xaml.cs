using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml.Linq;

namespace RaytracerGUI
{
    public partial class NameInputDialog : Window
    {
        public string EntityName { get; private set; }

        public NameInputDialog()
        {
            InitializeComponent();
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            EntityName = txtName.Text;
            DialogResult = true; // Close dialog and return true
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false; // Close dialog and return false
        }
    }
}
