using System;
using System.Collections.Generic;
using System.Linq;
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

namespace RPGEditor
{
    public partial class UnsavedChangesDialog : Window
    {
        public static String ReturnResponse = "Cancel";
        public UnsavedChangesDialog()
        {
            InitializeComponent();
        }
        public void SetFileName(String FileName)
        {
            if (FileName.Length >= 40)
            {
                int GoodIndex = 0;
                int LastIndex = 0;
                while (LastIndex != -1)
                {
                    Console.WriteLine(LastIndex);
                    LastIndex = FileName.IndexOf("\\", LastIndex + 1);
                    if (LastIndex < 40 && LastIndex != -1)
                        GoodIndex = LastIndex;
                }
                FileName = FileName.Replace("_", "__");
                FileName = "..." + FileName.Substring(GoodIndex);
            }
            Label_Main.Content += "\r\n\"" + FileName + "\"?";
        }

        private void Button_Save_Click(object sender, RoutedEventArgs e)
        {
            ReturnResponse = "Save";
            Close();
        }

        private void Button_Ignore_Click(object sender, RoutedEventArgs e)
        {
            ReturnResponse = "Ignore";
            Close();
        }

        private void Button_Cancel_Click(object sender, RoutedEventArgs e)
        {
            ReturnResponse = "Cancel";
            Close();
        }
    }
}
