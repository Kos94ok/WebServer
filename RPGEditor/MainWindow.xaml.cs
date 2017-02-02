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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml.Serialization;
using System.IO;

namespace RPGEditor
{
    public partial class MainWindow : Window
    {
        bool ChangeEventsDisabled = false;
        String LastSavePath = "";

        LocationData SelectedLocation;
        LocationSelector SelectedSelector;
        //==========================================================================================
        //==========================================================================================
        // Core
        //==========================================================================================
        //==========================================================================================
        public MainWindow()
        {
            InitializeComponent();
            WindowTitle.MakeDefault(this);
            DataHandler.SetTree(Tree_Hierarchy);

            ContextMenu LocationMenu = new ContextMenu();
            MenuItem LocationAddEntry = new MenuItem() { Header = "Создать локацию" };
            MenuItem LocationAddSelector = new MenuItem() { Header = "Создать разветвление" };
            MenuItem LocationAddFolder = new MenuItem() { Header = "Создать папку" };
            LocationAddEntry.Click += LocationAddEntry_Click;
            LocationAddSelector.Click += LocationAddSelector_Click;
            LocationAddFolder.Click += LocationAddFolder_Click;
            LocationMenu.Items.Add(LocationAddEntry);
            LocationMenu.Items.Add(LocationAddSelector);
            LocationMenu.Items.Add(LocationAddFolder);
            LocationMenu.Name = null;
            Tree_Hierarchy.ContextMenu = LocationMenu;

            (TabControl_Main.Items[0] as TabItem).Visibility = System.Windows.Visibility.Collapsed;
            (TabControl_Main.Items[1] as TabItem).Visibility = System.Windows.Visibility.Collapsed;
            (TabControl_Main.Items[2] as TabItem).Visibility = System.Windows.Visibility.Collapsed;
            InitializeDecisionButtons();
            InitializeDecisionButtonContextMenu();
        }

        //==========================================================================================
        // Toolbar - File
        //==========================================================================================
        private void Command_CanExecute_True(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }
        private void NewCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            ClearTreeView(Tree_Hierarchy);
            WindowTitle.MakeDefault(this);
        }
        private void OpenCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            dlg.DefaultExt = ".xml";
            dlg.Filter = "XML Files (*.xml)|*.xml|All Files|*";

