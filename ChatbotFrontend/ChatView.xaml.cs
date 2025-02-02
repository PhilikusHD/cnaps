using System;
using System.Collections.Generic;
using System.Globalization;
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
using System.Xml;

namespace ChatbotFrontend
{
    /// <summary>
    /// Interaktionslogik für ChatView.xaml
    /// </summary>
    /// 

    public partial class ChatView : Window
    {
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeChatbot();
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ShutdownChatbot();
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr GenerateResponse([MarshalAs(UnmanagedType.LPStr)] string input);
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void FreeResponse(IntPtr response);

        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetStatus();
        [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr GetError();

        string firstUserMessage;
        public ChatView(string userInput)
        {
            InitializeComponent();
            firstUserMessage = userInput;
            AddMessageToChatLog(firstUserMessage, false);
            string botResponse = GenerateResponseString(firstUserMessage);
            AddMessageToChatLog(botResponse, true);
        }

        public static string GenerateResponseString(string input)
        {
            if (string.IsNullOrWhiteSpace(input))
            {
                return "Please type something in!";
            }
            IntPtr resultPtr = GenerateResponse(input);
            string result = Marshal.PtrToStringAnsi(resultPtr);
            FreeResponse(resultPtr);
            return result;
        }
        public static string GetErrorString()
        {
            return Marshal.PtrToStringAnsi(GetError());
        }


        private void SendMessage_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }

        private void SendMessage()
        {
            string userInputText = UserInput.Text;
            if (!string.IsNullOrWhiteSpace(userInputText))
            {
                AddMessageToChatLog(userInputText, false); // Benutzernachricht
                string botResponse = GenerateResponseString(userInputText);
                AddMessageToChatLog(botResponse, true); // Bot-Nachricht
                UserInput.Clear();
            }
        }


        private void AddMessageToChatLog(string message, bool isBot)
        {
            Paragraph paragraph = new Paragraph();
            paragraph.Margin = new Thickness(0, 5, 0, 5);

            paragraph.TextAlignment = isBot ? TextAlignment.Left : TextAlignment.Right;

            Border border = new Border
            {
                CornerRadius = new CornerRadius(10),
                Background = isBot ? (Brush)new BrushConverter().ConvertFromString("#FFFFB9D3") : (Brush)new BrushConverter().ConvertFromString("#FFFB90B7"),
                Padding = new Thickness(10),
                HorizontalAlignment = isBot ? HorizontalAlignment.Left : HorizontalAlignment.Right,
                Margin = new Thickness(5)
            };

            TextBlock textBlock = new TextBlock
            {
                Text = message,
                TextWrapping = TextWrapping.Wrap,
                Foreground = Brushes.Black
            };

            border.Child = textBlock;
            paragraph.Inlines.Add(border);
            ChatLog.Document.Blocks.Add(paragraph);

            // Scrollen zum Ende des Chat-Logs
            ChatLog.ScrollToEnd();
        }
        private void RichTextBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            UserInput.Focus();
            UserInput.SelectAll();
        }

        private void UserInput_TextChanged(object sender, EventArgs e)
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