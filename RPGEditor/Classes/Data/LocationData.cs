using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RPGEditor
{
    public class LocationDecision
    {
        public String Text { get; set; }
        public String LocationLink { get; set; }
        public String Condition { get; set; }
        public String Action { get; set; }
        public LocationDecision() {}
        public LocationDecision(LocationDecision Original)
        {
            Text = Original.Text;
            LocationLink = Original.LocationLink;
            Condition = Original.Condition;
            Action = Original.Action;
        }
    }
    public class LocationData : GenericRPGObject
    {
        public String Guid { get; set; }
        public String Description { get; set; }
        public List<LocationDecision> Decisions { get; set; }

        public LocationData()
        {
            Decisions = new List<LocationDecision>();
        }
        public void CloneFrom(LocationData Original)
        {
            Guid = Original.Guid;
            Description = Original.Description;
            foreach (LocationDecision D in Original.Decisions)
                Decisions.Add(new LocationDecision(D));
        }
    }
    
}
