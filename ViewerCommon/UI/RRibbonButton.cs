using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace ViewerCommon.UI
{
    public class RRibbonButton : Fluent.Button,  IRibbonButton
    {
      
        string _description;


        string _iconPath;

        string _name;


        string _toolTip;
        

        public string Description
        {
            get
            {
                return _description;
            }

            set
            {
                _description = value;
            }
        }

        public string IconPath
        {
            get
            {
                return Icon as string;
            }

            set
            {
                Icon = value;
                LargeIcon = value;
            }
        }


        string IRibbonButton.ToolTip
        {
            get
            {
                return base.ToolTip as string;
            }

            set
            {
                ToolTip = value;
            }
        }

        public RRibbonButton(string name, string iconpath, ICommand executionCommand)
        {
            Header = name;
            Name = name;
            IconPath = iconpath;
            
            Command = executionCommand;
        }

    }
}
