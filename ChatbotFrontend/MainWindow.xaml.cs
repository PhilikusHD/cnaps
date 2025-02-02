using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

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