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
    public static class CopyPaster
    {
        public static bool IsCut { get; set; }
        public static String CopyObjectName { get; set; }
        public static void PasteTo(MainWindow WindowHandle, TreeView Tree, String ParentName)
        {
            TreeViewItem Parent = Utility.FindNodeByName(ParentName, Tree.Items);
            TreeViewItem CopiedObject = Utility.FindNodeByName(CopyObjectName, Tree.Items);

            if (DataHandler.IsFolder(CopyObjectName) == false)
            {
                // Creating a copy
                TreeViewItem NewNode;
                if (DataHandler.GetLeafType(CopyObjectName) == typeof(LocationData))
                {
                    NewNode = DataHandler.AddLocation(WindowHandle, null);
                    NewNode.Header = CopiedObject.Header;
                    DataHandler.GetLocationByTreeViewId(NewNode.Name).CloneFrom(DataHandler.GetLocationByTreeViewId(CopyObjectName));
                }
                else if (DataHandler.GetLeafType(CopyObjectName) == typeof(LocationSelector))
                {
                    NewNode = DataHandler.AddSelector(WindowHandle, null);
                    NewNode.Header = CopiedObject.Header;
                    DataHandler.GetSelectorByTreeViewId(NewNode.Name).CloneFrom(DataHandler.GetSelectorByTreeViewId(CopyObjectName));
                }
                else
                {
                    // TEMP!!!!
                    NewNode = DataHandler.AddLocation(WindowHandle, null);
                }
                // Pasting to folder
                if (DataHandler.IsFolder(ParentName) == true)
                {
                    ItemsControl NewNodeParent = NewNode.Parent as ItemsControl;
                    NewNodeParent.Items.Remove(NewNode);
                    Parent.Items.Add(NewNode);
                    Parent.IsExpanded = true;
                }
                // Pasting to other object
                else if (ParentName != null)
                {
                    ItemsControl NewNodeParent = NewNode.Parent as ItemsControl;
                    NewNodeParent.Items.Remove(NewNode);
                    ItemsControl ParentsParent = Parent.Parent as ItemsControl;
                    ParentsParent.Items.Insert(ParentsParent.Items.IndexOf(Parent) + 1, NewNode);
                }
            }
            if (IsCut && DataHandler.IsFolder(CopiedObject.Name) == false)
            {
                DataHandler.RemoveNode(CopiedObject.Name);
            }
        }
    }
}
