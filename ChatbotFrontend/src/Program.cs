using System;
using System.Runtime.InteropServices;

class Program
{
    [DllImport("ChatbotCore.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetGreeting();

    static void Main(string[] args)
    {
        // Call the C++ function
        IntPtr greetingPtr = GetGreeting();
        // Convert the C++ string to a C# string
        string greeting = Marshal.PtrToStringAnsi(greetingPtr);

        // Print the message
        Console.WriteLine(greeting);
    }
}