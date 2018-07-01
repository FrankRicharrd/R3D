using Fluent;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace ViewerCommon.UI
{
    public class RTab : RibbonTabItem ,ITab, IEqualityComparer
    {
        List<IButtonGroup> _buttonGroups = new List<IButtonGroup>();
        

        public string TabName
        {
            get
            {
                return Name;
            }

            set
            {
                Name = value;
            }
        }

        public List<IButtonGroup> ButtonGroups
        {
            get
            {
                return _buttonGroups;
            }

            set
            {
                _buttonGroups = value;
            }
        }

       

        public RTab(string name)
        {
            Header = name;

            TabName = name;
        }

        public IButtonGroup CreateButtonGroup(string name)
        {
            var buttongroup = new RButtonGroup(name);

            ButtonGroups.Add(buttongroup);

            Groups.Add(buttongroup);

            return buttongroup;
        }

        public IButtonGroup GetButtonGroup(string name)
        {
            return ButtonGroups.Where(a => a.GroupBoxName == name).Select(a => a).FirstOrDefault();
        }

        public new bool Equals(object x, object y)
        {
            var a = x as RTab;
            var b = y as RTab;
            if (a == null || b == null)
                return false;

            if (a.TabName == a.TabName)
                return true;
            else
                return false;
        }

        public int GetHashCode(object obj)
        {
            if (obj == null)
                return 0;

            return obj.GetHashCode();
        }

       
    }
}
