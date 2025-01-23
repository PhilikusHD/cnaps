using System;
using System.Runtime.InteropServices;
using System.Windows;
using ChatbotFrontend;
class Program
{
    [STAThread]
    static void Main(string[] args)
    {
        var app = new Application();
        var mainWindow = new MainWindow();
        app.Run(mainWindow);
    }
}