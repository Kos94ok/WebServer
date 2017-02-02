using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RPGEditor
{
    public static class WindowTitle
    {
        static bool Locked = false;
        static bool FileChanged = false;
        static string OpenFile = null;
        const string Default = "RPG Editor";

        public static void AddStar(MainWindow Handle)
        {
            if (Locked == true)
                return;
            if (FileChanged == false)
            {
                FileChanged = true;
                Handle.Title = Default + " [" + OpenFile + "]*";
            }
        }
        public static void RemoveStar(MainWindow Handle)
        {
            if (Locked == true)
                return;
            if (FileChanged == true)
            {
                FileChanged = false;
                Handle.Title = Default + " [" + OpenFile + "]";
            }
        }
        public static void SetOpenFileName(MainWindow Handle, String FileName)
        {
            if (FileName == null)
            {
                MakeDefault(Handle);
                return;
            }
            if (Locked == true)
                return;
            OpenFile = FileName;
            Handle.Title = Default + " [" + OpenFile + "]";
        }
        public static String GetOpenFileName()
        {
            if (OpenFile == null || OpenFile.Length == 0)
                return "Безымянный";
            else
                return OpenFile;
        }
        public static void Lock()
        {
            Locked = true;
        }
        public static void Unlock()
        {
            Locked = false;
        }
        public static void MakeDefault(MainWindow Handle)
        {
            Handle.Title = Default;
            OpenFile = null;
            FileChanged = false;
        }
        public static bool HasUnsavedChanges()
        {
            if (FileChanged)
                return true;
            else
                return false;
        }
    }
}
