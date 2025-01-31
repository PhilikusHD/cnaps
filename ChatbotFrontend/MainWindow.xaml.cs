using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Automation.Provider;
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
        private static extern IntPtr GenerateResponse([MarshalAs(UnmanagedType.LPStr)] string input);

        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr GetError();

        public MainWindow()
        {
            InitializeComponent();
            InitializeChatbot();

            IntPtr responsePtr = GenerateResponse("hello");
            string botResponse = Marshal.PtrToStringAnsi(responsePtr);

            WelcomeText.Text = botResponse;
        }


        private void ShowChatView(string userInput)
        {
            ChatView chatView = new ChatView(userInput);
            chatView.Show();
            this.Close();
        }


        private void SendMessage_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }

        private void MainLoaded(object sender, RoutedEventArgs e) 
        {
            UserInput.Focus();
            UserInput.SelectAll();
        }

        private void SendMessage()
        {
            WelcomeImage.Visibility = Visibility.Collapsed;

            string userMessage = UserInput.Text;
            ShowChatView(userMessage);
            //if (!string.IsNullOrWhiteSpace(userMessage))
            //{
            //    ChatLog.Items.Add("You: " + userMessage);

            //    ChatLog.Items.Add("Bot: " + GenerateResponseString(userMessage));

            //    MessageBox.Show("User input: " + userMessage);
            //}

        }

        private void UserInput_TextChanged(object sender, TextChangedEventArgs e)
        {
            int lineCount = UserInput.LineCount;

            if (lineCount > 1)
            {
                UserInput.Height = UserInput.FontSize * lineCount + 10;
            }
            else
            {
                UserInput.Height = 30;
            }
        }
        private void UserInput_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter && Keyboard.Modifiers != ModifierKeys.Shift)
            {
                // Verhindere, dass ein Zeilenumbruch in der TextBox hinzugefügt wird
                e.Handled = true;

                // Sende die Nachricht
                SendMessage();
            }
        }
    }
}