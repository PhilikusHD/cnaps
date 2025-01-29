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

namespace ChatbotFrontend
{
    /// <summary>
    /// Interaktionslogik für ChatView.xaml
    /// </summary>
    /// 

    public class ChatMessage
    {
        public string Message { get; set; }
        public bool isBot { get; set; }
    }

    public class BoolToBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool isBot)
            {
                var brushConverter = new BrushConverter();
                return isBot
                    ? (Brush)brushConverter.ConvertFromString("#FFFFB9D3") //Bot
                    : (Brush)brushConverter.ConvertFromString("#FFFB90B7"); //User
            }
            return Brushes.Transparent;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

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
            ChatLog.Items.Add(firstUserMessage);
        }

        public static string GenerateResponseString(string input)
        {
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
            string userInputText = FormatMessage(UserInput.Text);
            ChatLog.Items.Add(new ChatMessage { Message = FormatMessage(userInputText), isBot = false });

            string botResponse = GenerateResponseString(userInputText);
            ChatLog.Items.Add(new ChatMessage { Message = FormatMessage(botResponse), isBot = true });

            UserInput.Clear();
        }
        
        private void MessageInBox(string message)
        {
            ChatMessage chatMessage = new ChatMessage();
            if (chatMessage.isBot)
            {

            }
        }

        private List<string> userInputs;
        private List<string> botResponses;
        private void GenerateTextbox()
        {
            foreach (string input in userInputs)
            {

            }
        }
        private void ChatLog_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {

        }

        private string FormatMessage(string input)
        {
            int maxLength = 20;
            string[] words = input.Split(' ');
            StringBuilder formattedMessage = new StringBuilder();
            string currentLine = "";

            foreach (string word in words)
            {
                if ((currentLine + word).Length > maxLength)
                {
                    formattedMessage.AppendLine(currentLine.Trim());
                    currentLine = word + " ";
                }
                else
                {
                    currentLine += word + " ";
                }
            }

            formattedMessage.AppendLine(currentLine.Trim());
            return formattedMessage.ToString().TrimEnd();
        }

        private void RichTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}
