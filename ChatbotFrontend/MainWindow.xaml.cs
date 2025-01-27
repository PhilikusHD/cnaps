using System;
using System.Collections.Generic;
using System.Diagnostics;
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
        public static extern void InitializeChatbot();
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ShutdownChatbot();
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GenerateResponse();
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetStatus();
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetError();

        public static string GenerateResponseString() 
        { 
            return Marshal.PtrToStringAnsi(GenerateResponse());
        }

        public static string GetErrorString()
        {
            return Marshal.PtrToStringAnsi(GetError());
        }

        public MainWindow()
        {
            InitializeComponent();
        }

        private void SendMessage_Click(object sender, RoutedEventArgs e)
        {
            Console.WriteLine("Halllllpoooooooo");
        }

        private void ChatLog_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
