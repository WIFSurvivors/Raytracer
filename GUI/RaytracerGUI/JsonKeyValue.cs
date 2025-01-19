using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RaytracerGUI
{
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

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

}
