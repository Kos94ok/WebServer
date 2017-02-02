using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace RPGEditor
{
    public static class Utility
    {
        public static TreeViewItem FindNodeByName(String Name, ItemCollection Items)
        {
            foreach (TreeViewItem Item in Items)
            {
                // Check depth
                if (Item.Items.Count > 0)
                {
                    TreeViewItem Return = FindNodeByName(Name, Item.Items);
                    if (Return != null)
                        return Return;
                }
                // Check name
                if (Item.Name == Name)
                    return Item;
            }
            return null;
        }
        public static TreeViewItem GetClickedTreeViewItem(object sender, TreeView Tree_Hierarchy)
        {
            MenuItem ClickedButton = sender as MenuItem;
            ContextMenu ClickedMenu = ClickedButton.Parent as ContextMenu;
            return FindNodeByName(ClickedMenu.Name, Tree_Hierarchy.Items);
        }
    }
}
