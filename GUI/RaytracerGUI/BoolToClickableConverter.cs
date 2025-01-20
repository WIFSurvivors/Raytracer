using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows;

namespace RaytracerGUI
{
    public class BoolToClickableConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            // If the input value is true, the element is clickable (enabled), otherwise not clickable.
            return value is true;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            // If the input value is true (clickable), return true; otherwise, return false.
            return value is true;
        }
    }
}