            Nullable<bool> result = dlg.ShowDialog();
            if (result == true)
            {
                //Dogs = Importer.FromXML(dlg.FileName);
                WindowTitle.SetOpenFileName(this, dlg.FileName);
                LastSavePath = dlg.FileName;
                LoadTreeStructure(dlg.FileName);
            }
        }
        private void SaveCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            if (LastSavePath == null || LastSavePath.Length == 0)
            {
                SaveAsCommand_Executed(sender, e);
                return;
            }
            //Exporter.ToXML(Dogs, LastSavePath);
            SaveTreeStructure(Tree_Hierarchy, LastSavePath);
            WindowTitle.RemoveStar(this);
        }
        private void SaveAsCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog dlg = new Microsoft.Win32.SaveFileDialog();

            dlg.FileName = "output";
            dlg.DefaultExt = ".xml";
            dlg.Filter = "XML Files (*.xml)|*.xml|All Files|*";

            Nullable<bool> result = dlg.ShowDialog();
            if (result == true)
            {
                LastSavePath = dlg.FileName;
                WindowTitle.SetOpenFileName(this, dlg.FileName);
                SaveTreeStructure(Tree_Hierarchy, dlg.FileName);
            }
        }

        //==========================================================================================
        // Add new location
        //==========================================================================================
        public void LocationAddEntry_Click(object sender, RoutedEventArgs e)
        {
            MenuItem ClickedButton = sender as MenuItem;
            ContextMenu ClickedMenu = ClickedButton.Parent as ContextMenu;
            TreeViewItem ClickedItem = Utility.FindNodeByName(ClickedMenu.Name, Tree_Hierarchy.Items);
            DataHandler.AddLocation(this, ClickedItem);
            WindowTitle.AddStar(this);
        }
        public void LocationAddSelector_Click(object sender, RoutedEventArgs e)
        {
            MenuItem ClickedButton = sender as MenuItem;
            ContextMenu ClickedMenu = ClickedButton.Parent as ContextMenu;
            TreeViewItem ClickedItem = Utility.FindNodeByName(ClickedMenu.Name, Tree_Hierarchy.Items);
            DataHandler.AddSelector(this, ClickedItem);
            WindowTitle.AddStar(this);
        }
        //==========================================================================================
        // Add new folder
        //==========================================================================================
        public void LocationAddFolder_Click(object sender, RoutedEventArgs e)
        {
            MenuItem ClickedButton = sender as MenuItem;
            ContextMenu ClickedMenu = ClickedButton.Parent as ContextMenu;
            TreeViewItem ClickedItem = Utility.FindNodeByName(ClickedMenu.Name, Tree_Hierarchy.Items);
            DataHandler.AddFolderFull(this, ClickedItem);
            WindowTitle.AddStar(this);
        }
        //==========================================================================================
        // Delete object
        //==========================================================================================
        public void LocationRemove_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            DataHandler.RemoveLeaf(ClickedItem.Name);
            WindowTitle.AddStar(this);
        }
        public void FolderRemove_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            DataHandler.RemoveFolder(ClickedItem.Name);
            WindowTitle.AddStar(this);
        }
        
        //==========================================================================================
        // Copy/Paste
        //==========================================================================================
        public void LocationCopy_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            CopyPaster.IsCut = false;
            CopyPaster.CopyObjectName = ClickedItem.Name;
        }
        public void LocationCut_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            CopyPaster.IsCut = true;
            CopyPaster.CopyObjectName = ClickedItem.Name;
        }
        public void LocationPasteTo_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            CopyPaster.PasteTo(this, Tree_Hierarchy, ClickedItem.Name);
            WindowTitle.AddStar(this);
        }
        public void FolderCut_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            CopyPaster.IsCut = true;
            CopyPaster.CopyObjectName = ClickedItem.Name;
        }
        public void FolderCopy_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            CopyPaster.IsCut = false;
            CopyPaster.CopyObjectName = ClickedItem.Name;
        }
        public void FolderPasteTo_Click(object sender, RoutedEventArgs e)
        {
            TreeViewItem ClickedItem = Utility.GetClickedTreeViewItem(sender, Tree_Hierarchy);
            CopyPaster.PasteTo(this, Tree_Hierarchy, ClickedItem.Name);
            WindowTitle.AddStar(this);
        }
        //==========================================================================================
        // Editing
        //==========================================================================================
        public void Location_Selected(object sender, RoutedEventArgs e)
        {
            TabControl_Main.SelectedIndex = 1;
            e.Handled = true;
            TreeViewItem Sender = sender as TreeViewItem;

            ChangeEventsDisabled = true;
            SetControlStatusToLocation();
            foreach (LocationData Data in DataHandler.LocationList)
            {
                if (Sender.Name == Data.TreeViewId)
                {
                    SelectedLocation = Data;
                    SelectedSelector = null;
                    Text_Guid.Text = Data.Guid;
                    // Parse description
                    Text_Description.Document.Blocks.Clear();
                    // Simple parse - no markup
                    if (Data.Description == null || Data.Description.IndexOf("<p>") == -1)
                        Text_Description.Document.Blocks.Add(new Paragraph(new Run(Data.Description)));
                    // True parse
                    else
                    {
                        String Descr = new String(Data.Description.ToArray());
                        while (Descr.IndexOf("<p>") != -1)
                        {
                            int StartIndex = Descr.IndexOf("<p>") + 3;
                            int EndIndex = Descr.IndexOf("</p>");
                            String Substring = Descr.Substring(StartIndex, EndIndex - StartIndex);
                            Substring = Substring.Replace("<br>", "\r\n");
                            Text_Description.Document.Blocks.Add(new Paragraph(new Run(Substring)));
                            Descr = Descr.Remove(StartIndex - 3, 3);
                            Descr = Descr.Remove(EndIndex - 3, 4);
                        }
                    }
                    // End of parse
                    for (int i = 0; i < Data.Decisions.Count; i++)
                    {
                        String Index = i.ToString();
                        if (i < 10)
                            Index = "0" + Index;
                        TextBox Box = DecisionContainer.FindName("Text_DecisionText" + Index) as TextBox;
                        Box.Text = Data.Decisions[i].Text;
                        Box = DecisionContainer.FindName("Text_DecisionLink" + Index) as TextBox;
                        Box.Text = Data.Decisions[i].LocationLink;
                        Box = DecisionContainer.FindName("Text_DecisionCondition" + Index) as TextBox;
                        Box.Text = Data.Decisions[i].Condition;
                        Box = DecisionContainer.FindName("Text_DecisionAction" + Index) as TextBox;
                        Box.Text = Data.Decisions[i].Action;
                    }
                    // Check amount of decisions to unfold
                    int DecisionCount = 0;
                    for (int i = 0; i < Data.Decisions.Count; i++)
                    {
                        if ((Data.Decisions[i].Text != null && Data.Decisions[i].Text != "")
                            || (Data.Decisions[i].LocationLink != null && Data.Decisions[i].LocationLink != ""))
                        {
                            DecisionCount = i + 1;
                        }
                    }
                    UnfoldDecisions(DecisionCount);
                    break;
                }
            }
            ChangeEventsDisabled = false;
        }
        public void Selector_Selected(object sender, RoutedEventArgs e)
        {
            TabControl_Main.SelectedIndex = 2;
            e.Handled = true;
            TreeViewItem Sender = sender as TreeViewItem;

            ChangeEventsDisabled = true;

            LocationSelector Selector = DataHandler.GetSelectorByTreeViewId(Sender.Name);
            if (Selector != null)
            {
                SelectedSelector = Selector;
                SelectedLocation = null;
                Text_Guid.Text = Selector.Guid;
                DataGrid_SelectorLinks.ItemsSource = Selector.Options;
            }

            ChangeEventsDisabled = false;
        }
        public void Folder_Selected(object sender, RoutedEventArgs e)
        {
            ChangeEventsDisabled = true;
            TabControl_Main.SelectedIndex = 0;
            e.Handled = true;
            TreeViewItem Sender = sender as TreeViewItem;

            SelectedLocation = null;
            SelectedSelector = null;
            SetControlStatusToFolder();
            Text_Guid.Text = Sender.Header.ToString();
            ChangeEventsDisabled = false;
        }
        //==========================================================================================
        // Hotkeys
        //==========================================================================================
        public void Location_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
            TreeViewItem SelectedItem = sender as TreeViewItem;
            if (e.Key == Key.C && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                CopyPaster.IsCut = false;
                CopyPaster.CopyObjectName = SelectedItem.Name;
            }
            else if (e.Key == Key.X && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                CopyPaster.IsCut = true;
                CopyPaster.CopyObjectName = SelectedItem.Name;
            }
            else if (e.Key == Key.V && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                CopyPaster.PasteTo(this, Tree_Hierarchy, SelectedItem.Name);
                WindowTitle.AddStar(this);
            }
        }
        public void Folder_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
            TreeViewItem SelectedItem = sender as TreeViewItem;
            if (e.Key == Key.C && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                CopyPaster.IsCut = false;
                CopyPaster.CopyObjectName = SelectedItem.Name;
            }
            else if (e.Key == Key.X && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                CopyPaster.IsCut = true;
                CopyPaster.CopyObjectName = SelectedItem.Name;
            }
            else if (e.Key == Key.V && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                CopyPaster.PasteTo(this, Tree_Hierarchy, SelectedItem.Name);
                WindowTitle.AddStar(this);
            }
        }
        //==========================================================================================
        //==========================================================================================
        // Save/load
        //==========================================================================================
        //==========================================================================================
        public void SaveTreeStructure(TreeView Tree, String Filename)
        {
            try
            {
                StreamWriter Writer = new StreamWriter(Filename, false, System.Text.Encoding.UTF8);
                foreach (TreeViewItem Item in Tree.Items)
                {
                    SaveTreeItem(Writer, Item, 0);
                }
                Writer.Close();
                WindowTitle.RemoveStar(this);
            }
            catch (Exception) { MessageBox.Show("Сохранение не удалось", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error); }
        }
        private void SaveTreeItem(StreamWriter Writer, TreeViewItem Item, int Depth)
        {
            // LocationData
            LocationData Data = DataHandler.GetLocationByTreeViewId(Item.Name);
            if (Data != null)
            {
                AddSpaces(Writer, Depth);   Writer.WriteLine("<LocationData>");
                AddSpaces(Writer, Depth + 1);   Writer.WriteLine("<TreeViewId>" + Data.TreeViewId + "</TreeViewId>");
                AddSpaces(Writer, Depth + 1);   Writer.WriteLine("<Guid>" + Data.Guid + "</Guid>");
                AddSpaces(Writer, Depth + 1);   Writer.WriteLine("<Description>" + Data.Description + "</Description>");
                AddSpaces(Writer, Depth + 1);   Writer.WriteLine("<Decisions>");
                foreach (LocationDecision Decision in Data.Decisions)
                {
                    if (Decision.Text != "" && Decision.LocationLink != "")
                    {
                        AddSpaces(Writer, Depth + 2); Writer.WriteLine("<LocationDecision>");
                        AddSpaces(Writer, Depth + 3); Writer.WriteLine("<Text>" + Decision.Text + "</Text>");
                        AddSpaces(Writer, Depth + 3); Writer.WriteLine("<LocationLink>" + Decision.LocationLink + "</LocationLink>");
                        if (Decision.Condition != null && Decision.Condition != "")
                        {
                            AddSpaces(Writer, Depth + 3);
                            Writer.WriteLine("<Condition>" + Decision.Condition + "</Condition>");
                        }
                        if (Decision.Action != null && Decision.Action != "")
                        {
                            AddSpaces(Writer, Depth + 3);
                            Writer.WriteLine("<Action>" + Decision.Action + "</Action>");
                        }
                        AddSpaces(Writer, Depth + 2); Writer.WriteLine("</LocationDecision>");
                    }
                }
                AddSpaces(Writer, Depth + 1);   Writer.WriteLine("</Decisions>");
                AddSpaces(Writer, Depth);   Writer.WriteLine("</LocationData>");
            }
            // LocationSelector
            LocationSelector Selector = DataHandler.GetSelectorByTreeViewId(Item.Name);
            if (Selector != null)
            {
                AddSpaces(Writer, Depth); Writer.WriteLine("<LocationSelector>");
                AddSpaces(Writer, Depth + 1); Writer.WriteLine("<TreeViewId>" + Selector.TreeViewId + "</TreeViewId>");
                AddSpaces(Writer, Depth + 1); Writer.WriteLine("<Guid>" + Selector.Guid + "</Guid>");
                AddSpaces(Writer, Depth + 1); Writer.WriteLine("<Options>");
                foreach (LocationSelectorEntry Option in Selector.Options)
                {
                    if (Option.Link != "")
                    {
                        AddSpaces(Writer, Depth + 2); Writer.WriteLine("<SelectorOption>");
                        AddSpaces(Writer, Depth + 3); Writer.WriteLine("<Link>" + Option.Link + "</Link>");
                        AddSpaces(Writer, Depth + 3); Writer.WriteLine("<Chance>" + Option.Chance + "</Chance>");
                        if (Option.Condition != null && Option.Condition != "")
                        {
                            AddSpaces(Writer, Depth + 3);
                            Writer.WriteLine("<Condition>" + Option.Condition + "</Condition>");
                        }
                        if (Option.Action != null && Option.Action != "")
                        {
                            AddSpaces(Writer, Depth + 3);
                            Writer.WriteLine("<Action>" + Option.Action + "</Action>");
                        }
                        AddSpaces(Writer, Depth + 2); Writer.WriteLine("</SelectorOption>");
                    }
                }
                AddSpaces(Writer, Depth + 1); Writer.WriteLine("</Options>");
                AddSpaces(Writer, Depth); Writer.WriteLine("</LocationSelector>");
            }

            if (Data == null && Selector == null)
            {
                AddSpaces(Writer, Depth); Writer.WriteLine("<Folder>");
                AddSpaces(Writer, Depth + 1); Writer.WriteLine("<Name>" + Item.Name + "</Name>");
                AddSpaces(Writer, Depth + 1); Writer.WriteLine("<Header>" + Item.Header + "</Header>");
                foreach (TreeViewItem Child in Item.Items)
                {
                    SaveTreeItem(Writer, Child, Depth + 1);
                }
                AddSpaces(Writer, Depth); Writer.WriteLine("</Folder>");
            }
        }
        void AddSpaces(StreamWriter Writer, int Depth)
        {
            for (int i = 0; i < Depth * 4; i++)
                Writer.Write(' ');
        }
        void LoadTreeStructure(String Filename)
        {
            ClearTreeView(Tree_Hierarchy);
            DataHandler.LocationList.Clear();
            WindowTitle.RemoveStar(this);

            bool ParsingData = false;
            bool ParsingDecision = false;
            bool ParsingSelector = false;
            bool ParsingSelectorOption = false;
            TreeViewItem Folder = null;
            TreeViewItem LocationItem = null;
            LocationData LocationEntry = null;
            LocationSelector Selector = null;
            Stack<TreeViewItem> Parent = new Stack<TreeViewItem>();

            StreamReader Reader = new StreamReader(Filename, System.Text.Encoding.UTF8);
            while (!Reader.EndOfStream)
            {
                String Line = Reader.ReadLine();
                // Parsing location data
                if (Line.Contains("<LocationData>") == true)
                {
                    ParsingData = true;
                    LocationItem = DataHandler.AddLocation(this, Folder);
                    LocationEntry = new LocationData();
                    DataHandler.LocationList.Add(LocationEntry);
                }
                else if (Line.Contains("TreeViewId") == true && ParsingData)
                {
                    int StartIndex = Line.IndexOf("<TreeViewId>") + 12;
                    int EndIndex = Line.IndexOf("</TreeViewId>");
                    LocationItem.Name = Line.Substring(StartIndex, EndIndex - StartIndex);
                    LocationItem.ContextMenu.Name = LocationItem.Name;
                    LocationEntry.TreeViewId = LocationItem.Name;
                }
                else if (Line.Contains("<Guid>") == true && ParsingData)
                {
                    int StartIndex = Line.IndexOf("<Guid>") + 6;
                    int EndIndex = Line.IndexOf("</Guid>");
                    LocationEntry.Guid = Line.Substring(StartIndex, EndIndex - StartIndex);
                    LocationItem.Header = LocationEntry.Guid;
                    if (LocationItem.Header.ToString().Length == 0)
                        LocationItem.Header = "Unnamed";
                }
                else if (Line.Contains("<Description>") == true && ParsingData)
                {
                    int StartIndex = Line.IndexOf("<Description>") + 13;
                    int EndIndex = Line.IndexOf("</Description>");
                    LocationEntry.Description = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("<LocationDecision>") == true && ParsingData)
                {
                    ParsingDecision = true;
                    LocationEntry.Decisions.Add(new LocationDecision());
                }
                else if (Line.Contains("<Text>") == true && ParsingDecision)
                {
                    int StartIndex = Line.IndexOf("<Text>") + 6;
                    int EndIndex = Line.IndexOf("</Text>");
                    LocationEntry.Decisions[LocationEntry.Decisions.Count - 1].Text = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("<LocationLink>") == true && ParsingDecision)
                {
                    int StartIndex = Line.IndexOf("<LocationLink>") + 14;
                    int EndIndex = Line.IndexOf("</LocationLink>");
                    LocationEntry.Decisions[LocationEntry.Decisions.Count - 1].LocationLink = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("<Condition>") == true && ParsingDecision)
                {
                    int StartIndex = Line.IndexOf("<Condition>") + 11;
                    int EndIndex = Line.IndexOf("</Condition>");
                    LocationEntry.Decisions[LocationEntry.Decisions.Count - 1].Condition = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("<Action>") == true && ParsingDecision)
                {
                    int StartIndex = Line.IndexOf("<Action>") + 8;
                    int EndIndex = Line.IndexOf("</Action>");
                    LocationEntry.Decisions[LocationEntry.Decisions.Count - 1].Action = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("/LocationDecision>") == true)
                {
                    ParsingDecision = false;
                }
                else if (Line.Contains("</LocationData>") == true)
                {
                    LocationItem = null;
                    ParsingData = false;
                }
                // Parsing location selector data
                else if (Line.Contains("<LocationSelector>") == true)
                {
                    ParsingSelector = true;
                    LocationItem = DataHandler.AddSelector(this, Folder);
                    Selector = new LocationSelector();
                    DataHandler.SelectorList.Add(Selector);
                }
                else if (Line.Contains("TreeViewId") == true && ParsingSelector)
                {
                    int StartIndex = Line.IndexOf("<TreeViewId>") + 12;
                    int EndIndex = Line.IndexOf("</TreeViewId>");
                    LocationItem.Name = Line.Substring(StartIndex, EndIndex - StartIndex);
                    LocationItem.ContextMenu.Name = LocationItem.Name;
                    Selector.TreeViewId = LocationItem.Name;
                }
                else if (Line.Contains("<Guid>") == true && ParsingSelector)
                {
                    int StartIndex = Line.IndexOf("<Guid>") + 6;
                    int EndIndex = Line.IndexOf("</Guid>");
                    Selector.Guid = Line.Substring(StartIndex, EndIndex - StartIndex);
                    LocationItem.Header = Selector.Guid;
                    if (LocationItem.Header.ToString().Length == 0)
                        LocationItem.Header = "Unnamed";
                }
                else if (Line.Contains("<SelectorOption>") == true && ParsingSelector)
                {
                    ParsingSelectorOption = true;
                    Selector.Options.Add(new LocationSelectorEntry());
                }
                else if (Line.Contains("<Link>") == true && ParsingSelectorOption)
                {
                    int StartIndex = Line.IndexOf("<Link>") + 6;
                    int EndIndex = Line.IndexOf("</Link>");
                    Selector.Options[Selector.Options.Count - 1].Link = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("<Chance>") == true && ParsingSelectorOption)
                {
                    int StartIndex = Line.IndexOf("<Chance>") + 8;
                    int EndIndex = Line.IndexOf("</Chance>");
                    Selector.Options[Selector.Options.Count - 1].Chance = Int32.Parse(Line.Substring(StartIndex, EndIndex - StartIndex));
                }
                else if (Line.Contains("<Condition>") == true && ParsingSelectorOption)
                {
                    int StartIndex = Line.IndexOf("<Condition>") + 11;
                    int EndIndex = Line.IndexOf("</Condition>");
                    Selector.Options[Selector.Options.Count - 1].Condition = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("<Action>") == true && ParsingSelectorOption)
                {
                    int StartIndex = Line.IndexOf("<Action>") + 8;
                    int EndIndex = Line.IndexOf("</Action>");
                    Selector.Options[Selector.Options.Count - 1].Action = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("/SelectorOption>") == true)
                {
                    ParsingDecision = false;
                }
                else if (Line.Contains("</LocationSelector>") == true)
                {
                    LocationItem = null;
                    ParsingSelector = false;
                }
                // Parsing folder data
                else if (Line.Contains("<Folder>") == true)
                {
                    if (Folder != null)
                    {
                        TreeViewItem LastParent = Folder;
                        Parent.Push(LastParent);
                        Folder = DataHandler.AddFolderFull(this, LastParent);
                    }
                    else
                        Folder = DataHandler.AddFolderFull(this, null);
                }
                else if (Line.Contains("<Name>"))
                {
                    int StartIndex = Line.IndexOf("<Name>") + 6;
                    int EndIndex = Line.IndexOf("</Name>");
                    Folder.Name = Line.Substring(StartIndex, EndIndex - StartIndex);
                    Folder.ContextMenu.Name = Folder.Name;
                }
                else if (Line.Contains("<Header>"))
                {
                    int StartIndex = Line.IndexOf("<Header>") + 8;
                    int EndIndex = Line.IndexOf("</Header>");
                    Folder.Header = Line.Substring(StartIndex, EndIndex - StartIndex);
                }
                else if (Line.Contains("</Folder>"))
                {
                    if (Parent.Count == 0)
                    {
                        Folder = null;
                    }
                    else
                    {
                        Folder = Parent.Pop();
                    }
                }
            }
        }
        void ClearTreeView(TreeView Tree)
        {
            WindowTitle.RemoveStar(this);
            Tree.Items.Clear();
        }

        //==========================================================================================
        //==========================================================================================
        // Utility
        //==========================================================================================
        //==========================================================================================
        void SetControlStatusToLocation()
        {
            Text_Description.Document.Blocks.Clear();
            Text_DecisionLink00.Text = "";
            Text_DecisionLink01.Text = "";
            Text_DecisionLink02.Text = "";
            Text_DecisionLink03.Text = "";
            Text_DecisionText00.Text = "";
            Text_DecisionText01.Text = "";
            Text_DecisionText02.Text = "";
            Text_DecisionText03.Text = "";

            Text_Description.IsEnabled = true;
            Text_DecisionLink00.IsEnabled = true;
            Text_DecisionLink01.IsEnabled = true;
            Text_DecisionLink02.IsEnabled = true;
            Text_DecisionLink03.IsEnabled = true;
            Text_DecisionText00.IsEnabled = true;
            Text_DecisionText01.IsEnabled = true;
            Text_DecisionText02.IsEnabled = true;
            Text_DecisionText03.IsEnabled = true;
        }
        void SetControlStatusToFolder()
        {
            Text_Description.Document.Blocks.Clear();
            Text_DecisionLink00.Text = "";
            Text_DecisionLink01.Text = "";
            Text_DecisionLink02.Text = "";
            Text_DecisionLink03.Text = "";
            Text_DecisionText00.Text = "";
            Text_DecisionText01.Text = "";
            Text_DecisionText02.Text = "";
            Text_DecisionText03.Text = "";

            Text_Description.IsEnabled = false;
            Text_DecisionLink00.IsEnabled = false;
            Text_DecisionLink01.IsEnabled = false;
            Text_DecisionLink02.IsEnabled = false;
            Text_DecisionLink03.IsEnabled = false;
            Text_DecisionText00.IsEnabled = false;
            Text_DecisionText01.IsEnabled = false;
            Text_DecisionText02.IsEnabled = false;
            Text_DecisionText03.IsEnabled = false;
        }
        private void Text_Guid_Changed(object sender, TextChangedEventArgs e)
        {
            if (ChangeEventsDisabled == true)
                return;

            WindowTitle.AddStar(this);
            TreeViewItem SelectedItem = Tree_Hierarchy.SelectedItem as TreeViewItem;
            if (SelectedItem != null)
                SelectedItem.Header = Text_Guid.Text;

            if (SelectedLocation != null)
                SelectedLocation.Guid = Text_Guid.Text;
            else if (SelectedSelector != null)
                SelectedSelector.Guid = Text_Guid.Text;
        }
        private void Text_Description_Changed(object sender, TextChangedEventArgs e)
        {
            if (ChangeEventsDisabled == true)
                return;

            if (SelectedLocation != null)
            {
                WindowTitle.AddStar(this);
                //TextRange textRange = new TextRange(Text_Description.Document.ContentStart, Text_Description.Document.ContentEnd);
                var Paragraphs = Text_Description.Document.Blocks.ToList();
                String Output = "";
                for (int i = 0; i < Paragraphs.Count; i++)
                {
                    Output += "<p>";
                    TextRange textRange = new TextRange(Paragraphs[i].ContentStart, Paragraphs[i].ContentEnd);
                    Output += textRange.Text;
                    Output += "</p>";
                }
                Output = Output.Replace("\r\n", "<br>");
                SelectedLocation.Description = Output;
            }
        }
        private void Text_DecisionText_Changed(object sender, TextChangedEventArgs e)
        {
            if (ChangeEventsDisabled == true)
                return;

            WindowTitle.AddStar(this);
            TextBox Sender = sender as TextBox;
            int Index = Int32.Parse(Sender.Name.Substring(Sender.Name.Length - 2));
            if (SelectedLocation != null)
            {
                while (SelectedLocation.Decisions.Count < Index + 1)
                    SelectedLocation.Decisions.Add(new LocationDecision());
                SelectedLocation.Decisions[Index].Text = Sender.Text;
            }
        }
        private void Text_DecisionLink_Changed(object sender, TextChangedEventArgs e)
        {
            if (ChangeEventsDisabled == true)
                return;

            WindowTitle.AddStar(this);
            TextBox Sender = sender as TextBox;
            int Index = Int32.Parse(Sender.Name.Substring(Sender.Name.Length - 2));
            if (SelectedLocation != null)
            {
                while (SelectedLocation.Decisions.Count < Index + 1)
                    SelectedLocation.Decisions.Add(new LocationDecision());
                SelectedLocation.Decisions[Index].LocationLink = Sender.Text;
            }
        }
        private void Text_DecisionCondition_Changed(object sender, TextChangedEventArgs e)
        {
            if (ChangeEventsDisabled == true)
                return;

            WindowTitle.AddStar(this);
            TextBox Sender = sender as TextBox;
            int Index = Int32.Parse(Sender.Name.Substring(Sender.Name.Length - 2));
            if (SelectedLocation != null)
            {
                while (SelectedLocation.Decisions.Count < Index + 1)
                    SelectedLocation.Decisions.Add(new LocationDecision());
                SelectedLocation.Decisions[Index].Condition = Sender.Text;
            }
        }
        private void Text_DecisionAction_Changed(object sender, TextChangedEventArgs e)
        {
            if (ChangeEventsDisabled == true)
                return;

            WindowTitle.AddStar(this);
            TextBox Sender = sender as TextBox;
            int Index = Int32.Parse(Sender.Name.Substring(Sender.Name.Length - 2));
            if (SelectedLocation != null)
            {
                while (SelectedLocation.Decisions.Count < Index + 1)
                    SelectedLocation.Decisions.Add(new LocationDecision());
                SelectedLocation.Decisions[Index].Action = Sender.Text;
            }
        }
        private void Text_DecisionUnfolded_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                TextBox Sender = sender as TextBox;
                Keyboard.ClearFocus();
                Grid ParentGrid = Sender.Parent as Grid;
                ParentGrid.RowDefinitions[1].Height = new GridLength(0);
            }
        }
        public void TreeViewItem_ContextMenuOpened(object sender, RoutedEventArgs e)
        {
            ContextMenu Sender = sender as ContextMenu;
            TreeViewItem ClickedItem = Utility.FindNodeByName(Sender.Name, Tree_Hierarchy.Items);
            ClickedItem.IsSelected = true;
        }

        //==========================================================================================
        //==========================================================================================
        // Unsorted code
        //==========================================================================================
        //==========================================================================================
        int DecisionsCurrentlyDisplayed = 1;
        const int MaximumDecisionsPerLocation = 10;

        void InitializeDecisionButtons()
        {
            CollapseAllDecisions();
            InitializeDecisionButtonContextMenu();
        }
        void InitializeDecisionButtonContextMenu()
        {
            for (int i = 0; i < MaximumDecisionsPerLocation; i++)
            {
                Button DecisionButton = FindName("Button_Decision0" + i.ToString()) as Button;
                if (DecisionButton != null)
                {
                    ContextMenu ButtonMenu = new ContextMenu();
                    MenuItem ItemAdd = new MenuItem { Header = "Создать новый объект" };
                    MenuItem ItemGoto = new MenuItem { Header = "Перейти по ссылке" };
                    MenuItem ItemShowhide = new MenuItem { Name = "Item_ShowHide", Header = "Развернуть..." };

                    ItemAdd.Click += AddNewDecisionSlot_Click;
                    ItemGoto.Click += MoveToLocationEntry_Click;
                    ItemShowhide.Click += ShowHideDecision_Click;
                    ButtonMenu.Items.Add(ItemShowhide);
                    ButtonMenu.Items.Add(new Separator());
                    ButtonMenu.Items.Add(ItemAdd);
                    ButtonMenu.Items.Add(ItemGoto);
                    ButtonMenu.Name = DecisionButton.Name;
                    DecisionButton.ContextMenu = ButtonMenu;
                }
            }
        }
        void CollapseAllDecisions()
        {
            for (int i = 0; i < MaximumDecisionsPerLocation; i++)
            {
                Grid ParentGrid = FindName("Grid_Decision0" + i.ToString()) as Grid;
                ParentGrid.RowDefinitions[1].Height = new GridLength(0);
                if (i >= 1)
                {
                    GroupBox DecisionGroup = FindName("GroupBox_Decision0" + i.ToString()) as GroupBox;
                    if (DecisionGroup != null)
                    {
                        DecisionGroup.Visibility = System.Windows.Visibility.Collapsed;
                    }
                }
            }
            DecisionsCurrentlyDisplayed = 1;
        }
        void UnfoldDecisions(int count)
        {
            CollapseAllDecisions();
            DecisionsCurrentlyDisplayed = count;
            for (int i = 1; i < DecisionsCurrentlyDisplayed; i++)
            {
                GroupBox DecisionGroup = FindName("GroupBox_Decision0" + i.ToString()) as GroupBox;
                if (DecisionGroup != null)
                {
                    DecisionGroup.Visibility = System.Windows.Visibility.Visible;
                }
            }
        }

        private void Button_Decision_Click(object sender, RoutedEventArgs e)
        {
            Button ClickedButton = sender as Button;
            Grid ParentGrid = ClickedButton.Parent as Grid;
            for (int i = 0; i < ClickedButton.ContextMenu.Items.Count; i++)
            {
                MenuItem Item = ClickedButton.ContextMenu.Items[i] as MenuItem;
                if (Item != null && Item.Name.ToString() == "Item_ShowHide" && ParentGrid.RowDefinitions[1].Height.Value > 0)
                    Item.Header = "Свернуть";
                else if (Item != null && Item.Name.ToString() == "Item_ShowHide" && ParentGrid.RowDefinitions[1].Height.Value == 0)
                    Item.Header = "Развернуть";
            }

            ClickedButton.ContextMenu.IsEnabled = true;
            ClickedButton.ContextMenu.PlacementTarget = (sender as Button);
            ClickedButton.ContextMenu.Placement = System.Windows.Controls.Primitives.PlacementMode.Bottom;
            ClickedButton.ContextMenu.IsOpen = true;
        }
        private void AddNewDecisionSlot_Click(object sender, RoutedEventArgs e)
        {
            if (DecisionsCurrentlyDisplayed < MaximumDecisionsPerLocation)
            {
                String Number = DecisionsCurrentlyDisplayed.ToString();
                if (DecisionsCurrentlyDisplayed < 10)
                    Number = "0" + Number;
                GroupBox BoxToUnravel = DecisionContainer.FindName("GroupBox_Decision" + Number) as GroupBox;
                BoxToUnravel.Visibility = System.Windows.Visibility.Visible;

                DecisionsCurrentlyDisplayed += 1;
            }
        }
        private void MoveToLocationEntry_Click(object sender, RoutedEventArgs e)
        {

        }
        private void ShowHideDecision_Click(object sender, RoutedEventArgs e)
        {
            MenuItem ClickedButton = sender as MenuItem;
            ContextMenu ClickedMenu = ClickedButton.Parent as ContextMenu;
            String GridName = "Grid_Decision" + ClickedMenu.Name.Substring(ClickedMenu.Name.Length - 2);
            Grid ParentGrid = DecisionContainer.FindName(GridName) as Grid;
            if (ParentGrid.RowDefinitions[1].Height.Value > 0)
                ParentGrid.RowDefinitions[1].Height = new GridLength(0);
            else
                ParentGrid.RowDefinitions[1].Height = new GridLength(70);
        }

        private void CheckUnsavedChanges(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (WindowTitle.HasUnsavedChanges() && (DataHandler.LocationList.Count > 0 || DataHandler.SelectorList.Count > 0))
            {
                UnsavedChangesDialog dlg = new UnsavedChangesDialog();
                if (WindowState == WindowState.Maximized)
                {
                    dlg.Left = SystemParameters.VirtualScreenWidth / 2 - 175;
                    dlg.Top = SystemParameters.VirtualScreenHeight / 2 - 65;
                }
                else
                {
                    dlg.Left = Left + Width / 2 - 175;
                    dlg.Top = Top + Height / 2 - 65;
                }
                dlg.ResizeMode = System.Windows.ResizeMode.NoResize;
                dlg.Owner = this;
                dlg.SetFileName(WindowTitle.GetOpenFileName());
                dlg.ShowDialog();
                if (UnsavedChangesDialog.ReturnResponse == "Cancel")
                {
                    e.Cancel = true;
                }
                else if (UnsavedChangesDialog.ReturnResponse == "Save")
                {
                    SaveCommand_Executed(sender, null);
                }
            }
        }
        private void ShowAboutDialog(object sender, RoutedEventArgs e)
        {
            AboutDialog dlg = new AboutDialog();
            dlg.Left = Left + 10;
            dlg.Top = Top + 30;
            dlg.Owner = this;
            dlg.ShowDialog();
        }

        private void DataGrid_SelectorOptions_GeneratingColumn(object sender, DataGridAutoGeneratingColumnEventArgs e)
        {
            if (e.Column.Header.ToString() == "Link")
            {
                e.Column.Header = "Ссылка";
                e.Column.Width = new DataGridLength(5, DataGridLengthUnitType.Star);
                e.Column.MinWidth = 100;
            }
            else if (e.Column.Header.ToString() == "Chance")
            {
                e.Column.Header = "Шанс";
                e.Column.Width = new DataGridLength(55);
                e.Column.MinWidth = 45;
            }
            else if (e.Column.Header.ToString() == "Condition")
            {
                e.Column.Header = "Условие";
                e.Column.Width = new DataGridLength(3, DataGridLengthUnitType.Star);
                e.Column.MinWidth = 1;
            }
            else if (e.Column.Header.ToString() == "Action")
            {
                e.Column.Header = "Действие";
                e.Column.Width = new DataGridLength(3, DataGridLengthUnitType.Star);
                e.Column.MinWidth = 1;
            }
        }

        private void DataGrid_CellChanged(object sender, EventArgs e)
        {
            WindowTitle.AddStar(this);
        }
    }
}