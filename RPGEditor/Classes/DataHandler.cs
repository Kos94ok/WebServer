using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Collections.ObjectModel;

namespace RPGEditor
{
    public static class DataHandler
    {
        public static List<LocationData> LocationList = new List<LocationData>();
        public static List<LocationSelector> SelectorList = new List<LocationSelector>();

        public static TreeView Tree_Hierarchy { get; set; }
        public static void SetTree(TreeView Tree)
        {
            Tree_Hierarchy = Tree;
        }

        public static TreeViewItem AddLeaf(MainWindow WindowHandle, TreeViewItem Parent, Type LeafType)
        {
            // Creating new object
            TreeViewItem NewLeaf = new TreeViewItem();
            NewLeaf.Header = "Unnamed";
            // Generating the name
            String NameGen = Guid.NewGuid().ToString().Replace('-', '_');
            if (LeafType == typeof(LocationData))
                NameGen = "LocationData_" + NameGen;
            else if (LeafType == typeof(LocationSelector))
                NameGen = "LocationSelector_" + NameGen;
            NewLeaf.Name = NameGen;
            // Pushing to TreeView
            if (Parent == null)
                Tree_Hierarchy.Items.Add(NewLeaf);
            else
            {
                Parent.Items.Add(NewLeaf);
                Parent.IsExpanded = true;
            }
            // Checking the image
            //Console.WriteLine((Tree_Hierarchy.FindName("HeaderIcon") as Image).Source);
            // Pushing to database
            if (LeafType == typeof(LocationData))
            {
                LocationData NewLocation = new LocationData();
                NewLocation.TreeViewId = NewLeaf.Name;
                NewLocation.Guid = NewLeaf.Header.ToString();
                LocationList.Add(NewLocation);
            }
            else if (LeafType == typeof(LocationSelector))
            {
                LocationSelector NewSelector = new LocationSelector();
                NewSelector.TreeViewId = NewLeaf.Name;
                NewSelector.Guid = NewLeaf.Header.ToString();
                SelectorList.Add(NewSelector);
            }
            return NewLeaf;
        }
        public static TreeViewItem AddLocation(MainWindow WindowHandle, TreeViewItem Item)
        {
            // Create a leaf
            TreeViewItem Leaf = DataHandler.AddLeaf(WindowHandle, Item, typeof(LocationData));
            Leaf.Header = "Новая локация";
            Leaf.Selected += WindowHandle.Location_Selected;
            Leaf.KeyDown += WindowHandle.Location_KeyDown;

            // Create a context menu
            ContextMenu LocationMenu = new ContextMenu();
            MenuItem CutLocation = new MenuItem() { Header = "Вырезать" };
            MenuItem CopyLocation = new MenuItem() { Header = "Копировать" };
            MenuItem PasteToLocation = new MenuItem() { Header = "Вставить" };
            MenuItem LocationRemove = new MenuItem() { Header = "Удалить" };
            CutLocation.Click += WindowHandle.LocationCut_Click;
            CopyLocation.Click += WindowHandle.LocationCopy_Click;
            PasteToLocation.Click += WindowHandle.LocationPasteTo_Click;
            LocationRemove.Click += WindowHandle.LocationRemove_Click;
            LocationMenu.Items.Add(CutLocation);
            LocationMenu.Items.Add(CopyLocation);
            LocationMenu.Items.Add(PasteToLocation);
            LocationMenu.Items.Add(new Separator());
            LocationMenu.Items.Add(LocationRemove);
            LocationMenu.Name = Leaf.Name;
            Leaf.ContextMenu = LocationMenu;
            LocationMenu.Opened += WindowHandle.TreeViewItem_ContextMenuOpened;
            // Return
            return Leaf;
        }
        public static TreeViewItem AddSelector(MainWindow WindowHandle, TreeViewItem Item)
        {
            // Create a leaf
            TreeViewItem Leaf = DataHandler.AddLeaf(WindowHandle, Item, typeof(LocationSelector));
            Leaf.Header = "Новое разветвление";
            Leaf.Selected += WindowHandle.Selector_Selected;
            Leaf.KeyDown += WindowHandle.Location_KeyDown;

            // Create a context menu
            ContextMenu LocationMenu = new ContextMenu();
            MenuItem CutLocation = new MenuItem() { Header = "Вырезать" };
            MenuItem CopyLocation = new MenuItem() { Header = "Копировать" };
            MenuItem PasteToLocation = new MenuItem() { Header = "Вставить" };
            MenuItem LocationRemove = new MenuItem() { Header = "Удалить" };
            CutLocation.Click += WindowHandle.LocationCut_Click;
            CopyLocation.Click += WindowHandle.LocationCopy_Click;
            PasteToLocation.Click += WindowHandle.LocationPasteTo_Click;
            LocationRemove.Click += WindowHandle.LocationRemove_Click;
            LocationMenu.Items.Add(CutLocation);
            LocationMenu.Items.Add(CopyLocation);
            LocationMenu.Items.Add(PasteToLocation);
            LocationMenu.Items.Add(new Separator());
            LocationMenu.Items.Add(LocationRemove);
            LocationMenu.Name = Leaf.Name;
            Leaf.ContextMenu = LocationMenu;
            LocationMenu.Opened += WindowHandle.TreeViewItem_ContextMenuOpened;
            // Return
            return Leaf;
        }
        public static TreeViewItem AddFolder(MainWindow WindowHandle, TreeViewItem Parent)
        {
            // Creating new object
            TreeViewItem NewFolder = new TreeViewItem();
            NewFolder.Header = "Новая папка";
            // Generating the name
            String NameGen = Guid.NewGuid().ToString().Replace('-', '_');
            NameGen = "Folder_" + NameGen;
            NewFolder.Name = NameGen;

            // Pushing to TreeView
            if (Parent == null)
                Tree_Hierarchy.Items.Add(NewFolder);
            else
            {
                Parent.Items.Add(NewFolder);
                Parent.IsExpanded = true;
            }
            return NewFolder;
        }
        public static TreeViewItem AddFolderFull(MainWindow WindowHandle, TreeViewItem Item)
        {
            // Create a tree item
            TreeViewItem NewFolder = DataHandler.AddFolder(WindowHandle, Item);
            NewFolder.Selected += WindowHandle.Folder_Selected;
            NewFolder.KeyDown += WindowHandle.Folder_KeyDown;

            // Context menu
            ContextMenu LocationMenu = new ContextMenu();
            MenuItem LocationAddEntry = new MenuItem() { Header = "Создать локацию" };
            MenuItem LocationAddSelector = new MenuItem() { Header = "Создать разветвление" };
            MenuItem LocationAddFolder = new MenuItem() { Header = "Создать папку" };
            MenuItem CutFolder = new MenuItem() { Header = "Вырезать" };
            MenuItem CopyFolder = new MenuItem() { Header = "Копировать" };
            MenuItem PasteToFolder = new MenuItem() { Header = "Вставить" };
            MenuItem FolderRemove = new MenuItem() { Header = "Удалить" };
            CutFolder.Click += WindowHandle.FolderCut_Click;
            CopyFolder.Click += WindowHandle.FolderCopy_Click;
            PasteToFolder.Click += WindowHandle.FolderPasteTo_Click;
            LocationAddEntry.Click += WindowHandle.LocationAddEntry_Click;
            LocationAddSelector.Click += WindowHandle.LocationAddSelector_Click;
            LocationAddFolder.Click += WindowHandle.LocationAddFolder_Click;
            FolderRemove.Click += WindowHandle.FolderRemove_Click;
            LocationMenu.Items.Add(LocationAddEntry);
            LocationMenu.Items.Add(LocationAddSelector);
            LocationMenu.Items.Add(LocationAddFolder);
            LocationMenu.Items.Add(new Separator());
            //LocationMenu.Items.Add(CutFolder);
            //LocationMenu.Items.Add(CopyFolder);
            LocationMenu.Items.Add(PasteToFolder);
            LocationMenu.Items.Add(new Separator());
            LocationMenu.Items.Add(FolderRemove);
            LocationMenu.Name = NewFolder.Name;
            NewFolder.ContextMenu = LocationMenu;
            LocationMenu.Opened += WindowHandle.TreeViewItem_ContextMenuOpened;
            return NewFolder;
        }
        public static void RemoveLeaf(String Name, Type LeafType = null)
        {
            TreeViewItem ToRemove = Utility.FindNodeByName(Name, Tree_Hierarchy.Items);
            if (LeafType == null)
                LeafType = GetLeafType(Name);

            if (LeafType == typeof(LocationData))
            {
                // Remove from array
                for (int i = 0; i < LocationList.Count; i++)
                    if (LocationList[i].TreeViewId == Name)
                        LocationList.RemoveAt(i);
            }
            else if (LeafType == typeof(LocationSelector))
            {
                // Remove from array
                for (int i = 0; i < SelectorList.Count; i++)
                    if (SelectorList[i].TreeViewId == Name)
                        SelectorList.RemoveAt(i);
            }
            // Remove from tree view
            if (ToRemove.Parent == Tree_Hierarchy)
                Tree_Hierarchy.Items.Remove(ToRemove);
            else
            {
                TreeViewItem Parent = ToRemove.Parent as TreeViewItem;
                Parent.Items.Remove(ToRemove);
            }
        }
        public static void RemoveFolder(String Name)
        {
            TreeViewItem Folder = Utility.FindNodeByName(Name, Tree_Hierarchy.Items);
            if (Folder != null)
            {
                for (int i = 0; i < Folder.Items.Count; i++)
                {
                    TreeViewItem Child = Folder.Items[0] as TreeViewItem;
                    Type LeafType = GetLeafType(Child.Name);
                    if (LeafType == null)
                        RemoveFolder(Child.Name);
                    else
                        RemoveLeaf(Child.Name, LeafType);
                }
                if (Folder.Parent == Tree_Hierarchy)
                    Tree_Hierarchy.Items.Remove(Folder);
                else
                {
                    TreeViewItem Parent = Folder.Parent as TreeViewItem;
                    Parent.Items.Remove(Folder);
                }
            }
        }
        public static void RemoveNode(String Name)
        {
            if (IsFolder(Name))
                RemoveFolder(Name);
            else
                RemoveLeaf(Name);
        }
        public static Type GetLeafType(String Name)
        {
            // Location
            for (int i = 0; i < LocationList.Count; i++)
                if (LocationList[i].TreeViewId == Name)
                    return typeof(LocationData);
            // Selector
            for (int i = 0; i < SelectorList.Count; i++)
                if (SelectorList[i].TreeViewId == Name)
                    return typeof(LocationSelector);
                // Nothing found - it's a folder
                return null;
        }
        public static LocationData GetLocationByTreeViewId(String Name)
        {
            foreach (LocationData Data in LocationList)
            {
                if (Data.TreeViewId == Name)
                    return Data;
            }
            return null;
        }
        public static LocationSelector GetSelectorByTreeViewId(String Name)
        {
            foreach (LocationSelector Data in SelectorList)
            {
                if (Data.TreeViewId == Name)
                    return Data;
            }
            return null;
        }
        public static bool IsFolder(String Name)
        {
            return (GetLeafType(Name) == null);
        }
    }
}
