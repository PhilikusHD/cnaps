﻿using System;
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
using System.Xml.Serialization;

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
            ShowChatView();
        }

        //private void ShowChatView_Click(object sender, RoutedEventArgs e)
        //{
        //  ShowChatView();

        // }

        private void ShowChatView()
        {
            ContentArea.Content = new ChatView();
        }

        private void SendMessage_Click(object sender, RoutedEventArgs e)
        {
            ShowChatView();
            //Console.WriteLine("hallo");
        }

        private void ChatLog_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            WelcomeImage.Visibility = Visibility.Collapsed;

            string userMessage = UserInput.Text;
            if (!string.IsNullOrWhiteSpace(userMessage))
            {
                ChatLog.Items.Add("You: " + userMessage);

                ChatLog.Items.Add("Bot: " + GenerateResponseString());
            }
        }

    }
}