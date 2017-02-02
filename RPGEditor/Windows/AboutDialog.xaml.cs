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
    public partial class AboutDialog : Window
    {
        public AboutDialog()
        {
            InitializeComponent();
            String Content = (String)Label_Version.Content;
            Label_Version.Content = Content.Replace("%CurrentVersion%", Version.Current);

            FlowDocument Document = new FlowDocument();

            // Add paragraphs to the FlowDocument.
            Paragraph Para = new Paragraph();
            Para.Inlines.Add(new Underline(new Run("Версия 2.0\n")));
            Para.Inlines.Add(new Run("- Добавлена поддержка до 10 опций на локацию.\n"));
            Para.Inlines.Add(new Run("- Добавлена поддержка разветвлений.\n"));
            Para.Inlines.Add(new Run("- Многочисленные улучшения дизайна."));
            Document.Blocks.Add(Para);

            Text_VersionHistoryRich.Document = Document;
        }

        private void Button_Accept_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
