using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows;

namespace RaytracerGUI
{
    /// <summary>
    /// Converts a boolean value to determine if a UI element, such as a text field, 
    /// should be clickable (enabled) or not clickable (disabled).
    /// </summary>
    /// <remarks>
    /// Implements the <see cref="IValueConverter"/> interface to provide a 
    /// two-way data binding conversion for enabling or disabling UI elements 
    /// based on a boolean value.
    /// </remarks>

    public class BoolToClickableConverter : IValueConverter
    {

        /// <summary>
        /// Converts a boolean value to determine whether a UI element should be clickable.
        /// </summary>
        /// <param name="value">The source boolean value.</param>
        /// <returns>
        /// Returns <c>true</c> if the input value is <c>true</c>, enabling the UI element; 
        /// otherwise, returns <c>false</c>, disabling the element.
        /// </returns>
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            // If the input value is true, the element is clickable (enabled), otherwise not clickable.
            return value is true;
        }


        /// <summary>
        /// Converts a value back from the target to the source.
        /// </summary>
        /// <param name="value">The target boolean value.</param>
        /// <returns>
        /// Returns <c>true</c> if the input value is <c>true</c>, indicating the UI element 
        /// is clickable; otherwise, returns <c>false</c>.
        /// </returns>
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            // If the input value is true (clickable), return true; otherwise, return false.
            return value is true;
        }
    }
}
