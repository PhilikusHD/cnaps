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
    /// Interaktionslogik für ChatView.xaml
    /// </summary>
    public partial class ChatView : UserControl
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

        public static string GenerateResponseString(string input)
        {
            IntPtr resultPtr = GenerateResponse(input);
            string result = Marshal.PtrToStringAnsi(resultPtr);
            //FreeResponse(resultPtr);
            return result;
        }
        public static string GetErrorString()
        {
            return Marshal.PtrToStringAnsi(GetError());
        }


        string firstUserMessage;
        string botResponse;
        public ChatView(string userInput)
        {
            InitializeComponent();
            firstUserMessage = userInput;
            ChatLog.Items.Add(firstUserMessage);
        }

        private void SendMessage_Click(object sender, RoutedEventArgs e)
        {
            string userInputText = FormatMessage(UserInput.Text);
            ChatLog.Items.Add(new ChatMessage { Message = FormatMessage(userInputText), isBot = false});


            
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
    }

    public class ChatMessage
    { 
        public string Message {  get; set; }
        public bool isBot {  get; set; }
    }
}
