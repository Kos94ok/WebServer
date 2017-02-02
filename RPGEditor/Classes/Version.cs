using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.IO;

[assembly: AssemblyVersion("2.0.*")]

namespace RPGEditor
{
    public static class Version
    {
        public static String Current
        {
            get
            {
                String ReturnValue = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString();
                ReturnValue = ReturnValue.Substring(0, ReturnValue.LastIndexOf("."));
                return ReturnValue;
            }
        }
    }
}
