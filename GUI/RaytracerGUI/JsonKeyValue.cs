using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RaytracerGUI
{
    /// <summary>
    /// Stores the Listbox values for the ComponentOptions. 
    /// Includes information if the option is a path a and handles the propertychanged event
    /// </summary>
    public class JsonKeyValue : INotifyPropertyChanged
    {
        private string _key;
        public string Key
        {
            get => _key;
            set
            {
                _key = value;
                OnPropertyChanged(nameof(Key));
            }
        }

        private object _value;
        public object Value
        {
            get => _value;
            set
            {
                _value = value;
                OnPropertyChanged(nameof(Value));
            }
        }

        private bool _isPath;
        public bool IsPath
        {
            get => _isPath;
            set
            {
                _isPath = value;
                OnPropertyChanged(nameof(IsPath));
            }
        }

        /// <summary>
        /// only filled when isPath = true from the Treebuilder
        /// </summary>
        private string _path;
        public string Path
        {
            get => _path;
            set
            {
                _path = value;
                OnPropertyChanged(nameof(Path));
            }
        }

        /// <summary>
        /// When the path is changed this Method is called (as set in the MainWindow.xaml)
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

}
