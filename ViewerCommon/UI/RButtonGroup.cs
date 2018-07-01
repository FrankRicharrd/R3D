using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Viewer.Interfaces;
using Fluent;
using Viewer.Interfaces.UI;

namespace ViewerCommon.UI
{
    public class RButtonGroup : RibbonGroupBox, IButtonGroup, IEqualityComparer
    {
        List<IRibbonButton> _buttons = new List<IRibbonButton>();

        public List<IRibbonButton> Buttons
        {
            get
            {
                return _buttons;
            }

            set
            {
                _buttons = value;
            }
        }

        public string GroupBoxName
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

        

        public RButtonGroup(string name)
        {
            Header = name;
            GroupBoxName = name;
        }

        public IRibbonButton CreateButton(string name, string iconpath, ICommand executionCommand)
        {
            var button = new RRibbonButton(name, iconpath, executionCommand);

            Buttons.Add(button);

            Items.Add(button);


            return button;
        }

        public new bool Equals(object x, object y)
        {
            var a = x as RButtonGroup;
            var b = y as RButtonGroup;
            if (a == null || b == null)
                return false;

            if (a.GroupBoxName == a.GroupBoxName)
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

        public IRibbonToggleButton CreateToggleButton(string name, string iconPath, ICommand executionCommand)
        {
            var button = new RRibbonToggleButton(name, iconPath, executionCommand);

            Buttons.Add(button);

            Items.Add(button);

            return button;
        }
    }
}
