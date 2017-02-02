using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RPGEditor
{
    public class LocationSelectorEntry
    {
        public String Link { get; set; }
        public Int32 Chance { get; set; }
        public String Condition { get; set; }
        public String Action { get; set; }
        public LocationSelectorEntry()
        {
            Chance = 1;
        }
        public LocationSelectorEntry(LocationSelectorEntry Original)
        {
            Link = Original.Link;
            Chance = Original.Chance;
            Condition = Original.Condition;
            Action = Original.Action;
        }
    }
    public class LocationSelector : GenericRPGObject
    {
        public String Guid { get; set; }
        public List<LocationSelectorEntry> Options { get; set; }

        public LocationSelector()
        {
            Options = new List<LocationSelectorEntry>();
        }
        public void CloneFrom(LocationSelector Original)
        {
            Guid = Original.Guid;
            foreach (LocationSelectorEntry D in Original.Options)
                Options.Add(new LocationSelectorEntry(D));
        }
    }
}
