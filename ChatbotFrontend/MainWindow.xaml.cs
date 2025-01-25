using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ChatbotFrontend
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern nint GetGreeting();
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // Call the C++ function
            nint greetingPtr = GetGreeting();
            // Convert the C++ string to a C# string
            string greeting = Marshal.PtrToStringAnsi(greetingPtr);
            MessageBox.Show(greeting);
        }

        private void ChatLog_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
